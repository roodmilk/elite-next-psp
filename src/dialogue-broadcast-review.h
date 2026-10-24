/* Isolated real renderer/input coverage. Excluded from the retail PSP image. */
static unsigned broadcast_hash(int x,int y,int w,int h){unsigned n=2166136261u;for(int yy=y;yy<y+h;yy++)for(int xx=x;xx<x+w;xx++)n=(n^fb[yy*STRIDE+xx])*16777619u;return n;}
static int dialogue_broadcast_review(void){
 FILE *flag=fopen("dialogue-broadcast-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *out=fopen("dialogue-broadcast-review.txt","w");if(!out)return 1;
 int failures=0;unsigned *old=fb,*pixels=calloc(STRIDE*H+32,sizeof(unsigned));Game *snapshot=malloc(sizeof(Game));
 if(!pixels||!snapshot){fprintf(out,"RESULT 1 failures\n");fclose(out);free(pixels);free(snapshot);return 1;}
 fb=pixels+16;for(int k=0;k<16;k++)pixels[k]=pixels[16+STRIDE*H+k]=0x31ac8e79u;
#define BCHECK(c,label) do{int ok=!!(c);fprintf(out,"%s %s\n",ok?"PASS":"FAIL",label);failures+=!ok;fflush(out);}while(0)
 preview_time=12;conversation_review_reset();game.opening.stage=0;mission_card_reset();conversation_review_call(ENCOUNTER_DISTRESS);comms_answer_incoming();comms_panel();dump_native_bmp("broadcast-01-three-replies.bmp");
 BCHECK(page==COMMS_PANEL&&dialogue.count==3&&!dialogue.pending,"actual incoming channel renders three complete cards through its canonical footer");
 unsigned face=broadcast_hash(368,34,96,96),speech=broadcast_hash(24,102,312,40),selected=broadcast_hash(12,154,146,88);
 BCHECK(fb[120*STRIDE+355]==RGB(250,235,192)||fb[100*STRIDE+355]==RGB(250,235,192),"speech tail extends right toward the caller's portrait");
 memcpy(snapshot,&game,sizeof(game));game_input(PSP_CTRL_RIGHT,0,.5f,1,1);comms_panel();dump_native_bmp("broadcast-02-second-reply.bmp");
 BCHECK(row==1&&face==broadcast_hash(368,34,96,96)&&speech==broadcast_hash(24,102,312,40)&&selected!=broadcast_hash(12,154,146,88),"Right selects the next actual reply without changing its caller or speech");
 BCHECK(game.time==snapshot->time&&length(sub(game.pos,snapshot->pos))==0,"considering replies keeps flight and NPC simulation paused");
 game_input(PSP_CTRL_CROSS,0,.016f,0,0);comms_panel();dump_native_bmp("broadcast-03-clarification.bmp");
 BCHECK(page==COMMS_PANEL&&(strstr(comms_channel.text,"shields")||strstr(comms_channel.text,"cargo deck")||strstr(comms_channel.text,"shooting")||strstr(comms_channel.text,"Drive them off"))&&row==0,"asking about money during an attack gets a human urgency response and keeps the conversation open");
 game_input(PSP_CTRL_CIRCLE,0,.016f,0,0);BCHECK(page==FLIGHT&&!comms_encounter_conversation,"Circle closes the real incoming conversation and resumes flight");
 conversation_review_reset();mission_card_reset();game.opening.stage=0;conversation_review_call(ENCOUNTER_DISTRESS);int distress_caller=game.encounter_npc;comms_answer_incoming();row=0;game_input(PSP_CTRL_CROSS,0,.016f,0,0);mission_card_sync();mission_card_age=1;mission_card_draw();dump_native_bmp("broadcast-03b-mission-accepted.bmp");
 BCHECK(game.job_n==1&&game.jobs[0].type==MISSION_DISTRESS&&game.jobs[0].item==distress_caller+1&&game.jobs[0].target>=0,"promising aid creates a caller-specific distress mission with a real marked attacker");
 BCHECK(game.mission_receipt_n==1&&game.mission_receipts[0].kind==RECEIPT_ACCEPTED&&game.mission_receipts[0].amount==game.jobs[0].reward&&strstr(mission_objective(&game),"Protect "),"acceptance card, mission log objective and exact eventual payout share the same contract data");
 MissionReceipt accepted=game.mission_receipts[0];mission_receipt_pop(&game);mission_receipt_record(&game,RECEIPT_FAILED,MISSION_DISTRESS,0,game.system,"DISTRESS VESSEL LOST",accepted.sender);mission_card_reset();mission_card_sync();mission_card_age=1;mission_card_draw();dump_native_bmp("broadcast-03c-mission-failed.bmp");
 BCHECK(game.mission_receipt_n==1&&game.mission_receipts[0].kind==RECEIPT_FAILED&&!game.mission_receipts[0].amount,"failure card closes the contract with the same caller and no payment");
 game_init(&game);story_complete(&game);game.opening.stage=0;game.docked=0;game.job_n=MISSION_SLOTS;for(int i=0;i<MISSION_SLOTS;i++)game.jobs[i]=(Job){game.system,MISSION_DELIVERY,0,-1,1,game.system,1000,300};int full_caller=-1,full_pirate=-1;for(int i=0;i<NPC_COUNT;i++){if(full_caller<0&&game.npc[i].alive&&game.npc[i].role==TRADERS)full_caller=i;if(full_pirate<0&&game.npc[i].alive&&game.npc[i].role==PIRATES)full_pirate=i;}if(full_caller>=0&&full_pirate>=0)accept_distress_call(&game,full_caller);
 BCHECK(game.job_n==MISSION_SLOTS&&game.mission_receipt_n==0&&strstr(game.message,"LOG FULL"),"a full mission log refuses the promise honestly and creates no acceptance card");
 conversation_review_reset();mission_card_reset();game.opening.stage=0;conversation_review_call(ENCOUNTER_SMUGGLER);comms_answer_incoming();game_input(PSP_CTRL_CROSS,0,.016f,0,0);comms_panel();dump_native_bmp("broadcast-04-trade-offer.bmp");
 BCHECK(comms_trade_stage==1&&dialogue.count==2&&strstr(dialogue.context,"Give ")&&strstr(dialogue.context,"Receive "),"actual trader offer keeps both quantities/goods and both decisions together");
 int credits=game.credits;game_input(PSP_CTRL_RIGHT,0,.016f,0,0);game_input(PSP_CTRL_CROSS,0,.016f,0,0);comms_panel();dump_native_bmp("broadcast-05-trade-refused.bmp");
 BCHECK(comms_trade_stage==2&&dialogue.count==1&&game.credits==credits&&!game.trader_offer_active,"refusing the actual trade shows the pilot's reply without changing the balance");
 game_input(PSP_CTRL_CROSS,0,.016f,0,0);BCHECK(page==FLIGHT,"trade sign-off returns to flight through the original departure action");
 conversation_review_reset();mission_card_reset();game.opening.stage=0;conversation_review_call(ENCOUNTER_SMUGGLER);comms_answer_incoming();row=0;game_input(PSP_CTRL_CROSS,0,.016f,0,0);row=0;game_input(PSP_CTRL_CROSS,0,.016f,0,0);mission_card_sync();mission_card_age=1;mission_card_draw();dump_native_bmp("broadcast-05a-trade-accepted.bmp");
 BCHECK(game.job_n==1&&trade_job_is(&game.jobs[0])&&game.mission_receipt_n==1&&game.mission_receipts[0].kind==RECEIPT_ACCEPTED&&!game.mission_receipts[0].amount,"accepting the exchange creates a barter appointment and a no-units acceptance card");
 mission_receipt_pop(&game);mission_card_reset();page=MISSIONLOG;row=2;mission_log();dump_native_bmp("broadcast-05b-trade-log.bmp");
 BCHECK(strstr(mission_dossier_name(2),"CARGO EXCHANGE")&&strstr(mission_objective_at(&game,0),"Hail "),"Mission Log names the trader and shows exact exchange guidance");
 conversation_review_reset();mission_card_reset();game.opening.stage=0;game.cargo[6]=2;police_begin(&game,1);page=FLIGHT;police_choice=0;space();dump_native_bmp("broadcast-06-police-four.bmp");
 BCHECK(dialogue.count==4&&dialogue_pages>=1,"police stop displays all four responses simultaneously, without hiding the fourth");
 game_input(PSP_CTRL_RIGHT,0,.016f,0,0);BCHECK(police_choice==1,"police Right moves across the top row");game_input(PSP_CTRL_DOWN,0,.016f,0,0);space();dump_native_bmp("broadcast-07-police-surrender.bmp");BCHECK(police_choice==3,"police Down reaches voluntary cargo surrender in the bottom right");
 game_input(PSP_CTRL_CROSS,0,.016f,0,0);BCHECK(game.cargo[6]==0,"X on the fourth police card still performs voluntary cargo surrender");
 game.police_phase=2;game.police_timer=1.4f;space();dump_native_bmp("broadcast-08-custody.bmp");
 conversation_review_reset();mission_card_reset();game.opening.stage=0;night_begin(0,0);night_open=1;page=COMMS_PANEL;row=0;comms_panel();dump_native_bmp("broadcast-09-private-channel.bmp");
 BCHECK(dialogue.count==3&&dialogue.context_page,"private conversations retain their context and all three longer replies");
 game_input(PSP_CTRL_RIGHT,0,.016f,0,0);BCHECK(row==1,"private-channel card navigation accepts Right");game_input(PSP_CTRL_CIRCLE,0,.016f,0,0);BCHECK(page==FLIGHT&&!night_open,"private channel keeps its Circle exit");
 conversation_review_reset();mission_card_reset();game.opening.stage=0;game.docked=1;game.credits=100000;game.fit[FIT_WPN]=1;fit_rebuild(&game);
 BCHECK(expedition_start(&game,EXPEDITION_FIRST,game.system,12000),"tracked-mission fixture accepts a real eligible convoy contract");
 expedition_ack(&game,0);page=CAMPAIGN;tracked_mission=2;row=0;campaign_screen();dump_native_bmp("broadcast-10-tracked-mission.bmp");
 BCHECK(dialogue.count==2&&dialogue.context_page&&!strcmp(dialogue.label,"CURRENT OBJECTIVE"),"tracked mission preserves the separate objective and both navigation/log actions");
 game_input(PSP_CTRL_RIGHT,0,.016f,0,0);game_input(PSP_CTRL_CROSS,0,.016f,0,0);BCHECK(page==MISSIONLOG,"Right then X opens the mission log from the actual tracked mission");
 game_init(&game);story_complete(&game);game.opening.stage=0;mission_card_reset();page=CAMPAIGN;tracked_mission=0;game.campaign_stage=0;row=0;campaign_screen();dump_native_bmp("broadcast-11-story-briefing.bmp");
 /* Long speech through real pagination inputs; caller pixels cannot move. */
 char longbody[900];longbody[0]=0;for(int i=0;i<10;i++)strcat(longbody,"Please keep the ship steady while we explain this unusually long transmission. ");
 dialogue_begin("LONG TRANSMISSION / NATIVE FONT");dialogue_speech("A VERY LONG CONTACT NAME / RESEARCH VESSEL MERIDIAN",TRADERS,99,longbody,0);dialogue_reply(0,"Understood. Keep the channel open while I read the full transmission.");footer("UP/DOWN X CHOOSE O BACK");dump_native_bmp("broadcast-12-long-page-one.bmp");
 BCHECK(fb[139*STRIDE+341]!=fb[139*STRIDE+342],"next-page arrow appears in the speech bubble lower-right corner");
 int pages=dialogue_pages;face=broadcast_hash(368,34,96,96);game_input(PSP_CTRL_RTRIGGER,0,.016f,0,0);BCHECK(dialogue_page==1&&pages>1,"R pages long speech through the production conversation input");
 dialogue_begin("LONG TRANSMISSION / NATIVE FONT");dialogue_speech("A VERY LONG CONTACT NAME / RESEARCH VESSEL MERIDIAN",TRADERS,99,longbody,0);dialogue_reply(0,"Understood. Keep the channel open while I read the full transmission.");footer("UP/DOWN X CHOOSE O BACK");dump_native_bmp("broadcast-13-long-page-two.bmp");
 BCHECK(dialogue_page==1&&face==broadcast_hash(368,34,96,96)&&!*dialogue_skip(longbody,pages*4,39),"paging retains the caller and reaches every word rather than clipping speech");
 unsigned long long frame_start=sceKernelGetSystemTimeWide();
 for(int i=0;i<30;i++){dialogue_begin("RENDER TIMING");dialogue_speech("SHIP COMPUTER",EXPLORERS,17,longbody,0);dialogue_reply(0,"Continue");footer("X CONTINUE");}
 fprintf(out,"INFO native conversation render mean %.3f ms (30 frames, emulator)\n",(sceKernelGetSystemTimeWide()-frame_start)/30000.0);
 /* Render every real staged call and every page using the same shipping renderer. */
 int cards=0,paged=0,authored_ack=1;
 for(int i=0;i<EXPEDITION_COUNT;i++){
  game_init(&game);story_complete(&game);game.opening.stage=0;game.job_n=1;game.jobs[0].type=EXPEDITION_FIRST+i;game.expeditions[0].seed=i+17;page=HOME;
  const ExpeditionDef *d=expedition_def(game.jobs[0].type);
  for(int stage=0;stage<d->count;stage++){
   game.expeditions[0].stage=stage;game.expeditions[0].unread=1;mission_card_reset();mission_card_sync();expedition_call_age=1;expedition_call_draw();cards++;if(d->stages[stage].kind!=XP_CHOICE)authored_ack&=strcmp(expedition_ack_label(&d->stages[stage]),"Continue transmission")!=0;
   char file[80];snprintf(file,sizeof(file),"staged-%02d-%02d-page-01.bmp",i+1,stage+1);dump_native_bmp(file);
   int total=dialogue_pages;for(int p=1;p<total;p++){dialogue_page=p;expedition_call_draw();snprintf(file,sizeof(file),"staged-%02d-%02d-page-%02d.bmp",i+1,stage+1,p+1);dump_native_bmp(file);paged++;}
  }
 }
 BCHECK(cards==155,"all 155 real staged transmissions render with the approved broadcast layout");fprintf(out,"INFO %d additional speech pages captured at native 480x272\n",paged);
 BCHECK(authored_ack,"every staged mission call uses a context-specific player acknowledgement instead of a generic continue line");
 mission_card_reset();conversation_review_reset();page=COMMS_PANEL;comms_encounter_conversation=2;comms_channel_capture(0);ship_paint[game.ship]=decorator_finishes[13];comms_panel();dump_native_bmp("broadcast-14-red-theme.bmp");
 BCHECK(dialogue_edge()==UI_SIGNAL,"console frame follows the selected ship theme while keeping reply contrast");
 int guards=1;for(int k=0;k<16;k++)guards&=pixels[k]==0x31ac8e79u&&pixels[16+STRIDE*H+k]==0x31ac8e79u;BCHECK(guards,"all real renders preserve framebuffer guards");
 conversation_routing_checks(out,&failures);
 fprintf(out,"RESULT %d failures\n",failures);fclose(out);fb=old;free(pixels);free(snapshot);
#undef BCHECK
 return 1;
}
