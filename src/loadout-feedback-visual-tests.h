{
 TEST_INIT();page=INVENTORY;row=0;
 game.fit[0]=1;fit_rebuild(&game);game.message_time=0;rect(0,0,W,H,BG);inventory_screen();
 unsigned status_before[8*464];for(int yy=0;yy<8;yy++)memcpy(status_before+yy*464,pixels+(224+yy)*STRIDE+8,464*sizeof(unsigned));
 input(PSP_CTRL_SQUARE,0,.016f,0,0);rect(0,0,W,H,BG);inventory_screen();
 int intact=1;for(int yy=0;yy<8;yy++)intact&=!memcmp(status_before+yy*464,pixels+(224+yy)*STRIDE+8,464*sizeof(unsigned));
 INPUT_CHECK(intact,"loadout feedback: arming leaves HOLD/HULL readout unchanged");
 unsigned before=0,after=0;for(int y=0;y<H;y++)for(int x=0;x<W;x++)before=before*33+pixels[y*STRIDE+x];
 menu_notice();for(int y=0;y<H;y++)for(int x=0;x<W;x++)after=after*33+pixels[y*STRIDE+x];
 INPUT_CHECK(before==after,"loadout feedback: generic menu notice cannot draw a duplicate");
 dump_native_bmp("loadout-arm-feedback.bmp");TEST_INIT();
}

