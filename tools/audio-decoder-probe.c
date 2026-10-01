/* Silent PSP decoder regression: never opens an audio device. */
#include <pspkernel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifdef PROBE_GAME
#include "../src/game.h"
#include "../src/audio.h"
#elif defined(PROBE_TREMOR)
#include <tremor/ivorbisfile.h>
#else
#include <vorbis/vorbisfile.h>
#endif
PSP_MODULE_INFO("SilentAudioProbe",0,1,0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER|THREAD_ATTR_VFPU);
PSP_MAIN_THREAD_STACK_SIZE_KB(128);
static short pcm[4096];
int main(void){
 FILE *report=fopen("probe.txt","w"),*out=fopen("decoded.pcm","wb");
 if(!report||!out){sceKernelExitGame();return 1;}
 unsigned long long start=sceKernelGetSystemTimeWide();
 int channels=2,rate=44100;long samples=0;int ok=1;
#ifdef PROBE_GAME
 radio_track_count[RADIO_BATTLE_SOURCE]=1;
 snprintf(radio_tracks[RADIO_BATTLE_SOURCE][0],RADIO_TRACK_PATH,"input.ogg");
 for(int i=0;i<44100*8;i++){int l=0,r=0;if(!mp3_sample(RADIO_BATTLE_SOURCE,&l,&r)){ok=0;break;}pcm[0]=l;pcm[1]=r;fwrite(pcm,2,2,out);samples+=2;}
 mp3_close_track();
#else
 OggVorbis_File vf;
 if(ov_fopen("input.ogg",&vf)<0)ok=0;
 else{
  vorbis_info *info=ov_info(&vf,-1);channels=info->channels;rate=info->rate;
  while(samples<rate*channels*8){int section=0;long bytes;
#ifdef PROBE_TREMOR
   bytes=ov_read(&vf,(char*)pcm,sizeof(pcm),&section);
#else
   bytes=ov_read(&vf,(char*)pcm,sizeof(pcm),0,2,1,&section);
#endif
   if(bytes==0)break;if(bytes<0){ok=0;break;}fwrite(pcm,1,bytes,out);samples+=bytes/2;
  }
  ov_clear(&vf);
 }
#endif
 fclose(out);fprintf(report,"ok=%d rate=%d channels=%d samples=%ld microseconds=%llu\n",ok,rate,channels,samples,sceKernelGetSystemTimeWide()-start);fclose(report);
 sceKernelExitGame();return 0;
}
