/* Opt-in production-renderer and dispatcher tests; no player files touched. */
static int field_map_review(void){
 FILE *flag=fopen("field-map-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *f=fopen("field-map-review.txt","w");if(!f)return 1;
 unsigned *saved=fb,*pixels=calloc(STRIDE*H+32,sizeof(unsigned));if(!pixels){fclose(f);return 1;}
 fb=pixels+16;for(int i=0;i<16;i++)pixels[i]=pixels[STRIDE*H+16+i]=0xa55ac33cu;
 int failures=0;
 #define MCHECK(ok,label) do{int pass=(ok);fprintf(f,"%s %s\n",pass?"PASS":"FAIL",label);failures+=!pass;fflush(f);}while(0)
 for(int body=1;body<=4;body++){
  eva_map_leave();pilot_review_reset(body);game.approach=-1;game.planet=body;game.surface=2;page=FLIGHT;
  Vec3 pad=surface_site(&game,1);game.ship_pos=pad;game.rover_pos=add(pad,(Vec3){FIELD_GARAGE_X,0,-10});game.pos=add(pad,(Vec3){0,22,-100});game.yaw=0;
  input(PSP_CTRL_START,PSP_CTRL_START,.016f,0,0);MCHECK(eva_map_open,"Start opens field map");
  uint64_t t=sceKernelGetSystemTimeWide();space();unsigned long cold=(unsigned long)(sceKernelGetSystemTimeWide()-t);
  char file[64];snprintf(file,sizeof(file),"map-%d-new.bmp",body);dump_native_bmp(file);
  int builds=fm_builds;t=sceKernelGetSystemTimeWide();for(int i=0;i<8;i++)space();unsigned long warm=(unsigned long)((sceKernelGetSystemTimeWide()-t)/8);
  fprintf(f,"INFO Lave %d cold %lu us, warm %lu us, cache %u bytes\n",body,cold,warm,(unsigned)sizeof(fm_land));
  MCHECK(builds==fm_builds,"warm draws reuse terrain cache");
  int x,y;fm_xy(game.pos.x,game.pos.z,&x,&y);MCHECK(x==240&&y==137,"player remains centred");
  fm_xy(game.pos.x+60,game.pos.z+60,&x,&y);MCHECK(x==250&&y==127,"north/east and metric scale agree");
  Vec3 still=game.pos;float yaw=game.yaw;
  for(int k=0;k<12;k++)input(0,0,.05f,.8f,.8f);
  MCHECK(eva_map_pan_x>400&&eva_map_pan_z>400,"nub pans east and north");
  MCHECK(game.pos.x==still.x&&game.pos.z==still.z&&game.yaw==yaw,"map panning never moves the commander");
  fm_xy(game.pos.x,game.pos.z,&x,&y);MCHECK(x<240&&y>137,"player marker shifts correctly when chart pans");
  float px=eva_map_pan_x,pz=eva_map_pan_z;input(0,0,.05f,.05f,.05f);
  MCHECK(px==eva_map_pan_x&&pz==eva_map_pan_z,"neutral nub does not drift");
  for(int k=0;k<90;k++)input(0,0,.1f,1,1);
  MCHECK(game.pos.x+eva_map_pan_x<=pad.x+EVA_FIELD_RADIUS+.01f&&game.pos.z+eva_map_pan_z<=pad.z+EVA_FIELD_RADIUS+.01f,"panning clamps at survey bounds");
  space();snprintf(file,sizeof(file),"map-%d-panned.bmp",body);dump_native_bmp(file);
  FieldBuilding bay=field_port_building(&game,2),tower=field_port_building(&game,1);
  MCHECK(bay.height==6&&tower.height<300,"open low landing platforms replace massive hangars");
  if(body==1){Vec3 obs=surface_poi(&game,2);
   MCHECK(fabsf(obs.z-pad.z-FIELD_OBSERVATORY_DISTANCE)<.1f,"Lave observatory sits on the distant 4200 metre ridge");
   MCHECK(terrain_height(&game,obs.x,obs.z)>terrain_height(&game,pad.x,pad.z)+100,"observatory stands on a higher ridge");
  }
  game.pos=add(pad,(Vec3){-120,0,310});game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;game.pitch=.06f;
  planet_view();snprintf(file,sizeof(file),"port-%d-ridge.bmp",body);dump_native_bmp(file);
  game.pos=add(pad,(Vec3){0,160,-950});game.pitch=-.11f;planet_view();snprintf(file,sizeof(file),"port-%d-overview.bmp",body);dump_native_bmp(file);
  game.pos=still;game.pitch=0;eva_map_pan_x=eva_map_pan_z=0;
  input(0,0,.016f,0,0);input(PSP_CTRL_TRIANGLE,PSP_CTRL_TRIANGLE,.016f,0,0);
  MCHECK(page==CODEX&&codex_scope==ATLAS_WORLD&&codex_body==body+1&&codex_system==7,"Triangle opens correct planet root");
  input(0,0,.016f,0,0);codex_screen();snprintf(file,sizeof(file),"codex-%d.bmp",body);dump_native_bmp(file);
  input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);MCHECK(codex_scope==ATLAS_CATEGORY,"planet categories remain functional");
  input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);MCHECK(codex_scope==ATLAS_WORLD&&page==CODEX,"category Back returns to planet root");
  input(0,0,.016f,0,0);input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);MCHECK(page==FLIGHT&&eva_map_open,"planet root Back returns to map");
  input(0,PSP_CTRL_CIRCLE,.016f,0,0);MCHECK(eva_map_open,"held Back does not also close map");input(0,0,.016f,0,0);
  input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);Vec3 before=game.pos;input(0,PSP_CTRL_CIRCLE|PSP_CTRL_RTRIGGER,.05f,1,1);
  MCHECK(!eva_map_open&&eva_map_release&&game.pos.x==before.x&&game.pos.z==before.z,"closing map consumes held gameplay buttons");input(0,0,.016f,0,0);
  for(int z=-10000;z<=10000;z+=320)for(int x=-10000;x<=10000;x+=320)eva_map_reveal(pad.x+x,pad.z+z);
  game.pos=add(pad,(Vec3){650,22,650});surface_nav_poi=2;eva_map_open_now();space();snprintf(file,sizeof(file),"map-%d-explored.bmp",body);dump_native_bmp(file);
  Vec3 site=surface_poi(&game,2);game.pos=add(site,(Vec3){350,22,-200});space();snprintf(file,sizeof(file),"map-%d-site.bmp",body);dump_native_bmp(file);
  high_contrast=1;game.pos=add(pad,(Vec3){-2050,22,0});space();snprintf(file,sizeof(file),"map-%d-edge.bmp",body);dump_native_bmp(file);high_contrast=0;
  game.dead=1;input(PSP_CTRL_START,PSP_CTRL_START,.016f,0,0);MCHECK(!eva_map_open,"death cannot retain a modal map");
 }
 int guard=1;for(int i=0;i<16;i++)guard&=pixels[i]==0xa55ac33cu&&pixels[STRIDE*H+16+i]==0xa55ac33cu;
 MCHECK(guard,"framebuffer guards intact across all maps and Codex pages");
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);fb=saved;free(pixels);
 #undef MCHECK
 return 1;
}
