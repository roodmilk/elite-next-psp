/* Authored 64px portrait rigs: explicit anatomical masks and local parts.
 * No full-face roulette, colour-threshold masks or DNA-driven background. */
enum {PC_CLEAR,PC_INK,PC_WHITE,PC_SKIN0,PC_SKIN1,PC_SKIN2,PC_SKIN3,PC_HAIR0,PC_HAIR1,PC_HAIR2,PC_HAIR3,PC_SUIT0,PC_SUIT1,PC_SUIT2,PC_TRIM,PC_EYE,PC_LIP,PC_METAL,PC_COUNT};
#define PORTRAIT_PART_TILE 64
 typedef struct {unsigned char ink[4096],owner[4096];uint16_t pixels[4096];} PortraitParts;
 typedef struct {signed char x,y,w,h;} PortraitBox;
 typedef struct {unsigned char base,min_y; signed char skin[12][2],hair[12][2];PortraitBox eyes[2],mouth,ear;} PortraitRig;
 static const PortraitRig portrait_rigs[10]={
 {0,0,{{32,16},{43,16},{49,22},{50,33},{46,43},{44,48},{33,48},{26,40},{22,30},{27,23},{-1,-1}},{{21,14},{26,11},{31,12},{34,8},{42,6},{48,10},{52,17},{48,24},{43,17},{33,19},{29,30},{21,27}},{{33,21,7,6},{44,21,6,5}},{39,33,9,8},{22,27,7,10}},
 {16,0,{{20,11},{32,4},{42,7},{48,16},{50,28},{46,45},{39,52},{32,45},{29,30},{20,26},{-1,-1}},{{-1,-1}},{{34,24,9,8},{44,25,5,7}},{38,39,10,7},{0,0,0,0}},
 {24,18,{{18,24},{23,18},{36,18},{44,26},{44,46},{36,56},{23,53},{13,46},{10,37},{-1,-1}},{{-1,-1}},{{22,22,13,14},{36,25,5,10}},{12,37,25,9},{0,0,0,0}},
 {32,0,{{22,20},{34,16},{44,24},{42,43},{35,56},{28,53},{20,38},{-1,-1}},{{10,4},{17,8},{24,20},{34,18},{41,4},{42,25},{27,31},{19,21},{-1,-1}},{{14,25,11,18},{37,26,8,17}},{29,45,7,8},{0,0,0,0}},
 {40,6,{{31,18},{43,21},{56,30},{56,38},{39,42},{38,54},{27,53},{25,35},{-1,-1}},{{11,14},{21,8},{30,14},{35,23},{29,30},{24,48},{12,43},{17,32},{-1,-1}},{{34,23,10,7},{0,0,0,0}},{38,34,17,8},{0,0,0,0}},
 {48,0,{{39,23},{49,28},{55,36},{52,43},{40,38},{-1,-1}},{{14,23},{24,11},{38,8},{47,17},{46,26},{39,30},{35,50},{26,59},{13,52},{-1,-1}},{{28,21,11,10},{0,0,0,0}},{0,0,0,0},{0,0,0,0}},
 {64,0,{{29,8},{42,9},{49,18},{49,36},{43,46},{33,44},{25,30},{25,16},{-1,-1}},{{14,19},{22,15},{29,21},{28,33},{20,37},{14,31},{-1,-1}},{{34,22,10,8},{44,23,6,7}},{36,34,13,8},{14,18,15,19}},
 {76,0,{{31,14},{44,15},{49,23},{49,43},{43,50},{31,48},{26,37},{25,25},{-1,-1}},{{22,13},{28,7},{39,3},{47,8},{49,15},{43,18},{31,18},{27,28},{21,27},{-1,-1}},{{32,21,10,7},{43,21,7,6}},{37,34,11,9},{21,26,8,12}},
 {56,11,{{30,29},{42,27},{49,36},{48,53},{36,58},{25,53},{24,40},{-1,-1}},{{24,15},{33,12},{43,15},{50,26},{54,54},{48,58},{43,47},{44,33},{38,24},{28,28},{22,53},{14,51}},{{28,29,15,10},{0,0,0,0}},{27,41,19,12},{0,0,0,0}},
 {88,0,{{24,17},{37,13},{49,29},{47,44},{40,54},{26,49},{17,35},{-1,-1}},{{13,28},{18,12},{25,22},{28,3},{35,17},{40,8},{42,24},{50,20},{48,35},{38,28},{25,33},{-1,-1}},{{0,0,0,0},{0,0,0,0}},{0,0,0,0},{0,0,0,0}}
 };
 static int portrait_polygon(const signed char poly[12][2],int x,int y){
  int n=0,inside=0;while(n<12&&poly[n][0]>=0)n++;
  for(int i=0,j=n-1;i<n;j=i++)if((poly[i][1]>y)!=(poly[j][1]>y)){
   float cross=poly[i][0]+(y-poly[i][1])*(float)(poly[j][0]-poly[i][0])/(poly[j][1]-poly[i][1]);if(x<cross)inside=!inside;
  }return inside;
 }
