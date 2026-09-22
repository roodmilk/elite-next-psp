/* Local freight traffic: bounded schedules, physical routes and one trade per visit.
 * Runtime only. No claim is made to simulate unloaded systems. */
Vec3 freight_extent(const NPC *n){
 static const Vec3 hull[3]={{144,90,560},{116,100,480},{166,82,640}};
 return hull[n->freight_style%3];
}
static Vec3 freight_right(const NPC *n){return norm((Vec3){n->dir.z,0,-n->dir.x});}
static Vec3 freight_up(const NPC *n){Vec3 r=freight_right(n);return (Vec3){-n->dir.y*r.z,n->dir.z*r.x-n->dir.x*r.z,n->dir.y*r.x};}
Vec3 freight_world(const NPC *n,Vec3 p){return add(n->pos,add(mul(freight_right(n),p.x),add(mul(freight_up(n),p.y),mul(n->dir,p.z))));}
static Vec3 freight_local(const NPC *n,Vec3 p){p=sub(p,n->pos);return (Vec3){dot(p,freight_right(n)),dot(p,freight_up(n)),dot(p,n->dir)};}
float freight_intersection(const NPC *n,Vec3 start,Vec3 end,float padding){
 Vec3 a=freight_local(n,start),b=freight_local(n,end),e=freight_extent(n);
 float p[3]={a.x,a.y,a.z},d[3]={b.x-a.x,b.y-a.y,b.z-a.z},extent[3]={e.x+padding,e.y+padding,e.z+padding},lo=0,hi=1;
 for(int k=0;k<3;k++){if(fabsf(d[k])<.0001f){if(fabsf(p[k])>extent[k])return -1;continue;}
  float u=(-extent[k]-p[k])/d[k],v=(extent[k]-p[k])/d[k];if(u>v){float t=u;u=v;v=t;}lo=fmaxf(lo,u);hi=fminf(hi,v);if(lo>hi)return -1;
 }return lo;
}
const char *freight_status(const NPC *n){
 static const char *state[]={"OFF SYSTEM","WARP ARRIVAL","DELIVERING","CARGO TRANSFER","DEPARTING","WARP CHARGING"};
 return state[n->freight_state>=0&&n->freight_state<=FREIGHT_CHARGING?n->freight_state:0];
}
static int freight_capacity(const Game *g){
 int activity=traffic_budget(g);
 if(activity<5)return 0;
 if(g->system==7)return 1;
 return activity>=11&&prosperity(g,g->system)>=4?3:activity>=8?2:1;
}
static float freight_interval(const Game *g,unsigned seed){return 260-prosperity(g,g->system)*22+(seed%101);}
static int freight_export(const Game *g,int sys,unsigned seed){
 const int farm[]={0,1,4},industry[]={7,8,12};
 return g->systems[sys].economy>=4?farm[seed%3]:industry[seed%3];
}
static float route_clearance(Vec3 a,Vec3 b,Vec3 p){
 Vec3 d=sub(b,a);float l=dot(d,d),t=l>0?fmaxf(0,fminf(1,dot(sub(p,a),d)/l)):0;
 return length(sub(p,add(a,mul(d,t))));
}
static int freight_route(Game *g,NPC *n,int slot){
 unsigned seed=sector_hash(g->system*7919u+slot*97u+n->freight_trip*1009u);
 n->freight_hub=(slot/12+n->freight_trip+g->system)%HUB_COUNT;
 Vec3 hub=hub_position(g,n->freight_hub);
 /* An outer transfer berth keeps capital ships away from fighter-sized doors.
  * Reject entire routes through planets or other hubs, not just endpoints. */
 for(int attempt=0;attempt<24;attempt++){
  float a=(seed%6283)*.001f+attempt*2.399963f;
  Vec3 out={cosf(a),0,sinf(a)},berth=add(hub,mul(out,1800+n->freight_style*240)),gate=add(berth,mul(out,6200+(seed%2400)));
  int safe=length(sub(gate,g->pos))>2500;
  for(int b=0;b<BODY_COUNT;b++)if(route_clearance(berth,gate,g->bodies[b].pos)<g->bodies[b].radius+n->radius+500)safe=0;
  for(int h=0;h<HUB_COUNT;h++)if(route_clearance(berth,gate,hub_position(g,h))<n->radius+600)safe=0;
  for(int j=8;j<36;j+=12)if(&g->npc[j]!=n&&g->npc[j].alive&&route_clearance(berth,gate,g->npc[j].pos)<n->radius+g->npc[j].radius+500)safe=0;
  if(safe){n->freight_berth=berth;n->freight_gate=gate;n->dir=mul(out,-1);return 1;}
 }return 0;
}
static int freight_begin(Game *g,NPC *n,int slot,int initial){
 n->freight_trip++;
 if(!freight_route(g,n,slot))return 0;
 unsigned seed=sector_hash(g->system*233u+slot+n->freight_trip*19u);
 int candidates[256],count=0;
 for(int i=0;i<256;i++)if(i!=g->system&&distance_ly(g,g->system,i)<=10)candidates[count++]=i;
 if(!count)return 0;
 n->freight_peer=candidates[seed%count];n->freight_good=freight_export(g,n->freight_peer,seed);
 n->freight_qty=4+(seed%9);n->health=900;n->shield=100;n->flash=0;n->target=-1;n->alive=1;
 n->pos=n->freight_gate;n->freight_state=FREIGHT_ARRIVING;n->freight_timer=2.5f;
 if(initial){n->freight_state=FREIGHT_INBOUND;n->pos=add(n->freight_gate,mul(sub(n->freight_berth,n->freight_gate),.15f+(seed%55)*.01f));}
 return 1;
}
static void freight_trade(Game *g,NPC *n){
 int good=n->freight_good;if(g->stock[good]<99){g->stock[good]+=n->freight_qty;if(g->stock[good]>99)g->stock[good]=99;}
 good=freight_export(g,g->system,n->freight_trip+n->freight_style);
 int qty=4+(n->freight_trip%9);if(qty>g->stock[good])qty=g->stock[good];g->stock[good]-=qty;
 n->freight_good=good;n->freight_qty=qty;
}
static void freight_update(Game *g,float dt){
 g->freight_next-=dt;g->freight_gap=fmaxf(0,g->freight_gap-dt);
 int active=0;
 for(int i=8;i<36;i+=12){NPC *n=&g->npc[i];if(!n->freighter)continue;
  if(!n->alive){n->freight_timer=fmaxf(0,n->freight_timer-dt);continue;}active++;
  n->flash=fmaxf(0,n->flash-dt);n->cooldown=fmaxf(0,n->cooldown-dt);if(n->flash<=0)n->shield=fminf(100,n->shield+dt*.8f);
  /* Damaged freighters break schedule and return fire with heavy turrets. */
  int defending=n->health<880||n->target==-2;
  if(defending&&g->jump<=0&&!g->dead&&!g->docked){
   float pd=length(sub(g->pos,n->pos));n->target=-2;
   Vec3 desired=norm(sub(g->pos,n->pos));n->dir=norm(add(mul(n->dir,1-dt*1.1f),mul(desired,dt*1.1f)));
   if(pd>420)n->pos=add(n->pos,mul(n->dir,dt*fminf(n->cruise+40.f,140.f)));
   if(pd<3200&&dot(n->dir,desired)>.72f&&n->cooldown<=0){n->cooldown=1.1f;n->flash=.12f;g->energy-=7;g->attacked=3;g->cue=SFX_HIT;}
   continue;
  }
  n->target=-1;
  if(n->freight_state==FREIGHT_ARRIVING){n->freight_timer-=dt;if(n->freight_timer<=0)n->freight_state=FREIGHT_INBOUND;continue;}
  if(n->freight_state==FREIGHT_SERVICE){
   n->freight_timer-=dt;
   /* Turn slowly while loading: no instantaneous 180-degree hull flip. */
   Vec3 out=norm(sub(n->freight_gate,n->freight_berth));float yaw=atan2f(n->dir.x,n->dir.z),want=atan2f(out.x,out.z),delta=want-yaw;
   while(delta>3.141593f)delta-=6.283185f;
   while(delta< -3.141593f)delta+=6.283185f;
   yaw+=fmaxf(-dt*.16f,fminf(dt*.16f,delta));n->dir=(Vec3){sinf(yaw),0,cosf(yaw)};
   if(n->freight_timer<=0)n->freight_state=FREIGHT_OUTBOUND;
   continue;
  }
  if(n->freight_state==FREIGHT_CHARGING){
   if(g->freight_gap>0)continue;
   n->freight_timer-=dt;
   if(n->freight_timer<=0){n->alive=0;n->freight_state=FREIGHT_ABSENT;n->freight_timer=150+(i*7+g->system)%120;g->freight_gap=45;}
   continue;
  }
  Vec3 aim=n->freight_state==FREIGHT_INBOUND?n->freight_berth:n->freight_gate,delta=sub(aim,n->pos);float distance=length(delta);
  float step=fminf(distance,n->cruise*dt);n->dir=norm(delta);n->pos=add(n->pos,mul(n->dir,step));
  if(distance<=step+.01f){
   if(n->freight_state==FREIGHT_INBOUND){freight_trade(g,n);n->freight_state=FREIGHT_SERVICE;n->freight_timer=32+(g->system+i+n->freight_trip)%25;}
   else {n->freight_state=FREIGHT_CHARGING;n->freight_timer=3;/* Preserve outward heading at the gate. */n->dir=norm(sub(n->freight_gate,n->freight_berth));}
  }
 }
 if(g->freight_next<=0&&g->freight_gap<=0&&active<freight_capacity(g)){
  for(int i=8;i<36;i+=12){NPC *n=&g->npc[i];if(!n->freighter||n->alive||n->freight_timer>0)continue;
   if(freight_begin(g,n,i,0)){g->freight_next=freight_interval(g,sector_hash(g->system+n->freight_trip*73u));g->freight_gap=45;break;}
  }
  if(g->freight_next<=0)g->freight_next=20;
 }
}
