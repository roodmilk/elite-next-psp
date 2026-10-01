{
 static const int weapons[]={1,2,20,25,26,27,28,29,30,31,57,58,59,60,61,62,63,64,65,66,67,68};
 unsigned hashes[22]={0};int visible=1,distinct=0;
 for(int i=0;i<22;i++){
  memset(pixels,0,STRIDE*H*sizeof(unsigned));weapon_fx_draw(weapons[i],.31f,17+i,191);
  unsigned hash=2166136261u;int lit=0;for(int y=24;y<192;y++)for(int x=0;x<W;x++){unsigned c=pixels[y*STRIDE+x];if(c){lit++;hash=(hash^c)*16777619u;hash=(hash^(unsigned)(x+y*W))*16777619u;}}
  hashes[i]=hash;visible&=lit>24;int unique=1;for(int j=0;j<i;j++)if(hashes[j]==hash)unique=0;distinct+=unique;
 }
 INPUT_CHECK(visible&&distinct==22,"weapon FX: all 22 fitted weapons paint distinct visible firing signatures");
 memset(pixels,0,STRIDE*H*sizeof(unsigned));weapon_fx_draw(61,.31f,18,191);unsigned a=0;for(int y=24;y<192;y++)for(int x=0;x<W;x++)a=a*33u+pixels[y*STRIDE+x];
 memset(pixels,0,STRIDE*H*sizeof(unsigned));weapon_fx_draw(61,.31f,19,191);unsigned b=0;for(int y=24;y<192;y++)for(int x=0;x<W;x++)b=b*33u+pixels[y*STRIDE+x];
 INPUT_CHECK(a!=b,"weapon FX: Rainbow Prism cycles its gold, pink and cyan order between shots");
 memset(pixels,0,STRIDE*H*sizeof(unsigned));weapon_fx_draw(61,.31f,20,191);dump_native_bmp("weapon-rainbow-prism.bmp");
 memset(pixels,0,STRIDE*H*sizeof(unsigned));weapon_fx_draw(68,.31f,20,191);dump_native_bmp("weapon-blackstar.bmp");
}
