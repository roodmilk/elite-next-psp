typedef struct {float x,z,w;int axis;} CloudEdge;
static void draw_cloud_decks(Vec3 pad){
 static unsigned seed;static int ready,nfloor,nedge;static CloudEdge floors[1024],edges[2048];
 if(!ready||seed!=game.bodies[game.planet].seed){
  unsigned char cells[112*112];seed=game.bodies[game.planet].seed;ready=1;nfloor=nedge=0;
  for(int z=0;z<112;z++)for(int x=0;x<112;x++)cells[z*112+x]=surface_cloud_deck(&game,pad.x+(x-56+.5f)*40,pad.z+(z-56+.5f)*40);
  for(int z=1;z<111;z++){
   for(int x=1;x<111;){if(!cells[z*112+x]){x++;continue;}int start=x;while(x<111&&cells[z*112+x])x++;
    if(nfloor<1024)floors[nfloor++]=(CloudEdge){pad.x+(start-56)*40,pad.z+(z-56)*40,(x-start)*40,0};
   }
   for(int x=1;x<111;x++)if(cells[z*112+x])for(int side=0;side<4;side++){
    int nx=x+(side==0?-1:side==1?1:0),nz=z+(side==2?-1:side==3?1:0);
    if(!cells[nz*112+nx]&&nedge<2048)edges[nedge++]=(CloudEdge){pad.x+(x-56)*40+(side==1?40:0),pad.z+(z-56)*40+(side==3?40:0),40,side<2?1:0};
   }
  }
 }
 surface_material=17;
 for(int i=0;i<nfloor;i++){
  CloudEdge s=floors[i];Vec3 v=camera(&game,(Vec3){s.x+s.w*.5f,24,s.z+20});float radius=s.w*.5f+40;
  if(v.z+radius<4||v.z-radius>2300||fabsf(v.x)>v.z+radius*2)continue;
  if(drawcount>1800)flush_meshes();
  if(s.z>=pad.z-FIELD_PORT_HALF&&s.z+40<=pad.z+FIELD_PORT_HALF){
   float left=fminf(s.x+s.w,pad.x-FIELD_PORT_HALF),right=fmaxf(s.x,pad.x+FIELD_PORT_HALF);
   if(left>s.x)planet_quad((Vec3){s.x,24,s.z},(Vec3){left,24,s.z},(Vec3){left,24,s.z+40},(Vec3){s.x,24,s.z+40},RGB(103,129,143));
   if(right<s.x+s.w)planet_quad((Vec3){right,24,s.z},(Vec3){s.x+s.w,24,s.z},(Vec3){s.x+s.w,24,s.z+40},(Vec3){right,24,s.z+40},RGB(103,129,143));
   continue; /* The opaque main apron supplies the middle span. */
  }
  planet_quad((Vec3){s.x,24,s.z},(Vec3){s.x+s.w,24,s.z},(Vec3){s.x+s.w,24,s.z+40},(Vec3){s.x,24,s.z+40},RGB(103,129,143));
 }
 flush_meshes();surface_material=0;
 for(int i=0;i<nedge;i++){
  CloudEdge s=edges[i];Vec3 a={s.x,24,s.z},b={s.x+(s.axis?0:40),24,s.z+(s.axis?40:0)},v=camera(&game,a);
  if(v.z< -60||v.z>2000||fabsf(v.x)>v.z*1.3f+70)continue;
  if(drawcount>1800)flush_meshes();
  Vec3 c=b,d=a;c.y=d.y=-44;planet_quad(a,b,c,d,(i&1)?RGB(42,65,86):RGB(53,79,101));
  /* A visible safety parapet exactly follows each exposed collision edge. */
  c=b;d=a;c.y=d.y=37;planet_quad(a,b,c,d,RGB(140,161,169));
  if(v.z<650){a.y=b.y=38;c=b;d=a;c.y=d.y=40;planet_quad(a,b,c,d,RGB(224,175,72));}
 }
 flush_meshes();
}
