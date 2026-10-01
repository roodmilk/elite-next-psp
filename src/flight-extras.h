#include "game.h"
#include "travellers-data.h"
/* Flight presentation and target selection. Target IDs: station, bodies, NPCs, debris. */
static int selected_target=0,look_target=-1,autoaim=0,scan_cat=2,square_held=0,police_choice=0;
/* dialogue-ui.h is included later by main.c; keep the arrest renderer on the
 * same standard conversation primitives without changing include order. */
static void dialogue_begin(const char *title);
static void dialogue_speech(const char *speaker,int role,int seed,const char *a,const char *b);
static void dialogue_context(const char *label,const char *body);
static void dialogue_reply(int index,const char *label);
static float r_tap=10,l_tap=10,hard_brake=0;
static const char *scan_cat_names[]={"PLANETS","SHIPS","STATIONS","OTHER","FOE"};
/* Each physical hub has a stable target ID; zero always means PRIMARY. */
static int route_target_ready(void){return game.route_goal>=0&&game.destination>=0&&game.destination<256&&game.destination!=game.system;}
static Vec3 route_target_direction(void){
 int dest=game.destination,dx=game.systems[dest].x-game.systems[game.system].x,dz=game.systems[dest].y-game.systems[game.system].y;
 unsigned h=(unsigned)(game.system+1)*2246822519u^(unsigned)(dest+1)*3266489917u;Vec3 d={(float)dx,(float)((int)((h>>27)&15)-7)*.35f,(float)-dz};if(length(d)<1)d=(Vec3){0,0,1};return norm(d);
}
/* ENEMIES = ships currently going after the player. SHIPS lists every alive contact. */
static int npc_is_hostile(const NPC *n){return n&&n->target==-2;}
static int target_category(int id){
 if(id==ROUTE_TARGET_ID)return 3;
 if(IS_STATION_ID(id))return 2;
 if(id>0&&id<=BODY_COUNT)return 0;
 if(IS_NPC_ID(id))return npc_is_hostile(&game.npc[id-BODY_COUNT-1])?4:1;
 return 3;
}
static Vec3 target_position(int id){if(id==ROUTE_TARGET_ID)return add(game.pos,mul(route_target_direction(),180000.f));if(IS_STATION_ID(id))return hub_position(&game,station_target_hub(id));if(id<=BODY_COUNT)return game.bodies[id-1].pos;if(IS_NPC_ID(id))return game.npc[id-BODY_COUNT-1].pos;if(IS_DEBRIS_ID(id))return game.debris[id-DEBRIS_ID_MIN].pos;return game.anomaly[id-ANOMALY_ID_MIN].pos;}
static const char *target_name(int id){if(id==ROUTE_TARGET_ID){static char route[32];snprintf(route,sizeof(route),"ROUTE: %.11s",game.systems[game.destination].name);return route;}if(IS_STATION_ID(id))return station_name_for(&game,station_target_hub(id));if(id<=BODY_COUNT)return game.bodies[id-1].name;static char label[32];if(IS_DEBRIS_ID(id)){Debris *d=&game.debris[id-DEBRIS_ID_MIN];if(d->rock)snprintf(label,sizeof(label),d->rock==2?"ICE ASTEROID":"MINERAL ASTEROID");else if(d->wreck)snprintf(label,sizeof(label),"WRECKAGE #%02d",id-DEBRIS_ID_MIN+1);else snprintf(label,sizeof(label),"%s POD",goods[d->good>=0&&d->good<GOODS?d->good:12].name);return label;}if(IS_ANOMALY_ID(id)){Anomaly *a=&game.anomaly[id-ANOMALY_ID_MIN];snprintf(label,sizeof(label),"%s",rift_names[rift_type(game.system,id-ANOMALY_ID_MIN)]);return label;}NPC *n=&game.npc[id-BODY_COUNT-1];if(n->bounty_slot>=0){bounty_target_label(&game,n->bounty_slot,label,sizeof(label));return label;}if(n->traveller>=0)return traveller_name(n->traveller);int serial=(id*17+game.system*7)&255;if(n->role==LAW)snprintf(label,sizeof(label),"ENCRYPTED // %02X",serial);else if(n->role==PIRATES)snprintf(label,sizeof(label),"RAIDER %c-%02d",'A'+(serial%26),serial%100);else if(n->role==EXPLORERS)snprintf(label,sizeof(label),"GUILD %c-%02d",'A'+(serial%26),serial%100);else if(n->freighter)snprintf(label,sizeof(label),"CAPITAL MERCHANT %02d",serial%100);else snprintf(label,sizeof(label),"MERCHANT %c-%02d",'A'+(serial%26),serial%100);return label;}
static int valid_target(int id){if(IS_STATION_ID(id))return 1;if(id==ROUTE_TARGET_ID)return route_target_ready();if(id<0||id>ANOMALY_ID_MAX)return 0;if(id<=BODY_COUNT)return 1;if(IS_NPC_ID(id))return game.npc[id-BODY_COUNT-1].alive;if(IS_DEBRIS_ID(id))return game.debris[id-DEBRIS_ID_MIN].alive;return game.anomaly[id-ANOMALY_ID_MIN].alive;}
static int scanner_known(int id){if(!IS_NPC_ID(id))return 1;NPC *n=&game.npc[id-BODY_COUNT-1];return n->name_known||length(sub(target_position(id),game.pos))<=module_scan_range(&game);}
static int nearest_hostile_target(void){int best=-1;float range=1e9f;for(int i=0;i<NPC_COUNT;i++)if(game.npc[i].alive&&npc_is_hostile(&game.npc[i])){float d=length(sub(game.npc[i].pos,game.pos));if(d<range){range=d;best=BODY_COUNT+1+i;}}return best;}
static void pick_look_target(void);
static int occluded(Vec3 pos);
static int collect_scan_ids(int *ids,int cat){
 int n=0;
 if(cat==0){for(int i=1;i<BODY_COUNT;i++)ids[n++]=i+1;}
 else if(cat==1){for(int i=0;i<NPC_COUNT;i++)if(game.npc[i].alive)ids[n++]=BODY_COUNT+1+i;} /* all ships, hostiles included */
 else if(cat==2){for(int hub=0;hub<HUB_COUNT;hub++)ids[n++]=STATION_TARGET_ID(hub);}
 else if(cat==4){for(int i=0;i<NPC_COUNT;i++)if(game.npc[i].alive&&npc_is_hostile(&game.npc[i]))ids[n++]=BODY_COUNT+1+i;} /* engaging player only */
 else {
  if(route_target_ready())ids[n++]=ROUTE_TARGET_ID;
  for(int i=0;i<ANOMALY_COUNT;i++)if(game.anomaly[i].alive)ids[n++]=ANOMALY_ID_MIN+i;
  for(int i=0;i<DEBRIS_COUNT;i++)if(game.debris[i].alive)ids[n++]=DEBRIS_ID_MIN+i;
 }
 return n;
}
static void ensure_scan_cat_for_target(int id){
 if(!valid_target(id))return;
 int ids[TARGET_CAPACITY],n=collect_scan_ids(ids,scan_cat);
 for(int i=0;i<n;i++)if(ids[i]==id)return;
 scan_cat=target_category(id);
}
static void lock_local_target(int id,const char *kind){
 selected_target=id;
 if(IS_NPC_ID(id))game.npc[id-BODY_COUNT-1].name_known=1;
 /* Stay on SHIPS when browsing all traffic — hostiles live there too. */
 if(!(IS_NPC_ID(id)&&(scan_cat==1||scan_cat==4)))scan_cat=target_category(id);
 else ensure_scan_cat_for_target(id);
 nav_body=id>0&&id<=BODY_COUNT?id-1:-1;autoaim=0;
 (void)kind;
 story_event(&game,STORY_EV_TARGET);
}
static void step_scan_cat(int dir){
 /* Always visit every band, including empty ENEMIES, so Square+Left/Right is predictable. */
 scan_cat=(scan_cat+dir+5)%5;
}
static void tab_flight_category(int dir){
 step_scan_cat(dir);
 int ids[TARGET_CAPACITY],n=collect_scan_ids(ids,scan_cat);
 if(!n){message(&game,"No contacts in that band.");return;}
 int best=0;float br=1e9f;
 for(int i=0;i<n;i++){float d=length(sub(target_position(ids[i]),game.pos));if(d<br){br=d;best=i;}}
 static const char *lab[]={"Planet","Ship","Station","Other","Enemy"};
 lock_local_target(ids[best],lab[scan_cat]);
}
static void cycle_scan_item(int dir){
 int ids[TARGET_CAPACITY],n=collect_scan_ids(ids,scan_cat);
 if(!n){message(&game,"No contacts in that band.");return;}
 int cur=-1;for(int i=0;i<n;i++)if(ids[i]==selected_target)cur=i;
 if(cur<0)cur=dir>0?-1:0;
 static const char *lab[]={"Planet","Ship","Station","Other","Enemy"};
 lock_local_target(ids[(cur+dir+n)%n],lab[scan_cat]);
}
static void cycle_front_target(void){
 int ids[TARGET_CAPACITY],n=0,cur=-1;
 int top=view_top()+3,bot=view_bot()-3;
 for(int id=0;id<=FLIGHT_TARGET_MAX;id++)if(valid_target(id)){
  Vec3 world=target_position(id),v=camera(&game,world);if(v.z<20)continue;
  Point p=project(v);if(p.x<8||p.x>W-8||p.y<top||p.y>bot)continue;
  if(id>BODY_COUNT&&occluded(world))continue;
  ids[n]=id;if(id==selected_target)cur=n;n++;
 }
 if(!n){message(&game,"No contacts in front of the ship.");return;}
 int next=cur<0?0:(cur+1)%n;lock_local_target(ids[next],"In view");
}
static int target_nearest_reticle(void){
 int best=-1,cy=(view_top()+view_bot())/2;float best_score=1e30f;
 for(int id=0;id<=FLIGHT_TARGET_MAX;id++)if(valid_target(id)){
  Vec3 world=target_position(id),v=camera(&game,world);if(v.z<20)continue;
  Point p=project(v);if(p.x<8||p.x>W-8||p.y<view_top()+3||p.y>view_bot()-3)continue;
  if(id>BODY_COUNT&&occluded(world))continue;
  float dx=p.x-W*.5f,dy=p.y-cy,score=dx*dx+dy*dy+v.z*.0001f;
  if(score<best_score){best_score=score;best=id;}
 }
 if(best<0)return 0;
 lock_local_target(best,"Reticle");return 1;
}
static void tractor_beam_effect(void){
 if(game.tractor_time<=0||!IS_DEBRIS_ID(game.tractor_target))return;
 int i=game.tractor_target-DEBRIS_ID_MIN;if(i<0||i>=DEBRIS_COUNT||!game.debris[i].alive)return;
 Vec3 v=camera(&game,game.debris[i].pos);if(v.z<20)return;Point p=project(v);int cx=240,cy=view_bot()-18;
 float phase=game.time*18.f;for(int k=0;k<4;k++){int wob=(int)(sinf(phase+k*1.7f)*5);line(cx+k*2-3,cy,p.x+wob+k*2,p.y,CYAN);}
 circle((int)p.x,(int)p.y,8+(int)(sinf(phase)*2),CYAN);circle((int)p.x,(int)p.y,3,GOLD);
}
static int flight_target_combo(unsigned pressed,unsigned held){
 if(game.planet>=0||game.approach>=0||game.jump>0)return 0;
 if(!((held|pressed)&PSP_CTRL_SQUARE))return 0;
 unsigned lr=PSP_CTRL_LEFT|PSP_CTRL_RIGHT,ud=PSP_CTRL_UP|PSP_CTRL_DOWN;
 unsigned dir=0;
 if((pressed&PSP_CTRL_SQUARE)&&(held&(lr|ud)))dir=held&(lr|ud);
 if((held&PSP_CTRL_SQUARE)&&(pressed&(lr|ud)))dir=pressed&(lr|ud);
 if(!dir)return 0;
 if(dir&lr)tab_flight_category((dir&PSP_CTRL_LEFT)?-1:1);
 else cycle_scan_item((dir&PSP_CTRL_UP)?-1:1);
 return 1;
}
/* Reply choices belong only to a currently audible human transmission.
 * A stale encounter flag must never turn a computer notice into dialogue. */
