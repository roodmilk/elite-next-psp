/* Disposable, opt-in checks against real completion, input and save paths. */
static void mission_reward_fixture(void){
 game_init(&game);story_complete(&game);game.opening.stage=0;deck_reset();page=FLIGHT;
 paused=0;mission_card_reset();game.voice_time=game.message_time=0;
}
static int mission_reward_review(void){
 FILE *flag=fopen("mission-reward-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *out=fopen("mission-reward-review.txt","w");if(!out)return 1;
 unsigned *old=fb,*mem=calloc(STRIDE*H+32,sizeof(unsigned));Game *snapshot=malloc(sizeof(Game));
 if(!mem||!snapshot){fprintf(out,"RESULT 1 failures\n");fclose(out);free(mem);free(snapshot);return 1;}
 fb=mem+16;for(int i=0;i<16;i++)mem[i]=mem[STRIDE*H+16+i]=0x7194abceu;
 int failures=0;
 #define MRCHECK(c,label) do{int ok=!!(c);fprintf(out,"%s %s\n",ok?"PASS":"FAIL",label);failures+=!ok;fflush(out);}while(0)
 mission_reward_fixture();game.docked=0;game.dock_stage=3;game.dock_timer=1.39f;game.credits=10000;
 game.passenger_dest=game.system;game.passenger_pay=1200;game.cargo[0]=5;game.job_n=5;game.contract=-1;
 for(int i=0;i<5;i++)game.jobs[i]=(Job){.dest=game.system,.type=MISSION_DELIVERY,.target=-1,.item=0,.origin=7,.reward=1000+i*100,.time=900};
 game_tick(&game,.02f,0,0,0,0);
 MRCHECK(game.docked&&!game.dock_stage&&!game.job_n&&!game.cargo[0],"real final docking leg delivers all five reserved shipments");
 MRCHECK(game.mission_receipt_n==6&&game.credits==17200,"passenger and five simultaneous contracts each receive their own paid receipt");
 int values=game.mission_receipts[0].kind==RECEIPT_PASSENGER&&game.mission_receipts[0].amount==1200;
 for(int i=1;i<6;i++)values&=game.mission_receipts[i].amount==900+i*100&&game.mission_receipts[i].kind==RECEIPT_BOARD;
 MRCHECK(values,"receipt order and each actual reward match the production payments");
 mission_card_sync();MRCHECK(mission_card_open()&&!paused,"mission receipt opens its own modal without changing the player's pause preference");
 memcpy(snapshot,&game,sizeof(game));input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.02f,0,0);
 MRCHECK(game.mission_receipt_n==6,"the completion press cannot instantly dismiss its own celebration");
 for(int i=0;i<30;i++)input(PSP_CTRL_CIRCLE|PSP_CTRL_RTRIGGER|PSP_CTRL_START,PSP_CTRL_CROSS|PSP_CTRL_RTRIGGER,.05f,1,1);
 MRCHECK(!memcmp(snapshot,&game,sizeof(game))&&page==FLIGHT,"modal consumes flight, menu and cancel controls while the complete game state remains frozen");
 static const float phases[]={.1f,.3f,.62f,1.1f,2.f};
 for(int i=0;i<5;i++){mission_card_age=phases[i];mission_card_draw();char file[48];snprintf(file,sizeof(file),"mission-transmission-%d.bmp",i);dump_native_bmp(file);}
 MRCHECK(!memcmp(snapshot,&game,sizeof(game)),"all slide, signal and fireworks rendering leaves payout and gameplay state unchanged");
 int cash=game.credits;input(0,0,.016f,0,0);input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 MRCHECK(game.mission_receipt_n==5&&game.credits==cash,"fresh X acknowledges exactly one receipt without awarding money again");
 input(0,PSP_CTRL_CROSS,1,0,0);MRCHECK(game.mission_receipt_n==5,"held acknowledgement cannot skip the next queued transmission");
 for(int i=0;i<5;i++){input(0,0,.7f,0,0);input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);}
 MRCHECK(!game.mission_receipt_n&&game.credits==cash,"all six cards can be acknowledged in order with the original payment unchanged");
 int shots=game.shots;input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 MRCHECK(game.shots==shots&&mission_card_release,"closing X is consumed until release rather than firing or activating a menu");
 input(0,0,.016f,0,0);MRCHECK(!mission_card_release&&!mission_card_open(),"releasing controls restores normal input after the final transmission");
 game.docked=0;game.dock_stage=3;game.dock_timer=1.39f;game_tick(&game,.02f,0,0,0,0);
 MRCHECK(!game.mission_receipt_n&&game.credits==cash,"docking again cannot replay completed deliveries or their receipts");
 mission_reward_fixture();game.campaign_stage=5;int before=game.credits;
 MRCHECK(campaign_claim(&game)&&game.mission_receipt_n==1&&game.mission_receipts[0].amount==1000&&game.credits==before+1000,"first-flight claim records the exact committed payment");
 MRCHECK(!campaign_claim(&game)&&game.mission_receipt_n==1,"repeating a first-flight claim does not queue a duplicate card");
 int reward_ok=1;const int guild_pay[]={1000,1500,2000,3000},guild_need[]={GUILD_LAUNCH|GUILD_DOCK,GUILD_DELIVERY,GUILD_SCAN,GUILD_RESCUE};
 for(int i=0;i<4;i++){mission_reward_fixture();game.guild_chapter=i;game.guild_flags=guild_need[i];int c=game.credits;reward_ok&=guild_claim(&game)&&game.credits==c+guild_pay[i]&&game.mission_receipt_n==1&&game.mission_receipts[0].amount==guild_pay[i]&&game.mission_receipts[0].type==i;}
 MRCHECK(reward_ok,"all four Guild chapters record their own payment, chapter title and contact");
 mission_reward_fixture();MRCHECK(!guild_claim(&game)&&!game.mission_receipt_n,"an incomplete Guild assignment produces no celebration or receipt");
 game.saga_step=1;game.saga_chapter=0;game.saga_flags|=SAGA_CASE_HELD;before=game.credits;
 MRCHECK(saga_advance(&game)&&game.mission_receipt_n==1&&game.credits==before+game.mission_receipts[0].amount&&!strcmp(game.mission_receipts[0].sender,saga_beats[0].speaker),"story chapter acknowledgement records its real payout and canonical speaker");
 MRCHECK(!saga_advance(&game)&&game.mission_receipt_n==1,"unready next story chapter cannot produce a duplicate celebration");
 mission_reward_fixture();game.saga_step=1;game.saga_chapter=0;game.saga_flags|=SAGA_CASE_HELD;game.credits=100000000;
 MRCHECK(saga_advance(&game)&&game.mission_receipts[0].amount==0&&game.credits==100000000,"a credit-capped chapter never claims an unpaid reward was transferred");
 mission_reward_fixture();game.story=STORY_WORLD;story_advance(&game,STORY_FREE);
 MRCHECK(game.mission_receipt_n==1&&game.mission_receipts[0].kind==RECEIPT_STORY&&game.mission_receipts[0].amount==2500,"original story finale records its actual completion reward");
 mission_reward_fixture();MRCHECK(!game.mission_receipt_n,"ordinary story initialization does not falsely celebrate a completion");
 mission_reward_fixture();game.credits=100000;int accepted=accept_mission(&game,0);
 MRCHECK(accepted&&abandon_mission(&game,0)&&!game.mission_receipt_n,"abandoning a contract never produces a mission-completion celebration");
 mission_reward_fixture();sc_lave_set(6);before=game.credits;lave_action(SC_R_CARGO);
 MRCHECK(game.credits==before+600&&game.mission_receipt_n==1&&!strcmp(game.mission_receipts[0].sender,"MARA"),"Lave manifest completion records Mara's actual payment and identity");
 before=game.credits;lave_action(SC_R_CARGO);
 MRCHECK(game.credits==before&&game.mission_receipt_n==1,"revisiting Mara cannot reissue her payment or transmission");
 sc_lave_set(8);game.surface_progress[7][1]|=0x100u;before=game.credits;lave_action(SC_R_GUILD);
 MRCHECK(game.credits==before+900&&game.mission_receipt_n==2&&!strcmp(game.mission_receipts[1].sender,"IONA"),"Lave fieldwork completion records Iona's actual payment and identity");
 MRCHECK(mission_card_seed(&game.mission_receipts[1])==7*37+EXPLORERS*17+'I'*3,"station thank-you portrait uses its actual station NPC seed");
 mission_reward_fixture();
 game.campaign_stage=5;campaign_claim(&game);before=game.credits;
 MRCHECK(save_game(&game,"reward-check.sav")&&load_game(snapshot,"reward-check.sav"),"paid completion persists through the unchanged production save format");
 MRCHECK(snapshot->credits==before&&snapshot->campaign_stage==6&&!snapshot->mission_receipt_n,"loading retains payment and progression without reissuing transient receipt cards");
 MRCHECK(!campaign_claim(snapshot)&&snapshot->credits==before&&!snapshot->mission_receipt_n,"a reloaded completion cannot pay or celebrate a second time");
 remove("reward-check.sav");remove("reward-check.sav.bak");remove("reward-check.sav.tmp");
 mission_reward_fixture();mission_receipt_record(&game,RECEIPT_BOARD,MISSION_DELIVERY,3380,7,"MEMORIAL DELIVERY / A LONG WAY HOME","MISSION DESK");
 int safe=1;
 game.police_stop=1;mission_card_sync();safe&=!mission_card_open();game.police_stop=0;
 game.dock_stage=1;mission_card_sync();safe&=!mission_card_open();game.dock_stage=0;
 game.jump=1;mission_card_sync();safe&=!mission_card_open();game.jump=0;
 game.dead=1;mission_card_sync();safe&=!mission_card_open();game.dead=0;
 MRCHECK(safe&&game.mission_receipt_n==1,"celebration waits through police stops, docking, hyperspace and death without losing the receipt");
 paused=1;mission_card_sync();input(0,0,1.1f,0,0);mission_card_draw();dump_native_bmp("mission-transmission-delivery.bmp");
 MRCHECK(mission_card_open()&&paused&&mission_card_age>=1.1f,"card animation advances even when the underlying screen was already paused");
 unsigned expected_seed=0;for(const char *p="MISSION DESK";*p;p++)expected_seed=expected_seed*131u+(unsigned char)*p;
 MRCHECK(mission_card_seed(&game.mission_receipts[0])==(int)(expected_seed%2000)+TRADERS*37&&mission_card_role(&game.mission_receipts[0])==TRADERS,"mission desk uses the same faction portrait system as its conversation");
 for(int kind=RECEIPT_BOARD;kind<=RECEIPT_STATION;kind++){
  MissionReceipt *r=&game.mission_receipts[0];r->kind=kind;r->type=0;r->amount=kind==RECEIPT_STATION?0:100000000;
  snprintf(r->sender,sizeof(r->sender),"%s",kind==RECEIPT_CAMPAIGN||kind==RECEIPT_GUILD||kind==RECEIPT_STORY?"KEI":kind==RECEIPT_SAGA?saga_beats[0].speaker:"MISSION DESK");
  char file[48];snprintf(file,sizeof(file),"mission-transmission-kind-%d.bmp",kind);mission_card_draw();dump_native_bmp(file);
 }
 int all_fit=1;
 for(int type=0;type<MISSION_TYPES;type++){MissionReceipt r={.kind=RECEIPT_BOARD,.type=type};all_fit&=mission_card_wrap(32,56,52,2,WHITE,mission_name(type))&&mission_card_wrap(152,136,37,5,WHITE,mission_card_thanks(&r));}
 MRCHECK(all_fit,"every one of the 26 mission titles and thank-you messages fits its reserved text region");
 MissionReceipt *r=&game.mission_receipts[0];snprintf(r->title,sizeof(r->title),"A LONG MISSION TITLE THAT NEEDS TWO READABLE LINES ON SCREEN");snprintf(r->sender,sizeof(r->sender),"CAPTAIN ALEXANDRA VALE");r->amount=100000000;
 mission_card_draw();dump_native_bmp("mission-transmission-long.bmp");
 MRCHECK(mission_card_wrap(32,56,52,2,WHITE,r->title),"maximum authored receipt title wraps without truncating the last words");
 /* Sample all animation offsets: portraits as well as text must clip. */
 for(int frame=0;frame<=80;frame++){mission_card_age=frame*.03f;mission_card_draw();}
 int guards=1;for(int i=0;i<16;i++)guards&=mem[i]==0x7194abceu&&mem[STRIDE*H+16+i]==0x7194abceu;
 MRCHECK(guards,"all slide positions, portrait sizes and fireworks preserve framebuffer memory guards");
 mission_reward_fixture();for(int i=0;i<MISSION_RECEIPT_MAX+5;i++)mission_receipt_record(&game,RECEIPT_BOARD,0,100,7,"TEST","MISSION DESK");
 MRCHECK(game.mission_receipt_n==MISSION_RECEIPT_MAX,"receipt queue is bounded when deliberately overloaded");
 fprintf(out,"RESULT %d failures\n",failures);fclose(out);fb=old;free(mem);free(snapshot);mission_card_reset();
 #undef MRCHECK
 return 1;
}
