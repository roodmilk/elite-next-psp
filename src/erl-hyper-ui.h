/* Selected Hyper Circuit design. Actual domain values and PSP-sized art. */
static void erl_headline(int x,int y,int width,const char *name){
 int len=strlen(name);if(len*24<=width){erl_text(x,y,EC,name,3);return;}
 if(len*16<=width){erl_text(x,y+4,EC,name,2);return;}
 int cols=width/16,split=cols;while(split>0&&name[split]!=' ')split--;if(!split){erl_fit(x,y+10,width,EC,name);return;}
 char first[32];snprintf(first,sizeof(first),"%.*s",split,name);erl_text(x,y,EC,first,2);erl_text(x,y+18,EC,name+split+1,2);
}
static void erl_home_draw(void){
 ErlCareer *e=&game.erl;erl_header("ERL / HEADQUARTERS",game.systems[game.system].name);erl_preview(8,32,464,105,erl_layout());
 erl_chamfer(334,39,128,92,RGB(16,20,28),ET);erl_text(378,47,EC,"ROUND",1);char round[8];snprintf(round,8,"%02d",e->championship?e->round+1:1);erl_text(366,65,ER,round,4);erl_flag(388,105,5);
 erl_chamfer(8,96,292,43,EP,ER);const char *name=!e->referral?"EARN YOUR REFERRAL":!e->licence?"ROOKIE QUALIFIER":erl_course_at(&game,e->championship?erl_round_venue(&game,erl_layout()):game.system,erl_layout());erl_headline(20,strlen(name)>16?101:108,268,name);
 erl_chamfer(8,147,226,65,EP,ER);if(strlen(game.commander_name)<=12)erl_text(20,157,EC,game.commander_name,2);else erl_fit(20,161,202,EC,game.commander_name);erl_icon(ERLI_SHIP,20,180);erl_fit(61,185,161,ED,erl_racers[erl_racer_id(&game,0)].name);erl_fit(20,201,202,GOLD,e->championship?"SEASON SHIP LOCKED":e->module_pending?"REWARD READY":"TRIANGLE: CHOOSE RACER");
 erl_chamfer(242,147,230,65,RGB(16,20,28),ET);erl_text(254,156,ET,e->licence?erl_division_name(e->licence-1):"UNLICENSED",1);erl_text(254,170,ED,"LEAGUE XP",1);char progress[40];snprintf(progress,40,e->licence>=4?"%d":"%d/%d",e->xp,erl_xp_goal());erl_fit(386,170,74,EC,progress);erl_xp_bar(254,184,206);
 if(!e->licence)erl_text(254,201,EC,"WIN ROOKIE QUALIFIER",1);else if(e->licence>=4)erl_text(254,201,EC,"TOP LEAGUE",1);else if(erl_xp_remaining())text_px(254,201,EC,"%d XP TO NEXT LEVEL",erl_xp_remaining());else erl_text(254,201,EC,"NEXT QUALIFIER READY",1);
 erl_text(16,224,ED,"CUP PRIZES",1);erl_icon(ERLI_UNITS,112,216);erl_text(141,224,GOLD,"UNITS",1);erl_icon(ERLI_XP,202,216);erl_text(231,224,ET,"XP",1);erl_icon(ERLI_SHIP,273,216);erl_fit(310,224,64,ER,player_ships[erl_prizes[e->licence?e->licence-1:0]].name);erl_icon(ERLI_MODULE,379,216);erl_text(405,224,ET,"MODULE",1);
 if(e->championship)erl_button(102,214,124,"RACE SEASON",erl_choice==5,1);
 const char *buttons[]={"RACE","SEASON","DRIVER","RIVALS","GARAGE"};for(int i=0;i<5;i++)erl_button(8+i*94,240,88,buttons[i],erl_choice==i,1);
 if(e->module_pending||game.erl_local.pending_count){int x=e->championship?238:8,w=e->championship?234:464;rect(x,213,w,25,EB);button_icon(x+8,219,'S',ER);erl_text(x+29,224,EC,"CLAIM PRIZE MODULE",1);}
}
static void erl_garage_draw(void){
 ErlCareer *e=&game.erl;erl_header("ERL / GARAGE","SHARED FLEET");if(erl_garage_row>=e->fleet_n)erl_garage_row=0;ErlShip *s=&e->fleet[erl_garage_row];
 erl_headline(16,34,340,player_ships[s->hull].name);text_px(376,35,ET,"%02d / %02d",erl_garage_row+1,e->fleet_n);
 /* Platform rings are behind the actual ship mesh, never over the hull. */
 static short orbit[4][49][2];static int ready;if(!ready){for(int a=0;a<4;a++)for(int i=0;i<=48;i++){float t=i*.1309f;orbit[a][i][0]=240+cosf(t)*(142+a*9);orbit[a][i][1]=156+sinf(t)*(23+a*4);}ready=1;}
 for(int a=0;a<4;a++)for(int i=0;i<48;i++)line(orbit[a][i][0],orbit[a][i][1],orbit[a][i+1][0],orbit[a][i+1][1],a?planet_rgb_mix(ER,EB,145):ET);
 int light=(int)(preview_time*18)%48;if(light<0)light=0;for(int i=light;i<light+2&&i<48;i++)line(orbit[0][i][0],orbit[0][i][1],orbit[0][i+1][0],orbit[0][i+1][1],EC);
 unsigned old=ship_paint[s->hull];ship_paint[s->hull]=s->paint?s->paint:old;yc_paint_override=s->paint;yc_ship(s->hull,96,66,288,100);yc_paint_override=0;ship_paint[s->hull]=old;
 erl_text(15,119,ER,"<",2);erl_text(449,119,ER,">",2);
 rect(8,168,464,14,EB);text_px(16,172,ED,"CARGO %d>%dT  SPD %d>%d  HULL %d>%d",cargo_capacity(&game),player_ships[s->hull].capacity,player_ships[game.ship].speed,player_ships[s->hull].speed,player_ships[game.ship].hull,player_ships[s->hull].hull);
 int fitted=0;for(int i=0;i<48;i++)fitted+=s->fit[i]!=FIT_EMPTY;
 const int xs[]={8,126,244,362};const char *labels[]={"HULL","FUEL","MISSILES","FITTED"};char nums[4][24];snprintf(nums[0],24,"%.0f%%",s->health);snprintf(nums[1],24,"%.1f",s->fuel*.1f);snprintf(nums[2],24,"%d",s->missiles);snprintf(nums[3],24,"%d",fitted);
 for(int i=0;i<4;i++){erl_chamfer(xs[i],186,110,44,EP,ER);erl_text(xs[i]+10,194,ED,labels[i],1);erl_card_value(xs[i]+10,207,90,ET,nums[i]);}
 if(e->championship||e->event_kind){erl_button(8,240,464,"SHIP LOCKED",1,0);}
 else {erl_button(8,240,464,erl_garage_row==e->active_ship?"CURRENT SHIP":"SWAP SHIP",1,erl_garage_row!=e->active_ship);}
 if(erl_notice[0]){rect(8,172,464,10,EB);erl_fit(16,173,448,GOLD,erl_notice);}erl_footer(e->championship||e->event_kind?"LOCKED":"SWAP");
}
static void erl_season_draw(void){
 ErlCareer *e=&game.erl;erl_header("ERL / CHAMPIONSHIP",erl_division_name(e->licence?e->licence-1:0));
 erl_text(12,34,EC,"CHAMPIONSHIP",2);const int px[]={45,166,291,427},py[]={105,157,104,155};
 for(int i=1;i<4;i++){for(int d=-2;d<=2;d++)line(px[i-1],py[i-1]+d,px[i],py[i]+d,planet_rgb_mix(ER,EB,90));line(px[i-1],py[i-1],px[i],py[i],ET);}
 for(int i=0;i<4;i++){
  int active=e->championship&&e->round==i;erl_round_node(px[i],py[i],active?ER:EP,ET);char number[4];snprintf(number,sizeof(number),"%02d",i+1);erl_text(px[i]-16,py[i]-7,active?EB:EC,number,2);
  int venue=erl_round_venue(&game,i);const char *n=erl_course_at(&game,venue,i);int x=px[i]-(int)strlen(n)*4;if(x<8)x=8;if(x+(int)strlen(n)*8>472)x=472-strlen(n)*8;erl_text(x,py[i]+25,EC,n,1);int vx=px[i]-(int)strlen(game.systems[venue].name)*4;if(vx<8)vx=8;if(vx>384)vx=384;text_px(vx,py[i]+37,ET,"%s",game.systems[venue].name);
 }
 char prize[96];snprintf(prize,sizeof(prize),"WIN THE CUP / %s + ERL MODULE",player_ships[erl_prizes[e->licence?e->licence-1:0]].name);erl_fit(16,209,448,ET,prize);
 ErlLocalSeason *offer=&game.erl_local.season[game.system];if(offer->prize_state){rect(8,205,464,32,EB);erl_fit(16,207,448,ET,equipment_names[offer->prize.base]);text_px(16,222,EC,"OUTPUT %u.%u%%  SECONDARY %u.%u%%",offer->prize.power/10,offer->prize.power%10,offer->prize.secondary/10,offer->prize.secondary%10);}
 erl_button(8,240,228,e->championship?"STANDINGS":"ENTER SEASON",!erl_choice,e->licence>0);erl_button(244,240,228,"WITHDRAW",erl_choice,e->championship);erl_footer(e->championship?"STANDINGS":"ENTER");
}
static void erl_standings_draw(void){
 ErlCareer *e=&game.erl;erl_header("ERL / STANDINGS",erl_division_name(e->division));text_px(12,34,ET,"ROUND %d / 4",e->round);erl_text(330,34,ED,"PTS  WINS",1);
 int order[7]={0,1,2,3,4,5,6};for(int i=0;i<7;i++)for(int j=i+1;j<7;j++)if(erl_standing_better(&game,order[j],order[i])){int t=order[i];order[i]=order[j];order[j]=t;}
 for(int i=0;i<7;i++){int id=order[i],y=51+i*24;erl_chamfer(8,y,464,22,id?EP:ER,id?planet_rgb_mix(ET,EB,140):ET);unsigned ink=id?EC:EB;text_px(18,y+7,ink,"%02d",i+1);erl_fit(54,y+7,262,ink,erl_driver_name(id));text_px(336,y+7,ink,"%d",e->points[id]);text_px(424,y+7,ink,"%d",e->round_wins[id]);}
 text_px(16,218,ED,"TIES: PLACE COUNTBACK > FINAL ROUND");
 if(e->round<4)text_px(16,230,ET,"NEXT: %s",erl_course_name(&game,e->round));else text_px(16,230,GOLD,"SEASON COMPLETE / PLACED %d OF 7",e->season_place);erl_footer("CONTINUE");
}
static void erl_driver_draw(void){
 ErlCareer *e=&game.erl;erl_header("ERL / DRIVER",game.systems[game.system].name);erl_headline(12,34,376,game.commander_name);erl_flag(422,33,7);
 erl_chamfer(8,77,177,134,EP,ER);erl_text(22,92,EC,e->licence?erl_division_name(e->licence-1):"UNLICENSED",1);char xp[24];snprintf(xp,sizeof(xp),"%d",e->xp);erl_text(22,116,ET,xp,strlen(xp)<=4?4:strlen(xp)<=9?2:1);erl_text(22,171,EC,"LEAGUE XP",1);erl_xp_bar(20,154,149);
 if(!e->licence)erl_text(22,190,ED,"QUALIFIER FIRST",1);else if(e->licence>=4)erl_text(22,190,ED,"TOP LEAGUE",1);else text_px(22,190,ED,"%d XP TO NEXT",erl_xp_remaining());
 const char *labels[]={"WINS","PODIUMS","TAKEDOWNS","TROPHIES"};int values[]={e->wins,e->podiums,e->takedowns,e->trophies};
 for(int i=0;i<4;i++){int x=201+(i%2)*136,y=88+(i/2)*62;char n[24];snprintf(n,sizeof(n),"%d",values[i]);erl_text(x,y,ER,n,strlen(n)>5?1:strlen(n)>3?2:3);erl_text(x,y+31,ET,labels[i],1);}
 text_px(201,201,ET,"RIVALS BEATEN %u",game.erl_local.progress.rivals_beaten);
 {ErlProgress *p=&game.erl_local.progress;const char *title=p->milestones&4?"WRECKER":p->milestones&2?"GRID ACE":p->milestones&1?"RISING STAR":"NEW CHALLENGER";text_px(16,212,ET,"%s",title);if(p->records){float best=p->best[p->latest_mode][p->latest_objective];if(best>0)text_px(240,212,ED,"BEST IN MODE %.2f S",best);}}
 erl_chamfer(8,220,464,27,EP,planet_rgb_mix(ER,EB,60));erl_text(16,226,GOLD,"NEXT LEAGUE QUALIFIER",1);
 if(!e->licence)erl_text(16,237,ED,"WIN THE ROOKIE QUALIFIER TO EARN YOUR LICENCE",1);else if(e->licence>=4)erl_text(16,237,ED,"ELITE LICENCE EARNED",1);else if(erl_next_qualifier(&game))erl_text(16,237,ET,"READY / WIN THE QUALIFIER TO ADVANCE",1);else text_px(16,237,ED,"%d XP TO UNLOCK ENTRY",erl_xp_remaining());erl_footer("RECORD");
}
static void erl_brief_draw(void){
 ErlCareer *e=&game.erl;int mechanic=erl_screen==ERL_MECHANIC;
 int kind=mechanic?ERL_INTRO:e->event_kind?e->event_kind:erl_requested_kind?erl_requested_kind:ERL_QUALIFIER;
 int layout=e->event_kind?e->event_layout:kind==ERL_OFFICIAL?e->round:erl_requested_layout,type=e->event_kind?e->event_type:mechanic?ERL_SPRINT:kind==ERL_OFFICIAL?erl_discipline(layout):erl_requested_type;
 RaceRules q=e->event_kind||erl_board_brief?game.race.arcade.rules:erl_entry_rules(&game,kind,layout,type);
 erl_header(mechanic?"INTRODUCTION TO THE ERL":"ERL / RACE BRIEF",q.mode==RA_COMBAT?q.expert?"EXPERT / RINGS":"COMBAT":q.mode==RA_TIME_TRIAL?"SOLO":"CLEAN");
 erl_preview(8,32,464,110,layout);erl_chamfer(8,100,280,44,EP,ER);
 rect(296,101,176,28,EB);text_px(306,105,ET,"ALLOWED ITEMS");const char *pool=q.pool==RA_POOL_ALL?"ALL FOUR":q.pool==RA_POOL_CLEAN?"BOOST + SHIELD":q.pool==(1u<<RA_BOOST)?"BOOST":q.pool==(1u<<RA_MINIGUN)?"MINIGUN":q.pool==((1u<<RA_MISSILE)|(1u<<RA_SHIELD))?"MISSILE + SHIELD":"RESTRICTED";text_px(306,117,EC,"%s",pool);
 const char *name=mechanic?"TORR'S TEST":erl_course_at(&game,e->event_kind?e->event_system:kind==ERL_OFFICIAL?erl_round_venue(&game,layout):game.system,layout);erl_card_value(20,111,256,EC,name);
 int venue=e->event_kind?e->event_system:kind==ERL_OFFICIAL?erl_round_venue(&game,layout):game.system;if(kind==ERL_OFFICIAL)text_px(306,132,ET,"%s",game.systems[venue].name);
 char qualifier[32];int next=erl_next_qualifier(&game);snprintf(qualifier,32,"%s QUALIFIER",erl_division_name(next>0?next-1:0));erl_fit(20,132,256,ET,kind==ERL_QUALIFIER&&!erl_board_brief?qualifier:erl_rule_name(&q));
 char values[3][16];snprintf(values[0],16,"%d",q.field);snprintf(values[1],16,"%d",q.objective==RA_TAKEDOWNS?q.required:q.objective==RA_ENDURANCE?q.seconds/60:q.laps);snprintf(values[2],16,"%s",q.objective==RA_MEDALS?"GOLD":q.objective==RA_TAKEDOWNS?"K.O.":"1ST");
 const char *labels[]={"RACERS",q.objective==RA_TAKEDOWNS?"TARGETS":q.objective==RA_ENDURANCE?"MINUTES":"LAPS",q.objective==RA_MEDALS?"MEDAL":q.objective==RA_TAKEDOWNS?"SCORE":"WINS"};
 if(erl_board_brief){int j=race_job(&game);if(j>=0){int target=game.jobs[j].target;snprintf(values[2],16,"%s",target==40?"TOP3":target==41?"BEAT":target==42?"GOLD":target==43?"2KO":target==44?"5KO":target==46?"HOLD":"1ST");labels[2]="CONTRACT";}}
 for(int i=0;i<3;i++){int x=20+i*92;erl_icon(i==0?ERLI_SHIPS:i==1?(q.objective==RA_TAKEDOWNS?ERLI_TARGET:q.objective==RA_ENDURANCE?ERLI_CLOCK:ERLI_LAPS):ERLI_FLAG,x,157);erl_text(x+31,157,EC,values[i],strlen(values[i])>3?1:2);erl_text(x,185,ED,labels[i],1);}
 erl_chamfer(296,151,176,64,RGB(16,20,28),ET);erl_text(308,159,ET,kind==ERL_PRACTICE&&!erl_board_brief?"PRACTICE":"WIN REWARDS",1);erl_icon(ERLI_UNITS,307,177);
 int job=race_job(&game),reward=erl_board_brief&&job>=0?game.jobs[job].reward:kind==ERL_PRACTICE?0:kind==ERL_INTRO?5000:kind==ERL_QUALIFIER?8000:12000*(e->division+1);
 char units[24];snprintf(units,24,"%d",reward/10);erl_card_value(336,180,64,GOLD,units);erl_text(408,184,GOLD,"U",1);text_px(337,201,ET,"+%d XP",kind==ERL_PRACTICE||erl_board_brief?0:180+e->division*50);
 const char *goal=q.objective==RA_TAKEDOWNS?"DESTROY DRONES BEFORE TIME EXPIRES.":q.objective==RA_ELIMINATOR?"LAST PLACE ELIMINATED EVERY 20 SEC.":q.objective==RA_HUNTER?"TAKE THE LEAD. HOLD IT FOR 4 SEC.":q.objective==RA_ESCAPE?"ESCAPE. DO NOT LOSE THE LEAD.":q.objective==RA_MEDALS?"BEAT THE GOLD TIME. SOLO RUN.":q.course==RA_CORRIDOR?"FOLLOW THE ENERGY CORRIDOR. WIN.":"PASS EVERY GATE IN ORDER.";
 if(erl_board_brief&&job>=0)goal=race_contract_goal(&game.jobs[job]);erl_fit(20,209,268,EC,goal);
 if(q.objective==RA_MEDALS)text_px(20,224,ET,"G %.1f  S %.1f  B %.1f SEC",q.gold,q.silver,q.bronze);
 else if(q.sponsor){const char *task=q.sponsor==1?"75% HULL":q.sponsor==2?"2 TAKEDOWNS":q.sponsor==3?"3 BOOST RINGS":"CLEAN WIN";const char *bonus=erl_sponsor_module(&q)?erl_sponsor_room(&game)?"400U + MODULE":"800U / QUEUE FULL":"400U";text_px(20,224,GOLD,"SPONSOR: %s / %s",task,bonus);}
 erl_button(8,240,306,(e->event_kind&&race_ready(&game)&&game.race.phase==RACE_GRID&&!game.docked&&length(sub(game.pos,game.race.start))<5000)||erl_board_brief?"READY":"SET START BEACON",!erl_choice,erl_board_brief||e->event_kind||mechanic?1:e->referral>0);erl_button(322,240,150,"NOT NOW",erl_choice,1);
}
/* Reference-matched native event and paddock screens. */
static void erl_picker_footer(const char *action){
 rect(0,253,480,19,EB);line(0,253,479,253,ET);
 for(int i=0;i<5;i++){line(11+i,265-i,11+i,265+i,ET);line(25-i,265-i,25-i,265+i,ET);}
 erl_text(35,261,ET,"CHOOSE",1);line(183,261,189,267,ER);line(189,261,183,267,ER);erl_text(202,261,EC,action,1);
 line(405,261,410,261,ER);line(403,263,403,266,ER);line(412,263,412,266,ER);line(405,268,410,268,ER);erl_text(423,261,ET,"BACK",1);
}
static void erl_picker_header(const char *title,const char *right){
 erl_header(title,right);erl_text(29,8,EC,title,1);line(0,24,479,24,ER);
}
static void erl_rivals_draw(void){
 erl_picker_header("RIVALS","PADDOCK CHANNEL");int id=erl_choice+1;
 /* One integer scanline per row keeps the diagonal exact and inexpensive. */
 rect(0,25,480,228,EB);for(int y=25;y<188;y++)rect(0,y,350-(y-25)*72/163,1,EP);
 const char *name=erl_driver_name(id),*split=strchr(name,' ');char first[32];int n=split?(int)(split-name):strlen(name);snprintf(first,sizeof(first),"%.*s",n,name);
 erl_text(16,40,EC,first,strlen(first)<=12?3:2);erl_text(16,70,ER,split?split+1:"",split&&strlen(split+1)<=12?3:2);
 const char *role=id%3==1?"CONTACT RACER":id%3==2?"CLEAN DEFENDER":"TECHNICAL PILOT";
 erl_text(16,111,ET,role,1);int beaten=!!(game.erl_local.progress.beaten[game.system]&(1u<<(id-1)));if(id==1)for(int sys=0;sys<256;sys++)beaten|=game.erl_local.progress.beaten[sys]&1;erl_text(16,160,beaten?GOLD:ED,beaten?"BEATEN ON THE GRID":"NO WIN RECORDED",1);erl_text(16,135,EC,"\"SEE YOU ON THE GRID.\"",1);
 portrait_draw(367,37,96,96,erl_portrait(id),TRADERS);line(367,37,462,37,ET);line(367,132,462,132,ET);line(367,37,367,132,ET);line(462,37,462,132,ET);
 erl_chamfer(277,148,186,32,ER,ER);erl_text(318,160,EB,"CONTACT RACER",1);
 for(int i=0;i<6;i++){int x=7+(i%3)*158,y=191+(i/3)*25;erl_chamfer(x,y,148,22,i==erl_choice?ER:EB,i==erl_choice?ER:ET);const char *label=erl_driver_name(i+1);int len=strlen(label);erl_fit(x+(len<=17?(148-len*8)/2:6),y+7,len<=17?len*8:136,i==erl_choice?EB:EC,label);}
 erl_picker_footer("CONTACT RACER");
}
static void erl_events_draw(void){
 erl_picker_header("EVENTS","FREE ENTRY");rect(0,25,480,228,EB);
 erl_preview(194,27,208,159,erl_choice);
 /* Hide preview chrome beneath the diagonal and use an unframed scene. */
 line(194,27,401,27,EB);line(401,27,401,185,EB);line(194,185,401,185,EB);
 for(int y=27;y<186;y++){int edge=240-(y-27)*61/159;rect(0,y,edge,1,EP);rect(edge,y,13,1,ER);}
 erl_text(16,44,EC,"CHOOSE",2);erl_text(16,66,EC,"YOUR RACE",2);
 const char *title[]={"NEXT QUALIFIER","PRACTICE","ARCADE EVENTS"};
 const char *sub[]={"WIN TO PROMOTE","LEARN THE CIRCUIT","UNITS + XP"};RaceRules selected=erl_catalogue_rules(station_profile_hash(game.system,813),game.erl.licence,game.erl_local.selection);
 erl_text(15,107,ER,title[erl_choice],1);erl_fit(15,125,183,ET,erl_choice?erl_rule_name(&selected):sub[0]);if(erl_choice)text_px(15,172,ET,"L / R  EVENT %02d",game.erl_local.selection+1);
 int ready=erl_next_qualifier(&game);char xp[24];
 if(!erl_choice&&!ready&&game.erl.licence<4){snprintf(xp,sizeof(xp),"%d",erl_xp_remaining());erl_text(15,145,EC,xp,2);int tx=15+strlen(xp)*16+8;erl_text(tx,145,ET,"XP",1);erl_text(tx,155,EC,"TO UNLOCK",1);erl_xp_bar(15,173,152);}
 else {erl_text(15,148,EC,erl_choice==1?"FREE FLIGHT":erl_choice==2?"UNITS + XP":ready?"READY TO RACE":"ELITE LICENCE",1);if(!erl_choice)erl_xp_bar(15,173,152);}
 for(int i=0;i<3;i++){int x=7+i*158,selected=i==erl_choice;erl_chamfer(x,193,148,52,selected?ER:EB,selected?ER:ET);int len=strlen(title[i]);erl_text(x+(148-len*8)/2,204,selected?EB:EC,title[i],1);const char *label=i==0?(ready?"UNLOCKED":game.erl.licence>=4?"COMPLETE":"LOCKED"):i==1?"FREE FLIGHT":"UNITS + XP";erl_text(x+(148-(int)strlen(label)*8)/2,225,selected?EB:ET,label,1);}
 erl_picker_footer("OPEN");
}
