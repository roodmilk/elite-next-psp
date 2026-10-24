#include "dashboard-assets.h"
static unsigned dashboard_draw_calls;static uint64_t dashboard_draw_us;
/* Static approved shelf artwork. Computers own this region when expanded.
   Draw over the physical canopy edge, but never over its Comms key or a computer. */
static void dashboard_draw(void){
 int slot=cosmetic_installed(&game,DASHBOARD_FIRST);if(slot<0||!hc_presentation_context()||third_person||!hc_full_console_visible()||hc_browser)return;
 int item=game.fit[slot]-DASHBOARD_FIRST;if(item<0||item>=16)return;uint64_t start=sceKernelGetSystemTimeWide();
 rect(0,145,26,5,RGB(25,39,47));line(0,145,25,145,RGB(119,145,153));line(0,149,25,149,RGB(8,16,25));
 for(int y=0;y<32;y++)for(int x=0;x<24;x++){
  unsigned c=dashboard_pixels[item][y*24+x],a=c>>24;if(!a)continue;int px=dashboard_xy[item][0]+x,py=dashboard_xy[item][1]+y;
  if(a==255){pixel(px,py,c);continue;}unsigned b=fb[py*STRIDE+px],out=0xff000000u;
  for(int k=0;k<24;k+=8)out|=((((c>>k)&255)*a+((b>>k)&255)*(255-a)+127)/255)<<k;pixel(px,py,out);
 }
 dashboard_draw_calls++;dashboard_draw_us+=sceKernelGetSystemTimeWide()-start;
}
