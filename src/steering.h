#ifndef STEERING_H
#define STEERING_H
#include <pspctrl.h>
/* Require a neutral sample before enabling analog steering after launch. */
static int stick_ready(int valid,int x,int y,int *ready){
 if(!valid)return 0;
 if(x>=96&&x<=160&&y>=96&&y<=160)*ready=1;
 return *ready;
}
static float stick_axis(int value) {
 int delta=value-128;
 const int deadzone=32;
 if(delta>=-deadzone&&delta<=deadzone)return 0;
 return delta>0?(delta-deadzone)/95.f:(delta+deadzone)/96.f;
}
static float stick_axis_centered(int value,int center){
 int delta=value-center;const int deadzone=36;
 if(delta>=-deadzone&&delta<=deadzone)return 0;
 int span=delta>0?255-center-deadzone:center-deadzone;
 if(span<1)span=1;
 float out=(delta>0?delta-deadzone:delta+deadzone)/(float)span;
 return out>1?1:out<-1?-1:out;
}
static void steering_axes_centered(int valid,unsigned buttons,int x,int y,int analog,int cx,int cy,float *turn,float *pitch){
 *turn=*pitch=0;if(!valid)return;
 if(analog){*turn=stick_axis_centered(x,cx);*pitch=-stick_axis_centered(y,cy);}
 if(buttons&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT))*turn=((buttons&PSP_CTRL_RIGHT)?1.f:0)-((buttons&PSP_CTRL_LEFT)?1.f:0);
 if(buttons&(PSP_CTRL_UP|PSP_CTRL_DOWN))*pitch=((buttons&PSP_CTRL_UP)?1.f:0)-((buttons&PSP_CTRL_DOWN)?1.f:0);
}
static void steering_axes(int valid,unsigned buttons,int x,int y,int analog,float *turn,float *pitch) {
 *turn=*pitch=0;
 if(!valid)return;
 if(analog){*turn=stick_axis(x);*pitch=-stick_axis(y);}
 /* Deliberate D-pad input overrides the corresponding analog axis. */
 if(buttons&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT))
  *turn=((buttons&PSP_CTRL_RIGHT)?1.f:0)-((buttons&PSP_CTRL_LEFT)?1.f:0);
 if(buttons&(PSP_CTRL_UP|PSP_CTRL_DOWN))
  *pitch=((buttons&PSP_CTRL_UP)?1.f:0)-((buttons&PSP_CTRL_DOWN)?1.f:0);
}
#endif
