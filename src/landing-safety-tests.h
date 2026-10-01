/* Explicit transfer controls and release gating. */
{
 TEST_INIT();launch(&game);page=FLIGHT;game.approach=1;
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 int safe=1;
 for(int j=0;j<340&&planet_transfer_busy();j++){
  input(PSP_CTRL_CIRCLE|PSP_CTRL_TRIANGLE|PSP_CTRL_RTRIGGER,PSP_CTRL_CIRCLE|PSP_CTRL_TRIANGLE|PSP_CTRL_RTRIGGER,.05f,0,0);
  safe&=game.planet==1||planet_entry_body==1;
 }
 INPUT_CHECK(safe&&game.surface==2&&!game.planet_sequence,"landing flow: buttons during arrival/landing cannot interrupt automatic disembark");
 input(PSP_CTRL_TRIANGLE,PSP_CTRL_TRIANGLE,.016f,0,0);
 INPUT_CHECK(game.surface==2,"landing flow: carried-over held buttons cannot immediately board");
 input(0,0,.016f,0,0);input(PSP_CTRL_CIRCLE,0,.016f,0,0);
 INPUT_CHECK(game.surface==2,"landing flow: Circle is not a boarding or landing action on foot");
 input(PSP_CTRL_TRIANGLE,PSP_CTRL_TRIANGLE,.016f,0,0);
 INPUT_CHECK(game.surface==1&&!game.planet_sequence,"landing flow: Triangle beside ship opens parked-ship controls");
 input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);
 INPUT_CHECK(game.surface==1&&!game.planet_sequence,"landing flow: launch requires release after boarding");
 for(int j=0;j<24&&planet_seat.active;j++)input(0,0,.05f,0,0);input(0,0,.016f,0,0);input(PSP_CTRL_TRIANGLE,0,.016f,0,0);
 INPUT_CHECK(game.surface==1&&!game.planet_sequence,"landing flow: Triangle cannot launch from parked ship");
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 INPUT_CHECK(game.surface==2&&!game.planet_sequence,"landing flow: X explicitly steps outside from parked ship");
 for(int j=0;j<24&&planet_seat.active;j++)input(0,0,.05f,0,0);input(0,0,.016f,0,0);input(PSP_CTRL_TRIANGLE,0,.016f,0,0);for(int j=0;j<24&&planet_seat.active;j++)input(0,0,.05f,0,0);input(0,0,.016f,0,0);
 input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);
 INPUT_CHECK(game.surface==0&&game.planet_sequence==3,"landing flow: fresh R on parked-ship screen starts departure");
 for(int j=0;j<215&&planet_transfer_busy();j++)input(PSP_CTRL_CIRCLE|PSP_CTRL_TRIANGLE,PSP_CTRL_CIRCLE,.05f,0,0);
 INPUT_CHECK(game.planet<0&&!game.planet_sequence,"landing flow: departure finishes to orbit despite unrelated buttons");
 TEST_INIT();launch(&game);game.approach=1;enter_planet(&game);page=FLIGHT;
 message(&game,"Atmosphere entry complete. Approach the landing pad.");
 input(PSP_CTRL_TRIANGLE,0,.016f,0,0);
 INPUT_CHECK(game.planet==1&&game.message_time==0,"landing flow: Triangle closes atmosphere notices without orbit");
 input(PSP_CTRL_TRIANGLE,PSP_CTRL_TRIANGLE,.016f,0,0);
 INPUT_CHECK(planet_landing_menu&&!game.planet_sequence,"landing flow: Triangle in atmosphere only opens confirmation");
 input(0,0,.016f,0,0);input(PSP_CTRL_CIRCLE,0,.016f,0,0);
 INPUT_CHECK(!planet_landing_menu&&!game.surface&&!game.planet_sequence,"landing flow: landing confirmation can be cancelled");
 TEST_INIT();
}
