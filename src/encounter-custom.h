/* Dedicated encounter geometry; panel roles map to the corresponding atlas row.
 * Families: relay, outpost, transport, sanctuary, cinema, resort, mechanic. */
#include "encounter-art-ids.h"
static void encounter_box(Vec3 c,Vec3 p,Vec3 e,float yaw,unsigned paint,int family,int role){
 discovery_design_box(c,p,e,yaw,paint,ENCOUNTER_TILE(family,role),ENCOUNTER_TILE(family,1),ENCOUNTER_TILE(family,2),0);
}
static void encounter_lamp(Vec3 c,Vec3 p,float yaw,unsigned ink){discovery_dot(c,discovery_yaw(p,yaw),1,ink);}
/* A real joint-to-joint beam, rather than a horizontal box suggesting a crane. */
static void encounter_arm(Vec3 c,Vec3 a,Vec3 b,float width,float yaw,unsigned paint){
 Vec3 axis=norm(sub(b,a)),u=norm((Vec3){axis.y,-axis.x,0});
 if(length(u)<.1f)u=(Vec3){1,0,0};
 Vec3 v={axis.y*u.z-axis.z*u.y,axis.z*u.x-axis.x*u.z,axis.x*u.y-axis.y*u.x};
 Vec3 q[8];for(int k=0;k<8;k++){Vec3 p=k&4?b:a;q[k]=add(p,add(mul(u,k&1?width:-width),mul(v,k&2?width:-width)));}
 static const unsigned char faces[6][4]={{0,2,3,1},{4,5,7,6},{0,4,6,2},{1,3,7,5},{0,1,5,4},{2,6,7,3}};
 for(int k=0;k<6;k++)discovery_design_quad(c,q[faces[k][0]],q[faces[k][1]],q[faces[k][2]],q[faces[k][3]],yaw,paint,ENCOUNTER_TILE(6,1),0);
}
static void encounter_asteroid_foundation(Vec3 c,float yaw){
 Vec3 upper[8],lower[8];const float rim[8]={1.08f,.91f,1.02f,.85f,1.12f,.96f,1.05f,.89f};
 unsigned rock=RGB(100,89,79);
 for(int k=0;k<8;k++){float a=k*.78539816f;upper[k]=(Vec3){cosf(a)*1250*rim[k],-445-(k&1)*55,sinf(a)*1370*rim[k]};lower[k]=(Vec3){cosf(a+.12f)*880*rim[(k+3)&7],-1220-(k%3)*95,sinf(a+.12f)*990*rim[k]};}
 for(int k=0;k<8;k++){int n=(k+1)&7;discovery_design_quad(c,upper[k],upper[n],lower[n],lower[k],yaw,rock,ENCOUNTER_TILE(7,k&3),0);
  discovery_solid_quad(c,(Vec3){0,-450,0},upper[n],upper[k],(Vec3){0,-450,0},yaw,rock);
  discovery_solid_quad(c,(Vec3){0,-1510,0},lower[k],lower[n],(Vec3){0,-1510,0},yaw,rock);
 }
}
/* Faceted pressure tank: curved silhouette carries its own mapped skin. */
static void encounter_tank(Vec3 c,Vec3 p,float radius,float half,float yaw,unsigned paint,int family){
 for(int k=0;k<8;k++){float a=k*.78539816f,b=(k+1)*.78539816f;Vec3 v[4]={{cosf(a)*radius,sinf(a)*radius,-half},{cosf(b)*radius,sinf(b)*radius,-half},{cosf(b)*radius,sinf(b)*radius,half},{cosf(a)*radius,sinf(a)*radius,half}};
  for(int i=0;i<4;i++)v[i]=add(v[i],p);discovery_design_quad(c,v[0],v[1],v[2],v[3],yaw,paint,ENCOUNTER_TILE(family,0),0);
  Vec3 front=add(p,(Vec3){0,0,-half}),rear=add(p,(Vec3){0,0,half});discovery_design_quad(c,front,v[1],v[0],front,yaw,paint,ENCOUNTER_TILE(family,family==8?0:2),0);discovery_design_quad(c,rear,v[3],v[2],rear,yaw,paint,ENCOUNTER_TILE(family,family==8?0:3),0);
 }
}
/* Salvage village: inhabited reclaimed hulls anchored to a worked asteroid.
 * Open sorting courtyard, pressure corridors and a moving recovery winch. */