#include "portrait-material-masks.h"
#include "portrait-female-masks.h"
 static int portrait_source_material(int sp,int x,int y);
 static int portrait_rig_material(int sp,int female,int x,int y){
  if(!female)return portrait_source_material(sp,x,y);
  if(x<0||x>=64||y<0||y>=64)return PC_CLEAR;
  static const unsigned char material[]={PC_SKIN2,PC_HAIR1,PC_SUIT1,PC_EYE,PC_LIP};
  for(int i=0;i<5;i++)if((portrait_female_rows[sp==7][i][y]>>x)&1u)return material[i];return PC_CLEAR;
 }
 static int portrait_hair_pixel(int sp,int x,int y){
  if(x<0||x>=64||y<0||y>=64)return 0;
  return (portrait_material_rows[sp][1][y]>>x)&1u;
 }
 static int portrait_source_material(int sp,int x,int y){
  if(x<0||x>=64||y<0||y>=64)return PC_CLEAR;
  static const unsigned char material[]={PC_SKIN2,PC_HAIR1,PC_SUIT1,PC_EYE,PC_LIP};
  for(int i=0;i<5;i++)if((portrait_material_rows[sp][i][y]>>x)&1u)return material[i];return PC_CLEAR;
 }
 /* Sockets traced on each actual face, not copied from a human template. */
 static const PortraitBox portrait_eye_sockets[10][2]={
  {{34,23,9,8},{45,26,6,6}},{{27,23,10,12},{41,28,6,9}},{{20,21,15,16},{0}},
  {{17,25,15,20},{43,27,7,17}},{{34,20,8,8},{0}},{{33,19,11,12},{0}},
  {{33,20,10,10},{47,23,5,8}},{{31,21,11,7},{43,24,6,6}},{{28,31,5,7},{35,30,9,10}},{{0},{0}}
 };
 static const PortraitBox portrait_mouth_sockets[10]={
  {39,36,9,7},{34,42,8,7},{11,37,28,9},{29,44,8,10},{30,33,25,9},{0},
  {38,33,11,10},{35,33,11,9},{29,42,16,12},{0}
 };
 static const PortraitBox portrait_ear_sockets[10]={{22,27,7,11},{0},{0},{0},{0},{0},{12,17,12,19},{14,24,9,13},{0},{0}};
 static const PortraitBox portrait_iris_sockets[10][2]={
  {{36,25,5,3},{46,26,4,4}},{{28,26,7,7},{42,29,4,6}},{{24,25,7,9},{0}},
  {{21,28,8,12},{44,30,4,9}},{{35,22,5,5},{0}},{{35,21,6,7},{0}},
  {{35,21,6,7},{48,24,3,5}},{{32,23,8,3},{45,25,3,3}},{{30,33,2,3},{37,32,5,6}},{{0},{0}}
 };
 static int portrait_in_box(PortraitBox b,int x,int y){return b.w&&x>=b.x&&x<b.x+b.w&&y>=b.y&&y<b.y+b.h;}
 static int portrait_in_ellipse(PortraitBox b,int x,int y){
  if(!portrait_in_box(b,x,y))return 0;int u=2*(x-b.x)+1-b.w,v=2*(y-b.y)+1-b.h;return u*u*b.h*b.h+v*v*b.w*b.w<=b.w*b.w*b.h*b.h;
 }
 static unsigned portrait_parts_dna(unsigned raw){
  if(portrait_version(raw)==4)return raw;
  /* v2 selections already have the same meanings. Upgrade without resetting
   * inactive selections that may become visible after another species change. */
  if(portrait_version(raw)==2)return (raw&~(3u<<26))|(3u<<26);
  unsigned d=portrait_normalize(raw);if(portrait_version(d)!=3)d&=~(7u<<19);d=(d&~(3u<<26))|(3u<<26);d&=~((1u<<24)|(1u<<25));
  static const unsigned char shift[]={0,4,6,9,12,14,16,19,22,28},mask[]={15,3,7,7,3,3,7,7,3,7};
  for(int p=1;p<10;p++){if(p==7)continue;unsigned v=portrait_part_value(d,p)%portrait_choice_count(d,p);d=portrait_set_field(d,shift[p],mask[p],v);if(p==2)d=portrait_set_field(d,19,1,v>>3);if(p==3)d=portrait_set_field(d,20,1,v>>3);if(p==5)d=portrait_set_field(d,21,1,v>>2);}
  return d;
 }
