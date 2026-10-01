{
 for(int body=1;body<BODY_COUNT;body++){
  TEST_INIT();launch(&game);page=FLIGHT;game.approach=body;game.energy=75;
  input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
  INPUT_CHECK(planet_entry_body==body&&game.energy==75,"planet boundary: confirmed entry starts guidance without damage");
  for(int j=0;j<340&&planet_transfer_busy();j++)input(0,0,.05f,0,0);
  INPUT_CHECK(game.planet==body&&game.surface==2&&!game.planet_sequence,"planet boundary: solid worlds and gas platforms automatically land and disembark");
 }
 TEST_INIT();
}
