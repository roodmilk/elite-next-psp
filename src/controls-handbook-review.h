/* Opt-in review: uses the actual PSP renderer and production input path. */
static int controls_handbook_review(void){
 FILE *flag=fopen("controls-handbook-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *out=fopen("controls-handbook-review.txt","w");if(!out)return 1;
 int failures=0;unsigned *oldfb=fb,*pixels=malloc(STRIDE*H*sizeof(unsigned));
 if(!pixels){fprintf(out,"RESULT 1 failures\n");fclose(out);return 1;}fb=pixels;
#define HBCHECK(c,label) do{int ok=!!(c);fprintf(out,"%s %s\n",ok?"PASS":"FAIL",label);failures+=!ok;fflush(out);}while(0)
 game_init(&game);story_complete(&game);game.opening.stage=0;deck_reset();change_page(HOME);row=6;input(PSP_CTRL_CROSS,0,.016f,0,0);
 HBCHECK(page==HELP,"Command Deck Controls opens the illustrated handbook");
 int copy_ok=1,labels_ok=1;unsigned hashes[5]={0};
 for(int tab=0;tab<5;tab++)for(int lesson=0;lesson<8;lesson++){
  help_tab=tab;handbook_row[tab]=lesson;preview_time=4.25f;help();
  const HandbookLesson *l=&handbook_lessons[tab][lesson];
  copy_ok&=handbook_copy(0,0,30,4,HB_INK,l->instruction,0)==(int)strlen(l->instruction);
  labels_ok&=55+(int)strlen(l->label)*8<=187;
  char path[64];snprintf(path,sizeof(path),"controls-topic-%d-lesson-%d.bmp",tab,lesson);dump_native_bmp(path);
  if(!lesson)for(int y=91;y<185;y++)for(int x=208;x<464;x++)hashes[tab]=hashes[tab]*33u+pixels[y*STRIDE+x];
 }
 HBCHECK(copy_ok,"all forty real control instructions fit completely in four native lines");
 HBCHECK(labels_ok,"all list labels fit within the left page without cropping");
 HBCHECK(!strcmp(handbook_lessons[0][2].instruction,"Double press R + keep it held."),"Boost uses the requested single instruction verbatim");
 HBCHECK(hashes[0]!=hashes[1]&&hashes[1]!=hashes[2]&&hashes[2]!=hashes[3]&&hashes[3]!=hashes[4],"topic diagrams are visibly distinct");
 int topics_ok=1;help_tab=0;for(int t=0;t<5;t++){input(PSP_CTRL_RTRIGGER,0,.016f,0,0);topics_ok&=help_tab==(t+1)%5;}input(PSP_CTRL_LTRIGGER,0,.016f,0,0);
 HBCHECK(topics_ok&&help_tab==4,"both shoulder buttons switch topics and wrap predictably");
 input(PSP_CTRL_RIGHT,0,.016f,0,0);HBCHECK(help_tab==0,"existing D-pad topic shortcut still wraps from On Foot to Flight");
 handbook_row[0]=0;input(PSP_CTRL_UP,0,.016f,0,0);HBCHECK(handbook_row[0]==7,"Up wraps to the final control");input(PSP_CTRL_DOWN,0,.016f,0,0);HBCHECK(handbook_row[0]==0,"Down wraps to the first control");
 input(PSP_CTRL_DOWN,0,.016f,0,0);input(PSP_CTRL_RTRIGGER,0,.016f,0,0);input(PSP_CTRL_LTRIGGER,0,.016f,0,0);HBCHECK(handbook_row[0]==1,"each topic remembers its highlighted control");
 int analog=analog_enabled;analog_ready=1;input(PSP_CTRL_SQUARE,0,.016f,0,0);HBCHECK(analog_enabled!=analog&&!analog_ready,"Square toggles analog steering and requires nub centring");input(PSP_CTRL_SQUARE,0,.016f,0,0);HBCHECK(analog_enabled==analog,"second Square restores the original steering setting");
 float time=game.time,speed=game.speed,heat=game.heat;int credits=game.credits;for(int i=0;i<120;i++)input(0,0,.016f,0,0);
 HBCHECK(page==HELP&&game.time==time&&game.speed==speed&&game.heat==heat&&game.credits==credits,"reading the handbook pauses simulation without altering ship or credits");
 help_tab=0;handbook_row[0]=2;handbook_started=0;preview_time=1;help();dump_native_bmp("controls-boost-a.bmp");unsigned art_a=0,copy_a=0;for(int y=91;y<185;y++)for(int x=208;x<464;x++)art_a=art_a*33u+pixels[y*STRIDE+x];for(int y=195;y<235;y++)for(int x=212;x<452;x++)copy_a=copy_a*33u+pixels[y*STRIDE+x];
 preview_time=1.6f;help();dump_native_bmp("controls-boost-b.bmp");unsigned art_b=0,copy_b=0;for(int y=91;y<185;y++)for(int x=208;x<464;x++)art_b=art_b*33u+pixels[y*STRIDE+x];for(int y=195;y<235;y++)for(int x=212;x<452;x++)copy_b=copy_b*33u+pixels[y*STRIDE+x];
 HBCHECK(art_a!=art_b&&copy_a==copy_b,"instructional art animates while text remains stable");
 input(PSP_CTRL_CIRCLE,0,.016f,0,0);HBCHECK(page==HOME&&row==6,"Circle returns to the original Command Deck selection");
 game.docked=0;change_page(FLIGHT);change_page(COMMS_PANEL);row=11;input(PSP_CTRL_CROSS,0,.016f,0,0);HBCHECK(page==HELP,"Comms Panel Controls opens the same handbook");input(PSP_CTRL_CIRCLE,0,.016f,0,0);HBCHECK(page==COMMS_PANEL,"Circle returns to the Comms Panel without resuming flight");
 change_page(FLIGHT);change_page(HELP);input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);HBCHECK(page==FLIGHT&&tools_wait_release&&fire_blocked,"returning to flight blocks held buttons from firing tools");
 fprintf(out,"RESULT %d failures\n",failures);fclose(out);fb=oldfb;free(pixels);
#undef HBCHECK
 return 1;
}
