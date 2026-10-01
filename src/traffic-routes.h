/* Bounded local traffic: four repeatable station/world lanes. No save pointers. */
enum {TRAFFIC_CRUISE,TRAFFIC_SERVICE,TRAFFIC_PATROL,TRAFFIC_INTERCEPT,TRAFFIC_ESCAPE};
static void traffic_network(Game *g){
 g->law_scan_valid=0;memset(g->law_echo_seen,0,sizeof(g->law_echo_seen));
 for(int r=0;r<4;r++){
  Vec3 end=traffic_world_point(g,r+1),start={0,600,2400};
  Vec3 side=norm((Vec3){end.z-start.z,0,start.x-end.x});
  start=add(start,mul(side,(r-1.5f)*340));
  g->traffic_nodes[r][0]=start;
  g->traffic_nodes[r][1]=add(add(start,mul(sub(end,start),.5f)),
   add(mul(side,r>=2?4200:700),(Vec3){0,r>=2?1800:700,0}));
  g->traffic_nodes[r][2]=end;
 }
}
static void traffic_assign(Game *g,NPC *n,int i,int distributed){
 if(n->freighter)return;
 n->route_id=n->role==LAW?(i+g->system)%2:n->role==PIRATES?2+(i+g->system)%2:(i+g->system)%4;
 n->route_leg=1;n->route_step=1;n->route_wait=0;n->escape_time=0;n->player_tag=0;
 n->route_task=n->role==LAW?TRAFFIC_PATROL:TRAFFIC_CRUISE;
 if(n->role==EXPLORERS)return; /* Survey wings retain their formation flight. */
 int r=n->route_id;float t=distributed?.1f+((i*37+g->system*13)%80)*.01f:0;
 if(n->role==PIRATES){n->route_leg=2;t=.7f;}
 n->pos=add(g->traffic_nodes[r][n->route_leg-1],mul(sub(g->traffic_nodes[r][n->route_leg],g->traffic_nodes[r][n->route_leg-1]),t));
 n->pos=add(n->pos,(Vec3){(i%3-1)*180.f,(i%5)*90.f,0});
 n->dir=norm(sub(g->traffic_nodes[r][n->route_leg],n->pos));n->target=-1;
}
static Vec3 traffic_refuge(const Game *g,const NPC *n){
 Vec3 p=g->traffic_nodes[n->route_id][1];
 return add(p,mul(norm(sub(p,g->traffic_nodes[n->route_id][0])),6500));
}
static Vec3 traffic_aim(Game *g,NPC *n,float dt){
 n->player_tag=fmaxf(0,n->player_tag-dt);n->escape_time=fmaxf(0,n->escape_time-dt);
 n->route_wait=fmaxf(0,n->route_wait-dt);
 if(n->escape_time>0){n->route_task=TRAFFIC_ESCAPE;return traffic_refuge(g,n);}
 if(n->route_wait>0){n->route_task=TRAFFIC_SERVICE;return n->pos;}
 n->route_task=n->role==LAW?TRAFFIC_PATROL:TRAFFIC_CRUISE;
 Vec3 aim=g->traffic_nodes[n->route_id][n->route_leg];
 if(length(sub(aim,n->pos))<450){
  if(n->route_leg==0||n->route_leg==2){n->route_step=-n->route_step;n->route_wait=n->role==TRADERS?10+(n->route_id*3):0;}
  n->route_leg+=n->route_step;aim=g->traffic_nodes[n->route_id][n->route_leg];
 }
 return aim;
}
static float traffic_speed(const NPC *n){
 if(n->waypoint>=8)return n->cruise; /* Scripted/rescue holding patterns. */
 if(n->escape_time>0)return n->cruise*1.65f;
 if(n->target!=-1)return 300;
 if(n->route_wait>0)return 0;
 return n->role==EXPLORERS?650:n->cruise;
}
static int traffic_in_lane(const Game *g,const NPC *n){
 for(int leg=0;leg<2;leg++){Vec3 a=g->traffic_nodes[n->route_id][leg],v=sub(g->traffic_nodes[n->route_id][leg+1],a);
  float t=fmaxf(0,fminf(1,dot(sub(n->pos,a),v)/fmaxf(1,dot(v,v))));
  if(length(sub(n->pos,add(a,mul(v,t))))<5000)return 1;
 }return 0;
}
const char *traffic_activity(const NPC *n){
 if(n->freighter)return "FREIGHT SCHEDULE";
 if(n->escape_time>0)return "EVADING / OUTER LANE";
 if(n->target!=-1)return "INTERCEPTING";
 if(n->route_wait>0)return "TRANSFERRING CARGO";
 if(n->role==LAW)return "PATROLLING MAIN LANE";
 if(n->role==PIRATES)return "HUNTING OUTER LANE";
 if(n->role==EXPLORERS)return "SURVEY FORMATION";
 return n->route_step>0?"BOUND FOR WORLD PORT":"BOUND FOR STATION";
}
int law_scan(Game *g){
 if(g->docked||g->dead||g->planet>=0||g->jump>0||g->dock_stage||g->police_stop)return 0;
 if(fit_find(g,LAW_SCANNER_ITEM)<0){message(g,"Fit a Law Scanner in a UTIL slot first.");return 0;}
 if(g->law_scan_valid&&g->time-g->law_scan_time<3){message(g,"Law Scanner recharging.");return 0;}
 memset(g->law_echo_seen,0,sizeof(g->law_echo_seen));int count=0;
 for(int i=0;i<NPC_COUNT;i++)if(g->npc[i].alive&&g->npc[i].role==LAW&&length(sub(g->npc[i].pos,g->pos))<=20000){
  Vec3 delta=sub(g->npc[i].pos,g->pos);float distance=length(delta);Vec3 dir=norm(delta);int blocked=0;
  for(int b=0;b<BODY_COUNT;b++){Vec3 rel=sub(g->bodies[b].pos,g->pos);float along=dot(rel,dir);if(along>0&&along<distance&&length(sub(rel,mul(dir,along)))<g->bodies[b].radius)blocked=1;}
  if(!blocked){g->law_echo_seen[i]=1;g->law_echo[i]=g->npc[i].pos;count++;}
 }
 g->law_scan_valid=1;g->law_scan_time=g->time;g->cue=SFX_SCAN;
 char note[96];snprintf(note,sizeof(note),"LAW SCAN: %d patrols within 20 km. Radar echoes expire in 12s.",count);message(g,note);return 1;
}
