/* Regression checks executed by the PPSSPP smoke build. */
static void steering_tests(void){
 FILE *f=fopen("steering-check.txt","w");if(!f)return;
 int failures=0;float x,y;
#define VERIFY(c,name) do{int ok=(c);fprintf(f,"%s %s\n",ok?"PASS":"FAIL",name);failures+=!ok;}while(0)
 int stable=1;
 for(int raw=0;raw<256;raw++){steering_axes(1,0,raw,255-raw,0,&x,&y);if(x!=0||y!=0)stable=0;}
 VERIFY(stable,"digital mode ignores every possible analog reading");
 int ready=0;
 VERIFY(!stick_ready(1,255,128,&ready),"off-centre startup does not arm analog");
 VERIFY(!stick_ready(0,128,128,&ready),"missing sample cannot arm analog");
 VERIFY(stick_ready(1,128,128,&ready),"centred sample arms analog");
 VERIFY(stick_ready(1,255,128,&ready),"intentional movement works after centring");
 steering_axes(0,PSP_CTRL_RIGHT,255,0,1,&x,&y);VERIFY(x==0&&y==0,"failed sample is neutral");
 steering_axes(1,0,150,110,1,&x,&y);VERIFY(x==0&&y==0,"analog centre drift is neutral");
 steering_axes_centered(1,0,154,126,1,150,128,&x,&y);VERIFY(x==0&&y==0,"calibrated off-centre nub cannot cause right drift");
 steering_axes(1,PSP_CTRL_RIGHT,0,128,0,&x,&y);VERIFY(x==1&&y==0,"D-pad right works despite opposite stick");
 steering_axes(1,PSP_CTRL_LEFT,255,128,1,&x,&y);VERIFY(x==-1,"D-pad overrides opposite analog input");
 steering_axes(1,0,255,128,1,&x,&y);VERIFY(x==1,"analog right endpoint");
 steering_axes(1,0,0,128,1,&x,&y);VERIFY(x==-1,"analog left endpoint");
 Game test;game_init(&test);launch(&test);test.speed=0;
 for(int i=0;i<NPC_COUNT;i++)test.npc[i].alive=0;
 for(int i=0;i<600;i++){steering_axes(1,0,255,128,0,&x,&y);game_tick(&test,1.f/60,x,y,0,0);}
 VERIFY(test.yaw==0&&test.pitch==0,"ten seconds of flight do not turn with stuck-right analog");
 steering_axes(1,PSP_CTRL_RIGHT,128,128,0,&x,&y);game_tick(&test,1.f/60,x,y,0,0);float yaw=test.yaw;
 steering_axes(1,0,128,128,0,&x,&y);game_tick(&test,1.f/60,x,y,0,0);
 VERIFY(yaw>0&&yaw==test.yaw,"turning stops on button release");
 /* Judge the nose in the PREVIOUS cockpit frame, not by Euler yaw sign. */
 {
  static Game before;
  const float banks[]={0,1.57079633f,3.14159265f,-1.57079633f};
  const float elevations[]={0,1.4f,-1.4f,1.57079633f,-1.57079633f,3.14159265f};
  int space_ok=1,air_ok=1;
  for(int air=0;air<2;air++)for(int r=0;r<4;r++)for(int p=0;p<6;p++)for(int d=0;d<4;d++){
   game_init(&test);launch(&test);test.speed=0;
   for(int i=0;i<NPC_COUNT;i++)test.npc[i].alive=0;
   test.planet=air?1:-1;test.surface=0;test.pos=(Vec3){0,500,-20000};
   test.yaw=.7f;test.pitch=elevations[p];test.roll=banks[r];before=test;
   float tx=d<2?(d==0?1:-1):0,ty=d>=2?(d==2?1:-1):0;
   game_tick(&test,.016f,tx,ty,0,0);
   Vec3 nose=camera(&before,add(before.pos,forward(&test)));
   int ok=nose.x*tx+nose.y*ty>.015f&&fabsf(nose.x*ty-nose.y*tx)<.001f;
   if(air)air_ok&=ok;else space_ok&=ok;
  }
  VERIFY(space_ok,"space steering follows cockpit axes at all banks and poles");
  VERIFY(air_ok,"atmosphere steering follows cockpit axes at all banks and poles");
  game_init(&test);launch(&test);test.speed=0;test.pos=(Vec3){0,0,-20000};
  for(int i=0;i<NPC_COUNT;i++)test.npc[i].alive=0;
  int loops_ok=1;
  for(int i=0;i<1600;i++){
   before=test;game_tick(&test,.016f,0,1,0,0);
   Vec3 nose=camera(&before,add(before.pos,forward(&test)));
   loops_ok&=nose.y>.02f&&fabsf(nose.x)<.001f;
  }
  VERIFY(loops_ok,"repeated full loops keep Up continuous through both poles");
  test.docked=1;test.roll=2.4f;launch(&test);
  VERIFY(test.yaw==0&&test.pitch==0&&test.roll==0,"launch resets the complete docking attitude");
 }
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);
#undef VERIFY
}
