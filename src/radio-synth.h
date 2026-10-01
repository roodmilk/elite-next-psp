/* Original music for Starfarer. No recordings or third-party compositions.
 * 44,100 Hz, integer-only, seven voices plus filtered stereo delay and percussion, no allocation.
 * Sixteen-bar arrangements take 30--58 seconds and vary their second half.
 * All state belongs to the audio worker. Reset between station changes;
 * the caller should fade its master gain when changing stations. */
#ifndef STARFARER_RADIO_SYNTH_H
#define STARFARER_RADIO_SYNTH_H
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "radio-voice.h"

#define RADIO_STATION_COUNT 8

typedef struct {
 uint32_t phase,phase_b,increment;
 int envelope,target,gate;
} RadioVoice;
typedef struct {
 RadioVoice voice[7];
 short delay_l[8192],delay_r[8192];
 int delay_pos,filter_l,filter_r;
 uint32_t noise,kick_phase,kick_increment;
 uint32_t chatter_phase,chatter_phase_b;
 int talk_age,talk_length,talk_gap,talk_words,talk_voice,talk_pitch,talk_end,talk_vowel;
 int talk_line,talk_char,talk_letter,talk_noise_mix;
 int station,step,remaining,step_samples;
 int kick,snare,hat,previous_noise;
} RadioSynth;

