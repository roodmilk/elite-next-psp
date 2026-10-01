{
 TEST_INIT();change_page(GALNET);galnet_tab=3;row=0;
 snprintf(game.commander_name,sizeof(game.commander_name),"BEN");social_emit(&game,SB_PAINT);unsigned first=game.social.post[0];
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(social_reaction(0)==1,"social: X likes the real event card");
 input(PSP_CTRL_SQUARE,0,.016f,0,0);
 INPUT_CHECK(social_reaction(0)==2&&page==GALNET,"social: Square replaces Like with Dislike without changing page");
 input(PSP_CTRL_SQUARE,0,.016f,0,0);
 INPUT_CHECK(social_reaction(0)==0,"social: pressing same reaction again clears it");
 input(PSP_CTRL_CROSS,0,.016f,0,0);social_emit(&game,SB_LAUNCH);
 INPUT_CHECK(social_reaction(1)==1&&(game.social.post[1]&0x3fffffffu)==first,"social: Likes remain attached as new posts arrive");
 galnet_screen();dump_native_bmp("spacebook-live.bmp");
 unsigned avatar_seen[10]={0};int avatar_unique=0;unsigned same_a=spacebook_avatar_seed("NebulaMoth_42"),same_b=spacebook_avatar_seed("nebulamoth_42");
 INPUT_CHECK(same_a==same_b,"social avatars: username casing preserves the same identity");
 for(int k=0;k<160;k++){char avatar_name[40];social_author((unsigned)k*7919u,avatar_name,sizeof(avatar_name));unsigned seed=spacebook_avatar_seed(avatar_name);int family=(seed>>3)%10;if(!avatar_seen[family]){avatar_seen[family]=1;avatar_unique++;}}
 INPUT_CHECK(avatar_unique==10,"social avatars: generated usernames cover all ten portrait families");
 rect(0,0,W,H,RGB(238,242,247));for(int k=0;k<80;k++){char avatar_name[40];social_author((unsigned)k*7919u,avatar_name,sizeof(avatar_name));spacebook_avatar(8+(k%20)*23,8+(k/20)*23,20,avatar_name);}dump_native_bmp("spacebook-avatar-gallery.bmp");
 char date[96];social_date(0,date,sizeof(date));
 ScePspDateTime rtc;sceRtcGetCurrentClock(&rtc,0);char year[8];snprintf(year,sizeof(year),"%04d-",rtc.year);
 INPUT_CHECK(strstr(date,"UTC")!=0&&!strncmp(date,year,5),"social: saved date agrees with actual PSP clock year");
 INPUT_CHECK((SB_COUNT-1)*SOCIAL_POST_VARIANTS==2560,"social: combinatorial library exposes 2,560 event post bodies");
 for(int event=1;event<SB_COUNT;event++)for(int variant=0;variant<SOCIAL_POST_VARIANTS;variant++){
  char author[40],body[192];
  social_post_text((unsigned)event|(9u<<5)|((unsigned)variant<<9),0,author,sizeof(author),body,sizeof(body));
  INPUT_CHECK(strlen(author)>5&&strlen(body)<170,"social: every event has a bounded readable post");
  INPUT_CHECK(event==SB_AMBIENT||(strstr(body,"Ben")&&!strstr(body,"OPHIDIAN")),"social: posts identify BEN, not the ship");
  snprintf(game.commander_name,sizeof(game.commander_name),"ABCDEFGHIJKLMNOPQRSTUVWX");
  social_post_text((unsigned)event|(9u<<5)|((unsigned)variant<<9),0,author,sizeof(author),body,sizeof(body));
  INPUT_CHECK(strlen(body)<166,"social: maximum commander name still fits the card");
  snprintf(game.commander_name,sizeof(game.commander_name),"BEN");
 }
 game.system=129;
 INPUT_CHECK(social_count()==2&&game.social.origin[0]==7,"social: the same history is visible from another system");
 game.system=7;
 for(int k=0;k<20;k++){game.system=k;social_emit(&game,SB_LAND);}game.system=7;
 row=0;input(PSP_CTRL_RIGHT,0,.016f,0,0);
 INPUT_CHECK(row==10,"social: right skips ten posts without changing GalacticNet tab");
 input(PSP_CTRL_LEFT,0,.016f,0,0);INPUT_CHECK(row==0,"social: left returns ten posts");
 row=galnet_rows()-1;input(PSP_CTRL_RIGHT,0,.016f,0,0);
 INPUT_CHECK(row==galnet_rows()-1,"social: fast scrolling clamps at oldest post");
 galnet_screen();dump_native_bmp("spacebook-last-page.bmp");
 char formatted[25];social_name(formatted,sizeof(formatted),"BEN SMITH");
 INPUT_CHECK(!strcmp(formatted,"Ben Smith"),"social: multiword names are title-cased without changing saves");
 INPUT_CHECK(!strcmp(game.commander_name,"BEN"),"social: name formatting never changes the profile");
 TEST_INIT();page=HOME;
}
