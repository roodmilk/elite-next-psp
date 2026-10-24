/* Run in the isolated opening framebuffer. Exercise real billboard sampling,
 * not only the source atlas: wind, pitch, near scale and both screen edges. */
{
 static Game kept;kept=game;int guard_ok=1,drawn=0;
 game.pos=(Vec3){0,0,0};game.yaw=game.roll=0;game.system=7;game.planet=1;surface_light=1;
 surface_y_min=28;surface_y_max=239;surface_depth_on=1;
 lave_prepare_spans();
 for(int fog=0;fog<4;fog++)for(int c=0;c<32;c++)lave_tree_inks[fog][c]=lave_vegetation_palette[c];
 for(int fog=0;fog<4;fog++)for(int c=0;c<128;c++)biome_prop_inks[fog][c]=biome_prop_palette[c];
 for(int world=1;world<=4;world++)for(int kind=0;kind<4;kind++)for(int scale=0;scale<3;scale++)for(int side=0;side<3;side++)for(int angle=0;angle<3;angle++)for(int phase=0;phase<3;phase++){
  game.planet=world;
  float z=scale==0?90:scale==1?240:720;
  float x=((side==0?-20:side==1?240:500)-proj_ox)*z/240;
  game.pitch=(angle-1)*.6f;game.time=phase*1.3f;
  surface_cp=cosf(game.pitch);surface_sp=sinf(game.pitch);surface_sprite_tan=tanf(game.pitch);
  memset(surface_depth,255,sizeof(surface_depth));
  for(int y=0;y<H;y++)for(int x=0;x<STRIDE;x++)fb[y*STRIDE+x]=RGB(247,3,231);
  rect(0,28,W,212,RGB(17,28,41));
  draw_lave_vegetation_shape(x,z,17,kind,-70,128);
  for(int y=0;y<H;y++)for(int px=0;px<STRIDE;px++){
   if(y<28||y>=240||px>=W)guard_ok&=fb[y*STRIDE+px]==RGB(247,3,231);
   else drawn+=fb[y*STRIDE+px]!=RGB(17,28,41);
  }
 }
 OPEN_CHECK(guard_ok&&drawn>10000,"vegetation wind pitch and near-scale sampling stays inside the viewport across 1296 edge poses on four planets");
 game.planet=1;game.pitch=0;surface_cp=1;surface_sp=surface_sprite_tan=0;
 for(int phase=0;phase<3;phase++){
  game.time=phase*1.3f;rect(0,0,W,H,RGB(17,28,41));memset(surface_depth,255,sizeof(surface_depth));
  text_px(12,10,UI_GOLD,"FOUR COMPLETE TREE SPRITES / WIND %d",phase+1);
  for(int kind=0;kind<4;kind++)draw_lave_vegetation_shape(60+kind*120-proj_ox,240,17,kind,-112,160);
  char file[64];snprintf(file,sizeof(file),"vegetation-283-wind-%d.bmp",phase);dump_native_bmp(file);
 }
 for(int world=2;world<=4;world++){
  game.planet=world;rect(0,0,W,H,RGB(17,28,41));memset(surface_depth,255,sizeof(surface_depth));
  text_px(12,10,UI_GOLD,"PLANET %d / COMPLETE PROP SILHOUETTES",world);
  for(int kind=0;kind<4;kind++)draw_lave_vegetation_shape(60+kind*120-proj_ox,240,17,kind,-78,128);
  char file[64];snprintf(file,sizeof(file),"vegetation-283-planet-%d.bmp",world);dump_native_bmp(file);
 }
 game.planet=1;
 /* Empty/invalid cells must never read a neighbour in either atlas. */
 rect(0,0,W,H,RGB(17,28,41));memset(surface_depth,255,sizeof(surface_depth));
 draw_lave_vegetation_shape(0,240,17,-1,-112,160);draw_lave_vegetation_shape(0,240,17,8,-112,160);
 int untouched=1;for(int y=0;y<H;y++)for(int x=0;x<W;x++)untouched&=fb[y*STRIDE+x]==RGB(17,28,41);
 OPEN_CHECK(untouched,"invalid vegetation cells are rejected instead of sampling neighbouring memory");
 surface_depth_on=0;surface_sprite_z=0;game=kept;
}
