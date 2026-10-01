/* Bounded local steering: no allocation and no new persisted NPC fields. */
static Vec3 npc_turn_toward(Vec3 current,Vec3 desired,float dt){
 current=norm(current);desired=norm(desired);
 float c=fmaxf(-1,fminf(1,dot(current,desired))),angle=acosf(c),step=fminf(angle,1.8f*fmaxf(0,dt));
 if(angle<.0001f)return desired;
 Vec3 side=sub(desired,mul(current,c));
 if(length(side)<.001f){side=(Vec3){current.z,0,-current.x};if(length(side)<.001f)side=(Vec3){1,0,0};}
 return norm(add(mul(current,cosf(step)),mul(norm(side),sinf(step))));
}
static Vec3 npc_avoid_traffic(const Game *g,int id,Vec3 desired,float speed){
 const NPC *n=&g->npc[id];Vec3 push={0,0,0};
 {
  const MegaCity *m=station_architecture_for(g);float angle=station_angle(g);
  Vec3 a=station_arch_rotate(sub(n->pos,(Vec3){0,0,STATION_Z}),-angle),b=add(a,mul(station_arch_rotate(desired,-angle),fmaxf(900,speed*2)));
  for(int k=0;k<m->n;k++){const MegaBlock *box=&m->b[k];float t;
   if(mega_box_hit(a,b,box,n->radius+140,&t)){
    Vec3 q=sub(a,box->c);float dx=fabsf(q.x)-box->e.x,dy=fabsf(q.y)-box->e.y,dz=fabsf(q.z)-box->e.z;
    Vec3 away=dx>dy&&dx>dz?(Vec3){q.x<0?-1:1,0,0}:dy>dz?(Vec3){0,q.y<0?-1:1,0}:(Vec3){0,0,q.z<0?-1:1};
    push=add(push,mul(station_arch_rotate(away,angle),3));
   }
  }
 }
 for(int j=0;j<NPC_COUNT;j++){const NPC *o=&g->npc[j];if(j==id||!o->alive||o->freighter)continue;
  Vec3 delta=sub(n->pos,o->pos);float clear=n->radius+o->radius+65,d2=dot(delta,delta);
  if(d2>(clear+900)*(clear+900))continue;
  Vec3 relative=sub(mul(n->dir,speed),mul(o->dir,o->target==-1?o->cruise:300));
  float vv=dot(relative,relative),t=vv>.01f?fmaxf(0,fminf(1.2f,-dot(delta,relative)/vv)):0;
  Vec3 closest=add(delta,mul(relative,t));float near=length(closest),distance=sqrtf(d2);
  if(distance<clear||near<clear){
   Vec3 away=distance>.01f?mul(delta,1/distance):(Vec3){id<j?-1.f:1.f,0,0};
   Vec3 side={n->dir.z,0,-n->dir.x};if(length(side)<.01f)side=(Vec3){1,0,0};
   /* Both oncoming pilots pass to their own right. */
   float urgency=distance<clear?3.f:1.8f*(1-t/1.6f);
   push=add(push,mul(norm(add(mul(away,.65f),side)),urgency));
  }
 }
 return norm(add(desired,push));
}
/* Swept backstop while staggered AI turns; preserve bounty/mission identity. */
static void mega_npc_clear(Game *g,NPC *n,Vec3 previous){
 float angle=station_angle(g);
 Vec3 a=station_arch_rotate(sub(previous,(Vec3){0,0,STATION_Z}),-angle),b=station_arch_rotate(sub(n->pos,(Vec3){0,0,STATION_Z}),-angle);
 const MegaCity *m=station_architecture_for(g);
 for(int k=0;k<m->n;k++){
  const MegaBlock *box=&m->b[k];if(!mega_box_hit(a,b,box,n->radius+30,0))continue;
  if(!mega_box_hit(a,a,box,n->radius+32,0)){n->pos=previous;n->dir=npc_turn_toward(n->dir,station_arch_rotate(norm(sub(a,box->c)),angle),.2f);return;}
  Vec3 q=sub(b,box->c);float dx=box->e.x+n->radius+40-fabsf(q.x),dy=box->e.y+n->radius+40-fabsf(q.y),dz=box->e.z+n->radius+40-fabsf(q.z);
  if(dx<dy&&dx<dz)b.x+=(q.x<0?-dx:dx);else if(dy<dz)b.y+=(q.y<0?-dy:dy);else b.z+=(q.z<0?-dz:dz);
  n->pos=add(station_arch_rotate(b,angle),(Vec3){0,0,STATION_Z});a=b;
 }
}
/* Repair pre-existing intersections without damage, kills or bounty changes.
 * Only light hulls are moved here; capital berths keep their existing solver. */
static void npc_separate_traffic(Game *g){
 for(int pass=0;pass<3;pass++)for(int i=0;i<NPC_COUNT;i++){
  NPC *a=&g->npc[i];if(!a->alive||a->freighter)continue;
  for(int j=i+1;j<NPC_COUNT;j++){NPC *b=&g->npc[j];if(!b->alive||b->freighter)continue;
   Vec3 d=sub(a->pos,b->pos);float r=a->radius+b->radius+12,dd=dot(d,d);if(dd>=r*r)continue;
   float distance=sqrtf(dd);Vec3 away=distance>.001f?mul(d,1/distance):(Vec3){1,0,0};
   Vec3 shift=mul(away,(r-distance)*.5f+.01f);a->pos=add(a->pos,shift);b->pos=sub(b->pos,shift);
  }
 }
}
