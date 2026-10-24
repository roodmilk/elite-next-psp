#ifndef ELITE_EARTH_MEADOW_WORLD_H
#define ELITE_EARTH_MEADOW_WORLD_H
#include <math.h>
#define EARTH_MEADOW_RADIUS 350.f
#define EARTH_MEADOW_TREES 72
#define EARTH_MEADOW_ROCKS 18
#define EARTH_MEADOW_WATER_Y 21.f
#define EARTH_MEADOW_FLOWERS 25
/* A hand-placed, gently winding clue beside the landing apron. Keep this
 * shared with interaction and route tests so the clue is where it appears. */
static inline void earth_meadow_flower(int i,float *x,float *z){
 float t=i/(float)(EARTH_MEADOW_FLOWERS-1);
 *x=78+17*t+22*sinf(t*3.14159265f);*z=-58+106*t;
}
static inline int earth_meadow_near_flowers(float x,float z){
 /* Leave the last 35m to the stone's own interaction. */
 for(int i=0;i<EARTH_MEADOW_FLOWERS-7;i++){float fx,fz;earth_meadow_flower(i,&fx,&fz);if((fx-x)*(fx-x)+(fz-z)*(fz-z)<14*14)return 1;}
 return 0;
}
static inline float meadow_smooth(float t){t=fmaxf(0,fminf(1,t));return t*t*(3-2*t);}
static inline float meadow_edge(float a){return 324+19*sinf(a*3+.7f)+10*cosf(a*5);}
static inline float meadow_cliff(float x){return 290+12*sinf(x*.017f);}
/* One height function feeds the collision lattice and its rendered vertices.
 * The wooded banks enclose the clearing; the north side opens onto a valley. */
static inline float earth_meadow_height(float x,float z){
 float r=hypotf(x,z),a=atan2f(x,z),edge=meadow_edge(a);
 float local=meadow_smooth((r-80)/170)*(8*sinf(x*.014f)+6*sinf(z*.011f));
 float bank=meadow_smooth((r-edge+45)/95)*(64+24*sinf(a*4+.8f));
 bank*=1-meadow_smooth((z-130)/120)*(1-meadow_smooth((fabsf(x)-220)/220));
 float valley=meadow_smooth((z-meadow_cliff(x))/85)*135*(1-meadow_smooth((fabsf(x)-430)/400));
 float far=meadow_smooth((r-550)/1500)*(35*sinf(x*.0016f)+28*cosf(z*.002f));
 float peaks=0;
 const float px[5]={-1900,650,2650,-3600,4300},pz[5]={2600,3600,3100,-2400,-1600};
 for(int i=0;i<5;i++){float q=1-hypotf(x-px[i],z-pz[i])/(1400+i*190);if(q>0)peaks+=q*q*(820+i*115);}
 float y=24+local+bank-valley+far+peaks;
 /* Broad beds make the flat water meet solid banks, never a second warped
  * ground sheet. The water plane itself is depth-tested against this bed. */
 float lake=hypotf((x+174)/78,(z-132)/56);
 float river=fabsf(x-(-174+17*sinf(z*.018f)));
 float cut=1-meadow_smooth((lake-.65f)/.8f);
 if(z>-410&&z<105)cut=fmaxf(cut,(1-meadow_smooth((river-10)/42))*meadow_smooth((105-z)/25));
 return y+(10-y)*cut;
}
static inline int earth_meadow_water(float x,float z){
 float lx=(x+174)/78.f,lz=(z-132)/56.f;
 return lx*lx+lz*lz<1.f||(z>-320&&z<105&&fabsf(x-(-174+17*sinf(z*.018f)))<10);
}
static inline void earth_meadow_tree_make(int i,float *x,float *z){
 float a=i*2.39996323f,r=165+(i%5)*28;
 if(i>=24){a=(i-24)*.1308997f+.023f*sinf(i*5.f);r=meadow_edge(a)+8+(i%3)*18;}
 *x=sinf(a)*r;*z=cosf(a)*r;
 /* Keep the landing apron, orb path, lake and overlook clear. */
 if(*x>45&&*x<155&&*z>-20&&*z<120)*z-=150;
 if((*x-125)*(*x-125)+(*z-245)*(*z-245)<80*80)*x-=150;
 if(earth_meadow_water(*x,*z))*x-=55;
}
static inline void earth_meadow_tree(int i,float *x,float *z){
 static float points[EARTH_MEADOW_TREES][2];static int ready;
 if(!ready){for(int n=0;n<EARTH_MEADOW_TREES;n++)earth_meadow_tree_make(n,&points[n][0],&points[n][1]);ready=1;}
 *x=points[i][0];*z=points[i][1];
}
static inline void earth_meadow_rock_make(int i,float *x,float *z,float *rx,float *rz,float *h){
 float a=i*2.39996323f,r=meadow_edge(a)-8;
 *x=sinf(a)*r;*z=cosf(a)*r;
 if(i<5){static const float ledge[5]={-226,-140,-35,207,285};*x=ledge[i];*z=meadow_cliff(*x)+3+(i%2)*8;}
 *rx=17+(i%4)*5;*rz=14+(i%3)*5;*h=17+(i%5)*6;
}
static inline void earth_meadow_rock(int i,float *x,float *z,float *rx,float *rz,float *h){
 static float points[EARTH_MEADOW_ROCKS][5];static int ready;
 if(!ready){for(int n=0;n<EARTH_MEADOW_ROCKS;n++)earth_meadow_rock_make(n,&points[n][0],&points[n][1],&points[n][2],&points[n][3],&points[n][4]);ready=1;}
 *x=points[i][0];*z=points[i][1];*rx=points[i][2];*rz=points[i][3];*h=points[i][4];
}
static inline int earth_meadow_inside(float x,float z){
 return hypotf(x,z)<meadow_edge(atan2f(x,z))-8&&z<meadow_cliff(x)-18&&!earth_meadow_water(x,z);
}
#endif
