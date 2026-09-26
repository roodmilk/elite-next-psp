static float intro_time=0;
static int intro_choice=0;
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
 if(intro_time>.8f)text(5,19,WHITE,"Ryn vanished. Kei kept the channel open.");
 const char *choices[]={"START TUTORIAL - FIRST LIGHT","CONTINUE TUTORIAL","NEW GAME - OPEN CHANNEL","LOAD COMMANDER"};
 for(int i=0;i<4;i++){int y=22+i*2;if(i==intro_choice)rect(32,y*8-2,416,14,RGB(25,65,77));text(5,y,i==intro_choice?GOLD:WHITE,"%s %s",i==intro_choice?">":" ",choices[i]);}
 footer("UP/DOWN CHOOSE   X BEGIN");
 if(game.message_time>0)text(5,29,GOLD,"%.50s",game.message);
}
