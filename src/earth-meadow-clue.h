/* Native pixel-sized petals and a bounded, depth-tested glow. No texture
 * allocations or full-screen bloom; all accents belong to actual world points. */
static void meadow_glow(int cx,int cy,int r,float plane,unsigned ink,float strength){
 int top=cy-r,bottom=cy+r,left=cx-r,right=cx+r;
 if(top<surface_y_min)top=surface_y_min;if(bottom>surface_y_max)bottom=surface_y_max;
 if(left<0)left=0;if(right>=W)right=W-1;
 for(int y=top;y<=bottom;y++){
  float inv=(1-surface_sprite_tan*(proj_oy-y-.5f)/240.f)/plane;if(inv<=0)continue;
  unsigned depth=surface_encode_depth(inv);
  for(int x=left;x<=right;x++){
   int dx=x-cx,dy=y-cy,q=dx*dx+dy*dy;if(q>=r*r||depth>surface_depth[y*W+x])continue;
   float a=(1-q/(float)(r*r))*strength;
   fb[y*STRIDE+x]=mix_rgb(fb[y*STRIDE+x],ink,a*a+ a*.35f);
  }
 }
}
static void earth_meadow_flower_trail(Vec3 pad){
 for(int i=0;i<EARTH_MEADOW_FLOWERS;i++){
  float x,z;earth_meadow_flower(i,&x,&z);
  for(int j=0;j<3;j++){
   float side=j==0?-3:j==1?3:0,offset=j==2?2.5f:-1.5f;
   Vec3 base=meadow_ground_point(pad,x+side,z+offset,0);
   float height=5+(i+j)%3;
   /* Blossoms lean towards the stone; the gentle sway is only a pixel. */
   Vec3 top=add(base,(Vec3){(95-x)*.025f,height,(55-z)*.015f});
   Vec3 v=camera(&game,top),foot=camera(&game,base);if(v.z<4||v.z>430||foot.z<4)continue;
   Point p=project(v),b=project(foot);if(p.x< -18||p.x>W+18)continue;
   surface_sprite_z=v.z-surface_sprite_tan*v.y;
   int r=(int)fmaxf(1,fminf(4,210/v.z));
   int stem=(int)fmaxf(1,fminf(40,b.y-p.y));
   int px=(int)p.x+(sinf(game.time*.8f+i+j)>.5f),py=(int)p.y;
   unsigned petal=j&1?RGB(195,173,234):RGB(154,222,211);
   planet_cliprect(px,py,1,stem,RGB(66,119,89));
   planet_cliprect(px-2,py+stem/2,3,1,RGB(100,164,112));
   if(i%4==0&&j==1)meadow_glow(px,py,r+3,surface_sprite_z,petal,.20f);
   planet_cliprect(px-r,py-r,r*2+1,r*2+1,RGB(87,105,149));
   planet_cliprect(px-r*2,py-r/2,r,r+1,petal);planet_cliprect(px+r+1,py-r/2,r,r+1,petal);
   planet_cliprect(px-r/2,py-r*2,r+1,r,petal);planet_cliprect(px-r/2,py+r+1,r+1,r,petal);
   planet_cliprect(px,py,1+(r>2),1+(r>2),RGB(250,237,181));
  }
 }surface_sprite_z=0;
}
static void earth_meadow_stone(Vec3 pad){
 if(game.opening.stage<OP_FIND||game.opening.stage>OP_OBJECT_CHOICE)return;
 Vec3 p=meadow_ground_point(pad,95,55,5),v=camera(&game,p);if(v.z<4||v.z>850)return;
 Point q=project(v);if(q.x< -30||q.x>W+30||q.y<surface_y_min-30||q.y>surface_y_max+30)return;
 int awake=(game.opening.flags&OF_MEADOW_GLOW)!=0;
 float pulse=.82f+.18f*sinf(game.time*1.7f),plane=v.z-surface_sprite_tan*v.y;
 int r=(int)fmaxf(awake?7:3,fminf(awake?24:10,1200/v.z));
 unsigned glow=RGB(131,232,205);surface_sprite_z=plane;
 meadow_glow((int)q.x,(int)q.y,r,plane,glow,(awake?.70f:.27f)*pulse);
 int core=(int)fmaxf(awake?2:1,fminf(5,230/v.z));
 planet_cliprect(q.x-core,q.y-core,core*2+1,core*2+1,RGB(77,153,151));
 planet_cliprect(q.x-core,q.y,core*2+1,core,RGB(142,240,208));
 planet_cliprect(q.x-core/2,q.y-core,core+1,core+1,awake?RGB(244,255,223):RGB(172,222,196));
 if(awake){
  planet_cliprect(q.x-1,q.y-r/2,2,r,RGB(190,249,213));
  for(int i=0;i<5;i++){float a=game.time*.45f+i*1.256637f;int x=q.x+cosf(a)*(core+3),y=q.y+sinf(a)*(core+3);planet_cliprect(x,y,1,1,glow);}
 }
 surface_sprite_z=0;
}
