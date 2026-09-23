/* Second Shift preview art. Read-only, native1x; caller owns location and UI.
 * Original generated illustration, native palette export. No gameplay state.
 * Scene includes the three people: caller must not draw generic NPC sprites
 * or generic hatch artwork over this room. Keep options and SHIP overlays.
 */
#ifndef STATION_BAR_ART_H
#define STATION_BAR_ART_H
#include "generated/second-shift-pixels.h"
static const unsigned second_shift_palette[11]={
 RGB(8,13,24),RGB(21,28,39),RGB(41,54,70),RGB(90,96,76),
 RGB(139,75,55),RGB(193,139,77),RGB(229,210,163),RGB(155,154,165),
 RGB(85,212,212),RGB(240,180,91),RGB(200,90,75)
};
/* Contrast keeps the painted silhouette and landmark clusters, but collapses
 * low-value material shades into the readable PSP roles. */
static const unsigned second_shift_contrast_palette[11]={
 RGB(8,13,24),RGB(8,13,24),RGB(41,54,70),RGB(41,54,70),
 RGB(229,210,163),RGB(229,210,163),RGB(229,210,163),RGB(155,154,165),
 RGB(85,212,212),RGB(240,180,91),RGB(200,90,75)
};
static void sc_illust_second_shift(int x,int y,int w,int h){
 int rw=w<340?w:340,rh=h<168?h:168;
 const unsigned *palette=high_contrast?second_shift_contrast_palette:second_shift_palette;
 for(int py=0;py<rh;py++)for(int px=0;px<rw;px++){
  int xx=x+px,yy=y+py;if(xx<0||xx>=480||yy<0||yy>=272)continue;
  unsigned packed=second_shift_pixels[(py*340+px)/2];
  unsigned index=(px&1)?packed&15:packed>>4;
  pixel(xx,yy,palette[index]);
 }
}
/* Scene-relative top-left52x64 hotspots for the authored characters. */
static void second_shift_person_pos(int i,int *x,int *y){
 static const unsigned char positions[3][2]={{28,30},{155,66},{246,72}};
 if(i<0||i>2){*x=*y=0;return;}
 *x=positions[i][0];*y=positions[i][1];
}
#endif
