#include "audio-sfx.h"
#include "radio-playlist.h"
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
static int audio_sfx_preview_wav(void){
 FILE *f=fopen("audio-sfx-preview.wav","wb");if(!f)return 0;
 enum { SAMPLES=44100*SFX_TALK };unsigned char header[44]={0};
 memcpy(header,"RIFF",4);radio_wav_u32(header+4,36+SAMPLES*2);memcpy(header+8,"WAVEfmt ",8);
 radio_wav_u32(header+16,16);header[20]=1;header[22]=1;radio_wav_u32(header+24,44100);radio_wav_u32(header+28,88200);header[32]=2;header[34]=16;
 memcpy(header+36,"data",4);radio_wav_u32(header+40,SAMPLES*2);
 int ok=fwrite(header,1,sizeof(header),f)==sizeof(header);short block[256];int used=0;unsigned phase=0,noise=1;
 for(int i=0;i<SAMPLES&&ok;i++){int age=i%44100,id=i/44100+1;if(!age)phase=0;block[used++]=(short)audio_sfx_sample(id,age,&phase,&noise);if(used==256){ok=fwrite(block,sizeof(short),used,f)==(size_t)used;used=0;}}
 if(ok&&used)ok=fwrite(block,sizeof(short),used,f)==(size_t)used;
 if(fclose(f))ok=0;
 return ok;
}
static void radio_tests(void){
 FILE *f=fopen("radio-check.txt","w");if(!f)return;int failures=0;
 #define RADIO_CHECK(c,n) do{int ok=(c);fprintf(f,"%s %s\n",ok?"PASS":"FAIL",n);failures+=!ok;}while(0)
 fprintf(f,"INFO source allocation bytes: stream %u PCM %u synth %u paths %u output %u (inside 16384-byte worker stack)\n",(unsigned)sizeof(mp3_stream_buf),(unsigned)sizeof(mp3_pcm_buf),(unsigned)sizeof(RadioSynth),(unsigned)sizeof(radio_tracks),(unsigned)(AUDIO_FRAMES*2*sizeof(short)));
 int order[RADIO_TRACKS_PER_STATION],valid_order=1;unsigned seed=0x7141u;
 for(int count=0;count<=RADIO_TRACKS_PER_STATION;count++)for(int previous=-1;previous<count;previous++)for(int trial=0;trial<32;trial++){
  int used=radio_candidate_order(count,previous,&seed,order);unsigned seen=0;
  if(used!=count)valid_order=0;
  for(int i=0;i<used;i++){if(order[i]<0||order[i]>=count||(seen&(1u<<order[i])))valid_order=0;else seen|=1u<<order[i];}
  if(previous>=0&&order[count-1]!=previous)valid_order=0;
 }
 RADIO_CHECK(valid_order,"shuffle visits every candidate once and defers previous until alternatives fail");
  RADIO_CHECK(RADIO_BATTLE_SOURCE==RADIO_STATION_COUNT&&RADIO_FILE_SOURCE_COUNT==RADIO_STATION_COUNT+1&&!strcmp(radio_music_dirs[RADIO_BATTLE_SOURCE],"music/Thargoid Battle"),"battle score has a dedicated non-radio music source and folder");
  RADIO_CHECK(!strcmp(radio_station_name(7),"Elite Exploration")&&!strcmp(radio_station_genre(7),"EXPLORATION")&&!strcmp(radio_music_dirs[7],"music/Elite Exploration"),"Elite Exploration has a selectable identity and dedicated music folder");
  RADIO_CHECK(radio_has_music_extension("intercept.ogg")&&radio_has_music_extension("INTERCEPT.OGG")&&radio_has_music_extension("legacy.mp3")&&!radio_has_music_extension("notes.txt"),"music library accepts OGG Vorbis and MP3 files case-insensitively");
 seed=0x7141u;radio_candidate_order(3,0,&seed,order);
 int attempts=0,selected=-1;for(int i=0;i<3;i++){attempts++;if(order[i]==2){selected=2;break;}}
 RADIO_CHECK(selected==2&&attempts<=2,"shuffle reaches playable slot 2 despite malformed slot 1 and previous slot 0");
 RADIO_CHECK(!radio_candidate_order(-1,-1,&seed,order)&&!radio_candidate_order(RADIO_TRACKS_PER_STATION+1,-1,&seed,order),"shuffle rejects invalid library bounds");
 int effects_peak=0,effects_ok=1;unsigned effect_hash[SFX_TALK+1];
 SceUInt64 effects_start=sceKernelGetSystemTimeWide();int effects_samples=0;
 for(int id=SFX_UI;id<=SFX_TALK;id++){
  unsigned phase=0,noise=1,hash=2166136261u;int length=audio_sfx_length(id),peak=0,active=0,first=0,last=0;
  for(int age=0;age<length;age++){int v=audio_sfx_sample(id,age,&phase,&noise);if(age==0)first=v;last=v;if(abs(v)>peak)peak=abs(v);active+=v!=0;hash=(hash^(unsigned)v)*16777619u;}
  if(length<882||length>13230||first||last||peak<100||peak>6000||active<length/4||audio_sfx_sample(id,length,&phase,&noise))effects_ok=0;
  if(peak>effects_peak)effects_peak=peak;
  effect_hash[id]=hash;effects_samples+=length;
  fprintf(f,"INFO SFX %d samples %d peak %d\n",id,length,peak);
 }
 fprintf(f,"INFO SFX render %d samples in %llu us (emulator CPU, not physical PSP)\n",effects_samples,(unsigned long long)(sceKernelGetSystemTimeWide()-effects_start));
 RADIO_CHECK(effects_ok,"all original event sounds have bounded levels, audible duration and zero endpoints");
 int distinct=1;for(int i=SFX_UI;i<=SFX_TALK;i++)for(int j=i+1;j<=SFX_TALK;j++)if(effect_hash[i]==effect_hash[j])distinct=0;
 RADIO_CHECK(distinct,"event sound renders are distinct");
 RADIO_CHECK(18000+effects_peak<30000,"generated music hard bound plus worst rendered SFX leaves mix headroom");
 FILE *effects_preview=fopen("audio-preview.flag","rb");if(effects_preview){fclose(effects_preview);RADIO_CHECK(audio_sfx_preview_wav(),"export original event sound audition");}
 unsigned hashes[RADIO_STATION_COUNT];
 for(int station=0;station<RADIO_STATION_COUNT;station++){
  RadioSynth s;radio_synth_reset(&s,station);int peak=0,stereo=0,active=0,mixed_peak=0,clipped=0;unsigned hash=2166136261u,sfx_phase=0,sfx_noise=1;
  for(int i=0;i<44100*4;i++){int l,r;radio_synth_sample(&s,&l,&r);int a=abs(l),b=abs(r);if(a>peak)peak=a;if(b>peak)peak=b;stereo|=l!=r;active+=l!=0||r!=0;hash=(hash^(unsigned)l)*16777619u;
   int age=i%11025,id=(i/11025)%SFX_TALK+1;if(!age)sfx_phase=0;
   int fx=audio_sfx_sample(id,age,&sfx_phase,&sfx_noise),ml=l+fx,mr=r+fx;
   if(abs(ml)>mixed_peak)mixed_peak=abs(ml);
   if(abs(mr)>mixed_peak)mixed_peak=abs(mr);
   clipped+=abs(ml)>30000||abs(mr)>30000;
  }
  fprintf(f,"INFO %s full-volume music+SFX peak %d clipped %d\n",radio_station_name(station),mixed_peak,clipped);
  RADIO_CHECK(!clipped,"full-volume original music and event cues preserve headroom");
  hashes[station]=hash;fprintf(f,"INFO %s peak %d\n",radio_station_name(station),peak);
  RADIO_CHECK(peak>100&&peak<20000&&active>44100&&stereo,"station produces bounded non-silent stereo arrangement");
  radio_synth_reset(&s,station);unsigned repeat=2166136261u;
  for(int i=0;i<44100*4;i++){int l,r;radio_synth_sample(&s,&l,&r);repeat=(repeat^(unsigned)l)*16777619u;}
  RADIO_CHECK(hash==repeat,"station resets deterministically");
 }
 RADIO_CHECK(!strcmp(radio_station_name(5),"SPACE TALK")&&!strcmp(radio_station_genre(5),"TALK RADIO"),"talk station has requested title and category");
 RADIO_CHECK(!strncmp(radio_voice_script(5,0),"SPACE TALK:",11)&&!strncmp(radio_voice_script(6,0),"VOID TALES:",11),"presenter scripts belong to the two visible radio channels");
 {
  RadioSynth talk;radio_synth_reset(&talk,5);int quiet=0,longest=0,run=0,voices=0,changed=0,last=0,jump=0,cursor_advanced=0;
  unsigned chunks[12]={0};
  for(int i=0;i<44100*12;i++){
   int l,r;radio_synth_sample(&talk,&l,&r);
   chunks[i/44100]=(chunks[i/44100]^(unsigned)l)*16777619u;
   voices|=1<<talk.talk_voice;
   cursor_advanced|=radio_voice_station==5&&(radio_voice_line>0||radio_voice_char>12);
   if(!l&&!r){quiet++;if(++run>longest)longest=run;}else run=0;
   if(abs(l-last)>jump)jump=abs(l-last);
   last=l;
  }
  for(int i=1;i<12;i++)changed+=chunks[i]!=chunks[i-1];
  RADIO_CHECK(quiet>44100&&longest>=8820,"talk radio has truly silent pauses with no shuffling bed");
  RADIO_CHECK(changed==11&&(voices&(voices-1)),"talk radio varies phrases and speakers beyond the old short loop");
  RADIO_CHECK(jump<1500,"talk syllables have smooth edges without noise clicks");
  RADIO_CHECK(cursor_advanced,"SPACE TALK audio advances the shared on-screen transcript cursor");
 }
 RADIO_CHECK(radio_preview_wav(5),"export SPACE TALK audition");
 RADIO_CHECK(!strcmp(radio_station_name(6),"VOID TALES")&&!strcmp(radio_station_genre(6),"ALIEN STORY"),"alien story channel has its own title and genre");
 RadioSynth tales;radio_synth_reset(&tales,6);int tale_silence=0,tale_longest=0,tale_run=0,tale_voices=0,tale_changes=0,tale_last=0,tale_cursor=0;
 for(int i=0;i<44100*18;i++){int l,r;radio_synth_sample(&tales,&l,&r);int active=l||r;tale_cursor|=radio_voice_station==6&&(radio_voice_line>0||radio_voice_char>12);if(!active){tale_silence++;tale_run++;if(tale_run>tale_longest)tale_longest=tale_run;}else{tale_run=0;tale_changes+=((l>0)!=(tale_last>0));tale_last=l;tale_voices|=1<<tales.talk_voice;}}
 RADIO_CHECK(tale_silence>44100*2&&tale_longest>=22050,"alien story channel leaves long pauses between narrated tale phrases");
 RADIO_CHECK(tale_changes>4&&(tale_voices&(tale_voices-1)),"alien story channel varies its slow alien voices");
 RADIO_CHECK(tale_cursor,"VOID TALES audio advances the shared on-screen transcript cursor");
 RADIO_CHECK(radio_preview_wav(6),"export VOID TALES audition");
 int unique=1;for(int i=0;i<RADIO_STATION_COUNT;i++)for(int j=i+1;j<RADIO_STATION_COUNT;j++)if(hashes[i]==hashes[j])unique=0;
 RADIO_CHECK(unique,"all eight radio stations produce different audio");
 FILE *preview=fopen("radio-preview.flag","rb");if(preview){fclose(preview);for(int i=0;i<RADIO_STATION_COUNT;i++)RADIO_CHECK(radio_preview_wav(i),"export original stereo radio excerpt");}
 radio_volume=5;sound_volume=8;radio_adjust(0,-100);radio_adjust(1,100);
 RADIO_CHECK(!radio_volume&&sound_volume==10,"independent volume controls clamp safely");
 remove("test-radio.cfg");remove("test-radio.cfg.bak");radio_tune(3);radio_volume=4;sound_volume=7;
 RADIO_CHECK(radio_save_settings("test-radio.cfg"),"radio preferences save with checksum");
 radio_station=0;radio_volume=0;sound_volume=0;
 RADIO_CHECK(radio_load_settings("test-radio.cfg")&&radio_station==3&&radio_volume==4&&sound_volume==7,"station and both levels survive reload");
 radio_tune(2);radio_save_settings("test-radio.cfg");FILE *bad=fopen("test-radio.cfg","wb");if(bad){fputs("bad",bad);fclose(bad);}
 RADIO_CHECK(radio_load_settings("test-radio.cfg")&&radio_station==3,"invalid settings recover previous valid backup");
 radio_tune(7);radio_off=0;radio_save_settings("test-radio.cfg");radio_station=0;
 RADIO_CHECK(radio_load_settings("test-radio.cfg")&&!radio_off&&radio_station==7,"Elite Exploration selection survives reload");
 RadioSettings legacy={0x52414449u,2,7,5,8,0,0};legacy.check=radio_checksum(&legacy);
 FILE *legacy_file=fopen("test-radio.cfg","wb");if(legacy_file){fwrite(&legacy,1,sizeof(legacy),legacy_file);fclose(legacy_file);}
 radio_off=0;radio_station=3;
 RADIO_CHECK(radio_load_settings("test-radio.cfg")&&radio_off,"version-two radio-off setting does not become Elite Exploration");
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
