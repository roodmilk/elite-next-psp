#include <pspgu.h>

/* Keep GU limited to full-buffer clears; the established software renderer owns all other pixels. */
#define GU_ACCEL_LIST_WORDS 16384
static unsigned gu_accel_list[GU_ACCEL_LIST_WORDS] __attribute__((aligned(64)));
static int gu_accel_ready=0;

static void gu_accel_init(void){
 if(gu_accel_ready)return;
 if(sceGuInit()<0)return;
 sceGuStart(GU_DIRECT,gu_accel_list);
 sceGuDrawBuffer(GU_PSM_8888,(void*)0,512);
 sceGuScissor(0,0,480,272);
 sceGuEnable(GU_SCISSOR_TEST);
 sceGuFinish();sceGuSync(GU_SYNC_FINISH,GU_SYNC_WHAT_DONE);
 gu_accel_ready=1;
}
static void gu_accel_stop(void){if(gu_accel_ready){sceGuTerm();gu_accel_ready=0;}}
static void gu_accel_recover(void){gu_accel_stop();gu_accel_init();}
static int gu_accel_clear(int buffer,unsigned color){
 if(!gu_accel_ready)return 0;
 sceGuStart(GU_DIRECT,gu_accel_list);
 sceGuDrawBuffer(GU_PSM_8888,(void*)((unsigned)buffer*512*272*4),512);
 sceGuScissor(0,0,480,272);sceGuEnable(GU_SCISSOR_TEST);
 sceGuClearColor(color);sceGuClear(GU_COLOR_BUFFER_BIT);
 sceGuFinish();sceGuSync(GU_SYNC_FINISH,GU_SYNC_WHAT_DONE);
 return 1;
}
