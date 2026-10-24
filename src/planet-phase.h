/* Flight-only sun-driven terminator. Tiny normal LUT; no per-pixel sqrt. */
static int planet_phase_active,planet_phase_cx,planet_phase_cy,planet_phase_ready;
static float planet_phase_inverse,planet_phase_lx,planet_phase_ly,planet_phase_lz;
typedef struct {unsigned char z[33][33];unsigned short gain[256];} PlanetPhaseTables;
static PlanetPhaseTables*planet_phase_tables;
#define planet_phase_z (planet_phase_tables->z)
#define planet_phase_gain (planet_phase_tables->gain)
static void planet_phase_init(void){
 if(planet_phase_ready)return;if(!planet_phase_tables)planet_phase_tables=malloc(sizeof(*planet_phase_tables));if(!planet_phase_tables)return;planet_phase_ready=1;
 for(int y=0;y<33;y++)for(int x=0;x<33;x++){float q=(x*x+y*y)/1024.f;planet_phase_z[y][x]=q<1?(int)(sqrtf(1-q)*255):0;}
 for(int i=0;i<256;i++){float t=(i/255.f*2-1+.08f)*2.3f;if(t<0)t=0;if(t>1)t=1;t=t*t*(3-2*t);planet_phase_gain[i]=(int)((.13f+.87f*t)*256);}
}
static unsigned __attribute__((optimize(ELITE_VISUAL_OPT))) planet_phase_ink(unsigned c,int x,int y){
 if(!planet_phase_active)return c;
 float nx=(x+.5f-planet_phase_cx)*planet_phase_inverse,ny=(y+.5f-planet_phase_cy)*planet_phase_inverse;
 float fx=fabsf(nx)*32,fy=fabsf(ny)*32;if(fx>31.999f)fx=31.999f;if(fy>31.999f)fy=31.999f;int ix=(int)fx,iy=(int)fy;fx-=ix;fy-=iy;
 float za=planet_phase_z[iy][ix]*(1-fx)+planet_phase_z[iy][ix+1]*fx,zb=planet_phase_z[iy+1][ix]*(1-fx)+planet_phase_z[iy+1][ix+1]*fx;
 float nz=(za*(1-fy)+zb*fy)/255.f,lit=nx*planet_phase_lx+ny*planet_phase_ly+nz*planet_phase_lz;
 int index=(int)((lit+1)*127.5f);if(index<0)index=0;if(index>255)index=255;int g=planet_phase_gain[index];
 return (c&0xff000000u)|RGB((c&255)*g/256,((c>>8)&255)*g/256,((c>>16)&255)*g/256);
}
