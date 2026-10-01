{
 TEST_INIT();page=FACTIONS;
 for(int role=0;role<4;role++)for(int card=0;card<3;card++){
  row=role;faction_lore_card=card;rect(0,0,W,H,BG);factions();const char *left;
  text_wrap(18,14,39,8,WHITE,faction_dossier[role][card],&left);
  INPUT_CHECK(!*left,"faction dossier: full lore paragraph fits native panel");
  text_wrap(18,25,39,4,DIM,faction_note(role,card),&left);
  INPUT_CHECK(!*left,"faction dossier: complete note fits without truncation");
  char file[64];snprintf(file,sizeof(file),"faction-dossier-%d-%d.bmp",role,card);dump_native_bmp(file);
 }
 for(int role=0;role<4;role++){
  for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;
  game.npc[0].role=role;game.npc[0].alive=1;
  INPUT_CHECK(faction_local_count(role)==1,"faction snapshot counts living local ships");
  game.npc[0].alive=0;INPUT_CHECK(faction_local_count(role)==0,"faction snapshot removes destroyed ships");
 }
 TEST_INIT();page=FACTIONS;row=0;faction_lore_card=0;
 for(int i=0;i<3;i++)input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(faction_lore_card==0&&page==FACTIONS,"faction pages cycle back to identity without leaving dossier");
 input(PSP_CTRL_CROSS,0,.016f,0,0);input(PSP_CTRL_DOWN,0,.016f,0,0);
 INPUT_CHECK(row==1&&faction_lore_card==0,"changing faction starts at its identity page");
 TEST_INIT();page=HOME;
}
