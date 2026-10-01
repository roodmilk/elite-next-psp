/* The same authored identity is used by world billboards, scanner and Codex. */
#include "generated/field-wildlife-pixels.h"
#include "generated/fauna-animation-pixels.h"
static unsigned char field_minerals[8][3072];
static unsigned field_species_inks[8][64],field_sprite_world=~0u;
static int field_art_kind[8],field_art_family[8];static unsigned field_art_seed[8];
static void field_sprite_build(int sys,int body){
 unsigned world=(unsigned)(sys*BODY_COUNT+body);if(field_sprite_world==world)return;field_sprite_world=world;
 static const unsigned accents[]={RGB(229,173,76),RGB(174,212,199),RGB(186,196,208),RGB(125,109,145),RGB(191,218,101),RGB(197,183,99),RGB(153,108,137),RGB(99,124,164),RGB(193,124,103),RGB(182,136,89),RGB(226,217,172),RGB(147,104,166)};
 for(int slot=0;slot<8;slot++){
  unsigned h=field_species_seed(sys,body,slot);int kind=field_species_kind(sys,body,slot),family=(h>>8)%8;
  field_art_kind[slot]=kind;field_art_family[slot]=family;field_art_seed[slot]=h;
  for(int i=0;i<64;i++){
   unsigned c=field_art_palette[i],a=accents[h%12];int light=((c&255)+((c>>8)&255)+((c>>16)&255))/3;
   int red=((c&255)*7+(a&255)*light/160)/8,green=(((c>>8)&255)*7+((a>>8)&255)*light/160)/8,blue=(((c>>16)&255)*7+((a>>16)&255)*light/160)/8;
   if(sys==7&&body==4){red=(red*3+light)/4;green=(green*3+light*11/10)/4;blue=(blue*2+light*13/10)/3;}
   if(sys==7&&body==3){red=(red*3+light*11/10)/4;blue=(blue*2+light*12/10)/3;}
   field_species_inks[slot][i]=RGB(red>255?255:red,green>255?255:green,blue>255?255:blue);
  }
  if(kind!=LIFE_MINERAL)continue;
  for(int y=0;y<64;y++)for(int x=0;x<48;x++){
   int ink=255;
   for(int shard=0;shard<5;shard++){
    int cx=8+shard*8,tip=8+(int)((h>>(shard*4))&15)+abs(shard-2)*6,width=5+((h>>shard)&3);
    if(y>=tip&&y<62&&abs(x-cx)<width&&abs(x-cx)*2<y-tip+1){ink=x<cx?49:47;if(x==cx-1||y==tip+abs(x-cx)*2)ink=50;if(y>55)ink=24;}
   }
   field_minerals[slot][y*48+x]=(unsigned char)ink;
  }
 }
}
static int field_sprite_sample(int slot,int x,int y,int frame){
 if(slot<0||slot>=8)return 255;
 int kind=field_art_kind[slot],family=field_art_family[slot],sway=frame==1?1:frame==3?-1:0;
 if(kind==LIFE_FLORA)x-=sway*(63-y)/28;
 if(x<0||x>=48||y<0||y>=64)return 255;
 if(kind==LIFE_FAUNA)return fauna_anim_pixels[family][frame&3][y*48+x];
 return kind==LIFE_MINERAL?field_minerals[slot][y*48+x]:field_art_pixels[(kind==LIFE_FAUNA?8:0)+family][y*48+x];
}
static void field_draw_species(int cx,int base,int size,int sys,int body,int slot,float time,int top,int bottom){
 if(size<2||slot<0||slot>=8)return;
 field_sprite_build(sys,body);
 static const int cycle[]={0,1,0,2};
 int width=size*3/4,frame=field_art_kind[slot]==LIFE_FAUNA?cycle[(int)(time*4+(field_art_seed[slot]&3))&3]:(int)(time*3+(field_art_seed[slot]&3))&3,x0=cx-width/2,y0=base-size;
 for(int y=0;y<size;y++){int yy=y0+y;if(yy<top||yy>=bottom||yy<0||yy>=H)continue;
  for(int x=0;x<width;x++){int xx=x0+x;if(xx<0||xx>=W)continue;int ink=field_sprite_sample(slot,x*48/width,y*64/size,frame);if(ink!=255)fb[yy*STRIDE+xx]=field_species_inks[slot][ink];}
 }
}
