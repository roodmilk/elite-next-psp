/* Solid pointed rounds, in world space with shared scene depth. Four faces
 * per minigun round; fixed existing projectile pool, no particles/allocation. */
static void __attribute__((noinline,optimize("Os"))) race_neon_projectile_art(void){
 int material=surface_material;surface_material=0;
 for(int i=0;i<RA_PROJECTILES;i++){const RaceProjectile *p=&game.race.arcade.shot[i];if(!p->live||!neon_visible(p->pos,240))continue;
  Vec3 tip=p->pos,tail=add(tip,mul(p->dir,p->kind==RA_MINIGUN?-125:-300));
  Vec3 right=norm(race_ui_cross(p->dir,neon_up));if(dot(right,right)<.5f)right=neon_right;
  Vec3 up=norm(race_ui_cross(right,p->dir));float width=p->kind==RA_MINIGUN?10:16;
  Vec3 rim[4]={add(tail,mul(right,width)),add(tail,mul(up,width)),sub(tail,mul(right,width)),sub(tail,mul(up,width))};
  for(int face=0;face<4;face++)queue_triangle(neon_camera(tip),neon_camera(rim[face]),neon_camera(rim[(face+1)&3]),face&1?RGB(255,148,47):RGB(255,244,180));
 }
 neon_flush();surface_material=material;
}
