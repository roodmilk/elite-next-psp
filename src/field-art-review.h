/* Disposable-folder native review, never enabled in a delivered game. */
static unsigned field_review_hash(void){unsigned h=2166136261u;for(int y=28;y<240;y++)for(int x=0;x<W;x++)h=(h^fb[y*STRIDE+x])*16777619u;return h;}
static void field_review_setup(int sys,int body){
 game_init(&game);game.system=sys;system_bodies(&game);launch(&game);story_complete(&game);game.approach=body;enter_planet(&game);
 game.surface=2;game.planet_sequence=0;game.speed=0;game.world_clock=0;game.time=20;game.dead=0;
 Vec3 pad=surface_site(&game,1);game.ship_pos=pad;game.ship_pos.y+=6;
 page=FLIGHT;hud_mode=hud_hidden=high_contrast=0;surface_target_open=0;surface_target_lock=-1;surface_nav_poi=2;planet_landing_menu=0;preview_reset();
}
static void field_review_frame(const char *file){game.voice_time=game.message_time=0;drawcount=0;rect(0,0,W,H,BG);space();if(file)dump_native_bmp(file);}
static int field_art_review(void){
 FILE *flag=fopen("field-art-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *report=fopen("field-art-review.txt","w");unsigned *saved=fb,*pixels=calloc(STRIDE*H+32,sizeof(unsigned));if(!report||!pixels)return 1;
 for(int i=0;i<16;i++)pixels[i]=pixels[STRIDE*H+16+i]=0xa55ac33cu;
 fb=pixels+16;int failures=0;
 #define ART_CHECK(ok,name) do{int pass=(ok);fprintf(report,"%s %s\n",pass?"PASS":"FAIL",name);failures+=!pass;}while(0)
 unsigned foundation_palette[64];int foundation_palette_n=0,old_grey_found=0;
 for(int sys=0;sys<256;sys++)for(int body=1;body<BODY_COUNT;body++){
  game.system=sys;system_bodies(&game);game.planet=body;
  unsigned c=lm_ground_ink(FIELD_DISH,field_seed(sys,body));old_grey_found|=c==RGB(109,137,138);
  int known=0;for(int i=0;i<foundation_palette_n;i++)known|=foundation_palette[i]==c;
  if(!known&&foundation_palette_n<64)foundation_palette[foundation_palette_n++]=c;
 }
 ART_CHECK(foundation_palette_n>=12&&!old_grey_found,"POI foundations use varied biome ground colours, never the old universal grey");
 int natural_count=0;for(int kind=0;kind<12;kind++)natural_count+=lm_natural_site(kind);
 ART_CHECK(natural_count==4,"ruins, fossils, wrecks and crystal sites use natural earth berms without engineered pads");
 int worlds=0,bad=0,seen[16]={0},sample_sys[16]={0},sample_body[16]={0},sample_slot[16]={0};
 for(int sys=0;sys<256;sys++)for(int body=1;body<BODY_COUNT;body++){
  field_sprite_build(sys,body);worlds++;
  for(int slot=0;slot<8;slot++){
   unsigned h=field_species_seed(sys,body,slot);int kind=field_species_kind(sys,body,slot),family=(h>>8)%8,opaque=0;
   if(field_art_kind[slot]!=kind||field_art_family[slot]!=family||field_sprite_sample(slot,-1,0,0)!=255)bad++;
   for(int y=0;y<64;y++)for(int x=0;x<48;x++){int c=field_sprite_sample(slot,x,y,0);if(c!=255){opaque++;if(c<0||c>=64)bad++;}}
   if(opaque<30)bad++;
   if(kind!=LIFE_MINERAL){int n=family+(kind==LIFE_FAUNA?8:0);seen[n]++;sample_sys[n]=sys;sample_body[n]=body;sample_slot[n]=slot;}
  }
 }
 fprintf(report,"Catalogue: %d worlds, %d species identities\n",worlds,worlds*8);ART_CHECK(bad==0,"species identity, palette bounds and nonempty silhouettes");
 for(int n=0;n<16;n++)if(!seen[n])bad++;
 ART_CHECK(bad==0,"all eight flora and eight fauna families represented");
 for(int sheet=0;sheet<2;sheet++){
  rect(0,0,W,H,RGB(21,28,39));text(2,1,CYAN,sheet?"FIELD GUIDE / FAUNA":"FIELD GUIDE / FLORA");
  for(int i=0;i<8;i++){int n=sheet*8+i,x=(i%4)*120+60,y=(i/4)*112+113;char name[48];field_species_name(sample_sys[n],sample_body[n],sample_slot[n],name,sizeof(name));
   field_draw_species(x,y,84,sample_sys[n],sample_body[n],sample_slot[n],0,22,260);text((i%4)*15+1,(i/4)*14+15,WHITE,"%s",name);
  }dump_native_bmp(sheet?"species-fauna.bmp":"species-flora.bmp");
 }
 int covered=0,approachable=0;uint64_t worst=0;float average=0;
 for(int kind=0;kind<12;kind++){
  int found=0;
  for(int sys=0;sys<256&&!found;sys++)for(int body=1;body<BODY_COUNT&&!found;body++)for(int id=2;id<10&&!found;id++){
   if(id==6||field_type(sys,body)==GAS||field_site_kind(&game,sys,body,id)!=kind)continue;
   field_review_setup(sys,body);Vec3 p=surface_poi(&game,id);FieldBuilding s=field_site_building(&game,id);found=1;covered++;
   game.world_clock=0;float hour=field_local_hour(&game,sys,body);static const int days[]={600,900,1200,1800};game.world_clock=(10-hour+24)*days[(field_seed(sys,body)>>24)&3]/24.f;
   for(int angle=0;angle<2;angle++){
    float distance=fmaxf(fmaxf(s.w,s.d)*2.6f+100,s.height*2.7f),a=angle*.65f;
    game.pos=(Vec3){p.x+sinf(a)*distance,0,p.z-cosf(a)*distance};game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;
    game.yaw=atan2f(p.x-game.pos.x,p.z-game.pos.z);game.pitch=atan2f(p.y+s.height*.45f-game.pos.y,distance);game.roll=0;surface_nav_poi=id;
    uint64_t a0,b0;sceRtcGetCurrentTick(&a0);field_review_frame(0);sceRtcGetCurrentTick(&b0);average+=(b0-a0)*1000.f/sceRtcGetTickResolution();if(b0-a0>worst)worst=b0-a0;
    char file[64];snprintf(file,sizeof(file),"poi-%02d-%d.bmp",kind,angle);dump_native_bmp(file);
   }
   fprintf(report,"POI %d: system %d planet %d site %d\n",kind,sys,body,id);
   game.pos=(Vec3){p.x,0,p.z-s.d-22};game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;
   if(surface_nearest_site(&game,55)==id)approachable++;
  }
 }
 ART_CHECK(covered==12,"all twelve real POI families rendered in-world at two angles");ART_CHECK(approachable==12,"all twelve POI families interact from the visible perimeter");fprintf(report,"POI draw average %.3f ms / worst %.3f ms (cold scenes included)\n",average/24,worst*1000.f/sceRtcGetTickResolution());
 field_review_setup(7,1);Vec3 pad=surface_site(&game,1);game.pos=(Vec3){pad.x-100,0,pad.z+400};game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;game.yaw=game.pitch=game.roll=0;
 for(int slot=0;slot<8;slot++){game.life[slot].pos=(Vec3){game.pos.x+(slot-3.5f)*25,0,game.pos.z+120};game.life[slot].alive=1;game.life[slot].kind=field_species_kind(7,1,slot);}
 for(int i=0;i<3;i++){game.pitch=(i-1)*.25f;char file[48];snprintf(file,sizeof(file),"wildlife-pitch-%d.bmp",i);field_review_frame(file);}
 game.life[1].pos=add(game.pos,(Vec3){0,0,5});surface_target_lock=12;
 ART_CHECK(surface_scan_selected()&&field_species_logged(&game,7,1,1),"new flora sprite scans into its original persistent identity");
 input(0,0,.016f,0,0); /* Release the normal post-landing input safety gate. */
 input(PSP_CTRL_TRIANGLE,0,.016f,0,0);ART_CHECK(page==CODEX&&codex_scope==ATLAS_RECORD&&atlas_entry==1,"scan prompt opens exact Codex species record");
 codex_screen();dump_native_bmp("wildlife-codex.bmp");
 game.docked=1; /* Saving is intentionally restricted to a station berth. */
 int saved_ok=save_game(&game,"field-art-save.dat");game.surface_progress[7][1]=0;int loaded_ok=load_game(&game,"field-art-save.dat");ART_CHECK(saved_ok&&loaded_ok&&field_species_logged(&game,7,1,1),"discovery survives docked save/load");
 field_review_setup(7,1);game.pos=(Vec3){pad.x-100,0,pad.z+400};game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;game.yaw=game.pitch=game.roll=0;field_review_frame(0);
 int tree=-1;float nearest=1e20f;for(int i=0;i<lave_prop_count;i++)if(lave_prop_cache[i].kind<4){float dx=lave_prop_cache[i].x-game.pos.x,dz=lave_prop_cache[i].w-game.pos.z;if(dx*dx+dz*dz<nearest){nearest=dx*dx+dz*dz;tree=i;}}
 int natural_leaves=0;if(tree>=0){game.pos.x=lave_prop_cache[tree].x;game.pos.z=lave_prop_cache[tree].w-65;game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;game.pitch=.14f;
  for(int frame=0;frame<12;frame++){game.time=20+frame*.25f;char file[48];snprintf(file,sizeof(file),"tree-leaves-%02d.bmp",frame);field_review_frame(file);natural_leaves+=field_leaf_count;}
 }
 ART_CHECK(natural_leaves>0,"leaves present around a real placed Lave tree");
 /* Isolate leaf rendering to prove deterministic motion and real depth rejection. */
 game.pitch=0;game.time=20;game.yaw=0;surface_y_min=28;surface_y_max=239;surface_light=1;
 rect(0,0,W,H,BG);memset(surface_depth,255,sizeof(surface_depth));field_leaf_count=0;field_tree_leaves(game.pos.x,game.pos.z+60,123,70);unsigned first=field_review_hash();int near=field_leaf_count;
 rect(0,0,W,H,BG);memset(surface_depth,255,sizeof(surface_depth));field_leaf_count=0;game.time=22;field_tree_leaves(game.pos.x,game.pos.z+60,123,70);ART_CHECK(near>0&&first!=field_review_hash(),"near-tree leaves visibly move");
 for(int i=0;i<20;i++)field_tree_leaves(game.pos.x,game.pos.z+60,123+i,70);
 ART_CHECK(field_leaf_count==24,"leaf count capped at 24");
 field_leaf_count=0;field_tree_leaves(game.pos.x,game.pos.z+200,123,70);ART_CHECK(field_leaf_count==0,"leaves rejected beyond 180 metres");
 rect(0,0,W,H,BG);unsigned empty=field_review_hash();memset(surface_depth,0,sizeof(surface_depth));field_leaf_count=0;field_tree_leaves(game.pos.x,game.pos.z+60,123,70);ART_CHECK(empty==field_review_hash(),"foreground depth hides leaves");
 field_sprite_build(7,1);surface_cy=surface_cp=1;surface_sy=surface_sp=surface_sprite_tan=0;field_draw_world(&game.life[1]);ART_CHECK(empty==field_review_hash(),"foreground depth hides wildlife");
 int safe=1;for(int i=0;i<16;i++)if(pixels[i]!=0xa55ac33cu||pixels[STRIDE*H+16+i]!=0xa55ac33cu)safe=0;ART_CHECK(safe,"framebuffer guard words");
 fprintf(report,"RESULT %d failures\n",failures);fclose(report);fb=saved;free(pixels);return 1;
 #undef ART_CHECK
}
