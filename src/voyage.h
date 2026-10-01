static void sector_background(void){
 /* Shared 1950s-cover field: broad stepped colour planes first, then the
  * softer nebula/star passes. Keep the centre quiet for the reticle and HUD;
  * the large side mass and tiny relay give the frame a period illustration
  * composition without becoming gameplay geometry. */
 const unsigned colors[]={RGB(4,10,24),RGB(22,7,29),RGB(3,25,29),RGB(31,13,5),RGB(8,15,38),RGB(32,7,20),RGB(5,18,20),RGB(27,15,4),RGB(10,7,31),RGB(5,22,34),RGB(29,12,32),RGB(17,22,7)};
 unsigned seed=space_sky_seed();
 unsigned raw=colors[(seed>>27)%12],regional=colors[(seed>>19)%12];
 float zone=sinf((game.pos.x+game.pos.z)*.00007f+(seed&255)*.03f),field=.5f+.5f*sinf(game.pos.x*.000035f+(seed>>8)) * cosf(game.pos.z*.000041f+(seed>>17));
 raw=mix_rgb(raw,regional,.08f+field*.34f);float glow=.78f+(zone+1)*.10f+sinf(game.time*.045f+(seed&63))*0.025f;
 unsigned base=RGB((int)((raw&255)*glow),(int)(((raw>>8)&255)*glow),(int)(((raw>>16)&255)*glow));
 int top=view_top(),bot=view_bot();
 rect(0,top,W,bot-top+1,base);
 /* Three stepped bands create the inked sky/painted wash of a cover, while
    staying cheap enough for the PSP framebuffer and high-contrast mode. */
 unsigned upper=RGB((base&255)/2,((base>>8)&255)/2,((base>>16)&255)/2);
 unsigned lower=RGB((int)((base&255)*1.12f),(int)(((base>>8)&255)*1.08f),(int)(((base>>16)&255)*.94f));
 int horizon=top+(bot-top)*(2+(seed&3))/6;
 for(int y=top;y<horizon;y+=4)rect(0,y,W,4,mix_rgb(upper,base,(y-top)/(float)fmaxf(1,horizon-top)));
 for(int y=horizon;y<=bot;y+=4)rect(0,y,W,4,mix_rgb(base,lower,(y-horizon)/(float)fmaxf(1,bot-horizon)));
 /* Fine two-colour dithering keeps the dark field textured like painted
  * pixel art without filling it with gameplay-like points. */
 for(int i=0;i<72;i++){unsigned h=field_hash(seed+(unsigned)i*0x27d4eb2du);int x=(h>>9)%W,y=top+((h>>20)%(bot-top+1));unsigned c=fb[y*STRIDE+x];fb[y*STRIDE+x]=RGB((c&255)+((c&255)<18?2:0),((c>>8)&255)+(((c>>8)&255)<18?2:0),((c>>16)&255)+(((c>>16)&255)<28?3:0));}
}
static Vec3 station_vertex_at(Vec3 v,Vec3 center,float angle){return add(rotate(v,0,angle),center);}
static void station_quad_at(Vec3 a,Vec3 b,Vec3 c,Vec3 d,Vec3 center,float angle,unsigned ink){
 queue_triangle(camera(&game,station_vertex_at(a,center,angle)),camera(&game,station_vertex_at(b,center,angle)),camera(&game,station_vertex_at(c,center,angle)),ink);
 queue_triangle(camera(&game,station_vertex_at(a,center,angle)),camera(&game,station_vertex_at(c,center,angle)),camera(&game,station_vertex_at(d,center,angle)),ink);
}
static void station_quad(Vec3 a,Vec3 b,Vec3 c,Vec3 d,unsigned ink){
 station_quad_at(a,b,c,d,(Vec3){0,0,STATION_Z},station_angle(&game),ink);
}
static void station_box_at(Vec3 c,Vec3 e,Vec3 center,float angle,unsigned ink){
 Vec3 v[8];for(int i=0;i<8;i++)v[i]=(Vec3){c.x+(i&1?e.x:-e.x),c.y+(i&2?e.y:-e.y),c.z+(i&4?e.z:-e.z)};
 const int f[6][4]={{0,1,3,2},{4,6,7,5},{0,4,5,1},{2,3,7,6},{0,2,6,4},{1,5,7,3}};
 for(int i=0;i<6;i++)station_quad_at(v[f[i][0]],v[f[i][1]],v[f[i][2]],v[f[i][3]],center,angle,livery_tint(ink,i==1?12:i>3?-12:0));
}
static void station_box(Vec3 c,Vec3 e,unsigned ink){station_box_at(c,e,(Vec3){0,0,STATION_Z},station_angle(&game),ink);}
#include "mega-city-render.h"
static void station_model(void){
 if(!mc_legacy_review){mega_city_model();return;}
 StationProfile p=station_profile_for(&game,0);float angle=station_angle(&game),scale=p.radius/160.f;
 shipmesh_stretched(mesh_id("CORIOLIS"),(Vec3){0,0,STATION_Z},0,angle,scale,p.half/p.radius,p.hull,0);
 Vec3 outer[4]={{0,-p.radius,-p.half},{p.radius,0,-p.half},{0,p.radius,-p.half},{-p.radius,0,-p.half}};
 Vec3 inner[4];for(int i=0;i<4;i++)inner[i]=station_port_corner_for(&game,0,i);
 float door_z=p.half-24.f;
 for(int i=0;i<4;i++){
  int j=(i+1)%4;station_quad(outer[i],outer[j],inner[j],inner[i],livery_tint(p.hull,-6));
  Vec3 backi=inner[i],backj=inner[j];backi.z=door_z;backj.z=door_z;
  station_quad(inner[i],inner[j],backj,backi,RGB(8,19,24));
 }
 /* A real opaque pressure door closes the rear of the flight tunnel. The old
    open tube exposed stars and planets through the opposite side of the hub. */
 Vec3 dl={-STATION_PORT_HALF_W,-STATION_PORT_HALF_H,door_z},dr={STATION_PORT_HALF_W,-STATION_PORT_HALF_H,door_z};
 Vec3 ur={STATION_PORT_HALF_W,STATION_PORT_HALF_H,door_z},ul={-STATION_PORT_HALF_W,STATION_PORT_HALF_H,door_z};
 station_quad(dl,dr,ur,ul,RGB(20,29,36));
 station_box((Vec3){0,0,door_z-1},(Vec3){3,STATION_PORT_HALF_H,2},p.trim);
 station_box((Vec3){0,0,door_z-2},(Vec3){STATION_PORT_HALF_W,2,2},livery_tint(p.trim,-18));
 for(int lamp=-2;lamp<=2;lamp++)station_box((Vec3){lamp*22.f,STATION_PORT_HALF_H-5,door_z-4},(Vec3){3,2,1},p.light);
 if(p.station_class==STATION_MEGA){
  /* Four more black apertures are inset across the capital's forward face.
     They share the same safe slit dimensions as the central port. */
  for(int port=1;port<station_port_count_for(&game,0);port++){
   Vec3 o=station_port_offset_for(&game,0,port);float z=-p.half-3.f;
   station_quad((Vec3){o.x-STATION_PORT_HALF_W,o.y-STATION_PORT_HALF_H,z},(Vec3){o.x+STATION_PORT_HALF_W,o.y-STATION_PORT_HALF_H,z},(Vec3){o.x+STATION_PORT_HALF_W,o.y+STATION_PORT_HALF_H,z},(Vec3){o.x-STATION_PORT_HALF_W,o.y+STATION_PORT_HALF_H,z},RGB(4,9,16));
   station_box((Vec3){o.x,o.y-STATION_PORT_HALF_H,z-2},(Vec3){STATION_PORT_HALF_W+8,4,3},p.light);
   station_box((Vec3){o.x,o.y+STATION_PORT_HALF_H,z-2},(Vec3){STATION_PORT_HALF_W+8,4,3},p.light);
   station_box((Vec3){o.x-STATION_PORT_HALF_W,o.y,z-2},(Vec3){4,STATION_PORT_HALF_H,3},p.trim);
   station_box((Vec3){o.x+STATION_PORT_HALF_W,o.y,z-2},(Vec3){4,STATION_PORT_HALF_H,3},p.trim);
  }
  /* Tiered city blocks and illuminated commercial spines make the capital
     dwarf ordinary ports even before its advertising traffic is visible. */
  for(int tier=0;tier<4;tier++)for(int side=-1;side<=1;side+=2){float r=p.radius*(.42f+tier*.11f);station_box((Vec3){side*r,side*(tier&1)*p.radius*.18f,p.half*.18f-tier*p.half*.12f},(Vec3){p.radius*.12f,p.radius*.08f,p.half*.28f},tier&1?p.light:p.trim);}
  for(int blimp=0;blimp<2;blimp++){float a=game.time*(blimp?.018f:-.014f)+blimp*3.1f;Vec3 bp={cosf(a)*p.radius*1.55f,sinf(a)*p.radius*.72f,STATION_Z-p.half*.35f+blimp*260};shipmesh_stretched(mesh_id("TRANSPORTER"),bp,a+1.57f,0,9.f,2.6f,blimp?p.light:p.trim,0);}
 }
 /* Six inexpensive silhouette families, then seed-driven bands/pods within
    the same collision envelope. Scale, depth and colours vary independently. */
 if(p.family==0){for(int s=-1;s<=1;s+=2)station_box((Vec3){s*p.radius*.67f,0,p.half*.05f},(Vec3){p.radius*.16f,p.radius*.34f,p.half*.55f},p.trim);}
 else if(p.family==1){for(int s=-1;s<=1;s+=2)station_box((Vec3){0,s*p.radius*.66f,-p.half*.08f},(Vec3){p.radius*.42f,p.radius*.13f,p.half*.18f},p.trim);}
 else if(p.family==2){for(int i=0;i<p.pods;i++){float a=i*6.2831853f/p.pods;station_box((Vec3){cosf(a)*p.radius*.68f,sinf(a)*p.radius*.68f,p.half*.12f},(Vec3){p.radius*.10f,p.radius*.10f,p.half*.24f},i&1?p.trim:p.light);}}
 else if(p.family==3){station_box((Vec3){0,0,p.half*.48f},(Vec3){p.radius*.72f,p.radius*.18f,p.half*.13f},p.trim);station_box((Vec3){0,0,-p.half*.20f},(Vec3){p.radius*.18f,p.radius*.72f,p.half*.11f},p.light);}
 else if(p.family==4){for(int s=-1;s<=1;s+=2){station_box((Vec3){s*p.radius*.53f,0,p.half*.32f},(Vec3){p.radius*.27f,p.radius*.10f,p.half*.11f},p.trim);station_box((Vec3){0,s*p.radius*.53f,-p.half*.15f},(Vec3){p.radius*.10f,p.radius*.27f,p.half*.11f},p.light);}}
 else {station_box((Vec3){0,p.radius*.57f,p.half*.08f},(Vec3){p.radius*.38f,p.radius*.15f,p.half*.32f},p.trim);station_box((Vec3){0,-p.radius*.58f,-p.half*.18f},(Vec3){p.radius*.25f,p.radius*.13f,p.half*.24f},p.light);}
 for(int band=0;band<p.bands;band++){
  float z=-p.half*.45f+band*(p.half*.9f/fmaxf(1,p.bands-1));
  station_box((Vec3){0,p.radius*.76f,z},(Vec3){p.radius*.34f,p.radius*.035f,p.half*.035f},band&1?p.light:p.trim);
 }
}
/* Practical station windows — soft warm lamps only (no glitter masks). */
static void station_window_animation(void){
 if(!mc_legacy_review)return; /* New facades carry their own depth-tested windows. */
 if(station_class(&game)==STATION_MEGA)return;
 if(high_contrast)return;
 StationProfile profile=station_profile_for(&game,0);
 int top=clipy0>=0?clipy0:view_top(),bot=clipy1>=0?clipy1-1:view_bot();
 int count=6+profile.pods;
 for(int i=0;i<count;i++){
  float a=station_angle(&game)+(i+.5f)*6.2831853f/count;
  float radius=profile.radius*(.67f+(i&1)*.12f);
  Vec3 v=camera(&game,(Vec3){cosf(a)*radius,sinf(a)*radius,STATION_Z+((i&3)-1)*profile.half*.24f});
  if(v.z<20)continue;
  Point p=project(v);if(p.x<2||p.x>=W-2||p.y<top||p.y>bot)continue;
  unsigned c=((i+(int)(game.time*1.4f))&3)?livery_tint(profile.light,-55):profile.light;
  sfx_add((int)p.x,(int)p.y,c,top,bot);
 }
}
/* Screen-space lettering is anchored to projected hull panels.  Keeping the
 * words out of the mesh avoids new textures and remains cheap on PSP. */