static int incoming_reply_ready(void){return game.message_time<=0&&game.voice_time>0&&game.voice[0]&&game.voice_who!=VOICE_COMP&&encounter_requires_reply(&game);}
static int night_ready(void);
static int docking_target_hub(void){return game.docked?game.station_variant:IS_STATION_ID(selected_target)?station_target_hub(selected_target):nearest_hub(&game);}
static int dock_selected_station(void){return dock_hub(&game,docking_target_hub());}
static int speech_active(void){
 if(!quiet_comms&&game.voice_time>0&&game.voice[0]){
  if(game.voice_who==VOICE_COMP)return 0;
  if(game.voice_who==VOICE_LAW&&!game.police_stop&&game.legal<=0&&game.attacked<=0)return 0;
  if(game.voice_who==VOICE_CONTACT&&game.attacked<=0&&game.encounter_kind!=ENCOUNTER_NONE&&!encounter_requires_reply(&game))return 0;
  return 1;
 }
 if(game.message_time>0&&!game.dead&&game.message[0]&&strncmp(game.message,"WELCOME",7)&&strncmp(game.message,"TRIANGLE",8))return 1;
 return 0;
}
static void speech_ok(void){game.voice_time=0;game.voice[0]=0;game.message_time=0;game.cue=SFX_UI;}
static void contact_speak(int role,const char *line){speak(&game,VOICE_CONTACT,line);game.voice_role=role;game.voice_seed=game.system*NPC_COUNT+selected_target;if(game.attacked>0||role==PIRATES)game.cue=SFX_TALK;}
static void hail_target(void){
 pick_look_target();
 int id=valid_target(selected_target)?selected_target:look_target;
 if(!valid_target(id)){message(&game,"No one on comms. Lock a ship, then Triangle.");return;}
 if(IS_NPC_ID(id)){
  if(mission_interact(&game,id))return;
  NPC *n=&game.npc[id-BODY_COUNT-1];
  selected_target=id;scan_cat=target_category(id);autoaim=0;story_event(&game,STORY_EV_TARGET);
  if(n->traveller>=0){contact_speak(n->role,traveller_hail(n->traveller));game.travellers[n->traveller].flags|=1;return;}
  if(n->role==PIRATES){speak(&game,VOICE_COMP,"No reply. They're painting us.");game.cue=SFX_TALK;}
  else if(n->role==LAW)contact_speak(LAW,game.legal?"Stop. Pay the fine or take custody.":"Clear. Keep the lane clean.");
  else if(n->role==TRADERS){if(n->freighter){char cargo[80];snprintf(cargo,sizeof(cargo),"%s %s. %d%c %s. %s.",n->freight_state<=FREIGHT_INBOUND?"From":"To",game.systems[n->freight_peer].name,n->freight_qty,goods[n->freight_good].unit,goods[n->freight_good].name,freight_status(n));contact_speak(TRADERS,cargo);}else if(trader_offer_hail(&game,id-BODY_COUNT-1)){game.voice_role=TRADERS;game.voice_seed=game.system*NPC_COUNT+selected_target;}}
  else contact_speak(EXPLORERS,"Survey channel. We are mapping this sky.");
  return;
 }
 if(IS_STATION_ID(id)){selected_target=id;scan_cat=2;open_station_channel();return;}
 if(id>1&&id<=BODY_COUNT){if(approach_planet(&game,id-1)){selected_target=id;scan_cat=0;autoaim=0;}return;}
 if(id==1){message(&game,"The sun has no landing approach.");return;}
 if(IS_ANOMALY_ID(id)){analysis_scan(&game,id);return;}
 if(IS_DEBRIS_ID(id)){autoaim=0;salvage(&game,id);return;}
}
static int occluded(Vec3 pos){if(mega_depth_valid&&clipy0<0&&!(pos.x==0&&pos.y==0&&pos.z==STATION_Z)){Vec3 v=camera(&game,pos);if(v.z>15){Point p=project(v);int x=(int)p.x,y=(int)p.y;if(x>=0&&x<W&&y>=view_top()&&y<=view_bot()&&mega_encode_depth(v.z)>mega_occlusion[(y/2)*240+x/2]+4u)return 1;}}Vec3 delta=sub(pos,game.pos);float distance=length(delta);Vec3 ray=norm(delta);for(int i=0;i<BODY_COUNT;i++){Vec3 d=sub(game.bodies[i].pos,game.pos);float along=dot(d,ray);if(along>0&&along<distance&&length(sub(d,mul(ray,along)))<game.bodies[i].radius)return 1;}return 0;}
static int station_circle_ready(void){
 if(game.docked||game.dead||game.dock_stage||game.jump>0||game.planet>=0)return 0;
 int hub=IS_STATION_ID(selected_target)?station_target_hub(selected_target):nearest_hub(&game);Vec3 world=hub_position(&game,hub);
 Vec3 p=camera(&game,world);float range=station_comms_range(&game,hub);
 return p.z>1&&sqrtf(p.x*p.x+p.y*p.y)<=fmaxf(160.f,p.z*.10f)&&length(sub(world,game.pos))<=range;
}
static void pick_look_target(void){look_target=-1;float best=1e9f;for(int id=0;id<=FLIGHT_TARGET_MAX;id++){if(!valid_target(id))continue;Vec3 world=target_position(id),p=camera(&game,world);if(p.z<1)continue;float radius=id==ROUTE_TARGET_ID?2200:id>0&&id<=BODY_COUNT?game.bodies[id-1].radius:IS_NPC_ID(id)?game.npc[id-BODY_COUNT-1].radius:IS_DEBRIS_ID(id)?game.debris[id-DEBRIS_ID_MIN].radius:IS_ANOMALY_ID(id)?40:160;float lateral=sqrtf(p.x*p.x+p.y*p.y);if(lateral>fmaxf(radius,p.z*.08f))continue;float distance=length(p)-radius;if(distance<best&&(id>0&&id<=BODY_COUNT?1:!occluded(world))){best=distance;look_target=id;}}}
static void align_target(float dt,float ax,float ay){
 if(!autoaim)return;
 if(!valid_target(selected_target)||fabsf(ax)>.1f||fabsf(ay)>.1f){autoaim=0;return;}
 Vec3 d=norm(sub(target_position(selected_target),game.pos));float yaw=atan2f(d.x,d.z),ny=d.y;if(ny>1)ny=1;if(ny<-1)ny=-1;float pitch=asinf(ny);float diff=atan2f(sinf(yaw-game.yaw),cosf(yaw-game.yaw));float blend=1-expf(-12*fmaxf(0,dt));game.yaw+=fmaxf(-dt*2,fminf(dt*2,diff*blend));game.pitch+=fmaxf(-dt*2,fminf(dt*2,(pitch-game.pitch)*blend));
}
static void route_hyperdrive_tick(float dt){
 int armed=page==FLIGHT&&!game.docked&&!game.dead&&!game.dock_stage&&game.jump<=0&&game.planet<0&&game.approach<0&&route_target_ready()&&selected_target==ROUTE_TARGET_ID&&autoaim&&game.boost;
 float alignment=armed?dot(forward(&game),route_target_direction()):-1.f;
 if(!armed||alignment<.985f){route_boost_charge=fmaxf(0,route_boost_charge-dt*2.5f);return;}
 route_boost_charge+=dt;
 if(route_boost_charge<5.f)return;
 route_boost_charge=0;
 if(jump_start(&game)){route_jump_engaged=1;autoaim=0;message(&game,"Vector locked. Hyperdrive engaged.");}
}
static void route_target_star(void){
 if(!route_target_ready()||game.jump>0)return;
 Vec3 v=camera(&game,target_position(ROUTE_TARGET_ID));if(v.z<=15)return;Point p=project(v);
 int top=view_top(),bot=view_bot(),x=(int)p.x,y=(int)p.y;if(x<5||x>=W-5||y<top+5||y>bot-5)return;
 float pulse=sinf(game.time*3.4f)*.5f+.5f;unsigned core=RGB(230,247,255),glow=RGB(52+(int)(pulse*35),160+(int)(pulse*50),220+(int)(pulse*35));
 line(x-5,y,x+5,y,glow);line(x,y-5,x,y+5,glow);circle(x,y,2,core);pixel(x,y,WHITE);
 if(selected_target==ROUTE_TARGET_ID){
  int w=76,filled=(int)(w*fminf(1.f,route_boost_charge/5.f));
  if(route_boost_charge>0){rect(x-w/2,y+12,w,5,RGB(15,31,46));if(filled)rect(x-w/2,y+12,filled,5,CYAN);}
 }
}
static void starfield(void){
 /* Every system rotates and reorders the same bounded direction table, then
  * applies its own temperature balance and twinkle clock. This yields a
  * recognisable sky without 256 textures or per-frame allocation. */
 static Vec3 stars[600];static int initialized=0;
 if(!initialized){unsigned seed=91731;for(int i=0;i<600;i++){seed=seed*1664525u+1013904223u;float a=(seed&65535)*6.2831853f/65536;seed=seed*1664525u+1013904223u;float y=(seed&65535)/32767.5f-1;float h=sqrtf(1-y*y);stars[i]=(Vec3){cosf(a)*h,y,sinf(a)*h};}initialized=1;}
 int xt=clipy0>=0?clipx0:1,xb=clipy0>=0?clipx1-1:478,yt=clipy0>=0?clipy0:view_top(),yb=clipy0>=0?clipy1-1:view_bot();
 unsigned seed=space_sky_seed(),style=(seed>>27)&7;float sky_yaw=(seed%6283)*.001f,sky_roll=((int)((seed>>11)&255)-128)*.006f,cy=cosf(sky_yaw),sy=sinf(sky_yaw),cr=cosf(sky_roll),sr=sinf(sky_roll);
 for(int i=0;i<600;i++){
  unsigned h=field_hash(seed^(unsigned)(i+1)*0x9e3779b9u);Vec3 base=stars[(i+(seed%599))%600],yawed={base.x*cy+base.z*sy,base.y,-base.x*sy+base.z*cy},direction={yawed.x*cr-yawed.y*sr,yawed.x*sr+yawed.y*cr,yawed.z};Vec3 p=camera(&game,add(game.pos,mul(direction,30000)));if(p.z<100)continue;Point q=project(p);
  if(q.x<xt||q.x>xb||q.y<yt||q.y>yb)continue;
  int temperature=(h>>19)%5;unsigned c=temperature==0?RGB(105,181,255):temperature==1?RGB(197,226,255):temperature==2?RGB(255,246,205):temperature==3?RGB(255,192,126):RGB(223,151,187);
  if(style==0||style==6)c=sky_mix(c,RGB(91,181,255),.20f);else if(style==1||style==7)c=sky_mix(c,RGB(237,128,205),.16f);else if(style==3||style==5)c=sky_mix(c,RGB(255,190,92),.18f);
  if(!high_contrast&&((h&7)==0||(h&63)==1))c=space_fx_twinkle(c,i+(seed&255),game.time);
  pixel((int)q.x,(int)q.y,c);
  if((h&7)==0){pixel((int)q.x+1,(int)q.y,c);pixel((int)q.x,(int)q.y+1,c);}
  if(!high_contrast&&(h&31)==0){
   /* Fixed-size tapered rays. Only their light varies gently; expanding,
    * contracting solid crosses are distracting during camera motion. */
   int x=(int)q.x,y=(int)q.y,flare=3+((h>>5)&1);float light=.91f+.06f*sinf(game.time*.55f+(h&1023)*.006f);
   for(int k=1;k<=flare;k++){unsigned glow=sky_mix(RGB(0,0,0),c,light*(flare+1-k)/(flare+1));
    if(x-k>=xt)pixel(x-k,y,glow);if(x+k<=xb)pixel(x+k,y,glow);
    if(y-k>=yt)pixel(x,y-k,glow);if(y+k<=yb)pixel(x,y+k,glow);
   }
   pixel(x,y,sky_mix(c,WHITE,.35f));
  }
 }
 for(int i=0;i<48;i++){Vec3 d={(float)((i*719)%4000)-2000,(float)((i*353)%4000)-2000,(float)((i*991)%4000)-2000};d.x-=game.pos.x;d.y-=game.pos.y;d.z-=game.pos.z;d.x-=floorf((d.x+2000)/4000)*4000;d.y-=floorf((d.y+2000)/4000)*4000;d.z-=floorf((d.z+2000)/4000)*4000;Vec3 p=camera(&game,add(game.pos,d));if(p.z<100)continue;Point q=project(p);if(q.x<xt||q.x>xb||q.y<yt||q.y>yb)continue;if(game.boost){float stretch=.04f;line((int)q.x,(int)q.y,(int)(q.x+(q.x-proj_ox)*stretch),(int)(q.y+(q.y-proj_oy)*stretch),DIM);}else pixel((int)q.x,(int)q.y,RGB(60,80,105));}
}
static void station_entrance(void){
 float entry=station_entry_z_for(&game,0);
 Vec3 p[4];for(int i=0;i<4;i++){Vec3 corner=station_port_corner_for(&game,0,i);corner.z-=1;p[i]=camera(&game,add(rotate(corner,0,station_angle(&game)),(Vec3){0,0,STATION_Z}));}
 if(game.pos.z<entry){
  int top=view_top(),bot=view_bot();
  for(int i=0;i<4;i++)if(p[i].z>15&&p[(i+1)%4].z>15){
   Point a=project(p[i]),b=project(p[(i+1)%4]);
   /* Warm structural rim + soft cyan aperture (nav cut only). */
   line((int)a.x,(int)a.y,(int)b.x,(int)b.y,RGB(193,139,77));
   line((int)a.x+1,(int)a.y,(int)b.x+1,(int)b.y,RGB(60,140,145));
   if(!high_contrast)sfx_add((int)((a.x+b.x)*.5f),(int)((a.y+b.y)*.5f),RGB(40,90,95),top,bot);
  }
 }
}
static void target_overlay(void){
 pick_look_target();
 if(valid_target(selected_target)){Vec3 p=camera(&game,target_position(selected_target));if(p.z>15){Point q=project(p);if(q.x>16&&q.x<464&&q.y>84&&q.y<168){int x=(int)q.x,y=(int)q.y;unsigned lock=autoaim?CYAN:AMBER;if(IS_NPC_ID(selected_target))lock=faction_colors[game.npc[selected_target-BODY_COUNT-1].role];line(x-14,y-14,x-5,y-14,lock);line(x-14,y-14,x-14,y-5,lock);line(x+14,y+14,x+5,y+14,lock);line(x+14,y+14,x+14,y+5,lock);line(x-14,y+14,x-5,y+14,lock);line(x+14,y-14,x+5,y-14,lock);int pulse=1+(int)(fabsf(sinf(game.time*4))*3);circle(x,y,pulse,lock);if(autoaim){line(x-20,y,x-16,y,CYAN);line(x+16,y,x+20,y,CYAN);line(x,y-20,x,y-16,CYAN);line(x,y+16,x,y+20,CYAN);}}}}
}
/* Bottom-of-canopy combat cue — keeps speech free at the top. */
static int combat_alert_active(void){return game.incoming_missile>0||game.attacked>0||game.collision>0;}
static void combat_alert_banner(void){
 if(!combat_alert_active())return;
 int bot=view_bot(),y=bot-14;if(y<28)y=28;
 if(y+12>=192&&hud_mode==0)y=178; /* sit in the canopy, above the full HUD strip */
 rect(100,y,280,12,RGB(110,14,20));rect(102,y+1,276,10,RGB(62,8,12));
 const char *msg=game.incoming_missile>0?"RED ALERT - MISSILE":game.collision>0?"RED ALERT - IMPACT":"RED ALERT";
 int len=(int)strlen(msg);text(30-len/2,y/8,WHITE,"%s",msg);
}
/* Live aft camera for the cockpit header.  The very wide projection behaves
 * like a vehicle mirror: it sacrifices vertical sky for useful battle
 * awareness, but reads real world positions rather than a radar abstraction. */
