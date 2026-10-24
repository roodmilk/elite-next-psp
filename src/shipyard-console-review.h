/* Opt-in checks use the production renderer, catalogue and input dispatcher. */
static unsigned yc_review_hash(int x,int y,int w,int h){unsigned hash=2166136261u;for(int yy=y;yy<y+h;yy++)for(int xx=x;xx<x+w;xx++)hash=(hash^fb[yy*STRIDE+xx])*16777619u;return hash;}
/* Compare the Engineer's actual ship pixels against the catalogue renderer.
 * Names, backgrounds or diagnostic text cannot satisfy this comparison. */
static int yc_engineer_matches(unsigned *reference){
 unsigned *actual=fb,blank=RGB(251,1,253);fb=reference;rect(0,0,W,H,blank);
 yc_ship(game.ship,24,91,246,64);fb=actual;int pixels=0,ok=1;
 for(int y=0;y<H;y++)for(int x=0;x<W;x++)if(reference[y*STRIDE+x]!=blank){
  pixels++;ok&=x>=24&&x<270&&y>=91&&y<155&&reference[y*STRIDE+x]==actual[y*STRIDE+x];
 }
 return ok&&pixels>100;
}
static int shipyard_console_review(void){
 FILE *flag=fopen("shipyard-console-review.flag","r");if(!flag)return 0;fclose(flag);FILE *out=fopen("shipyard-console-review.txt","w");if(!out)return 1;
 int failures=0;unsigned *oldfb=fb,*pixels=malloc(STRIDE*H*sizeof(unsigned));if(!pixels){fprintf(out,"RESULT 1 failures\n");fclose(out);return 1;}fb=pixels;
#define YCHECK(c,label) do{int ok=!!(c);fprintf(out,"%s %s\n",ok?"PASS":"FAIL",label);failures+=!ok;fflush(out);}while(0)
 game_init(&game);story_complete(&game);game.opening.stage=0;game.docked=1;game.system=7;game.station_variant=0;game.ship=10;fit_clear_all(&game);game.credits=7500000;deck_reset();change_page(YARD);row=16;preview_time=12;shipyard_stats_view=1;
 unsigned *reference=malloc(STRIDE*H*sizeof(unsigned));
 YCHECK(reference!=NULL,"Engineer reference framebuffer allocates within the review budget");
 if(reference){
  int models=1,livery=1,unchanged=1,clipping=1;Game before_draw;
  for(int ship=0;ship<player_ship_count;ship++)for(int angle=0;angle<3;angle++){
   game.ship=ship;preview_time=angle*17.f;memcpy(&before_draw,&game,sizeof(game));repair_screen();
   unchanged&=!memcmp(&before_draw,&game,sizeof(game));models&=yc_engineer_matches(reference);
   clipping&=drawcount==0&&clipx0==0&&clipx1==W&&clipy0==-1&&clipy1==-1;
   if(angle==0&&(ship==0||ship==4||ship==10||ship==21)){char path[48];snprintf(path,sizeof(path),"engineer-hull-%02d.bmp",ship);dump_native_bmp(path);}
  }
  YCHECK(models,"Engineers shows the exact Shipyard hull pixels for every regular and Mega ship at three angles");
  for(int ship=0;ship<2;ship++){
   game.ship=ship?21:4;unsigned paint=ship_paint[game.ship];preview_time=12;
   for(int finish=0;finish<DECORATOR_COUNT;finish++){ship_paint[game.ship]=decorator_finishes[finish];repair_screen();livery&=yc_engineer_matches(reference);}
   ship_paint[game.ship]=paint;
  }
  YCHECK(livery,"Engineers uses current ship paint across every finish on regular and Mega hulls");
  YCHECK(unchanged&&clipping,"Engineer drawing preserves commander data and restores viewport clipping and triangle queues");
  game.ship=4;unsigned paint=ship_paint[4];ship_paint[4]=decorator_finishes[13];preview_time=12;repair_screen();dump_native_bmp("engineer-painted-ship.bmp");ship_paint[4]=paint;
  game.ship=0;page=REPAIR;game.hull=55;game.energy=45;game.heat=40;game.damaged=1;game.fuel=player_ships[0].range-4;game.message_time=0;
  int repair_quote=ship_repair_cost(&game),funds=game.credits;input(PSP_CTRL_CROSS,0,.016f,0,0);
  YCHECK(game.credits==funds-repair_quote&&game.hull==100&&game.energy==100&&game.heat==0&&!game.damaged,"Engineers X still repairs the ship for the quoted price");
  int fuel_quote=engineer_fuel_cost();funds=game.credits;input(PSP_CTRL_TRIANGLE,0,.016f,0,0);
  YCHECK(game.fuel==player_ships[0].range&&game.credits==funds-fuel_quote,"Engineers Triangle still refuels the current hull for the quoted price");
  free(reference);
 }
 /* Restore the original showroom fixture after Engineer service checks. */
 game_init(&game);story_complete(&game);game.opening.stage=0;game.docked=1;game.system=7;game.station_variant=0;game.ship=10;fit_clear_all(&game);game.credits=7500000;deck_reset();change_page(YARD);row=16;preview_time=12;shipyard_stats_view=1;
 yard();dump_native_bmp("crown-grand-stats.bmp");unsigned scene=yc_review_hash(8,65,464,108),cash=yc_review_hash(332,252,140,17),catalogue=yc_review_hash(8,181,176,62);
 YCHECK(pixels[199*STRIDE+277]==YC_UP&&pixels[199*STRIDE+437]==YC_UP,"higher cargo and range show green up arrows beside their own values");
 YCHECK(pixels[231*STRIDE+293]==YC_DOWN&&pixels[231*STRIDE+445]==YC_DOWN,"lower shields and heat capacity show red down arrows beside their own values");
 YCHECK(yc_delta(100,100)==0&&yc_delta(101,100)==1&&yc_delta(99,100)==-1,"comparison direction distinguishes better, worse and equal bare-hull stats");
 float time=game.time,speed=game.speed,heat=game.heat;int credit=game.credits;input(PSP_CTRL_SQUARE,0,.016f,0,0);yard();dump_native_bmp("crown-grand-description.bmp");
 YCHECK(page==YARD&&row==16&&!shipyard_stats_view,"Square switches the same selected ship from stats to description");
 YCHECK(scene==yc_review_hash(8,65,464,108)&&cash==yc_review_hash(332,252,140,17)&&catalogue==yc_review_hash(8,181,176,62),"Square changes only the reading panel and its footer hint");
 for(int i=0;i<90;i++)input(0,0,.016f,0,0);
 YCHECK(game.time==time&&game.speed==speed&&game.heat==heat&&game.credits==credit,"reading either shipyard view pauses simulation and leaves funds unchanged");
 int copy_ok=1,names_ok=1,price_ok=1,ship_names_ok=1;
 for(int ship=0;ship<player_ship_count;ship++){
  copy_ok&=yc_copy(200,192,33,7,8,shipyard_description[ship],0)>0;copy_ok&=yc_copy(16,202,56,4,10,shipyard_description[ship],0)>0;
  ship_names_ok&=strlen(player_ships[ship].name)*8<=144;
  for(int owned=0;owned<player_ship_count;owned++){game.ship=owned;char b[40];yc_exchange_label(ship,176,b,sizeof(b));price_ok&=strlen(b)*8<=176;}
 }
 YCHECK(copy_ok,"every full ship description fits both native reading layouts without clipping");
 YCHECK(ship_names_ok,"all 22 complete hull names fit the catalogue and selected-ship title");
 YCHECK(price_ok,"all 484 exchanges including large refunds fit the compact cost box exactly");
 char identities[256*HUB_COUNT][32];int identity_count=0;
 for(int sys=0;sys<256;sys++)for(int hub=0;hub<HUB_COUNT;hub++){
  char b[32],again[32];yc_name(sys,hub,b,sizeof(b));yc_name(sys,hub,again,sizeof(again));names_ok&=!strcmp(b,again)&&strlen(b)*16<=224;
  for(int i=0;i<identity_count;i++)names_ok&=strcmp(b,identities[i])!=0;snprintf(identities[identity_count++],32,"%s",b);
 }
 YCHECK(names_ok,"all 768 canonical dealer names are unique, deterministic and fit the large wordmark");
 int stock_ok=1,stock_unique=1,stock_n=0,art_unique=1;unsigned stock_masks[256*HUB_COUNT],art_hashes[256*HUB_COUNT];
 game.ship=10;preview_time=12;
 for(int sys=0;sys<256;sys++)if(station_class_for_system(&game,sys)==STATION_MEGA)for(int hub=0;hub<HUB_COUNT;hub++){
  game.system=sys;game.station_variant=hub;unsigned mask=0;stock_ok&=mega_yard_stock_count(&game)==4;
  for(int j=0;j<4;j++){int ship=mega_yard_stock_ship(&game,j);stock_ok&=ship>=10&&ship<22&&!(mask&(1u<<ship));mask|=1u<<ship;stock_ok&=shipyard_stocks_ship(&game,ship)&&shipyard_list_ship(&game,10+j)==ship;}
  for(int j=0;j<stock_n;j++)stock_unique&=mask!=stock_masks[j];stock_masks[stock_n]=mask;
  yc_grand_scene(station_profile_hash(sys,hub),16);unsigned hash=yc_review_hash(8,65,464,108);for(int j=0;j<stock_n;j++)art_unique&=hash!=art_hashes[j];art_hashes[stock_n]=hash;
  if(stock_n<3){page=YARD;row=10;shipyard_stats_view=1;yard();char path[64];snprintf(path,sizeof(path),"mega-dealer-%d-hub-%d.bmp",sys,hub);dump_native_bmp(path);}stock_n++;
 }
 fprintf(out,"INFO %d capital dealers checked\n",stock_n);
 YCHECK(stock_ok&&stock_n>0,"all actual Mega dealers list four valid purchasable capital hulls");
 YCHECK(stock_unique,"every actual Mega dealer has a distinct four-hull stock set");
 YCHECK(art_unique,"every actual Mega dealer has a different stable showroom image");
 game.system=7;game.station_variant=0;YCHECK(mega_yard_stock_count(&game)==12&&shipyard_list_count(&game)==22,"Lave retains all twelve Mega hulls and ten regular hulls for testing");
 int preview_ok=1;
 for(int ship=0;ship<22;ship++)for(int angle=0;angle<3;angle++){
  preview_time=angle*17.f;rect(0,0,W,H,YC_BG);yc_ship(ship,113,76,262,78);
  for(int y=0;y<H;y++)for(int x=0;x<W;x++)if((x<113||x>=375||y<76||y>=154)&&pixels[y*STRIDE+x]!=YC_BG)preview_ok=0;
 }
 YCHECK(preview_ok&&clipx0==0&&clipx1==W&&clipy0==-1&&clipy1==-1,"all 22 rotating real hulls stay inside their aperture and restore renderer clipping");
 /* A dealer's layout must survive every regular/capital selection and both
  * reading modes; owning a capital hull never upgrades an ordinary dealer. */
 int layout_ok=1;game.system=7;game.station_variant=0;preview_time=12;
 for(int owned=0;owned<2;owned++){game.ship=owned?16:0;for(int view=0;view<2;view++){
  shipyard_stats_view=view;row=0;rect(0,0,W,H,YC_BG);yard();unsigned brand=yc_review_hash(8,26,272,36);
  for(int offer=0;offer<shipyard_list_count(&game);offer++){row=offer;rect(0,0,W,H,YC_BG);yard();layout_ok&=pixels[179*STRIDE+192]==RGB(101,115,106)&&brand==yc_review_hash(8,26,272,36);}
 }}
 YCHECK(layout_ok,"Lave keeps its Mega showroom and dealer branding for all 22 offers, both reading modes and either owned hull class");
 game.ship=0;row=0;shipyard_stats_view=1;yard();dump_native_bmp("mega-dealer-lave-basic-ship.bmp");
 int regular=-1,regional=-1,stock_layout=1;
 for(int sys=0;sys<256;sys++){
  game.system=sys;
  if(sys!=7&&station_class(&game)!=STATION_MEGA){stock_layout&=mega_yard_stock_count(&game)==0&&shipyard_list_count(&game)==PLAYER_SHIP_STANDARD_COUNT;for(int i=0;i<shipyard_list_count(&game);i++)stock_layout&=!player_ship_is_mega(shipyard_list_ship(&game,i));if(regular<0&&station_class(&game)==STATION_RICH)regular=sys;}
  else if(sys!=7&&regional<0)regional=sys;
 }
 YCHECK(stock_layout&&regular>=0&&regional>=0,"ordinary dealers only list regular hulls while mega-capable dealers remain distinct");
 game.system=regional;game.station_variant=0;game.ship=0;row=0;shipyard_stats_view=1;yard();unsigned regional_brand=yc_review_hash(8,26,272,36);int regional_ok=pixels[179*STRIDE+192]==RGB(101,115,106);dump_native_bmp("mega-dealer-regional-basic-ship.bmp");
 row=10;yard();regional_ok&=pixels[179*STRIDE+192]==RGB(101,115,106)&&regional_brand==yc_review_hash(8,26,272,36);dump_native_bmp("mega-dealer-regional-capital-ship.bmp");
 YCHECK(regional_ok,"a regional Mega dealer keeps the same layout and name when moving from a basic ship to a capital ship");
 game.system=regular;game.station_variant=0;game.ship=16;row=0;shipyard_stats_view=1;yard();dump_native_bmp("regular-dealer-capital-owner.bmp");
 YCHECK(pixels[179*STRIDE+192]!=RGB(101,115,106),"owning a capital ship does not turn an ordinary dealer into a Mega showroom");
 game.ship=0;row=4;preview_time=12;shipyard_stats_view=1;yard();dump_native_bmp("orbit-motors-stats.bmp");input(PSP_CTRL_SQUARE,0,.016f,0,0);yard();dump_native_bmp("orbit-motors-description.bmp");
 YCHECK(!shipyard_stats_view&&page==YARD&&row==4,"regular dealer uses the same Square toggle without changing selection");
 game.system=7;game.station_variant=0;row=0;input(PSP_CTRL_UP,0,.016f,0,0);YCHECK(row==21,"catalogue Up wraps to the last real local offer");input(PSP_CTRL_DOWN,0,.016f,0,0);YCHECK(row==0,"catalogue Down wraps to the first real local offer");
 game.ship=16;row=16;shipyard_stats_view=1;yard();dump_native_bmp("crown-grand-owned.bmp");
 YCHECK(pixels[199*STRIDE+277]!=YC_UP&&pixels[231*STRIDE+445]!=YC_DOWN,"current hull shows no better-or-worse arrows");
 game.ship=14;row=10;yard();dump_native_bmp("crown-grand-refund.bmp");char refund[40];yc_exchange_label(10,176,refund,sizeof(refund));YCHECK(!strcmp(refund,"YOU RECEIVE: 200000 U"),"refund uses YOU RECEIVE with positive exact units and no clipped digits");
 game.ship=0;game.credits=10000000;fit_clear_all(&game);game.cargo[0]=1;row=4;int before=game.credits,cost=ship_exchange_cost(&game,4);input(PSP_CTRL_CROSS,0,.016f,0,0);
 YCHECK(game.ship==4&&game.credits==before-cost&&game.cargo[0]==1&&ship_purchase_open(),"new screen exchanges the real selected hull and opens the existing delivery card");
 preview_time+=.3f;input(PSP_CTRL_CIRCLE,0,.016f,0,0);input(0,0,.016f,0,0);input(PSP_CTRL_CIRCLE,0,.016f,0,0);YCHECK(page!=YARD&&!ship_purchase_open(),"Circle closes the delivery card after its existing purchase guard, then returns from the shipyard");
 fprintf(out,"RESULT %d failures\n",failures);fclose(out);fb=oldfb;free(pixels);
#undef YCHECK
 return 1;
}
