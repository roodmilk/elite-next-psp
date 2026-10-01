/* Upright, depth-tested discovery sprites. Never clamp apparent near size. */
static float field_fauna_height(int family,unsigned seed){
 static const float sizes[]={16,36,23,28,18,22,34,19};return sizes[family]+(seed&3);
}
/* Small contact decals, reconstructed from the existing depth buffer. No
 * geometry/depth writes: terrain stays visible and nearer objects occlude them. */
static int field_fauna_shadow(const Lifeform *l){
 if(!l->alive||l->kind!=LIFE_FAUNA||!surface_depth_on)return 0;
 int slot=(int)(l-game.life);if(slot<0||slot>=LIFE_COUNT)return 0;
 if(terrain_is_water(&game,l->pos.x,l->pos.z))return 0;
 field_sprite_build(game.system,game.planet);
 float radius=field_fauna_height(field_art_family[slot],field_art_seed[slot])*.29f;
 float ground=terrain_height(&game,l->pos.x,l->pos.z);
 Vec3 centre=camera(&game,(Vec3){l->pos.x,ground,l->pos.z});
 if(centre.z<radius+4||centre.z>600)return 0;
 float opacity=.38f/(1+fmaxf(0,l->lift)*.035f);
 if(centre.z>400)opacity*=(600-centre.z)/200;
 opacity*=fminf(1,.35f+surface_light);
 int left=W,right=-1,top=surface_y_max,bottom=surface_y_min;
 for(int i=0;i<4;i++){
  float x=l->pos.x+((i&1)?radius:-radius),z=l->pos.z+((i&2)?radius:-radius);
  Vec3 v=camera(&game,(Vec3){x,terrain_height(&game,x,z),z});if(v.z<=4)return 0;
  Point p=project(v);left=(int)fminf(left,floorf(p.x)-2);right=(int)fmaxf(right,ceilf(p.x)+2);
  top=(int)fminf(top,floorf(p.y)-2);bottom=(int)fmaxf(bottom,ceilf(p.y)+2);
 }
 left=left<0?0:left;right=right>=W?W-1:right;
 top=top<surface_y_min?surface_y_min:top;bottom=bottom>surface_y_max?surface_y_max:bottom;
 float sx=(terrain_height(&game,l->pos.x+radius,l->pos.z)-terrain_height(&game,l->pos.x-radius,l->pos.z))/(2*radius);
 float sz=(terrain_height(&game,l->pos.x,l->pos.z+radius)-terrain_height(&game,l->pos.x,l->pos.z-radius))/(2*radius);
 int drawn=0;
 for(int y=top;y<=bottom;y++)for(int x=left;x<=right;x++){
  unsigned encoded=surface_depth[y*W+x];if(encoded==65535)continue;
  float depth=262140.f/(65535-encoded);
  /* Terrain raster samples 2x2 blocks at their centres. */
  float rx=((x&~1)+1-proj_ox)/240.f,ry=(proj_oy-((y&~1)+1))/240.f;
  float rz=surface_cp-surface_sp*ry;
  float dx=game.pos.x+(surface_cy*rx+surface_sy*rz)*depth-l->pos.x;
  float dz=game.pos.z+(-surface_sy*rx+surface_cy*rz)*depth-l->pos.z;
  float q=(dx*dx+dz*dz)/(radius*radius);if(q>=1)continue;
  float wy=game.pos.y+(surface_sp+surface_cp*ry)*depth;
  if(fabsf(wy-(ground+sx*dx+sz*dz))>1.f+depth*depth/262140.f)continue;
  /* Blend into the actual biome/deck colour, preserving its pixel texture. */
  int shade=(int)(256*(1-opacity*(q<.35f?1:q<.7f?.65f:.28f)));
  unsigned c=fb[y*STRIDE+x];fb[y*STRIDE+x]=RGB(((c&255)*shade)>>8,(((c>>8)&255)*shade)>>8,(((c>>16)&255)*shade)>>8);drawn++;
 }
 return drawn;
}
static void field_draw_world(const Lifeform *l){
 int slot=(int)(l-game.life);if(slot<0||slot>=LIFE_COUNT)return;
 field_sprite_build(game.system,game.planet);unsigned h=field_art_seed[slot];int family=field_art_family[slot],kind=field_art_kind[slot];
 float ground=terrain_height(&game,l->pos.x,l->pos.z),height=kind==LIFE_FAUNA?34.f+(h&7):kind==LIFE_FLORA?24.f+(h&7):22.f;
 if(kind==LIFE_FAUNA){height=field_fauna_height(family,h);ground+=l->lift;}
 else if(game.system==7&&game.planet==3)ground+=5;
 Vec3 base=camera(&game,(Vec3){l->pos.x,ground,l->pos.z}),topv=camera(&game,(Vec3){l->pos.x,ground+height,l->pos.z});
 float plane=base.z-surface_sprite_tan*base.y,width=height*.75f;if(plane<3||plane>800)return;
 int first=surface_y_min,last=surface_y_max;if(topv.z>4)first=(int)fmaxf(first,floorf(project(topv).y));if(base.z>4)last=(int)fminf(last,ceilf(project(base).y));
 unsigned colors[64];for(int i=0;i<64;i++)colors[i]=surface_shade(field_species_inks[slot][i]);
 int frame=(int)(game.time*3+(h&3))&3;
 int mirror=0;
 if(kind==LIFE_FAUNA){
  static const int stride[]={0,1,0,2};
  int moving=l->speed>.2f;frame=moving?stride[(int)(l->phase*4)&3]:l->behaviour==FAUNA_FEED?3:0;
  if(!moving&&(family==0||family==5))frame=3;
  if(family==3)frame=stride[(int)(l->phase*4)&3];
  if(l->behaviour==FAUNA_ALERT)frame=0;
  mirror=sinf(l->heading)*surface_cy-cosf(l->heading)*surface_sy<-.05f;
 }
 for(int y=first;y<=last;y++){
  float ry=(proj_oy-y-.5f)/240.f,inv=(1-surface_sprite_tan*ry)/plane;if(inv<=0)continue;
  float wy=game.pos.y-ground+(ry*surface_cp+surface_sp)/inv;int sy=(int)((1-wy/height)*64);if(sy<0||sy>=64)continue;
  float left=proj_ox+(base.x-width*.5f)*inv*240,sw=width*inv*240;if(sw<1)continue;
  int x0=(int)fmaxf(0,ceilf(left)),x1=(int)fminf(W-1,left+sw);float step=48/sw,u=(x0+.5f-left)*step;unsigned depth=surface_encode_depth(inv);
  for(int x=x0;x<=x1;x++,u+=step){if(depth>surface_depth[y*W+x])continue;int sx=(int)u;if(mirror)sx=47-sx;int ink=field_sprite_sample(slot,sx,sy,frame);if(ink!=255)surface_depth_pixel(x,y,depth,colors[ink]);}
 }
}
