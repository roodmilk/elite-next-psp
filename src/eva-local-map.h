/* Visit-scoped EVA survey map. The reveal mask deliberately lives outside
 * Game: landing starts a fresh paper chart and saves never grow for UI fog. */
#define EVA_MAP_GRID 40
#define EVA_MAP_WORDS ((EVA_MAP_GRID*EVA_MAP_GRID+31)/32)
static unsigned eva_map_seen[EVA_MAP_WORDS];
static int eva_map_active=0,eva_map_open=0,eva_map_system=-1,eva_map_body=-1;

static int eva_map_cell(float x,float z,int *cx,int *cz){
 Vec3 pad=surface_site(&game,1);float span=EVA_FIELD_RADIUS*2.f;
 int ix=(int)((x-(pad.x-EVA_FIELD_RADIUS))*EVA_MAP_GRID/span);
 int iz=(int)((z-(pad.z-EVA_FIELD_RADIUS))*EVA_MAP_GRID/span);
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
 /* Roughly 260 m around the walked route: useful without revealing the field. */
 for(int dz=-2;dz<=2;dz++)for(int dx=-2;dx<=2;dx++)if(dx*dx+dz*dz<=5){
  int xx=cx+dx,zz=cz+dz;if(xx<0||xx>=EVA_MAP_GRID||zz<0||zz>=EVA_MAP_GRID)continue;
  int bit=zz*EVA_MAP_GRID+xx;eva_map_seen[bit>>5]|=1u<<(bit&31);
 }
}
static void eva_map_leave(void){eva_map_active=0;eva_map_open=0;}
static void eva_map_visit(void){
 if(game.planet<1||game.surface!=2){eva_map_leave();return;}
 if(!eva_map_active||eva_map_system!=game.system||eva_map_body!=game.planet){
  memset(eva_map_seen,0,sizeof(eva_map_seen));eva_map_active=1;
  eva_map_system=game.system;eva_map_body=game.planet;
 }
 eva_map_reveal(game.pos.x,game.pos.z);
}
static void eva_map_mark(int mx,int my,char kind,unsigned ink){
 if(kind=='S'){line(mx-3,my+3,mx,my-4,ink);line(mx,my-4,mx+3,my+3,ink);line(mx-3,my+3,mx+3,my+3,ink);}
 else if(kind=='R'){rect(mx-3,my-2,7,5,ink);pixel(mx-2,my+3,ink);pixel(mx+2,my+3,ink);}
 else if(kind=='P'){line(mx-3,my,mx+3,my,ink);line(mx,my-3,mx,my+3,ink);circle(mx,my,3,ink);}
 else {rect(mx-2,my-2,5,5,ink);rect(mx-1,my-1,3,3,UI_PANEL);}
}
static void eva_map_world_to_screen(float x,float z,int ox,int oy,int size,int *sx,int *sy){
 Vec3 pad=surface_site(&game,1);float span=EVA_FIELD_RADIUS*2.f;
 *sx=ox+(int)((x-(pad.x-EVA_FIELD_RADIUS))*size/span);
 *sy=oy+size-(int)((z-(pad.z-EVA_FIELD_RADIUS))*size/span);
}
static void eva_map_structure(float x,float z,float hw,float hd,int ox,int oy,int size,unsigned ink){
 int x0,y0,x1,y1;eva_map_world_to_screen(x-hw,z+hd,ox,oy,size,&x0,&y0);eva_map_world_to_screen(x+hw,z-hd,ox,oy,size,&x1,&y1);
 if(x1<x0){int t=x0;x0=x1;x1=t;}if(y1<y0){int t=y0;y0=y1;y1=t;}
 if(x0<ox)x0=ox;if(y0<oy)y0=oy;if(x1>=ox+size)x1=ox+size-1;if(y1>=oy+size)y1=oy+size-1;
 if(x1>=x0&&y1>=y0){rect(x0,y0,x1-x0+1,1,ink);rect(x0,y1,x1-x0+1,1,ink);rect(x0,y0,1,y1-y0+1,ink);rect(x1,y0,1,y1-y0+1,ink);}
}
static void eva_map_draw(void){
 const int ox=20,oy=45,size=160,step=size/EVA_MAP_GRID;
 unsigned fog=high_contrast?RGB(17,22,29):RGB(11,18,26);
 rect(0,0,W,H,BG);header("EVA / LOCAL SURVEY MAP");rect(8,32,464,208,UI_PANEL);rect(8,32,464,1,UI_EDGE);
 rect(ox-2,oy-2,size+4,size+4,UI_EDGE);
 Vec3 pad=surface_site(&game,1);float cell=(EVA_FIELD_RADIUS*2.f)/EVA_MAP_GRID;
 for(int z=0;z<EVA_MAP_GRID;z++)for(int x=0;x<EVA_MAP_GRID;x++){
  int px=ox+x*step,py=oy+(EVA_MAP_GRID-1-z)*step;
  if(!eva_map_known_cell(x,z)){rect(px,py,step,step,fog);continue;}
  float wx=pad.x-EVA_FIELD_RADIUS+(x+.5f)*cell,wz=pad.z-EVA_FIELD_RADIUS+(z+.5f)*cell;
  float dx=wx-pad.x,dz=wz-pad.z;unsigned ink;
  if(dx*dx+dz*dz>EVA_FIELD_RADIUS*EVA_FIELD_RADIUS)ink=RGB(35,39,45);
  else if(terrain_is_water(&game,wx,wz))ink=high_contrast?RGB(38,104,142):RGB(22,65,91);
  else {float h=terrain_height(&game,wx,wz)-terrain_height(&game,pad.x,pad.z);ink=h>55?RGB(109,93,69):h<-18?RGB(66,83,67):RGB(80,94,69);}
  rect(px,py,step,step,ink);
 }
 /* The field edge is a known suit limit even where the ground remains fogged. */
 circle(ox+size/2,oy+size/2,size/2-1,UI_MUTED);
 for(int i=0;i<FIELD_PORT_BUILDINGS;i++){const FieldBuilding *b=&field_port_buildings[i];eva_map_structure(pad.x+b->x,pad.z+b->z,b->w,b->d,ox,oy,size,WHITE);}
 for(int i=0;i<3;i++){const FieldBuilding *b=&field_garage_walls[i];eva_map_structure(pad.x+b->x,pad.z+b->z,b->w,b->d,ox,oy,size,UI_MUTED);}
 int sx,sy;eva_map_world_to_screen(game.ship_pos.x,game.ship_pos.z,ox,oy,size,&sx,&sy);eva_map_mark(sx,sy,'S',UI_GOLD);
 eva_map_world_to_screen(game.rover_pos.x,game.rover_pos.z,ox,oy,size,&sx,&sy);eva_map_mark(sx,sy,'R',UI_CYAN);
 eva_map_world_to_screen(pad.x,pad.z,ox,oy,size,&sx,&sy);eva_map_mark(sx,sy,'P',WHITE);
 for(int id=1;id<10;id++)if(id!=6){Vec3 p=surface_poi(&game,id);if(!eva_map_known_world(p.x,p.z))continue;eva_map_world_to_screen(p.x,p.z,ox,oy,size,&sx,&sy);eva_map_mark(sx,sy,'L',id==surface_nav_poi?UI_GOLD:UI_SIGNAL);}
 eva_map_world_to_screen(game.pos.x,game.pos.z,ox,oy,size,&sx,&sy);circle(sx,sy,3,WHITE);line(sx,sy,sx+(int)(sinf(game.yaw)*8),sy-(int)(cosf(game.yaw)*8),WHITE);
 text(25,6,UI_GOLD,"CURRENT VISIT");text(25,8,WHITE,"%.24s",game.bodies[game.planet].name);
 text(25,11,UI_CYAN,"MARKERS");text(25,13,WHITE,"ARROW  YOU");text(25,15,UI_GOLD,"GOLD POINTER  SHIP");text(25,17,UI_CYAN,"BLOCK  ROVER");text(25,19,UI_SIGNAL,"SQUARE  FOUND SITE");
 text(25,22,UI_CYAN,"SURVEY FOG");text_wrap(25,24,31,3,UI_MUTED,"Only ground crossed during this landing is charted. Shore and terrain come from the live world.",0);
 footer("START / O CLOSE   TRI FIELD GUIDE");
}
static void eva_map_open_now(void){eva_map_visit();eva_map_open=1;game.eva_run_arm=0;game.eva_run_hold=0;game.eva_running=0;game.cue=SFX_UI;}
static int eva_map_input(unsigned pressed){if(pressed&PSP_CTRL_TRIANGLE){eva_map_open=0;game.cue=SFX_UI;return 1;}if(pressed&(PSP_CTRL_START|PSP_CTRL_CIRCLE)){eva_map_open=0;game.cue=SFX_UI;}return 0;}
