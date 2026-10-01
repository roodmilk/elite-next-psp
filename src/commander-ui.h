/* Three manual checkpoint slots. Slot one retains the legacy filename. */
static void tutorial_reset_frontend(void);
static int profile_slot=0,profile_from_intro=0,profile_mode=0,profile_portrait_edit=0,profile_portrait_part=0,profile_confirm=0,profile_key=0;
static char profile_draft[25];
static struct {int state,credits,system,ship;unsigned portrait;char name[25];} profile_info[3];
static const char *profile_path(void){return game.tutorial_step&&!profile_from_intro?"tutorial.sav":commander_slots[profile_slot];}
static int profile_file_exists(const char *path){FILE *f=fopen(path,"rb");if(f){fclose(f);return 1;}return 0;}
static void profile_refresh(void){
 Game *preview=malloc(sizeof(Game));
 for(int i=0;i<3;i++){
  memset(&profile_info[i],0,sizeof(profile_info[i]));if(!preview){profile_info[i].state=-1;continue;}
  const char *path=game.tutorial_step&&!profile_from_intro?"tutorial.sav":commander_slots[i];char bak[64];snprintf(bak,sizeof(bak),"%s.bak",path);
  if(load_game(preview,path)){profile_info[i].state=strstr(preview->message,"backup")?3:1;profile_info[i].credits=preview->credits;profile_info[i].system=preview->system;profile_info[i].ship=preview->ship;profile_info[i].portrait=preview->commander_portrait;memcpy(profile_info[i].name,preview->commander_name,25);}
  else if(profile_file_exists(path)||profile_file_exists(bak))profile_info[i].state=2;
 }
 free(preview);
}
static void profile_enter(void){profile_slot=commander_active_slot;profile_from_intro=profile_mode=profile_portrait_edit=profile_confirm=0;profile_refresh();}
static void profile_open_load(void){change_page(STATUS);profile_from_intro=1;row=1;profile_refresh();}
static int profile_save(void){
 if(!game.docked){message(&game,"Dock at a station to save a checkpoint.");return 0;}
 if(!save_game(&game,profile_path()))return 0;
 if(!game.tutorial_step)commander_active_slot=profile_slot;profile_refresh();return 1;
}
static void profile_load(void){
 if(!load_game(&game,profile_path()))return;
 int recovered=strstr(game.message,"backup")!=NULL;commander_active_slot=profile_slot;
 night_npc=-1;night_open=0;night_wait=480;night_seen=0;
 tutorial_reset_frontend();change_page(HOME);message(&game,recovered?"Commander recovered from backup.":"Commander loaded. Welcome back.");
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
  if(pressed&PSP_CTRL_CIRCLE){profile_portrait_edit=0;message(&game,"Portrait updated. Save to keep your changes.");return 0;}
  if(pressed&PSP_CTRL_UP)profile_portrait_part=(profile_portrait_part+PORTRAIT_PART_COUNT-1)%PORTRAIT_PART_COUNT;
  if(pressed&PSP_CTRL_DOWN)profile_portrait_part=(profile_portrait_part+1)%PORTRAIT_PART_COUNT;
  if(pressed&PSP_CTRL_LEFT)game.commander_portrait=portrait_adjust(game.commander_portrait,profile_portrait_part,-1);
  if(pressed&PSP_CTRL_RIGHT)game.commander_portrait=portrait_adjust(game.commander_portrait,profile_portrait_part,1);
  if(pressed&PSP_CTRL_SQUARE)game.commander_portrait=portrait_commander_random(art_hash(game.rng^(unsigned)(game.time*1000.f)^game.commander_portrait));
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
 if(!profile_from_intro){if(pressed&PSP_CTRL_UP)row=(row+4)%5;if(pressed&PSP_CTRL_DOWN)row=(row+1)%5;}
 if(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT)){
  int dir=(pressed&PSP_CTRL_LEFT)?-1:1;
  if(row<=1&&(!game.tutorial_step||profile_from_intro)){profile_slot=(profile_slot+3+dir)%3;game.message_time=0;}
 }
 if(pressed&PSP_CTRL_CROSS){
  if(row==0){if(!game.docked)message(&game,"Dock at a station to save a checkpoint.");else if(profile_info[profile_slot].state<0)message(&game,"Unable to inspect slots. Reopen SAVE/STATUS.");else if(profile_info[profile_slot].state)profile_confirm=1;else return profile_save();}
  else if(row==1){if(profile_info[profile_slot].state==1||profile_info[profile_slot].state==3)profile_confirm=2;else message(&game,"No valid save or recovery backup in this slot.");}
  else if(row==2){memcpy(profile_draft,game.commander_name,25);profile_mode=1;profile_key=0;game.message_time=0;}
  else if(row==3){game.commander_portrait=portrait_normalize(game.commander_portrait);profile_portrait_edit=1;profile_portrait_part=0;game.message_time=0;}
  else if(row==4){if(game.docked&&game.legal>0)police_pay_desk(&game);else message(&game,game.legal?"Dock to pay your local fine.":"No local fines to pay.");}
 }
 return 0;
}
static void status(void){
 header(profile_from_intro?"LOAD COMMANDER":"SAVE / STATUS");panel(8,32,464,214);
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
 if(profile_confirm){text(3,14,UI_CYAN,"SLOT %d: %.24s",profile_slot+1,profile_info[profile_slot].name);text(3,17,UI_GOLD,profile_confirm==3?"PERMANENTLY DELETE THIS SAVE?":profile_confirm==1?"REPLACE THIS SLOT?":"LOAD THIS CHECKPOINT?");text_wrap(3,20,53,3,WHITE,profile_confirm==3?"Only this slot and its recovery backup will be deleted. This cannot be undone. Other slots and tutorial progress are kept.":profile_confirm==1?"The selected file will be replaced. Its previous valid save is retained as a recovery backup.":"Unsaved progress and profile changes will be lost. Your ship returns to its saved station.",0);footer("X CONFIRM   O CANCEL");return;}
 if(profile_portrait_edit){
  rect(12,36,456,206,UI_PANEL);draw_commander_portrait(24,82,96,game.commander_portrait);text(3,5,UI_GOLD,"MODULAR COMMANDER PORTRAIT");text(3,7,UI_CYAN,"SPECIES: %s",portrait_species_name(game.commander_portrait));
  for(int i=0;i<PORTRAIT_PART_COUNT;i++){int yy=6+i*2;if(i==profile_portrait_part)rect(144,yy*8-2,320,15,UI_EDGE);text(19,yy,i==profile_portrait_part?UI_GOLD:WHITE,"%s",portrait_part_name(i));if(i==0)text(45,yy,UI_CYAN,"%s",portrait_species_name(game.commander_portrait));else text(50,yy,UI_CYAN,"%d / %d",portrait_part_value(game.commander_portrait,i)+1,portrait_part_count(i));}
  text(3,24,UI_MUTED,"All species can command ships, including robots.");text(3,26,UI_MUTED,"The same four-byte identity appears everywhere.");footer("UP/DOWN PART   LEFT/RIGHT CHANGE   SQ RANDOM   O DONE");return;
 }
 if(profile_mode){text(3,14,UI_GOLD,"NAME: %.24s_",profile_draft);for(int i=0;i<40;i++){int x=3+(i%8)*7,y=17+(i/8)*3;if(i==profile_key)rect(x*8-2,y*8-2,48,18,UI_EDGE);if(i<36)text(x+2,y,i==profile_key?UI_GOLD:WHITE,"%c","ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"[i]);else text(x,y,i==profile_key?UI_GOLD:WHITE,"%s",i==36?"SPACE":i==37?"  -":i==38?" DEL":" DONE");}if(game.message_time>0)text(3,15,UI_GOLD,"%.54s",game.message);footer("X TYPE  SQ DELETE  START DONE  O CANCEL");return;}
 text(3,14,UI_GOLD,game.tutorial_step&&!profile_from_intro?"TUTORIAL CHECKPOINT":"COMMANDER SLOT %d / 3   < LEFT / RIGHT >",profile_slot+1);
 int state=profile_info[profile_slot].state;
 if(state==1||state==3){text(3,16,WHITE,"%.24s  /  %.12s",profile_info[profile_slot].name,game.systems[profile_info[profile_slot].system].name);text(3,18,UI_MUTED,"%.18s  %.1f U%s",player_ships[profile_info[profile_slot].ship].name,profile_info[profile_slot].credits*.1f,state==3?"  BACKUP":"");}
 else text(3,16,UI_MUTED,state==0?"EMPTY SLOT":state==2?"UNREADABLE SAVE - NO VALID BACKUP":"CANNOT INSPECT SAVE FILES");
 if(profile_from_intro){text(3,22,UI_GOLD,"> LOAD SELECTED COMMANDER");footer("LEFT/RIGHT SLOT   X LOAD   O TITLE");}
 else {const char *labels[]={"SAVE CHECKPOINT","LOAD CHECKPOINT","RENAME COMMANDER","CREATE PORTRAIT","PAY LOCAL FINE"};for(int i=0;i<5;i++){int y=20+i*2;if(row==i)rect(16,y*8-2,448,15,UI_EDGE);text(3,y,row==i?UI_GOLD:WHITE,"%s %s",row==i?">":" ",labels[i]);if(i==3)text(29,y,UI_CYAN,"%s  X EDIT",portrait_species_name(game.commander_portrait));}footer("UP/DOWN OPTION   X CHOOSE   O BACK");}
 if(game.message_time>0)text(2,30,UI_GOLD,"%.56s",game.message);
}
