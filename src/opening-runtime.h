#include "opening-dialogue.h"
/* Transient dream state; never changes the commander save format. */
static float dream_time,dream_wake;
static int dream_spawned,dream_ending;
#include "opening-dream.h"
/* New-game Homeward tutorial. All milestones observe real completed actions. */
typedef struct {const char *speaker,*line,*task;int service;} OpeningBeat;
static const OpeningBeat opening_beats[OPENING_STAGE_COUNT+1]={
 [OP_CREATE]={"GAME","Create your commander, then begin your first day.","Name, portrait, then START NEW GAME.",-1},
 [OP_DREAM]={"COMPUTER","Incoming fighters. Hold your nerve. Steer clear and return fire.","Turn LEFT/RIGHT, pitch UP/DOWN, then X fires.",-1},
 [OP_HOME]={"DAD","Come on. Big day. Dealer opens at nine. You've been talking about this for years.","X talks to Dad.",-1},
 [OP_CITY]={"DAD","There it is. Come on, let's find you a ship.","X enters the ship dealer.",-1},
 [OP_DEALER]={"DAD","It's your choice. Something you can handle, with room for a few jobs.","LEFT/RIGHT compares starters. X chooses.",-1},
 [OP_SAVE_INFO]={"GAME","Save and Load are in Commander / Save status. Opening checkpoints preserve this journey. Ordinary saves still require a station.","X continues to your first save.",-1},
 [OP_FIRST_SAVE]={"GAME","Save your new commander before flying. Load this slot from the title screen.","Commander Profile: X saves your checkpoint.",5},
 [OP_RADIO]={"DAD","First things first. Can't take your first flight without some music.","X: Elite Exploration. O: return.",16},
 [OP_LAUNCH]={"DAD","Well? It's yours. Let's take her up.","Select Launch.",0},
 [OP_SOL_MAP]={"COMPUTER","Welcome aboard. Let's establish your location. Open System Details.","See Sol and its eight planets. O returns.",11},
 [OP_YAW]={"DAD","Small corrections. Turn left and right. Watch how the stars move.","D-pad LEFT/RIGHT or nub: practise turning, then settle the ship.",-1},
 [OP_PITCH]={"DAD","Now raise or lower the nose. There's Earth. Lovely, isn't it?","D-pad UP/DOWN or nub: practise raising and lowering the nose.",-1},
 [OP_THROTTLE]={"DAD","Give her a little speed. No hurry.","Hold R until speed reaches 150.",-1},
 [OP_BRAKE]={"DAD","And slowing down matters just as much. Bring us below forty.","Hold L to brake below 40.",-1},
 [OP_EARTH_NET]={"DAD","Your friends will want to know how it's going. Take a look at GalacticNet while we're clear of traffic.","Browse Earth news, friends and Jobs. O returns.",14},
 [OP_EARTH_ROUTE]={"DAD","Before the station, let's stretch our legs. I know a nice landing spot in the countryside.","System Details: X selects Earth countryside.",11},
 [OP_APPROACH]={"COMPUTER","Earth landing site acquired. Confirm atmospheric entry.","X confirms guided atmospheric entry.",-1},
 [OP_TOUCHDOWN]={"DAD","Take it steady. Follow the landing guidance.","Finish approach and touchdown.",-1},
 [OP_WALK]={"DAD","Come on, stretch your legs. You can't spend your whole life in that cockpit.","Walk away from ship. D-pad walks; nub looks.",-1},
 [OP_PEP]={"DAD","Thought your first trip deserved a decent view.","X talks with Dad in the field.",-1},
 [OP_FIND]={"DAD","Take your time. I will be here by the ship.","Explore the meadow.",-1},
 [OP_OBJECT]={"PLAYER","That doesn't look like survey equipment. It's warm.","X takes the unknown object.",-1},
 [OP_OBJECT_CHOICE]={"PLAYER","Dad's looking toward the mountains. Do I tell him?","LEFT/RIGHT chooses. X confirms show or hide.",-1},
 [OP_RETURN]={"DAD","Ready? We've still got a bit of flying to do.","Return beside your ship. Triangle boards.",-1},
 [OP_ORBIT]={"COMPUTER","Cabin secure. Launch when ready.","Release controls, then R launches to orbit.",-1},
 [OP_GIFT]={"DAD","I've got you the best weapon in the galaxy. The MiniBolt MiningGun Lite. Against a rock? Terrifying.","X accepts Dad's starter mining kit.",-1},
 [OP_LOADOUT]={"DAD","Open your loadout. Let's fit it to the first weapon mount.","Ship Loadout: X fits MiniBolt. O returns.",21},
 [OP_LOG]={"DAD","Try collecting some minerals. I've written a small practice task for you.","Mission Log: X tracks Dad's mining task.",13},
 [OP_TRACK]={"COMPUTER","Tracked Mission shows your selected objective. Jobs are recorded here, not in your personal messages.","Read Tracked Mission. O returns to fly.",17},
 [OP_MINE]={"DAD","There's a good rock ahead. Point the MiniBolt at it and fire.","Release X, then hold X to mine the marked rock.",-1},
 [OP_COLLECT]={"DAD","Look at that. Rock never stood a chance. Let's get the ore aboard.","Triangle activates the tractor within 500 m.",-1},
 [OP_ROCK]={"DAD","There's your first dent. Let's visit the engineers before you collect any more.","X acknowledges the minor hull damage.",-1},
 [OP_STATION_ROUTE]={"DAD","Fuel and repairs first. Select the orbital station.","System Details: X selects Earth Orbital.",11},
 [OP_MUM_RING]={"COMPUTER","Incoming call: Mum.","Triangle responds to the incoming ship call.",-1},
 [OP_MUM_TALK]={"COMPUTER","Family channel connected. Select Talk.","Comms: X selects TALK.",-1},
 [OP_MUM_CALL]={"MUM","You two haven't forgotten the pizza, have you?","X advances the family conversation.",-1},
 [OP_STATION_LOCK]={"DAD","Let's ask the station for clearance. Keep it selected on the scanner.","Approach Earth Orbital. Hold Triangle for comms.",-1},
 [OP_DOCK]={"COMPUTER","Station clearance available.","Comms: REQUEST AUTO-DOCK. X confirms.",-1},
 [OP_REPAIR]={"DAD","Hull first. Shields coming back doesn't mend a dent.","Engineers: X repairs the hull.",24},
 [OP_REFUEL]={"DAD","And fill the tank. Running out makes for a bad afternoon.","Engineers: Triangle refuels.",24},
 [OP_YARD_INTRO]={"PLAYER","Can we go look at the ships?","X hears Dad's answer.",-1},
 [OP_YARD]={"DAD","All right. Just looking. Keep taking jobs, save your credits. This is what you could earn.","Browse ships. X tries to buy; O continues.",3},
 [OP_OUTFIT_INTRO]={"SALESMAN","New ship? Come and see what we can fit her with.","X continues to Outfitting.",-1},
 [OP_OUTFIT]={"SALESMAN","Our biggest ship-mounted cannon. Military-grade. You'll want to protect those pizzas.","Browse equipment. X asks Dad; O continues.",4},
 [OP_FOOD_INTRO]={"DAD","Actually we're just here to get a few pizzas. Come on, let's finish the shopping.","X continues to Cargo.",-1},
 [OP_FOOD]={"DAD","Food. One order. Check the price and the hold before you buy.","Cargo: RIGHT buys Food for dinner.",1},
 [OP_HOME_ROUTE]={"DAD","That's everything. Let's head home before dinner gets cold.","System Details: X selects home on Earth.",11},
 [OP_HOME_LAUNCH]={"COMPUTER","Homeward route confirmed.","Launch from Earth Orbital.",0},
 [OP_HOME_FLIGHT]={"DAD","Good first day? Apart from the dent.","Fly toward Earth. Home is selected.",-1},
 [OP_THARGOID]={"THARGOID","THE ... MUST BE RETURNED.","X continues the transmission.",-1},
 [OP_BURST]={"DAD","Stay close to me!","An overwhelming energy burst strikes.",-1},
 [OP_WAKE]={"PLAYER","Dad? Dad, where are you?","X checks the damaged computer.",-1},
 [OP_DAD_INBOX]={"COMPUTER","Navigation fault. Earth channel unavailable. Cached conversations retained.","GalacticNet / Inbox: read Dad's thread. O back.",14},
 [OP_LONELY]={"PLAYER","Nothing. No station. No answer.","Fly while the computer recovers.",-1},
 [OP_MAP_INTRO]={"COMPUTER","Navigation catalogue recovered. Open Galaxy.Maps to search for Sol.","X reveals the new navigation option.",-1},
 [OP_GALAXY]={"PLAYER","How many stars are there? Where's Sol?","Galaxy.Maps: look around. O returns.",2},
 [OP_LAVE_LOCK]={"COMPUTER","Navigation reference acquired. System identified: Lave.","X accepts the local signal.",-1},
 [OP_CIVILISATION]={"COMPUTER","Local traffic detected. Radio signal acquired.","Fly toward the station. Civilised space!",-1},
 [OP_ADVERT]={"PRESENTER","New system. New you. Still flying something your grandad would apologise for? It's time to pimp your ship!","X continues the commercial transmission.",-1},
 [OP_APP]={"PRESENTER","We've delivered a FREE shortcut straight to your menu. You're welcome, space legend!","X watches pimp-my-ship.net install.",-1},
 [OP_APP_VIEW]={"COMPUTER","Menu updated. Application installed.","Open glowing pimp-my-ship.net. O returns.",23},
 [OP_LAVE_DOCK]={"COMPUTER","Lave station is accepting arrivals.","Triangle -> REQUEST AUTO-DOCK.",-1},
 [OP_NEWS_INTRO]={"DOCKWORKER","Oh my God, look at the news.","X highlights LAVE LOCAL TV.",-1},
 [OP_NEWS]={"NEWSREADER","Reports are emerging of a Thargoid attack in Sol. Communications with Earth remain disrupted.","LAVE LOCAL TV: X next bulletin.",26},
 [OP_AFTER_NEWS]={"PLAYER","That's home. Dad's still out there somewhere.","X begins your life in this galaxy.",-1}
};
static int opening_prepared=-1,opening_notice=0,opening_ship_choice=0,opening_create_row=0;
static float opening_pulse=0,opening_dialogue_clock=0,opening_boundary_voice=0;
static int opening_coaching(int s){return s==OP_YAW||s==OP_PITCH||s==OP_THROTTLE||s==OP_BRAKE||s==OP_APPROACH||s==OP_TOUCHDOWN||s==OP_WALK||s==OP_FIND||s==OP_RETURN||s==OP_ORBIT||s==OP_MINE||s==OP_COLLECT||s==OP_STATION_LOCK||s==OP_DOCK||s==OP_LAVE_DOCK||s==OP_CIVILISATION;}
static int opening_seen_node=-1,opening_input_gate=1,opening_dealer_confirm=0,opening_dealer_answer=0,opening_dealer_mesh=0;
static const OpeningDialogueNode *opening_current_dialogue(void){
 if(!opening_active(&game)||(game.opening.flags&OF_DIALOG_DONE)||game.opening.stage==OP_CREATE)return 0;
 if(game.opening.stage==OP_MUM_RING||game.opening.stage==OP_MUM_TALK)return 0;
 if(game.opening.stage==OP_NEWS&&page!=LOCALTV)return 0;
 const OpeningDialogueNode *n=opening_dialogue_node(game.opening.counter/10);
 return n&&n->stage==game.opening.stage?n:opening_dialogue_entry(game.opening.stage);
}
static const char *opening_dialogue_text(const OpeningDialogueNode *n){
 if(n->id==OP_THARGOID*100+12)return (game.opening.flags&OF_SHOWN)?"That thing you found. It's lighting up. Put it down, now!":"There's a light coming from your bag. What have you brought aboard?";
 return n->text;
}
static int opening_dialogue_revealed(void){const OpeningDialogueNode *n=opening_current_dialogue();return !n||opening_dialogue_clock>=(float)strlen(opening_dialogue_text(n))*.028f;}
static void opening_dialogue_sync(void){
 const OpeningDialogueNode *n=opening_current_dialogue();if(!n)return;
 if(game.opening.counter/10!=n->id)game.opening.counter=n->id*10;
 if(opening_seen_node!=n->id){opening_seen_node=n->id;opening_dialogue_clock=0;opening_input_gate=1;}
}
static void opening_dialogue_enter(int id){
 const OpeningDialogueNode *n=opening_dialogue_node(id);if(!n)return;
 game.opening.counter=id*10;opening_seen_node=-1;opening_dialogue_sync();
 if(n->effect==OD_EFFECT_SHOW_OBJECT){game.opening.flags|=OF_SHOWN;game.opening.choice=1;}
 if(n->effect==OD_EFFECT_HIDE_OBJECT){game.opening.flags&=~OF_SHOWN;game.opening.choice=2;}
}
static const char *opening_notice_speaker="DAD",*opening_notice_copy="";
static const OpeningBeat *opening_beat(void){int s=game.opening.stage;return s>0&&s<=OPENING_STAGE_COUNT?&opening_beats[s]:0;}
static int opening_required_service(void){const OpeningBeat *b=opening_beat();return b?b->service:-1;}
static void opening_notice_set(const char *who,const char *copy){opening_notice_speaker=who;opening_notice_copy=copy;opening_notice=1;opening_input_gate=1;}
static void opening_checkpoint(void){if(!smoke&&game.opening.stage>OP_FIRST_SAVE&&opening_save_allowed(&game))save_game(&game,commander_slots[commander_active_slot]);}
static void opening_frontend_reset(void){
 tutorial_reset_frontend();opening_prepared=-1;opening_notice=0;opening_seen_node=-1;opening_dialogue_clock=0;opening_input_gate=1;opening_dealer_confirm=0;station_tour_stage=STATION_TOUR_DONE;
 ps_open=ps_release=0;planet_landing_menu=0;planet_seat.active=0;planet_orbit_release=planet_entry_body=0;planet_orbit_veil=0;departure_release=0;thargoid_reset();
 surface_target_reset();tools_reset_gesture();tools_wait_release=1;planet_controls_ready=1;
}
static void opening_new_game(void){
 game_init(&game);story_complete(&game);game.campaign_stage=6;game.saga_chapter=SAGA_COUNT;
 game.commander_portrait=portrait_expand_dna(portrait_parts_dna(portrait_seeded(177,EXPLORERS,PORTRAIT_HUMAN)));
 game.opening.stage=OP_CREATE;game.cargo[0]=0;game.credits=6000;game.voice_time=0;
 opening_frontend_reset();opening_world_apply(&game);game_spawn(&game);opening_create_row=0;
 change_page(STATUS);profile_mode=profile_portrait_edit=profile_confirm=0;
}
static int opening_skip_prompt=0,opening_skip_choice=0;
static void opening_skip_to_lave(void){
 char name[25];uint32_t portrait=game.commander_portrait;memcpy(name,game.commander_name,25);
 game_init(&game);memcpy(game.commander_name,name,25);game.commander_portrait=portrait;
 story_complete(&game);game.campaign_stage=6;game.saga_chapter=SAGA_COUNT;game.opening.stage=OP_FREE;
 game.system=7;game.docked=1;game.station_variant=0;game.planet=game.approach=-1;system_bodies(&game);game_spawn(&game);
 opening_skip_prompt=opening_skip_choice=0;opening_frontend_reset();change_page(HOME);
 message(&game,"Tutorial skipped. Welcome to Lave Hub.");opening_checkpoint();
}
static void opening_restart_intro(void){
 quit_to_main_menu();game_init(&game);opening_frontend_reset();deck_reset();
 load_arrival_time=-1;load_arrival_release=0;profile_mode=profile_portrait_edit=profile_confirm=0;
 intro_boot_pending=1;intro_enter();intro_profile_ready=0;
 radio_dream_gain=-1;audio_replay_intro=1;
}
static void opening_focus(int service){change_page(HOME);row=service;deck_last=row;deck_focus[deck_group(row)]=row;}
static void opening_advance(void){
 if(!opening_active(&game))return;
 game.opening.stage++;game.opening.flags&=~OF_DIALOG_DONE;game.opening.counter=0;game.opening.distance=game.opening.elapsed=0;
 opening_prepared=-1;opening_notice=0;opening_seen_node=-1;opening_dialogue_clock=0;opening_input_gate=1;game.voice_time=game.message_time=0;game.boost=0;opening_checkpoint();
}
static void opening_restore(void){if(!opening_exists(&game))return;opening_frontend_reset();opening_world_apply(&game);}
static Vec3 opening_object_pos(void){Vec3 p=game.ship_pos;p.x+=95;p.z+=55;p.y=terrain_height(&game,p.x,p.z)+4;return p;}
static void opening_practice_rock(void){
 Debris *d=&game.debris[DEBRIS_COUNT-1];memset(d,0,sizeof(*d));d->alive=1;
 d->rock=game.opening.stage==OP_MINE;d->good=12;d->qty=1;d->health=d->rock?54:1;d->radius=d->rock?45:10;d->life=3600;
 d->pos=add(game.pos,mul(forward(&game),230));selected_target=DEBRIS_ID_MIN+DEBRIS_COUNT-1;scan_cat=3;game.speed=0;
}
static void opening_displace(void){
 game.opening.flags=(game.opening.flags|OF_DISPLACED)&~OF_DIALOG_DONE;game.opening.stage=OP_WAKE;game.opening.counter=0;game.opening.elapsed=game.opening.distance=0;
 game.system=game.destination=7;game.planet=-1;game.surface=0;game.approach=-1;game.planet_sequence=0;
 game.docked=0;game.dock_stage=game.dock_phase=0;game.hull=82;game.damaged=1;game.energy=55;game.dead=0;
 game.speed=game.yaw=game.pitch=game.roll=game.jump=0;game.boost=0;
 game.visited[0]&=~(1u<<7);game.landed_planets[7]=0;memset(game.surface_progress[7],0,sizeof(game.surface_progress[7]));
 opening_world_apply(&game);game_spawn(&game);game.pos=(Vec3){0,400,-65000};game.speed=0;game.voice_time=game.message_time=0;
 for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;for(int i=0;i<DEBRIS_COUNT;i++)game.debris[i].alive=0;
 radio_off=1;radio_dirty=1;opening_frontend_reset();change_page(FLIGHT);opening_checkpoint();
}
static void opening_prepare(void){
 if(!opening_active(&game)||opening_prepared==game.opening.stage)return;
 opening_prepared=game.opening.stage;game.voice_time=game.message_time=0;station_tour_stage=STATION_TOUR_DONE;
 const OpeningBeat *b=opening_beat();int s=game.opening.stage;
 if(s==OP_CREATE)return;
 if(s==OP_DREAM){
  dream_time=dream_wake=0;dream_spawned=dream_ending=0;radio_dream_gain=0;
  game.opening.flags|=OF_DIALOG_DONE;
  launch(&game);game.fit[FIT_WPN]=1;fit_rebuild(&game);game.speed=0;game.pos=(Vec3){0,0,-12000};
  game.dock_stage=0;departure_release=0;game.yaw=game.pitch=game.roll=0;
  dream_reset();r_tap=l_tap=10;hard_brake=0;fire_blocked=0;
  change_page(FLIGHT);
 }
 if(s==OP_HOME){OpeningState progress=game.opening;char name[25];uint32_t portrait=game.commander_portrait;memcpy(name,game.commander_name,25);game_init(&game);game.opening=progress;memcpy(game.commander_name,name,25);game.commander_portrait=portrait;selected_target=0;autoaim=0;game.opening.stage=OP_HOME;game.cargo[0]=0;game.credits=6000;story_complete(&game);game.campaign_stage=6;game.saga_chapter=SAGA_COUNT;fit_clear_all(&game);opening_world_apply(&game);game_spawn(&game);change_page(HOME);}
 if(s==OP_CITY||s==OP_DEALER||s==OP_SAVE_INFO||s==OP_GIFT||s==OP_YARD_INTRO||s==OP_OUTFIT_INTRO||s==OP_FOOD_INTRO||s==OP_MAP_INTRO||s==OP_APP||s==OP_NEWS_INTRO||s==OP_AFTER_NEWS)change_page(HOME);
 if(b->service>=0)opening_focus(b->service);
 if(s==OP_FIRST_SAVE){profile_confirm=0;profile_slot=commander_active_slot;}
 if(s==OP_RADIO){radio_off=1;radio_dirty=1;}
 if(s==OP_YAW||s==OP_PITCH||s==OP_THROTTLE||s==OP_BRAKE||s==OP_WALK||s==OP_FIND||s==OP_RETURN||s==OP_ORBIT||s==OP_MINE||s==OP_COLLECT||s==OP_STATION_LOCK||s==OP_DOCK||s==OP_HOME_FLIGHT||s==OP_LONELY||s==OP_CIVILISATION||s==OP_LAVE_DOCK)change_page(FLIGHT);
 if(s==OP_APPROACH){change_page(FLIGHT);game.pos=add(game.bodies[1].pos,(Vec3){0,0,-game.bodies[1].radius-900});game.yaw=game.pitch=0;game.speed=0;game.approach=1;}
 if(s==OP_WALK&&game.surface==1){disembark_planet(&game);planet_controls_ready=1;}
 /* Older checkpoints may now lie in water, a tree or an outcrop as well as
  * outside the clearing. Use the actual walking rules before resuming. */
 if(game.planet==1&&game.surface==2&&s>=OP_WALK&&s<=OP_RETURN){Vec3 pad=surface_site(&game,1);if(!eva_position_allowed(&game,game.pos.x,game.pos.z)){game.pos=add(pad,(Vec3){75,0,-80});game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;game.jetpack=0;}}

 if(s==OP_MINE||s==OP_COLLECT)opening_practice_rock();
 if(s==OP_ROCK&&!(game.opening.flags&OF_ROCK)){game.hull=88;game.damaged=1;game.damage_fx=1;game.fuel=fmaxf(0,game.fuel-8);game.opening.flags|=OF_ROCK;game.cue=SFX_HIT;}
 if(s==OP_MUM_RING){change_page(FLIGHT);game.speed=0;game.cue=SFX_COMM;}
 if(s==OP_MUM_TALK){change_page(COMMS_PANEL);row=0;}
 if(s==OP_MUM_CALL)game.opening.flags|=OF_MUM;
 if(s==OP_STATION_LOCK||s==OP_LAVE_DOCK){selected_target=0;autoaim=1;game.pos=(Vec3){0,0,900};game.yaw=game.pitch=0;game.speed=0;}
 if(s==OP_DOCK){change_page(COMMS_PANEL);row=3;comms_return=FLIGHT;}
 if(s==OP_FOOD)game.stock[0]=game.stock[0]>0?game.stock[0]:1;
 if(s==OP_THARGOID||s==OP_BURST||s==OP_WAKE)change_page(FLIGHT);
 if(s==OP_BURST)game.cue=SFX_WARP;
 if(s>=OP_WAKE&&s<=OP_LAVE_LOCK){for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;for(int i=0;i<DEBRIS_COUNT;i++)game.debris[i].alive=0;}
 if(s==OP_DAD_INBOX)galnet_tab=4;
 if(s==OP_CIVILISATION){game_spawn(&game);if(game.opening.elapsed<=0&&game.opening.distance<=0){game.pos=(Vec3){2000,0,8000};game.yaw=atan2f(-2000.f,-8000.f);game.pitch=0;game.speed=110;}radio_tune(RADIO_DEFAULT_STATION);}
 if(s==OP_ADVERT){change_page(FLIGHT);NPC *ad=&game.npc[NPC_COUNT-1];ad->alive=1;ad->role=TRADERS;ad->pos=add(game.pos,(Vec3){250,70,450});ad->health=1000;ad->traveller=-1;game.speed=0;game.cue=SFX_COMM;}
 if(s==OP_APP){game.opening.flags|=OF_APP;game.cue=SFX_UI;row=23;deck_last=23;}
 if(s==OP_NEWS&&(game.opening.flags&OF_DIALOG_DONE))game.opening.counter=0;
 if(s==OP_AFTER_NEWS){game.opening.flags|=OF_NEWS;opening_checkpoint();}
 if(opening_coaching(s)){game.opening.flags|=OF_DIALOG_DONE;game.opening.counter=0;opening_input_gate=0;}
 opening_dialogue_sync();
}
static int opening_is_scene(int s){return s==OP_HOME||s==OP_CITY||s==OP_DEALER||s==OP_PEP||s==OP_OBJECT||s==OP_OBJECT_CHOICE;}
static int opening_is_dialogue(int s){return s==OP_SAVE_INFO||s==OP_GIFT||s==OP_ROCK||s==OP_YARD_INTRO||s==OP_OUTFIT_INTRO||s==OP_FOOD_INTRO||s==OP_THARGOID||s==OP_WAKE||s==OP_MAP_INTRO||s==OP_LAVE_LOCK||s==OP_ADVERT||s==OP_APP||s==OP_NEWS_INTRO||s==OP_NEWS||s==OP_AFTER_NEWS||s==OP_MUM_CALL;}
static int opening_is_flight(int s){return s==OP_DREAM||s==OP_YAW||s==OP_PITCH||s==OP_THROTTLE||s==OP_BRAKE||s==OP_WALK||s==OP_FIND||s==OP_RETURN||s==OP_ORBIT||s==OP_APPROACH||s==OP_TOUCHDOWN||s==OP_MINE||s==OP_COLLECT||s==OP_STATION_LOCK||s==OP_DOCK||s==OP_HOME_FLIGHT||s==OP_LONELY||s==OP_CIVILISATION||s==OP_LAVE_DOCK;}
static int opening_guard_flight(void){
 if(opening_dream(&game))return 0;
 opening_keep_clear(&game,game.pos);
 if(opening_sol(&game)&&game.message_time>0&&!strcmp(game.message,"Let's not get too close to the planet just yet.")){
  autoaim=0;planet_entry_body=planet_landing_menu=0;planet_orbit_release=0;
  game.message_time=game.voice_time=0;opening_boundary_voice=8;return 1;
 }return 0;
}
static int opening_input(unsigned pressed,unsigned held,float dt,float ax,float ay){
 if(!opening_exists(&game))return 0;opening_pulse+=dt;opening_boundary_voice=fmaxf(0,opening_boundary_voice-dt);
 if(!((pressed|held)&(PSP_CTRL_CROSS|PSP_CTRL_CIRCLE)))opening_input_gate=0;
 if(opening_done(&game)){
  if(game.mission_result>0)game.opening.flags|=OF_FACTIONS;if(game.discoveries>0)game.opening.flags|=OF_LORE;
  if(page==GALNET)return opening_net_input(pressed);
  if(page==HOME&&(pressed&PSP_CTRL_CROSS)&&row==17){change_page(CAMPAIGN);return 1;}
  if(page==CAMPAIGN&&tracked_mission==0){if(pressed&PSP_CTRL_CIRCLE)change_page(HOME);if(pressed&PSP_CTRL_SELECT)change_page(MISSIONLOG);return 1;}
  return 0;
 }
 if(game.dock_stage==4){page=FLIGHT;tracked_game_input(0,0,dt,0,0);if(!game.dock_stage&&game.opening.stage==OP_SOL_MAP){game.pos=add(game.bodies[1].pos,(Vec3){0,0,-game.bodies[1].radius-1600});game.yaw=game.pitch=0;selected_target=2;autoaim=0;opening_prepared=-1;opening_checkpoint();}return 1;}
 opening_prepare();if(opening_guard_flight())return 1;const OpeningBeat *b=opening_beat();int s=game.opening.stage;
 if(s==OP_DREAM){
  dream_time+=dt;
  if(dream_ending){dream_wake+=dt;radio_dream_gain=(int)(1000*fmaxf(0,1-dream_wake/6));if(dream_wake>=6){radio_dream_gain=-1;opening_advance();opening_prepare();}return 1;}
  radio_dream_gain=dream_time>=DREAM_ATTACK?1000:0;
  Vec3 previous_player=game.pos;
  if(dream_time>=1.6f){
   unsigned mask=PSP_CTRL_UP|PSP_CTRL_DOWN|PSP_CTRL_LEFT|PSP_CTRL_RIGHT|PSP_CTRL_RTRIGGER|PSP_CTRL_LTRIGGER|PSP_CTRL_CROSS;
   selected_target=-1;autoaim=0;
   tracked_game_input(pressed&mask,held&mask,dt,ax,ay);
  }
  dream_fleet_tick(dt,previous_player);
  int alive=0;for(int i=0;i<3;i++)alive+=game.npc[i].alive;
  if(game.dead||game.hull<=25||(dream_spawned==3&&!alive)||dream_time>=DREAM_LIMIT){dream_ending=1;game.dead=0;game.boost=0;game.speed=0;game.voice_time=game.message_time=0;}
  return 1;
 }
 if(opening_notice){if((pressed&(PSP_CTRL_CROSS|PSP_CTRL_CIRCLE))&&!opening_input_gate){opening_notice=0;opening_input_gate=1;}return 1;}
 if(game.dead){game.dead=0;game.hull=50;game.energy=30;game.damaged=1;game.explosion=0;opening_notice_set("COMPUTER","Training recovery. Your ship is safe; continue the current lesson.");return 1;}
 if(s==OP_CREATE){
  if(opening_skip_prompt){
   if(pressed&PSP_CTRL_CIRCLE){opening_skip_prompt=0;opening_input_gate=1;return 1;}
   if(pressed&(PSP_CTRL_UP|PSP_CTRL_DOWN|PSP_CTRL_LEFT|PSP_CTRL_RIGHT))opening_skip_choice=!opening_skip_choice;
   if((pressed&PSP_CTRL_CROSS)&&!opening_input_gate){opening_input_gate=1;if(opening_skip_choice)opening_skip_to_lave();else {opening_skip_prompt=0;opening_advance();}}
   return 1;
  }
  if(profile_mode||profile_portrait_edit){profile_input(pressed);return 1;}
  if(pressed&PSP_CTRL_CIRCLE){quit_to_main_menu();opening_input_gate=1;return 1;}
  if(pressed&PSP_CTRL_UP)opening_create_row=(opening_create_row+2)%3;if(pressed&PSP_CTRL_DOWN)opening_create_row=(opening_create_row+1)%3;
  if((pressed&PSP_CTRL_CROSS)&&!opening_input_gate){if(opening_create_row<2){row=opening_create_row?3:2;profile_input(PSP_CTRL_CROSS);}else {opening_skip_prompt=1;opening_skip_choice=0;opening_input_gate=1;}}return 1;
 }
 const OpeningDialogueNode *dialogue=opening_current_dialogue();
 if(dialogue){
  opening_dialogue_sync();opening_dialogue_clock+=dt;
  if(opening_dialogue_revealed()&&dialogue->choice_count){
   int choice=game.opening.counter%10;if(choice>=dialogue->choice_count)choice=0;
   if(pressed&PSP_CTRL_UP)choice=(choice+dialogue->choice_count-1)%dialogue->choice_count;
   if(pressed&PSP_CTRL_DOWN)choice=(choice+1)%dialogue->choice_count;
   game.opening.counter=dialogue->id*10+choice;
  }
  if((pressed&PSP_CTRL_CROSS)&&!opening_input_gate){
   opening_input_gate=1;
   if(!opening_dialogue_revealed()){opening_dialogue_clock=(float)strlen(opening_dialogue_text(dialogue))*.028f;return 1;}
   int next=dialogue->choice_count?dialogue->choice_next[game.opening.counter%10]:dialogue->next;
   if(next){opening_dialogue_enter(next);return 1;}
   game.opening.flags|=OF_DIALOG_DONE;game.opening.counter=0;opening_seen_node=-1;
   if(s==OP_OBJECT){game.opening.flags|=OF_OBJECT;game.opening.choice=1;}
   if(s==OP_GIFT)game.opening.flags|=OF_GUN;
   if(s==OP_AFTER_NEWS){game.opening.stage=OP_FREE;game.opening.counter=0;opening_prepared=-1;change_page(HOME);opening_checkpoint();return 1;}
   if((opening_is_scene(s)&&s!=OP_DEALER)||opening_is_dialogue(s))opening_advance();
   else opening_checkpoint();
  }return 1;
 }
 if(s==OP_DEALER){
  if(opening_dealer_confirm){
   if(pressed&(PSP_CTRL_UP|PSP_CTRL_DOWN))opening_dealer_answer^=1;
   if((pressed&PSP_CTRL_CIRCLE)&&!opening_input_gate){opening_dealer_confirm=0;opening_input_gate=1;return 1;}
   if((pressed&PSP_CTRL_CROSS)&&!opening_input_gate){opening_input_gate=1;
    if(opening_dealer_answer){opening_dealer_confirm=0;return 1;}
    game.ship=opening_ship_choice;game.fuel=(float)player_ships[game.ship].range;
    game.energy=game.hull=100;game.damaged=0;opening_dealer_confirm=0;opening_advance();
   }return 1;
  }
  if(pressed&PSP_CTRL_LEFT)opening_ship_choice=(opening_ship_choice+2)%3;
  if(pressed&PSP_CTRL_RIGHT)opening_ship_choice=(opening_ship_choice+1)%3;
  if(pressed&PSP_CTRL_SQUARE)opening_dealer_mesh^=1;
  if((pressed&PSP_CTRL_CROSS)&&!opening_input_gate){opening_dealer_confirm=1;opening_dealer_answer=0;opening_input_gate=1;opening_dialogue_clock=100;}
  return 1;
 }
 if(opening_input_gate){return 1;}
 if(s==OP_BURST){game.opening.elapsed+=dt;if(game.opening.elapsed>=2.5f)opening_displace();return 1;}
 if(s==OP_MUM_RING){if(pressed&PSP_CTRL_TRIANGLE)opening_advance();return 1;}
 if(s==OP_MUM_TALK){row=0;if(pressed&PSP_CTRL_CROSS)opening_advance();return 1;}
 if(page==HOME){
  if(b->service>=0){row=b->service;deck_last=row;if(pressed&PSP_CTRL_CROSS){if(s==OP_LAUNCH||s==OP_HOME_LAUNCH){launch_departure(&game);change_page(FLIGHT);opening_advance();}else {change_page(s==OP_NEWS?LOCALTV:tutorial_page_for(b->service));if(s==OP_DAD_INBOX)galnet_tab=4;}}return 1;}
  if(opening_is_flight(s)){if(pressed&PSP_CTRL_CROSS)change_page(FLIGHT);return 1;}
 }
 if(page==GALNET){int leave=(pressed&PSP_CTRL_CIRCLE)&&!opening_net_detail;opening_net_input(pressed);if(leave&&(s==OP_EARTH_NET||s==OP_DAD_INBOX))opening_advance();return 1;}
 if(page==STATUS){int result=profile_input(pressed);if(s==OP_FIRST_SAVE&&result)opening_advance();return 1;}
 if(page==DETAILS&&opening_sol(&game)){
  if(pressed&PSP_CTRL_DOWN)row=(row+1)%(BODY_COUNT+1);if(pressed&PSP_CTRL_UP)row=(row+BODY_COUNT)%(BODY_COUNT+1);
  if((pressed&PSP_CTRL_CIRCLE)&&s==OP_SOL_MAP){opening_advance();return 1;}
  if(pressed&PSP_CTRL_CROSS){if(s==OP_EARTH_ROUTE){selected_target=2;opening_advance();}else if(s==OP_STATION_ROUTE){selected_target=0;opening_advance();}else if(s==OP_HOME_ROUTE){selected_target=2;opening_advance();}}return 1;
 }
 if(page==INVENTORY&&s==OP_LOADOUT){
  if(pressed&PSP_CTRL_CROSS){game.fit[FIT_WPN]=OPENING_MINIBOLT;game.active_weapon=0;fit_rebuild(&game);opening_notice_set("COMPUTER","MiniBolt fitted to WPN 1. Mining tool ready. Barely scratches a ship.");}
  if((pressed&PSP_CTRL_CIRCLE)&&fit_find(&game,OPENING_MINIBOLT)>=0)opening_advance();return 1;
 }
 if(page==MISSIONLOG&&s==OP_LOG){if(pressed&PSP_CTRL_CROSS)opening_advance();return 1;}
 if(page==CAMPAIGN&&s==OP_TRACK){if(pressed&PSP_CTRL_CIRCLE){opening_advance();change_page(FLIGHT);}return 1;}
 if((page==YARD&&s==OP_YARD)||(page==EQUIP&&s==OP_OUTFIT)){
  if(pressed&PSP_CTRL_CROSS){opening_notice_set("DAD",s==OP_YARD?"Can I have tha-? No. Let's get you parking this one first.":"Actually we're just here to get a few pizzas.");return 1;}
  if(pressed&PSP_CTRL_CIRCLE){opening_advance();return 1;}tracked_game_input(pressed&~PSP_CTRL_SELECT,0,dt,0,0);return 1;
 }
 if(page==MARKET&&s==OP_FOOD){row=0;int before=game.cargo[0];if(pressed&PSP_CTRL_RIGHT)trade(&game,0,1);if(game.cargo[0]>before){game.opening.flags|=OF_PIZZA;opening_advance();}return 1;}
 if(page==REPAIR&&(s==OP_REPAIR||s==OP_REFUEL)){
  if(s==OP_REPAIR&&(pressed&PSP_CTRL_CROSS)&&repair_ship(&game)&&game.hull>=100)opening_advance();
  if(s==OP_REFUEL&&(pressed&PSP_CTRL_TRIANGLE)){engineer_refuel();if(game.fuel>=player_ships[game.ship].range)opening_advance();}return 1;
 }
 if(page==RADIO&&s==OP_RADIO){if(pressed&PSP_CTRL_CROSS)radio_tune(RADIO_DEFAULT_STATION);if((pressed&PSP_CTRL_CIRCLE)&&!radio_off){opening_advance();return 1;}tracked_game_input(pressed&~PSP_CTRL_SELECT,held,dt,ax,ay);return 1;}
 if(page==CHART&&s==OP_GALAXY){if(pressed&PSP_CTRL_CIRCLE){opening_advance();return 1;}tracked_game_input(pressed&~(PSP_CTRL_CROSS|PSP_CTRL_SELECT),held&~PSP_CTRL_CROSS,dt,ax,ay);return 1;}
 if(page==DECORATOR&&s==OP_APP_VIEW){if(pressed&PSP_CTRL_CIRCLE){opening_advance();return 1;}tracked_game_input(pressed&~PSP_CTRL_SELECT,held,dt,ax,ay);return 1;}
 if(page==LOCALTV&&s==OP_NEWS){if((pressed&PSP_CTRL_CROSS)&&++game.opening.counter>=3)opening_advance();return 1;}
 if(opening_is_flight(s)){
  if(s==OP_APPROACH&&!planet_entry_body&&game.planet<0&&!(pressed&PSP_CTRL_CROSS))return 1;
  float yaw=game.yaw,pitch=game.pitch;int shots=game.shots,surface=game.surface,planet=game.planet,cargo=game.cargo[12];Vec3 before=game.pos;
  unsigned p=pressed,h=held;
  p&=~(PSP_CTRL_SELECT|PSP_CTRL_SQUARE);h&=~(PSP_CTRL_SELECT|PSP_CTRL_SQUARE);
  if(game.surface==2){p&=~PSP_CTRL_START;h&=~PSP_CTRL_START;}
  int triangle_allowed=s==OP_COLLECT||s==OP_STATION_LOCK||s==OP_DOCK||s==OP_LAVE_DOCK||(s==OP_RETURN&&length(sub(game.pos,game.ship_pos))<150);
  if(!triangle_allowed){p&=~PSP_CTRL_TRIANGLE;h&=~PSP_CTRL_TRIANGLE;}
  p&=~PSP_CTRL_CIRCLE;h&=~PSP_CTRL_CIRCLE;
  if(s!=OP_DREAM&&s!=OP_MINE&&s!=OP_APPROACH&&s!=OP_TOUCHDOWN){p&=~(PSP_CTRL_CROSS|PSP_CTRL_CIRCLE);h&=~(PSP_CTRL_CROSS|PSP_CTRL_CIRCLE);}
  if(s==OP_FIND&&(pressed&PSP_CTRL_TRIANGLE)){
   Vec3 pad=surface_site(&game,1);
   float dx=game.pos.x-pad.x,dz=game.pos.z-pad.z;
   if((game.opening.flags&OF_MEADOW_GLOW)&&length(sub(game.pos,opening_object_pos()))<35){opening_advance();return 1;}
   if(!(game.opening.flags&OF_MEADOW_FLOWERS)&&earth_meadow_near_flowers(dx,dz)){game.opening.flags|=OF_MEADOW_FLOWERS|OF_MEADOW_GLOW;opening_notice_set("DAD","Funny little flowers. They are all leaning the same way, away from the sun. Follow them gently. Let's see what they have found.");opening_checkpoint();return 1;}
   if(!(game.opening.flags&OF_MEADOW_LAKE)&&hypotf(dx+86,dz-118)<38){game.opening.flags|=OF_MEADOW_LAKE;opening_notice_set("DAD","Those little blue ones are damselflies. Your mum would know the proper name. She always does. Leave the flowers for them, eh?");opening_checkpoint();return 1;}
   if(!(game.opening.flags&OF_MEADOW_VIEW)&&hypotf(dx-125,dz-245)<38){game.opening.flags|=OF_MEADOW_VIEW;opening_notice_set("DAD","That stream goes all the way down there. You can hear it before you see it. Imagine finding a place like this on a world nobody has named yet.");opening_checkpoint();return 1;}
  }
  if((s==OP_DOCK||s==OP_LAVE_DOCK)&&page==COMMS_PANEL){row=3;p=pressed&PSP_CTRL_CROSS;h=0;}
  tracked_game_input(p,h,dt,ax,ay);if(opening_guard_flight())return 1;game.opening.elapsed+=dt;
  if(page==FLIGHT&&!paused&&s!=OP_YAW&&s!=OP_PITCH){float travelled=length(sub(game.pos,before));if(travelled<5000)game.opening.distance+=travelled;}
  if(s==OP_DREAM){game.energy=game.hull=100;game.damaged=game.dead=0;if(fabsf(game.yaw-yaw)>.0001f)game.opening.counter|=256;if(fabsf(game.pitch-pitch)>.0001f)game.opening.counter|=512;if(game.shots>shots&&(game.opening.counter&255)<250)game.opening.counter++;if((game.opening.counter&768)==768&&(game.opening.counter&255)>=3)opening_advance();}
  if(s==OP_YAW&&fabsf(game.yaw-yaw)>.0001f){game.opening.distance+=dt;if(game.opening.distance>=3)opening_advance();}
  if(s==OP_PITCH&&fabsf(game.pitch-pitch)>.0001f){game.opening.distance+=dt;if(game.opening.distance>=3)opening_advance();}
  if(s==OP_THROTTLE&&game.speed>=150)opening_advance();
  if(s==OP_BRAKE&&game.speed<40&&(held&PSP_CTRL_LTRIGGER))opening_advance();
  if(s==OP_APPROACH&&game.planet>=0)opening_advance();
  if(s==OP_TOUCHDOWN&&game.surface>=1&&!game.planet_sequence)opening_advance();
  if(s==OP_WALK&&game.surface==2&&game.opening.distance>=35)opening_advance();
  if(s==OP_FIND&&!(game.opening.flags&OF_MEADOW_GLOW)&&game.opening.elapsed>=18&&game.opening.distance>=55){game.opening.flags|=OF_MEADOW_GLOW;opening_notice_set("PLAYER","Those pale flowers are glowing. There's a brighter light where the trail ends.");opening_checkpoint();}
  if(s==OP_RETURN&&surface==2&&game.surface==1)opening_advance();
  if(s==OP_ORBIT&&game.planet<0&&!planet_entry_body&&!planet_orbit_veil)opening_advance();
  if(s==OP_MINE&&!game.debris[DEBRIS_COUNT-1].rock)opening_advance();
  if(s==OP_COLLECT&&game.cargo[12]>cargo){game.opening.flags|=OF_MINED;opening_advance();}
  if(s==OP_STATION_LOCK&&page==COMMS_PANEL)opening_advance();
  if(s==OP_DOCK&&game.docked&&!game.dock_stage)opening_advance();
  if(s==OP_HOME_FLIGHT&&game.opening.distance>=1600&&game.opening.elapsed>=18)opening_advance();
  if(s==OP_LONELY&&game.opening.distance>=1800&&game.opening.elapsed>=24)opening_advance();
  if(s==OP_CIVILISATION&&game.opening.distance>=1200&&game.opening.elapsed>=14)opening_advance();
  if(s==OP_LAVE_DOCK&&game.docked&&!game.dock_stage)opening_advance();return 1;
 }return 1;
}
static int opening_space_caption(void){return page==FLIGHT&&game.planet<0&&game.opening.stage!=OP_SOL_MAP;}
static void opening_bottom(const char *speaker,const char *copy,const char *task){
 int top=opening_space_caption()?24:184;
 adventure_panel(0,top,W,88,!strcmp(speaker,"PLAYER"));
 opening_portrait(speaker,12,top+14,56);rect(12,top+70,56,1,UI_GOLD);
 text_px(80,top+10,UI_GOLD,"%s",!strcmp(speaker,"PLAYER")?game.commander_name:speaker);
 text_wrap(10,(top+24)/8,48,5,WHITE,copy,0);
 if(task)text_px(12,top+77,UI_CYAN,"%.57s",task);
}
static void opening_dialogue_plate(const OpeningDialogueNode *n){
 int choices=n->choice_count&&opening_dialogue_revealed();int top=opening_space_caption()?24:choices?128:184;int height=choices?144:88;
 adventure_panel(0,top,W,height,!strcmp(n->speaker,"PLAYER"));
 opening_portrait(n->speaker,12,top+14,56);rect(12,top+70,56,1,UI_GOLD);
 text_px(80,top+10,UI_GOLD,"%s",!strcmp(n->speaker,"PLAYER")?game.commander_name:n->speaker);
 const char *copy=opening_dialogue_text(n);char revealed[192];int len=(int)strlen(copy),count=(int)(opening_dialogue_clock/.028f);if(count>len)count=len;if(count>191)count=191;
 memcpy(revealed,copy,count);revealed[count]=0;
 text_wrap(10,(top+24)/8,48,5,WHITE,revealed,0);
 if(choices){int selected=n->id?game.opening.counter%10:opening_dealer_answer;if(selected>=n->choice_count)selected=0;
  for(int i=0;i<n->choice_count;i++){int y=top+74+i*17;rect(80,y,384,15,i==selected?RGB(29,62,73):RGB(13,30,42));if(i==selected)rect(80,y,2,15,UI_GOLD);text_px(88,y+3,i==selected?UI_GOLD:WHITE,"%s %.44s",i==selected?">":" ",n->choices[i]);}
 }
 text_px(12,top+height-11,UI_CYAN,choices?"UP/DOWN CHOOSE   X REPLY":opening_dialogue_revealed()?"X CONTINUE":"X REVEAL LINE");
}
static void opening_city_ascent(void){
 int blend=(int)(fminf(1,fmaxf(0,(game.dock_timer-1.1f)/2.7f))*256);
 const OpeningBitmap *a=&opening_city_bitmap,*b=&opening_orbit_bitmap;
 for(int y=0;y<H;y++)for(int x=0;x<W;x++){
  int at=y*W+x;unsigned ca=a->palette[a->pixels[at]],cb=b->palette[b->pixels[at]];
  int r=((ca&255)*(256-blend)+(cb&255)*blend)>>8;
  int g=(((ca>>8)&255)*(256-blend)+((cb>>8)&255)*blend)>>8;
  int blue=(((ca>>16)&255)*(256-blend)+((cb>>16)&255)*blend)>>8;
  fb[y*STRIDE+x]=RGB(r,g,blue);
 }
 opening_scene_title(game.dock_timer<2?"EARTH / FIRST DEPARTURE":"SOL / YOUR FIRST FLIGHT","GUIDANCE ON");
 opening_bottom("DAD","There we go. Your first flight. Have a look down there.","City traffic clear. Climbing to Earth orbit.");
}
static void opening_sol_details(void){
 rect(0,0,W,H,BG);header("SYSTEM DETAILS / SOL");
 static const char *names[]={"Mercury","Venus","Earth","Mars","Jupiter","Saturn","Uranus","Neptune"};
 for(int i=0;i<8;i++){int x=22+i*58;unsigned ink=i==2?CYAN:i>=4?GOLD:DIM;rect(x,86,10+(i>3?5:0),10+(i>3?5:0),ink);text_px(x-10,114,WHITE,"%.7s",names[i]);}
 text(2,5,GOLD,"THE SUN / YOUR LOCAL SOLAR SYSTEM");text(2,17,CYAN,"EARTH ORBITAL / COUNTRYSIDE / HOME");
 text(2,20,DIM,"Home, the countryside, then Earth Orbital.");
}
static void opening_mission_screen(void){
 rect(0,0,W,H,BG);header(page==MISSIONLOG?"MISSION LOG":"TRACKED MISSION");
 text(3,6,CYAN,"DAD / FIRST MINING PRACTICE");text(3,9,WHITE,"Break the practice rock with your MiniBolt.");
 text(3,12,WHITE,"Collect the mineral with Triangle / tractor.");text(3,15,GOLD,"MINERALS COLLECTED: %d / 1",(game.opening.flags&OF_MINED)?1:0);
 text(3,18,DIM,"Dad has given you a safe practice task.");text(3,21,GOLD,page==MISSIONLOG?"> X TRACK THIS MISSION":"O RETURN TO FLIGHT");
}
static void opening_news_screen(void){
 rect(0,0,W,H,RGB(4,11,24));opening_scene_draw(1);
 rect(0,0,W,28,RGB(6,15,28));text(2,1,GOLD,"LAVE LOCAL TV / SOL ATTACK");
 text(3,6,WHITE,"EARTH / DISTANT SOL SYSTEM");text(3,19,RED,"THARGOID ATTACK / DEVELOPING STORY");
 static const char *copy[]={"Reports are emerging of a Thargoid attack in the distant Sol system. Communications with Earth remain disrupted.","The number of missing vessels is unknown. These images show Earth before the disruption.","No confirmed passenger list is available. Pilots are asked to retain navigation records."};
 const OpeningDialogueNode *n=opening_current_dialogue();
 if(n)opening_dialogue_plate(n);else opening_bottom("NEWSREADER",copy[game.opening.counter<3?game.opening.counter:2],"X NEXT BULLETIN");
}
static void opening_draw(void){
 if(!opening_exists(&game))return;
 if(opening_done(&game)){
  if(page==CAMPAIGN&&tracked_mission==0){rect(0,0,W,H,BG);header("MAIN STORY / FIND DAD");text(3,8,GOLD,"DAD IS MISSING. EARTH IS OUT OF REACH.");text_wrap(3,12,53,6,WHITE,"Keep the unknown object. Earn money, meet local people and look for information about the Sol attack. Your personal story continues through Spacebook Inbox. Jobs are in Mission Log.",0);footer("SELECT MISSION LOG   O BACK");}return;
 }
 const OpeningBeat *b=opening_beat();int s=game.opening.stage;
 if(s==OP_DREAM){
  dream_overlay();
  return;
 }
 if(s==OP_SOL_MAP&&game.dock_stage==4){opening_city_ascent();return;}
 if(s==OP_CREATE){
  if(profile_mode||profile_portrait_edit)return;
  opening_scene_draw(0);rect(20,40,440,200,RGB(9,22,34));rect(20,40,440,1,UI_GOLD);
  rect(20,40,2,200,UI_GOLD);rect(38,57,88,88,RGB(42,71,82));
  draw_commander_portrait(42,61,80,game.commander_portrait);
  text_px(146,55,UI_GOLD,"YOUR FIRST DAY STARTS HERE");
  text_px(146,76,UI_CYAN,"%.24s",game.commander_name);
  text_px(146,94,WHITE,"EARTH / 2050   AGE 17");
  const char *labels[]={"CHOOSE YOUR NAME","CREATE YOUR PORTRAIT","START NEW GAME"};
  for(int i=0;i<3;i++){int y=126+i*30;rect(146,y,294,25,i==opening_create_row?RGB(30,62,73):RGB(13,30,42));if(i==opening_create_row)rect(146,y,2,25,UI_GOLD);text_px(158,y+9,i==opening_create_row?UI_GOLD:WHITE,"%s %s",i==opening_create_row?">":" ",labels[i]);}
  if(opening_skip_prompt){
   rect(62,76,356,128,RGB(7,18,29));rect(62,76,356,2,UI_GOLD);rect(62,76,2,128,UI_GOLD);rect(416,76,2,128,UI_GOLD);
   text_px(154,91,UI_GOLD,"SKIP THE TUTORIAL?");
   text_wrap(11,14,38,4,WHITE,"Start docked at Lave Hub after the opening lessons. Your commander name and portrait are kept.",0);
   const char *opts[]={"NO - PLAY THE STORY","YES - START AT LAVE"};for(int i=0;i<2;i++){int y=165+i*22;rect(108,y,264,18,i==opening_skip_choice?RGB(30,62,73):RGB(13,30,42));text_px(120,y+5,i==opening_skip_choice?UI_GOLD:WHITE,"%s %s",i==opening_skip_choice?">":" ",opts[i]);}
  }
  rect(20,244,440,23,RGB(9,22,34));text_px(32,253,UI_CYAN,"UP/DOWN CHOOSE");button_icon(176,250,'X',UI_CYAN);text_px(192,253,WHITE,"SELECT");button_icon(352,250,'O',UI_CYAN);text_px(368,253,WHITE,"BACK");return;
 }
 /* Hold the orbital artwork throughout the first conversation, then hand
  * control to the actual cockpit once the player chooses to open the map. */
 if(s==OP_SOL_MAP&&opening_current_dialogue()){
  opening_bitmap_draw(&opening_orbit_bitmap,0,0,W,H);opening_scene_title("EARTH / YOUR FIRST ORBIT","SOL 2050");
 }
 if(s==OP_HOME)opening_scene_draw(0);if(s==OP_CITY)opening_scene_draw(1);if(s==OP_DEALER)opening_scene_draw(2);
 if((s==OP_PEP||s==OP_OBJECT||s==OP_OBJECT_CHOICE)&&!(page==FLIGHT&&game.surface==2))opening_scene_draw(3);
 if(s==OP_THARGOID||s==OP_BURST){opening_scene_draw(4);opening_alien_contacts();}if(s==OP_WAKE)opening_scene_draw(5);
 if(s==OP_DEALER){
  if(opening_dealer_mesh)opening_showroom_mesh(opening_ship_choice);else opening_showroom_illustration(opening_ship_choice);
  if(!opening_current_dialogue()){
   opening_dealer_card(opening_ship_choice);
   if(opening_dealer_confirm){
    OpeningDialogueNode confirm={0,0,0,"DAD","This the one? All right. I'll cover the ship. The credits you've saved are for your first jobs.",2,{"This is the one.","Keep looking.",0},{0,0,0},0};
    opening_dialogue_plate(&confirm);
   }return;
  }
 }
 /* System Details uses the same native almanac renderer as free play. */
 if((s==OP_LOG||s==OP_TRACK)&&(page==MISSIONLOG||page==CAMPAIGN))opening_mission_screen();
 if(s==OP_MUM_TALK){rect(8,64,220,22,RGB(38,83,94));text(3,9,GOLD,"> TALK TO MUM");}
 if(s==OP_NEWS&&page==LOCALTV){opening_news_screen();return;}
 if(s==OP_BURST)opening_burst_overlay(game.opening.elapsed);
 if(s==OP_FIND&&page==FLIGHT&&game.surface==2){
  Vec3 pad=surface_site(&game,1);float dx=game.pos.x-pad.x,dz=game.pos.z-pad.z;
  int near=((game.opening.flags&OF_MEADOW_GLOW)&&length(sub(game.pos,opening_object_pos()))<35)||(!(game.opening.flags&OF_MEADOW_FLOWERS)&&earth_meadow_near_flowers(dx,dz))||(!(game.opening.flags&OF_MEADOW_LAKE)&&hypotf(dx+86,dz-118)<38)||(!(game.opening.flags&OF_MEADOW_VIEW)&&hypotf(dx-125,dz-245)<38);
  if(near&&!opening_notice){rect(182,238,116,20,UI_PANEL);button_icon(190,242,'T',UI_CYAN);text_px(206,244,WHITE,"INSPECT");}
 }
 if((s==OP_DOCK||s==OP_LAVE_DOCK)&&page==COMMS_PANEL){rect(16,93,448,20,RGB(34,76,89));text(3,12,GOLD,"> REQUEST AUTO-DOCK");}
 if(page==HOME&&!(s==OP_SOL_MAP&&opening_current_dialogue())&&(b->service>=0||s==OP_APP)){int required=s==OP_APP?23:b->service,y,h;if(dc_row_geometry(deck_group(row),required,&y,&h)){unsigned glow=(int)(opening_pulse*3)&1?GOLD:CYAN;line(11,y-1,220,y-1,glow);line(11,y+h,220,y+h,glow);rect(10,y,2,h,glow);}}
 const OpeningDialogueNode *dialogue=opening_current_dialogue();
 if(opening_notice){opening_bottom(opening_notice_speaker,opening_notice_copy,"X CONTINUE");return;}
 if(dialogue){opening_dialogue_plate(dialogue);return;}
 if(page==FLIGHT&&s==OP_FIND&&!opening_notice&&!dialogue)return;
 if(page==FLIGHT&&opening_coaching(s)){
  int top=game.planet<0?24:222;
  rect(0,top,W,50,RGB(8,19,30));rect(0,top,W,1,UI_GOLD);opening_portrait(b->speaker,8,top+7,28);
  text_wrap(5,(top+2)/8,54,3,WHITE,opening_boundary_voice>0?"Let's not get too close to the planet just yet. Plenty of room to practise out here.":b->line,0);
  text_px(8,top+39,UI_CYAN,"%.58s",b->task);return;
 }
 if(page!=HOME&&page!=FLIGHT){rect(0,248,W,24,UI_PANEL);text_px(8,252,UI_CYAN,"%.58s",b->task);return;}
 opening_bottom(b->speaker,b->line,b->task);
}