#include "portrait-authored-parts.h"
 /* Reshape authored hair clusters, keeping all face/neck/garment pixels out
  * of the hair layer. Classic is deliberately the unchanged original rig. */
 static void portrait_style_hair(PortraitParts *p,int style,int sp,int female){
  if(!style)return;
  if(style==1&&female){portrait_authored_crop(p,sp);return;}
  PortraitParts old=*p;
  if(style==1||style==2)for(int x=0;x<64;x++){
   int top=64,end=0;
   for(int y=0;y<23;y++)if(old.ink[y*64+x]==PC_HAIR1){if(top==64)top=y;end=y+1;}
   if(top>=end-3)continue;
   int start=style==1?top+4:top-4;if(start<2)start=2;if(start>=end)start=end-1;
   for(int y=0;y<end;y++){
    int at=y*64+x;if(old.ink[at]&&old.ink[at]!=PC_HAIR1)continue;
    if(y<start){if(old.ink[at]==PC_HAIR1){p->ink[at]=p->owner[at]=0;p->pixels[at]=0;}continue;}
    int sy=top+(y-start)*(end-top)/(end-start),from=sy*64+x;
    if(old.ink[from]!=PC_HAIR1)continue;
    p->ink[at]=PC_HAIR1;p->owner[at]=11;p->pixels[at]=old.pixels[from];
   }
  }
  if(style==1)for(int y=35;y<55;y++)for(int x=0;x<64;x++){
   int at=y*64+x;if(p->ink[at]!=PC_HAIR1)continue;
   if((female&&sp==0&&(x<20||x>51))||(female&&sp==7&&x>53))p->ink[at]=p->owner[at]=0,p->pixels[at]=0;
  }
  if(style==3){
   /* Reverse the authored crown's sweep; retain the original hairline,
    * temple, forehead openings and ear cutout. No dangling hair stamps. */
   for(int y=0;y<17;y++){
    int lo=64,hi=-1;for(int x=0;x<64;x++)if(old.ink[y*64+x]==PC_HAIR1){if(lo==64)lo=x;hi=x;}
    if(hi<lo)continue;
    for(int x=lo;x<=hi;x++){
     int at=y*64+x,from=y*64+lo+hi-x;
     if(old.ink[at]&&old.ink[at]!=PC_HAIR1)continue;
     if(old.ink[from]!=PC_HAIR1)continue;
     p->ink[at]=PC_HAIR1;p->owner[at]=11;p->pixels[at]=old.pixels[from];
    }
   }
  }
 }
 static void portrait_parts_build(PortraitParts *p,unsigned d){
  memset(p,0,sizeof(*p));int sp=portrait_species(d),modern=portrait_version(d)==4,female=modern&&(sp==0||sp==7)&&portrait_part_value(d,11);const PortraitRig *r=&portrait_rigs[sp];const uint16_t *src=portrait_rig_source(sp,female);
  PortraitBox eye_box[2],iris_box[2],mouth_box,ear_box;
  for(int i=0;i<2;i++){eye_box[i]=portrait_eye_sockets[sp][i];iris_box[i]=portrait_iris_sockets[sp][i];}
  mouth_box=portrait_mouth_sockets[sp];ear_box=portrait_ear_sockets[sp];
  if(female){
   eye_box[0]=(PortraitBox){sp==7?25:28,24,12,10};eye_box[1]=(PortraitBox){0};
   iris_box[0]=(PortraitBox){sp==7?28:30,26,7,5};iris_box[1]=(PortraitBox){0};
   mouth_box=sp==7?(PortraitBox){19,35,10,9}:(PortraitBox){22,38,10,9};
   ear_box=sp==7?(PortraitBox){43,26,11,14}:(PortraitBox){43,29,9,12};
  }
  for(int y=r->min_y;y<64;y++)for(int x=0;x<64;x++){
   int at=y*64+x,material=portrait_rig_material(sp,female,x,y);if(!material)continue;p->pixels[at]=src[at];p->ink[at]=material;
   p->owner[at]=material==PC_SUIT1?6:material==PC_HAIR1?4:3;
   for(int eye=0;eye<2;eye++)if(portrait_in_ellipse(eye_box[eye],x,y))p->owner[at]=5;
   if(portrait_in_ellipse(mouth_box,x,y))p->owner[at]=9;
   if(portrait_in_box(ear_box,x,y))p->owner[at]=10;
  }
  if(modern&&(sp==0||sp==7))portrait_style_hair(p,portrait_part_value(d,10),sp,female);
  /* Squint inside each actual eye socket. Eyelids clone the authored skin
   * just above that socket; no eye stamp can land on another face's cheek. */
  if(portrait_choice_count(d,4)>1&&portrait_part_value(d,4)==1)for(int eye=0;eye<2;eye++){
   PortraitBox b=eye_box[eye];for(int y=b.y;y<b.y+b.h;y++)for(int x=b.x;x<b.x+b.w;x++){
    int at=y*64+x;if((p->ink[at]!=PC_SKIN2&&p->ink[at]!=PC_EYE&&!(sp==5&&p->ink[at]==PC_HAIR1))||!portrait_in_ellipse(b,x,y))continue;int centre=b.y+b.h/2,sy=portrait_part_value(d,4)==1?centre+(y-centre)*2:centre+(y-centre)*2/3;
    if(sy<b.y||sy>=b.y+b.h)sy=b.y-1;
    int material=portrait_rig_material(sp,female,x,sy);if(material!=PC_SKIN2&&material!=PC_EYE&&!(sp==5&&material==PC_HAIR1))continue;
    p->pixels[at]=src[sy*64+x];p->ink[at]=material;p->owner[at]=5;
   }
  }
  /* Luminous optics use authored iris masks, not a skin-colour threshold or
   * a stretched eye bitmap. The surrounding eyelids and brows stay intact. */
  if(portrait_choice_count(d,4)>2&&portrait_part_value(d,4)==2)for(int eye=0;eye<2;eye++){
   PortraitBox b=iris_box[eye];for(int y=b.y;y<b.y+b.h;y++)for(int x=b.x;x<b.x+b.w;x++){
    int at=y*64+x;if((p->ink[at]!=PC_EYE&&p->ink[at]!=PC_SKIN2)||(female&&p->ink[at]!=PC_EYE)||!portrait_in_ellipse(b,x,y))continue;uint16_t c=src[at];
    int light=15+((((c>>10)&31)*3+((c>>5)&31)*5+(c&31)*2)/10)/2;if(light>31)light=31;
    p->pixels[at]=(uint16_t)(0x8000|((light/3)<<10)|(light<<5)|(light*29/31));p->ink[at]=PC_EYE;p->owner[at]=5;
   }
  }
  if(modern&&portrait_choice_count(d,4)>1&&portrait_part_value(d,4)>=3)for(int eye=0;eye<2;eye++){
   PortraitBox b=eye_box[eye];int style=portrait_part_value(d,4),centre=b.y+b.h/2;
   for(int y=b.y;y<b.y+b.h;y++)for(int x=b.x;x<b.x+b.w;x++){
    int at=y*64+x;if(!portrait_in_ellipse(b,x,y)||(p->ink[at]!=PC_SKIN2&&p->ink[at]!=PC_EYE&&!(sp==5&&p->ink[at]==PC_HAIR1)))continue;
    int sy=style==3?centre+(y-centre)*2/3:y+(x-b.x-b.w/2)/2;
    if(sy<b.y||sy>=b.y+b.h)sy=b.y-1;
    int material=portrait_rig_material(sp,female,x,sy);if(material!=PC_SKIN2&&material!=PC_EYE&&!(sp==5&&material==PC_HAIR1))continue;
    p->pixels[at]=src[sy*64+x];p->ink[at]=material;p->owner[at]=5;
   }
  }
  if(female&&portrait_part_value(d,8)==2)portrait_authored_smirk(p,sp,mouth_box);
  else if(portrait_choice_count(d,8)>1&&portrait_part_value(d,8)){
   PortraitBox b=mouth_box;for(int y=b.y;y<b.y+b.h;y++)for(int x=b.x;x<b.x+b.w;x++){
    int at=y*64+x;if((p->ink[at]!=PC_SKIN2&&p->ink[at]!=PC_LIP)||!portrait_in_ellipse(b,x,y))continue;int sy=portrait_part_value(d,8)==1?b.y+(y-b.y)*2/3:y+(x>b.x+b.w/2?2:-1);if(sy>=b.y+b.h)sy=b.y+b.h-1;if(sy<b.y)sy=b.y;
    int material=portrait_rig_material(sp,female,x,sy);if(material!=PC_SKIN2&&material!=PC_LIP)continue;p->pixels[at]=src[sy*64+x];p->ink[at]=material;p->owner[at]=9;
   }
  }
  if(female&&portrait_part_value(d,9))portrait_authored_ear(p,sp,portrait_part_value(d,9));
  else if(portrait_choice_count(d,9)>1&&portrait_part_value(d,9)){
   PortraitBox b=ear_box;for(int y=b.y;y<b.y+b.h;y++){
    int extra=portrait_part_value(d,9)==1?3:5*(b.h-(y-b.y))/b.h;
    for(int x=female?b.x:b.x-extra;x<b.x+b.w+(female?extra:0);x++){
    int sx=b.x+(x-(female?b.x:b.x-extra))*b.w/(b.w+extra),at=y*64+x,material=portrait_rig_material(sp,female,sx,y);
    if(x<0||x>=64||(material!=PC_SKIN2&&material!=PC_HAIR1))continue;
    /* Preserve the selected scalp and eye sockets beside the ear. */
    if(p->owner[at]==5||p->owner[at]==9)continue;
    p->pixels[at]=src[y*64+sx];p->ink[at]=material;p->owner[at]=10;
    }
   }
  }
  
  /* A tapered, shaded diagonal scar on the visible cheek. Authored only for
   * the human/android cheek rig; never stamp a stripe across the nose. */
  if(portrait_choice_count(d,6)>1&&portrait_part_value(d,6)==1)for(int y=29;y<41;y++){
   int centre=female?38+(y-29)/6:31+(y-29)/2,width=y==29||y==40?1:3;
   for(int dx=0;dx<width;dx++){
    int at=y*64+centre+dx;if(p->ink[at]!=PC_SKIN2||p->owner[at]!=3)continue;
    p->pixels[at]=dx==0?0xb16a:dx==1?0xe6d4:0xc9ed;p->ink[at]=PC_LIP;p->owner[at]=7;
   }
  }
  if(portrait_choice_count(d,6)>2&&portrait_part_value(d,6)==2){
   static const unsigned char dots[][2]={{31,28},{34,32},{31,38},{34,35}};
   static const unsigned char female_dots[][2]={{37,34},{40,37},{37,40},{40,34}};
   for(int n=0;n<4;n++)for(int yy=0;yy<(female?2:1);yy++)for(int xx=0;xx<2;xx++){
    int at=((female?female_dots[n][1]:dots[n][1])+yy)*64+(female?female_dots[n][0]:dots[n][0])+xx;if(p->ink[at]!=PC_SKIN2||p->owner[at]!=3)continue;
    p->pixels[at]=xx?0xc5cc:0xb16a;p->ink[at]=PC_LIP;p->owner[at]=7;
   }
  }
  /* Shape the assembled head as one continuous surface: eyes, ears, marks
   * and scalp travel together. The neck and clothes remain anchored. This
   * avoids independently extending jaw rows into disconnected black bars. */
  if(portrait_version(d)>=3&&portrait_choice_count(d,1)>1&&portrait_part_value(d,1)){
   PortraitParts original=*p;int centre=sp==7?33:35;
   for(int y=0;y<44;y++)for(int x=0;x<64;x++){
    int at=y*64+x;if(original.ink[at]==PC_SUIT1)continue;
    int scale=y<=35?56:56+y-35,sx=centre+(x-centre)*64/scale,from=y*64+sx;
    if(sx<0||sx>=64||original.ink[from]==PC_SUIT1){p->ink[at]=p->owner[at]=0;p->pixels[at]=0;continue;}
    p->ink[at]=original.ink[from];p->owner[at]=original.owner[from];p->pixels[at]=original.pixels[from];
   }
  }
 }
 static unsigned portrait_source_colour(uint16_t v){return RGB(((v>>10)&31)*255/31,((v>>5)&31)*255/31,(v&31)*255/31);}
 static unsigned portrait_material_colour(unsigned source,unsigned target){
  int l=((source&255)*3+((source>>8)&255)*5+((source>>16)&255)*2)/10;
  return RGB((int)(target&255)*l/160>255?255:(int)(target&255)*l/160,(int)((target>>8)&255)*l/160>255?255:(int)((target>>8)&255)*l/160,(int)((target>>16)&255)*l/160>255?255:(int)((target>>16)&255)*l/160);
 }
 static unsigned portrait_hair_colour(unsigned source,unsigned target){
  int l=((source&255)*3+((source>>8)&255)*5+((source>>16)&255)*2)/10;
  if(l<10)return source; /* Retain the authored black outline. */
  l=144+l*3/5;if(l>255)l=255;
  return RGB((target&255)*l/255,((target>>8)&255)*l/255,((target>>16)&255)*l/255);
 }
 static unsigned portrait_styled_hair_colour(unsigned source,unsigned target){
  int l=((source&255)*3+((source>>8)&255)*5+((source>>16)&255)*2)/10;
  if(l<10)return source;
  /* Four intentional pixel-art tones: dark roots, body, lit locks, glints.
   * A blue/blonde choice colours the whole traced hair mass, not a box. */
  int level=l<42?88:l<94?154:l<160?213:255;
  int r=(target&255)*level/255,g=((target>>8)&255)*level/255,b=((target>>16)&255)*level/255;
  if(level==255){r+=(255-r)/5;g+=(255-g)/5;b+=(255-b)/5;}return RGB(r,g,b);
 }
 static void portrait_draw_parts(int x,int y,int w,int h,unsigned d,int role){
  static PortraitParts parts;static unsigned pixels[4096],last;static int ready;
  if(!ready||last!=d){
   static const unsigned skin[12]={0,RGB(104,70,52),RGB(235,190,143),RGB(154,98,66),RGB(217,155,109),RGB(73,49,42),RGB(167,123,98),RGB(225,181,153),RGB(153,143,80),RGB(184,116,47),RGB(89,64,56),RGB(250,219,204)};
   static const unsigned shell[12]={0,RGB(75,115,163),RGB(194,164,99),RGB(160,85,65),RGB(136,112,183),RGB(184,201,180),RGB(78,104,102),RGB(202,154,151),RGB(50,175,105),RGB(235,162,55),RGB(220,231,235),RGB(208,77,157)};
   static const unsigned hair[12]={0,RGB(105,57,31),RGB(196,72,30),RGB(246,213,115),RGB(65,182,101),RGB(65,133,245),RGB(167,100,222),RGB(225,224,216),RGB(29,30,40),RGB(247,114,178),RGB(125,244,200),RGB(156,49,76)};
   static const unsigned outfit[6]={0,RGB(41,139,139),RGB(185,64,44),RGB(225,179,77),RGB(93,92,194),RGB(230,228,215)};
   portrait_parts_build(&parts,d);int sp=portrait_species(d),skin_id=portrait_part_value(d,2),hair_id=portrait_part_value(d,3),suit=portrait_part_value(d,5);
   for(int at=0;at<4096;at++){
    unsigned c=portrait_source_colour(parts.pixels[at]);int material=parts.ink[at];
    if(material==PC_SKIN2&&skin_id)c=portrait_material_colour(c,(sp==0||sp==7?skin:shell)[skin_id]);
    if(material==PC_HAIR1&&hair_id)c=portrait_version(d)==4&&(sp==0||sp==7)&&(portrait_part_value(d,10)||portrait_part_value(d,11))?portrait_styled_hair_colour(c,hair[hair_id]):portrait_hair_colour(c,hair[hair_id]);
    if(material==PC_SUIT1&&suit)c=portrait_material_colour(c,outfit[suit]);pixels[at]=c;
   }last=d;ready=1;
  }
  int side=w<h?w:h,ox=x+(w-side)/2,oy=y+(h-side)/2;if(side<=0)return;rect(x,y,w,h,RGB(15,24,34));
  for(int yy=0;yy<side;yy++)for(int xx=0;xx<side;xx++){int at=(yy*64/side)*64+xx*64/side;if(parts.ink[at]&&(parts.pixels[at]&0x8000)&&ox+xx>=0&&ox+xx<W&&oy+yy>=0&&oy+yy<H)fb[(oy+yy)*STRIDE+ox+xx]=pixels[at];}
  rect(x,y,w,1,RGB(206,172,92));rect(x,y+h-1,w,1,faction_colors[role>=0&&role<FACTION_COUNT?role:EXPLORERS]);rect(x,y,1,h,RGB(39,45,44));rect(x+w-1,y,1,h,RGB(39,45,44));
 }
