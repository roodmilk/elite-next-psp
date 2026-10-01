/* Runs inside input_tests, with the real encounter state and controls. */
 for(int phase=0;phase<3;phase++){
  TEST_INIT();launch(&game);page=FLIGHT;game.jump=4;thargoid_begin();
  if(phase)thargoid_intro=0;if(phase==2)thargoid_wave_delay=1;
  int credits=game.credits,kills=game.kills,origin=game.system,destination=game.destination;float fuel=game.fuel;
  thargoid_hp=1;thargoid_bolt[0]=(ThargoidBolt){thargoid_cursor_x,thargoid_cursor_y,0,0,.001f,1};
  input(PSP_CTRL_SELECT,PSP_CTRL_SELECT|PSP_CTRL_CROSS,.016f,0,0);
  INPUT_CHECK(!thargoid_active&&page==FLIGHT&&game.jump==2.6f&&game.system==origin&&game.destination==destination&&game.fuel==fuel&&game.credits==credits&&game.kills==kills&&!thargoid_bolt[0].alive&&!thargoid_maybe_start(),"thargoid Select skip works before damage/fire in intro, combat and reinforcement pause without extra rewards");
 }
 TEST_INIT();{
  launch(&game);game.jump=4;thargoid_begin();int credits=game.credits;
  float z=thargoid_raider[0].z;
  for(int n=0;n<120;n++)thargoid_tick(1.f/60,1,1,PSP_CTRL_CROSS);
  INPUT_CHECK(thargoid_intro>0&&thargoid_hp==100&&game.credits==credits&&thargoid_raider[0].z==z,"thargoid countdown blocks shooting, movement and enemy damage");
  thargoid_intro=0;thargoid_fire();INPUT_CHECK(thargoid_laser>0,"thargoid firing creates a visible laser pulse");
  int stable=1;
  for(int i=0;i<48;i++){int x,y,xx,yy;thargoid_star(i,2.f,&x,&y);thargoid_star(i,2.f+1.f/60,&xx,&yy);if(abs(xx-x)>2||abs(yy-y)>2)stable=0;}
  INPUT_CHECK(stable,"thargoid stars retain identity and move continuously between frames");
  thargoid_hp=10000;
  for(int n=0;n<1800;n++)thargoid_tick(1.f/60,0,0,0);
  int safe=1,readable=0;
  for(int sample=0;sample<90;sample++){
   int visible=0;for(int i=0;i<THARGOID_MAX;i++)if(thargoid_raider[i].alive){float x,y;Vec3 p=thargoid_position(&thargoid_raider[i]);thargoid_project(&thargoid_raider[i],&x,&y);if(p.z<110||p.z>1800||!isfinite(x)||!isfinite(y))safe=0;if(x>20&&x<460&&y>35&&y<222)visible++;}
   if(visible>=2)readable=1;thargoid_time+=1.f/30;for(int i=0;i<THARGOID_MAX;i++)if(thargoid_raider[i].alive)thargoid_raider[i].age+=1.f/30;
  }
  INPUT_CHECK(safe&&readable,"thargoid attack passes stay finite and repeatedly reform into readable groups");
  {int sections=1;for(int s=0;s<5;s++){thargoid_time=s*9.f+.1f;sections&=thargoid_route_section()==s;}thargoid_time=9.25f;INPUT_CHECK(sections&&thargoid_route_power()>3.f,"thargoid route cycles space, asteroid, crystal, megastructure and storm scenes with speed surges");}
  {thargoid_begin();thargoid_intro=0;memset(thargoid_missile,0,sizeof(thargoid_missile));thargoid_enemy_missile(thargoid_cursor_x,thargoid_cursor_y);int hp=thargoid_hp;thargoid_fire_cd=0;thargoid_fire();INPUT_CHECK(!thargoid_missile[0].alive&&thargoid_hp==hp,"thargoid missiles can be shot down before impact");
   thargoid_enemy_missile(thargoid_cursor_x,thargoid_cursor_y);thargoid_missile[0].life=.001f;thargoid_tick(.016f,0,0,0);INPUT_CHECK(thargoid_hp<hp,"unanswered Thargoid missiles strike for meaningful hull damage");}
  {memset(thargoid_missile,0,sizeof(thargoid_missile));thargoid_wave=1;for(int i=0;i<8;i++)thargoid_enemy_missile(120+i*20,80);int incoming=0;float life=0;for(int i=0;i<THARGOID_MISSILES;i++)if(thargoid_missile[i].alive){incoming++;life=thargoid_missile[i].life;}INPUT_CHECK(incoming==2&&life>=4.f,"thargoid fire control limits early formations to two slow readable missiles");
   memset(thargoid_missile,0,sizeof(thargoid_missile));thargoid_wave=3;for(int i=0;i<8;i++)thargoid_enemy_missile(120+i*20,80);incoming=0;for(int i=0;i<THARGOID_MISSILES;i++)incoming+=thargoid_missile[i].alive;INPUT_CHECK(incoming==3,"thargoid final formation remains capped at three incoming missiles");}
  INPUT_CHECK(audio_sfx_length(SFX_ALIEN_PLAYER)>0&&audio_sfx_length(SFX_ALIEN_FIRE)>0&&audio_sfx_length(SFX_ALIEN_MISSILE)>0&&audio_sfx_length(SFX_ALIEN_KILL)>0&&audio_sfx_length(SFX_SPEED_SURGE)>0,"thargoid rail fire, alien fire, missile, kill and surge cues all have bounded synth sounds");
  thargoid_begin();thargoid_intro=0;int total_kills=0;
  while(thargoid_active&&total_kills<60){
   int found=0;for(int i=0;i<THARGOID_MAX;i++)if(thargoid_raider[i].alive){thargoid_kill(&thargoid_raider[i]);total_kills++;found=1;break;}
   if(!found&&thargoid_active)thargoid_tick(.05f,0,0,0);
  }
  INPUT_CHECK(!thargoid_active&&total_kills==45&&game.jump>0,"thargoid extended blockade releases 45 enemies and completes without losing the route");
 }
