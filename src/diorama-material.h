/* Diorama Stage A: removable, ordinary-flight material pilot. No allocation,
 * saves, geometry or global framebuffer colour pass. ADder mesh 4 only. */
static int dg_enabled=1;
static int dg_active(void){return dg_enabled&&!high_contrast&&page==FLIGHT&&!game.surface&&!race_is_active(&game);}
/* Explicit stern outlets: inside the Adder's x +/-30, y +/-7 rear face,
 * z=-40 (ships.c v4). They are geometry metadata, not collision state. */
static const Vec3 dg_adder_outlets[2]={{-17,0,-40.15f},{17,0,-40.15f}};
static unsigned dg_material(unsigned c,int lit){
 int shade=256-lit;unsigned tint=sun_light_colour(game.bodies[0].seed);
 int r=((c&255)*(176+lit*137/256)+lit*44*(tint&255)/255)/256;
 int g=(((c>>8)&255)*(213+lit*75/256)+lit*44*((tint>>8)&255)/255+shade*3)/256;
 int b=(((c>>16)&255)*(255-lit*28/256)+lit*44*((tint>>16)&255)/255+shade*9)/256;
 return RGB(r>255?255:r,g>255?255:g,b>255?255:b);
}
static int dg_station_lit[6];
static int dg_face_index(Vec3 n){return n.z<-.5f?0:n.z>.5f?1:n.y<-.5f?2:n.y>.5f?3:n.x<0?4:5;}
static void dg_station_prepare(Vec3 sun){
 const float axis[6]={-sun.z,sun.z,-sun.y,sun.y,-sun.x,sun.x};
 for(int i=0;i<6;i++)dg_station_lit[i]=(int)(fmaxf(0,(axis[i]+.22f)/1.22f)*256);
}
