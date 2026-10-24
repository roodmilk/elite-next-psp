static int final_menu_review(void){FILE *flag=fopen("final-menu.flag","r");if(!flag)return 0;fclose(flag);FILE *out=fopen("final-menu.txt","w");if(!out)return 1;setvbuf(out,0,_IONBF,0);int fails=0;
 #define FM(c,s) do{int ok=!!(c);fails+=!ok;fprintf(out,"%s %s\n",ok?"PASS":"FAIL",s);}while(0)
 page=INTRO;intro_time=40;intro_menu_age=1;intro_wait_release=0;intro_extras=0;intro_profile_ready=1;for(int i=0;i<3;i++)memset(&profile_info[i],0,sizeof(profile_info[i]));intro_choice=0;
 FM(intro_first_choice()==1,"no-save menu hides duplicate Begin New Commander");input(PSP_CTRL_UP,PSP_CTRL_UP,.016f,0,0);FM(intro_choice==3,"no-save Up wraps from New Commander to Extras");input(PSP_CTRL_DOWN,PSP_CTRL_DOWN,.016f,0,0);FM(intro_choice==1,"no-save Down wraps to New Commander");
 profile_info[0].state=1;snprintf(profile_info[0].name,25,"BEN");profile_info[0].credits=12500;profile_info[0].system=7;profile_info[0].ship=0;FM(intro_first_choice()==0,"Continue remains available when a saved commander exists");intro_choice=2;intro_load_slot=0;int active=commander_active_slot;input(PSP_CTRL_LTRIGGER,PSP_CTRL_LTRIGGER,.016f,0,0);FM(intro_load_slot==2,"L reaches an empty slot without loading");input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);FM(intro_load_slot==0&&commander_active_slot==active,"R wraps and browsing preserves active commander");
 unsigned *old=fb;fb=calloc(STRIDE*H,sizeof(unsigned));intro_movie_attempted=0;int film=fb&&intro_movie_open();FM(film,"new embedded cinematic opens in PSP decoder");
 if(film)for(int state=0;state<3;state++){intro_choice=state?2:1;intro_load_slot=state==2?2:0;for(int f=0;f<32;f++){intro_time=46+f/8.f;intro_menu_age=fminf(1,f/8.f);intro_screen();char p[64];snprintf(p,sizeof(p),"final-menu-%d-%02d.bmp",state,f);dump_native_bmp(p);}}
 intro_movie_close();if(fb)free(fb);fb=old;fprintf(out,"RESULT %d failures\n",fails);fclose(out);return 1;
 #undef FM
}
