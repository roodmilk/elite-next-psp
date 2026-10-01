/* At most 24 cosmetic leaves, evaluated only near real trees. No simulation,
 * allocation, collision, save data, or random calls in the update loop. */
static int field_leaf_count;
static void field_tree_leaves(float x,float z,unsigned seed,float height){
 float dx=x-game.pos.x,dz=z-game.pos.z;if(field_leaf_count>=24||dx*dx+dz*dz>180*180||high_contrast)return;
 float ground=terrain_height(&game,x,z);
 for(int i=0;i<4&&field_leaf_count<24;i++){
  unsigned h=field_hash(seed+i*977u);float duration=9+(h&7),phase=fmodf(game.time/duration+(h>>8&255)/255.f,1.f);
  float a=(h%628)*.01f,r=height*.22f;
  Vec3 p={x+cosf(a)*r+sinf(phase*6.2831853f+(h&7))*7,ground+2+height*.85f*(1-phase),z+sinf(a)*r+phase*9};
  Vec3 v=camera(&game,p);if(v.z<4)continue;Point q=project(v);int sx=(int)q.x,sy=(int)q.y;
  if(sx<1||sx>=W-2||sy<surface_y_min||sy>=surface_y_max-2)continue;
  unsigned ink=surface_shade((h&3)?RGB(149,163,69):RGB(197,155,75)),depth=surface_encode_depth(1/v.z);
  int sway=((int)(game.time*4+i)&1)?1:-1;
  surface_depth_pixel(sx,sy,depth,ink);surface_depth_pixel(sx+sway,sy+1,depth,ink);
  if(v.z<90)surface_depth_pixel(sx,sy+1,depth,ink);
  field_leaf_count++;
 }
}
