static int comms_return=FLIGHT,triangle_arm=0,comms_encounter_conversation=0;static float triangle_hold=0;
static void comms_panel(void){
 if(game.encounter_kind!=ENCOUNTER_NONE&&game.encounter>0){
  header(comms_encounter_conversation?"INCOMING CHANNEL":"ENCOUNTER TRANSMISSION");panel(8,32,464,156);
  if(comms_encounter_conversation){
   text(3,6,GOLD,"CONTACT RESPONSE");text_wrap(3,9,54,7,WHITE,game.voice[0]?game.voice:"Channel open.",0);text(3,20,CYAN,"X CONTINUE");text(30,20,DIM,"O RETURN TO FLIGHT");
  }else{
   text(3,6,GOLD,"NEW SPACE ENCOUNTER");text_wrap(3,9,54,5,WHITE,game.voice[0]?game.voice:"Transmission received.",0);text(3,20,row==0?GOLD:WHITE,"RESPOND");text(20,20,row==1?GOLD:WHITE,"IGNORE");
  }
  footer("UP/DOWN   X CHOOSE   O RETURN");return;
 }
 header("COMMS / CHANNEL CONTROL");panel(8,32,464,156);
 const char *options[]={quiet_comms?"Text chatter: QUIET":"Text chatter: ON","Dismiss current message","Hail current target",valid_target(selected_target)?"Clear current target":"Clear current target (none)","Next radio station","Open radio page","Request station docking","Guild assignments","Change cockpit HUD","Walk station deck","Spacewalk salvage"};
 int first=(row/8)*8;
 for(int i=first;i<11&&i<first+8;i++){int y=5+(i-first)*2;if(i==row)rect(10,y*8-2,458,14,RGB(25,65,77));text(3,y,i==row?GOLD:WHITE,"%s",options[i]);}
 text(3,22,DIM,"PAGE %d / 2",row/8+1);
 text(3,29,CYAN,"Safety alerts stay visible.");
 footer("UP/DOWN   X CHOOSE   O RETURN");
}