static inline const char *radio_station_name(int index){
 static const char *const names[RADIO_STATION_COUNT]={
  "Deep Field","Neon Transit","Pixel Comet","Velvet Orbit","Far Horizons","SPACE TALK","VOID TALES","Elite Exploration"};
  return names[index>=0&&index<RADIO_STATION_COUNT?index:0];
}
static inline const char *radio_station_genre(int index){
 static const char *const genres[RADIO_STATION_COUNT]={
  "Ambient","Synthwave","Chiptune","Lounge","Orchestral","TALK RADIO","ALIEN STORY","EXPLORATION"};
 return genres[index>=0&&index<RADIO_STATION_COUNT?index:0];
}
static inline uint32_t radio_note_increment(int note){
 /* C4 through B4; 2^32 * frequency / 44100, rounded offline. */
 static const uint32_t table[12]={25480119u,26995246u,28600467u,
  30301139u,32102938u,34011878u,36034330u,38177043u,
  40447168u,42852281u,45400411u,48100060u};
 int octave;
 if(note<24)note=24;
 if(note>95)note=95;
 octave=note/12-5;
 return octave<0?table[note%12]>>(-octave):table[note%12]<<octave;
}
static inline int radio_triangle(uint32_t phase){
 int x=(int)(phase>>22); /* -256..256, centred, no sample table. */
 return x<512?x-256:768-x;
}
static inline void radio_note(RadioVoice *v,int note,int level,int gate){
 v->increment=radio_note_increment(note);
 v->target=level*256;
 v->gate=gate;
 /* Continuous phase and smoothed amplitude suppress key-on clicks. */
}
static inline void radio_sequence_step(RadioSynth *s){
 static const unsigned char roots[5][8]={
  {45,41,48,43,45,41,38,43},
  {45,41,48,43,45,41,38,40},
  {48,45,41,43,48,45,38,43},
  {41,38,43,48,41,38,40,45},
  {38,46,41,48,38,46,43,45}};
 static const unsigned char minor[5][8]={
  {1,0,0,0,1,0,1,0},
  {1,0,0,0,1,0,1,0},
  {0,1,0,0,0,1,1,0},
  {0,1,0,0,0,1,1,1},
  {1,0,0,0,1,0,1,0}};
 /* Melody codes are chord degrees, 0=root, 1=third, 2=fifth,
  * 3=octave, 4=ninth. -1 leaves air for the world and radio SFX. */
 static const signed char melodies[5][16]={
  {0,-1,-1,-1,2,-1,1,-1,4,-1,-1,-1,2,-1,1,-1},
  {0,-1,2,-1,3,2,1,-1,0,-1,1,2,4,-1,2,-1},
  {0,2,3,2,1,2,3,-1,0,1,2,4,3,2,1,-1},
  {-1,0,-1,2,-1,-1,4,-1,1,-1,-1,2,-1,0,-1,-1},
  {0,-1,-1,-1,1,-1,2,-1,3,-1,-1,-1,2,-1,1,-1}};
 int station=s->station,style=station==7?0:station,bar=s->step>>4,beat=s->step&15;
 if(station==5||station==6){s->remaining=s->step_samples;s->step=(s->step+1)&255;return;}
 int chord=bar&7,root=roots[style][chord]+(station==7?5:0);
 int third=minor[style][chord]?3:4;
 int degrees[5]={0,third,7,12,14};
 int duration=s->step_samples,code,lead_gate;
 if(style==3)duration+=(beat&1)?-(duration/8):duration/8;
 s->remaining=duration;
 if(beat==0){
  int pad=style==2?140:(style==0?420:330);
  radio_note(&s->voice[0],root+12,pad,duration*32);
  radio_note(&s->voice[1],root+12+third,pad,duration*32);
  radio_note(&s->voice[2],root+19,pad,duration*32);
  radio_note(&s->voice[5],root+12+(minor[style][chord]?10:11),style==0?200:140,duration*32);
  radio_note(&s->voice[6],root+26,style==0?120:style==4?160:70,duration*24);
 }
 if((style==0&&beat==0)||(style==4&&(beat==0||beat==8))||
    (style==1&&(beat%2==0))||
    (style==2&&(beat%4==0))||
    (style==3&&(beat==0||beat==6||beat==10))){
  int bass_note=root-12;
  if(style==1&&(beat==6||beat==14))bass_note+=12;
  if(style==3&&beat==10)bass_note+=7;
  radio_note(&s->voice[3],bass_note,style==0?450:720,
   duration*(style==0?12:style==4?6:1));
 }
 code=melodies[style][beat];
 /* The answer phrase changes contour and octave without random dissonance. */
 if(bar>=8&&code>=0){
  if((bar&1)&&code==0)code=2;
  else if(beat>=8&&code==2)code=1;
 }
 if(code>=0){
  int note=root+24+degrees[code];
  int level=style==0?420:style==4?550:620;
  if(style==2&&bar>=8)note+=12;
  if(note>95)note-=12; /* Preserve the chord degree at the oscillator limit. */
  lead_gate=duration*(style==0?3:style==4?2:1);
  if(style==1||style==2||style==3)lead_gate=duration*3/4;
  radio_note(&s->voice[4],note,level,lead_gate);
 }
 if((style==1&&beat%4==0)||
    (style==2&&(beat==0||beat==8||beat==10))||
    (style==3&&(beat==0||beat==6||beat==10))||
    (style==4&&beat==0&&(bar&1)==0)){
  s->kick=(style==4?620:920)*256;
  s->kick_increment=radio_note_increment(style==4?38:45);
 }
 if(style>=1&&style<=3&&(beat==4||beat==12))
  s->snare=(style==3?150:240)*256;
 if((style==1&&beat%2==0)||(style==2&&beat%2==0)||
    (style==3&&(beat==3||beat==6||beat==11||beat==14))||
    (style==0&&beat==14&&(bar&1)))
  s->hat=(style==0?20:style==3?40:55)*256;
 /* Small final-bar fills, never additional oscillators or allocations. */
 if((bar==7||bar==15)&&beat==15&&style>=1&&style<=3)
  s->snare=190*256;
 s->step=(s->step+1)&255;
}
static inline void radio_synth_reset(RadioSynth *s,int station){
 static const int tempo[RADIO_STATION_COUNT]={66,112,128,88,92,176,72,78};
 if(!s)return;
 memset(s,0,sizeof(*s));
 s->station=station>=0&&station<RADIO_STATION_COUNT?station:0;
 s->noise=0x65ace173u+(uint32_t)s->station*1009u;
 s->step_samples=661500/tempo[s->station]; /* 44100*60/4/BPM */
 if(s->station==5||s->station==6){
  s->talk_line=0;s->talk_char=0;s->talk_letter=-1;
  s->talk_voice=s->station==6?1:0;
  radio_voice_station=s->station;radio_voice_line=0;
  radio_voice_char=0;radio_voice_active=0;
 }
}
static inline void radio_synth_sample(RadioSynth *s,int *left,int *right){
 int l=0,r=0,i,n,high,percussion=0;
 if(!s){if(left)*left=0;if(right)*right=0;return;}
 if(s->station==5||s->station==6){
  /* The script is the score. Each character becomes a tiny voiced or noisy
   * phoneme and the audio worker publishes the exact cursor to the ticker.
   * This deliberately resembles compact life-sim speech rather than trying
   * to store recorded dialogue or run a heavyweight TTS engine on the PSP. */
  const char *script=radio_voice_script(s->station,s->talk_line);
  if(s->talk_gap>0){
   --s->talk_gap;radio_voice_active=0;
   if(left)*left=0;if(right)*right=0;return;
  }
  if(s->talk_age>=s->talk_length){
   if(s->talk_letter>=0)s->talk_char++;
   script=radio_voice_script(s->station,s->talk_line);
   if(!script[s->talk_char]){
    s->talk_line=(s->talk_line+1)%RADIO_VOICE_LINE_COUNT;
    s->talk_char=0;s->talk_letter=-1;s->talk_length=0;s->talk_age=0;
    s->talk_voice=(s->talk_voice+1)%4;s->talk_words=0;
    s->talk_gap=s->station==6?52920:22050;
    radio_voice_station=s->station;radio_voice_line=s->talk_line;
    radio_voice_char=0;radio_voice_active=0;
    if(left)*left=0;if(right)*right=0;return;
   }
   {
    unsigned char raw=(unsigned char)script[s->talk_char];
    int c=tolower(raw),slow=s->station==6;
    s->talk_letter=c;s->talk_age=0;s->talk_noise_mix=0;
    if(c==' '){
     s->talk_length=slow?2400:1250;
     if(++s->talk_words%7==0)s->talk_voice=(s->talk_voice+1)%4;
    }else if(c=='.'||c=='?'||c=='!')s->talk_length=slow?26460:9702;
    else if(c==','||c==':'||c==';')s->talk_length=slow?7938:4410;
    else{
     int wobble=(c*97+s->talk_char*31)%900;
     s->talk_length=(slow?4700:2750)+wobble*(slow?2:1);
     s->talk_vowel=c=='a'?0:c=='e'?1:c=='i'||c=='y'?2:c=='o'?3:c=='u'?4:1;
     s->talk_pitch=(slow?118:205)+s->talk_voice*(slow?13:24)+(c%7)*(slow?3:6);
     s->talk_end=s->talk_pitch+(((c+s->talk_char)&3)-1)*(slow?5:11);
     if(strchr("cfhkqspxt",c))s->talk_noise_mix=3;
     else if(!strchr("aeiouylmnrwbdgvjz0123456789",c))s->talk_noise_mix=1;
    }
    radio_voice_station=s->station;radio_voice_line=s->talk_line;
    radio_voice_char=s->talk_char;
   }
  }
  if(s->talk_letter==' '||s->talk_letter=='.'||s->talk_letter=='?'||
     s->talk_letter=='!'||s->talk_letter==','||s->talk_letter==':'||s->talk_letter==';'){
   s->talk_age++;radio_voice_active=0;
   if(left)*left=0;if(right)*right=0;return;
  }
  {
   int age=s->talk_age++,left_in=s->talk_length-s->talk_age;
   int edge=s->station==6?520:320,envelope=age<edge?age:edge;
   if(left_in<envelope)envelope=left_in;
   if(envelope<0)envelope=0;
   int hz=s->talk_pitch+(s->talk_end-s->talk_pitch)*age/s->talk_length;
   s->chatter_phase+=(uint32_t)hz*97391u;
   s->chatter_phase_b+=(uint32_t)(hz*(2+s->talk_vowel))*97391u;
   s->noise=s->noise*1664525u+1013904223u;
   int fundamental=radio_triangle(s->chatter_phase);
   int formant=radio_triangle(s->chatter_phase_b);
   /* A restrained breath component distinguishes consonants without the
    * sample-to-sample crackle that made the old generated speech abrasive. */
   int hiss=((int)(s->noise>>24)-128)*s->talk_noise_mix/4;
   int value=(fundamental*(6-s->talk_vowel)+formant*(2+s->talk_vowel)+hiss)*envelope/edge;
   if(s->station==6)value=(value*3+radio_triangle(s->chatter_phase/2u)*2)/5;
   int pan=s->talk_voice&1;
   int l=value*(pan?3:4),r=value*(pan?4:3);
   radio_voice_active=envelope>edge/4;
   if(l>18000)l=18000;else if(l<-18000)l=-18000;
   if(r>18000)r=18000;else if(r<-18000)r=-18000;
   if(left)*left=l;if(right)*right=r;return;
  }
 }
 if(s->remaining<=0)radio_sequence_step(s);
 --s->remaining;
 for(i=0;i<7;++i){
  RadioVoice *v=&s->voice[i];
  int wave,value,difference;
  if(v->gate>0){if(--v->gate==0)v->target=0;}
  difference=v->target-v->envelope;
  /* Shift with explicit sign keeps release symmetric and avoids relying
   * on implementation-defined negative right shifts. Snap the last unit. */
  int slew=(i<3||i>=5)?2048:256;
  if(difference>0)v->envelope+=(difference+slew-1)/slew;
  else if(difference<0)v->envelope-=((-difference)+slew-1)/slew;
  v->phase+=v->increment;
  wave=radio_triangle(v->phase);
  if(s->station!=2){int magnitude=wave<0?-wave:wave;wave=wave*(512-magnitude)/256;}
  if(i<3||i>=5){
   v->phase_b+=v->increment+v->increment/900u;
   int second=radio_triangle(v->phase_b),magnitude=second<0?-second:second;
   second=second*(512-magnitude)/256;wave=(wave*3+second)/4;
  }
  if(s->station==2&&i==4){
   /* Softened pulse for an arcade lead, without hard full-scale edges. */
   wave=(wave+((v->phase&0x80000000u)?-192:192))/2;
  }else if((s->station==1&&i<3)||(s->station==4&&i==4)){
   wave=(wave*3+radio_triangle(v->phase*2u))/4;
  }
  value=wave*(v->envelope>>8)/256;
  if(i==0){l+=value;r+=value/2;}
  else if(i==2){l+=value/2;r+=value;}
  else if(i==4){l+=value*3/4;r+=value;}
  else {l+=value;r+=value;}
 }
 s->noise=s->noise*1664525u+1013904223u;
 n=(int)(s->noise>>24)-128;
 high=n-s->previous_noise;s->previous_noise=n;
 if(s->kick>0){
  s->kick_phase+=s->kick_increment;
  if(s->kick_increment>radio_note_increment(28))s->kick_increment-=460u;
  percussion+=radio_triangle(s->kick_phase)*(s->kick>>8)/256;
  s->kick-=s->station==4?28:48;
  if(s->kick<0)s->kick=0;
 }
 if(s->snare>0){
  percussion+=high*(s->snare>>8)/256;
  s->snare-=42;if(s->snare<0)s->snare=0;
 }
 if(s->hat>0){
  int hat=high*(s->hat>>8)/256;
  l+=hat;r+=hat/2;
  s->hat-=28;if(s->hat<0)s->hat=0;
 }
 l+=percussion;r+=percussion;
 /* Far Horizons carries a low, deliberately unintelligible alien chatter bed. */
 if(s->station==4&&(((s->step>>3)&15)==3||((s->step>>3)&15)==11)){int chatter=(high*3+n)/4;l+=chatter*3;r+=chatter*2;}
 /* Bounded feedback taps add space without allocations or another thread. */
 int dl=s->delay_l[(s->delay_pos+8192-6615)&8191],dr=s->delay_r[(s->delay_pos+8192-7938)&8191];
 s->filter_l+=(l-s->filter_l)/4;s->filter_r+=(r-s->filter_r)/4;
 int wet=s->station==0||s->station==4?3:2;
 l=s->filter_l+dl*wet/5;r=s->filter_r+dr*wet/5;
 s->delay_l[s->delay_pos]=(short)(s->filter_l/2+dr/3);
 s->delay_r[s->delay_pos]=(short)(s->filter_r/2+dl/3);
 s->delay_pos=(s->delay_pos+1)&8191;
 /* Music-only gain. SFX output gain and saved volume choices stay unchanged. */
 l*=4;r*=4;
 /* Defensive final bound leaves ample headroom for the caller's SFX. */
 if(l>18000)l=18000;else if(l<-18000)l=-18000;
 if(r>18000)r=18000;else if(r<-18000)r=-18000;
 if(left)*left=l;
 if(right)*right=r;
}
#endif
