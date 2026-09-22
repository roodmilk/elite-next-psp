/* PSP frontend for the early Elite-A native adaptation. */
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <psppower.h>
#include <psprtc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "game.h"
#include "story.h"
#include "guild.h"
#include "campaign.h"
#include "saga.h"
#include "audio.h"
#include "radio-tests.h"
#include "steering.h"
#include "steering-test.h"

PSP_MODULE_INFO("ELITE NEXT",0,1,7);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(8192);
#define W 480
#define H 272
#define STRIDE 512
#define RGB(r,g,b) ((unsigned)(r)|((unsigned)(g)<<8)|((unsigned)(b)<<16)|0xff000000u)
#define WHITE RGB(222,236,239)
#define CYAN RGB(63,209,216)
#define GOLD RGB(248,192,93)
#define AMBER RGB(236,148,36)
#define AMBDIM RGB(168,88,22)
#define DASH RGB(16,8,5)
#define DIM RGB(114,142,154)
#define BG RGB(6,12,22)
#define RED RGB(244,85,88)
static volatile int running=1;
static volatile int resume_requested=0,suspend_requested=0;
static unsigned *fb;
static Game game;
static int page=0,row=0,paused=0,smoke=0,visual_hold=0,hud_hidden=0,hud_mode=0,high_contrast=0;
/* 0 = Kei/Ryn campaign, 1 = Guild assignments, 2+ = accepted job slot. */
static int tracked_mission=0;
static int analog_center_x=128,analog_center_y=128;
static int walk_kind=0;
static float walk_x=0,walk_z=0,walk_yaw=0;
static float walk_oxygen=100,walk_integrity=100;
static int walk_salvaged=0;
static int analog_enabled=1,analog_ready=0,fire_blocked=0;
static float preview_time=0;
static int nearby[256],near_count,chart_mode=0,chart_cursor=7,chart_zoom=1;
static int view_top(void){return hud_hidden?0:(hud_mode==0?0:23);}
static int buffer=0;
static void display_recover(void){
 /* After sleep the LCD/framebuffer pairing can be invalid; rebuild both planes. */
 sceDisplaySetMode(0,W,H);
 for(int b=0;b<2;b++){unsigned *plane=(unsigned*)(0x44000000u+(unsigned)b*STRIDE*H*4);for(int i=0;i<STRIDE*H;i++)plane[i]=BG;}
 fb=(unsigned*)(0x44000000u+(unsigned)buffer*STRIDE*H*4);
 pspDebugScreenInit();pspDebugScreenEnableBackColor(0);pspDebugScreenSetOffset(buffer*STRIDE*H*4);
 sceDisplayWaitVblankStart();
 sceDisplaySetFrameBuf((void*)fb,STRIDE,PSP_DISPLAY_PIXEL_FORMAT_8888,PSP_DISPLAY_SETBUF_NEXTFRAME);
}
static void runtime_recover_from_sleep(void){
 /* Tear down audio left over from a long suspend, then restore display/input/radio. */
 audio_stop();
 scePowerSetClockFrequency(333,333,166);
 display_recover();
 sceCtrlSetSamplingCycle(0);sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
 analog_ready=0;
 audio_init();
 message(&game,"PSP resumed. Display, controls and radio restored.");
}
static int view_bot(void){return hud_hidden?H-1:(hud_mode==0?191:247);}
enum { HOME,FLIGHT,MARKET,CHART,YARD,EQUIP,STATUS,HELP,FACTIONS,LOCAL,DEBUG,COMMS,DETAILS,MISSIONS,MISSIONLOG,TARGETING,GALNET,CODEX,STORY,GUILD,RADIO,COMMS_PANEL,INTRO,CAMPAIGN,COMFORT,WALK };
#include "deck-nav.h"
static int pip_sel=1,comms_rescue_confirm=0,abandon_confirm=0;
static const char *faction_names[]={"TRADERS","LAW","PIRATES","EXPLORERS GUILD"};
static const unsigned faction_colors[]={GOLD,RGB(90,165,255),RED,RGB(100,235,150)};
static int nav_body=-1;
static int proj_ox=240,proj_oy=110,clipx0=0,clipx1=W,clipy0=-1,clipy1=-1;
static int exit_callback(int a,int b,void *c){(void)a;(void)b;(void)c;running=0;return 0;}
static int power_callback(int a,int flags,void *c){
 (void)a;(void)c;
 /* Freeze MP3 I/O immediately so a long Memory Stick wake cannot hang the audio worker. */
 if(flags&(PSP_POWER_CB_SUSPENDING|PSP_POWER_CB_STANDBY)){audio_prepare_suspend();suspend_requested=1;}
 if(flags&(PSP_POWER_CB_RESUME_COMPLETE|PSP_POWER_CB_RESUMING))resume_requested=1;
 return 0;
}
static int callback_thread(SceSize a,void *b){(void)a;(void)b;int id=sceKernelCreateCallback("Exit",exit_callback,0);sceKernelRegisterExitCallback(id);id=sceKernelCreateCallback("Power",power_callback,0);scePowerRegisterCallback(0,id);sceKernelSleepThreadCB();return 0;}
static void rect(int x,int y,int w,int h,unsigned color){
 int x0=x<0?0:x,y0=y<0?0:y,x1=x+w>W?W:x+w,y1=y+h>H?H:y+h;
 for(int j=y0;j<y1;j++)for(int i=x0;i<x1;i++)fb[j*STRIDE+i]=color;
}
static void pixel(int x,int y,unsigned c){if(clipy0>=0&&(x<clipx0||x>=clipx1||y<clipy0||y>=clipy1))return;if(x>=0&&x<W&&y>=0&&y<H)fb[y*STRIDE+x]=c;}
static void line(int x,int y,int xx,int yy,unsigned c){
 // Reject pathological off-screen projections before stepping.
 if(abs(x)>3000||abs(y)>3000||abs(xx)>3000||abs(yy)>3000)return;
 int dx=abs(xx-x),sx=x<xx?1:-1,dy=-abs(yy-y),sy=y<yy?1:-1,e=dx+dy;
 for(;;){pixel(x,y,c);if(x==xx&&y==yy)break;int e2=e*2;if(e2>=dy){e+=dy;x+=sx;}if(e2<=dx){e+=dx;y+=sy;}}
}
#include "font8.h"
#include "art-runtime.h"
static void text(int x,int y,unsigned c,const char *fmt,...){
 char buf[128];va_list args;va_start(args,fmt);vsnprintf(buf,sizeof(buf),fmt,args);va_end(args);
 int px=x*8,py=y*8;
 for(int i=0;buf[i]&&px<W;i++,px+=8){
  unsigned char ch=(unsigned char)buf[i];if(ch<32||ch>126)ch='?';
  const unsigned char *g=font8[ch-32];
  for(int row=0;row<8;row++){
   int yy=py+row;if(yy<0||yy>=H)continue;
   unsigned bits=g[row];
   for(int col=0;col<8;col++)if((bits>>(7-col))&1){int xx=px+col;if(xx>=0&&xx<W)fb[yy*STRIDE+xx]=c;}
  }
 }
}
/* Coloured name plate so "KEI SAYS" / "REI SAYS" read at a glance on 480x272. */
static void speaker_name_tag(int col,int rowy,const char *name,unsigned color){
 char tag[28];snprintf(tag,sizeof(tag),"%.12s SAYS",name&&name[0]?name:"CONTACT");
 int n=(int)strlen(tag);if(n<1)n=1;if(n>18)n=18;
 int x=col*8,y=rowy*8;
 rect(x-2,y-1,n*8+6,10,color);
 rect(x-1,y,n*8+4,8,RGB(((color&255)*28)/100,(((color>>8)&255)*28)/100,(((color>>16)&255)*28)/100));
 text(col,rowy,WHITE,"%.*s",n,tag);
}
static void dump_native_bmp(const char *path){
 FILE *f=fopen(path,"wb");if(!f||!fb)return;
 int w=W,h=H;unsigned size=54+(unsigned)w*h*3;unsigned char hdr[54]={0};
 hdr[0]='B';hdr[1]='M';hdr[2]=(unsigned char)size;hdr[3]=(unsigned char)(size>>8);hdr[4]=(unsigned char)(size>>16);hdr[5]=(unsigned char)(size>>24);
 hdr[10]=54;hdr[14]=40;hdr[18]=(unsigned char)w;hdr[19]=(unsigned char)(w>>8);hdr[22]=(unsigned char)h;hdr[23]=(unsigned char)(h>>8);hdr[26]=1;hdr[28]=24;
 fwrite(hdr,1,54,f);
 for(int y=h-1;y>=0;y--)for(int x=0;x<w;x++){unsigned c=fb[y*STRIDE+x];unsigned char p[3]={(unsigned char)((c>>16)&255),(unsigned char)((c>>8)&255),(unsigned char)(c&255)};fwrite(p,1,3,f);}
 fclose(f);
}
static void circle(int x,int y,int r,unsigned c){for(int i=0;i<64;i++){float a=i*6.2831853f/64,b=(i+1)*6.2831853f/64;line(x+(int)(cosf(a)*r),y+(int)(sinf(a)*r),x+(int)(cosf(b)*r),y+(int)(sinf(b)*r),c);}}
typedef struct {float x,y,z;} Point;
typedef struct {Point p[3];unsigned color;float depth;} DrawTri;
static DrawTri drawlist[2048];static int drawcount;
static Point project(Vec3 v){return (Point){proj_ox+v.x*240/v.z,proj_oy-v.y*240/v.z,v.z};}
static void preview_clip(int ox,int oy,int x0,int y0,int x1,int y1){proj_ox=ox;proj_oy=oy;clipx0=x0;clipy0=y0;clipx1=x1;clipy1=y1;}
static void preview_reset(void){proj_ox=240;proj_oy=110;clipx0=0;clipx1=W;clipy0=-1;clipy1=-1;}
static float edge(Point a,Point b,float x,float y){return (x-a.x)*(b.y-a.y)-(y-a.y)*(b.x-a.x);}
static void triangle(DrawTri *t){
 Point a=t->p[0],b=t->p[1],c=t->p[2];float area=edge(a,b,c.x,c.y);if(fabsf(area)<.05f)return;
 int x0=(int)fmaxf(clipx0,floorf(fminf(a.x,fminf(b.x,c.x)))),x1=(int)fminf(clipx1-1,ceilf(fmaxf(a.x,fmaxf(b.x,c.x))));
 int yt=clipy0>=0?clipy0:view_top(),yb=clipy1>=0?clipy1-1:view_bot();
 int y0=(int)fmaxf(yt,floorf(fminf(a.y,fminf(b.y,c.y)))),y1=(int)fminf(yb,ceilf(fmaxf(a.y,fmaxf(b.y,c.y))));
 /* All faces sample one shared 2px grid; per-face grids left dotted seams. */
 x0=(int)fmaxf(clipx0,x0&~1);y0=(int)fmaxf(yt,y0&~1);
 for(int y=y0;y<=y1;y+=2)for(int x=x0;x<=x1;x+=2){float e0=edge(a,b,x+1.f,y+1.f),e1=edge(b,c,x+1.f,y+1.f),e2=edge(c,a,x+1.f,y+1.f);
 if((e0>=-.05f&&e1>=-.05f&&e2>=-.05f)||(e0<=.05f&&e1<=.05f&&e2<=.05f))rect(x,y,x<x1?2:1,y<y1?2:1,t->color);}
}
static void queue_triangle(Vec3 a,Vec3 b,Vec3 c,unsigned color){
 // Clip the near plane, preserving triangles which cross the camera.
 Vec3 in[4]={a,b,c},out[4];int count=0;
 for(int i=0;i<3;i++){Vec3 u=in[i],v=in[(i+1)%3];int ui=u.z>=15,vi=v.z>=15;if(ui)out[count++]=u;if(ui!=vi){float t=(15-u.z)/(v.z-u.z);out[count++]=add(u,mul(sub(v,u),t));}}
 for(int i=1;i<count-1&&drawcount<2048;i++){DrawTri *t=&drawlist[drawcount++];t->p[0]=project(out[0]);t->p[1]=project(out[i]);t->p[2]=project(out[i+1]);t->color=color;t->depth=(out[0].z+out[i].z+out[i+1].z)/3;}
}
static Vec3 rotate(Vec3 p,float yaw,float roll){float c=cosf(yaw),s=sinf(yaw);Vec3 v={p.x*c+p.z*s,p.y,-p.x*s+p.z*c};c=cosf(roll);s=sinf(roll);return (Vec3){v.x*c-v.y*s,v.x*s+v.y*c,v.z};}
static unsigned livery_tint(unsigned base,int delta){int r=(int)(base&255)+delta,g=(int)((base>>8)&255)+delta,b=(int)((base>>16)&255)+delta;if(r<0)r=0;if(g<0)g=0;if(b<0)b=0;if(r>255)r=255;if(g>255)g=255;if(b>255)b=255;return RGB(r,g,b);}
static void shipmesh_stretched(int id,Vec3 pos,float yaw,float roll,float scale,float stretch,unsigned base,int preview){
 const Mesh *m=&meshes[id];Vec3 vertices[128],world[128];if(m->vertices>128)return;
 for(int i=0;i<m->vertices;i++){Vec3 v=m->v[i];v.z*=stretch;Vec3 p=add(pos,mul(rotate(v,yaw,roll),scale));world[i]=p;vertices[i]=preview?p:camera(&game,p);}
 for(int i=0;i<m->triangles;i++){const MeshTri *t=&m->t[i];if(!preview&&id==mesh_id("CORIOLIS")&&t->normal.z<-.99f)continue;Vec3 n=rotate(t->normal,yaw,roll);if(!preview){Vec3 center=mul(add(add(world[t->a],world[t->b]),world[t->c]),1.f/3);if(dot(n,sub(game.pos,center))<=0)continue;}float light=.3f+.7f*fmaxf(0,dot(n,norm((Vec3){-.3f,.7f,-.6f})));
 unsigned color=RGB((int)((base&255)*light),(int)(((base>>8)&255)*light),(int)(((base>>16)&255)*light));
 /* Two-tone livery makes the old Elite silhouettes read as actual painted
  * ships: upper facets catch a cool highlight, undersides go deep, and a
  * sparse accent facet supplies a cockpit/panel colour without textures. */
 if(n.y>.42f)color=livery_tint(color,22);else if(n.y<-.35f)color=livery_tint(color,-20);else if((i&7)==0)color=livery_tint(color,12);
 queue_triangle(vertices[t->a],vertices[t->b],vertices[t->c],color);}
}
static void shipmesh(int id,Vec3 pos,float yaw,float roll,float scale,unsigned base,int preview){shipmesh_stretched(id,pos,yaw,roll,scale,1,base,preview);}
static void shipwire_stretched(int id,Vec3 pos,float yaw,float scale,float stretch,unsigned color){
 const Mesh *m=&meshes[id];Vec3 vertices[128];if(m->vertices>128)return;for(int i=0;i<m->vertices;i++){Vec3 v=m->v[i];v.z*=stretch;vertices[i]=camera(&game,add(pos,mul(rotate(v,yaw,0),scale)));}
 for(int i=0;i<m->triangles;i++){const MeshTri *t=&m->t[i];int ids[3]={t->a,t->b,t->c};for(int e=0;e<3;e++){Vec3 a=vertices[ids[e]],b=vertices[ids[(e+1)%3]];if(a.z<20||b.z<20)continue;Point p=project(a),q=project(b);line((int)p.x,(int)p.y,(int)q.x,(int)q.y,color);}}
}
static void capital_box(const NPC *n,Vec3 center,Vec3 half,unsigned color){
 Vec3 world[8],side=norm((Vec3){n->dir.z,0,-n->dir.x}),up={0,1,0};
 for(int i=0;i<8;i++)world[i]=freight_world(n,add(center,(Vec3){i&2?half.x:-half.x,i&4?half.y:-half.y,i&1?half.z:-half.z}));
 const int faces[6][4]={{0,2,6,4},{1,5,7,3},{0,4,5,1},{2,3,7,6},{0,1,3,2},{4,6,7,5}};
 Vec3 normal[6]={mul(n->dir,-1),n->dir,mul(side,-1),side,mul(up,-1),up};
 for(int f=0;f<6;f++){
  Vec3 a=world[faces[f][0]],b=world[faces[f][1]],c=world[faces[f][2]],d=world[faces[f][3]];
  if(dot(normal[f],sub(game.pos,a))<=0)continue;
  unsigned ink=livery_tint(color,f==5?24:f==4?-32:f<2?-14:0);
  queue_triangle(camera(&game,a),camera(&game,b),camera(&game,c),ink);
  queue_triangle(camera(&game,a),camera(&game,c),camera(&game,d),ink);
 }
}
static void capital_model(const NPC *n,unsigned color){
 Vec3 e=freight_extent(n);int detailed=length(sub(n->pos,game.pos))<5000,style=n->freight_style;
 unsigned steel=n->flash>0?WHITE:RGB(65,76,84);
 unsigned paint=RGB(30+(color&255)/3,36+((color>>8)&255)/3,42+((color>>16)&255)/3);
 /* Shared silhouettes at every distance: spine, engine block, cargo and bridge. */
 capital_box(n,(Vec3){0,-e.y*.38f,0},(Vec3){e.x*.36f,e.y*.34f,e.z*.96f},steel);
 capital_box(n,(Vec3){0,0,-e.z*.83f},(Vec3){e.x*.65f,e.y*.62f,e.z*.17f},steel);
 capital_box(n,(Vec3){0,e.y*.48f,-e.z*.55f},(Vec3){e.x*.56f,e.y*.52f,e.z*.13f},paint);
 if(style==0){ /* Container carrier: paired stacks and visible gaps. */
  for(int row=0;row<3;row++)for(int side=-1;side<=1;side+=2){
   unsigned cargo= row==0?RGB(120,90,61):row==1?RGB(68,97,109):RGB(98,103,76);
   capital_box(n,(Vec3){side*e.x*.58f,0,-e.z*.23f+row*e.z*.43f},(Vec3){e.x*.42f,e.y*.63f,e.z*.18f},cargo);
   if(detailed)capital_box(n,(Vec3){side*e.x*.58f,e.y*.66f,-e.z*.23f+row*e.z*.43f},(Vec3){e.x*.4f,3,e.z*.035f},steel);
  }
 }else if(style==1){ /* Fuel tanker: four stepped tank shoulders. */
  for(int row=0;row<2;row++)for(int side=-1;side<=1;side+=2){
   Vec3 c={side*e.x*.55f,0,-e.z*.12f+row*e.z*.65f};
   capital_box(n,c,(Vec3){e.x*.45f,e.y*.42f,e.z*.29f},RGB(83,105,112));
   c.y=e.y*.54f;capital_box(n,c,(Vec3){e.x*.30f,e.y*.18f,e.z*.25f},RGB(96,118,125));
  }
 }else { /* Bulk ore barge: wide, low armored bins around the spine. */
  for(int row=0;row<4;row++){
   capital_box(n,(Vec3){0,-e.y*.05f,-e.z*.35f+row*e.z*.34f},(Vec3){e.x,e.y*.55f,e.z*.145f},row&1?RGB(107,92,72):RGB(82,83,78));
   if(detailed)capital_box(n,(Vec3){0,e.y*.54f,-e.z*.35f+row*e.z*.34f},(Vec3){e.x*.84f,3,e.z*.11f},RGB(44,51,54));
  }
 }
 if(detailed){
  /* Narrow illuminated bridge glazing, recessed stern engines, navigation rails. */
  capital_box(n,(Vec3){0,e.y*.70f,-e.z*.412f},(Vec3){e.x*.45f,e.y*.075f,2},RGB(89,166,172));
  for(int side=-1;side<=1;side+=2)capital_box(n,(Vec3){side*e.x*.38f,-e.y*.05f,-e.z*.998f},(Vec3){e.x*.14f,e.y*.24f,2},RGB(104,149,180));
 }
}
static int depth_sort(const void *a,const void *b){float d=((const DrawTri*)b)->depth-((const DrawTri*)a)->depth;return d>0?1:d<0?-1:0;}
static void flush_meshes(void){qsort(drawlist,drawcount,sizeof(*drawlist),depth_sort);for(int i=0;i<drawcount;i++)triangle(&drawlist[i]);drawcount=0;}
static void header(const char *title){rect(0,0,W,22,RGB(5,12,20));rect(0,0,W,1,GOLD);rect(0,21,W,1,CYAN);rect(0,0,4,22,GOLD);draw_next_art(next_logo_small,100,18,5,2,100,18);text(14,1,DIM,"/");text(16,1,WHITE,"%.42s",title);}
static void button_icon(int x,int y,char b,unsigned c){
 if(b=='O'){circle(x+4,y+4,3,c);}
 else if(b=='X'){line(x+1,y+1,x+7,y+7,c);line(x+7,y+1,x+1,y+7,c);}
 else if(b=='T'){line(x+1,y+7,x+7,y+7,c);line(x+1,y+7,x+4,y+1,c);line(x+7,y+7,x+4,y+1,c);}
 else {rect(x+1,y+1,7,7,c);rect(x+3,y+3,3,3,DASH);}
}
#include "hud-pixels.h"
static int footer_token(const char *s,int n,char *icon){
 struct {const char *word;char icon;} keys[]={{"X",'X'},{"O",'O'},{"TRI",'T'},{"TRIANGLE",'T'},{"SQ",'S'},{"SQUARE",'S'}};
 for(unsigned i=0;i<sizeof(keys)/sizeof(keys[0]);i++)if((int)strlen(keys[i].word)==n&&!strncmp(s,keys[i].word,n)){*icon=keys[i].icon;return 1;}
 return 0;
}
static void footer(const char *s){
 char label[59];snprintf(label,sizeof(label),"%.58s",s);
 rect(0,248,W,24,RGB(5,12,20));rect(0,248,W,1,CYAN);
 /* Parse complete button tokens, never letters inside NEXT, STORY or WORK. */
 for(int i=0;label[i];){
  if(label[i]==' '||label[i]=='/'||label[i]=='+'||label[i]=='|'){i++;continue;}
  int start=i;while(label[i]&&label[i]!=' '&&label[i]!='/'&&label[i]!='+'&&label[i]!='|')i++;
  char icon;if(footer_token(label+start,i-start,&icon)){
   for(int j=start;j<i;j++)label[j]=' ';
   button_icon(8+start*8,255,icon,icon=='O'?RED:CYAN);
  }
 }
 text(1,32,DIM,"%s",label);
}
static void selected_span(int y,int w){if(w<48)w=48;rect(8,y*8-2,w,12,high_contrast?RGB(38,78,88):RGB(16,38,48));rect(8,y*8-2,3,12,GOLD);}
static void selected(int y){selected_span(y,464);}
static const char *stars(int n){static char result[8];for(int i=0;i<5;i++)result[i]=i<n?'*':'.';result[5]=0;return result;}
static void page_number_at(int col,int rownum,int current,int total){if(total>1)text(col,rownum,DIM,"%d/%d",current,total);}
static void update_nearby(void){near_count=0;for(int i=0;i<256;i++)if(i!=game.system&&distance_ly(&game,game.system,i)<=10.01f)nearby[near_count++]=i;
 for(int i=0;i<near_count;i++)for(int j=i+1;j<near_count;j++)if(distance_ly(&game,game.system,nearby[j])<distance_ly(&game,game.system,nearby[i])){int t=nearby[i];nearby[i]=nearby[j];nearby[j]=t;}
}
static void chart_move_cursor(int dx,int dy){
 int from=chart_cursor,best=from,best_score=999999;System *a=&game.systems[from];
 for(int i=0;i<256;i++)if(i!=from){int sx=game.systems[i].x-a->x,sy=game.systems[i].y-a->y,forward=sx*dx+sy*dy;if(forward<=0)continue;int side=abs(sx*dy-sy*dx),score=forward+side*5;if(score<best_score){best_score=score;best=i;}}
 chart_cursor=best;game.cue=SFX_SELECT;
}
static void change_page(int p){
 if(p==FLIGHT&&page!=FLIGHT)fire_blocked=1;
 if(page==HOME&&row>=0&&row<DECK_ITEMS){deck_last=row;deck_focus[deck_group(row)]=row;}
 if(p==HOME||p==FLIGHT||p==INTRO)nav_depth=0;
 else if(!nav_back&&p!=page){if(nav_depth<8){nav_pages[nav_depth]=page==INTRO?HOME:page;nav_rows[nav_depth]=page==INTRO?deck_last:row;nav_depth++;}}
 comms_rescue_confirm=0;abandon_confirm=0;game.boost=0;page=p;row=p==HOME?deck_last:0;game.message_time=0;
 if(p==CHART){update_nearby();if(tracked_mission==0&&game.campaign_stage>=6&&game.saga_step&&game.saga_chapter<SAGA_COUNT)chart_cursor=game.saga_dest;else if(game.route_goal>=0)chart_cursor=game.route_goal;else chart_cursor=game.destination;for(int i=0;i<near_count;i++)if(nearby[i]==game.destination)row=i;}
 if(p==MISSIONLOG){int max=2+game.job_n;if(tracked_mission>=max)tracked_mission=0;row=tracked_mission;}
}
static void menu_back(void){
 int p=HOME,r=deck_last;if(nav_depth){nav_depth--;p=nav_pages[nav_depth];r=nav_rows[nav_depth];}
 nav_back=1;change_page(p);nav_back=0;row=r;
}