static void mega_capital_signs(void){ /* Capital signage is now depth-tested facade art. */ }
static void secondary_hubs(void);
#include "menu-ship-preview.h"
static void ambient_space(void){
 if(system_whales(game.system)){Body *b=&game.bodies[3];for(int i=0;i<3;i++){float a=game.time*.028f+i*.62f;Vec3 pos=add(b->pos,(Vec3){cosf(a)*(b->radius+5600),700+sinf(a+i)*.5f*480,sinf(a)*(b->radius+5600)});if(length(sub(pos,game.pos))<14000)shipmesh(mesh_id("WORM"),pos,a+1.57f,sinf(game.time*.35f+i)*.16f,7.2f+i*1.3f,RGB(96,186,198),0);}}
 if(system_comet(game.system)){float a=game.time*.018f;Vec3 pos={cosf(a)*17000,1800,sinf(a)*17000};if(length(sub(pos,game.pos))<12000){shipmesh(mesh_id("BOULDER"),pos,a,a*.3f,1.4f,RGB(210,230,240),0);Vec3 tail=add(pos,(Vec3){sinf(a)*900,-200,-cosf(a)*900});Vec3 u=camera(&game,pos),v=camera(&game,tail);if(u.z>30&&v.z>30){Point p=project(u),q=project(v);if(p.y>view_top()&&p.y<view_bot()&&q.y>view_top()&&q.y<view_bot()){line((int)p.x,(int)p.y,(int)q.x,(int)q.y,RGB(85,160,200));if(!high_contrast){int top=view_top(),bot=view_bot();for(int k=0;k<5;k++){float t=k/4.f;int x=(int)(p.x+(q.x-p.x)*t),y=(int)(p.y+(q.y-p.y)*t);sfx_add(x,y,RGB(50,90,120),top,bot);}}}}}}
}
/* Decorative deep-space traffic deliberately lives outside the NPC system:
 * it cannot be scanned, targeted, collided with, or consume an encounter
 * slot.  These are the enormous freighters seen crossing a sector many
 * kilometres beyond normal flight space. */
static Vec3 deep_traffic_position(int lane,float *heading,float *phase){
 unsigned seed=game.bodies[0].seed^(unsigned)(game.system*0x45d9f3bu+lane*0x9e3779b9u);
 /* These are immense ships crossing the far sky, not nearby fighters. A full
    passage takes several minutes and moves by less than a pixel most seconds. */
 float cycle=150.f+(float)(seed%81),t=fmodf(game.time+(seed>>8)%149,cycle)/cycle;
 float a=(float)(seed%6283)*.001f+t*.24f;
 float elevation=-.62f+(float)((seed>>16)%125)*.01f;
 float radius=68000.f+(float)((seed>>23)%18000);
 Vec3 radial=norm((Vec3){cosf(a),elevation,sinf(a)});
 Vec3 side=norm((Vec3){-sinf(a),0,cosf(a)});
 /* It is a sky route rather than a reachable flight path: player movement
    never brings the set dressing into scanner, weapons, or collision range. */
 float crossing=(t-.5f)*28000.f;
 if(heading)*heading=atan2f(side.x,side.z);
 if(phase)*phase=t;
 return add(game.pos,add(mul(radial,radius),mul(side,crossing)));
}
static unsigned deep_traffic_color(unsigned seed,int lane,int accent){
 static const unsigned hulls[]={RGB(64,112,150),RGB(142,73,108),RGB(122,91,55),RGB(55,126,112),RGB(108,80,154),RGB(154,76,52)};
 static const unsigned trims[]={RGB(76,213,236),RGB(246,94,174),RGB(247,178,70),RGB(83,231,173),RGB(173,112,247),RGB(255,111,69)};
 int style=(seed/17u+(unsigned)lane*3u)%6;return accent?trims[style]:hulls[style];
}
/* Large modular boxes remain readable at 70-85 km and let distant freighters
 * have genuinely different silhouettes without adding texture memory. */
static void deep_traffic_box(Vec3 pos,Vec3 facing,Vec3 center,Vec3 half,unsigned ink){
 Vec3 right=norm((Vec3){facing.z,0,-facing.x}),up={0,1,0},world[8];
 for(int i=0;i<8;i++){Vec3 local=mul(add(center,(Vec3){i&2?half.x:-half.x,i&4?half.y:-half.y,i&1?half.z:-half.z}),2.10f);world[i]=add(pos,add(mul(right,local.x),add(mul(up,local.y),mul(facing,local.z))));}
 const int faces[6][4]={{0,2,6,4},{1,5,7,3},{0,4,5,1},{2,3,7,6},{0,1,3,2},{4,6,7,5}};
 Vec3 normals[6]={mul(facing,-1),facing,mul(right,-1),right,mul(up,-1),up};
 for(int face=0;face<6;face++){
  Vec3 a=world[faces[face][0]],b=world[faces[face][1]],c=world[faces[face][2]],d=world[faces[face][3]];
  if(dot(normals[face],sub(game.pos,a))<=0)continue;
  unsigned shade=livery_tint(ink,face==5?28:face==4?-34:face<2?-12:0);
  queue_triangle(camera(&game,a),camera(&game,b),camera(&game,c),shade);queue_triangle(camera(&game,a),camera(&game,c),camera(&game,d),shade);
 }
}
static void deep_traffic_wedge(Vec3 pos,Vec3 facing,Vec3 center,Vec3 half,unsigned ink){
 Vec3 right=norm((Vec3){facing.z,0,-facing.x}),up={0,1,0},local[6]={{0,0,half.z},{0,0,-half.z},{-half.x,0,0},{half.x,0,0},{0,half.y,0},{0,-half.y,0}},point[6];
 for(int i=0;i<6;i++){Vec3 p=mul(add(center,local[i]),2.10f);point[i]=camera(&game,add(pos,add(mul(right,p.x),add(mul(up,p.y),mul(facing,p.z)))));}
 const int face[8][3]={{0,2,4},{0,4,3},{0,3,5},{0,5,2},{1,4,2},{1,3,4},{1,5,3},{1,2,5}};
 for(int i=0;i<8;i++)queue_triangle(point[face[i][0]],point[face[i][1]],point[face[i][2]],livery_tint(ink,i<4?18:-20));
}
static void deep_traffic_model(Vec3 pos,float heading,int design,unsigned hull,unsigned trim){
 Vec3 f={sinf(heading),0,cosf(heading)};
 /* Every design shares a long drive spine and luminous bridge, then gets a
    distinct cargo architecture: container train, tanker, wing carrier or ark. */
 deep_traffic_box(pos,f,(Vec3){0,0,0},(Vec3){310,210,2300},livery_tint(hull,-25));
 deep_traffic_box(pos,f,(Vec3){0,260,-1500},(Vec3){420,180,250},trim);
 deep_traffic_wedge(pos,f,(Vec3){0,0,2050},(Vec3){520,300,600},trim);
 if(design==0){
  for(int row=0;row<4;row++)for(int side=-1;side<=1;side+=2){unsigned c=row&1?hull:livery_tint(trim,-24);deep_traffic_box(pos,f,(Vec3){side*650.f,0,-900+row*610.f},(Vec3){430,360,250},c);}
 }else if(design==1){
  for(int row=0;row<3;row++)for(int side=-1;side<=1;side+=2){deep_traffic_box(pos,f,(Vec3){side*600.f,0,-720+row*760.f},(Vec3){390,430,330},row==1?trim:hull);}
  deep_traffic_box(pos,f,(Vec3){0,-330,350},(Vec3){820,130,1080},livery_tint(trim,-18));
 }else if(design==2){
  deep_traffic_box(pos,f,(Vec3){0,-40,250},(Vec3){1420,135,920},hull);
  for(int side=-1;side<=1;side+=2){deep_traffic_wedge(pos,f,(Vec3){side*1450.f,80,420},(Vec3){520,260,620},trim);deep_traffic_box(pos,f,(Vec3){side*720.f,-170,-850},(Vec3){240,180,540},livery_tint(hull,20));}
 }else{
  deep_traffic_box(pos,f,(Vec3){0,40,300},(Vec3){900,400,1260},hull);
  for(int side=-1;side<=1;side+=2){deep_traffic_box(pos,f,(Vec3){side*720.f,350,200},(Vec3){250,520,700},trim);deep_traffic_box(pos,f,(Vec3){side*760.f,-320,650},(Vec3){300,180,520},livery_tint(trim,-20));}
  deep_traffic_box(pos,f,(Vec3){0,720,650},(Vec3){260,400,460},livery_tint(hull,26));
 }
}
static void deep_traffic(void){
 if(game.jump>0||game.dock_stage||game.dead)return;
 unsigned seed=game.bodies[0].seed^(unsigned)game.system;
 int count=2+(seed&1);
 for(int lane=0;lane<count;lane++){
  float heading,phase;Vec3 pos=deep_traffic_position(lane,&heading,&phase);
  /* Hide the hull during the first/last moments so each passage reads as a
     distant jump rather than a ship appearing from nowhere. */
  if(phase<.055f||phase>.945f||occluded(pos))continue;
  int design=(int)((seed>>5)+lane+game.system)%4;
  deep_traffic_model(pos,heading,design,deep_traffic_color(seed,lane,0),deep_traffic_color(seed,lane,1));
 }
}
/* Called immediately after the distant hull flush and before planets. Long,
 * layered exhaust therefore sits behind worlds together with its parent ship. */
