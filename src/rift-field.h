/* Layered cosmic fields: bounded motes, filaments and natural mineral geometry.
 * No extra target objects, particle allocation or unbounded close-up loops. */
static void rift_glow(int x,int y,int radius,unsigned color,int top,int bot){
 if(high_contrast){if(x>=0&&x<W&&y>=top&&y<=bot)pixel(x,y,color);return;}
 for(int dy=-radius;dy<=radius;dy++)for(int dx=-radius;dx<=radius;dx++){
  int d=dx*dx+dy*dy,rr=radius*radius+1;if(d>=rr)continue;
  int k=(rr-d)*100/rr;unsigned c=RGB((color&255)*k/220,((color>>8)&255)*k/220,((color>>16)&255)*k/220);
  sun_bloom_dot(x+dx,y+dy,c,0,top,W,bot);
 }
}
static void natural_portrait(int,int,float,int,float,int,int);
static void rift_portrait(int cx,int cy,float radius,int type,float time,int top,int bot){
 if(type>=4&&type<RIFT_TYPE_COUNT){natural_portrait(cx,cy,radius,type,time,top,bot);return;}
 static const unsigned colors[][3]={{0xffb14b,0xe755ba,0xfcebb9},{0x79d9f5,0xd4d650,0xdaf8ff},{0xad50ed,0x438cfa,0xc4e7ff},{0xffb878,0xe3daba,0xfff8e0}};
 const unsigned *c=colors[type&3];if(radius<3)radius=3;if(radius>160)radius=160;
 int segments=radius<15?16:48,glow=radius<20?1:radius<60?2:3;
 /* Eight differently inclined braided lobes form a porous, asymmetric field. */
 for(int layer=0;layer<8;layer++)for(int j=0;j<segments;j++){
  float a=j*6.2831853f/segments,phase=time*(.09f+layer*.007f)+layer*.73f;
  float r=radius*(.32f+layer*.079f)*(1+.14f*sinf(a*(3+type)+phase));
  float tilt=.32f+layer*.39f;
  float u=cosf(a+phase)*r,v=sinf(a+phase)*r*(.24f+.065f*(layer%4));
  int x=cx+(int)(u*cosf(tilt)-v*sinf(tilt)),y=cy+(int)(u*sinf(tilt)+v*cosf(tilt));
  rift_glow(x,y,glow,c[layer%2],top,bot);
  if((j+layer)%13==0)rift_glow(x,y,glow+2,c[2],top,bot);
 }
 /* Sparse drifting dust outside the filaments, plus a breathing diffuse heart. */
 for(int i=0;i<36;i++){float a=i*2.399963f+time*.025f,r=radius*(.18f+(i%13)*.083f);int x=cx+cosf(a)*r,y=cy+sinf(a)*r*.77f;rift_glow(x,y,i%9?1:3,c[i%3],top,bot);}
 for(int i=0;i<9;i++){float a=i*.698f+time*.1f;rift_glow(cx+cosf(a)*radius*.13f,cy+sinf(a)*radius*.09f,glow+3,c[0],top,bot);}
}
static int rift_body_hidden(Vec3 world){
 Vec3 origin=camera_origin(&game),delta=sub(world,origin);float d2=dot(delta,delta);if(d2<1)return 0;
 for(int b=0;b<BODY_COUNT;b++){const Body*p=&game.bodies[b];if(!body_active(p))continue;Vec3 d=sub(p->pos,origin);float along=dot(d,delta);if(along<=0||along>=d2)continue;Vec3 off=sub(d,mul(delta,along/d2));if(dot(off,off)<p->radius*p->radius)return 1;}
 return 0;
}
static unsigned char rift_falloff[9][129];static int rift_falloff_ready;
static void rift_glow_assets_init(void){if(rift_falloff_ready)return;for(int r=1;r<=8;r++)for(int d=0;d<129;d++){int rr=r*r+1;rift_falloff[r][d]=d<rr?(rr-d)*255/rr:0;}rift_falloff_ready=1;}
static void __attribute__((optimize("O3"))) rift_volume_sample(Vec3 world,unsigned ink,int radius,float strength){
 int top=view_top(),bot=view_bot();
 Vec3 v=camera(&game,world);if(v.z<12)return;Point p=project(v);int x=(int)p.x,y=(int)p.y;
 if(x<-radius||x>=W+radius||y<top-radius||y>bot+radius)return;
 if(rift_body_hidden(world))return;
 if(radius<1)radius=1;if(radius>8)radius=8;rift_glow_assets_init();unsigned depth=surface_encode_depth(1.f/v.z);
 int gain=(int)(strength*256);if(gain<0)gain=0;if(gain>256)gain=256;
 int red=(ink&255)*gain>>8,green=((ink>>8)&255)*gain>>8,blue=((ink>>16)&255)*gain>>8;
 for(int yy=-radius;yy<=radius;yy++)for(int xx=-radius;xx<=radius;xx++){
  int sx=x+xx,sy=y+yy,d=xx*xx+yy*yy,rr=radius*radius+1;
  if(d>=rr||sx<0||sx>=W||sy<top||sy>bot)continue;
  if(mega_depth_valid&&depth>surface_depth[sy*W+sx])continue;
  int weight=rift_falloff[radius][d];unsigned old=fb[sy*STRIDE+sx];int r=(old&255)+(red*weight>>8),g=((old>>8)&255)+(green*weight>>8),b=((old>>16)&255)+(blue*weight>>8);
  fb[sy*STRIDE+sx]=RGB(r>255?255:r,g>255?255:g,b>255?255:b);
 }
}
#include "natural-anomaly-render.h"
static void rift_volume(Vec3 centre,int type,float time,float pixels){
 if(type>=4&&type<RIFT_TYPE_COUNT){natural_volume(centre,type,time,pixels);return;}
 static const unsigned colors[4][3]={{0x00ffb14b,0x00e755ba,0x00fcebb9},{0x0079d9f5,0x00d4d650,0x00daf8ff},{0x00ad50ed,0x00438cfa,0x00c4e7ff},{0x00ffb878,0x00e3daba,0x00fff8e0}};
 const unsigned*c=colors[type&3];int segments=pixels<15?16:32,layers=pixels<10?3:6;
 /* These filaments are world-space volumes. Camera translation reveals
  * different layers rather than rotating a flat screen-space portrait. */
 for(int layer=0;layer<layers;layer++){
  float phase=time*(.065f+layer*.009f)+layer*.83f,tilt=layer*.63f+type*.37f;
  for(int j=0;j<segments;j++){
   float a=j*6.2831853f/segments,r=(95+layer*27)*(1+.12f*sinf(a*(3+type)+phase));
   Vec3 p={cosf(a+phase)*r,sinf(a+phase)*r*(type==0?.55f:.29f),sinf(a*2+phase)*r*.22f};
   p=(Vec3){p.x*cosf(tilt)-p.y*sinf(tilt),p.x*sinf(tilt)+p.y*cosf(tilt),p.z+(layer-2.5f)*40};
   if(type==1){float y=p.y;p.y=p.z;p.z=y;}else if(type==2){p.x*=.68f;p.y*=1.3f;}
   rift_volume_sample(add(centre,p),c[layer%2],pixels<20?1:2,.33f);
   if((j+layer*3)%11==0)rift_volume_sample(add(centre,p),c[2],pixels<20?2:4,.30f);
  }
 }
 /* Soft breathing heart and depth-distributed luminous dust; bounded sample
  * radii avoid a full-screen software blur during close fly-throughs. */
 for(int k=0;k<24;k++){float a=k*2.399963f+time*.025f,r=70+(k%9)*25;Vec3 p={cosf(a)*r,sinf(a)*r*.7f,sinf(k*1.71f)*220};rift_volume_sample(add(centre,p),c[k%3],pixels<20?1:2,.24f);}
 for(int k=0;k<7;k++){float a=k*.8976f+time*.09f;Vec3 p={cosf(a)*35,sinf(a)*26,(k-3)*18.f};rift_volume_sample(add(centre,p),c[0],pixels<20?2:6,.16f+.035f*sinf(time*.8f+k));}
}
static void rift_fields(void){
 for(int i=0;i<ANOMALY_COUNT;i++){Anomaly *a=&game.anomaly[i];int type=rift_type(game.system,i);if(!natural_visible(a,type))continue;
  Vec3 v=camera(&game,a->pos);if(length(v)>24000)continue;
  float radius=fminf(480,240*(type<4?300:natural_looks[type].radius*1.6f)/fmaxf(80,v.z));
  /* Sphere visibility includes looking out from inside or past the centre.
   * Individual samples handle their own near-plane and viewport clipping. */
  rift_volume(a->pos,type,game.time+i*7,radius);
 }
}
static void rift_report_view(void){
 int slot=game.rift_report-1;if(slot<0||slot>=ANOMALY_COUNT)return;
 int type=rift_type(game.system,slot);
 rect(0,0,W,H,BG);dialogue_begin("ANALYSIS / SHIP COMPUTER");
 dialogue_speech("SHIP COMPUTER",EXPLORERS,17,rift_reports[type],rift_lore[type]);
 dialogue_context(rift_names[type],"Scan filed under Discovery Codex > system > Space Signals. This is an instrument report, not an incoming conversation.");
 footer("L/R READ   TRI CLOSE   O CLOSE");
 rift_portrait(412,201,30,type,preview_time,194,242);
}
