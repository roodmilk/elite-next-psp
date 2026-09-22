/* Small independent preferences file. Commander saves remain compatible. */
static volatile int radio_station=0,radio_volume=5,sound_volume=8;
static volatile int radio_off=0,radio_static_ms=0;
static int radio_dirty=0;
static int quiet_comms=0;
typedef struct {uint32_t magic,version,station,music,effects,quiet,check;} RadioSettings;
static uint32_t radio_checksum(const RadioSettings *s){return s->magic^s->version^(s->station*65537u)^(s->music*257u)^(s->effects*17u)^(s->quiet*8191u)^0x597214a3u;}
static int radio_read_settings(const char *path,RadioSettings *s){
 FILE *f=fopen(path,"rb");if(!f)return 0;
 int ok=fread(s,1,sizeof(*s),f)==sizeof(*s);fclose(f);
 return ok&&s->magic==0x52414449u&&s->version==2&&s->station<=RADIO_STATION_COUNT&&s->music<=10&&s->effects<=10&&s->quiet<=1&&s->check==radio_checksum(s);
}
static int radio_load_settings(const char *path){
 RadioSettings s;char bak[256];int len=snprintf(bak,sizeof(bak),"%s.bak",path);
 if(!radio_read_settings(path,&s)&&(len<0||len>=(int)sizeof(bak)||!radio_read_settings(bak,&s)))return 0;
 if(s.station>=RADIO_STATION_COUNT){radio_off=1;radio_station=0;}else{radio_off=0;radio_station=s.station;}
 radio_volume=s.music;sound_volume=s.effects;quiet_comms=s.quiet;radio_dirty=0;return 1;
}
static int radio_save_settings(const char *path){
 char tmp[256],bak[256];int n=snprintf(tmp,sizeof(tmp),"%s.tmp",path),m=snprintf(bak,sizeof(bak),"%s.bak",path);
 if(n<0||m<0||n>=(int)sizeof(tmp)||m>=(int)sizeof(bak))return 0;
 unsigned st=radio_off?(unsigned)RADIO_STATION_COUNT:(unsigned)radio_station;
 RadioSettings s={0x52414449u,2,st,(unsigned)radio_volume,(unsigned)sound_volume,(unsigned)quiet_comms,0},old;
 s.check=radio_checksum(&s);FILE *f=fopen(tmp,"wb");if(!f)return 0;
 int ok=fwrite(&s,1,sizeof(s),f)==sizeof(s);if(fflush(f))ok=0;if(fclose(f))ok=0;
 if(!ok||!radio_read_settings(tmp,&old))return 0;
 if(radio_read_settings(path,&old)){
  f=fopen(bak,"rb");if(f){fclose(f);if(remove(bak))return 0;}
  if(rename(path,bak))return 0;
 }else {f=fopen(path,"rb");if(f){fclose(f);if(remove(path))return 0;}}
 if(rename(tmp,path)){if(radio_read_settings(bak,&old))rename(bak,path);return 0;}
 radio_dirty=0;return 1;
}
static void radio_tune(int station){
 if(station>=0&&station<RADIO_STATION_COUNT){radio_station=station;radio_off=0;radio_dirty=1;}
}
static void radio_adjust(int effects,int delta){
 int volume=(effects?sound_volume:radio_volume)+delta;
 if(volume<0)volume=0;
 if(volume>10)volume=10;
 if(effects)sound_volume=volume;else radio_volume=volume;radio_dirty=1;
}