static void deep_traffic_lights(void){
 if(game.jump>0||game.dock_stage||game.dead)return;
 unsigned seed=game.bodies[0].seed^(unsigned)game.system;int count=2+(seed&1),top=view_top(),bot=view_bot();
 for(int lane=0;lane<count;lane++){
  float heading,phase;Vec3 pos=deep_traffic_position(lane,&heading,&phase),cam=camera(&game,pos);
  if(phase<.055f||phase>.945f||occluded(pos)||cam.z<80)continue;Point p=project(cam);if(p.x<4||p.x>=W-4||p.y<top+3||p.y>bot-3)continue;
  Vec3 facing={sinf(heading),0,cosf(heading)},right=norm((Vec3){facing.z,0,-facing.x});unsigned glow=deep_traffic_color(seed,lane,1);
  /* Twin thick fire streams: dim wide sheath, saturated body, white-hot root. */
  for(int engine=-1;engine<=1;engine+=2){Vec3 root=add(add(pos,mul(facing,-4900.f)),mul(right,engine*1080.f));Vec3 root_cam=camera(&game,root);if(root_cam.z<=80)continue;Point last=project(root_cam);
   for(int segment=1;segment<=8;segment++){float distance=4900.f+segment*2500.f,wave=sinf(game.time*.8f+lane*1.7f+segment*.9f);Vec3 tail=add(add(pos,mul(facing,-distance)),mul(right,engine*(1080.f+segment*38.f)+wave*segment*18.f));tail.y+=cosf(game.time*.65f+lane+segment*.75f)*segment*11.f;Vec3 tc=camera(&game,tail);if(tc.z<=80)break;Point q=project(tc);unsigned fire=segment==1?livery_tint(glow,34):segment<=3?glow:segment<=5?livery_tint(glow,-42):livery_tint(glow,-92);int width=segment<=2?2:segment<=5?1:0;
    for(int edge=-width;edge<=width;edge++){unsigned ink=edge?livery_tint(fire,-42):fire;line((int)last.x,(int)last.y+edge,(int)q.x,(int)q.y+edge,ink);}if(!high_contrast&&segment<7)sfx_add((int)q.x,(int)q.y,livery_tint(fire,-30),top,bot);last=q;
   }
   if(!high_contrast){sfx_add((int)last.x,(int)last.y,livery_tint(glow,-65),top,bot);}
  }
  sfx_add((int)p.x-1,(int)p.y,glow,top,bot);sfx_add((int)p.x+1,(int)p.y,glow,top,bot);pixel((int)p.x,(int)p.y,WHITE);
  if(phase<.13f||phase>.87f){
   float k=phase<.13f?(.13f-phase)/.13f:(phase-.87f)/.13f;
   int span=12+(int)(k*42.f);line((int)p.x-span,(int)p.y,(int)p.x+span,(int)p.y,glow);
   line((int)p.x,(int)p.y-2,(int)p.x,(int)p.y+2,WHITE);
  }
 }
}
/* Speed lines scale with actual velocity: a quiet hint at cruise, a dense
 * tunnel of streaks during boost. */