static void draw_bodies(void){
 int order[BODY_COUNT];for(int i=0;i<BODY_COUNT;i++)order[i]=i;
 for(int i=0;i<BODY_COUNT;i++)for(int j=i+1;j<BODY_COUNT;j++)if(length(sub(game.bodies[order[j]].pos,game.pos))>length(sub(game.bodies[order[i]].pos,game.pos))){int t=order[i];order[i]=order[j];order[j]=t;}
 for(int i=0;i<BODY_COUNT;i++){Body *b=&game.bodies[order[i]];Vec3 v=camera(&game,b->pos);if(v.z<100)continue;Point p=project(v);float radius=fminf(700,240*b->radius/v.z);if(radius<1||p.x+radius<0||p.x-radius>W)continue;
  int yt=clipy0>=0?clipy0:view_top(),yb=clipy1>=0?clipy1:view_bot(),xt=clipy0>=0?clipx0:0,xb=clipy0>=0?clipx1:W;
  if(p.x+radius<xt||p.x-radius>=xb)continue;
  if(b->type!=SUN){draw_planet_sprite(p.x,p.y,(int)radius,b->seed,b->type,xt,yt,xb,yb);continue;}
  /* Sparse corona, rays and prominences form a crisp PSP-scale star sprite. */
  unsigned corona=RGB((b->color&255)*3/5,((b->color>>8)&255)*3/5,((b->color>>16)&255)*3/5);int style=b->seed&3;
  for(int k=0;k<96;k++){float a=k*6.2831853f/96,r=radius+2+(k%5);int x=(int)(p.x+cosf(a)*r),y=(int)(p.y+sinf(a)*r);if(x>=xt&&x<xb&&y>=yt&&y<=yb)pixel(x,y,corona);}
  for(int k=0;k<12+style*3;k++){float a=(k*6.2831853f/(12+style*3))+(b->seed%100)*.01f;float reach=radius*(1.08f+.05f*((k*7+style)%4));int x0=(int)(p.x+cosf(a)*(radius+1)),y0r=(int)(p.y+sinf(a)*(radius+1)),x1=(int)(p.x+cosf(a)*reach),y1r=(int)(p.y+sinf(a)*reach);if(y0r>=yt&&y0r<=yb&&y1r>=yt&&y1r<=yb)line(x0,y0r,x1,y1r,corona);}
  if(radius>10)for(int k=0;k<2+style;k++){float a=(k*2.1f+(b->seed&31)*.07f);int r=(int)radius;int x0=(int)p.x+(int)(cosf(a)*r),y0r=(int)p.y+(int)(sinf(a)*r),xm=(int)p.x+(int)(cosf(a+.12f)*(r+5+style*2)),ym=(int)p.y+(int)(sinf(a+.12f)*(r+5+style*2)),x1=(int)p.x+(int)(cosf(a+.25f)*r),y1r=(int)p.y+(int)(sinf(a+.25f)*r);if(y0r>=yt&&y0r<=yb&&ym>=yt&&ym<=yb&&y1r>=yt&&y1r<=yb){line(x0,y0r,xm,ym,corona);line(xm,ym,x1,y1r,corona);}}
  int y0=(int)fmaxf(yt,p.y-radius),y1=(int)fminf(yb,p.y+radius);
  for(int y=y0;y<=y1;y++){float yy=(y-p.y)/radius,z=sqrtf(fmaxf(0,1-yy*yy)),light=.3f+.7f*fmaxf(0,-yy*.35f+z*.8f);int width=(int)(radius*z),x0=(int)fmaxf(xt,p.x-width),x1=(int)fminf(xb-1,p.x+width);unsigned tint=b->color;
   if(b->type==OCEAN&&(((y+(int)b->seed)/6)&3)==0)tint=b->accent;
   float texture=b->type==GAS?.68f+.08f*((y+(int)b->seed)/5%4):b->type==SUN?.92f:.78f+.16f*z;
   if(b->type==SUN){light=.82f+.18f*z;texture=.92f;}
   /* Coarse, stable solar granulation. No full-frame shader or texture. */
   for(int x=x0;x<=x1;x+=2){
    float xx=(x-p.x)/radius;float centre=fmaxf(0,1-xx*xx-yy*yy);
    int grain=((int)((x-p.x+radius)*24/fmaxf(1,radius))+(int)((y-p.y+radius)*19/fmaxf(1,radius))*7+(int)(b->seed&15))&7;
    float plasma=.92f+.08f*sinf((x+y*2+(int)(b->seed&255))*0.19f+game.time*.18f);float shade=(.66f+.34f*centre)*(grain==0?.86f:grain==3?1.06f:1.f)*texture*plasma;
    rect(x,y,x<x1?2:1,1,RGB((int)((tint&255)*shade),(int)(((tint>>8)&255)*shade),(int)(((tint>>16)&255)*shade)));
   }
   (void)light;
  }
 }
}
static void draw_portrait(int x,int y,int w,int h,int system,int role);
#include "flight-extras.h"
#include "planet.h"
#include "pixel-art.h"
#include "voyage.h"
static void space(void){
 if(!valid_target(selected_target)){selected_target=0;autoaim=0;}
 /* Modal views draw only part of the world. Clear both alternating buffers
  * so a prior warp, laser or target marker cannot remain behind the panel. */
 if(game.approach>=0||game.police_stop||game.dead||game.dock_stage>=2){
  rect(0,0,W,H,BG);
 }
 if(game.planet>=0){
  planet_view();hud_postfx();
  if(game.dead)death_effect();
  if(game.surface==2){planet_eva_hud();return;}
  if(hud_mode==0||game.dead)cockpit();
  else if(hud_mode==1)minimal_overlay();
  return;
 }
 if(game.jump>0){warp_effect();cockpit();return;}
 if(game.approach>=0){planet_prompt();cockpit();return;}
 if(game.police_stop){if(hud_mode==0)cockpit();police_dialog();return;}
 if(game.dead){death_effect();if(hud_mode==0)cockpit();return;}
 if(game.dock_stage>=2){docking_view();if(hud_mode==0)cockpit();return;}
 sector_background();starfield();celestial_rims();draw_bodies();lens_flares();station_model();secondary_hubs();ambient_space();
 int npc_detailed[NPC_COUNT]={0};
 for(int i=0;i<NPC_COUNT;i++){NPC *n=&game.npc[i];if(!n->alive||occluded(n->pos))continue;float distance=length(sub(n->pos,game.pos)),limit=n->freighter?12000.f:5200.f;if(distance>limit)continue;npc_detailed[i]=n->freighter?2:1;unsigned c=n->flash>0?WHITE:faction_colors[n->role];float yaw=atan2f(n->dir.x,n->dir.z);if(npc_detailed[i]==2){capital_model(n,c);continue;}shipmesh(n->mesh,n->pos,yaw,0,n->scale,c,0);}
 for(int i=0;i<DEBRIS_COUNT;i++){Debris *d=&game.debris[i];if(!d->alive||occluded(d->pos))continue;float distance=length(sub(d->pos,game.pos));if(distance>11000)continue;
  int mesh=mesh_id(d->rock==2?"BOULDER":d->rock?"ASTEROID":d->wreck?"BOULDER":"CANISTER");
  float size=1;for(int v=0;v<meshes[mesh].vertices;v++)size=fmaxf(size,length(meshes[mesh].v[v]));
  unsigned color=d->flash>0?RGB(225,216,181):d->rock==2?RGB(143,178,193):d->rock?RGB(121,106,87):d->wreck?RGB(101,108,112):GOLD;
  shipmesh(mesh,d->pos,game.time*(d->rock?.045f:.2f)+i,i*.21f,d->radius/size,color,0);
 }
 for(int i=0;i<ANOMALY_COUNT;i++)if(game.anomaly[i].alive){float d=length(sub(game.anomaly[i].pos,game.pos));if(d>180&&d<10000)shipmesh(mesh_id("WORM"),game.anomaly[i].pos,game.time*.7f+i,sinf(game.time+i)*.2f,4.2f,game.anomaly[i].kind?CYAN:GOLD,0);}
 flush_meshes();
 for(int i=0;i<NPC_COUNT;i++)if(npc_detailed[i]==1){NPC *n=&game.npc[i];ship_sprite_detail(n,n->flash>0?WHITE:faction_colors[n->role]);}
 npc_engine_glow();station_glow();station_entrance();speed_lines();engine_flare();missile_effects();
 for(int i=0;i<ANOMALY_COUNT;i++)if(game.anomaly[i].alive&&length(sub(game.anomaly[i].pos,game.pos))<=180){unsigned c=game.anomaly[i].kind?CYAN:GOLD;circle(240,110,18+(int)(sinf(game.time*4)*4),c);circle(240,110,7,c);}
 for(int i=0;i<NPC_COUNT;i++){NPC *n=&game.npc[i];if(!n->alive||npc_detailed[i]||occluded(n->pos))continue;Vec3 v=camera(&game,n->pos);if(v.z<30)continue;Point p=project(v);if(p.x<2||p.x>477||p.y<view_top()+2||p.y>view_bot()-2)continue;unsigned c=faction_colors[n->role];rect((int)p.x-1,(int)p.y-1,n->freighter?5:3,n->freighter?3:2,c);}
 freight_effects();mining_effects();
 for(int i=0;i<NPC_COUNT;i++){NPC *n=&game.npc[i];if(!n->alive||n->flash<=0||n->target==-1)continue;Vec3 end=n->target==-2?add(game.pos,mul(forward(&game),30)):game.npc[n->target].pos;Vec3 a=camera(&game,n->pos),b=camera(&game,end);if(a.z>15&&b.z>15){Point p=project(a),q=project(b);if(p.y>view_top()&&p.y<view_bot()&&q.y>view_top()&&q.y<view_bot())line((int)p.x,(int)p.y,(int)q.x,(int)q.y,n->role==LAW?CYAN:RED);}}
 if(game.shot>.1f){line(50,view_bot(),236,110,RED);line(430,view_bot(),244,110,RED);}
 line(227,110,236,110,AMBER);line(244,110,253,110,AMBER);line(240,97,240,106,AMBER);line(240,114,240,123,AMBER);
 line(232,102,236,106,AMBER);line(244,106,248,102,AMBER);line(232,118,236,114,AMBER);line(244,114,248,118,AMBER);
 hud_postfx();
 if(hud_mode==0)target_overlay();else if(hud_mode==1)minimal_overlay();
 warp_effect();planet_prompt();police_dialog();death_effect();
 if(hud_mode==0||game.dock_stage||game.dead||game.police_stop||game.approach>=0)cockpit();
 else if(hud_mode==2)combat_alert_banner(); /* scenic: still show bottom RED ALERT */
 hud_postfx();
}
#include "ship-preview.h"
#include "ui-modern.h"
#include "narrative-nav.h"
#include "guild-ui.h"
#include "campaign-ui.h"
#include "radio-ui.h"
#include "comms-panel.h"
#include "intro.h"
static void walk_screen(void){
 static const struct {float x,z;int role;const char *name;const char *line;} folk[]={
  {-90,80,TRADERS,"DOCKHAND","Berths are tight today. Watch the freighter lane."},
  {70,140,LAW,"PATROL","Keep your warrant clean and we stay friendly."},
  {-40,260,EXPLORERS,"SURVEYOR","Guild charts need fresh surface samples."},
  {110,40,TRADERS,"TRADER","Market board is live. Food moves fast here."},
  {20,320,EXPLORERS,"KEI","When you are ready, open Tracked Mission."},
  {-140,200,TRADERS,"TECH","Fuel nozzles are free on bay three."},
  {160,280,LAW,"CUSTOMS","Restricted goods still raise local warrants."}
 };
 int folk_n=(int)(sizeof(folk)/sizeof(folk[0]));
 int horizon=96+(int)(sinf(preview_time*.45f+walk_yaw)*.5f);rect(0,0,W,H,walk_kind==2?RGB(3,8,16):RGB(10,16,24));
 for(int y=horizon;y<H;y++){unsigned c=mix_rgb(RGB(28,40,52),RGB(12,18,26),(y-horizon)/(float)fmaxf(1,H-horizon));for(int x=0;x<W;x++)fb[y*STRIDE+x]=c;}
 for(int i=0;i<14;i++){int y=horizon+i*12;int inset=i*9;line(0,y,W/2-inset,horizon,RGB(36+i,52+i,66+i));line(W,y,W/2+inset,horizon,RGB(36+i,52+i,66+i));}
 line(0,horizon,W,horizon,CYAN);
 rect(0,0,W,28,RGB(8,19,28));rect(0,26,W,2,GOLD);
 const char *place=walk_kind==0?station_name(&game):walk_kind==1?"SHIP / PERSONAL DECK":"DERELICT / AIRLOCK";
 text(1,0,GOLD,"ON FOOT / %.18s",place);text(1,2,DIM,walk_kind==2?"NUB LOOK  D-PAD MOVE  X SALVAGE  O EXIT":"NUB LOOK  D-PAD MOVE  X TALK  O EXIT");
 int nearest=-1;float best=999;
 if(walk_kind==0)for(int i=0;i<folk_n;i++){
  float dx=folk[i].x-walk_x,dz=folk[i].z-walk_z;float d=sqrtf(dx*dx+dz*dz);if(d<best){best=d;nearest=i;}
  float relx=dx*cosf(-walk_yaw)-dz*sinf(-walk_yaw),relz=dx*sinf(-walk_yaw)+dz*cosf(-walk_yaw);
  if(relz<20||relz>420)continue;
  int sx=240+(int)(relx*180.f/relz),sy=horizon+(int)(42.f*180.f/relz);int h=(int)fmaxf(18,fminf(70,5200.f/relz));
  if(sx<-20||sx>500)continue;
  unsigned ink=faction_colors[folk[i].role];
  rect(sx-h/5,sy-h,h/2,h,RGB(22,34,44));rect(sx-h/6,sy-h,h/3,h/4,ink);pixel(sx,sy-h+h/8,WHITE);
  if(i==nearest&&best<70){rect(sx-18,sy+4,36,3,GOLD);text((sx-20)/8,(sy+8)/8,GOLD,"X");}
 }
 int sway=(int)(sinf(walk_x*.04f)*2);
 rect(188+sway,horizon-8,104,78,RGB(24,43,52));rect(198+sway,horizon,84,62,RGB(10,24,34));
 rect(220+sway,horizon+18,40,24,walk_kind==0?RGB(35,120,124):walk_kind==2?RGB(88,56,45):RGB(90,64,40));
 rect(42,horizon+20,70,40,RGB(17,30,38));rect(368,horizon+20,70,40,RGB(17,30,38));
 text(6,(horizon+28)/8,walk_kind==0?GOLD:CYAN,walk_kind==0?"ARRIVALS":walk_kind==2?"SALVAGE":"AIRLOCK");
 text(40,(horizon+28)/8,WHITE,"%s",walk_kind==0?"CONCOURSE":walk_kind==2?"TETHER":"DECK");
 rect(0,248,W,24,RGB(8,19,28));rect(0,248,W,2,CYAN);
 if(walk_kind==2){rect(8,252,180,8,RGB(22,28,36));rect(8,252,(int)(1.8f*walk_oxygen),8,CYAN);rect(8,262,180,8,RGB(36,22,26));rect(8,262,(int)(1.8f*walk_integrity),8,RED);text(25,32,WHITE,"O2 %d  SUIT %d",(int)walk_oxygen,(int)walk_integrity);}
 else if(walk_kind==0&&nearest>=0&&best<70){text(1,32,GOLD,"%.12s",folk[nearest].name);text(16,32,WHITE,"%.40s",folk[nearest].line);}
 else text(1,32,DIM,walk_kind==0?"Walk toward people. X talks. O returns to deck.":"Workshop. Triangle returns to the concourse.");
}
static void input(unsigned pressed,unsigned held,float dt,float ax,float ay){
 static unsigned in_held=0;static int sq_arm=0;
 unsigned released=in_held&~held;in_held=held;if(page!=FLIGHT||paused||game.police_stop||game.approach>=0||game.dock_stage)sq_arm=0;
 if(!(held&PSP_CTRL_CROSS))fire_blocked=0;
 square_held=page==FLIGHT&&game.planet<0&&game.jump<=0&&!game.dock_stage&&!game.dead&&(held&PSP_CTRL_SQUARE);
 int oldpage=page;r_tap+=dt;l_tap+=dt;if(hard_brake>0){hard_brake-=dt;if(hard_brake<0)hard_brake=0;}if(page==INTRO){intro_time+=dt;if(pressed&PSP_CTRL_CROSS){game.voice_time=0;change_page(CAMPAIGN);}else if(pressed&PSP_CTRL_START){game.voice_time=0;change_page(HOME);}else if(pressed&PSP_CTRL_TRIANGLE){if(load_game(&game,"commander.sav")){selected_target=0;autoaim=0;change_page(HOME);}else message(&game,"No saved commander. X begins your journey.");}return;}
 if(page==WALK){walk_yaw+=ax*dt*1.8f;float move=ay*dt*(walk_kind==2?68.f:90.f);walk_x+=sinf(walk_yaw)*move;walk_z+=cosf(walk_yaw)*move;if(walk_x>220)walk_x=220;if(walk_x<-220)walk_x=-220;if(walk_z>420)walk_z=420;if(walk_z<-80)walk_z=-80;
  if(walk_kind==2){walk_oxygen-=dt*2.2f;if(walk_x>185||walk_x<-185||walk_z>385||walk_z<-45)walk_integrity-=dt*3.f;if(walk_oxygen<=0||walk_integrity<=0){walk_oxygen=0;page=FLIGHT;message(&game,"Suit reserve exhausted. Emergency tether return.");return;}}
  if(pressed&PSP_CTRL_TRIANGLE&&walk_kind==0){walk_kind=1;walk_x=walk_z=walk_yaw=0;message(&game,"Ship deck unlocked. X inspects the workshop.");}
  else if(pressed&PSP_CTRL_CIRCLE){page=walk_kind==2?FLIGHT:HOME;message(&game,walk_kind==2?"Airlock sealed. Back aboard the ship.":"Back on the command deck.");}
  else if(pressed&PSP_CTRL_CROSS){
   if(walk_kind==2&&!walk_salvaged){walk_salvaged=1;game.credits+=500;game.discoveries++;message(&game,"Salvage secured: relay core +500 units.");}
   else if(walk_kind==0){
    static const struct {float x,z;int role;const char *name;const char *line;} folk[]={
     {-90,80,TRADERS,"DOCKHAND","Berths are tight today. Watch the freighter lane."},
     {70,140,LAW,"PATROL","Keep your warrant clean and we stay friendly."},
     {-40,260,EXPLORERS,"SURVEYOR","Guild charts need fresh surface samples."},
     {110,40,TRADERS,"TRADER","Market board is live. Food moves fast here."},
     {20,320,EXPLORERS,"KEI","When you are ready, open Tracked Mission."},
     {-140,200,TRADERS,"TECH","Fuel nozzles are free on bay three."},
     {160,280,LAW,"CUSTOMS","Restricted goods still raise local warrants."}
    };
    int nearest=-1;float best=999;for(int i=0;i<(int)(sizeof(folk)/sizeof(folk[0]));i++){float dx=folk[i].x-walk_x,dz=folk[i].z-walk_z,d=sqrtf(dx*dx+dz*dz);if(d<best){best=d;nearest=i;}}
    if(nearest>=0&&best<70){speak(&game,folk[nearest].role==LAW?VOICE_LAW:folk[nearest].role==EXPLORERS?VOICE_KEI:VOICE_DOCK,folk[nearest].line);message(&game,folk[nearest].line);game.cue=SFX_UI;}
    else message(&game,"Move closer to someone, then press X.");
   }else message(&game,"Workshop inspection complete.");
  }
  return;}
 if(page==FLIGHT&&!paused&&!game.police_stop&&!game.dock_stage&&game.jump<=0&&game.approach<0&&!game.dead){
  if((pressed&PSP_CTRL_TRIANGLE)&&(held&PSP_CTRL_TRIANGLE)){triangle_arm=1;triangle_hold=0;}
  if(triangle_arm){
   pressed&=~PSP_CTRL_TRIANGLE;
   if(held&PSP_CTRL_TRIANGLE){triangle_hold+=dt;if(triangle_hold>=.55f){triangle_arm=0;comms_return=FLIGHT;autoaim=0;change_page(COMMS_PANEL);return;}}
   else {triangle_arm=0;pressed|=PSP_CTRL_TRIANGLE;}
  }
 }else {triangle_arm=0;triangle_hold=0;}
 if(page==FLIGHT&&(pressed&PSP_CTRL_SELECT)&&(held&PSP_CTRL_LTRIGGER)){hud_mode=(hud_mode+1)%3;hud_hidden=hud_mode==2;message(&game,hud_mode==0?"HUD: full.":hud_mode==1?"HUD: minimal.":"HUD: scenic / hidden.");return;}
 if(game.police_stop){game.boost=0;autoaim=0;if(pressed&PSP_CTRL_UP){police_choice=(police_choice+2)%3;game.cue=SFX_SELECT;}if(pressed&PSP_CTRL_DOWN){police_choice=(police_choice+1)%3;game.cue=SFX_SELECT;}if(pressed&PSP_CTRL_CROSS){if(police_choice==0)police_resolve(&game,0);else if(police_choice==1){if(police_resolve(&game,1))change_page(HOME);}else police_escape(&game);if(!game.police_stop)police_choice=0;}return;}
 if(pressed&PSP_CTRL_START){if(game.dead){if(campaign_retry(&game)){selected_target=0;autoaim=0;change_page(CAMPAIGN);}else {game_init(&game);deck_reset();selected_target=0;autoaim=0;change_page(STORY);}}}
 if(page==FLIGHT&&!game.dead&&!game.police_stop&&game.jump<=0&&!game.dock_stage)paused=(held&PSP_CTRL_START)!=0;else if(paused)paused=0;
 /* Hold Start: redistribute on the existing SYS/ENG/WEP meters — no separate panel. */
 if(paused){fire_blocked=1;game.boost=0;if(pressed&PSP_CTRL_LEFT)pip_sel=(pip_sel+2)%3;if(pressed&PSP_CTRL_RIGHT)pip_sel=(pip_sel+1)%3;if(pressed&PSP_CTRL_UP)pip_shift(&game,pip_sel);if(pressed&PSP_CTRL_DOWN)pip_selected_move(&game,pip_sel,-1);return;}
 if(game.dock_stage){autoaim=0;if(game.dock_stage==1&&(pressed&PSP_CTRL_CIRCLE)){game.dock_stage=0;game.speed=0;game.boost=0;message(&game,"Docking guidance cancelled. You have control.");return;}game_tick(&game,dt,0,0,0,0);if(game.docked)change_page(HOME);return;}
 if(game.dead){game_tick(&game,dt,0,0,0,0);return;}
 if(page==FLIGHT&&game.jump>0){game.boost=0;game_tick(&game,dt,0,0,0,0);if(game.jump<=0){selected_target=0;autoaim=0;}return;}
 if(page==FLIGHT&&(pressed&PSP_CTRL_RTRIGGER)&&!(held&PSP_CTRL_SQUARE)){if(r_tap<.32f&&game.heat<85){game.boost=1;game.cue=SFX_BOOST;}else if(r_tap<.32f&&game.heat>=85)message(&game,"Too hot to boost. Cool down first.");r_tap=0;}if(!(held&PSP_CTRL_RTRIGGER)||page!=FLIGHT||(game.planet>=0&&game.surface==1)||(held&PSP_CTRL_SQUARE)||game.heat>=90)game.boost=0;
 if(page==FLIGHT&&(pressed&PSP_CTRL_LTRIGGER)&&!(held&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT|PSP_CTRL_SQUARE))){if(l_tap<.32f&&game.speed>player_ships[game.ship].speed*.35f){hard_brake=.55f;game.boost=0;game.cue=SFX_UI;message(&game,"Hard brake.");}l_tap=0;}
 if(page==FLIGHT&&game.approach>=0){game.boost=0;if(pressed&PSP_CTRL_CIRCLE){turn_back(&game);autoaim=0;}else if(pressed&PSP_CTRL_CROSS){if(enter_planet(&game))autoaim=0;}return;}
 if(page==FLIGHT&&game.planet>=0&&(pressed&PSP_CTRL_CIRCLE)){if(game.surface)eva_toggle(&game);else land_planet(&game);autoaim=0;return;}
 if(page==FLIGHT&&game.planet>=0&&(pressed&PSP_CTRL_TRIANGLE)){if(speech_active())speech_ok();else if(game.surface==1)takeoff_planet(&game);else if(game.surface==2)message(&game,"Board the ship before takeoff.");else leave_planet(&game);autoaim=0;return;}
 if(page==FLIGHT&&game.planet>=0&&(pressed&PSP_CTRL_SQUARE)){
  if(game.surface==2)survey_scan(&game);
  else if(game.surface==1)message(&game,"Leave the ship to scan. Press O.");
  else {Vec3 pad=surface_site(&game,1);float dx=pad.x-game.pos.x,dz=pad.z-game.pos.z;float dist=sqrtf(dx*dx+dz*dz);game.yaw=atan2f(dx,dz);game.pitch=fminf(-.12f,game.pitch);autoaim=0;char note[80];snprintf(note,sizeof(note),dist<160?"Pad under you. Slow down and press O.":"Pad %d m ahead. Slow down to land.",(int)dist);message(&game,note);}
  return;}
 if(page==FLIGHT&&game.jump<=0&&!game.dead&&!game.dock_stage&&(pressed&PSP_CTRL_LTRIGGER)&&(held&PSP_CTRL_SQUARE)){sq_arm=0;cycle_front_target();}
 else if(page==FLIGHT&&flight_target_combo(pressed,held))sq_arm=0;
 else if(page==FLIGHT&&game.jump<=0&&!game.dead&&!game.dock_stage&&(pressed&PSP_CTRL_RTRIGGER)&&(held&PSP_CTRL_SQUARE)){sq_arm=0;if(valid_target(selected_target)){autoaim=1;story_event(&game,STORY_EV_TARGET);if(selected_target==0)campaign_event(&game,CP_LOCK);message(&game,"Target locked. Auto-turn active; steer to cancel.");game.cue=SFX_UI;}else message(&game,"Choose a target with Square and the D-pad first.");}
 else if(page==FLIGHT&&game.jump<=0&&!game.dead&&!game.dock_stage&&(pressed&PSP_CTRL_SQUARE)&&(held&PSP_CTRL_RTRIGGER)){int hostile=nearest_hostile_target();sq_arm=0;if(hostile>=0){selected_target=hostile;scan_cat=4;autoaim=1;story_event(&game,STORY_EV_TARGET);if(selected_target==0)campaign_event(&game,CP_LOCK);message(&game,"Nearest hostile locked.");game.cue=SFX_UI;}else message(&game,"No hostile contacts.");}
 else if(page==FLIGHT&&game.jump<=0&&!game.dead&&!game.dock_stage&&(pressed&PSP_CTRL_SQUARE))sq_arm=1;
 if(page==FLIGHT&&game.jump<=0&&!game.dead&&!game.dock_stage&&(released&PSP_CTRL_SQUARE)&&sq_arm){sq_arm=0;if(valid_target(selected_target))ensure_scan_cat_for_target(selected_target);change_page(TARGETING);target_count=collect_scan_ids(target_ids,scan_cat);if(row>=target_count)row=0;story_event(&game,STORY_EV_TARGET);if(selected_target==0)campaign_event(&game,CP_LOCK);}
 else if(page==FLIGHT&&(released&PSP_CTRL_SQUARE))sq_arm=0;
 if(page==FLIGHT&&(pressed&PSP_CTRL_CROSS)&&(held&PSP_CTRL_LTRIGGER))fire_missile(&game,selected_target);
 else if(page==FLIGHT&&game.planet>=0&&(pressed&PSP_CTRL_CROSS))message(&game,"Lasers are offline in atmosphere.");
 if(page==HELP&&(pressed&PSP_CTRL_LTRIGGER)){analog_enabled=!analog_enabled;analog_ready=0;message(&game,analog_enabled?"Centre the nub to enable analog steering.":"D-pad steering. Analog input ignored.");ax=ay=0;}
  if(page==FLIGHT){if(pressed&PSP_CTRL_SELECT)change_page(HOME);else if(pressed&PSP_CTRL_TRIANGLE){if(speech_active())speech_ok();else hail_target();}else if(pressed&PSP_CTRL_CIRCLE){pick_look_target();int target=selected_target;if(look_target>=0)target=look_target;if(IS_NPC_ID(target)){selected_target=target;scan_cat=target_category(target);autoaim=0;message(&game,npc_is_hostile(&game.npc[target-BODY_COUNT-1])?"Hostile locked. Triangle to hail.":"Ship locked. Triangle to talk.");}else if(IS_ANOMALY_ID(target))analysis_scan(&game,target);else if(IS_DEBRIS_ID(target))salvage(&game,target);else if(target>=2&&target<=BODY_COUNT){if(approach_planet(&game,target-1)){selected_target=target;scan_cat=0;autoaim=0;}}else if(target==0){if(dock(&game)){autoaim=0;change_page(FLIGHT);}}else if(target==1)message(&game,"The sun has no landing approach.");else message(&game,"Look at a station, planet, echo, cargo, or mission target.");}}
 else {
  if(page==LOCAL)contacts_refresh();
   if(page==TARGETING){int ids[1+BODY_COUNT+NPC_COUNT+DEBRIS_COUNT+ANOMALY_COUNT];target_count=collect_scan_ids(ids,scan_cat);if(target_count>0){for(int i=0;i<target_count;i++)target_ids[i]=ids[i];}if(row>=target_count)row=0;}
  int saga_choices=game.campaign_stage>=6&&game.saga_chapter<SAGA_COUNT&&game.saga_step&&saga_beats[game.saga_chapter].kind==SAGA_CHOICE;
  int count=page==COMFORT?5:page==CAMPAIGN?(tracked_mission==0&&game.campaign_stage==0?1:tracked_mission==0&&game.campaign_stage>=6&&game.saga_chapter<SAGA_COUNT&&!game.saga_step?1:saga_choices?3:tracked_mission>=2?2:1):page==GUILD?2:page==STORY?(game.story<STORY_FREE?2:1):page==COMMS_PANEL?11:page==RADIO?3:page==HOME?DECK_ITEMS:page==MISSIONS?mission_count(&game):page==MISSIONLOG?2+game.job_n:page==DEBUG?8:page==LOCAL?contact_count:page==TARGETING?target_count:page==GALNET?galnet_rows():page==MARKET?cargo_rows():page==CHART?near_count:page==YARD?player_ship_count:page==EQUIP?9:page==FACTIONS?FACTION_COUNT:page==DETAILS?(1+BODY_COUNT):page==CODEX?codex_rows():1;
  if(page==MARKET&&!game.docked&&(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT))){message(&game,"Dock to buy or sell. Market controls are locked.");game.cue=SFX_UI;return;}
  if(count<1)count=1;
  if(pressed&(PSP_CTRL_UP|PSP_CTRL_DOWN))game.cue=SFX_SELECT;
  if(pressed&PSP_CTRL_UP){if(page==HOME)deck_step(-1);else if(page==CHART&&chart_mode)chart_move_cursor(0,-1);else row=(row+count-1)%count;}
  if(pressed&PSP_CTRL_DOWN){if(page==HOME)deck_step(1);else if(page==CHART&&chart_mode)chart_move_cursor(0,1);else row=(row+1)%count;}
 if(page==CHART&&chart_mode&&(pressed&PSP_CTRL_LEFT))chart_move_cursor(-1,0);
 if(page==CHART&&chart_mode&&(pressed&PSP_CTRL_RIGHT))chart_move_cursor(1,0);
 if(page==HOME&&(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT)))deck_tab(pressed&PSP_CTRL_RIGHT?1:-1);
 if(page==HELP&&(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT)))help_tab=(help_tab+(pressed&PSP_CTRL_RIGHT?1:3))%4;
   if(page==TARGETING&&(pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER))){step_scan_cat(pressed&PSP_CTRL_RTRIGGER?1:-1);target_count=collect_scan_ids(target_ids,scan_cat);row=0;}
   if(page==CHART&&chart_mode&&(pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER))){chart_zoom+=pressed&PSP_CTRL_RTRIGGER?1:-1;if(chart_zoom<1)chart_zoom=1;if(chart_zoom>4)chart_zoom=4;game.cue=SFX_SELECT;}
   if(page==GALNET&&(pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER))){galnet_tab=(galnet_tab+(pressed&PSP_CTRL_RTRIGGER?1:5))%6;row=0;}
   if(page==CODEX&&(pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER))){codex_tab=(codex_tab+(pressed&PSP_CTRL_RTRIGGER?1:5))%6;row=0;}
   if(pressed&PSP_CTRL_CIRCLE){if(page==CAMPAIGN&&story_brief_locked()){message(&game,"Finish this conversation first.");game.cue=SFX_UI;return;}if(page==MISSIONLOG&&abandon_confirm){abandon_confirm=0;message(&game,"Abandon cancelled. Your job is safe.");return;}if((page==RADIO||page==COMMS_PANEL||page==COMFORT)&&radio_dirty&&!radio_save_settings("radio.cfg")){message(&game,"Audio settings could not be saved. Try again.");return;}if(page==COMMS_PANEL)change_page(comms_return);else if(page==TARGETING){if(game.docked)menu_back();else change_page(FLIGHT);}else if(page==GALNET)change_page(HOME);else if(page==HOME){if(!game.docked)change_page(FLIGHT);}else menu_back();}
   else if(page==COMMS&&(pressed&PSP_CTRL_SQUARE))change_page(RADIO);
   else if(page==STORY&&(pressed&PSP_CTRL_CROSS)){
    if(row==1&&game.story<STORY_FREE){story_skip(&game);change_page(HOME);}
    else {int next=game.story<STORY_FREE?story_home_row(&game):0;change_page(HOME);row=next;deck_last=next;}
   }
   else if(page==GUILD&&(pressed&PSP_CTRL_CROSS)){if(row==0)narrative_do(GUILD);else change_page(STORY);}
   else if(page==HOME&&(pressed&PSP_CTRL_CROSS)){if(row==20){if(game.docked){walk_kind=0;walk_x=walk_z=walk_yaw=0;page=WALK;message(&game,"Station concourse. Walk to people and press X to talk.");}else message(&game,"Dock first to walk the station deck.");return;}int pages[]={FLIGHT,MARKET,CHART,YARD,EQUIP,STATUS,HELP,FACTIONS,TARGETING,DEBUG,COMMS,DETAILS,MISSIONS,MISSIONLOG,GALNET,CODEX,RADIO,CAMPAIGN,GUILD,COMFORT};if(row<0||row>=20)return;int opened=row,next=pages[row];if(!story_menu_ok(&game,opened)){message(&game,story_task(&game));game.cue=SFX_UI;return;}if(!game.docked&&(next==YARD||next==EQUIP||next==MISSIONS)){message(&game,"Dock at a station to open this service.");game.cue=SFX_UI;return;}if(next==FLIGHT){int leaving=game.docked;int keep=(selected_target>=0&&selected_target<=BODY_COUNT)?selected_target:0;analog_ready=0;ax=ay=0;launch(&game);if(leaving){selected_target=keep;autoaim=0;if(valid_target(keep))scan_cat=target_category(keep);}}change_page(next);story_on_open(&game,opened);if(!game.cue)game.cue=SFX_UI;}
   else if(page==LOCAL&&contact_count>0&&row>=0&&row<contact_count&&(pressed&(PSP_CTRL_CROSS|PSP_CTRL_TRIANGLE))){selected_target=contact_ids[row];scan_cat=target_category(selected_target);nav_body=selected_target>0&&selected_target<=BODY_COUNT?selected_target-1:-1;autoaim=(pressed&PSP_CTRL_TRIANGLE)!=0;story_event(&game,STORY_EV_TARGET);if(selected_target==0)campaign_event(&game,CP_LOCK);message(&game,"Target set.");if(!game.docked)change_page(FLIGHT);}
   else if(page==DETAILS&&(pressed&(PSP_CTRL_CROSS|PSP_CTRL_TRIANGLE))){int id=row==0?0:row;if(valid_target(id)){selected_target=id;scan_cat=target_category(id);nav_body=id>0&&id<=BODY_COUNT?id-1:-1;autoaim=(pressed&PSP_CTRL_TRIANGLE)!=0;story_event(&game,STORY_EV_TARGET);if(selected_target==0)campaign_event(&game,CP_LOCK);message(&game,id==0?"Station locked.":id==1?"Sun locked. No landing.":"Body locked.");if(!game.docked)change_page(FLIGHT);}}
  else if(page==COMMS&&(pressed&PSP_CTRL_TRIANGLE)){comms_rescue_confirm=!comms_rescue_confirm;}else if(page==COMMS&&(pressed&PSP_CTRL_CROSS)){if(comms_rescue_confirm){if(emergency_rescue(&game)){selected_target=0;autoaim=0;change_page(HOME);}return;}if(game.docked){message(&game,"Already docked.");game.cue=SFX_UI;}else if(dock(&game)){selected_target=0;autoaim=0;change_page(FLIGHT);}}
   else if(page==CAMPAIGN&&(pressed&PSP_CTRL_SELECT)){if(story_brief_locked()){message(&game,"Finish this conversation first.");game.cue=SFX_UI;return;}change_page(MISSIONLOG);}
   else if(page==MISSIONS&&(pressed&PSP_CTRL_SELECT))change_page(MISSIONLOG);
   else if(page==MISSIONLOG&&(pressed&PSP_CTRL_SELECT)){tracked_mission=row;change_page(CAMPAIGN);}
   else if(page==MISSIONS&&(pressed&PSP_CTRL_CROSS)){accept_mission(&game,row);}
   else if(page==MISSIONLOG){
    if(row>=2&&(pressed&PSP_CTRL_TRIANGLE)){abandon_confirm=1;message(&game,"Abandon this job? Press X again to confirm, Circle to cancel.");}
    if((pressed&PSP_CTRL_CROSS)&&!abandon_confirm){tracked_mission=row;if(row>=2)game.job_sel=row-2;message(&game,row==0?"Tracking: Kei and Ryn.":row==1?"Tracking: Explorers Guild assignment.":"Contract tracked. Select opens its next step.");}
    if(row>=2&&row-2<game.job_n){int ji=row-2;Job *j=&game.jobs[ji];game.job_sel=ji;game.contract=j->dest;game.mission_type=j->type;game.mission_stage=j->stage;game.mission_target=j->target;game.mission_item=j->item;game.contract_reward=j->reward;game.contract_time=j->time;
     if(pressed&PSP_CTRL_CROSS){
      if(abandon_confirm){abandon_confirm=0;abandon_mission(&game,ji);if(tracked_mission==row)tracked_mission=0;if(row>=2+game.job_n)row=1+game.job_n;}
     }
    }
   }
   else if(page==TARGETING&&(pressed&PSP_CTRL_CROSS)){if(target_count){selected_target=target_ids[row];scan_cat=target_category(selected_target);nav_body=selected_target>0&&selected_target<=BODY_COUNT?selected_target-1:-1;autoaim=!game.docked;story_event(&game,STORY_EV_TARGET);if(selected_target==0)campaign_event(&game,CP_LOCK);message(&game,game.docked?"Target selected for launch.":"Target locked. Auto-align active.");if(game.docked)menu_back();else change_page(FLIGHT);}}
   else if(page==TARGETING&&(pressed&PSP_CTRL_TRIANGLE)){target_details=!target_details;}
   else if(page==COMFORT&&(pressed&PSP_CTRL_CROSS)){
 if(row==0){hud_mode=(hud_mode+1)%3;hud_hidden=hud_mode==2;}
 else if(row==1){quiet_comms=!quiet_comms;radio_dirty=1;}
 else if(row==2){high_contrast=!high_contrast;message(&game,high_contrast?"High contrast focus on.":"High contrast focus off.");}
 else change_page(row==3?RADIO:HELP);
}
else if(page==CAMPAIGN&&(pressed&PSP_CTRL_CROSS)){
 if(tracked_mission==0&&prologue_brief_locked()){
  if(prologue_brief_beat<PROLOGUE_BRIEF_BEATS-1){prologue_brief_beat++;if(prologue_brief_beat==1)game.campaign_choice=1;else if(prologue_brief_beat==2)game.campaign_choice=2;row=0;game.cue=SFX_SELECT;}
  else {narrative_do(CAMPAIGN);prologue_brief_beat=0;}
 }
 else if(tracked_mission==0&&game.campaign_stage==0)narrative_do(CAMPAIGN);
 else if(tracked_mission==0&&game.campaign_stage>=6){if(game.saga_chapter<SAGA_COUNT&&!game.saga_step){if(saga_brief_beat<SAGA_BRIEF_BEATS-1){saga_brief_beat++;row=0;game.cue=SFX_SELECT;}else saga_begin(&game);}else if(game.saga_chapter<SAGA_COUNT&&saga_beats[game.saga_chapter].kind==SAGA_CHOICE){game.saga_choice=row+1;saga_advance(&game);row=0;}else if(!saga_advance(&game)){int hops=0,hop=saga_next_hop(&game,&hops);if(hop<0){message(&game,"No route with this drive. Fit more jump range.");}else{route_clear(&game);game.destination=hop;change_page(CHART);char note[96];snprintf(note,sizeof(note),hop==game.saga_dest?"Destination selected: %s.":"Next jump: %s. Final destination: %s.",game.systems[hop].name,game.systems[game.saga_dest].name);message(&game,note);}}}
 else if(tracked_mission==0){if(narrative_action(CAMPAIGN)==NA_REWARD)campaign_claim(&game);else game.cue=SFX_UI;}
 else if(tracked_mission==1){if(narrative_action(GUILD)==NA_REWARD)guild_claim(&game);else game.cue=SFX_UI;}
 else {int ji=tracked_mission-2;if(row==0&&ji>=0&&ji<game.job_n)navigate_job(ji);else change_page(MISSIONLOG);}
}
else if(page==COMMS_PANEL&&(pressed&PSP_CTRL_CROSS)){
  if(row==0){quiet_comms=!quiet_comms;radio_dirty=1;speech_ok();message(&game,quiet_comms?"Text chatter muted. Safety alerts remain.":"Text chatter restored.");}
  else if(row==1){speech_ok();change_page(comms_return);}
  else if(row==2){change_page(FLIGHT);hail_target();}
  else if(row==3){selected_target=0;autoaim=0;look_target=-1;message(&game,"Target cleared.");}
  else if(row==4){radio_tune((radio_station+1)%RADIO_STATION_COUNT);char note[64];snprintf(note,sizeof(note),"Radio: %s",radio_station_name(radio_station));message(&game,note);}
  else if(row==5)change_page(RADIO);
  else if(row==6){if(dock(&game)){selected_target=0;change_page(FLIGHT);}}
  else if(row==7){tracked_mission=1;change_page(MISSIONLOG);row=1;}
  else if(row==8){hud_mode=(hud_mode+1)%3;hud_hidden=hud_mode==2;message(&game,hud_mode==0?"HUD: full.":hud_mode==1?"HUD: minimal.":"HUD: scenic.");}
  else if(row==9){if(game.docked){walk_kind=0;walk_x=walk_z=walk_yaw=0;page=WALK;}else message(&game,"Dock first to walk the station deck.");}
  else if(row==10){if(IS_ANOMALY_ID(selected_target)&&length(sub(target_position(selected_target),game.pos))<1400){walk_kind=2;walk_x=walk_z=walk_yaw=0;walk_oxygen=100;walk_integrity=100;walk_salvaged=0;page=WALK;message(&game,"Tether attached. Salvage the derelict relay core.");}else message(&game,"Lock a nearby derelict echo before spacewalking.");}
 }
 else if(page==GALNET&&(galnet_tab==3||galnet_tab==4)){if(galnet_tab==3&&(pressed&PSP_CTRL_CROSS))spacebook_likes[game.system]^=1u<<row;if(pressed&PSP_CTRL_TRIANGLE)spacebook_comments=!spacebook_comments;}
 else if(page==RADIO){if(pressed&PSP_CTRL_TRIANGLE){radio_off=!radio_off;if(radio_off)radio_static_ms=0;radio_dirty=1;}else if(row==0&&(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT))){int dir=pressed&PSP_CTRL_RIGHT?1:-1;int next=radio_off?-1:radio_station;next+=dir;if(next<-1)next=-1;if(next>=RADIO_STATION_COUNT)next=RADIO_STATION_COUNT-1;if(next<0){radio_off=1;radio_static_ms=6;radio_dirty=1;}else{radio_off=0;radio_tune(next);radio_static_ms=8;}}else if(row>=1&&row<=2&&(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT))){radio_adjust(row==2,pressed&PSP_CTRL_RIGHT?1:-1);}else if(row==0&&(pressed&PSP_CTRL_CROSS)){if(radio_off){radio_off=0;radio_static_ms=5;radio_dirty=1;}else radio_tune(radio_station);}}else if(page==DEBUG&&(pressed&PSP_CTRL_CROSS)){debug_action();}
  else if(page==MARKET&&game.docked){if(pressed&PSP_CTRL_RIGHT)trade(&game,row,1);if(pressed&PSP_CTRL_LEFT)trade(&game,row,0);}
  else if(page==CHART){
   if(pressed&PSP_CTRL_TRIANGLE){chart_mode=!chart_mode;if(chart_mode){if(tracked_mission==0&&game.campaign_stage>=6&&game.saga_step&&game.saga_chapter<SAGA_COUNT)chart_cursor=game.saga_dest;else if(game.route_goal>=0)chart_cursor=game.route_goal;else chart_cursor=game.destination;}message(&game,chart_mode?"Galaxy overview: all 256 systems.":"Nearby jump list.");}
   else if(chart_mode&&(pressed&PSP_CTRL_CROSS)){Game route=game;route.fuel=(float)player_ships[game.ship].range;int jumps=0,hop=route_next_hop(&route,chart_cursor,&jumps);if(hop<0)message(&game,"No route with the fitted jump drive.");else{route_set_goal(&game,chart_cursor);game.destination=hop;chart_mode=0;update_nearby();row=0;for(int i=0;i<near_count;i++)if(nearby[i]==hop)row=i;char note[96];snprintf(note,sizeof(note),"Route to %s: %d jump%s. Next: %s.",game.systems[chart_cursor].name,jumps,jumps==1?"":"s",game.systems[hop].name);message(&game,note);}}
   else {if(near_count)game.destination=nearby[row];if(pressed&PSP_CTRL_CROSS){if(near_count&&distance_ly(&game,game.system,game.destination)*10<=game.fuel+.01f){if(game.docked)launch(&game);if(jump_start(&game)){selected_target=0;autoaim=0;change_page(FLIGHT);}}else message(&game,"Refuel first. That star is out of range.");}}
  }
  else if(page==YARD&&(pressed&PSP_CTRL_CROSS)){if(!game.docked)message(&game,"Dock to exchange ships.");else buy_ship(&game,row);}
  else if(page==EQUIP&&(pressed&PSP_CTRL_CROSS)){buy_equipment(equip_show[row]);}
  else if(page==STATUS&&game.docked){if(pressed&PSP_CTRL_CROSS)save_game(&game,"commander.sav");if(pressed&PSP_CTRL_TRIANGLE){if(load_game(&game,"commander.sav")){selected_target=0;autoaim=0;look_target=-1;}else message(&game,"Load failed, or no save found.");}}
 }
 float turn=0,pitch=0;int throttle=0,fire=0;if(page==FLIGHT){turn=ax;pitch=ay;int rolling=(held&PSP_CTRL_LTRIGGER)&&(held&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT));if(rolling){game.roll+=((held&PSP_CTRL_RIGHT)?1:-1)*dt*2;turn=pitch=0;game.boost=0;autoaim=0;}throttle=(held&PSP_CTRL_RTRIGGER?1:0)-(held&PSP_CTRL_LTRIGGER?1:0);if(rolling||(held&PSP_CTRL_SQUARE)||hard_brake>0)throttle=0;if(hard_brake>0){game.speed*=fmaxf(.15f,1.f-dt*5.5f);if(game.speed<40)game.speed=0;}fire=!fire_blocked&&oldpage==FLIGHT&&!game.dock_stage&&game.approach<0&&(held&PSP_CTRL_CROSS)!=0&&!(held&PSP_CTRL_LTRIGGER)&&game.jump<=0;align_target(dt,ax,ay);}
 if(page==FLIGHT){if(oldpage!=FLIGHT){turn=pitch=0;throttle=fire=0;}game_tick(&game,dt,turn,pitch,throttle,fire);if(game.docked)change_page(HOME);}
}
static void input_tests(void){
 FILE *f=fopen("input-check.txt","w");if(!f)return;int failures=0;
#define INPUT_CHECK(c,n) do{int ok=(c);fprintf(f,"%s %s\n",ok?"PASS":"FAIL",n);failures+=!ok;}while(0)
#define TEST_INIT() do{game_init(&game);deck_reset();story_complete(&game);paused=0;selected_target=0;autoaim=0;scan_cat=2;tracked_mission=0;prologue_brief_beat=0;saga_brief_beat=0;saga_brief_chapter=-1;}while(0)
 TEST_INIT();launch(&game);page=FLIGHT;game.pos=(Vec3){0,0,-20000};game.speed=0;for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;
 input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);INPUT_CHECK(!game.boost,"single R press does not boost");
 input(0,0,.1f,0,0);input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);INPUT_CHECK(game.boost,"double R press starts boost");
 input(0,PSP_CTRL_RTRIGGER,.016f,0,0);INPUT_CHECK(game.boost,"held R maintains boost");input(0,0,.016f,0,0);INPUT_CHECK(!game.boost,"R release cancels boost");
 TEST_INIT();launch(&game);page=FLIGHT;game.speed=player_ships[game.ship].speed;game.heat=0;hard_brake=0;l_tap=10;
 input(PSP_CTRL_LTRIGGER,PSP_CTRL_LTRIGGER,.016f,0,0);INPUT_CHECK(hard_brake<=0,"single L press does not hard-brake");
 input(0,0,.1f,0,0);input(PSP_CTRL_LTRIGGER,PSP_CTRL_LTRIGGER,.016f,0,0);INPUT_CHECK(hard_brake>0,"double L while fast starts hard brake");
 float braked=game.speed;input(0,0,.05f,0,0);INPUT_CHECK(game.speed<braked,"hard brake dumps speed quickly");
 game.heat=90;r_tap=0;input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);input(0,0,.1f,0,0);input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);INPUT_CHECK(!game.boost,"critical heat refuses a boost double-tap");
 change_page(LOCAL);row=2;input(PSP_CTRL_TRIANGLE,0,.016f,0,0);INPUT_CHECK(page==FLIGHT&&selected_target==2&&autoaim,"local menu selects named planet and auto-aligns");
 input(0,0,.016f,1,0);INPUT_CHECK(!autoaim,"manual steering cancels auto-aim");
 game.approach=1;game.pos=add(game.bodies[1].pos,(Vec3){0,0,-game.bodies[1].radius-800});game.yaw=game.pitch=0;Vec3 facing=forward(&game);input(PSP_CTRL_CIRCLE,0,.016f,0,0);INPUT_CHECK(game.approach==-1&&dot(facing,forward(&game))<-.999f,"Circle exits approach and turns around");
 TEST_INIT();change_page(CHART);row=0;input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);INPUT_CHECK(!game.docked&&page==FLIGHT&&game.jump>0&&game.shots==0,"chart X launches and warps without firing");
 input(PSP_CTRL_TRIANGLE,0,.016f,0,0);INPUT_CHECK(page==FLIGHT&&game.jump>0,"warp countdown locks navigation until arrival");
 input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE,.016f,0,0);input(0,0,.016f,0,0);INPUT_CHECK(page==FLIGHT&&game.jump>0,"square tap during warp does not open the computer");
 input(PSP_CTRL_START,0,.016f,0,0);INPUT_CHECK(page==FLIGHT&&game.jump>0&&!paused,"start during warp does not freeze hyperspace");
 TEST_INIT();launch(&game);page=FLIGHT;pip_sel=1;game.pip_sys=2;game.pip_eng=2;game.pip_wep=4;
 input(0,PSP_CTRL_START,.016f,0,0);INPUT_CHECK(paused,"holding Start enters power redistribute on the cockpit meters");
 input(PSP_CTRL_RIGHT,PSP_CTRL_START|PSP_CTRL_RIGHT,.016f,0,0);INPUT_CHECK(pip_sel==2,"Start+Right selects WEP on the existing meters");
 input(PSP_CTRL_UP,PSP_CTRL_START|PSP_CTRL_UP,.016f,0,0);INPUT_CHECK(game.pip_wep==4&&game.pip_sys+game.pip_eng+game.pip_wep==8,"Start+Up cannot push a bank past four pips");
 input(PSP_CTRL_LEFT,PSP_CTRL_START|PSP_CTRL_LEFT,.016f,0,0);INPUT_CHECK(pip_sel==1,"Start+Left returns to ENG");
 int eng_before=game.pip_eng;input(PSP_CTRL_UP,PSP_CTRL_START|PSP_CTRL_UP,.016f,0,0);INPUT_CHECK(game.pip_eng==eng_before+1,"Start+Up puts more power into ENG");
 int eng_hi=game.pip_eng;input(PSP_CTRL_DOWN,PSP_CTRL_START|PSP_CTRL_DOWN,.016f,0,0);INPUT_CHECK(game.pip_eng==eng_hi-1,"Start+Down takes power out of ENG");
 input(0,0,.016f,0,0);INPUT_CHECK(!paused&&game.pip_sys+game.pip_eng+game.pip_wep==8,"releasing Start resumes flight with eight pips still assigned");
 TEST_INIT();game.campaign_stage=6;saga_begin(&game);chart_mode=0;chart_zoom=1;change_page(CHART);input(PSP_CTRL_TRIANGLE,0,.016f,0,0);
 INPUT_CHECK(chart_mode&&chart_cursor==game.saga_dest,"galaxy overview opens focused on the tracked mission destination");
 input(PSP_CTRL_RTRIGGER,0,.016f,0,0);INPUT_CHECK(chart_zoom==2,"galaxy overview R zooms in");
 input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(!chart_mode&&game.destination!=game.system&&distance_ly(&game,game.system,game.destination)<=player_ships[game.ship].range*.1f+.001f,"galaxy route planner selects a reachable first jump");
 INPUT_CHECK(game.route_goal==game.saga_dest,"galaxy plot stores the final mission destination as the route goal");
 TEST_INIT();{int far=-1,hops=0,first=-1;for(int i=0;i<256;i++){int hop=route_next_hop(&game,i,&hops);if(hop>=0&&hops>1){far=i;first=hop;break;}}INPUT_CHECK(far>=0&&first>=0,"manual route test finds a multi-jump destination");change_page(CHART);chart_mode=1;chart_cursor=far;chart_zoom=1;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(game.route_goal==far&&game.destination==first&&game.destination!=far,"manual galaxy plot keeps final goal separate from the next hop");}
 TEST_INIT();launch(&game);page=FLIGHT;selected_target=2;Body *body=&game.bodies[1];game.pos=add(body->pos,(Vec3){0,0,-body->radius-800});game.speed=0;game.yaw=game.pitch=0;input(PSP_CTRL_CIRCLE,0,.016f,0,0);INPUT_CHECK(game.approach==1,"Circle approaches the nearby targeted planet");input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(game.planet==1&&game.approach<0,"X from approach enters atmosphere flight");input(PSP_CTRL_CIRCLE,0,.016f,0,0);INPUT_CHECK(game.planet==1&&game.surface==0,"Circle does not leave orbit while flying high");input(PSP_CTRL_TRIANGLE,0,.016f,0,0);input(PSP_CTRL_TRIANGLE,0,.016f,0,0);INPUT_CHECK(game.planet<0,"Triangle returns from atmosphere to orbit");
 change_page(HOME);row=3;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(page==HOME,"ship shop cannot open in flight");row=4;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(page==HOME,"equipment shop cannot open in flight");
 game.cargo[0]=2;game.cargo[7]=1;INPUT_CHECK(cargo_rows()==2&&cargo_item(1)==7,"flight inventory lists only owned cargo");contacts_refresh();INPUT_CHECK(contact_count>=BODY_COUNT+1&&contact_ids[BODY_COUNT]==BODY_COUNT,"contacts include station and every celestial body");
 page=FLIGHT;game.pos=(Vec3){0,0,-20000};float speed=game.speed;input(0,PSP_CTRL_LTRIGGER|PSP_CTRL_RIGHT,.016f,1,0);INPUT_CHECK(game.roll>0&&game.speed==speed,"L and right rolls without changing throttle");
 change_page(DEBUG);row=0;int cash=game.credits;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(game.credits==cash+10000,"debug adds 1000 displayed units");
  TEST_INIT();launch(&game);page=FLIGHT;hud_mode=hud_hidden=0;input(PSP_CTRL_SELECT,PSP_CTRL_SELECT|PSP_CTRL_LTRIGGER,.016f,0,0);INPUT_CHECK(hud_mode==1&&!hud_hidden&&page==FLIGHT,"L and Select selects minimal HUD");input(PSP_CTRL_SELECT,PSP_CTRL_SELECT|PSP_CTRL_LTRIGGER,.016f,0,0);INPUT_CHECK(hud_mode==2&&hud_hidden,"L and Select selects scenic HUD");input(PSP_CTRL_SELECT,PSP_CTRL_SELECT|PSP_CTRL_LTRIGGER,.016f,0,0);INPUT_CHECK(hud_mode==0&&!hud_hidden,"L and Select restores full HUD");
 TEST_INIT();game.credits=20000;game.systems[game.system].tech=12;page=EQUIP;row=6;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK((game.upgrades&1)&&game.credits==17500,"outfitting installs docking computer and charges balance");row=8;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(cargo_capacity(&game)==player_ships[game.ship].capacity+8,"outfitting installs expanded cargo bay");
 row=3;int missiles=game.missiles;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(game.missiles==missiles+1&&game.credits==13000,"outfitting reloads one missile and charges balance");
 TEST_INIT();launch(&game);page=FLIGHT;input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE,.016f,0,0);INPUT_CHECK(page==FLIGHT,"Square press waits for release before opening the computer");input(0,0,.016f,0,0);INPUT_CHECK(page==TARGETING&&target_count>0,"Square tap opens populated targeting computer");
 for(int i=0;i<NPC_COUNT;i++)if(game.npc[i].alive&&game.npc[i].role==PIRATES){game.npc[i].target=-2;break;}
 target_filter=2;target_refresh();INPUT_CHECK(target_count>0,"hostile targeting filter finds ships engaging the player");target_filter=10;target_refresh();INPUT_CHECK(target_count>=1,"anomaly filter lists rare system echoes"); galnet_tab=3;INPUT_CHECK(galnet_rows()==7,"SpaceBook provides a scrollable generated feed");galnet_tab=4;INPUT_CHECK(galnet_rows()==5,"Messages sits beside Spacebook with its own feed");
 change_page(HOME);row=15;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(page==CODEX,"command deck opens the Discovery Codex");codex_tab=0;INPUT_CHECK(codex_kind_count(0)>=1,"Codex Systems lists visited systems");codex_tab=1;INPUT_CHECK(codex_kind_count(1)==systems_visited(&game)*4,"Codex Planets lists four worlds per visited system");
 TEST_INIT();game.system=0;launch(&game);page=FLIGHT;game.pos=(Vec3){0,0,0};
 for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;
 game.npc[0].alive=1;game.npc[0].role=TRADERS;game.npc[0].target=-1;game.npc[0].pos=(Vec3){200,0,800};
 game.npc[1].alive=1;game.npc[1].role=PIRATES;game.npc[1].target=-2;game.npc[1].pos=(Vec3){-200,0,900};
 scan_cat=2;selected_target=0;
 input(PSP_CTRL_LEFT,PSP_CTRL_SQUARE|PSP_CTRL_LEFT,.016f,0,0);INPUT_CHECK(page==FLIGHT&&IS_NPC_ID(selected_target)&&scan_cat==1,"Square+Left tabs to ships (all contacts) without opening the computer");
 /* SHIPS must include the engaging pirate — locking them must not flip to ENEMIES. */
 scan_cat=1;selected_target=BODY_COUNT+1; /* trader */
 input(PSP_CTRL_UP,PSP_CTRL_SQUARE|PSP_CTRL_UP,.016f,0,0);
 INPUT_CHECK(page==FLIGHT&&scan_cat==1&&IS_NPC_ID(selected_target),"Square+Up cycles within SHIPS without leaving the band");
 {int saw_hostile=0,ids[NPC_COUNT],n=collect_scan_ids(ids,1);for(int i=0;i<n;i++)if(ids[i]==BODY_COUNT+2)saw_hostile=1;INPUT_CHECK(saw_hostile&&n==2,"SHIPS band lists every ship including hostiles");}
 game.npc[1].alive=1;game.npc[1].role=PIRATES;game.npc[1].target=-2;
 {int ids[NPC_COUNT],n=collect_scan_ids(ids,4);INPUT_CHECK(n==1&&ids[0]==BODY_COUNT+2,"ENEMIES band lists only ships currently attacking the player");}
 input(PSP_CTRL_RIGHT,PSP_CTRL_SQUARE|PSP_CTRL_RIGHT,.016f,0,0);INPUT_CHECK(page==FLIGHT&&selected_target==0,"Square+Right tabs back to the station");input(PSP_CTRL_RIGHT,PSP_CTRL_SQUARE|PSP_CTRL_RIGHT,.016f,0,0);INPUT_CHECK(page==FLIGHT&&(IS_DEBRIS_ID(selected_target)||IS_ANOMALY_ID(selected_target)),"Square+Right tabs to other contacts");
 /* Keep only the pirate in range so AI keeps engaging the player (not a nearer trader). */
 game.npc[0].alive=0;
 game.npc[1].alive=1;game.npc[1].role=PIRATES;game.npc[1].pos=(Vec3){0,0,600};game.npc[1].target=-2;
 input(PSP_CTRL_RIGHT,PSP_CTRL_SQUARE|PSP_CTRL_RIGHT,.016f,0,0);INPUT_CHECK(page==FLIGHT&&IS_NPC_ID(selected_target)&&npc_is_hostile(&game.npc[selected_target-BODY_COUNT-1])&&scan_cat==4,"Square+Right tabs to the ENEMIES band");
 for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;
 scan_cat=3;
 input(PSP_CTRL_RIGHT,PSP_CTRL_SQUARE|PSP_CTRL_RIGHT,.016f,0,0);INPUT_CHECK(page==FLIGHT&&scan_cat==4,"Square+Right still opens empty ENEMIES band");
 game.attacked=2.f;INPUT_CHECK(combat_alert_active(),"combat alert arms under fire");
 input(PSP_CTRL_RIGHT,PSP_CTRL_SQUARE|PSP_CTRL_RIGHT,.016f,0,0);INPUT_CHECK(page==FLIGHT&&selected_target>=2&&selected_target<=BODY_COUNT,"Square+Right tabs to planets");int planet_lock=selected_target;input(PSP_CTRL_UP,PSP_CTRL_SQUARE|PSP_CTRL_UP,.016f,0,0);INPUT_CHECK(page==FLIGHT&&selected_target>=2&&selected_target<=BODY_COUNT&&selected_target!=planet_lock,"Square+Up cycles the highlighted planet band");
 INPUT_CHECK(!autoaim,"Square+D-pad highlights without turning the ship");input(PSP_CTRL_RTRIGGER,PSP_CTRL_SQUARE|PSP_CTRL_RTRIGGER,.016f,0,0);INPUT_CHECK(page==FLIGHT&&autoaim&&!game.boost,"R while holding Square locks the selected target without boosting");input(0,0,.016f,0,0);INPUT_CHECK(page==FLIGHT&&autoaim,"releasing Square keeps the explicit target lock active");
 TEST_INIT();launch(&game);page=FLIGHT;for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;game.pos=(Vec3){0,0,0};game.yaw=game.pitch=0;for(int i=0;i<3;i++){game.npc[i].alive=1;game.npc[i].pos=(Vec3){i?100:-100,0,i==2?-900:900};}selected_target=-1;input(PSP_CTRL_LTRIGGER,PSP_CTRL_SQUARE|PSP_CTRL_LTRIGGER,.016f,0,0);int view_first=selected_target;INPUT_CHECK(valid_target(view_first)&&camera(&game,target_position(view_first)).z>20&&view_first!=NPC_ID_MIN+2&&!autoaim,"Square+L highlights a visible forward contact without auto-turning");input(0,PSP_CTRL_SQUARE,.016f,0,0);input(PSP_CTRL_LTRIGGER,PSP_CTRL_SQUARE|PSP_CTRL_LTRIGGER,.016f,0,0);INPUT_CHECK(valid_target(selected_target)&&selected_target!=view_first&&camera(&game,target_position(selected_target)).z>20&&selected_target!=NPC_ID_MIN+2,"repeated Square+L cycles only contacts in front");
 TEST_INIT();launch(&game);page=FLIGHT;input(PSP_CTRL_SQUARE|PSP_CTRL_LEFT,PSP_CTRL_SQUARE|PSP_CTRL_LEFT,.016f,0,0);INPUT_CHECK(page==FLIGHT,"Square+Left chord never opens the computer");input(0,0,.016f,0,0);INPUT_CHECK(page==FLIGHT,"releasing a Square chord still leaves flight");
 TEST_INIT();game.system=0;launch(&game);page=FLIGHT;for(int i=0;i<NPC_COUNT;i++)if(game.npc[i].alive&&game.npc[i].role==PIRATES){game.npc[i].target=-2;game.npc[i].pos=(Vec3){0,0,600};break;}
 input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE|PSP_CTRL_RTRIGGER,.016f,0,0);INPUT_CHECK(page==FLIGHT&&selected_target>BODY_COUNT&&npc_is_hostile(&game.npc[selected_target-BODY_COUNT-1])&&scan_cat==4,"R and Square locks nearest hostile on the ENEMIES band");int shots=game.shots;input(PSP_CTRL_CROSS,PSP_CTRL_CROSS|PSP_CTRL_LTRIGGER,.016f,0,0);INPUT_CHECK(game.missile_time>0&&game.missiles==0&&game.shots==shots,"L and X launches missile without firing laser");
 TEST_INIT();game.contract=game.system;game.mission_type=MISSION_DELIVERY;page=TARGETING;target_filter=5;target_refresh();INPUT_CHECK(target_count==1&&target_ids[0]==0,"mission filter isolates marked station objective");
 TEST_INIT();INPUT_CHECK(!strcmp(tracked_hud_cue(),"DOCK: LAVE HUB"),"HUD cue tells the player to dock for the tracked story");game.credits=20000;accept_mission(&game,0);tracked_mission=2;INPUT_CHECK(strstr(tracked_hud_cue(),game.systems[game.jobs[0].dest].name)!=0,"HUD cue follows a tracked station contract destination");
 TEST_INIT();game.credits=20000;int took=0;for(int i=0;i<5;i++)took+=accept_mission(&game,i)!=0;int old_destination=game.destination;INPUT_CHECK(took==5&&game.job_n==5&&!accept_mission(&game,0)&&game.destination==old_destination,"mission board rejects a sixth job when the log is full");
 TEST_INIT();game.credits=20000;accept_mission(&game,0);INPUT_CHECK(mission_offer_active(&game,0)&&!accept_mission(&game,0)&&game.job_n==1,"accepted offer stays in progress and cannot be taken twice");change_page(MISSIONLOG);row=1;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(tracked_mission==1,"mission log tracks the optional Guild assignment");input(PSP_CTRL_SELECT,0,.016f,0,0);INPUT_CHECK(page==CAMPAIGN&&tracked_mission==1,"tracked next-step screen follows the selected Guild mission");change_page(MISSIONLOG);row=2;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(tracked_mission==2&&game.job_sel==0,"mission log tracks an accepted station contract");input(PSP_CTRL_TRIANGLE,0,.016f,0,0);INPUT_CHECK(game.job_n==1&&abandon_confirm,"triangle asks before abandoning the focused mission");input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(game.job_n==0&&!abandon_confirm&&tracked_mission==0,"second X abandons the contract and restores story tracking");
 game_init(&game);deck_reset();change_page(HOME);INPUT_CHECK(row==0,"new commander opens the Fly category with Launch focused");row=6;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(page==HELP&&game.story==STORY_LAUNCH,"opening Controls advances the optional coach");
 game_init(&game);deck_reset();change_page(HOME);row=3;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(page==YARD&&game.story==STORY_BRIEF,"optional coaching leaves the shipyard available");
 TEST_INIT();change_page(HOME);row=15;input(PSP_CTRL_CIRCLE,0,.016f,0,0);INPUT_CHECK(page==HOME&&row==15,"circle on a docked deck keeps the selected door");
 TEST_INIT();change_page(COMMS);input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(game.docked&&page==COMMS,"comms while docked stays on the station channel");
 game_init(&game);deck_reset();change_page(STORY);row=1;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(game.story==STORY_FREE&&page==HOME,"flight guide ends only through its visible menu option");
 TEST_INIT();change_page(HOME);row=20;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(page==WALK&&walk_kind==0,"docked Fly menu disembarks onto the station concourse");
 input(0,0,.016f,0,.8f);INPUT_CHECK(page==WALK&&fabsf(walk_z)>1.f,"station walk: D-pad / nub forward moves along the concourse");
 walk_x=-90;walk_z=80;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(game.voice_time>0||game.message_time>0,"station walk: X talks to a nearby concourse NPC");
 input(PSP_CTRL_CIRCLE,0,.016f,0,0);INPUT_CHECK(page==HOME,"station walk: Circle returns to the command deck");
 TEST_INIT();launch(&game);page=FLIGHT;game.approach=1;enter_planet(&game);{Vec3 pad=surface_site(&game,1);game.pos=add(pad,(Vec3){0,18,0});game.speed=8;land_planet(&game);eva_toggle(&game);}
 {Vec3 before=game.pos;input(0,0,.05f,0,.9f);INPUT_CHECK(game.surface==2&&length(sub(game.pos,before))>1.f,"planet EVA: nub forward walks across the surface");}
 #include "journey-input-tests.h"
 #include "radio-input-tests.h"
 #include "comms-input-tests.h"
 #include "campaign-input-tests.h"
 #include "ui-navigation-tests.h"
 {
  RadarPoint ahead=radar_point((Vec3){0,0,1000}),aft=radar_point((Vec3){0,0,-1000});
  RadarPoint left=radar_point((Vec3){-1000,0,0}),right=radar_point((Vec3){1000,0,0});
  INPUT_CHECK(ahead.y<230&&aft.y>230&&ahead.x==240&&aft.x==240,"radar: ahead and behind occupy opposite halves");
  INPUT_CHECK(left.x<240&&right.x>240,"radar: left and right match ship-relative bearing");
  int bounded=1;for(int i=0;i<360;i++){float a=i*.017453293f;RadarPoint q=radar_point((Vec3){sinf(a)*100000,50000,cosf(a)*100000});if(q.x<178||q.x>302||q.y<214||q.y>246||abs(q.lift)>4)bounded=0;}
  INPUT_CHECK(bounded,"radar: all 360-degree distant bearings remain inside instrument");
 }
 {
  unsigned *saved_fb=fb,*pixels=malloc(STRIDE*H*sizeof(unsigned));
  INPUT_CHECK(pixels!=0,"graphics: disposable framebuffer allocated");
  if(pixels){
   fb=pixels;TEST_INIT();hud_mode=hud_hidden=0;int old_quiet=quiet_comms;quiet_comms=0;
   memset(pixels,0,STRIDE*H*sizeof(unsigned));button_icon(10,10,'T',WHITE);
   INPUT_CHECK(pixels[11*STRIDE+14]&&pixels[17*STRIDE+11]&&pixels[17*STRIDE+17]&&!pixels[12*STRIDE+11],"graphics: PSP Triangle icon points upward");
   int clean=1;
   for(int speaker=VOICE_KEI;speaker<=VOICE_COMP;speaker++){
    memset(pixels,0,STRIDE*H*sizeof(unsigned));
    speak(&game,speaker,"Long radio transmission checks that every word remains in the top caption band.");
    cockpit();
    for(int y=56;y<192;y++)for(int x=0;x<W;x++)if(pixels[y*STRIDE+x])clean=0;
   }
   INPUT_CHECK(clean,"graphics: routine cockpit text never paints central viewing area");
   memset(pixels,0,STRIDE*H*sizeof(unsigned));launch(&game);page=FLIGHT;tracked_mission=0;hud_mode=hud_hidden=0;quiet_comms=1;game.voice_time=0;game.message_time=0;cockpit();
   {
    const char *cue=tracked_hud_cue();int clen=(int)strlen(cue);int cols=W/8,inset=1,left=34,max=cols-inset-left;if(clen>max)clen=max;
    int start=cols-inset-clen,right_px=(start+clen)*8,gold=0,far=0;
    for(int y=0;y<8;y++)for(int x=start*8;x<right_px&&x<W;x++)if(pixels[y*STRIDE+x]==GOLD){gold=1;if(x>=W-16)far=1;}
    INPUT_CHECK(gold&&far&&start>=left,"graphics: mission cue sits flush on the top-right header");
   }
   memset(pixels,0,STRIDE*H*sizeof(unsigned));add_crime(&game,15);cockpit();
   {
    int wl=wanted_level(&game),seen=0;
    for(int y=8;y<16;y++)for(int x=8;x<120;x++)if(pixels[y*STRIDE+x]==RED)seen=1;
    INPUT_CHECK(wl==3&&seen,"graphics: local wanted level sits under the system name when police may pursue");
   }
   memset(pixels,0,STRIDE*H*sizeof(unsigned));launch(&game);page=HOME;menu_space_view(246,64,218,92);
   {
    int gold=0;for(int y=65;y<150;y++)for(int x=247;x<460;x++)if(pixels[y*STRIDE+x]==GOLD||((pixels[y*STRIDE+x]&255)>180&&((pixels[y*STRIDE+x]>>8)&255)>140))gold=1;
    INPUT_CHECK(gold,"graphics: Select deck top-right shows a third-person ship silhouette");
   }
   {
    /* Engine roots must sit on the mesh aft tip — radius-scaled glow floated past wide ships. */
    int aft_ok=1;
    for(int mesh=0;mesh<mesh_count;mesh++){
     float aft=0,rad=1;const Mesh *m=&meshes[mesh];
     for(int v=0;v<m->vertices;v++){float L=length(m->v[v]);if(L>rad)rad=L;aft=fmaxf(aft,-m->v[v].z);}
     if(aft<1)continue;
     NPC sample={0};sample.mesh=mesh;sample.scale=1;sample.radius=rad;sample.dir=(Vec3){0,0,1};
     if(npc_engine_aft(&sample)>aft+0.01f)aft_ok=0;
    }
    NPC tip={0};tip.mesh=mesh_id("VIPER");tip.scale=1;tip.radius=80;tip.dir=(Vec3){0,0,1};
    float viperaft=0;for(int v=0;v<meshes[tip.mesh].vertices;v++)viperaft=fmaxf(viperaft,-meshes[tip.mesh].v[v].z);
    Vec3 root=npc_engine_root(&tip,0);
    INPUT_CHECK(aft_ok&&fabsf(root.z+viperaft)<0.05f&&viperaft<tip.radius*.65f,"graphics: engine glow roots at the mesh aft tip (not past the silhouette)");
   }
   memset(pixels,0,STRIDE*H*sizeof(unsigned));preview_clip(240,110,20,20,25,25);
   DrawTri t={{{20,20,30},{30,20,30},{20,30,30}},WHITE,30};triangle(&t);
   int clipped=1;for(int y=0;y<H;y++)for(int x=0;x<W;x++)if(pixels[y*STRIDE+x]&&(x<20||x>=25||y<20||y>=25))clipped=0;
   INPUT_CHECK(clipped,"graphics: odd-sized preview clips contain every raster pixel");
   preview_reset();quiet_comms=old_quiet;fb=saved_fb;free(pixels);TEST_INIT();
  }
 }
 {
  int fits=1;
  preview_clip(353,84,242,40,464,128);
  for(int ship=0;ship<player_ship_count;ship++){
   int id=mesh_id(player_ships[ship].name);Vec3 centre;float scale;ship_preview_layout(id,&centre,&scale);
   for(int a=0;a<16;a++)for(int b=0;b<16;b++)for(int v=0;v<meshes[id].vertices;v++){
    Vec3 point=add((Vec3){0,0,320},mul(rotate(sub(meshes[id].v[v],centre),a*.392699f,b*.392699f),scale));Point p=project(point);
    if(point.z<15||p.x<246||p.x>460||p.y<44||p.y>124)fits=0;
   }
  }
  preview_reset();INPUT_CHECK(fits,"shipyard: every hull fits with padding throughout full yaw and roll rotations");
 }
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);
#undef INPUT_CHECK
#undef TEST_INIT
 game_init(&game);deck_reset();story_complete(&game);change_page(HOME);selected_target=0;nav_body=-1;autoaim=0;r_tap=10;hud_mode=hud_hidden=0;
}
int main(void){
 int cb=sceKernelCreateThread("Callbacks",callback_thread,0x11,4096,0,0);if(cb>=0)sceKernelStartThread(cb,0,0);
 scePowerSetClockFrequency(333,333,166);sceCtrlSetSamplingCycle(0);sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
 sceDisplaySetMode(0,W,H);pspDebugScreenInit();pspDebugScreenEnableBackColor(0);
 game_init(&game);deck_reset();FILE *flag=fopen("smoke.flag","r");if(flag){smoke=1;fclose(flag);FILE *visual=fopen("visual.flag","r");if(visual){visual_hold=1;fclose(visual);}FILE *log=fopen("boot-check.txt","w");if(log){fprintf(log,"PSP main reached; %d meshes loaded.\n",mesh_count);fclose(log);}}
 if(smoke){radio_tests();steering_tests();game_tests("game-check.txt");input_tests();}
 else {game.voice_time=0;change_page(INTRO);}
 FILE *introflag=fopen("open-intro.flag","r");if(introflag){fclose(introflag);change_page(INTRO);intro_time=6;}FILE *socialflag=fopen("open-spacebook.flag","r");if(socialflag){fclose(socialflag);change_page(GALNET);galnet_tab=3;game.voice_time=0;}FILE *netflag=fopen("open-galnet.flag","r");if(netflag){int tab=0;fscanf(netflag,"%d",&tab);fclose(netflag);change_page(GALNET);galnet_tab=tab>=0&&tab<6?tab:0;row=0;game.voice_time=0;}FILE *helpflag=fopen("open-help.flag","r");if(helpflag){int tab=0;fscanf(helpflag,"%d",&tab);fclose(helpflag);change_page(HELP);help_tab=tab>=0&&tab<4?tab:0;}
 FILE *yardflag=fopen("open-yard.flag","r");if(yardflag){int ship=0;fscanf(yardflag,"%d",&ship);fclose(yardflag);change_page(YARD);row=ship>=0&&ship<player_ship_count?ship:0;game.voice_time=0;story_complete(&game);}
 FILE *sflag=fopen("open-story.flag","r");if(sflag){fclose(sflag);change_page(STORY);}
 FILE *factionflag=fopen("open-factions.flag","r");if(factionflag){fclose(factionflag);change_page(FACTIONS);row=1;}FILE *comfortflag=fopen("open-comfort.flag","r");if(comfortflag){fclose(comfortflag);change_page(COMFORT);}FILE *homeflag=fopen("open-home.flag","r");if(homeflag){int item=0;fscanf(homeflag,"%d",&item);fclose(homeflag);change_page(HOME);row=item>=0&&item<DECK_ITEMS?item:0;game.voice_time=0;}FILE *codexflag=fopen("open-codex.flag","r");if(codexflag){int tab=0;fscanf(codexflag,"%d",&tab);fclose(codexflag);change_page(CODEX);codex_tab=tab>=0&&tab<6?tab:0;row=0;game.voice_time=0;}FILE *cpflag=fopen("open-campaign.flag","r");if(cpflag){int stage=0,flying=0;fscanf(cpflag,"%d %d",&stage,&flying);fclose(cpflag);if(flying)launch(&game);game.campaign_stage=stage>=0&&stage<=6?stage:0;game.voice_time=0;tracked_mission=0;change_page(CAMPAIGN);}FILE *guildflag=fopen("open-guild.flag","r");if(guildflag){int chapter=0,ready=0;fscanf(guildflag,"%d %d",&chapter,&ready);fclose(guildflag);game.guild_chapter=chapter>=0&&chapter<=4?chapter:0;game.guild_flags=ready?31:0;game.voice_time=0;tracked_mission=1;change_page(CAMPAIGN);}
 FILE *flyflag=fopen("open-flight.flag","r");if(flyflag){fclose(flyflag);launch(&game);page=FLIGHT;hud_mode=hud_hidden=0;}
 FILE *freightflag=fopen("open-freighter.flag","r");if(freightflag){int style=0,phase=0;fscanf(freightflag,"%d %d",&style,&phase);fclose(freightflag);launch(&game);story_complete(&game);NPC *n=&game.npc[8];n->freight_style=style%3;n->radius=length(freight_extent(n));if(phase){n->freight_state=phase==1?FREIGHT_ARRIVING:FREIGHT_CHARGING;n->freight_timer=phase==1?2.4f:1.5f;game.freight_gap=0;}game.pos=freight_world(n,(Vec3){1100,450,1000});Vec3 d=norm(sub(n->pos,game.pos));game.yaw=atan2f(d.x,d.z);game.pitch=asinf(d.y);game.speed=0;game.voice_time=game.message_time=0;page=FLIGHT;selected_target=NPC_ID_MIN+8;}
 FILE *rockflag=fopen("open-rock.flag","r");if(rockflag){fclose(rockflag);launch(&game);story_complete(&game);game.pos=add(game.debris[0].pos,(Vec3){0,60,-430});game.yaw=0;game.pitch=-.1386f;game.speed=0;game.voice_time=game.message_time=0;page=FLIGHT;selected_target=DEBRIS_ID_MIN;}
 FILE *bodyflag=fopen("open-body.flag","r");if(bodyflag){fclose(bodyflag);launch(&game);game.pos=add(game.bodies[1].pos,(Vec3){0,0,-game.bodies[1].radius*3});game.speed=0;game.yaw=game.pitch=0;game.voice_time=game.message_time=0;story_complete(&game);page=FLIGHT;hud_mode=1;}FILE *pflag=fopen("open-planet.flag","r");if(pflag){fclose(pflag);launch(&game);game.approach=1;enter_planet(&game);page=FLIGHT;hud_mode=hud_hidden=0;game.speed=12;}
 FILE *eflag=fopen("open-eva.flag","r");if(eflag){fclose(eflag);launch(&game);game.approach=1;enter_planet(&game);{Vec3 pad=surface_site(&game,1);game.pos=add(pad,(Vec3){0,18,0});game.speed=8;land_planet(&game);eva_toggle(&game);}page=FLIGHT;hud_mode=hud_hidden=0;}
 FILE *mflag=fopen("open-missions.flag","r");if(mflag){fclose(mflag);game.credits=20000;accept_mission(&game,0);accept_mission(&game,2);change_page(MISSIONS);}
 FILE *lflag=fopen("open-log.flag","r");if(lflag){fclose(lflag);game.credits=20000;accept_mission(&game,0);accept_mission(&game,2);change_page(MISSIONLOG);}
 int dump_native=0,audit_all=0;FILE *dflag=fopen("dump-native.flag","r");if(dflag){fclose(dflag);dump_native=1;}FILE *aflag=fopen("audit-all.flag","r");if(aflag){fclose(aflag);audit_all=1;}
 FILE *radioflag=fopen("open-radio.flag","r");if(radioflag){fclose(radioflag);game.voice_time=0;story_complete(&game);change_page(RADIO);}audio_init();
 unsigned previous=0;int frames=0,frame_samples=0,slow_frames=0,scene_frames[43]={0};double frame_seconds=0,scene_seconds[43]={0};float worst_frame=0;uint64_t last,now;sceRtcGetCurrentTick(&last);float frequency=(float)sceRtcGetTickResolution();
 while(running){
  if(suspend_requested){suspend_requested=0;audio_prepare_suspend();}
  if(resume_requested){resume_requested=0;runtime_recover_from_sleep();previous=0;sceRtcGetCurrentTick(&last);}
  sceRtcGetCurrentTick(&now);float raw_dt=(now-last)/frequency,dt=raw_dt;last=now;if(smoke&&frames>30&&raw_dt<.25f){int scene=frames/10;if(scene>42)scene=42;frame_seconds+=raw_dt;frame_samples++;scene_seconds[scene]+=raw_dt;scene_frames[scene]++;if(raw_dt>worst_frame)worst_frame=raw_dt;if(raw_dt>.025f)slow_frames++;}if(dt>.05f)dt=.05f;if(dt<.001f)dt=.001f;
  SceCtrlData pad={0};pad.Lx=pad.Ly=128;
  int valid=sceCtrlPeekBufferPositive(&pad,1)>0;
  if(!valid){pad.Buttons=0;pad.Lx=pad.Ly=128;}
  unsigned pressed=pad.Buttons&~previous;previous=pad.Buttons;
  int was_ready=analog_ready,ready=stick_ready(valid,pad.Lx,pad.Ly,&analog_ready);
  if(ready&&!was_ready){analog_center_x=pad.Lx;analog_center_y=pad.Ly;}
  if(ready&&!(pad.Buttons&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT|PSP_CTRL_UP|PSP_CTRL_DOWN))&&abs((int)pad.Lx-analog_center_x)<18&&abs((int)pad.Ly-analog_center_y)<18){analog_center_x=(analog_center_x*15+pad.Lx)/16;analog_center_y=(analog_center_y*15+pad.Ly)/16;}
  unsigned steer=pad.Buttons;if(page==FLIGHT&&(pad.Buttons&PSP_CTRL_SQUARE))steer&=~(PSP_CTRL_UP|PSP_CTRL_DOWN|PSP_CTRL_LEFT|PSP_CTRL_RIGHT);
  float ax,ay;steering_axes_centered(valid,steer,pad.Lx,pad.Ly,analog_enabled&&ready,analog_center_x,analog_center_y,&ax,&ay);
  if(!paused)preview_time+=dt;
  input(pressed,pad.Buttons,dt,ax,ay);
  if(game.voice_time>0){game.voice_time-=dt;if(game.voice_time<0)game.voice_time=0;}
  if(page!=FLIGHT&&!paused){game.message_time-=dt;if(game.message_time<0)game.message_time=0;}
  if(game.cue){if(!quiet_comms||game.cue!=SFX_COMM)audio_play(game.cue);game.cue=0;}
  audio_duck=(!quiet_comms&&game.voice_time>0)||game.police_stop;audio_scene_set(game.planet>=0?1:(game.attacked>0||game.incoming_missile>0)?2:game.docked||page!=FLIGHT?3:0);
  if(smoke&&frames==20){suspend_requested=1;resume_requested=1;}
  if(smoke&&frames==21){suspend_requested=1;resume_requested=1;}
  if(smoke&&frames>0&&frames%80==0)radio_tune((frames/80)%RADIO_STATION_COUNT);
  if(smoke&&frames>0&&frames<=90&&frames%10==0){
   if(page!=HOME)input(PSP_CTRL_CIRCLE,0,.016f,0,0);
   row=frames<90?frames/10:0;
   input(PSP_CTRL_CROSS,0,.016f,0,0);
   if(frames==90)game.pos=(Vec3){0,0,2200};
  }
  if(smoke&&frames==110){page=FLIGHT;game.jump=3;game.approach=-1;}
  if(smoke&&frames==120){game.jump=0;game.approach=1;}
  if(smoke&&frames==130){game.approach=-1;game.attacked=3;}
  if(smoke&&frames==140){game.attacked=0;add_crime(&game,15);game.police_stop=1;}
  if(smoke&&frames==150){game.police_stop=0;game.dead=1;game.explosion=.8f;}
  if(smoke&&frames==160){game_init(&game);deck_reset();change_page(DEBUG);}
  if(smoke&&frames==170){launch(&game);game.cargo[0]=3;game.cargo[7]=1;change_page(MARKET);}
  if(smoke&&frames==180){game_init(&game);deck_reset();launch(&game);game.pos=(Vec3){0,0,2600};page=FLIGHT;dock(&game);}
  if(smoke&&frames==190){game.dock_stage=2;game.dock_timer=1;}
  if(smoke&&frames==200){game.dock_stage=3;game.dock_timer=.1f;}
  if(smoke&&frames==210){game_init(&game);deck_reset();game.system=0;launch(&game);page=FLIGHT;game.speed=player_ships[game.ship].speed;}
  if(smoke&&frames>=220&&frames<230){game.boost=1;game.speed=player_ships[game.ship].speed*20;}
  if(smoke&&frames==230){game_init(&game);deck_reset();change_page(DETAILS);}
  if(smoke&&frames==240){change_page(MISSIONS);}
  if(smoke&&frames==250){launch(&game);page=FLIGHT;game.speed=0;game.pos=add(game.npc[8].pos,(Vec3){600,200,-1900});Vec3 aim=norm(sub(game.npc[8].pos,game.pos));game.yaw=atan2f(aim.x,aim.z);float ap=aim.y;if(ap>1)ap=1;if(ap<-1)ap=-1;game.pitch=asinf(ap);selected_target=BODY_COUNT+1+8;}
  if(smoke&&frames==260){game_init(&game);deck_reset();game.system=19;launch(&game);page=FLIGHT;game.speed=0;}
  if(smoke&&frames==270){game_init(&game);deck_reset();launch(&game);page=FLIGHT;hud_hidden=1;game.speed=player_ships[game.ship].speed;}
  if(smoke&&frames==280){hud_hidden=0;game.contract=0;game.contract_reward=4500;game.contract_time=188;change_page(MISSIONLOG);}
  if(smoke&&frames==290){game_init(&game);deck_reset();game.systems[game.system].tech=12;change_page(EQUIP);row=7;}
  if(smoke&&frames==300){launch(&game);change_page(TARGETING);target_filter=2;target_details=1;target_refresh();}
  if(smoke&&frames==310){change_page(GALNET);galnet_tab=3;row=0;}
  if(smoke&&frames==320){game_init(&game);deck_reset();game.systems[game.system].tech=12;change_page(EQUIP);row=3;}
  if(smoke&&frames==330){game_init(&game);deck_reset();change_page(MISSIONS);row=2;}
  if(smoke&&frames==340){game_init(&game);deck_reset();game.contract=game.system;game.contract_time=300;game.contract_reward=5000;game.mission_type=MISSION_BOUNTY;game.mission_stage=0;game.docked=0;game_spawn(&game);change_page(TARGETING);target_filter=5;target_details=1;target_refresh();}
  if(smoke&&frames==350){page=FLIGHT;selected_target=target_count?target_ids[0]:0;game.incoming_missile=3.2f;game.incoming_source=game.mission_target;}
  if(smoke&&frames==360){game_init(&game);deck_reset();launch(&game);page=FLIGHT;hud_mode=hud_hidden=0;game.pos=add(game.bodies[1].pos,(Vec3){0,0,-game.bodies[1].radius-800});game.yaw=game.pitch=0;game.approach=1;enter_planet(&game);}
  if(smoke&&frames==370){Vec3 pad=surface_site(&game,1);game.pos=add(pad,(Vec3){0,18,0});game.speed=8;land_planet(&game);}
  if(smoke&&frames==380){eva_toggle(&game);}
  if(smoke&&!visual_hold&&frames==390){game.pos=game.ship_pos;eva_toggle(&game);takeoff_planet(&game);}
  if(smoke&&!visual_hold&&frames==400){leave_planet(&game);page=FLIGHT;}
  if(smoke&&!visual_hold&&frames==410){change_page(CODEX);}
  if(smoke&&!visual_hold&&frames==420){change_page(RADIO);}if(smoke&&!visual_hold&&frames==415){game_init(&game);deck_reset();launch(&game);page=FLIGHT;game.pos=game.anomaly[0].pos;analysis_scan(&game,ANOMALY_ID_MIN);}
  fb=(unsigned *)(0x44000000u+(unsigned)buffer*STRIDE*H*4);pspDebugScreenSetOffset(buffer*STRIDE*H*4);if(page!=FLIGHT||hud_mode==1)rect(0,0,W,H,BG);drawcount=0;
  switch(page){case FLIGHT:space();break;case MARKET:market_screen();break;case CHART:chart();break;case YARD:yard();break;case EQUIP:equipment();break;case STATUS:status();break;case HELP:help();break;case FACTIONS:factions();break;case LOCAL:local_system();break;case DEBUG:debug_screen();break;case COMMS:communications();break;case DETAILS:system_details();break;case MISSIONS:mission_board();break;case MISSIONLOG:mission_log();break;case TARGETING:targeting_screen();break;case GALNET:galnet_screen();break;case CODEX:codex_screen();break;case STORY:story_screen();break;case GUILD:guild_screen();break;case RADIO:radio_screen();break;case COMMS_PANEL:comms_panel();break;case INTRO:intro_screen();break;case CAMPAIGN:campaign_screen();break;case COMFORT:comfort_screen();break;case WALK:walk_screen();break;default:home();}
  if(page!=FLIGHT&&!paused&&page!=INTRO&&page!=GALNET)menu_notice();
 if(dump_native&&frames==6)dump_native_bmp("native-480x272.bmp");
  if(dump_native&&smoke&&(frames==95||frames==125||frames==205||frames==215||frames==255||frames==275||frames==355||frames==365||frames==385||frames==425)){char capture[64];snprintf(capture,sizeof(capture),"scene-%03d.bmp",frames);dump_native_bmp(capture);}
  if(dump_native&&smoke&&audit_all&&frames>=160&&frames<=420&&frames%10==5){char capture[64];snprintf(capture,sizeof(capture),"audit-%03d.bmp",frames);dump_native_bmp(capture);}
  sceDisplayWaitVblankStart();sceDisplaySetFrameBuf((void*)fb,STRIDE,PSP_DISPLAY_PIXEL_FORMAT_8888,PSP_DISPLAY_SETBUF_NEXTFRAME);buffer^=1;frames++;
  if(smoke&&!visual_hold&&frames==425){double fps=frame_seconds>0?frame_samples/frame_seconds:0;FILE *log=fopen("boot-check.txt","a");if(log){fprintf(log,"Rendered 42 scenes in 425 frames, including landing, EVA, ship compass, Codex and anomaly scan.\n");fprintf(log,"Performance: %.2f average FPS, %.2f ms worst frame, %d frames over 25 ms.\n",fps,worst_frame*1000,slow_frames);fclose(log);}FILE *perf=fopen("performance-check.txt","w");if(perf){int planet_fail=0;for(int i=36;i<=39;i++)if(scene_frames[i]&&scene_frames[i]/scene_seconds[i]<24)planet_fail=1;int fail=fps<50||planet_fail;fprintf(perf,"%s average frame rate >= 50 FPS (%.2f FPS)\n",fps>=50?"PASS":"FAIL",fps);fprintf(perf,"%s planetary flight/EVA scenes remain >= 24 FPS\n",planet_fail?"FAIL":"PASS");fprintf(perf,"INFO worst frame %.2f ms; %d frames over 25 ms\n",worst_frame*1000,slow_frames);for(int i=3;i<43;i++)if(scene_frames[i])fprintf(perf,"SCENE %02d %.2f FPS\n",i,scene_frames[i]/scene_seconds[i]);fprintf(perf,"RESULT %d failures\n",fail);fclose(perf);}running=0;}
 }
 audio_stop();
 sceKernelExitGame();return 0;
}










