/* Native 64px rig parts. Each dot is transparent; digits are shaded hair
 * clusters, never a scaled rectangle or a brightness-derived material mask.
 * Human and android hairlines have separately authored fits. */
static const char *const portrait_crop_human[]={
 "..............000000000..............",
 "...........00111222111000............",
 ".........01112223322211100...........",
 ".......00112223333222211100..........",
 "......0111222332223322211100.........",
 ".....011122233222233222211100........",
 "....01122233221122233222111000.......",
 "...011223322111122233222211100.......",
 "..01122332211100112223222111000......",
 ".0112233221100000112223222111100.....",
 "01122332211000000011222322111100....",
 "011223221100000000011222322111100...",
 "011222211000000000001122232211100...",
 "0112221100000000000001122232211100..",
 ".011221100000000000000112223211100..",
 ".011211000000000000000011222211110..",
 "..01111000000000000000011222111100..",
 "..01110000000000000000001122111100..",
 "...011000000000000000000011211100...",
 "...01000000000000000000000111100....",
 "....000000000000000000000001100.....",
 "............................00......"
};
static const char *const portrait_crop_android[]={
 "............000000000...............",
 ".........001112222211000............",
 ".......0011222333322211100..........",
 "......0112233332223322211100........",
 ".....0112233221112233222111100......",
 "....011223221100112233222111100.....",
 "...01122322100000112233222111100....",
 "..0112232110000000112233222111100...",
 ".011223211000000000112233221111100..",
 "0112232110000000000011223221111100..",
 "0112321100000000000001122322111100..",
 "0112211000000000000000112232111100..",
 "011211000000000000000001122211100...",
 "01111000000000000000000011221100....",
 ".011000000000000000000000011100.....",
 "..000000000000000000000000000......."
};
static void portrait_authored_crop(PortraitParts *p,int sp){
 /* Retain face, neck and garment exactly. The short cut exposes background
  * where the original bob was; it does not invent coloured neck skin. */
 PortraitParts old=*p;
 for(int at=0;at<4096;at++)if(p->ink[at]==PC_HAIR1)p->ink[at]=p->owner[at]=0,p->pixels[at]=0;
 const char *const *rows=sp==7?portrait_crop_android:portrait_crop_human;
 int y0=sp==7?5:7,x0=sp==7?12:15,height=sp==7?16:22;
 static const uint16_t tones[]={0x8000,0x98a4,0xb569,0xd20d};
 for(int y=0;y<height;y++)for(int x=0;rows[y][x];x++){
  char v=rows[y][x];int at=(y0+y)*64+x0+x;
  if(v=='.'||(old.ink[at]&&old.ink[at]!=PC_HAIR1))continue;
  p->ink[at]=PC_HAIR1;p->owner[at]=11;p->pixels[at]=tones[v-'0'];
 }
}
static void portrait_authored_smirk(PortraitParts *p,int sp,PortraitBox b){
 /* The original neutral lips are removed only through their authored lip
  * mask. Nearby cheek samples supply skin, then a small asymmetric closed
  * smile is drawn at the native mouth socket. No diagonal bitmap shear. */
 for(int y=b.y;y<b.y+b.h;y++)for(int x=b.x;x<b.x+b.w;x++){
  int at=y*64+x;if(p->ink[at]!=PC_LIP)continue;
  int from=y*64+b.x+b.w;while(from<y*64+63&&p->ink[from]!=PC_SKIN2)from++;
  if(p->ink[from]==PC_SKIN2){p->ink[at]=PC_SKIN2;p->pixels[at]=p->pixels[from];p->owner[at]=9;}
 }
 static const char *const shape[]={"........a.",".......ab.",".aaaaabb..",".bbbbcc...","..bcc.....",".........."};
 static const uint16_t tones[]={0x9c83,0xc0e7,0xed8a};
 int x0=sp==7?19:23,y0=sp==7?36:39;
 for(int y=0;y<6;y++)for(int x=0;x<10;x++)if(shape[y][x]!='.'){
  int at=(y0+y)*64+x0+x;if(p->ink[at]!=PC_SKIN2&&p->ink[at]!=PC_LIP)continue;
  p->ink[at]=PC_LIP;p->owner[at]=9;p->pixels[at]=tones[shape[y][x]-'a'];
 }
}
static void portrait_authored_ear(PortraitParts *p,int sp,int style){
 /* A tapered outer rim and a shaded inner fold. Never copy neighbouring
  * bob hair into an enlarged rectangular ear socket. The visible root is
  * anchored to the original ear; the crown, eye and mouth remain untouched. */
 static const char *const extended[]={
  "....00......","...01100....","..0122100...",".012332100..",
  ".0123232100.",".0123211100.",".012211100..","..0121100...",
  "...01100....","....00......"
 };
 static const char *const pointed[]={
  "..........0.",".........10.",".......0120.",".....012210.",
  "...01233210.","..012323210.","...0122110..","....012110..",
  ".....0110...","......00...."
 };
 static const uint16_t tones[2][4]={{0x8000,0xb8c3,0xe165,0xfeaa},{0x8000,0xda4c,0xf331,0xffb6}};
 const char *const *rows=style==2?pointed:extended;int x0=sp==7?47:46,y0=sp==7?27:29;
 for(int y=0;y<10;y++)for(int x=0;x<12;x++)if(rows[y][x]!='.'){
  int at=(y0+y)*64+x0+x;if(p->owner[at]==5||p->owner[at]==9||p->ink[at]==PC_SUIT1)continue;
  p->ink[at]=PC_SKIN2;p->owner[at]=10;p->pixels[at]=tones[sp==7][rows[y][x]-'0'];
 }
}
