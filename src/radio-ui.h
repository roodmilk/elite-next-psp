static void radio_screen(void){
 header("RADIO / DEEP SPACE FM");panel(8,32,260,156);panel(276,32,196,156);
 text(2,5,CYAN,"STATION");
 for(int i=0;i<RADIO_STATION_COUNT;i++){int y=7+i*2;
  if(row==i)rect(10,y*8-2,256,14,RGB(25,65,77));
  text(2,y,i==radio_station?GOLD:WHITE,"%s %-18.18s %2d",i==radio_station?">":" ",radio_station_name(i),radio_track_count[i]);
 }
 for(int i=0;i<2;i++){int y=19+i*3,volume=i?sound_volume:radio_volume;
  if(row==RADIO_STATION_COUNT+i)rect(10,y*8-2,256,14,RGB(25,65,77));
  text(2,y,WHITE,"%-9s %2d / 10",i?"EFFECTS":"MUSIC",volume);
  rect(192,y*8+1,60,5,DIM);rect(192,y*8+1,volume*6,5,CYAN);
 }
 text(36,5,GOLD,radio_volume?"NOW PLAYING":"MUSIC MUTED");
 text(36,8,WHITE,"%.22s",radio_station_name(radio_station));
 text(36,10,CYAN,"%.22s",radio_station_genre(radio_station));
 text(36,13,DIM,radio_track_count[radio_station]?"SHUFFLED MUSIC FOLDER":"GENERATED FALLBACK");
 if(radio_file_station==radio_station)text(36,15,WHITE,"%.22s",(const char*)radio_file_title);else if(radio_file_error)text(36,15,RED,"CAN'T PLAY THIS MP3");else text(36,15,DIM,"Waiting for next track");
 text(36,17,DIM,"%d MP3%s FOUND",radio_track_count[radio_station],radio_track_count[radio_station]==1?"":"S");
 text(36,19,WHITE,"Triangle: music mute");
 text(36,21,DIM,"O saves and returns");
 footer("UP/DOWN  X TUNE  LEFT/RIGHT LEVEL  TRI MUTE  O BACK");
}
