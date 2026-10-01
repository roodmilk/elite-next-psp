{
 unsigned *traffic_saved_fb=fb,*traffic_pixels=malloc(STRIDE*H*sizeof(unsigned));
 INPUT_CHECK(traffic_pixels!=0,"traffic: private native preview buffer allocated");if(traffic_pixels)fb=traffic_pixels;
 TEST_INIT();launch(&game);page=FLIGHT;game.pos=(Vec3){0,100000,0};speech_ok();night_wait=480;
 game.fit[FIT_UTIL]=LAW_SCANNER_ITEM;fit_rebuild(&game);
 game.npc[0].role=LAW;game.npc[0].alive=1;game.npc[0].pos=add(game.pos,(Vec3){800,250,500});
 input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);
 input(PSP_CTRL_RTRIGGER,PSP_CTRL_CIRCLE|PSP_CTRL_RTRIGGER,.016f,0,0);
 input(0,0,.016f,0,0);
 INPUT_CHECK(tools_selected==4&&!game.law_scan_valid&&!game.boost,"law scanner: Circle plus R selects without scanning or boosting");
 input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);input(0,0,.016f,0,0);
 INPUT_CHECK(game.law_scan_valid,"law scanner: subsequent Circle tap scans");
 rect(0,0,W,H,BG);space();dump_native_bmp("traffic-radar.bmp");
 input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);input(0,PSP_CTRL_CIRCLE,.25f,0,0);
 rect(0,0,W,H,BG);space();tools_panel();dump_native_bmp("traffic-tools.bmp");input(0,0,.016f,0,0);
 change_page(DETAILS);system_route_view=0;input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE,.016f,0,0);
 INPUT_CHECK(page==DETAILS&&!system_route_view,"system almanac: Square does not replace the illustrated overview with a route screen");
 rect(0,0,W,H,BG);system_details();dump_native_bmp("system-almanac.png");
 fb=traffic_saved_fb;free(traffic_pixels);TEST_INIT();fflush(f);
}
