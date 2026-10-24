/* Procedural radio, Vorbis/MP3 music and SFX; one worker owns the decoders. */
#include <pspaudio.h>
#include <pspthreadman.h>
#include <pspmp3.h>
#include <psputility.h>
#include <vorbis/vorbisfile.h>
#include "radio-synth.h"
#include "radio-config.h"
#include "audio-intro.h"
#include "radio-files.h"
#include "radio-playlist.h"
#include "generated/audio-flight-pcm.h"
#include "generated/audio-surface-pcm.h"
#include "audio-sfx.h"
#include "audio-mixer.h"
#include "local-tv-audio.h"
static volatile int audio_run=0,audio_sfx=SFX_NONE,audio_scene=0,audio_ch=-1; /* audio_sfx remains an observable last-cue receipt for review code. */
/* Set by the surface renderer: (biome << 2) | weather.  The audio worker
 * deliberately needs no planet/render headers, which keeps this ambience
 * independent from surface controls and targeting. */
static volatile int audio_surface_ambient=-1;
static volatile int audio_duck=0,audio_station_room=-1,audio_station_juke=1,audio_battle=0;
/* Quantised on the game thread and read by the audio worker.  No allocation,
 * locks or game-state pointer crosses the real-time boundary. */
static volatile int audio_flight_active=0,audio_flight_speed=0,audio_flight_turn=0,audio_flight_boost=0,audio_flight_hull=1000,audio_flight_thrust=0,audio_flight_accel=0;
static volatile int audio_traffic_active=0,audio_traffic_distance=6000,audio_traffic_pan=0,audio_traffic_speed=0,audio_traffic_kind=0,audio_traffic_character=0,audio_traffic_blocked=0,audio_traffic_relative=0;
static volatile int audio_approach_active=0,audio_approach_distance=20000,audio_approach_pan=0,audio_dock_stage=0,audio_dock_phase=0,audio_is_docked=0;
static volatile int audio_jump_remaining_ms=0;
#ifdef ELITE_ENABLE_REVIEW
static volatile unsigned audio_jump_review_flags=0;
#endif
static volatile int mp3_frozen=0;
static int audio_thread=-1,audio_device_thread=-1;
static int audio_free_sem=-1,audio_ready_sem=-1,audio_start_sem=-1;
static void audio_stop(void);
/* Measured at the device boundary, rather than counting SFX queue drops. */
static volatile unsigned audio_blocks=0,audio_buffer_starves=0,audio_mix_worst_us=0;
static int audio_race_key_cue(int cue){return cue==SFX_SCAN||cue==SFX_MISSILE||cue==SFX_ALERT||cue==SFX_DEATH||cue==SFX_BOOST||cue==SFX_SPEED_SURGE;}
/* Same total PCM safety window, smaller CPU bursts on the PSP-3000. */
#ifdef ELITE_PSP3000
#define AUDIO_FRAMES 512
#define AUDIO_QUEUE_BLOCKS 20
#else
#define AUDIO_FRAMES 2048
#define AUDIO_QUEUE_BLOCKS 5
#endif
static short audio_output[AUDIO_QUEUE_BLOCKS][AUDIO_FRAMES*2] __attribute__((aligned(64)));
static unsigned char music_bad[RADIO_FILE_SOURCE_COUNT][RADIO_TRACKS_PER_STATION];
/* A large encoded reservoir avoids audible gaps when the Memory Stick pauses
 * for a filesystem read. The bounded PCM queue holds about 232 ms in either profile. */
