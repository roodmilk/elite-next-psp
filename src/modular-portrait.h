/* Four-byte, seeded character portraits assembled from reusable pixel parts. */
#ifndef ELITE_MODULAR_PORTRAIT_H
#define ELITE_MODULAR_PORTRAIT_H
#define PORTRAIT_DNA_MARK 0x80000000u
#define PORTRAIT_DNA_VERSION_SHIFT 26
#define PORTRAIT_DNA_VERSION_MASK 3u
#define PORTRAIT_DNA_VERSION 1u
#define PORTRAIT_DNA_RESERVED_MASK 0u
#include "portrait-dna.h"
#define PORTRAIT_DNA_BASE (PORTRAIT_DNA_MARK|(PORTRAIT_DNA_VERSION<<PORTRAIT_DNA_VERSION_SHIFT))
#include "portrait-rig-atlas.h"
#include "portrait-legacy-atlas.h"
enum {PORTRAIT_HUMAN,PORTRAIT_ELONGATED,PORTRAIT_AMPHIBIAN,PORTRAIT_INSECT,
 PORTRAIT_REPTILE,PORTRAIT_AVIAN,PORTRAIT_ROBOT,PORTRAIT_ANDROID,
 PORTRAIT_MASKED,PORTRAIT_CRYSTAL,PORTRAIT_SPECIES_COUNT};
enum {PORTRAIT_PART_SPECIES,PORTRAIT_PART_HEAD,PORTRAIT_PART_SURFACE,
 PORTRAIT_PART_HAIR,PORTRAIT_PART_EYES,PORTRAIT_PART_OUTFIT,
 PORTRAIT_PART_ACCESSORY,PORTRAIT_PART_COLOUR,PORTRAIT_PART_EXPRESSION,PORTRAIT_PART_EARS,PORTRAIT_PART_STYLE,PORTRAIT_PART_GENDER,PORTRAIT_PART_COUNT};
