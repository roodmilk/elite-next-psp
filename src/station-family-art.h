/* Native authored proof-room scenes. Each payload is baked at 1x for the
 * existing 340x168 station viewport; this header owns no gameplay state. */
#ifndef STATION_FAMILY_ART_H
#define STATION_FAMILY_ART_H
#include "station-bar-art.h"
#include "generated/qube-freight-canteen-pixels.h"
#include "generated/xequerin-research-canteen-pixels.h"
enum { STATION_FAMILY_QUBE=1, STATION_FAMILY_XEQUERIN=2 };
static void sc_illust_native_family(const unsigned char *pixels,int x,int y,int w,int h){
 int rw=w<340?w:340,rh=h<168?h:168;
 const unsigned *palette=high_contrast?second_shift_contrast_palette:second_shift_palette;
 for(int py=0;py<rh;py++)for(int px=0;px<rw;px++){
  int xx=x+px,yy=y+py;if(xx<0||xx>=480||yy<0||yy>=272)continue;
  unsigned packed=pixels[(py*340+px)/2];
  unsigned index=(px&1)?packed&15:packed>>4;
  pixel(xx,yy,palette[index]);
 }
}
static void sc_illust_qube_freight(int x,int y,int w,int h){
 sc_illust_native_family(qube_freight_canteen_pixels,x,y,w,h);
}
static void sc_illust_xequerin_research(int x,int y,int w,int h){
 sc_illust_native_family(xequerin_research_canteen_pixels,x,y,w,h);
}
/* Scene-relative top-left52x64 origins for the painted inhabitants. */
static void station_family_person_pos(int family,int i,int *x,int *y){
 static const unsigned char qube[3][2]={{24,28},{154,60},{246,70}};
 static const unsigned char xequerin[3][2]={{26,64},{150,66},{246,76}};
 const unsigned char (*positions)[2]=family==STATION_FAMILY_XEQUERIN?xequerin:qube;
 if(i<0||i>2){*x=*y=0;return;}
 *x=positions[i][0];*y=positions[i][1];
}
#endif