static void encounter_salvage_box(Vec3 c,Vec3 p,Vec3 e,float yaw,unsigned paint,int family,int role){
 discovery_design_box(c,p,e,yaw,paint,ENCOUNTER_TILE(family,role),ENCOUNTER_TILE(family,2),ENCOUNTER_TILE(family,role),0);
}
static void encounter_salvage_rock(Vec3 c,float yaw){
 const float radius[10]={1,.83f,1.1f,.9f,1.04f,.88f,1.08f,.86f,1.03f,.94f};
 Vec3 rim[10],bottom[10],core={120,-410,-90};unsigned rock=RGB(132,121,108);
 for(int k=0;k<10;k++){float a=k*.62831853f;rim[k]=(Vec3){cosf(a)*1280*radius[k],-450-(k%3)*85,sinf(a)*1390*radius[k]};bottom[k]=(Vec3){cosf(a+.17f)*820,-1180-(k%4)*100,sinf(a+.17f)*930};}
 for(int k=0;k<10;k++){int n=(k+1)%10;
  discovery_design_quad(c,core,rim[n],rim[k],core,yaw,rock,ENCOUNTER_TILE(7,k&3),0);
  discovery_design_quad(c,rim[k],rim[n],bottom[n],bottom[k],yaw,rock,ENCOUNTER_TILE(7,(k+1)&3),0);
  Vec3 tip={-80,-1640,70};discovery_design_quad(c,tip,bottom[k],bottom[n],tip,yaw,rock,ENCOUNTER_TILE(7,2),0);
 }
}
static void encounter_salvage_village(Vec3 c,float yaw){
 unsigned hull=RGB(198,209,194),rust=RGB(196,149,102);int f=8;
 encounter_salvage_rock(c,yaw);
 encounter_salvage_box(c,(Vec3){0,-380,0},(Vec3){860,60,920},yaw,rust,f,2);
 /* An offset bridge connects two salvaged pressure vessels. */
 encounter_tank(c,(Vec3){-610,-10,60},250,680,yaw,hull,f);
 encounter_salvage_box(c,(Vec3){580,-70,360},(Vec3){280,240,410},yaw,hull,f,0);
 encounter_salvage_box(c,(Vec3){0,-120,590},(Vec3){450,110,95},yaw,hull,f,0);
 encounter_salvage_box(c,(Vec3){-610,250,290},(Vec3){250,75,260},yaw,hull,f,0);
 /* Sorted scrap has a clear frame, alternating bundles and open approach. */
 for(int k=0;k<3;k++){
  encounter_salvage_box(c,(Vec3){580,-200,-650+k*220},(Vec3){260,125,85},yaw,rust,f,1);
 }
 /* Broken ship bow, angled ribs and intact panel fragments in the yard. */
 encounter_salvage_box(c,(Vec3){-80,-180,-580},(Vec3){190,120,260},yaw+.26f,hull,f,2);
 encounter_arm(c,(Vec3){-250,-300,-790},(Vec3){-150,60,-610},24,yaw,rust);
 encounter_arm(c,(Vec3){170,-300,-780},(Vec3){100,20,-580},24,yaw,rust);
 /* Recovery gantry and its slowly translating suspended magnet. */
 encounter_salvage_box(c,(Vec3){-790,80,-750},(Vec3){65,440,65},yaw,rust,f,3);
 encounter_salvage_box(c,(Vec3){-340,480,-750},(Vec3){500,55,65},yaw,rust,f,3);
 float x=-260+sinf(game.time*.18f)*170;
 encounter_arm(c,(Vec3){x,430,-750},(Vec3){x,70,-750},9,yaw,hull);
 encounter_salvage_box(c,(Vec3){x,40,-750},(Vec3){85,35,85},yaw,rust,f,3);
 for(int k=0;k<4;k++)encounter_lamp(c,(Vec3){-230+k*155,-285,-935},yaw,RGB(224,166,72));
 encounter_lamp(c,(Vec3){-610,355,290},yaw,((int)(game.time*2)&1)?RGB(80,201,181):RGB(39,91,89));
}
/* Open-ended dry dock: ship hull fragments are supported by the cradle,
 * rather than indistinguishable free-floating containers. */
