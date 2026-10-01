{
 TEST_INIT();launch(&game);page=FLIGHT;row=17;police_choice=0;police_begin(&game,0);add_crime(&game,10);
 input(PSP_CTRL_DOWN,0,.016f,0,0);INPUT_CHECK(police_choice==1,"police: down selects custody independently of menu focus");
 input(PSP_CTRL_UP,0,.016f,0,0);INPUT_CHECK(police_choice==0,"police: up selects fine");
 input(PSP_CTRL_UP,0,.016f,0,0);INPUT_CHECK(police_choice==2,"police: clean hold has exactly three replies");
 game.cargo[6]=2;police_choice=0;input(PSP_CTRL_UP,0,.016f,0,0);
 INPUT_CHECK(police_choice==3,"police: dirty hold exposes fourth surrender reply");
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(game.cargo[6]==0&&game.legal==10&&game.police_stop&&police_choice==0,"police: surrender action preserves unrelated warrant and resets focus");
 TEST_INIT();launch(&game);page=FLIGHT;game.cargo[6]=2;police_begin(&game,1);police_choice=3;
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(game.police_phase==8&&game.police_stop&&game.legal==0,"police: cargo-only stand-down waits for receipt");
 input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(page==FLIGHT&&!game.police_stop&&fire_blocked,"police: receipt resumes flight without firing");
 for(int paid=0;paid<2;paid++){
  TEST_INIT();launch(&game);page=FLIGHT;game.credits=paid?2000:0;police_begin(&game,0);add_crime(&game,10);police_choice=1;
  input(PSP_CTRL_CROSS,0,.016f,0,0);
  INPUT_CHECK(page==FLIGHT&&game.police_stop&&game.police_phase==(paid?4:2),"police: custody stays in animated flight modal");
  for(int i=0;i<140;i++)input(0,0,.05f,0,0);
  INPUT_CHECK(game.police_phase==(paid?7:6)&&game.police_stop&&page==FLIGHT,"police: release explanation cannot time out");
  input(PSP_CTRL_CROSS,0,.016f,0,0);
  INPUT_CHECK(page==HOME&&game.docked&&!game.police_stop,"police: acknowledged release returns to station services");
 }
 TEST_INIT();police_choice=0;page=HOME;
}

