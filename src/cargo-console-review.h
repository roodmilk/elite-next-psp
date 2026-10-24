static int cargo_console_review(void){
 FILE *flag=fopen("cargo-console-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *report=fopen("cargo-console-review.txt","w");if(!report)return 1;
 unsigned *oldfb=fb,*pixels=malloc(STRIDE*H*sizeof(unsigned));int failures=0;
 if(!pixels){fprintf(report,"RESULT 1 failures\n");fclose(report);return 1;}fb=pixels;
#define CARGOCHECK(c,label) do{int ok=!!(c);fprintf(report,"%s %s\n",ok?"PASS":"FAIL",label);failures+=!ok;fflush(report);}while(0)
 game_init(&game);story_complete(&game);game.opening.stage=0;page=MARKET;preview_time=20;row=0;
 int descriptions=1,icons=1,seen[GOODS]={0};for(int i=0;i<GOODS;i++){descriptions&=cargo_description(i,0,WHITE)>0&&cargo_description(i,0,WHITE)<=5;int ink=0;for(int y=0;y<9;y++)ink|=cargo_symbols[i][y];icons&=ink!=0;seen[cargo_order[i]]++;}
 CARGOCHECK(descriptions,"all 17 complete descriptions fit their native five-line aperture");CARGOCHECK(icons,"every commodity has an icon authored at its actual small display size");int unique=1;for(int i=0;i<GOODS;i++)unique&=seen[i]==1;CARGOCHECK(unique,"commodity order contains every real commodity exactly once");
 memset(game.cargo,0,sizeof(game.cargo));game.cargo[0]=8;game.cargo[1]=3;game.cargo[11]=4;game.cargo[8]=2;game.cargo[9]=5;game.cargo[12]=4;game.credits=12500;
 int best=100000;for(int i=0;i<player_ship_count;i++){int distance=abs(player_ships[i].capacity-105);if(distance<best){best=distance;game.ship=i;}}fit_clear_all(&game);fit_rebuild(&game);
 game.job_n=2;for(int i=0;i<2;i++){game.jobs[i].type=MISSION_DELIVERY;game.jobs[i].dest=7;game.jobs[i].origin=7;game.jobs[i].time=600;game.jobs[i].target=-1;game.jobs[i].item=1;game.jobs[i].reward=500;}
 game.docked=0;row=2;rect(0,0,W,H,BG);market_screen();dump_native_bmp("cargo-hold-reference.bmp");
 CARGOCHECK(cargo_rows()==6&&cargo_item(row)==11&&cargo_used(&game)==26&&mission_cargo_reserved(&game,0)==2,"hold overview uses actual cargo amounts, Furs selection and reserved delivery crates");
 int before=game.credits,qty=game.cargo[11];input(PSP_CTRL_RIGHT,0,.016f,0,0);input(PSP_CTRL_LEFT,0,.016f,0,0);
 CARGOCHECK(game.credits==before&&game.cargo[11]==qty,"in-flight overview cannot trade through either direction");
 input(PSP_CTRL_DOWN,0,.016f,0,0);CARGOCHECK(row==3&&cargo_item(row)==8,"D-pad selects the neighbouring displayed commodity");input(PSP_CTRL_UP,0,.016f,0,0);
 int poor=-1;
 for(int s=0;s<256;s++)if(station_class_for_system(&game,s)==STATION_POOR&&(s&1)==0){poor=s;break;}
 if(poor<0)for(int s=0;s<256;s++)if(station_class_for_system(&game,s)==STATION_POOR){poor=s;break;}
 game.system=poor;game.docked=1;market(&game);row=2;
 rect(0,0,W,H,BG);market_screen();dump_native_bmp("cargo-exchange-reference.bmp");
 int hidden=-1;for(int i=0;i<GOODS;i++)if(!station_market_item_visible(&game,i)){hidden=i;break;}
 game.cargo[hidden]=3;int hiddenrow=-1;for(int i=0;i<cargo_rows();i++)if(cargo_item(i)==hidden)hiddenrow=i;
 CARGOCHECK(poor>=0&&hiddenrow>=0&&!cargo_can_trade(hidden),"held goods remain visible at a port that does not trade them");
 row=hiddenrow;before=game.credits;qty=game.cargo[hidden];int stock=game.stock[hidden];input(PSP_CTRL_RIGHT,0,.016f,0,0);input(PSP_CTRL_LEFT,0,.016f,0,0);
 CARGOCHECK(game.credits==before&&game.cargo[hidden]==qty&&game.stock[hidden]==stock,"NOT TRADED rows reject both buy and sell without changing cargo, credits or stock");
 rect(0,0,W,H,BG);market_screen();dump_native_bmp("cargo-exchange-not-traded.bmp");
 game.system=7;market(&game);game.job_n=0;game.contract=-1;row=0;game.credits=100000;game.stock[0]=8;before=game.credits;qty=game.cargo[0];int price=game.price[0];input(PSP_CTRL_RIGHT,0,.016f,0,0);
 CARGOCHECK(game.credits==before-price&&game.cargo[0]==qty+1&&game.stock[0]==7,"Right buys the selected listed commodity at its exact displayed local price");
 input(PSP_CTRL_LEFT,0,.016f,0,0);CARGOCHECK(game.credits==before&&game.cargo[0]==qty&&game.stock[0]==8,"Left sells it at the same real local price and updates stock");
 game.job_n=1;game.jobs[0].type=MISSION_DELIVERY;game.cargo[0]=1;before=game.credits;input(PSP_CTRL_LEFT,0,.016f,0,0);CARGOCHECK(game.cargo[0]==1&&game.credits==before,"reserved mission cargo remains protected from selling");game.job_n=0;
 row=cargo_rows()-1;input(PSP_CTRL_DOWN,0,.016f,0,0);CARGOCHECK(row==0,"exchange selection wraps from the last page to the first");input(PSP_CTRL_UP,0,.016f,0,0);CARGOCHECK(row==cargo_rows()-1,"Up wraps back to the final exchange page");
 rect(0,0,W,H,BG);market_screen();dump_native_bmp("cargo-exchange-last-page.bmp");
 for(int i=0;i<GOODS;i++){for(row=0;row<cargo_rows()&&cargo_item(row)!=i;row++);rect(0,0,W,H,BG);market_screen();char name[48];snprintf(name,sizeof(name),"cargo-commodity-%02d.bmp",i);dump_native_bmp(name);}
 row=0;game.credits=100000000;game.cargo[0]=100000;game.cargo[13]=100000;game.stock[0]=100000;rect(0,0,W,H,BG);market_screen();dump_native_bmp("cargo-large-values.bmp");CARGOCHECK(game.cargo[0]==100000&&game.credits==100000000,"rendering maximum numeric values does not alter inventory or credits");
 memset(game.cargo,0,sizeof(game.cargo));game.docked=0;row=9;market_screen();CARGOCHECK(row==0&&cargo_rows()==1&&cargo_item(row)==-1,"empty hold clamps stale selection and exposes no invalid commodity");dump_native_bmp("cargo-empty.bmp");
 game.cargo[13]=4;game.cargo[15]=9;market_screen();CARGOCHECK(cargo_used(&game)==0&&cargo_rows()==2,"kilogram and gram consignments keep their real units and consume no tonne slots");dump_native_bmp("cargo-precious-goods.bmp");
 input(PSP_CTRL_CIRCLE,0,.016f,0,0);CARGOCHECK(page!=MARKET,"Circle returns from cargo through the existing menu route");
 fprintf(report,"RESULT %d failures\n",failures);fclose(report);fb=oldfb;free(pixels);
#undef CARGOCHECK
 return 1;
}