static void encounter_shipbreaker(Vec3 c,float yaw){
 int f=9;unsigned steel=RGB(185,177,156),hull=RGB(146,167,183);
 for(int side=-1;side<=1;side+=2){
  encounter_salvage_box(c,(Vec3){side*900,-340,0},(Vec3){80,110,1640},yaw,steel,f,1);
  for(int k=-1;k<=1;k++){
   encounter_salvage_box(c,(Vec3){side*900,160,k*1100},(Vec3){75,450,70},yaw,steel,f,1);
   encounter_salvage_box(c,(Vec3){side*600,650,k*1100},(Vec3){370,60,80},yaw,steel,f,1);
  }
 }
 /* Two uneven separated shell sections, each with an open centre. */
 for(int k=0;k<2;k++){
  float z=-650+k*1250;
  encounter_salvage_box(c,(Vec3){0,-160,z},(Vec3){590,70,380},yaw,hull,f,0);
  for(int side=-1;side<=1;side+=2){
   encounter_salvage_box(c,(Vec3){side*540,70,z},(Vec3){65,230,380},yaw+side*.06f,hull,f,0);
   encounter_arm(c,(Vec3){side*490,-80,z-290},(Vec3){side*370,370,z-290},32,yaw,steel);
  }
 }
 /* Overhead travelling cutting head. Pulsed blue lamps mark active equipment. */
 float z=sinf(game.time*.16f)*820;
 encounter_salvage_box(c,(Vec3){0,650,z},(Vec3){500,65,75},yaw,steel,f,1);
 encounter_arm(c,(Vec3){0,590,z},(Vec3){170,330,z},30,yaw,steel);
 encounter_salvage_box(c,(Vec3){170,300,z},(Vec3){60,65,60},yaw,steel,f,2);
 encounter_lamp(c,(Vec3){170,215,z},yaw,((int)(game.time*7)&1)?RGB(104,223,239):RGB(51,110,140));
 /* Occupied control gallery, isolated from the dismantling volume. */
 encounter_salvage_box(c,(Vec3){1120,170,900},(Vec3){210,220,390},yaw,steel,f,3);
 encounter_salvage_box(c,(Vec3){1080,-240,-780},(Vec3){160,100,540},yaw,steel,f,2);
 for(int k=0;k<3;k++)encounter_lamp(c,(Vec3){(k-1)*360,-220,-1580},yaw,RGB(235,171,65));
}
/* Three vaulted pressure conservatories: framed glazing carries crop art,
 * opaque insulated floor and a real vestibule close the inhabited volume. */
static void encounter_greenhouse(Vec3 c,float yaw){
 int f=10;unsigned ivory=RGB(194,214,186),green=RGB(137,196,161);
 encounter_salvage_box(c,(Vec3){0,-230,0},(Vec3){1060,95,870},yaw,ivory,f,1);
 for(int bay=-1;bay<=1;bay++){
  float x=bay*690;
  for(int k=0;k<6;k++){
   float a=k*.52359878f,b=(k+1)*.52359878f;
   Vec3 aa={x+cosf(a)*300,-120+sinf(a)*430,-700},bb={x+cosf(b)*300,-120+sinf(b)*430,-700};
   Vec3 cc=bb,dd=aa;cc.z=700;dd.z=700;
   discovery_design_quad(c,aa,bb,cc,dd,yaw,green,ENCOUNTER_TILE(f,0),0);
   Vec3 front={x,-120,-700},back={x,-120,700};
   discovery_design_quad(c,front,bb,aa,front,yaw,ivory,ENCOUNTER_TILE(f,2),0);
   discovery_design_quad(c,back,dd,cc,back,yaw,ivory,ENCOUNTER_TILE(f,2),0);
  }
  encounter_salvage_box(c,(Vec3){x,-30,-810},(Vec3){130,160,120},yaw,ivory,f,2);
 }
 /* The shaded underside holds water recovery and a paired radiator bank. */
 for(int side=-1;side<=1;side+=2){
  encounter_salvage_box(c,(Vec3){side*650,-450,0},(Vec3){260,150,430},yaw,ivory,f,1);
  encounter_salvage_box(c,(Vec3){side*1170,-260,0},(Vec3){110,30,680},yaw,ivory,f,3);
 }
 encounter_salvage_box(c,(Vec3){0,-470,820},(Vec3){240,160,180},yaw,ivory,f,2);
 for(int k=0;k<3;k++)encounter_lamp(c,(Vec3){(k-1)*690,340,0},yaw,((int)(game.time*.7f)+k)%3?RGB(73,142,98):RGB(136,225,174));
}
/* Repaired fuel tender: two pressure vessels on a structural spine,
 * isolated operator cabin, pump skids and an articulated loading coupling. */
