{
 TEST_INIT();launch(&game);page=FLIGHT;game.speed=0;game.yaw=game.pitch=0;
 for(int type=0;type<4;type++){
  rect(0,0,W,H,BG);rift_portrait(240,128,90,type,2.5f,24,242);
  char path[40];snprintf(path,40,"rift-type-%d.bmp",type);dump_native_bmp(path);
 }
 game.pos=sub(game.anomaly[0].pos,(Vec3){0,0,700});selected_target=ANOMALY_ID_MIN;speech_ok();
 input(PSP_CTRL_TRIANGLE,PSP_CTRL_TRIANGLE,.016f,0,0);input(0,0,.016f,0,0);
 INPUT_CHECK(game.rift_report==1&&(game.rift_logged[7]&1)&&!incoming_reply_ready(),"rift: actual Triangle scan opens report without reply choices");
 space();dump_native_bmp("rift-report.bmp");
 input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);
 INPUT_CHECK(dialogue_page==1,"rift: report shoulder paging reads full lore");space();dump_native_bmp("rift-report-page2.bmp");
 int credits=game.credits;input(PSP_CTRL_TRIANGLE,PSP_CTRL_TRIANGLE,.016f,0,0);input(0,0,.016f,0,0);
 INPUT_CHECK(!game.rift_report&&page==FLIGHT&&game.credits==credits,"rift: Triangle closes report without another action");
 atlas_reset();page=CODEX;row=0;atlas_open();row=codex_rows()-1;atlas_open();
 INPUT_CHECK(codex_scope==ATLAS_SIGNALS&&atlas_rift_count()==1,"rift: logged field appears in its system signals");
 row=0;atlas_open();codex_screen();dump_native_bmp("rift-codex.bmp");
 INPUT_CHECK(codex_scope==7&&atlas_entry==0,"rift: Codex opens the same stable field report");
 TEST_INIT();page=HOME;
}
