/* Regression for overlapping patrol/pirate pairs rotating at point blank. */
{
 Vec3 turn={0,0,1};for(int k=0;k<120;k++)turn=npc_turn_toward(turn,(Vec3){0,0,-1},1.f/60);
 CHECK(turn.z<-.99f&&fabsf(length(turn)-1)<.001f,"NPC steering: exact opposite bearing completes a finite turn");
 /* Each frame must move the hull, even when its decision phase is skipped. */
 game_init(&g);launch(&g);g.pos=(Vec3){0,0,-50000};g.speed=0;
 for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;
 NPC *smooth=&g.npc[0];smooth->alive=1;smooth->freighter=0;smooth->role=TRADERS;
 smooth->radius=30;smooth->health=100;smooth->shield=0;smooth->cooldown=1000;
 smooth->cruise=500;smooth->target=-1;smooth->waypoint=1;smooth->bounty_slot=-1;
 smooth->pos=(Vec3){50000,50000,50000};smooth->dir=(Vec3){0,0,1};
 int even_motion=1;
 for(int frame=0;frame<120;frame++){
  float step=frame%3==0?.03f:1.f/60;Vec3 previous=smooth->pos;
  game_tick(&g,step,0,0,0,0);
  even_motion&=fabsf(length(sub(smooth->pos,previous))-500*step)<.1f;
 }
 CHECK(even_motion,"NPC tracking: motion advances each frame with actual elapsed time, never alternate-frame jumps");
 int clear=1,moving=1,finite=1;
 for(int scenario=0;scenario<3;scenario++){
  game_init(&g);launch(&g);g.pos=(Vec3){0,0,-50000};g.speed=0;
  for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;
  for(int i=0;i<2;i++){NPC *n=&g.npc[i];n->alive=1;n->freighter=0;n->role=i?LAW:PIRATES;n->radius=80;n->health=10000;n->shield=0;n->cooldown=1000;n->cruise=650;n->waypoint=0;n->bounty_slot=-1;n->pos=(Vec3){20000,500,20000+(scenario==0?0:i*(scenario==1?140:1200))};n->dir=(Vec3){0,0,i?-1:1};}
  Vec3 initial=g.npc[0].pos;float travelled=0;
  float step=scenario==2?.05f:1.f/60;
  for(int frame=0;frame<600;frame++){Vec3 previous=g.npc[0].pos;game_tick(&g,step,0,0,0,0);travelled+=length(sub(g.npc[0].pos,previous));
   clear&=length(sub(g.npc[0].pos,g.npc[1].pos))>=159.9f;
   for(int i=0;i<2;i++)finite&=isfinite(g.npc[i].pos.x)&&isfinite(g.npc[i].dir.x)&&fabsf(length(g.npc[i].dir)-1)<.01f;
  }
  moving&=travelled>1500&&length(sub(initial,g.npc[0].pos))>100;
 }
 CHECK(clear,"NPC traffic: coincident, overlapping and head-on opponents retain hull separation");
 CHECK(moving&&finite,"NPC combat: close opponents keep flying instead of spinning in place at 60/20 Hz");
 game_init(&g);for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;
 for(int i=0;i<3;i++){NPC *n=&g.npc[i];n->alive=1;n->freighter=i==2;n->radius=50;n->health=100;n->pos=(Vec3){0,0,0};}
 Vec3 capital=g.npc[2].pos;int credits_before=g.credits,legal_before=g.legal,kills_before=g.npc_kills;
 npc_separate_traffic(&g);
 CHECK(length(sub(g.npc[0].pos,g.npc[1].pos))>=100&&length(sub(capital,g.npc[2].pos))==0&&g.npc[0].health==100&&g.credits==credits_before&&g.legal==legal_before&&g.npc_kills==kills_before,"NPC separation: recovery cannot award bounties, create crimes or move capital berths");
 game_init(&g);
}
