/* Bounded planetary ambience: deterministic identity, transient motion.
 * All effects are decorative, allocation-free and deliberately tiny on PSP. */
typedef struct {
 int particles,flocks,ground_density,particle_kind;
 float wind;
 unsigned particle,bird;
} SurfaceAmbience;
enum { AMB_LEAVES,AMB_DUST,AMB_SNOW,AMB_ASH,AMB_SPORES,AMB_MIST };

static SurfaceAmbience surface_ambience_profile(const Body *b,int biome,FieldProfile fp){
 SurfaceAmbience a={12,0,2,AMB_MIST,.45f,mix_rgb(b->accent,WHITE,.34f),mix_rgb(b->color,RGB(38,45,52),.58f)};
 if(biome==BIOME_FOREST)a=(SurfaceAmbience){20,2,4,fp.weather==2?AMB_SPORES:AMB_LEAVES,.72f,mix_rgb(b->accent,RGB(226,173,71),.42f),mix_rgb(b->color,RGB(31,43,39),.64f)};
 else if(biome==BIOME_DESERT)a=(SurfaceAmbience){14,1,1,AMB_DUST,1.15f,mix_rgb(b->color,RGB(213,164,91),.52f),mix_rgb(b->accent,RGB(74,55,37),.58f)};
 else if(biome==BIOME_ICE)a=(SurfaceAmbience){22,1,1,AMB_SNOW,.66f,mix_rgb(b->accent,RGB(228,239,255),.68f),mix_rgb(b->color,RGB(70,91,112),.56f)};
 else if(biome==BIOME_VOLCANIC)a=(SurfaceAmbience){18,1,0,AMB_ASH,.88f,mix_rgb(b->accent,RGB(231,92,38),.46f),RGB(39,31,32)};
 else if(biome==BIOME_OCEAN)a=(SurfaceAmbience){13,2,3,fp.weather==1?AMB_MIST:AMB_SPORES,.52f,mix_rgb(b->accent,RGB(189,232,214),.48f),mix_rgb(b->color,RGB(35,59,67),.60f)};
 else if(biome==BIOME_CLOUD)a=(SurfaceAmbience){18,1,0,fp.weather==3?AMB_ASH:AMB_MIST,1.30f,mix_rgb(b->accent,WHITE,.50f),mix_rgb(b->color,RGB(45,53,70),.58f)};
 if(fp.weather==2&&a.particle_kind!=AMB_ASH){a.particle_kind=AMB_SPORES;a.particles+=4;}
 if(fp.weather==3){a.wind*=1.35f;a.particles+=3;}
 /* The same authored identity shown in the Field Guide changes the world:
  * agricultural settlements are greener, industrial sites barer, frontier
  * weather rougher, and mineral-rich geology contributes more motes. */
 if(fp.culture==1&&a.ground_density>0)a.ground_density++;
 else if(fp.culture==0&&a.ground_density>1)a.ground_density--;
 if(fp.culture==3)a.wind*=1.12f;
 if(fp.geology==2||fp.geology==3)a.particles+=2;
 if(a.ground_density>4)a.ground_density=4;
 /* Only worlds whose existing fauna catalogue contains gliding species show
  * distant flocks.  These silhouettes therefore agree with the Codex rather
  * than adding generic birds to every barren world. */
 int gliders=0;for(int slot=0;slot<LIFE_COUNT;slot++)if(field_species_kind(game.system,game.planet,slot)==LIFE_FAUNA&&((field_species_seed(game.system,game.planet,slot)>>16)&3)==3)gliders++;
 a.flocks=gliders>1?2:gliders;
 if(a.particles>24)a.particles=24;
 return a;
}

static void surface_ambient_mark(Vec3 world,int w,int h,unsigned ink){
 Vec3 v=camera(&game,world);if(v.z<12||v.z>720)return;Point q=project(v);
 if(q.x<-6||q.x>W+6||q.y<view_top()-6||q.y>view_bot()+6)return;
 surface_sprite_z=v.z-surface_sprite_tan*v.y;planet_cliprect((int)q.x-w/2,(int)q.y-h/2,w,h,ink);surface_sprite_z=0;
}