static int rear_mirror_project(const Game *view,Vec3 world,int x,int y,int w,int h,int *sx,int *sy,float *depth){
 Vec3 v=camera(view,world);if(v.z<20)return 0;
 float fx=w*.33f,fy=h*.80f;int px=x+w/2+(int)(v.x*fx/v.z),py=y+h/2-(int)(v.y*fy/v.z);
 if(depth)*depth=v.z;if(sx)*sx=px;if(sy)*sy=py;
 return px>=x&&px<x+w&&py>=y&&py<y+h;
}
static unsigned rear_mirror_dim(unsigned c,int num,int den){return RGB((int)((c&255)*num/den),(int)(((c>>8)&255)*num/den),(int)(((c>>16)&255)*num/den));}
static void rear_view_mirror(int x,int y,int w,int h){
 if(w<48||h<12)return;
 unsigned seed=space_sky_seed(),wash=RGB(3+(seed&7),8+((seed>>4)&11),16+((seed>>9)&15));
 rect(x,y,w,h,wash);rect(x,y,w,1,UI_EDGE);rect(x,y+h-1,w,1,UI_RAISED);rect(x,y,1,h,UI_RAISED);rect(x+w-1,y,1,h,UI_RAISED);
 Game rear=game;rear.yaw+=3.14159265f;rear.pitch=-game.pitch;
 int oldox=proj_ox,oldoy=proj_oy,oldx0=clipx0,oldx1=clipx1,oldy0=clipy0,oldy1=clipy1;
 preview_clip(x+w/2,y+h/2,x+1,y+1,x+w-1,y+h-1);
 /* Stable celestial points rotate with the ship, making the strip visibly a
  * camera rather than another decorative animation. */
 for(int i=0;i<92;i++){
  unsigned q=field_hash(seed^(unsigned)(i+1)*0x9e3779b9u);Vec3 d=norm((Vec3){(float)((int)(q&1023)-511),(float)((int)((q>>10)&1023)-511),(float)((int)((q>>20)&1023)-511)});
  int sx,sy;if(!rear_mirror_project(&rear,add(game.pos,mul(d,30000)),x+1,y+1,w-2,h-2,&sx,&sy,0))continue;
  unsigned c=(q&7)?RGB(75,104,130):(q&8)?RGB(255,205,126):RGB(134,198,255);pixel(sx,sy,c);if((q&31)==0)pixel(sx+1,sy,c);
 }
 /* Large real bodies are restrained to rings/discs so a nearby planet cannot
  * erase the entire tactical view. */
 for(int i=0;i<BODY_COUNT;i++){
  int sx,sy;float z;if(!rear_mirror_project(&rear,game.bodies[i].pos,x+1,y+1,w-2,h-2,&sx,&sy,&z))continue;
  int r=(int)(h*.8f*game.bodies[i].radius/z);if(r<1)r=1;if(r>h/2-2)r=h/2-2;
  unsigned c=game.bodies[i].type==SUN?RGB(255,190,74):game.bodies[i].color;circle(sx,sy,r,c);if(r>2)circle(sx,sy,r-2,rear_mirror_dim(c,2,3));
 }
 /* Station/secondary hub markers are world-positioned, not heading glyphs. */
 for(int hub=0;hub<HUB_COUNT;hub++){
  int sx,sy;float z;Vec3 p=hub_position(&game,hub);if(!rear_mirror_project(&rear,p,x+1,y+1,w-2,h-2,&sx,&sy,&z))continue;
  int r=(int)(9000.f/fmaxf(600,z));if(r<2)r=2;if(r>6)r=6;unsigned c=hub?UI_MUTED:UI_SIGNAL;
  line(sx-r,sy,sx,sy-r/2,c);line(sx,sy-r/2,sx+r,sy,c);line(sx+r,sy,sx,sy+r/2,c);line(sx,sy+r/2,sx-r,sy,c);
 }
 for(int i=0;i<NPC_COUNT;i++){
  NPC *n=&game.npc[i];if(!n->alive)continue;int sx,sy;float z;if(!rear_mirror_project(&rear,n->pos,x+1,y+1,w-2,h-2,&sx,&sy,&z))continue;
  int r=(int)(n->radius*72.f/fmaxf(150,z));if(n->freighter)r+=2;if(r<2)r=2;if(r>8)r=8;
  unsigned c=n->flash>0?WHITE:faction_colors[n->role],dim=rear_mirror_dim(c,1,2);Vec3 tail=sub(n->pos,mul(n->dir,n->radius*(n->freighter?5.f:8.f)));int tx,ty;
  if(rear_mirror_project(&rear,tail,x+1,y+1,w-2,h-2,&tx,&ty,0))line(sx,sy,tx,ty,dim);
  line(sx-r,sy,sx,sy-r/2,c);line(sx,sy-r/2,sx+r,sy,c);line(sx+r,sy,sx,sy+r/2,c);line(sx,sy+r/2,sx-r,sy,c);pixel(sx,sy,WHITE);
  int id=NPC_ID_MIN+i,threat=game.fire_bearing_time[i]>0||(game.incoming_missile>0&&game.incoming_source==i);
  if(id==selected_target||threat){unsigned lock=threat?RED:UI_ACCENT;line(sx-r-3,sy-r-2,sx-r,sy-r-2,lock);line(sx-r-3,sy-r-2,sx-r-3,sy,lock);line(sx+r+3,sy+r+2,sx+r,sy+r+2,lock);line(sx+r+3,sy+r+2,sx+r+3,sy,lock);}
 }
 preview_clip(oldox,oldoy,oldx0,oldy0,oldx1,oldy1);
}
static void minimal_overlay(void){
 pick_look_target();
 rect(0,0,W,24,RGB(21,28,39));rect(0,23,W,1,RGB(193,139,77));
 rect(0,248,W,24,RGB(21,28,39));rect(0,248,W,1,RGB(193,139,77));
 rear_view_mirror(4,1,252,22);
 text(43,1,RGB(229,210,163),"SPD %d",(int)game.speed);
 text(1,32,game.energy<30?RED:RGB(85,212,212),"SHIELD %d%%",(int)game.energy);
 int id=valid_target(selected_target)?selected_target:valid_target(look_target)?look_target:-1;
 if(id>=0)text(25,32,RGB(240,180,91),"%.32s",target_name(id));
 combat_alert_banner();
}
static int fire_indicator_position(int source,float *x,float *y,float *dx,float *dy){
 if(source<0||source>=NPC_COUNT||!game.npc[source].alive||(game.fire_bearing_time[source]<=0&&!(game.incoming_missile>0&&game.incoming_source==source)))return 0;
 Vec3 p=camera(&game,game.npc[source].pos);
 float cy=(view_top()+view_bot())*.5f,rx=224,ry=(view_bot()-view_top())*.5f-22;
 if(ry<12)ry=12;
 if(p.z>15){
  Point q=project(p);float ex=(q.x-240)/rx,ey=(q.y-cy)/ry;
  if(ex*ex+ey*ey<.85f){*x=q.x;*y=q.y-12;*dx=0;*dy=1;return 1;}
 }
 float vx=p.x,vy=-p.y;
 if(fabsf(vx)+fabsf(vy)<.01f){vx=0;vy=p.z<0?1:-1;}
 float mag=sqrtf(vx*vx+vy*vy);vx/=mag;vy/=mag;
 *x=240+rx*vx;*y=cy+ry*vy;*dx=vx;*dy=vy;return p.z<0?2:1;
}
static void incoming_fire_indicators(void){
 if(game.docked||game.dead||game.planet>=0||game.jump>0)return;
 for(int i=0;i<NPC_COUNT;i++){
  float x,y,dx,dy;int rear=fire_indicator_position(i,&x,&y,&dx,&dy);if(!rear)continue;
  unsigned ink=game.fire_bearing_time[i]>.8f?RGB(255,110,72):RGB(139,75,56);
  for(int k=0;k<(rear==2?2:1);k++){
   float tx=x-dx*k*4,ty=y-dy*k*4;
   line((int)(tx-dx*6-dy*3),(int)(ty-dy*6+dx*3),(int)tx,(int)ty,ink);
   line((int)(tx-dx*6+dy*3),(int)(ty-dy*6-dx*3),(int)tx,(int)ty,ink);
  }
 }
}
static void distant_ship_trail(const NPC *n){
 if(n->cruise<=0&&n->target==-1)return;
 float distance=length(sub(n->pos,game.pos));
 Vec3 root=sub(n->pos,mul(n->dir,n->radius)),tail=sub(root,mul(n->dir,fminf(1600,fmaxf(240,distance*.07f))));
 Vec3 a=camera(&game,root),b=camera(&game,tail);if(a.z<=30||b.z<=30)return;
 Point p=project(a),q=project(b);int top=view_top()+2,bot=view_bot()-2;
 if(p.x<2||p.x>477||q.x<2||q.x>477||p.y<top||p.y>bot||q.y<top||q.y>bot)return;
 unsigned glow=n->role==LAW?RGB(95,177,255):n->role==PIRATES?RGB(255,108,72):n->role==EXPLORERS?RGB(92,235,192):RGB(255,195,105);
 for(int i=3;i>=1;i--){
  float t=i/3.f,prior=(i-1)/3.f;
  unsigned ink=RGB((glow&255)/(i+1),((glow>>8)&255)/(i+1),((glow>>16)&255)/(i+1));
  line((int)(p.x+(q.x-p.x)*prior),(int)(p.y+(q.y-p.y)*prior),(int)(p.x+(q.x-p.x)*t),(int)(p.y+(q.y-p.y)*t),ink);
 }
 pixel((int)p.x,(int)p.y,glow);
}
static void missile_effects(void){
 int top=view_top(),bot=view_bot();
 if(game.missile_time>0&&!occluded(game.missile_pos)){
  for(int i=1;i<game.missile_trail_n;i++){
   Vec3 a=camera(&game,game.missile_trail[i-1]),b=camera(&game,game.missile_trail[i]);
   if(a.z<=15||b.z<=15)continue;Point p=project(a),q=project(b);
   if(p.x<3||p.x>476||q.x<3||q.x>476||p.y<top+3||p.y>bot-3||q.y<top+3||q.y>bot-3)continue;
   line((int)p.x,(int)p.y,(int)q.x,(int)q.y,i<4?RGB(255,176,72):RGB(108,112,126));
  }
  Vec3 p=camera(&game,game.missile_pos);if(p.z>15){Point q=project(p);
   if(q.x>5&&q.x<474&&q.y>top+5&&q.y<bot-5){int x=(int)q.x,y=(int)q.y;
    circle(x,y,2,GOLD);pixel(x,y,WHITE);
   }
  }
 }
}
static void warp_effect(void){
 if(game.jump<=0)return;
 int top=view_top(),bot=view_bot();
 float elapsed=8-game.jump,progress;
 int phase=elapsed<3?0:elapsed<5?1:2;
 progress=phase==0?elapsed/3.f:phase==1?(elapsed-3)/2.f:(elapsed-5)/3.f;
 int shake=(int)(sinf(game.time*62)*((phase==0?1:phase==1?4:2)*(progress+.2f)));
 unsigned bg=phase==1?RGB(5+(int)(progress*8),12+(int)(progress*18),42+(int)(progress*44)):RGB(3,7,24+(int)(progress*18));
 if(phase==0){
  /* Keep the pre-jump cockpit view visible during the charge-up, then dim it
   * progressively before the corridor takes over. */
  int shade=68-(int)(progress*24);if(shade<38)shade=38;
  for(int y=top;y<=bot;y++)for(int x=0;x<W;x++){unsigned c=fb[y*STRIDE+x];fb[y*STRIDE+x]=RGB(((c&255)*shade)/100,(((c>>8)&255)*shade)/100,(((c>>16)&255)*shade)/100);}
 }else rect(0,top,W,bot-top+1,bg);
 int lines=phase==0?18+(int)(progress*72):phase==1?150:110-(int)(progress*55);if(lines<18)lines=18;
 for(int i=0;i<lines;i++){
  float angle=i*2.39996f+elapsed*.7f;
  float radius=phase==1?8+fmodf(i*17+progress*700,255):12+fmodf(i*17+progress*420,210);
  float end=radius+(phase==0?15+progress*45:phase==1?80+progress*170:170-progress*115);
  int y0=(int)fmaxf(top,fminf(bot,110+sinf(angle)*radius*.5f));
  int y1=(int)fmaxf(top,fminf(bot,110+sinf(angle)*end*.5f));
  unsigned ink=phase==1?(i%5==0?RGB(255,96,220):i%5==1?RGB(75,238,255):i%5==2?RGB(255,194,73):i%5==3?RGB(130,108,255):RGB(210,245,255)):i%4==0?RGB(229,210,163):i%4==1?RGB(85,212,212):i%4==2?RGB(193,139,77):RGB(85,105,160);
  line(240+shake+(int)(cosf(angle)*radius),y0+shake,240+shake+(int)(cosf(angle)*end),y1+shake,ink);
  if((i&3)==0)sfx_add(240+(int)(cosf(angle)*end),y1,ink,top,bot);
 }
 if(route_jump_engaged){
  /* Route jumps tear the live cockpit into short horizontal offsets before
   * the corridor takes over.  This deliberately reaches the instrument
   * panels as well as the canopy, so the transition feels ship-bound. */
  int bands=phase==0?5+(int)(progress*10):phase==1?12:7;
  for(int i=0;i<bands;i++){
   unsigned seed=(unsigned)(i+1)*2654435761u^(unsigned)(elapsed*190.f);
   int y=(int)((seed>>9)%H),span=24+(int)((seed>>17)%112),x=(int)((seed>>2)%(W-span));
   int shift=2+(int)((seed>>27)&7);if(seed&1)shift=-shift;
   if(shift>0){for(int px=x+span-1;px>=x;px--)if(px+shift<W)fb[y*STRIDE+px+shift]=fb[y*STRIDE+px];}
   else {for(int px=x;px<x+span;px++)if(px+shift>=0)fb[y*STRIDE+px+shift]=fb[y*STRIDE+px];}
   line(x,y,x+span,y,(i%3)==0?RGB(255,74,196):(i%3)==1?RGB(70,225,255):RGB(222,207,157));
  }
 }
 rect(80,88,320,28,phase==1?RGB(16,22,56):RGB(21,28,39));rect(80,88,320,2,phase==1?RGB(75,238,255):RGB(193,139,77));
 text(15,12,phase==1?RGB(75,238,255):RGB(240,180,91),phase==0?"HYPERDRIVE CHARGING / %.0f%%":phase==1?"HYPERSPACE CORRIDOR / %.0f%%":"HYPERSPACE BRAKING / %.0f%%",progress*100);
}
static void planet_prompt(void){
 if(game.approach<0||game.approach>=BODY_COUNT)return;
 /* The safety modal always shares the full cockpit's clear centre. */
 int y0=88,solid=game.bodies[game.approach].type!=SUN;
 rect(20,y0,440,84,RGB(21,28,39));rect(20,y0,440,2,RGB(193,139,77));rect(20,y0+82,440,2,RGB(41,54,70));
 text(7,(y0+8)/8,RGB(240,180,91),"LANDING CLEARANCE: %s",game.bodies[game.approach].name);
 if(solid){button_icon(58,y0+38,'X',RGB(85,212,212));text(10,(y0+39)/8,RGB(229,210,163),"land & exit");}
 button_icon(210,y0+38,'O',RED);text(29,(y0+39)/8,RGB(229,210,163),"cancel");
 text(7,(y0+64)/8,RGB(155,154,165),solid?"Guided landing, then automatic disembark.":"No solid surface. Circle turns back.");
}
static void celestial_rims(void){
 int top=view_top(),bot=view_bot();
 for(int i=0;i<BODY_COUNT;i++){
  Body *b=&game.bodies[i];Vec3 v=camera(&game,b->pos);if(v.z<100)continue;Point p=project(v);
  int r=(int)fminf(700,240*b->radius/v.z);if(r<3||p.x+r<0||p.x-r>W||p.y+r<top||p.y-r>bot)continue;
  unsigned tint=b->type==SUN?b->color:b->type==OCEAN?RGB(70,150,195):b->type==GAS?RGB(105,115,143):RGB(133,102,82);
  /* Match the actual disc radius; no detached circular halos on approach. */
  for(int k=3;k>=1;k--){unsigned ink=RGB((tint&255)/(k+2),((tint>>8)&255)/(k+2),((tint>>16)&255)/(k+2));
   for(int s=0;s<96;s++){float angle=s*6.2831853f/96;int x=(int)p.x+(int)(cosf(angle)*(r+k)),y=(int)p.y+(int)(sinf(angle)*(r+k));if(y>=top&&y<=bot)pixel(x,y,ink);}
  }
 }
}
/* Ringed worlds need painter-order depth: the distant half is drawn before
 * the globe, then the near half crosses visibly over its foreground. */
