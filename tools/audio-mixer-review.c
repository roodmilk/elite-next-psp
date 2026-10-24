#include <stdio.h>
#include <string.h>
enum { SFX_NONE, SFX_UI, SFX_LASER, SFX_HIT, SFX_WARP, SFX_SCAN, SFX_LAND, SFX_MINE, SFX_BOOST, SFX_COMM, SFX_DOCK, SFX_MISSILE, SFX_ALERT, SFX_DEATH, SFX_SELECT, SFX_TALK, SFX_OBSERVATORY, SFX_ALIEN_PLAYER, SFX_ALIEN_FIRE, SFX_ALIEN_MISSILE, SFX_ALIEN_KILL, SFX_SPEED_SURGE, SFX_PAPER, SFX_POWER_BUS, SFX_POWER_RELAY, SFX_POWER_READY, SFX_AD_OPEN, SFX_AD_CLOSE };
#include "../src/audio-sfx.h"
#include "../src/audio-mixer.h"
static int failures=0;
#define CHECK(ok,text) do{printf("%s %s\n",(ok)?"PASS":"FAIL",text);if(!(ok))failures++;}while(0)
int main(void){
 AudioVoice voices[AUDIO_EVENT_VOICES];memset(voices,0,sizeof(voices));
 for(int i=0;i<AUDIO_EVENT_VOICES;i++)audio_command_push(SFX_UI,1000,(i&1)?-100:100);
 int serial=audio_voice_commands(voices,1),active=0;for(int i=0;i<AUDIO_EVENT_VOICES;i++)active+=voices[i].id!=0;
 CHECK(active==AUDIO_EVENT_VOICES,"eight events coexist");
 CHECK(audio_voice_peak==0,"peak counter starts clean");
 audio_command_push(SFX_ALERT,1000,-100);serial=audio_voice_commands(voices,serial);
 int alerts=0,ui=0;for(int i=0;i<AUDIO_EVENT_VOICES;i++){alerts+=voices[i].id==SFX_ALERT;ui+=voices[i].id==SFX_UI;}
 CHECK(alerts==1&&ui==AUDIO_EVENT_VOICES-1,"critical warning steals one low-priority voice");
 CHECK(audio_voice_steals==1,"voice stealing is counted deterministically");
 int bus[AUDIO_BUS_COUNT]={1000,1000,1000,1000},left=0,right=0;audio_voice_mix(voices,bus,&left,&right);
 CHECK(left!=right,"left/right panning reaches the stereo mix");
 CHECK(audio_voice_peak==AUDIO_EVENT_VOICES,"concurrency measurement records the peak");
 printf("RESULT %d failures\n",failures);return failures?1:0;
}
