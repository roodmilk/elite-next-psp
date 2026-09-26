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
static int page=0,row=0,paused=0,smoke=0,visual_hold=0,hud_hidden=0,hud_mode=0,high_contrast=0,third_person=0;
static unsigned ship_paint[16]={GOLD,CYAN,RGB(240,120,96),RGB(160,120,240),RGB(120,220,150),RGB(240,210,120),RGB(180,190,205),RGB(230,150,210),GOLD,GOLD};
static const unsigned decorator_finishes[8]={GOLD,CYAN,RGB(240,120,96),RGB(160,120,240),RGB(120,220,150),RGB(240,210,120),RGB(180,190,205),RGB(230,150,210)};
static int decorator_feedback=0;
/* 0 = Kei/Ryn campaign, 1 = Guild assignments, 2+ = accepted job slot. */
static int tracked_mission=0;
static float warp_arrival_fade=0;
/* Short, session-local onboarding mission that showcases the authored station bar. */
enum { STATION_TOUR_OFF=0, STATION_TOUR_ROUTE, STATION_TOUR_DOCK, STATION_TOUR_WALK, STATION_TOUR_BAR, STATION_TOUR_TALK, STATION_TOUR_DONE };
#define STATION_TOUR_DEST 39
static int station_tour_stage=STATION_TOUR_OFF;
static int station_tour_wrong_hub_notice=0;
static int analog_center_x=128,analog_center_y=128;
static int walk_kind=0;
static float walk_x=0,walk_z=0,walk_yaw=0;
static float walk_oxygen=100,walk_integrity=100;
static int walk_salvaged=0;
static int analog_enabled=1,analog_ready=0,fire_blocked=0;
static float preview_time=0;
static int nearby[256],near_count,chart_mode=0,chart_cursor=7,chart_zoom=1;
static int view_top(void){return game.planet>=0&&game.surface==2?28:hud_hidden?0:(hud_mode==0?0:23);}
static int buffer=0;
static void display_recover(void){
 /* After sleep the LCD/framebuffer pairing can be invalid; rebuild both planes. */
 sceDisplaySetMode(0,W,H);
 for(int b=0;b<2;b++){unsigned *plane=(unsigned*)(0x44000000u+(unsigned)b*STRIDE*H*4);for(int i=0;i<STRIDE*H;i++)plane[i]=BG;}
 fb=(unsigned*)(0x44000000u+(unsigned)buffer*STRIDE*H*4);
 pspDebugScreenInit();pspDebugScreenEnableBackColor(0);pspDebugScreenSetOffset(buffer*STRIDE*H*4);
 sceDisplayWaitVblankStart();
 /* IMMEDIATE: show the recovered plane now. NEXTFRAME here left the LCD on a stale/empty buffer. */
 sceDisplaySetFrameBuf((void*)fb,STRIDE,PSP_DISPLAY_PIXEL_FORMAT_8888,PSP_DISPLAY_SETBUF_IMMEDIATE);
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
static int view_bot(void){return game.planet>=0&&game.surface==2?239:hud_hidden?H-1:(hud_mode==0?191:247);}
enum { HOME,FLIGHT,MARKET,CHART,YARD,EQUIP,STATUS,HELP,FACTIONS,LOCAL,DEBUG,COMMS,DETAILS,MISSIONS,MISSIONLOG,TARGETING,GALNET,CODEX,STORY,GUILD,RADIO,COMMS_PANEL,INTRO,CAMPAIGN,COMFORT,WALK,INVENTORY,REPAIR,DECORATOR };
#include "deck-nav.h"
static int pip_sel=1,comms_rescue_confirm=0,abandon_confirm=0,faction_lore_card=0,sell_confirm_slot=-1;
static int comms_quick=0,comms_quick_choice=0;
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
#include "decorator-monitor-art.h"
static void warp_arrival_overlay(void){
 if(warp_arrival_fade<=0)return;
 int top=view_top(),bot=view_bot();float reveal=1.f-warp_arrival_fade;int keep=35+(int)(65.f*reveal);
 for(int y=top;y<=bot;y++)for(int x=0;x<W;x++){unsigned c=fb[y*STRIDE+x];int r=c&255,g=(c>>8)&255,b=(c>>16)&255;fb[y*STRIDE+x]=RGB((r*keep+8*(100-keep))/100,(g*keep+16*(100-keep))/100,(b*keep+42*(100-keep))/100);}
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
#include "native-art-scenes.h"
#include "space-animation-kit.h"
static void draw_text_pixels(int px,int py,unsigned c,const char *buf){
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
static void text(int x,int y,unsigned c,const char *fmt,...){
 char buf[128];va_list args;va_start(args,fmt);vsnprintf(buf,sizeof(buf),fmt,args);va_end(args);
 draw_text_pixels(x*8,y*8,c,buf);
}
static void text_px(int x,int y,unsigned c,const char *fmt,...){
 char buf[128];va_list args;va_start(args,fmt);vsnprintf(buf,sizeof(buf),fmt,args);va_end(args);
 draw_text_pixels(x,y,c,buf);
}
/* Word-wrap into a fixed column width. Returns rows used; leftover returns via *left. */
static int text_wrap(int col,int rowy,int cols,int max_rows,unsigned ink,const char *s,const char **left){
 int used=0;if(cols<4)cols=4;if(left)*left=s;
 for(;used<max_rows&&s&&*s;used++){
  int len=(int)strlen(s),cut=len<cols?len:cols;
  if(len>cols){int k;for(k=cut;k>cols/3;k--)if(s[k]==' '){cut=k;break;}}
  text(col,rowy+used,ink,"%.*s",cut,s);s+=cut;while(*s==' ')s++;
 }
 if(left)*left=s;
 return used;
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
static void shipmesh_preview_edges(int id,Vec3 pos,float yaw,float roll,float scale,unsigned ink){
 const Mesh *m=&meshes[id];
 for(int i=0;i<m->triangles;i++){
  const MeshTri *t=&m->t[i];int corners[3]={t->a,t->b,t->c};
  for(int e=0;e<3;e++){
   Vec3 a=add(pos,mul(rotate(m->v[corners[e]],yaw,roll),scale));
   Vec3 b=add(pos,mul(rotate(m->v[corners[(e+1)%3]],yaw,roll),scale));
   if(a.z<15||b.z<15)continue;
   Point pa=project(a),pb=project(b);line((int)pa.x,(int)pa.y,(int)pb.x,(int)pb.y,ink);
  }
 }
}
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
/* Art kit chrome — charcoal + ochre/cream rules; cyan stays a nav signal only. */
static void header(const char *title){rect(0,0,W,22,RGB(21,28,39));rect(0,0,W,1,RGB(193,139,77));rect(0,21,W,1,RGB(85,212,212));rect(0,0,3,22,RGB(240,180,91));draw_next_art(next_logo_small,100,18,5,2,100,18);text(14,1,RGB(155,154,165),"/");text(16,1,RGB(229,210,163),"%.42s",title);}
/* One tiny, shared PSP button alphabet. Keep these 10x10 so every prompt
 * aligns on the same baseline, whether it is a face button or a direction. */
static unsigned button_ink(char b,unsigned fallback){
 if(b=='X')return RGB(80,220,110);       /* Cross */
 if(b=='O')return RGB(240,72,82);        /* Circle */
 if(b=='T')return RGB(92,164,244);       /* Triangle */
 if(b=='S')return RGB(236,112,188);      /* Square */
 if(b=='U'||b=='D'||b=='L'||b=='R'||b=='P'||b=='l'||b=='r'||b=='N')return RGB(229,210,163);
 if(b=='A'||b=='E')return RGB(85,212,212);/* Start / Select */
 return fallback;
}
static void mini_letter(int x,int y,char ch,unsigned ink){
 static const unsigned char blank[5]={0,0,0,0,0};
 static const unsigned char S[5]={7,4,7,1,7},T[5]={7,2,2,2,2},E[5]={7,4,6,4,7},L[5]={4,4,4,4,7},R[5]={6,5,6,5,5};
 const unsigned char *rows=blank;
 if(ch=='S')rows=S;else if(ch=='T')rows=T;else if(ch=='E')rows=E;else if(ch=='L')rows=L;else if(ch=='R')rows=R;
 for(int yy=0;yy<5;yy++)for(int xx=0;xx<3;xx++)if(rows[yy]&(1<<(2-xx)))rect(x+xx,y+yy,1,1,ink);
}
static void mini_label_button(int x,int y,const char *label){
 unsigned black=RGB(4,7,10),white=RGB(242,242,236);
 /* A tiny rounded-rectangle silhouette, like the real PSP Start/Select keys. */
 rect(x+2,y+1,6,8,black);rect(x+1,y+2,8,6,black);
 int len=(int)strlen(label),start=x+5-(len*2);
 for(int i=0;i<len;i++)mini_letter(start+i*4,y+3,label[i],white);
}
static void button_icon(int x,int y,char b,unsigned fallback){
 unsigned c=button_ink(b,fallback);
 if(b=='O'){circle(x+5,y+5,3,c);}
 else if(b=='X'){line(x+2,y+2,x+8,y+8,c);line(x+8,y+2,x+2,y+8,c);}
 else if(b=='T'){line(x+1,y+7,x+7,y+7,c);line(x+1,y+7,x+4,y+1,c);line(x+7,y+7,x+4,y+1,c);}
 else if(b=='S'){rect(x+2,y+2,6,6,c);rect(x+4,y+4,2,2,RGB(13,20,28));}
 else if(b=='U'||b=='D'||b=='L'||b=='R'){
  int cx=x+5,cy=y+5;
  if(b=='U'){line(cx,cy-3,cx,cy+3,c);line(cx,cy-3,cx-2,cy-1,c);line(cx,cy-3,cx+2,cy-1,c);}
  if(b=='D'){line(cx,cy-3,cx,cy+3,c);line(cx,cy+3,cx-2,cy+1,c);line(cx,cy+3,cx+2,cy+1,c);}
  if(b=='L'){line(cx-3,cy,cx+3,cy,c);line(cx-3,cy,cx-1,cy-2,c);line(cx-3,cy,cx-1,cy+2,c);}
  if(b=='R'){line(cx-3,cy,cx+3,cy,c);line(cx+3,cy,cx+1,cy-2,c);line(cx+3,cy,cx+1,cy+2,c);}
 }
 else if(b=='P'){rect(x+4,y+1,2,8,c);rect(x+1,y+4,8,2,c);}
 else if(b=='l'||b=='r')mini_label_button(x,y,b=='l'?"L":"R");
 else if(b=='N'){circle(x+5,y+5,4,c);circle(x+5,y+5,2,RGB(13,20,28));rect(x+4,y+4,2,2,c);}
 else if(b=='A'||b=='E')mini_label_button(x,y,b=='A'?"ST":"SE");
}
#include "hud-pixels.h"
static int footer_token(const char *s,int n,char *icon){
 struct {const char *word;char icon;} keys[]={
  {"X",'X'},{"CROSS",'X'},{"O",'O'},{"CIRCLE",'O'},{"TRI",'T'},{"TRIANGLE",'T'},
  {"SQ",'S'},{"SQUARE",'S'},{"UP",'U'},{"DOWN",'D'},{"LEFT",'L'},{"RIGHT",'R'},
  {"L",'l'},{"R",'r'},{"D-PAD",'P'},{"NUB",'N'},{"ANALOG",'N'},{"START",'A'},{"SELECT",'E'}
 };
 for(unsigned i=0;i<sizeof(keys)/sizeof(keys[0]);i++)if((int)strlen(keys[i].word)==n&&!strncmp(s,keys[i].word,n)){*icon=keys[i].icon;return 1;}
 return 0;
}
static void footer(const char *s){
 char label[59];snprintf(label,sizeof(label),"%.58s",s);
 rect(0,248,W,24,RGB(21,28,39));rect(0,248,W,1,RGB(193,139,77));
 /* Parse complete button tokens, never letters inside NEXT, STORY or WORK. */
 for(int i=0;label[i];){
  if(label[i]==' '||label[i]=='/'||label[i]=='+'||label[i]=='|'){i++;continue;}
  int start=i;while(label[i]&&label[i]!=' '&&label[i]!='/'&&label[i]!='+'&&label[i]!='|')i++;
  char icon;if(footer_token(label+start,i-start,&icon)){
   for(int j=start;j<i;j++)label[j]=' ';
   button_icon(8+start*8,255,icon,WHITE);
  }
 }
 text(1,32,DIM,"%s",label);
}
static void credits_badge(void){
 rect(382,228,90,16,RGB(21,28,39));rect(382,228,90,1,RGB(193,139,77));
 text(48,29,GOLD,"%.1f U",game.credits*.1f);
}
static void selected_span(int y,int w){if(w<48)w=48;rect(8,y*8-2,w,12,high_contrast?RGB(58,72,88):RGB(41,54,70));rect(8,y*8-2,3,12,RGB(240,180,91));}
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
 comms_rescue_confirm=0;abandon_confirm=0;sell_confirm_slot=-1;game.boost=0;page=p;row=p==HOME?deck_last:0;game.message_time=0;
 if(p==CHART){update_nearby();if(tracked_mission==0&&game.campaign_stage>=6&&game.saga_step&&game.saga_chapter<SAGA_COUNT)chart_cursor=game.saga_dest;else if(game.route_goal>=0)chart_cursor=game.route_goal;else chart_cursor=game.destination;for(int i=0;i<near_count;i++)if(nearby[i]==game.destination)row=i;}
 if(p==MISSIONLOG){int max=2+game.job_n;if(tracked_mission>=max)tracked_mission=0;row=tracked_mission;}
 if(p==HOME)deck_clamp_row();
 if(p==FACTIONS)faction_lore_card=0;
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
  draw_sun_sprite((int)p.x,(int)p.y,(int)radius,b->color,b->seed,game.time,xt,yt,xb,yb);
 }
}
static void draw_portrait(int x,int y,int w,int h,int system,int role);
#include "space-fx.h"
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
 if(game.jump>0){
  if(game.jump>5){sector_background();space_fx_nebula();starfield();celestial_rims();draw_bodies();station_model();ambient_space();flush_meshes();}
  warp_effect();cockpit();return;
 }
 if(game.approach>=0){planet_prompt();cockpit();return;}
 if(game.police_stop){if(hud_mode==0)cockpit();police_dialog();return;}
 if(game.dead){death_effect();sfx_maybe_death_embers();sfx_explosion_embers_draw(1.f/60);if(hud_mode==0)cockpit();return;}
 if(game.dock_stage>=2){docking_view();if(hud_mode==0)cockpit();return;}
 sector_background();space_fx_nebula();starfield();celestial_rims();draw_bodies();sfx_planet_beauty();lens_flares();sfx_sun_canopy_wash();station_model();station_window_animation();secondary_hubs();ambient_space();sfx_travel_beauty();sfx_travel_fun();
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
 npc_engine_glow();
 /* Wave A soft-FB: densify NPC plumes using the same aft roots. */
 if(!sfx_fx_muted()){
  int top=view_top(),bot=view_bot();
  for(int i=0;i<NPC_COUNT;i++){NPC *n=&game.npc[i];if(!n->alive||occluded(n->pos))continue;float d=length(sub(n->pos,game.pos));if(d>9000)continue;
   float lateral=n->freighter?freight_extent(n).x*.38f:0;int plumes=n->freighter?2:1;
   for(int plume=0;plume<plumes;plume++){
    float offset=n->freighter?(plume?1:-1)*lateral:0;
    Vec3 rear=npc_engine_root(n,offset);Vec3 rv=camera(&game,rear);if(rv.z<25)continue;Point root=project(rv);
    sfx_engine_plume_at((int)root.x,(int)root.y,n->freighter?0:(n->cruise>80),top,bot);
    sfx_engine_plume_mask((int)root.x,(int)root.y,n->freighter?0:(n->cruise>80));
   }
  }
 }
 station_glow();
 /* Soft docking aperture lamps — additive haze, not four-point sparkles. */
 if(!sfx_fx_muted()&&game.pos.z<STATION_ENTRY_Z&&!occluded((Vec3){0,0,STATION_ENTRY_Z})){
  int top=view_top(),bot=view_bot();
  for(int i=0;i<4;i++){
   Vec3 corner=station_port_corner(i);corner.z-=2;
   Vec3 v=camera(&game,add(rotate(corner,0,station_angle(&game)),(Vec3){0,0,STATION_Z}));
   if(v.z<20)continue;Point p=project(v);
   sfx_add((int)p.x,(int)p.y,RGB(40,90,95),top,bot);
  }
 }
 station_entrance();speed_lines();engine_flare();sfx_engine_plume_player();
 if(game.damaged&&!sfx_fx_muted()){
  int top=view_top(),bot=view_bot();int damage=(int)fmaxf(0.f,fminf(100.f,100.f-game.hull));int count=3+damage/12;
  if(count>11)count=11;
  /* Smoke is a dim blended grey rather than an opaque blob. More hull loss
   * emits more particles, drifting out from the ship's forward view. */
  for(int i=0;i<count;i++){float t=game.time*(1.8f+(i%3)*.35f)+i*1.7f;int x=240+(int)(sinf(t)*((i+2)*7))-(int)(game.speed*.002f*i);int y=112+(int)(cosf(t*.83f)*((i+1)*6));if(x<8||x>471||y<top+4||y>bot-4)continue;int r=2+(i%3)+(damage>65?1:0);circle(x,y,r,RGB(48,54,62));if((i&1)==0)pixel(x+r+1,y,RGB(74,68,58));}
  if(((int)(game.time*12)&1)==0){line(430,82,438,106,RED);line(438,106,433,130,AMBER);}
 }
 if(game.boost&&!sfx_fx_muted())sfx_engine_plume_mask(240,view_bot()-8,1);
 missile_effects();
 for(int i=0;i<ANOMALY_COUNT;i++)if(game.anomaly[i].alive&&length(sub(game.anomaly[i].pos,game.pos))<=180){unsigned c=game.anomaly[i].kind?CYAN:GOLD;circle(240,110,18+(int)(sinf(game.time*4)*4),c);circle(240,110,7,c);}
 for(int i=0;i<NPC_COUNT;i++){NPC *n=&game.npc[i];if(!n->alive||npc_detailed[i]||occluded(n->pos))continue;Vec3 v=camera(&game,n->pos);if(v.z<30)continue;Point p=project(v);if(p.x<2||p.x>477||p.y<view_top()+2||p.y>view_bot()-2)continue;unsigned c=faction_colors[n->role];rect((int)p.x-1,(int)p.y-1,n->freighter?5:3,n->freighter?3:2,c);}
 freight_effects();mining_effects();tractor_beam_effect();
 for(int i=0;i<NPC_COUNT;i++){NPC *n=&game.npc[i];if(!n->alive||n->flash<=0)continue;
  Vec3 nv=camera(&game,n->pos);if(nv.z>15){Point hp=project(nv);sfx_maybe_flash_sparks((int)hp.x,(int)hp.y,n->flash,(unsigned)(i*97)^(unsigned)(game.time*40));}
  if(n->target==-1)continue;
  {Vec3 end=n->target==-2?add(game.pos,mul(forward(&game),30)):game.npc[n->target].pos;Vec3 a=camera(&game,n->pos),b=camera(&game,end);if(a.z>15&&b.z>15){Point p=project(a),q=project(b);if(p.y>view_top()&&p.y<view_bot()&&q.y>view_top()&&q.y<view_bot())line((int)p.x,(int)p.y,(int)q.x,(int)q.y,n->role==LAW?CYAN:RED);}}
 }
 for(int i=0;i<DEBRIS_COUNT;i++){Debris *d=&game.debris[i];if(!d->alive||d->flash<=0)continue;Vec3 dv=camera(&game,d->pos);if(dv.z<30||dv.z>2400)continue;Point dp=project(dv);
  int burst=(d->rock&&d->flash>.19f&&d->flash<.23f)||(!d->rock&&d->flash>.60f&&d->flash<.66f);
  if(burst)sfx_explosion_embers_spawn((int)dp.x,(int)dp.y,(unsigned)(i*131)^0xDEBu);}
 if(game.shot>.1f){line(50,view_bot(),236,110,RED);line(430,view_bot(),244,110,RED);}
 line(227,110,236,110,AMBER);line(244,110,253,110,AMBER);line(240,97,240,106,AMBER);line(240,114,240,123,AMBER);
 line(232,102,236,106,AMBER);line(244,106,248,102,AMBER);line(232,118,236,114,AMBER);line(244,114,248,118,AMBER);
 sfx_hit_sparks_draw(1.f/60);sfx_maybe_death_embers();sfx_explosion_embers_draw(1.f/60);
 /* Bloom the world once — never after cockpit glyphs (Commander: no text glow). */
 hud_postfx();
 warp_arrival_overlay();
 if(hud_mode==0)target_overlay();else if(hud_mode==1)minimal_overlay();
 warp_effect();planet_prompt();police_dialog();death_effect();
 if(third_person&&!game.dock_stage&&!game.dead&&!game.police_stop&&!game.approach){int pm=mesh_id(player_ships[game.ship].name);shipmesh(pm,add(game.pos,mul(forward(&game),260)),game.yaw+3.14159265f,game.roll,1.2f,ship_paint[game.ship],0);flush_meshes();}
 if(hud_mode==0||game.dock_stage||game.dead||game.police_stop||game.approach>=0)cockpit();
 else if(hud_mode==2)combat_alert_banner(); /* scenic: still show bottom RED ALERT */
}
#include "ship-preview.h"
#include "ui-modern.h"
#include "station-crawl.h"
#include "station-tour.h"
#include "narrative-nav.h"
#include "guild-ui.h"
#include "campaign-ui.h"
#include "radio-ui.h"
#include "comms-panel.h"
#include "intro.h"
static void walk_screen(void){
 if(walk_kind==0){sc_draw_ui();return;}
 /* Ship deck / derelict keep the open walk prototype. */
 int horizon=96+(int)(sinf(preview_time*.45f+walk_yaw)*.5f);rect(0,0,W,H,walk_kind==2?RGB(8,13,24):RGB(21,28,39));
 for(int y=horizon;y<H;y++){unsigned c=mix_rgb(RGB(41,54,70),RGB(21,28,39),(y-horizon)/(float)fmaxf(1,H-horizon));for(int x=0;x<W;x++)fb[y*STRIDE+x]=c;}
 for(int i=0;i<14;i++){int y=horizon+i*12;int inset=i*9;line(0,y,W/2-inset,horizon,RGB(41+i,54+i,70+i));line(W,y,W/2+inset,horizon,RGB(41+i,54+i,70+i));}
 line(0,horizon,W,horizon,RGB(193,139,77));
 rect(0,0,W,28,RGB(21,28,39));rect(0,26,W,2,RGB(193,139,77));
 const char *place=walk_kind==1?"SHIP / PERSONAL DECK":"DERELICT / AIRLOCK";
 text(1,0,RGB(229,210,163),"ON FOOT / %.18s",place);text(1,2,RGB(155,154,165),walk_kind==2?"NUB LOOK  D-PAD MOVE  X SALVAGE  O EXIT":"NUB LOOK  D-PAD MOVE  X INSPECT  O EXIT");
 int sway=(int)(sinf(walk_x*.04f)*2);
 rect(188+sway,horizon-8,104,78,RGB(41,54,70));rect(198+sway,horizon,84,62,RGB(21,28,39));
 rect(220+sway,horizon+18,40,24,walk_kind==2?RGB(139,75,55):RGB(193,139,77));
 text(6,(horizon+28)/8,walk_kind==2?RGB(240,180,91):RGB(85,212,212),walk_kind==2?"SALVAGE":"AIRLOCK");
 text(40,(horizon+28)/8,RGB(229,210,163),"%s",walk_kind==2?"TETHER":"DECK");
 rect(0,248,W,24,RGB(21,28,39));rect(0,248,W,2,RGB(193,139,77));
 if(walk_kind==2){rect(8,252,180,8,RGB(41,54,70));rect(8,252,(int)(1.8f*walk_oxygen),8,RGB(85,212,212));rect(8,262,180,8,RGB(70,40,38));rect(8,262,(int)(1.8f*walk_integrity),8,RGB(200,90,75));text(25,32,RGB(229,210,163),"O2 %d  SUIT %d",(int)walk_oxygen,(int)walk_integrity);}
 else text(1,32,RGB(155,154,165),"Workshop. Triangle returns to the concourse.");
}
static unsigned flight_steer_buttons(unsigned buttons){
 if(page==FLIGHT&&((buttons&PSP_CTRL_SQUARE)||(game.planet>=0&&game.surface==2)))
  buttons&=~(PSP_CTRL_UP|PSP_CTRL_DOWN|PSP_CTRL_LEFT|PSP_CTRL_RIGHT);
 return buttons;
}
static void input(unsigned pressed,unsigned held,float dt,float ax,float ay){
 static unsigned in_held=0;static int sq_arm=0,boost_was_active=0;static float square_hold=0;
 station_tour_tick();
 unsigned released=in_held&~held;in_held=held;if(page!=FLIGHT||paused||game.police_stop||game.approach>=0||game.dock_stage)sq_arm=0;
 if(!(held&PSP_CTRL_CROSS))fire_blocked=0;
 int square_ready=page==FLIGHT&&game.planet<0&&game.jump<=0&&!game.dock_stage&&!game.dead;
 if(square_ready&&(held&PSP_CTRL_SQUARE)){if(pressed&PSP_CTRL_SQUARE)square_hold=0;square_hold+=dt;if(square_hold>=.20f&&sq_arm)sq_arm=0;square_held=square_hold>=.20f;}
 else {square_hold=0;square_held=0;}
 int oldpage=page;r_tap+=dt;l_tap+=dt;if(hard_brake>0){hard_brake-=dt;if(hard_brake<0)hard_brake=0;}if(page==INTRO){intro_time+=dt;if(pressed&PSP_CTRL_CROSS){game.voice_time=0;change_page(CAMPAIGN);}else if(pressed&PSP_CTRL_START){game.voice_time=0;change_page(HOME);}else if(pressed&PSP_CTRL_TRIANGLE){if(load_game(&game,"commander.sav")){selected_target=0;autoaim=0;change_page(HOME);}else message(&game,"No saved commander. X begins your journey.");}return;}
 if(page==WALK){
  if(walk_kind==0){
   /* MacVenture station: O / TRI board ship; options list handled in sc_input. */
   if(pressed&PSP_CTRL_CIRCLE&&sc_menu==0){page=HOME;message(&game,"Back on the command deck.");return;}
   sc_input(pressed); return;
  }
  walk_yaw+=ax*dt*1.8f;float move=ay*dt*(walk_kind==2?68.f:90.f);walk_x+=sinf(walk_yaw)*move;walk_z+=cosf(walk_yaw)*move;if(walk_x>220)walk_x=220;if(walk_x<-220)walk_x=-220;if(walk_z>420)walk_z=420;if(walk_z<-80)walk_z=-80;
  if(walk_kind==2){walk_oxygen-=dt*2.2f;if(walk_x>185||walk_x<-185||walk_z>385||walk_z<-45)walk_integrity-=dt*3.f;if(walk_oxygen<=0||walk_integrity<=0){walk_oxygen=0;page=FLIGHT;message(&game,"Suit reserve exhausted. Emergency tether return.");return;}}
  if(pressed&PSP_CTRL_CIRCLE){page=walk_kind==2?FLIGHT:HOME;message(&game,walk_kind==2?"Airlock sealed. Back aboard the ship.":"Back on the command deck.");}
  else if(pressed&PSP_CTRL_TRIANGLE&&walk_kind==1){walk_kind=0;sc_built_for=-1;message(&game,"Back on the station concourse.");}
  else if(pressed&PSP_CTRL_CROSS){
   if(walk_kind==2&&!walk_salvaged){walk_salvaged=1;game.credits+=500;game.discoveries++;message(&game,"Salvage secured: relay core +500 units.");}
   else message(&game,"Workshop inspection complete.");
  }
  return;}
 if(page==FLIGHT&&!paused&&!game.police_stop&&!game.dock_stage&&game.jump<=0&&game.approach<0&&!game.dead){
  if((pressed&PSP_CTRL_TRIANGLE)&&(held&PSP_CTRL_TRIANGLE)){triangle_arm=1;triangle_hold=0;}
  if(triangle_arm){
   pressed&=~PSP_CTRL_TRIANGLE;
   if(held&PSP_CTRL_TRIANGLE){triangle_hold+=dt;if(triangle_hold>=.55f){triangle_arm=0;comms_return=FLIGHT;autoaim=0;if(encounter_requires_reply(&game)){comms_quick=1;comms_quick_choice=1;return;}change_page(COMMS_PANEL);return;}}
   else {triangle_arm=0;pressed|=PSP_CTRL_TRIANGLE;}
  }
 }else {triangle_arm=0;triangle_hold=0;}
 if(page==FLIGHT&&(pressed&PSP_CTRL_SELECT)&&(held&PSP_CTRL_LTRIGGER)){hud_mode=(hud_mode+1)%3;hud_hidden=hud_mode==2;message(&game,hud_mode==0?"HUD: full.":hud_mode==1?"HUD: minimal.":"HUD: scenic / hidden.");return;}
 if(game.police_stop){game.boost=0;autoaim=0;if(game.police_phase>=2){game_tick(&game,dt,0,0,0,0);if(!game.police_stop)change_page(HOME);return;}if(pressed&PSP_CTRL_UP){police_choice=(police_choice+2)%3;game.cue=SFX_SELECT;}if(pressed&PSP_CTRL_DOWN){police_choice=(police_choice+1)%3;game.cue=SFX_SELECT;}if(pressed&PSP_CTRL_CROSS){
  if(game.police_phase==1){if(police_choice==0)police_scan_submit(&game);else if(police_choice==1)police_scan_refuse(&game);else police_escape(&game);}
  else {if(police_choice==0)police_resolve(&game,0);else if(police_choice==1){if(police_resolve(&game,1))change_page(HOME);}else police_escape(&game);}
  if(!game.police_stop)police_choice=0;}return;}
 if(pressed&PSP_CTRL_START){if(game.dead){if(campaign_retry(&game)){selected_target=0;autoaim=0;change_page(CAMPAIGN);}else {game_init(&game);deck_reset();selected_target=0;autoaim=0;change_page(STORY);}}}
 if(page==FLIGHT&&!game.dead&&!game.police_stop&&game.jump<=0&&!game.dock_stage)paused=(held&PSP_CTRL_START)!=0;else if(paused)paused=0;
 /* Hold Start: redistribute on the existing SYS/ENG/WEP meters — no separate panel. */
 if(paused){fire_blocked=1;game.boost=0;if(pressed&PSP_CTRL_LEFT)pip_sel=(pip_sel+2)%3;if(pressed&PSP_CTRL_RIGHT)pip_sel=(pip_sel+1)%3;if(pressed&PSP_CTRL_UP)pip_shift(&game,pip_sel);if(pressed&PSP_CTRL_DOWN)pip_selected_move(&game,pip_sel,-1);return;}
 if(game.dock_stage){autoaim=0;if(game.dock_stage==1&&(pressed&PSP_CTRL_CIRCLE)){game.dock_stage=0;game.speed=0;game.boost=0;message(&game,"Docking guidance cancelled. You have control.");return;}game_tick(&game,dt,0,0,0,0);if(game.docked)change_page(HOME);return;}
 if(game.dead){game_tick(&game,dt,0,0,0,0);return;}
 if(page==FLIGHT&&game.jump>0){game.boost=0;game_tick(&game,dt,0,0,0,0);if(game.jump<=0){selected_target=0;autoaim=0;}return;}
 /* On foot owns its controls; spacecraft roll, boost and target chords never run here. */
 if(page==FLIGHT&&game.planet>=0&&game.surface==2){
  autoaim=0;sq_arm=0;hard_brake=0;r_tap=l_tap=10;
  if(pressed&PSP_CTRL_SELECT){game.boost=0;change_page(HOME);return;}
  if(pressed&PSP_CTRL_CIRCLE){eva_toggle(&game);return;}
  if(pressed&PSP_CTRL_TRIANGLE){game.yaw=atan2f(game.ship_pos.x-game.pos.x,game.ship_pos.z-game.pos.z);game.pitch=game.roll=0;message(&game,"Facing your ship. D-pad forward to return.");}
  if(pressed&PSP_CTRL_SQUARE)survey_scan(&game);
  int walk=((held&PSP_CTRL_UP)!=0)-((held&PSP_CTRL_DOWN)!=0);
  float strafe=((held&PSP_CTRL_RIGHT)!=0)-((held&PSP_CTRL_LEFT)!=0);
  if(held&PSP_CTRL_LTRIGGER){ax=strafe;ay=(float)walk;walk=0;strafe=0;}
  game_eva_tick(&game,dt,ax,ay,walk,strafe,(held&PSP_CTRL_RTRIGGER)!=0);return;
 }
 if(page==FLIGHT&&(pressed&PSP_CTRL_RTRIGGER)&&!(held&PSP_CTRL_SQUARE)){if(r_tap<.32f&&game.heat<85){game.boost=1;game.cue=SFX_BOOST;}else if(r_tap<.32f&&game.heat>=85){game.boost=1;game.cue=SFX_BOOST;message(&game,"Boost held at critical heat. Shields are taking the load.");}r_tap=0;}if(!(held&PSP_CTRL_RTRIGGER)||page!=FLIGHT||(game.planet>=0&&game.surface==1)||(held&PSP_CTRL_SQUARE))game.boost=0;
 if(page==FLIGHT&&(pressed&PSP_CTRL_LTRIGGER)&&!(held&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT|PSP_CTRL_SQUARE))){if(l_tap<.32f&&game.speed>player_ships[game.ship].speed*.35f){hard_brake=.55f;game.boost=0;game.cue=SFX_UI;message(&game,"Hard brake.");}l_tap=0;}
 if(page==FLIGHT&&game.approach>=0){game.boost=0;if(pressed&PSP_CTRL_CIRCLE){turn_back(&game);autoaim=0;}else if(pressed&PSP_CTRL_CROSS){if(enter_planet(&game))autoaim=0;}return;}
 if(page==FLIGHT&&game.planet>=0&&(pressed&PSP_CTRL_CIRCLE)){if(game.surface)eva_toggle(&game);else land_planet(&game);autoaim=0;return;}
 if(page==FLIGHT&&comms_quick){
  if(pressed&PSP_CTRL_LEFT){comms_quick_choice=0;game.cue=SFX_SELECT;}
  if(pressed&PSP_CTRL_RIGHT){comms_quick_choice=1;game.cue=SFX_SELECT;}
  if(pressed&PSP_CTRL_CROSS){
   if(comms_quick_choice==1){comms_quick=0;comms_encounter_conversation=1;encounter_respond(&game);change_page(COMMS_PANEL);}
   else {comms_quick=0;encounter_ignore(&game);}
   return;
  }
  return;
 }
 if(page==FLIGHT&&game.planet>=0&&(pressed&PSP_CTRL_TRIANGLE)){if(speech_active())speech_ok();else if(game.surface==1)takeoff_planet(&game);else if(game.surface==2)message(&game,"Board the ship before takeoff.");else leave_planet(&game);autoaim=0;return;}
 if(page==FLIGHT&&game.planet>=0&&(pressed&PSP_CTRL_SQUARE)){
  if(game.surface==2)survey_scan(&game);
  else if(game.surface==1)message(&game,"Leave the ship to scan. Press O.");
  else {Vec3 pad=surface_site(&game,1);float dx=pad.x-game.pos.x,dz=pad.z-game.pos.z;float dist=sqrtf(dx*dx+dz*dz);game.yaw=atan2f(dx,dz);game.pitch=fminf(-.12f,game.pitch);autoaim=0;char note[80];snprintf(note,sizeof(note),dist<160?"Pad under you. Slow down and press O.":"Pad %d m ahead. Slow down to land.",(int)dist);message(&game,note);}
  return;}
 if(page==FLIGHT&&game.jump<=0&&!game.dead&&!game.dock_stage&&(pressed&PSP_CTRL_LTRIGGER)&&(held&PSP_CTRL_SQUARE)){sq_arm=0;cycle_front_target();}
 else if(page==FLIGHT&&flight_target_combo(pressed,held))sq_arm=0;
 else if(page==FLIGHT&&game.jump<=0&&!game.dead&&!game.dock_stage&&(pressed&PSP_CTRL_RTRIGGER)&&(held&PSP_CTRL_SQUARE)){sq_arm=0;if(valid_target(selected_target)){autoaim=1;story_event(&game,STORY_EV_TARGET);if(selected_target==0)campaign_event(&game,CP_LOCK);}else message(&game,"Choose a target with Square and the D-pad first.");}
 else if(page==FLIGHT&&game.jump<=0&&!game.dead&&!game.dock_stage&&(pressed&PSP_CTRL_SQUARE)&&(held&PSP_CTRL_RTRIGGER)){int hostile=nearest_hostile_target();sq_arm=0;if(hostile>=0){selected_target=hostile;scan_cat=4;autoaim=1;story_event(&game,STORY_EV_TARGET);if(selected_target==0)campaign_event(&game,CP_LOCK);}else message(&game,"No hostile contacts.");}
 else if(page==FLIGHT&&game.jump<=0&&!game.dead&&!game.dock_stage&&(pressed&PSP_CTRL_SQUARE))sq_arm=1;
 if(page==FLIGHT&&game.jump<=0&&!game.dead&&!game.dock_stage&&(released&PSP_CTRL_SQUARE)&&sq_arm){
  /* Tap is the quick targeter; hold Square is the full browser. Keeping tap
   * in flight removes a redundant menu and preserves the centre view. */
  sq_arm=0;target_nearest_reticle();
 }
 else if(page==FLIGHT&&(released&PSP_CTRL_SQUARE))sq_arm=0;
 if(page==FLIGHT&&(pressed&PSP_CTRL_CROSS)&&(held&PSP_CTRL_LTRIGGER))fire_missile(&game,selected_target);
 else if(page==FLIGHT&&game.planet>=0&&(pressed&PSP_CTRL_CROSS))message(&game,"Lasers are offline in atmosphere.");
 if(page==HELP&&(pressed&PSP_CTRL_LTRIGGER)){analog_enabled=!analog_enabled;analog_ready=0;message(&game,analog_enabled?"Centre the nub to enable analog steering.":"D-pad steering. Analog input ignored.");ax=ay=0;}
  if(page==FLIGHT){if(pressed&PSP_CTRL_SELECT)change_page(HOME);else if(pressed&PSP_CTRL_TRIANGLE){if(speech_active()&&game.encounter_kind!=ENCOUNTER_NONE){comms_quick=1;comms_quick_choice=1;}else if(speech_active())speech_ok();else hail_target();}else if(pressed&PSP_CTRL_CIRCLE){pick_look_target();int target=selected_target;if(station_circle_ready())target=0;else if(look_target>=0)target=look_target;if(IS_NPC_ID(target)){selected_target=target;game.npc[target-BODY_COUNT-1].name_known=1;scan_cat=target_category(target);autoaim=0;message(&game,npc_is_hostile(&game.npc[target-BODY_COUNT-1])?"Hostile locked. Triangle to hail.":"Ship locked. Triangle to talk.");}else if(IS_ANOMALY_ID(target))analysis_scan(&game,target);else if(IS_DEBRIS_ID(target)){selected_target=target;scan_cat=3;autoaim=0;salvage(&game,target);}else if(target>=2&&target<=BODY_COUNT){if(approach_planet(&game,target-1)){selected_target=target;scan_cat=0;autoaim=0;}}else if(target==0){if(dock(&game)){autoaim=0;change_page(FLIGHT);}}else if(target==1)message(&game,"The sun has no landing approach.");else message(&game,"Look at a station, planet, echo, cargo, or mission target.");}}
 else {
  if(page==LOCAL)contacts_refresh();
   if(page==TARGETING){int ids[1+BODY_COUNT+NPC_COUNT+DEBRIS_COUNT+ANOMALY_COUNT];target_count=collect_scan_ids(ids,scan_cat);if(target_count>0){for(int i=0;i<target_count;i++)target_ids[i]=ids[i];}if(row>=target_count)row=0;}
  int saga_choices=game.campaign_stage>=6&&game.saga_chapter<SAGA_COUNT&&game.saga_step&&saga_beats[game.saga_chapter].kind==SAGA_CHOICE;
  int count=page==COMFORT?6:page==DECORATOR?8:page==CAMPAIGN?(tracked_mission==0&&game.campaign_stage==0?1:tracked_mission==0&&saga_coda_pending>=0?1:tracked_mission==0&&game.campaign_stage>=6&&game.saga_chapter<SAGA_COUNT&&!game.saga_step?1:saga_choices?3:tracked_mission>=2?2:1):page==GUILD?2:page==STORY?(game.story<STORY_FREE?2:1):page==COMMS_PANEL?(comms_encounter_conversation?3:(game.encounter_kind!=ENCOUNTER_NONE&&game.encounter>0?2:17)):page==RADIO?2:page==HOME?DECK_ITEMS:page==MISSIONS?mission_count(&game):page==MISSIONLOG?2+game.job_n:page==DEBUG?11:page==LOCAL?contact_count:page==TARGETING?target_count:page==GALNET?galnet_rows():page==MARKET?cargo_rows():page==CHART?near_count:page==YARD?player_ship_count:page==EQUIP?equip_row_count():page==INVENTORY?6:page==FACTIONS?FACTION_COUNT:page==DETAILS?(1+BODY_COUNT):page==CODEX?codex_rows():1;
  if(page==MARKET&&!game.docked&&(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT))){message(&game,"Dock to buy or sell. Market controls are locked.");game.cue=SFX_UI;return;}
  if(page==REPAIR)count=1;
  if(page==COMMS_PANEL&&!comms_encounter_conversation&&!encounter_requires_reply(&game))count=17;
  if(count<1)count=1;
  if(pressed&(PSP_CTRL_UP|PSP_CTRL_DOWN))game.cue=SFX_SELECT;
  if(page==INVENTORY&&(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT|PSP_CTRL_UP|PSP_CTRL_DOWN))){
   int col=row%3, rslot=row/3;
   if(pressed&PSP_CTRL_LEFT)col=(col+2)%3; if(pressed&PSP_CTRL_RIGHT)col=(col+1)%3;
   if(pressed&PSP_CTRL_UP)rslot=(rslot+1)%2; if(pressed&PSP_CTRL_DOWN)rslot=(rslot+1)%2;
   row=rslot*3+col; game.cue=SFX_SELECT;
  } else {
   if(pressed&PSP_CTRL_UP){if(page==HOME)deck_step(-1);else if(page==CHART&&chart_mode)chart_move_cursor(0,-1);else {row=(row+count-1)%count;if(page==FACTIONS)faction_lore_card=0;}}
   if(pressed&PSP_CTRL_DOWN){if(page==HOME)deck_step(1);else if(page==CHART&&chart_mode)chart_move_cursor(0,1);else {row=(row+1)%count;if(page==FACTIONS)faction_lore_card=0;}}
  }
 if(page==CHART&&chart_mode&&(pressed&PSP_CTRL_LEFT))chart_move_cursor(-1,0);
 if(page==CHART&&chart_mode&&(pressed&PSP_CTRL_RIGHT))chart_move_cursor(1,0);
 if(page==HOME&&(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT)))deck_tab(pressed&PSP_CTRL_RIGHT?1:-1);
 if(page==HELP&&(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT)))help_tab=(help_tab+(pressed&PSP_CTRL_RIGHT?1:4))%5;
   if(page==TARGETING&&(pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER))){step_scan_cat(pressed&PSP_CTRL_RTRIGGER?1:-1);target_count=collect_scan_ids(target_ids,scan_cat);row=0;}
   if(page==CHART&&chart_mode&&(pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER))){chart_zoom+=pressed&PSP_CTRL_RTRIGGER?1:-1;if(chart_zoom<1)chart_zoom=1;if(chart_zoom>4)chart_zoom=4;game.cue=SFX_SELECT;}
   if(page==GALNET&&(pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER))){galnet_tab=(galnet_tab+(pressed&PSP_CTRL_RTRIGGER?1:5))%6;row=0;}
   if(page==CODEX&&codex_scope==0&&(pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER))){codex_tab=(codex_tab+(pressed&PSP_CTRL_RTRIGGER?1:3))%4;row=0;}
   if(page==CODEX&&codex_scope==3&&(pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER))){int category=row/3;category=(category+(pressed&PSP_CTRL_RTRIGGER?1:5))%6;row=category*3;game.cue=SFX_SELECT;}
   if(pressed&PSP_CTRL_CIRCLE){if(page==INVENTORY&&sell_confirm_slot>=0){sell_confirm_slot=-1;message(&game,"Sale cancelled.");return;}if(page==CODEX&&codex_scope==3){codex_scope=0;menu_back();return;}if(page==CODEX&&codex_scope==2){codex_scope=1;row=codex_system_row_for_body(codex_system,codex_body);return;}if(page==CODEX&&codex_scope==1){codex_scope=0;int n=vis_count();row=0;for(int i=0;i<n;i++)if(vis_sys(i)==codex_system)row=i;return;}if(page==MISSIONLOG&&abandon_confirm){abandon_confirm=0;message(&game,"Abandon cancelled. Your job is safe.");return;}if((page==RADIO||page==COMMS_PANEL||page==COMFORT)&&radio_dirty&&!radio_save_settings("radio.cfg")){message(&game,"Audio settings could not be saved. Try again.");return;}if(page==COMMS_PANEL)change_page(comms_return);else if(page==TARGETING){if(game.docked)menu_back();else change_page(FLIGHT);}else if(page==GALNET)change_page(HOME);else if(page==HOME){if(!game.docked)change_page(FLIGHT);}else menu_back();}
   else if(page==COMMS&&(pressed&PSP_CTRL_SQUARE))change_page(RADIO);
   else if(page==STORY&&(pressed&PSP_CTRL_CROSS)){
    if(row==1&&game.story<STORY_FREE){story_skip(&game);change_page(HOME);}
    else {int next=game.story<STORY_FREE?story_home_row(&game):0;change_page(HOME);row=next;deck_last=next;}
   }
   else if(page==GUILD&&(pressed&PSP_CTRL_CROSS)){if(row==0)narrative_do(GUILD);else change_page(STORY);}
   else if(page==REPAIR&&(pressed&PSP_CTRL_CROSS)){repair_ship(&game);}
   else if(page==DECORATOR&&(pressed&PSP_CTRL_CROSS)){decorator_feedback=1;if(!game.docked){message(&game,"Dock at a station to repaint your ship.");return;}static const int fees[]={120,180,240,320,400,520,700,900};int finish=row<0?0:row>7?7:row;int fee=fees[finish]*10;if(ship_paint[game.ship]==decorator_finishes[finish]){message(&game,"That finish is already on your ship.");return;}if(game.credits<fee){message(&game,"Not enough units for that paint finish.");return;}game.credits-=fee;ship_paint[game.ship]=decorator_finishes[finish];message(&game,"Paint finish applied. Exterior preview updated.");}
   else if(page==DECORATOR&&(pressed&PSP_CTRL_TRIANGLE)){change_page(REPAIR);}
   else if(page==HOME&&row==23&&(pressed&PSP_CTRL_CROSS)){decorator_feedback=0;change_page(DECORATOR);}
   else if(page==HOME&&row==24&&(pressed&PSP_CTRL_CROSS)){change_page(REPAIR);}
   else if(page==HOME&&(pressed&PSP_CTRL_CROSS)){if(row==20){if(game.docked){walk_kind=0;walk_x=walk_z=walk_yaw=0;sc_built_for=-1;page=WALK;message(&game,"Station deck. U/D options, X do, TRI ship.");}else message(&game,"Dock first to walk the station deck.");return;}if(row==21){change_page(INVENTORY);return;}if(row==22){codex_scope=3;row=0;change_page(CODEX);return;}if(row==15){codex_tab=0;codex_scope=0;}int pages[]={FLIGHT,MARKET,CHART,YARD,EQUIP,STATUS,HELP,FACTIONS,TARGETING,DEBUG,COMMS,DETAILS,MISSIONS,MISSIONLOG,GALNET,CODEX,RADIO,CAMPAIGN,GUILD,COMFORT};if(row<0||row>=20)return;int opened=row,next=pages[row];if(!story_menu_ok(&game,opened)){message(&game,story_task(&game));game.cue=SFX_UI;return;}if(!game.docked&&(next==YARD||next==EQUIP||next==MISSIONS)){message(&game,"Dock at a station to open this service.");game.cue=SFX_UI;return;}if(next==FLIGHT){int leaving=game.docked;int keep=(selected_target>=0&&selected_target<=BODY_COUNT)?selected_target:0;analog_ready=0;ax=ay=0;launch(&game);if(leaving){selected_target=keep;autoaim=0;if(valid_target(keep))scan_cat=target_category(keep);}}change_page(next);story_on_open(&game,opened);if(!game.cue)game.cue=SFX_UI;}
   else if(page==CODEX&&(pressed&PSP_CTRL_CROSS)){if(codex_tab==0&&codex_scope==0){codex_system=vis_sys(row);codex_scope=1;row=0;}else if(codex_scope==1){codex_body=codex_system_body_at(codex_system,row);codex_scope=2;row=0;}game.cue=SFX_SELECT;}
   else if(page==LOCAL&&contact_count>0&&row>=0&&row<contact_count&&(pressed&(PSP_CTRL_CROSS|PSP_CTRL_TRIANGLE))){selected_target=contact_ids[row];scan_cat=target_category(selected_target);nav_body=selected_target>0&&selected_target<=BODY_COUNT?selected_target-1:-1;autoaim=(pressed&PSP_CTRL_TRIANGLE)!=0;story_event(&game,STORY_EV_TARGET);if(selected_target==0)campaign_event(&game,CP_LOCK);message(&game,"Target set.");if(!game.docked)change_page(FLIGHT);}
   else if(page==DETAILS&&(pressed&(PSP_CTRL_CROSS|PSP_CTRL_TRIANGLE))){int id=row==0?0:row;if(valid_target(id)){selected_target=id;scan_cat=target_category(id);nav_body=id>0&&id<=BODY_COUNT?id-1:-1;autoaim=(pressed&PSP_CTRL_TRIANGLE)!=0;story_event(&game,STORY_EV_TARGET);if(selected_target==0)campaign_event(&game,CP_LOCK);message(&game,id==0?"Station locked.":id==1?"Sun locked. No landing.":"Body locked.");if(!game.docked)change_page(FLIGHT);}}
   else if(page==FACTIONS&&(pressed&PSP_CTRL_CROSS)){faction_lore_card=faction_lore_card>=2?0:faction_lore_card+1;game.cue=SFX_SELECT;message(&game,faction_lore_card?"Faction channel open.":"Faction ops brief.");}
   else if(page==FACTIONS&&(pressed&PSP_CTRL_TRIANGLE)){
    int best=-1;float best_d=1e9f;for(int i=0;i<NPC_COUNT;i++)if(game.npc[i].alive&&game.npc[i].role==row){float d=length(sub(game.npc[i].pos,game.pos));if(d<best_d){best_d=d;best=i;}}
    if(best<0){message(&game,"No ships of that colour on scanner.");game.cue=SFX_UI;}
    else {selected_target=BODY_COUNT+1+best;scan_cat=target_category(selected_target);autoaim=!game.docked;message(&game,"Faction contact locked.");if(!game.docked)change_page(FLIGHT);game.cue=SFX_SELECT;}
   }
  else if(page==COMMS&&(pressed&PSP_CTRL_TRIANGLE)){comms_rescue_confirm=!comms_rescue_confirm;}else if(page==COMMS&&(pressed&PSP_CTRL_CROSS)){if(comms_rescue_confirm){if(emergency_rescue(&game)){selected_target=0;autoaim=0;change_page(HOME);}return;}if(game.docked){message(&game,"Already docked.");game.cue=SFX_UI;}else if(dock(&game)){selected_target=0;autoaim=0;change_page(FLIGHT);}}
   else if(page==CAMPAIGN&&(pressed&PSP_CTRL_SELECT)){change_page(MISSIONLOG);}
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
 else if(row==3)change_page(RADIO);
 else if(row==4)change_page(HELP);
 else if(row==5){third_person=!third_person;message(&game,third_person?"Third-person flight view on.":"Cockpit flight view on.");}
}
else if(page==CAMPAIGN&&(pressed&PSP_CTRL_CROSS)){
 if(station_tour_active()){
  station_tour_action();
 }
 else if(tracked_mission==0&&prologue_brief_locked()){
  if(prologue_brief_beat<PROLOGUE_BRIEF_BEATS-1){
   prologue_brief_echo=0;prologue_brief_beat++;
   if(prologue_brief_beat==1)game.campaign_choice=1;else if(prologue_brief_beat==2)game.campaign_choice=2;
   row=0;game.cue=SFX_SELECT;
  }else {narrative_do(CAMPAIGN);prologue_brief_beat=0;prologue_brief_echo=0;}
 }
 else if(tracked_mission==0&&game.campaign_stage==0)narrative_do(CAMPAIGN);
 else if(tracked_mission==0&&game.campaign_stage>=6){
  if(saga_coda_pending>=0){saga_coda_pending=-1;row=0;game.cue=SFX_SELECT;}
  else if(game.saga_chapter<SAGA_COUNT&&!game.saga_step){if(saga_brief_beat<SAGA_BRIEF_BEATS-1){if(!saga_brief_echo&&saga_brief_needs_echo(saga_brief_beat)){saga_brief_echo=1;row=0;game.cue=SFX_SELECT;}else{saga_brief_echo=0;saga_brief_beat++;row=0;game.cue=SFX_SELECT;}}else{saga_begin(&game);saga_brief_echo=0;}}
  else if(game.saga_chapter<SAGA_COUNT&&saga_beats[game.saga_chapter].kind==SAGA_CHOICE){game.saga_choice=row+1;saga_advance(&game);row=0;}
  else if(!saga_advance(&game)){int hops=0,hop=saga_next_hop(&game,&hops);if(hop<0){message(&game,"No route with this drive. Fit more jump range.");}else{route_clear(&game);game.destination=hop;change_page(CHART);char note[96];snprintf(note,sizeof(note),hop==game.saga_dest?"Destination selected: %s.":"Next jump: %s. Final destination: %s.",game.systems[hop].name,game.systems[game.saga_dest].name);message(&game,note);}}
 }
 else if(tracked_mission==0){if(narrative_action(CAMPAIGN)==NA_REWARD)campaign_claim(&game);else game.cue=SFX_UI;}
 else if(tracked_mission==1){if(narrative_action(GUILD)==NA_REWARD)guild_claim(&game);else game.cue=SFX_UI;}
 else {int ji=tracked_mission-2;if(row==0&&ji>=0&&ji<game.job_n)navigate_job(ji);else change_page(MISSIONLOG);}
}
else if(page==COMMS_PANEL&&(pressed&PSP_CTRL_CROSS)){
  if(comms_encounter_conversation){
   if(row==0){speak(&game,VOICE_CONTACT,"Go on, Commander. I am listening.");game.voice_role=game.voice_role>=0?game.voice_role:EXPLORERS;row=1;return;}
   if(row==1){speak(&game,VOICE_CONTACT,"The details are complicated. Check your scanner and ask again if the signal returns.");game.voice_role=game.voice_role>=0?game.voice_role:EXPLORERS;row=0;return;}
   speech_ok();comms_encounter_conversation=0;encounter_ignore(&game);change_page(comms_return);return;
  }
  if(encounter_requires_reply(&game)){if(row==0){comms_encounter_conversation=1;encounter_respond(&game);}else{encounter_ignore(&game);comms_encounter_conversation=0;change_page(comms_return);}return;}
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
  else if(row==11){hud_mode=(hud_mode+1)%3;hud_hidden=hud_mode==2;message(&game,hud_mode==0?"HUD: full.":hud_mode==1?"HUD: minimal.":"HUD: scenic.");}
  else if(row==12){quiet_comms=!quiet_comms;radio_dirty=1;speech_ok();message(&game,quiet_comms?"Text chatter muted. Safety alerts remain.":"Text chatter restored.");}
  else if(row==13){high_contrast=!high_contrast;message(&game,high_contrast?"High contrast focus on.":"High contrast focus off.");}
  else if(row==14){change_page(RADIO);}
  else if(row==15){change_page(HELP);}
  else if(row==16){third_person=!third_person;message(&game,third_person?"Third-person preview on.":"Third-person preview off.");}
 }
 else if(page==GALNET&&(galnet_tab==3||galnet_tab==4)){if(galnet_tab==3&&(pressed&PSP_CTRL_CROSS))spacebook_likes[game.system]^=1u<<row;if(pressed&PSP_CTRL_TRIANGLE)spacebook_comments=!spacebook_comments;}
 else if(page==RADIO){if(pressed&PSP_CTRL_TRIANGLE){radio_off=!radio_off;if(radio_off)radio_static_ms=0;radio_dirty=1;}else if(pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER)){int dir=pressed&PSP_CTRL_RTRIGGER?1:-1;int next=radio_off?-1:radio_station;next+=dir;if(next<-1)next=-1;if(next>=RADIO_STATION_COUNT)next=RADIO_STATION_COUNT-1;if(next<0){radio_off=1;radio_static_ms=6;radio_dirty=1;}else{radio_off=0;radio_tune(next);radio_static_ms=8;}}else if(row<2&&(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT))){radio_adjust(row==1,pressed&PSP_CTRL_RIGHT?1:-1);}}else if(page==DEBUG&&(pressed&PSP_CTRL_CROSS)){debug_action();}
  else if(page==MARKET&&game.docked){if(pressed&PSP_CTRL_RIGHT)trade(&game,row,1);if(pressed&PSP_CTRL_LEFT)trade(&game,row,0);}
  else if(page==CHART){
   if(pressed&PSP_CTRL_TRIANGLE){chart_mode=!chart_mode;if(chart_mode){if(tracked_mission==0&&game.campaign_stage>=6&&game.saga_step&&game.saga_chapter<SAGA_COUNT)chart_cursor=game.saga_dest;else if(game.route_goal>=0)chart_cursor=game.route_goal;else chart_cursor=game.destination;}message(&game,chart_mode?"Galaxy overview: all 256 systems.":"Nearby jump list.");}
   else if(chart_mode&&(pressed&PSP_CTRL_CROSS)){Game route=game;route.fuel=(float)player_ships[game.ship].range;int jumps=0,hop=route_next_hop(&route,chart_cursor,&jumps);if(hop<0)message(&game,"No route with the fitted jump drive.");else{route_set_goal(&game,chart_cursor);game.destination=hop;chart_mode=0;update_nearby();row=0;for(int i=0;i<near_count;i++)if(nearby[i]==hop)row=i;char note[96];snprintf(note,sizeof(note),"Route to %s: %d jump%s. Next: %s.",game.systems[chart_cursor].name,jumps,jumps==1?"":"s",game.systems[hop].name);message(&game,note);}}
   else {if(near_count)game.destination=nearby[row];if(pressed&PSP_CTRL_CROSS){if(near_count&&distance_ly(&game,game.system,game.destination)*10<=game.fuel+.01f){if(game.docked)launch(&game);if(jump_start(&game)){selected_target=0;autoaim=0;change_page(FLIGHT);}}else message(&game,"Refuel first. That star is out of range.");}}
  }
  else if(page==YARD&&(pressed&PSP_CTRL_CROSS)){if(!game.docked)message(&game,"Dock to exchange ships.");else buy_ship(&game,row);}
  else if(page==EQUIP&&(pressed&PSP_CTRL_SQUARE)){int list[EQUIP_COUNT],n=equipment_stock_list(list,EQUIP_COUNT);if(row>=0&&row<n)sell_equipment_row(list[row]);}
  else if(page==EQUIP&&(pressed&PSP_CTRL_CROSS)){int list[EQUIP_COUNT],n=equipment_stock_list(list,EQUIP_COUNT);if(row>=0&&row<n)buy_equipment(list[row]);}
  else if(page==INVENTORY&&game.docked&&(pressed&PSP_CTRL_CROSS)){if(row>=0&&row<FIT_SLOTS&&game.fit[row]!=FIT_EMPTY){if(sell_confirm_slot==row){sell_confirm_slot=-1;unequip_slot(row,1);}else{sell_confirm_slot=row;message(&game,"Sell this module for the shown refund? X confirm, O cancel.");}}}
  else if(page==STATUS&&game.docked){if(pressed&PSP_CTRL_CROSS)save_game(&game,"commander.sav");if(pressed&PSP_CTRL_TRIANGLE){if(load_game(&game,"commander.sav")){selected_target=0;autoaim=0;look_target=-1;}else message(&game,"Load failed, or no save found.");}if((pressed&PSP_CTRL_SQUARE)&&game.legal>0)police_pay_desk(&game);}
 }
 float turn=0,pitch=0;int throttle=0,fire=0;if(page==FLIGHT){turn=ax;pitch=ay;int rolling=(held&PSP_CTRL_LTRIGGER)&&(held&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT))&&!(held&PSP_CTRL_SQUARE);if(rolling){game.roll+=((held&PSP_CTRL_RIGHT)?1:-1)*dt*2;turn=pitch=0;game.boost=0;autoaim=0;}throttle=(held&PSP_CTRL_RTRIGGER?1:0)-(held&PSP_CTRL_LTRIGGER?1:0);if(rolling||(held&PSP_CTRL_SQUARE)||hard_brake>0)throttle=0;if(hard_brake>0){game.speed*=fmaxf(.15f,1.f-dt*5.5f);if(game.speed<40)game.speed=0;}fire=!fire_blocked&&oldpage==FLIGHT&&!game.dock_stage&&game.approach<0&&game.laser&&(held&PSP_CTRL_CROSS)!=0&&!(held&PSP_CTRL_LTRIGGER)&&game.jump<=0;if(fire&&valid_target(selected_target)&&IS_NPC_ID(selected_target))autoaim=1;align_target(dt,ax,ay);}
 if(page==FLIGHT){if(oldpage!=FLIGHT){turn=pitch=0;throttle=fire=0;}game_tick(&game,dt,turn,pitch,throttle,fire);if(boost_was_active&&!game.boost)game.roll=0;boost_was_active=game.boost;if(game.docked)change_page(HOME);}else boost_was_active=0;
}
static void input_tests(void){
 FILE *f=fopen("input-check.txt","w");if(!f)return;int failures=0;
#define INPUT_CHECK(c,n) do{int ok=(c);fprintf(f,"%s %s\n",ok?"PASS":"FAIL",n);failures+=!ok;}while(0)
#define TEST_INIT() do{game_init(&game);deck_reset();story_complete(&game);paused=0;selected_target=0;autoaim=0;scan_cat=2;tracked_mission=0;prologue_brief_beat=0;prologue_brief_echo=0;saga_brief_beat=0;saga_brief_echo=0;saga_brief_chapter=-1;saga_coda_pending=-1;}while(0)
 TEST_INIT();launch(&game);page=FLIGHT;game.pos=(Vec3){0,0,-20000};game.speed=0;for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;
 input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);INPUT_CHECK(!game.boost,"single R press does not boost");
 input(0,0,.1f,0,0);input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);INPUT_CHECK(game.boost,"double R press starts boost");
 input(0,PSP_CTRL_RTRIGGER,.016f,0,0);INPUT_CHECK(game.boost,"held R maintains boost");input(0,0,.016f,0,0);INPUT_CHECK(!game.boost,"R release cancels boost");
 TEST_INIT();launch(&game);page=FLIGHT;game.speed=player_ships[game.ship].speed;game.heat=0;hard_brake=0;l_tap=10;
 input(PSP_CTRL_LTRIGGER,PSP_CTRL_LTRIGGER,.016f,0,0);INPUT_CHECK(hard_brake<=0,"single L press does not hard-brake");
 input(0,0,.1f,0,0);input(PSP_CTRL_LTRIGGER,PSP_CTRL_LTRIGGER,.016f,0,0);INPUT_CHECK(hard_brake>0,"double L while fast starts hard brake");
 float braked=game.speed;input(0,0,.05f,0,0);INPUT_CHECK(game.speed<braked,"hard brake dumps speed quickly");
 game.heat=90;r_tap=0;input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);input(0,0,.1f,0,0);input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);INPUT_CHECK(game.boost,"critical heat still allows a boost double-tap");
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
 {int vis[6],n=deck_fill(1,vis),saw=0;for(int i=0;i<n;i++)if(vis[i]==3||vis[i]==4)saw=1;INPUT_CHECK(!saw,"undocked Ship tab omits Shipyard and Outfitting");}
 {int vis[6],n=deck_fill(2,vis),saw=0;for(int i=0;i<n;i++)if(vis[i]==12)saw=1;INPUT_CHECK(!saw,"undocked Work tab omits Mission board");}
 game.cargo[0]=2;game.cargo[7]=1;INPUT_CHECK(cargo_rows()==2&&cargo_item(1)==7,"flight inventory lists only owned cargo");contacts_refresh();INPUT_CHECK(contact_count>=BODY_COUNT+1&&contact_ids[BODY_COUNT]==BODY_COUNT,"contacts include station and every celestial body");
 page=FLIGHT;game.pos=(Vec3){0,0,-20000};float speed=game.speed;input(0,PSP_CTRL_LTRIGGER|PSP_CTRL_RIGHT,.016f,1,0);INPUT_CHECK(game.roll>0&&game.speed==speed,"L and right rolls without changing throttle");
 game.roll=3.1415926f;float yaw_before=game.yaw;input(0,PSP_CTRL_RIGHT,.016f,1,0);INPUT_CHECK(game.yaw>yaw_before,"right steering stays right-way-up after a full roll");
 TEST_INIT();game.ship=9;game.credits=10000;change_page(DECORATOR);row=2;
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(ship_paint[9]==decorator_finishes[2]&&game.credits==7600,"decorator applies chosen paint to the tenth hull at its displayed 240-unit cost");
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(game.credits==7600&&decorator_finishes[2]==RGB(240,120,96),"equipped paint is free to reselect and preset swatches stay unchanged");
 change_page(DEBUG);row=0;int cash=game.credits;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(game.credits==cash+10000,"debug adds 1000 displayed units");
  TEST_INIT();launch(&game);page=FLIGHT;hud_mode=hud_hidden=0;input(PSP_CTRL_SELECT,PSP_CTRL_SELECT|PSP_CTRL_LTRIGGER,.016f,0,0);INPUT_CHECK(hud_mode==1&&!hud_hidden&&page==FLIGHT,"L and Select selects minimal HUD");input(PSP_CTRL_SELECT,PSP_CTRL_SELECT|PSP_CTRL_LTRIGGER,.016f,0,0);INPUT_CHECK(hud_mode==2&&hud_hidden,"L and Select selects scenic HUD");input(PSP_CTRL_SELECT,PSP_CTRL_SELECT|PSP_CTRL_LTRIGGER,.016f,0,0);INPUT_CHECK(hud_mode==0&&!hud_hidden,"L and Select restores full HUD");
 TEST_INIT();game.credits=20000;game.systems[game.system].tech=12;page=EQUIP;{int list[EQUIP_COUNT],n=equipment_stock_list(list,EQUIP_COUNT),dock_row=-1,cargo_row=-1,mis_row=-1,mil_row=-1,pulse_row=-1;for(int i=0;i<n;i++){if(list[i]==4)dock_row=i;if(list[i]==10)cargo_row=i;if(list[i]==3)mis_row=i;if(list[i]==7)mil_row=i;if(list[i]==1)pulse_row=i;}INPUT_CHECK(dock_row>=0&&cargo_row>=0&&mis_row>=0,"outfitting lists dock, cargo and missile stock at high-tech hub");row=dock_row;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK((game.upgrades&1)&&game.fit[FIT_NAV]==4&&game.credits==17500,"outfitting fits docking computer into NAV and charges balance");row=cargo_row;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(cargo_capacity(&game)==player_ships[game.ship].capacity+8&&game.fit[FIT_HOLD]==10,"outfitting fits expanded cargo bay into HOLD");int missiles=game.missiles;row=mis_row;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(game.missiles==missiles+1&&game.credits==13000,"outfitting reloads one missile and charges balance");if(pulse_row>=0){row=pulse_row;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(game.fit[FIT_WPN]==1&&game.laser,"pulse laser fits WPN slot");}game.systems[game.system].economy=0;n=equipment_stock_list(list,EQUIP_COUNT);mil_row=-1;for(int i=0;i<n;i++)if(list[i]==7)mil_row=i;INPUT_CHECK(mil_row>=0,"industrial hub stocks military shield");{int before=game.credits;row=mil_row;page=EQUIP;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(game.fit[FIT_DEF]==7&&(game.upgrades&128)&&shield_regen_rate(&game)>3.0f,"military shield fits DEF and raises regen to 4.5");page=INVENTORY;row=FIT_DEF;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(game.fit[FIT_DEF]==FIT_EMPTY&&game.credits==before-equipment_costs[7]/2,"loadout X sells fitted DEF module for half price");}}
 TEST_INIT();launch(&game);page=FLIGHT;input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE,.016f,0,0);INPUT_CHECK(page==FLIGHT&&!square_held,"Square tap keeps flight active without flashing the target computer");input(0,0,.016f,0,0);INPUT_CHECK(page==FLIGHT&&valid_target(selected_target),"Square tap selects a visible target near the centre reticle");
 for(int i=0;i<NPC_COUNT;i++)if(game.npc[i].alive&&game.npc[i].role==PIRATES){game.npc[i].target=-2;break;}
 target_filter=2;target_refresh();INPUT_CHECK(target_count>0,"hostile targeting filter finds ships engaging the player");target_filter=10;target_refresh();INPUT_CHECK(target_count>=1,"anomaly filter lists rare system echoes"); galnet_tab=3;INPUT_CHECK(galnet_rows()==7,"SpaceBook provides a scrollable generated feed");galnet_tab=4;INPUT_CHECK(galnet_rows()==5,"Messages sits beside Spacebook with its own feed");
 {char author[40],body[96];galnet_tab=0;galnet_post(2,author,sizeof(author),body,sizeof(body));INPUT_CHECK(strstr(body,"cleared")&&strstr(body,"bound"),"Traffic Control names a remote traveller route");galnet_tab=3;galnet_post(6,author,sizeof(author),body,sizeof(body));INPUT_CHECK(strstr(body,"Spotted")||strstr(body,"lingering"),"Spacebook Spotters name traveller traffic");}
 change_page(HOME);row=15;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(page==CODEX,"command deck opens the Discovery Codex");codex_tab=0;INPUT_CHECK(codex_kind_count(0)>=1,"Codex Systems lists visited systems");codex_tab=1;INPUT_CHECK(codex_kind_count(1)==0,"Codex Planets starts empty before landing");game.landed_planets[game.system]=1;INPUT_CHECK(codex_kind_count(1)==1,"Codex Planets adds a world only after landing");
 TEST_INIT();game.system=0;launch(&game);page=FLIGHT;game.pos=(Vec3){123456,65432,-222222};game.yaw=game.pitch=game.roll=0;
 for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;
 game.npc[0].alive=1;game.npc[0].role=TRADERS;game.npc[0].target=-1;game.npc[0].pos=add(game.pos,(Vec3){0,0,800});
 game.npc[1].alive=1;game.npc[1].role=PIRATES;game.npc[1].target=-2;game.npc[1].pos=add(game.pos,(Vec3){200,0,900});
 selected_target=-1;game.message_time=0;input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE,.016f,0,0);input(0,0,.016f,0,0);INPUT_CHECK(selected_target==BODY_COUNT+1&&!autoaim&&game.message_time<=0,"Square tap silently selects the contact nearest the centre reticle");
 selected_target=BODY_COUNT+2;input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE,.016f,0,0);for(int i=0;i<14;i++)input(0,PSP_CTRL_SQUARE,.016f,0,0);INPUT_CHECK(square_held,"holding Square past the tap threshold opens the target computer");input(0,0,.016f,0,0);INPUT_CHECK(selected_target==BODY_COUNT+2&&!square_held,"releasing a held target computer preserves its highlighted target");
 game.pos=(Vec3){0,0,0};game.npc[0].pos=(Vec3){200,0,800};game.npc[1].pos=(Vec3){-200,0,900};
 scan_cat=2;selected_target=0;
 game.message_time=0;input(PSP_CTRL_LEFT,PSP_CTRL_SQUARE|PSP_CTRL_LEFT,.016f,0,0);INPUT_CHECK(page==FLIGHT&&IS_NPC_ID(selected_target)&&scan_cat==1,"Square+Left tabs to ships (all contacts) without opening the computer");INPUT_CHECK(game.message_time<=0,"Square targeting changes selection without opening computer chatter");
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
 INPUT_CHECK(!autoaim,"Square+D-pad highlights without turning the ship");game.message_time=0;input(PSP_CTRL_RTRIGGER,PSP_CTRL_SQUARE|PSP_CTRL_RTRIGGER,.016f,0,0);INPUT_CHECK(page==FLIGHT&&autoaim&&!game.boost,"R while holding Square locks the selected target without boosting");INPUT_CHECK(game.message_time<=0,"Square+R locks silently while the navigation computer supplies the control reference");input(0,0,.016f,0,0);INPUT_CHECK(page==FLIGHT&&autoaim,"releasing Square keeps the explicit target lock active");
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
 input(0,0,.016f,0,0); /* build station rooms */
 {int h0=sc_hot;input(PSP_CTRL_DOWN,0,.016f,0,0);INPUT_CHECK(page==WALK&&sc_hot!=h0,"station: DOWN cycles options list");}
 /* First person in the options list talks when X is pressed. */
 {
  ScHot hot[24];int hn=sc_hotspots(hot,24),person=-1;
  for(int i=0;i<hn;i++)if(hot[i].kind==SC_H_PERSON){person=i;break;}
  INPUT_CHECK(person>=0,"station: arrivals lists at least one person");
  if(person>=0){sc_hot=person;input(PSP_CTRL_CROSS,0,.016f,0,0);
   INPUT_CHECK(sc_menu==SC_MENU_TALK,"station: X on a person opens talk choices");
   input(PSP_CTRL_CROSS,0,.016f,0,0); /* pick first choice */
   INPUT_CHECK(sc_menu==SC_MENU_NONE||sc_menu==SC_MENU_SHOP||game.message_time>0||game.voice_time>0,"station: talk choice resolves");}
 }
 input(PSP_CTRL_CIRCLE,0,.016f,0,0);input(PSP_CTRL_CIRCLE,0,.016f,0,0);INPUT_CHECK(page==HOME,"station: Circle returns to the command deck");
 /* Selecting a door option walks the small room graph. */
 TEST_INIT();change_page(HOME);row=20;input(PSP_CTRL_CROSS,0,.016f,0,0);input(0,0,.016f,0,0);
 {int start=sc_room;
  ScHot hot[24];int hn=sc_hotspots(hot,24),ex=-1;
  for(int i=0;i<hn;i++)if(hot[i].kind==SC_H_EXIT&&hot[i].id!=SC_EXIT_SHIP){ex=i;break;}
  if(ex>=0){sc_hot=ex;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(sc_room!=start,"station: X on a door changes room");}
  else INPUT_CHECK(0,"station: arrivals must list at least one exit hotspot");}
 TEST_INIT();launch(&game);page=FLIGHT;game.approach=1;enter_planet(&game);{Vec3 pad=surface_site(&game,1);game.pos=add(pad,(Vec3){0,18,0});game.speed=8;land_planet(&game);eva_toggle(&game);}
 {Vec3 before=game.pos;input(0,0,.05f,0,.9f);INPUT_CHECK(game.surface==2&&length(sub(game.pos,before))<.01f&&game.pitch>0,"planet EVA: nub looks without walking");}
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
 #include "planet-approach-input-tests.h"
 #include "planet-eva-input-tests.h"
 {
  unsigned *saved_fb=fb,*pixels=malloc(STRIDE*H*sizeof(unsigned));
  INPUT_CHECK(pixels!=0,"graphics: disposable framebuffer allocated");
  if(pixels){
   fb=pixels;TEST_INIT();hud_mode=hud_hidden=0;int old_quiet=quiet_comms;quiet_comms=0;
   {
    int old_contrast=high_contrast,contained=1,gas_action=1;
    FILE *capture=fopen("dump-native.flag","r");int capture_approach=capture!=0;if(capture)fclose(capture);
    for(int kind=0;kind<2;kind++)for(int mode=0;mode<3;mode++){
     TEST_INIT();launch(&game);page=FLIGHT;game.approach=1;game.bodies[1].type=kind?GAS:OCEAN;
     hud_mode=mode;hud_hidden=mode==2;high_contrast=mode==2;
     memset(pixels,0,STRIDE*H*sizeof(unsigned));planet_prompt();
     int title=0,note=0,entry=0;
     for(int y=0;y<H;y++)for(int x=0;x<W;x++)if(pixels[y*STRIDE+x]){
      if(x<20||x>=460||y<88||y>=172)contained=0;
      if(y>=96&&y<104&&pixels[y*STRIDE+x]==RGB(240,180,91))title=1;
      if(y>=152&&y<160&&pixels[y*STRIDE+x]==RGB(155,154,165))note=1;
      if(x>=58&&x<200&&y>=120&&y<144&&pixels[y*STRIDE+x]!=RGB(21,28,39))entry=1;
     }
     contained &= title&&note;gas_action &= kind?!entry:entry;
     if(capture_approach){char path[64];space();snprintf(path,sizeof(path),"approach-%s-mode-%d.bmp",kind?"gas":"solid",mode);dump_native_bmp(path);}
    }
    INPUT_CHECK(contained,"graphics: approach title and explanation stay inside modal in all HUD modes");
    INPUT_CHECK(gas_action,"graphics: only solid worlds offer X surface flight");
    high_contrast=old_contrast;TEST_INIT();hud_mode=hud_hidden=0;
   }
   {
    TEST_INIT();launch(&game);game.approach=1;enter_planet(&game);game.pos=surface_site(&game,1);game.speed=8;land_planet(&game);eva_toggle(&game);page=FLIGHT;
    memset(pixels,0,STRIDE*H*sizeof(unsigned));drawcount=0;space();
    int filled=1;for(int y=192;y<240;y++)for(int x=0;x<W;x++)if(!pixels[y*STRIDE+x])filled=0;
    INPUT_CHECK(view_top()==28&&view_bot()==239&&filled,"EVA graphics: world fills the space between header and return strip");
    FILE *capture=fopen("eva-capture.flag","r");
    if(capture){
     fclose(capture);int old_contrast=high_contrast;
     TEST_INIT();launch(&game);game.approach=1;enter_planet(&game);
     game.pos=surface_site(&game,1);game.speed=8;land_planet(&game);eva_toggle(&game);
     page=FLIGHT;game.message_time=0;
     for(int mode=0;mode<2;mode++){high_contrast=mode;drawcount=0;space();dump_native_bmp(mode?"eva-boarding-contrast.bmp":"eva-boarding.bmp");}
     game.pos.x+=160;game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;
     game.yaw=atan2f(game.ship_pos.x-game.pos.x,game.ship_pos.z-game.pos.z);game.hazard=42;game.message_time=0;high_contrast=0;drawcount=0;space();dump_native_bmp("eva-return.bmp");
     game.pos=game.ship_pos;game.pos.y+=4;game.hazard=0;survey_scan(&game);drawcount=0;space();dump_native_bmp("eva-scan-result.bmp");
     game.pos.y+=35;eva_toggle(&game);drawcount=0;space();dump_native_bmp("eva-airborne-board.bmp");
     game.hazard=100;game.energy=35;drawcount=0;space();dump_native_bmp("eva-exposure.bmp");
     game.hazard=0;game.message_time=0;game.pitch=.75f;drawcount=0;space();dump_native_bmp("eva-look-up.bmp");
     game.pitch=-.75f;drawcount=0;space();dump_native_bmp("eva-look-down.bmp");
     change_page(HELP);help_tab=4;rect(0,0,W,H,BG);help();dump_native_bmp("eva-controls.bmp");
     high_contrast=old_contrast;
    }
    TEST_INIT();hud_mode=hud_hidden=0;
   }
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
    /* Green arrow-wrapped objective cue — top-right header with a margin. */
    const char *cue=tracked_hud_cue();char route[52];snprintf(route,sizeof(route),">> %s >>",cue);int clen=(int)strlen(route);int cols=W/8,inset=1,left=33,max=cols-inset-left;if(clen>max)clen=max;
    int start=cols-inset-clen,right_px=(start+clen)*8,seen=0,far=0,flush=0;
    unsigned ink=RGB(100,235,150);
    for(int y=0;y<8;y++)for(int x=start*8;x<right_px&&x<W;x++)if(pixels[y*STRIDE+x]==ink){seen=1;if(x>=W-32)far=1;if(x>=W-8)flush=1;}
    INPUT_CHECK(seen&&far&&!flush&&start>=left,"graphics: mission cue top-right in header with margin");
   }
   memset(pixels,0,STRIDE*H*sizeof(unsigned));add_crime(&game,15);cockpit();
   {
    int wl=wanted_level(&game),seen=0;
    for(int y=8;y<16;y++)for(int x=8;x<120;x++)if(pixels[y*STRIDE+x]==RED)seen=1;
    INPUT_CHECK(wl==3&&seen,"graphics: local wanted level sits under the system name when police may pursue");
   }
   memset(pixels,0,STRIDE*H*sizeof(unsigned));launch(&game);page=HOME;menu_space_view(246,64,218,92);
   {
    int gold=0;for(int y=65;y<150;y++)for(int x=247;x<460;x++){unsigned p=pixels[y*STRIDE+x];if(p==GOLD||p==RGB(193,139,77)||((p&255)>180&&((p>>8)&255)>130))gold=1;}
    INPUT_CHECK(gold,"graphics: Select deck top-right shows a third-person ship silhouette");
   }
   #include "menu-preview-tests.h"
   #include "station-bar-preview-tests.h"
   {
    int saved_row=row,saved_contrast=high_contrast,labels_ok=1;
    for(int contrast=0;contrast<2;contrast++)for(int slot=0;slot<6;slot++){
     high_contrast=contrast;row=slot;memset(pixels,0,STRIDE*H*sizeof(unsigned));inventory_screen();
     int glyphs=0,col=3+(slot%3)*10,baseline=7+(slot/3)*10;
     for(int yy=baseline*8;yy<baseline*8+8;yy++)for(int xx=col*8;xx<col*8+40;xx++)if(pixels[yy*STRIDE+xx]==GOLD)glyphs++;
     if(!glyphs)labels_ok=0;
     char capture[64];snprintf(capture,sizeof(capture),"loadout-slot-%d-%s.bmp",slot,contrast?"contrast":"normal");dump_native_bmp(capture);
    }
    INPUT_CHECK(labels_ok,"loadout: all six selected hull labels render visibly in both palettes");
    row=saved_row;high_contrast=saved_contrast;
   }
   {
    Job saved_jobs[MISSION_SLOTS];memcpy(saved_jobs,game.jobs,sizeof(saved_jobs));
    int saved_n=game.job_n,saved_row=row,saved_track=tracked_mission,saved_confirm=abandon_confirm,saved_contrast=high_contrast;
    game.job_n=MISSION_SLOTS;row=MISSION_SLOTS+1;tracked_mission=0;abandon_confirm=0;
    for(int i=0;i<MISSION_SLOTS;i++){memset(&game.jobs[i],0,sizeof(Job));game.jobs[i].type=MISSION_DELIVERY;game.jobs[i].dest=i;game.jobs[i].time=300;}
    int visible=1;
    for(int mode=0;mode<2;mode++){
     high_contrast=mode;memset(pixels,0,STRIDE*H*sizeof(unsigned));mission_log();
     int label=0;for(int yy=168;yy<176;yy++)for(int xx=32;xx<216;xx++)if(pixels[yy*STRIDE+xx]==WHITE)label++;
     if(!label)visible=0;
     dump_native_bmp(mode?"mission-log-full-contrast.bmp":"mission-log-full-normal.bmp");
    }
    INPUT_CHECK(visible,"mission log: fifth contract remains visible above the objective in both palettes");
    memcpy(game.jobs,saved_jobs,sizeof(saved_jobs));game.job_n=saved_n;row=saved_row;tracked_mission=saved_track;abandon_confirm=saved_confirm;high_contrast=saved_contrast;
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
   /* Soft-FB space FX kit: nebula/clouds paint the canopy; high contrast skips them. */
   {
    high_contrast=0;game.system=3;system_bodies(&game);launch(&game);page=FLIGHT;game.jump=0;game.boost=0;
    memset(pixels,0,STRIDE*H*sizeof(unsigned));sector_background();space_fx_nebula();starfield();
    int haze=0;for(int y=view_top();y<=view_bot();y++)for(int x=0;x<W;x++){unsigned c=pixels[y*STRIDE+x];if(c&&c!=BG)haze++;}
    INPUT_CHECK(haze>2000,"graphics: space-fx nebula and starfield paint the canopy");
    high_contrast=1;memset(pixels,0,STRIDE*H*sizeof(unsigned));sector_background();space_fx_nebula();space_fx_meteors();
    int plain=0;for(int y=view_top();y<=view_bot();y++)for(int x=0;x<W;x++)if(pixels[y*STRIDE+x])plain++;
    /* Base wash only — no soft nebula/meteors when high contrast is on. */
    INPUT_CHECK(plain>1000,"graphics: high contrast keeps the sector wash without decorative FX");
    high_contrast=0;sfx_fx_reset();
    memset(pixels,0,STRIDE*H*sizeof(unsigned));sfx_hit_sparks_spawn(240,120,0xABCDu);sfx_hit_sparks_draw(0);
    int sparks=0;for(int y=view_top();y<=view_bot();y++)for(int x=0;x<W;x++)if(pixels[y*STRIDE+x])sparks++;
    INPUT_CHECK(sparks>=8,"graphics: Wave A hit-spark pool paints soft-FB streaks");
    high_contrast=1;memset(pixels,0,STRIDE*H*sizeof(unsigned));sfx_hit_sparks_draw(0);
    int muted=0;for(int y=view_top();y<=view_bot();y++)for(int x=0;x<W;x++)if(pixels[y*STRIDE+x])muted++;
    INPUT_CHECK(muted==0,"graphics: high contrast mutes Wave A hit sparks");
    high_contrast=0;sfx_fx_reset();
    /* Wave B: planet bloom / specular paints soft dots around a nearby world. */
    {
     game.system=0;system_bodies(&game);launch(&game);page=FLIGHT;game.jump=0;game.boost=0;
     game.pos=add(game.bodies[1].pos,(Vec3){0,0,-game.bodies[1].radius*3.5f});
     memset(pixels,0,STRIDE*H*sizeof(unsigned));sector_background();draw_bodies();sfx_planet_beauty();
     int bloom=0;for(int y=view_top();y<=view_bot();y++)for(int x=0;x<W;x++)if(pixels[y*STRIDE+x])bloom++;
     INPUT_CHECK(bloom>400,"graphics: Wave B planet bloom/specular paints soft-FB atmosphere");
     high_contrast=1;memset(pixels,0,STRIDE*H*sizeof(unsigned));sfx_planet_beauty();sfx_travel_beauty();
     int mute=0;for(int y=view_top();y<=view_bot();y++)for(int x=0;x<W;x++)if(pixels[y*STRIDE+x])mute++;
     INPUT_CHECK(mute==0,"graphics: high contrast mutes Wave B planet/travel beauty");
     high_contrast=0;sfx_fx_reset();
    }
   }
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
 FILE *introflag=fopen("open-intro.flag","r");if(introflag){fclose(introflag);change_page(INTRO);intro_time=6;}FILE *socialflag=fopen("open-spacebook.flag","r");if(socialflag){fclose(socialflag);change_page(GALNET);galnet_tab=3;game.voice_time=0;}FILE *netflag=fopen("open-galnet.flag","r");if(netflag){int tab=0;fscanf(netflag,"%d",&tab);fclose(netflag);change_page(GALNET);galnet_tab=tab>=0&&tab<6?tab:0;row=0;game.voice_time=0;}FILE *helpflag=fopen("open-help.flag","r");if(helpflag){int tab=0;fscanf(helpflag,"%d",&tab);fclose(helpflag);change_page(HELP);help_tab=tab>=0&&tab<5?tab:0;}
 FILE *yardflag=fopen("open-yard.flag","r");if(yardflag){int ship=0;fscanf(yardflag,"%d",&ship);fclose(yardflag);change_page(YARD);row=ship>=0&&ship<player_ship_count?ship:0;game.voice_time=0;story_complete(&game);}
 FILE *decoratorflag=fopen("open-decorator.flag","r");if(decoratorflag){fclose(decoratorflag);decorator_feedback=0;change_page(DECORATOR);row=2;game.voice_time=0;story_complete(&game);}
 FILE *sflag=fopen("open-story.flag","r");if(sflag){fclose(sflag);change_page(STORY);}
 FILE *factionflag=fopen("open-factions.flag","r");if(factionflag){fclose(factionflag);change_page(FACTIONS);row=1;}FILE *comfortflag=fopen("open-comfort.flag","r");if(comfortflag){fclose(comfortflag);change_page(COMFORT);}FILE *homeflag=fopen("open-home.flag","r");if(homeflag){int item=0;fscanf(homeflag,"%d",&item);fclose(homeflag);change_page(HOME);row=item>=0&&item<DECK_ITEMS?item:0;game.voice_time=0;}FILE *codexflag=fopen("open-codex.flag","r");if(codexflag){int tab=0;fscanf(codexflag,"%d",&tab);fclose(codexflag);change_page(CODEX);codex_tab=tab>=0&&tab<4?tab:0;row=0;game.voice_time=0;}FILE *cpflag=fopen("open-campaign.flag","r");if(cpflag){int stage=0,flying=0;fscanf(cpflag,"%d %d",&stage,&flying);fclose(cpflag);if(flying)launch(&game);game.campaign_stage=stage>=0&&stage<=6?stage:0;game.voice_time=0;tracked_mission=0;change_page(CAMPAIGN);}FILE *guildflag=fopen("open-guild.flag","r");if(guildflag){int chapter=0,ready=0;fscanf(guildflag,"%d %d",&chapter,&ready);fclose(guildflag);game.guild_chapter=chapter>=0&&chapter<=4?chapter:0;game.guild_flags=ready?31:0;game.voice_time=0;tracked_mission=1;change_page(CAMPAIGN);}
 FILE *flyflag=fopen("open-flight.flag","r");if(flyflag){fclose(flyflag);launch(&game);page=FLIGHT;hud_mode=hud_hidden=0;}
 FILE *freightflag=fopen("open-freighter.flag","r");if(freightflag){int style=0,phase=0;fscanf(freightflag,"%d %d",&style,&phase);fclose(freightflag);launch(&game);story_complete(&game);NPC *n=&game.npc[8];n->freight_style=style%3;n->radius=length(freight_extent(n));if(phase){n->freight_state=phase==1?FREIGHT_ARRIVING:FREIGHT_CHARGING;n->freight_timer=phase==1?2.4f:1.5f;game.freight_gap=0;}game.pos=freight_world(n,(Vec3){1100,450,1000});Vec3 d=norm(sub(n->pos,game.pos));game.yaw=atan2f(d.x,d.z);game.pitch=asinf(d.y);game.speed=0;game.voice_time=game.message_time=0;page=FLIGHT;selected_target=NPC_ID_MIN+8;}
 FILE *rockflag=fopen("open-rock.flag","r");if(rockflag){fclose(rockflag);launch(&game);story_complete(&game);game.pos=add(game.debris[0].pos,(Vec3){0,60,-430});game.yaw=0;game.pitch=-.1386f;game.speed=0;game.voice_time=game.message_time=0;page=FLIGHT;selected_target=DEBRIS_ID_MIN;}
 FILE *bodyflag=fopen("open-body.flag","r");if(bodyflag){fclose(bodyflag);launch(&game);game.pos=add(game.bodies[1].pos,(Vec3){0,0,-game.bodies[1].radius*3});game.speed=0;game.yaw=game.pitch=0;game.voice_time=game.message_time=0;story_complete(&game);page=FLIGHT;hud_mode=1;}FILE *pflag=fopen("open-planet.flag","r");if(pflag){fclose(pflag);launch(&game);game.approach=1;enter_planet(&game);page=FLIGHT;hud_mode=hud_hidden=0;game.speed=12;}
 FILE *eflag=fopen("open-eva.flag","r");if(eflag){fclose(eflag);launch(&game);game.approach=1;enter_planet(&game);{Vec3 pad=surface_site(&game,1);game.pos=add(pad,(Vec3){0,18,0});game.speed=8;land_planet(&game);eva_toggle(&game);}page=FLIGHT;hud_mode=hud_hidden=0;}
 FILE *mflag=fopen("open-missions.flag","r");if(mflag){fclose(mflag);game.credits=20000;accept_mission(&game,0);accept_mission(&game,2);change_page(MISSIONS);}
 FILE *lflag=fopen("open-log.flag","r");if(lflag){fclose(lflag);game.credits=20000;accept_mission(&game,0);accept_mission(&game,2);change_page(MISSIONLOG);}
 int dump_native=0,audit_all=0;FILE *dflag=fopen("dump-native.flag","r");if(dflag){fclose(dflag);dump_native=1;}FILE *aflag=fopen("audit-all.flag","r");if(aflag){fclose(aflag);audit_all=1;}
 FILE *radioflag=fopen("open-radio.flag","r");if(radioflag){fclose(radioflag);game.voice_time=0;story_complete(&game);change_page(RADIO);}audio_init();
 FILE *walkflag=fopen("open-walk.flag","r");if(walkflag){int room=0;fscanf(walkflag,"%d",&room);fclose(walkflag);game.docked=1;walk_kind=0;walk_x=walk_z=walk_yaw=0;sc_built_for=-1;sc_build_map();if(room>=0&&room<SC_R_COUNT)sc_room=room;sc_verb=SC_V_LOOK;sc_hot=0;sc_menu=0;page=WALK;game.voice_time=0;}
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
  unsigned steer=flight_steer_buttons(pad.Buttons);
  float ax,ay;steering_axes_centered(valid,steer,pad.Lx,pad.Ly,analog_enabled&&ready,analog_center_x,analog_center_y,&ax,&ay);
  if(!paused)preview_time+=dt;
  float jump_before=game.jump;input(pressed,pad.Buttons,dt,ax,ay);if(jump_before>0&&game.jump<=0)warp_arrival_fade=1.f;else if(warp_arrival_fade>0)warp_arrival_fade=fmaxf(0,warp_arrival_fade-dt/1.35f);
  if(game.voice_time>0){game.voice_time-=dt;if(game.voice_time<0)game.voice_time=0;}
  if(page!=FLIGHT&&!paused){game.message_time-=dt;if(game.message_time<0)game.message_time=0;}
  if(game.cue){if(!quiet_comms||(game.cue!=SFX_COMM&&game.cue!=SFX_TALK))audio_play(game.cue);game.cue=0;}
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
  if(smoke&&!visual_hold&&frames==390){game.pos=game.ship_pos;game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;eva_toggle(&game);takeoff_planet(&game);}
  if(smoke&&!visual_hold&&frames==400){leave_planet(&game);page=FLIGHT;}
  if(smoke&&!visual_hold&&frames==410){change_page(CODEX);}
  if(smoke&&!visual_hold&&frames==420){change_page(RADIO);}if(smoke&&!visual_hold&&frames==415){game_init(&game);deck_reset();launch(&game);page=FLIGHT;game.pos=game.anomaly[0].pos;analysis_scan(&game,ANOMALY_ID_MIN);}
  fb=(unsigned *)(0x44000000u+(unsigned)buffer*STRIDE*H*4);pspDebugScreenSetOffset(buffer*STRIDE*H*4);if(page!=FLIGHT||hud_mode==1)rect(0,0,W,H,BG);drawcount=0;
 switch(page){case FLIGHT:space();break;case MARKET:market_screen();break;case CHART:chart();break;case YARD:yard();break;case EQUIP:equipment();break;case INVENTORY:inventory_screen();break;case REPAIR:repair_screen();break;case DECORATOR:decorator_screen();break;case STATUS:status();break;case HELP:help();break;case FACTIONS:factions();break;case LOCAL:local_system();break;case DEBUG:debug_screen();break;case COMMS:communications();break;case DETAILS:system_details();break;case MISSIONS:mission_board();break;case MISSIONLOG:mission_log();break;case TARGETING:targeting_screen();break;case GALNET:galnet_screen();break;case CODEX:codex_screen();break;case STORY:story_screen();break;case GUILD:guild_screen();break;case RADIO:radio_screen();break;case COMMS_PANEL:comms_panel();break;case INTRO:intro_screen();break;case CAMPAIGN:campaign_screen();break;case COMFORT:comfort_screen();break;case WALK:walk_screen();break;default:home();}
  if(page!=FLIGHT&&!paused&&page!=INTRO&&page!=GALNET&&page!=DECORATOR)menu_notice();
 if(dump_native&&frames==6)dump_native_bmp("native-480x272.bmp");
  if(dump_native&&smoke&&(frames==95||frames==125||frames==205||frames==215||frames==255||frames==275||frames==355||frames==365||frames==385||frames==425)){char capture[64];snprintf(capture,sizeof(capture),"scene-%03d.bmp",frames);dump_native_bmp(capture);}
  if(dump_native&&smoke&&audit_all&&frames>=160&&frames<=420&&frames%10==5){char capture[64];snprintf(capture,sizeof(capture),"audit-%03d.bmp",frames);dump_native_bmp(capture);}
  /* IMMEDIATE after vblank: display the frame we just finished, then draw into the other plane.
   * NEXTFRAME (2.5.4) scheduled the back buffer one frame late and left us painting the live
   * front buffer — black flash / strobing on hardware. Keep sleep recover; fix the flip mode. */
  sceDisplayWaitVblankStart();sceDisplaySetFrameBuf((void*)fb,STRIDE,PSP_DISPLAY_PIXEL_FORMAT_8888,PSP_DISPLAY_SETBUF_IMMEDIATE);buffer^=1;frames++;
  if(smoke&&!visual_hold&&frames==425){double fps=frame_seconds>0?frame_samples/frame_seconds:0;FILE *log=fopen("boot-check.txt","a");if(log){fprintf(log,"Rendered 42 scenes in 425 frames, including landing, EVA, ship compass, Codex and anomaly scan.\n");fprintf(log,"Performance: %.2f average FPS, %.2f ms worst frame, %d frames over 25 ms.\n",fps,worst_frame*1000,slow_frames);fclose(log);}FILE *perf=fopen("performance-check.txt","w");if(perf){int planet_fail=0;for(int i=36;i<=39;i++)if(scene_frames[i]&&scene_frames[i]/scene_seconds[i]<24)planet_fail=1;int fail=fps<50||planet_fail;fprintf(perf,"%s average frame rate >= 50 FPS (%.2f FPS)\n",fps>=50?"PASS":"FAIL",fps);fprintf(perf,"%s planetary flight/EVA scenes remain >= 24 FPS\n",planet_fail?"FAIL":"PASS");fprintf(perf,"INFO worst frame %.2f ms; %d frames over 25 ms\n",worst_frame*1000,slow_frames);for(int i=3;i<43;i++)if(scene_frames[i])fprintf(perf,"SCENE %02d %.2f FPS\n",i,scene_frames[i]/scene_seconds[i]);fprintf(perf,"RESULT %d failures\n",fail);fclose(perf);}running=0;}
 }
 audio_stop();
 sceKernelExitGame();return 0;
}