static void celestial_rings(int foreground){
 int top=view_top(),bot=view_bot();float cr=cosf(game.roll),sr=sinf(game.roll);
 for(int i=0;i<BODY_COUNT;i++){
  Body *b=&game.bodies[i];if(b->type!=GAS)continue;
  Vec3 v=camera(&game,b->pos);if(v.z<100)continue;Point p=project(v);
  int r=(int)fminf(700,240*b->radius/v.z);if(r<7||r>240)continue;
  float rx=r*(1.82f+(float)((b->seed>>7)&3)*.08f),ry=r*(.42f+(float)((b->seed>>11)&3)*.025f);
  float skew=((int)((b->seed>>15)&7)-3)*r*.025f;
  if(p.x+rx<0||p.x-rx>W||p.y+rx<top||p.y-rx>bot)continue;
  for(int band=0;band<4;band++){
   float offset=(band-1.5f)*fmaxf(1.f,r*.027f),brx=rx+offset,bry=ry+offset*.32f;
   unsigned ink=foreground?(band==1||band==2?RGB(176,164,151):RGB(121,126,141)):(band==1||band==2?RGB(93,100,119):RGB(64,70,88));
   for(int s=0;s<96;s++){
    float a=s*6.2831853f/96.f,bb=(s+1)*6.2831853f/96.f,mid=(a+bb)*.5f;
    if((sinf(mid)>=0)!=(foreground!=0))continue;
    float lx0=cosf(a)*brx,ly0=sinf(a)*bry+cosf(a)*skew;
    float lx1=cosf(bb)*brx,ly1=sinf(bb)*bry+cosf(bb)*skew;
    int x0=(int)(p.x+lx0*cr-ly0*sr),y0=(int)(p.y+lx0*sr+ly0*cr);
    int x1=(int)(p.x+lx1*cr-ly1*sr),y1=(int)(p.y+lx1*sr+ly1*cr);
    line(x0,y0,x1,y1,ink);
   }
  }
  if(!high_contrast){
   for(int s=0;s<10;s++){
    float a=(s+.5f)*6.2831853f/10.f;if((sinf(a)>=0)!=(foreground!=0))continue;
    float lx=cosf(a)*(rx+r*.08f),ly=sinf(a)*(ry+r*.025f)+cosf(a)*skew;
    int x=(int)(p.x+lx*cr-ly*sr),y=(int)(p.y+lx*sr+ly*cr);
    if(y>=top&&y<=bot)sun_bloom_dot(x,y,foreground?RGB(64,58,54):RGB(35,39,52),0,top,W,bot);
   }
  }
 }
}
static void station_glow(void){
 float entry=station_entry_z_for(&game,0);
 if(game.pos.z>=entry||occluded((Vec3){0,0,entry}))return;
 int top=view_top(),bot=view_bot();
 for(int i=0;i<4;i++){
  Vec3 corner=station_port_corner_for(&game,0,i);corner.z-=2;
  Vec3 v=camera(&game,add(rotate(corner,0,station_angle(&game)),(Vec3){0,0,STATION_Z}));
  if(v.z<20)continue;Point p=project(v);
  if(!high_contrast)sfx_add((int)p.x,(int)p.y,RGB(40,90,95),top,bot);
 }
}
/* Mesh ships are drawn with yaw only; freighters follow full dir. Match that here. */
static Vec3 npc_draw_facing(const NPC *n){
 if(n->freighter)return n->dir;
 Vec3 f={n->dir.x,0,n->dir.z};float L=length(f);return L>.001f?mul(f,1.f/L):(Vec3){0,0,1};
}
static float npc_engine_aft(const NPC *n){
 if(n->freighter)return freight_extent(n).z*.998f; /* capital stern nozzles */
 float aft=0;const Mesh *m=&meshes[n->mesh];
 for(int v=0;v<m->vertices;v++)aft=fmaxf(aft,-m->v[v].z*n->scale);
 if(aft<1)aft=n->radius*.45f;
 return aft;
}
static Vec3 npc_engine_root(const NPC *n,float lateral){
 Vec3 facing=npc_draw_facing(n),side=norm((Vec3){facing.z,0,-facing.x});
 /* Flush to the aft face — not radius-behind, which floated past the silhouette. */
 return add(add(n->pos,mul(facing,-npc_engine_aft(n))),mul(side,lateral));
}
static unsigned npc_trail_neon(const NPC *n){
 /* Keep the plume attached to the same aft root as the mesh. The tint is a
  * readable livery cue at distance, not a second light source under the hull. */
 if(n->role==PIRATES)return RGB(242,68,168);       /* hot magenta */
 if(n->role==EXPLORERS)return RGB(72,224,255);    /* ion cyan */
 if(n->role==LAW)return RGB(116,168,255);         /* patrol blue */
 return n->freighter?RGB(255,190,70):RGB(255,112,48); /* amber / orange */
}
static void ship_sprite_detail(const NPC *n,unsigned color){
 /* Cockpit/engine glint on the mesh aft tip (same anchor as the plume). */
 float d=length(sub(n->pos,game.pos));if(d>3000)return;
 Vec3 c=camera(&game,n->pos);if(c.z<25)return;
 Vec3 back=camera(&game,npc_engine_root(n,0));
 if(back.z<25)return;
 Point p=project(back);int s=(int)fmaxf(1,fminf(3,700.f/c.z));
 world_spark((int)p.x,(int)p.y,s,color);
}
static void npc_engine_glow(void){
 int top=view_top(),bot=view_bot();
 for(int i=0;i<NPC_COUNT;i++){NPC *n=&game.npc[i];if(!n->alive||occluded(n->pos))continue;float d=length(sub(n->pos,game.pos));if(d>(n->freighter?55000.f:18000.f))continue;
  Vec3 facing=npc_draw_facing(n),side=norm((Vec3){facing.z,0,-facing.x});
  float lateral=n->freighter?freight_extent(n).x*.38f:0;int plumes=n->freighter?2:1;
  for(int plume=0;plume<plumes;plume++){
   float offset=n->freighter?(plume?1:-1)*lateral:0;
   Vec3 rear=npc_engine_root(n,offset);Vec3 rv=camera(&game,rear);if(rv.z<25)continue;Point root=project(rv);if(root.x<3||root.x>477||root.y<top+3||root.y>bot-3)continue;
   unsigned neon=npc_trail_neon(n);int pulse=1+(int)(fabsf(sinf(game.time*(n->freighter?2.2f:5.5f)+i+plume))*2);
   world_spark((int)root.x,(int)root.y,d>9000?1:n->freighter?2+pulse:1+pulse,neon);pixel((int)root.x,(int)root.y,WHITE);
   Point last=root;int segments=n->freighter?6:4;float step=n->freighter?180.f:55.f;
   for(int k=1;k<=segments;k++){
    float flicker=sinf(game.time*7+i*1.9f+plume*2.3f+k)*(.55f+k*.28f);
    Vec3 tail=add(add(rear,mul(facing,-step*k)),mul(side,flicker));tail.y+=cosf(game.time*5+i+k)*k*.22f;
    Vec3 tv=camera(&game,tail);if(tv.z<25)break;Point q=project(tv);if(q.x<2||q.x>478||q.y<top+2||q.y>bot-2)break;
    unsigned flame=k==1?neon:k==2?RGB(255,185,70):k<=3?RGB(224,76,28):k<segments?RGB(105,45,30):RGB(47,50,55);
    line((int)last.x,(int)last.y,(int)q.x,(int)q.y,flame);
    if(k<3){pixel((int)q.x+(plume?1:-1),(int)q.y,GOLD);if(k==2)pixel((int)q.x,(int)q.y-1,neon);}
    else if((k&1)==0){int haze=(int)(fabsf(sinf(game.time*2.7f+i+k))*2);if(haze)sfx_add((int)q.x,(int)q.y,RGB(70,76,86),top,bot);}
    last=q;
   }
  }
 }
}
static void lens_flares(void){
 if(high_contrast||occluded(game.bodies[0].pos))return;
 Vec3 p=camera(&game,game.bodies[0].pos);if(p.z<=100)return;Point q=project(p);
 int top=clipy0>=0?clipy0:view_top(),bot=clipy1>=0?clipy1-1:view_bot();
 if(q.x<-60||q.x>=W+60||q.y<top-60||q.y>bot+60)return;
 float power=fmaxf(0,1-length((Vec3){q.x-proj_ox,q.y-proj_oy,0})/320.f);
 if(power<=0)return;
 unsigned tint=game.bodies[0].color;int fam=sun_family(game.bodies[0].seed);
 /* Horizontal anamorphic streak */
 float apparent=240*game.bodies[0].radius/p.z;
 int span=(int)fminf(220,(60+fam*2+apparent*.9f)*power*(1.f+1.3f*fminf(1,game.departure_glow*.5f)));
 if(span<1)return;
 for(int dx=-span;dx<=span;dx+=2){
  int x=(int)q.x+dx,y=(int)q.y;if(x<0||x>=W||y<top||y>bot)continue;
  int fall=span?span-abs(dx):1;unsigned c=RGB(((tint&255)*fall*power)/(span*(3.f-1.6f*fminf(1,game.departure_glow*.5f))),(((tint>>8)&255)*fall*power)/(span*3),(((tint>>16)&255)*fall*power)/(span*3));
  sun_bloom_dot(x,y,c,0,top,W,bot);
  if((dx&3)==0&&y+1<=bot)sun_bloom_dot(x,y+1,c,0,top,W,bot);
 }
 /* Ghost orbs along the optical axis */
 for(int i=0;i<5;i++){float t=.3f+i*.4f;int x=(int)(q.x+(proj_ox-q.x)*t),y=(int)(q.y+(proj_oy-q.y)*t);
  if(x>3&&x<W-3&&y>top+2&&y<bot-2){
   unsigned ghost=i&1?RGB(40,70,100):RGB(90,60,30);
   int r=1+(int)fminf(5,apparent*.025f*power);
   for(int dy=-r;dy<=r;dy++)for(int dx=-r;dx<=r;dx++){int d=dx*dx+dy*dy;if(d<=r*r){float fade=power*(r*r+1-d)/(r*r+1.f)*.4f;sun_bloom_dot(x+dx,y+dy,sky_mix(RGB(0,0,0),ghost,fade),0,top,W,bot);}}
  }
 }
}
static void police_custody_animation(void){
 int transfer=game.police_phase==2||game.police_phase==4;
 float p=1-game.police_timer/(transfer?2.8f:3.5f);p=fmaxf(0,fminf(1,p));
 rect(16,184,448,58,RGB(5,12,23));
 if(transfer){
  for(int i=0;i<24;i++)pixel(20+(i*73+(int)(p*140))%436,189+i*17%45,RGB(112,142,164));
  int x=45+(int)(p*320);
  rect(394,190,48,40,RGB(57,68,83));rect(406,198,24,24,RGB(8,17,28));
  rect(x,203,44,18,RGB(124,142,159));rect(x+33,207,15,10,RGB(78,155,183));
  rect(x-10,208,10,7,RGB(241,153,60));rect(x+8,199,21,4,RGB(64,98,134));
  text_px(24,232,UI_GOLD,"SECURITY SHUTTLE / BERTH APPROACH");
 }else{
  rect(52,189,376,40,RGB(37,44,54));rect(82,208,102,12,RGB(73,89,106));
  for(int i=0;i<9;i++){int x=60+i*43+(int)((1-p)*20);rect(x,188,4,42,RGB(100,116,130));}
  rect(16,239,(int)(448*p),3,UI_GOLD);
  text_px(208,200,UI_CYAN,"DETENTION / REVIEW");
  text_px(208,215,WHITE,"TIME SERVED...");
 }
}
static void police_dialog(void){
 if(!game.police_stop)return;
 int saved_row=row;dialogue_begin("LOCAL LAW");
 if(game.police_phase>=6){
  int seized=game.police_phase==6,surrender=game.police_phase==8;
  dialogue_speech("LOCAL LAW",LAW,VOICE_LAW*37,
   surrender?"Voluntary cargo transfer confirmed. The cargo charge is waived; your local record is clear.":
   seized?"Your detention is complete. With no funds to settle, your ship, cargo and fitted equipment were seized.":
   "Your detention is complete. The release fee was paid on admission. Any restricted goods were confiscated.",
   surrender?"Patrols are standing down. Your legal cargo, ship and funds remain yours.":
   seized?"A basic Adder and fuel have been assigned to your station berth. Your local warrant is cleared. You may rebuild from here.":
   "Your ship, legal cargo and remaining equipment are waiting at the station. Your local warrant is cleared. Welcome back, Commander.");
  dialogue_context(surrender?"CARGO RECEIPT":"RELEASE PAPERS",surrender?"Other systems keep their own records. Confirm when ready to resume flight.":"Station security has returned your flight licence. Confirm when ready to return to your ship.");
  row=0;dialogue_reply(0,surrender?"Understood. Resume flight.":"Collect my release papers and return.");
  footer("X ACKNOWLEDGE   FLIGHT PAUSED");
 }else if(game.police_phase>=2){
  int transfer=game.police_phase==2||game.police_phase==4,unpaid=game.police_phase==2||game.police_phase==3;
  dialogue_speech("LOCAL LAW",LAW,VOICE_LAW*37,
   transfer?"Power down. A security shuttle is transferring you to the station detention wing.":"The cell lights dim. Beyond the door, a clerk reviews your record. Time passes...",
   unpaid?"No funds were available. Property seizure will settle the local warrant; a basic ship will be assigned on release.":"Your release fee is reserved. After processing and time served, your ship will be returned minus restricted cargo.");
  dialogue_context("CUSTODY",transfer?"ESCORT TRANSFER / LOCAL STATION":"DETENTION REVIEW / RELEASE AUTHORISATION");
  police_custody_animation();footer("CUSTODY IN PROGRESS   FLIGHT PAUSED");
 }else{
  int scan=game.police_phase==1,count=cargo_contraband(&game)>0?4:3;
  int selected=police_choice;if(selected<0||selected>=count)selected=0;
  int first=selected/3*3;
  char detail[512];if(scan)snprintf(detail,sizeof(detail),"This is a hold inspection. Submit to a scan, or transfer any restricted goods voluntarily. Refusing or fleeing creates a warrant.");else police_charge_details(&game,detail,sizeof(detail));
  dialogue_speech("LOCAL LAW",LAW,VOICE_LAW*37,scan?"Commander, Local Law here. Reduce power and stand by for a cargo inspection.":police_accusation(&game),detail);
  dialogue_context("YOUR REPLY",game.message_time>0?game.message:cargo_contraband(&game)>0?"Surrender waives the cargo charge only. Other offences remain. Up/down reaches all four replies.":"Pay the fine, accept custody, or run. Custody without funds means property seizure.");
  char custody[96];snprintf(custody,sizeof(custody),game.credits<=0?"Custody: ship and property will be seized":"Accept custody / fee %.1f units",fminf(game.credits,police_fine(&game)/2)*.1f);
  const char *options[]={scan?"Submit to scan":(cargo_contraband(&game)>0?"Pay fine; surrender restricted goods":"Pay the fine and settle my warrant"),scan?"Refuse inspection":custody,"Run from law","Surrender illegal cargo voluntarily"};
  row=selected-first;
  for(int i=first;i<count&&i<first+3;i++)dialogue_reply(i-first,options[i]);
  footer(count==4?(first?"UP/DOWN   REPLY 4/4   X CONFIRM":"UP/DOWN   REPLIES 1-3/4   X CONFIRM"):"UP/DOWN   X CONFIRM   FLIGHT PAUSED");
 }
 row=saved_row;
}
static void death_effect(void){if(!game.dead)return;rect(0,38,W,142,BG);float age=game.explosion;
 const Mesh *m=&meshes[mesh_id(player_ships[game.ship].name)];
 for(int i=0;i<m->triangles;i++){const MeshTri *t=&m->t[i];Vec3 center=mul(add(add(m->v[t->a],m->v[t->b]),m->v[t->c]),1.f/3);Vec3 dir=norm(add(center,(Vec3){sinf(i*4.f)*25,cosf(i*2.f)*25,20}));Vec3 shift=add((Vec3){0,0,340},mul(dir,age*130));int indices[3]={t->a,t->b,t->c};Point p[3];int visible=1;for(int j=0;j<3;j++){Vec3 v=add(shift,rotate(mul(m->v[indices[j]],1.5f),age*.8f,age*.3f));if(v.z<15){visible=0;break;}p[j]=project(v);}if(!visible)continue;for(int j=0;j<3;j++){Point a=p[j],b=p[(j+1)%3];if(a.y>=40&&a.y<178&&b.y>=40&&b.y<178)line((int)a.x,(int)a.y,(int)b.x,(int)b.y,i%3?GOLD:RED);}}
 if(age>1){rect(76,82,328,51,BG);text(23,11,RED,"SHIP DESTROYED");text(13,14,WHITE,"%s",game.system==7&&campaign_training(&game)?"START: training recovery":"START: new commander");}text(2,21,DIM,"WIREFRAME DEBRIS / HULL LOST");}
