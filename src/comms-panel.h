static int comms_return=FLIGHT,triangle_arm=0,comms_encounter_conversation=0;static float triangle_hold=0;
static void comms_panel(void){
 if(encounter_requires_reply(&game)){
  header(comms_encounter_conversation?"INCOMING CHANNEL":"ENCOUNTER TRANSMISSION");panel(8,32,464,156);
  if(comms_encounter_conversation){
   text(3,6,GOLD,"CONTACT RESPONSE");text_wrap(3,9,54,5,WHITE,game.voice[0]?game.voice:"Channel open.",0);
   const char *options[]={"CONTINUE DISCUSSION","ASK ABOUT THIS ENCOUNTER","END CHANNEL"};
   for(int i=0;i<3;i++){int y=15+i*2;text(3,y,row==i?GOLD:WHITE,"%s%s",row==i?"> ":"  ",options[i]);}
  }else{
   text(3,6,GOLD,"NEW SPACE ENCOUNTER");text_wrap(3,9,54,5,WHITE,game.voice[0]?game.voice:"Transmission received.",0);text(3,20,row==0?GOLD:WHITE,"RESPOND");text(20,20,row==1?GOLD:WHITE,"IGNORE");
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
