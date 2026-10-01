/* Manual tools share one eligibility guard, including calls outside the HUD. */
static int flight_tools_ready(const Game *g){return !g->docked&&!g->dead&&g->planet<0&&g->jump<=0&&!g->dock_stage&&g->approach<0&&!g->police_stop;}
int ecm_fitted(const Game *g){return fit_find(g,16)>=0;}
int flare_capacity(const Game *g){return fit_find(g,17)>=0?6:3;}
int deploy_flare(Game *g){
 if(!flight_tools_ready(g))return 0;
 if(g->flare_charges<=0){message(g,"Decoy bank recharging.");return 0;}
 if(g->flare_cd>0){message(g,"Decoy launcher cooling.");return 0;}
 g->flare_charges--;g->flare_cd=3;g->flare_fx=1.8f;g->flare_dir=mul(forward(g),-1);
 g->flare_pos=add(g->pos,mul(g->flare_dir,70));g->cue=SFX_SCAN;message(g,"Rear decoy deployed.");return 1;
}
int activate_ecm(Game *g){
 if(!flight_tools_ready(g))return 0;
 if(!ecm_fitted(g)){message(g,"Fit an ECM suite in DEF first.");return 0;}
 if(g->ecm_cd>0){message(g,"ECM recharging.");return 0;}
 if(g->energy<18){message(g,"ECM needs 18 shield energy.");return 0;}
 g->energy-=18;g->ecm_cd=18;g->cue=SFX_SCAN;
 int incoming=g->incoming_missile>0;g->incoming_missile=0;g->incoming_source=-1;
 message(g,incoming?"ECM pulse broke incoming lock.":"ECM pulse emitted. No incoming lock.");return 1;
}
int dump_heat_sink(Game *g){
 if(!flight_tools_ready(g))return 0;
 if(!(g->upgrades&2048)){message(g,"Fit a heat sink in UTIL first.");return 0;}
 if(g->heat_sink_cd>0){message(g,"Heat sink recharging.");return 0;}
 if(g->heat<1){message(g,"Ship already cool.");return 0;}
 g->heat=fmaxf(0,g->heat-40);g->heat_sink_cd=30;g->cue=SFX_UI;message(g,"Heat sink dumped.");return 1;
}
static void flight_tools_tick(Game *g,float dt){
 g->ecm_cd=fmaxf(0,g->ecm_cd-dt);g->flare_cd=fmaxf(0,g->flare_cd-dt);
 /* Tractor replaces the manual ECM direction; a fitted suite still protects the ship. */
 if(g->incoming_missile>0&&g->incoming_missile<=1.6f&&ecm_fitted(g)&&g->ecm_cd<=0&&g->energy>=18)activate_ecm(g);
 int capacity=flare_capacity(g);if(g->flare_charges>capacity)g->flare_charges=capacity;
 if(g->flare_charges<capacity){g->flare_reload+=dt;float delay=fit_find(g,17)>=0?10:20;
  if(g->flare_reload>=delay){g->flare_reload-=delay;g->flare_charges++;}}
 else g->flare_reload=0;
 if(g->flare_fx>0){g->flare_fx=fmaxf(0,g->flare_fx-dt);g->flare_pos=add(g->flare_pos,mul(g->flare_dir,180*dt));
  if(g->flare_fx>0&&g->incoming_missile>.25f&&g->incoming_missile<=1.6f){g->incoming_missile=0;g->incoming_source=-1;message(g,"Incoming missile diverted by decoy.");}}
}
