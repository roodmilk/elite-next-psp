static int menu_seven_review(void){
 FILE *flag=fopen("menu-seven.flag","r");if(!flag)return 0;fclose(flag);FILE *out=fopen("menu-seven.txt","w");if(!out)return 1;setvbuf(out,0,_IONBF,0);
 int failures=0;
 #define M7CHK(c,label) do{int ok=!!(c);failures+=!ok;fprintf(out,"%s %s\n",ok?"PASS":"FAIL",label);}while(0)
 page=INTRO;intro_time=40;intro_menu_age=1;intro_wait_release=0;intro_extras=0;intro_choice=2;intro_load_slot=0;intro_profile_ready=1;
 for(int i=0;i<3;i++){profile_info[i].state=i==2?0:1;profile_info[i].credits=12000+i*1700;profile_info[i].system=7;profile_info[i].ship=i;profile_info[i].portrait=portrait_seeded(7+i,EXPLORERS,0);snprintf(profile_info[i].name,25,"%s",i?"NOVA REED":"BEN");}
 int original_slot=commander_active_slot;
 input(PSP_CTRL_LTRIGGER,PSP_CTRL_LTRIGGER,.016f,0,0);M7CHK(intro_load_slot==2&&page==INTRO,"L wraps to slot three without loading");
 input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);M7CHK(intro_load_slot==0,"R wraps to slot one");
 input(PSP_CTRL_RIGHT,PSP_CTRL_RIGHT,.016f,0,0);M7CHK(intro_load_slot==1,"Right selects slot two");
 M7CHK(commander_active_slot==original_slot,"browsing preserves the active commander slot");
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);M7CHK(page==STATUS&&profile_slot==1&&profile_from_intro,"X opens the selected save card for existing confirmation flow");
 page=INTRO;intro_choice=0;intro_extras=0;intro_profile_ready=1;intro_movie_attempted=0;
 unsigned *old=fb;fb=calloc(STRIDE*H,sizeof(unsigned));M7CHK(fb!=NULL,"native capture framebuffer allocated");
 if(fb){M7CHK(intro_movie_open(),"embedded cinematic opens behind real menu");for(int frame=0;frame<48;frame++){intro_time=40+frame/12.f;intro_menu_age=fminf(1,frame/12.f);intro_choice=(frame/12)%4;intro_load_slot=(frame/6)%3;intro_screen();char path[64];snprintf(path,sizeof(path),"menu-seven-%02d.bmp",frame);dump_native_bmp(path);}intro_movie_close();free(fb);}fb=old;
 fprintf(out,"RESULT %d failures\n",failures);fclose(out);return 1;
 #undef M7CHK
}