static void surface_ambient_particles(const Body *b,SurfaceAmbience a,FieldProfile fp){
 float time=game.time,span=620.f;
 for(int i=0;i<a.particles;i++){
  unsigned h=planet_hash(b->seed+i*2654435761u),phase=(h>>8)&1023;
  float base_x=(int)(h%620)-310,base_z=(int)((h>>12)%620)-310;
  float drift=fmodf(time*(18.f+a.wind*18.f)+phase,span);
  float x=game.pos.x+base_x+drift-span*.5f,z=game.pos.z+base_z+sinf(time*.31f+i)*24.f;
  while(x<game.pos.x-span*.5f)x+=span;
  while(x>game.pos.x+span*.5f)x-=span;
  float ground=terrain_height(&game,x,z),y=ground+18+(h>>21)%105;
  if(a.particle_kind==AMB_SNOW||a.particle_kind==AMB_ASH)y=ground+12+fmodf((h>>16)%130-time*(a.particle_kind==AMB_SNOW?12.f:5.f)+520.f,130.f);
  else y+=sinf(time*(.8f+(i%3)*.12f)+i)*12.f;
  int w=1,hh=1;unsigned ink=a.particle;
  if(a.particle_kind==AMB_LEAVES){w=2+(h&1);hh=2;ink=mix_rgb(a.particle,b->color,(h>>4&3)*.12f);}
  else if(a.particle_kind==AMB_SNOW){w=(i%7==0)?2:1;hh=w;}
  else if(a.particle_kind==AMB_ASH){w=(i%5==0)?2:1;hh=1;ink=(h&8)?a.particle:RGB(72,65,66);}
  else if(a.particle_kind==AMB_SPORES){w=(i%4==0)?2:1;hh=w;ink=mix_rgb(a.particle,RGB(203,245,170),(h&3)*.12f);}
  else if(a.particle_kind==AMB_DUST){w=2+(h&1);hh=1;}
  else {w=2;hh=1;}
  surface_ambient_mark((Vec3){x,y,z},w,hh,ink);
 }
 /* Electric weather gets a rare, single-frame distant fork—not a screen flash. */
 if(fp.weather==3&&((int)(game.time*5)+(int)(b->seed&31))%97==0){
  Vec3 p=add(game.pos,(Vec3){220,210,410});Vec3 v=camera(&game,p);if(v.z>20){Point q=project(v);line((int)q.x,(int)q.y,(int)q.x-3,(int)q.y+8,RGB(145,213,232));line((int)q.x-3,(int)q.y+8,(int)q.x+1,(int)q.y+14,RGB(211,235,234));}
 }
}

static void surface_ambient_birds(const Body *b,SurfaceAmbience a,float daylight){
 if(daylight<.18f)return;
 for(int flock=0;flock<a.flocks;flock++){
  int birds=3+(int)((b->seed>>(flock*4))&3);float phase=game.world_clock*(.010f+flock*.003f)+(b->seed%628)*.01f+flock*2.1f;
  float radius=420+flock*270;Vec3 centre=add(game.pos,(Vec3){sinf(phase)*radius,145+flock*85,cosf(phase)*radius});
  for(int i=0;i<birds;i++){
   Vec3 p=add(centre,(Vec3){(i-birds/2)*24.f,sinf(game.time*2.2f+i)*7.f,(i&1)*18.f});Vec3 v=camera(&game,p);if(v.z<40||v.z>1100)continue;Point q=project(v);int flap=((int)(game.time*6+i+flock)&1),x=(int)q.x,y=(int)q.y;
   surface_sprite_z=v.z-surface_sprite_tan*v.y;planet_cliprect(x-1,y,3,1,a.bird);if(flap){planet_cliprect(x-3,y-1,2,1,a.bird);planet_cliprect(x+2,y-1,2,1,a.bird);}else{planet_cliprect(x-3,y+1,2,1,a.bird);planet_cliprect(x+2,y+1,2,1,a.bird);}surface_sprite_z=0;
  }
 }
}

