/* Integration checks use the real hail, panel, reply and consequence paths. */
static void conversation_review_reset(void){
 game_init(&game);story_complete(&game);launch(&game);game.dock_stage=0;
 game.voice_time=game.message_time=0;game.encounter_kind=ENCOUNTER_NONE;game.encounter=0;
 night_npc=-1;night_open=0;comms_encounter_conversation=comms_trade_stage=comms_quick=0;
 quiet_comms=comms_quiet=0;comms_return=FLIGHT;deck_reset();page=FLIGHT;
 game.npc[0].alive=1;game.npc[0].role=TRADERS;game.npc[0].traveller=-1;
 game.npc[0].pos=add(game.pos,(Vec3){0,0,500});
}
static void conversation_review_call(int kind){
 game.encounter_kind=kind;game.encounter_npc=0;game.encounter=8;
 const char *line=kind==ENCOUNTER_PIRATE?"This is your only warning, Commander. Jettison two tonnes of cargo, or we open fire.":
  kind==ENCOUNTER_SMUGGLER?"Commander, I have a private cargo exchange. Interested?":
  kind==ENCOUNTER_BOUNTY?"Local bounty network. A wanted pilot is in your lane; their transponder and warrant are confirmed.":
  kind==ENCOUNTER_ESCAPE_POD?"Escape pod calling. Life support is failing. Can you bring me aboard?":
  "Mayday! We are taking fire and our shields are failing. Any ship nearby, please answer.";
 encounter_speak(&game,VOICE_CONTACT,line);game.voice_role=kind==ENCOUNTER_BOUNTY?LAW:kind==ENCOUNTER_PIRATE?PIRATES:TRADERS;
}
static void conversation_routing_checks(FILE *out,int *failures){
#define ROUTE_CHECK(c,label) do{int ok=!!(c);fprintf(out,"%s %s\n",ok?"PASS":"FAIL",label);*failures+=!ok;fflush(out);}while(0)
 conversation_review_reset();int valid=1;
 const int actionable[]={ENCOUNTER_DISTRESS,ENCOUNTER_CONVOY,ENCOUNTER_PIRATE,ENCOUNTER_BOUNTY,ENCOUNTER_ESCAPE_POD,ENCOUNTER_SMUGGLER};
 for(unsigned i=0;i<sizeof(actionable)/sizeof(*actionable);i++){conversation_review_call(actionable[i]);valid&=incoming_reply_ready();}
 ROUTE_CHECK(valid,"only explicitly authored actionable calls offer replies");
 int catalogue=1;
 for(int e=0;e<75;e++)for(unsigned k=0;k<sizeof(actionable)/sizeof(*actionable);k++){
  conversation_review_call(actionable[k]);speak(&game,VOICE_CONTACT,discovery_catalogue[e].reply);
  game.voice_role=TRADERS;game.voice_seed=-1000-e;
  catalogue&=!incoming_reply_ready();comms_channel_capture(0);
  catalogue&=comms_encounter_conversation==2&&comms_channel.kind==ENCOUNTER_NONE&&!strcmp(comms_channel.name,discovery_speakers[e])&&!strcmp(comms_channel.text,discovery_catalogue[e].reply);
 }
 ROUTE_CHECK(catalogue,"all 75 discovery speakers and texts reject six unrelated actionable encounter types");
 int discovered=1,safe=1;const int systems[]={0,1,2,7};
 for(int s=0;s<4;s++){
  conversation_review_reset();game.system=systems[s];system_bodies(&game);
  for(int slot=0;slot<discovery_count();slot++)for(int kind=1;kind<=ENCOUNTER_UNKNOWN;kind++){
   selected_target=DISCOVERY_ID_MIN+slot;target_explicit=1;game.pos=add(discovery_position(slot),(Vec3){0,0,-300});
   conversation_review_call(kind);game.voice_time=0;game.message_time=0;
   int credits=game.credits,passenger=game.passenger_dest;float attacked=game.attacked;int cargo[GOODS];memcpy(cargo,game.cargo,sizeof(cargo));
   change_page(COMMS_PANEL);row=1;game_input(PSP_CTRL_CROSS,0,.016f,0,0);
   discovered&=comms_encounter_conversation==2&&!incoming_reply_ready()&&!strcmp(comms_channel.text,discovery_data(selected_target)->reply)&&!strcmp(comms_channel.name,discovery_speakers[discovery_entry(slot)]);
   row=0;game_input(PSP_CTRL_CROSS,0,.016f,0,0);
   safe&=page==FLIGHT&&credits==game.credits&&passenger==game.passenger_dest&&attacked==game.attacked&&!memcmp(cargo,game.cargo,sizeof(cargo))&&game.encounter_kind==kind;
  }
 }
 ROUTE_CHECK(discovered,"all 32 live discoveries open their own contact-only channel across all 14 background encounter types");
 ROUTE_CHECK(safe,"closing every live discovery cannot award credits or cargo, rescue passengers, provoke combat or consume another encounter");
 conversation_review_reset();selected_target=DISCOVERY_ID_MIN+3;target_explicit=1;game.pos=add(discovery_position(3),(Vec3){0,0,-300});
 conversation_review_call(ENCOUNTER_DISTRESS);game.voice_time=0;change_page(COMMS_PANEL);row=1;game_input(PSP_CTRL_CROSS,0,.016f,0,0);
 int ranger=!strcmp(comms_channel.name,"RANGER")&&!strcmp(comms_channel.text,discovery_catalogue[63].reply)&&comms_channel.kind==ENCOUNTER_NONE;
 rect(0,0,W,H,BG);comms_panel();dump_native_bmp("conversation-sanctuary-374.bmp");
 ROUTE_CHECK(ranger,"sanctuary buoy uses Ranger speech and a close-channel option rather than Promise assistance");
 conversation_review_reset();conversation_review_call(ENCOUNTER_DISTRESS);speak(&game,VOICE_DOCK,"You are clear to dock.");
 ROUTE_CHECK(!incoming_reply_ready(),"station control speech cannot inherit a trader rescue action");
 conversation_review_call(ENCOUNTER_DISTRESS);contact_speak(EXPLORERS,"We are surveying this sky.");
 ROUTE_CHECK(!incoming_reply_ready(),"an outgoing pilot hail cannot inherit another pilot's encounter");
 conversation_review_call(ENCOUNTER_DISTRESS);speak(&game,VOICE_KEI,"A mission transmission.");
 ROUTE_CHECK(!incoming_reply_ready(),"mission speech cannot inherit ambient encounter replies");
 conversation_review_call(ENCOUNTER_DISTRESS);speak(&game,VOICE_CONTACT,"An unrelated broadcast.");int credits=game.credits;encounter_respond(&game);
 ROUTE_CHECK(game.credits==credits&&!strcmp(game.voice,"An unrelated broadcast."),"backend refuses a response when the current transmission does not own the encounter");
 conversation_review_reset();conversation_review_call(ENCOUNTER_DISTRESS);comms_answer_incoming();
 int snapshot=comms_channel_valid();speak(&game,VOICE_CONTACT,"A new pilot on another channel.");
 row=0;game_input(PSP_CTRL_CROSS,0,.016f,0,0);
 ROUTE_CHECK(snapshot&&page==FLIGHT&&!strcmp(game.voice,"A new pilot on another channel.")&&game.voice_time>0&&game.credits==1000,"a replaced transmission invalidates the old response without dismissing or acting on the new speaker");
 conversation_review_reset();conversation_review_call(ENCOUNTER_DISTRESS);comms_answer_incoming();game.encounter_kind=ENCOUNTER_PIRATE;row=0;game_input(PSP_CTRL_CROSS,0,.016f,0,0);
 ROUTE_CHECK(page==FLIGHT&&game.attacked<=0&&game.encounter_kind==ENCOUNTER_PIRATE,"changed encounter kinds cannot execute through an already open channel");
 conversation_review_reset();conversation_review_call(ENCOUNTER_DISTRESS);comms_answer_incoming();game.encounter_npc=1;
 ROUTE_CHECK(!comms_channel_valid()&&!encounter_requires_reply(&game),"changed caller IDs invalidate both panel actions and quick reply availability");
 conversation_review_reset();conversation_review_call(ENCOUNTER_DISTRESS);comms_answer_incoming();game.npc[0].alive=0;
 ROUTE_CHECK(!comms_channel_valid()&&!encounter_requires_reply(&game),"destroyed callers cannot retain actionable dialogue");
 conversation_review_reset();conversation_review_call(ENCOUNTER_DISTRESS);comms_answer_incoming();game.system=1;
 ROUTE_CHECK(!comms_channel_valid()&&!encounter_requires_reply(&game),"channels cannot transfer their actions across solar systems");
 conversation_review_reset();conversation_review_call(ENCOUNTER_DISTRESS);comms_answer_incoming();game.encounter=0;
 ROUTE_CHECK(!comms_channel_valid()&&!encounter_requires_reply(&game),"expired encounters cannot keep actionable responses");
 conversation_review_reset();conversation_review_call(ENCOUNTER_DISTRESS);comms_answer_incoming();credits=game.credits;row=0;game_input(PSP_CTRL_CROSS,0,.016f,0,0);encounter_respond(&game);
 ROUTE_CHECK(page==FLIGHT&&game.credits==credits&&game.job_n==1&&game.jobs[0].type==MISSION_DISTRESS&&strstr(game.voice,"attacker is marked")&&!strstr(game.voice,"saved our convoy"),"promising distress assistance creates one real unpaid contract without a false success or repeatable reward");
 conversation_review_reset();conversation_review_call(ENCOUNTER_PIRATE);comms_answer_incoming();row=1;game_input(PSP_CTRL_CROSS,0,.016f,0,0);
 int explained=comms_channel_valid()&&strstr(comms_channel.text,"two tonnes");row=0;game_input(PSP_CTRL_CROSS,0,.016f,0,0);
 ROUTE_CHECK(explained&&game.attacked>0&&page==FLIGHT,"a real pirate call preserves clarification and its own refusal consequence");
 conversation_review_reset();conversation_review_call(ENCOUNTER_PIRATE);comms_answer_incoming();row=2;game_input(PSP_CTRL_CROSS,0,.016f,0,0);
 ROUTE_CHECK(page==FLIGHT&&game.attacked>0&&strstr(game.voice,"Dump cargo"),"closing a pirate threat is an explicit refusal and starts the promised attack");
 conversation_review_reset();conversation_review_call(ENCOUNTER_DISTRESS);comms_answer_incoming();row=2;game_input(PSP_CTRL_CROSS,0,.016f,0,0);
 ROUTE_CHECK(page==FLIGHT&&!game.encounter&&strstr(game.voice,"keep broadcasting"),"declining a distress call gives a natural acknowledgement without accepting it");
 conversation_review_reset();conversation_review_call(ENCOUNTER_DISTRESS);comms_answer_incoming();row=1;game_input(PSP_CTRL_CROSS,0,.016f,0,0);
 ROUTE_CHECK(page==COMMS_PANEL&&(strstr(comms_channel.text,"shields")||strstr(comms_channel.text,"cargo deck")||strstr(comms_channel.text,"shooting")||strstr(comms_channel.text,"Drive them off")),"asking about payment during an attack receives an urgent human reaction rather than calm contract copy");
 conversation_review_reset();conversation_review_call(ENCOUNTER_BOUNTY);game.voice_role=LAW;comms_answer_incoming();comms_panel();dump_native_bmp("broadcast-15-bounty-network.bmp");
 ROUTE_CHECK(!strcmp(comms_channel.name,"LOCAL BOUNTY NETWORK")&&comms_channel.role==LAW,"bounty leads come from the network rather than the wanted target reporting itself");
 row=1;game_input(PSP_CTRL_CROSS,0,.016f,0,0);ROUTE_CHECK(strstr(comms_channel.text,"Verified payout")&&strstr(comms_channel.text,"Target threat"),"bounty clarification supplies new payout and risk information instead of repeating the opening line");
 conversation_review_reset();game.npc[0].freighter=0;game.npc[0].target=-1;night_begin(0,0);int ready=night_ready();speak(&game,VOICE_CONTACT,discovery_catalogue[63].reply);game.voice_seed=-1063;
 ROUTE_CHECK(ready&&!night_ready(),"discovery speech cannot inherit Quiet Watch Talk and Ignore controls before its next tick");
 conversation_review_reset();conversation_review_call(ENCOUNTER_DISTRESS);game_spawn(&game);
 ROUTE_CHECK(game.encounter_kind==ENCOUNTER_NONE&&!encounter_requires_reply(&game),"spawning a new local world clears prior encounter ownership");
 {unsigned char saved=game.travellers[0].flags;game.travellers[0].flags=0;
  const char *intro=traveller_hail(&game,0);traveller_note_hail(&game,0);
  const char *familiar=traveller_hail(&game,0);traveller_note_hail(&game,0);
  const char *personal=traveller_hail(&game,0);traveller_note_hail(&game,0);
  const char *closed=traveller_hail(&game,0);traveller_note_hail(&game,0);
  ROUTE_CHECK(strstr(intro,"Mira Vane")&&strcmp(intro,familiar)&&strcmp(familiar,personal)&&strcmp(personal,closed),"named travellers progress from an introduction through familiar and personal conversation to a distinct sign-off");
  ROUTE_CHECK(game.travellers[0].flags==7&&!strcmp(closed,traveller_hail(&game,0)),"exhausted traveller conversation stays closed instead of repeating its introduction");game.travellers[0].flags=saved;}
 conversation_review_reset();
#undef ROUTE_CHECK
}
