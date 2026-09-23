/* Direct-native Arrivals room candidate. Scene/UI ownership stays with Station. */
#ifndef ELITE_STATION_ARRIVALS_AUTHORED_ART_H
#define ELITE_STATION_ARRIVALS_AUTHORED_ART_H
#include "station-art-style.h"
#include "generated/station-arrivals-authored-pixels.h"

static const unsigned station_arrivals_authored_palette[11]={
 ART_VOID,ART_CHARCOAL,ART_SLATE,ART_OLIVE,ART_RUST,ART_OCHRE,
 ART_CREAM,ART_LAVENDER,ART_CYAN,ART_AMBER,ART_DANGER
};

static void station_arrivals_authored_draw(int x,int y,int w,int h){
 int rw=w<STATION_ART_ROOM_W?w:STATION_ART_ROOM_W;
 int rh=h<STATION_ART_ROOM_H?h:STATION_ART_ROOM_H;
 for(int py=0;py<rh;py++)for(int px=0;px<rw;px++){
  int xx=x+px,yy=y+py;
  if(xx<0||xx>=480||yy<0||yy>=272)continue;
  unsigned packed=station_arrivals_authored_pixels[(py*STATION_ART_ROOM_W+px)>>1];
  unsigned index=(px&1)?(packed&15):(packed>>4);
  pixel(xx,yy,station_arrivals_authored_palette[index]);
 }
}

#endif
