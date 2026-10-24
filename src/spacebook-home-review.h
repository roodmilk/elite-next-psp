static int spacebook_home_review(void){
 FILE *flag=fopen("spacebook-home-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *f=fopen("spacebook-home-review.txt","w");if(!f)return 1;int failures=0;
 unsigned *pixels=malloc(STRIDE*H*sizeof(unsigned)),*saved_fb=fb;if(!pixels){fprintf(f,"RESULT 1 failures\n");fclose(f);return 1;}fb=pixels;
#define BCHECK(ok,label) do{int pass=!!(ok);fprintf(f,"%s %s\n",pass?"PASS":"FAIL",label);failures+=!pass;fflush(f);}while(0)
#define BFRAME(name) do{rect(0,0,W,H,BG);galnet_screen();dump_native_bmp(name);}while(0)
 game_init(&game);story_complete(&game);deck_reset();change_page(GALNET);galnet_tab=3;row=0;
 game.opening.stage=OP_EARTH_NET;social_emit(&game,SB_LAUNCH);social_emit(&game,SB_PAINT);
 int stages[]={OP_EARTH_NET,OP_DAD_INBOX,OP_FREE};int clean=1;
 for(int k=0;k<3;k++){game.opening.stage=stages[k];galnet_tab=3;
  clean&=opening_net_count()==social_rows()&&social_offset()==0;
  for(int i=0;i<social_rows();i++){char who[40],body[192];galnet_post(i,who,sizeof(who),body,sizeof(body));clean&=!strstr(who,"Scott")&&!strstr(who,"Kat")&&!strstr(who,"first ship")&&!strstr(who,"Kei");}
 }
 BCHECK(clean,"Earth, displaced and completed openings all use only the real NPC event feed");
 BFRAME("spacebook-home-feed.bmp");unsigned code=social_code(0);input(PSP_CTRL_CROSS,0,.016f,0,0);
 BCHECK(social_reaction(0)==1&&(social_code(0)&0x3fffffff)==(code&0x3fffffff),"opening-complete X likes the same durable activity post");
 input(PSP_CTRL_TRIANGLE,0,.016f,0,0);BCHECK(galnet_tab==4&&page==GALNET,"Triangle opens story Inbox without leaving GalacticNet");
 int found[5]={0},scott=-1;
 for(int i=0;i<story_inbox_count();i++){int index,kind=story_inbox_record(i,&index);if(kind==INBOX_HOME){found[index]=1;if(index==3)scott=i;}}
 BCHECK(found[0]&&found[1]&&found[2]&&found[3]&&found[4],"Dad, Mum, Kat, Scott and the first-ship note are archived as private correspondence");
 row=scott;BFRAME("spacebook-scott-letter.bmp");input(PSP_CTRL_SQUARE,0,.016f,0,0);BCHECK(opening_net_detail,"Scott's migrated photo opens from his Inbox message");BFRAME("spacebook-scott-photo.bmp");input(PSP_CTRL_CIRCLE,0,.016f,0,0);BCHECK(!opening_net_detail&&page==GALNET,"closing the attached photo returns to the same Inbox thread");
 game.opening.stage=OP_DAD_INBOX;galnet_tab=4;row=0;BFRAME("spacebook-dad-inbox.bmp");opening_net_input(PSP_CTRL_CROSS);
 BCHECK(game.opening.flags&(1<<16),"Dad's undelivered reply still uses the existing persistent flag");
 game.opening.stage=OP_FREE;game.campaign_stage=6;game.saga_chapter=4;galnet_tab=4;row=0;int archive=1,sagas=0;
 for(int i=0;i<story_inbox_count();i++){int index,kind=story_inbox_record(i,&index);if(kind==INBOX_SAGA){sagas++;archive&=index<=4;}}
 BCHECK(archive&&sagas==5,"Inbox includes completed chapters and the current briefing without future spoilers");
 inbox_message_page=99;BFRAME("spacebook-story-inbox.bmp");BCHECK(inbox_message_page==0,"changed message clamps a stale page instead of showing blank text");int count=story_inbox_count();char a[40],s[128],b[768];story_inbox_text(0,a,sizeof(a),s,sizeof(s),b,sizeof(b));
 input(PSP_CTRL_CROSS,0,.016f,0,0);BCHECK(page==MISSIONLOG&&tracked_mission==0,"story letter X leads to Mission Log without accepting or advancing the chapter");
 change_page(GALNET);galnet_tab=4;row=0;game.docked=1;int saved=save_game(&game,"spacebook-home-test.sav");game.saga_chapter=0;game.opening.stage=0;
 saved=saved&&load_game(&game,"spacebook-home-test.sav")&&story_inbox_count()==count;char aa[40],ss[128],bb[768];story_inbox_text(0,aa,sizeof(aa),ss,sizeof(ss),bb,sizeof(bb));
 BCHECK(saved&&!strcmp(b,bb)&&social_reaction(0)==1&&(game.opening.flags&(1<<16)),"save/load restores archived story, offline reply and existing liked NPC posts");remove("spacebook-home-test.sav");remove("spacebook-home-test.sav.bak");
 game.saga_chapter=SAGA_COUNT;galnet_tab=4;row=story_inbox_count()-1;BFRAME("spacebook-archive-last.bmp");
 int all=1;for(int i=0;i<story_inbox_count();i++){story_inbox_text(i,a,sizeof(a),s,sizeof(s),b,sizeof(b));all&=a[0]&&s[0]&&b[0];}
 BCHECK(all,"completed campaign archive has bounded readable content for every unlocked record");
 galnet_tab=3;row=0;snprintf(game.commander_name,sizeof(game.commander_name),"ABCDEFGHIJKLMNOPQRSTUVWX");BFRAME("spacebook-long-name.bmp");
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);fb=saved_fb;free(pixels);
#undef BCHECK
#undef BFRAME
 return 1;
}
