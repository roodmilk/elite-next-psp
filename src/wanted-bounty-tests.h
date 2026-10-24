/* Compiled only into the review binary. Exercise the real destruction path. */
int wanted_bounty_tests(Game *g,const char *path){
 FILE *out=fopen(path,"w");if(!out)return 1;int failures=0;game_init(g);story_complete(g);g->opening.stage=OP_FREE;
#define WBCHECK(c,label) do{int ok=!!(c);fprintf(out,"%s %s\n",ok?"PASS":"FAIL",label);failures+=!ok;fflush(out);}while(0)
 int payouts=1,duplicates=1,respawn=1,range=1,display=1,min=999999,max=0;
 for(int sys=0;sys<256;sys++){
  g->system=sys;g->bounty_claimed[sys]=0;g->job_n=0;g->contract=-1;game_spawn(g);
  for(int p=0;p<BOUNTY_POSTER_COUNT;p++){
   int id=bounty_target_index(g,p),cash=g->credits,reward=bounty_target_reward(g,p);char shown[24];
   snprintf(shown,sizeof(shown),"%.1f U",reward*.1f);display&=(int)roundf(strtof(shown,0)*10)==reward;
   if(reward<min)min=reward;
   if(reward>max)max=reward;
   range&=reward>=6000&&reward<=25000;
   hit(g,id,100000,1);payouts&=g->credits-cash==reward&&bounty_target_taken(g,p)&&!g->npc[id].alive;
   cash=g->credits;hit(g,id,100000,1);duplicates&=cash==g->credits;
   /* Even a stale/reanimated actor must not repay an already claimed record. */
   g->npc[id].alive=1;g->npc[id].health=1;g->npc[id].shield=0;hit(g,id,100000,1);duplicates&=cash==g->credits;
  }
  game_spawn(g);for(int p=0;p<BOUNTY_POSTER_COUNT;p++)respawn&=!g->npc[bounty_target_index(g,p)].alive;
 }
 WBCHECK(payouts,"all 1280 real wanted-target destructions pay exactly the shared board reward");
 WBCHECK(display,"all 1280 displayed decimal rewards round-trip to the exact credited amount");
 WBCHECK(duplicates,"repeated hits and stale reanimated actors never pay a claimed warrant twice");
 WBCHECK(respawn,"all five claimed targets remain absent after every system respawn");
 WBCHECK(range&&min==6000&&max==25000,"actual galaxy bounty range is 600.0 to 2500.0 units");
 g->system=7;g->bounty_claimed[7]=0;game_spawn(g);int cash=g->credits;
 g->npc[BOUNTY_NPC_FIRST].player_tag=90;hit(g,BOUNTY_NPC_FIRST,100000,0);
 WBCHECK(g->credits-cash==bounty_target_reward(g,0),"police assistance preserves the engaged player's bounty credit");
 cash=g->credits;int saved=save_game(g,"wanted-test.sav");g->bounty_claimed[7]=0;g->credits=0;
 int loaded=load_game(g,"wanted-test.sav");
 WBCHECK(saved&&loaded&&g->credits==cash&&bounty_target_taken(g,0)&&!g->npc[BOUNTY_NPC_FIRST].alive,"current-save reload preserves paid credits and claimed-state protection");
 WBCHECK(bounty_target_reward(g,-1)==0&&bounty_target_reward(g,5)==0&&!bounty_target_reward(0,0),"invalid warrant identities cannot create a bounty");
 fprintf(out,"RESULT %d failures\n",failures);fclose(out);
#undef WBCHECK
 return failures;
}
