/* Authored catalogue silhouettes, mapped functional panels and real sunlight.
 * IDs, locations, scanning and dialogue remain in space-discoveries.h. */
enum {DA_TANK,DA_HABITAT,DA_DOCK,DA_GARDEN,DA_LINER,DA_MONUMENT,DA_CARGO,
 DA_DISH,DA_SALVAGE,DA_LANTERN,DA_BILLBOARD,DA_RING,DA_SPIRE,DA_SAIL,
 DA_CAGE,DA_FAUNA,DA_COMET,DA_DRONE,DA_BUOY};
typedef struct {unsigned char form,modules,accent;float width,height,length;} DiscoveryDesign;
/* Each entry has its own architectural footprint and function. This is not
 * the former four interchangeable wireframe boxes selected by entry%4. */
static const DiscoveryDesign discovery_designs[75]={
 {DA_TANK,3,0,850,310,1100},{DA_HABITAT,5,0,1050,280,760},{DA_DOCK,2,0,800,230,1100},{DA_GARDEN,3,1,740,370,820},
 {DA_LINER,3,0,360,240,1250},{DA_MONUMENT,3,2,660,760,500},{DA_LINER,2,0,470,200,900},{DA_LINER,2,2,620,190,1100},
 {DA_CARGO,4,0,940,280,980},{DA_DOCK,4,0,1100,210,650},{DA_BUOY,3,2,420,640,380},{DA_HABITAT,6,0,820,430,1150},
 {DA_RING,8,0,1100,180,650},{DA_TANK,4,1,1000,390,750},{DA_DISH,2,0,610,620,520},{DA_SALVAGE,5,0,1100,250,1200},
 {DA_LANTERN,7,3,1000,420,800},{DA_BILLBOARD,1,2,960,560,150},{DA_LINER,3,0,480,320,1100},{DA_RING,6,1,1050,180,500},
 {DA_DOCK,4,3,1100,260,980},{DA_SPIRE,4,3,650,1100,620},{DA_RING,10,3,1100,300,800},{DA_GARDEN,5,1,1000,550,920},
 {DA_MONUMENT,4,3,900,560,650},{DA_DOCK,3,2,1200,340,1100},{DA_HABITAT,5,2,1200,440,980},{DA_LINER,5,3,600,510,1400},
 {DA_SAIL,3,3,1100,750,850},{DA_DISH,3,2,850,830,720},{DA_BILLBOARD,2,3,850,650,180},{DA_SPIRE,3,3,510,700,650},
 {DA_GARDEN,3,1,860,540,800},{DA_CARGO,5,2,1000,370,1000},{DA_HABITAT,3,3,730,320,1000},{DA_DOCK,5,3,1200,250,950},
 {DA_RING,6,1,960,280,760},{DA_CARGO,4,3,1100,450,1100},{DA_LINER,4,1,510,430,1250},{DA_HABITAT,6,1,1060,500,900},
 {DA_DOCK,6,2,1250,450,1200},{DA_DOCK,4,0,1300,650,1300},{DA_SALVAGE,6,3,1250,500,1150},{DA_HABITAT,7,3,1250,700,1150},
 {DA_RING,10,3,1250,420,1000},{DA_DOCK,6,2,1300,280,1300},{DA_DISH,4,2,1000,1100,800},{DA_RING,12,2,1300,320,900},
 {DA_HABITAT,8,1,1250,650,1150},{DA_RING,8,0,1000,280,800},{DA_MONUMENT,5,3,1000,1200,800},{DA_SPIRE,6,3,760,1400,700},
 {DA_HABITAT,4,3,1200,400,1100},{DA_CARGO,6,2,1250,650,1250},{DA_LINER,6,3,730,600,1500},{DA_RING,12,1,1200,300,880},
 {DA_BILLBOARD,5,3,1200,600,400},{DA_RING,10,1,1100,450,750},{DA_DOCK,5,2,1250,350,1200},{DA_HABITAT,7,3,1200,270,1050},
 {DA_DISH,3,0,1040,1500,1040},{DA_SALVAGE,6,0,940,1030,2700},{DA_CAGE,5,0,900,520,1300},{DA_RING,8,1,900,180,430},
 {DA_FAUNA,5,1,850,420,1000},{DA_COMET,3,2,800,600,950},{DA_BUOY,2,2,320,700,360},{DA_DRONE,1,2,400,250,500},
 {DA_CARGO,6,0,500,300,1500},{DA_TANK,4,0,750,450,1350},{DA_DISH,2,2,550,700,500},{DA_LINER,2,3,450,220,1100},
 {DA_BILLBOARD,2,3,1100,480,800},{DA_DRONE,4,0,1000,300,1000},{DA_SALVAGE,3,0,700,400,1100}
};
static unsigned discovery_paint(int accent){static const unsigned c[]={0x006b7983,0x006b8d72,0x008f8a78,0x0096a4af};return c[accent&3];}
static void discovery_design_light(Vec3 centre,Vec3 *out_sun,Vec3 *out_half){
 static Vec3 last_c,last_eye,last_star,sun,half;static int light_ready;
 Vec3 eye=camera_origin(&game),star=game.bodies[0].pos;
 if(!light_ready||memcmp(&last_c,&centre,sizeof(Vec3))||memcmp(&last_eye,&eye,sizeof(Vec3))||memcmp(&last_star,&star,sizeof(Vec3))){
  light_ready=1;last_c=centre;last_eye=eye;last_star=star;sun=norm(sub(star,centre));half=norm(add(sun,norm(sub(eye,centre))));
 }
 *out_sun=sun;*out_half=half;
}
static void discovery_design_quad(Vec3 centre,Vec3 a,Vec3 b,Vec3 c,Vec3 d,float yaw,unsigned paint,int tile,int bevel){
 a=add(centre,discovery_yaw(a,yaw));b=add(centre,discovery_yaw(b,yaw));c=add(centre,discovery_yaw(c,yaw));d=add(centre,discovery_yaw(d,yaw));
 Vec3 u=sub(b,a),v=sub(c,a),normal=norm((Vec3){u.y*v.z-u.z*v.y,u.z*v.x-u.x*v.z,u.x*v.y-u.y*v.x});
 if(dot(normal,sub(camera_origin(&game),a))<=0)return;
 Vec3 sun,half;discovery_design_light(centre,&sun,&half);
 unsigned ink=ship_art_light(paint,normal,sun,half,tile);
 fleet_panel_quad(camera(&game,a),camera(&game,b),camera(&game,c),camera(&game,d),ink,tile,bevel);
}
static void discovery_design_box(Vec3 c,Vec3 p,Vec3 size,float yaw,unsigned paint,int side,int roof,int end,int bevel){
 Vec3 q[8],view[8];for(int i=0;i<8;i++){q[i]=add(c,discovery_yaw(add(p,(Vec3){i&1?size.x:-size.x,i&2?size.y:-size.y,i&4?size.z:-size.z}),yaw));view[i]=camera(&game,q[i]);}
 /* Outward faces. Unlike the old wire pass these write actual surface depth. */
 static const unsigned char f[6][4]={{2,3,1,0},{7,6,4,5},{6,2,0,4},{3,7,5,1},{0,1,5,4},{6,7,3,2}};
 static const Vec3 axis[6]={{0,0,-1},{0,0,1},{-1,0,0},{1,0,0},{0,-1,0},{0,1,0}};
 Vec3 sun,half,eye=camera_origin(&game);discovery_design_light(c,&sun,&half);
 for(int k=0;k<6;k++){Vec3 n=discovery_yaw(axis[k],yaw);if(dot(n,sub(eye,q[f[k][0]]))<=0)continue;int tile=k<2?end:k==5?roof:side;if(end==FLEET_ENGINE&&k==1)tile=FLEET_SERVICE;unsigned ink=ship_art_light(paint,n,sun,half,tile);fleet_panel_quad(view[f[k][0]],view[f[k][1]],view[f[k][2]],view[f[k][3]],ink,tile,bevel);}
}
static void discovery_design_ring(Vec3 c,float radius,float depth,float yaw,unsigned paint,int tile,int sectors){
 for(int k=0;k<sectors;k++){float a=k*6.2831853f/sectors,b=(k+1)*6.2831853f/sectors,r=radius*.82f;
  Vec3 q[8]={{cosf(a)*radius,sinf(a)*radius,-depth},{cosf(b)*radius,sinf(b)*radius,-depth},{cosf(b)*r,sinf(b)*r,-depth},{cosf(a)*r,sinf(a)*r,-depth},
  {cosf(a)*radius,sinf(a)*radius,depth},{cosf(b)*radius,sinf(b)*radius,depth},{cosf(b)*r,sinf(b)*r,depth},{cosf(a)*r,sinf(a)*r,depth}};
  discovery_design_quad(c,q[3],q[2],q[1],q[0],yaw,paint,tile,1);discovery_design_quad(c,q[4],q[5],q[6],q[7],yaw,paint,tile,1);
  discovery_design_quad(c,q[0],q[1],q[5],q[4],yaw,paint,FLEET_ARMOUR,0);discovery_design_quad(c,q[2],q[3],q[7],q[6],yaw,paint,FLEET_WINDOWS,0);
 }
}
static void discovery_catalogue_model(Vec3 c,int entry,float yaw){
 const DiscoveryDesign*d=&discovery_designs[entry];float w=d->width,h=d->height,z=d->length;unsigned paint=discovery_paint(d->accent);
 float px=render_pixel_radius(c,2200);int detail=px>25,n=d->modules;if(px<10)n=n>3?3:n;
 int body=d->form==DA_GARDEN?FLEET_GARDEN:d->form==DA_TANK?FLEET_TANK:d->form==DA_CARGO?FLEET_CARGO:FLEET_ARMOUR;
 if(d->form==DA_RING){
  discovery_design_ring(c,w,h*.5f,yaw,paint,entry==55||entry==63?FLEET_GARDEN:FLEET_WINDOWS,detail?16:8);
  discovery_design_box(c,(Vec3){0,0,0},(Vec3){w*.12f,w*.12f,z*.65f},yaw,paint,FLEET_SERVICE,FLEET_GLASS,FLEET_HANGAR,detail);
  for(int k=0;k<4;k++){float a=k*1.5707963f;Vec3 p={cosf(a)*w*.47f,sinf(a)*w*.47f,0};discovery_design_box(c,p,(Vec3){k&1?w*.045f:w*.37f,k&1?w*.37f:w*.045f,h*.18f},yaw,paint,FLEET_SERVICE,FLEET_ARMOUR,FLEET_SERVICE,0);}
 }else if(d->form==DA_DOCK){
  discovery_design_box(c,(Vec3){0,-h*.7f,0},(Vec3){w,h*.3f,z},yaw,paint,FLEET_WINDOWS,FLEET_HANGAR,FLEET_SERVICE,detail);
  for(int k=0;k<n;k++){float at=-z*.8f+z*1.6f*k/fmaxf(1,n-1);for(int s=-1;s<=1;s+=2){discovery_design_box(c,(Vec3){s*w*.9f,h*.3f,at},(Vec3){w*.10f,h,z*.07f},yaw,paint,FLEET_SERVICE,FLEET_ARMOUR,FLEET_WINDOWS,detail);discovery_design_box(c,(Vec3){s*w*.69f,h*1.25f,at},(Vec3){w*.32f,h*.10f,z*.07f},yaw,paint,FLEET_RADIATOR,FLEET_ARMOUR,FLEET_SERVICE,0);}}
 }else if(d->form==DA_SPIRE||d->form==DA_MONUMENT){
  for(int k=0;k<n;k++){float t=k/(float)n,r=w*(1-t*.68f);discovery_design_box(c,(Vec3){0,-h+h*2*t,0},(Vec3){r,h/n*.75f,z*(1-t*.57f)},yaw+k*.11f,paint,FLEET_WINDOWS,FLEET_ARMOUR,FLEET_WINDOWS,detail);}
  discovery_design_box(c,(Vec3){0,h,0},(Vec3){w*.14f,h*.24f,z*.17f},yaw,paint,FLEET_GLASS,FLEET_ANTENNA,FLEET_GLASS,detail);
 }else if(d->form==DA_DISH||d->form==DA_BUOY){
  discovery_design_box(c,(Vec3){0,-h*.25f,0},(Vec3){w*.34f,h*.55f,z*.6f},yaw,paint,FLEET_SERVICE,FLEET_ANTENNA,FLEET_WINDOWS,detail);
  for(int k=0;k<n;k++){float a=k*6.2831853f/n;Vec3 p={cosf(a)*w*.72f,h*.4f,sinf(a)*z*.7f};discovery_design_box(c,p,(Vec3){w*.26f,h*.08f,z*.3f},yaw,paint,FLEET_SERVICE,FLEET_SOLAR,FLEET_ANTENNA,detail);}
  discovery_design_ring(add(c,discovery_yaw((Vec3){0,h*.7f,0},yaw)),w*.55f,h*.09f,yaw,paint,FLEET_ANTENNA,detail?12:8);
 }else if(d->form==DA_SAIL||d->form==DA_BILLBOARD){
  discovery_design_box(c,(Vec3){0,-h*.4f,0},(Vec3){w*.16f,h*.6f,z*.9f},yaw,paint,FLEET_SERVICE,FLEET_ARMOUR,FLEET_WINDOWS,detail);
  for(int k=0;k<n;k++){float at=(k-(n-1)*.5f)*w*.65f;discovery_design_box(c,(Vec3){at,h*.3f,k%2*z*.3f},(Vec3){w*.29f,h*.55f,30},yaw,paint,FLEET_SERVICE,FLEET_ARMOUR,d->form==DA_SAIL?FLEET_SOLAR:FLEET_WINDOWS,detail);}
 }else if(d->form==DA_LANTERN){
  for(int k=0;k<n;k++){float a=k*6.2831853f/n;Vec3 p={cosf(a)*w,sinf(a)*h,cosf(a*2)*z*.5f};discovery_design_box(c,p,(Vec3){45,90,45},yaw,paint,FLEET_WINDOWS,FLEET_ARMOUR,FLEET_GLASS,detail);}
 }else if(d->form==DA_CAGE){
  for(int s=-1;s<=1;s+=2)for(int y=-1;y<=1;y+=2)discovery_design_box(c,(Vec3){s*w,y*h,0},(Vec3){w*.06f,h*.08f,z},yaw,paint,FLEET_SERVICE,FLEET_ARMOUR,FLEET_SERVICE,detail);
  for(int k=0;k<5;k++)for(int s=-1;s<=1;s+=2)discovery_design_box(c,(Vec3){s*w,0,(k-2)*z*.48f},(Vec3){w*.04f,h,z*.035f},yaw,paint,FLEET_SERVICE,FLEET_ARMOUR,FLEET_SERVICE,0);
  discovery_design_box(c,(Vec3){0,-h,0},(Vec3){w,h*.06f,z},yaw,paint,FLEET_SERVICE,FLEET_GARDEN,FLEET_SERVICE,detail);
 }else if(d->form==DA_FAUNA||d->form==DA_COMET){
  /* Faceted shells/ice, not industrial crates. Individual lobes occupy depth. */
  for(int k=0;k<n;k++){float a=k*2.399963f+game.time*.06f;Vec3 p={cosf(a)*w*.65f,sinf(a)*h*.65f,(k-(n-1)*.5f)*z*.32f};
   Vec3 tip=add(p,(Vec3){0,h*.24f,z*.17f}),tail=add(p,(Vec3){0,-h*.19f,-z*.24f});
   for(int j=0;j<8;j++){float u=j*.785398f,v=(j+1)*.785398f;Vec3 b=add(p,(Vec3){cosf(u)*w*.23f,sinf(u)*h*.18f,0}),q=add(p,(Vec3){cosf(v)*w*.23f,sinf(v)*h*.18f,0});discovery_design_quad(c,tip,b,q,tip,yaw,paint,FLEET_ALIEN,0);discovery_design_quad(c,tail,q,b,tail,yaw,paint,FLEET_CERAMIC,0);}}
 }else{
  /* Pressure spine, separate fitted pods and raised inhabited command deck. */
  discovery_design_box(c,(Vec3){0,-h*.3f,0},(Vec3){w*.24f,h*.42f,z},yaw,paint,FLEET_SERVICE,FLEET_ARMOUR,FLEET_ENGINE,detail);
  for(int k=0;k<n;k++){float at=(k-(n-1)*.5f)*z*1.5f/fmaxf(1,n-1);float side=k&1?1:-1;
   Vec3 p={side*w*.53f,sinf(k*1.1f)*h*.18f,at};
   if(d->form==DA_LINER)p=(Vec3){0,h*(.28f+k*.1f),at};
   discovery_design_box(c,p,(Vec3){w*(d->form==DA_LINER?.9f:.42f),h*.47f,z/(n+1)*.77f},yaw+(d->form==DA_SALVAGE?(k-2)*.13f:0),paint,body,d->form==DA_GARDEN?FLEET_GARDEN:FLEET_ARMOUR,d->form==DA_HABITAT||d->form==DA_LINER?FLEET_WINDOWS:body,detail);
  }
  discovery_design_box(c,(Vec3){0,h*.95f,-z*.55f},(Vec3){w*.32f,h*.22f,z*.18f},yaw,paint,FLEET_WINDOWS,FLEET_ARMOUR,FLEET_GLASS,detail);
 }
}
