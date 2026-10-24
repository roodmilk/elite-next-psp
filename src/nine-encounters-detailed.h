/* Individually designed structures with continuous paintings per major surface.
 * Shared helpers provide geometry/UV arithmetic, never a shared facade layout. */
#include "nine-encounter-art.h"
typedef struct {int front,rear,left,right,roof;} NineRoomArt;
static const NineRoomArt nine_rooms[]={
 {N_SALVAGE_FRONT,N_SALVAGE_REAR,N_SALVAGE_LEFT,N_SALVAGE_RIGHT,N_SALVAGE_ROOF},
 {N_BREAKER_FRONT,N_BREAKER_REAR,N_BREAKER_LEFT,N_BREAKER_RIGHT,N_BREAKER_ROOF},
 {N_GARDEN_FRONT,N_GARDEN_REAR,N_GARDEN_LEFT,N_GARDEN_RIGHT,N_GARDEN_ROOF},
 {N_FUEL_FRONT,N_FUEL_REAR,N_FUEL_LEFT,N_FUEL_RIGHT,N_FUEL_ROOF},
 {N_FERRY_FRONT,N_FERRY_REAR,N_FERRY_LEFT,N_FERRY_RIGHT,N_FERRY_ROOF},
 {N_TEA_FRONT,N_TEA_REAR,N_TEA_LEFT,N_TEA_RIGHT,N_TEA_ROOF},
 {N_ORE_FRONT,N_ORE_REAR,N_ORE_LEFT,N_ORE_RIGHT,N_ORE_ROOF},
 {N_PARTS_FRONT,N_PARTS_REAR,N_PARTS_LEFT,N_PARTS_RIGHT,N_PARTS_ROOF},
 {N_BEACON_FRONT,N_BEACON_REAR,N_BEACON_LEFT,N_BEACON_RIGHT,N_BEACON_ROOF}};
