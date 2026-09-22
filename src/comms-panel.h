static int comms_return=FLIGHT,triangle_arm=0;static float triangle_hold=0;
static void comms_panel(void){
 header("COMMS / CHANNEL CONTROL");panel(8,32,464,156);
 const char *options[]={quiet_comms?"Text chatter: QUIET":"Text chatter: ON","Dismiss current message","Hail current target",valid_target(selected_target)?"Clear current target":"Clear current target (none)","Next radio station","Open radio page","Request station docking","Guild assignments","Change cockpit HUD","Walk station deck","Spacewalk salvage"};
 int first=(row/8)*8;
 for(int i=first;i<11&&i<first+8;i++){int y=5+(i-first)*2;if(i==row)rect(10,y*8-2,458,14,RGB(25,65,77));text(3,y,i==row?GOLD:WHITE,"%s",options[i]);}
 text(3,22,DIM,"PAGE %d / 2",row/8+1);
 text(3,29,CYAN,"Safety alerts stay visible.");
 footer("UP/DOWN   X CHOOSE   O RETURN");
}
