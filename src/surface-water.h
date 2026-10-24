/* Water uses the same 40m cell mask as collision/map, independent of mesh LOD.
 * Tagged 80KiB cache is bounded; animation changes colour, never geometry. */
static unsigned water_tags[128*128];
static unsigned char water_flags[128*128];
static unsigned water_seed,water_palette[32];static int water_system=-1,water_body=-1,water_phase;
static float water_padx,water_padz,water_dry,water_c,water_s;static int water_ocean,water_lakes,water_river;
static void surface_water_prepare(void){
 if(water_seed!=game.bodies[game.planet].seed||water_system!=game.system||water_body!=game.planet){
  memset(water_flags,0,sizeof(water_flags));water_seed=game.bodies[game.planet].seed;water_system=game.system;water_body=game.planet;
  Vec3 pad=surface_site(&game,1);water_padx=pad.x;water_padz=pad.z;water_dry=field_port_flat(&game)+398.f;
  float angle=((water_seed>>8)%6283)*.001f;water_c=cosf(angle);water_s=sinf(angle);water_ocean=game.bodies[game.planet].type==OCEAN;
  const WorldProfile *p=world_profile(water_seed,game.bodies[game.planet].type);water_lakes=0;water_river=world_river_type(p->type);
  if(p->type<18&&!(game.system==7&&game.planet==1))for(int i=0;i<p->count;i++)water_lakes|=world_wet[p->type][p->biome[i]%6]>45;
 }
}
static unsigned char water_cell(int x,int z){
 int at=(x&127)+((z&127)<<7);unsigned tag=(unsigned)(unsigned short)x|((unsigned)(unsigned short)z<<16);
 if(!water_flags[at]||water_tags[at]!=tag){
  float wx=(x+.5f)*SURFACE_CELL,wz=(z+.5f)*SURFACE_CELL;
  unsigned flags=terrain_water_tile(&game,wx,wz)?1:0;
  if(flags){if(!terrain_water_tile(&game,wx-40,wz))flags|=2;if(!terrain_water_tile(&game,wx+40,wz))flags|=4;if(!terrain_water_tile(&game,wx,wz-40))flags|=8;if(!terrain_water_tile(&game,wx,wz+40))flags|=16;}
  water_tags[at]=tag;water_flags[at]=flags|128;
 }
 return water_flags[at];
}
static int water_grid(float v){int i=(int)(v/40);return v<i*40?i-1:i;}
static int surface_water_region(float x0,float z0,float x1,float z1){
 float ax=x0-water_padx,bx=x1-water_padx,az=z0-water_padz,bz=z1-water_padz;
 if(fmaxf(fabsf(ax),fabsf(bx))<water_dry&&fmaxf(fabsf(az),fabsf(bz))<water_dry)return 0;
 if(water_ocean){float dx=fmaxf(fabsf(ax),fabsf(bx))+20,dz=fmaxf(fabsf(az),fabsf(bz))+20;if(dx*dx+dz*dz>=9800.f*9800.f)return 1;}
 if(water_lakes)return 1;if(!water_river)return 0;
 float a=-ax*water_s+az*water_c,b=-bx*water_s+az*water_c,c=-ax*water_s+bz*water_c,d=-bx*water_s+bz*water_c;
 return fminf(fminf(a,b),fminf(c,d))<=750&&fmaxf(fmaxf(a,b),fmaxf(c,d))>=-750;
}
static int surface_water_sample(float wx,float wz,float depth,unsigned *ink){
 float dxpad=wx-water_padx,dzpad=wz-water_padz;
 /* Conservative rejection only: the exact 40m mask still owns every shore. */
 if(fabsf(dxpad)<water_dry&&fabsf(dzpad)<water_dry)return 0;

 if(!water_lakes){float v=-dxpad*water_s+dzpad*water_c;if((!water_river||fabsf(v)>750)&&!(water_ocean&&dxpad*dxpad+dzpad*dzpad>9700.f*9700.f))return 0;}
 int x=water_grid(wx),z=water_grid(wz);unsigned flags=water_cell(x,z);if(!(flags&1))return 0;
 int a=(int)(wx*.35f+16384)-16384,b=(int)(wz*.55f+16384)-16384;
 unsigned h=(unsigned)a*374761393u+(unsigned)b*668265263u;h^=h>>13;
 int ripple=(a+b+water_phase)&31,other=(a-b*2-water_phase)&31;
 int tone=depth>1400?11:6+((ripple<16?ripple:31-ripple)+(other<16?other:31-other))/3;
 if(depth<1000&&ripple<2&&((h>>8)&3))tone=22+((h>>12)&3);
 float dx=wx-x*40,dz=wz-z*40;
 if(((flags&2)&&dx<5)||((flags&4)&&dx>35)||((flags&8)&&dz<5)||((flags&16)&&dz>35))tone=28+(h&3);
 *ink=water_palette[tone];return 1;
}
