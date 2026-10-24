#include "earth-meadow-scene.h"
#include "earth-meadow-clue.h"
#define MEADOW_DISTANT_TREES 96
static struct {Vec3 p;float height;unsigned seed;int kind;} meadow_far_trees[MEADOW_DISTANT_TREES];
static int meadow_far_ready,meadow_far_visible;
static Vec3 meadow_far_pad;
static void earth_meadow_distant_trees(Vec3 pad){
 if(!meadow_far_ready||pad.x!=meadow_far_pad.x||pad.z!=meadow_far_pad.z){
  meadow_far_ready=1;meadow_far_pad=pad;
  /* Unequal groves frame open pasture, rather than another circular fence. */
  static const float centres[12][2]={{-600,740},{530,970},{-970,1470},{1220,1840},{-1630,2200},{570,2420},
   {-910,-700},{690,-1130},{1670,470},{-1990,-340},{-1210,-2140},{2040,-1570}};
  for(int i=0;i<MEADOW_DISTANT_TREES;i++){
   unsigned seed=planet_hash(1709u+i*7907u);float a=(seed%6283)*.001f,r=35+(seed>>16)%190;
   float x=centres[i/8][0]+sinf(a)*r,z=centres[i/8][1]+cosf(a)*r;
   meadow_far_trees[i].p=meadow_landscape_point(pad,x,z);
   meadow_far_trees[i].height=(i%8==0?190.f:88.f)+(seed>>8)%65;
   meadow_far_trees[i].seed=seed;meadow_far_trees[i].kind=i%4;
  }
 }
 meadow_far_visible=0;
 for(int i=0;i<MEADOW_DISTANT_TREES;i++){
  Vec3 p=meadow_far_trees[i].p;float height=meadow_far_trees[i].height;
  Vec3 v=camera(&game,add(p,(Vec3){0,height*.5f,0}));
  if(v.z<4||fabsf(v.x)>v.z+height||fabsf(v.y)>v.z*.8f+height)continue;
  meadow_far_visible++;
  draw_lave_vegetation_shape(p.x,p.z,meadow_far_trees[i].seed,meadow_far_trees[i].kind,p.y,height);
 }
 surface_sprite_z=0;
}
static void earth_meadow_life(Vec3 pad){
 earth_meadow_flower_trail(pad);
 for(int i=0;i<9;i++){
  float a=game.time*.085f+i*.7f;Vec3 p=meadow_ground_point(pad,sinf(a)*245,cosf(a)*245,105+i%3*14),v=camera(&game,p);if(v.z<25)continue;Point q=project(v);
  surface_sprite_z=v.z-surface_sprite_tan*v.y;unsigned ink=RGB(50,65,67);int flap=((int)(game.time*5)+i)&1;planet_cliprect(q.x-1,q.y,3,1,ink);planet_cliprect(q.x-4,q.y+(flap?-1:1),3,1,ink);planet_cliprect(q.x+2,q.y+(flap?-1:1),3,1,ink);
 }
 for(int i=0;i<14;i++){
  float a=game.time*.7f+i*2.2f,x=-75+(i%4)*61+sinf(a)*7,z=85+(i/4)*34+cosf(a)*8;
  Vec3 p=meadow_ground_point(pad,x,z,7+sinf(a*1.7f)*3),v=camera(&game,p);if(v.z<5||v.z>170)continue;Point q=project(v);
  surface_sprite_z=v.z-surface_sprite_tan*v.y;unsigned ink=i&1?RGB(229,181,84):RGB(156,201,230);int wing=((int)(game.time*10+i)&1)?2:1;planet_cliprect(q.x-wing,q.y,wing,2,ink);planet_cliprect(q.x+1,q.y,wing,2,ink);planet_cliprect(q.x,q.y,1,3,RGB(63,66,49));
 }
 earth_meadow_stone(pad);
 surface_sprite_z=0;
 /* Dad waits by the landing gear while the player explores. */
 Vec3 dad=meadow_ground_point(pad,62,-42,0),base=camera(&game,dad),head=camera(&game,add(dad,(Vec3){0,29,0}));
 if(base.z>5&&head.z>5){Point a=project(base),b=project(head);int h=(int)(a.y-b.y),w=(int)fmaxf(3,h*.22f),x=(int)a.x,y=(int)b.y;
  if(h>4&&h<220){surface_sprite_z=base.z-surface_sprite_tan*base.y;
   planet_cliprect(x-w,y+h/3,w*2,h/3,RGB(62,96,109));planet_cliprect(x-w-2,y+h/3+2,2,h/3,RGB(162,125,91));planet_cliprect(x+w,y+h/3+2,2,h/3,RGB(162,125,91));
   planet_cliprect(x-w,y+h*2/3,w,h/3,RGB(39,49,59));planet_cliprect(x+1,y+h*2/3,w,h/3,RGB(46,56,64));
   planet_cliprect(x-w/2,y,w,h/3,RGB(190,151,110));planet_cliprect(x-w/2,y,w,h/10+1,RGB(90,79,66));
   planet_cliprect(x-w/2+1,y+h/7,1,1,RGB(35,39,40));planet_cliprect(x+w/2-1,y+h/7,1,1,RGB(35,39,40));
  }
 }surface_sprite_z=0;flush_meshes();
}
