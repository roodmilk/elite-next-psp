/* PSP input dispatch under PPSSPP, including analog-off fallback and complete mineral return. */
{
 TEST_INIT();launch(&game);page=FLIGHT;game.approach=1;
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 game.pos=surface_site(&game,1);game.speed=8;
 input(PSP_CTRL_CIRCLE,0,.016f,0,0);input(PSP_CTRL_CIRCLE,0,.016f,0,0);
 INPUT_CHECK(game.surface==2&&eva_can_board(&game),"EVA controls: land and disembark keep immediate boarding reachable");
 float ax,ay;Vec3 before=game.pos;
 steering_axes_centered(1,flight_steer_buttons(PSP_CTRL_UP|PSP_CTRL_RIGHT),128,128,1,128,128,&ax,&ay);
 float yaw=game.yaw;input(0,PSP_CTRL_UP|PSP_CTRL_RIGHT,.05f,ax,ay);
 INPUT_CHECK(ax==0&&ay==0&&game.yaw==yaw&&length(sub(game.pos,before))>1,"EVA controls: D-pad movement does not leak into nub look");
 before=game.pos;int analog=analog_enabled;analog_enabled=0;
 steering_axes_centered(1,flight_steer_buttons(PSP_CTRL_LTRIGGER|PSP_CTRL_RIGHT|PSP_CTRL_UP),255,0,0,128,128,&ax,&ay);
 input(0,PSP_CTRL_LTRIGGER|PSP_CTRL_RIGHT|PSP_CTRL_UP,.05f,ax,ay);
 INPUT_CHECK(game.pos.x==before.x&&game.pos.z==before.z&&game.yaw>yaw&&game.pitch>0&&game.roll==0,"EVA controls: analog-off L+D-pad looks without walking or rolling");
 analog_enabled=analog;autoaim=1;before=game.pos;float fuel=game.fuel;
 input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.05f,0,0);
 INPUT_CHECK(!autoaim&&game.pos.y>before.y&&game.pos.x==before.x&&game.pos.z==before.z&&game.fuel==fuel,"EVA controls: held R lifts immediately without ship throttle or fuel use");
 input(0,0,.05f,0,0);INPUT_CHECK(!game.boost,"EVA controls: releasing R releases the jet");
 for(int i=0;i<60;i++)input(0,0,.05f,0,0);
 int shots=game.shots,missiles=game.missiles;input(PSP_CTRL_CROSS,PSP_CTRL_LTRIGGER|PSP_CTRL_CROSS,.05f,0,0);
 INPUT_CHECK(game.shots==shots&&game.missiles==missiles,"EVA controls: spacecraft weapon chords cannot fire on foot");
 /* One naturally spawned mineral; other scanned flags isolate repeat-press behaviour. */
 int mineral=-1;for(int i=0;i<LIFE_COUNT;i++){game.life[i].scanned=1;if(game.life[i].kind==LIFE_MINERAL&&mineral<0)mineral=i;}
 INPUT_CHECK(mineral>=0,"EVA activity: starting ocean world supplies a mineral target");
 if(mineral>=0){
  game.life[mineral].scanned=0;int ore=game.cargo[12],cash=game.credits,discoveries=game.discoveries;
  input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE,.016f,0,0);
  INPUT_CHECK(game.cargo[12]==ore+1&&game.credits==cash+120&&game.discoveries==discoveries+1,"EVA activity: Square records a mineral and gives existing ore/payment");
  input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE,.016f,0,0);
  INPUT_CHECK(game.cargo[12]==ore+1&&game.credits==cash+120,"EVA activity: repeated scan does not pay twice in the same visit");
  /* Walk out, use the advertised return control, then walk back and board. */
  game.yaw=atan2f(game.pos.x-game.ship_pos.x,game.pos.z-game.ship_pos.z);
  for(int i=0;i<35;i++)input(0,PSP_CTRL_UP,.05f,0,0);
  input(PSP_CTRL_TRIANGLE,PSP_CTRL_TRIANGLE,.016f,0,0);
  input(0,0,.016f,0,0);
  INPUT_CHECK(dot(forward(&game),norm(sub(game.ship_pos,game.pos)))>.99f&&page==FLIGHT,"EVA return: Triangle faces ship instead of hailing or leaving atmosphere");
  for(int i=0;i<80&&!eva_can_board(&game);i++)input(0,PSP_CTRL_UP,.05f,0,0);
  input(PSP_CTRL_CIRCLE,0,.016f,0,0);INPUT_CHECK(game.surface==1&&game.cargo[12]==ore+1,"EVA return: walking home and Circle boarding retains mineral cargo");
  game.voice_time=game.message_time=0;input(PSP_CTRL_TRIANGLE,0,.016f,0,0);
  game.voice_time=game.message_time=0;input(PSP_CTRL_TRIANGLE,0,.016f,0,0);
  INPUT_CHECK(game.planet<0&&game.cargo[12]==ore+1&&game.discoveries==discoveries+1,"EVA return: takeoff and orbit preserve the survey result");
  game.pos=(Vec3){0,0,2800};game.speed=0;dock(&game);
  for(int i=0;i<1200&&!game.docked;i++)game_tick(&game,1.f/60,0,0,0,0);
  {Game saved;int keep=game.credits;
   INPUT_CHECK(save_game(&game,"test-eva-return.sav")&&load_game(&saved,"test-eva-return.sav")&&saved.cargo[12]==ore+1&&saved.credits==keep&&saved.discoveries==game.discoveries,"EVA reward: docked V13 round trip retains unsold ore and discovery");
   remove("test-eva-return.sav");remove("test-eva-return.sav.bak");}
  cash=game.credits;int sale=game.price[12];
  INPUT_CHECK(game.docked&&trade(&game,12,0)&&game.cargo[12]==ore&&game.credits==cash+sale,"EVA reward: returned mineral sells through existing hub trade");
 }
 TEST_INIT();change_page(HELP);help_tab=4;
 input(PSP_CTRL_RIGHT,0,.016f,0,0);INPUT_CHECK(help_tab==0,"EVA help: fifth controls page wraps to flight");
}
