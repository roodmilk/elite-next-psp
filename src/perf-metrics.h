#include <pspkernel.h>

typedef struct {
 int samples,slow_frames;
 unsigned worst_frame_us;
 int min_free_mem,min_free_block;
} PerfMetrics;

static void perf_metrics_reset(PerfMetrics *p){
 p->samples=0;p->slow_frames=0;p->worst_frame_us=0;
 p->min_free_mem=0x7fffffff;p->min_free_block=0x7fffffff;
}
static void perf_metrics_sample_memory(PerfMetrics *p){
 int free_mem=sceKernelTotalFreeMemSize(),free_block=sceKernelMaxFreeMemSize();
 if(free_mem>=0&&free_mem<p->min_free_mem)p->min_free_mem=free_mem;
 if(free_block>=0&&free_block<p->min_free_block)p->min_free_block=free_block;
}
static void perf_metrics_sample_frame(PerfMetrics *p,float seconds){
 unsigned us=seconds>0? (unsigned)(seconds*1000000.f):0;
 p->samples++;if(us>p->worst_frame_us)p->worst_frame_us=us;if(us>25000)p->slow_frames++;
}
