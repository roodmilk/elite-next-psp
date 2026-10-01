/* Four-byte, seeded character portraits assembled from reusable pixel parts. */
#ifndef ELITE_MODULAR_PORTRAIT_H
#define ELITE_MODULAR_PORTRAIT_H
#define PORTRAIT_DNA_MARK 0x80000000u
#define PORTRAIT_DNA_VERSION_SHIFT 25
#define PORTRAIT_DNA_VERSION_MASK 7u
#define PORTRAIT_DNA_VERSION 0u
#define PORTRAIT_DNA_RESERVED_MASK 0x71000000u
#define PORTRAIT_DNA_BASE (PORTRAIT_DNA_MARK|(PORTRAIT_DNA_VERSION<<PORTRAIT_DNA_VERSION_SHIFT))
#include "portrait-atlas.h"
enum {PORTRAIT_HUMAN,PORTRAIT_ELONGATED,PORTRAIT_AMPHIBIAN,PORTRAIT_INSECT,
 PORTRAIT_REPTILE,PORTRAIT_AVIAN,PORTRAIT_ROBOT,PORTRAIT_ANDROID,
 PORTRAIT_MASKED,PORTRAIT_CRYSTAL,PORTRAIT_SPECIES_COUNT};
enum {PORTRAIT_PART_SPECIES,PORTRAIT_PART_HEAD,PORTRAIT_PART_SURFACE,
 PORTRAIT_PART_HAIR,PORTRAIT_PART_EYES,PORTRAIT_PART_OUTFIT,
 PORTRAIT_PART_ACCESSORY,PORTRAIT_PART_COLOUR,PORTRAIT_PART_EXPRESSION,PORTRAIT_PART_COUNT};
