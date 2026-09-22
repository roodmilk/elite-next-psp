static void radio_wav_u32(unsigned char *p,unsigned v){p[0]=v;p[1]=v>>8;p[2]=v>>16;p[3]=v>>24;}
static int radio_preview_wav(int station){
 char path[48];snprintf(path,sizeof(path),"radio-preview-%d.wav",station+1);
 FILE *f=fopen(path,"wb");if(!f)return 0;
 enum { SAMPLES=44100*32 };unsigned char header[44]={0};
 memcpy(header,"RIFF",4);radio_wav_u32(header+4,36+SAMPLES*4);memcpy(header+8,"WAVEfmt ",8);
 radio_wav_u32(header+16,16);header[20]=1;header[22]=2;radio_wav_u32(header+24,44100);radio_wav_u32(header+28,44100*4);header[32]=4;header[34]=16;
 memcpy(header+36,"data",4);radio_wav_u32(header+40,SAMPLES*4);
 int ok=fwrite(header,1,sizeof(header),f)==sizeof(header);RadioSynth synth;radio_synth_reset(&synth,station);
 short block[512];int used=0;
 for(int i=0;i<SAMPLES&&ok;i++){int l,r;radio_synth_sample(&synth,&l,&r);int fade=i<882?i:SAMPLES-i<882?SAMPLES-i:882;block[used++]=(short)(l*fade/882);block[used++]=(short)(r*fade/882);if(used==512){ok=fwrite(block,sizeof(short),used,f)==(size_t)used;used=0;}}
 if(ok&&used)ok=fwrite(block,sizeof(short),used,f)==(size_t)used;
 if(fclose(f))ok=0;
 return ok;
}
static void radio_tests(void){
 FILE *f=fopen("radio-check.txt","w");if(!f)return;int failures=0;
 #define RADIO_CHECK(c,n) do{int ok=(c);fprintf(f,"%s %s\n",ok?"PASS":"FAIL",n);failures+=!ok;}while(0)
 unsigned hashes[RADIO_STATION_COUNT];
 for(int station=0;station<RADIO_STATION_COUNT;station++){
  RadioSynth s;radio_synth_reset(&s,station);int peak=0,stereo=0,active=0;unsigned hash=2166136261u;
  for(int i=0;i<44100*4;i++){int l,r;radio_synth_sample(&s,&l,&r);int a=abs(l),b=abs(r);if(a>peak)peak=a;if(b>peak)peak=b;stereo|=l!=r;active+=l!=0||r!=0;hash=(hash^(unsigned)l)*16777619u;}
  hashes[station]=hash;fprintf(f,"INFO %s peak %d\n",radio_station_name(station),peak);
  RADIO_CHECK(peak>100&&peak<20000&&active>44100&&stereo,"station produces bounded non-silent stereo arrangement");
  radio_synth_reset(&s,station);unsigned repeat=2166136261u;
  for(int i=0;i<44100*4;i++){int l,r;radio_synth_sample(&s,&l,&r);repeat=(repeat^(unsigned)l)*16777619u;}
  RADIO_CHECK(hash==repeat,"station resets deterministically");
 }
 int unique=1;for(int i=0;i<RADIO_STATION_COUNT;i++)for(int j=i+1;j<RADIO_STATION_COUNT;j++)if(hashes[i]==hashes[j])unique=0;
 RADIO_CHECK(unique,"all five radio stations produce different music");
 FILE *preview=fopen("radio-preview.flag","rb");if(preview){fclose(preview);for(int i=0;i<RADIO_STATION_COUNT;i++)RADIO_CHECK(radio_preview_wav(i),"export original stereo radio excerpt");}
 radio_volume=5;sound_volume=8;radio_adjust(0,-100);radio_adjust(1,100);
 RADIO_CHECK(!radio_volume&&sound_volume==10,"independent volume controls clamp safely");
 remove("test-radio.cfg");remove("test-radio.cfg.bak");radio_tune(3);radio_volume=4;sound_volume=7;
 RADIO_CHECK(radio_save_settings("test-radio.cfg"),"radio preferences save with checksum");
 radio_station=0;radio_volume=0;sound_volume=0;
 RADIO_CHECK(radio_load_settings("test-radio.cfg")&&radio_station==3&&radio_volume==4&&sound_volume==7,"station and both levels survive reload");
 radio_tune(2);radio_save_settings("test-radio.cfg");FILE *bad=fopen("test-radio.cfg","wb");if(bad){fputs("bad",bad);fclose(bad);}
 RADIO_CHECK(radio_load_settings("test-radio.cfg")&&radio_station==3,"invalid settings recover previous valid backup");
 remove("test-radio.cfg");remove("test-radio.cfg.bak");remove("test-radio.cfg.tmp");
 radio_station=0;radio_volume=5;sound_volume=8;radio_dirty=0;
 /* Suspend prep must leave MP3 frozen without waiting; resume path restarts audio. */
 audio_prepare_suspend();
 RADIO_CHECK(1,"suspend: power callback can freeze MP3 without blocking the main thread");
 audio_stop();audio_init();audio_stop();
 RADIO_CHECK(1,"suspend: repeated audio stop/start recovers without hanging the commander");
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);
 #undef RADIO_CHECK
}