static Vec3 nine_at(Vec3 c,Vec3 p,float yaw){return add(c,discovery_yaw(p,yaw));}
static void nine_room(Vec3 c,Vec3 p,Vec3 e,float yaw,unsigned paint,int art){
 NineRoomArt a=nine_rooms[art];float left=p.x-e.x,right=p.x+e.x,front=p.z-e.z,rear=p.z+e.z,low=p.y-e.y,top=p.y+e.y,high=top+e.y*.22f;
 radio_paint_surface(c,(Vec3){left,top,front},(Vec3){right,top,front},(Vec3){right,low,front},(Vec3){left,low,front},yaw,paint,a.front,3,2);
 radio_paint_surface(c,(Vec3){right,high,rear},(Vec3){left,high,rear},(Vec3){left,low,rear},(Vec3){right,low,rear},yaw,paint,a.rear,3,2);
 radio_paint_surface(c,(Vec3){left,high,rear},(Vec3){left,top,front},(Vec3){left,low,front},(Vec3){left,low,rear},yaw,paint,a.left,2,2);
 radio_paint_surface(c,(Vec3){right,top,front},(Vec3){right,high,rear},(Vec3){right,low,rear},(Vec3){right,low,front},yaw,paint,a.right,2,2);
 radio_paint_surface(c,(Vec3){left-25,high+10,rear+20},(Vec3){right+25,high+10,rear+20},(Vec3){right+25,top+10,front-20},(Vec3){left-25,top+10,front-20},yaw,paint,a.roof,3,2);
 discovery_solid_quad(c,(Vec3){left,low,front},(Vec3){left,low,rear},(Vec3){right,low,rear},(Vec3){right,low,front},yaw,paint);
}
static void nine_beam(Vec3 c,Vec3 a,Vec3 b,float width,float yaw,unsigned paint,int tile){
 Vec3 axis=norm(sub(b,a)),u=norm((Vec3){axis.y,-axis.x,0});if(length(u)<.1f)u=(Vec3){1,0,0};
 Vec3 v={axis.y*u.z-axis.z*u.y,axis.z*u.x-axis.x*u.z,axis.x*u.y-axis.y*u.x},q[8];
 for(int k=0;k<8;k++)q[k]=add(k&4?b:a,add(mul(u,k&1?width:-width),mul(v,k&2?width:-width)));
 static const unsigned char faces[6][4]={{0,2,3,1},{4,5,7,6},{0,4,6,2},{1,3,7,5},{0,1,5,4},{2,6,7,3}};
 for(int k=0;k<6;k++)discovery_design_quad(c,q[faces[k][0]],q[faces[k][1]],q[faces[k][2]],q[faces[k][3]],yaw,paint,tile,0);
}
static void nine_deck(Vec3 c,float y,float rx,float rz,float yaw,unsigned paint,int tile){
 for(int k=0;k<8;k++){
  float a=k*.78539816f,b=(k+1)*.78539816f;Vec3 aa={cosf(a)*rx,y,sinf(a)*rz},bb={cosf(b)*rx,y,sinf(b)*rz},la=aa,lb=bb;la.y=lb.y=y-100;
  discovery_design_quad(c,(Vec3){0,y,0},bb,aa,(Vec3){0,y,0},yaw,paint,tile+(k&3),0);
  discovery_design_quad(c,aa,bb,lb,la,yaw,paint,tile+(k&3),0);
 }
}
static void nine_rock(Vec3 c,float yaw,int tile,int variant){
 Vec3 q[3][12];const float edge[12]={1.05f,.89f,1.13f,.95f,.84f,1.06f,.92f,1.12f,.88f,1.04f,.94f,1.09f};
 const float radius[3]={1080,1260,660},height[3]={-380,-880,-1490};unsigned stone=variant?RGB(132,115,91):RGB(130,124,113);
 for(int r=0;r<3;r++)for(int k=0;k<12;k++){float a=k*.52359878f+r*.11f+variant*.16f;q[r][k]=(Vec3){cosf(a)*radius[r]*edge[(k+variant*3)%12],height[r]-(k%3)*55,sinf(a)*radius[r]*edge[(k+4)%12]};}
 for(int k=0;k<12;k++){
  int n=(k+1)%12;Vec3 top={90,-310,-90},bottom={-70,-1700,40};
  discovery_design_quad(c,top,q[0][n],q[0][k],top,yaw,stone,tile+k,0);
  for(int r=0;r<2;r++)discovery_design_quad(c,q[r][k],q[r][n],q[r+1][n],q[r+1][k],yaw,stone,tile+r*12+k,0);
  discovery_design_quad(c,bottom,q[2][k],q[2][n],bottom,yaw,stone,tile+12+k,0);
 }
}
static Vec3 nine_loft_vertex(Vec3 p,float rx,float ry,float len,const float*shape,int ring,int side){
 static const float z[4]={-1,-.55f,.55f,1};float a=side*.78539816f;
 return add(p,(Vec3){cosf(a)*rx*shape[ring],sinf(a)*ry*shape[ring],len*z[ring]});
}
static void nine_loft(Vec3 c,Vec3 p,float rx,float ry,float len,const float*shape,float yaw,unsigned paint,int skin,int caps,int front_art,int rear_art){
 for(int r=0;r<3;r++)for(int k=0;k<8;k++)discovery_design_quad(c,nine_loft_vertex(p,rx,ry,len,shape,r,k),nine_loft_vertex(p,rx,ry,len,shape,r,k+1),nine_loft_vertex(p,rx,ry,len,shape,r+1,k+1),nine_loft_vertex(p,rx,ry,len,shape,r+1,k),yaw,paint,skin+r*8+k,0);
 for(int k=0;k<8;k++){
  Vec3 front=add(p,(Vec3){0,0,-len}),back=add(p,(Vec3){0,0,len});
  if(caps&1)discovery_solid_quad(c,front,nine_loft_vertex(p,rx,ry,len,shape,0,k+1),nine_loft_vertex(p,rx,ry,len,shape,0,k),front,yaw,paint);
  if(caps&2)discovery_solid_quad(c,back,nine_loft_vertex(p,rx,ry,len,shape,3,k),nine_loft_vertex(p,rx,ry,len,shape,3,k+1),back,yaw,paint);
 }
 for(int end=0;end<2;end++)if(caps&(1<<end)){
  float x=rx*shape[end?3:0]*.68f,y=ry*shape[end?3:0]*.68f,z=(end?1:-1)*(len+2);Vec3 a,b,cc,d;
  if(!end){a=(Vec3){-x,y,z};b=(Vec3){x,y,z};cc=(Vec3){x,-y,z};d=(Vec3){-x,-y,z};}
  else {a=(Vec3){x,y,z};b=(Vec3){-x,y,z};cc=(Vec3){-x,-y,z};d=(Vec3){x,-y,z};}
  radio_paint_surface(c,add(p,a),add(p,b),add(p,cc),add(p,d),yaw,paint,end?rear_art:front_art,2,1);
 }

}
static void nine_tank(Vec3 c,Vec3 p,float rx,float ry,float len,float yaw,unsigned paint,int skin,int end){
 for(int r=0;r<2;r++)for(int k=0;k<8;k++){
  float a=k*.78539816f,b=(k+1)*.78539816f,z0=-len+r*len,z1=z0+len;
  Vec3 aa=add(p,(Vec3){cosf(a)*rx,sinf(a)*ry,z0}),bb=add(p,(Vec3){cosf(b)*rx,sinf(b)*ry,z0}),cc=add(p,(Vec3){cosf(b)*rx,sinf(b)*ry,z1}),dd=add(p,(Vec3){cosf(a)*rx,sinf(a)*ry,z1});
  discovery_design_quad(c,aa,bb,cc,dd,yaw,paint,skin+r*8+k,0);
 }
 for(int k=0;k<8;k++){
  float a=k*.78539816f,b=(k+1)*.78539816f;
  Vec3 front=add(p,(Vec3){0,0,-len}),back=add(p,(Vec3){0,0,len});
  discovery_solid_quad(c,front,add(front,(Vec3){cosf(b)*rx,sinf(b)*ry,0}),add(front,(Vec3){cosf(a)*rx,sinf(a)*ry,0}),front,yaw,paint);
  discovery_solid_quad(c,back,add(back,(Vec3){cosf(a)*rx,sinf(a)*ry,0}),add(back,(Vec3){cosf(b)*rx,sinf(b)*ry,0}),back,yaw,paint);
 }
 Vec3 a=add(p,(Vec3){-rx*.55f,ry*.55f,-len-2}),b=add(p,(Vec3){rx*.55f,ry*.55f,-len-2}),cc=add(p,(Vec3){rx*.55f,-ry*.55f,-len-2}),d=add(p,(Vec3){-rx*.55f,-ry*.55f,-len-2});
 discovery_design_quad(c,a,b,cc,d,yaw,paint,end,0);
 a=add(p,(Vec3){rx*.55f,ry*.55f,len+2});b=add(p,(Vec3){-rx*.55f,ry*.55f,len+2});cc=add(p,(Vec3){-rx*.55f,-ry*.55f,len+2});d=add(p,(Vec3){rx*.55f,-ry*.55f,len+2});discovery_design_quad(c,a,b,cc,d,yaw,paint,end+1,0);
}

