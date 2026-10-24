/* Preserve the listening post/freighter's authored palettes in the shared GU
 * pass. Previously these faces fell back to a per-pixel CPU rasterizer even
 * when the rest of the encounter was hardware rendered. */
typedef struct {unsigned color;unsigned short tile;unsigned char mip,ready;} EncounterGeLegacyKey;
static EncounterGeLegacyKey encounter_ge_legacy_keys[128];
static unsigned *encounter_ge_legacy_pixels;static unsigned encounter_ge_epoch=1,encounter_ge_used[128];
static int encounter_ge_legacy_draw(DrawTri*t){
 int material=t->material,wreck=material>=5100&&material<5172;
 if(!wreck&&(material<5000||material>=5084))return 0;
 int m=material-(wreck?5100:5000),tile=m/6,mip=(m%6)/2,listening=!wreck&&tile>=6;if(listening){tile-=6;mip=0;}
 int code=wreck?32+tile:listening?16+tile:tile,size=32>>mip,offset=mip==0?0:mip==1?512:640;
 unsigned color=listening?0:t->color;
 if(!encounter_ge_legacy_pixels){encounter_ge_legacy_pixels=memalign(16,128*4096);if(!encounter_ge_legacy_pixels)return 0;}
 unsigned h=(color^(color>>11)^(code*2654435761u)^(mip*2246822519u))&127;int slot=-1,available=-1;
 for(int k=0;k<128;k++){int id=(h+k)&127;EncounterGeLegacyKey*q=&encounter_ge_legacy_keys[id];if(q->ready&&q->tile==code&&q->mip==mip&&q->color==color){slot=id;break;}if(available<0&&encounter_ge_used[id]!=encounter_ge_epoch)available=id;}
 if(slot<0){
  if(available<0)return 0;slot=available;EncounterGeLegacyKey*q=&encounter_ge_legacy_keys[slot];*q=(EncounterGeLegacyKey){color,code,mip,1};
  static const unsigned listen_palette[16]={0x00483d36,0x00685b51,0x009b8d7d,0x00291e17,0x00342b25,0x00201815,0x006b6259,0x0044aef2,0x0067cef8,0x003d769e,0x004ea7dc,0x00c6c23f,0x001a1310,0x005f5147,0x008d8175,0x00a69b8c};
  unsigned palette[16];for(int i=0;i<16;i++)palette[i]=0xff000000u|(listening?listen_palette[i]:freight_art_ink(color,i));unsigned*data=encounter_ge_legacy_pixels+slot*1024;
  for(int y=0;y<size;y++)for(int x=0;x<32;x++){int xx=x<size?x:size-1,index=y*size+xx,packed=wreck?wreck_art_tiles[tile][offset+(index>>1)]:listening?listening_post_art_tiles[tile][index>>1]:freight_art_tiles[tile][offset+(index>>1)],at=(y>>3)*256+(x>>2)*32+(y&7)*4+(x&3);data[at]=palette[(packed>>((index&1)*4))&15];}
  sceKernelDcacheWritebackRange(data,4096);
 }
 encounter_ge_used[slot]=encounter_ge_epoch;sceGuEnable(GU_TEXTURE_2D);sceGuTexMode(GU_PSM_8888,0,0,1);sceGuTexFunc(GU_TFX_REPLACE,GU_TCC_RGBA);sceGuTexFilter(GU_NEAREST,GU_NEAREST);sceGuTexLevelMode(GU_TEXTURE_CONST,0);sceGuTexImage(0,size,size,32,encounter_ge_legacy_pixels+slot*1024);sceGuTexFlush();
 static const unsigned char uv[2][6]={{0,0,1,0,1,1},{0,0,1,1,0,1}};StationGeVertex*v=sceGuGetMemory(3*sizeof(*v));float scale=(size-1.f)/size;
 for(int k=0;k<3;k++){Point p=t->p[k];v[k]=(StationGeVertex){uv[m&1][k*2]*scale,uv[m&1][k*2+1]*scale,(p.x-proj_ox)*p.z/proj_focal,(proj_oy-p.y)*p.z/proj_focal,p.z};}
 sceGuDrawArray(GU_TRIANGLES,GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D,3,0,v);fleet_ge_triangles++;return 1;
}
