/* One bounded authored station plate. Native 1x; no gameplay or state ownership. */
#ifndef ELITE_STATION_AUTHORED_ART_H
#define ELITE_STATION_AUTHORED_ART_H

static void sc_illust_authored_arrivals(int x,int y,int w,int h){
 /* Second Shift palette and staging: one berth aperture, one board, one rail. */
 const unsigned deep=RGB(8,13,24),ink=RGB(21,28,39),slate=RGB(41,54,70);
 const unsigned olive=RGB(90,96,76),rust=RGB(139,75,55),ochre=RGB(193,139,77);
 const unsigned cream=RGB(229,210,163),cyan=RGB(85,212,212),amber=RGB(240,180,91);
 rect(x,y,w,h,deep);
 rect(x,y,w,34,ink);
 rect(x+8,y+8,w-16,2,ochre);
 rect(x+28,y+13,58,3,slate);rect(x+220,y+13,82,3,slate);
 for(int i=0;i<4;i++){
  int lx=x+42+i*78;
  rect(lx,y+18,18,2,(i&1)?amber:cream);
  pixel(lx+8,y+17,cyan);
 }
 /* Deep berth aperture with a warm practical and sparse navigation lights. */
 rect(x+24,y+42,194,74,slate);rect(x+28,y+46,186,66,deep);
 rect(x+28,y+46,186,3,cream);rect(x+28,y+109,186,3,rust);
 for(int i=0;i<10;i++){
  int sx=x+34+(i*31)%174, sy=y+55+(i*17)%47;
  pixel(sx,sy,(i%4==0)?amber:(i%3==0)?cyan:mix_rgb(cream,deep,.35f));
 }
 /* One readable freighter silhouette, held in the aperture. */
 int fx=x+86+(int)(fmodf(game.time*9.0f,72.0f));
 if(fx>x+156)fx=x+86;
 int fy=y+78+(int)(sinf(game.time*.8f)*2);
 rect(fx,fy,54,8,olive);rect(fx+10,fy-5,24,5,mix_rgb(olive,cream,.22f));
 rect(fx+44,fy+2,10,4,rust);pixel(fx+4,fy+2,amber);pixel(fx+49,fy+3,cyan);
 /* Arrival board: the same warm/cyan hierarchy as the Second Shift bar. */
 rect(x+238,y+42,76,74,rust);rect(x+242,y+46,68,66,ink);
 text((x+248)/8,(y+51)/8,cream,"ARRIVALS");
 for(int i=0;i<4;i++){
  unsigned row=(i==1)?amber:(i&1)?cyan:ochre;
  rect(x+248,y+65+i*10,48,2,row);rect(x+300,y+65+i*10,6,2,(i==1)?cream:slate);
 }
 rect(x+248,y+103,30,3,amber);rect(x+282,y+103,24,3,cyan);
 /* Foreground service rail and floor planes give the room a stable walk lane. */
 rect(x+20,y+126,w-40,14,slate);rect(x+20,y+126,w-40,3,cream);
 rect(x+20,y+140,w-40,2,olive);rect(x+20,y+153,w-40,2,rust);
 rect(x+46,y+146,84,3,ochre);rect(x+178,y+146,94,3,ochre);
 rect(x+38,y+132,8,8,amber);rect(x+w-48,y+132,8,8,cyan);
 /* Contact-free focal cue: people remain drawn by station-crawl. */
 rect(x+226,y+124,78,2,mix_rgb(cream,deep,.35f));
}

#endif
