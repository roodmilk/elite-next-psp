{
 TEST_INIT();launch(&game);page=FLIGHT;game.planet=2;game.surface=2;game.pos=surface_poi(&game,2);ps_begin(2);ps_ready=1;
 int money=game.credits;ps_row=4;ps_choose();
 INPUT_CHECK(!ps_reply&&game.credits==money,"observatory UI: decision gated behind actual inspections");
 for(int i=0;i<3;i++){ps_row=i;ps_choose();}
 INPUT_CHECK((game.surface_progress[7][2]&OBS_CLUES)==OBS_CLUES,"observatory UI: all three inspected objects persist");
 ps_row=4;ps_choose();INPUT_CHECK(ps_reply==2&&ps_obs_count()==3,"observatory UI: two outcomes and a leave option");
 ps_row=1;ps_choose();INPUT_CHECK(ps_reply==3&&game.credits==money&&!ps_done(),"observatory UI: choosing trace requires confirmation and does not pay yet");
 ps_row=1;ps_choose();INPUT_CHECK(ps_reply==2&&!ps_done(),"observatory UI: cancelling confirmation preserves unfinished work");
 ps_row=1;ps_choose();ps_row=0;ps_choose();
 INPUT_CHECK(ps_done()&&observatory_choice(&game,7,2)==2&&!ps_reply,"observatory UI: confirmed trace records branch and closes choice");
 ps_open=ps_release=0;ps_begin(2);
 INPUT_CHECK(strstr(sc_read_text,"preserved")!=NULL&&ps_done(),"observatory UI: reopening describes the saved outcome");
 ps_row=4;ps_choose();INPUT_CHECK(!ps_reply,"observatory UI: completed room offers report rather than another decision");
 ps_open=ps_release=0;TEST_INIT();
}
