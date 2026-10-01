/* Opt-in combined release proof; never enabled in a player installation. */
static int integration_241_review(void){
 FILE *flag=fopen("integration-241.flag","r");if(!flag)return 0;fclose(flag);
 FILE *f=fopen("integration-241.txt","w");if(!f)return 1;
 unsigned *saved=fb,*pixels=calloc(STRIDE*H+32,sizeof(unsigned));if(!pixels){fclose(f);return 1;}
 fb=pixels+16;for(int i=0;i<16;i++)pixels[i]=pixels[STRIDE*H+16+i]=0xa55ac33cu;
 int failures=0;static Game g;
 #define CHECK(ok,label) do{int pass=(ok);fprintf(f,"%s %s\n",pass?"PASS":"FAIL",label);failures+=!pass;fflush(f);}while(0)
 #define INPUT_CHECK CHECK
 #define TEST_INIT() do{eva_map_leave();pilot_review_reset(1);}while(0)
 #include "debug-tools-tests.h"
 TEST_INIT();change_page(DEBUG);row=13;debug_action();
 CHECK((game.debug_flags&(DEBUG_MODIFIED|DEBUG_UNLIMITED_FUEL))==(DEBUG_MODIFIED|DEBUG_UNLIMITED_FUEL),"menu enables fuel protection");
 row=14;debug_action();CHECK(game.debug_flags&DEBUG_UNLIMITED_RANGE,"menu enables unlimited range");
 debug_screen();dump_native_bmp("debug-toggles.bmp");
 row=13;debug_action();row=14;debug_action();
 CHECK(game.debug_flags==DEBUG_MODIFIED,"menu disables both cheats and preserves modified marker");
 #include "eva-local-map-tests.h"
 for(int body=1;body<=4;body++){
  TEST_INIT();game.planet=body;game.surface=2;page=FLIGHT;
  Vec3 pad=surface_site(&game,1);game.ship_pos=pad;game.rover_pos=add(pad,(Vec3){FIELD_GARAGE_X,0,-10});
  game.pos=add(pad,(Vec3){0,0,-120});game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;game.yaw=0;game.pitch=0;
  eva_map_visit();int cx,cz;
  CHECK(!eva_map_cell(pad.x-EVA_FIELD_RADIUS-1,pad.z,&cx,&cz),"map rejects coordinates just outside the west bound");
  input(PSP_CTRL_START,PSP_CTRL_START,.016f,0,0);CHECK(eva_map_open,"Start opens actual map input on each Lave planet");
  Vec3 before=game.pos;float clock=game.world_clock;
  input(PSP_CTRL_CROSS|PSP_CTRL_CIRCLE,PSP_CTRL_CROSS|PSP_CTRL_CIRCLE,.05f,1,1);
  input(0,PSP_CTRL_CIRCLE|PSP_CTRL_UP|PSP_CTRL_RTRIGGER,.05f,1,1);
  CHECK(!eva_map_open&&eva_map_release&&game.pos.x==before.x&&game.pos.z==before.z&&game.world_clock==clock,"map close consumes buttons until release without scan/jump/movement");
  input(0,0,.016f,0,0);CHECK(!eva_map_release,"release restores EVA controls");
  input(PSP_CTRL_START,PSP_CTRL_START,.016f,0,0);input(PSP_CTRL_TRIANGLE,PSP_CTRL_TRIANGLE,.016f,0,0);
  CHECK(page==CODEX&&codex_scope==ATLAS_WORLD&&codex_system==game.system&&codex_body==body+1&&!eva_map_open,"Triangle opens this planet's Discovery Codex, not ship boarding");
  input(0,0,.016f,0,0);change_page(FLIGHT);
  for(int z=-1200;z<=1200;z+=160)for(int x=-1200;x<=1200;x+=160)eva_map_reveal(pad.x+x,pad.z+z);
  for(int contrast=0;contrast<2;contrast++){
   high_contrast=contrast;eva_map_open_now();eva_map_draw();char file[64];
   snprintf(file,sizeof(file),"map-lave-%d-%s.bmp",body,contrast?"contrast":"normal");dump_native_bmp(file);
   eva_map_open=0;game.message_time=0;game.hazard=contrast?70:0;game.energy=contrast?25:100;
   drawcount=0;space();snprintf(file,sizeof(file),"hud-lave-%d-%s.bmp",body,contrast?"contrast":"normal");dump_native_bmp(file);
  }
  game.dead=1;input(PSP_CTRL_START,PSP_CTRL_START,.016f,0,0);CHECK(!eva_map_open,"destroyed commander cannot enter survey map");
 }
 CHECK(surface_condition_ink(100)!=surface_condition_ink(50)&&surface_condition_ink(50)!=surface_condition_ink(20),"HUD meters use distinct healthy/caution/critical colours");
 int guard=1;for(int i=0;i<16;i++)guard&=pixels[i]==0xa55ac33cu&&pixels[STRIDE*H+16+i]==0xa55ac33cu;
 CHECK(guard,"combined native map and HUD preserve framebuffer guards");
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);fb=saved;free(pixels);
 #undef TEST_INIT
 #undef INPUT_CHECK
 #undef CHECK
 return 1;
}
