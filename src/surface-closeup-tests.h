/* Reachable eight-unit wall clearance must remain outside the camera clip. */
{
 TEST_INIT();launch(&game);game.approach=1;enter_planet(&game);game.surface=2;page=FLIGHT;
 int solid=1;unsigned background=RGB(5,9,17);
 static const float positions[8][2]={{0,-28},{28,-28},{28,0},{28,28},{0,28},{-28,28},{-28,0},{-28,-28}};
 for(int side=0;side<8;side++)for(int tilt=0;tilt<3;tilt++){
  game.pos=(Vec3){positions[side][0],22,positions[side][1]};game.yaw=atan2f(-game.pos.x,-game.pos.z);game.pitch=(tilt-1)*.7f;game.roll=0;
  rect(0,0,W,H,background);memset(surface_depth,255,sizeof(surface_depth));surface_depth_on=1;surface_light=1;surface_y_min=28;surface_y_max=239;drawcount=0;
  surface_box((Vec3){0,0,0},20,20,64,RGB(179,146,88));flush_meshes();
  for(int y=100;y<120;y++)for(int x=220;x<260;x++)if(pixels[y*STRIDE+x]==background)solid=0;
  if(side==0&&tilt==1){FILE *flag=fopen("eva-capture.flag","r");if(flag){fclose(flag);dump_native_bmp("building-wall-closeup.bmp");}}
 }
 INPUT_CHECK(solid,"surface close-up: walls/corners stay opaque at reachable clearance across camera pitch");
 memset(surface_depth,255,sizeof(surface_depth));surface_pixel(240,110,6,RED);surface_pixel(240,110,12,WHITE);
 INPUT_CHECK(pixels[110*STRIDE+240]==RED,"surface close-up: depth still distinguishes surfaces nearer than fifteen units");
 surface_depth_on=0;surface_light=1;drawcount=0;TEST_INIT();
}
