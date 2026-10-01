{
 unsigned saved_paint[16];memcpy(saved_paint,ship_paint,sizeof(saved_paint));
 int old_mode=hud_mode,old_hidden=hud_hidden,old_contrast=high_contrast,old_sel=pip_sel;
 TEST_INIT();launch(&game);page=FLIGHT;game.ship=0;
 int visible=1,unique=1,labels=1;
 for(int finish=0;finish<DECORATOR_COUNT;finish++){
  ship_paint[0]=decorator_finishes[finish];
  for(int mode=0;mode<3;mode++)for(int contrast=0;contrast<2;contrast++)for(int bank=0;bank<3;bank++){
   hud_mode=mode;hud_hidden=mode==2;high_contrast=contrast;pip_sel=bank;paused=1;rect(0,0,W,H,RGB(1,2,3));cockpit();
   for(int k=0;k<3;k++){int x=336+k*36;int lit=pixels[197*STRIDE+x-4]==RGB(255,236,150);if(k==bank)visible&=lit;else unique&=!lit;}
   int x=336+bank*36,dark=0;for(int y=200;y<208;y++)for(int xx=x;xx<x+24;xx++)dark+=pixels[y*STRIDE+xx]==RGB(12,18,26);labels&=dark>12;
  }
 }
 INPUT_CHECK(visible&&unique&&labels,"power selection: one filled, readable bank highlight across 16 themes, 3 HUD modes and both contrast settings");
 /* Real frame dispatch must also show it in scenic mode, then retain that preference. */
 hud_mode=2;hud_hidden=1;high_contrast=0;paused=0;pip_sel=1;input(0,PSP_CTRL_START,.016f,0,0);space();
 INPUT_CHECK(paused&&pixels[197*STRIDE+368]==RGB(255,236,150),"power selection: holding Start reveals the controls in scenic flight");
 input(PSP_CTRL_RIGHT,PSP_CTRL_START|PSP_CTRL_RIGHT,.016f,0,0);space();
 INPUT_CHECK(pip_sel==2&&pixels[197*STRIDE+404]==RGB(255,236,150),"power selection: Start+Right moves the visible highlight to the actual WEP bank");
 input(0,0,.016f,0,0);INPUT_CHECK(!paused&&hud_mode==2&&hud_hidden,"power selection: releasing Start keeps the chosen scenic HUD preference");
 FILE *flag=fopen("eva-capture.flag","r");int capture=flag!=NULL;if(flag)fclose(flag);
 if(capture){for(int finish=0;finish<DECORATOR_COUNT;finish++){ship_paint[0]=decorator_finishes[finish];paused=1;pip_sel=finish%3;hud_mode=hud_hidden=0;space();char file[64];snprintf(file,sizeof(file),"power-selection-theme-%02d.bmp",finish);dump_native_bmp(file);}}
 paused=0;TEST_INIT();page=DECORATOR;row=7;input(PSP_CTRL_DOWN,0,.016f,0,0);
 INPUT_CHECK(row==8,"decorator pages: Down reaches the first new finish");
 input(PSP_CTRL_UP,0,.016f,0,0);INPUT_CHECK(row==7,"decorator pages: Up returns to the original finishes");
 row=DECORATOR_COUNT-1;input(PSP_CTRL_DOWN,0,.016f,0,0);INPUT_CHECK(row==0,"decorator pages: all sixteen finishes wrap safely");
 int purchases=1;game.ship=0;
 for(int finish=8;finish<DECORATOR_COUNT;finish++){
  game.credits=100000;ship_paint[0]=decorator_finishes[0];row=finish;input(PSP_CTRL_CROSS,0,.016f,0,0);
  purchases&=ship_paint[0]==decorator_finishes[finish]&&game.credits==100000-decorator_fees[finish]*10&&ship_theme_index()==finish;
  int before=game.credits;input(PSP_CTRL_CROSS,0,.016f,0,0);purchases&=game.credits==before;
 }
 INPUT_CHECK(purchases,"decorator purchases: all eight new finishes apply their matching theme at the displayed fee; reselection is free");
 for(int i=0;i<16;i++)ship_paint[i]=decorator_finishes[i];
 int stored=paint_save();memset(ship_paint,0,sizeof(ship_paint));paint_load();int restored=1;for(int i=0;i<16;i++)restored&=ship_paint[i]==decorator_finishes[i];
 INPUT_CHECK(stored&&restored,"decorator persistence: original and new finishes survive settings reload on all ship slots");
 if(capture){game.credits=123456;decorator_feedback=0;for(int page_index=0;page_index<2;page_index++){row=page_index*8+2;decorator_screen();dump_native_bmp(page_index?"decorator-finishes-page-2.bmp":"decorator-finishes-page-1.bmp");}}
 memcpy(ship_paint,saved_paint,sizeof(saved_paint));paint_save();TEST_INIT();hud_mode=old_mode;hud_hidden=old_hidden;high_contrast=old_contrast;pip_sel=old_sel;paused=0;
}
