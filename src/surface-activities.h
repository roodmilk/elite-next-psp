/* Finite, seeded expeditions: shared persistence, different local purposes. */
const char *surface_poi_name(int id){static const char *names[]={"SPACEPORT","RELAY","SITE 3","SITE 4","SITE 5","SITE 6","ROVER","SITE 7","SITE 8","SITE 9"};return names[id<0||id>9?0:id];}
const char *surface_site_name(const Game *g,int sys,int body,int id){
 static const char *names[]={"SUPPLY CACHE","TIDE RUINS","SKY OBSERVATORY","RESCUE BEACON","SEED GARDEN","FOSSIL BED","THERMAL VENT","SURVEY DRONE","MIGRATION POST","CRYSTAL GROTTO","LOCAL ARCHIVE","WEATHER ARRAY"};
 if(id==0)return field_type(sys,body)==GAS?"FLOATING SKYPORT":"SPACEPORT";if(id==6)return "ROVER";
 if(id==1){static const char *jobs[]={"POWER RELAY","BIOLOGY ARRAY","ARCHIVE UPLINK"};return jobs[field_profile(g,sys,body).job];}
 if(sys==7&&body==3){static const char *sky[]={"FREIGHT DEPOT","SALVAGED RUINS","SKY OBSERVATORY","RESCUE PLATFORM","AEROPONIC GARDEN","FOSSIL COLLECTION","HEAT EXCHANGER","RECOVERY DECK","SKY FAUNA WATCH","CRYSTAL LAB","SKYPORT ARCHIVE","WEATHER ARRAY"};return sky[field_site_kind(g,sys,body,id)];}
 return names[field_site_kind(g,sys,body,id)];
}
const char *surface_site_brief(const Game *g,int sys,int body,int id){
 static const char *brief[]={"A sealed expedition crate. Salvage a mineral cargo unit if your hold has space.","Flood marks form a calendar. Map the stones to preserve a record of this world's older inhabitants.","A patient telescope waits for calibration. Align its receiver and submit a stellar survey.","An isolated camp lost its shuttle link. Restore the beacon so the port can send help.","Local growers need new seed samples. Log two species, then share your field data here.","Trace the layered impressions and record organisms that lived here before the present landscape.","This vent powers remote shelters. Inspect its regulator and vent the excess charge.","A survey drone lost its memory beacon. Recover its route recordings for the expedition archive.","Record at least one local animal before uploading its behaviour to this migration study.","Register the three nearby crystal faces for a research payment.","Weathered tapes hold the voices of early settlers. Restore their index and preserve the archive.","Reset the remote pressure baseline and submit measurements to local flight control."};
 if(id==0){static const char *jobs[]={"Port contract: restore the POWER RELAY, then return for payment. The port also recharges your suit.","Port contract: log two local species, activate the BIOLOGY ARRAY, then return for payment.","Port contract: investigate SITE 4, transmit its record at the ARCHIVE UPLINK, then return for payment."};return jobs[field_profile(g,sys,body).job];}
 if(id==6)return "Your field rover: X boards nearby or dismounts. R accelerates; L brakes/reverses. Nub steers, Circle drifts, X boosts. Triangle parks.";
 if(id==1)return surface_site_brief(g,sys,body,0);
 if(sys==7&&body==3){static const char *sky[]={
 "A skyport freight crate holds mineral samples. Transfer one tonne if your ship has room.",
 "Salvaged stones were brought up to this research deck. Map their original flood marks; there is no solid ground beneath this platform.",
 "Above the thickest cloud banks, calibrate the telescope and submit a stellar survey.",
 "An isolated service deck has lost its shuttle link. Repair the beacon so the skyport can send help.",
 "The aeroponic growers need records of two skyport species. Plants and sheltered animals live on the maintained decks, not on a hidden planet surface.",
 "Imported fossil slabs are secured on a research deck. Record their layers for the expedition archive.",
 "The skyport heat exchanger supports the outlying decks. Inspect its regulator and release the excess pressure.",
 "A damaged atmospheric survey drone rests on a recovery deck. Preserve its route recording.",
 "Log a local animal, then upload its behaviour to the skyport's managed wildlife study.",
 "Imported mineral samples stand on a laboratory deck. Register their crystal faces without breaking them.",
 "Restore the early skyport crews' recordings and preserve their account of life above the clouds.",
 "Reset the weather array and transmit pressure readings to approaching shuttle pilots."};return sky[field_site_kind(g,sys,body,id)];}
 return brief[field_site_kind(g,sys,body,id)];
}
Vec3 surface_poi(const Game *g,int id){
 if(id==6)return g->rover_pos;
 static Vec3 cache[10];static unsigned cached_seed;static int cached_sys=-1,cached_body=-1,cached_type=-1,cached_job=-1,valid;
 unsigned seed=g->bodies[g->planet].seed;int type=g->bodies[g->planet].type,job=g->systems[g->system].economy+16*g->systems[g->system].government+256*g->systems[g->system].tech;
 if(cached_sys!=g->system||cached_body!=g->planet||cached_seed!=seed||cached_type!=type||cached_job!=job){valid=0;cached_sys=g->system;cached_body=g->planet;cached_seed=seed;cached_type=type;cached_job=job;}
 if(id>=0&&id<10&&(valid&(1<<id)))return cache[id];
 Vec3 p=surface_site(g,1);
 if(id==2&&g->system==7&&g->planet==1)p.z+=FIELD_OBSERVATORY_DISTANCE;
 else if(id==0){FieldBuilding terminal=field_port_building(g,0);p.x+=terminal.x;p.z+=terminal.z+terminal.d+40;} /* Accessible forecourt of the actual terminal. */
 else {
  unsigned h=field_hash(g->bodies[g->planet].seed+id*7159u);int rank=id<6?id-1:id-2;
  /* Two broad expedition rings make the larger worlds meaningful. Gas worlds
   * remain a denser network of connected platforms above the cloud sea. */
  float radius=g->bodies[g->planet].type==GAS?1800.f+rank*470.f+(h%420):
               2700.f+rank*720.f+(h%620);
  float angle=rank*.897598f+(g->bodies[g->planet].seed%628)*.01f+((int)(h%100)-50)*.002f;
  p.x+=sinf(angle)*radius;p.z+=cosf(angle)*radius;
  if(g->bodies[g->planet].type!=GAS){
   Vec3 port=surface_site(g,1);
   for(int attempt=0;attempt<8&&terrain_is_water(g,p.x,p.z);attempt++){
    angle+=.43f;p.x=port.x+sinf(angle)*radius;p.z=port.z+cosf(angle)*radius;
   }
  }
 }
 if(id>0){
  Vec3 port=surface_site(g,1);FieldBuilding b=field_site_building(g,id);
  float dx=p.x-port.x,dz=p.z-port.z;
  if(fabsf(dx)<field_port_clear(g)+b.w+60&&fabsf(dz)<field_port_clear(g)+b.d+60){
   float scale=fminf((field_port_clear(g)+b.w+60)/fmaxf(1,fabsf(dx)),(field_port_clear(g)+b.d+60)/fmaxf(1,fabsf(dz)));
   p.x=port.x+dx*scale;p.z=port.z+dz*scale;
  }
 }
 p.y=terrain_height(g,p.x,p.z)+22;if(id>=0&&id<10){cache[id]=p;valid|=1<<id;}return p;
}
int surface_rover(Game *g){
 if(g->surface!=2||g->planet<1||g->dead)return 0;
 if(g->rover_driving){g->rover_driving=0;g->rover_pos=g->pos;for(int i=0;i<8;i++){float a=i*.785398f,x=g->pos.x+cosf(a)*32,z=g->pos.z+sinf(a)*32;if(eva_position_allowed(g,x,z)){g->pos.x=x;g->pos.z=z;g->pos.y=terrain_height(g,x,z)+22;g->speed=0;g->rover_velocity=(Vec3){0,0,0};g->rover_crack=g->rover_reverse_wait=0;message(g,"Rover parked. X to board it again.");return 1;}}g->rover_driving=1;message(g,"No safe exit. Move the rover to clear ground.");return 0;}
 if(length(sub(g->pos,g->rover_pos))>48){message(g,"Find the rover at the spaceport. X boards nearby.");return 0;}
 if(fabsf(g->pos.y-terrain_height(g,g->pos.x,g->pos.z)-22)>2){message(g,"Land before boarding the rover.");return 0;}
 g->pos=g->rover_pos;g->pos.y=terrain_height(g,g->pos.x,g->pos.z)+22;g->yaw=0;g->pitch=g->roll=0;
 g->rover_driving=1;g->jetpack=0;g->boost=0;g->rover_velocity=(Vec3){0,0,0};g->rover_charge=100;g->rover_crack=g->rover_impact_cd=g->rover_reverse_wait=0;message(g,"ROAMER: R accelerate, L brake/reverse. Circle drift, X boost. Triangle park.");return 1;
}
/* Interact from a site's accessible perimeter, not its mathematical centre.
 * This matters for landmark-scale gardens and ruins whose centres are solid. */
