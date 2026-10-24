/* Exact-EBOOT checks: continuous simulation, no artificial enemy repositioning. */
{
 int ship=0,radio_before=radio_station,off_before=radio_off,quiet_before=quiet_comms;
 opening_new_game();game.opening.stage=OP_DREAM;opening_prepared=-1;opening_prepare();
 OPEN_CHECK(dream_eye_open(0)==0&&dream_eye_open(.57f)>.45f&&dream_eye_open(.78f)<.01f&&dream_eye_open(1.6f)==1,"dream eyelids blink briefly and finish opening in 1.6 seconds");
 {Vec3 turn={0,1,0};for(int t=0;t<100;t++)turn=dream_turn(turn,(Vec3){0,-1,0},1.f/30);OPEN_CHECK(turn.y<-.99f&&fabsf(length(turn)-1)<.001f,"attacker steering can reverse through a vertical loop without getting stuck");}
 OPEN_CHECK(strlen(dream_radio_script)>=986,"dream host has non-repeating copy for the full two-minute ticker");
 int extra=0,motion_ok=1,passes=0,shots_before=0,visible_frames=0,battle_frames=0;float closest=1e9f;
 unsigned capture_mask=0;static const float times[]={.45f,1.8f,8.f,12.f,16.f,28.f,119.f};
 for(int frame=0;frame<3605&&!dream_ending;frame++){
  Vec3 old[3];for(int i=0;i<3;i++)old[i]=game.npc[i].pos;
  int spawned=dream_spawned;
  /* Keep this observation run alive to verify the entire radio/timeout window. */
  game.hull=game.energy=100;game.damaged=0;
  input(0,0,1.f/30,0,0);
  for(int i=0;i<3;i++)if(game.npc[i].alive){
   if(spawned)motion_ok&=length(sub(game.npc[i].pos,old[i]))<13;
   closest=fminf(closest,length(sub(game.npc[i].pos,game.pos)));
   passes|=dream_leg[i]?1<<i:0;
  }
  for(int i=3;i<NPC_COUNT;i++)extra|=game.npc[i].alive;
  if(dream_time>DREAM_ATTACK){int visible=0;for(int i=0;i<3;i++)if(game.npc[i].alive){Vec3 v=camera(&game,game.npc[i].pos);visible|=v.z>30&&fabsf(v.x)<v.z*.85f&&fabsf(v.y)<v.z*.4f;}visible_frames+=visible;battle_frames++;}
  for(int i=0;i<DEBRIS_COUNT;i++)extra|=game.debris[i].alive;
  for(int c=0;c<7;c++)if(!(capture_mask&(1u<<c))&&dream_time>=times[c]){
   char path[48];snprintf(path,sizeof(path),"dream-281-%02d.bmp",c);opening_test_capture(path);capture_mask|=1u<<c;
   if(c==2){Vec3 a=game.npc[0].pos,b=game.npc[1].pos,d=game.npc[2].pos;OPEN_CHECK(b.x<a.x&&d.x>a.x&&b.z>a.z&&d.z>a.z&&dot(game.npc[0].dir,game.npc[1].dir)>.99f,"three enemies approach in an intact arrowhead formation");}
  }
 }
 OPEN_CHECK(!extra,"dream never spawns civilian traffic freighters police or loot");
 OPEN_CHECK(motion_ok&&passes==7&&closest<700,"all three ships close in and break away without camera-relative teleporting");
 OPEN_CHECK(visible_frames>battle_frames/3,"repeated flanking runs bring attackers back into the forward view");
 fprintf(f,"INFO dream forward visibility %d/%d frames, shots %d hits %d closest %.1fm\n",visible_frames,battle_frames,dream_shots_fired,dream_hits,closest);
 OPEN_CHECK(dream_shots_fired>12&&dream_hits>0,"attackers fire actual projectiles and aimed shots can hit the player");
 OPEN_CHECK(dream_ending&&dream_time>=120&&dream_time<120.1f&&capture_mask==127,"dream remains playable through the full two-minute timeout");
 OPEN_CHECK(!game.legal&&!game.police_stop&&game.approach<0&&!game.dock_stage,"empty-space battle cannot cause local law docking or landing prompts");
 /* Rendering is independent of underlying Sol geometry and incidental world UI. */
 unsigned before=2166136261u,after=2166136261u;opening_test_capture("dream-281-empty.bmp");
 for(int y=0;y<H;y++)for(int x=0;x<W;x++)before=(before^fb[y*STRIDE+x])*16777619u;
 for(int i=0;i<BODY_COUNT;i++){game.bodies[i].pos=game.pos;game.bodies[i].radius=900000;}
 game.legal=100;game.police_stop=1;game.message_time=10;snprintf(game.message,sizeof(game.message),"LAW IS AFTER YOU");
 opening_test_capture("dream-281-isolation.bmp");
 for(int y=0;y<H;y++)for(int x=0;x<W;x++)after=(after^fb[y*STRIDE+x])*16777619u;
 OPEN_CHECK(before==after,"dream pixels contain no planets stations law warnings or normal-system ticker");
 opening_new_game();game.opening.stage=OP_DREAM;opening_prepared=-1;opening_prepare();
 dream_time=6;input(0,0,1.f/30,0,0);game.speed=0;
 for(int i=0;i<18;i++)dream_bolts[i].life=0;
 for(int i=0;i<3;i++)game.npc[i].cooldown=100;
 game.energy=100;dream_bolts[0]=(DreamBolt){add(game.pos,(Vec3){0,0,40}),{0,0,-1800},1};
 dream_fleet_tick(1.f/30,game.pos);OPEN_CHECK(game.energy==92&&dream_bolts[0].life==0,"swept bolt collision catches shots crossing the player between frames");
 game.energy=100;dream_bolts[0]=(DreamBolt){add(game.pos,(Vec3){90,0,40}),{0,0,-1800},1};
 dream_fleet_tick(1.f/30,game.pos);OPEN_CHECK(game.energy==100&&dream_bolts[0].life>0,"a near miss causes no timer-based damage");
 for(int i=1;i<3;i++)game.npc[i].alive=0;
 game.npc[0].pos=add(game.pos,mul(forward(&game),650));game.npc[0].health=1;game.npc[0].shield=0;
 int credits=game.credits;shots_before=game.shots;game.shot=0;game.heat=0;fire_blocked=0;
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,1.f/30,0,0);
 OPEN_CHECK(game.shots>shots_before&&!game.npc[0].alive&&dream_ending,"normal fitted laser can destroy the last attacker and wake the player");
 OPEN_CHECK(!game.legal&&game.credits==credits&&!game.police_stop,"shooting dream enemies never records a crime or awards a bounty");
 dream_wake=6;input(0,0,1.f/30,0,0);
 OPEN_CHECK(game.opening.stage==OP_HOME&&game.credits==6000&&!game.kills&&!game.shots&&!game.legal,"waking restores the real commander with no dream combat progress");
 OPEN_CHECK(radio_station==radio_before&&radio_off==off_before&&quiet_comms==quiet_before&&radio_dream_gain==-1,"dream audio and ticker leave saved radio settings unchanged");
}
