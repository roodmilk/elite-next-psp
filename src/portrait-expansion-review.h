/* Expanded DNA, actual PSP editor/save actions and native screenshot review. */
static unsigned portrait_expansion_select(unsigned d,int p,int value){
 for(int n=0;n<16&&portrait_part_value(d,p)!=value;n++)d=portrait_adjust(d,p,1);return d;
}
static void portrait_expansion_review(FILE *out,int *fail,int *checks){
#define PEC(c,s) do{int ok=(c);fprintf(out,"%s %s\n",ok?"PASS":"FAIL",s);(*checks)++;*fail+=!ok;fflush(out);}while(0)
 int fields=1,visible=1,nav=1,bindings=1,valid=1,upgrade=1,hidden=1;unsigned coverage[12]={0};
 for(int sp=0;sp<10;sp++){
  unsigned previous=portrait_review_base(sp),d=portrait_expand_dna(previous);
  for(int size=20;size<=96;size=size==20?32:size==32?96:97)upgrade&=portrait_review_hash(previous,size)==portrait_review_hash(d,size);
  for(int part=0;part<12;part++)upgrade&=portrait_part_value(previous,part)==portrait_part_value(d,part);
  int count=0,seen=0;for(int part=0;part<12;part++)count+=portrait_choice_count(d,part)>1;
  game.commander_portrait=d;profile_portrait_edit=1;profile_portrait_part=0;
  for(int i=0;i<count;i++){nav&=portrait_choice_count(d,profile_portrait_part)>1&&!(seen&(1<<profile_portrait_part));seen|=1<<profile_portrait_part;profile_input(PSP_CTRL_DOWN);}
  nav&=profile_portrait_part==0;profile_input(PSP_CTRL_UP);nav&=portrait_choice_count(d,profile_portrait_part)>1;profile_input(PSP_CTRL_DOWN);nav&=profile_portrait_part==0;profile_input(PSP_CTRL_CIRCLE);
  for(int gender=0;gender<(sp==0||sp==7?2:1);gender++)for(int context=0;context<2;context++){
   unsigned base=portrait_expansion_select(d,11,gender);
   if(context)for(int p=1;p<11;p++)base=portrait_expansion_select(base,p,portrait_choice_count(base,p)-1);
   for(int part=1;part<12;part++){
    int options=portrait_choice_count(base,part);if(options<2)continue;unsigned ids[16],hash[16];
    for(int value=0;value<options;value++){
     unsigned next=portrait_expansion_select(base,part,value);ids[value]=next;
     valid&=portrait_valid(next)&&portrait_version(next)==4;coverage[part]|=1u<<value;
     for(int other=0;other<12;other++)if(other!=part)fields&=portrait_part_value(next,other)==portrait_part_value(base,other);
     game.commander_portrait=next;profile_portrait_edit=1;profile_portrait_part=part;profile_input(PSP_CTRL_RIGHT);bindings&=game.commander_portrait==portrait_adjust(next,part,1);profile_input(PSP_CTRL_LEFT);bindings&=game.commander_portrait==next;
    }
    for(int size=20;size<=96;size=size==20?32:size==32?96:97){
     for(int value=0;value<options;value++){hash[value]=portrait_review_hash(ids[value],size);for(int other=0;other<value;other++)visible&=hash[value]!=hash[other];}
    }
   }
  }
  /* Gender and hairstyle remain stored but must not alter unsupported rigs. */
  if(sp!=0&&sp!=7){unsigned altered=d|(1u<<29)|(1u<<25)|(1u<<28);hidden&=portrait_valid(altered)&&portrait_review_hash(d,96)==portrait_review_hash(altered,96)&&portrait_adjust(altered,10,1)==altered&&portrait_adjust(altered,11,1)==altered;}
 }
 profile_portrait_edit=0;
 PEC(fields&&valid,"expanded controls preserve every other selected field and valid four-byte DNA");
 PEC(visible,"every expanded option is visibly distinct at actual 20px, 32px and 96px sizes");
 PEC(nav,"expanded D-pad navigation wraps through supported controls in display order");
 PEC(bindings,"expanded actual left/right editor input visits every option and reverses exactly");
 PEC(upgrade,"v3-to-expanded migration is pixel-identical and preserves every existing selection");
 PEC(hidden,"unsupported gender and hairstyle selections stay inactive without being erased");
 int saved=1;static Game loaded;game_init(&game);
 for(int sp=0;sp<10;sp++)for(int gender=0;gender<(sp==0||sp==7?2:1);gender++)for(int style=0;style<(sp==0||sp==7?4:1);style++){
  unsigned d=portrait_expand_dna(portrait_review_base(sp));d=portrait_expansion_select(d,11,gender);d=portrait_expansion_select(d,10,style);
  for(int part=1;part<10;part++)d=portrait_expansion_select(d,part,portrait_choice_count(d,part)-1);
  game.commander_portrait=d;unsigned a=portrait_review_hash(d,20),b=portrait_review_hash(d,32),c=portrait_review_hash(d,96);
  saved&=save_game(&game,"portrait-expanded.sav")&&load_game(&loaded,"portrait-expanded.sav")&&loaded.commander_portrait==d&&portrait_review_hash(loaded.commander_portrait,20)==a&&portrait_review_hash(loaded.commander_portrait,32)==b&&portrait_review_hash(loaded.commander_portrait,96)==c;
 }
 PEC(saved,"expanded gender, hair styles, colours and local parts survive real V32 save/load");
 {unsigned d=portrait_expand_dna(portrait_review_base(0));int reject=1;
  static const unsigned char shifts[]={0,5,9,13,16,19,21,23},masks[]={15,15,15,7,7,3,3,3},bad[]={10,12,12,5,6,3,3,3};
  for(int n=0;n<8;n++)reject&=!portrait_valid(portrait_set_field(d,shifts[n],masks[n],bad[n]));
  reject&=!portrait_valid(d&~PORTRAIT_DNA_MARK);PEC(reject,"expanded validator rejects out-of-range fields and unmarked data");
 }
 {int random=1;unsigned seen[12]={0};for(unsigned seed=0;seed<2000;seed++){unsigned d=portrait_commander_random(seed);random&=portrait_valid(d);for(int part=0;part<12;part++)seen[part]|=1u<<portrait_part_value(d,part);}
  unsigned d=portrait_expand_dna(portrait_review_base(0));for(int part=1;part<12;part++)random&=seen[part]==(1u<<portrait_choice_count(d,part))-1u;PEC(random,"2000 random new looks cover expanded gender, hairstyle and eye choices legally");
 }
 opening_new_game();
 {unsigned old=portrait_review_base(0);for(int p=1;p<10;p++)old=portrait_expansion_select(old,p,portrait_choice_count(old,p)-1);old=portrait_adjust(old,0,1);
  game.commander_portrait=old;row=3;profile_input(PSP_CTRL_CROSS);int retained=portrait_valid(game.commander_portrait);for(int p=0;p<12;p++)retained&=portrait_part_value(old,p)==portrait_part_value(game.commander_portrait,p);PEC(retained,"opening the real editor preserves inactive saved selections when upgrading species");profile_portrait_edit=0;
 }
 opening_new_game();row=3;profile_input(PSP_CTRL_CROSS);
 PEC(profile_portrait_edit&&portrait_version(game.commander_portrait)==4&&portrait_choice_count(game.commander_portrait,10)==4&&portrait_choice_count(game.commander_portrait,11)==2,"real new-commander flow opens the expanded editor with hair and gender controls");
 for(int sp=0;sp<=7;sp+=7)for(int gender=0;gender<2;gender++){
  unsigned base=portrait_expand_dna(portrait_review_base(sp));base=portrait_expansion_select(base,11,gender);
  rect(0,0,W,H,BG);text_px(8,8,UI_GOLD,"HAIR STYLES / %s / %s",portrait_species_name(base),gender?"FEMALE":"MALE");
  for(int style=0;style<4;style++){
   unsigned d=portrait_expansion_select(base,10,style);int x=8+style*118;
   portrait_draw(x,32,96,96,d,EXPLORERS);portrait_draw(x,138,20,20,d,EXPLORERS);portrait_draw(x+28,134,32,32,d,EXPLORERS);text_px(x,174,WHITE,"%s",portrait_choice_caption(d,10));
   d=portrait_expansion_select(d,3,3);portrait_draw(x,194,64,64,d,EXPLORERS);d=portrait_expansion_select(d,3,5);portrait_draw(x+70,210,32,32,d,EXPLORERS);
  }
  char file[80];snprintf(file,sizeof(file),"expanded-styles-%d-%d.bmp",sp,gender);dump_native_bmp(file);
  game.commander_portrait=portrait_expansion_select(portrait_expansion_select(base,10,2),3,gender?5:3);profile_portrait_edit=1;profile_portrait_part=11;page=STATUS;rect(0,0,W,H,BG);status();snprintf(file,sizeof(file),"expanded-editor-%d-%d.bmp",sp,gender);dump_native_bmp(file);
 }
 {unsigned base=portrait_expansion_select(portrait_expand_dna(portrait_review_base(0)),10,2);
  portrait_review_pair("expanded-gender.bmp",base,portrait_adjust(base,11,1),"GENDER / MALE TO FEMALE");
  portrait_review_pair("expanded-blonde-blue.bmp",portrait_expansion_select(base,3,3),portrait_expansion_select(base,3,5),"HAIR / BLONDE TO BLUE");
  portrait_review_pair("expanded-wide-eyes.bmp",base,portrait_expansion_select(base,4,3),"EYES / NATURAL TO WIDE");
 }
 for(int sp=0;sp<=7;sp+=7){
  unsigned female=portrait_expansion_select(portrait_expand_dna(portrait_review_base(sp)),11,1);
  char file[80];snprintf(file,sizeof(file),"authored-crop-%d.bmp",sp);
  portrait_review_pair(file,female,portrait_expansion_select(female,10,1),"HAIR / CLASSIC TO AUTHORED CROP");
  snprintf(file,sizeof(file),"authored-smirk-%d.bmp",sp);
  portrait_review_pair(file,female,portrait_expansion_select(female,8,2),"MOUTH / NATURAL TO AUTHORED SMIRK");
  unsigned cropped=portrait_expansion_select(female,10,1);
  snprintf(file,sizeof(file),"authored-ears-%d.bmp",sp);
  portrait_review_pair(file,portrait_expansion_select(cropped,9,1),portrait_expansion_select(cropped,9,2),"EARS / EXTENDED TO POINTED");
 }
 profile_portrait_edit=0;remove("portrait-expanded.sav");remove("portrait-expanded.sav.bak");remove("portrait-expanded.sav.tmp");
#undef PEC
}
