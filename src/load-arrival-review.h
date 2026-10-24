/* Opt-in integration review; run only in a disposable emulator directory. */
static unsigned load_review_hash(void){unsigned h=2166136261u;for(int y=0;y<H;y++)for(int x=0;x<W;x++)h=(h^fb[y*STRIDE+x])*16777619u;return h;}
static unsigned load_review_monitor_hash(int outside){unsigned h=2166136261u;for(int y=0;y<H;y++)for(int x=0;x<W;x++)if((x>=232&&x<480&&y>=59&&y<194)!=outside)h=(h^fb[y*STRIDE+x])*16777619u;return h;}
static int load_arrival_review(void){
 FILE *flag=fopen("load-arrival-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *out=fopen("load-arrival-review.txt","w");if(!out)return 1;
 unsigned *old=fb,*mem=calloc(STRIDE*H+32,sizeof(unsigned));Game *snapshot=malloc(sizeof(Game));
 if(!mem||!snapshot){fprintf(out,"RESULT 1 failures\n");fclose(out);free(mem);free(snapshot);return 1;}
 fb=mem+16;for(int i=0;i<16;i++)mem[i]=mem[STRIDE*H+16+i]=0x7194abceu;
 int failures=0;
 #define LACHECK(c,label) do{int ok=!!(c);fprintf(out,"%s %s\n",ok?"PASS":"FAIL",label);failures+=!ok;fflush(out);}while(0)
 game_init(&game);story_complete(&game);game.opening.stage=0;game.system=7;system_bodies(&game);game.planet=-1;game.docked=1;game.station_variant=0;
 commander_active_slot=profile_slot=0;game.credits=76543;game.hull=73;game.fuel=3.25f;
 LACHECK(save_game(&game,commander_slots[0]),"station checkpoint fixture saves through production serializer");
 LACHECK(load_game(snapshot,commander_slots[0]),"unmodified production loader supplies the expected checkpoint values");int expected_hull=snapshot->hull;float expected_fuel=snapshot->fuel;
 change_page(INTRO);intro_menu_age=1;intro_wait_release=0;intro_choice=0;
 intro_profile_ready=1;profile_info[0].state=1;profile_info[1].state=profile_info[2].state=0;
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 LACHECK(page==HOME&&load_arrival_time==0,"Continue starts the station arrival after a successful main-menu load");
 LACHECK(game.credits==76543&&game.hull==expected_hull&&game.fuel==expected_fuel,"load presentation retains the production loader's commander and ship values");
 memcpy(snapshot,&game,sizeof(game));int start_row=row;
 for(int i=0;i<30;i++)input(PSP_CTRL_CROSS|PSP_CTRL_START|PSP_CTRL_SELECT|PSP_CTRL_RTRIGGER|PSP_CTRL_CIRCLE,0,.05f,1,1);
 LACHECK(page==HOME&&row==start_row&&!memcmp(snapshot,&game,sizeof(game)),"all controls are consumed while the station shot runs; simulation and save state stay frozen");
 LACHECK(load_arrival_sounds==0,"dashboard sounds do not play over the exterior establishing shot");
 static const float phases[]={0,.45f,1.8f,3.75f,4.35f,4.75f,4.98f,5.05f,5.4f,5.85f,6.35f,6.64f};
 unsigned orbit_hash=0,zoom_hash=0;
 for(int i=0;i<12;i++){load_arrival_time=phases[i];drawcount=0;home();load_arrival_draw();char path[48];snprintf(path,sizeof(path),"load-arrival-%02d.bmp",i);dump_native_bmp(path);unsigned hash=load_review_hash();if(i==2)orbit_hash=hash;if(i==4)zoom_hash=hash;if(i==0||i==6||i==7){int dark=1;for(int y=0;y<H;y++)for(int x=0;x<W;x++)dark&=(fb[y*STRIDE+x]&0xffffffu)==0;LACHECK(dark,"fade boundaries are fully black with no flash or exposed UI");}}
 LACHECK(orbit_hash!=zoom_hash,"orbit and accelerating zoom produce distinct native station views");
 /* Hold the orbit angle fixed: growing station pixels must come from zoom. */
 int solid[2]={0,0};for(int shot=0;shot<2;shot++){drawcount=0;rect(0,0,W,H,0);preview_clip(W/2,H/2,0,0,W-1,H-1);menu_station_arrival_camera=1;menu_station_arrival=(MenuStationCamera){.75f,-.28f,shot?4.f:1.15f,0,0};menu_station_generated(0,64,W,208,.75f);menu_station_arrival_camera=0;for(int y=60;y<H;y++)for(int x=0;x<W;x++)solid[shot]+=(fb[y*STRIDE+x]&0xffffffu)!=0;}
 LACHECK(solid[1]>solid[0]*2,"station silhouette grows over twofold at a fixed angle; zoom cannot pass on orbit-only changes");preview_clip(0,0,0,0,W-1,H-1);
 /* Read all real architecture classes, plus the saved relay variant. */
 for(int kind=0;kind<4;kind++){
  int found=-1;
  if(kind==3){game.station_variant=1;found=game.system;}
  else for(int s=0;s<256;s++){game.system=s;game.station_variant=0;if(station_profile_for(&game,0).station_class==kind){found=s;break;}}
  LACHECK(found>=0,"a real station exists for each exterior architecture fixture");
  system_bodies(&game);drawcount=0;load_arrival_time=2;load_arrival_draw();char path[64];snprintf(path,sizeof(path),"load-station-class-%d.bmp",kind);dump_native_bmp(path);
  int count=0;for(int y=75;y<250;y++)for(int x=40;x<440;x++)if(fb[y*STRIDE+x]!=RGB(3,8,17))count++;
  LACHECK(count>200&&!menu_station_arrival_camera&&drawcount==0,"actual saved station renders with camera/mesh state restored");
 }
 game.system=7;game.station_variant=0;system_bodies(&game);snprintf(game.systems[7].name,sizeof(game.systems[7].name),"LONGSTAR");drawcount=0;load_arrival_time=2;load_arrival_draw();dump_native_bmp("load-long-station-name.bmp");
 load_arrival_time=LOAD_DECK_START;load_arrival_sounds=0;
 input(0,0,.01f,0,0);LACHECK(load_arrival_sounds==1&&audio_sfx==SFX_POWER_BUS,"first dashboard lighting triggers the low power-bus sweep");
 for(int i=0;i<10;i++)input(0,0,.05f,0,0);LACHECK(load_arrival_sounds==3&&audio_sfx==SFX_POWER_RELAY,"second lighting bank triggers its relay sweep once");
 for(int i=0;i<10;i++)input(0,0,.05f,0,0);LACHECK(load_arrival_sounds==7&&audio_sfx==SFX_POWER_READY,"final lighting bank triggers the ready sweep once");
 load_arrival_time=LOAD_END-.01f;input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.02f,0,0);
 LACHECK(load_arrival_time<0&&load_arrival_release&&page==HOME,"held confirm cannot activate a menu item when the startup finishes");
 input(0,PSP_CTRL_CROSS,.016f,0,0);LACHECK(page==HOME,"held buttons remain blocked until released");input(0,0,.016f,0,0);LACHECK(!load_arrival_release,"release returns control to the live command deck");
 preview_time=12;home();unsigned normal=load_review_hash();load_arrival_time=LOAD_END-.001f;home();load_arrival_draw();LACHECK(load_review_hash()==normal,"fully lit dashboard is pixel-identical to the normal command deck");
 load_arrival_time=-1;profile_from_intro=0;profile_load();LACHECK(load_arrival_time<0,"loads outside the main menu do not replay the cinematic");
 change_page(INTRO);intro_menu_age=1;intro_wait_release=0;intro_choice=2;input(PSP_CTRL_CROSS,0,.016f,0,0);
 LACHECK(page==STATUS&&profile_from_intro,"Load Commander retains the three-slot loader");profile_slot=0;profile_load();LACHECK(load_arrival_time==0,"the main-menu slot loader also starts the arrival sequence");
 load_arrival_time=-1;game.opening.stage=OP_PITCH;opening_world_apply(&game);game.docked=0;game.planet=-1;LACHECK(save_game(&game,commander_slots[1]),"nonstation opening checkpoint fixture saves");profile_slot=1;profile_from_intro=1;profile_load();LACHECK(load_arrival_time<0&&!game.docked,"nonstation checkpoint loads keep their original location without a false station scene");
 profile_slot=2;remove(commander_slots[2]);profile_load();LACHECK(load_arrival_time<0,"failed loads cannot start a station presentation");
 change_page(INTRO);intro_menu_age=1;intro_wait_release=0;int station=radio_station,off=radio_off;input(PSP_CTRL_SELECT,PSP_CTRL_SELECT,.016f,0,0);
 LACHECK(page==INTRO&&intro_time==0&&intro_menu_age<0&&intro_wait_release,"Select restarts the complete intro with a release gate");
 LACHECK(radio_station==station&&radio_off==off&&audio_replay_intro,"intro replay requests decoder-owned restart and preserves radio settings");
 LACHECK(load_game(snapshot,commander_slots[0])&&snapshot->credits==76543&&snapshot->hull==expected_hull&&snapshot->fuel==expected_fuel,"intro replay leaves the saved commander intact");
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);input(0,0,1.f,0,0);intro_menu_age=1;intro_wait_release=0;intro_choice=1;input(PSP_CTRL_CROSS,0,.016f,0,0);input(0,0,.016f,0,0);
 LACHECK(page==STATUS&&game.opening.stage==OP_CREATE,"New Commander still opens the real creator");input(PSP_CTRL_CIRCLE,0,.016f,0,0);
 LACHECK(page==INTRO,"Circle returns from the creation landing screen to the main menu");
 LACHECK(load_game(snapshot,commander_slots[0])&&snapshot->credits==76543,"cancelled character creation does not overwrite a checkpoint");
 game_init(&game);story_complete(&game);game.opening.stage=0;game.docked=1;game.planet=-1;game.system=7;system_bodies(&game);deck_reset();change_page(HOME);preview_time=12;load_arrival_begin();
 int ranks=0,nav_on[DECK_GROUPS]={0},nav_off[DECK_GROUPS]={0},mixed=0;
 for(int i=0;i<DECK_GROUPS;i++)ranks|=1<<load_arrival_nav_order[i];
 LACHECK(ranks==(1<<DECK_GROUPS)-1,"startup shuffles every navigation category exactly once");
 for(int tick=0;tick<120;tick++){int lit=0;float t=tick*.01f;for(int g=0;g<DECK_GROUPS;g++){float light=load_arrival_nav_light(g,t);nav_on[g]|=light>.8f;nav_off[g]|=light<.1f;lit+=light>.8f;}mixed|=lit>0&&lit<DECK_GROUPS;}
 int all=1;for(int g=0;g<DECK_GROUPS;g++)all&=nav_on[g]&&nav_off[g]&&load_arrival_nav_light(g,1.3f)==1;
 LACHECK(all&&mixed,"each navigation button switches on and off independently, then stays fully illuminated");
 unsigned seed=load_arrival_seed;load_arrival_begin();LACHECK(load_arrival_seed!=seed,"successive loads vary the startup flicker without consuming gameplay RNG");
 home();unsigned outside=load_review_monitor_hash(1);load_arrival_monitor(.24f);int black=1,beam=0;for(int y=59;y<194;y++)for(int x=232;x<W;x++){unsigned c=fb[y*STRIDE+x]&0xffffffu;if(y<125||y>127)black&=!c;else beam+=!!c;}
 LACHECK(black&&beam>20,"monitor starts as a thin bright horizontal power-on beam");
 LACHECK(load_review_monitor_hash(1)==outside,"monitor effect preserves navigation, menu labels, footer and information panel");dump_native_bmp("load-power-monitor-line.bmp");
 home();unsigned source[135];for(int y=0;y<135;y++)source[y]=fb[(59+y)*STRIDE+352];load_arrival_monitor(.44f);
 float p=(.44f-.32f)/.46f;int height=1+(int)(134*(1-(1-p)*(1-p)*(1-p))),top=59+(135-height)/2,a=(int)(256*(.52f+.48f*p));
 int samples=1;for(int y=top+height/2;y<top+height;y+=7){unsigned c=source[(y-top)*135/height],expected=RGB(((c&255)*a)>>8,(((c>>8)&255)*a)>>8,(((c>>16)&255)*a)>>8);samples&=fb[y*STRIDE+352]==expected;}
 LACHECK(samples,"expanding monitor displays the real background art compressed into its opening raster");dump_native_bmp("load-power-monitor-opening.bmp");
 home();unsigned image=load_review_hash();load_arrival_monitor(.92f);LACHECK(load_review_hash()==image,"monitor settles to the exact original background with no residual lines or dimming");
 home();load_arrival_dashboard(.58f);dump_native_bmp("load-nav-shuffled.bmp");
 for(int kind=0;kind<4;kind++){
  if(kind==3)game.station_variant=1;
  else for(int s=0;s<256;s++){game.system=s;game.station_variant=0;if(station_profile_for(&game,0).station_class==kind)break;}
  system_bodies(&game);unsigned hash[2];
  for(int shot=0;shot<2;shot++){rect(0,0,W,H,0);drawcount=0;preview_clip(W/2,H/2,0,0,W-1,H-1);menu_station_arrival_camera=1;menu_station_arrival=(MenuStationCamera){.75f,-.28f,shot?5.f:4.f,0,0};if(kind==3)menu_station_relay(W,208,.75f);else menu_station_generated(0,64,W,208,.75f);menu_station_arrival_camera=0;hash[shot]=load_review_hash();}
  LACHECK(hash[0]!=hash[1],"late zoom keeps changing the projected station after safe dolly travel stops");
  load_arrival_time=4.63f;load_arrival_draw();char name[64];snprintf(name,sizeof(name),"load-zoom-fade-class-%d.bmp",kind);dump_native_bmp(name);
 }
 game.system=7;game.station_variant=0;system_bodies(&game);
 load_arrival_time=4.80f;load_arrival_draw();dump_native_bmp("load-zoom-through-black.bmp");
 for(int id=SFX_POWER_BUS;id<=SFX_POWER_READY;id++){unsigned phase=0,noise=71;int peak=0,energy=0,n=audio_sfx_length(id);for(int age=0;age<n;age++){int a=abs(audio_sfx_sample(id,age,&phase,&noise));peak=peak>a?peak:a;energy|=a;}LACHECK(n>=13230&&n<=19845&&energy&&peak<5000&&audio_sfx_sample(id,n,&phase,&noise)==0,"startup sweep is bounded, audible, quiet and ends cleanly");}
 int guards=1;for(int i=0;i<16;i++)guards&=mem[i]==0x7194abceu&&mem[STRIDE*H+16+i]==0x7194abceu;
 LACHECK(guards,"every native cinematic/dashboard frame preserves framebuffer guard words");
 fprintf(out,"RESULT %d failures\n",failures);fclose(out);load_arrival_time=-1;intro_leave();fb=old;free(mem);free(snapshot);return 1;
 #undef LACHECK
}
