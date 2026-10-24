#include <pspkernel.h>
#include <stdio.h>
#include <string.h>
PSP_MODULE_INFO("AudioPanRegression",0,1,0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);
enum { SFX_NONE, SFX_UI, SFX_LASER, SFX_HIT, SFX_WARP, SFX_SCAN, SFX_LAND, SFX_MINE, SFX_BOOST, SFX_COMM, SFX_DOCK, SFX_MISSILE, SFX_ALERT, SFX_DEATH, SFX_SELECT, SFX_TALK, SFX_OBSERVATORY, SFX_ALIEN_PLAYER, SFX_ALIEN_FIRE, SFX_ALIEN_MISSILE, SFX_ALIEN_KILL, SFX_SPEED_SURGE, SFX_PAPER, SFX_POWER_BUS, SFX_POWER_RELAY, SFX_POWER_READY, SFX_AD_OPEN, SFX_AD_CLOSE };
#include "../src/generated/audio-flight-pcm.h"
#include "../src/audio-sfx.h"
#include "../src/audio-mixer.h"
int main(void){FILE *f=fopen("pan-check.txt","w");int fail=0;long count=0,overflows=0;int peak=0;
for(int sample=-32768;sample<=32767;sample+=31)for(int gain=0;gain<=1000;gain+=25)for(int pan=0;pan<=200;pan+=10){int expected=(int)((long long)sample*gain*pan/100000);if(audio_pan_sample(sample,gain,pan)!=expected)fail++;count++;}
for(int id=1;id<=SFX_AD_CLOSE;id++){unsigned phase=0,noise=0x7141;for(int age=0;age<audio_sfx_length(id);age++){int v=audio_sfx_sample(id,age,&phase,&noise);if(v>peak)peak=v;if(-v>peak)peak=-v;if((long long)v*1000*200>2147483647LL||(long long)v*1000*200<-2147483648LL)overflows++;}}
fprintf(f,"PASS %ld full-range PCM/gain/pan combinations agree with wide reference\nINFO authored SFX peak %d, vulnerable full-pan samples %ld\nRESULT %d failures\n",count,peak,overflows,fail);fclose(f);sceKernelExitGame();return 0;}
