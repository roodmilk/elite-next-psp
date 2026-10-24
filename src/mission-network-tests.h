{
 mission_legacy_review=1;
 int offered=0,accepted=0,completed=0,paid_once=0,reachable=0,receipts=0;uint32_t families=0;
 for(int system=0;system<256;system++)for(int offer=0;offer<5;offer++){
  game_init(&g);g.system=system;g.docked=1;g.credits=100000;g.story=STORY_FREE;
  int dest=mission_destination(&g,offer),type=mission_type_for_offer(&g,offer),base=mission_base_type(type);
  families|=1u<<type;
  offered+=mission_offer_valid(&g,offer,0)!=0;
  reachable+=dest>=0&&distance_ly(&g,system,dest)<=player_ships[g.ship].range*.1f+.001f;
  if(!accept_mission(&g,offer))continue;
  accepted++;
  int reward=g.jobs[0].reward,cash=g.credits;
  g.jobs[0].time=.001f;mission_timers(&g,36000);
  if(g.job_n!=1)continue;
  g.system=dest;g.docked=0;game_spawn(&g);
  if(base==MISSION_BOUNTY){int target=g.jobs[0].target;if(target>=0)hit(&g,target,2000,1);}
  else if(base==MISSION_EXPLORATION){int item=g.jobs[0].item;Body *body=&g.bodies[item];g.pos=add(body->pos,(Vec3){0,0,-body->radius-500});g.yaw=g.pitch=0;approach_planet(&g,item);}
  else if(base==MISSION_RESCUE){int target=g.jobs[0].target;if(target>=0){g.pos=g.npc[target].pos;mission_interact(&g,BODY_COUNT+1+target);docking_complete(&g);}}
  else docking_complete(&g);
  completed+=g.job_n==0&&g.mission_result==1&&g.credits>=cash+reward;
  int receipt_count=g.mission_receipt_n;
  MissionReceipt *r=&g.mission_receipts[0];receipts+=receipt_count==1&&r->kind==RECEIPT_BOARD&&r->type==type&&r->amount==reward&&r->system==system&&!strcmp(r->title,mission_name(type));
  cash=g.credits;docking_complete(&g);paid_once+=g.job_n==0&&g.credits==cash&&g.mission_receipt_n==receipt_count;
 }
 mission_legacy_review=0;
 CHECK(offered==1280&&reachable==1280,"mission network: all five offers in all 256 systems have reachable destinations");
 CHECK(families==((1u<<MISSION_RACE)-1u),"mission network: all 26 confirmed families appear across the galaxy");
 CHECK(accepted==1280,"mission network: all 1280 offers can be accepted");
 CHECK(completed==1280,"mission network: all 1280 contracts complete through real objective handlers and pay rewards");
 CHECK(receipts==1280,"mission receipts: all 1280 contracts record exactly one correct reward, title and issuer");
 CHECK(paid_once==1280,"mission network: completed contracts cannot pay twice");
}
