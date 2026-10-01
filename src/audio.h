/* Procedural radio, Vorbis/MP3 music and SFX; one worker owns the decoders. */
#include <pspaudio.h>
#include <pspthreadman.h>
#include <pspmp3.h>
#include <psputility.h>
#include <vorbis/vorbisfile.h>
#include "radio-synth.h"
#include "radio-config.h"
#include "radio-files.h"
#include "radio-playlist.h"
#include "audio-sfx.h"
#include "local-tv-audio.h"
static volatile int audio_run=0,audio_sfx=SFX_NONE,audio_scene=0,audio_ch=-1;
/* Set by the surface renderer: (biome << 2) | weather.  The audio worker
 * deliberately needs no planet/render headers, which keeps this ambience
 * independent from surface controls and targeting. */
static volatile int audio_surface_ambient=-1;
static volatile int audio_duck=0,audio_station_room=-1,audio_station_juke=1,audio_battle=0;
static volatile int mp3_frozen=0;
static int audio_thread=-1;
#define AUDIO_FRAMES 2048
/* Alternate buffers: the device can still be reading the submitted block. */
static short audio_output[2][AUDIO_FRAMES*2] __attribute__((aligned(64)));
static unsigned char music_bad[RADIO_FILE_SOURCE_COUNT][RADIO_TRACKS_PER_STATION];
/* A large encoded reservoir avoids audible gaps when the Memory Stick pauses
 * for a filesystem read.  The output block supplies about 46 ms of headroom. */