static void encounter_fuel_stop(Vec3 c,float yaw){
 int f=11;unsigned paint=RGB(187,169,125),patch=RGB(146,177,185);
 encounter_salvage_box(c,(Vec3){0,-340,0},(Vec3){940,95,1070},yaw,paint,f,0);
 for(int side=-1;side<=1;side+=2){
  encounter_tank(c,(Vec3){side*570,0,210},320,810,yaw,side>0?patch:paint,f);
  for(int k=-1;k<=1;k++)encounter_salvage_box(c,(Vec3){side*570,-320,k*660+200},(Vec3){360,70,65},yaw,paint,f,2);
  encounter_salvage_box(c,(Vec3){side*260,-30,-760},(Vec3){170,210,170},yaw,paint,f,1);
 }
 encounter_salvage_box(c,(Vec3){0,60,1000},(Vec3){350,270,240},yaw,patch,f,3);
 encounter_salvage_box(c,(Vec3){0,410,1030},(Vec3){260,80,85},yaw,paint,f,3);
 /* Rigid articulated hose boom; a slow swivel suggests a serviced berth. */
 float sway=sinf(game.time*.15f)*100;Vec3 shoulder={-900,-100,-700},joint={-1130,380,-760},nozzle={-620+sway,540,-1050};
 encounter_arm(c,shoulder,joint,45,yaw,paint);encounter_arm(c,joint,nozzle,33,yaw,patch);
 encounter_salvage_box(c,nozzle,(Vec3){95,85,70},yaw,paint,f,2);
 for(int k=0;k<4;k++)encounter_lamp(c,(Vec3){-410+k*270,-220,-1100},yaw,((int)(game.time*2)+k)%4==0?RGB(101,225,199):RGB(51,111,103));
 encounter_lamp(c,(Vec3){0,525,1030},yaw,RGB(237,173,69));
}
/* Civilian ferry: tapered eight-sided pressure hull, layered cabin decks,
 * boarding collar, separate engine pods and raised forward bridge. */
static void encounter_passenger_ferry(Vec3 c,float yaw){
 int f=12;unsigned ivory=RGB(192,201,180),blue=RGB(116,155,178);
 const float z[4]={-1350,-800,820,1190},r[4]={.18f,1,1,.64f};
 for(int section=0;section<3;section++)for(int k=0;k<8;k++){
  float a=k*.78539816f,b=(k+1)*.78539816f;
  Vec3 aa={cosf(a)*540*r[section],sinf(a)*300*r[section],z[section]},bb={cosf(b)*540*r[section],sinf(b)*300*r[section],z[section]},cc={cosf(b)*540*r[section+1],sinf(b)*300*r[section+1],z[section+1]},dd={cosf(a)*540*r[section+1],sinf(a)*300*r[section+1],z[section+1]};
  discovery_design_quad(c,aa,bb,cc,dd,yaw,section==1?ivory:blue,ENCOUNTER_TILE(f,section==1?0:2),0);
 }
 encounter_salvage_box(c,(Vec3){0,340,-740},(Vec3){300,140,220},yaw,ivory,f,3);
 encounter_salvage_box(c,(Vec3){0,0,1190},(Vec3){346,193,35},yaw,blue,f,1);
 encounter_salvage_box(c,(Vec3){0,0,-1320},(Vec3){95,55,35},yaw,blue,f,3);
 for(int side=-1;side<=1;side+=2){
  encounter_salvage_box(c,(Vec3){side*620,-180,650},(Vec3){130,150,460},yaw,blue,f,1);
  encounter_salvage_box(c,(Vec3){side*550,-40,-240},(Vec3){80,190,190},yaw,ivory,f,2);
  encounter_lamp(c,(Vec3){side*770,-170,1120},yaw,RGB(68,142,180));
 }
 for(int k=0;k<3;k++)encounter_lamp(c,(Vec3){(k-1)*150,495,-740},yaw,RGB(231,187,104));
}
/* Independent neighbourhood broadcaster: compact occupied shack, receiver
 * dish, coax-fed mast, separate transmitter cooling and off-axis solar wing. */
#include "radio-shack-detailed.h"
static void encounter_radio_shack(Vec3 c,float yaw){radio_shack_detailed(c,yaw);}
/* Tea barge: wide glazed mess room, sheltered boarding deck, roof cup sign,
 * galley cupboards and service engines instead of a generic cargo crate. */