/* Small, clipped effects anchored to physical contacts; no extra HUD labels. */
static void freight_effects(void){
 for(int i=8;i<36;i+=12){
  NPC *n=&game.npc[i];if(!n->alive||!n->freighter||occluded(n->pos))continue;
  if(n->freight_state!=FREIGHT_ARRIVING&&n->freight_state!=FREIGHT_CHARGING)continue;
  if(n->freight_state==FREIGHT_CHARGING&&game.freight_gap>0)continue;
  Vec3 v=camera(&game,n->pos);if(v.z<50||v.z>22000)continue;Point p=project(v);
  float phase=n->freight_state==FREIGHT_ARRIVING?n->freight_timer/2.5f:1-n->freight_timer/3.f;
  int radius=(int)fminf(64,fmaxf(3,freight_extent(n).z*280/v.z*(.4f+phase)));
  for(int k=0;k<32;k++){float a=k*.1963495f;world_spark((int)p.x+(int)(cosf(a)*radius),(int)p.y+(int)(sinf(a)*radius*.45f),1,RGB(105,169,196));}
  world_spark((int)p.x,(int)p.y,2+(int)(phase*5),RGB(191,216,220));
 }
}
static void mining_effects(void){
 for(int i=0;i<DEBRIS_COUNT;i++){Debris *d=&game.debris[i];if(!d->alive||d->flash<=0||occluded(d->pos))continue;
  Vec3 v=camera(&game,d->pos);if(v.z<30||v.z>2400)continue;Point p=project(v);
  float age=(d->rock?.22f:.65f)-d->flash;
  for(int k=0;k<9;k++){float a=k*2.399963f+i,r=(d->rock?7:12)+age*(d->rock?35:90);
   world_spark((int)(p.x+cosf(a)*r),(int)(p.y+sinf(a)*r*.65f),1,k&1?GOLD:RGB(169,192,198));}
 }
}