static void nine_salvage(Vec3 c,float yaw){
 unsigned hull=RGB(181,192,178),steel=RGB(158,137,107);static const float capsule[4]={.6f,1,1,.65f};
 nine_rock(c,yaw,N_SALVAGE_ROCK,0);nine_deck(c,-330,1120,1060,yaw,steel,N_SALVAGE_DECK);
 nine_loft(c,(Vec3){-560,-25,100},310,270,730,capsule,yaw,hull,N_SALVAGE_SKIN,3,N_SALVAGE_CAP_FRONT,N_SALVAGE_CAP_BACK);
 nine_room(c,(Vec3){440,0,310},(Vec3){360,260,420},yaw,hull,0);
 nine_tank(nine_at(c,(Vec3){-80,-110,600},yaw),(Vec3){0,0,0},120,100,260,yaw+1.5707963f,hull,N_SALVAGE_SKIN,N_SALVAGE_TOOLS);
 nine_tank(c,(Vec3){690,-155,-630},180,160,260,yaw,steel,N_SALVAGE_SKIN,N_SALVAGE_TOOLS);
 Vec3 shoulder={-840,420,-550},elbow={-420,850,-430},wrist={60+sinf(game.time*.18f)*110,420,-770};
 nine_beam(c,shoulder,elbow,45,yaw,steel,N_SALVAGE_TOOLS);nine_beam(c,elbow,wrist,32,yaw,steel,N_SALVAGE_TOOLS+1);
 discovery_design_box(c,wrist,(Vec3){100,40,100},yaw,steel,N_SALVAGE_TOOLS,N_SALVAGE_TOOLS+1,N_SALVAGE_TOOLS,0);
 for(int k=0;k<4;k++)encounter_lamp(c,(Vec3){-240+k*210,-250,-1040},yaw,RGB(220,170,91));
}
static void nine_breaker(Vec3 c,float yaw){
 unsigned steel=RGB(165,155,133),hull=RGB(142,164,179);static const float bow[4]={.2f,.9f,1,1},stern[4]={1,1,.85f,.6f};
 for(int side=-1;side<=1;side+=2){
  nine_beam(c,(Vec3){side*1000,-450,-1770},(Vec3){side*1000,-450,1770},90,yaw,steel,N_BREAKER_DECK);
  for(int k=-1;k<=1;k++){
   nine_beam(c,(Vec3){side*1000,-450,k*1150},(Vec3){side*1000,800,k*1150},50,yaw,steel,N_BREAKER_DECK+1);
   nine_beam(c,(Vec3){side*1000,800,k*1150},(Vec3){side*640,800,k*1150},50,yaw,steel,N_BREAKER_DECK+2);
   nine_beam(c,(Vec3){side*1000,-350,k*1150},(Vec3){side*590,-80,k*1150},45,yaw,steel,N_BREAKER_TOOLS);
  }
 }
 nine_loft(c,(Vec3){0,-20,-850},660,390,650,bow,yaw,hull,N_BREAKER_SKIN,1,N_BREAKER_CAP_FRONT,N_BREAKER_CAP_BACK);
 nine_loft(c,(Vec3){0,-20,820},660,390,600,stern,yaw,hull,N_BREAKER_SKIN,2,N_BREAKER_CAP_FRONT,N_BREAKER_CAP_BACK);
 radio_paint_surface(c,(Vec3){-450,260,225},(Vec3){450,260,225},(Vec3){450,-260,225},(Vec3){-450,-260,225},yaw,hull,N_BREAKER_CUT,2,2);
 for(int side=-1;side<=1;side+=2)nine_beam(c,(Vec3){side*550,-300,-185},(Vec3){side*470,350,-185},35,yaw,steel,N_BREAKER_TOOLS);
 nine_room(c,(Vec3){1250,0,750},(Vec3){260,240,380},yaw,steel,1);
 float z=sinf(game.time*.15f)*660;nine_beam(c,(Vec3){-950,800,z},(Vec3){950,800,z},50,yaw,steel,N_BREAKER_DECK+3);
 nine_beam(c,(Vec3){0,750,z},(Vec3){180,300,z},32,yaw,steel,N_BREAKER_TOOLS+1);encounter_lamp(c,(Vec3){180,260,z},yaw,((int)(game.time*6)&1)?RGB(102,221,232):RGB(58,115,139));
 for(int k=0;k<3;k++)encounter_lamp(c,(Vec3){(k-1)*380,-340,-1780},yaw,RGB(232,172,72));
}
static void nine_greenhouse(Vec3 c,float yaw){
 unsigned ivory=RGB(188,207,179),green=RGB(130,184,145);
 nine_deck(c,-300,1120,1090,yaw,ivory,N_GARDEN_DECK);
 for(int side=-1;side<=1;side+=2){
  float x=side*490;int skin=side<0?N_GARDEN_GLAZE:N_GARDEN_GLAZE_B;
  for(int r=0;r<2;r++)for(int k=0;k<6;k++){
   float a=k*.52359878f,b=(k+1)*.52359878f,z0=-940+r*900,z1=z0+900;
   discovery_design_quad(c,(Vec3){x+cosf(a)*430,-100+sinf(a)*500,z0},(Vec3){x+cosf(b)*430,-100+sinf(b)*500,z0},(Vec3){x+cosf(b)*430,-100+sinf(b)*500,z1},(Vec3){x+cosf(a)*430,-100+sinf(a)*500,z1},yaw,green,skin+r*6+k,0);
  }
  for(int k=0;k<6;k++){
   float a=k*.52359878f,b=(k+1)*.52359878f;Vec3 front={x,-100,-940},back={x,-100,860};
   discovery_solid_quad(c,front,add(front,(Vec3){cosf(b)*430,sinf(b)*500,0}),add(front,(Vec3){cosf(a)*430,sinf(a)*500,0}),front,yaw,RGB(64,119,109));
   discovery_solid_quad(c,back,add(back,(Vec3){cosf(a)*430,sinf(a)*500,0}),add(back,(Vec3){cosf(b)*430,sinf(b)*500,0}),back,yaw,RGB(64,119,109));
  }
  radio_paint_surface(c,(Vec3){x-225,270,-942},(Vec3){x+225,270,-942},(Vec3){x+225,-100,-942},(Vec3){x-225,-100,-942},yaw,ivory,N_GARDEN_END,2,2);
  radio_paint_surface(c,(Vec3){x+225,270,862},(Vec3){x-225,270,862},(Vec3){x-225,-100,862},(Vec3){x+225,-100,862},yaw,ivory,N_GARDEN_END,2,2);
  discovery_design_box(c,(Vec3){side*1150,-170,0},(Vec3){170,25,720},yaw,ivory,N_GARDEN_ROOF,N_GARDEN_ROOF,N_GARDEN_DECK,0);
  nine_beam(c,(Vec3){side*500,-300,0},(Vec3){0,-650,0},28,yaw,ivory,N_GARDEN_DECK+1);
 }
 nine_room(c,(Vec3){0,-230,830},(Vec3){260,220,260},yaw,ivory,2);
 nine_tank(c,(Vec3){0,-650,-100},220,200,720,yaw,ivory,N_GARDEN_TANK,N_GARDEN_REAR);
 for(int k=0;k<3;k++)encounter_lamp(c,(Vec3){(k-1)*420,430,-100},yaw,RGB(117,210,159));
}
static void nine_fuel(Vec3 c,float yaw){
 unsigned cream=RGB(180,164,123),patch=RGB(143,168,175);static const float keel[4]={.2f,1,1,.5f};
 nine_loft(c,(Vec3){0,-410,0},1040,170,1430,keel,yaw,patch,N_FUEL_SKIN,3,N_FUEL_CAP_FRONT,N_FUEL_CAP_BACK);
 nine_tank(c,(Vec3){-540,-10,40},330,330,940,yaw,cream,N_FUEL_TANK_A,N_FUEL_TOOLS);
 nine_tank(c,(Vec3){540,-10,40},330,330,940,yaw,patch,N_FUEL_TANK_B,N_FUEL_TOOLS);
 for(int side=-1;side<=1;side+=2){
  for(int k=-1;k<=1;k++)nine_beam(c,(Vec3){side*540-360,-250,k*630},(Vec3){side*540+360,-250,k*630},65,yaw,cream,N_FUEL_DECK);
  discovery_design_box(c,(Vec3){side*290,-35,-1080},(Vec3){160,210,150},yaw,cream,N_FUEL_TOOLS,N_FUEL_TOOLS+1,N_FUEL_TOOLS,0);
 }
 nine_room(c,(Vec3){0,420,1000},(Vec3){390,230,300},yaw,cream,3);
 Vec3 elbow={-1320,440,-780},end={-890+sinf(game.time*.14f)*110,620,-1320};
 nine_beam(c,(Vec3){-930,-220,-650},elbow,45,yaw,cream,N_FUEL_TOOLS);nine_beam(c,elbow,end,32,yaw,patch,N_FUEL_TOOLS+1);
 discovery_design_box(c,end,(Vec3){90,75,75},yaw,cream,N_FUEL_TOOLS,N_FUEL_TOOLS+1,N_FUEL_TOOLS,0);
 for(int k=0;k<4;k++)encounter_lamp(c,(Vec3){-450+k*300,-190,-1370},yaw,((int)(game.time*2)+k)%4==0?RGB(108,220,198):RGB(54,104,99));
}
static void nine_ferry(Vec3 c,float yaw){
 unsigned ivory=RGB(189,201,184),blue=RGB(117,155,177);static const float shape[4]={.12f,1,1,.6f};
 nine_loft(c,(Vec3){0,-30,0},660,330,1680,shape,yaw,ivory,N_FERRY_SKIN,3,N_FERRY_CAP_FRONT,N_FERRY_CAP_BACK);
 nine_room(c,(Vec3){0,380,-760},(Vec3){350,140,300},yaw,ivory,4);
 for(int side=-1;side<=1;side+=2){
  nine_tank(c,(Vec3){side*650,-230,900},180,155,590,yaw,blue,N_FERRY_SKIN,N_FERRY_ENGINE+(side>0?2:0));
  discovery_design_box(c,(Vec3){side*690,-20,-80},(Vec3){80,200,180},yaw,ivory,N_FERRY_LEFT,N_FERRY_ROOF,N_FERRY_RIGHT,0);
  nine_beam(c,(Vec3){side*580,-180,380},(Vec3){side*1030,-130,970},45,yaw,blue,N_FERRY_ROOF+3);
  encounter_lamp(c,(Vec3){side*650,-230,1500},yaw,RGB(63,134,176));
 }
 for(int k=0;k<3;k++)encounter_lamp(c,(Vec3){(k-1)*150,555,-760},yaw,RGB(228,184,105));
}
static void nine_tea(Vec3 c,float yaw){
 unsigned cream=RGB(195,176,133),teal=RGB(118,158,154);static const float keel[4]={.3f,1,1,.65f};
 nine_loft(c,(Vec3){0,-430,0},1030,250,1260,keel,yaw,teal,N_TEA_SKIN,3,N_TEA_CAP_FRONT,N_TEA_CAP_BACK);
 nine_room(c,(Vec3){-70,0,140},(Vec3){650,290,610},yaw,cream,5);
 nine_deck(c,-180,1000,1180,yaw,cream,N_TEA_DECK);
 radio_paint_surface(c,(Vec3){-850,385,-430},(Vec3){800,385,-430},(Vec3){800,330,-1000},(Vec3){-850,330,-1000},yaw,teal,N_TEA_AWNING,4,1);
 for(int side=-1;side<=1;side+=2){
  nine_beam(c,(Vec3){side*770,-180,-970},(Vec3){side*770,340,-970},15,yaw,cream,N_TEA_DECK);
  nine_tank(c,(Vec3){side*690,-400,970},150,135,260,yaw,teal,N_TEA_SKIN,N_TEA_REAR);
 }
 nine_beam(c,(Vec3){370,360,0},(Vec3){370,770,0},20,yaw,cream,N_TEA_DECK+1);
 radio_paint_surface(c,(Vec3){150,1000,-10},(Vec3){590,1000,-10},(Vec3){590,580,-10},(Vec3){150,580,-10},yaw,cream,N_TEA_SIGN,2,2);
 radio_paint_surface(c,(Vec3){590,1000,10},(Vec3){150,1000,10},(Vec3){150,580,10},(Vec3){590,580,10},yaw,cream,N_TEA_SIGN,2,2);
 for(int k=0;k<4;k++)encounter_lamp(c,(Vec3){-650+k*420,350,-1010},yaw,RGB(230,184,106));
}
static void nine_ore_silo(Vec3 c,Vec3 p,float yaw,unsigned paint){
 const float y[4]={-330,-80,420,510},r[4]={130,320,320,265};
 for(int band=0;band<3;band++)for(int k=0;k<8;k++){
  float a=k*.78539816f,b=(k+1)*.78539816f;
  Vec3 aa=add(p,(Vec3){cosf(a)*r[band],y[band],sinf(a)*r[band]}),bb=add(p,(Vec3){cosf(b)*r[band],y[band],sinf(b)*r[band]}),cc=add(p,(Vec3){cosf(b)*r[band+1],y[band+1],sinf(b)*r[band+1]}),dd=add(p,(Vec3){cosf(a)*r[band+1],y[band+1],sinf(a)*r[band+1]});
  discovery_design_quad(c,aa,dd,cc,bb,yaw,paint,band?N_ORE_SILO+(band-1)*8+k:N_ORE_HOPPER+(k&3),0);
 }
 for(int k=0;k<8;k++){float a=k*.78539816f,b=(k+1)*.78539816f;Vec3 top=add(p,(Vec3){0,610,0});discovery_design_quad(c,top,add(p,(Vec3){cosf(b)*265,510,sinf(b)*265}),add(p,(Vec3){cosf(a)*265,510,sinf(a)*265}),top,yaw,paint,N_ORE_SILO+8+k,0);}
}
static void nine_ore(Vec3 c,float yaw){
 unsigned steel=RGB(172,150,113);nine_rock(c,yaw,N_ORE_ROCK,1);nine_deck(c,-330,1140,1060,yaw,steel,N_ORE_DECK);
 nine_ore_silo(c,(Vec3){-560,0,-460},yaw,steel);nine_ore_silo(c,(Vec3){560,0,-460},yaw,steel);nine_ore_silo(c,(Vec3){0,0,570},yaw,steel);
 nine_room(c,(Vec3){-750,80,730},(Vec3){290,230,300},yaw,steel,6);
 nine_beam(c,(Vec3){-550,-230,-450},(Vec3){0,-250,-1100},80,yaw,steel,N_ORE_DECK+1);nine_beam(c,(Vec3){550,-230,-450},(Vec3){0,-250,-1100},80,yaw,steel,N_ORE_DECK+2);
 discovery_design_ring(nine_at(c,(Vec3){0,40,-1090},yaw),240,65,yaw,steel,N_ORE_HOPPER,8);
 float a=game.time*.16f;nine_beam(c,(Vec3){cosf(a)*160,40+sinf(a)*160,-1170},(Vec3){-cosf(a)*160,40-sinf(a)*160,-1170},25,yaw,steel,N_ORE_DECK+3);
 for(int k=0;k<3;k++)encounter_lamp(c,(Vec3){-430+k*430,-180,-1230},yaw,RGB(225,166,72));
}
static void nine_parts(Vec3 c,float yaw){
 unsigned paint=RGB(180,185,167),trim=RGB(142,132,113);nine_deck(c,-350,1320,1220,yaw,trim,N_PARTS_DECK);
 nine_room(c,(Vec3){0,-50,30},(Vec3){340,240,340},yaw,paint,7);
 for(int side=-1;side<=1;side+=2){
  Vec3 p={side*740,-20,300};nine_tank(nine_at(c,p,yaw),(Vec3){0,0,0},310,290,520,yaw+side*.40f,paint,side<0?N_PARTS_POD_A:N_PARTS_POD_B,N_PARTS_FRONT);
  nine_beam(c,(Vec3){side*280,-130,0},p,95,yaw,trim,N_PARTS_DECK+1);
  nine_beam(c,(Vec3){0,-610,120},add(p,(Vec3){0,-260,0}),65,yaw,trim,N_PARTS_DECK+2);
 }
 nine_tank(c,(Vec3){0,-20,-720},330,300,490,yaw,paint,N_PARTS_POD_B,N_PARTS_FRONT);
 nine_beam(c,(Vec3){0,-620,120},(Vec3){0,-300,-820},65,yaw,trim,N_PARTS_DECK+3);
 for(int k=0;k<5;k++)encounter_lamp(c,(Vec3){-720+k*360,-230,-1060},yaw,((int)(game.time*2)+k)%5==0?RGB(226,173,89):RGB(98,168,167));
}
static void nine_beacon(Vec3 c,float yaw){
 unsigned paint=RGB(182,191,170),blue=RGB(119,158,174);static const float tug[4]={.2f,.9f,1,.6f};
 nine_loft(c,(Vec3){-650,-190,100},380,250,950,tug,yaw,blue,N_BEACON_SKIN,3,N_BEACON_CAP_FRONT,N_BEACON_CAP_BACK);
 nine_room(c,(Vec3){-650,160,-360},(Vec3){300,150,270},yaw,paint,8);
 for(int side=-1;side<=1;side+=2)nine_tank(c,(Vec3){-650+side*300,-280,870},130,120,260,yaw,blue,N_BEACON_SKIN,N_BEACON_REAR);
 const float yy[4]={-480,-160,250,570},rr[4]={110,270,300,170};
 for(int band=0;band<3;band++)for(int k=0;k<8;k++){
  float a=k*.78539816f,b=(k+1)*.78539816f;Vec3 aa={640+cosf(a)*rr[band],yy[band],sinf(a)*rr[band]},bb={640+cosf(b)*rr[band],yy[band],sinf(b)*rr[band]},cc={640+cosf(b)*rr[band+1],yy[band+1],sinf(b)*rr[band+1]},dd={640+cosf(a)*rr[band+1],yy[band+1],sinf(a)*rr[band+1]};
  discovery_design_quad(c,aa,dd,cc,bb,yaw,paint,N_BEACON_LENS+(band==1?4:0)+(k&3),0);
 }
 for(int k=0;k<8;k++){float a=k*.78539816f,b=(k+1)*.78539816f;Vec3 top={640,680,0};discovery_design_quad(c,top,(Vec3){640+cosf(b)*170,570,sinf(b)*170},(Vec3){640+cosf(a)*170,570,sinf(a)*170},top,yaw,paint,N_BEACON_LENS+(k&3),0);}
 nine_beam(c,(Vec3){-270,-60,-250},(Vec3){60,240,-260},42,yaw,paint,N_BEACON_TOOLS);
 nine_beam(c,(Vec3){60,240,-260},(Vec3){370,40,-180},32,yaw,paint,N_BEACON_TOOLS+1);
 for(int k=0;k<3;k++){
  float a=k*2.0943951f;Vec3 end={640+cosf(a)*320,40+sinf(a)*320,-40};nine_beam(c,(Vec3){370,40,-180},end,18,yaw,paint,N_BEACON_TOOLS);
 }
 for(int k=0;k<2;k++){
  Vec3 p={1090,120+k*350,160+k*170};discovery_design_box(c,p,(Vec3){95,70,130},yaw,paint,N_BEACON_ROBOT+k,N_BEACON_ROBOT+2,N_BEACON_ROBOT+3,0);
  nine_beam(c,add(p,(Vec3){-80,0,-70}),add(p,(Vec3){-230,60,-180}),12,yaw,paint,N_BEACON_TOOLS);encounter_lamp(c,add(p,(Vec3){0,0,-135}),yaw,RGB(116,210,224));
 }
 encounter_lamp(c,(Vec3){640,710,0},yaw,((int)(game.time*.8f)&3)==0?RGB(223,165,80):RGB(63,92,89));
}
static int nine_dispatch(Vec3 c,int entry,float yaw){
 switch(entry){case 1:nine_salvage(c,yaw);break;case 15:nine_breaker(c,yaw);break;case 3:nine_greenhouse(c,yaw);break;case 0:nine_fuel(c,yaw);break;case 4:nine_ferry(c,yaw);break;case 6:nine_tea(c,yaw);break;case 8:nine_ore(c,yaw);break;case 9:nine_parts(c,yaw);break;case 10:nine_beacon(c,yaw);break;default:return 0;}return 1;
}
