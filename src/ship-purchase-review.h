static int ship_purchase_review(void){
 FILE *flag=fopen("ship-purchase-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *report=fopen("ship-purchase-review.txt","w");if(!report)return 1;
 int failures=0;unsigned *saved_fb=fb,*pixels=malloc(STRIDE*H*sizeof(unsigned));
 if(!pixels){fprintf(report,"RESULT 1 failures\n");fclose(report);return 1;}fb=pixels;
#define SCHECK(ok,label) do{int pass=!!(ok);fprintf(report,"%s %s\n",pass?"PASS":"FAIL",label);failures+=!pass;fflush(report);}while(0)
 game_init(&game);story_complete(&game);deck_reset();ship_purchase_reset();game.system=7;game.docked=1;game.credits=10000000;
 fit_clear_all(&game);game.fit[FIT_WPN]=1;fit_rebuild(&game);game.module_store[1]=2;game.cargo[0]=1;
 change_page(YARD);row=1;preview_time=20;int cash=game.credits,cost=ship_exchange_cost(&game,1);
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 SCHECK(ship_purchase_open()&&game.ship==1&&game.credits==cash-cost&&game.cargo[0]==1&&game.fit[FIT_WPN]==1&&game.module_store[1]==2,"successful shipyard exchange opens one card and preserves exact payment, cargo, fitted weapon and stored modules");
 cash=game.credits;int selected=row;input(PSP_CTRL_DOWN,0,.016f,0,0);input(PSP_CTRL_CROSS,0,.016f,0,0);
 SCHECK(ship_purchase_open()&&!ship_card_finished&&row==selected&&game.credits==cash,"purchase press cannot dismiss its own card or change selection/pay twice");
 preview_time=ship_card_started+.8f;input(PSP_CTRL_CROSS,0,.016f,0,0);
 SCHECK(ship_purchase_open()&&ship_card_finished&&game.credits==cash,"X during handwriting completes the note without another transaction");
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 SCHECK(!ship_purchase_open()&&page==YARD&&game.ship==1&&game.credits==cash,"second X closes the finished card and returns to the owned ship");
 input(PSP_CTRL_CROSS,0,.016f,0,0);SCHECK(!ship_purchase_open()&&game.credits==cash,"already-owned hull does not trigger a celebration or payment");
 row=2;game.credits=0;input(PSP_CTRL_CROSS,0,.016f,0,0);SCHECK(!ship_purchase_open()&&game.ship==1&&game.credits==0,"unaffordable ship does not trigger a celebration");
 row=0;game.cargo[0]=9;input(PSP_CTRL_CROSS,0,.016f,0,0);SCHECK(!ship_purchase_open()&&game.ship==1&&game.cargo[0]==9,"cargo-incompatible exchange leaves the ship and celebration unchanged");
 game.cargo[0]=1;game.credits=10000000;int varied=1,last=-1;unsigned seen=0;
 for(int i=0;i<12;i++){ship_purchase_reset();row=game.ship==1?2:1;preview_time+=4;input(PSP_CTRL_CROSS,0,.016f,0,0);varied&=ship_purchase_open()&&ship_card_variant!=last;last=ship_card_variant;seen|=1u<<last;preview_time+=.3f;cash=game.credits;input(PSP_CTRL_CIRCLE,0,.016f,0,0);varied&=!ship_purchase_open()&&game.credits==cash;}
 SCHECK(varied&&seen==63,"repeated successful purchases vary all six notes; Circle closes without undoing a purchase");
 game.ship=20;ship_purchase_start();unsigned hashes[3]={0};float ages[]={.18f,1.45f,3.65f};const char *frames[]={"ship-card-arriving.bmp","ship-card-writing.bmp","ship-card-complete.bmp"};
 for(int i=0;i<3;i++){ship_card_variant=0;ship_card_finished=0;preview_time=ship_card_started+ages[i];rect(0,0,W,H,BG);yard();ship_purchase_draw();dump_native_bmp(frames[i]);
  for(int y=95;y<185;y++)for(int x=112;x<368;x++)hashes[i]=hashes[i]*33u+pixels[y*STRIDE+x];
 }
 SCHECK(hashes[0]!=hashes[1]&&hashes[1]!=hashes[2],"arriving, partial pen strokes and completed handwriting are visibly distinct");
 int footer_hidden=1;for(int y=248;y<H;y++)for(int x=0;x<W;x++)footer_hidden&=pixels[y*STRIDE+x]==RGB(3,7,14);
 SCHECK(footer_hidden,"inactive shipyard controls are hidden while the card owns input");
 int fit=1;for(int i=0;i<6;i++){ship_card_variant=i;ship_card_finished=1;preview_time=ship_card_started+3.65f;rect(0,0,W,H,BG);yard();ship_purchase_draw();char path[48];snprintf(path,sizeof(path),"ship-card-note-%d.bmp",i);dump_native_bmp(path);
  for(int j=0;j<2;j++)fit&=ship_card_width(ship_card_lines[i][j])*2.7f<=300.f;
 }
 SCHECK(fit,"six native-size handwritten messages fit clear of the fireworks, ship name and controls");
 change_page(HOME);SCHECK(!ship_purchase_open()&&ship_card_started<-50,"leaving the shipyard resets the transient card");
 fprintf(report,"RESULT %d failures\n",failures);fclose(report);fb=saved_fb;free(pixels);
#undef SCHECK
 return 1;
}
