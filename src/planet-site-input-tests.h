{
 TEST_INIT();launch(&game);page=FLIGHT;game.planet=1;game.surface=2;game.rover_driving=0;planet_controls_ready=1;
 game.pos=surface_poi(&game,2);Vec3 origin=game.pos;int money=game.credits;
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 INPUT_CHECK(ps_open&&game.credits==money,"site: X opens scene without granting a reward");
 ps_row=4;input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 INPUT_CHECK(!ps_inspected&&game.credits==money,"site: entry press cannot activate an option");
 input(0,0,.016f,0,0);float clock_before=game.world_clock;
 input(0,PSP_CTRL_RTRIGGER,.5f,1,1);
 INPUT_CHECK(length(sub(game.pos,origin))==0&&game.world_clock==clock_before,"site: movement and world simulation pause while inside");
 ps_row=0;input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(ps_inspected,"site: deliberate inspection commits text");
 game.surface_progress[game.system][game.planet]|=0xff00u;
 ps_row=4;ps_action();int after_work=game.credits;ps_action();
 INPUT_CHECK(game.credits==after_work,"site: repeat completion cannot claim a second payment");
 char reading[2048];snprintf(reading,sizeof(reading),"%s",sc_read_text);
 input(PSP_CTRL_DOWN,0,.016f,0,0);
 INPUT_CHECK(!strcmp(reading,sc_read_text),"site: highlighting does not replace description");
 if(ps_staffed()){
  ps_row=3;ps_choose();ps_row=1;ps_choose();
  INPUT_CHECK(ps_reply&&strcmp(sc_read_text,"LIFE OUT HERE?")&&strlen(sc_read_text)>100,"site: personal reply shows NPC prose, not player echo");
  ps_row=3;ps_choose();
 }
 input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);
 INPUT_CHECK(!ps_open&&ps_release&&length(sub(game.pos,origin))==0,"site: back restores exact surface position");
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 INPUT_CHECK(!ps_open,"site: closing gesture blocks accidental re-entry");
 input(0,0,.016f,0,0);
 int coverage=1;
 for(int sys=0;sys<256;sys++)for(int body=1;body<BODY_COUNT;body++){
  game.system=sys;game.planet=body;
  for(int id=0;id<10;id++){if(id==6)continue;ps_begin(id);int k=ps_kind();coverage&=ps_open&&k>=0&&k<12&&sc_read_text[0]&&ps_objects[k][0][0];ps_open=0;}
 }
 INPUT_CHECK(coverage,"site: every planetary site resolves a bounded scene and description");
 ps_open=ps_release=0;TEST_INIT();
}