static void encounter_tea_barge(Vec3 c,float yaw){
 int f=14;unsigned cream=RGB(202,179,132),teal=RGB(115,164,155);
 encounter_salvage_box(c,(Vec3){0,-320,0},(Vec3){960,110,1100},yaw,teal,f,2);
 encounter_salvage_box(c,(Vec3){0,30,200},(Vec3){750,300,660},yaw,cream,f,0);
 encounter_salvage_box(c,(Vec3){0,370,120},(Vec3){850,45,800},yaw,teal,f,3);
 encounter_salvage_box(c,(Vec3){0,-60,-810},(Vec3){650,170,180},yaw,cream,f,1);
 for(int side=-1;side<=1;side+=2){
  encounter_salvage_box(c,(Vec3){side*710,-230,900},(Vec3){150,120,300},yaw,teal,f,2);
  encounter_salvage_box(c,(Vec3){side*750,90,-810},(Vec3){35,300,35},yaw,cream,f,1);
 }
 /* A small illuminated cup-shaped advertising fixture sits on a real post. */
 encounter_salvage_box(c,(Vec3){0,540,100},(Vec3){25,130,25},yaw,cream,f,1);
 encounter_salvage_box(c,(Vec3){0,760,100},(Vec3){200,115,55},yaw,cream,f,3);
 encounter_arm(c,(Vec3){200,830,100},(Vec3){310,830,100},25,yaw,cream);
 encounter_arm(c,(Vec3){310,830,100},(Vec3){310,710,100},25,yaw,cream);
 encounter_arm(c,(Vec3){310,710,100},(Vec3){200,710,100},25,yaw,cream);
 for(int k=0;k<4;k++)encounter_lamp(c,(Vec3){-600+k*400,420,-700},yaw,RGB(235,189,111));
 encounter_lamp(c,(Vec3){-60,920,90},yaw,((int)(game.time*1.4f)&1)?RGB(181,225,218):RGB(94,136,129));
}
/* Shared ore equipment anchored to rock. Secured external mineral bays
 * feed enclosed augers; the design does not rely on gravity in open space. */
static void encounter_ore_depot(Vec3 c,float yaw){
 int f=15;unsigned steel=RGB(180,157,113);encounter_salvage_rock(c,yaw);
 encounter_salvage_box(c,(Vec3){0,-350,0},(Vec3){970,75,1000},yaw,steel,f,2);
 for(int k=0;k<3;k++){
  float x=k==2?0:(k?590:-590),z=k==2?600:-450;
  encounter_salvage_box(c,(Vec3){x,-120,z},(Vec3){330,170,350},yaw,steel,f,0);
  for(int side=-1;side<=1;side+=2)encounter_salvage_box(c,(Vec3){x+side*330,90,z},(Vec3){30,80,370},yaw,steel,f,2);
  encounter_salvage_box(c,(Vec3){x,140,z},(Vec3){290,65,310},yaw,steel,f,0);
  encounter_salvage_box(c,(Vec3){x,-230,z-400},(Vec3){125,85,120},yaw,steel,f,1);
 }
 encounter_salvage_box(c,(Vec3){-570,290,760},(Vec3){300,210,240},yaw,steel,f,3);
 encounter_salvage_box(c,(Vec3){0,-180,-1070},(Vec3){780,70,120},yaw,steel,f,2);
 float turn=game.time*.16f;
 discovery_design_ring(add(c,discovery_yaw((Vec3){0,210,-1000},yaw)),280,65,yaw,steel,ENCOUNTER_TILE(f,1),8);
 encounter_arm(c,(Vec3){cosf(turn)*190,210+sinf(turn)*190,-1080},(Vec3){-cosf(turn)*190,210-sinf(turn)*190,-1080},25,yaw,steel);
 for(int k=0;k<3;k++)encounter_lamp(c,(Vec3){-440+k*440,-85,-1210},yaw,RGB(231,174,69));
}
/* A three-front cooperative market. The covered promenade joins real pressure
 * shops, with open display shutters, component racks and roof-mounted signs. */
static void encounter_parts_exchange(Vec3 c,float yaw){
 int f=16;unsigned paint=RGB(177,181,161),trim=RGB(148,118,89);
 encounter_salvage_box(c,(Vec3){0,-330,0},(Vec3){1120,100,950},yaw,trim,f,1);
 for(int k=-1;k<=1;k++){
  float x=k*690,h=k?210:340;
  encounter_salvage_box(c,(Vec3){x,h-170,150},(Vec3){300,h,560},yaw,paint,f,0);
  encounter_salvage_box(c,(Vec3){x,h*2-140,150},(Vec3){330,45,600},yaw,trim,f,3);
  encounter_salvage_box(c,(Vec3){x,h*2-10,-360},(Vec3){200,75,45},yaw,paint,f,3);
  encounter_salvage_box(c,(Vec3){x,-170,-640},(Vec3){250,120,180},yaw,paint,f,2);
 }
 for(int side=-1;side<=1;side+=2)encounter_salvage_box(c,(Vec3){side*350,-50,0},(Vec3){80,100,230},yaw,paint,f,2);
 encounter_salvage_box(c,(Vec3){0,-70,820},(Vec3){600,160,150},yaw,trim,f,1);
 for(int k=0;k<5;k++)encounter_lamp(c,(Vec3){-800+k*400,-175,-860},yaw,((int)(game.time*2)+k)%5==0?RGB(226,176,91):RGB(111,171,168));
}
/* The crew's tug physically grips the failed beacon. Separate service robots
 * carry tool reels; a slow boom movement is an actual mechanical joint. */
