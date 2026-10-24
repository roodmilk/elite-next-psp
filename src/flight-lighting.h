/* Ordinary-flight light only. No saves, allocations, race branding or post blur.
 * 65x65 cached additive halo: bounded 5.2 KiB, integer blit each frame. */
static int flight_lighting_enabled=1;
static unsigned char flight_light_halo[65*65];
static unsigned flight_light_palette[256];
static int flight_light_ready;
static unsigned flight_light_colour;
static Vec3 flight_station_key;
static int flight_lighting_active(void){return flight_lighting_enabled&&!high_contrast&&page==FLIGHT&&clipy0<0&&!game.surface&&!race_is_active(&game);}
static void flight_lighting_init(void){
 unsigned tint=sun_light_colour(game.bodies[0].seed);
 if(flight_light_ready&&tint==flight_light_colour)return;
 flight_light_colour=tint;
 flight_light_ready=1;
 for(int a=0;a<256;a++){int r=a*225/255;flight_light_palette[a]=RGB(r*(tint&255)/255,r*((tint>>8)&255)/255,r*((tint>>16)&255)/255);}
 for(int y=0;y<65;y++)for(int x=0;x<65;x++){
  int dx=x-32,dy=y-32,q=dx*dx+dy*dy,a=q<1024?(1024-q)*255/1024:0;
  a=a*a/255;
  flight_light_halo[y*65+x]=a;
 }
}
static inline unsigned flight_light_add(unsigned a,unsigned b){
 unsigned rb=(a&0x00ff00ffu)+(b&0x00ff00ffu),o=rb&0x01000100u;rb=(rb|(o-(o>>8)))&0x00ff00ffu;
 unsigned g=(a&0x0000ff00u)+(b&0x0000ff00u);o=g&0x00010000u;g=(g|(o-(o>>8)))&0x0000ff00u;
 return (a&0xff000000u)|rb|g;
}
static void flight_lighting_halo(int cx,int cy,int sun_radius,int xt,int yt,int xb,int yb){
 if(!flight_lighting_active()||sun_radius<1)return;
 int r=80;
 int left=cx-r>xt?cx-r:xt,right=cx+r+1<xb?cx+r+1:xb,top=cy-r>yt?cy-r:yt,bottom=cy+r+1<yb+1?cy+r+1:yb+1;
 if(left<0)left=0;
 if(right>W)right=W;
 if(top<0)top=0;
 if(bottom>H)bottom=H;
 if(left>=right||top>=bottom)return;
 flight_lighting_init();unsigned short sample[W];int d=r*2;
 for(int x=left;x<right;x++)sample[x-left]=(x-cx+r)*64/d;
 for(int y=top;y<bottom;y++){
  const unsigned char *row=flight_light_halo+((y-cy+r)*64/d)*65;unsigned *dest=fb+y*STRIDE+left;
  for(int x=left;x<right;x++,dest++){unsigned c=flight_light_palette[row[sample[x-left]]];if(c)*dest=flight_light_add(*dest,c);}
 }
 /* Quiet diffraction line, behind world geometry like the halo. */
 for(int n=-45;n<=45;n++){int x=cx+n,y=cy+n/3;if(x<xt||x>=xb||y<yt||y>yb||x<0||x>=W||y<0||y>=H)continue;int a=(45-abs(n))/5;fb[y*STRIDE+x]=flight_light_add(fb[y*STRIDE+x],RGB(a,a*2/3,a/3));}
}
static void flight_lighting_station_prepare(void){
 Vec3 sun=sub(game.bodies[0].pos,(Vec3){0,0,STATION_Z});flight_station_key=rotate(norm(sun),0,-station_angle(&game));dg_station_prepare(flight_station_key);
}
static unsigned flight_lighting_station_face(unsigned c,Vec3 n){
 if(!flight_lighting_active())return c;
 float lit=fmaxf(0,dot(n,flight_station_key));
 int r=(c&255)*(.82f+.35f*lit)+lit*22*(sun_light_colour(game.bodies[0].seed)&255)/255,g=((c>>8)&255)*(.82f+.22f*lit)+lit*22*((sun_light_colour(game.bodies[0].seed)>>8)&255)/255,b=((c>>16)&255)*(.88f+.10f*lit)+lit*22*((sun_light_colour(game.bodies[0].seed)>>16)&255)/255;
 return RGB(r>255?255:r,g>255?255:g,b>255?255:b);
}
