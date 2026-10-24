/* Actual input/render coverage for the 371 menu fixes. */
static int mf_text_at(int x,int y,int width,unsigned bg,unsigned ink,const char *label){
 unsigned expected[144*8];rect(0,0,width,8,bg);text_px(0,0,ink,"%s",label);
 for(int yy=0;yy<8;yy++)for(int xx=0;xx<width;xx++)expected[yy*144+xx]=fb[yy*STRIDE+xx];
 system_details();int same=1;for(int yy=0;yy<8;yy++)for(int xx=0;xx<width;xx++)same&=expected[yy*144+xx]==fb[(y+yy)*STRIDE+x+xx];return same;
}
static int menu_fixes_review(void){
 FILE *flag=fopen("menu-fixes-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *out=fopen("menu-fixes-review.txt","w");if(!out)return 1;
 unsigned *old=fb,*pixels=calloc(STRIDE*H+32,sizeof(unsigned));if(!pixels){fprintf(out,"RESULT 1 failures\n");fclose(out);return 1;}fb=pixels+16;
 for(int i=0;i<16;i++)pixels[i]=pixels[STRIDE*H+16+i]=0x13579adfu;
 int failures=0;
#define MFCHECK(c,s) do{int ok=!!(c);fprintf(out,"%s %s\n",ok?"PASS":"FAIL",s);failures+=!ok;fflush(out);}while(0)
 game_init(&game);story_complete(&game);game.opening.stage=OP_FREE;game.tutorial_step=0;page=DETAILS;row=0;preview_time=18;
 int counts=1,labels=1,crowded=7,multiple=7,max=0;
 for(int sys=0;sys<256;sys++){
  game.system=sys;system_bodies(&game);int stars=0,worlds=0,n=system_body_slots(sys);for(int i=0;i<n;i++)if(game.bodies[i].type==SUN)stars++;else worlds++;
  char value[32];snprintf(value,sizeof(value),"%d / %d",stars,worlds);
  counts&=mf_text_at(324,217,136,UI_PANEL,WHITE,value);labels&=mf_text_at(324,205,136,UI_PANEL,UI_CYAN,"STARS / WORLDS");
  if(n>max){max=n;crowded=sys;}if(stars>1)multiple=sys;
 }
 MFCHECK(counts&&labels,"all 256 systems show exact unobscured stars/worlds numbers on their own line");
 int systems[]={7,crowded,multiple};for(int i=0;i<3;i++){game.system=systems[i];system_bodies(&game);system_details();char file[64];snprintf(file,sizeof(file),"system-details-%d.bmp",i);dump_native_bmp(file);}
 launch(&game);game.dock_stage=0;game.encounter_kind=ENCOUNTER_NONE;game.voice_time=game.message_time=0;night_open=comms_encounter_conversation=0;change_page(COMMS_PANEL);row=1;
 float clock=game.time;input(PSP_CTRL_RIGHT,0,.016f,0,0);MFCHECK(row==3&&page==COMMS_PANEL&&game.time==clock,"Contact Target Right selects Request Docking without activating or resuming");
 input(PSP_CTRL_LEFT,0,.016f,0,0);MFCHECK(row==1,"Request Docking Left returns directly to Contact Target");comms_panel();dump_native_bmp("comms-contact-neighbour.bmp");
 /* Jettison a real canister through the production menu, then recover it. */
 game_init(&game);story_complete(&game);game.opening.stage=OP_FREE;launch(&game);game.dock_stage=0;game.approach=-1;game.police_stop=0;game.planet=-1;game.pos=(Vec3){20000,4000,20000};game.speed=0;
 memset(game.debris,0,sizeof(game.debris));memset(game.cargo,0,sizeof(game.cargo));game.job_n=0;game.contract=-1;game.cargo[11]=2;page=MARKET;row=0;deck_reset();int money=game.credits,stock=game.stock[11];
 input(PSP_CTRL_SQUARE,0,.016f,0,0);int pod=-1,pods=0;for(int i=0;i<DEBRIS_COUNT;i++)if(game.debris[i].alive){pod=i;pods++;}
 MFCHECK(game.cargo[11]==1&&pods==1&&pod>=0&&game.debris[pod].good==11&&game.debris[pod].qty==1&&game.credits==money&&game.stock[11]==stock,"Square jettisons exactly one selected consignment into a real pod without selling it");
 MFCHECK(pod>=0&&length(sub(game.debris[pod].pos,game.pos))>player_ships[game.ship].collision_radius,"jettison pod spawns outside the ship hull");market_screen();dump_native_bmp("cargo-jettison.bmp");
 int recovered=0;if(pod>=0){game.pos=game.debris[pod].pos;recovered=salvage(&game,DEBRIS_ID_MIN+pod);for(int i=0;i<240;i++)game_tick(&game,1.f/60,0,0,0,0);recovered&=!game.debris[pod].alive&&game.cargo[11]==2;}
 MFCHECK(recovered,"jettisoned cargo can be recovered through the real tractor loop");
 game.docked=1;int qty=game.cargo[11];input(PSP_CTRL_SQUARE,0,.016f,0,0);MFCHECK(game.cargo[11]==qty,"jettison cannot discard cargo while docked");game.docked=0;
 for(int i=0;i<DEBRIS_COUNT;i++)game.debris[i].alive=1;input(PSP_CTRL_SQUARE,0,.016f,0,0);MFCHECK(game.cargo[11]==qty,"full debris pool cannot lose player cargo");memset(game.debris,0,sizeof(game.debris));
 memset(game.cargo,0,sizeof(game.cargo));game.cargo[0]=1;game.job_n=1;memset(&game.jobs[0],0,sizeof(Job));game.jobs[0].type=MISSION_DELIVERY;game.jobs[0].item=1;game.jobs[0].dest=7;game.jobs[0].time=600;row=0;
 input(PSP_CTRL_SQUARE,0,.016f,0,0);MFCHECK(game.cargo[0]==1,"mission-reserved delivery cargo cannot be jettisoned");game.job_n=0;game.cargo[0]=1;input(PSP_CTRL_SQUARE,0,.016f,0,0);
 MFCHECK(game.cargo[0]==0&&row==0&&cargo_item(row)<0,"jettisoning the final consignment safely clamps the empty list");
 game.message_time=5;strcpy(game.message,"Old notice");input(PSP_CTRL_LEFT,0,.016f,0,0);MFCHECK(game.message_time==0,"in-flight Left/Right produce no market-controls popup");
 game.docked=1;game.system=7;market(&game);game.credits=100000;game.stock[0]=5;row=0;qty=game.cargo[0];input(PSP_CTRL_RIGHT,0,.016f,0,0);int bought=game.cargo[0]==qty+1&&!game.message_time;input(PSP_CTRL_LEFT,0,.016f,0,0);
 MFCHECK(bought&&game.cargo[0]==qty&&!game.message_time,"station buy/sell retains actual trading without Bought/Sold popups");
 /* Highest bank index: Square must act on the selected slot, never bank one. */
 game.ship=14;fit_clear_all(&game);memset(game.module_store,0,sizeof(game.module_store));game.fit[FIT_NAV+12]=13;fit_rebuild(&game);change_page(INVENTORY);row=FIT_NAV+12;money=game.credits;preview_time=18;loadout_store_until=-1;inventory_screen();dump_native_bmp("loadout-nav-alignment.bmp");
 input(PSP_CTRL_SQUARE,0,.016f,0,0);MFCHECK(game.fit[FIT_NAV+12]==FIT_EMPTY&&game.module_store[13]==1&&game.credits==money,"Square stores the selected NAV bank three module exactly once without selling it");
 MFCHECK(loadout_store_until>preview_time&&strstr(loadout_store_note,"Item Storage"),"loadout gives visible bounded confirmation of storing the module");inventory_screen();dump_native_bmp("loadout-store-feedback.bmp");
 game.docked=0;game.fit[FIT_NAV+12]=13;fit_rebuild(&game);input(PSP_CTRL_SQUARE,0,.016f,0,0);MFCHECK(game.fit[FIT_NAV+12]==13&&strstr(loadout_store_note,"Dock"),"flight loadout explains why storing fitted equipment requires docking");
 loadout_store_until=-1;game.docked=1;row=FIT_NAV+12;inventory_screen();
 unsigned note_before=0,note_after=0;for(int y=249;y<268;y++)for(int x=8;x<472;x++)note_before=note_before*33+fb[y*STRIDE+x];
 lc_base("SHIP LOADOUT");lc_hint(14,'P',"SLOT");lc_hint(105,'S',"STORE");lc_hint(225,'T',"ITEM STORAGE");lc_hint(396,'O',"BACK");
 for(int y=249;y<268;y++)for(int x=8;x<472;x++)note_after=note_after*33+fb[y*STRIDE+x];
 MFCHECK(note_before==note_after,"actual loadout footer contains only D-pad, Square Store, Triangle Item Storage and Circle Back");
 int guards=1;for(int i=0;i<16;i++)guards&=pixels[i]==0x13579adfu&&pixels[STRIDE*H+16+i]==0x13579adfu;
 MFCHECK(guards,"all affected native menu renders preserve framebuffer guard words");
 fprintf(out,"RESULT %d failures\n",failures);fclose(out);fb=old;free(pixels);
#undef MFCHECK
 return 1;
}