static unsigned portrait_field(unsigned d,int shift,unsigned mask){return(d>>shift)&mask;}
static unsigned portrait_version(unsigned d){unsigned v=portrait_field(d,PORTRAIT_DNA_VERSION_SHIFT,PORTRAIT_DNA_VERSION_MASK);return !v&&(d&0xc0000000u)==0xc0000000u?4u:v;}
static int portrait_valid(unsigned d){return portrait_dna_valid(d);}
static unsigned portrait_set_field(unsigned d,int shift,unsigned mask,unsigned value){return(d&~(mask<<shift))|((value&mask)<<shift)|(d&PORTRAIT_DNA_MARK?PORTRAIT_DNA_MARK:PORTRAIT_DNA_BASE);}
static unsigned portrait_legacy(unsigned old){
 unsigned tone=old&3u,head=(old>>2)&1u;
 return PORTRAIT_DNA_BASE|PORTRAIT_HUMAN|(head<<4)|(tone<<6)|(((old*3u+1u)&7u)<<9)|(((old+1u)&3u)<<12)|(1u<<14)|(((old>>1)&3u)<<16)|(((old+2u)&7u)<<19);
}
static unsigned portrait_normalize(unsigned d){return d&PORTRAIT_DNA_MARK?d:portrait_legacy(d&7u);}
static unsigned portrait_seeded(unsigned seed,int role,int forced_species){
 /* Identity is independent from employment. If somebody changes allegiance,
  * their face remains recognisable and only the faction kit changes. */
 (void)role;unsigned h=art_hash(seed);unsigned species=forced_species>=0?(unsigned)forced_species:h%PORTRAIT_SPECIES_COUNT;
 return PORTRAIT_DNA_BASE|(species&15u)|(((h>>4)&3u)<<4)|(((h>>7)&7u)<<6)|(((h>>10)&7u)<<9)|(((h>>13)&3u)<<12)|(((h>>15)&3u)<<14)|(((h>>17)&7u)<<16)|(((h>>20)&7u)<<19)|(((h>>23)&3u)<<22)|(((art_hash(h^0x71839u)>>4)&1u)<<24)|(((art_hash(h^0x93731u)>>7)&1u)<<25)|(((art_hash(h^0x71839u)>>15)&7u)<<28);
}
static unsigned portrait_parts_dna(unsigned d);
static unsigned portrait_adjust(unsigned raw,int p,int dir);
static int portrait_choice_count(unsigned d,int p);
static int portrait_part_value(unsigned raw,int p);
static unsigned portrait_expand_dna(unsigned d);
static unsigned portrait_commander_random(unsigned seed){
 unsigned d=portrait_expand_dna(portrait_parts_dna(portrait_seeded(seed,EXPLORERS,-1)));
 for(int part=1;part<PORTRAIT_PART_COUNT;part++){
  int target=art_hash(seed^((unsigned)part*0x9e3779b9u))%(unsigned)portrait_choice_count(d,part);
  while(portrait_part_value(d,part)!=target)d=portrait_adjust(d,part,1);
 }return d;
}
static int portrait_species(unsigned d){return(int)portrait_field(portrait_normalize(d),0,15);}
static const char *portrait_species_name(unsigned d){static const char *const n[]={"HUMAN","ORRITH","AMPHIBIAN","INSECTOID","SAURAN","AVIAN","ROBOT","ANDROID","MASKED","CRYSTAL"};int i=portrait_species(d);return n[i>=0&&i<PORTRAIT_SPECIES_COUNT?i:0];}
static const char *portrait_part_name(int p){static const char *const n[]={"SPECIES","HEAD SHAPE","SKIN / SHELL","HAIR / CREST","EYE SHAPE","OUTFIT","FACE VARIANT","COLOUR","MOUTH STYLE","EARS / SENSORS","HAIR STYLE","GENDER"};return n[p>=0&&p<PORTRAIT_PART_COUNT?p:0];}
static int portrait_part_value(unsigned raw,int p){
 unsigned d=portrait_normalize(raw);static const unsigned char shift[]={0,4,6,9,12,14,16,19,22,28},mask[]={15,3,7,7,3,3,7,7,3,7};
 if(p<0||p>=PORTRAIT_PART_COUNT)return 0;
 if(portrait_version(d)==4){
  static const unsigned char sh[]={0,4,5,9,13,16,19,0,21,23,25,29},ma[]={15,1,15,15,7,7,3,0,3,3,1,1};
  int v=portrait_field(d,sh[p],ma[p]);return p==10?v|(((d>>28)&1u)<<1):v;
 }
 if(p>=10)return 0;
 int v=portrait_field(d,shift[p],mask[p]);if(p==4)v|=((d>>24)&1)<<2;if(p==8)v|=((d>>25)&1)<<2;
 if(portrait_version(d)==3){if(p==2)v|=((d>>19)&1)<<3;if(p==3)v|=((d>>20)&1)<<3;if(p==5)v|=((d>>21)&1)<<2;if(p==7)v=0;}return v;
}
static int portrait_part_count(int p){return p>=10?1:p==0?PORTRAIT_SPECIES_COUNT:(p==1||p==5)?4:8;}
static int portrait_choice_count(unsigned d,int p){
 if(portrait_version(d)<2)return portrait_part_count(p);
 int sp=portrait_species(d);
 if(portrait_version(d)>=3){
  if(p==10)return portrait_version(d)==4&&(sp==0||sp==7)?4:1;
  if(p==11)return portrait_version(d)==4&&(sp==0||sp==7)?2:1;
  if(p==0)return 10;if(p==1)return sp==0||sp==7?2:1;if(p==7)return 1;
  if(p==2)return 12;if(p==3)return sp==1||sp==2?1:12;if(p==5)return 6;
  if(p==4)return sp==9?1:portrait_version(d)==4?5:3;if(p==6)return sp==0||sp==7?3:1;
  if(p==8)return sp==1||sp==5||sp==9?1:3;
  if(p==9)return sp==0||sp==6||sp==7?3:1;return 1;
 }
 if(p==0)return 10;if(p==1||p==7)return 1;if(p==2)return 8;
 if(p==3)return sp==1||sp==2?1:8;
 if(p==4)return sp==9?1:2;if(p==5)return 4;if(p==6)return sp==0||sp==7?2:1;
 if(p==8)return sp==1||sp==5||sp==9?1:2;
 if(p==9)return sp==0||sp==6||sp==7?2:1;return 1;
}
static const char *portrait_choice_label(unsigned d,int p){
 if(portrait_version(d)<2)return portrait_part_name(p);
 static const char *const names[]={"SPECIES","HEAD SHAPE","SKIN / SHELL","HAIR COLOUR","EYE STYLE","OUTFIT COLOUR","FACE MARKS","BACKGROUND","MOUTH STYLE","EARS / SENSORS","HAIR STYLE","GENDER"};
 int sp=portrait_species(d);
 if(p==2&&sp==5)return "BEAK COLOUR";
 if(p==3){if(sp==5)return "FEATHER COLOUR";if(sp==6)return "TRIM COLOUR";if(sp==8)return "HOOD COLOUR";if(sp!=0&&sp!=7)return "CREST COLOUR";}
 if(p==8&&sp==8)return "GRILLE STYLE";if(p==8&&sp==6)return "JAW STYLE";
 return names[p];
}
static const char *portrait_choice_caption(unsigned d,int p){
 static const char *const skin[]={"NATURAL","DEEP","PALE","SIENNA","WARM","EBONY","TAUPE","ROSE","OLIVE","BRONZE","UMBER","PORCELAIN"};
 static const char *const shell[]={"NATURAL","COBALT","SAND","RUST","VIOLET","PALE","SLATE","ROSE","EMERALD","AMBER","PEARL","MAGENTA"};
 static const char *const colour[]={"NATURAL","BROWN","COPPER","BLONDE","JADE","BLUE","VIOLET","SILVER","BLACK","PINK","MINT","AUBURN"};
 static const char *const outfit[]={"NATURAL","TEAL","RED","GOLD","INDIGO","IVORY"};static const char *const marks[]={"CLEAR","SCAR","FRECKLES"};
 static const char *const eyes[]={"NATURAL","NARROW","LUMINOUS","WIDE","ANGLED"},*const mouth[]={"NATURAL","FULLER","SMIRK"},*const ears[]={"NATURAL","EXTENDED","POINTED"};
 int v=portrait_part_value(d,p),sp=portrait_species(d);
 if(p==10){static const char *const styles[]={"CLASSIC","CROPPED","HIGH SWEEP","SIDE PART"};return styles[v];}
 if(p==11)return v?"FEMALE":"MALE";
 if(p==0)return portrait_species_name(d);if(p==1)return v?"SLENDER":"NATURAL";if(p==2)return (sp==0||sp==7?skin:shell)[v];if(p==3)return colour[v];if(p==4)return eyes[v];if(p==5)return outfit[v];if(p==6)return marks[v];if(p==8)return mouth[v];if(p==9)return sp==6&&v==2?"ANGLED":ears[v];return "";
}
static unsigned portrait_adjust(unsigned raw,int p,int dir){
 if(p<0||p>=PORTRAIT_PART_COUNT)return raw;
 unsigned d=portrait_normalize(raw);static const unsigned char shift[]={0,4,6,9,12,14,16,19,22,28},mask[]={15,3,7,7,3,3,7,7,3,7};
 int count=portrait_choice_count(d,p);if(portrait_version(d)>=2&&count<2)return raw;
 int v=(portrait_part_value(d,p)+count+(dir<0?-1:1))%count;
 if(portrait_version(d)==4){
  static const unsigned char sh[]={0,4,5,9,13,16,19,0,21,23,25,29},ma[]={15,1,15,15,7,7,3,0,3,3,1,1};
  d=portrait_set_field(d,sh[p],ma[p],v);if(p==10)d=portrait_set_field(d,28,1,v>>1);return d;
 }
 if(p>=10)return raw;
 d=portrait_set_field(d,shift[p],mask[p],v);if(p==4)d=portrait_set_field(d,24,1,v>>2);if(p==8)d=portrait_set_field(d,25,1,v>>2);
 if(portrait_version(d)==3){if(p==2)d=portrait_set_field(d,19,1,v>>3);if(p==3)d=portrait_set_field(d,20,1,v>>3);if(p==5)d=portrait_set_field(d,21,1,v>>2);}return d;
}
static unsigned portrait_expand_dna(unsigned raw){
 if(portrait_version(raw)==4)return raw;
 unsigned old=portrait_version(raw)>=2?raw:portrait_parts_dna(raw),d=0xc0000000u;
 static const unsigned char sh[]={0,4,5,9,13,16,19,0,21,23},ma[]={15,1,15,15,7,7,3,0,3,3};
 for(int p=0;p<10;p++)if(p!=7)d|=((unsigned)portrait_part_value(old,p)&ma[p])<<sh[p];return d;
}
/* The original male/classic rig remains pixel-identical on migration. */
static unsigned portrait_expanded_legacy(unsigned d){
 unsigned old=PORTRAIT_DNA_MARK|(3u<<26);
 static const unsigned char sh[]={0,4,6,9,12,14,16,19,22,28},ma[]={15,3,7,7,3,3,7,0,3,7};
 for(int p=0;p<10;p++)if(p!=7){int v=portrait_part_value(d,p);if(p==4&&v>2)v=0;old|=((unsigned)v&ma[p])<<sh[p];if(p==2)old|=(v>>3)<<19;if(p==3)old|=(v>>3)<<20;if(p==5)old|=(v>>2)<<21;}return old;
}
static void portrait_rect(int ox,int oy,int side,int x,int y,int w,int h,unsigned c){int x0=ox+x*side/32,y0=oy+y*side/32,x1=ox+(x+w)*side/32,y1=oy+(y+h)*side/32;if(x1<=x0)x1=x0+1;if(y1<=y0)y1=y0+1;rect(x0,y0,x1-x0,y1-y0,c);}
static void portrait_dot(int ox,int oy,int side,int x,int y,unsigned c){portrait_rect(ox,oy,side,x,y,1,1,c);}
static void portrait_pulp_rect(int ox,int oy,int side,int x,int y,int w,int h,unsigned c){int x0=ox+x*side/64,y0=oy+y*side/64,x1=ox+(x+w)*side/64,y1=oy+(y+h)*side/64;if(x1<=x0)x1=x0+1;if(y1<=y0)y1=y0+1;rect(x0,y0,x1-x0,y1-y0,c);}
static int portrait_pulp_base(unsigned d){
 static const unsigned char first[PORTRAIT_SPECIES_COUNT]={0,16,24,32,40,48,64,76,56,88};
 static const unsigned char count[PORTRAIT_SPECIES_COUNT]={16,8,8,8,8,8,12,12,8,8};
 int species=portrait_species(d);unsigned head=portrait_field(d,4,3),set=portrait_field(d,16,7);
 if(portrait_version(d)==0)return first[species]+(int)((head+(set<<2))%count[species]);
 /* The atlas contains complete, hand-authored faces. Select one of those
  * faces for every structural choice instead of drawing generic features on
  * top at coordinates that only fit one head. Odd strides keep each adjacent
  * editor choice distinct across the 8, 12 and 16 portrait families. */
 unsigned eyes=(unsigned)portrait_part_value(d,PORTRAIT_PART_EYES);
 unsigned mouth=(unsigned)portrait_part_value(d,PORTRAIT_PART_EXPRESSION);
 unsigned ears=(unsigned)portrait_part_value(d,PORTRAIT_PART_EARS);
 return first[species]+(int)((head+5u*set+7u*eyes+11u*mouth+13u*ears)%count[species]);
}
static void portrait_pulp_sprite(int ox,int oy,int side,unsigned d){
 /* Legacy records select whole authored faces. Preserve their actual pixel
  * colours: rectangle/brightness heuristics tinted necks and facial shadows
  * as clothing or hair. The modular editor has explicit material masks. */
 int base=portrait_pulp_base(d);const uint16_t *src=portrait_legacy_source(base);
 for(int dy=0;dy<side;dy++){int yy=oy+dy;if(yy<0||yy>=H)continue;int sy=dy*PORTRAIT_PULP_TILE/side;
  for(int dx=0;dx<side;dx++){int xx=ox+dx;if(xx<0||xx>=W)continue;int sx=dx*PORTRAIT_PULP_TILE/side;unsigned p=src[sy*PORTRAIT_PULP_TILE+sx];if(!(p&0x8000))continue;
   fb[yy*STRIDE+xx]=RGB(((p>>10)&31)*255/31,((p>>5)&31)*255/31,(p&31)*255/31);
  }
 }
}
static void portrait_pulp_badge(int ox,int oy,int side,int role){
 /* A single readable uniform pin replaces the old geometric overlays. Keep it
  * on the lower-right shoulder so authored faces and silhouettes stay clean. */
 int x=51,y=53;unsigned dark=RGB(24,29,30),light=RGB(227,213,165);
 portrait_pulp_rect(ox,oy,side,x-1,y-1,7,7,dark);
 if(role==LAW){unsigned c=RGB(105,211,220);portrait_pulp_rect(ox,oy,side,x+1,y,3,1,c);portrait_pulp_rect(ox,oy,side,x,y+1,5,3,c);portrait_pulp_rect(ox,oy,side,x+1,y+4,3,1,c);portrait_pulp_rect(ox,oy,side,x+2,y+1,1,3,light);}
 else if(role==TRADERS){unsigned c=RGB(224,169,60);portrait_pulp_rect(ox,oy,side,x,y,1,5,c);portrait_pulp_rect(ox,oy,side,x+2,y+1,1,4,c);portrait_pulp_rect(ox,oy,side,x+4,y+2,1,3,c);}
 else if(role==PIRATES){unsigned c=RGB(192,62,42);portrait_pulp_rect(ox,oy,side,x,y,2,2,c);portrait_pulp_rect(ox,oy,side,x+1,y+1,2,2,c);portrait_pulp_rect(ox,oy,side,x+2,y+2,2,2,c);portrait_pulp_rect(ox,oy,side,x+3,y+3,2,2,c);}
 else {unsigned c=RGB(70,177,164);portrait_pulp_rect(ox,oy,side,x+2,y,1,5,c);portrait_pulp_rect(ox,oy,side,x,y+2,5,1,c);portrait_pulp_rect(ox,oy,side,x+1,y+1,3,3,c);portrait_pulp_rect(ox,oy,side,x+2,y+2,1,1,light);}
}
static void portrait_draw_pulp(int x,int y,int w,int h,unsigned raw,int role){
 static const unsigned sky[8]={RGB(221,181,32),RGB(13,108,101),RGB(185,55,39),RGB(49,91,65),RGB(214,122,29),RGB(30,79,102),RGB(119,53,72),RGB(40,50,50)};
 unsigned d=portrait_normalize(raw);int side=w<h?w:h,ox=x+(w-side)/2,oy=y+(h-side)/2,colour=portrait_field(d,19,7);if(role<0||role>=FACTION_COUNT)role=EXPLORERS;
 rect(x,y,w,h,RGB(16,20,21));rect(ox,oy,side,side,sky[colour]);
 int sunr=side/7;if(sunr<1)sunr=1;fill_disc(ox+side*52/64,oy+side*10/64,sunr,art_tint(sky[colour],40,35,18));
 for(int i=0;i<8;i++){unsigned q=art_hash(d+(unsigned)i*211u);int bx=(int)(q%58u),bh=3+(int)((q>>8)%13u);portrait_pulp_rect(ox,oy,side,bx,49-bh,4,bh,art_tint(sky[colour],-75,-67,-48));}
 portrait_pulp_sprite(ox,oy,side,d);portrait_pulp_badge(ox,oy,side,role);
 rect(x,y,w,1,RGB(206,172,92));rect(x,y+h-1,w,1,faction_colors[role]);rect(x,y,1,h,RGB(39,45,44));rect(x+w-1,y,1,h,RGB(39,45,44));
}
#include "portrait-parts.h"
static void portrait_draw(int x,int y,int w,int h,unsigned raw,int role){
 if(portrait_version(raw)>=2)portrait_draw_parts(x,y,w,h,raw,role);
 else portrait_draw_pulp(x,y,w,h,raw,role);
}
static unsigned portrait_ship_computer(void){unsigned d=portrait_seeded(0x434f4d50u,EXPLORERS,PORTRAIT_ROBOT);d=portrait_set_field(d,12,3,1);d=portrait_set_field(d,16,7,7);return portrait_set_field(d,19,7,1);}
#endif
