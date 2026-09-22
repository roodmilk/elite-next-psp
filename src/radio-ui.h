static void radio_screen(void){
 header("RADIO / DEEP SPACE FM");
 /* Radio face: dial, station notches, levels on the right. */
 rect(8,32,304,188,RGB(18,22,28));rect(10,34,300,184,RGB(28,34,42));
 rect(20,48,276,78,RGB(12,14,18));rect(22,50,272,74,RGB(8,10,14));
 /* Frequency dial line with OFF + five station ticks. */
 int dial_y=88,dial_x0=36,dial_x1=278,span=dial_x1-dial_x0;
 line(dial_x0,dial_y,dial_x1,dial_y,RGB(90,110,120));
 /* OFF at left, stations evenly spaced. */
 int notches=RADIO_STATION_COUNT+1;
 for(int i=0;i<notches;i++){
  int x=dial_x0+(span*i)/(notches-1);
  int active=radio_off?i==0:(!radio_off&&radio_station==i-1);
  line(x,dial_y-10,x,dial_y+10,active?GOLD:RGB(120,140,150));
  if(i==0)text(x/8-1,dial_y/8+2,active?GOLD:DIM,"OFF");
  else text(x/8-1,dial_y/8+2,active?GOLD:DIM,"%d",i);
 }
 /* Needle */
 {
  int sel=radio_off?0:radio_station+1;
  int x=dial_x0+(span*sel)/(notches-1);
  line(x,dial_y-22,x,dial_y+4,CYAN);rect(x-3,dial_y-26,7,5,CYAN);
 }
 if(row==0)rect(18,46,280,82,RGB(25,65,77));
 text(3,6,row==0?WHITE:DIM,"TUNER  LEFT/RIGHT");
 text(3,16,radio_static_ms>0?AMBER:radio_off?DIM:CYAN,radio_static_ms>0?"-- STATIC --":radio_off?"RADIO OFF":"LOCKED");
 text(3,18,WHITE,"%.18s",radio_off?"(silence)":radio_station_name(radio_station));
 text(3,20,CYAN,"%.18s",radio_off?"Turn on with Right":radio_station_genre(radio_station));
 text(3,22,DIM,radio_off?"":(radio_track_count[radio_station]?"FOLDER MP3":"GENERATED"));

 panel(320,32,152,188);
 text(41,5,GOLD,"LEVELS");
 for(int i=0;i<2;i++){
  int y=8+i*4,volume=i?sound_volume:radio_volume;
  if(row==1+i)rect(326,y*8-2,140,14,RGB(25,65,77));
  text(41,y,WHITE,"%-7s %2d",i?"FX":"MUSIC",volume);
  rect(330,y*8+10,120,5,DIM);rect(330,y*8+10,volume*12,5,i?AMBER:CYAN);
 }
 text(41,18,DIM,"Stations marked 1-5");
 text(41,20,DIM,"Left of 1 is OFF");
 text(41,22,DIM,"Triangle: power");
 footer("UP/DOWN  L/R TUNE OR LEVEL  TRI OFF  O BACK");
}
