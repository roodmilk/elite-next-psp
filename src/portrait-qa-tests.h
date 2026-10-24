/* New Commander Creation foreground checks. The detailed standalone review
 * additionally checks region isolation, all pairs at three sizes and saves. */
{
 int ship=0,all_contained=1;
 for(int part=0;part<PORTRAIT_PART_COUNT;part++){
  if(part==7){OPEN_CHECK(portrait_choice_count(portrait_parts_dna(PORTRAIT_DNA_BASE),part)==1,"background roulette is not advertised");continue;}
  int all_distinct=1;
  for(int sp=0;sp<(part?PORTRAIT_SPECIES_COUNT:1);sp++){
   unsigned d=portrait_parts_dna(portrait_seeded(0x51a7u,EXPLORERS,sp));
   int count=portrait_choice_count(d,part);unsigned long long hashes[16]={0};
   for(int value=0;value<count;value++){
    while(portrait_part_value(d,part)!=value)d=portrait_adjust(d,part,1);
    PortraitParts model;portrait_parts_build(&model,d);
    const unsigned guard=RGB(3,5,7);rect(0,0,W,H,guard);portrait_draw(192,72,96,96,d,EXPLORERS);
    for(int x=190;x<=289;x++){all_contained&=fb[70*STRIDE+x]==guard;all_contained&=fb[169*STRIDE+x]==guard;}
    for(int y=70;y<=169;y++){all_contained&=fb[y*STRIDE+190]==guard;all_contained&=fb[y*STRIDE+289]==guard;}
    unsigned long long h=1469598103934665603ull;
    for(int y=1;y<95;y++)for(int x=1;x<95;x++)if(model.ink[(y*PORTRAIT_PART_TILE/96)*PORTRAIT_PART_TILE+x*PORTRAIT_PART_TILE/96]){h^=(unsigned)(x+y*96);h*=1099511628211ull;h^=fb[(72+y)*STRIDE+192+x];h*=1099511628211ull;}
    hashes[value]=h;for(int prior=0;prior<value;prior++)if(hashes[prior]==h)all_distinct=0;
   }
  }
  char label[120];snprintf(label,sizeof(label),"portrait foreground: every %s choice is visibly distinct",portrait_choice_label(portrait_parts_dna(PORTRAIT_DNA_BASE),part));OPEN_CHECK(all_distinct,label);
 }
 OPEN_CHECK(all_contained,"portrait render: all actual parts stay inside the native editor preview");
}
