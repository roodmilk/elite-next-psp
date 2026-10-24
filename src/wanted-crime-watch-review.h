static int wanted_crime_watch_review(void){
 FILE *flag=fopen("wanted-crime-watch-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *out=fopen("wanted-crime-watch-review.txt","w");if(!out)return 1;int failures=0;
 unsigned *oldfb=fb,*allocation=malloc((STRIDE*H+32)*sizeof(unsigned));if(!allocation){fclose(out);return 1;}
 for(int i=0;i<16;i++)allocation[i]=allocation[16+STRIDE*H+i]=0xdeadbeef;
 fb=allocation+16;
#define WCCHECK(c,label) do{int ok=!!(c);fprintf(out,"%s %s\n",ok?"PASS":"FAIL",label);failures+=!ok;fflush(out);}while(0)
 game_init(&game);story_complete(&game);game.opening.stage=OP_FREE;game.system=7;game_spawn(&game);game.docked=1;
 identity_slot=-99;identity_sync();page=GALNET;galnet_tab=2;row=0;deck_open_cancel();wanted_ui_reset();
 int cash=game.credits,target=selected_target,aim=autoaim;float world=game.time;Vec3 pos=game.pos;
 input(PSP_CTRL_RIGHT,PSP_CTRL_RIGHT,.016f,0,0);WCCHECK(row==1&&wanted_record==1,"D-pad Right selects the next coherent warrant");
 input(PSP_CTRL_LEFT,PSP_CTRL_LEFT,.016f,0,0);WCCHECK(row==0,"D-pad Left selects the previous warrant");
 input(PSP_CTRL_LEFT,PSP_CTRL_LEFT,.016f,0,0);WCCHECK(row==4,"Left wraps from first to last suspect");
 input(PSP_CTRL_RIGHT,PSP_CTRL_RIGHT,.016f,0,0);WCCHECK(row==0,"Right wraps from last to first suspect");
 game.opening.stage=0;input(PSP_CTRL_RIGHT,PSP_CTRL_RIGHT,.016f,0,0);
 WCCHECK(row==1&&wanted_record==1,"legacy commanders without opening state use the same horizontal controls");
 input(PSP_CTRL_LEFT,PSP_CTRL_LEFT,.016f,0,0);game.opening.stage=OP_FREE;
 input(PSP_CTRL_UP,PSP_CTRL_UP,.016f,0,0);WCCHECK(row==0,"Up no longer browses wanted records");
 input(PSP_CTRL_DOWN,PSP_CTRL_DOWN,.016f,0,0);WCCHECK(row==0,"Down no longer browses wanted records");
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE,.016f,0,0);input(PSP_CTRL_TRIANGLE,PSP_CTRL_TRIANGLE,.016f,0,0);
 WCCHECK(page==GALNET&&galnet_tab==2&&selected_target==target&&autoaim==aim&&game.credits==cash&&game.docked,"board has no hidden tracking, launch, payout or target-selection action");
 for(int i=0;i<7;i++)input(PSP_CTRL_RIGHT,PSP_CTRL_RIGHT,.016f,0,0);
 WCCHECK(row==2&&wanted_record==2&&wanted_slide==0,"rapid input retargets immediately and newest selection wins");
 input(PSP_CTRL_LTRIGGER,PSP_CTRL_LTRIGGER,.016f,0,0);WCCHECK(galnet_tab==1&&row==0&&wanted_record==-1,"L shoulder interrupts motion and switches to Market");
 input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);WCCHECK(galnet_tab==2,"Market R shoulder reopens Wanted");
 input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);WCCHECK(galnet_tab==3&&wanted_record==-1,"Wanted R shoulder switches to Spacebook");
 galnet_tab=2;row=0;input(0,0,.05f,0,0);input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);
 WCCHECK(page==GALNET&&wanted_exit>=0,"Circle starts a short reverse broadcast wipe");
 input(0,0,.1f,0,0);input(0,0,.016f,0,0);WCCHECK(page==HOME&&wanted_record==-1,"Circle completes return within 140ms and clears stale animations");
 change_page(GALNET);galnet_tab=2;row=0;deck_open_cancel();input(0,0,.016f,0,0);
 WCCHECK(wanted_enter<.02f&&wanted_exit<0,"reopening starts a fresh entrance without stale exit or identity");
 for(int i=0;i<10;i++)input(0,0,.05f,0,0);
 WCCHECK(game.time==world&&length(sub(game.pos,pos))<.001f,"browsing and animations preserve paused world time and ship position");
 int exact=1,guards=1,tint=1,steady=1;u64 start,end;double elapsed=0;
 for(int p=0;p<5;p++){
  row=p;wanted_sync();wanted_enter=wanted_slide=1;wanted_clock=1.2f;
  const uint16_t *source=portrait_legacy_source(portrait_pulp_base(wanted_dna[p]));
  rect(0,0,W,H,BG);sceRtcGetCurrentTick(&start);galnet_screen();sceRtcGetCurrentTick(&end);elapsed+=(end-start)*1000.0/sceRtcGetTickResolution();
  char file[64];snprintf(file,sizeof(file),"crime-watch-suspect-%02d.bmp",p+1);dump_native_bmp(file);
  /* Exact independently sampled source pixels at both real displayed sizes. */
  int tx=62+p*82+2+(31-24)/2;
  for(int y=0;y<24;y++)for(int x=0;x<24;x++){
   uint16_t raw=source[(y*64/24)*64+x*64/24];unsigned expected=raw&32768?wanted_rgb(raw):RGB(7,16,21);
   exact&=fb[(216+y)*STRIDE+tx+x]==expected;
  }
  unsigned tinted[112*169];for(int y=0;y<112;y++)memcpy(tinted+y*169,fb+(73+y)*STRIDE+300,169*sizeof(unsigned));
  wanted_portrait(p,300,73,169,112,0,0);snprintf(file,sizeof(file),"crime-watch-untinted-%02d.bmp",p+1);dump_native_bmp(file);
  for(int y=0;y<112;y++)for(int x=0;x<169;x++){
   unsigned clean=fb[(73+y)*STRIDE+300+x],expected=art_mix(clean,RGB(69,201,138),20);
   if(((y-(int)(wanted_clock*4))&3)==0)expected=art_mix(expected,RGB(6,33,20),20);
   tint&=tinted[y*169+x]==expected;
   if(x>=28&&x<140){uint16_t raw=source[(y*64/112)*64+(x-28)*64/112];exact&=clean==(raw&32768?wanted_rgb(raw):RGB(7,16,21));}
  }
  wanted_clock=1.45f;wanted_portrait(p,300,73,169,112,0,0);
  for(int y=0;y<112;y++)for(int x=28;x<140;x++){uint16_t raw=source[(y*64/112)*64+(x-28)*64/112];steady&=fb[(73+y)*STRIDE+300+x]==(raw&32768?wanted_rgb(raw):RGB(7,16,21));}
  for(int i=0;i<16;i++)guards&=allocation[i]==0xdeadbeef&&allocation[16+STRIDE*H+i]==0xdeadbeef;
 }
 WCCHECK(exact,"all five large and small portraits exactly match corrected source rows, including fish and suspect 02");
 WCCHECK(tint,"all five treatments are exactly 8-percent tint plus faint viewport-fixed scanlines");
 WCCHECK(steady,"changing scanline phase never moves or alters the underlying face");
 WCCHECK(guards,"every suspect preserves framebuffer allocation guards");
 fprintf(out,"INFO native Crime Watch mean draw %.3f ms\n",elapsed/5);
 WCCHECK(elapsed/5<33.34,"whole native Crime Watch screen renders within the 30fps emulator budget");
 /* Outside the viewport, tint must have no effect, including banner/thumbs. */
 row=0;wanted_slide=wanted_enter=1;galnet_screen();unsigned outside=0;
 for(int y=42;y<248;y++)for(int x=0;x<W;x++)if(!(x>=300&&x<469&&y>=73&&y<185))outside=outside*33+fb[y*STRIDE+x];
 wanted_portrait(0,300,73,169,112,0,0);unsigned outside_after=0;
 for(int y=42;y<248;y++)for(int x=0;x<W;x++)if(!(x>=300&&x<469&&y>=73&&y<185))outside_after=outside_after*33+fb[y*STRIDE+x];
 WCCHECK(outside==outside_after,"portrait treatment never touches text, status banner or small thumbnails");
 float phases[]={0,.04f,.1f,.18f,.26f};
 for(int i=0;i<5;i++){wanted_enter=phases[i];wanted_slide=1;galnet_screen();char file[48];snprintf(file,sizeof(file),"crime-watch-entry-%d.bmp",i);dump_native_bmp(file);}
 wanted_enter=1;input(PSP_CTRL_RIGHT,PSP_CTRL_RIGHT,.016f,0,0);
 for(int i=0;i<4;i++){wanted_slide=i*.06f;galnet_screen();char file[48];snprintf(file,sizeof(file),"crime-watch-slide-%d.bmp",i);dump_native_bmp(file);}
 int clipped=1;
 for(int shift=-169;shift<=169;shift+=13){
  rect(299,72,171,114,RGB(211,7,191));wanted_portrait(row,300,73,169,112,shift,1);
  for(int y=72;y<=185;y++)clipped&=fb[y*STRIDE+299]==RGB(211,7,191)&&fb[y*STRIDE+469]==RGB(211,7,191);
  for(int x=299;x<=469;x++)clipped&=fb[72*STRIDE+x]==RGB(211,7,191)&&fb[185*STRIDE+x]==RGB(211,7,191);
 }
 WCCHECK(clipped,"horizontal transitions remain bounded by the portrait aperture");
 row=0;game.bounty_claimed[game.system]=1;wanted_sync();wanted_enter=wanted_slide=1;wanted_claim_age=.1f;galnet_screen();dump_native_bmp("crime-watch-claimed.bmp");
 WCCHECK(wanted_seen_claim&&game.bounty_claimed[game.system]==1,"claimed banner and thumbnail check use the actual saved warrant mask");
 row=1;wanted_sync();row=0;wanted_sync();
 WCCHECK(wanted_claim_age>=1,"a previously viewed claimed warrant does not replay its stamp");
 /* Every legacy base, at both sizes, and all ten modern species use production rendering. */
 int valid=1;
 for(int page_i=0;page_i<16;page_i++){
  rect(0,0,W,H,RGB(12,20,30));wanted_text(8,8,UI_GOLD,"CORRECTED SOURCE PORTRAITS / 112 + 24",1);
  for(int n=0;n<6;n++){
   int base=page_i*6+n,x=(n%3)*160,y=30+(n/3)*120;const uint16_t *source=portrait_legacy_source(base);
   for(int size_i=0;size_i<2;size_i++){int side=size_i?24:112,ox=x+(size_i?120:0);
    for(int yy=0;yy<side;yy++)for(int xx=0;xx<side;xx++){uint16_t raw=source[(yy*64/side)*64+xx*64/side];if(raw&32768)fb[(y+yy)*STRIDE+ox+xx]=wanted_rgb(raw);}
   }
   char label[8];snprintf(label,sizeof(label),"%02d",base);wanted_text(x+120,y+32,UI_TEXT,label,1);
   for(int x0=0;x0<64;x0++)valid&=source[x0]==0&&source[63*64+x0]==0;
  }
  char file[48];snprintf(file,sizeof(file),"crime-watch-atlas-%02d.bmp",page_i);dump_native_bmp(file);
 }
 WCCHECK(valid,"all 96 supported legacy faces have clean isolated first and last rows at both sizes");
 for(int sp=0;sp<10;sp++){
  rect(0,0,W,H,RGB(12,20,30));unsigned dna=portrait_expand_dna(portrait_parts_dna(portrait_seeded(991,EXPLORERS,sp)));
  wanted_text(8,8,UI_GOLD,portrait_species_name(dna),1);portrait_draw(40,40,112,112,dna,EXPLORERS);portrait_draw(180,40,24,24,dna,EXPLORERS);
  char file[48];snprintf(file,sizeof(file),"crime-watch-modern-%02d.bmp",sp);dump_native_bmp(file);
 }
 int all_systems=1,longest=0,sys_longest=7;
 for(int sys=0;sys<256;sys++){
  game.system=sys;int len=strlen(game.systems[sys].name);if(len>longest){longest=len;sys_longest=sys;}
  wanted_cache();for(int p=0;p<5;p++)all_systems&=portrait_pulp_base(wanted_dna[p])>=0&&portrait_pulp_base(wanted_dna[p])<96;
 }
 WCCHECK(all_systems,"all 1280 real warrant identities resolve to valid corrected portrait tiles");
 game.system=sys_longest;row=4;wanted_ui_reset();wanted_sync();wanted_enter=wanted_slide=1;galnet_screen();dump_native_bmp("crime-watch-longest-system.bmp");
 WCCHECK(longest<=8,"every real system name fits the reserved local-law ribbon in full");
 game.system=7;row=0;unsigned saved_paint[sizeof(ship_paint)/sizeof(ship_paint[0])];memcpy(saved_paint,ship_paint,sizeof(ship_paint));int themed=1;
 for(int theme=0;theme<16;theme++){
  ship_paint[game.ship]=decorator_finishes[theme];wanted_ui_reset();wanted_sync();wanted_enter=wanted_slide=1;galnet_screen();
  char file[48];snprintf(file,sizeof(file),"crime-watch-theme-%02d.bmp",theme);dump_native_bmp(file);
  themed&=fb[104*STRIDE+15]==RGB(255,75,131);
 }
 memcpy(ship_paint,saved_paint,sizeof(ship_paint));WCCHECK(themed,"all sixteen ship themes preserve the approved broadcast palette and readable chrome");
 game.system=7;game.docked=0;row=0;wanted_ui_reset();input(0,0,.05f,0,0);
 float time=game.time;pos=game.pos;for(int i=0;i<60;i++)input(0,0,.05f,0,0);
 WCCHECK(game.time==time&&length(sub(game.pos,pos))<.001f,"in-flight Wanted browsing remains paused throughout idle animation");
 input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);input(0,0,.1f,0,0);input(0,0,.02f,0,0);
 WCCHECK(page==HOME&&!game.docked,"closing in flight preserves the existing return-to-deck behaviour");
 WCCHECK(wanted_bounty_tests(&game,"wanted-bounty-review.txt")==0,"real gameplay payout, duplicate, respawn and save checks pass");
 fprintf(out,"RESULT %d failures\n",failures);fclose(out);fb=oldfb;free(allocation);
#undef WCCHECK
 return 1;
}
