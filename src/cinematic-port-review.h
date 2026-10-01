/* Opt-in native proof: actual input dispatcher and production renderer. */
static void pilot_review_reset(int body){
 game_init(&game);game.system=7;system_bodies(&game);launch(&game);story_complete(&game);
 deck_reset();page=FLIGHT;paused=0;ps_open=ps_release=0;planet_entry_body=planet_seat.active=planet_orbit_release=0;planet_orbit_veil=0;
 planet_landing_menu=0;planet_controls_ready=1;surface_target_reset();tools_reset_gesture();tools_wait_release=0;
 game.approach=body;game.voice_time=game.message_time=0;hud_mode=hud_hidden=high_contrast=0;preview_reset();
}
static int cinematic_port_review(void){
 FILE *flag=fopen("cinematic-port-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *f=fopen("cinematic-port-review.txt","w");unsigned *saved=fb,*pixels=calloc(STRIDE*H+32,sizeof(unsigned));if(!f||!pixels)return 1;
 fb=pixels+16;for(int i=0;i<16;i++)pixels[i]=pixels[STRIDE*H+16+i]=0xa55ac33cu;int failures=0;
 #define PCHECK(ok,name) do{int pass=(ok);fprintf(f,"%s %s\n",pass?"PASS":"FAIL",name);failures+=!pass;fflush(f);}while(0)
 unsigned spam=PSP_CTRL_CROSS|PSP_CTRL_CIRCLE|PSP_CTRL_TRIANGLE|PSP_CTRL_SQUARE|PSP_CTRL_SELECT|PSP_CTRL_START|PSP_CTRL_RTRIGGER;
 for(int body=1;body<=4;body++){
  pilot_review_reset(body);input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
  PCHECK(game.planet<0&&planet_entry_body==body,"entry waits in orbit before opaque cloud handoff");
  for(int i=0;i<20;i++)input(spam,spam,.05f,0,0);
  PCHECK(game.planet<0&&planet_entry_time>=.85f,"opaque veil precedes synchronous world initialisation");
  for(int i=0;i<300&&planet_transfer_busy();i++)input(spam,spam,.05f,1,1);
  PCHECK(page==FLIGHT&&game.surface==2&&game.planet==body&&!planet_transfer_busy(),"arrival ignores all buttons and auto-disembarks exactly once");
  Vec3 pad=surface_site(&game,1);
  PCHECK(fabsf(game.ship_pos.x-pad.x)<.01f&&fabsf(game.ship_pos.z-pad.z)<.01f&&eva_can_board(&game),"touchdown centred and automatic airlock exit can reboard");
  input(PSP_CTRL_TRIANGLE,spam,.05f,0,0);PCHECK(game.surface==2,"carried buttons cannot immediately reboard");
  input(0,0,.05f,0,0);input(PSP_CTRL_TRIANGLE,PSP_CTRL_TRIANGLE,.05f,0,0);
  PCHECK(game.surface==1&&planet_seat.active,"ship boarding starts pilot-seat motion");
  for(int i=0;i<23;i++)input(spam,spam,.05f,0,0);
  PCHECK(game.surface==1&&!game.planet_sequence&&!planet_seat.active,"seat animation cannot launch or exit from held buttons");
  input(0,0,.05f,0,0);input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.05f,0,0);
  PCHECK(game.planet_sequence==3,"fresh launch gesture selects departure, never arrival");
  for(int i=0;i<205&&planet_transfer_busy();i++)input(spam,spam,.05f,0,0);
  PCHECK(game.planet<0&&!game.planet_sequence&&!planet_transfer_busy(),"departure reaches orbit under the cloud veil despite button spam");
  input(0,0,.05f,0,0);
  /* Render deterministic phases from the same planet and safe model state. */
  pilot_review_reset(body);enter_planet(&game);game.ship_pos=surface_site(&game,1);game.ship_pos.y-=4;
  float hour=field_local_hour(&game,7,body);static const int days[]={600,900,1200,1800};game.world_clock=(10-hour+24)*days[(field_seed(7,body)>>24)&3]/24.f;
  int clear=1,continuous=1;float max_step=0;unsigned long long total=0,worst=0;int samples=0;
  for(int kind=1;kind<=3;kind++){
   game.surface=kind==2?1:0;planet_sequence_begin(kind);PlanetPilotCamera previous=planet_pilot_sample(kind,0);
   int n=(int)(planet_sequence_duration(kind)*30);
   for(int i=0;i<=n;i++){
    float t=i/30.f;PlanetPilotCamera c=planet_pilot_sample(kind,t);float step=length(sub(c.eye,previous.eye));max_step=fmaxf(max_step,step);previous=c;
    if(!isfinite(c.eye.y)||step>28)continuous=0;
    if(kind!=2&&c.eye.y<terrain_height(&game,c.eye.x,c.eye.z)+20)clear=0;
    for(int p=1;p<10;p++)if(p!=6){Vec3 site=surface_poi(&game,p);FieldBuilding b=field_site_building(&game,p);if(fabsf(c.eye.x-site.x)<b.w+8&&fabsf(c.eye.z-site.z)<b.d+8&&c.eye.y<terrain_height(&game,site.x,site.z)+b.height+10)clear=0;}
    if(i%10)continue;
    game.planet_sequence_time=t;game.time=20+t;Vec3 pos=game.pos;float yaw=game.yaw,pitch=game.pitch;unsigned long long a,b;
    sceRtcGetCurrentTick(&a);space();sceRtcGetCurrentTick(&b);total+=b-a;if(b-a>worst)worst=b-a;samples++;
    if(length(sub(pos,game.pos))>.001f||yaw!=game.yaw||pitch!=game.pitch)continuous=0;
    if(body==1||i==n/2/10*10){char path[80];snprintf(path,sizeof(path),"pilot-%d-phase-%d-%03d.bmp",body,kind,i);dump_native_bmp(path);}
   }
  }
  fprintf(f,"Planet %d: max camera step %.3fm; render %.3fms avg / %.3fms worst\n",body,max_step,total*1000.f/sceRtcGetTickResolution()/samples,worst*1000.f/sceRtcGetTickResolution());
  PCHECK(clear,"camera paths clear terrain and actual landmark footprints");PCHECK(continuous,"finite continuous camera motion; draw preserves gameplay camera");
  PlanetPilotCamera end=planet_pilot_sample(1,10),start=planet_pilot_sample(2,0);
  PCHECK(length(sub(end.eye,start.eye))<.001f&&fabsf(end.pitch-start.pitch)<.001f,"approach and touchdown camera endpoints match exactly");
  game.planet_sequence=0;game.pos=surface_site(&game,1);game.surface=0;game.speed=0;land_planet(&game);game.voice_time=game.message_time=0;planet_boarded_view();char shot[48];snprintf(shot,sizeof(shot),"pilot-%d-parked.bmp",body);dump_native_bmp(shot);
  disembark_planet(&game);game.pos=add(game.rover_pos,(Vec3){0,0,38});game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;planet_controls_ready=1;
  input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);PCHECK(game.rover_driving&&planet_seat.active&&length(sub(game.pos,game.rover_pos))<1,"roamer boarding moves into actual cabin, not explorer position");
  for(int i=0;i<8;i++){planet_seat.time=i*.14f;planet_seat_view();if(body==1){char path[48];snprintf(path,sizeof(path),"roamer-seat-%02d.bmp",i);dump_native_bmp(path);}}
  planet_seat.active=0;planet_controls_ready=1;Vec3 before=game.pos;game.yaw=0;
  for(int i=0;i<55;i++)game_eva_tick(&game,.05f,0,0,1,0,0);
  PCHECK(game.pos.z>before.z+160,"roamer can leave garage and cross wide port circulation lane");
 }
 #define TEST_INIT() do{pilot_review_reset(1);game.approach=-1;}while(0)
 #define INPUT_CHECK(ok,name) PCHECK(ok,name)
 #include "planet-sequence-tests.h"
 #include "landing-safety-tests.h"
 #include "planet-approach-input-tests.h"
 #include "planet-eva-input-tests.h"
 #undef INPUT_CHECK
 #undef TEST_INIT
 int routes_clear=1;
 for(int sys=0;sys<256;sys+=17)for(int body=1;body<=4;body++){
  pilot_review_reset(body);game.system=sys;system_bodies(&game);game.approach=body;enter_planet(&game);
  for(int kind=1;kind<=3;kind+=2)for(int i=0;i<=40;i++){
   PlanetPilotCamera c=planet_pilot_sample(kind,i*planet_sequence_duration(kind)/40);
   if(c.eye.y<terrain_height(&game,c.eye.x,c.eye.z)+20)routes_clear=0;
   for(int id=1;id<10;id++)if(id!=6){Vec3 p=surface_poi(&game,id);FieldBuilding b=field_site_building(&game,id);
    if(fabsf(c.eye.x-p.x)<b.w+8&&fabsf(c.eye.z-p.z)<b.d+8&&c.eye.y<terrain_height(&game,p.x,p.z)+b.height+10){fprintf(f,"Route obstruction system %d body %d kind %d time %.2f site %d\n",sys,body,kind,i*planet_sequence_duration(kind)/40,id);routes_clear=0;}
   }
  }
 }
 PCHECK(routes_clear,"sampled 64-world arrival/departure paths clear generated landmarks");
 int guards=1;for(int i=0;i<16;i++)if(pixels[i]!=0xa55ac33cu||pixels[STRIDE*H+16+i]!=0xa55ac33cu)guards=0;PCHECK(guards,"framebuffer guard words intact");
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);fb=saved;free(pixels);return 1;
 #undef PCHECK
}
