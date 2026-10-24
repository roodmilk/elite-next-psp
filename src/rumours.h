/* Space Secrets: local, stable world knowledge; no mission slots or sensor cheats. */
static int rumour_area_ready(void);
static Vec3 rumour_area_position(void);
static int rumour_target(int k){return k<8?DISCOVERY_ID_MIN+k:k<12?ANOMALY_ID_MIN+k-8:WHALE_TARGET_ID;}
static Vec3 rumour_object_position(int k){return k<8?discovery_position(k):k<12?game.anomaly[k-8].pos:whale_position(&game,0);}
static int rumour_safe(Vec3 p,float margin){
 if(!isfinite(p.x)||!isfinite(p.y)||!isfinite(p.z))return 0;
 for(int i=0;i<BODY_COUNT;i++)if(body_active(&game.bodies[i])&&length(sub(p,game.bodies[i].pos))<game.bodies[i].radius+margin)return 0;
 for(int i=0;i<HUB_COUNT;i++)if(length(sub(p,hub_position(&game,i)))<400+margin)return 0;
 return 1;
}
static int rumour_valid(int k){
 if(k>=RUMOUR_SLOTS&&k<RUMOUR_SLOTS+3)return navigation_enabled(&game);
 if(k<0||k>=RUMOUR_SLOTS||game.system<0||game.system>=256||opening_active(&game))return 0;
 if(k<8){if(k>=discovery_count())return 0;}
 else if(k<12){if(!game.anomaly[k-8].alive)return 0;}
 else if(!system_whales(game.system))return 0;
 return rumour_safe(k==12?whale_home(&game):rumour_object_position(k),150);
}
static int rumour_kind(int k){
 if(k>=RUMOUR_SLOTS)return 0;
 if(k==12)return 2;if(k>=8)return 3;
 int e=discovery_entry(k);return e==61||e==74?1:0;
}
static const char *rumour_name(int k){
 if(k>=RUMOUR_SLOTS){static const char*names[]={"Outer transit buoy sighting","Cloud-side buoy sighting","Survey buoy sighting"};return names[(k-RUMOUR_SLOTS)%3];}
 if(k==12)return "Space whale pod";
 if(k>=8)return "Uncharted signal";
 return discovery_catalogue[discovery_entry(k)].name;
}
static unsigned rumour_hash(int k){return ((unsigned)game.system+1)*2654435761u+(unsigned)(k+1)*2246822519u;}
static Vec3 rumour_centre(int k){
 Vec3 p=k==12?whale_home(&game):rumour_object_position(k);unsigned h=rumour_hash(k);
 float a=(h&65535)*.0000958738f;
 Vec3 q=add(p,(Vec3){cosf(a)*700,350,sinf(a)*700});
 if(rumour_safe(q,180))return q;
 /* Alternate deterministic points; never a centre buried inside a planet. */
 for(int i=0;i<12;i++){a+=.5235988f;q=add(p,(Vec3){cosf(a)*700,350,sinf(a)*700});if(rumour_safe(q,180))return q;}
 return add(p,(Vec3){0,800,0});
}
static float rumour_radius(int k){return k==12?2100:1700;}
static int rumour_found(int k){
 if(k>=RUMOUR_SLOTS){static const int buoys[]={0,2,4};return (game.navigation.scanned[game.system]>>(buoys[(k-RUMOUR_SLOTS)%3]))&1;}
 if(k<8&&(navigation_source(&game,NAV_DISCOVERY(k))&NAV_VISITED))return 1;
 if(k>=8&&k<12&&(navigation_source(&game,NAV_ANOMALY(k-8))&NAV_VISITED))return 1;
 unsigned b=1u<<k;identity_sync();
 if(k<8&&(discovery_identified[game.system]&b))return 1;
 if(k>=8&&k<12&&(game.rift_logged[game.system]&(1u<<(k-8))))return 1;
 return (game.rumours.found[game.system]&b)!=0;
}
static int rumour_price(int k){if(k>=RUMOUR_SLOTS)return 25+(k-RUMOUR_SLOTS)*10;return (rumour_kind(k)==1?75:rumour_kind(k)==2?80:rumour_kind(k)==3?40:55)+(int)((rumour_hash(k)>>17)%5)*5;}
static int rumour_owned(int k){if(k>=RUMOUR_SLOTS)return (game.navigation.paid[game.system]>>(k-RUMOUR_SLOTS))&1;return (game.rumours.owned[game.system]&(1u<<k))!=0;}
static int rumour_slot(void){return game.rumours.active?(int)((game.rumours.active-1)%RUMOUR_SLOTS):-1;}
static int rumour_area_ready(void){
 return game.rumours.active&&(int)((game.rumours.active-1)/RUMOUR_SLOTS)==game.system&&rumour_valid(rumour_slot())&&!(game.rumours.refunded[game.system]&(1u<<rumour_slot()));
}
static Vec3 rumour_area_position(void){return rumour_area_ready()?rumour_centre(rumour_slot()):game.pos;}
/* 1 newly purchased, 2 already known, negative = honest failure. */
static int rumour_purchase(int k){
 if(k>=RUMOUR_SLOTS)return rumour_valid(k)?navigation_rumour_buy(&game,k-RUMOUR_SLOTS):-1;
 if(!rumour_valid(k)||!rumour_safe(rumour_centre(k),180))return -1;
 if(rumour_found(k)||rumour_owned(k))return 2;
 int cost=rumour_price(k)*10;if(game.credits<cost)return -2;
 /* Both fields belong to the same CRC-protected commander record. */
 game.credits-=cost;game.rumours.owned[game.system]|=1u<<k;
 game.rumours.paid[game.system][k]=cost;game.cue=SFX_SELECT;return 1;
}
static int rumour_track(int k){
 if(k>=RUMOUR_SLOTS){if(!rumour_valid(k)||(!rumour_owned(k)&&!rumour_found(k)))return 0;game.navigation.search_system=game.system;game.navigation.search_key=k-RUMOUR_SLOTS+1;game.navigation.waypoint_key=0;return 1;}
 if(!rumour_valid(k)||(!rumour_owned(k)&&!rumour_found(k))||(game.rumours.refunded[game.system]&(1u<<k)))return 0;
 if(rumour_found(k))game.rumours.found[game.system]|=1u<<k;
 game.rumours.active=1+game.system*RUMOUR_SLOTS+k;return 1;
}
static int rumour_arrived_key=-1;
static void rumour_tick(void){
 if(opening_active(&game)||game.jump>0||game.dead||game.planet>=0)return;
 if(game.rumours.active&&(int)((game.rumours.active-1)/RUMOUR_SLOTS)!=game.system)game.rumours.active=0;
 identity_sync();
 for(int k=0;k<RUMOUR_SLOTS;k++){
  unsigned b=1u<<k;if(rumour_found(k))game.rumours.found[game.system]|=b;
  if(rumour_owned(k)&&!(game.rumours.refunded[game.system]&b)&&!rumour_valid(k)){
   int paid=game.rumours.paid[game.system][k];
   if(game.credits<=100000000-paid){
    game.credits+=paid;game.rumours.refunded[game.system]|=b;
    char note[96];snprintf(note,sizeof(note),"SpacePay refunded %.1f U. That sighting is no longer available.",paid*.1f);message(&game,note);
   }
  }
 }
 if(!rumour_area_ready()){game.rumours.active=0;rumour_arrived_key=-1;return;}
 int key=(int)game.rumours.active;
 if(length(sub(game.pos,rumour_area_position()))<=rumour_radius(rumour_slot())&&rumour_arrived_key!=key){
  rumour_arrived_key=key;message(&game,"Search area reached. Watch your scanner for contacts.");
 }
}
