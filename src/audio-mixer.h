#ifndef ELITE_AUDIO_MIXER_H
#define ELITE_AUDIO_MIXER_H

/* Small, allocation-free event mixer. The main thread only appends commands;
 * the audio worker owns every voice and drains the queue once per output block. */
#define AUDIO_EVENT_VOICES 8
#define AUDIO_COMMANDS 64 /* Bounded burst headroom while a music file/codec opens. */
enum { AUDIO_BUS_SHIP, AUDIO_BUS_WORLD, AUDIO_BUS_WARNING, AUDIO_BUS_UI, AUDIO_BUS_COUNT };
typedef struct { unsigned char id,priority,bus; signed char pan; unsigned short gain; } AudioCommand;
typedef struct {
 int id,age,length,priority,bus,pan,gain,target_gain,serial;
 unsigned phase,noise;
} AudioVoice;
static volatile AudioCommand audio_command[AUDIO_COMMANDS];
static volatile unsigned audio_command_write=0,audio_command_read=0,audio_command_drops=0,audio_voice_rejects=0;
static volatile unsigned audio_voice_steals=0,audio_voice_peak=0;

static int audio_event_priority(int id){
 switch(id){
 case SFX_ALERT:case SFX_MISSILE:return 100;
 case SFX_DEATH:case SFX_HIT:return 90;
 case SFX_LASER:case SFX_ALIEN_PLAYER:case SFX_ALIEN_FIRE:case SFX_ALIEN_MISSILE:return 82;
 case SFX_BOOST:case SFX_SPEED_SURGE:case SFX_WARP:return 74;
 case SFX_DOCK:case SFX_LAND:case SFX_SCAN:case SFX_MINE:return 58;
 case SFX_COMM:case SFX_TALK:return 42;
 default:return 24;
 }
}
static int audio_event_bus(int id){
 if(id==SFX_ALERT||id==SFX_MISSILE||id==SFX_DEATH||id==SFX_HIT)return AUDIO_BUS_WARNING;
 if(id==SFX_BOOST||id==SFX_SPEED_SURGE||id==SFX_WARP||id==SFX_DOCK||id==SFX_LAND)return AUDIO_BUS_SHIP;
 if(id==SFX_LASER||id==SFX_ALIEN_PLAYER||id==SFX_ALIEN_FIRE||id==SFX_ALIEN_MISSILE||id==SFX_ALIEN_KILL||id==SFX_MINE)return AUDIO_BUS_WORLD;
 return AUDIO_BUS_UI;
}
static void audio_command_push(int id,int gain,int pan){
 unsigned write=audio_command_write,next=(write+1u)%AUDIO_COMMANDS;
 if(next==audio_command_read){audio_command_drops++;return;}
 AudioCommand command={(unsigned char)id,(unsigned char)audio_event_priority(id),(unsigned char)audio_event_bus(id),(signed char)(pan<-100?-100:pan>100?100:pan),(unsigned short)(gain<0?0:gain>1000?1000:gain)};
 audio_command[write]=command;audio_command_write=next;
}
static int audio_voice_choose(AudioVoice *voice,int priority){
 int free_slot=-1,best=-1,best_priority=1000,best_serial=0x7fffffff;
 for(int i=0;i<AUDIO_EVENT_VOICES;i++){
  if(!voice[i].id){free_slot=i;break;}
  if(voice[i].priority<best_priority||(voice[i].priority==best_priority&&voice[i].serial<best_serial)){best=i;best_priority=voice[i].priority;best_serial=voice[i].serial;}
 }
 if(free_slot>=0)return free_slot;
 if(best>=0&&best_priority<=priority){audio_voice_steals++;return best;}
 audio_voice_rejects++;return -1;
}
static void audio_voice_start(AudioVoice *voice,AudioCommand command,int serial){
 int slot=audio_voice_choose(voice,command.priority);if(slot<0)return;
 AudioVoice *v=&voice[slot];v->id=command.id;v->age=0;v->length=audio_sfx_length(command.id);v->priority=command.priority;v->bus=command.bus;v->pan=command.pan;v->gain=0;v->target_gain=command.gain;v->serial=serial;v->phase=0;v->noise=0x7141u^(unsigned)(serial*2654435761u);
}
static int audio_voice_commands(AudioVoice *voice,int serial){
 while(audio_command_read!=audio_command_write){unsigned read=audio_command_read;AudioCommand command=audio_command[read];audio_command_read=(read+1u)%AUDIO_COMMANDS;audio_voice_start(voice,command,serial++);}
 return serial;
}
/* Divide before applying pan: sample*gain*200 exceeds signed 32-bit
 * range for perfectly valid full-level PCM. Preserve sub-LSB precision
 * in a wide intermediate without wrapping into an audible polarity click. */
static int audio_pan_sample(int sample,int gain,int pan_gain){
 return (int)((long long)sample*gain*pan_gain/100000);
}
static void audio_voice_mix(AudioVoice *voice,int bus_gain[AUDIO_BUS_COUNT],int *left,int *right){
 int active=0,l=0,r=0;
 for(int i=0;i<AUDIO_EVENT_VOICES;i++){
  AudioVoice *v=&voice[i];if(!v->id)continue;active++;
  int remain=v->length-v->age;if(remain<=0){v->id=0;continue;}
  int target=remain<128?v->target_gain*remain/128:v->target_gain;
  int step=target>v->gain?8:-8;if((step>0&&v->gain+step>target)||(step<0&&v->gain+step<target))v->gain=target;else v->gain+=step;
  int sample=audio_sfx_sample(v->id,v->age++,&v->phase,&v->noise);
  int gain=v->gain*bus_gain[v->bus]/1000;
  l+=audio_pan_sample(sample,gain,100-v->pan);r+=audio_pan_sample(sample,gain,100+v->pan);
 }
 if((unsigned)active>audio_voice_peak)audio_voice_peak=(unsigned)active;*left=l;*right=r;
}
#endif