static int surface_render_kind(int poi){
 int kind=field_site_kind(&game,game.system,game.planet,poi);
 if(poi==1){static const int contract[]={FIELD_WEATHER,FIELD_GARDEN,FIELD_ARCHIVE};kind=contract[field_profile(&game,game.system,game.planet).job];}
 return kind;
}

static void surface_poi_ambient_fx(Vec3 *positions,FieldBuilding *shapes,const Body *b){
 for(int poi=1;poi<10;poi++){if(poi==6)continue;int kind=surface_render_kind(poi);Vec3 base=positions[poi];float t=game.time+poi*.71f;
  if(kind==FIELD_VENT){for(int puff=0;puff<4;puff++){float rise=fmodf(t*13+puff*27,90.f);surface_ambient_mark((Vec3){base.x+sinf(t+puff)*9,base.y+shapes[poi].height*.65f+rise,base.z+cosf(t*.7f+puff)*8},3+puff/2,2,mix_rgb(b->accent,WHITE,.63f));}}
  else if(kind==FIELD_GARDEN){for(int bug=0;bug<6;bug++){float a=t*(.7f+bug*.03f)+bug;surface_ambient_mark((Vec3){base.x+sinf(a)*shapes[poi].w*.72f,base.y+18+(bug%3)*12,base.z+cosf(a)*shapes[poi].d*.72f},1+(bug==0),1,bug&1?RGB(244,192,85):RGB(183,224,131));}}
  else if(kind==FIELD_RUINS||kind==FIELD_MIGRATION){for(int bird=0;bird<2;bird++){float a=t*.45f+bird*3.14f;surface_ambient_mark((Vec3){base.x+sinf(a)*shapes[poi].w,base.y+shapes[poi].height*(.72f+bird*.12f),base.z+cosf(a)*shapes[poi].d},3,1,RGB(52,48,45));}}
  else if(kind==FIELD_WEATHER&&((int)(t*7)&7)==0)surface_ambient_mark((Vec3){base.x,base.y+shapes[poi].height*.95f,base.z},4,2,RGB(120,225,240));
  else if(kind==FIELD_RESCUE&&((int)(t*3)&1)==0)surface_ambient_mark((Vec3){base.x+shapes[poi].w*.46f,base.y+shapes[poi].height*.92f,base.z},4,3,RGB(248,90,68));
  else if(kind==FIELD_DISH){float a=t*.55f;surface_ambient_mark((Vec3){base.x+sinf(a)*shapes[poi].w*.55f,base.y+shapes[poi].height*.93f,base.z+cosf(a)*shapes[poi].d*.55f},3,2,RGB(143,225,235));}
  else if(kind==FIELD_CRYSTAL&&((int)(t*5)&3)==0)surface_ambient_mark((Vec3){base.x+sinf(t)*22,base.y+shapes[poi].height*.72f,base.z+cosf(t)*22},2,3,RGB(220,184,255));
  else if(kind==FIELD_WRECK&&((int)(t*9)&15)==0)surface_ambient_mark((Vec3){base.x-shapes[poi].w*.3f,base.y+shapes[poi].height*.4f,base.z},3,2,RGB(246,155,67));
  else if((kind==FIELD_CACHE||kind==FIELD_ARCHIVE)&&((int)(t*2)&1)==0)surface_ambient_mark((Vec3){base.x,base.y+shapes[poi].height*.86f,base.z-shapes[poi].d*.6f},2,2,RGB(226,189,83));
  else if(kind==FIELD_FOSSIL){for(int mote=0;mote<3;mote++)surface_ambient_mark((Vec3){base.x+(mote-1)*18,base.y+12+fmodf(t*4+mote*11,22),base.z},1,1,RGB(174,139,91));}
 }
}
