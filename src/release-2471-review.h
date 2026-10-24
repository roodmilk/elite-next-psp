#if defined(ELITE_ENABLE_REVIEW) || defined(ELITE_ENCOUNTER_REVIEW)
static int release_2471_review(void){
 FILE *flag=fopen("release-2471-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *out=fopen("release-2471-review.txt","w");if(!out)return 1;
 int failures=0,checks=0,old_smoke=smoke;smoke=1;
 #define R2471(c,label) do{int ok=!!(c);checks++;failures+=!ok;fprintf(out,"%s %s\n",ok?"PASS":"FAIL",label);}while(0)
 game_init(&game);story_complete(&game);change_page(INTRO);intro_time=40;intro_menu_age=1;intro_wait_release=0;intro_choice=3;intro_extras=0;
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 R2471(page==INTRO&&intro_extras&&intro_extras_choice==0,"Extras opens on First Light Tutorial");
 input(PSP_CTRL_DOWN,PSP_CTRL_DOWN,.016f,0,0);
 R2471(intro_extras&&intro_extras_choice==1,"Extras selects ERL Race 1 Test");
 input(PSP_CTRL_CIRCLE,0,.016f,0,0);
 R2471(page==INTRO&&!intro_extras&&intro_choice==3,"Circle returns from Extras to the title menu");
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);input(PSP_CTRL_DOWN,PSP_CTRL_DOWN,.016f,0,0);input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 R2471(page==INTRO&&erl_racer_picker&&!erl_quick_race,"ERL Race 1 Test opens racer selection without altering the career");
 erl_racer_picker=0;intro_extras=0;
 opening_new_game();snprintf(game.commander_name,sizeof(game.commander_name),"SKIP TEST");uint32_t portrait=0x5a31c27du;game.commander_portrait=portrait;opening_create_row=2;
 opening_input(0,0,.016f,0,0);opening_input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 R2471(opening_skip_prompt&&opening_skip_choice==0&&game.opening.stage==OP_CREATE,"Start New Game opens the tutorial-skip question with Play Story selected");
 opening_input(0,0,.016f,0,0);opening_input(PSP_CTRL_DOWN,PSP_CTRL_DOWN,.016f,0,0);opening_input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 R2471(!opening_skip_prompt&&game.opening.stage==OP_FREE&&game.system==7&&game.docked&&page==HOME,"Skip Tutorial starts docked at Lave in free flight");
 R2471(!strcmp(game.commander_name,"SKIP TEST")&&game.commander_portrait==portrait,"Skip Tutorial preserves commander name and portrait");
 fprintf(out,"RESULT %d failures / %d checks\n",failures,checks);fclose(out);smoke=old_smoke;
 #undef R2471
 return 1;
}
#endif
