/* Production input/render checks, enabled only by an isolated review flag. */
static unsigned ml_review_hash(int x,int y,int w,int h){unsigned hash=5381;for(int yy=y;yy<y+h;yy++)for(int xx=x;xx<x+w;xx++)hash=hash*33u+fb[yy*STRIDE+xx];return hash;}
static void ml_review_frame(const char *file){rect(0,0,W,H,BG);mission_log();if(file)dump_native_bmp(file);}
static int ml_review_tracked_count(void){int count=0;for(int i=0;i<mission_log_count();i++)count+=!strcmp(mission_dossier_heading(i),"TRACKED MISSION");return count;}
static int mission_log_review(void){
 FILE *flag=fopen("mission-log-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *out=fopen("mission-log-review.txt","w");if(!out)return 1;
 int failures=0;unsigned *old=fb,*pixels=malloc(STRIDE*H*sizeof(unsigned));if(!pixels){fprintf(out,"RESULT 1 failures\n");fclose(out);return 1;}fb=pixels;
#define MCHECK(c,label) do{int ok=!!(c);fprintf(out,"%s %s\n",ok?"PASS":"FAIL",label);failures+=!ok;fflush(out);}while(0)
 game_init(&game);story_complete(&game);game.opening.stage=0;game.tutorial_step=0;game.guild_chapter=0;game.system=7;system_bodies(&game);game.docked=1;station_tour_stage=STATION_TOUR_OFF;tracked_mission=0;preview_time=10;change_page(MISSIONLOG);ml_review_frame("log-main-tracked.bmp");
 MCHECK(row==0&&ml_review_tracked_count()==1&&!strcmp(mission_dossier_heading(0),"TRACKED MISSION")&&!strcmp(mission_dossier_heading(1),"MISSION DETAILS"),"only the actual tracked main mission has the tracked heading");
 unsigned tracked_heading=ml_review_hash(238,39,120,8);
 input(PSP_CTRL_DOWN,0,.016f,0,0);ml_review_frame("log-guild-hover.bmp");
 MCHECK(row==1&&tracked_mission==0&&ml_review_tracked_count()==1,"hovering the second main mission does not change tracking");
 MCHECK(tracked_heading!=ml_review_hash(238,39,120,8),"actual right-hand heading changes when hovering an untracked mission");
 MCHECK(!strcmp(mission_dossier_name(row),"A SHIP WITH YOUR NAME ON IT"),"ticker fixture uses the real full Guild mission title");
 unsigned first=ml_review_hash(47,82,150,8),dossier=ml_review_hash(226,31,246,211),edge=ml_review_hash(197,79,22,26);
 preview_time=10.75f;ml_review_frame(0);MCHECK(first==ml_review_hash(47,82,150,8),"highlighted long title pauses for one second before moving");
 preview_time=12;ml_review_frame("log-guild-ticker-middle.bmp");MCHECK(first!=ml_review_hash(47,82,150,8),"highlighted title visibly scrolls in the actual native list");
 MCHECK(dossier==ml_review_hash(226,31,246,211)&&edge==ml_review_hash(197,79,22,26),"ticker leaves the dossier, marker gutter and panel edge pixel-stable");
 int excess=(int)strlen(mission_dossier_name(row))*8-150;preview_time=11+excess/24.f+.5f;ml_review_frame("log-guild-ticker-end.bmp");
 unsigned actual_end=ml_review_hash(157,82,40,8);rect(0,0,W,H,RGB(27,53,64));text_px(157,82,WHITE,"ON IT");MCHECK(actual_end==ml_review_hash(157,82,40,8),"final ON IT words are fully visible at the end of the ticker");
 input(PSP_CTRL_CROSS,0,.016f,0,0);ml_review_frame("log-guild-tracked.bmp");
 MCHECK(tracked_mission==1&&ml_review_tracked_count()==1&&!strcmp(mission_dossier_heading(0),"MISSION DETAILS"),"X transfers the sole tracked status to the Guild mission");
 MCHECK(tracked_heading==ml_review_hash(238,39,120,8),"newly tracked mission renders the same actual tracked heading");
 input(PSP_CTRL_UP,0,.016f,0,0);ml_review_frame(0);input(PSP_CTRL_DOWN,0,.016f,0,0);ml_review_frame(0);
 MCHECK(mission_title_offset(1,mission_dossier_name(1),150,1)==0,"returning focus restarts a long title at its beginning");
 preview_time+=2;ml_review_frame(0);change_page(HOME);change_page(MISSIONLOG);ml_review_frame(0);
 MCHECK(row==1&&mission_title_offset(1,mission_dossier_name(1),150,1)==0,"reopening the log preserves tracking and resets the reading pause");
 /* Check clipped rendering independently from the list, with untouched guard
  * pixels. Include both partial glyph boundaries and the final endpoint. */
 int bounded=1,stationary=1;const int offsets[]={0,1,7,24,58};
 for(int i=0;i<5;i++){for(int p=0;p<STRIDE*H;p++)pixels[p]=0x12345678;mission_title_pixels(47,82,150,"A SHIP WITH YOUR NAME ON IT",WHITE,offsets[i]);
  for(int y=0;y<H;y++)for(int x=0;x<STRIDE;x++)if((x<47||x>=197||y<82||y>=90)&&pixels[y*STRIDE+x]!=0x12345678)bounded=0;
 }
 MCHECK(bounded,"ticker clips partial glyphs to its exact 150 by 8 pixel region without touching framebuffer gutters");
 for(int i=0;i<50;i++){preview_time+=.2f;stationary&=mission_title_offset(0,"SHORT TITLE",150,1)==0&&mission_title_offset(1,"A SHIP WITH YOUR NAME ON IT",150,0)==0;}
 MCHECK(stationary,"short titles and unselected titles never animate");
 game.job_n=MISSION_SLOTS;for(int i=0;i<MISSION_SLOTS;i++){memset(&game.jobs[i],0,sizeof(Job));game.jobs[i].type=MISSION_DELIVERY;game.jobs[i].dest=7;game.jobs[i].item=0;game.jobs[i].time=300;}
 station_tour_stage=STATION_TOUR_ROUTE;game.station_progress[7][0]=1<<2;
 change_page(MISSIONLOG);int unique=1,inputs=1;
 for(int i=0;i<mission_log_count();i++){row=i;input(PSP_CTRL_CROSS,0,.016f,0,0);unique&=ml_review_tracked_count()==1;inputs&=tracked_mission==mission_track_at(i);}
 MCHECK(unique&&inputs,"X tracks exactly one main, contract, station-tour or Lave entry including remapped special IDs");
 row=2;input(PSP_CTRL_CROSS,0,.016f,0,0);row=3;ml_review_frame("log-contract-hover.bmp");
 MCHECK(tracked_mission==2&&!strcmp(mission_dossier_heading(row),"MISSION DETAILS"),"hovering a side contract retains the previously tracked contract");
 input(PSP_CTRL_CROSS,0,.016f,0,0);ml_review_frame("log-contract-tracked.bmp");MCHECK(tracked_mission==3&&game.job_sel==1&&ml_review_tracked_count()==1,"tracking a second contract updates the authoritative job selection");
 input(PSP_CTRL_SQUARE,0,.016f,0,0);MCHECK(page==CAMPAIGN&&tracked_mission==3,"Square still opens the selected tracked mission next-step screen");
 change_page(MISSIONLOG);row=MISSION_SLOTS+1;ml_review_frame("log-full-list.bmp");MCHECK(row==MISSION_SLOTS+1,"full side list retains its selected final contract");
 /* Every real main-story and Guild title must reach its true endpoint. */
 int titles=1;for(int i=0;i<SAGA_COUNT+5;i++){const char *name;
  if(i<SAGA_COUNT)name=saga_beats[i].title;else{game.guild_chapter=i-SAGA_COUNT;name=guild_title(&game);}
  mission_title_focus=-1;preview_time=100;int start=mission_title_offset(i,name,150,1),end=(int)strlen(name)*8-150;if(end<0)end=0;
  preview_time=101+end/24.f+.25f;titles&=start==0&&mission_title_offset(i,name,150,1)==end;
 }
 MCHECK(titles,"all authored story and Guild titles pause and expose their full ending");
 change_page(MISSIONLOG);row=1;game.guild_chapter=0;preview_time=200;ml_review_frame(0);preview_time=202;ml_review_frame(0);game.guild_chapter=1;ml_review_frame(0);
 MCHECK(mission_title_start==202&&mission_title_offset(1,mission_dossier_name(1),150,1)==0,"a changed mission title at the same row resets the ticker");
 fprintf(out,"RESULT %d failures\n",failures);fclose(out);fb=old;free(pixels);
#undef MCHECK
 return 1;
}
