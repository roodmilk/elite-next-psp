/* Original native-pixel retrofuturist composition. No texture, heap or Game writes.
 * Only the arrivals backdrop belongs here; existing people/doors/UI draw above it.
 * Coordinates are authored for the canonical 340x168 room, not a scalable mockup. */
#ifndef ELITE_STATION_ARRIVALS_ART_H
#define ELITE_STATION_ARRIVALS_ART_H
#define ARRIVALS_COVER_ART 1
static void ar_box(int ox,int oy,int x,int y,int w,int h,unsigned ink){
 int right=x+w,bottom=y+h;if(x<0)x=0;if(y<0)y=0;
 if(right>340)right=340;if(bottom>168)bottom=168;
 if(right>x&&bottom>y)rect(ox+x,oy+y,right-x,bottom-y,ink);
}
static void ar_quad(int ox,int oy,const short p[8],unsigned ink){
 int ymin=168,ymax=0;for(int i=0;i<4;i++){if(p[i*2+1]<ymin)ymin=p[i*2+1];if(p[i*2+1]>ymax)ymax=p[i*2+1];}
 if(ymin<0)ymin=0;if(ymax>168)ymax=168;
 for(int y=ymin;y<ymax;y++){
  int left=340,right=0,hits=0;
  for(int i=0;i<4;i++){
   int j=(i+1)%4,ax=p[i*2],ay=p[i*2+1],bx=p[j*2],by=p[j*2+1];
   if((ay<=y&&by>y)||(by<=y&&ay>y)){
    int x=ax+(y-ay)*(bx-ax)/(by-ay);if(x<left)left=x;if(x>right)right=x;hits++;
   }
  }
  if(hits>1)ar_box(ox,oy,left,y,right-left+1,1,ink);
 }
}
static void sc_arrivals_cover_art(int x,int y,int w,int h){
 if(w!=340||h!=168)return;
 #define AB(X,Y,W,H,C) ar_box(x,y,X,Y,W,H,C)
 #define AQ(A,B,C,D,E,F,G,H,I) do{const short points[8]={A,B,C,D,E,F,G,H};ar_quad(x,y,points,I);}while(0)
 /* Massive vaulted ribs surround a quiet aperture. No invented travel signage. */
 AB(0,0,340,168,SC_CHAR);AB(0,0,340,112,SC_SLATE);
 AB(61,8,220,99,SC_VOID);
 if(!high_contrast){
  AB(64,62,214,14,SC_CHAR);AB(64,76,214,21,SC_SLATE);
  /* Deliberate sparse pixels, never per-frame random state. */
  unsigned seed=game.bodies[0].seed^0x41525249u;
  for(int i=0;i<24;i++){seed=seed*1664525u+1013904223u;int sx=67+(seed%204);seed=seed*1664525u+1013904223u;int sy=13+seed%61;AB(sx,sy,1,1,i%5?SC_LAV:SC_CREAM);}
 }
 /* Explicit association: the current system's body 1, using the orbit selector.
  * This is a composed window view, not a navigational distance/heading display. */
 const Body *body=&game.bodies[1];
 if(body->type>=1&&body->type<=3)
  draw_planet_sprite(x+237,y+44,30,body->seed,body->type,x+64,y+11,x+278,y+99);
 /* Far docking architecture: asymmetric buttresses and suspended platforms.
  * Silhouettes are scenery, not new explorable stations or landing targets. */
 AQ(72,98,90,48,103,48,104,98,SC_OLIVE);
 AQ(78,96,94,55,97,55,93,96,SC_OCHRE);
 AQ(110,99,121,34,130,34,142,99,SC_CHAR);
 AB(121,31,9,3,SC_CREAM);AB(124,23,2,8,SC_OCHRE);
 AQ(153,98,166,66,170,66,186,98,SC_CHAR);
 AB(68,91,207,5,SC_OLIVE);AB(68,91,207,1,SC_OCHRE);
 /* One long, swept freighter: restrained hull slabs instead of glitter/noise. */
 AQ(105,65,174,55,198,69,132,79,SC_OCHRE);
 AQ(132,79,198,69,184,79,141,85,SC_RUST);
 AQ(106,65,132,79,141,85,116,76,SC_OLIVE);
 AQ(137,57,158,47,178,56,155,63,SC_CREAM);
 AB(155,57,12,3,SC_SLATE);AB(128,69,4,2,SC_VOID);
 AB(141,67,6,2,SC_VOID);AB(155,65,6,2,SC_VOID);
 AB(177,67,11,2,SC_CREAM);AB(114,72,5,2,SC_CYAN);
 /* Rib profiles echo organic shells without moving any side-hatch anchors. */
 AQ(0,0,43,0,78,29,64,46,SC_OLIVE);
 AQ(0,0,32,0,70,26,64,32,SC_OCHRE);
 AQ(25,0,40,0,74,28,69,32,SC_CREAM);
 AQ(340,0,304,0,273,27,283,44,SC_RUST);
 AQ(340,0,314,0,278,28,274,22,SC_OCHRE);
 AQ(340,0,329,0,282,28,279,23,SC_CREAM);
 AB(58,28,5,79,SC_OLIVE);AB(62,33,2,74,SC_CREAM);
 AB(279,30,6,77,SC_OLIVE);AB(278,34,2,73,SC_OCHRE);
 AB(63,102,216,5,SC_OCHRE);AB(67,104,208,3,SC_CREAM);
 /* Large, clear floor planes; the original person/door silhouettes remain above. */
 AB(0,108,340,60,SC_OLIVE);
 AQ(61,110,279,110,340,168,0,168,SC_RUST);
 AQ(79,111,143,111,129,168,18,168,SC_OCHRE);
 AQ(197,111,230,111,278,168,220,168,SC_CHAR);
 AQ(150,111,155,111,150,168,144,168,SC_CREAM);
 AB(0,156,340,12,SC_CHAR);
 /* Fixed logical traffic-board anchor; abstract static marks are not fake jobs. */
 AB(196,84,90,40,SC_CHAR);AB(196,84,90,2,SC_CREAM);
 text((x+204)/8,(y+90)/8,SC_CYAN,"BERTHS");
 for(int i=0;i<5;i++){AB(202+i*15,103,10,2,SC_OLIVE);AB(202+i*15,108,6,2,SC_LAV);}
 AB(201,116,75,1,SC_OCHRE);
 /* Existing rail anchor and steady practical lamps. No playback/alarm claims. */
 AB(72,118,160,4,SC_OCHRE);AB(72,118,160,1,SC_CREAM);
 AB(72,122,160,3,SC_CHAR);AB(79,125,4,21,SC_CHAR);
 AB(222,125,4,23,SC_CHAR);
 AB(7,8,25,3,SC_AMBER);AB(308,8,25,3,SC_AMBER);
 #undef AQ
 #undef AB
}
#endif