static unsigned char mp3_stream_buf[64*1024] __attribute__((aligned(64)));
static unsigned char mp3_pcm_buf[16*(1152/2)] __attribute__((aligned(64)));
static short ogg_pcm_buf[AUDIO_FRAMES*2] __attribute__((aligned(64)));
static int mp3_handle=-1,mp3_channels=2,mp3_rate=44100,mp3_frames=0,mp3_frame=0,mp3_phase=0,mp3_step=44100;static FILE *mp3_file=0;
static int mp3_lm1=0,mp3_rm1=0,mp3_l0=0,mp3_r0=0,mp3_l1=0,mp3_r1=0,mp3_l2=0,mp3_r2=0,mp3_resample_ready=0;
static int mp3_modules_loaded=0,mp3_resource_ready=0;
static short *mp3_pcm=0;static unsigned mp3_shuffle=0x7141u;
static OggVorbis_File ogg_file;static int ogg_open=0,music_format=0;
static void mp3_close_track(void){if(mp3_handle>=0)sceMp3ReleaseMp3Handle(mp3_handle);if(mp3_file)fclose(mp3_file);if(ogg_open)ov_clear(&ogg_file);mp3_handle=-1;mp3_file=0;ogg_open=0;music_format=0;mp3_frames=mp3_frame=mp3_phase=0;mp3_resample_ready=0;mp3_pcm=0;radio_file_station=radio_file_track=-1;snprintf((char*)radio_file_title,sizeof(radio_file_title),"Generated broadcast");}
static int mp3_fill(void){unsigned char *dst;SceInt32 write,pos;if(mp3_handle<0||!mp3_file||sceMp3GetInfoToAddStreamData(mp3_handle,&dst,&write,&pos)<0)return 0;if(fseek(mp3_file,pos,SEEK_SET))return 0;int got=(int)fread(dst,1,write,mp3_file);if(got<=0)return 0;return sceMp3NotifyAddStreamData(mp3_handle,got)>=0;}
static int mp3_open_track(int station,int slot){
 mp3_close_track();if(station<0||station>=RADIO_FILE_SOURCE_COUNT||slot<0||slot>=radio_track_count[station])return 0;
 if(radio_has_ogg_extension(radio_tracks[station][slot])){
  radio_file_error=10;if(ov_fopen(radio_tracks[station][slot],&ogg_file)<0)return 0;ogg_open=1;
  vorbis_info *info=ov_info(&ogg_file,-1);if(!info||info->channels<1||info->channels>2||info->rate<8000||info->rate>48000){mp3_close_track();return 0;}
  mp3_channels=info->channels;mp3_rate=info->rate;mp3_step=mp3_rate;music_format=2;
  radio_file_error=0;radio_file_station=station;radio_file_track=slot;snprintf((char*)radio_file_title,sizeof(radio_file_title),"%.59s",radio_track_label(radio_tracks[station][slot]));return 1;
 }
 if(!radio_has_mp3_extension(radio_tracks[station][slot])||!mp3_resource_ready)return 0;
 radio_file_error=1;mp3_file=fopen(radio_tracks[station][slot],"rb");if(!mp3_file)return 0;fseek(mp3_file,0,SEEK_END);long stream_end=ftell(mp3_file);fseek(mp3_file,0,SEEK_SET);
 SceMp3InitArg init;memset(&init,0,sizeof(init));init.mp3StreamStart=0;init.mp3StreamEnd=stream_end;init.mp3Buf=mp3_stream_buf;init.mp3BufSize=sizeof(mp3_stream_buf);init.pcmBuf=mp3_pcm_buf;init.pcmBufSize=sizeof(mp3_pcm_buf);
 radio_file_error=2;mp3_handle=sceMp3ReserveMp3Handle(&init);if(mp3_handle<0){mp3_close_track();return 0;}radio_file_error=3;if(!mp3_fill()){mp3_close_track();return 0;}radio_file_error=4;if(sceMp3Init(mp3_handle)<0){mp3_close_track();return 0;}
 radio_file_error=5;mp3_channels=sceMp3GetMp3ChannelNum(mp3_handle);mp3_rate=sceMp3GetSamplingRate(mp3_handle);if(mp3_channels<1||mp3_channels>2||mp3_rate<8000||mp3_rate>48000){mp3_close_track();return 0;}mp3_step=mp3_rate;
 music_format=1;radio_file_error=0;radio_file_station=station;radio_file_track=slot;snprintf((char*)radio_file_title,sizeof(radio_file_title),"%.59s",radio_track_label(radio_tracks[station][slot]));return 1;
}
static int mp3_shuffle_track(int station){
 int count=radio_track_count[station];if(!count){mp3_close_track();return 0;}int previous=radio_file_station==station?radio_file_track:-1;
 int order[RADIO_TRACKS_PER_STATION];int candidates=radio_candidate_order(count,previous,&mp3_shuffle,order);
 for(int i=0;i<candidates;i++)if(!music_bad[station][order[i]]){if(mp3_open_track(station,order[i]))return 1;music_bad[station][order[i]]=1;}
 return 0;
}
static int mp3_decode_block(void){
 if(mp3_frozen)return 0;
 if(music_format==2){
  int section=0,attempts=0;long bytes;
  do{bytes=ov_read(&ogg_file,(char*)ogg_pcm_buf,sizeof(ogg_pcm_buf),0,2,1,&section);}while(bytes<0&&++attempts<4);
  if(bytes<=0){if(bytes<0&&radio_file_station>=0)music_bad[radio_file_station][radio_file_track]=1;return 0;}
  vorbis_info *info=ov_info(&ogg_file,section);
  if(!info||info->channels<1||info->channels>2||info->rate<8000||info->rate>48000){music_bad[radio_file_station][radio_file_track]=1;return 0;}
  mp3_channels=info->channels;mp3_rate=info->rate;mp3_step=mp3_rate;
  mp3_pcm=ogg_pcm_buf;mp3_frames=(int)bytes/(2*mp3_channels);mp3_frame=0;return mp3_frames>0;
 }
 if(music_format!=1||mp3_handle<0)return 0;
 if(sceMp3CheckStreamDataNeeded(mp3_handle)>0)mp3_fill();
 short *decoded=0;int bytes=sceMp3Decode(mp3_handle,&decoded);if(bytes<=0)return 0;
 mp3_pcm=decoded;mp3_frames=bytes/(2*mp3_channels);mp3_frame=0;return mp3_frames>0;
}
static int mp3_source_frame(int station,int *left,int *right){
 if(mp3_frozen||!radio_track_count[station])return 0;
 if(radio_file_station!=station&&!mp3_shuffle_track(station))return 0;
 if(mp3_frame>=mp3_frames&&!mp3_decode_block()){if(!mp3_shuffle_track(station))return 0;if(!mp3_decode_block()){music_bad[station][radio_file_track]=1;mp3_close_track();return 0;}}
 int i=mp3_frame*mp3_channels;*left=mp3_pcm[i];*right=mp3_channels==2?mp3_pcm[i+1]:mp3_pcm[i];
 mp3_frame++;return 1;
}
static int mp3_cubic(int xm1,int x0,int x1,int x2,int t){
 /* Catmull-Rom interpolation in Q16.  It is smoother than linear conversion
  * for 32/48 kHz user tracks and 64-bit intermediates prevent wrap noise. */
 long long a=-xm1+3LL*x0-3LL*x1+x2,b=2LL*xm1-5LL*x0+4LL*x1-x2,c=-xm1+x1;
 long long y=x0+((((((a*t)>>16)+b)*t>>16)+c)*t>>17);
 if(y>32767)y=32767;
 if(y<-32768)y=-32768;
 return (int)y;
}
static int mp3_sample(int station,int *left,int *right){
 if(mp3_frozen||!radio_track_count[station])return 0;
 if(radio_file_station!=station){if(!mp3_shuffle_track(station))return 0;mp3_resample_ready=0;}
 if(!mp3_resample_ready){if(!mp3_source_frame(station,&mp3_l0,&mp3_r0)||!mp3_source_frame(station,&mp3_l1,&mp3_r1)||!mp3_source_frame(station,&mp3_l2,&mp3_r2))return 0;mp3_lm1=mp3_l0;mp3_rm1=mp3_r0;mp3_phase=0;mp3_resample_ready=1;}
 *left=mp3_cubic(mp3_lm1,mp3_l0,mp3_l1,mp3_l2,(int)((unsigned)mp3_phase*65536u/44100u));*right=mp3_cubic(mp3_rm1,mp3_r0,mp3_r1,mp3_r2,(int)((unsigned)mp3_phase*65536u/44100u));
 mp3_phase+=mp3_step;while(mp3_phase>=44100){mp3_phase-=44100;mp3_lm1=mp3_l0;mp3_rm1=mp3_r0;mp3_l0=mp3_l1;mp3_r0=mp3_r1;mp3_l1=mp3_l2;mp3_r1=mp3_r2;if(!mp3_source_frame(station,&mp3_l2,&mp3_r2)){mp3_resample_ready=0;break;}}
 return 1;
}
static int audio_worker(SceSize n,void *a){
 (void)n;(void)a;int output_index=0;unsigned phase=0,noise=0x7141u;int sfx_t=0,sfx_id=SFX_NONE,sfx_len=800;
 static RadioSynth synth,station_synth;unsigned station_phase=0,station_noise=0x7911u;int station_gain=0;
 LocalTVVoice tv_voice={0};
 unsigned surface_noise=0x5a17u,surface_phase=0;int surface_filter=0,surface_clock=0;
 radio_synth_reset(&station_synth,0);int station=radio_station,music_gain=0,effects_gain=sound_volume*100,duck_gain=1000;
 radio_synth_reset(&synth,station);
 while(audio_run){
  short *buf=audio_output[output_index];
  if(audio_sfx){
   sfx_id=audio_sfx;audio_sfx=SFX_NONE;
   sfx_len=audio_sfx_length(sfx_id);phase=0;
   sfx_t=sfx_len;
  }
  for(int i=0;i<AUDIO_FRAMES;i++){
   int music_l=0,music_r=0;
   int tuning=radio_static_ms>0&&!audio_battle&&!audio_tv_on;
   /* Battle score is not a radio station: it overrides radio power/tuning but
    * still obeys the player's MUSIC level.  Source changes crossfade through
    * zero so the MP3 decoder never tears between files. */
   int wanted_source=audio_battle?RADIO_BATTLE_SOURCE:radio_station;
   int score_on=(audio_battle||!radio_off)&&!audio_tv_on;
   int wanted=score_on&&(!tuning&&wanted_source==station)?radio_volume*100:0;
   music_gain+=(music_gain<wanted)-(music_gain>wanted);
   int effect_target=sound_volume*100;effects_gain+=(effects_gain<effect_target)-(effects_gain>effect_target);
   int duck_target=audio_battle?(sfx_t>0?760:1000):(audio_scene==2||(sfx_id==SFX_ALERT&&sfx_t>0))?450:audio_duck?750:1000;
   duck_gain+=(duck_gain<duck_target)-(duck_gain>duck_target);
   if(!music_gain&&wanted_source!=station){station=wanted_source;radio_synth_reset(&synth,station==RADIO_BATTLE_SOURCE?2:station);}
   if((score_on||music_gain)&&!tuning){
    /* The presenter channels are generated from their visible scripts, so an
     * unrelated music file must never replace the voice and desynchronise it. */
    if(station==5||station==6)radio_synth_sample(&synth,&music_l,&music_r);
    else if(!mp3_sample(station,&music_l,&music_r)){if(station!=RADIO_BATTLE_SOURCE)radio_synth_sample(&synth,&music_l,&music_r);}
   }
   if(tuning){noise=noise*1664525u+1013904223u;int crackle=((int)((noise>>24)&255)-128)*22;music_l=crackle;music_r=((int)((noise>>16)&255)-128)*18;}
   int sfx=0;if(sfx_t>0){
    sfx=audio_sfx_sample(sfx_id,sfx_len-sfx_t,&phase,&noise);
    sfx_t--;
   }
   sfx+=local_tv_voice_sample(&tv_voice,audio_tv_on?audio_tv_code:0);
   int room=audio_station_room;
   int ambient_target=room>=0?200:0;station_gain+=(station_gain<ambient_target)-(station_gain>ambient_target);
   if(station_gain){
    station_phase+=radio_note_increment(room==3?32:room==5?48:36);
    station_noise=station_noise*1664525u+1013904223u;
    int hum=((int)(station_phase>>24)-128)/2+((int)(station_noise>>27)-16);
    sfx+=hum*station_gain/100;
   }
   /* Lightweight procedural surface sound: filtered wind with restrained
    * biome accents.  It has no files to stream and shares the SFX volume. */
   int surface_code=audio_surface_ambient;
   if(audio_scene==1&&surface_code>=0){
    int surface_biome=surface_code>>2,weather=surface_code&3;
    surface_noise=surface_noise*1664525u+1013904223u;
    int raw=((int)(surface_noise>>24)&255)-128;
    int smooth=weather==3?20:(surface_biome==5?28:48);
    surface_filter+=(raw*10-surface_filter)/smooth;
    int surface_mix=surface_filter*(weather==3?5:3)/10;
    surface_clock++;surface_phase+=radio_note_increment(surface_biome==4?76:surface_biome==0?48:34);
    /* Forest and ocean life calls are infrequent, soft and short. */
    int call_pos=surface_clock%220500;
    if((surface_biome==4||surface_biome==0)&&call_pos<1700){
     int envelope=call_pos<250?call_pos:(1700-call_pos)/2;
     int call=((int)(surface_phase>>24)-128)*envelope/900;
     surface_mix+=call;
    }
    /* Volcanic ground receives occasional dry crackle, never a loud pop. */
    if(surface_biome==3&&((surface_noise>>27)&31)==0)surface_mix+=raw/2;
    sfx+=surface_mix;
   }else surface_filter-=surface_filter/32;
   int station_music=0;
   if(room==2&&audio_station_juke&&radio_off){int l,r;radio_synth_sample(&station_synth,&l,&r);station_music=(l+r)*radio_volume/50;}
   sfx=sfx*effects_gain/1000+station_music*duck_gain/1000;
   int left=(music_l*music_gain/1000)*duck_gain/1000+sfx;
   int right=(music_r*music_gain/1000)*duck_gain/1000+sfx;
   if(left>30000)left=30000;
   if(left<-30000)left=-30000;
   if(right>30000)right=30000;
   if(right<-30000)right=-30000;
   buf[i*2]=(short)left;buf[i*2+1]=(short)right;
  }
  if(radio_static_ms>0)radio_static_ms--;
  if(sceAudioOutputBlocking(audio_ch,PSP_AUDIO_VOLUME_MAX/3,buf)<0){audio_run=0;break;}
  output_index^=1;
 }
 return 0;
}
static void audio_init(void){
 FILE *silent_test=fopen("silent-test.flag","r");if(silent_test){fclose(silent_test);return;}
 mp3_frozen=0;
 memset(music_bad,0,sizeof(music_bad));
 radio_load_settings("radio.cfg");
 radio_scan_music();int has_mp3=0;for(int i=0;i<RADIO_FILE_SOURCE_COUNT;i++)for(int j=0;j<radio_track_count[i];j++)has_mp3|=radio_has_mp3_extension(radio_tracks[i][j]);if(has_mp3){if(!mp3_modules_loaded){if(sceUtilityLoadModule(PSP_MODULE_AV_AVCODEC)>=0&&sceUtilityLoadModule(PSP_MODULE_AV_MP3)>=0)mp3_modules_loaded=1;}if(mp3_modules_loaded&&!mp3_resource_ready&&sceMp3InitResource()>=0)mp3_resource_ready=1;}
 audio_ch=sceAudioChReserve(PSP_AUDIO_NEXT_CHANNEL,AUDIO_FRAMES,PSP_AUDIO_FORMAT_STEREO);if(audio_ch<0){audio_run=0;return;}
 audio_run=1;audio_thread=sceKernelCreateThread("audio",audio_worker,0x1a,0x10000,THREAD_ATTR_VFPU,0);
 if(audio_thread<0||sceKernelStartThread(audio_thread,0,0)<0){audio_run=0;if(audio_thread>=0)sceKernelDeleteThread(audio_thread);audio_thread=-1;sceAudioChRelease(audio_ch);audio_ch=-1;}
}
static void audio_play(int id){if(id)audio_sfx=id;}
static void audio_scene_set(int scene){audio_scene=scene;}
/* Signal the worker to leave MP3/file I/O before the PSP finishes sleeping.
 * Safe from the power callback: no waits, no frees. */
static void audio_prepare_suspend(void){mp3_frozen=1;audio_run=0;}
static void audio_stop(void){
 mp3_frozen=1;audio_run=0;
 if(audio_thread>=0){SceUInt timeout=100000;if(sceKernelWaitThreadEnd(audio_thread,&timeout)<0)sceKernelTerminateDeleteThread(audio_thread);else sceKernelDeleteThread(audio_thread);audio_thread=-1;}
 if(audio_ch>=0)sceAudioChRelease(audio_ch);
 audio_ch=-1;
 mp3_close_track();if(mp3_resource_ready){sceMp3TermResource();mp3_resource_ready=0;}
 if(radio_dirty)radio_save_settings("radio.cfg");
 mp3_frozen=0;
}
