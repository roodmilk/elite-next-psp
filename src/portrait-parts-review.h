/* Opt-in actual renderer, editor input and V32 save verification. */
static unsigned portrait_review_hash(unsigned d,int side){
 portrait_draw(8,8,side,side,d,EXPLORERS);unsigned h=2166136261u;
 for(int y=1;y<side-1;y++)for(int x=1;x<side-1;x++)h=(h^fb[(8+y)*STRIDE+8+x])*16777619u;
 return h;
}
static unsigned portrait_review_base(int sp){
 unsigned d=PORTRAIT_DNA_MARK|(3u<<26)|(unsigned)sp;
 d=portrait_set_field(d,6,7,0);d=portrait_set_field(d,9,7,1);d=portrait_set_field(d,14,3,1);return portrait_parts_dna(d);
}
static void portrait_review_pair(const char *file,unsigned a,unsigned b,const char *label){
 rect(0,0,W,H,RGB(13,23,34));text_px(12,8,UI_GOLD,"COMMANDER / FEATURE COMPARISON");
 text_px(24,28,WHITE,"BEFORE");text_px(264,28,WHITE,"AFTER");
 portrait_draw(24,44,96,96,a,EXPLORERS);portrait_draw(264,44,96,96,b,EXPLORERS);
 portrait_draw(136,44,20,20,a,EXPLORERS);portrait_draw(376,44,20,20,b,EXPLORERS);
 portrait_draw(136,76,32,32,a,EXPLORERS);portrait_draw(376,76,32,32,b,EXPLORERS);
 text_px(24,152,UI_CYAN,"%s",label);
 text_px(24,174,UI_MUTED,"96PX EDITOR / 20PX + 32PX DIALOGUE");
 rect(12,200,456,60,UI_PANEL);portrait_draw(20,212,32,32,b,EXPLORERS);
 text_px(64,210,UI_GOLD,"YOUR COMMANDER");text_px(64,230,WHITE,"Same selected features. New detail.");
 dump_native_bmp(file);
}
#include "portrait-expansion-review.h"
static int portrait_parts_review(void){
 FILE *flag=fopen("portrait-parts-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *out=fopen("portrait-parts-review.txt","w");unsigned *pixels=calloc(STRIDE*H+32,sizeof(unsigned)),*old=fb,*refs=malloc(16*96*96*sizeof(unsigned));
 if(!out||!pixels||!refs)return 1;fb=pixels+16;int fail=0,checks=0,total=0;
 for(int i=0;i<16;i++)pixels[i]=pixels[STRIDE*H+16+i]=0x3a815e24u;
#define PRC(c,s) do{int ok=(c);fprintf(out,"%s %s\n",ok?"PASS":"FAIL",s);checks++;fail+=!ok;fflush(out);}while(0)
 for(int part=0;part<PORTRAIT_PART_COUNT;part++){
  if(part==7){PRC(portrait_choice_count(portrait_review_base(0),part)==1,"background roulette omitted from the editor");continue;}
  int meaningful=1,isolated=1,preserved=1,contained=1;
  for(int sp=0;sp<(part?10:1);sp++)for(int context=0;context<2;context++){
   unsigned base=context?portrait_parts_dna(portrait_seeded(0x51a7u,EXPLORERS,sp)):portrait_review_base(sp);
   /* A nonzero context exercises marks, asymmetric heads and crest shapes. */
   int count=portrait_choice_count(base,part);unsigned ids[16];PortraitParts models[16];
   for(int v=0;v<count;v++){
    unsigned d=base;while(portrait_part_value(d,part)!=v)d=portrait_adjust(d,part,1);ids[v]=d;portrait_parts_build(&models[v],d);
    for(int other=0;other<PORTRAIT_PART_COUNT;other++)if(other!=part)preserved&=portrait_part_value(d,other)==portrait_part_value(base,other);
    preserved&=portrait_valid(d)&&portrait_version(d)==3;
   }
   for(int size=0;size<3;size++){
    int side=size==0?20:size==1?32:96;
    for(int v=0;v<count;v++){
     rect(0,0,W,H,RGB(3,5,7));portrait_draw(192,72,side,side,ids[v],EXPLORERS);
     for(int y=0;y<side;y++)for(int x=0;x<side;x++)refs[v*96*96+y*side+x]=fb[(72+y)*STRIDE+192+x];
     for(int y=70;y<72+side+2;y++){contained&=fb[y*STRIDE+190]==RGB(3,5,7)&&fb[y*STRIDE+193+side]==RGB(3,5,7);}
     for(int x=190;x<194+side;x++)contained&=fb[70*STRIDE+x]==RGB(3,5,7)&&fb[(73+side)*STRIDE+x]==RGB(3,5,7);
     for(int prior=0;prior<v;prior++){
      int changed=0,unrelated=0,background=0,colour_delta=0;
      for(int y=1;y<side-1;y++)for(int x=1;x<side-1;x++){
       if(refs[v*96*96+y*side+x]==refs[prior*96*96+y*side+x])continue;
       int at=(y*PORTRAIT_PART_TILE/side)*PORTRAIT_PART_TILE+x*PORTRAIT_PART_TILE/side,a=models[v].ink[at],b=models[prior].ink[at];
       if(!a&&!b){background++;continue;}changed++;
       unsigned ca=refs[v*96*96+y*side+x],cb=refs[prior*96*96+y*side+x];
       colour_delta+=abs((int)(ca&255)-(int)(cb&255))+abs((int)((ca>>8)&255)-(int)((cb>>8)&255))+abs((int)((ca>>16)&255)-(int)((cb>>16)&255));
       int allowed=part==0||(part==1&&a!=PC_SUIT1&&b!=PC_SUIT1)||models[v].owner[at]==part+1||models[prior].owner[at]==part+1;
       if(part==2)allowed=(a>=PC_SKIN0&&a<=PC_SKIN3)||(b>=PC_SKIN0&&b<=PC_SKIN3);
       if(part==3)allowed=(a>=PC_HAIR0&&a<=PC_HAIR3)||(b>=PC_HAIR0&&b<=PC_HAIR3);
       if(part==5)allowed=(a>=PC_SUIT0&&a<=PC_SUIT2)||(b>=PC_SUIT0&&b<=PC_SUIT2);
       unrelated+=!allowed;
      }
      int readable=changed>=2&&(part!=3||(changed>=3&&colour_delta>=changed*54));
      meaningful&=readable;isolated&=!unrelated&&!background;
      if(!readable||unrelated||background)fprintf(out,"DETAIL part%d sp%d context%d size%d values%d/%d foreground%d unrelated%d background%d colourdelta%d\n",part,sp,context,side,prior,v,changed,unrelated,background,colour_delta);
     }
     total++;
    }
   }
  }
  char label[160];snprintf(label,sizeof(label),"%s: every choice changes foreground at 96, 32 and 20px",portrait_choice_label(portrait_review_base(0),part));PRC(meaningful,label);
  snprintf(label,sizeof(label),"%s: only its named part/material changes; background excluded",portrait_choice_label(portrait_review_base(0),part));PRC(isolated,label);
  snprintf(label,sizeof(label),"%s: other selected fields, version and preview bounds preserved",portrait_choice_label(portrait_review_base(0),part));PRC(preserved&&contained,label);
 }
 /* Negative control: changing only transparent/background pixels must not
  * meet the same foreground-change threshold used above. */
 {unsigned d=portrait_review_base(0);PortraitParts m;portrait_parts_build(&m,d);portrait_draw(8,8,20,20,d,EXPLORERS);int foreground=0,background=0;
  for(int y=1;y<19;y++)for(int x=1;x<19;x++){int at=(y*64/20)*64+x*64/20;unsigned before=fb[(8+y)*STRIDE+8+x];if(!m.ink[at])fb[(8+y)*STRIDE+8+x]^=0x003f25u;int changed=before!=fb[(8+y)*STRIDE+8+x];foreground+=changed&&m.ink[at];background+=changed&&!m.ink[at];}
  PRC(background>0&&foreground<2,"negative control: a background-only change fails the foreground requirement");}
 int nav=1;
 for(int sp=0;sp<10;sp++){
  game.commander_portrait=portrait_review_base(sp);profile_portrait_edit=1;profile_portrait_part=0;int count=0,seen=0;
  for(int p=0;p<10;p++)count+=portrait_choice_count(game.commander_portrait,p)>1;
  for(int i=0;i<count;i++){nav&=portrait_choice_count(game.commander_portrait,profile_portrait_part)>1&&!(seen&(1<<profile_portrait_part));seen|=1<<profile_portrait_part;profile_input(PSP_CTRL_DOWN);}
  nav&=profile_portrait_part==0;profile_input(PSP_CTRL_UP);nav&=portrait_choice_count(game.commander_portrait,profile_portrait_part)>1;profile_input(PSP_CTRL_CIRCLE);
 }
 PRC(nav,"actual D-pad navigation visits only supported controls and wraps for all ten species");
 {int bindings=1;for(int sp=0;sp<10;sp++)for(int part=0;part<10;part++){
   unsigned d=portrait_review_base(sp);int count=portrait_choice_count(d,part);if(count<2)continue;
   for(int value=0;value<count;value++){
    game.commander_portrait=d;profile_portrait_edit=1;profile_portrait_part=part;
    unsigned expected=portrait_adjust(d,part,1);profile_input(PSP_CTRL_RIGHT);bindings&=game.commander_portrait==expected;
    profile_input(PSP_CTRL_LEFT);bindings&=game.commander_portrait==d;d=expected;
   }
  }
  unsigned high=portrait_review_base(0);for(int part=2;part<=5;part++){if(part==4)continue;int target=portrait_choice_count(high,part)-1;while(portrait_part_value(high,part)!=target)high=portrait_adjust(high,part,1);}
  bindings&=portrait_adjust(high,7,1)==high&&portrait_adjust(high,7,-1)==high;
  PRC(bindings,"actual left/right visits every visible choice; hidden controls cannot reset expanded palette fields");}
 int hidden=1;
 for(int sp=0;sp<10;sp++)for(int p=1;p<10;p++){
  if(p==5||p==7)continue;unsigned d=portrait_review_base(sp);if(portrait_choice_count(d,p)>1)continue;
  static const unsigned char shift[]={0,4,6,9,12,14,16,19,22,28},mask[]={15,3,7,7,3,3,7,7,3,7};
  unsigned a=portrait_set_field(d,shift[p],mask[p],0);if(p==3)a=portrait_set_field(a,20,1,0);
  int limit=p==1?2:p==3?12:3;
  for(int value=1;value<limit;value++){
   unsigned b=portrait_set_field(a,shift[p],mask[p],value);if(p==3)b=portrait_set_field(b,20,1,value>>3);
   hidden&=portrait_valid(a)&&portrait_valid(b)&&portrait_review_hash(a,96)==portrait_review_hash(b,96)&&portrait_review_hash(a,20)==portrait_review_hash(b,20);
  }
 }
 PRC(hidden,"inactive saved feature selections do not draw unsupported anatomy after changing species");
 /* Independently reviewed source-art landmarks. A wrong mask must fail even
  * when its own material/ownership bookkeeping is internally consistent. */
 {static const unsigned char probes[][4]={
   {0,43,5,1},{0,34,12,1},{0,26,17,1},{0,24,20,1},{0,28,22,1},{0,49,11,1},
   {0,40,14,0},{0,41,17,0},{0,34,20,0},{0,44,18,0},{0,34,18,0},{0,24,32,0},
   {7,35,5,1},{7,22,8,1},{7,19,14,1},{7,15,18,1},{7,45,8,1},
   {7,34,12,0},{7,32,16,0},{7,42,17,0},{7,18,28,0},{7,36,31,0}};
  int landmarks=1,rendered=1;
  for(unsigned n=0;n<sizeof(probes)/sizeof(probes[0]);n++){
   int sp=probes[n][0],x=probes[n][1],y=probes[n][2],hair=probes[n][3];
   unsigned d=portrait_review_base(sp);PortraitParts m;portrait_parts_build(&m,d);
   landmarks&=(m.ink[y*64+x]==PC_HAIR1)==hair;
   portrait_draw(192,72,64,64,d,EXPLORERS);unsigned before=fb[(72+y)*STRIDE+192+x];
   for(int colour=3;colour<=5;colour+=2){unsigned next=portrait_set_field(d,9,7,colour);portrait_draw(192,72,64,64,next,EXPLORERS);unsigned after=fb[(72+y)*STRIDE+192+x];rendered&=hair?after!=before:after==before;}
  }
  PRC(landmarks,"independent atlas landmarks classify top curls and temples as hair, forehead and ears as face");
  PRC(rendered,"blonde and blue recolour reviewed hair landmarks and leave reviewed face pixels identical");
 }
 int saved=1;static Game loaded;game_init(&game);
 for(int sp=0;sp<10;sp++)for(int version=0;version<4;version++){
  unsigned d=version==0?(unsigned)(sp%8):version==1?portrait_seeded(177,EXPLORERS,sp):version==2?(PORTRAIT_DNA_MARK|(2u<<26)|(unsigned)sp|(1u<<9)|(1u<<14)):portrait_review_base(sp);
  game.commander_portrait=d;unsigned a=portrait_review_hash(d,96),b=portrait_review_hash(d,20);
  int ok=save_game(&game,"portrait-compat.sav")&&load_game(&loaded,"portrait-compat.sav");
  saved&=ok&&loaded.commander_portrait==d&&portrait_review_hash(loaded.commander_portrait,96)==a&&portrait_review_hash(loaded.commander_portrait,20)==b;
 }
 PRC(saved,"V32 real save/load preserves legacy IDs, authored DNA and new parts DNA/pixels");
 {int expanded=1,upgrade=1;
  for(int sp=0;sp<10;sp++){
   fprintf(out,"INFO expanded save and migration species %d\n",sp);fflush(out);
   unsigned d=portrait_review_base(sp);
   for(int part=1;part<10;part++){
    int target=portrait_choice_count(d,part)-1;for(int steps=0;portrait_part_value(d,part)!=target&&steps<32;steps++)d=portrait_adjust(d,part,1);
    expanded&=portrait_part_value(d,part)==target;
   }
   game.commander_portrait=d;unsigned a=portrait_review_hash(d,96),b=portrait_review_hash(d,20);
   expanded&=save_game(&game,"portrait-compat.sav")&&load_game(&loaded,"portrait-compat.sav")&&loaded.commander_portrait==d&&portrait_review_hash(loaded.commander_portrait,96)==a&&portrait_review_hash(loaded.commander_portrait,20)==b;
   unsigned v2=PORTRAIT_DNA_MARK|(2u<<26)|(unsigned)sp|(7u<<6)|(7u<<9)|(1u<<12)|(3u<<14)|(1u<<16)|(1u<<22)|(1u<<28),v3=portrait_parts_dna(v2);
   for(int part=0;part<10;part++)upgrade&=portrait_part_value(v2,part)==portrait_part_value(v3,part);
   upgrade&=portrait_valid(v2)&&portrait_valid(v3)&&portrait_review_hash(v2,96)==portrait_review_hash(v3,96)&&portrait_review_hash(v2,20)==portrait_review_hash(v3,20);
  }
  PRC(expanded,"expanded palette high bits and maximum local-part selections survive real V32 saves for all ten species");
  PRC(upgrade,"v2-to-v3 upgrade retains every selected field, inactive features and the same rendered appearance");
 }
 unsigned legacy=portrait_seeded(277,TRADERS,0);game.commander_portrait=legacy;profile_portrait_edit=1;profile_portrait_part=4;profile_input(PSP_CTRL_RIGHT);int keep=game.commander_portrait==legacy;profile_input(PSP_CTRL_CIRCLE);
 PRC(keep&&game.commander_portrait==legacy,"opening and leaving legacy editor preserves the saved appearance until explicit New Look");
 game.commander_portrait=legacy;profile_portrait_edit=1;profile_input(PSP_CTRL_SQUARE);int opt=portrait_version(game.commander_portrait)==4;profile_portrait_part=4;unsigned expected=portrait_adjust(game.commander_portrait,4,1);profile_input(PSP_CTRL_RIGHT);
 PRC(opt&&game.commander_portrait==expected,"New Look and actual D-pad editor input use isolated feature actions");
 int random_ok=1;unsigned seen[10]={0};for(unsigned i=0;i<1000;i++){unsigned d=portrait_commander_random(i);random_ok&=portrait_valid(d);for(int part=0;part<10;part++)seen[part]|=1u<<portrait_part_value(d,part);}
 for(int part=0;part<10;part++)if(part!=7)random_ok&=seen[part]==(1u<<portrait_choice_count(portrait_expand_dna(portrait_review_base(0)),part))-1u;
 PRC(random_ok,"1000 random new looks remain legal and reach every expanded colour and feature option");
 opening_new_game();PRC(portrait_version(game.commander_portrait)==4,"new Commander Creation starts with the expanded parts renderer");
 for(int part=0;part<PORTRAIT_PART_COUNT;part++){
  unsigned a=portrait_review_base(0);if(portrait_choice_count(a,part)<2)continue;unsigned b=portrait_adjust(a,part,1);char file[80],label[80];snprintf(file,sizeof(file),"comparison-%02d.bmp",part);snprintf(label,sizeof(label),"%s / %d TO %d",portrait_choice_label(a,part),portrait_part_value(a,part)+1,portrait_part_value(b,part)+1);portrait_review_pair(file,a,b,label);
 }
 portrait_review_pair("legacy-before-after.bmp",legacy,portrait_adjust(legacy,4,1),"OLD EYE CONTROL SWAPPED THE WHOLE FACE");
 {unsigned a=portrait_review_base(0),blonde=a,blue=a;while(portrait_part_value(blonde,3)!=3)blonde=portrait_adjust(blonde,3,1);while(portrait_part_value(blue,3)!=5)blue=portrait_adjust(blue,3,1);
  portrait_review_pair("hair-brown-blonde.bmp",a,blonde,"HAIR COLOUR / BROWN TO BLONDE");portrait_review_pair("hair-brown-blue.bmp",a,blue,"HAIR COLOUR / BROWN TO BLUE");}
 rect(0,0,W,H,BG);text_px(12,8,UI_GOLD,"SUPPORTED PARTS / ALL TEN SPECIES");
 for(int sp=0;sp<10;sp++){int x=14+(sp%5)*94,y=32+(sp/5)*108;unsigned d=portrait_review_base(sp);portrait_draw(x,y,64,64,d,EXPLORERS);portrait_draw(x+68,y+4,20,20,d,EXPLORERS);text_px(x,y+72,UI_CYAN,"%.10s",portrait_species_name(d));}
 dump_native_bmp("species-parts.bmp");
 rect(0,0,W,H,BG);text_px(12,8,UI_GOLD,"COMBINED FEATURES / ALL TEN SPECIES");
 for(int sp=0;sp<10;sp++){
  unsigned d=portrait_review_base(sp);for(int part=1;part<10;part++){int target=portrait_choice_count(d,part)-1;for(int steps=0;portrait_part_value(d,part)!=target&&steps<32;steps++)d=portrait_adjust(d,part,1);}
  int x=14+(sp%5)*94,y=32+(sp/5)*108;portrait_draw(x,y,64,64,d,EXPLORERS);portrait_draw(x+68,y+4,20,20,d,EXPLORERS);text_px(x,y+72,UI_CYAN,"%.10s",portrait_species_name(d));
 }dump_native_bmp("species-max.bmp");
 for(int part=2;part<=5;part++){
  if(part==4)continue;unsigned base=portrait_review_base(0);int count=portrait_choice_count(base,part);
  rect(0,0,W,H,BG);text_px(12,8,UI_GOLD,"NATIVE PALETTES / %s",portrait_choice_label(base,part));
  for(int v=0;v<count;v++){
   unsigned d=base;while(portrait_part_value(d,part)!=v)d=portrait_adjust(d,part,1);
   int x=10+(v%4)*116,y=28+(v/4)*76;portrait_draw(x,y,64,64,d,EXPLORERS);portrait_draw(x+70,y+2,20,20,d,EXPLORERS);text_px(x,y+65,WHITE,"%s",portrait_choice_caption(d,part));
  }char file[64];snprintf(file,sizeof(file),"palettes-%d.bmp",part);dump_native_bmp(file);
 }
 {unsigned a=portrait_review_base(0),b=portrait_set_field(a,14,3,2);portrait_review_pair("neck-teal-red.bmp",a,b,"OUTFIT / NECK SKIN STAYS UNCHANGED");}
 {static const unsigned char neck[][3]={{0,29,49},{0,30,51},{0,32,53},{0,26,46},{7,21,43},{7,23,46},{7,29,47},{7,33,49}};int okay=1;
  for(unsigned n=0;n<sizeof(neck)/sizeof(neck[0]);n++){
   int sp=neck[n][0],x=neck[n][1],y=neck[n][2];unsigned d=portrait_review_base(sp);PortraitParts m;portrait_parts_build(&m,d);okay&=m.ink[y*64+x]==PC_SKIN2;
   portrait_draw(8,8,64,64,d,EXPLORERS);unsigned colour=fb[(8+y)*STRIDE+8+x];
   for(int suit=0;suit<6;suit++){unsigned next=d;while(portrait_part_value(next,5)!=suit)next=portrait_adjust(next,5,1);portrait_draw(8,8,64,64,next,EXPLORERS);okay&=fb[(8+y)*STRIDE+8+x]==colour;}
  }PRC(okay,"independent exposed-neck landmarks retain skin and never take any of six uniform colours");
 }
 game.commander_portrait=portrait_review_base(0);profile_portrait_edit=1;profile_from_intro=0;profile_portrait_part=4;page=STATUS;rect(0,0,W,H,BG);status();dump_native_bmp("editor-before.bmp");profile_input(PSP_CTRL_RIGHT);rect(0,0,W,H,BG);status();dump_native_bmp("editor-after.bmp");
 /* Same legacy crop as the pre-repair opening-check capture, for external
  * byte comparison against the already shipped 2.5.361 executable. */
 rect(0,0,W,H,BG);draw_commander_portrait(24,82,96,legacy);dump_native_bmp("legacy-preserved.bmp");
 portrait_expansion_review(out,&fail,&checks);
 int guards=1;for(int i=0;i<16;i++)guards&=pixels[i]==0x3a815e24u&&pixels[STRIDE*H+16+i]==0x3a815e24u;PRC(guards,"framebuffer guard words remain intact after every render and editor screen");
 fprintf(out,"INFO %d real foreground render cases; no background-only hashes.\nRESULT %d failures\n",total,fail);fclose(out);fb=old;free(pixels);free(refs);
 remove("portrait-compat.sav");remove("portrait-compat.sav.bak");remove("portrait-compat.sav.tmp");return 1;
#undef PRC
}
