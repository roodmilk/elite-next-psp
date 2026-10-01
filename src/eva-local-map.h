/* Visit-scoped EVA survey map. The reveal mask deliberately lives outside
 * Game: landing starts a fresh paper chart and saves never grow for UI fog. */
#define EVA_MAP_GRID 80
#define EVA_MAP_WORDS ((EVA_MAP_GRID*EVA_MAP_GRID+31)/32)
static unsigned eva_map_seen[EVA_MAP_WORDS];
static float eva_map_pan_x=0,eva_map_pan_z=0;
static void eva_map_pan(float ax,float ay,float dt){
 if(!isfinite(ax)||!isfinite(ay)||!isfinite(dt))return;
 dt=fmaxf(0,fminf(.1f,dt));
 if(fabsf(ax)>.12f)eva_map_pan_x+=ax*3600.f*dt;
 if(fabsf(ay)>.12f)eva_map_pan_z+=ay*3600.f*dt;
 Vec3 pad=surface_site(&game,1);
 eva_map_pan_x=fmaxf(pad.x-EVA_FIELD_RADIUS-game.pos.x,fminf(pad.x+EVA_FIELD_RADIUS-game.pos.x,eva_map_pan_x));
 eva_map_pan_z=fmaxf(pad.z-EVA_FIELD_RADIUS-game.pos.z,fminf(pad.z+EVA_FIELD_RADIUS-game.pos.z,eva_map_pan_z));
}
static int eva_map_active=0,eva_map_open=0,eva_map_release=0,eva_map_system=-1,eva_map_body=-1;

static int eva_map_cell(float x,float z,int *cx,int *cz){
 Vec3 pad=surface_site(&game,1);float span=EVA_FIELD_RADIUS*2.f;
 int ix=(int)floorf((x-(pad.x-EVA_FIELD_RADIUS))*EVA_MAP_GRID/span);
 int iz=(int)floorf((z-(pad.z-EVA_FIELD_RADIUS))*EVA_MAP_GRID/span);
 if(ix<0||ix>=EVA_MAP_GRID||iz<0||iz>=EVA_MAP_GRID)return 0;
 if(cx)*cx=ix;
 if(cz)*cz=iz;
 return 1;
}
static int eva_map_known_cell(int x,int z){
 if(x<0||x>=EVA_MAP_GRID||z<0||z>=EVA_MAP_GRID)return 0;
 int bit=z*EVA_MAP_GRID+x;return (eva_map_seen[bit>>5]>>(bit&31))&1u;
}
static int eva_map_known_world(float x,float z){int cx,cz;return eva_map_cell(x,z,&cx,&cz)&&eva_map_known_cell(cx,cz);}
static void eva_map_reveal(float x,float z){
 int cx,cz;if(!eva_map_cell(x,z,&cx,&cz))return;
 /* Roughly 500 m around the walked route: useful without revealing the field. */
 for(int dz=-2;dz<=2;dz++)for(int dx=-2;dx<=2;dx++)if(dx*dx+dz*dz<=5){
  int xx=cx+dx,zz=cz+dz;if(xx<0||xx>=EVA_MAP_GRID||zz<0||zz>=EVA_MAP_GRID)continue;
  int bit=zz*EVA_MAP_GRID+xx;eva_map_seen[bit>>5]|=1u<<(bit&31);
 }
}
static void eva_map_leave(void){eva_map_active=0;eva_map_open=eva_map_release=0;}
static void eva_map_visit(void){
 if(game.planet<1||!game.surface){eva_map_leave();return;}
 if(game.surface!=2){eva_map_open=0;return;}
 if(!eva_map_active||eva_map_system!=game.system||eva_map_body!=game.planet){
  memset(eva_map_seen,0,sizeof(eva_map_seen));eva_map_active=1;
  eva_map_system=game.system;eva_map_body=game.planet;
 }
 eva_map_reveal(game.pos.x,game.pos.z);
}
static void eva_map_open_now(void){eva_map_visit();eva_map_pan_x=eva_map_pan_z=0;eva_map_open=1;game.eva_run_arm=0;game.eva_run_hold=0;game.eva_running=0;game.cue=SFX_UI;}
static int eva_map_input(unsigned pressed){if(pressed&PSP_CTRL_TRIANGLE){eva_map_open=0;eva_map_release=1;game.cue=SFX_UI;return 1;}if(pressed&(PSP_CTRL_START|PSP_CTRL_CIRCLE)){eva_map_open=0;eva_map_release=1;game.cue=SFX_UI;}return 0;}
