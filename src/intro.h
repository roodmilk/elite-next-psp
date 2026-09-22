static float intro_time=0;
static void intro_screen(void){
 rect(0,0,W,H,RGB(3,8,19));
 for(int i=0;i<75;i++){
  unsigned seed=(unsigned)(i+1)*2654435761u;int x=(int)((seed>>8)%480),base=(seed>>19)%272;
  int y=(base+(int)(intro_time*(3+i%5)))%272;
  unsigned c=i%7?RGB(65,95,130):RGB(165,210,223);pixel(x,y,c);if(i%9==0)pixel(x,y+1,c);
 }
 draw_next_art(next_logo,320,72,80,45,320,72);
 int glint=80+(int)(intro_time*30)%320;rect(80,122,320,1,RGB(19,74,95));rect(glint,122,3,1,CYAN);
 text(12,17,GOLD,"A SIGNAL WORTH FOLLOWING");
 if(intro_time>.8f)text(5,20,WHITE,"Ryn followed a quiet signal beyond the trade lanes.");
 if(intro_time>2.0f)text(5,22,WHITE,"Then her ship went silent — and three calls went with it.");
 if(intro_time>3.2f)text(5,24,WHITE,"Kei kept the channel open. Now the berth, and the ship, are yours.");
 if(intro_time>4.4f)text(5,27,CYAN,"Trade. Explore. Bring someone home.");
 footer("X BEGIN   TRIANGLE LOAD COMMANDER   START SKIP");
 if(game.message_time>0)text(5,29,GOLD,"%.50s",game.message);
}
