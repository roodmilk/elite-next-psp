/* Orbit Motors / Crown Grand. Native 480x272; authoritative hull and stock data.
 * Presentation only: purchase validation, saves and celebration remain shared. */
#define YC_BG (ship_theme_index()?ui_paint_surface(RGB(8,20,31)):RGB(8,20,31))
#define YC_INK (ship_theme_index()?UI_TEXT:RGB(241,223,183))
#define YC_DIM (ship_theme_index()?UI_MUTED:RGB(157,180,177))
#define YC_GOLD (ship_theme_index()?UI_ACCENT:RGB(233,187,114))
#define YC_LABEL (ship_theme_index()?UI_MUTED:RGB(218,179,132))
#define YC_TEAL (ship_theme_index()?UI_SIGNAL:RGB(145,213,197))
#define YC_UP RGB(140,222,139)
#define YC_DOWN RGB(242,134,120)
static void yc_box(int x,int y,int w,int h,unsigned bg,unsigned edge){rect(x,y,w,h,bg);line(x,y,x+w-1,y,edge);line(x,y+h-1,x+w-1,y+h-1,edge);line(x,y,x,y+h-1,edge);line(x+w-1,y,x+w-1,y+h-1,edge);}
static unsigned yc_hash(unsigned *v){*v^=*v>>16;*v*=0x7feb352du;*v^=*v>>15;*v*=0x846ca68bu;return *v^(*v>>16);}
static void yc_text_size(int x,int y,int size,unsigned ink,const char *s){
 for(int i=0;s[i];i++){
  unsigned ch=(unsigned char)s[i];if(ch>='a'&&ch<='z')ch-=32;if(ch<32||ch>126)ch='?';
  for(int yy=0;yy<size;yy++)for(int xx=0;xx<size;xx++)if(font8[ch-32][yy*8/size]&(128>>(xx*8/size)))pixel(x+i*size+xx,y+yy,ink);
 }
}
static void yc_name(int system,int variant,char *out,size_t cap){
 static const char *first[]={"CROWN","AURUM","NOVA","VANTA","ROYAL","SOLAR","HALO","ORION","ASTER","LUNAR","IVORY","AMBER","OPAL","SILVER","GOLD","VELVET","AETHER","NORTH","SABLE","EMBER","JADE","POLAR","PRIME","TERRA","RUBY","AZURE","ECHO","ATLAS","APEX","ULTRA","VISTA","ZENITH"};
 static const char *last[]={"GRAND","VAULT","ORBIT","SPIRE","GATE","COURT","REACH","CREST","HAVEN","CROWN","POINT","ARCADE","LOFT","SANDS","PEARL","TOWER","HALL","SUMMIT","PALACE","PORT","NEXUS","ARC","QUAY","DOMUS","HOUSE","FORGE","GARDEN","PLAZA","RING","VOW","STAR","LIGHT"};
 /* Bijective names for all 256 systems x four canonical hub identities.
    Lave's capital showroom is the approved Crown Grand reference. */
 unsigned key=(unsigned)(system*4+variant);if(key==28)key=0;else if(!key)key=28;
 snprintf(out,cap,"%s %s",first[key&31],last[(key>>5)&31]);
}
static int yc_copy(int x,int y,int cols,int rows,int step,const char *s,int draw){
 int used=0;while(s&&*s&&used<rows){int len=(int)strlen(s),cut=len<cols?len:cols;if(len>cols){int k=cut;while(k>0&&s[k]!=' ')k--;if(k)cut=k;}
  if(draw){char b[64];snprintf(b,sizeof(b),"%.*s",cut,s);yc_text_size(x,y+used*step,8,YC_DIM,b);}s+=cut;while(*s==' ')s++;used++;
 }return s&&*s?-1:used;
}
static int yc_delta(int candidate,int current){return candidate>current?1:candidate<current?-1:0;}
static void yc_arrow(int x,int y,int delta){
 if(delta>0){pixel(x+2,y,YC_UP);line(x+1,y+1,x+3,y+1,YC_UP);line(x,y+2,x+4,y+2,YC_UP);rect(x+2,y+3,1,4,YC_UP);}
 else if(delta<0){rect(x+2,y,1,4,YC_DOWN);line(x,y+4,x+4,y+4,YC_DOWN);line(x+1,y+5,x+3,y+5,YC_DOWN);pixel(x+2,y+6,YC_DOWN);}
}
static void yc_stat(int x,int y,const char *label,const char *value,int delta){
 yc_text_size(x,y,8,YC_LABEL,label);int vx=x+((int)strlen(label)+1)*8;yc_text_size(vx,y,8,YC_TEAL,value);yc_arrow(vx+(int)strlen(value)*8+3,y,delta);
}
static void yc_stats(int ship,int mega){
 const PlayerShip *p=&player_ships[ship],*o=&player_ships[game.ship];char value[16];
 const char *label[]={"CARGO","RANGE","SPEED","HULL","SHIELD","HEAT CAP"};
 int a[]={p->capacity,p->range,p->speed,p->hull,p->shields,p->heat_capacity};int b[]={o->capacity,o->range,o->speed,o->hull,o->shields,o->heat_capacity};
 for(int i=0;i<6;i++){
  if(i==0)snprintf(value,sizeof(value),"%dT",a[i]);else if(i==1)snprintf(value,sizeof(value),"%.1fLY",a[i]*.1f);else if(i==2)snprintf(value,sizeof(value),"%d",a[i]);else snprintf(value,sizeof(value),"%d%%",a[i]);
  int x=mega?200+(i%2)*136:16+(i%3)*156,y=mega?199+(i/2)*16:205+(i/3)*20;yc_stat(x,y,label[i],value,yc_delta(a[i],b[i]));
 }
}
static void yc_mega_specs(int ship){
 const PlayerShip *p=&player_ships[ship],*owned=&player_ships[game.ship];
 static const char *label[]={"CARGO","RANGE","SPEED","HULL","SHIELD","HEAT CAP"};
 static const unsigned colours[]={0x82beed,0xedbfa3,0xcbd273,0x85c1ed,0xedd77b,0x91a6ed};
 int values[]={p->capacity,p->range,p->speed,p->hull,p->shields,p->heat_capacity},current[]={owned->capacity,owned->range,owned->speed,owned->hull,owned->shields,owned->heat_capacity};
 yc_box(192,156,280,92,ui_paint_surface(RGB(12,28,38)),RGB(101,130,135));yc_text_size(200,160,8,YC_GOLD,p->name);
 if(shipyard_stats_view)for(int i=0;i<6;i++){
  int maximum=1;for(int j=0;j<player_ship_count;j++){const PlayerShip *q=&player_ships[j];int v[]={q->capacity,q->range,q->speed,q->hull,q->shields,q->heat_capacity};if(v[i]>maximum)maximum=v[i];}
  int x=200+(i%2)*136,y=174+(i/2)*15;char value[16];
  if(i==0)snprintf(value,sizeof(value),"%dT",values[i]);else if(i==1)snprintf(value,sizeof(value),"%.1fLY",values[i]*.1f);else snprintf(value,sizeof(value),i==2?"%d":"%d%%",values[i]);
  yc_text_size(x,y+1,6,YC_DIM,label[i]);yc_text_size(x+62,y,8,YC_INK,value);yc_arrow(x+64+strlen(value)*8,y,yc_delta(values[i],current[i]));
  int filled=values[i]*120/maximum,marker=current[i]*120/maximum;
  for(int k=0;k<30;k++)rect(x+k*4,y+9,3,2,k*4<filled?colours[i]:RGB(35,52,64));
  if(marker>119)marker=119;rect(x+marker,y+8,1,4,YC_GOLD);
 }else yc_copy(200,172,33,7,8,shipyard_description[ship],1);
 static const char *names[]={"WPN","DEF","NAV","HOLD","FUEL","UTIL"};
 for(int i=0;i<6;i++){
  int x=200+i*44,n=p->slots[i];unsigned ink=colours[i];yc_box(x,229,42,17,RGB(11,23,31),n<owned->slots[i]?YC_DOWN:RGB(56,83,94));
  yc_text_size(x+3,232,6,ink,names[i]);char number[8];snprintf(number,sizeof(number),"%d",n);yc_text_size(x+31,231,8,YC_INK,number);
  for(int k=0;k<8;k++)rect(x+4+k*4,241,3,2,k<n?ink:RGB(31,44,53));
 }
}
static void yc_exchange_label(int ship,int width,char *b,size_t cap){
 int cost=ship_exchange_cost(&game,ship);
 if(ship==game.ship)snprintf(b,cap,"CURRENT SHIP");else{
  snprintf(b,cap,cost<0?"YOU RECEIVE: %.1f U":"COSTS: %.1f U",(cost<0?-(double)cost:cost)*.1);
  /* Whole-unit refunds omit only the redundant .0, never real precision. */
  if((int)strlen(b)*8>width&&cost%10==0)snprintf(b,cap,cost<0?"YOU RECEIVE: %d U":"COSTS: %d U",abs(cost)/10);
 }
}
static void yc_exchange(int x,int y,int w,int h,int ship){
 char b[40];
 yc_box(x,y,w,h,ui_paint_surface(RGB(44,41,36)),YC_GOLD);yc_text_size(x+4,y+5,9,YC_GOLD,"EXCHANGE YOUR SHIP");
 yc_exchange_label(ship,w-8,b,sizeof(b));
 yc_text_size(x+4,y+h-15,8,YC_INK,b);
}
static int yc_first(int selected,int count,int visible){int first=selected-visible/2;if(first>count-visible)first=count-visible;return first<0?0:first;}
static void yc_catalogue(int x,int y,int w,int h,int visible,int ship){
 int count=shipyard_list_count(&game),first=yc_first(row,count,visible);yc_box(x,y,w,h,ui_paint_surface(RGB(17,36,45)),ui_paint_surface(RGB(67,87,99)));
 int rail=h-10,thumb=count>visible?rail*visible/count:rail;if(thumb<8)thumb=8;rect(x+4,y+5,2,rail,ui_paint_surface(RGB(49,76,89)));rect(x+4,y+5+(count>visible?(rail-thumb)*first/(count-visible):0),2,thumb,YC_GOLD);
 int step=(h-10)/visible;
 for(int j=0;j<visible&&first+j<count;j++){int id=shipyard_list_ship(&game,first+j),yy=y+7+j*step;int selected=id==ship;
  if(selected){rect(x+10,yy-2,w-15,step-2,ui_paint_surface(RGB(74,66,55)));rect(x+10,yy-2,2,step-2,YC_GOLD);}
  yc_text_size(x+16,yy,8,selected?YC_GOLD:YC_DIM,player_ships[id].name);
  if(id==game.ship)rect(x+w-8,yy+2,3,3,YC_UP);
 }
}
static Vec3 yc_ship_point(Vec3 p,float yaw,float roll){p=rotate(p,yaw,0);p=(Vec3){p.x,p.y*.82f+p.z*.57f,p.z*.82f-p.y*.57f};return rotate(p,0,roll);}
static void yc_ship_extent(Vec3 p,float yaw,float roll,Vec3 *lo,Vec3 *hi){p=yc_ship_point(p,yaw,roll);lo->x=fminf(lo->x,p.x);lo->y=fminf(lo->y,p.y);lo->z=fminf(lo->z,p.z);hi->x=fmaxf(hi->x,p.x);hi->y=fmaxf(hi->y,p.y);hi->z=fmaxf(hi->z,p.z);}
static void yc_ship_face(Vec3 a,Vec3 b,Vec3 c,unsigned paint){
 Vec3 u=sub(b,a),v=sub(c,a),normal=norm((Vec3){u.y*v.z-u.z*v.y,u.z*v.x-u.x*v.z,u.x*v.y-u.y*v.x});
 float light=.55f+.45f*fabsf(normal.x*.28f+normal.y*.76f-normal.z*.58f);
 /* Soft showroom light preserves livery without overflowing RGB channels. */
 unsigned ink=RGB((int)(((paint&255)*.68f+241*.32f)*light),(int)((((paint>>8)&255)*.68f+223*.32f)*light),(int)((((paint>>16)&255)*.68f+183*.32f)*light));
 queue_triangle(a,b,c,ink);
}
static unsigned yc_paint_override;
static void yc_ship(int ship,int x,int y,int w,int h){
 int art=mega_hull_art_id(player_ships[ship].name),id=mesh_id(player_ships[ship].name);float yaw=.55f+sinf(preview_time*.17f)*.38f,roll=.1f;
 Vec3 lo={1e9f,1e9f,1e9f},hi={-1e9f,-1e9f,-1e9f};
 if(art>=0){const MegaHullArt *a=&mega_hull_art[art];for(int i=0;i<a->parts;i++){const MegaHullPart *b=&a->part[i];for(int k=0;k<8;k++){float nose=k&4?.62f:1.f;Vec3 p={b->x+((k&1)?b->hx:-b->hx)*nose,b->y+((k&2)?b->hy:-b->hy)*(.78f+.22f*nose),b->z+((k&4)?b->hz:-b->hz)};yc_ship_extent(p,yaw,roll,&lo,&hi);}}}
 else {const Mesh*m=fleet_visual_mesh(id);for(int i=0;i<m->vertices;i++)yc_ship_extent(m->v[i],yaw,roll,&lo,&hi);}
 Vec3 centre=mul(add(lo,hi),.5f),half=mul(sub(hi,lo),.5f);float sx=(w-10)*.5f,sy=(h-10)*.5f;
 float scale=fminf(sx*320.f/(240.f*fmaxf(1,half.x)+sx*half.z),sy*320.f/(240.f*fmaxf(1,half.y)+sy*half.z));Vec3 pos=sub((Vec3){0,0,320},mul(centre,scale));
 unsigned paint=yc_paint_override?yc_paint_override:ship==game.ship?ship_paint[ship]:decorator_finishes[ship%DECORATOR_COUNT];preview_clip(x+w/2,y+h/2,x,y,x+w,y+h);
 if(art>=0){
  static const unsigned char faces[12][3]={{0,1,3},{0,3,2},{4,6,7},{4,7,5},{0,4,5},{0,5,1},{2,3,7},{2,7,6},{0,2,6},{0,6,4},{1,5,7},{1,7,3}};
  const MegaHullArt *a=&mega_hull_art[art];for(int i=0;i<a->parts;i++){const MegaHullPart *b=&a->part[i];Vec3 p[8];for(int k=0;k<8;k++){float nose=k&4?.62f:1.f;Vec3 q={b->x+((k&1)?b->hx:-b->hx)*nose,b->y+((k&2)?b->hy:-b->hy)*(.78f+.22f*nose),b->z+((k&4)?b->hz:-b->hz)};p[k]=add(pos,mul(yc_ship_point(q,yaw,roll),scale));}for(int f=0;f<12;f++){int first=drawcount;yc_ship_face(p[faces[f][0]],p[faces[f][1]],p[faces[f][2]],paint);if(drawcount==first+1)drawlist[first].material=FLEET_QUAD_BASE+(b->shade==3?FLEET_RADIATOR:b->shade==2?FLEET_GLASS:b->shade==1?FLEET_CARGO:FLEET_ARMOUR)*2+(f&1);}}
 }else{
  const Mesh *m=fleet_visual_mesh(id);Vec3 points[256];for(int i=0;i<m->vertices;i++)points[i]=add(pos,mul(yc_ship_point(m->v[i],yaw,roll),scale));
  for(int i=0;i<m->triangles;i++){const MeshTri *f=&m->t[i];int first=drawcount;yc_ship_face(points[f->a],points[f->b],points[f->c],paint);if(drawcount==first+1)drawlist[first].material=fleet_visual_material(id,i);}
 }
 qsort(drawlist,drawcount,sizeof(*drawlist),depth_sort);
 for(int i=0;i<drawcount;i++){DrawTri *t=&drawlist[i];triangle(t);}
 drawcount=0;preview_reset();
}
static void yc_tug(int x,int y,unsigned light){line(x,y+3,x+7,y,RGB(177,198,193));line(x+7,y,x+13,y+3,RGB(177,198,193));line(x,y+3,x+10,y+6,RGB(113,139,148));line(x+10,y+6,x+13,y+3,RGB(113,139,148));rect(x+4,y+2,5,3,RGB(113,139,148));rect(x+12,y+2,4,2,light);}
static void yc_crew(int x,int y){rect(x,y,2,2,RGB(238,213,172));rect(x-1,y+2,4,4,RGB(138,155,159));line(x,y+6,x-1,y+8,RGB(82,113,126));line(x+1,y+6,x+2,y+8,RGB(82,113,126));}
static void yc_world(int cx,int cy,int r,unsigned seed){
 unsigned palettes[4][3]={{RGB(33,77,86),RGB(65,124,119),RGB(143,177,144)},{RGB(63,57,93),RGB(114,91,144),RGB(185,159,181)},{RGB(83,61,42),RGB(145,102,60),RGB(206,167,109)},{RGB(32,67,101),RGB(63,120,164),RGB(149,193,198)}};
 unsigned *p=palettes[(seed>>4)&3];
 for(int yy=-r;yy<=r;yy++)for(int xx=-r;xx<=r;xx++){int rr=xx*xx+yy*yy;if(rr>r*r)continue;unsigned v=seed+(unsigned)((xx+100)*157+(yy+100)*419);yc_hash(&v);float shade=.45f+.55f*fmaxf(0,(-xx*.55f-yy*.32f+sqrtf(fmaxf(0,r*r-rr))*.75f)/r);unsigned c=p[(v%23<8)?2:(v%23<16)?1:0];pixel(cx+xx,cy+yy,RGB((int)((c&255)*shade),(int)(((c>>8)&255)*shade),(int)(((c>>16)&255)*shade)));}
 circle(cx,cy,r+1,RGB(64,100,112));
}
static void yc_grand_scene(unsigned seed,int ship){
 yc_box(8,65,464,108,RGB(9,28,43),RGB(138,115,84));unsigned v=seed;
 for(int i=0;i<240;i++){int x=11+(int)(yc_hash(&v)%458),y=69+(int)(yc_hash(&v)%68);unsigned c=i%7?RGB(52,79,99):RGB(145,178,179);pixel(x,y,c);}
 int radius=25+(int)(seed%8),cx=66+(int)((seed>>7)%27);yc_world(cx,105,radius,seed);
 /* Each dealer has its own stable image: world, palette, window ribs, remote
    facilities and traffic placement are derived from its canonical site. */
 int rib=125+(int)((seed>>12)%21),station=393+(int)((seed>>18)%25);
 for(int j=0;j<4;j++){int x=j==0?13:j==1?31:j==2?449:467;rect(x,70,3,91,RGB(108,102,81));line(x+1,70,x+1,161,RGB(212,185,132));}
 line(17,90,51,72,YC_GOLD);line(51,72,428,72,YC_GOLD);line(428,72,464,90,YC_GOLD);
 for(int j=0;j<2;j++){int x=j?480-rib:rib;line(x,74,x,132,RGB(35,68,76));line(x+2,74,x+2,132,RGB(83,100,91));}
 yc_tug(381+(int)(sinf(preview_time*.12f+(seed&7))*15),97+(seed%11),YC_TEAL);yc_tug(101+(int)(sinf(preview_time*.08f)*9),84,YC_GOLD);
 line(station-22,118,station+23,118,RGB(65,99,110));rect(station,114,10,8,RGB(97,119,125));rect(station+3,111,4,14,RGB(154,172,166));
 rect(12,142,456,29,RGB(22,43,48));for(int x=20;x<463;x+=34)line(240,134,x,170,RGB(60,73,68));for(int y=151;y<=167;y+=8)line(14,y,465,y,RGB(59,75,68));
 for(int yy=-11;yy<=11;yy++){float f=sqrtf(fmaxf(0,1.f-yy*yy/121.f));int half=(int)(132*f);line(240-half,151+yy,240+half,151+yy,RGB(7,23,28));}
 for(int i=0;i<48;i++){float a=i*3.14159265f/47;pixel(240+(int)(130*cosf(a)),151+(int)(11*sinf(a)),YC_TEAL);}
 for(int i=0;i<4;i++){int x=i==0?34:i==1?85:i==2?393:443;rect(x,148,2,10,RGB(187,156,107));rect(x-1,147,4,2,RGB(232,212,151));}
 yc_crew(101,153);yc_crew(384,153);yc_ship(ship,113,76,262,78);
}
#include "mega-yard-showroom.h"
static void yc_regular_scene(int ship){
 yc_box(188,58,284,94,RGB(16,40,42),RGB(111,130,113));for(int x=201;x<467;x+=28)line(x,64,x,136,RGB(43,66,63));
 line(204,67,454,67,YC_TEAL);rect(192,131,276,18,RGB(24,44,43));for(int y=134;y<150;y+=5)line(192,y,468,y,RGB(56,76,66));
 line(203,80,203,142,RGB(141,157,124));line(465,80,465,142,RGB(141,157,124));yc_box(205,135,18,9,RGB(62,87,83),RGB(156,165,127));rect(443,136,10,8,RGB(141,117,72));
 yc_ship(ship,207,69,246,70);
}
static void yc_footer(void){
 rect(0,249,480,23,ui_paint_surface(RGB(17,31,41)));line(8,249,471,249,RGB(83,106,107));
 button_icon(10,254,'U',YC_TEAL);button_icon(22,254,'D',YC_TEAL);text_px(38,257,YC_DIM,"SHIP");
 button_icon(84,254,'S',YC_TEAL);text_px(100,257,YC_DIM,shipyard_stats_view?"DETAILS":"STATS");
 button_icon(163,254,'X',YC_TEAL);text_px(179,257,YC_DIM,"EXCHANGE");button_icon(267,254,'O',YC_DOWN);text_px(283,257,YC_DIM,"BACK");
 yc_box(332,252,140,17,ui_paint_surface(RGB(8,19,25)),ui_paint_surface(RGB(77,100,109)));char b[24];snprintf(b,sizeof(b),"%.1f U",game.credits*.1f);text_px(464-(int)strlen(b)*8,257,YC_GOLD,"%s",b);
}
static void shipyard_console_draw(void){
 int count=shipyard_list_count(&game);if(row<0)row=0;if(row>=count)row=count-1;int ship=shipyard_list_ship(&game,row);if(ship<0)ship=0;
 /* The dealer owns the layout, including Lave's complete mega showroom. */
 int mega=mega_yard_stock_count(&game)>0;
 rect(0,0,W,H,YC_BG);rect(0,0,480,22,ui_paint_surface(RGB(17,31,41)));text_px(8,7,YC_GOLD,"ELITE: NEXT");text_px(112,7,YC_INK,mega?"/ MEGA SHIPYARD":"/ SHIPYARD");line(0,21,479,21,YC_TEAL);
 char place[32];snprintf(place,sizeof(place),game.station_variant?"%s HUB %d":"%s HUB",game.systems[game.system].name,game.station_variant+1);yc_text_size(472-(int)strlen(place)*8,7,8,YC_DIM,place);
 if(!game.docked){text_px(24,64,YC_DIM,"Dock to view ships for sale.");yc_footer();return;}
 if(mega){
  int style=mega_yard_style(&game);char dealer[32];yc_name(game.system,game.station_variant,dealer,sizeof(dealer));yc_text_size(16,29,16,YC_GOLD,dealer);yc_text_size(17,50,8,YC_INK,mega_yard_style_name(style));
  yc_exchange(288,26,184,36,ship);mega_yard_showroom(style,ship);yc_catalogue(8,156,176,90,8,ship);
  yc_mega_specs(ship);yc_footer();return;
 }
 /* Both dealers use the same seven-row catalogue; module capacities remain
  * visible in details and stats, before any exchange confirmation. */
 yc_box(8,27,170,32,ui_paint_surface(RGB(42,62,54)),RGB(128,139,113));
 char dealer[32];if(mega)yc_name(game.system,game.station_variant,dealer,sizeof(dealer));else snprintf(dealer,sizeof(dealer),"ORBIT MOTORS");
 yc_text_size(13,33,8,YC_GOLD,dealer);yc_text_size(13,46,8,YC_INK,mega?"CAPITAL COLLECTION":"LOCAL COLLECTION");
 yc_catalogue(8,69,170,114,7,ship);yc_text_size(191,32,8,YC_INK,player_ships[ship].name);
 yc_text_size(191,45,8,YC_TEAL,mega?"THE CAPITAL COLLECTION":"THE LOCAL COLLECTION");
 yc_regular_scene(ship);yc_exchange(188,154,284,30,ship);
 yc_box(8,188,464,58,ui_paint_surface(RGB(16,35,43)),RGB(101,115,106));
 if(shipyard_stats_view){
  const PlayerShip *p=&player_ships[ship],*o=&player_ships[game.ship];char value[16];
  const char *label[]={"CARGO","RANGE","SPEED","HULL","SHIELD","HEAT CAP"};
  int v[]={p->capacity,p->range,p->speed,p->hull,p->shields,p->heat_capacity};int old[]={o->capacity,o->range,o->speed,o->hull,o->shields,o->heat_capacity};
  for(int i=0;i<6;i++){if(i==0)snprintf(value,sizeof(value),"%dT",v[i]);else if(i==1)snprintf(value,sizeof(value),"%.1fLY",v[i]*.1f);else snprintf(value,sizeof(value),i<3?"%d":"%d%%",v[i]);yc_stat(16+(i%3)*156,193+(i/3)*15,label[i],value,yc_delta(v[i],old[i]));}
 }else yc_copy(16,192,56,4,9,shipyard_description[ship],1);
 char modules[64];const uint8_t *slots=player_ships[ship].slots;
 snprintf(modules,sizeof(modules),"SLOTS WPN %d DEF %d NAV %d HOLD %d FUEL %d UTIL %d",slots[0],slots[1],slots[2],slots[3],slots[4],slots[5]);
 yc_text_size(16,233,8,YC_TEAL,modules);
 yc_footer();
}
