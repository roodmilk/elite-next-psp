{
 TEST_INIT();launch(&game);page=FLIGHT;police_begin(&game,0);row=17;
 for(int choice=0;choice<4;choice++){
  game.cargo[6]=2;police_choice=choice;rect(0,0,W,H,BG);police_dialog();
  int py=182+(choice%3)*24;
  INPUT_CHECK(pixels[py*STRIDE+16]==RGB(245,157,62)&&row==17,"police: actual choice owns yellow highlight and does not overwrite menu focus");
  char file[48];snprintf(file,sizeof(file),"police-reply-%d.bmp",choice);dump_native_bmp(file);
 }
 for(int phase=2;phase<=8;phase++){
  game.police_phase=phase;game.police_timer=1.4f;rect(0,0,W,H,BG);police_dialog();
  char file[48];snprintf(file,sizeof(file),"police-phase-%d.bmp",phase);dump_native_bmp(file);
 }
 TEST_INIT();launch(&game);page=FLIGHT;record_crime(&game,5,CRIME_LAW_ASSAULT);police_begin(&game,0);police_choice=0;rect(0,0,W,H,BG);police_dialog();dump_native_bmp("police-assault.bmp");
 game.cargo[6]=2;police_begin(&game,1);police_scan_submit(&game);rect(0,0,W,H,BG);police_dialog();dump_native_bmp("police-mixed.bmp");
 INPUT_CHECK(dialogue_pages>1,"law: long charge list is paginated");input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);
 INPUT_CHECK(dialogue_page==1&&game.police_stop,"law: shoulder button reads next page without selecting a reply");rect(0,0,W,H,BG);police_dialog();dump_native_bmp("police-mixed-page2.bmp");
 TEST_INIT();page=HOME;police_choice=0;
}

