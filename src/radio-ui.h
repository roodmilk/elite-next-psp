static void radio_screen(void){
 header("RADIO / DEEP SPACE FM");
 /* Radio chassis: speaker grille left, dial face, levels right. */
 rect(8,32,304,188,RGB(22,18,14));rect(10,34,300,184,RGB(36,30,22));
 rect(16,42,112,168,RGB(14,12,10));
 for(int g=0;g<9;g++){int y=50+g*16;line(24,y,112,y,RGB(48,40,30));line(24,y+4,112,y+4,RGB(28,24,18));}
 rect(36,70,68,68,RGB(20,18,16));circle(70,104,26,RGB(70,60,48));circle(70,104,18,RGB(40,34,28));
 text(5,22,DIM,"SPKR");
 /* Physical power switch on the radio chassis. Triangle toggles it. */
 rect(32,154,80,32,RGB(25,21,17));rect(34,156,76,28,RGB(55,44,30));
 circle(47,170,8,radio_off?RGB(110,55,45):RGB(85,212,212));line(47,158,47,169,WHITE);
 text(8,20,radio_off?RED:GOLD,"TRI POWER");

 /* Dial glass */
 rect(136,42,166,110,RGB(10,12,16));rect(138,44,162,106,RGB(6,8,12));
 if(row==0)rect(136,42,166,110,RGB(25,65,77));
 text(18,6,row==0?WHITE:DIM,"TUNER");
 /* Frequency dial: OFF then stations 1-5, obvious notches. */
 int dial_y=96,dial_x0=152,dial_x1=286,span=dial_x1-dial_x0;
 line(dial_x0,dial_y,dial_x1,dial_y,RGB(90,110,120));
 int notches=RADIO_STATION_COUNT+1;
 for(int i=0;i<notches;i++){
  int x=dial_x0+(span*i)/(notches-1);
  int active=radio_off?i==0:(!radio_off&&radio_station==i-1);
  line(x,dial_y-12,x,dial_y+12,active?GOLD:RGB(120,140,150));
  if(i==0)text(x/8-1,dial_y/8+2,active?GOLD:DIM,"OFF");
  else {text(x/8,dial_y/8+2,active?GOLD:CYAN,"%d",i);rect(x-2,dial_y-16,5,3,active?GOLD:RGB(70,90,100));}
 }
 /* Needle */
 {
  int sel=radio_off?0:radio_station+1;
  int x=dial_x0+(span*sel)/(notches-1);
  line(x,dial_y-24,x,dial_y+6,CYAN);rect(x-4,dial_y-28,9,6,CYAN);
 }
 /* Between-station static cue on the glass */
 if(radio_static_ms>0){
  for(int s=0;s<18;s++){int sx=148+(s*17+radio_static_ms*3)%150,sy=52+(s*11)%36;pixel(sx,sy,AMBER);pixel(sx+1,sy,RGB(180,140,40));}
  text(18,14,AMBER,"-- STATIC --");
 }else if(radio_off)text(18,14,DIM,"RADIO OFF");
 else text(18,14,CYAN,"LOCKED");
 text(18,16,WHITE,"%.18s",radio_off?"(silence)":radio_station_name(radio_station));
 text(18,18,CYAN,"%.18s",radio_off?"Right: station 1":radio_station_genre(radio_station));

 /* Volume panel */
 panel(320,32,152,188);
 text(41,5,GOLD,"LEVELS");
 for(int i=0;i<2;i++){
  int y=8+i*4,volume=i?sound_volume:radio_volume;
  if(row==1+i)rect(326,y*8-2,140,14,RGB(25,65,77));
  text(41,y,WHITE,"%-7s %2d",i?"FX":"MUSIC",volume);
  rect(330,y*8+10,120,5,DIM);rect(330,y*8+10,volume*12,5,i?AMBER:CYAN);
 }
 text(41,23,DIM,radio_off?"":(radio_track_count[radio_station]?"FOLDER MP3":"GENERATED"));
 footer("O BACK");
}