static void speed_lines(void){
 if(game.speed<player_ships[game.ship].speed*.18f||game.dock_stage||game.jump>0||game.dead)return;
 float normal=game.speed/fmaxf(1,player_ships[game.ship].speed),power=fminf(1,normal/(game.boost?20.f:1.15f));
 int top=view_top(),bottom=view_bot();
 int count=12+(int)(power*36.f);
 unsigned edge=game.boost?RGB(50,130,170):RGB(48,86,112);
 for(int i=0;i<count;i++){
  float a=i*2.39996f;float r=110+fmodf(i*37+game.time*(game.boost?520.f:180.f),160);float trail=6+power*46;
  int x=240+(int)(cosf(a)*r),y=110+(int)(sinf(a)*r*.5f);
  int xx=240+(int)(cosf(a)*(r+trail)),yy=110+(int)(sinf(a)*(r+trail)*.5f);
  if(y>top&&y<bottom&&yy>top&&yy<bottom){sfx_add(x,y,edge,top,bottom);sfx_add(xx,yy,game.boost?RGB(85,180,205):RGB(60,110,135),top,bottom);}
 }
}
static void engine_flare(void){
 if(game.speed<80||game.dock_stage||game.jump>0||game.dead)return;
 int bottom=view_bot();if(bottom<150)return;float normal=game.speed/fmaxf(1,player_ships[game.ship].speed),power=game.boost?fminf(1,normal/20):fminf(1,normal);
 int pulse=(int)(sinf(game.time*(game.boost?18:9))*2),length=(game.boost?18:7)+(int)(power*10)+pulse;if(length<3)length=3;
 unsigned core=game.boost?CYAN:AMBER,edge=game.boost?RGB(45,120,170):RGB(150,92,28);int y=bottom-6;
 for(int i=0;i<length;i++){int w=game.boost?2+(i%3):1+(i%2),yy=y+i,spread=(i*2)/3+1;rect(240-w-spread,yy,w,1,edge);rect(240+spread,yy,w,1,edge);if((i&1)==0)pixel(240,yy,core);}line(240-length/2,y-1,240+length/2,y-1,edge);
 space_anim_draw(SPACE_ANIM_PLUME,240,y+length/2,(int)(game.time*12.f),core);
}
static void secondary_hubs(void){
 for(int i=1;i<HUB_COUNT;i++){
  Vec3 center=hub_position(&game,i);float d=length(sub(center,game.pos));if(d>30000||occluded(center))continue;
  StationProfile profile=station_profile_for(&game,i);float scale=profile.radius/160.f,angle=game.time*profile.spin+i*1.7f;
  shipmesh_stretched(mesh_id("CORIOLIS"),center,0,angle,scale,profile.half/profile.radius,profile.hull,0);
  Vec3 outer[4]={{0,-profile.radius,-profile.half},{profile.radius,0,-profile.half},{0,profile.radius,-profile.half},{-profile.radius,0,-profile.half}};
  Vec3 inner[4];for(int k=0;k<4;k++)inner[k]=station_port_corner_for(&game,i,k);
  float door_z=profile.half-18.f;
  for(int k=0;k<4;k++){int j=(k+1)%4;station_quad_at(outer[k],outer[j],inner[j],inner[k],center,angle,livery_tint(profile.hull,-8));Vec3 a=inner[k],b=inner[j];a.z=b.z=door_z;station_quad_at(inner[k],inner[j],b,a,center,angle,RGB(8,19,24));}
  station_quad_at((Vec3){-STATION_PORT_HALF_W,-STATION_PORT_HALF_H,door_z},(Vec3){STATION_PORT_HALF_W,-STATION_PORT_HALF_H,door_z},(Vec3){STATION_PORT_HALF_W,STATION_PORT_HALF_H,door_z},(Vec3){-STATION_PORT_HALF_W,STATION_PORT_HALF_H,door_z},center,angle,RGB(20,29,36));
  /* A compact family signature keeps relays/outposts distinct too, while
     their common slit remains immediately readable to pilots. */
  if(profile.family&1)for(int s=-1;s<=1;s+=2)station_box_at((Vec3){s*profile.radius*.62f,0,profile.half*.12f},(Vec3){profile.radius*.18f,profile.radius*.12f,profile.half*.30f},center,angle,profile.trim);
  else for(int s=-1;s<=1;s+=2)station_box_at((Vec3){0,s*profile.radius*.62f,-profile.half*.08f},(Vec3){profile.radius*.28f,profile.radius*.12f,profile.half*.15f},center,angle,profile.trim);
 }
}
/* Station-local traffic glitter removed — read as hull lights, not sparkle dots. */
static void station_traffic_glints(void){}
static void docking_view(void){
 if(game.dock_stage==3){
  rect(0,23,W,195,RGB(8,13,24));
  rect(40,70,400,2,RGB(193,139,77));
  text(18,10,RGB(229,210,163),"DOCKING COMPLETE");
  text(12,14,WHITE,"Welcome to %s",station_name(&game));
  text(14,18,RGB(155,154,165),"Opening station services...");
  return;
 }
 Vec3 oldpos=game.pos;float oldyaw=game.yaw,oldpitch=game.pitch,oldroll=game.roll;
 game.pos=(Vec3){260,120,2820};Vec3 aim=norm(sub((Vec3){0,0,3400},game.pos));
 game.yaw=atan2f(aim.x,aim.z);game.pitch=asinf(aim.y);game.roll=0;
 sector_background();space_fx_nebula();starfield();
 station_model();station_window_animation();
 float t=fminf(1,game.dock_timer/3);
 shipmesh(mesh_id(player_ships[game.ship].name),(Vec3){0,0,3070+t*470},0,station_angle(&game),.7f,RGB(193,139,77),0);
 flush_meshes();
 station_entrance();
 /* Soft approach haze only — no glitter stars in the corridor. */
 if(!high_contrast){
  int top=view_top(),bot=view_bot();
  for(int i=0;i<8;i++){
   float a=i*.52f+game.time*.5f;
   int x=240+(int)(cosf(a)*(20+t*40)),y=110+(int)(sinf(a)*(10+t*18));
   sfx_add(x,y,RGB(40,32,22),top,bot);
  }
 }
 text(2,5,RGB(229,210,163),"ARRIVAL / %.16s",station_name(&game));
 game.pos=oldpos;game.yaw=oldyaw;game.pitch=oldpitch;game.roll=oldroll;
}
/* Edge-band captions. Body text is word wrapped to actual 8px cell capacity. */
static void speech_box(int x,int y,int w){
 const char *s=0;int who=VOICE_COMP;
 if(!quiet_comms&&speech_active()){s=game.voice;who=game.voice_who;}
 /* During combat, status text stays on the bottom RED ALERT banner — not here. */
 else if(game.message_time>0&&game.message[0]&&!combat_alert_active())s=game.message;
 if(!s)return;
 if(who<VOICE_KEI||who>VOICE_CONTACT)who=VOICE_COMP;
 int role=who==VOICE_LAW?LAW:who==VOICE_KEI?EXPLORERS:TRADERS;
 if(who==VOICE_CONTACT)role=game.voice_role>=0&&game.voice_role<FACTION_COUNT?game.voice_role:EXPLORERS;
 static const char *names[]={"","KEI","VENN","DOCKHAND","LOCAL LAW","COMPUTER","CONTACT"};
 /* Computer voice uses soft cyan; named speakers keep faction ink on charcoal plate. */
 unsigned ink=who==VOICE_COMP?RGB(85,212,212):faction_colors[role];
 int portrait=1,tx=x+40,col=tx/8,cap=(x+w-8-tx)/8;
 /* Three body rows plus two pixels of padding fit inside the caption backing. */
 int box_h=34;
 rect(x,y,w,box_h,RGB(21,28,39));rect(x,y,w,1,RGB(193,139,77));rect(x,y+box_h-1,w,1,RGB(41,54,70));
 rect(x,y,2,box_h,ink);
 if(portrait){if(who==VOICE_KEI)draw_kei(x+4,y,32,0);else if(who==VOICE_COMP)portrait_draw(x+4,y,32,32,portrait_ship_computer(),EXPLORERS);else draw_portrait(x+4,y,32,32,who==VOICE_CONTACT?game.voice_seed:who*37,role);}
 speaker_name_tag(col,y/8,who==VOICE_CONTACT?faction_names[role]:names[who],ink);
 button_icon(x+w-16,y,'T',ink);
 if(incoming_reply_ready()||night_ready())text((x+w-56)/8,y/8,RGB(240,180,91),"TALK");
 else text((x+w-56)/8,y/8,RGB(120,245,220),"CLOSE");
 /* Two extra pixels above the message; retain all three wrapped rows. */
 for(int line=0;line<3&&*s;line++){
  int len=(int)strlen(s),cut=len<cap?len:cap;
  if(len>cap)for(int k=cut;k>cap/3;k--)if(s[k]==' '){cut=k;break;}
  text_px(col*8,(y/8+1+line)*8+2,RGB(229,210,163),"%.*s",cut,s);
  s+=cut;while(*s==' ')s++;
 }
}
static void quick_comms_box(void){
 if(!comms_quick||game.encounter_kind==ENCOUNTER_NONE)return;
 int x=96,y=78,w=288,h=30;rect(x,y,w,h,RGB(10,18,35));rect(x,y,w,2,RGB(240,180,91));rect(x,y+h-2,w,2,RGB(41,54,70));
 rect(112,y+8,104,14,comms_quick_choice==0?RGB(66,45,38):RGB(25,35,45));
 rect(264,y+8,104,14,comms_quick_choice==1?RGB(25,65,77):RGB(25,35,45));
 text(16,11,comms_quick_choice==0?RGB(240,180,91):RGB(155,154,165),"[ IGNORE ]");
 text(35,11,comms_quick_choice==1?RGB(240,180,91):RGB(155,154,165),"[ RESPOND ]");
}
static void pip_bar(int x,int y,int w,int h,int fill,unsigned c){
 if(fill<0)fill=0;
 if(fill>100)fill=100;
 rect(x,y,w,h,RGB(32,14,6));int fw=w*fill/100;if(fw>0)rect(x,y,fw,h,c);rect(x,y,w,1,AMBDIM);
}
static void target_segments(int x,int y,int segments,int filled,unsigned ink){
 if(filled<0)filled=0;if(filled>segments)filled=segments;
 for(int i=0;i<segments;i++){
  unsigned c=i<filled?ink:UI_RAISED;
  rect(x+i*7,y,5,4,c);
  if(i<segments-1)pixel(x+i*7+5,y+1,UI_PANEL);
 }
}
static void target_condition(int x,int y,int id){
 if(!IS_NPC_ID(id)||!scanner_known(id))return;
 NPC *n=&game.npc[id-BODY_COUNT-1];
 float max_h=n->bounty_slot>=0?110.f+danger_rating(&game,game.system)*35.f:n->freighter?900.f:n->role==LAW?110.f:80.f;
 float max_s=n->bounty_slot>=0?55.f+danger_rating(&game,game.system)*15.f:n->freighter?100.f:n->role==LAW?60.f:40.f;
 int hull=(int)fmaxf(0,fminf(100,100.f*n->health/max_h));
 int shield=(int)fmaxf(0,fminf(100,100.f*n->shield/fmaxf(1.f,max_s)));
 text(x/8,y/8,UI_MUTED,"H");target_segments(x+10,y,5,(hull+19)/20,hull<30?RED:UI_SIGNAL);
 text((x+54)/8,y/8,UI_MUTED,"S");target_segments(x+64,y,5,(shield+19)/20,UI_SIGNAL);
}
static unsigned target_overlay_color(int id){
 if(is_mission_target(&game,id))return WHITE;
 if(IS_NPC_ID(id))return faction_colors[game.npc[id-BODY_COUNT-1].role];
 if(IS_ANOMALY_ID(id))return GOLD;
 if(IS_DEBRIS_ID(id))return DIM;
 return IS_STATION_ID(id)?CYAN:GOLD;
}
/* The compact target list has a five-character range column.  Raw metre
 * counts jitter on every frame at flight speed and eventually collide with
 * the contact name, so switch to coarser kilometre steps at long range. */
static void target_range_label(float metres,char *out,int cap){
 if(metres<0)metres=0;
 if(metres<1000)snprintf(out,cap,"%dM",(int)metres);
 else if(metres<10000)snprintf(out,cap,"%.1fKM",floorf(metres/100.f)/10.f);
 else snprintf(out,cap,"%dKM",(int)(metres/1000.f));
}
/* Selected names get a short dwell, then ticker through the same 13-column
 * window.  Unselected rows remain still, keeping the browser easy to scan. */
static void target_ticker_label(int id,const char *name,char *out,int cap){
 enum { WINDOW=13,GAP=3 };
 static int ticker_id=-1;
 static float ticker_since=0;
 int len=(int)strlen(name);
 if(id!=ticker_id){ticker_id=id;ticker_since=game.time;}
 if(len<=WINDOW){snprintf(out,cap,"%s",name);return;}
 float age=game.time-ticker_since;
 int offset=age<1.f?0:(int)((age-1.f)*4.f)%(len+GAP);
 int n=cap-1;if(n>WINDOW)n=WINDOW;
 for(int i=0;i<n;i++){int p=(offset+i)%(len+GAP);out[i]=p<len?name[p]:' ';}
 out[n]=0;
}
static void targeting_overlay(int x,int y,int w,int h,int detailed){
 int ids[2+BODY_COUNT+NPC_COUNT+DEBRIS_COUNT+ANOMALY_COUNT],n=collect_scan_ids(ids,scan_cat);
 int shown=n>5?5:n;
 rect(x,y,w,h,RGB(10,18,29));rect(x,y,w,1,UI_EDGE);rect(x,y+h-1,w,1,UI_RAISED);
 text(x/8+1,y/8+1,UI_TEXT,detailed?"TARGET COMPUTER":"TARGETS");
 text(x/8+1,y/8+3,UI_ACCENT,"< BAND: %.10s >",scan_cat_names[scan_cat<0?0:scan_cat>4?4:scan_cat]);
 int hint_y=((y+h-12)/8)*8;
 button_icon(x+8,hint_y-1,'l',UI_MUTED);text(x/8+3,hint_y/8,UI_MUTED,": NEXT");
 button_icon(x+104,hint_y-1,'r',UI_MUTED);text(x/8+15,hint_y/8,UI_MUTED,": LOCK");
 if(!n){text(x/8+1,y/8+6,DIM,"NO CONTACTS");return;}
 int first=0;
 for(int i=0;i<n;i++)if(ids[i]==selected_target){first=i-2;break;}
 if(first<0)first=0;if(first>n-shown)first=n-shown;
 if(n>shown){
  int track_y=y+42,track_h=80;
  int thumb_h=(track_h*shown)/n;if(thumb_h<8)thumb_h=8;
  int thumb_y=track_y+(n==shown?0:(track_h-thumb_h)*first/(n-shown));
  /* Keep the scroll rail on the bezel side; the right edge is reserved for
   * each contact's distance readout. */
  rect(x+4,track_y,3,track_h,UI_RAISED);
  rect(x+4,thumb_y,3,thumb_h,UI_ACCENT);
 }
 for(int i=0;i<shown;i++){
  int id=ids[first+i],yy=y+42+i*16,row_y=(yy/8)*8;
  /* Text is snapped to 8px font rows. Use that exact snapped origin for
   * the selection rule; otherwise 14px list spacing makes it drift. */
  if(id==selected_target)rect(x+8,row_y-2,w-11,12,RGB(25,65,77));
  unsigned ink=id==selected_target?UI_ACCENT:target_overlay_color(id);
  const char *name=scanner_known(id)?target_name(id):"UNKNOWN";char shown_name[14],range[8];
  if(id==selected_target)target_ticker_label(id,name,shown_name,sizeof(shown_name));else snprintf(shown_name,sizeof(shown_name),"%.13s",name);
  target_range_label(length(sub(target_position(id),game.pos)),range,sizeof(range));
  text(x/8+2,row_y/8,ink,"%c %s",id==selected_target?'>':' ',shown_name);
  text((x+w-43)/8,row_y/8,UI_TEXT,"%5s",range);
 }
 if(detailed&&valid_target(selected_target)){
  int cy=y+h-37;
  text(x/8+1,cy/8,UI_ACCENT,"%.21s",target_name(selected_target));
  target_condition(x+8,cy+10,selected_target);
 }
}
static unsigned dim_rgb(unsigned c,int num,int den){
 int r=((c&255)*num)/den,g=(((c>>8)&255)*num)/den,b=(((c>>16)&255)*num)/den;
 if(r>255)r=255;
 if(g>255)g=255;
 if(b>255)b=255;
 return RGB(r,g,b);
}
/* Soft bloom on the space canopy only — never after UI glyphs.
 * Header/instrument chrome stays crisp; bloom is for suns/engines/haze. */
