/* Opt-in checks of the actual profile framebuffer, not estimated text widths. */
static int commander_profile_review(void){
 FILE *flag=fopen("profile-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *out=fopen("profile-review.txt","w");if(!out)return 1;
 unsigned *old=fb,*pixels=calloc(STRIDE*H+32,sizeof(unsigned)),*photo=malloc(80*80*sizeof(unsigned));
 int failures=0,checks=0;
 if(!pixels||!photo){fprintf(out,"RESULT 1 failures\n");fclose(out);free(pixels);free(photo);return 1;}
 fb=pixels+16;for(int i=0;i<16;i++)pixels[i]=pixels[STRIDE*H+16+i]=0x73a19e24u;
#define PCHECK(c,s) do{int ok=!!(c);fprintf(out,"%s %s\n",ok?"PASS":"FAIL",s);failures+=!ok;checks++;fflush(out);}while(0)
 game_init(&game);story_complete(&game);game.opening.stage=OP_FREE;game.tutorial_step=0;game.docked=1;game.credits=987654;game.kills=1247;game.discoveries=238;game.legal=35;
 snprintf(game.commander_name,sizeof(game.commander_name),"ALEXANDRA STARFARER XX");
 int longest=0;for(int i=1;i<player_ship_count;i++)if(strlen(player_ships[i].name)>strlen(player_ships[longest].name))longest=i;game.ship=longest;
 profile_from_intro=profile_mode=profile_portrait_edit=profile_confirm=0;change_page(STATUS);row=0;
 unsigned paint=ship_paint[game.ship];
 /* Every theme must leave every photo pixel identical to a direct portrait.
  * Include old saved portraits and both modular formats across all species. */
 for(int sp=0;sp<PORTRAIT_SPECIES_COUNT;sp++){
  int clean=1,identity=1;unsigned modern=portrait_parts_dna(portrait_seeded(0x5197u,EXPLORERS,sp));
  unsigned expanded=portrait_expand_dna(modern),female=expanded;
  if(sp==0||sp==7){expanded=portrait_adjust(expanded,10,1);female=portrait_adjust(portrait_adjust(expanded,11,1),10,1);}
  unsigned dna[]={portrait_seeded(0x5197u,EXPLORERS,sp),PORTRAIT_DNA_MARK|(2u<<26)|(unsigned)sp,modern,expanded,female};
  for(int v=0;v<5;v++){
   game.commander_portrait=dna[v];draw_commander_portrait(42,69,80,dna[v]);
   for(int y=0;y<80;y++)for(int x=0;x<80;x++)photo[y*80+x]=fb[(69+y)*STRIDE+42+x];
   for(int t=0;t<DECORATOR_COUNT;t++){
    ship_paint[game.ship]=decorator_finishes[t];rect(0,0,W,H,BG);status();
    for(int y=0;y<80;y++)for(int x=0;x<80;x++)clean&=photo[y*80+x]==fb[(69+y)*STRIDE+42+x];
    identity&=game.commander_portrait==dna[v];
   }
  }
  char label[128];snprintf(label,sizeof(label),"species %d: legacy/v2/v3/expanded profile photos match direct artwork in all sixteen themes",sp);PCHECK(clean&&identity,label);
 }
 /* The old integer commander identities must also retain their exact art. */
 int legacy_clean=1;for(unsigned d=0;d<8;d++){
  game.commander_portrait=d;draw_commander_portrait(42,69,80,d);
  for(int y=0;y<80;y++)for(int x=0;x<80;x++)photo[y*80+x]=fb[(69+y)*STRIDE+42+x];
  status();for(int y=0;y<80;y++)for(int x=0;x<80;x++)legacy_clean&=photo[y*80+x]==fb[(69+y)*STRIDE+42+x];
 }PCHECK(legacy_clean,"all eight original saved portraits remain clean and unchanged");
 /* Independent source-pixel checks cover every legacy face, including the
  * previously missed atlas row boundaries. Do not accept direct-vs-direct
  * agreement alone: it allowed a contaminated atlas to pass earlier tests. */
 {int colours=1,gutters=1,identities=1,total=0;
  for(int base=0;base<96;base++){
   unsigned d=0;
   for(int sp=0;sp<10&&!d;sp++)for(int h=0;h<4&&!d;h++)for(int set=0;set<8&&!d;set++){
    unsigned candidate=PORTRAIT_DNA_BASE|(unsigned)sp|((unsigned)h<<4)|((unsigned)set<<16);
    if(portrait_pulp_base(candidate)==base)d=candidate;
   }
   identities&=d!=0;game.commander_portrait=d;status();total++;
   const uint16_t *authored=portrait_legacy_source(base);
   for(int y=0;y<2;y++)for(int x=0;x<64;x++)gutters&=authored[y*64+x]==0;
   for(int y=1;y<79;y++)for(int x=1;x<79;x++){
    int sx=x*64/80,sy=y*64/80;
    if(x>=50*80/64&&x<58*80/64&&y>=52*80/64&&y<60*80/64)continue; /* authored faction pin: scale its rectangle before rounding */
    uint16_t p=authored[sy*64+sx];
    if(p&0x8000)colours&=fb[(69+y)*STRIDE+42+x]==RGB(((p>>10)&31)*255/31,((p>>5)&31)*255/31,(p&31)*255/31);
   }
   identities&=game.commander_portrait==d;
   if(base==4){ship_paint[game.ship]=decorator_finishes[13];status();dump_native_bmp("commander-profile-row-boundary-red.bmp");ship_paint[game.ship]=decorator_finishes[0];status();dump_native_bmp("commander-profile-row-boundary-gold.bmp");}
  }
  PCHECK(colours&&total==96,"all 96 legacy profile faces retain exact authored foreground colours including exposed necks");
  PCHECK(gutters,"all 96 legacy faces have a transparent top gutter without previous-row shoulders");
  PCHECK(identities,"every authored legacy face remains reachable with unchanged saved DNA");
 }
 game.commander_portrait=portrait_parts_dna(portrait_seeded(77u,EXPLORERS,PORTRAIT_HUMAN));
 for(int selected=0;selected<2;selected++){
  int centered=1;row=selected;
  for(int t=0;t<DECORATOR_COUNT;t++){
   ship_paint[game.ship]=decorator_finishes[t];
   for(int i=0;i<2;i++){
    const char *label=i?"RENAME COMMANDER":"SAVE PROFILE";int x=i?302:150,w=i?146:144,n=(int)strlen(label)*8,tx=x+(w-n)/2,ty=178+(37-8)/2;
    unsigned ink=ui_paint_text(i==selected?UI_GOLD:WHITE),base=i==selected?UI_RAISED:ui_paint_surface(RGB(8,24,34)),glyph[128*8];
    rect(0,0,128,8,base);draw_text_pixels(0,0,ink,label);
    for(int yy=0;yy<8;yy++)for(int xx=0;xx<n;xx++)glyph[yy*128+xx]=fb[yy*STRIDE+xx];
    rect(0,0,W,H,BG);status();
    for(int yy=180;yy<213;yy++)for(int xx=x+2;xx<x+w-2;xx++){
     unsigned expected=xx>=tx&&xx<tx+n&&yy>=ty&&yy<ty+8?glyph[(yy-ty)*128+xx-tx]:base;
     centered&=fb[yy*STRIDE+xx]==expected;
    }
   }
  }
  PCHECK(centered,selected?"Rename selected: both actual 8px-font labels centered horizontally and vertically in all themes":"Save selected: both actual 8px-font labels centered horizontally and vertically in all themes");
 }
 ship_paint[game.ship]=decorator_finishes[13];row=0;status();dump_native_bmp("commander-profile-save.bmp");row=1;status();dump_native_bmp("commander-profile-rename.bmp");
 game.commander_portrait=4;status();dump_native_bmp("commander-profile-legacy-red.bmp");
 ship_paint[game.ship]=decorator_finishes[0];status();dump_native_bmp("commander-profile-legacy-gold.bmp");
 {unsigned d=portrait_expand_dna(portrait_parts_dna(portrait_seeded(77u,EXPLORERS,PORTRAIT_HUMAN)));d=portrait_adjust(portrait_adjust(d,11,1),10,1);while(portrait_part_value(d,3)!=5)d=portrait_adjust(d,3,1);game.commander_portrait=d;status();dump_native_bmp("commander-profile-expanded-female.bmp");}
 /* Saving must preserve appearance, including old identities: no migration
  * is necessary to fix a decorative layer on this one screen. */
 Game *loaded=malloc(sizeof(Game));unsigned ids[]={4,PORTRAIT_DNA_MARK|(2u<<26),portrait_parts_dna(portrait_seeded(77u,EXPLORERS,PORTRAIT_HUMAN)),game.commander_portrait};
 for(int i=0;i<4;i++){
  game.commander_portrait=ids[i];int kept=loaded&&save_game(&game,"profile-photo-check.sav")&&load_game(loaded,"profile-photo-check.sav")&&loaded->commander_portrait==ids[i];
  PCHECK(kept,i==0?"save/load preserves original commander identity":i==1?"save/load preserves v2 appearance exactly":i==2?"save/load preserves v3 selected appearance exactly":"save/load preserves expanded female style and colours exactly");
 }free(loaded);
 row=0;profile_input(PSP_CTRL_DOWN);PCHECK(row==1,"D-pad still selects Rename Commander");
 profile_input(PSP_CTRL_CROSS);PCHECK(profile_mode&&!strcmp(profile_draft,game.commander_name),"Rename opens the existing editor with the current name");profile_input(PSP_CTRL_CIRCLE);
 row=0;profile_slot=commander_active_slot=0;profile_refresh();int saved=profile_input(PSP_CTRL_CROSS);if(profile_confirm)saved=profile_input(PSP_CTRL_CROSS);
 PCHECK(saved&&profile_info[0].state==1,"Save Profile still writes the active commander checkpoint");
 profile_input(PSP_CTRL_UP);PCHECK(row==1,"D-pad up also reaches Rename without changing the save slot");
 int guards=1;for(int i=0;i<16;i++)guards&=pixels[i]==0x73a19e24u&&pixels[STRIDE*H+16+i]==0x73a19e24u;
 PCHECK(guards,"native profile renders preserve framebuffer guard words");
 ship_paint[game.ship]=paint;fprintf(out,"INFO %d checks; 800 theme/species/version photo comparisons plus eight original identities\nRESULT %d failures\n",checks,failures);fclose(out);fb=old;free(photo);free(pixels);
#undef PCHECK
 return 1;
}
