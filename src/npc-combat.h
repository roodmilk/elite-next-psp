/* Combat intent is transient; NPC layout and commander saves stay unchanged. */
static void npc_pirate_decision(Game *g,NPC *n,float pd,int *target,float *best,Vec3 *aim){
 int engaged=n->player_tag>0&&pd<12000,law_count=0;
 Vec3 law_away={0,0,0};
 for(int j=0;j<NPC_COUNT;j++)if(g->npc[j].alive&&g->npc[j].role==LAW){
  Vec3 away=sub(n->pos,g->npc[j].pos);float d=length(away);
  if(d<2200){law_count++;law_away=add(law_away,mul(norm(away),1.f-d/2600.f));}
 }
 float max_health=n->bounty_slot>=0?110.f+danger_rating(g,g->system)*35.f:80.f;
 float max_shield=n->bounty_slot>=0?55.f+danger_rating(g,g->system)*15.f:40.f;
 int crippled=n->health<max_health*.30f&&n->shield<max_shield*.15f;
 int retreat=engaged&&(crippled||law_count>=2);
 if(engaged){
  n->route_wait=0;
  if(retreat)n->escape_time=fmaxf(n->escape_time,8);
  else {n->escape_time=0;*target=-2;*best=pd;*aim=g->pos;}
 }else if(law_count)n->escape_time=fmaxf(n->escape_time,12);
 if(n->escape_time>0){
  *target=-1;n->route_wait=0;n->route_task=TRAFFIC_ESCAPE;
  /* Break away from the actual attacker/patrol, rather than calmly travelling
   * to a fixed lane waypoint that might even lie behind that threat. */
  Vec3 away=engaged?norm(sub(n->pos,g->pos)):norm(sub(traffic_refuge(g,n),n->pos));
  away=add(away,law_away);if(length(away)<.01f)away=n->dir;
  *aim=add(n->pos,mul(norm(away),6500));
 }
}

static Vec3 npc_combat_direction(Game *g,NPC *n,int id,Vec3 aim,float distance,int previous_task){
 Vec3 direct=norm(sub(aim,n->pos));
 if(n->target==-1)return direct;
 /* Hysteresis gives the ship space to complete a pass before turning back;
  * the old single 450m threshold made close opponents orbit indecisively. */
 if(distance<400||(previous_task==TRAFFIC_BREAK&&distance<1050)){
  n->route_task=TRAFFIC_BREAK;
  Vec3 side={direct.z,0,-direct.x};if(length(side)<.01f)side=(Vec3){1,0,0};
  return norm(add(mul(direct,-1.8f),mul(side,(id&1)?1.f:-1.f)));
 }
 n->route_task=TRAFFIC_ATTACK;
 Vec3 lead={0,0,0};
 if(n->target==-2)lead=mul(forward(g),fmaxf(0,fminf(900,g->speed*.35f)));
 else if(n->target>=0&&n->target<NPC_COUNT){NPC *other=&g->npc[n->target];lead=mul(other->dir,traffic_speed(other)*.35f);}
 Vec3 desired=norm(sub(add(aim,lead),n->pos));
 /* Small, smooth jinks on the approach. Keep the firing window clear at
  * close range; these are flying manoeuvres, never random teleportation. */
 if(distance>650&&n->player_tag>0){
  Vec3 side=norm((Vec3){direct.z,0,-direct.x});
  float phase=g->time*2.2f+id*1.618f;
  desired=norm(add(desired,add(mul(side,sinf(phase)*.22f),(Vec3){0,cosf(phase*.73f)*.12f,0})));
 }
 return desired;
}