static void hud_postfx(void){
 if(high_contrast)return;
 /* Keep bloom off the 24px header band so mission cue / system text stay sharp. */
 int top=view_top();if(top<24&&!hud_hidden)top=24;
 int bot=view_bot();if(bot>191&&hud_mode==0)bot=191;
 if(bot<=top+6)return;
 for(int y=top;y<=bot;y++)for(int x=0;x<8;x++){
  fb[y*STRIDE+x]=dim_rgb(fb[y*STRIDE+x],24+x,32);
  fb[y*STRIDE+W-1-x]=dim_rgb(fb[y*STRIDE+W-1-x],24+x,32);
 }
 /* Sparse bloom — high threshold so cream/amber HUD ink never blooms. */
 for(int y=top+3;y<=bot-3;y+=3)for(int x=3;x<W-3;x+=3){
  unsigned c=fb[y*STRIDE+x];int r=c&255,g=(c>>8)&255,b=(c>>16)&255,lum=r+g+b;
  if(lum<620)continue;
  for(int dy=-2;dy<=2;dy++)for(int dx=-2;dx<=2;dx++){
   int dist=dx*dx+dy*dy;if(dist==0||dist>8)continue;
   int yy=y+dy,xx=x+dx;if(yy<top||yy>bot)continue;unsigned d=fb[yy*STRIDE+xx];
   int fall=5-dist/2;if(fall<1)fall=1;
   int nr=((d&255)*6+(r*fall)/5)/6,ng=(((d>>8)&255)*6+(g*fall)/5)/6,nb=(((d>>16)&255)*6+(b*fall)/5)/6;
   if(nr>255)nr=255;if(ng>255)ng=255;if(nb>255)nb=255;
   fb[yy*STRIDE+xx]=RGB(nr,ng,nb);
  }
 }
}
/* Session-long text radio: a cheap marquee beside the activity strip. The
 * station voices are deliberately short, strange and varied so the cockpit
 * feels inhabited without needing another audio stream or PSP UI panel. */
