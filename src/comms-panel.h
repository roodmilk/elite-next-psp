static int comms_return=FLIGHT,triangle_arm=0,comms_encounter_conversation=0;static float triangle_hold=0;
#include "quiet-hails.h"
static void comms_panel(void){
 if(night_open&&night_live()){night_panel();return;}
 if(comms_encounter_conversation||incoming_reply_ready()){
  dialogue_begin(comms_encounter_conversation?"INCOMING CHANNEL":"ENCOUNTER TRANSMISSION");
  static const char *names[]={"CONTACT","KEI","VENN","DOCKHAND","LOCAL LAW","COMPUTER","CONTACT"};
  int who=game.voice_who,role=game.voice_role;
  if(who<0||who>VOICE_CONTACT)who=0;
  if(role<0||role>EXPLORERS)role=EXPLORERS;
  dialogue_speech(who==VOICE_CONTACT?faction_names[role]:names[who],role,game.voice_seed,game.voice[0]?game.voice:"Channel open.",0);
  dialogue_context("YOUR REPLY",comms_encounter_conversation?"Choose how to continue the conversation.":"Respond to this encounter, or ignore the channel.");
  if(comms_encounter_conversation){
   const char *options[]={"CONTINUE DISCUSSION","ASK ABOUT THIS ENCOUNTER","END CHANNEL"};
   for(int i=0;i<3;i++)dialogue_reply(i,options[i]);
  }else{
   dialogue_reply(0,"Respond");dialogue_reply(1,"Ignore");
  }
  footer("UP/DOWN   X CHOOSE   O RETURN");return;
 }
 header("COMMS PANEL");panel(8,32,464,194);
 const char *options[]={"Dismiss current message","Hail current target",valid_target(selected_target)?"Clear current target":"Clear current target (none)","REQUEST AUTO-DOCK","Guild assignments","Walk station deck","Spacewalk salvage","HUD layout","Text chatter","High contrast focus","Radio and audio","Controls","Third-person view"};
 int first=row/7*7;
 text(3,5,UI_SIGNAL,first?"DISPLAY / AUDIO / CONTROLS":"CHANNEL ACTIONS");
 text(49,5,UI_MUTED,"%d / 2",first?2:1);
 for(int i=first;i<first+7&&i<COMMS_OPTION_COUNT;i++){
  int y=60+(i-first)*22;if(i==row){rect(12,y-2,454,18,UI_RAISED);rect(12,y-2,3,18,UI_ACCENT);}
  text_px(24,y,i==row?UI_ACCENT:UI_TEXT,"%s",options[i]);
  const char *value=i==7?(hud_mode==0?"FULL":hud_mode==1?"MINIMAL":"SCENIC"):i==8?(quiet_comms?"QUIET":"ON"):i==9?(high_contrast?"ON":"OFF"):i==12?(third_person?"ON":"OFF"):i==10||i==11?"OPEN":"";
  text_px(368,y,UI_SIGNAL,"%s",value);
 }
 text(3,29,UI_MUTED,"Safety alerts stay visible. Up/down continues.");
 footer("UP/DOWN   X CHANGE / OPEN   O RETURN");
}
