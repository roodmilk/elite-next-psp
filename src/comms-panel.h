static int comms_return=FLIGHT,triangle_arm=0,comms_encounter_conversation=0;static float triangle_hold=0;
static void comms_panel(void){
 if(comms_encounter_conversation||encounter_requires_reply(&game)){
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
 header("COMMS PANEL");panel(8,32,464,156);
 const char *options[]={quiet_comms?"Text chatter: QUIET":"Text chatter: ON","Dismiss current message","Hail current target",valid_target(selected_target)?"Clear current target":"Clear current target (none)","Next radio station","Open radio page","Request station docking","Guild assignments","Change cockpit HUD","Walk station deck","Spacewalk salvage","HUD layout","Text chatter","High contrast focus","Radio and audio","Controls","Third-person view"};
 int first=(row/8)*8;
 for(int i=first;i<17&&i<first+8;i++){int y=5+(i-first)*2;if(i==row)rect(10,y*8-2,458,14,RGB(25,65,77));text(3,y,i==row?GOLD:WHITE,"%s",options[i]);}
 text(3,22,DIM,"PAGE %d / 3",row/8+1);
 text(3,29,CYAN,"Safety alerts stay visible.");
 footer("UP/DOWN   X CHOOSE   O RETURN");
}