static const char *radio_talk_lines[RADIO_STATION_COUNT][24]={
 {"DEEP FIELD: Tonight, moons answer questions with gravity.","DEEP FIELD: The quiet between stars is not empty. It is listening.","DEEP FIELD: Our guest insists comets have excellent memories.","DEEP FIELD: Please do not name a wormhole after your ex.","DEEP FIELD: Scientists confirm the signal was definitely not a sneeze.","DEEP FIELD: Three stars, one cup, and a very long night ahead.","DEEP FIELD: We now take calls from anyone outside normal space.","DEEP FIELD: If the sky blinks, remain calm and write it down.","DEEP FIELD: Today's forecast is radiant with a chance of radiation.","DEEP FIELD: The universe is expanding. Please keep your elbows in.","DEEP FIELD: A black hole called in to complain about its personal space.","DEEP FIELD: Tonight's caller says destiny is just very confident navigation.","DEEP FIELD: Research update: nobody has found the end of Tuesday.","DEEP FIELD: The observatory has upgraded its telescope and its snacks.","DEEP FIELD: If your moon follows you home, contact a professional.","DEEP FIELD: Guest debate: are stars burning, or simply overachieving?","DEEP FIELD: We apologise for the earlier eclipse. It was a scheduling error.","DEEP FIELD: A wormhole is not a shortcut if you forget where you parked.","DEEP FIELD: Listeners report a mysterious hum. Experts report a second hum.","DEEP FIELD: Tonight's prize is a certificate proving you saw something.","DEEP FIELD: The galaxy is vast, but the studio cupboard is somehow smaller.","DEEP FIELD: Please keep all existential questions after the station ident.","DEEP FIELD: Our guest claims aliens invented awkward silence.","DEEP FIELD: Stay tuned for the weather, traffic and one impossible constellation."},
 {"NEON TRANSIT: Welcome back, night pilots and daytime smugglers.","NEON TRANSIT: Our traffic report says the fast lane is mostly pirates.","NEON TRANSIT: Listener poll: best planet name? We accept bribes.","NEON TRANSIT: A station clerk has declared war on loose paperwork.","NEON TRANSIT: Today's advice: never race a courier with nothing to lose.","NEON TRANSIT: We play the hits, the misses, and one suspicious distress call.","NEON TRANSIT: Someone left a goldfish in dock seven. It wants a pilot.","NEON TRANSIT: Local law says this joke is still under investigation.","NEON TRANSIT: The next song is sponsored by three identical moon shops.","NEON TRANSIT: Keep your engines cool and your opinions warmer.","NEON TRANSIT: Live now: two aliens, one couch and a disagreement about parking.","NEON TRANSIT: Advert break: buy a moon, regret the paperwork later.","NEON TRANSIT: Our caller says their ex stole the family cargo hauler.","NEON TRANSIT: The audience votes to forgive the pirate. The pirate votes no.","NEON TRANSIT: Sponsored by Emergency Apology Insurance for accidental lasers.","NEON TRANSIT: Tonight's guest brought receipts, witnesses and three lawyers.","NEON TRANSIT: The station lift is stuck again. Please use the emotional lift.","NEON TRANSIT: Call now if your neighbour is secretly a disguised moon.","NEON TRANSIT: New dating service matches pilots by fuel efficiency.","NEON TRANSIT: A listener asks if fines count as souvenirs. Law says no.","NEON TRANSIT: We interrupt this argument for a very short advertisement.","NEON TRANSIT: Buy one docking permit, get the second one emotionally free.","NEON TRANSIT: Our host has been advised not to mention the incident. Here it is.","NEON TRANSIT: Keep your engines cool and your opinions warmer."},
 {"PIXEL COMET: Tiny rocks, enormous consequences, excellent radio.","PIXEL COMET: Mining tip: the shiny one is rarely the friendly one.","PIXEL COMET: We asked an asteroid how it felt. It gave us a hard answer.","PIXEL COMET: Tonight's guest is a mineral with a very low voice.","PIXEL COMET: Space dust gets everywhere. Especially in the microphone.","PIXEL COMET: A rock and a hard place walk into a docking bay.","PIXEL COMET: Comet etiquette: wave first, scoop later.","PIXEL COMET: We are broadcasting from somewhere with no return address.","PIXEL COMET: If your scanner says nothing, ask the rock again.","PIXEL COMET: Today's forecast: crunchy with pockets of vacuum.","PIXEL COMET: Advertisement: rent a mining laser, return it mostly intact.","PIXEL COMET: Our guest claims asteroids have no feelings. It then threw a moon.","PIXEL COMET: A prospector called in from a hole with excellent acoustics.","PIXEL COMET: Mineral dating advice: never say you are looking for something shiny.","PIXEL COMET: The rock cycle is just geology's longest talk show.","PIXEL COMET: Today's caller wants to know if cargo can be emotionally fragile.","PIXEL COMET: Buy a crate of ore, receive a free warning label.","PIXEL COMET: We asked a crystal for advice. It was remarkably clear.","PIXEL COMET: Asteroid etiquette update: no tailgating in the belt.","PIXEL COMET: A miner's helmet is not formal wear, despite what the advert says.","PIXEL COMET: Our studio wall is now technically a mineral sample.","PIXEL COMET: If it sparkles, scan it. If it screams, leave.","PIXEL COMET: Tonight's debate: rock, stone or very patient planet?","PIXEL COMET: Stay tuned for more hard news from soft microphones."},
 {"VELVET ORBIT: Slow down, breathe out, and admire that gas giant.","VELVET ORBIT: Our guest says luxury is having a working cooling fan.","VELVET ORBIT: A gentle reminder: docking is a dance, not a collision.","VELVET ORBIT: Tonight we discuss poetry, propulsion, and bad insurance.","VELVET ORBIT: Someone has put a tiny hat on the station beacon.","VELVET ORBIT: The calmest pilot is usually the one with fuel left.","VELVET ORBIT: We accept dedications from ships still in one piece.","VELVET ORBIT: Beauty tip: polished hulls reflect fewer regrets.","VELVET ORBIT: Our horoscope says avoid suspicious cargo today.","VELVET ORBIT: Stay soft, stay curious, and mind the approach vector.","VELVET ORBIT: Tonight's panel asks: is a yacht still a yacht with tractor damage?","VELVET ORBIT: Advertise your luxury cabin before the neighbours do.","VELVET ORBIT: We welcome a caller who has feelings about station carpet.","VELVET ORBIT: Fine dining tip: never ask what the sauce was before docking.","VELVET ORBIT: A host, a diplomat and a space slug enter a quiet lounge.","VELVET ORBIT: Sponsored by Soft Landing, the apology service for hard arrivals.","VELVET ORBIT: Your aura is calm. Your engine temperature disagrees.","VELVET ORBIT: Listener confession: they polished the cargo bay instead of sleeping.","VELVET ORBIT: Today's meditation is sponsored by a very loud compressor.","VELVET ORBIT: Our guest says romance is sharing the last fuel scoop.","VELVET ORBIT: The station beacon is wearing the hat again.","VELVET ORBIT: Luxury is a clean visor and a dock that says welcome.","VELVET ORBIT: Please remember: serenity does not stop missiles.","VELVET ORBIT: Stay soft, stay curious, and mind the approach vector."},
 {"FAR HORIZONS: Greetings, travellers. Your stars are behaving beautifully.","FAR HORIZONS: Tonight's alien panel asks whether humans dream in maps.","FAR HORIZONS: A distant voice says hello. It may be three systems away.","FAR HORIZONS: We discuss old Earth recipes and very new black holes.","FAR HORIZONS: The best route is not always the shortest. Sometimes it sings.","FAR HORIZONS: Listener question: can a nebula be homesick? We think yes.","FAR HORIZONS: Our guest has crossed a thousand suns and lost one shoe.","FAR HORIZONS: Please enjoy the quiet glow of the next horizon.","FAR HORIZONS: A pilot reports finding hope between two unremarkable stars.","FAR HORIZONS: Keep exploring. The dark has more stories than maps.","FAR HORIZONS: Live from the void: an alien couple argues about who named the moon.","FAR HORIZONS: Advertisement: insure your memories before visiting a time anomaly.","FAR HORIZONS: Tonight's guest says humans are adorable when they over-explain maps.","FAR HORIZONS: A three-eyed caller asks whether destiny accepts return journeys.","FAR HORIZONS: We discuss old Earth recipes, new black holes and a missing spoon.","FAR HORIZONS: Listener mail: can a star be lonely? Our panel says it depends.","FAR HORIZONS: Sponsored by Galactic Mediation for disputes between moons.","FAR HORIZONS: An alien poet joins us after accidentally inventing a new colour.","FAR HORIZONS: The quietest signal in space may simply be a very polite caller.","FAR HORIZONS: Tonight's debate: are humans brave, or just curious near danger?","FAR HORIZONS: A traveller found a shortcut and returned with excellent eyebrows.","FAR HORIZONS: If the horizon calls your name, ask which horizon.","FAR HORIZONS: Our studio guest has seven opinions and no visible mouth.","FAR HORIZONS: Keep exploring. The dark has more stories than maps."},
 {"CROSS-LING: Krru-vaa. Krru-vaa. The channel is awake.","CROSS-LING: Shaa? Tekk-takk. No, that was not a distress call.","CROSS-LING: Vrr-oo, vrr-oo, kha. Three mouths, one translator.","CROSS-LING: We asked the nest for traffic news. It answered in clicks.","CROSS-LING: Oruu-eh. A soft call from something with too many eyes.","CROSS-LING: The translator agrees this means hello, probably.","CROSS-LING: Chik-chik-raa. Please keep your antennae inside the ship.","CROSS-LING: Listener sample: mrr-ah, mrr-ah, gruu. Beautifully inconclusive.","CROSS-LING: A long-distance purr is crossing the static now.","CROSS-LING: Kha-kha-voom. The local flock has opinions about your engine.","CROSS-LING: No words today. Just beaks, throats and a little weather.","CROSS-LING: Sss-ora-ket. That phrase translates as watch the bright moon.","CROSS-LING: Two callers overlap. One is chirping. One is definitely aquatic.","CROSS-LING: The brood signal rises, folds and forgets what it meant.","CROSS-LING: Rruu-rru-rru. A lullaby from a station nobody charts.","CROSS-LING: Translator note: emotional clicking is not a navigation command.","CROSS-LING: Kett-oi, kett-oi. The reply came from inside the asteroid.","CROSS-LING: A cave-dweller has joined the broadcast. Please do not feed it.","CROSS-LING: Vaa-tek-shuu. We think that was a joke about humans.","CROSS-LING: The signal has feathers, scales or both. We remain respectful.","CROSS-LING: Soft trill, hard rattle, sudden squeak. That is the whole bulletin.","CROSS-LING: A distant pack is harmonising across three light-minutes.","CROSS-LING: Krru-vaa returns your greeting with seventeen corrections.","CROSS-LING: Stay curious. The galaxy is making sounds before it makes sense."}
 ,{"VOID TALES: The first traveller found a door in the dark and knocked for seven years.","VOID TALES: On the eighth year, the door opened inward, though there was no room behind it.","VOID TALES: A voice asked the traveller to name the star they had left.","VOID TALES: They gave three names. The door accepted all of them.","VOID TALES: Beyond the threshold, an old moon was waiting with its lights on.","VOID TALES: The moon remembered every ship that had passed, except one.","VOID TALES: That missing ship was still travelling, somewhere between two breaths.","VOID TALES: The traveller followed its wake and heard singing in the engine heat.","VOID TALES: The song had no words, but it knew the shape of home.","VOID TALES: When the traveller turned back, the door had become a window.","VOID TALES: Outside the glass, the stars were moving like patient lanterns.","VOID TALES: One lantern blinked twice. The traveller answered once.","VOID TALES: The answer returned from behind the moon, older and warmer.","VOID TALES: It said: carry a light for the ones who cannot cross.","VOID TALES: So the traveller lit the smallest lamp and kept going.","VOID TALES: Years later, another pilot found that lamp between the lanes.","VOID TALES: They called it a beacon. The dark called it a promise.","VOID TALES: The pilot followed it until the instruments forgot their numbers.","VOID TALES: There they met a creature made of weather and unfinished maps.","VOID TALES: It asked why humans keep returning to dangerous places.","VOID TALES: The pilot said: because something wonderful may be waiting.","VOID TALES: The creature considered this, then moved one star closer.","VOID TALES: The beacon still burns for anyone willing to listen.","VOID TALES: This tale is not finished. The next voice may be yours."}
 ,{"ELITE EXPLORATION: A new horizon begins where the route line ends.","ELITE EXPLORATION: Survey crews report untouched valleys beyond the western ridge.","ELITE EXPLORATION: Log the life, mark the crossing, leave the wilderness intact.","ELITE EXPLORATION: Tonight we follow a beacon nobody remembers placing.","ELITE EXPLORATION: The best discoveries rarely have docking permits.","ELITE EXPLORATION: A blue-white moon is rising over an uncharted salt plain.","ELITE EXPLORATION: Field note: the flowers close when the second sun appears.","ELITE EXPLORATION: Pack fuel, water and one unreasonable amount of curiosity.","ELITE EXPLORATION: A rover team has found warm rain beneath a frozen sky.","ELITE EXPLORATION: Scan gently. Some worlds are still learning your name.","ELITE EXPLORATION: The old trail ends at a bridge made before local history.","ELITE EXPLORATION: Clouds have cleared above the northern observatory.","ELITE EXPLORATION: A distant herd is moving between the amber trees.","ELITE EXPLORATION: Never confuse an empty map with an empty world.","ELITE EXPLORATION: Today's route crosses three rivers and one excellent mystery.","ELITE EXPLORATION: The ridge ahead offers a clear view of the ringed giant.","ELITE EXPLORATION: Leave a marker for the next pilot, not a mess.","ELITE EXPLORATION: A ruin in the highlands is reflecting signals after sunset.","ELITE EXPLORATION: The expedition pauses while something enormous crosses the mist.","ELITE EXPLORATION: Record the weather. It may be alive, or merely dramatic.","ELITE EXPLORATION: No road reaches the crater lake. That is rather the point.","ELITE EXPLORATION: New flora logged near the shadow of the western peak.","ELITE EXPLORATION: Home is behind you. The interesting part is ahead.","ELITE EXPLORATION: Keep moving, keep looking, and bring back a story."}
};
static void radio_ticker_display(void){
 int x=260,w=216;rect(x,1,w,21,UI_PANEL);rect(x,1,w,1,UI_RAISED);
 if(radio_off){text(33,1,DIM,"RADIO OFF");return;}
 int station=radio_station<0?0:radio_station>=RADIO_STATION_COUNT?RADIO_STATION_COUNT-1:radio_station;
 int segment=(int)(game.time/16.5f),line_index=(segment*5+station*3)%24;
 const char *broadcast;int len,pos;char shown[29];
 if(station==5||station==6){
  line_index=radio_voice_station==station?radio_voice_line:0;
  broadcast=radio_voice_script(station,line_index);len=(int)strlen(broadcast);
  int cursor=radio_voice_station==station?radio_voice_char:0;
  pos=cursor-11;if(pos<0)pos=0;if(pos>len-23)pos=len-23;if(pos<0)pos=0;
  for(int i=0;i<28;i++){int src=pos+i;shown[i]=(src>=0&&src<len)?broadcast[src]:' ';}
 }else{
  broadcast=radio_talk_lines[station][line_index];len=(int)strlen(broadcast);
  int cycle=len+28;float in=fmodf(game.time,16.5f),pause=.45f+((segment*11+station*7)%5)*.22f;
  pos=in<pause?-28:(int)((in-pause)*7.f)%cycle;
  for(int i=0;i<28;i++){int src=pos-28+i;shown[i]=(src>=0&&src<len)?broadcast[src]:' ';}
 }
 shown[28]=0;
 /* Keep the scrolling ticker on its own line. The visualizer lives below it
  * so the moving copy is never crossed by a waveform. */
 unsigned ink=station==7?RGB(115,245,190):station==6?RGB(140,230,255):station==5?RGB(218,142,255):station==4?UI_SIGNAL:UI_TEXT;
 /* Tiny but readable talking-host icon sits directly left of the ticker.
  * Four little expressions sell the illusion of a live presenter. */
 int face=(station==5||station==6)?(radio_voice_active?1+(radio_voice_char%3):0):(int)(game.time*2.2f)%4;
 /* The host sits on the ticker baseline: five pixels lower keeps its mouth
  * level with the moving copy instead of floating above it. */
 rect(280,7,12,9,RGB(14,18,28));rect(281,8,10,7,ink);
 if(face==3){line(283,10,285,10,RGB(14,18,28));line(287,10,289,10,RGB(14,18,28));}
 else {rect(283,10,2,2,RGB(14,18,28));rect(287,10,2,2,RGB(14,18,28));}
 if(face==0){rect(284,13,4,1,RGB(14,18,28));}
 else if(face==1){rect(284,12,4,2,RGB(14,18,28));}
 else if(face==2){rect(285,12,2,2,RGB(14,18,28));}
 else {line(284,13,288,13,RGB(14,18,28));}
 /* A longer, centred rail lives beneath the ticker. Bars breathe slowly and
  * crossfade between station colours without touching the scrolling text. */
 unsigned eq_a=station==7?RGB(70,205,155):station==6?RGB(80,180,220):station==5?RGB(218,142,255):station==4?UI_SIGNAL:RGB(90,165,255),eq_b=station==7?RGB(245,188,80):station==6?RGB(210,100,255):station==5?RGB(110,220,185):station==4?RGB(190,125,245):UI_ACCENT;
 for(int i=0;i<28;i++){int h=2+(int)((sinf(game.time*2.4f+i*1.35f+station)*.5f+.5f)*5.f);int y=23-h;float fade=sinf(game.time*.42f+i*.11f+station)*.5f+.5f;line(344+i*3,y,346+i*3,23,mix_rgb(eq_a,eq_b,fade));}
 text(37,1,ink,"%.23s",shown);
}
/* Ship-relative plan radar: up is ahead, down is behind. Full 360 degrees;
 * logarithmic range keeps both nearby craft and remote worlds visible. */
