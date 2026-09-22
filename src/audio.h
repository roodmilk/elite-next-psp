/* Original procedural radio and SFX. Integer-only worker; silent-safe. */
#include <pspaudio.h>
#include <pspthreadman.h>
#include <pspmp3.h>
#include <psputility.h>
#include "radio-synth.h"
#include "radio-config.h"
#include "radio-files.h"
static volatile int audio_run=0,audio_sfx=SFX_NONE,audio_scene=0,audio_ch=-1;
static volatile int audio_duck=0;
static volatile int mp3_frozen=0;
static int audio_thread=-1;
#define AUDIO_FRAMES 2048
/* A large encoded reservoir avoids audible gaps when the Memory Stick pauses
 * for a filesystem read.  The output block supplies about 46 ms of headroom. */
static unsigned char mp3_stream_buf[64*1024] __attribute__((aligned(64)));
static unsigned char mp3_pcm_buf[16*(1152/2)] __attribute__((aligned(64)));
static int mp3_handle=-1,mp3_channels=2,mp3_rate=44100,mp3_frames=0,mp3_frame=0,mp3_phase=0,mp3_step=65536;static FILE *mp3_file=0;
static int mp3_lm1=0,mp3_rm1=0,mp3_l0=0,mp3_r0=0,mp3_l1=0,mp3_r1=0,mp3_l2=0,mp3_r2=0,mp3_resample_ready=0;
static int mp3_modules_loaded=0,mp3_resource_ready=0;
static short *mp3_pcm=0;static unsigned mp3_shuffle=0x7141u;
static void mp3_close_track(void){if(mp3_handle>=0)sceMp3ReleaseMp3Handle(mp3_handle);if(mp3_file)fclose(mp3_file);mp3_handle=-1;mp3_file=0;mp3_frames=mp3_frame=mp3_phase=0;mp3_resample_ready=0;mp3_pcm=0;radio_file_station=radio_file_track=-1;snprintf((char*)radio_file_title,sizeof(radio_file_title),"Generated broadcast");}
static int mp3_fill(void){unsigned char *dst;SceInt32 write,pos;if(mp3_handle<0||!mp3_file||sceMp3GetInfoToAddStreamData(mp3_handle,&dst,&write,&pos)<0)return 0;if(fseek(mp3_file,pos,SEEK_SET))return 0;int got=(int)fread(dst,1,write,mp3_file);if(got<=0)return 0;return sceMp3NotifyAddStreamData(mp3_handle,got)>=0;}
static int mp3_open_track(int station,int slot){
 mp3_close_track();if(station<0||station>=RADIO_STATION_COUNT||slot<0||slot>=radio_track_count[station])return 0;
 radio_file_error=1;mp3_file=fopen(radio_tracks[station][slot],"rb");if(!mp3_file)return 0;fseek(mp3_file,0,SEEK_END);long stream_end=ftell(mp3_file);fseek(mp3_file,0,SEEK_SET);
 SceMp3InitArg init;memset(&init,0,sizeof(init));init.mp3StreamStart=0;init.mp3StreamEnd=stream_end;init.mp3Buf=mp3_stream_buf;init.mp3BufSize=sizeof(mp3_stream_buf);init.pcmBuf=mp3_pcm_buf;init.pcmBufSize=sizeof(mp3_pcm_buf);
 radio_file_error=2;mp3_handle=sceMp3ReserveMp3Handle(&init);if(mp3_handle<0){mp3_close_track();return 0;}radio_file_error=3;if(!mp3_fill()){mp3_close_track();return 0;}radio_file_error=4;if(sceMp3Init(mp3_handle)<0){mp3_close_track();return 0;}
 radio_file_error=5;mp3_channels=sceMp3GetMp3ChannelNum(mp3_handle);mp3_rate=sceMp3GetSamplingRate(mp3_handle);if(mp3_channels<1||mp3_channels>2||mp3_rate<8000||mp3_rate>48000){mp3_close_track();return 0;}mp3_step=(int)(((long long)mp3_rate*65536)/44100);
 radio_file_error=0;radio_file_station=station;radio_file_track=slot;snprintf((char*)radio_file_title,sizeof(radio_file_title),"%.59s",radio_track_label(radio_tracks[station][slot]));return 1;
}
static int mp3_shuffle_track(int station){
 int count=radio_track_count[station];if(!count){mp3_close_track();return 0;}int previous=radio_file_station==station?radio_file_track:-1;
 for(int tries=0;tries<count;tries++){mp3_shuffle=mp3_shuffle*1664525u+1013904223u;int slot=(mp3_shuffle>>8)%count;if(count>1&&slot==previous)slot=(slot+1)%count;if(mp3_open_track(station,slot))return 1;}return 0;
}
static int mp3_decode_block(void){
 if(mp3_frozen||mp3_handle<0)return 0;
 if(sceMp3CheckStreamDataNeeded(mp3_handle)>0)mp3_fill();
 short *decoded=0;int bytes=sceMp3Decode(mp3_handle,&decoded);if(bytes<=0)return 0;
 mp3_pcm=decoded;mp3_frames=bytes/(2*mp3_channels);mp3_frame=0;return mp3_frames>0;
}
static int mp3_source_frame(int station,int *left,int *right){
 if(mp3_frozen||!radio_track_count[station])return 0;
 if(radio_file_station!=station&&!mp3_shuffle_track(station))return 0;
 if(mp3_frame>=mp3_frames&&!mp3_decode_block()){if(!mp3_shuffle_track(station)||!mp3_decode_block())return 0;}
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
 *left=mp3_cubic(mp3_lm1,mp3_l0,mp3_l1,mp3_l2,mp3_phase);*right=mp3_cubic(mp3_rm1,mp3_r0,mp3_r1,mp3_r2,mp3_phase);
 mp3_phase+=mp3_step;while(mp3_phase>=65536){mp3_phase-=65536;mp3_lm1=mp3_l0;mp3_rm1=mp3_r0;mp3_l0=mp3_l1;mp3_r0=mp3_r1;mp3_l1=mp3_l2;mp3_r1=mp3_r2;if(!mp3_source_frame(station,&mp3_l2,&mp3_r2)){mp3_resample_ready=0;break;}}
 return 1;
}
static int triangle_wave(unsigned phase){int x=(phase>>8)&255;return x<128?x-64:192-x;}
static int audio_worker(SceSize n,void *a){
 (void)n;(void)a;short buf[AUDIO_FRAMES*2] __attribute__((aligned(64)));unsigned phase=0,noise=0x7141u;int sfx_t=0,sfx_id=SFX_NONE,sfx_len=800;
 static RadioSynth synth;int station=radio_station,music_gain=0,effects_gain=sound_volume*100,duck_gain=1000;
 radio_synth_reset(&synth,station);
 while(audio_run){
  if(audio_sfx){
   sfx_id=audio_sfx;audio_sfx=SFX_NONE;
   if(sfx_id==SFX_SELECT)sfx_len=140;
   else if(sfx_id==SFX_UI)sfx_len=280;
   else if(sfx_id==SFX_COMM)sfx_len=640;
   else if(sfx_id==SFX_LASER)sfx_len=420;
   else if(sfx_id==SFX_HIT)sfx_len=520;
   else if(sfx_id==SFX_SCAN)sfx_len=720;
   else if(sfx_id==SFX_LAND)sfx_len=900;
   else if(sfx_id==SFX_DOCK)sfx_len=760;
   else if(sfx_id==SFX_MISSILE)sfx_len=980;
   else if(sfx_id==SFX_ALERT)sfx_len=1400;
   else if(sfx_id==SFX_WARP)sfx_len=1500;
   else if(sfx_id==SFX_DEATH)sfx_len=1700;
   else if(sfx_id==SFX_BOOST)sfx_len=500;
   else sfx_len=700;
   sfx_t=sfx_len;
  }
  for(int i=0;i<AUDIO_FRAMES;i++){
   int music_l=0,music_r=0;
   int tuning=radio_static_ms>0;
   /* Static must stay audible while retuning; only mute when OFF or crossfading. */
   int wanted=radio_off?0:(tuning||radio_station==station)?radio_volume*100:0;
   music_gain+=(music_gain<wanted)-(music_gain>wanted);
   int effect_target=sound_volume*100;effects_gain+=(effects_gain<effect_target)-(effects_gain>effect_target);
   int duck_target=(audio_scene==2||(sfx_id==SFX_ALERT&&sfx_t>0))?450:audio_duck?750:1000;
   duck_gain+=(duck_gain<duck_target)-(duck_gain>duck_target);
   if(!music_gain&&radio_station!=station){station=radio_station;radio_synth_reset(&synth,station);}
   if(!radio_off&&!tuning){if(!mp3_sample(station,&music_l,&music_r))radio_synth_sample(&synth,&music_l,&music_r);}
   if(tuning){noise=noise*1664525u+1013904223u;int crackle=((int)((noise>>24)&255)-128)*22;music_l=crackle;music_r=((int)((noise>>16)&255)-128)*18;}
   int sfx=0;if(sfx_t>0){
    noise=noise*1664525u+1013904223u;int wave=triangle_wave(phase*13),e=sfx_t,den=sfx_len>0?sfx_len:1;
    if(sfx_id==SFX_SELECT)sfx=triangle_wave(phase*21)*52*e/den;
    else if(sfx_id==SFX_UI)sfx=wave*40*e/den;
    else if(sfx_id==SFX_COMM)sfx=triangle_wave(phase*(e>320?11:7))*42*e/den;
    else if(sfx_id==SFX_LASER)sfx=wave*50*e/den;
    else if(sfx_id==SFX_HIT)sfx=((int)((noise>>24)&255)-128)*24*e/den;
    else if(sfx_id==SFX_WARP)sfx=triangle_wave(phase*(3+(den-e)/90))*44*e/den;
    else if(sfx_id==SFX_SCAN)sfx=triangle_wave(phase*17)*36*e/den;
    else if(sfx_id==SFX_LAND)sfx=triangle_wave(phase*4)*44*e/den;
    else if(sfx_id==SFX_MINE)sfx=((int)((noise>>25)&127)-64)*36*e/den;
    else if(sfx_id==SFX_BOOST)sfx=triangle_wave(phase*6)*30*e/den;
    else if(sfx_id==SFX_DOCK)sfx=triangle_wave(phase*3)*34*e/den+triangle_wave(phase*15)*16*(e<220?e:220)/den;
    else if(sfx_id==SFX_MISSILE)sfx=triangle_wave(phase*(4+(den-e)/70))*42*e/den;
    else if(sfx_id==SFX_ALERT)sfx=((e/70)&1)?triangle_wave(phase*15)*50*e/den:0;
    else if(sfx_id==SFX_DEATH)sfx=((int)((noise>>24)&255)-128)*30*e/den;
    sfx_t--;
   }
   sfx=sfx*effects_gain/1000;
   int left=(music_l*music_gain/1000)*duck_gain/1000+sfx;
   int right=(music_r*music_gain/1000)*duck_gain/1000+sfx;
   if(left>30000)left=30000;
   if(left<-30000)left=-30000;
   if(right>30000)right=30000;
   if(right<-30000)right=-30000;
   buf[i*2]=(short)left;buf[i*2+1]=(short)right;
   phase++;
  }
  if(radio_static_ms>0)radio_static_ms--;
  if(sceAudioOutputBlocking(audio_ch,PSP_AUDIO_VOLUME_MAX/3,buf)<0){audio_run=0;break;}
 }
 return 0;
}
static void audio_init(void){
 mp3_frozen=0;
 radio_load_settings("radio.cfg");
 radio_scan_music();int custom=0;for(int i=0;i<RADIO_STATION_COUNT;i++)custom+=radio_track_count[i];if(custom){if(!mp3_modules_loaded){if(sceUtilityLoadModule(PSP_MODULE_AV_AVCODEC)>=0&&sceUtilityLoadModule(PSP_MODULE_AV_MP3)>=0)mp3_modules_loaded=1;}if(mp3_modules_loaded&&!mp3_resource_ready&&sceMp3InitResource()>=0)mp3_resource_ready=1;if(!mp3_resource_ready)for(int i=0;i<RADIO_STATION_COUNT;i++)radio_track_count[i]=0;}
 audio_ch=sceAudioChReserve(PSP_AUDIO_NEXT_CHANNEL,AUDIO_FRAMES,PSP_AUDIO_FORMAT_STEREO);if(audio_ch<0){audio_run=0;return;}
 audio_run=1;audio_thread=sceKernelCreateThread("audio",audio_worker,0x1a,0x4000,0,0);
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