int surface_nearest_site(const Game *g,float range){
 int closest=-1;
 for(int i=0;i<10;i++){
  if(i==6)continue;Vec3 p=surface_poi(g,i);float dx=p.x-g->pos.x,dz=p.z-g->pos.z;
  if(i>0){FieldBuilding b=field_site_building(g,i);dx=fmaxf(0,fabsf(dx)-b.w);dz=fmaxf(0,fabsf(dz)-b.d);}
  float d=sqrtf(dx*dx+dz*dz);if(d<range){range=d;closest=i;}
 }
 return closest;
}
int surface_interact(Game *g){
 if(g->surface!=2||g->planet<1||g->dead)return 0;
 int closest=surface_nearest_site(g,55);if(closest<0)return survey_scan(g);
 uint32_t *flags=&g->surface_progress[g->system][g->planet];FieldProfile p=field_profile(g,g->system,g->planet);
 int logged=0,animals=0;for(int i=0;i<LIFE_COUNT;i++)if(field_species_logged(g,g->system,g->planet,i)){logged++;animals+=field_species_kind(g->system,g->planet,i)==LIFE_FAUNA;}
 if(closest==0){
  g->hazard=0;g->energy=fminf(100,g->energy+25);
  if(!(*flags&1)){*flags|=1;message(g,p.job==0?"JOB: restore POWER RELAY; return to port for payment.":p.job==1?"JOB: log two species, activate BIOLOGY ARRAY, return here.":"JOB: investigate site 4, use ARCHIVE UPLINK, return here.");}
  else if((*flags&2)&&!(*flags&64)){int pay=field_job_reward(g,g->system,g->planet);*flags|=64;g->credits+=pay;char note[96];snprintf(note,96,"Expedition report accepted. %.1f units paid.",pay*.1f);message(g,note);}
  else message(g,(*flags&64)?"Suit recharged. Contract complete. Other field sites remain open.":"Suit recharged. START opens your local contract and site guide.");return 1;
 }
 unsigned bit=field_site_bit(closest);if(*flags&bit){message(g,"Site already completed. START lists other field activities.");return 0;}
 if(closest==1){if(p.job==1&&logged<2){message(g,"The biology array needs two logged species. Square scans.");return 0;}if(p.job==2&&!(*flags&8)){message(g,"Investigate site 4 before transmitting its archive.");return 0;}*flags|=bit;message(g,"Field objective complete. Return to port for payment.");return 1;}
 int kind=field_site_kind(g,g->system,g->planet,closest),pay=150+(field_hash(p.seed+closest)%36)*10;
 if(kind==FIELD_GARDEN&&logged<2){message(g,"Garden research needs two species records. Scan wildlife first.");return 0;}
 if(kind==FIELD_MIGRATION&&!animals){message(g,"Log a local animal before submitting a migration study.");return 0;}
 if(kind==FIELD_CACHE){if(cargo_used(g)>=cargo_capacity(g)){message(g,"Hold full. Free 1t to recover the expedition cache.");return 0;}g->cargo[12]++;message(g,"Cache recovered: 1t minerals transferred to your ship.");}
 else {static const char *done[]={"Cache recovered","Ancient calendar mapped","Telescope calibrated","Rescue shuttle called","Seed archive enriched","Fossil record preserved","Shelter power stabilised","Drone records recovered","Migration study submitted","Crystal formation mapped","Settler voices preserved","Weather readings uploaded"};g->credits+=pay;g->discoveries++;char note[96];snprintf(note,96,"%s. +%.1f units.",done[kind],pay*.1f);message(g,note);}
 *flags|=bit;g->cue=SFX_SCAN;return 1;
}
/* One atomic choice through the existing reward path, never a second claim. */
int observatory_resolve(Game *g,int id,int choice){
 if(choice<1||choice>2||g->system<0||g->system>=256||g->planet<1||g->planet>=BODY_COUNT||g->surface!=2||g->dead||g->docked||g->rover_driving)return 0;
 if(id!=observatory_site(g,g->system,g->planet)||surface_nearest_site(g,55)!=id)return 0;
 uint32_t *flags=&g->surface_progress[g->system][g->planet];
 if((*flags&field_site_bit(id))||(*flags&OBS_CLUES)!=OBS_CLUES)return 0;
 if(!surface_interact(g))return 0;
 *flags=(*flags&~(3u<<OBS_CHOICE_SHIFT))|((unsigned)choice<<OBS_CHOICE_SHIFT);
 if(choice==1)g->credits+=200;else g->discoveries++;
 message(g,choice==1?"Public beacon restored. Service bonus: 20 units.":"Trace preserved. Extra discovery filed in the Codex.");
 g->cue=SFX_SCAN;return 1;
}