typedef struct {int x,y,lift;} RadarPoint;
static RadarPoint radar_point(Vec3 p){
 float d=length(p),h=sqrtf(p.x*p.x+p.z*p.z),r=fminf(1,logf(1+d/100)/7.f);
 RadarPoint q={240,230,0};
 if(h>.001f){q.x+=(int)(p.x/h*r*62);q.y-=(int)(p.z/h*r*16);}
 q.lift=(int)fmaxf(-4,fminf(4,p.y/fmaxf(1,d)*4));return q;
}
static void radar_dot(Vec3 p,unsigned ink,int kind,int focus){
 RadarPoint q=radar_point(p);line(q.x,q.y,q.x,q.y-q.lift,ink);q.y-=q.lift;
 if(focus){line(q.x-3,q.y-3,q.x+3,q.y-3,WHITE);line(q.x-3,q.y+3,q.x+3,q.y+3,WHITE);}
 if(kind==1){pixel(q.x,q.y-2,ink);line(q.x-2,q.y,q.x+2,q.y,ink);pixel(q.x,q.y+2,ink);}
 else if(kind==2){rect(q.x-2,q.y-2,5,5,ink);rect(q.x-1,q.y-1,3,3,UI_PANEL);}
 else rect(q.x-1,q.y-1,3,3,ink);
}
static void law_radar(void){
 if(fit_find(&game,LAW_SCANNER_ITEM)<0||!game.law_scan_valid)return;
 float age=game.time-game.law_scan_time;if(age<0||age>=12)return;
 unsigned ink=age<6?RGB(72,160,230):RGB(54,87,115);
 /* Public main lanes, not an assertion that the whole corridor scans cargo. */
 for(int r=0;r<2;r++)for(int leg=0;leg<2;leg++){
  Vec3 a=game.traffic_nodes[r][leg],delta=sub(game.traffic_nodes[r][leg+1],a);
  RadarPoint last=radar_point(camera(&game,a));
  for(int k=1;k<=8;k++){RadarPoint q=radar_point(camera(&game,add(a,mul(delta,k/8.f))));line(last.x,last.y-last.lift,q.x,q.y-q.lift,RGB(35,64,84));last=q;}
 }
 for(int i=0;i<NPC_COUNT;i++)if(game.law_echo_seen[i]){
  /* Actual 650 m inspection reach projected through the same 3D radar. */
  for(int plane=0;plane<2;plane++){RadarPoint last={0,0,0};for(int k=0;k<=12;k++){
   float a=k*6.2831853f/12;Vec3 offset={cosf(a)*650,plane?sinf(a)*650:0,plane?0:sinf(a)*650};
   RadarPoint q=radar_point(camera(&game,add(game.law_echo[i],offset)));
   if(k)line(last.x,last.y-last.lift,q.x,q.y-q.lift,ink);last=q;
  }}
 }
 text_px(180,200,ink,"LAW %02ds / 20KM",(int)age);
}
static const char *tracked_hud_cue(void){
 static char out[40];
 if(tracked_mission==TRACK_LAVE)return sc_lave_objective();
 if(tracked_mission==TRACK_STATION_TOUR&&station_tour_stage>=STATION_TOUR_ROUTE&&station_tour_stage<STATION_TOUR_DONE){
  if(station_tour_stage==STATION_TOUR_ROUTE||station_tour_stage==STATION_TOUR_DOCK){
   if(game.system!=STATION_TOUR_DEST)snprintf(out,sizeof(out),"JUMP: REORTE");
   else snprintf(out,sizeof(out),"DOCK: REORTE HUB");
  }else if(station_tour_stage==STATION_TOUR_WALK)snprintf(out,sizeof(out),"WALK: CANTEEN");
  else if(station_tour_stage==STATION_TOUR_BAR)snprintf(out,sizeof(out),"TALK: LYSA KEST");
  else snprintf(out,sizeof(out),"TALK: LYSA KEST");
  return out;
 }
 if(tracked_mission==0){if(game.campaign_stage>=6&&game.saga_chapter<SAGA_COUNT){if(!game.saga_step)snprintf(out,sizeof(out),"OPEN: STORY BRIEF");else if(saga_ready(&game))snprintf(out,sizeof(out),"OPEN: TRACKED MISSION");else if(game.system!=game.saga_dest){int jumps=0,hop=saga_next_hop(&game,&jumps);snprintf(out,sizeof(out),hop>=0?"JUMP: %.18s":"OPEN: GALAXY MAP",hop>=0?game.systems[hop].name:"");}else if(saga_beats[game.saga_chapter].kind==SAGA_SCAN)snprintf(out,sizeof(out),"SCAN: LOCAL SIGNAL");else if(saga_beats[game.saga_chapter].kind==SAGA_HUNT)snprintf(out,sizeof(out),"CLEAR: HOSTILE SHIPS");else snprintf(out,sizeof(out),"DOCK: LOCAL HUB");return out;}if(game.system!=7)snprintf(out,sizeof(out),"JUMP: LAVE");else if(game.docked)snprintf(out,sizeof(out),game.campaign_stage==1?"LAUNCH: FIRST FLIGHT":"OPEN: TRACKED MISSION");else if(game.campaign_stage==2&&!(game.campaign_flags&CP_LOCKED))snprintf(out,sizeof(out),"LOCK: LAVE HUB");else if(game.campaign_stage==3&&!(game.campaign_flags&CP_FLEW))snprintf(out,sizeof(out),"FLY: %.0f/600 M",game.campaign_distance);else snprintf(out,sizeof(out),"DOCK: LAVE HUB");return out;}
 if(tracked_mission==1){int ji=-1,type=guild_required_contract(&game);for(int i=0;i<game.job_n;i++)if(game.jobs[i].type==type){ji=i;break;}if(guild_ready(&game)){snprintf(out,sizeof(out),"DOCK: %.14s HUB",game.systems[game.system].name);return out;}if(ji>=0){Job *j=&game.jobs[ji];if(game.system!=j->dest)snprintf(out,sizeof(out),"JUMP: %.18s",game.systems[j->dest].name);else snprintf(out,sizeof(out),j->type==MISSION_RESCUE&&!j->stage?"FIND: RESCUE SIGNAL":"DOCK: %.14s HUB",game.systems[game.system].name);return out;}if(game.guild_chapter==2){snprintf(out,sizeof(out),"SCAN: LOCAL SIGNAL");return out;}if(type>=0){int station=guild_contract_station(&game);if(game.system!=station)snprintf(out,sizeof(out),"JUMP: %.18s",game.systems[station].name);else if(!game.docked)snprintf(out,sizeof(out),"DOCK: %.14s HUB",game.systems[station].name);else snprintf(out,sizeof(out),"OPEN: MISSION BOARD");return out;}snprintf(out,sizeof(out),game.docked?"LAUNCH: TEST FLIGHT":"DOCK: LOCAL HUB");return out;}
 int ji=tracked_mission-2;if(ji<0||ji>=game.job_n){snprintf(out,sizeof(out),"OPEN: MISSION LOG");return out;}Job *j=&game.jobs[ji];if(game.system!=j->dest)snprintf(out,sizeof(out),"JUMP: %.18s",game.systems[j->dest].name);else if(j->type==MISSION_DELIVERY||j->type==MISSION_SMUGGLING||(j->type==MISSION_RESCUE&&j->stage))snprintf(out,sizeof(out),"DOCK: %.14s HUB",game.systems[game.system].name);else if(j->type==MISSION_EXPLORATION&&j->item>=1&&j->item<BODY_COUNT)snprintf(out,sizeof(out),"FLY TO: %.18s",game.bodies[j->item].name);else if(j->type==MISSION_BOUNTY)snprintf(out,sizeof(out),"HUNT: MISSION TARGET");else if(j->type==MISSION_RESCUE)snprintf(out,sizeof(out),"FIND: RESCUE SIGNAL");else snprintf(out,sizeof(out),"FIND: MISSION CONTACT");return out;
}
static void cockpit(void){
 if((hud_hidden||hud_mode==2)&&!paused)return;
 /* Top 24px: live aft mirror, radio and route. Bottom 80px: all instruments.
  * Charcoal + ochre instrument bands — McQuarrie chrome, not cyan debug boxes. */
 rect(0,0,W,24,UI_PANEL);rect(0,23,W,1,UI_EDGE);
 rear_view_mirror(4,1,252,22);
 {int wl=wanted_level(&game);if(wl){unsigned ink=wl>=4?(((int)(game.time*8.f)&1)?RED:RGB(255,120,50)):wl>=2?RED:UI_ACCENT;text(1,1,ink,"LAW IS AFTER YOU!");}}
 radio_ticker_display();
 /* Mission cue top-right in the header band with a 2-col margin — not flush
  * to the screen edge. ART_AMBER objective ink (ART DIRECTOR palette); clear of danger badge. */
 {
  const char *cue=tracked_hud_cue();char route[52];snprintf(route,sizeof(route),">> %s >>",cue);
  int cols=W/8,inset=1,left=33,clen=(int)strlen(route),max=cols-inset-left;
  if(max<8)max=8;if(clen>max)clen=max;
  text(cols-inset-clen,0,RGB(100,235,150),"%.*s",clen,route);
 }
 if(game.dock_stage==1){rect(8,24,464,16,UI_PANEL);rect(8,24,464,1,UI_EDGE);text(2,4,UI_SIGNAL,"DOCKING GUIDANCE ACTIVE");}
 else if(square_held){
  /* Hold-Square is the expanded tactical browser. Keep it pinned to the
   * left edge so the central flight view remains readable and playable. */
  targeting_overlay(8,28,192,156,0);
  rect(8,204,132,16,RGB(24,63,73));rect(8,204,132,1,UI_SIGNAL);
  text(2,25,RGB(120,245,220),"(R = LOCK)");
 }
 else if(game.approach<0&&!game.police_stop&&!game.dead&&!game.dock_stage&&game.jump<=0){speech_box(8,24,464);quick_comms_box();}
 combat_alert_banner();
 rect(0,192,W,80,UI_PANEL);rect(0,192,W,1,UI_EDGE);
 line(155,198,155,258,UI_RAISED);line(323,198,323,258,UI_RAISED);
 int id=valid_target(selected_target)?selected_target:valid_target(look_target)?look_target:-1;
 text(4,25,UI_MUTED,game.surface==2?"PARKED SHIP":game.planet>=0?"LANDING PAD":"TARGET");
 hud_pixel_icon(8,200,id<0?3:IS_STATION_ID(id)?0:id<=BODY_COUNT?1:2,UI_SIGNAL);
 if(id>=0||game.planet>=0){
  Vec3 wp=game.surface==2?game.ship_pos:game.planet>=0?surface_site(&game,1):target_position(id),p=camera(&game,wp);
  const char *name=game.surface==2?player_ships[game.ship].name:game.planet>=0?game.bodies[game.planet].name:scanner_known(id)?target_name(id):"UNKNOWN CONTACT";
  int cut=(int)strlen(name);if(cut>18){cut=18;for(int k=18;k>8;k--)if(name[k]==' '){cut=k;break;}}
  text(1,27,UI_ACCENT,"%.*s",cut,name);
  const char *tail=name+cut;while(*tail==' ')tail++;
  if(*tail)text(1,28,UI_ACCENT,"%.18s",tail);
  text(1,30,UI_TEXT,"%d M",(int)length(p));
  if(IS_NPC_ID(id)&&scanner_known(id)){
   NPC *n=&game.npc[id-BODY_COUNT-1];
   float mh=n->bounty_slot>=0?110.f+danger_rating(&game,game.system)*35.f:n->freighter?900.f:n->role==LAW?110.f:80.f,ms=n->bounty_slot>=0?55.f+danger_rating(&game,game.system)*15.f:n->freighter?100.f:n->role==LAW?60.f:40.f;
   int hull=(int)fmaxf(0,fminf(100,100.f*n->health/mh)),shld=(int)fmaxf(0,fminf(100,100.f*n->shield/fmaxf(1.f,ms)));
   /* Stack the two compact target bars vertically. They share the target card
    * but never compete for the same row or overlap the radar/power columns. */
   text(1,31,UI_MUTED,"HULL");pip_bar(40,248,72,4,hull,hull<30?RED:(n->freighter?UI_ACCENT:UI_SIGNAL));
   text(1,32,UI_MUTED,"SHLD");pip_bar(40,258,72,3,shld,UI_SIGNAL);
  }else if(autoaim)text(1,31,UI_SIGNAL,"LOCKED / ALIGNING");
 }
 else {text(1,27,UI_MUTED,"NO TARGET");text(1,30,UI_TEXT,"SQUARE TO SELECT");}
 if(!(fit_find(&game,LAW_SCANNER_ITEM)>=0&&game.law_scan_valid&&game.time-game.law_scan_time>=0&&game.time-game.law_scan_time<12))text(28,25,UI_MUTED,"AHEAD");text(29,31,UI_MUTED,"AFT");
 line(171,230,309,230,UI_RAISED);line(240,211,240,249,UI_RAISED);
 line(177,214,303,214,UI_RAISED);line(177,246,303,246,UI_RAISED);
 if(game.planet>=0){for(int i=0;i<3;i++)radar_dot(camera(&game,surface_site(&game,i)),i==1?UI_SIGNAL:UI_ACCENT,2,0);if(game.surface==2)radar_dot(camera(&game,game.ship_pos),UI_SIGNAL,0,1);}
 else {
  /* Draw faint scenery first, selected contact last. */
  law_radar();
  for(int pass=0;pass<2;pass++)for(int tid=0;tid<=FLIGHT_TARGET_MAX;tid++)if(valid_target(tid)&&((tid==selected_target)==pass)){
   int ship=IS_NPC_ID(tid);unsigned ink=tid==ROUTE_TARGET_ID?CYAN:ship?faction_colors[game.npc[tid-BODY_COUNT-1].role]:IS_STATION_ID(tid)?UI_SIGNAL:tid<=BODY_COUNT?RGB(125,122,91):UI_MUTED;
   radar_dot(camera(&game,target_position(tid)),ink,IS_STATION_ID(tid)?2:tid<=BODY_COUNT?1:0,pass);
  }
  /* Each relay already has its own stable target/radar entry above. */
 }
 hud_pixel_icon(237,227,2,UI_TEXT);
 for(int i=0;i<3;i++){
  int x=336+i*36;int n=i==0?game.pip_sys:i==1?game.pip_eng:game.pip_wep;
  int selected=paused&&i==pip_sel;
  unsigned selected_bg=RGB(255,236,150),selected_ink=RGB(12,18,26);
  if(selected)rect(x-4,197,36,17,selected_bg);
  text_px(x,200,selected?selected_ink:UI_MUTED,"%s",i==0?"SYS":i==1?"ENG":"WEP");
  for(int k=0;k<4;k++)rect(x+k*5,210,3,2,selected?(k<n?selected_ink:RGB(190,160,94)):(k<n?(i==0?UI_SIGNAL:i==1?UI_ACCENT:RED):UI_RAISED));
 }
 /* Compact missile silhouette beside the power banks, never over a selection. */
 unsigned missile_ink=game.missiles>0?UI_TEXT:UI_MUTED;
 line(448,198,445,202,missile_ink);line(448,198,451,202,missile_ink);
 rect(446,202,5,6,missile_ink);line(446,205,443,211,missile_ink);line(450,205,453,211,missile_ink);
 rect(447,209,3,3,missile_ink);text_px(456,200,missile_ink,"%d",game.missiles);
 const char *labels[]={"SHLD","HULL","HEAT","FUEL"};
 int vmax=player_ships[game.ship].speed;if(vmax<1)vmax=1;
 int values[]={(int)game.energy,(int)game.hull,(int)game.heat,(int)(100*game.fuel/fmaxf(1,player_ships[game.ship].range))};
 unsigned cols[]={game.energy<30?RED:UI_SIGNAL,game.hull<35?RED:UI_SIGNAL,game.heat>70?RED:RGB(139,106,72),UI_ACCENT};
 /* Compact speed readout sits above the power pips, clear of radar and bars. */
 /* Keep the speed fill pegged to the normal cruise scale. Boost is shown as
  * a state effect (colour/pulse), rather than making the bar appear to empty. */
 int speed_pct=(int)fminf(100.f,100.f*game.speed/fmaxf(1.f,(float)vmax));
 for(int i=0;i<4;i++){text(42,27+i,UI_MUTED,"%s",labels[i]);pip_bar(378,216+i*8,90,5,values[i],cols[i]);}
 text(42,31,game.boost?RGB(240,120,96):UI_MUTED,"SPD %3d",(int)game.speed);
 {int sx=game.boost?400+(int)(sinf(game.time*18.f)*2.f):400;
  pip_bar(sx,248,68,5,speed_pct,game.boost?RGB(240,120,96):RGB(139,184,198));
  if(game.boost){rect(sx-2,246,72,1,UI_ACCENT);rect(sx-2,254,72,1,UI_ACCENT);}
 }
 rect(0,262,W,10,UI_PANEL);rect(0,262,W,1,UI_EDGE);
 button_icon(8,263,'T',UI_SIGNAL);text(3,33,game.damaged?RED:UI_MUTED,game.damaged?"HULL DAMAGE / ENGINEERS REQUIRED":"COMMS");
 if(game.dead)text(20,33,RED,"START: RECOVER");
 else if(paused)text(20,33,UI_TEXT,"POWER %s  L/R BANK  U/D +/-",pip_sel==0?"SYS":pip_sel==1?"ENG":"WEP");
 else if(game.police_stop)text(16,33,UI_ACCENT,game.police_phase?"X CONFIRM SCAN MENU":"X CONFIRM SETTLE MENU");
 else if(game.approach>=0)text(20,33,UI_ACCENT,game.bodies[game.approach].type==GAS?"O TURN BACK":"X LAND   O CANCEL");
 else if(game.surface==2)text(20,33,UI_MUTED,"TRI BOARD  S SCAN  R JUMP/RUN");
 else if(game.planet>=0)text(20,33,UI_MUTED,game.surface==1?"R LAUNCH   X STEP OUTSIDE":"TRIANGLE LANDING OPTIONS");
 else {
  int action=valid_target(look_target)?look_target:id;
  int rock=IS_DEBRIS_ID(action)&&game.debris[action-DEBRIS_ID_MIN].rock;
  const char *verb=rock?"MINE":IS_DEBRIS_ID(action)?"COLLECT":IS_NPC_ID(action)?"LOCK":IS_ANOMALY_ID(action)?"SCAN":action==0?"DOCK":"APPROACH";
  button_icon(158,263,'S',UI_ACCENT);text(22,33,UI_MUTED,"TARGETS");button_icon(264,263,'O',UI_SIGNAL);if(tools_selected<2)text(35,33,UI_MUTED,"%s %d",tool_names[tools_selected],tools_selected==0?game.missiles:game.flare_charges);else text(35,33,UI_MUTED,"%s",tool_names[tools_selected]);(void)verb;
 }
}
