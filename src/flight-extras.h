/* Flight presentation and target selection. Target IDs: station, bodies, NPCs, debris. */
static int selected_target=0,look_target=-1,autoaim=0,scan_cat=2,square_held=0,police_choice=0;
static float r_tap=10;
static const char *scan_cat_names[]={"PLANETS","SHIPS","STATIONS","OTHER"};
static int target_category(int id){
 if(id==0)return 2;
 if(id>0&&id<=BODY_COUNT)return 0;
 if(IS_NPC_ID(id))return 1;
 return 3;
}
static Vec3 target_position(int id){if(id==0)return hub_position(&game,game.docked?game.station_variant:nearest_hub(&game));if(id<=BODY_COUNT)return game.bodies[id-1].pos;if(IS_NPC_ID(id))return game.npc[id-BODY_COUNT-1].pos;if(IS_DEBRIS_ID(id))return game.debris[id-DEBRIS_ID_MIN].pos;return game.anomaly[id-ANOMALY_ID_MIN].pos;}
static const char *target_name(int id){if(id==0)return station_name(&game);if(id<=BODY_COUNT)return game.bodies[id-1].name;static char label[32];if(IS_DEBRIS_ID(id)){Debris *d=&game.debris[id-DEBRIS_ID_MIN];if(d->rock)snprintf(label,sizeof(label),d->rock==2?"ICE ASTEROID":"MINERAL ASTEROID");else if(d->wreck)snprintf(label,sizeof(label),"WRECKAGE #%02d",id-DEBRIS_ID_MIN+1);else snprintf(label,sizeof(label),"%s POD",goods[d->good>=0&&d->good<GOODS?d->good:12].name);return label;}if(IS_ANOMALY_ID(id)){Anomaly *a=&game.anomaly[id-ANOMALY_ID_MIN];snprintf(label,sizeof(label),a->kind?"MERIDIAN ECHO":"STELLAR RIFT");return label;}NPC *n=&game.npc[id-BODY_COUNT-1];int serial=(id*17+game.system*7)&255;if(n->role==LAW)snprintf(label,sizeof(label),"ENCRYPTED // %02X",serial);else if(n->role==PIRATES)snprintf(label,sizeof(label),"RAIDER %c-%02d",'A'+(serial%26),serial%100);else if(n->role==EXPLORERS)snprintf(label,sizeof(label),"GUILD %c-%02d",'A'+(serial%26),serial%100);else if(n->freighter)snprintf(label,sizeof(label),"CAPITAL MERCHANT %02d",serial%100);else snprintf(label,sizeof(label),"MERCHANT %c-%02d",'A'+(serial%26),serial%100);return label;}
static int valid_target(int id){if(id<0||id>ANOMALY_ID_MAX)return 0;if(id<=BODY_COUNT)return 1;if(IS_NPC_ID(id))return game.npc[id-BODY_COUNT-1].alive;if(IS_DEBRIS_ID(id))return game.debris[id-DEBRIS_ID_MIN].alive;return game.anomaly[id-ANOMALY_ID_MIN].alive;}
static int scanner_known(int id){if(!IS_NPC_ID(id)||(game.upgrades&16))return 1;return length(sub(target_position(id),game.pos))<=2500;}
static int nearest_hostile_target(void){int best=-1;float range=1e9f;for(int i=0;i<NPC_COUNT;i++)if(game.npc[i].alive&&(game.npc[i].role==PIRATES||game.npc[i].target==-2)){float d=length(sub(game.npc[i].pos,game.pos));if(d<range){range=d;best=BODY_COUNT+1+i;}}return best;}
static void pick_look_target(void);
static int occluded(Vec3 pos);
static void lock_local_target(int id,const char *kind){
 selected_target=id;scan_cat=target_category(id);nav_body=id>0&&id<=BODY_COUNT?id-1:-1;autoaim=0;
 snprintf(game.message,sizeof(game.message),"%s: %s / HOLD SQUARE + R TO LOCK",kind,target_name(id));game.message_time=2.4f;game.cue=SFX_UI;
 story_event(&game,STORY_EV_TARGET);
}
static int collect_scan_ids(int *ids,int cat){
 int n=0;
 if(cat==0){for(int i=1;i<BODY_COUNT;i++)ids[n++]=i+1;}
 else if(cat==1){for(int i=0;i<NPC_COUNT;i++)if(game.npc[i].alive)ids[n++]=BODY_COUNT+1+i;}
 else if(cat==2)ids[n++]=0;
 else {
  for(int i=0;i<ANOMALY_COUNT;i++)if(game.anomaly[i].alive)ids[n++]=ANOMALY_ID_MIN+i;
  for(int i=0;i<DEBRIS_COUNT;i++)if(game.debris[i].alive)ids[n++]=DEBRIS_ID_MIN+i;
 }
 return n;
}
static void step_scan_cat(int dir){
 for(int n=0;n<4;n++){
  int ids[BODY_COUNT+NPC_COUNT+DEBRIS_COUNT+ANOMALY_COUNT];
  scan_cat=(scan_cat+dir+4)%4;
  if(collect_scan_ids(ids,scan_cat))return;
 }
}
static void tab_flight_category(int dir){
 step_scan_cat(dir);
 int ids[BODY_COUNT+NPC_COUNT+DEBRIS_COUNT+ANOMALY_COUNT],n=collect_scan_ids(ids,scan_cat);
 if(!n){message(&game,"No contacts in that band.");return;}
 int best=0;float br=1e9f;
 for(int i=0;i<n;i++){float d=length(sub(target_position(ids[i]),game.pos));if(d<br){br=d;best=i;}}
 static const char *lab[]={"Planet","Ship","Station","Other"};
 lock_local_target(ids[best],lab[scan_cat]);
}
static void cycle_scan_item(int dir){
 int ids[BODY_COUNT+NPC_COUNT+DEBRIS_COUNT+ANOMALY_COUNT],n=collect_scan_ids(ids,scan_cat);
 if(!n){message(&game,"No contacts in that band.");return;}
 int cur=-1;for(int i=0;i<n;i++)if(ids[i]==selected_target)cur=i;
 if(cur<0)cur=dir>0?-1:0;
 static const char *lab[]={"Planet","Ship","Station","Other"};
 lock_local_target(ids[(cur+dir+n)%n],lab[scan_cat]);
}
static void cycle_front_target(void){
 int ids[1+BODY_COUNT+NPC_COUNT+DEBRIS_COUNT+ANOMALY_COUNT],n=0,cur=-1;
 int top=view_top()+3,bot=view_bot()-3;
 for(int id=0;id<=ANOMALY_ID_MAX;id++)if(valid_target(id)){
  Vec3 world=target_position(id),v=camera(&game,world);if(v.z<20)continue;
  Point p=project(v);if(p.x<8||p.x>W-8||p.y<top||p.y>bot)continue;
  if(id>BODY_COUNT&&occluded(world))continue;
  ids[n]=id;if(id==selected_target)cur=n;n++;
 }
 if(!n){message(&game,"No contacts in front of the ship.");return;}
 int next=cur<0?0:(cur+1)%n;lock_local_target(ids[next],"In view");
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
static int speech_active(void){
 if(!quiet_comms&&game.voice_time>0&&game.voice[0])return 1;
 if(game.message_time>0&&!game.dead&&game.message[0]&&strncmp(game.message,"WELCOME",7)&&strncmp(game.message,"TRIANGLE",8))return 1;
 return 0;
}
static void speech_ok(void){game.voice_time=0;game.voice[0]=0;game.message_time=0;game.cue=SFX_UI;}
static void contact_speak(int role,const char *line){speak(&game,VOICE_CONTACT,line);game.voice_role=role;game.voice_seed=game.system*NPC_COUNT+selected_target;}
static void hail_target(void){
 pick_look_target();
 int id=look_target>=0?look_target:selected_target;
 if(!valid_target(id)){message(&game,"No one on comms. Lock a ship, then Triangle.");return;}
 if(IS_NPC_ID(id)){
  if(mission_interact(&game,id))return;
  NPC *n=&game.npc[id-BODY_COUNT-1];
  selected_target=id;scan_cat=1;autoaim=0;story_event(&game,STORY_EV_TARGET);
  if(n->role==PIRATES)speak(&game,VOICE_COMP,"No reply. They're painting us.");
  else if(n->role==LAW)contact_speak(LAW,game.legal?"Stop. Pay the fine or take custody.":"Clear. Keep the lane clean.");
  else if(n->role==TRADERS){if(n->freighter){char cargo[80];snprintf(cargo,sizeof(cargo),"%s %s. %d%c %s. %s.",n->freight_state<=FREIGHT_INBOUND?"From":"To",game.systems[n->freight_peer].name,n->freight_qty,goods[n->freight_good].unit,goods[n->freight_good].name,freight_status(n));contact_speak(TRADERS,cargo);}else contact_speak(TRADERS,"Market's open at the hub. Don't scrape the paint.");}
  else contact_speak(EXPLORERS,"Survey channel. We are mapping this sky.");
  return;
 }
 if(id==0){selected_target=0;scan_cat=2;speak(&game,VOICE_VENN,"Tower. Need a docking slot?");return;}
 if(id>0&&id<=BODY_COUNT){speak(&game,VOICE_COMP,"No one lives on that body.");return;}
 if(IS_ANOMALY_ID(id)){speak(&game,VOICE_KEI,"That's an echo. Close in and scan.");return;}
 speak(&game,VOICE_COMP,"Salvage doesn't talk. Circle to collect.");
}
static int occluded(Vec3 pos){Vec3 delta=sub(pos,game.pos);float distance=length(delta);Vec3 ray=norm(delta);for(int i=0;i<BODY_COUNT;i++){Vec3 d=sub(game.bodies[i].pos,game.pos);float along=dot(d,ray);if(along>0&&along<distance&&length(sub(d,mul(ray,along)))<game.bodies[i].radius)return 1;}return 0;}
static void pick_look_target(void){look_target=-1;float best=1e9f;for(int id=0;id<=ANOMALY_ID_MAX;id++){if(!valid_target(id))continue;Vec3 world=target_position(id),p=camera(&game,world);if(p.z<1)continue;float radius=id>0&&id<=BODY_COUNT?game.bodies[id-1].radius:IS_NPC_ID(id)?game.npc[id-BODY_COUNT-1].radius:IS_DEBRIS_ID(id)?game.debris[id-DEBRIS_ID_MIN].radius:IS_ANOMALY_ID(id)?40:160;float lateral=sqrtf(p.x*p.x+p.y*p.y);if(lateral>fmaxf(radius,p.z*.08f))continue;float distance=length(p)-radius;if(distance<best&&(id>0&&id<=BODY_COUNT?1:!occluded(world))){best=distance;look_target=id;}}}
static void align_target(float dt,float ax,float ay){
 if(!autoaim)return;
 if(!valid_target(selected_target)||fabsf(ax)>.1f||fabsf(ay)>.1f){autoaim=0;return;}
 Vec3 d=norm(sub(target_position(selected_target),game.pos));float yaw=atan2f(d.x,d.z),ny=d.y;if(ny>1)ny=1;if(ny<-1)ny=-1;float pitch=asinf(ny);float diff=atan2f(sinf(yaw-game.yaw),cosf(yaw-game.yaw));game.yaw+=fmaxf(-dt*2,fminf(dt*2,diff));game.pitch+=fmaxf(-dt*2,fminf(dt*2,pitch-game.pitch));
}
static void starfield(void){
 /* Fixed celestial directions and a separate nearby dust layer for parallax. */
 static Vec3 stars[600];static int initialized=0;
 if(!initialized){unsigned seed=91731;for(int i=0;i<600;i++){seed=seed*1664525u+1013904223u;float a=(seed&65535)*6.2831853f/65536;seed=seed*1664525u+1013904223u;float y=(seed&65535)/32767.5f-1;float h=sqrtf(1-y*y);stars[i]=(Vec3){cosf(a)*h,y,sinf(a)*h};}initialized=1;}
 int xt=clipy0>=0?clipx0:1,xb=clipy0>=0?clipx1-1:478,yt=clipy0>=0?clipy0:view_top(),yb=clipy0>=0?clipy1-1:view_bot();
 for(int i=0;i<600;i++){Vec3 p=camera(&game,add(game.pos,mul(stars[i],30000)));if(p.z<100)continue;Point q=project(p); if(q.x<xt||q.x>xb||q.y<yt||q.y>yb)continue;unsigned c=i%7==0?RGB(196,215,255):i%11==0?RGB(240,213,164):RGB(95+i%90,110+i%90,140+i%90);if((i%17)==0){int tw=(int)(sinf(game.time*(1.2f+(i&3)*.25f)+i)*18);c=RGB((int)fmaxf(25,(c&255)+tw),(int)fmaxf(30,((c>>8)&255)+tw),(int)fmaxf(40,((c>>16)&255)+tw));}pixel((int)q.x,(int)q.y,c);if(i%29==0){pixel((int)q.x+1,(int)q.y,c);pixel((int)q.x,(int)q.y+1,c);}}
 for(int i=0;i<48;i++){Vec3 d={(float)((i*719)%4000)-2000,(float)((i*353)%4000)-2000,(float)((i*991)%4000)-2000};d.x-=game.pos.x;d.y-=game.pos.y;d.z-=game.pos.z;d.x-=floorf((d.x+2000)/4000)*4000;d.y-=floorf((d.y+2000)/4000)*4000;d.z-=floorf((d.z+2000)/4000)*4000;Vec3 p=camera(&game,add(game.pos,d));if(p.z<100)continue;Point q=project(p);if(q.x<xt||q.x>xb||q.y<yt||q.y>yb)continue;if(game.boost){float stretch=.04f;line((int)q.x,(int)q.y,(int)(q.x+(q.x-proj_ox)*stretch),(int)(q.y+(q.y-proj_oy)*stretch),DIM);}else pixel((int)q.x,(int)q.y,RGB(60,80,105));}
}
static void station_entrance(void){
 Vec3 p[4];for(int i=0;i<4;i++){Vec3 corner=station_port_corner(i);corner.z-=1;p[i]=camera(&game,add(rotate(corner,0,station_angle(&game)),(Vec3){0,0,STATION_Z}));}
 if(game.pos.z<3340)for(int i=0;i<4;i++)if(p[i].z>15&&p[(i+1)%4].z>15){Point a=project(p[i]),b=project(p[(i+1)%4]);line((int)a.x,(int)a.y,(int)b.x,(int)b.y,CYAN);}
}
static void target_overlay(void){
 pick_look_target();
 if(valid_target(selected_target)){Vec3 p=camera(&game,target_position(selected_target));if(p.z>15){Point q=project(p);if(q.x>16&&q.x<464&&q.y>84&&q.y<168){int x=(int)q.x,y=(int)q.y;unsigned lock=autoaim?CYAN:AMBER;if(IS_NPC_ID(selected_target))lock=faction_colors[game.npc[selected_target-BODY_COUNT-1].role];line(x-14,y-14,x-5,y-14,lock);line(x-14,y-14,x-14,y-5,lock);line(x+14,y+14,x+5,y+14,lock);line(x+14,y+14,x+14,y+5,lock);line(x-14,y+14,x-5,y+14,lock);line(x+14,y-14,x+5,y-14,lock);int pulse=1+(int)(fabsf(sinf(game.time*4))*3);circle(x,y,pulse,lock);if(autoaim){line(x-20,y,x-16,y,CYAN);line(x+16,y,x+20,y,CYAN);line(x,y-20,x,y-16,CYAN);line(x,y+16,x,y+20,CYAN);}}}}
}
static void minimal_overlay(void){
 pick_look_target();
 rect(0,0,W,24,RGB(6,15,24));rect(0,248,W,24,RGB(6,15,24));
 text(1,1,CYAN,"System: %.11s",game.systems[game.system].name);
 text(43,1,WHITE,"SPD %d",(int)game.speed);
 text(1,32,game.energy<30?RED:CYAN,"SHIELD %d%%",(int)game.energy);
 int id=valid_target(selected_target)?selected_target:valid_target(look_target)?look_target:-1;
 if(id>=0)text(25,32,GOLD,"%.32s",target_name(id));
 if(game.incoming_missile>0||game.attacked>0||game.collision>0){rect(8,24,464,16,RGB(70,15,22));text(2,4,WHITE,game.incoming_missile>0?"MISSILE INBOUND - BOOST TO EVADE":game.attacked>0?"UNDER ATTACK":"COLLISION - SLOW DOWN");}
}
static void missile_effects(void){int top=view_top(),bot=view_bot();if(game.missile_time>0){Vec3 p=camera(&game,game.missile_pos);if(p.z>15){Point q=project(p);int x=(int)q.x,y=(int)q.y;if(x>2&&x<478&&y>top&&y<bot){circle(x,y,3,GOLD);line(x,y,x-6,y+8,RED);}}}if(game.incoming_missile>0&&game.incoming_source>=0&&game.incoming_source<NPC_COUNT&&game.npc[game.incoming_source].alive){Vec3 p=camera(&game,game.npc[game.incoming_source].pos);if(p.z>15){Point q=project(p);if(q.x>4&&q.x<476&&q.y>top+4&&q.y<bot-4)circle((int)q.x,(int)q.y,8,RED);}}}
static void warp_effect(void){if(game.jump<=0)return;int top=view_top(),bot=view_bot();rect(0,top,W,bot-top+1,RGB(4,8,24));float progress=(5-game.jump)/5;for(int i=0;i<85;i++){float angle=i*2.39996f;float radius=15+fmodf(i*19+progress*450,230);float end=radius+20+progress*90;int y0=(int)fmaxf(top,fminf(bot,110+sinf(angle)*radius*.5f)),y1=(int)fmaxf(top,fminf(bot,110+sinf(angle)*end*.5f));line(240+(int)(cosf(angle)*radius),y0,240+(int)(cosf(angle)*end),y1,i%3?CYAN:WHITE);}rect(80,88,320,28,DASH);rect(80,88,320,2,AMBER);text(15,12,AMBER,"WARP TO %.12s  %.1f",game.systems[game.destination].name,game.jump);}
static void planet_prompt(void){if(game.approach<0)return;int y0=hud_mode==0?88:view_top()+8;rect(20,y0,440,84,DASH);rect(20,y0,440,2,AMBER);text(7,12,AMBER,"APPROACH: %s",game.bodies[game.approach].name);button_icon(58,y0+38,'X',CYAN);text(10,(y0+39)/8,WHITE,"fly the surface");button_icon(210,y0+38,'O',RED);text(29,(y0+39)/8,WHITE,"turn back");text(7,19,AMBDIM,game.bodies[game.approach].type==GAS?"Gas giant: scan it, then reverse away.":"Land on the cyan pad, then O to walk.");}
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
  if(b->type==GAS&&r>7&&r<180)for(int s=0;s<64;s++){
   float a=s*6.2831853f/64,bb=(s+1)*6.2831853f/64,rx=r*1.5f,ry=r*.36f;
   int x0=(int)(p.x+cosf(a)*rx),y0=(int)(p.y+sinf(a)*ry+cosf(a)*r*.14f);
   int x1=(int)(p.x+cosf(bb)*rx),y1=(int)(p.y+sinf(bb)*ry+cosf(bb)*r*.14f);
   if(y0>=top&&y0<=bot&&y1>=top&&y1<=bot)line(x0,y0,x1,y1,RGB(100,109,133));
  }
 }
}
static void station_glow(void){
 if(game.pos.z>=STATION_ENTRY_Z||occluded((Vec3){0,0,STATION_ENTRY_Z}))return;
 for(int i=0;i<4;i++){Vec3 corner=station_port_corner(i);corner.z-=2;Vec3 v=camera(&game,add(rotate(corner,0,station_angle(&game)),(Vec3){0,0,STATION_Z}));if(v.z<20)continue;Point p=project(v);world_spark((int)p.x,(int)p.y,3,CYAN);}
}
static void npc_engine_glow(void){
 int top=view_top(),bot=view_bot();
 for(int i=0;i<NPC_COUNT;i++){NPC *n=&game.npc[i];if(!n->alive||occluded(n->pos))continue;float d=length(sub(n->pos,game.pos));if(d>9000)continue;
  Vec3 extent=n->freighter?freight_extent(n):(Vec3){n->radius,n->radius,n->radius*1.7f};
  Vec3 side=norm((Vec3){n->dir.z,0,-n->dir.x});int plumes=n->freighter?2:1;
  for(int plume=0;plume<plumes;plume++){
   float offset=n->freighter?(plume?1:-1)*extent.x*.38f:0;
   Vec3 rear=add(add(n->pos,mul(n->dir,-extent.z)),mul(side,offset));Vec3 rv=camera(&game,rear);if(rv.z<25)continue;Point root=project(rv);if(root.x<3||root.x>477||root.y<top+3||root.y>bot-3)continue;
   int pulse=1+(int)(fabsf(sinf(game.time*(n->freighter?2.2f:5.5f)+i+plume))*2);world_spark((int)root.x,(int)root.y,n->freighter?2+pulse:1+pulse,RGB(255,218,125));pixel((int)root.x,(int)root.y,WHITE);
   Point last=root;int segments=n->freighter?6:4;float step=n->freighter?30.f:13.f;
   for(int k=1;k<=segments;k++){
    float flicker=sinf(game.time*7+i*1.9f+plume*2.3f+k)*(.8f+k*.35f);
    Vec3 tail=add(add(rear,mul(n->dir,-step*k)),mul(side,flicker));tail.y+=cosf(game.time*5+i+k)*k*.28f;
    Vec3 tv=camera(&game,tail);if(tv.z<25)break;Point q=project(tv);if(q.x<2||q.x>478||q.y<top+2||q.y>bot-2)break;
    unsigned flame=k==1?RGB(255,185,70):k<=3?RGB(224,76,28):k<segments?RGB(105,45,30):RGB(47,50,55);
    line((int)last.x,(int)last.y,(int)q.x,(int)q.y,flame);if(k<3)pixel((int)q.x+(plume?1:-1),(int)q.y,GOLD);last=q;
   }
  }
 }
}
static void lens_flares(void){
 if(high_contrast||occluded(game.bodies[0].pos))return;
 Vec3 p=camera(&game,game.bodies[0].pos);if(p.z<=100)return;Point q=project(p);
 int top=clipy0>=0?clipy0:view_top(),bot=clipy1>=0?clipy1-1:view_bot();
 if(q.x<0||q.x>=W||q.y<top||q.y>bot)return;
 float power=fmaxf(0,1-length((Vec3){q.x-proj_ox,q.y-proj_oy,0})/280.f);
 if(power<=0)return;
 unsigned tint=game.bodies[0].color;
 for(int i=0;i<4;i++){float t=.5f+i*.43f;int x=(int)(q.x+(proj_ox-q.x)*t),y=(int)(q.y+(proj_oy-q.y)*t);
  world_spark(x,y,2+i,i&1?RGB(62,101,139):RGB(119,86,48));
 }
 int span=(int)(22*power);for(int dx=-span;dx<=span;dx+=2)world_spark((int)q.x+dx,(int)q.y,1,tint);
}
static void police_dialog(void){if(!game.police_stop)return;const int bx=76,by=38,bw=388,bh=65;unsigned edge=faction_colors[LAW];draw_portrait(16,45,48,48,VOICE_LAW*37,LAW);rect(bx,by,bw,bh,RGB(14,29,39));rect(bx,by,bw,2,edge);rect(bx,by+bh-2,bw,2,RGB(30,78,86));rect(bx+bw-2,by,2,bh,edge);line(bx,by+20,bx-12,by+28,edge);line(bx-12,by+28,bx,by+36,edge);rect(bx-3,by+22,4,13,RGB(14,29,39));text(11,6,edge,"LOCAL LAW SAYS");text(11,8,WHITE,"Commander, your vessel is under local arrest.");text(11,10,WHITE,"Warrant %d/5 in %s. Choose now.",wanted_level(&game),game.systems[game.system].name);text(2,14,DIM,"YOUR RESPONSE");const char *opts[]={"PAY FINE AND LEAVE","ACCEPT STATION CUSTODY","RUN FROM LAW"};for(int i=0;i<3;i++){int y=16+i*3;if(i==police_choice)selected(y);text(3,y,i==police_choice?WHITE:DIM,"%s %s",i==police_choice?">":" ",opts[i]);if(i==0)text(32,y,i==police_choice?CYAN:DIM,"%.1f U",police_fine(&game)*.1f);if(i==1)text(32,y,i==police_choice?GOLD:DIM,"UP TO %.1f U",police_fine(&game)*.05f);if(i==2)text(32,y,i==police_choice?RED:DIM,"WARRANT + PURSUIT");}text(2,26,DIM,"UP/DOWN CHOOSE   X CONFIRM   FLIGHT PAUSED");}
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