static unsigned char mp3_stream_buf[48*1024] __attribute__((aligned(64)));
static unsigned char mp3_pcm_buf[16*(1152/2)] __attribute__((aligned(64)));
static short ogg_pcm_buf[AUDIO_FRAMES*2] __attribute__((aligned(64)));
static int mp3_handle=-1,mp3_channels=2,mp3_rate=44100,mp3_frames=0,mp3_frame=0,mp3_phase=0,mp3_step=44100;static FILE *mp3_file=0;
static int mp3_lm1=0,mp3_rm1=0,mp3_l0=0,mp3_r0=0,mp3_l1=0,mp3_r1=0,mp3_l2=0,mp3_r2=0,mp3_resample_ready=0;
static int mp3_modules_loaded=0,mp3_resource_ready=0;
static short *mp3_pcm=0;static unsigned mp3_shuffle=0x7141u;
static OggVorbis_File ogg_file;static int ogg_open=0,music_format=0;
static int audio_first_start=1,audio_intro_seek_pending=0;
static volatile int audio_replay_intro=0;
static volatile int audio_title_active=1; /* Intro score ignores saved station/power. */
static volatile int audio_intro_skip_remaining=0,audio_intro_skipped_samples=0,audio_intro_source_rate=0;
static volatile unsigned audio_intro_fade_frame=AUDIO_INTRO_FADE_FRAMES;
static void audio_intro_track_opened(int station){
 if(audio_intro_seek_pending&&station==AUDIO_INTRO_STATION){
  audio_intro_seek_pending=0;audio_intro_source_rate=mp3_rate;
  audio_intro_skip_remaining=mp3_rate*AUDIO_INTRO_SKIP_SECONDS;
 }
}
#include "audio-mp3-prefetch.h"
static void mp3_close_track(void){music_read_close();if(mp3_handle>=0)sceMp3ReleaseMp3Handle(mp3_handle);if(mp3_file)fclose(mp3_file);if(ogg_open)ov_clear(&ogg_file);mp3_handle=-1;mp3_file=0;ogg_open=0;music_format=0;mp3_frames=mp3_frame=mp3_phase=0;mp3_resample_ready=0;mp3_pcm=0;audio_intro_skip_remaining=0;radio_file_station=radio_file_track=-1;snprintf((char*)radio_file_title,sizeof(radio_file_title),"Generated broadcast");}
static int mp3_fill(void){unsigned char *dst;SceInt32 write,pos;if(mp3_handle<0||!mp3_file||sceMp3GetInfoToAddStreamData(mp3_handle,&dst,&write,&pos)<0)return 0;int got;if(music_read_fd>=0)got=music_read_copy(dst,write,pos);else{if(fseek(mp3_file,pos,SEEK_SET))return 0;got=(int)fread(dst,1,write,mp3_file);}if(got<=0)return 0;return sceMp3NotifyAddStreamData(mp3_handle,got)>=0;}
static int mp3_open_track(int station,int slot){
 mp3_close_track();if(station<0||station>=RADIO_FILE_SOURCE_COUNT||slot<0||slot>=radio_track_count[station])return 0;
 if(radio_has_ogg_extension(radio_tracks[station][slot])){
  radio_file_error=10;if(ov_fopen(radio_tracks[station][slot],&ogg_file)<0)return 0;ogg_open=1;
  vorbis_info *info=ov_info(&ogg_file,-1);if(!info||info->channels<1||info->channels>2||info->rate<8000||info->rate>48000){mp3_close_track();return 0;}
  mp3_channels=info->channels;mp3_rate=info->rate;mp3_step=mp3_rate;music_format=2;
  radio_file_error=0;radio_file_station=station;radio_file_track=slot;snprintf((char*)radio_file_title,sizeof(radio_file_title),"%.59s",radio_track_label(radio_tracks[station][slot]));audio_intro_track_opened(station);return 1;
 }
 if(!radio_has_mp3_extension(radio_tracks[station][slot])||!mp3_resource_ready)return 0;
 radio_file_error=1;mp3_file=fopen(radio_tracks[station][slot],"rb");if(!mp3_file)return 0;fseek(mp3_file,0,SEEK_END);long stream_end=ftell(mp3_file);fseek(mp3_file,0,SEEK_SET);
 /* Large ID3v2 cover-art tags exceed the PSP decoder's encoded reservoir.
  * Strip metadata at the stream boundary, preserving all MPEG audio frames. */
 unsigned char tag[10];long stream_start=0;if(fread(tag,1,10,mp3_file)==10&&!memcmp(tag,"ID3",3)&&!(tag[6]&128)&&!(tag[7]&128)&&!(tag[8]&128)&&!(tag[9]&128)){
  stream_start=10L+((long)tag[6]<<21)+((long)tag[7]<<14)+((long)tag[8]<<7)+tag[9];if(tag[3]==4&&(tag[5]&16))stream_start+=10;
  if(stream_start>=stream_end){mp3_close_track();return 0;}
 }fseek(mp3_file,stream_start,SEEK_SET);
 SceMp3InitArg init;memset(&init,0,sizeof(init));init.mp3StreamStart=stream_start;init.mp3StreamEnd=stream_end;init.mp3Buf=mp3_stream_buf;init.mp3BufSize=sizeof(mp3_stream_buf);init.pcmBuf=mp3_pcm_buf;init.pcmBufSize=sizeof(mp3_pcm_buf);
 music_read_open(radio_tracks[station][slot],stream_start,stream_end);
 radio_file_error=2;mp3_handle=sceMp3ReserveMp3Handle(&init);if(mp3_handle<0){mp3_close_track();return 0;}radio_file_error=3;if(!mp3_fill()){mp3_close_track();return 0;}radio_file_error=4;if(sceMp3Init(mp3_handle)<0){mp3_close_track();return 0;}
 sceMp3SetLoopNum(mp3_handle,0); /* EOF advances the playlist once. */
 radio_file_error=5;mp3_channels=sceMp3GetMp3ChannelNum(mp3_handle);mp3_rate=sceMp3GetSamplingRate(mp3_handle);if(mp3_channels<1||mp3_channels>2||mp3_rate<8000||mp3_rate>48000){mp3_close_track();return 0;}mp3_step=mp3_rate;
 music_format=1;radio_file_error=0;radio_file_station=station;radio_file_track=slot;snprintf((char*)radio_file_title,sizeof(radio_file_title),"%.59s",radio_track_label(radio_tracks[station][slot]));audio_intro_track_opened(station);return 1;
}
static int mp3_shuffle_track(int station){
 int count=radio_track_count[station];if(!count){mp3_close_track();return 0;}int previous=radio_file_station==station?radio_file_track:-1;
 int order[RADIO_TRACKS_PER_STATION];int candidates=radio_candidate_order(count,previous,&mp3_shuffle,order);
 /* This is the player's named race theme. Prefer it at each race entry; keep
  * normal playlist alternatives if it is missing or cannot be decoded. */
 if(station==8&&radio_race_on&&previous<0)for(int i=0;i<candidates;i++)if(!strcasecmp(radio_track_label(radio_tracks[station][order[i]]),"Secret Agent Rock.mp3")){int swap=order[0];order[0]=order[i];order[i]=swap;break;}
 for(int i=0;i<candidates;i++)if(!music_bad[station][order[i]]){if(mp3_open_track(station,order[i]))return 1;music_bad[station][order[i]]=1;}
 return 0;
}
static int mp3_decode_raw_block(void){
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
static int mp3_decode_block(void){
 /* Decode/discard preserves MP3 bit-reservoir state and works with VBR and
  * Vorbis too. No real-time wait or guessed compressed-byte offset. The
  * lower-priority worker performs this once, before submitting audible PCM. */
 for(;;){
  if(mp3_frozen||!mp3_decode_raw_block())return 0;
  int skip=audio_intro_skip_remaining;
  if(skip>mp3_frames)skip=mp3_frames;
  mp3_frame=skip;audio_intro_skip_remaining-=skip;audio_intro_skipped_samples+=skip;
  if(mp3_frame<mp3_frames)return 1;
 }
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
 /* Native-rate music needs no cubic resampling or 64-bit multiplies. */
 if(mp3_step==44100){mp3_resample_ready=0;return mp3_source_frame(station,left,right);}
 if(!mp3_resample_ready){if(!mp3_source_frame(station,&mp3_l0,&mp3_r0)||!mp3_source_frame(station,&mp3_l1,&mp3_r1)||!mp3_source_frame(station,&mp3_l2,&mp3_r2))return 0;mp3_lm1=mp3_l0;mp3_rm1=mp3_r0;mp3_phase=0;mp3_resample_ready=1;}
 *left=mp3_cubic(mp3_lm1,mp3_l0,mp3_l1,mp3_l2,(int)((unsigned)mp3_phase*65536u/44100u));*right=mp3_cubic(mp3_rm1,mp3_r0,mp3_r1,mp3_r2,(int)((unsigned)mp3_phase*65536u/44100u));
 mp3_phase+=mp3_step;while(mp3_phase>=44100){mp3_phase-=44100;mp3_lm1=mp3_l0;mp3_rm1=mp3_r0;mp3_l0=mp3_l1;mp3_r0=mp3_r1;mp3_l1=mp3_l2;mp3_r1=mp3_r2;if(!mp3_source_frame(station,&mp3_l2,&mp3_r2)){mp3_resample_ready=0;break;}}
 return 1;
}
static int audio_worker(SceSize n,void *a){
 (void)n;(void)a;int output_index=0,prepared=0;unsigned noise=0x7141u;
 AudioVoice event_voice[AUDIO_EVENT_VOICES];memset(event_voice,0,sizeof(event_voice));int voice_serial=1;
 int bus_gain[AUDIO_BUS_COUNT]={1000,900,1000,850};
 static RadioSynth synth,station_synth;unsigned station_phase=0,station_air_phase=701u<<16;int station_gain=0,station_air_filter=0,station_room_body_filter=0;
 LocalTVVoice tv_voice={0};
 unsigned engine_phase=0,cockpit_phase=0,load_phase=0,boost_phase=0,traffic_phase=0,manoeuvre_phase=0,settle_phase=0,approach_phase=0,station_body_phase=0,dock_phase=0,dock_tail_phase=0,jump_phase=0,jump_pressure_phase=0,jump_bloom_phase=0;int engine_gain=0,cockpit_gain=0,load_gain=0,load_filter=0,boost_was_on=0,boost_voice=0,traffic_gain=0,traffic_pan_current=0,traffic_filter=0,traffic_body_filter=0,manoeuvre_gain=0,turn_was=0,settle_left=0,settle_pan=0,settle_filter=0,approach_gain=0,approach_filter=0,station_body_filter=0,dock_motor_gain=0,dock_voice=0,dock_voice_kind=0,dock_tail_left=0,dock_tail_filter=0,dock_stage_seen=0,docked_seen=0,jump_was_active=0,jump_start_ms=0,jump_gain=0,jump_slew=0,jump_release=0,jump_stage_seen=0,jump_pressure_filter=0,jump_bloom_left=0,jump_bloom_filter=0;
 unsigned surface_phase=0,surface_phase_b=901u<<16;int surface_filter=0,surface_body_filter=0,surface_gain=0;
 int jump_space=1000,jump_pan=0,output_limiter=1024;
 radio_synth_reset(&station_synth,0);int station=radio_station,music_gain=0,effects_gain=sound_volume*100,duck_gain=1000;
 int race_duck_left=0,race_duck_div=0,static_frames=0;
 radio_synth_reset(&synth,station);
 while(audio_run){
  if(sceKernelWaitSema(audio_free_sem,1,0)<0||!audio_run)break;
  if(audio_replay_intro){audio_replay_intro=0;mp3_close_track();music_gain=0;audio_intro_seek_pending=1;audio_intro_fade_frame=0;}
  unsigned long long mix_start=sceKernelGetSystemTimeWide();
  short *buf=audio_output[output_index];
  voice_serial=audio_voice_commands(event_voice,voice_serial);
  int events_active=0;for(int v=0;v<AUDIO_EVENT_VOICES;v++)events_active|=event_voice[v].id!=0;
  for(int v=0;v<AUDIO_EVENT_VOICES;v++)if(event_voice[v].id&&radio_race_on&&audio_race_key_cue(event_voice[v].id))race_duck_left=22050;
  for(int i=0;i<AUDIO_FRAMES;i++){
   int music_l=0,music_r=0;
   int tuning=radio_static_ms>0&&!audio_battle&&!audio_tv_on&&radio_dream_gain<0&&!radio_race_on;
   /* Battle score is not a radio station: it overrides radio power/tuning but
    * still obeys the player's MUSIC level.  Source changes crossfade through
    * zero so the MP3 decoder never tears between files. */
   int wanted_source=audio_title_active?AUDIO_INTRO_STATION:radio_dream_gain>=0||radio_race_on?8:audio_battle?RADIO_BATTLE_SOURCE:radio_station;
   int score_on=(audio_title_active||radio_dream_gain>=0||radio_race_on||audio_battle||!radio_off)&&!audio_tv_on&&(!comms_quiet||audio_title_active||radio_race_on);
   if(wanted_source!=AUDIO_INTRO_STATION||!score_on){audio_intro_seek_pending=0;audio_intro_fade_frame=AUDIO_INTRO_FADE_FRAMES;}
   int wanted=score_on&&(!tuning&&wanted_source==station)?(audio_title_active&&radio_volume==0?5:radio_volume)*100:0;
   if(radio_dream_gain>=0)wanted=wanted*radio_dream_gain/1000;
   music_gain+=(music_gain<wanted)-(music_gain>wanted);
   int effect_target=sound_volume*100;effects_gain+=(effects_gain<effect_target)-(effects_gain>effect_target);
   int critical_active=0,major_active=0;if(events_active)for(int v=0;v<AUDIO_EVENT_VOICES;v++)if(event_voice[v].id){critical_active|=event_voice[v].priority>=90;major_active|=event_voice[v].priority>=74;}
   int duck_target=audio_battle?(major_active?760:1000):(audio_scene==2||critical_active)?450:major_active?680:audio_duck?750:1000;
   if(audio_jump_remaining_ms>0&&duck_target>jump_space)duck_target=jump_space;
   if(radio_race_on){if(race_duck_left>0){race_duck_left--;if(duck_target>650)duck_target=650;}if(++race_duck_div>=24){race_duck_div=0;duck_gain+=(duck_gain<duck_target)-(duck_gain>duck_target);}}
   else {race_duck_left=race_duck_div=0;duck_gain+=(duck_gain<duck_target)-(duck_gain>duck_target);}
   if(!music_gain&&wanted_source!=station){station=wanted_source;radio_synth_reset(&synth,station==RADIO_BATTLE_SOURCE?2:station);}
   if((score_on||music_gain)&&!tuning){
    /* The presenter channels are generated from their visible scripts, so an
     * unrelated music file must never replace the voice and desynchronise it. */
    if(station==5||station==6)radio_synth_sample(&synth,&music_l,&music_r);
    else if(!mp3_sample(station,&music_l,&music_r)){if(station!=RADIO_BATTLE_SOURCE)radio_synth_sample(&synth,&music_l,&music_r);}
   }
   if(tuning){noise=noise*1664525u+1013904223u;int crackle=((int)((noise>>24)&255)-128)*22;music_l=crackle;music_r=((int)((noise>>16)&255)-128)*18;}
   if(audio_intro_fade_frame<AUDIO_INTRO_FADE_FRAMES){
    int gain=audio_intro_gain(audio_intro_fade_frame++);
    music_l=music_l*gain/1000;music_r=music_r*gain/1000;
   }
   int sfx_l=0,sfx_r=0;if(events_active)audio_voice_mix(event_voice,bus_gain,&sfx_l,&sfx_r);
   /* Resident recorded machinery: half-rate PCM is interpolated into the
    * 44.1 kHz output.  Speed changes pitch and load without file I/O. */
   int flight=audio_flight_active,speed=audio_flight_speed,boost=audio_flight_boost,turn=audio_flight_turn,hull=audio_flight_hull,thrust=audio_flight_thrust,accel=audio_flight_accel;
   /* Do not restart an onset which is still decaying: jumping an 8-bit voice
    * back to sample zero during a very fast release/repress would click. */
   if(boost&&!boost_was_on&&!boost_voice){boost_phase=0;boost_voice=1;}boost_was_on=boost;
   /* Idle stays present but subdued. Speed carries steady cruise, while actual
    * positive acceleration and held thrust add load independently. */
   int engine_target=flight?(42+(speed>1000?1000:speed)*235/1000+(thrust?55:0)+accel*70/1000+(boost?115:0)):0;if(audio_dock_stage==1)engine_target=engine_target*7/10;else if(audio_dock_stage>=2)engine_target=engine_target/4;
   int cockpit_target=flight?(hull<450?50:30):0;
   int load_target=flight?((thrust?35:0)+accel*65/1000+(boost?60:0)):0;
   engine_gain+=(engine_gain<engine_target)*2-(engine_gain>engine_target)*2;
   cockpit_gain+=(cockpit_gain<cockpit_target)-(cockpit_gain>cockpit_target);
   load_gain+=(load_gain<load_target)-(load_gain>load_target);
   unsigned engine_step=(unsigned)(28000+(speed>2500?2500:speed)*13+(boost?12000:0));
   unsigned engine_length=(unsigned)(sizeof(flight_engine_industrial_pcm)/sizeof(flight_engine_industrial_pcm[0])),cockpit_length=(unsigned)(sizeof(flight_cockpit_machinery_pcm)/sizeof(flight_cockpit_machinery_pcm[0]));
   unsigned eindex=engine_phase>>16,enext=(eindex+1)%engine_length;
   int ef=(int)(engine_phase&65535),engine_sample=(((int)flight_engine_industrial_pcm[eindex]*(65536-ef)+(int)flight_engine_industrial_pcm[enext]*ef)>>16);engine_phase+=engine_step;if(engine_phase>=engine_length<<16)engine_phase-=engine_length<<16;
   unsigned cindex=cockpit_phase>>16,cnext=(cindex+1)%cockpit_length;
   int cf=(int)(cockpit_phase&65535),cockpit_sample=(((int)flight_cockpit_machinery_pcm[cindex]*(65536-cf)+(int)flight_cockpit_machinery_pcm[cnext]*cf)>>16);cockpit_phase+=32768;if(cockpit_phase>=cockpit_length<<16)cockpit_phase-=cockpit_length<<16;
   /* A separate recorded load layer moves more slowly than the main engine.
    * Low-pass smoothing makes it read as pressure through the hull rather than
    * another bright loop. It only emerges under thrust, acceleration or boost. */
   unsigned li=load_phase>>16,ln=(li+1)%cockpit_length;int lf=(int)(load_phase&65535);
   int load_sample=(((int)flight_cockpit_machinery_pcm[li]*(65536-lf)+(int)flight_cockpit_machinery_pcm[ln]*lf)>>16);load_phase+=(unsigned)(20500+(speed>1400?1400:speed)*3);if(load_phase>=cockpit_length<<16)load_phase-=cockpit_length<<16;load_filter+=(load_sample-load_filter)/12;
   int damage_pulse=hull<450?900+(radio_triangle(cockpit_phase/19u)*100/256):1000;
   /* Two bounded stages prevent overflow even at historic gain135; rounding is below2 PCM units. */
   int flight_sample=engine_sample*engine_gain/1000+(cockpit_sample*cockpit_gain/1000)*damage_pulse/1000+load_filter*load_gain/1000;
   if(boost_voice){
    unsigned boost_length=(unsigned)(sizeof(flight_boost_warm_gravity_pcm)/sizeof(flight_boost_warm_gravity_pcm[0]));
    unsigned bindex=boost_phase>>16;
    if(bindex>=boost_length)boost_voice=0;
    else {flight_sample+=(int)flight_boost_warm_gravity_pcm[bindex]*110;boost_phase+=(unsigned)((FLIGHT_BOOST_RATE*65536u)/44100u);}
   }
   flight_sample=flight_sample*jump_space/1000;
   int pan=turn/18;if(pan<-90)pan=-90;if(pan>90)pan=90;
   sfx_l+=flight_sample*(100-pan)/125;sfx_r+=flight_sample*(100+pan)/125;
   /* Attitude jets are a restrained pressure/mechanical layer. They are most
    * audible when steering, live opposite the requested yaw, and disappear
    * smoothly at centred controls. */
   int manoeuvre_target=flight?(turn<0?-turn:turn)*42/1000:0;
   manoeuvre_gain+=(manoeuvre_gain<manoeuvre_target)-(manoeuvre_gain>manoeuvre_target);
   if(manoeuvre_gain){unsigned mi=manoeuvre_phase>>16,mn=(mi+1)%cockpit_length;int mf=(int)(manoeuvre_phase&65535);int ms=(((int)flight_cockpit_machinery_pcm[mi]*(65536-mf)+(int)flight_cockpit_machinery_pcm[mn]*mf)>>16);manoeuvre_phase+=24500;if(manoeuvre_phase>=cockpit_length<<16)manoeuvre_phase-=cockpit_length<<16;int mp=-turn/14;if(mp<-78)mp=-78;if(mp>78)mp=78;sfx_l+=audio_pan_sample(ms,manoeuvre_gain,100-mp);sfx_r+=audio_pan_sample(ms,manoeuvre_gain,100+mp);}
   /* Releasing a firm steering input leaves a brief, dark structural answer.
    * This is recorded machinery filtered into the hull, not a synthetic chirp. */
   if(flight&&abs(turn)<90&&abs(turn_was)>360&&settle_left<=0){settle_left=11025;settle_phase=manoeuvre_phase;settle_pan=turn_was>0?-34:34;turn_was=0;}if(abs(turn)>180)turn_was=turn;else if(!flight)turn_was=0;
   if(settle_left>0){unsigned si=settle_phase>>16,sn=(si+1)%cockpit_length;int sf=(int)(settle_phase&65535);int ss=(((int)flight_cockpit_machinery_pcm[si]*(65536-sf)+(int)flight_cockpit_machinery_pcm[sn]*sf)>>16);settle_phase+=17200;if(settle_phase>=cockpit_length<<16)settle_phase-=cockpit_length<<16;settle_filter+=(ss-settle_filter)/16;int sg=settle_left*44/11025;sfx_l+=audio_pan_sample(settle_filter,sg,100-settle_pan);sfx_r+=audio_pan_sample(settle_filter,sg,100+settle_pan);settle_left--;}
   /* Closest audible traffic is a separate spatial voice. Distance controls
    * level; bearing controls pan; speed and hull size alter pitch. */
   int traffic_target=0,traffic_distance=audio_traffic_distance;
   if(flight&&audio_traffic_active&&traffic_distance<6000){int near=6000-traffic_distance;static const unsigned char levels[3]={118,142,175};int kind=audio_traffic_kind;if(kind<0)kind=0;if(kind>2)kind=2;traffic_target=near*levels[kind]/6000;}
   traffic_gain+=(traffic_gain<traffic_target)-(traffic_gain>traffic_target);
   if(traffic_gain){
    int kind=audio_traffic_kind;if(kind<0)kind=0;if(kind>2)kind=2;static const unsigned short steps[3]={35500,28500,20500};unsigned traffic_step=(unsigned)(steps[kind]+(audio_traffic_speed>1000?1000:audio_traffic_speed)*8+(audio_traffic_character-1)*700);
    unsigned ti=traffic_phase>>16,tn=ti+1;if(tn==engine_length)tn=0;int tf=(int)((traffic_phase>>4)&4095);
    int traffic_sample=(flight_engine_industrial_pcm[ti]*(4096-tf)+flight_engine_industrial_pcm[tn]*tf)>>12;
    traffic_phase+=traffic_step;if(traffic_phase>=engine_length<<16)traffic_phase-=engine_length<<16;
    /* The earlier separate cockpit loop exposed its repeating fan impulses as
     * small taps during a flyby. Two cascaded bands from the sustained engine
     * recording produce continuous propulsion: the first retains motion, the
     * second supplies soft hull mass without adding another periodic source. */
    int smooth=kind==0?7:(kind==1?11:17);if(audio_traffic_blocked)smooth+=12;
    traffic_filter+=(traffic_sample-traffic_filter)/smooth;
    traffic_body_filter+=(traffic_filter-traffic_body_filter)/(kind==0?28:(kind==1?36:48));
    traffic_sample=traffic_filter*7/10+traffic_body_filter*(kind+2)/10;
    int flyby=audio_traffic_relative;if(flyby<0)flyby=-flyby;if(flyby>1000)flyby=1000;int moving_gain=traffic_gain+flyby*36/1000;
    int tpan_target=audio_traffic_pan;if(tpan_target<-92)tpan_target=-92;if(tpan_target>92)tpan_target=92;
    traffic_pan_current+=(traffic_pan_current<tpan_target)-(traffic_pan_current>tpan_target);int tpan=traffic_pan_current;
    if(audio_traffic_blocked)moving_gain=moving_gain*3/10;
    sfx_l+=audio_pan_sample(traffic_sample,moving_gain,100-tpan);sfx_r+=audio_pan_sample(traffic_sample,moving_gain,100+tpan);
   }
   /* Station mass enters gradually on approach. It reuses the recorded
    * machinery bed at a lower rate, avoiding another resident loop. */
   int approach_target=0;if(audio_approach_active&&audio_approach_distance<16000)approach_target=(16000-audio_approach_distance)*78/16000;if(audio_dock_stage==1&&approach_target<92)approach_target=92;else if(audio_dock_stage==2)approach_target=125;else if(audio_dock_stage==3)approach_target=98;
   approach_gain+=(approach_gain<approach_target)-(approach_gain>approach_target);
   if(approach_gain){unsigned ai=approach_phase>>16,an=(ai+1)%cockpit_length;int af=(int)(approach_phase&65535);int as=(((int)flight_cockpit_machinery_pcm[ai]*(65536-af)+(int)flight_cockpit_machinery_pcm[an]*af)>>16);unsigned approach_step=audio_dock_stage==2?14500:audio_dock_stage==3?11000:18500+(unsigned)audio_dock_phase*900;approach_phase+=approach_step;if(approach_phase>=cockpit_length<<16)approach_phase-=cockpit_length<<16;/* The station bed must read as distant structure, never the fan's individual impulses. Cascaded inertia is unnecessary here: a strong one-pole filter removes the taps while preserving slow machinery movement. */approach_filter+=(as-approach_filter)/(audio_dock_stage?24:32);int ap=audio_dock_stage>=2?0:audio_approach_pan;sfx_l+=audio_pan_sample(approach_filter,approach_gain,100-ap);sfx_r+=audio_pan_sample(approach_filter,approach_gain,100+ap);
    /* Inside six kilometres a second, darker recorded component supplies the
     * apparent mass of the structure. It stays centred during capture so the
     * moving aperture layer remains easy to localise. */
    int body_gain=audio_approach_distance<6000?(6000-audio_approach_distance)*34/6000:0;unsigned sbi=station_body_phase>>16,sbn=(sbi+1)%engine_length;int sbf=(int)(station_body_phase&65535);int station_body=(((int)flight_engine_industrial_pcm[sbi]*(65536-sbf)+(int)flight_engine_industrial_pcm[sbn]*sbf)>>16);station_body_phase+=12500;if(station_body_phase>=engine_length<<16)station_body_phase-=engine_length<<16;station_body_filter+=(station_body-station_body_filter)/18;sfx_l+=station_body_filter*body_gain/1000;sfx_r+=station_body_filter*body_gain/1000;}
   /* Guidance, capture and clamp share one continuous hydraulic mass layer.
    * Its gain changes with the real docking stage, so the discrete contacts
    * land inside an already moving structure rather than appearing from silence. */
   int dock_motor_target=audio_dock_stage==1?22+(audio_dock_phase<4?audio_dock_phase:4)*4:audio_dock_stage==2?48:audio_dock_stage==3?34:0;
   dock_motor_gain+=(dock_motor_gain<dock_motor_target)-(dock_motor_gain>dock_motor_target);
   if(dock_motor_gain){sfx_l+=station_body_filter*dock_motor_gain/1000;sfx_r+=station_body_filter*dock_motor_gain/1000;}
   int current_dock_stage=audio_dock_stage,current_docked=audio_is_docked;
   if(current_dock_stage>=2&&dock_stage_seen<2){dock_phase=0;dock_voice=1;dock_voice_kind=1;}if(current_docked&&!docked_seen){dock_phase=0;dock_voice=1;dock_voice_kind=2;}dock_stage_seen=current_dock_stage;docked_seen=current_docked;
   if(dock_voice){unsigned dock_length=(unsigned)(sizeof(flight_dock_clamp_pcm)/sizeof(flight_dock_clamp_pcm[0])),di=dock_phase>>16;if(di>=dock_length){dock_voice=0;dock_tail_left=dock_voice_kind==2?24255:9702;dock_tail_phase=approach_phase;}else {int dg=dock_voice_kind==2?190:82,ds=(int)flight_dock_clamp_pcm[di]*dg;sfx_l+=ds;sfx_r+=ds;unsigned rate=dock_voice_kind==2?FLIGHT_BOOST_RATE:6200;dock_phase+=(unsigned)((rate*65536u)/44100u);}}
   if(dock_tail_left>0){unsigned dti=dock_tail_phase>>16,dtn=(dti+1)%cockpit_length;int dtf=(int)(dock_tail_phase&65535);int dts=(((int)flight_cockpit_machinery_pcm[dti]*(65536-dtf)+(int)flight_cockpit_machinery_pcm[dtn]*dtf)>>16);dock_tail_phase+=dock_voice_kind==2?10500:15800;if(dock_tail_phase>=cockpit_length<<16)dock_tail_phase-=cockpit_length<<16;dock_tail_filter+=(dts-dock_tail_filter)/(dock_voice_kind==2?20:13);int dtg=dock_tail_left*(dock_voice_kind==2?56:28)/(dock_voice_kind==2?24255:9702);sfx_l+=dock_tail_filter*dtg/1000;sfx_r+=dock_tail_filter*dtg/1000;dock_tail_left--;}
   /* Hyperspace is one continuous recorded-engine transformation. Charge
    * duration follows the real fitted drive; exit receives a soft pressure
    * release instead of replaying the old generic sweep. */
   int jump_ms=audio_jump_remaining_ms,jump_active=jump_ms>0;
   if(jump_active&&!jump_was_active){jump_start_ms=jump_ms<500?500:jump_ms;jump_phase=jump_pressure_phase=jump_bloom_phase=0;jump_stage_seen=0;jump_bloom_left=0;}
   if(!jump_active&&jump_was_active){jump_release=26460;
#ifdef ELITE_ENABLE_REVIEW
    audio_jump_review_flags|=8;
#endif
   }
   jump_was_active=jump_active;int jump_progress=jump_active?(jump_start_ms-jump_ms)*1000/jump_start_ms:1000;if(jump_progress<0)jump_progress=0;if(jump_progress>1000)jump_progress=1000;
   /* A fitted-drive-relative quiet beat leads into the corridor. Keep event
    * voices outside this dip so missile and damage cues remain intelligible. */
   int space_target=1000;if(jump_active&&jump_progress>=250&&jump_progress<400){space_target=jump_progress<320?1000-(jump_progress-250)*870/70:jump_progress<350?130:130+(jump_progress-350)*870/50;}
   jump_space+=(jump_space<space_target)-(jump_space>space_target);
   int jump_stage=jump_progress<400?0:2;if(jump_active&&jump_stage>jump_stage_seen){jump_stage_seen=jump_stage;jump_bloom_left=15435;}
   int pan_target=jump_active&&jump_stage==2?(jump_progress<700?(jump_progress-400)*28/300:28-(jump_progress-700)*56/300):0;jump_pan+=(jump_pan<pan_target)-(jump_pan>pan_target);
   int jump_target=jump_active?(72+jump_progress*138/1000+(jump_stage==2?22:0))*jump_space/1000:0;if(++jump_slew>=48){jump_slew=0;jump_gain+=(jump_gain<jump_target)-(jump_gain>jump_target);}
#ifdef ELITE_ENABLE_REVIEW
   if(jump_active)audio_jump_review_flags|=1;
   if(jump_active&&jump_space<=180)audio_jump_review_flags|=2;
   if(jump_active&&jump_stage==2)audio_jump_review_flags|=4;
   if(!jump_active&&(audio_jump_review_flags&8)&&!jump_release&&!jump_gain)audio_jump_review_flags|=16;
   if(jump_pan>=20)audio_jump_review_flags|=32;
   if(jump_pan<=-20)audio_jump_review_flags|=64;
#endif
   if(jump_gain||jump_release>0){unsigned ji=jump_phase>>16,jn=(ji+1)%engine_length;int jf=(int)(jump_phase&65535);int js=(((int)flight_engine_industrial_pcm[ji]*(65536-jf)+(int)flight_engine_industrial_pcm[jn]*jf)>>16);unsigned jstep=(unsigned)(16500+jump_progress*22000/1000+jump_stage*1800);jump_phase+=jstep;if(jump_phase>=engine_length<<16)jump_phase-=engine_length<<16;unsigned pi=jump_pressure_phase>>16,pn=(pi+1)%cockpit_length;int pf=(int)(jump_pressure_phase&65535);int pressure=(((int)flight_cockpit_machinery_pcm[pi]*(65536-pf)+(int)flight_cockpit_machinery_pcm[pn]*pf)>>16);jump_pressure_phase+=9000+jump_stage*2500;if(jump_pressure_phase>=cockpit_length<<16)jump_pressure_phase-=cockpit_length<<16;jump_pressure_filter+=(pressure-jump_pressure_filter)/20;int release_gain=jump_release>0?jump_release*85/26460:0;if(jump_release>0)jump_release--;int total=jump_gain+release_gain,mass=jump_pressure_filter*(jump_stage==2?38:26)/100;int body=js*total/1000,structure=mass*total/260;sfx_l+=body+structure*(100-jump_pan)/100;sfx_r+=body+structure*(100+jump_pan)/100;}
   if(jump_bloom_left>0){unsigned bi=jump_bloom_phase>>16,bn=(bi+1)%cockpit_length;int bf=(int)(jump_bloom_phase&65535);int bs=(((int)flight_cockpit_machinery_pcm[bi]*(65536-bf)+(int)flight_cockpit_machinery_pcm[bn]*bf)>>16);jump_bloom_phase+=14500;if(jump_bloom_phase>=cockpit_length<<16)jump_bloom_phase-=cockpit_length<<16;jump_bloom_filter+=(bs-jump_bloom_filter)/18;int elapsed=15435-jump_bloom_left,env=jump_bloom_left<2205?jump_bloom_left:(elapsed<2205?elapsed:2205);int bloom=jump_bloom_filter*env/2205*7/100;jump_bloom_left--;sfx_l+=bloom;sfx_r+=bloom;}
   int utility=local_tv_voice_sample(&tv_voice,audio_tv_on?audio_tv_code:0);
   int room=audio_station_room;
   int ambient_target=room<0?0:(room==3?220:(room==2?92:(room==5?72:110)));station_gain+=(station_gain<ambient_target)-(station_gain>ambient_target);
   if(station_gain){
    unsigned el=sizeof(flight_engine_industrial_pcm)/sizeof(flight_engine_industrial_pcm[0]);
    unsigned ei=station_phase>>16;int machinery=flight_engine_industrial_pcm[ei];
    unsigned al=SURFACE_WIND_LENGTH;
    unsigned ai=station_air_phase>>16;int air=(int)surface_open_field_pcm[SURFACE_WIND_OFFSET+ai]<<8;
    unsigned machine_step=room==3?15100:12400,air_step=room==2?9400:10200;
    station_phase+=machine_step;if(station_phase>=el<<16)station_phase-=el<<16;
    station_air_phase+=air_step;if(station_air_phase>=al<<16)station_air_phase-=al<<16;
    station_room_body_filter+=(machinery-station_room_body_filter)/(room==3?28:42);
    station_air_filter+=(air-station_air_filter)/(room==2?34:24);
    int room_body=station_room_body_filter*(room==3?58:36)/100;
    int room_air=station_air_filter*(room==2?28:42)/100;
    utility+=(room_body+room_air)*station_gain/1000;
   }
   /* Recorded open-field wind: two slowly diverging playback heads provide
    * breadth, while biome/weather only alter perspective and level. */
   int surface_code=audio_surface_ambient;
   if(audio_scene==1&&surface_code>=0){
    int surface_biome=surface_code>>2,weather=surface_code&3;
    unsigned surface_base=SURFACE_WIND_OFFSET;
    unsigned surface_length=SURFACE_WIND_LENGTH;
    unsigned body_base=surface_biome==4?SURFACE_BIRDS_OFFSET:surface_base;
    unsigned body_length=surface_biome==4?SURFACE_BIRDS_LENGTH:surface_length;
    while(surface_phase>=surface_length<<16)surface_phase-=surface_length<<16;
    while(surface_phase_b>=body_length<<16)surface_phase_b-=body_length<<16;
    unsigned si=surface_phase>>16,sn=(si+1)%surface_length;int sf=(int)(surface_phase&65535);
    int wind=(int)(((long long)surface_open_field_pcm[surface_base+si]*(65536-sf)+(long long)surface_open_field_pcm[surface_base+sn]*sf)>>8);
    unsigned bi=surface_phase_b>>16,bn=(bi+1)%body_length;int bf=(int)(surface_phase_b&65535);
    int body=(int)(((long long)surface_open_field_pcm[body_base+bi]*(65536-bf)+(long long)surface_open_field_pcm[body_base+bn]*bf)>>8);
    unsigned step=surface_biome==2?10380:(surface_biome==3?10920:(surface_biome==1?11570:11889));
    surface_phase+=step;if(surface_phase>=surface_length<<16)surface_phase-=surface_length<<16;
    surface_phase_b+=step-(surface_biome==4?91:47);if(surface_phase_b>=body_length<<16)surface_phase_b-=body_length<<16;
    int smooth=surface_biome==2?34:(surface_biome==3?27:(surface_biome==4?17:22));
    surface_filter+=(wind-surface_filter)/smooth;surface_body_filter+=(body-surface_body_filter)/(smooth+18);
    int target=weather==3?125:(weather==2?105:(weather==1?91:78));
    if(surface_biome==0)target=0; /* Ocean ambience stays silent until its listening review passes. */
    if(surface_biome==2)target=target*72/100;if(surface_biome==5)target=target*82/100;
    surface_gain+=(surface_gain<target)-(surface_gain>target);
    int air=surface_filter*(surface_biome==4?70:58)/100;
    int mass=surface_body_filter*(surface_biome==3?56:38)/100;
    utility+=(air+mass)*surface_gain/1000;
   }else{surface_gain+=(surface_gain>0?-1:0);surface_filter-=surface_filter/32;surface_body_filter-=surface_body_filter/40;}
   int station_music=0;
   if(room==2&&audio_station_juke&&radio_off&&!comms_quiet){int l,r;radio_synth_sample(&station_synth,&l,&r);station_music=(l+r)*radio_volume/50;}
   utility=utility*effects_gain/1000+station_music*duck_gain/1000;
   sfx_l=sfx_l*effects_gain/1000+utility;sfx_r=sfx_r*effects_gain/1000+utility;
   int left=(music_l*music_gain/1000)*duck_gain/1000+sfx_l;
   int right=(music_r*music_gain/1000)*duck_gain/1000+sfx_r;
   /* Stereo-linked peak control avoids hard clipping when music, boost and
    * multiple effects coincide. Fast attack, smooth release, no channel drift. */
   int peak=abs(left)>abs(right)?abs(left):abs(right);
   int limit=peak>26000?26624000/peak:1024;
   if(limit<output_limiter)output_limiter=limit;
   else output_limiter+=(1024-output_limiter+1023)>>10;
   left=left*output_limiter/1024;right=right*output_limiter/1024;
   buf[i*2]=(short)left;buf[i*2+1]=(short)right;
  }
  if(radio_static_ms>0){static_frames+=AUDIO_FRAMES;if(static_frames>=2048){static_frames-=2048;radio_static_ms--;}}else static_frames=0;
  unsigned mix_us=(unsigned)(sceKernelGetSystemTimeWide()-mix_start);
  if(prepared>2&&mix_us>audio_mix_worst_us)audio_mix_worst_us=mix_us;
  sceKernelSignalSema(audio_ready_sem,1);
  if(++prepared==AUDIO_QUEUE_BLOCKS-1)sceKernelSignalSema(audio_start_sem,1);
  output_index=(output_index+1)%AUDIO_QUEUE_BLOCKS;
 }
 return 0;
}
/* Only this deadline thread talks to the device. File/codec stalls remain
 * on the producer; PCM buffers cannot be reused until output releases them. */
static int audio_device_worker(SceSize n,void *a){
 (void)n;(void)a;int index=0;
 if(sceKernelWaitSema(audio_start_sem,1,0)<0)return 0;
 while(audio_run){
  if(sceKernelWaitSema(audio_ready_sem,1,0)<0||!audio_run)break;
  if(audio_blocks>2&&sceAudioGetChannelRestLen(audio_ch)==0)audio_buffer_starves++;
  if(sceAudioOutputBlocking(audio_ch,PSP_AUDIO_VOLUME_MAX/3,audio_output[index])<0){audio_run=0;break;}
  audio_blocks++;index=(index+1)%AUDIO_QUEUE_BLOCKS;
  /* Retain two submitted buffers: the PSP may still be reading them. */
  if(audio_blocks>2)sceKernelSignalSema(audio_free_sem,1);
 }
 return 0;
}
static void audio_init(void){
 /* The MP3 library's legacy SDK mode rejects 48/32 kHz user tracks. Declare
  * the firmware API we build against; never rewrite an MP3's audio header. */
 sceKernelSetCompiledSdkVersion(0x06000010);
 FILE *silent_test=fopen("silent-test.flag","r");if(silent_test){fclose(silent_test);return;}
 mp3_frozen=0;
 memset(music_bad,0,sizeof(music_bad));
 radio_load_settings("radio.cfg");
 if(audio_first_start){
  audio_first_start=0;audio_intro_seek_pending=1;
  audio_intro_skipped_samples=audio_intro_source_rate=0;
  audio_intro_fade_frame=audio_intro_seek_pending?0:AUDIO_INTRO_FADE_FRAMES;
 }
 radio_scan_music();int has_mp3=0;for(int i=0;i<RADIO_FILE_SOURCE_COUNT;i++)for(int j=0;j<radio_track_count[i];j++)has_mp3|=radio_has_mp3_extension(radio_tracks[i][j]);if(has_mp3){
  /* JPEG startup can already own AVCODEC. A negative "already loaded" result
   * must not short-circuit loading MP3. Resource init is the usability check. */
  if(!mp3_modules_loaded){sceUtilityLoadModule(PSP_MODULE_AV_AVCODEC);sceUtilityLoadModule(PSP_MODULE_AV_MP3);mp3_modules_loaded=1;}
  if(!mp3_resource_ready&&sceMp3InitResource()>=0)mp3_resource_ready=1;
 }
 audio_ch=sceAudioChReserve(PSP_AUDIO_NEXT_CHANNEL,AUDIO_FRAMES,PSP_AUDIO_FORMAT_STEREO);if(audio_ch<0){audio_run=0;return;}
 /* Device deadlines outrank decoding, and both outrank busy rendering. */
 audio_blocks=audio_buffer_starves=audio_mix_worst_us=0;
 audio_free_sem=sceKernelCreateSema("audio-free",0,AUDIO_QUEUE_BLOCKS,AUDIO_QUEUE_BLOCKS,0);
 audio_ready_sem=sceKernelCreateSema("audio-ready",0,0,AUDIO_QUEUE_BLOCKS,0);
 audio_start_sem=sceKernelCreateSema("audio-start",0,0,1,0);
 if(audio_free_sem<0||audio_ready_sem<0||audio_start_sem<0){audio_stop();return;}
 audio_run=1;
 audio_device_thread=sceKernelCreateThread("audio-device",audio_device_worker,0x18,0x4000,0,0);
 audio_thread=sceKernelCreateThread("audio-mix",audio_worker,0x19,0x10000,THREAD_ATTR_VFPU,0);
 if(audio_thread<0||audio_device_thread<0){audio_stop();return;}
 if(sceKernelStartThread(audio_device_thread,0,0)<0||sceKernelStartThread(audio_thread,0,0)<0)audio_stop();
}
static void audio_play_at(int id,int gain,int pan){if(id){audio_sfx=id;audio_command_push(id,gain,pan);}}
static void audio_play(int id){audio_play_at(id,1000,0);}
static void audio_scene_set(int scene){audio_scene=scene;}
static void audio_flight_update(int active,float speed_fraction,float steering,int boost,int hull,int thrust){
 static int previous_speed=0;
 int speed=(int)(speed_fraction*1000.f),turn=(int)(steering*1000.f);
 if(speed<0)speed=0;if(speed>2500)speed=2500;if(turn<-1000)turn=-1000;if(turn>1000)turn=1000;
 if(!active)previous_speed=speed;
 int change=speed-previous_speed;previous_speed=speed;int accel=change>0?change*24:0;if(accel>1000)accel=1000;
 audio_flight_speed=speed;audio_flight_turn=turn;audio_flight_boost=boost!=0;audio_flight_hull=hull<0?0:hull>100?1000:hull*10;audio_flight_thrust=thrust!=0;audio_flight_accel=accel;audio_flight_active=active!=0;
}
static void audio_traffic_update(int active,float distance,float pan,float speed_fraction,int kind,int character,int blocked,float relative_speed){
 int d=(int)distance,p=(int)(pan*100.f),speed=(int)(speed_fraction*1000.f),relative=(int)relative_speed;
 if(d<0)d=0;if(d>6000)d=6000;if(p<-100)p=-100;if(p>100)p=100;if(speed<0)speed=0;if(speed>2000)speed=2000;if(relative<-1000)relative=-1000;if(relative>1000)relative=1000;
 audio_traffic_distance=d;audio_traffic_pan=p;audio_traffic_speed=speed;audio_traffic_kind=kind;audio_traffic_character=character&3;audio_traffic_blocked=blocked!=0;audio_traffic_relative=relative;audio_traffic_active=active!=0;
}
static void audio_approach_update(int active,float distance,float pan,int dock_stage,int dock_phase,int docked){int d=(int)distance,p=(int)(pan*100.f);if(d<0)d=0;if(d>20000)d=20000;if(p<-85)p=-85;if(p>85)p=85;audio_approach_active=active!=0;audio_approach_distance=d;audio_approach_pan=p;audio_dock_stage=dock_stage;audio_dock_phase=dock_phase;audio_is_docked=docked!=0;}
static void audio_hyperspace_update(float remaining){int ms=(int)(remaining*1000.f);if(ms<0)ms=0;if(ms>20000)ms=20000;audio_jump_remaining_ms=ms;}
static void audio_warning_update(int active,float heat,float hull,float missile,float dt,int pan){
 static int previous=0;static float repeat=0;int level=!active?0:missile>0?3:heat>=90?2:hull<25?1:0;
 if(level!=previous){repeat=level==3?1.15f:level==2?1.8f:3.2f;previous=level;}
 else if(level&&((repeat-=dt)<=0)){audio_command_push(SFX_ALERT,level==3?850:level==2?700:620,level==3?pan:0);repeat=level==3?1.4f:level==2?2.25f:4.f;}
 if(!level)repeat=0;
}
/* Signal the worker to leave MP3/file I/O before the PSP finishes sleeping.
 * Safe from the power callback: no waits, no frees. */
static void audio_prepare_suspend(void){mp3_frozen=1;audio_run=0;audio_flight_active=audio_traffic_active=audio_approach_active=0;audio_jump_remaining_ms=0;audio_command_read=audio_command_write;}
static void audio_stop(void){
 mp3_frozen=1;audio_run=0;
 if(audio_free_sem>=0)sceKernelSignalSema(audio_free_sem,1);
 if(audio_ready_sem>=0)sceKernelSignalSema(audio_ready_sem,1);
 if(audio_start_sem>=0)sceKernelSignalSema(audio_start_sem,1);
 if(audio_device_thread>=0){SceUInt timeout=1000000;if(sceKernelWaitThreadEnd(audio_device_thread,&timeout)<0)sceKernelTerminateDeleteThread(audio_device_thread);else sceKernelDeleteThread(audio_device_thread);audio_device_thread=-1;}
 if(audio_thread>=0){SceUInt timeout=1000000;if(sceKernelWaitThreadEnd(audio_thread,&timeout)<0)sceKernelTerminateDeleteThread(audio_thread);else sceKernelDeleteThread(audio_thread);audio_thread=-1;}
 if(audio_ch>=0)sceAudioChRelease(audio_ch);
 audio_ch=-1;
 if(audio_free_sem>=0)sceKernelDeleteSema(audio_free_sem);if(audio_ready_sem>=0)sceKernelDeleteSema(audio_ready_sem);if(audio_start_sem>=0)sceKernelDeleteSema(audio_start_sem);audio_free_sem=audio_ready_sem=audio_start_sem=-1;
 mp3_close_track();if(mp3_resource_ready){sceMp3TermResource();mp3_resource_ready=0;}
 if(radio_dirty)radio_save_settings("radio.cfg");
 mp3_frozen=0;
}