static void encounter_beacon_crew(Vec3 c,float yaw){
 int f=17;unsigned paint=RGB(189,193,161),blue=RGB(119,157,177);
 encounter_salvage_box(c,(Vec3){-590,-140,80},(Vec3){360,230,780},yaw,paint,f,2);
 encounter_salvage_box(c,(Vec3){-590,150,-420},(Vec3){310,150,220},yaw,paint,f,2);
 for(int side=-1;side<=1;side+=2)encounter_salvage_box(c,(Vec3){-590+side*390,-200,590},(Vec3){110,140,280},yaw,blue,f,3);
 encounter_salvage_box(c,(Vec3){460,60,0},(Vec3){150,580,150},yaw,blue,f,1);
 for(int side=-1;side<=1;side+=2)encounter_salvage_box(c,(Vec3){460+side*240,150,0},(Vec3){130,320,50},yaw,blue,f,1);
 encounter_salvage_box(c,(Vec3){460,690,0},(Vec3){210,130,210},yaw,paint,f,0);
 Vec3 elbow={-120,340,-230},tool={260,260+sinf(game.time*.18f)*80,-150};
 encounter_arm(c,(Vec3){-260,80,-350},elbow,42,yaw,paint);encounter_arm(c,elbow,tool,32,yaw,paint);
 encounter_salvage_box(c,tool,(Vec3){75,65,75},yaw,paint,f,3);
 for(int k=0;k<2;k++){
  Vec3 p={820,100+k*310,160+k*180};encounter_salvage_box(c,p,(Vec3){95,70,130},yaw,paint,f,3);
  encounter_arm(c,add(p,(Vec3){-80,0,-70}),add(p,(Vec3){-210,40,-150}),14,yaw,paint);
  encounter_lamp(c,add(p,(Vec3){0,0,-140}),yaw,RGB(127,211,224));
 }
 encounter_lamp(c,(Vec3){460,840,0},yaw,((int)(game.time*.7f)&3)==0?RGB(213,153,72):RGB(61,87,82));
}
#include "nine-encounters-detailed.h"
static int encounter_custom(Vec3 c,int entry,float yaw){
 if(nine_dispatch(c,entry,yaw))return 1;
 if(entry==8){encounter_ore_depot(c,yaw);return 1;}
 if(entry==9){encounter_parts_exchange(c,yaw);return 1;}
 if(entry==10){encounter_beacon_crew(c,yaw);return 1;}
 if(entry==4){encounter_passenger_ferry(c,yaw);return 1;}
 if(entry==14){encounter_radio_shack(c,yaw);return 1;}
 if(entry==6){encounter_tea_barge(c,yaw);return 1;}
 if(entry==0){encounter_fuel_stop(c,yaw);return 1;}
 if(entry==3){encounter_greenhouse(c,yaw);return 1;}
 if(entry==15){encounter_shipbreaker(c,yaw);return 1;}
 if(entry==1){encounter_salvage_village(c,yaw);return 1;}
 if(entry!=62&&entry!=63&&entry!=17&&entry!=12&&entry!=2)return 0;
 int family=entry==62?2:entry==63?3:entry==17?4:entry==12?5:6;
 unsigned paint=entry==62?RGB(76,104,118):entry==63?RGB(100,149,132):entry==17?RGB(91,116,157):entry==12?RGB(160,145,126):RGB(150,118,77);
 if(entry==62){ /* Containment ark: reinforced keel, paired protected habitat pods. */
  encounter_box(c,(Vec3){0,-470,0},(Vec3){930,100,1300},yaw,paint,family,3);
  for(int side=-1;side<=1;side+=2){
   encounter_box(c,(Vec3){side*900,0,0},(Vec3){72,520,1250},yaw,paint,family,1);
   for(int k=-1;k<=1;k++){encounter_tank(c,(Vec3){side*440,-60,k*790},290,330,yaw,paint,family);encounter_box(c,(Vec3){side*440,340,k*790},(Vec3){335,48,335},yaw,paint,family,2);}
  }
  encounter_box(c,(Vec3){0,70,-1280},(Vec3){320,190,180},yaw,paint,family,2);
  for(int k=0;k<4;k++){Vec3 p={sinf(game.time*.2f+k*2)*250,-70+k*35,cosf(game.time*.18f+k)*830};encounter_lamp(c,p,yaw,RGB(199,221,143));}
 }else if(entry==63){ /* Garden lighthouse: offset observation pod and inhabited refuge tiers. */
  discovery_design_ring(c,900,110,yaw,paint,ENCOUNTER_TILE(family,0),12);
  encounter_box(c,(Vec3){0,-470,0},(Vec3){210,360,240},yaw,paint,family,3);
  for(int k=0;k<3;k++){float a=k*2.094395f;encounter_tank(c,(Vec3){cosf(a)*660,sinf(a)*660,100},180,240,yaw,paint,family);}
  encounter_box(c,(Vec3){0,960,0},(Vec3){110,240,150},yaw,paint,family,2);
  float a=game.time*.38f;encounter_lamp(c,(Vec3){cosf(a)*210,1130,sinf(a)*210},yaw,RGB(106,237,193));
 }else if(entry==17){ /* Cinema: two-sided poster screen, stepped marquee, projection booth. */
  discovery_design_box(c,(Vec3){0,140,0},(Vec3){940,610,100},yaw,paint,ENCOUNTER_TILE(family,1),ENCOUNTER_TILE(family,1),ENCOUNTER_TILE(family,0),0);
  for(int k=0;k<3;k++)encounter_box(c,(Vec3){0,-520-k*110,-100-k*90},(Vec3){1040-k*65,48,180},yaw,paint,family,2);
  encounter_box(c,(Vec3){0,50,280},(Vec3){340,250,180},yaw,paint,family,3);
  for(int side=-1;side<=1;side+=2){encounter_box(c,(Vec3){side*1040,110,0},(Vec3){70,710,170},yaw,paint,family,1);for(int k=0;k<8;k++)encounter_lamp(c,(Vec3){side*1040,-470+k*160,-180},yaw,((int)(game.time*4)+k)&1?RGB(244,173,68):RGB(63,164,195));}
 }else if(entry==12){ /* Closed resort: stacked hotel wings, shuttered atrium, idle wheel. */
  for(int k=0;k<3;k++)for(int side=-1;side<=1;side+=2)encounter_box(c,(Vec3){side*(430+k*85),k*240-190,0},(Vec3){340,105,700-k*100},yaw,paint,family,0);
  encounter_box(c,(Vec3){0,-440,0},(Vec3){1130,110,730},yaw,paint,family,1);
  encounter_box(c,(Vec3){0,150,-230},(Vec3){210,500,280},yaw,paint,family,3);
  discovery_design_ring(add(c,discovery_yaw((Vec3){0,800,0},yaw)),430,55,yaw,paint,ENCOUNTER_TILE(family,2),12);
  if(((int)(game.time*.8f)&3)==0)encounter_lamp(c,(Vec3){0,770,-65},yaw,RGB(240,156,50));
 }else{ /* Mobile mechanic: open repair cradle, service tug, articulated crane. */
  encounter_asteroid_foundation(c,yaw);
  encounter_box(c,(Vec3){0,-340,0},(Vec3){930,130,1100},yaw,paint,family,3);
  for(int side=-1;side<=1;side+=2){encounter_box(c,(Vec3){side*820,0,0},(Vec3){90,380,900},yaw,paint,family,0);encounter_box(c,(Vec3){side*820,440,-430},(Vec3){125,90,480},yaw,paint,family,1);}
  encounter_box(c,(Vec3){0,60,980},(Vec3){580,320,250},yaw,paint,family,2);for(int side=-1;side<=1;side+=2)encounter_tank(c,(Vec3){side*600,-160,460},150,340,yaw,paint,family);
  float swing=sinf(game.time*.22f);Vec3 shoulder={-700,460,-430},elbow={-320,820,-200},wrist={180+swing*100,530,-380};
  encounter_arm(c,shoulder,elbow,55,yaw,paint);encounter_arm(c,elbow,wrist,42,yaw,paint);
  encounter_box(c,elbow,(Vec3){90,90,90},yaw,paint,family,1);
  encounter_box(c,wrist,(Vec3){80,70,80},yaw,paint,family,0);
  for(int side=-1;side<=1;side+=2)encounter_arm(c,add(wrist,(Vec3){side*65,-40,0}),add(wrist,(Vec3){side*45,-160,0}),22,yaw,paint);
  for(int k=0;k<3;k++)encounter_lamp(c,(Vec3){(k-1)*290,210,-1110},yaw,RGB(91,218,221));
 }
 return 1;
}
static void encounter_station(Vec3 c,int hub,StationProfile p){
 int family=hub==1?0:1;float r=p.radius,h=p.half;float yaw=0; /* Dock mouth faces the existing Z-axis approach. */
 unsigned paint=hub==1?RGB(111,142,158):RGB(155,136,107);
 /* Four shoulders leave the existing docking slit clear; rear bulkhead stays opaque. */
 for(int side=-1;side<=1;side+=2){encounter_box(c,(Vec3){side*r*.70f,0,0},(Vec3){r*.29f,r*.72f,h},yaw,paint,family,0);encounter_box(c,(Vec3){0,side*r*.68f,0},(Vec3){r*.42f,r*.25f,h},yaw,paint,family,1);}
 encounter_box(c,(Vec3){0,0,h*.92f},(Vec3){r*.44f,r*.44f,20},yaw,paint,family,3);
 if(hub==1){ /* Paired broadcast sails and a turning receiver head. */
  for(int side=-1;side<=1;side+=2){encounter_box(c,(Vec3){side*r*1.35f,r*.12f,h*.2f},(Vec3){r*.44f,r*.65f,50},yaw,paint,family,1);encounter_box(c,(Vec3){side*r*.92f,0,0},(Vec3){r*.24f,45,70},yaw,paint,family,3);}
  encounter_box(c,(Vec3){0,r*1.02f,0},(Vec3){r*.37f,65,100},yaw+.3f*sinf(game.time*.2f),paint,family,2);
  Vec3 dish={0,r*1.25f,0};float turn=yaw+.3f*sinf(game.time*.2f);
  for(int k=0;k<12;k++){float a=k*.5235988f,b=(k+1)*.5235988f;Vec3 inner={cosf(a)*r*.12f,sinf(a)*r*.12f,0},outer={cosf(a)*r*.85f,sinf(a)*r*.85f,-r*.24f},next={cosf(b)*r*.85f,sinf(b)*r*.85f,-r*.24f},ni={cosf(b)*r*.12f,sinf(b)*r*.12f,0};discovery_design_quad(c,add(dish,inner),add(dish,outer),add(dish,next),add(dish,ni),turn,paint,ENCOUNTER_TILE(family,2),0);}
  encounter_box(c,add(dish,(Vec3){0,0,-r*.5f}),(Vec3){45,45,r*.18f},turn,paint,family,3);

 }else{ /* Frontier workshop: pressure tanks, cargo spine, covered habitation tier. */
  for(int side=-1;side<=1;side+=2){encounter_box(c,(Vec3){side*r*.80f,-r*.8f,h*.3f},(Vec3){r*.35f,r*.25f,h*.75f},yaw,paint,family,3);encounter_box(c,(Vec3){side*r*.7f,r*.75f,0},(Vec3){r*.32f,r*.22f,h*.8f},yaw,paint,family,2);}
 }
 for(int k=-2;k<=2;k++)encounter_lamp(c,(Vec3){k*26,-45,-h-3},yaw,RGB(81,229,211));
}

