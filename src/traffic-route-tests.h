{
 int valid=1;
 for(int sys=0;sys<256;sys++){g.system=sys;game_spawn(&g);
  for(int i=0;i<NPC_COUNT;i++){NPC *n=&g.npc[i];if(n->freighter)continue;
   valid&=n->route_id>=0&&n->route_id<4&&n->route_leg>=0&&n->route_leg<3;
   if(n->role==LAW)valid&=n->route_id<2;
   if(n->role==PIRATES)valid&=n->route_id>=2;
  }
 }CHECK(valid,"traffic: all 256 systems assign bounded role-specific routes");
 game_init(&g);launch(&g);NPC *n=&g.npc[BOUNTY_NPC_FIRST];int cash=g.credits;
 hit(&g,BOUNTY_NPC_FIRST,100000,0);
 CHECK(n->alive&&n->health>=35&&n->escape_time>0&&!bounty_target_taken(&g,0)&&g.credits==cash,"traffic: unengaged poster target escapes NPC lethal damage without reward or claim");
 hit(&g,BOUNTY_NPC_FIRST,1,1);hit(&g,BOUNTY_NPC_FIRST,100000,0);
 CHECK(!n->alive&&bounty_target_taken(&g,0)&&g.credits>cash,"traffic: police assistance credits player-engaged wanted target");
 game_init(&g);launch(&g);n=&g.npc[0];n->role=TRADERS;n->freighter=0;traffic_assign(&g,n,0,0);
 n->route_leg=2;n->route_step=1;n->pos=g.traffic_nodes[n->route_id][2];traffic_aim(&g,n,.1f);
 CHECK(n->route_leg==1&&n->route_step==-1&&traffic_speed(n)==0,"traffic: world arrival transfers cargo and reverses route");
 traffic_aim(&g,n,30);CHECK(traffic_speed(n)>0,"traffic: trader resumes station return after service");
 n->role=LAW;g.pos=(Vec3){90000,90000,90000};traffic_assign(&g,n,36,0);
 CHECK(length(sub(n->pos,g.pos))>50000,"traffic: reinforcements dispatch from station, never beside player");
 CHECK(!law_scan(&g),"law scanner: missing UTIL module cannot scan");
 g.fit[FIT_UTIL]=LAW_SCANNER_ITEM;fit_rebuild(&g);g.pos=(Vec3){0,100000,0};
 for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;
 n=&g.npc[0];n->alive=1;n->role=LAW;n->pos=add(g.pos,(Vec3){1000,0,0});
 g.npc[1]=*n;g.npc[1].pos=add(g.pos,(Vec3){21000,0,0});
 CHECK(law_scan(&g)&&g.law_echo_seen[0]&&!g.law_echo_seen[1],"law scanner: records only patrols inside 20 km");
 Vec3 echo=g.law_echo[0];n->pos.x+=3000;
 CHECK(g.law_echo[0].x==echo.x&&!law_scan(&g),"law scanner: snapshots do not cheat-track movement and obey recharge");
 g.docked=1;CHECK(save_game(&g,"traffic-fit-test.sav"),"law scanner: installed module saves");
 CHECK(load_game(&g,"traffic-fit-test.sav")&&fit_find(&g,LAW_SCANNER_ITEM)>=0&&!g.law_scan_valid,"law scanner: module loads but transient echoes clear");
 remove("traffic-fit-test.sav");game_init(&g);fflush(f);
}
