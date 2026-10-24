/* Group authored station triangles by identical texture/face palette. The
 * original UV calculation, mipmaps, lettering and GPU depth test are retained. */
static void station_ge_authored_batch(unsigned char*handled){
 int counts[STATION_GE_TEXTURE_SLOTS]={0},start[STATION_GE_TEXTURE_SLOTS],cursor[STATION_GE_TEXTURE_SLOTS];short group[2048];unsigned short order[2048];
 for(int i=0;i<drawcount;i++){
  group[i]=-1;DrawTri*t=&drawlist[i];if(handled[i]||t->material>=0)continue;
  int id=(-t->material-1)/6,face=(-t->material-1)%6;station_art_face_prepare(&mc_draw_city->b[id],face);
  if(!station_art_face.active||station_art_face.ad)continue;
  int key=station_ge_slot(id,face,t->depth>9000);group[i]=key;counts[key]++;
 }
 int total=0;for(int g=0;g<STATION_GE_TEXTURE_SLOTS;g++){start[g]=cursor[g]=total;total+=counts[g];}
 for(int i=0;i<drawcount;i++)if(group[i]>=0)order[cursor[group[i]]++]=i;
 for(int g=0;g<STATION_GE_TEXTURE_SLOTS;g++)if(counts[g]){
  int n=counts[g];if(sceGuCheckList()+n*60+512>480000)continue;
  DrawTri*first=&drawlist[order[start[g]]];int id=(-first->material-1)/6,face=(-first->material-1)%6;station_art_face_prepare(&mc_draw_city->b[id],face);
  unsigned*texture=station_ge_texture(id,face,first->depth>9000),*mip_data=station_ge_mip_prepare(face,station_art_face.role);
  sceGuEnable(GU_TEXTURE_2D);sceGuTexFunc(GU_TFX_REPLACE,GU_TCC_RGBA);sceGuTexMode(GU_PSM_8888,5,0,1);sceGuTexFilter(GU_NEAREST_MIPMAP_LINEAR,GU_NEAREST);sceGuTexLevelMode(GU_TEXTURE_AUTO,1.f);sceGuTexImage(0,32,32,32,texture);
  for(int level=1;level<=5;level++){int size=32>>level;sceGuTexImage(level,size,size,16,mip_data+station_ge_mip_offsets[level-1]);}sceGuTexFlush();
  StationGeVertex*v=sceGuGetMemory(n*3*sizeof(*v));
  for(int k=0;k<n;k++){
   int index=order[start[g]+k];DrawTri*t=&drawlist[index];id=(-t->material-1)/6;face=(-t->material-1)%6;const MegaBlock*b=&mc_draw_city->b[id];station_art_face_prepare(b,face);handled[index]=2;
   int ua=face<4?0:2,va=face==2||face==3?2:1;float offsets[3]={mc_eye.x-b->c.x,mc_eye.y-b->c.y,mc_eye.z-b->c.z};
   for(int j=0;j<3;j++){
    Point p=t->p[j];float rx=(p.x-proj_ox)/proj_focal,ry=(proj_oy-p.y)/proj_focal,u=offsets[ua]+p.z*(mc_axis[ua].x*rx+mc_axis[ua].y*ry+mc_axis[ua].z),vv=offsets[va]+p.z*(mc_axis[va].x*rx+mc_axis[va].y*ry+mc_axis[va].z),tu=(u*station_art_face.us+1)*.5f,tv=(1-vv*station_art_face.vs)*.5f;
    if(station_art_face.ring){float ca=station_art_face.ca,sa=station_art_face.sa,inv=station_art_face.inv;tu=.5f+(-sa*u+ca*vv)*inv*1.68f;tv=1-((ca*u+sa*vv)*inv+.208f)*3.135f;}if(station_art_face.swap){float q=tu;tu=tv;tv=q;}
    v[k*3+j]=(StationGeVertex){tu,tv,(p.x-proj_ox)*p.z/proj_focal,(proj_oy-p.y)*p.z/proj_focal,p.z};
   }
  }
  sceGuDrawArray(GU_TRIANGLES,GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D,n*3,0,v);station_ge_triangles+=n;
 }
}
