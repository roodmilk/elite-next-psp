/* Three manual checkpoint slots. Slot one retains the legacy filename. */
static void tutorial_reset_frontend(void);
static int profile_slot=0,profile_from_intro=0,profile_mode=0,profile_portrait_edit=0,profile_portrait_part=0,profile_confirm=0,profile_key=0;
static char profile_draft[25];
static struct {int state,credits,system,ship,opening_stage;unsigned portrait;char name[25];} profile_info[3];
static const char *profile_path(void){return game.tutorial_step&&!profile_from_intro?"tutorial.sav":commander_slots[profile_slot];}
static int profile_file_exists(const char *path){FILE *f=fopen(path,"rb");if(f){fclose(f);return 1;}return 0;}
static void profile_refresh(void){
 Game *preview=malloc(sizeof(Game));
 for(int i=0;i<3;i++){
  memset(&profile_info[i],0,sizeof(profile_info[i]));if(!preview){profile_info[i].state=-1;continue;}
  const char *path=game.tutorial_step&&!profile_from_intro?"tutorial.sav":commander_slots[i];char bak[64];snprintf(bak,sizeof(bak),"%s.bak",path);
  if(load_game(preview,path)){profile_info[i].state=strstr(preview->message,"backup")?3:1;profile_info[i].credits=preview->credits;profile_info[i].system=preview->system;profile_info[i].opening_stage=preview->opening.stage;profile_info[i].ship=preview->ship;profile_info[i].portrait=preview->commander_portrait;memcpy(profile_info[i].name,preview->commander_name,25);}
  else if(profile_file_exists(path)||profile_file_exists(bak))profile_info[i].state=2;
 }
 free(preview);
}
static void profile_enter(void){profile_slot=commander_active_slot;profile_from_intro=profile_mode=profile_portrait_edit=profile_confirm=0;profile_refresh();}
static void profile_open_load(void){change_page(STATUS);profile_from_intro=1;row=1;profile_refresh();}
static int profile_save(void){
 if(!opening_save_allowed(&game)){message(&game,"Dock at a station to save a checkpoint.");return 0;}
 if(game.erl.fleet_n&&!game.erl.event_kind)erl_ship_capture(&game,ship_paint[game.ship]);
 if(!save_game(&game,profile_path()))return 0;
 if(!game.tutorial_step)commander_active_slot=profile_slot;profile_refresh();return 1;
}
static void profile_load(void){
 int title_load=profile_from_intro;
 if(!load_game(&game,profile_path()))return;
 int recovered=strstr(game.message,"backup")!=NULL;commander_active_slot=profile_slot;if(game.erl.fleet_n)ship_paint[game.ship]=game.erl.fleet[game.erl.active_ship].paint;
 night_npc=-1;night_open=0;night_wait=480;night_seen=0;
 tutorial_reset_frontend();opening_restore();change_page(HOME);message(&game,recovered?"Commander recovered from backup.":"Commander loaded. Welcome back.");
 db_restore_frontend();
 if(title_load&&game.docked&&game.planet<0)load_arrival_begin();
}
static int profile_apply_name(void){
 int n=(int)strlen(profile_draft);while(n>0&&profile_draft[n-1]==' ')profile_draft[--n]=0;
 int start=0;while(profile_draft[start]==' ')start++;
 if(!profile_draft[start]){message(&game,"Please enter a commander name.");return 0;}
 snprintf(game.commander_name,sizeof(game.commander_name),"%s",profile_draft+start);profile_mode=0;message(&game,"Name updated. Save to keep your changes.");return 1;
}
static void profile_delete(void){
 const char *path=profile_path();char side[80];int ok=1;
 if(profile_file_exists(path)&&remove(path))ok=0;
 snprintf(side,sizeof(side),"%s.bak",path);if(profile_file_exists(side)&&remove(side))ok=0;
 snprintf(side,sizeof(side),"%s.tmp",path);if(profile_file_exists(side)&&remove(side))ok=0;
 profile_refresh();message(&game,ok?"Selected save and its backup deleted.":"Could not delete every file. Check the memory stick.");
}
static int profile_input(unsigned pressed){
 if(profile_confirm){
  if(pressed&PSP_CTRL_CIRCLE){profile_confirm=0;return 0;}
  if(pressed&PSP_CTRL_CROSS){int action=profile_confirm;profile_confirm=0;if(action==1)return profile_save();if(action==3)profile_delete();else profile_load();}
  return 0;
 }
 if(profile_portrait_edit){
  if(portrait_version(game.commander_portrait)<2){
   if(pressed&PSP_CTRL_CIRCLE)profile_portrait_edit=0;
   if(pressed&PSP_CTRL_SQUARE)game.commander_portrait=portrait_expand_dna(portrait_parts_dna(game.commander_portrait));
   return 0;
  }
  if(pressed&PSP_CTRL_CIRCLE){profile_portrait_edit=0;message(&game,"Portrait updated. Save to keep your changes.");return 0;}
  if(pressed&PSP_CTRL_UP)do{profile_portrait_part=(profile_portrait_part+PORTRAIT_PART_COUNT-1)%PORTRAIT_PART_COUNT;}while(portrait_choice_count(game.commander_portrait,profile_portrait_part)<2);
  if(pressed&PSP_CTRL_DOWN)do{profile_portrait_part=(profile_portrait_part+1)%PORTRAIT_PART_COUNT;}while(portrait_choice_count(game.commander_portrait,profile_portrait_part)<2);
  if(pressed&PSP_CTRL_LEFT)game.commander_portrait=portrait_adjust(game.commander_portrait,profile_portrait_part,-1);
  if(pressed&PSP_CTRL_RIGHT)game.commander_portrait=portrait_adjust(game.commander_portrait,profile_portrait_part,1);
  if(pressed&PSP_CTRL_SQUARE){game.commander_portrait=portrait_commander_random(art_hash(game.rng^(unsigned)(game.time*1000.f)^game.commander_portrait));if(portrait_choice_count(game.commander_portrait,profile_portrait_part)<2)profile_portrait_part=0;}
  return 0;
 }
 if(profile_mode){
  if(pressed&PSP_CTRL_CIRCLE){profile_mode=0;return 0;}
  if(pressed&PSP_CTRL_LEFT)profile_key=(profile_key+39)%40;
  if(pressed&PSP_CTRL_RIGHT)profile_key=(profile_key+1)%40;
  if(pressed&PSP_CTRL_UP)profile_key=(profile_key+32)%40;
  if(pressed&PSP_CTRL_DOWN)profile_key=(profile_key+8)%40;
  int n=(int)strlen(profile_draft);
  if((pressed&PSP_CTRL_SQUARE)||((pressed&PSP_CTRL_CROSS)&&profile_key==38)){if(n)profile_draft[n-1]=0;}
  else if((pressed&PSP_CTRL_START)||((pressed&PSP_CTRL_CROSS)&&profile_key==39))profile_apply_name();
  else if(pressed&PSP_CTRL_CROSS){if(n<24){profile_draft[n]="ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 -"[profile_key];profile_draft[n+1]=0;}else message(&game,"Names can contain up to 24 characters.");}
  return 0;
 }
 if(pressed&PSP_CTRL_CIRCLE){if(profile_from_intro)change_page(INTRO);else menu_back();return 0;}
 if(profile_from_intro&&(pressed&PSP_CTRL_SQUARE)&&profile_info[profile_slot].state>0){profile_confirm=3;return 0;}
 int creating=opening_exists(&game)&&game.opening.stage==OP_CREATE;
 if(!profile_from_intro&&!creating){
  profile_slot=commander_active_slot;
  if(pressed&PSP_CTRL_UP)row=(row+1)%2;
  if(pressed&PSP_CTRL_DOWN)row=(row+1)%2;
  if(pressed&PSP_CTRL_CROSS){
   if(row==0){if(!opening_save_allowed(&game))message(&game,"Dock at a station to save your commander.");else if(profile_info[profile_slot].state)profile_confirm=1;else return profile_save();}
   else {memcpy(profile_draft,game.commander_name,25);profile_mode=1;profile_key=0;game.message_time=0;}
  }
  return 0;
 }
 if(!profile_from_intro){if(pressed&PSP_CTRL_UP)row=(row+4)%5;if(pressed&PSP_CTRL_DOWN)row=(row+1)%5;}
 if(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT)){
  int dir=(pressed&PSP_CTRL_LEFT)?-1:1;
  if(row<=1&&(!game.tutorial_step||profile_from_intro)){profile_slot=(profile_slot+3+dir)%3;game.message_time=0;}
 }
 if(pressed&PSP_CTRL_CROSS){
  if(row==0){if(!opening_save_allowed(&game))message(&game,"Dock at a station to save a checkpoint.");else if(profile_info[profile_slot].state<0)message(&game,"Unable to inspect slots. Reopen SAVE/STATUS.");else if(profile_info[profile_slot].state)profile_confirm=1;else return profile_save();}
  else if(row==1){if(profile_info[profile_slot].state==1||profile_info[profile_slot].state==3)profile_confirm=2;else message(&game,"No valid save or recovery backup in this slot.");}
  else if(row==2){memcpy(profile_draft,game.commander_name,25);profile_mode=1;profile_key=0;game.message_time=0;}
  else if(row==3){game.commander_portrait=portrait_normalize(game.commander_portrait);if(portrait_version(game.commander_portrait)>=2)game.commander_portrait=portrait_expand_dna(game.commander_portrait);profile_portrait_edit=1;profile_portrait_part=0;game.message_time=0;}
  else if(row==4){if(game.docked&&game.legal>0)police_pay_desk(&game);else message(&game,game.legal?"Dock to pay your local fine.":"No local fines to pay.");}
 }
 return 0;
}
static void status(void){
 header(profile_from_intro?"LOAD COMMANDER":"COMMANDER PROFILE");panel(8,32,464,214);
 if(profile_from_intro&&!profile_confirm){
  for(int i=0;i<3;i++){
   int x=10+i*154,st=profile_info[i].state;rect(x,38,148,194,i==profile_slot?UI_EDGE:UI_PANEL);rect(x+2,40,144,190,UI_PANEL);
   text(x/8+1,6,i==profile_slot?UI_GOLD:UI_MUTED,"%s SLOT %d",i==profile_slot?">":" ",i+1);
   if(st==1||st==3){draw_commander_portrait(x+42,62,64,profile_info[i].portrait);text_wrap(x/8+1,17,16,2,UI_GOLD,profile_info[i].name,0);text(x/8+1,20,WHITE,"%.1f U",profile_info[i].credits*.1f);text(x/8+1,22,UI_CYAN,"%.16s",game.systems[profile_info[i].system].name);text(x/8+1,24,UI_MUTED,"%.16s",player_ships[profile_info[i].ship].name);if(st==3)text(x/8+1,27,UI_GOLD,"RECOVERY BACKUP");}
   else {text(x/8+1,17,UI_MUTED,st==0?"EMPTY SLOT":"UNREADABLE SAVE");text(x/8+1,20,UI_MUTED,st==0?"SAVE IN COMMANDER":"NO VALID BACKUP");}
  }
  if(game.message_time>0)text(2,30,UI_GOLD,"%.56s",game.message);
  footer("LEFT/RIGHT  X LOAD  SQ DELETE  O TITLE");return;
 }
 unsigned portrait=profile_from_intro&&profile_info[profile_slot].state%2==1?profile_info[profile_slot].portrait:game.commander_portrait;
 draw_commander_portrait(20,40,64,portrait);
 text(13,5,UI_GOLD,"CMDR %.24s",profile_from_intro?profile_info[profile_slot].name:game.commander_name);
 text(13,8,WHITE,"%.18s  %.1f U",player_ships[game.ship].name,game.credits*.1f);
 text(13,10,UI_CYAN,"%.12s  KILLS %d",game.systems[game.system].name,game.kills);
 if(profile_confirm){int ingame=!profile_from_intro&&game.opening.stage!=OP_CREATE;text(3,14,UI_CYAN,ingame?"ACTIVE COMMANDER RECORD":"SLOT %d: %.24s",profile_slot+1,profile_info[profile_slot].name);text(3,17,UI_GOLD,profile_confirm==3?"PERMANENTLY DELETE THIS SAVE?":profile_confirm==1?(ingame?"UPDATE COMMANDER RECORD?":"REPLACE THIS SLOT?"):"LOAD THIS CHECKPOINT?");text_wrap(3,20,53,3,WHITE,profile_confirm==3?"Only this slot and its recovery backup will be deleted. This cannot be undone. Other slots and tutorial progress are kept.":profile_confirm==1?(ingame?"Your current ship, position, career and identity will be written to the active memory record.":"The selected file will be replaced. Its previous valid save is retained as a recovery backup."):"Unsaved progress and profile changes will be lost. Your ship returns to its saved station.",0);footer("X CONFIRM   O CANCEL");return;}
 if(profile_portrait_edit){
  rect(12,36,456,206,UI_PANEL);draw_commander_portrait(24,82,96,game.commander_portrait);text(3,5,UI_GOLD,"CREATE YOUR COMMANDER");text(3,7,UI_CYAN,"SPECIES: %s",portrait_species_name(game.commander_portrait));
  if(portrait_version(game.commander_portrait)<2){
   text_px(152,88,UI_GOLD,"SAVED APPEARANCE");text_wrap(19,14,35,4,WHITE,"Your original portrait is preserved. Create a new look to use individual feature controls.",0);
   footer("SQ CREATE NEW LOOK   O KEEP PORTRAIT");return;
  }
  for(int i=0,visible=0;i<PORTRAIT_PART_COUNT;i++){if(portrait_choice_count(game.commander_portrait,i)<2)continue;int yy=6+visible++*2;if(i==profile_portrait_part)rect(144,yy*8-2,320,15,UI_EDGE);text(19,yy,i==profile_portrait_part?UI_GOLD:WHITE,"%s",portrait_choice_label(game.commander_portrait,i));if(i==0)text(45,yy,UI_CYAN,"%s",portrait_species_name(game.commander_portrait));else {text_px(288,yy*8,UI_CYAN,"%s",portrait_choice_caption(game.commander_portrait,i));text(50,yy,UI_MUTED,"%d / %d",portrait_part_value(game.commander_portrait,i)+1,portrait_choice_count(game.commander_portrait,i));}}
  draw_commander_portrait(24,184,32,game.commander_portrait);text_px(64,192,UI_MUTED,"32PX");text_px(144,232,UI_MUTED,"SAVE TO KEEP CHANGES.");footer("UP/DOWN PART   LEFT/RIGHT CHANGE   SQ RANDOM   O DONE");return;
 }
 if(profile_mode){text(3,14,UI_GOLD,"NAME: %.24s_",profile_draft);for(int i=0;i<40;i++){int x=3+(i%8)*7,y=17+(i/8)*3;if(i==profile_key)rect(x*8-2,y*8-2,48,18,UI_EDGE);if(i<36)text(x+2,y,i==profile_key?UI_GOLD:WHITE,"%c","ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"[i]);else text(x,y,i==profile_key?UI_GOLD:WHITE,"%s",i==36?"SPACE":i==37?"  -":i==38?" DEL":" DONE");}if(game.message_time>0)text(3,15,UI_GOLD,"%.54s",game.message);footer("X TYPE  SQ DELETE  START DONE  O CANCEL");return;}
 if(!profile_from_intro&&game.opening.stage!=OP_CREATE){
  unsigned frame=ui_paint_surface(RGB(48,77,87)),screen=ui_paint_surface(RGB(7,19,29)),card=ui_paint_surface(RGB(13,31,42)),lamp=RGB(82,214,191),chip=ui_paint_surface(RGB(18,43,53));
  /* A single bounded pilot card, authored at 480x272. Its cut corners,
   * clean photo and career chips make it feel issued rather than tabular. */
  rect(14,38,452,194,screen);rect(18,42,444,186,card);rect(24,42,432,2,UI_ACCENT);rect(24,226,432,2,frame);
  rect(18,42,6,6,screen);rect(456,42,6,6,screen);rect(18,222,6,6,screen);rect(456,222,6,6,screen);
  for(int i=0;i<18;i++){unsigned h=field_hash((unsigned)i*1709u+game.commander_portrait);int sx=150+(h%298),sy=47+((h>>9)%39);pixel(sx,sy,(i%5)?ui_paint_surface(RGB(37,67,75)):UI_CYAN);}
  text_px(150,48,UI_MUTED,"PILOT AUTHORITY // FLIGHT RECORD");
  rect(26,53,112,122,ui_paint_surface(RGB(5,15,24)));rect(26,53,112,2,UI_CYAN);rect(32,59,100,100,ui_paint_surface(RGB(27,51,62)));draw_commander_portrait(42,69,80,game.commander_portrait);
  rect(34,163,96,8,ui_paint_surface(RGB(17,50,56)));text_px(42,164,lamp,"ID VERIFIED");
  text_px(27,181,UI_MUTED,"EN-PILOT %04X",(unsigned)(game.commander_portrait^game.kills^game.system)&65535u);
  for(int i=0;i<15;i++)rect(27+i*7,194,((i*3+game.system)&3)+1,14,((i+game.system)&1)?UI_MUTED:UI_CYAN);
  text_px(150,65,UI_GOLD,"CMDR %.24s",game.commander_name);text_px(150,80,UI_CYAN,"SHIP  %.22s",player_ships[game.ship].name);
  rect(366,58,82,30,ui_paint_surface(RGB(9,25,35)));rect(374,71,20,2,UI_GOLD);rect(394,65,12,14,UI_GOLD);rect(406,71,34,2,UI_GOLD);pixel(400,61,WHITE);text_px(371,82,UI_MUTED,"INDEPENDENT");
  const char *legal=game.legal?"WARRANT":"CLEAR";int cx[3]={150,248,346},cw[3]={92,92,102};const char *cl[3]={"BALANCE","KILLS","LEGAL"};
  for(int i=0;i<3;i++){rect(cx[i],96,cw[i],31,chip);rect(cx[i],96,cw[i],1,i==2&&game.legal?RED:frame);text_px(cx[i]+7,101,UI_MUTED,"%s",cl[i]);if(i==0)text_px(cx[i]+7,114,WHITE,"%.1f U",game.credits*.1f);else if(i==1)text_px(cx[i]+7,114,WHITE,"%d",game.kills);else text_px(cx[i]+7,114,game.legal?RED:lamp,"%s",legal);}
  rect(150,133,142,31,chip);rect(298,133,150,31,chip);text_px(157,138,UI_MUTED,"DISCOVERIES");text_px(157,151,WHITE,"%d LOGGED",game.discoveries);text_px(305,138,UI_MUTED,"CURRENT BERTH");text_px(305,151,UI_CYAN,"%.20s",game.systems[game.system].name);
  for(int i=0;i<2;i++){int x=i?302:150,w=i?146:144,y=178;const char *action=i?"RENAME COMMANDER":"SAVE PROFILE";int tx=x+(w-(int)strlen(action)*8)/2;rect(x,y,w,37,row==i?UI_RAISED:ui_paint_surface(RGB(8,24,34)));rect(x,y,w,2,row==i?UI_GOLD:frame);text_px(tx,y+(37-8)/2,row==i?UI_GOLD:WHITE,"%s",action);}
  if(game.message_time>0){rect(150,218,298,8,ui_paint_surface(RGB(8,25,34)));text_px(156,218,UI_CYAN,"%.47s",game.message);}
  footer("UP/DOWN   X OPEN   O BACK");return;
 }
 text(3,14,UI_GOLD,game.tutorial_step&&!profile_from_intro?"TUTORIAL CHECKPOINT":"COMMANDER SLOT %d / 3   < LEFT / RIGHT >",profile_slot+1);
 int state=profile_info[profile_slot].state;
 if(state==1||state==3){text(3,16,WHITE,"%.24s  /  %.12s",profile_info[profile_slot].name,game.systems[profile_info[profile_slot].system].name);text(3,18,UI_MUTED,"%.18s  %.1f U%s",player_ships[profile_info[profile_slot].ship].name,profile_info[profile_slot].credits*.1f,state==3?"  BACKUP":"");}
 else text(3,16,UI_MUTED,state==0?"EMPTY SLOT":state==2?"UNREADABLE SAVE - NO VALID BACKUP":"CANNOT INSPECT SAVE FILES");
 if(profile_from_intro){text(3,22,UI_GOLD,"> LOAD SELECTED COMMANDER");footer("LEFT/RIGHT SLOT   X LOAD   O TITLE");}
 else {const char *labels[]={"SAVE CHECKPOINT","LOAD CHECKPOINT","RENAME COMMANDER","CREATE PORTRAIT","PAY LOCAL FINE"};for(int i=0;i<5;i++){int y=20+i*2;if(row==i)rect(16,y*8-2,448,15,UI_EDGE);text(3,y,row==i?UI_GOLD:WHITE,"%s %s",row==i?">":" ",labels[i]);if(i==3)text(29,y,UI_CYAN,"%s  X EDIT",portrait_species_name(game.commander_portrait));}footer("UP/DOWN OPTION   X CHOOSE   O BACK");}
 if(game.message_time>0)text(2,30,UI_GOLD,"%.56s",game.message);
}