static unsigned portrait_field(unsigned d,int shift,unsigned mask){return(d>>shift)&mask;}
static unsigned portrait_version(unsigned d){return portrait_field(d,PORTRAIT_DNA_VERSION_SHIFT,PORTRAIT_DNA_VERSION_MASK);}
static int portrait_valid(unsigned d){return d<8u||((d&PORTRAIT_DNA_MARK)&&!(d&PORTRAIT_DNA_RESERVED_MASK)&&portrait_version(d)<=PORTRAIT_DNA_VERSION&&(d&15u)<PORTRAIT_SPECIES_COUNT);}
static unsigned portrait_set_field(unsigned d,int shift,unsigned mask,unsigned value){return(d&~(mask<<shift))|((value&mask)<<shift)|PORTRAIT_DNA_BASE;}
static unsigned portrait_legacy(unsigned old){
 unsigned tone=old&3u,head=(old>>2)&1u;
 return PORTRAIT_DNA_BASE|PORTRAIT_HUMAN|(head<<4)|(tone<<6)|(((old*3u+1u)&7u)<<9)|(((old+1u)&3u)<<12)|(1u<<14)|(((old>>1)&3u)<<16)|(((old+2u)&7u)<<19);
}
static unsigned portrait_normalize(unsigned d){return d&PORTRAIT_DNA_MARK?d:portrait_legacy(d&7u);}
static unsigned portrait_seeded(unsigned seed,int role,int forced_species){
 /* Identity is independent from employment. If somebody changes allegiance,
  * their face remains recognisable and only the faction kit changes. */
 (void)role;unsigned h=art_hash(seed);unsigned species=forced_species>=0?(unsigned)forced_species:h%PORTRAIT_SPECIES_COUNT;
 return PORTRAIT_DNA_BASE|(species&15u)|(((h>>4)&3u)<<4)|(((h>>7)&7u)<<6)|(((h>>10)&7u)<<9)|(((h>>13)&3u)<<12)|(((h>>15)&3u)<<14)|(((h>>17)&7u)<<16)|(((h>>20)&7u)<<19)|(((h>>23)&3u)<<22);
}
static unsigned portrait_commander_random(unsigned seed){return portrait_seeded(seed,EXPLORERS,-1);}
static int portrait_species(unsigned d){return(int)portrait_field(portrait_normalize(d),0,15);}
static const char *portrait_species_name(unsigned d){static const char *const n[]={"HUMAN","ORRITH","AMPHIBIAN","INSECTOID","SAURAN","AVIAN","ROBOT","ANDROID","MASKED","CRYSTAL"};int i=portrait_species(d);return n[i>=0&&i<PORTRAIT_SPECIES_COUNT?i:0];}
static const char *portrait_part_name(int p){static const char *const n[]={"SPECIES","FACE DETAIL","SKIN / SHELL","HAIR / CREST","EYES","OUTFIT","FACE SET","COLOUR","MOUTH / EXPRESSION"};return n[p>=0&&p<PORTRAIT_PART_COUNT?p:0];}
static int portrait_part_value(unsigned d,int p){static const unsigned char shift[]={0,4,6,9,12,14,16,19,22},mask[]={15,3,7,7,3,3,7,7,3};return(int)portrait_field(portrait_normalize(d),shift[p],mask[p]);}
static int portrait_part_count(int p){return p==0?PORTRAIT_SPECIES_COUNT:(p==1||p==4||p==5||p==8)?4:8;}
static unsigned portrait_adjust(unsigned d,int p,int dir){static const unsigned char shift[]={0,4,6,9,12,14,16,19,22},mask[]={15,3,7,7,3,3,7,7,3};int count=portrait_part_count(p),v=portrait_part_value(d,p);v=(v+count+(dir<0?-1:1))%count;return portrait_set_field(portrait_normalize(d),shift[p],mask[p],v);}
static void portrait_rect(int ox,int oy,int side,int x,int y,int w,int h,unsigned c){int x0=ox+x*side/32,y0=oy+y*side/32,x1=ox+(x+w)*side/32,y1=oy+(y+h)*side/32;if(x1<=x0)x1=x0+1;if(y1<=y0)y1=y0+1;rect(x0,y0,x1-x0,y1-y0,c);}
static void portrait_dot(int ox,int oy,int side,int x,int y,unsigned c){portrait_rect(ox,oy,side,x,y,1,1,c);}
static void portrait_pulp_rect(int ox,int oy,int side,int x,int y,int w,int h,unsigned c){int x0=ox+x*side/64,y0=oy+y*side/64,x1=ox+(x+w)*side/64,y1=oy+(y+h)*side/64;if(x1<=x0)x1=x0+1;if(y1<=y0)y1=y0+1;rect(x0,y0,x1-x0,y1-y0,c);}
static int portrait_pulp_base(unsigned d){
 static const unsigned char first[PORTRAIT_SPECIES_COUNT]={0,16,24,32,40,48,64,76,56,88};
 static const unsigned char count[PORTRAIT_SPECIES_COUNT]={16,8,8,8,8,8,12,12,8,8};
 int species=portrait_species(d);unsigned head=portrait_field(d,4,3),set=portrait_field(d,16,7);return first[species]+(int)((head+(set<<2))%count[species]);
}
static unsigned portrait_pulp_variant_colour(int value){
 static const unsigned c[8]={RGB(42,30,28),RGB(116,55,37),RGB(203,76,38),RGB(224,170,55),RGB(67,132,86),RGB(42,139,153),RGB(111,76,151),RGB(213,205,177)};
 return c[value&7];
}
static void portrait_pulp_sprite(int ox,int oy,int side,unsigned d){
 static const signed char tone[8][3]={{0,0,0},{-12,-8,-4},{8,5,-2},{-4,5,9},{7,-3,-5},{10,10,6},{3,-5,9},{-8,-4,2}};
 int base=portrait_pulp_base(d),species=portrait_species(d),surface=portrait_field(d,6,7),hair=portrait_field(d,9,7),outfit=portrait_field(d,14,3);const uint16_t *src=portrait_pulp_bases[base];
 for(int dy=0;dy<side;dy++){int yy=oy+dy;if(yy<0||yy>=H)continue;int sy=dy*PORTRAIT_PULP_TILE/side;
  for(int dx=0;dx<side;dx++){int xx=ox+dx;if(xx<0||xx>=W)continue;int sx=dx*PORTRAIT_PULP_TILE/side;unsigned p=src[sy*PORTRAIT_PULP_TILE+sx];if(!(p&0x8000))continue;
   int r=((p>>10)&31)*255/31,g=((p>>5)&31)*255/31,b=(p&31)*255/31;r+=tone[surface][0];g+=tone[surface][1];b+=tone[surface][2];
   /* Recolour existing authored clusters only. No new blocks cross the face. */
   int hair_region=sy<29&&((species==PORTRAIT_HUMAN&&r<155&&g<125)||(species==PORTRAIT_ELONGATED&&sy<16)||((species>=PORTRAIT_AMPHIBIAN&&species<=PORTRAIT_AVIAN)&&((sx+sy+hair)&3)==0)||(species>=PORTRAIT_ROBOT&&r+g+b>170));
   if(hair_region&&hair){unsigned hc=portrait_pulp_variant_colour(hair);int mix=species==PORTRAIT_HUMAN?112:54;r=(r*(256-mix)+(int)(hc&255)*mix)>>8;g=(g*(256-mix)+(int)((hc>>8)&255)*mix)>>8;b=(b*(256-mix)+(int)((hc>>16)&255)*mix)>>8;}
   if(sy>47&&outfit){static const unsigned oc[4]={0,RGB(38,111,108),RGB(164,54,42),RGB(207,166,75)};unsigned q=oc[outfit];int mix=58;r=(r*(256-mix)+(int)(q&255)*mix)>>8;g=(g*(256-mix)+(int)((q>>8)&255)*mix)>>8;b=(b*(256-mix)+(int)((q>>16)&255)*mix)>>8;}
   if(r<0)r=0;if(r>255)r=255;if(g<0)g=0;if(g>255)g=255;if(b<0)b=0;if(b>255)b=255;fb[yy*STRIDE+xx]=RGB(r,g,b);
  }
 }
}
static void portrait_pulp_features(int ox,int oy,int side,unsigned d){
 static const signed char eye1x[PORTRAIT_SPECIES_COUNT]={23,37,36,18,39,38,23,23,24,31};
 static const signed char eye2x[PORTRAIT_SPECIES_COUNT]={39,46,36,46,39,38,41,41,42,35};
 static const signed char eyey[PORTRAIT_SPECIES_COUNT] ={22,22,20,23,22,21,26,24,26,24};
 static const signed char mouthx[PORTRAIT_SPECIES_COUNT]={31,42,31,32,39,-1,-1,32,-1,-1};
 static const signed char mouthy[PORTRAIT_SPECIES_COUNT]={34,34,32,39,35,-1,-1,36,-1,-1};
 int species=portrait_species(d),eyes=portrait_field(d,12,3),expression=portrait_field(d,22,3),colour=portrait_field(d,19,7);
 static const unsigned ep[4]={RGB(34,38,35),RGB(231,184,55),RGB(75,210,201),RGB(221,72,42)};unsigned eye=ep[(eyes+(colour&1))&3],glint=RGB(239,229,190);
 int ex1=eye1x[species],ex2=eye2x[species],ey=eyey[species];portrait_pulp_rect(ox,oy,side,ex1,ey,2,2,eye);portrait_pulp_rect(ox,oy,side,ex1,ey,1,1,glint);if(ex2!=ex1){portrait_pulp_rect(ox,oy,side,ex2,ey,2,2,eye);portrait_pulp_rect(ox,oy,side,ex2,ey,1,1,glint);}
 int mx=mouthx[species],my=mouthy[species];if(mx>=0){unsigned dark=RGB(45,26,25),lip=species==PORTRAIT_HUMAN?RGB(153,55,45):RGB(79,49,35),teeth=RGB(226,214,174);
  if(expression==0)portrait_pulp_rect(ox,oy,side,mx-3,my,7,1,dark);
  else if(expression==1){portrait_pulp_rect(ox,oy,side,mx-3,my-1,2,1,lip);portrait_pulp_rect(ox,oy,side,mx-1,my,4,1,lip);portrait_pulp_rect(ox,oy,side,mx+3,my-1,2,1,lip);}
  else if(expression==2){portrait_pulp_rect(ox,oy,side,mx-4,my,9,2,dark);portrait_pulp_rect(ox,oy,side,mx-2,my,5,1,teeth);}
  else {portrait_pulp_rect(ox,oy,side,mx-3,my-1,7,3,dark);portrait_pulp_rect(ox,oy,side,mx-2,my-1,5,1,teeth);}
 }
}
static void __attribute__((unused)) portrait_pulp_accessory(int ox,int oy,int side,unsigned d){
 int accessory=portrait_field(d,16,7);unsigned brass=RGB(224,169,60),glass=RGB(103,211,211),dark=RGB(25,29,30),cream=RGB(224,210,168),rust=RGB(175,59,39);
 if(accessory==1){portrait_pulp_rect(ox,oy,side,8,13,3,26,cream);portrait_pulp_rect(ox,oy,side,53,13,3,26,cream);portrait_pulp_rect(ox,oy,side,10,9,44,2,glass);portrait_pulp_rect(ox,oy,side,12,7,40,1,cream);}
 else if(accessory==2){portrait_pulp_rect(ox,oy,side,5,19,5,20,dark);portrait_pulp_rect(ox,oy,side,54,19,5,20,dark);portrait_pulp_rect(ox,oy,side,8,17,3,24,brass);portrait_pulp_rect(ox,oy,side,48,40,10,2,brass);portrait_pulp_rect(ox,oy,side,47,39,3,4,glass);}
 else if(accessory==3){portrait_pulp_rect(ox,oy,side,12,21,40,9,dark);portrait_pulp_rect(ox,oy,side,14,23,36,4,glass);portrait_pulp_rect(ox,oy,side,18,23,10,1,WHITE);}
 else if(accessory==4){portrait_pulp_rect(ox,oy,side,21,34,22,12,dark);portrait_pulp_rect(ox,oy,side,24,36,16,7,rust);portrait_pulp_rect(ox,oy,side,18,37,5,4,cream);portrait_pulp_rect(ox,oy,side,41,37,5,4,cream);}
 else if(accessory==5){portrait_pulp_rect(ox,oy,side,5,4,8,44,rust);portrait_pulp_rect(ox,oy,side,51,4,8,44,rust);portrait_pulp_rect(ox,oy,side,10,3,44,6,dark);}
 else if(accessory==6){portrait_pulp_rect(ox,oy,side,7,53,13,3,brass);portrait_pulp_rect(ox,oy,side,44,53,13,3,brass);portrait_pulp_rect(ox,oy,side,9,54,9,1,cream);portrait_pulp_rect(ox,oy,side,46,54,9,1,cream);}
 else if(accessory==7){portrait_pulp_rect(ox,oy,side,31,11,2,31,glass);portrait_pulp_rect(ox,oy,side,32,13,1,16,WHITE);portrait_pulp_rect(ox,oy,side,47,15,5,6,brass);portrait_pulp_rect(ox,oy,side,48,16,3,4,rust);}
}
static void __attribute__((unused)) portrait_pulp_faction(int ox,int oy,int side,int role){
 if(role==LAW){portrait_pulp_rect(ox,oy,side,5,52,15,6,RGB(222,213,177));portrait_pulp_rect(ox,oy,side,44,52,15,6,RGB(222,213,177));portrait_pulp_rect(ox,oy,side,8,54,9,2,RGB(70,165,201));portrait_pulp_rect(ox,oy,side,47,54,9,2,RGB(70,165,201));portrait_pulp_rect(ox,oy,side,29,49,7,9,RGB(26,52,69));portrait_pulp_rect(ox,oy,side,31,51,3,4,RGB(122,225,224));}
 else if(role==TRADERS){portrait_pulp_rect(ox,oy,side,11,49,11,4,RGB(221,164,54));portrait_pulp_rect(ox,oy,side,42,49,11,4,RGB(221,164,54));portrait_pulp_rect(ox,oy,side,14,53,6,7,RGB(28,102,94));portrait_pulp_rect(ox,oy,side,44,53,6,7,RGB(28,102,94));portrait_pulp_rect(ox,oy,side,30,53,5,6,RGB(238,198,85));}
 else if(role==PIRATES){portrait_pulp_rect(ox,oy,side,4,54,18,8,RGB(48,43,42));portrait_pulp_rect(ox,oy,side,45,50,15,12,RGB(106,45,37));portrait_pulp_rect(ox,oy,side,16,47,35,5,RGB(136,42,35));portrait_pulp_rect(ox,oy,side,9,56,3,3,RGB(224,181,111));portrait_pulp_rect(ox,oy,side,53,55,3,3,RGB(224,181,111));}
 else {portrait_pulp_rect(ox,oy,side,8,53,13,5,RGB(219,208,164));portrait_pulp_rect(ox,oy,side,43,53,13,5,RGB(219,208,164));portrait_pulp_rect(ox,oy,side,11,54,7,2,RGB(50,153,148));portrait_pulp_rect(ox,oy,side,46,54,7,2,RGB(50,153,148));portrait_pulp_rect(ox,oy,side,29,50,7,8,RGB(25,82,88));}
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
 portrait_pulp_sprite(ox,oy,side,d);portrait_pulp_features(ox,oy,side,d);portrait_pulp_badge(ox,oy,side,role);
 rect(x,y,w,1,RGB(206,172,92));rect(x,y+h-1,w,1,faction_colors[role]);rect(x,y,1,h,RGB(39,45,44));rect(x+w-1,y,1,h,RGB(39,45,44));
}
static void portrait_draw(int x,int y,int w,int h,unsigned raw,int role){
 portrait_draw_pulp(x,y,w,h,raw,role);return;
 static const unsigned surf[8][3]={{RGB(224,170,121),RGB(150,87,61),RGB(255,213,163)},{RGB(123,77,54),RGB(66,40,34),RGB(184,116,77)},{RGB(73,155,99),RGB(36,91,70),RGB(146,202,113)},{RGB(92,153,174),RGB(42,84,111),RGB(158,209,204)},{RGB(192,91,67),RGB(111,48,49),RGB(236,145,83)},{RGB(202,184,142),RGB(112,102,88),RGB(241,220,170)},{RGB(145,120,183),RGB(77,61,116),RGB(201,167,218)},{RGB(159,166,172),RGB(75,87,100),RGB(220,213,188)}};
 static const unsigned pulp[8][3]={{RGB(229,190,38),RGB(12,83,78),RGB(192,59,40)},{RGB(36,106,101),RGB(225,176,47),RGB(126,45,37)},{RGB(201,75,39),RGB(25,47,55),RGB(229,194,105)},{RGB(93,126,64),RGB(223,155,40),RGB(49,55,63)},{RGB(205,112,30),RGB(30,88,100),RGB(231,213,160)},{RGB(38,81,103),RGB(212,70,52),RGB(212,181,75)},{RGB(125,57,77),RGB(43,111,101),RGB(225,169,60)},{RGB(33,55,61),RGB(194,81,42),RGB(215,202,155)}};
 unsigned d=portrait_normalize(raw);int species=portrait_species(d),head=portrait_field(d,4,3),surface=portrait_field(d,6,7),hair=portrait_field(d,9,7),eyes=portrait_field(d,12,3),outfit=portrait_field(d,14,3),accessory=portrait_field(d,16,7),colour=portrait_field(d,19,7),expression=portrait_field(d,22,3);
 if(role<0||role>=FACTION_COUNT)role=EXPLORERS;int side=w<h?w:h,ox=x+(w-side)/2,oy=y+(h-side)/2;
 unsigned bg=pulp[colour][1],sky=pulp[colour][0],accent=pulp[colour][2],skin=surf[surface][0],shade=surf[surface][1],light=surf[surface][2];unsigned uniform=art_mix(faction_colors[role],sky,96),udark=art_tint(uniform,-70,-55,-38);
#define PR(a,b,c,e,f) portrait_rect(ox,oy,side,a,b,c,e,f)
#define PP(a,b,c) portrait_dot(ox,oy,side,a,b,c)
 rect(x,y,w,h,RGB(17,22,25));PR(1,1,30,30,bg);PR(1,1,30,11,sky);fill_disc(ox+25*side/32,oy+6*side/32,side/7,art_mix(sky,light,105));
 for(int i=0;i<5;i++){unsigned q=art_hash(d+i*101u);int bx=2+(q%27),bh=2+((q>>5)%7);PR(bx,22-bh,2,bh,bg);if(i&1)PP(bx,20-bh,accent);}for(int i=0;i<10;i++){unsigned q=art_hash(d+i*271u);PP(2+(q%28),2+((q>>8)%27),(q&3)?art_tint(bg,24,22,12):accent);}
 PR(5,25,22,6,udark);PR(3,28,26,3,uniform);PR(9,22,14,7,uniform);if(outfit==0){PR(10,23,12,2,accent);PR(15,25,2,6,bg);}else if(outfit==1){PR(8,22,16,3,art_tint(uniform,28,18,4));PR(11,25,3,6,bg);PR(18,25,3,6,bg);}else if(outfit==2){PR(7,24,18,2,accent);PR(5,28,4,3,light);PR(23,28,4,3,light);}else{PR(6,24,20,5,RGB(205,195,160));PR(9,24,14,5,uniform);PR(14,27,4,4,accent);}
 if(species==PORTRAIT_HUMAN){PR(13,20,6,5,skin);PR(9-head,6,14+head*2,15,skin);PR(10-head,7,12+head*2,4,light);PR(8-head,11,2,6,skin);PR(23+head,11,2,6,skin);if(hair<6){PR(9-head,4,14+head*2,4,shade);if(hair&1){PR(8-head,7,3,10,shade);PR(22+head,7,3,8,shade);}if(hair==2||hair==5)PR(12,2,9,4,shade);}else if(hair==6){PR(8,7,16,2,accent);PR(10,5,12,2,accent);}else PR(10,4,12,3,art_tint(skin,-28,-20,-12));}
 else if(species==PORTRAIT_ELONGATED){PR(14,21,4,5,skin);PR(11,3,10,20,skin);PR(12,4,8,5,light);PR(10,8,2,8,skin);PR(20,8,2,8,skin);if(hair&1)PR(14,1,4,4,shade);}
 else if(species==PORTRAIT_AMPHIBIAN){PR(12,20,8,5,shade);PR(6,7,20,14,skin);PR(4,10,4,8,skin);PR(24,10,4,8,skin);PR(8,8,16,4,light);if(hair&1){PR(8,5,3,4,shade);PR(21,5,3,4,shade);}}
 else if(species==PORTRAIT_INSECT){PR(13,20,6,5,shade);PR(8,6,16,16,skin);PR(6,8,5,10,shade);PR(21,8,5,10,shade);PR(10,5,2,3,skin);PR(20,5,2,3,skin);PR(9,2,1,5,accent);PR(22,2,1,5,accent);}
 else if(species==PORTRAIT_REPTILE){PR(13,20,7,5,shade);PR(8,7,17,15,skin);PR(21,12,6,7,skin);for(int i=0;i<5;i++)PR(9+i*3,4-(i&1),2,5,shade);}
 else if(species==PORTRAIT_AVIAN){PR(13,20,6,5,shade);PR(9,5,14,17,skin);PR(20,11,8,5,accent);for(int i=0;i<5;i++)PR(8+i*3,5+(i&1),3,3,light);}
 else if(species==PORTRAIT_ROBOT){fill_disc(ox+16*side/32,oy+13*side/32,side*9/32,skin);PR(8,11,16,7,shade);PR(13,21,6,5,shade);PR(15,3,3,3,accent);PR(16,1,1,3,shade);}
 else if(species==PORTRAIT_ANDROID){PR(13,20,6,5,shade);PR(9,5,14,17,skin);PR(10,6,12,4,light);PR(8,9,2,9,accent);PR(22,9,2,9,accent);PR(15,5,2,17,shade);}
 else if(species==PORTRAIT_MASKED){PR(7,5,18,20,shade);PR(5,9,4,15,uniform);PR(23,9,4,15,uniform);PR(10,8,12,13,RGB(27,32,34));PR(11,10,10,5,skin);}
 else {PR(13,21,6,4,shade);if(hair&1){PR(8,9,16,13,skin);PR(10,5,12,6,light);PR(13,2,6,5,accent);}else{PR(11,9,10,13,shade);PR(7,4,18,7,skin);PR(9,2,14,5,light);}}
 unsigned eye=eyes==0?RGB(250,225,91):eyes==1?RGB(105,225,214):eyes==2?RGB(239,97,54):RGB(232,236,214),pupil=RGB(19,24,25);
 if(species==PORTRAIT_INSECT){PR(7,8,7,8,eye);PR(18,8,7,8,eye);PR(9,10,3,4,light);PR(20,10,3,4,light);}else if(species==PORTRAIT_ROBOT){PR(9,11,5,5,eye);PR(18,11,5,5,eye);PP(11,13,pupil);PP(20,13,pupil);}else{PR(11,11,4,2,eye);PR(18,11,4,2,eye);PP(13,12,pupil);PP(19,12,pupil);if(eyes==3)PR(15,8,2,2,eye);}
 if(species!=PORTRAIT_INSECT&&species!=PORTRAIT_ROBOT){if(expression==0)PR(13,17,7,1,shade);else if(expression==1){PR(13,17,7,2,shade);PR(14,17,5,1,light);}else if(expression==2){PR(14,16,5,3,shade);PR(15,17,3,1,RGB(225,209,172));}else{PR(12,17,9,2,shade);PR(15,16,3,1,shade);}}
 if(accessory==1){circle(ox+16*side/32,oy+13*side/32,side*11/32,RGB(210,222,199));PP(7,8,WHITE);PR(6,20,5,3,shade);PR(21,20,5,3,shade);}else if(accessory==2){PR(7,9,2,10,accent);PR(23,9,2,10,accent);PR(6,11,3,6,shade);PR(23,11,3,6,shade);PR(22,18,5,1,accent);}else if(accessory==3){PR(8,10,16,5,RGB(29,57,61));PR(10,11,12,2,eye);PP(22,11,WHITE);}else if(accessory==4){PR(11,16,10,6,RGB(50,55,55));PR(13,17,6,3,accent);PR(8,18,4,2,shade);PR(20,18,4,2,shade);}else if(accessory==5){PR(6,4,20,5,udark);PR(5,8,5,17,udark);PR(22,8,5,17,udark);}else if(accessory==6){PR(6,27,3,2,accent);PR(23,27,3,2,accent);PR(7,26,1,1,light);PR(24,26,1,1,light);}else if(accessory==7){PR(15,5,2,16,eye);PP(16,9,WHITE);PR(21,7,2,3,accent);}
 /* Faction kits sit above personal clothing without replacing personal DNA.
  * Their repeated shapes read even on 20px social portraits. */
 if(role==LAW){
  PR(4,26,6,3,RGB(224,216,180));PR(22,26,6,3,RGB(224,216,180));
  PR(5,27,4,1,RGB(68,151,195));PR(23,27,4,1,RGB(68,151,195));
  PR(14,23,4,5,RGB(31,54,70));PR(15,24,2,2,RGB(116,221,224));
 }else if(role==TRADERS){
  PR(7,23,5,2,RGB(211,157,55));PR(20,23,5,2,RGB(211,157,55));
  PR(8,25,3,4,RGB(30,94,88));PR(21,25,3,4,RGB(30,94,88));
  PR(15,26,3,3,RGB(232,195,91));PP(16,27,RGB(59,45,31));
 }else if(role==PIRATES){
  PR(4,27,9,4,RGB(55,48,45));PR(21,25,7,6,RGB(104,49,41));
  PR(7,26,2,3,RGB(190,63,43));PR(24,26,1,1,RGB(225,166,63));
  PR(11,23,12,2,RGB(126,44,38));PP(6,29,RGB(220,190,126));PP(26,28,RGB(220,190,126));
 }else{
  PR(6,26,5,3,RGB(211,202,157));PR(21,26,5,3,RGB(211,202,157));
  PR(7,27,3,1,RGB(56,150,146));PR(22,27,3,1,RGB(56,150,146));
  PR(14,24,4,4,RGB(28,86,91));PP(15,25,RGB(123,231,218));PP(17,27,RGB(123,231,218));
 }
 rect(x,y,w,1,RGB(210,178,99));rect(x,y+h-1,w,1,faction_colors[role]);rect(x,y,1,h,RGB(43,51,50));rect(x+w-1,y,1,h,RGB(43,51,50));
#undef PR
#undef PP
}
static unsigned portrait_ship_computer(void){unsigned d=portrait_seeded(0x434f4d50u,EXPLORERS,PORTRAIT_ROBOT);d=portrait_set_field(d,12,3,1);d=portrait_set_field(d,16,7,7);return portrait_set_field(d,19,7,1);}
#endif
