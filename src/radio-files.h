/* User radio library. Paths are relative to EBOOT.PBP. */
#include <pspiofilemgr.h>
#include <ctype.h>
#define RADIO_TRACKS_PER_STATION 24
#define RADIO_TRACK_PATH 160
static const char *radio_music_dirs[RADIO_STATION_COUNT]={
 "music/Deep Field","music/Neon Transit","music/Pixel Comet",
 "music/Velvet Orbit","music/Far Horizons"
};
static char radio_tracks[RADIO_STATION_COUNT][RADIO_TRACKS_PER_STATION][RADIO_TRACK_PATH];
static int radio_track_count[RADIO_STATION_COUNT]={0};
static volatile int radio_file_station=-1,radio_file_track=-1;
static volatile char radio_file_title[64]="Generated broadcast";
static volatile int radio_file_error=0;

static int radio_has_mp3_extension(const char *name){
 int n=(int)strlen(name);if(n<5)return 0;const char *e=name+n-4;
 return e[0]=='.'&&tolower((unsigned char)e[1])=='m'&&tolower((unsigned char)e[2])=='p'&&tolower((unsigned char)e[3])=='3';
}
static void radio_make_folders(void){
 sceIoMkdir("music",0777);for(int i=0;i<RADIO_STATION_COUNT;i++)sceIoMkdir(radio_music_dirs[i],0777);
}
static void radio_scan_music(void){
 radio_make_folders();
 for(int station=0;station<RADIO_STATION_COUNT;station++){
  radio_track_count[station]=0;SceUID dir=sceIoDopen(radio_music_dirs[station]);if(dir<0)continue;
  SceIoDirent entry;memset(&entry,0,sizeof(entry));
  while(radio_track_count[station]<RADIO_TRACKS_PER_STATION&&sceIoDread(dir,&entry)>0){
   if(radio_has_mp3_extension(entry.d_name)){int slot=radio_track_count[station]++;snprintf(radio_tracks[station][slot],RADIO_TRACK_PATH,"%s/%s",radio_music_dirs[station],entry.d_name);}
   memset(&entry,0,sizeof(entry));
  }
  sceIoDclose(dir);
 }
}
static const char *radio_track_label(const char *path){const char *name=strrchr(path,'/');return name?name+1:path;}
