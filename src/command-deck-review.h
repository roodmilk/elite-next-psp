/* Actual production input and renderer, enabled only by a disposable flag. */
static unsigned dc_review_hash(int x,int y,int w,int h){unsigned hash=0;for(int yy=y;yy<y+h;yy++)for(int xx=x;xx<x+w;xx++)hash=hash*33u+fb[yy*STRIDE+xx];return hash;}
/* Previous shipped sampler: regression reference for exact artwork pixels. */
static void dc_review_old_art(const uint16_t *data,int sw,int sh,int x,int y,int w,int h){
 for(int dy=0;dy<h;dy++){int yy=y+dy;if(yy<0||yy>=H)continue;int sy=dy*sh/h;
  for(int dx=0;dx<w;dx++){int xx=x+dx;if(xx<0||xx>=W)continue;unsigned p=data[sy*sw+dx*sw/w];if(p&0x8000)fb[yy*STRIDE+xx]=dc_rgb(p);}
 }
}
static int command_deck_review(void){
 FILE *flag=fopen("command-deck-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *out=fopen("command-deck-review.txt","w");if(!out)return 1;
 int failures=0;unsigned *old=fb,*pixels=malloc(STRIDE*H*sizeof(unsigned));if(!pixels){fprintf(out,"RESULT 1 failures\n");fclose(out);return 1;}fb=pixels;
#define DC_CHECK(c,label) do{int ok=!!(c);fprintf(out,"%s %s\n",ok?"PASS":"FAIL",label);failures+=!ok;fflush(out);}while(0)
 game_init(&game);story_complete(&game);game.opening.stage=0;game.system=7;system_bodies(&game);game.docked=1;game.station_variant=0;deck_reset();change_page(HOME);preview_time=1;row=4;home();
 int exact=1;const int art_cases[5][4]={{8,3,100,18},{-18,-8,200,118},{430,250,150,96},{30,50,80,50},{-240,-140,600,360}};
 for(int i=0;i<5;i++){int x=art_cases[i][0],y=art_cases[i][1],w=art_cases[i][2],h=art_cases[i][3];const uint16_t *art=i?almanac_station_mega:next_logo_small;int sw=i?150:100,sh=i?96:18;
  rect(0,0,W,H,DC_PANEL);dc_review_old_art(art,sw,sh,x,y,w,h);unsigned before=dc_review_hash(0,0,W,H);
  rect(0,0,W,H,DC_PANEL);draw_next_art(art,sw,sh,x,y,w,h);exact&=before==dc_review_hash(0,0,W,H);
 }
 DC_CHECK(exact,"fast artwork matches shipped native/scaled/clipped/transparent pixels exactly");home();
 DC_CHECK(!strcmp(dc_label(4),"Outfitting")&&!strcmp(dc_copy[4],"Buy modules for your ship."),"Outfitting uses the approved title and description");
 int copy_ok=1,labels_ok=1;for(int id=0;id<DECK_ITEMS;id++){copy_ok&=dc_copy_lines(dc_copy[id],0)<=2;labels_ok&=strlen(dc_label(id))<=21;}
 DC_CHECK(copy_ok,"every service description fits in the reserved two native lines");DC_CHECK(labels_ok,"all service labels fit the 21-cell button label region");
 DC_CHECK(dc_icon_for(23)==4&&dc_icon_for(24)==5&&strcmp(dc_icons[4][0],dc_icons[5][0]),"Decorator has a flat paintbrush; Mechanics has a distinct spanner");
 int icon_rows=1;for(int i=0;i<DC_ICON_COUNT;i++)for(int y=0;y<12;y++){icon_rows&=strlen(dc_icons[i][y])==12;for(int x=0;x<12;x++)icon_rows&=strchr(".#B",dc_icons[i][y][x])!=NULL;}
 DC_CHECK(icon_rows,"every native icon has twelve complete bounded rows with valid transparent pixels");
 const int icon_services[]={16,25,12,13,2,15,26};const int icon_shapes[]={DC_ICON_MUSIC,DC_ICON_EXIT,DC_ICON_BOARD,DC_ICON_LOG,DC_ICON_MAP,DC_ICON_CODEX,DC_ICON_TV};
 const char *icon_names[]={"music","exit","board","log","map","codex","tv"};unsigned icon_hashes[7];
 for(int i=0;i<7;i++){
  int id=icon_services[i],shape=dc_icon_for(id),inside=0,outside=0;unsigned clear=RGB(2,3,4);rect(0,0,W,H,clear);dc_icon(32,32,id,DC_GOLD);
  for(int y=0;y<H;y++)for(int x=0;x<W;x++)if(fb[y*STRIDE+x]!=clear){if(x>=32&&x<44&&y>=32&&y<44)inside++;else outside++;}
  icon_hashes[i]=dc_review_hash(32,32,12,12);int unique=1;for(int j=0;j<i;j++)unique&=icon_hashes[i]!=icon_hashes[j];
  DC_CHECK(shape==icon_shapes[i]&&inside>=20&&!outside&&unique,icon_names[i]);
  int iy=0,ih=0;row=id;deck_motion_start=-10;preview_time=2;home();char file[64];snprintf(file,sizeof(file),"deck-icon-%s.bmp",icon_names[i]);dump_native_bmp(file);
  int row_ok=dc_row_geometry(deck_group(id),id,&iy,&ih),visible=0;for(int y=iy+(ih-12)/2;row_ok&&y<iy+(ih-12)/2+12;y++)for(int x=22;x<34;x++)visible+=fb[y*STRIDE+x]==DC_GOLD;
  DC_CHECK(row_ok&&visible>=20,"selected service icon remains visible in its actual menu button");
 }
 int rows_fit=1;for(int g=0;g<5;g++){int vis[9],n=deck_fill(g,vis);row=vis[0];preview_time=2;home();char path[64];snprintf(path,sizeof(path),"deck-category-%d.bmp",g);dump_native_bmp(path);for(int i=0;i<n;i++){int y,h;rows_fit&=dc_row_geometry(g,vis[i],&y,&h)&&y>=85&&y+h<=240;}}
 DC_CHECK(rows_fit,"all five categories including seven Fly services fit without scrolling");
 row=4;preview_time=1;home();unsigned sun_a=dc_review_hash(414,61,66,120),sky_a=dc_review_hash(228,61,65,130),station_a=dc_review_hash(299,102,118,76),text_a=dc_review_hash(266,206,168,8);dump_native_bmp("deck-animation-a.bmp");
 preview_time=2.2f;home();dump_native_bmp("deck-animation-b.bmp");DC_CHECK(sun_a!=dc_review_hash(414,61,66,120),"sun shimmers softly using native art");DC_CHECK(sky_a!=dc_review_hash(228,61,65,130),"star glow and nebula illumination visibly animate");DC_CHECK(station_a!=dc_review_hash(299,102,118,76),"fixed station port lights glow and twinkle");DC_CHECK(text_a==dc_review_hash(266,206,168,8),"description text stays pixel-stable during ambient animation");
 for(int i=0;i<32;i++){preview_time=1+i*.125f;home();char path[48];snprintf(path,sizeof(path),"deck-ambient-%02d.bmp",i);dump_native_bmp(path);}
 row=4;preview_time=4;deck_tab(1);DC_CHECK(deck_group(row)==2&&deck_motion_from==1&&deck_motion_direction==1,"R/right changes actual category immediately and starts the directional slide");
 for(int i=0;i<4;i++){preview_time=4+i*.06f;home();char path[48];snprintf(path,sizeof(path),"deck-tab-right-%d.bmp",i);dump_native_bmp(path);}
 deck_tab(-1);DC_CHECK(row==4&&deck_motion_direction==-1,"L/left restores the remembered Outfitting selection");home();dump_native_bmp("deck-tab-left.bmp");
 preview_time=5;deck_motion_start=-10;home();input(PSP_CTRL_CROSS,0,.016f,0,0);DC_CHECK(page==EQUIP&&deck_open_active()&&ui_page_anim==0,"X starts one themed button pulse and live-page zoom without CRT flash");
 for(int i=0;i<7;i++){preview_time=5+i*.045f;rect(0,0,W,H,ui_paint_surface(BG));equipment();deck_open_draw();char path[48];snprintf(path,sizeof(path),"deck-open-%d.bmp",i);dump_native_bmp(path);}
 preview_time=5.35f;DC_CHECK(!deck_open_active(),"live destination is fully revealed within 270ms with no lingering overlay");input(PSP_CTRL_CIRCLE,0,.016f,0,0);DC_CHECK(page==HOME&&row==4,"Circle restores original service after the handoff");
 home();input(PSP_CTRL_CROSS,0,.016f,0,0);input(PSP_CTRL_CIRCLE,0,.016f,0,0);DC_CHECK(page==HOME&&row==4&&!deck_open_active(),"early Circle interrupts animation and returns without input delay");
 row=29;home();input(PSP_CTRL_CROSS,0,.016f,0,0);DC_CHECK(page==MULTIPLAYER,"existing 2.5.357 Multiplayer console remains connected");input(PSP_CTRL_CIRCLE,0,.016f,0,0);DC_CHECK(page==HOME&&row==29,"Multiplayer retains its caller-specific return");
 float time=game.time,heat=game.heat,speed=game.speed;int credits=game.credits;for(int i=0;i<120;i++)input(0,0,.016f,0,0);DC_CHECK(time==game.time&&heat==game.heat&&speed==game.speed&&credits==game.credits,"animated Command Deck keeps simulation paused");
 int geometry=1;for(int s=0;s<256;s++){game.system=s;system_bodies(&game);game.station_variant=0;game.docked=1;for(int g=0;g<5;g++){int vis[9],n=deck_fill(g,vis);for(int i=0;i<n;i++){int y,h;geometry&=dc_row_geometry(g,vis[i],&y,&h)&&y+h<=240;}}}
 DC_CHECK(geometry,"every system's authoritative service list fits the native layout");
 game.system=7;system_bodies(&game);game.docked=1;game.station_variant=1;row=24;home();dump_native_bmp("deck-relay.bmp");char name[24];dc_status_name(name);DC_CHECK(!strcmp(name,station_name_for(&game,1)),"relay header uses the current station's actual name");
 int mega=-1,poor=-1;for(int s=0;s<256;s++){if(station_class_for_system(&game,s)==STATION_MEGA&&mega<0)mega=s;if(station_class_for_system(&game,s)==STATION_POOR&&poor<0)poor=s;}
 game.system=mega;system_bodies(&game);game.station_variant=0;row=3;home();dump_native_bmp("deck-mega.bmp");DC_CHECK(!strcmp(dc_label(3),"Mega Shipyard"),"Mega hubs preserve the correct shipyard service");
 game.system=poor;system_bodies(&game);row=24;home();dump_native_bmp("deck-poor.bmp");DC_CHECK(!deck_service_visible(3)&&!deck_service_visible(4)&&!deck_service_visible(23),"poor hubs retain the real unavailable-service filtering");
 game.system=7;system_bodies(&game);game.docked=0;row=21;home();dump_native_bmp("deck-flight.bmp");DC_CHECK(!deck_service_visible(3)&&!deck_service_visible(4)&&!deck_service_visible(12)&&!deck_service_visible(20),"flight hides dock-only services while retaining ship preview");
 game.docked=1;game.station_variant=0;snprintf(game.systems[7].name,sizeof(game.systems[7].name),"RIEDQUAT");row=26;home();dump_native_bmp("deck-long-name.bmp");DC_CHECK(strlen(dc_label(26))<=21,"long local TV name stays inside its button");
 game_init(&game);story_complete(&game);game.opening.stage=0;game.docked=1;row=4;page=HOME;deck_motion_start=-10;preview_time=6;home();unsigned icon_a=dc_review_hash(276,5,16,12);preview_time=6.7f;home();DC_CHECK(icon_a!=dc_review_hash(276,5,16,12),"green docking status has a small animated icon");
 game.credits=2000000000;home();dump_native_bmp("deck-large-balance.bmp");DC_CHECK(game.credits==2000000000,"large balances format compactly without changing credits");
 SceUInt64 start,end;sceRtcGetCurrentTick(&start);for(int i=0;i<30;i++){preview_time+=.016f;home();}sceRtcGetCurrentTick(&end);double ms=(double)(end-start)*1000./sceRtcGetTickResolution()/30.;fprintf(out,"INFO %.3f ms average native deck render (30 frames, emulator)\n",ms);DC_CHECK(ms<33.34,"native deck renderer stays within the 30fps frame budget in emulator");
 const int menu_pages[]={EQUIP,YARD,INVENTORY,MARKET,DECORATOR,COMMS_PANEL};
 for(int m=0;m<6;m++){change_page(menu_pages[m]);sceRtcGetCurrentTick(&start);for(int i=0;i<10;i++){preview_time+=.016f;drawcount=0;switch(page){case EQUIP:equipment();break;case YARD:yard();break;case INVENTORY:inventory_screen();break;case MARKET:market_screen();break;case DECORATOR:decorator_screen();break;default:comms_panel();break;}}sceRtcGetCurrentTick(&end);fprintf(out,"MENU page %d avg %.3f ms (10 frames, emulator)\n",page,(double)(end-start)*1000./sceRtcGetTickResolution()/10.);}
 DC_CHECK(ui_page_anim==0&&!deck_open_active(),"nested shop/loadout/cargo/decorator/comms switches do not replay the deck or flash");
 /* Prove the one-buffer shrink against an immutable reference, including
  * asymmetric rectangles where a naive top-down copy corrupts later rows. */
 unsigned *morph_reference=malloc(STRIDE*H*sizeof(unsigned));int morph_exact=morph_reference!=NULL;
 const int apertures[][4]={{12,85,208,22},{6,80,340,150},{1,3,478,268},{0,0,480,272}};
 for(int a=0;morph_reference&&a<4;a++){
  for(int y=0;y<H;y++)for(int x=0;x<STRIDE;x++){unsigned c=RGB((x*13+y*7)&255,(x+y*19)&255,(x*3+y*11)&255);fb[y*STRIDE+x]=morph_reference[y*STRIDE+x]=c;dc_open_source[y*STRIDE+x]=RGB(12,18,28);}
  int x0=apertures[a][0],y0=apertures[a][1],w=apertures[a][2],h=apertures[a][3];dc_open_window(x0,y0,w,h);
  for(int y=0;y<H;y++)for(int x=0;x<STRIDE;x++){
   unsigned want=x>=W?morph_reference[y*STRIDE+x]:x>=x0&&x<x0+w&&y>=y0&&y<y0+h?morph_reference[((y-y0)*H/h)*STRIDE+(x-x0)*W/w]:dc_open_source[y*STRIDE+x];morph_exact&=fb[y*STRIDE+x]==want;
  }
 }
 free(morph_reference);DC_CHECK(morph_exact,"growing live-page image is pixel-exact with intact row gutters across four overlap cases");
 game_init(&game);story_complete(&game);game.opening.stage=0;game.docked=1;page=HOME;row=4;preview_time=10;deck_motion_start=-10;
 SceUInt64 click_begin,click_end;sceRtcGetCurrentTick(&click_begin);input(PSP_CTRL_CROSS,0,.016f,0,0);sceRtcGetCurrentTick(&click_end);
 fprintf(out,"OPEN click %.3f ms including clean source render (emulator)\n",(double)(click_end-click_begin)*1000./sceRtcGetTickResolution());
 double open_total=0,open_worst=0;
 for(int i=0;i<17;i++){preview_time=10+i*.016f;sceRtcGetCurrentTick(&click_begin);rect(0,0,W,H,ui_paint_surface(BG));equipment();deck_open_draw();sceRtcGetCurrentTick(&click_end);double frame=(double)(click_end-click_begin)*1000./sceRtcGetTickResolution();open_total+=frame;open_worst=fmaxf(open_worst,frame);}
 fprintf(out,"OPEN render average %.3f ms worst %.3f ms (17 frames, emulator)\n",open_total/17,open_worst);
 DC_CHECK(open_total/17<16.67,"opening render fits the 60fps CPU frame budget in emulator");
 preview_time=10.30f;rect(0,0,W,H,ui_paint_surface(BG));equipment();unsigned revealed=dc_review_hash(0,0,W,H);deck_open_draw();DC_CHECK(revealed==dc_review_hash(0,0,W,H)&&!deck_open_active(),"completed zoom leaves the real destination pixels untouched");
 change_page(HOME);row=4;input(PSP_CTRL_CROSS,0,.016f,0,0);int before_click=game.credits;input(PSP_CTRL_CROSS,0,.016f,0,0);DC_CHECK(page==EQUIP&&!deck_open_active()&&game.credits==before_click,"rapid second X reveals the page without purchasing an unseen item");
 change_page(HOME);row=4;input(PSP_CTRL_CROSS,0,.016f,0,0);input(PSP_CTRL_DOWN,0,.016f,0,0);DC_CHECK(page==EQUIP&&!deck_open_active()&&row==1,"directional input interrupts the zoom and moves selection immediately");
 change_page(HOME);row=0;input(PSP_CTRL_CROSS,0,.016f,0,0);DC_CHECK(page==FLIGHT&&!deck_open_active(),"launch skips menu animation and never hides live flight controls");deck_open_cancel();
 unsigned original_paint[sizeof(ship_paint)/sizeof(*ship_paint)];memcpy(original_paint,ship_paint,sizeof(original_paint));unsigned theme_hash[14][DECORATOR_COUNT];int themed=1;
 const int theme_pages[]={HOME,EQUIP,YARD,INVENTORY,STORAGE,MARKET,COMMS_PANEL,MULTIPLAYER,STATUS,CHART,MISSIONS,MISSIONLOG,CODEX,DECORATOR};
 const char *theme_names[]={"deck","outfitting","shipyard","loadout","storage","cargo","comms","multiplayer","profile","map","board","missions","codex","decorator"};
 game_init(&game);story_complete(&game);game.opening.stage=0;game.docked=1;game.ship=0;profile_mode=0;mp.modal=0;deck_motion_start=-10;
 for(int finish=0;finish<DECORATOR_COUNT;finish++){ship_paint[0]=decorator_finishes[finish];themed&=ship_theme_index()==finish;
  for(int m=0;m<14;m++){page=theme_pages[m];row=page==HOME?4:0;preview_time=12;drawcount=0;rect(0,0,W,H,ui_paint_surface(BG));
   switch(page){case HOME:home();break;case EQUIP:equipment();break;case YARD:yard();break;case INVENTORY:inventory_screen();break;case STORAGE:storage_screen();break;case MARKET:market_screen();break;case COMMS_PANEL:comms_panel();break;case MULTIPLAYER:multiplayer_draw();break;case STATUS:status();break;case CHART:chart();break;case MISSIONS:mission_board();break;case MISSIONLOG:mission_log();break;case CODEX:codex_screen();break;default:decorator_screen();break;}
   theme_hash[m][finish]=dc_review_hash(0,0,W,H);for(int previous=0;previous<finish;previous++)themed&=theme_hash[m][previous]!=theme_hash[m][finish];
   if(finish==0||finish==4||finish==13){char file[64];snprintf(file,sizeof(file),"menu-theme-%02d-%s.bmp",finish,theme_names[m]);dump_native_bmp(file);}
  }
 }
 DC_CHECK(themed,"all sixteen ship finishes update all fourteen main menu palettes immediately");
 memcpy(ship_paint,original_paint,sizeof(original_paint));page=HOME;
 fprintf(out,"RESULT %d failures\n",failures);fclose(out);fb=old;free(pixels);deck_open_cancel();
#undef DC_CHECK
 return 1;
}
