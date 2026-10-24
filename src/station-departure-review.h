/* Opt-in native-frame and production-input certification. Disposable saves. */
static unsigned departure_review_hash(void){unsigned h=2166136261u;for(int y=0;y<H;y++)for(int x=0;x<W;x++)h=(h^fb[y*STRIDE+x])*16777619u;return h;}
static int station_departure_review(void){
 FILE *flag=fopen("station-departure-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *out=fopen("station-departure-review.txt","w");if(!out)return 1;
 unsigned *old=fb,*mem=calloc(STRIDE*H+32,sizeof(unsigned));Game *snapshot=malloc(sizeof(Game));
 if(!mem||!snapshot){fprintf(out,"RESULT 1 failures\n");fclose(out);free(mem);free(snapshot);return 1;}
 fb=mem+16;for(int i=0;i<16;i++)mem[i]=mem[STRIDE*H+16+i]=0x7194abceu;
 int failures=0;unsigned class_hash[4]={0};float final_travelled=0;
 #define DPCHECK(c,label) do{int ok=!!(c);fprintf(out,"%s %s\n",ok?"PASS":"FAIL",label);failures+=!ok;fflush(out);}while(0)
 for(int kind=0;kind<4;kind++){
  game_init(&game);story_complete(&game);page=FLIGHT;game.opening.stage=0;game.station_variant=0;game.planet=-1;
  int found=-1;for(int s=0;s<256;s++)if(station_class_for_system(&game,s)==(kind==3?STATION_MEGA:kind)){found=s;break;}
  game.system=found;system_bodies(&game);game.docked=1;if(kind==3)game.station_variant=1;
  launch_departure(&game);float hull=game.hull,fuel=game.fuel;int credits=game.credits;
  DPCHECK(departure_grade()==(kind==3?STATION_POOR:kind),"actual station class selects its departure architecture; relay stays industrial");
  static const float phases[]={0,1.1f,2.3f,3.5f,4.4f,5.2f,6.3f,7.1f,7.9f};
  float previous=0;uint64_t elapsed=0,geometry_elapsed=0;unsigned prior_hash=0;int animated=1,pure=1,states=1,geometry_peak=0;
  for(int shot=0;shot<9;shot++){
   /* Match real frame-sized ticks: the simulation rejects steps over .1s. */
   float remaining=phases[shot]-previous;
   while(remaining>.00001f){float step=fminf(.05f,remaining);game_tick(&game,step,0,0,0,0);remaining-=step;}
   previous=phases[shot];
   memcpy(snapshot,&game,sizeof(game));int ox=proj_ox,oy=proj_oy,x0=clipx0,x1=clipx1,y0=clipy0,y1=clipy1;
   int top=view_top(),bot=view_bot(),cy=(top+bot)/2;
   float travelled=dot(sub(game.pos,game.dock_from),game.dock_to),distance=station_half_for(&game,game.station_variant)-travelled;
   preview_clip(240,cy,0,top,W,bot);
   if(distance>15){int hw=(int)(70*280/distance),hh=(int)(32*280/distance);preview_clip(240,cy,(int)fmaxf(0,240-hw),(int)fmaxf(top,cy-hh),(int)fminf(W,240+hw),(int)fminf(bot,cy+hh));}
   int submitted=render_submitted;uint64_t geometry_began=sceKernelGetSystemTimeWide();departure_district(travelled,departure_grade());geometry_elapsed+=sceKernelGetSystemTimeWide()-geometry_began;
   if(render_submitted-submitted>geometry_peak)geometry_peak=render_submitted-submitted;
   preview_clip(ox,oy,x0,y0,x1,y1);
   rect(0,0,W,H,BG);uint64_t began=sceKernelGetSystemTimeWide();space();elapsed+=sceKernelGetSystemTimeWide()-began;
   pure&=!memcmp(snapshot,&game,sizeof(game));states&=ox==proj_ox&&oy==proj_oy&&x0==clipx0&&x1==clipx1&&y0==clipy0&&y1==clipy1&&drawcount==0;
   unsigned hash=departure_review_hash();if(shot>0)animated&=hash!=prior_hash;prior_hash=hash;
   if(shot==2)class_hash[kind]=hash;
   char path[64];snprintf(path,sizeof(path),"departure-class-%d-%02d.bmp",kind,shot);dump_native_bmp(path);
  }
  DPCHECK(pure,"departure rendering preserves every byte of live game state");
  DPCHECK(states,"departure restores projection/clipping and empties its triangle queue");
  DPCHECK(geometry_peak<1900,"port scenery remains inside the bounded triangle queue with headroom");
  DPCHECK(animated,"all sampled departure phases visibly advance at native resolution");
  int guard=1;for(int i=0;i<16;i++)guard&=mem[i]==0x7194abceu&&mem[STRIDE*H+16+i]==0x7194abceu;
  DPCHECK(guard,"departure never writes beyond the native framebuffer allocation");
  for(int i=0;i<5&&game.dock_stage==4;i++)game_tick(&game,.05f,0,0,0,0);
  final_travelled=dot(sub(game.pos,game.dock_from),game.dock_to);
  DPCHECK(!game.dock_stage&&game.speed==100&&!game.dead&&game.hull==hull&&game.fuel==fuel&&game.credits==credits&&!memcmp(snapshot->npc,game.npc,sizeof(game.npc)),"eight-second launch hands off safely; traffic cannot damage, reward or populate the simulation");
  fprintf(out,"INFO class %d: nine full frames average %llu us; port art alone %llu us, peak %d triangles (emulator only)\n",kind,(unsigned long long)(elapsed/9),(unsigned long long)(geometry_elapsed/9),geometry_peak);
  if(kind<3){game.dock_stage=2;game.dock_timer=.9f;memcpy(snapshot,&game,sizeof(game));rect(0,0,W,H,BG);space();char path[64];snprintf(path,sizeof(path),"autopilot-docking-class-%d.bmp",kind);dump_native_bmp(path);DPCHECK(!memcmp(snapshot,&game,sizeof(game)),"actual third-person docking shot retains game state with its shared annunciator");}
 }
 DPCHECK(class_hash[0]!=class_hash[1]&&class_hash[1]!=class_hash[2]&&class_hash[2]!=class_hash[3],"poor, rich, capital and relay native compositions are distinct");
 int clear_handoff=1;game.station_variant=0;game.dock_timer=8;preview_clip(240,95,0,0,W,191);
 for(int system=0;system<256;system++){
  game.system=system;rect(0,0,W,H,BG);int before=render_submitted;
  departure_district(final_travelled-35,departure_grade());
  clear_handoff&=render_submitted==before;
 }
 DPCHECK(clear_handoff,"all 256 station profiles clear port scenery before handoff; no final-frame geometry pop");preview_reset();
 /* Test the real Launch action and the input release gate, not just renderers. */
 game_init(&game);story_complete(&game);page=HOME;row=0;game.opening.stage=0;departure_release=0;
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 DPCHECK(page==FLIGHT&&game.dock_stage==4&&!game.docked,"real Launch menu starts the guided departure");
 Vec3 dir=game.dock_to;int shots=game.shots;float fuel=game.fuel,hull=game.hull;
 for(int i=0;i<520;i++)input(PSP_CTRL_CROSS|PSP_CTRL_CIRCLE|PSP_CTRL_TRIANGLE|PSP_CTRL_SELECT,PSP_CTRL_CROSS|PSP_CTRL_CIRCLE|PSP_CTRL_TRIANGLE|PSP_CTRL_SELECT,.016f,1,1);
 DPCHECK(page==FLIGHT&&!game.dock_stage&&departure_release&&game.shots==shots&&dot(dir,forward(&game))>.999f,"button mashing cannot skip, steer, fire or open menus during autopilot");
 DPCHECK(game.fuel==fuel&&game.hull==hull&&!game.dead,"full departure consumes no boost fuel and causes no collision damage");
 input(0,0,.016f,0,0);DPCHECK(!departure_release,"releasing controls enables the fresh-input handoff");
 float yaw=game.yaw;input(0,0,.016f,.5f,0);DPCHECK(game.yaw!=yaw,"fresh steering works after autopilot releases control");
 /* Shared annunciator is identical in docking/departure, and stays readable
  * at its darker phase rather than disappearing completely. */
 rect(0,0,W,H,BG);autopilot_annunciator(34,0);unsigned bright=departure_review_hash();dump_native_bmp("autopilot-bright.bmp");
 rect(0,0,W,H,BG);autopilot_annunciator(34,1);DPCHECK(departure_review_hash()!=bright,"green autopilot indicator slowly changes brightness");dump_native_bmp("autopilot-dim.bmp");
 game.dock_stage=1;game.dock_timer=0;rect(0,0,W,H,BG);cockpit();dump_native_bmp("autopilot-docking.bmp");
 int notice=1;unsigned ink=RGB(126,241,162);for(int y=0;y<8;y++)for(int x=0;x<136;x++){
  unsigned char c="AUTOPILOT ENGAGED"[x/8];if(font8[c-32][y]&(128>>(x%8)))notice&=fb[(32+y)*STRIDE+172+x]==ink;
 }
 DPCHECK(notice,"station docking uses exactly the same centered green autopilot label");
 /* Canopy footer accepts the whole 17-character label without clipping. */
 rect(0,0,W,H,BG);planet_pilot_canopy(0,1);autopilot_annunciator(250,0);dump_native_bmp("autopilot-planet-footer.bmp");
 int bottom=1;for(int y=0;y<8;y++)for(int x=0;x<136;x++){
  unsigned char c="AUTOPILOT ENGAGED"[x/8];if(font8[c-32][y]&(128>>(x%8)))bottom&=fb[(250+y)*STRIDE+172+x]==ink;
 }
 DPCHECK(bottom,"planetary autopilot label fits centered between the canopy lamps");
 game_init(&game);story_complete(&game);game.system=7;system_bodies(&game);launch(&game);game.approach=1;
 DPCHECK(enter_planet(&game),"Lave landing fixture enters the actual planet");
 planet_sequence_begin(1);game.planet_sequence_time=4.5f;rect(0,0,W,H,BG);planet_sequence_view();dump_native_bmp("autopilot-planet-approach.bmp");
 DPCHECK(game.planet_sequence==1&&game.surface==0,"actual planetary approach retains its state after the cleaned presentation");
 /* Six actual billboard marks at distance, then readable native lettering.
  * This sheet is a renderer capture, not a separate mockup or asset. */
 game.planet=-1;game.dock_stage=0;rect(0,0,W,H,BG);preview_clip(240,110,0,0,W,H);
 for(int brand=0;brand<6;brand++)for(int near=0;near<2;near++){
  float z=near?260:400,w=near?38:46,x=(44+brand*78-240)*z/240.f,y=(110-(near?140:66))*z/240.f;
  unsigned accent=brand%3==0?RGB(255,110,173):brand%3==1?RGB(255,194,95):RGB(92,229,222);
  departure_box(x,y,z,w+2,27,8,departure_board_ink(brand));
  departure_lamp(x,y+24,z-1,w,1.3f,accent);departure_lamp(x,y-24,z-1,w,1.3f,accent);
  departure_ad_graphic(x,y,z-2,w,brand,accent);
 }
 flush_meshes();
 for(int brand=0;brand<6;brand++){
  float z=260,x=(44+brand*78-240)*z/240.f,y=(110-140)*z/240.f;
  unsigned accent=brand%3==0?RGB(255,110,173):brand%3==1?RGB(255,194,95):RGB(92,229,222);
  departure_ad(x,y,z-2,38,brand,accent);
  int mark=0;for(int yy=54;yy<78;yy++)for(int xx=26+brand*78;xx<62+brand*78;xx++)mark+=fb[yy*STRIDE+xx]==accent||fb[yy*STRIDE+xx]==RGB(218,238,223);
  DPCHECK(mark>4,"distant billboard has a genuine neon graphic instead of an empty rectangle");
 }
 dump_native_bmp("departure-neon-adverts.bmp");
 rect(20,60,8,8,departure_board_ink(0));int masked_x=-1,masked_y=-1;
 for(int yy=0;yy<8&&masked_x<0;yy++)for(int xx=0;xx<8;xx++)if(font8['K'-32][yy]&(128>>xx)){masked_x=20+xx;masked_y=60+yy;break;}
 fb[masked_y*STRIDE+masked_x]=RED;departure_ad_text(20,60,WHITE,departure_board_ink(0),"K");
 DPCHECK(fb[masked_y*STRIDE+masked_x]==RED,"billboard lettering cannot overwrite an occluding structure");
 preview_reset();
 fprintf(out,"RESULT %d failures\n",failures);fclose(out);fb=old;free(mem);free(snapshot);
 #undef DPCHECK
 return 1;
}
