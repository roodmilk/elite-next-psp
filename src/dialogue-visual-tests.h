/* Included inside the native-framebuffer input fixture. */
{
 int layout_ok=1,replies_ok=1,copy_ok=1;
 for(int mode=0;mode<2;mode++)for(int scene=0;scene<12;scene++){
  TEST_INIT();high_contrast=mode;page=CAMPAIGN;row=0;game.message_time=0;
  game.system=7;game.docked=1;game.campaign_stage=0;comms_encounter_conversation=0;
  int replies=1;
  if(scene>=1&&scene<=5){game.campaign_stage=6;game.saga_chapter=0;game.saga_step=0;saga_brief_reset(0);}
  if(scene==2)saga_brief_echo=1;
  if(scene==3){game.saga_step=1;game.saga_flags|=SAGA_TIMESTAMP_FOUND;for(int ch=0;ch<SAGA_COUNT;ch++)if(saga_beats[ch].kind==SAGA_CHOICE){game.saga_chapter=ch;break;}row=2;replies=3;}
  if(scene==4){saga_coda_pending=0;}
  if(scene==5){game.saga_chapter=SAGA_COUNT;replies=0;}
  if(scene==6||scene==7){game.guild_chapter=1;page=scene==6?GUILD:CAMPAIGN;tracked_mission=1;replies=1;}
  if(scene==8){tracked_mission=2;game.job_n=1;memset(&game.jobs[0],0,sizeof(Job));game.jobs[0].type=MISSION_DELIVERY;game.jobs[0].dest=7;game.jobs[0].time=300;game.jobs[0].reward=900;replies=2;}
  if(scene==9||scene==10){page=COMMS_PANEL;game.encounter_kind=ENCOUNTER_DISTRESS;game.encounter=4;speak(&game,VOICE_CONTACT,"Thank you, Commander. Please help us get home.");game.voice_role=TRADERS;comms_encounter_conversation=scene==10;replies=scene==10?3:2;}
  if(scene==11){game.campaign_stage=6;game.saga_chapter=0;game.saga_step=1;}
  memset(pixels,0,STRIDE*H*sizeof(unsigned));
  if(page==GUILD)guild_screen();else if(page==COMMS_PANEL)comms_panel();else campaign_screen();
  if(scene==0)INPUT_CHECK(pixels[257*STRIDE+90]!=RGB(21,28,39),"dialogue: Cross footer icon remains visible after a clipped flight preview");
  unsigned edge=scene==2?RGB(245,157,62):RGB(76,181,190);
  if(pixels[56*STRIDE+100]!=edge||pixels[134*STRIDE+100]!=edge||pixels[144*STRIDE+17]!=RGB(13,36,43))layout_ok=0;
  for(int i=0;i<replies;i++)if(pixels[(182+i*24)*STRIDE+20]!=(row==i?RGB(245,157,62):RGB(119,71,38)))replies_ok=0;
  message(&game,"Feedback must stay above all reply choices.");dialogue_notice();
  for(int i=0;i<replies;i++)if(pixels[(182+i*24)*STRIDE+20]!=(row==i?RGB(245,157,62):RGB(119,71,38)))replies_ok=0;
  game.message_time=0;
  if(page==GUILD)guild_screen();else if(page==COMMS_PANEL)comms_panel();else campaign_screen();
  char capture[64];snprintf(capture,sizeof(capture),"dialogue-%02d-%s.bmp",scene,mode?"contrast":"normal");dump_native_bmp(capture);
 }
 INPUT_CHECK(layout_ok,"dialogue: every mission family and Triangle channel share speech and objective geometry in both palettes");
 INPUT_CHECK(replies_ok,"dialogue: every reply uses the same highlight and notifications cannot cover choices");
 for(int ch=0;ch<SAGA_COUNT;ch++){
  for(int beat=0;beat<SAGA_BRIEF_BEATS;beat++)if(*dialogue_skip(saga_brief_reply(&saga_beats[ch],beat),2,51))copy_ok=0;
  if(saga_beats[ch].kind==SAGA_CHOICE)for(int i=0;i<3;i++)if(*dialogue_skip(saga_choice_label(ch,i),2,51))copy_ok=0;
 }
 INPUT_CHECK(copy_ok,"dialogue: every authored story reply fits without clipping");
 char ascii[80];dialogue_ascii(ascii,sizeof(ascii),"Wait \xe2\x80\x94 \xe2\x80\x9c" "come home" "\xe2\x80\x9d.");
 INPUT_CHECK(!strcmp(ascii,"Wait - \"come home\"."),"dialogue: authored punctuation renders with supported PSP glyphs");
 TEST_INIT();page=COMMS_PANEL;comms_encounter_conversation=1;game.encounter_kind=ENCOUNTER_NONE;game.encounter=0;game.voice_role=TRADERS;
 memset(pixels,0,STRIDE*H*sizeof(unsigned));comms_panel();
 INPUT_CHECK(pixels[56*STRIDE+100]==RGB(76,181,190),"dialogue: open Triangle conversation does not turn into settings when encounter ends");
 comms_return=HOME;radio_dirty=0;input(PSP_CTRL_CIRCLE,0,.016f,0,0);
 INPUT_CHECK(page==HOME&&!comms_encounter_conversation,"dialogue: Back closes conversation mode before reopening comms settings");
 page=CAMPAIGN;row=0;char long_line[800];memset(long_line,'a',sizeof(long_line)-1);long_line[sizeof(long_line)-1]=0;
 dialogue_named("KEI",EXPLORERS,long_line,0);
 INPUT_CHECK(dialogue_pages==3&&dialogue_page==0,"dialogue: long speech is paginated rather than truncated");
 input(PSP_CTRL_RTRIGGER,0,.016f,0,0);
 INPUT_CHECK(dialogue_page==1&&row==0,"dialogue: R pages speech without changing the selected response");
 input(PSP_CTRL_LTRIGGER,0,.016f,0,0);
 INPUT_CHECK(dialogue_page==0,"dialogue: L returns to previous speech page");
 dialogue_named("KEI",EXPLORERS,"New speech.",0);
 INPUT_CHECK(dialogue_page==0&&dialogue_pages==1,"dialogue: a new speech resets pagination");
 TEST_INIT();page=CAMPAIGN;tracked_mission=1;game.campaign_stage=6;game.saga_step=1;
 for(int ch=0;ch<SAGA_COUNT;ch++)if(saga_beats[ch].kind==SAGA_CHOICE){game.saga_chapter=ch;break;}
 row=0;input(PSP_CTRL_DOWN,0,.016f,0,0);
 INPUT_CHECK(row==0,"dialogue: an untracked story decision cannot add invisible Guild reply choices");
 tracked_mission=2;game.job_n=1;row=1;input(PSP_CTRL_DOWN,0,.016f,0,0);
 INPUT_CHECK(row==0,"dialogue: a tracked contract has only its two visible replies even during a story decision");
 high_contrast=0;comms_encounter_conversation=0;TEST_INIT();
}