/* Hybrid presentation keeps the physical silhouette in 3D. Small animated
 * sprite masks supply readable screens/fauna rather than expensive micro-meshes. */
static int encounter_sprite_enabled=1;
static void encounter_sprite(Vec3 world,int kind,float size,unsigned ink){
 static const unsigned short masks[4][8]={{0x0180,0x03c0,0x07e0,0x0db0,0x0990,0x03c0,0x0660,0x0c30},{0x0180,0x03c0,0x07e0,0x0990,0x0db0,0x03c0,0x0660,0x0c30},{0x0180,0x07e0,0x0ff0,0x1ff8,0x3ffc,0x0180,0x0180,0x03c0},{0x0660,0x0ff0,0x03c0,0x0180,0x03c0,0x0660,0x0c30,0x1818}};
 Vec3 v=camera(&game,world);if(v.z<30)return;Point p=project(v);int scale=(int)(size*proj_focal/v.z);if(scale<1)return;if(scale>4)scale=4;unsigned depth=surface_depth_on?surface_encode_depth(1.f/v.z):mega_encode_depth(v.z);
 for(int y=0;y<8;y++)for(int x=0;x<16;x++)if(masks[kind&3][y]&(1u<<(15-x)))for(int yy=0;yy<scale;yy++)for(int xx=0;xx<scale;xx++){int sx=p.x+(x-8)*scale+xx,sy=p.y+(y-4)*scale+yy;if(sx<0||sx>=W||sy<view_top()||sy>=view_bot())continue;if((surface_depth_on||mega_depth_valid)&&depth>surface_depth[sy*W+sx]+16)continue;pixel(sx,sy,ink);}
}
static void encounter_sprite_details(Vec3 c,int entry,float yaw){
 if(!encounter_sprite_enabled)return;
 if(entry==17)encounter_sprite(add(c,discovery_yaw((Vec3){0,140,-110},yaw)),(int)(game.time*2)&1,25,RGB(100,204,216));
 else if(entry==63)for(int i=0;i<3;i++)encounter_sprite(add(c,discovery_yaw((Vec3){(i-1)*370,sinf(game.time*.4f+i)*230, -140},yaw)),2,15,RGB(104,209,157));
 else if(entry==2&&((int)(game.time*5)&3)==0)encounter_sprite(add(c,discovery_yaw((Vec3){0,310,-100},yaw)),3,18,RGB(240,189,95));
}
