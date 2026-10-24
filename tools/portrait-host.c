/* Compile the production portrait headers on Windows for exhaustive QA.
 * Only the framebuffer and unrelated drawing dependencies are stubbed. */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#define W 480
#define H 272
#define STRIDE 512
#define RGB(r,g,b) ((unsigned)(r)|((unsigned)(g)<<8)|((unsigned)(b)<<16))
enum {EXPLORERS,LAW,TRADERS,PIRATES,FACTION_COUNT};
static unsigned storage[STRIDE*H+32],*fb=storage+16;
static unsigned faction_colors[FACTION_COUNT]={RGB(70,177,164),0,0,0};
static void rect(int x,int y,int w,int h,unsigned c){for(int yy=y;yy<y+h;yy++)for(int xx=x;xx<x+w;xx++)if(xx>=0&&xx<W&&yy>=0&&yy<H)fb[yy*STRIDE+xx]=c;}
static void fill_disc(int x,int y,int r,unsigned c){(void)x;(void)y;(void)r;(void)c;}
static unsigned art_hash(unsigned x){x^=x>>16;x*=0x7feb352du;x^=x>>15;x*=0x846ca68bu;return x^(x>>16);}
static unsigned art_tint(unsigned c,int r,int g,int b){(void)r;(void)g;(void)b;return c;}
#include "../src/modular-portrait.h"
__declspec(dllexport) unsigned ph_dna(int sp){return portrait_parts_dna(PORTRAIT_DNA_BASE|(unsigned)sp);}
__declspec(dllexport) unsigned ph_modern(int sp){return portrait_expand_dna(ph_dna(sp));}
__declspec(dllexport) unsigned ph_expand(unsigned d){return portrait_expand_dna(d);}
__declspec(dllexport) unsigned ph_random(unsigned seed){return portrait_commander_random(seed);}
__declspec(dllexport) unsigned ph_adjust(unsigned d,int p,int dir){return portrait_adjust(d,p,dir);}
__declspec(dllexport) int ph_count(unsigned d,int p){return portrait_choice_count(d,p);}
__declspec(dllexport) int ph_value(unsigned d,int p){return portrait_part_value(d,p);}
__declspec(dllexport) int ph_valid(unsigned d){return portrait_valid(d);}
__declspec(dllexport) void ph_model(unsigned d,PortraitParts *p){portrait_parts_build(p,d);}
__declspec(dllexport) void ph_render(unsigned d,int side,unsigned *out){
 portrait_draw(4,4,side,side,d,EXPLORERS);
 for(int y=0;y<side;y++)memcpy(out+y*side,fb+(y+4)*STRIDE+4,side*sizeof(unsigned));
}
__declspec(dllexport) int ph_guards(void){for(int i=0;i<16;i++)if(storage[i]||storage[STRIDE*H+16+i])return 0;return 1;}
__declspec(dllexport) unsigned ph_legacy_dna(int base){
 for(int sp=0;sp<10;sp++)for(int h=0;h<4;h++)for(int set=0;set<8;set++){
  unsigned d=PORTRAIT_DNA_BASE|(unsigned)sp|((unsigned)h<<4)|((unsigned)set<<16);
  if(portrait_pulp_base(d)==base)return d;
 }return 0;
}
typedef struct {unsigned d,h20,h32,h96;} PhRecord;
static int ph_exhaustive_version(const char *path,int version){
 FILE *out=fopen(path,"wb");if(!out)return -1;int fail=0,total=0;
 const int order[]={1,4,6,8,9,2,3,5};
 for(int sp=0;sp<10;sp++){
  unsigned base=version==2?PORTRAIT_DNA_MARK|(2u<<26)|(unsigned)sp:ph_dna(sp),d=base;int count[8],v[8]={0},done=0,local=0;
  for(int k=0;k<8;k++)count[k]=portrait_choice_count(base,order[k]);
  while(!done){
   PhRecord record={d,0,0,0};fail+=!portrait_valid(d);PortraitParts m;portrait_parts_build(&m,d);
   for(int k=0;k<8;k++){
    int part=order[k];fail+=portrait_part_value(d,part)!=v[k];
    unsigned next=portrait_adjust(d,part,1);fail+=!portrait_valid(next);
    for(int other=0;other<10;other++)if(other!=part)fail+=portrait_part_value(d,other)!=portrait_part_value(next,other);
   }
   for(int at=0;at<4096;at++)fail+=(m.ink[at]>=PC_COUNT)||(m.owner[at]>10)||(m.ink[at]&&!(m.pixels[at]&32768));
   const int sizes[]={20,32,96};
   for(int s=0;s<3;s++){
    int side=sizes[s];fb[2*STRIDE+2]=0x13579u;fb[(6+side)*STRIDE+6+side]=0x24680u;
    portrait_draw(4,4,side,side,d,EXPLORERS);unsigned h=2166136261u;int foreground=0;
    for(int y=1;y<side-1;y++)for(int x=1;x<side-1;x++){
     int at=y*64/side*64+x*64/side;if(!m.ink[at])continue;foreground++;
     h=(h^fb[(y+4)*STRIDE+x+4])*16777619u;
    }
    fail+=foreground<side*side/6;fail+=fb[2*STRIDE+2]!=0x13579u||fb[(6+side)*STRIDE+6+side]!=0x24680u;
    if(s==0)record.h20=h;if(s==1)record.h32=h;if(s==2)record.h96=h;
   }
   fail+=!ph_guards();fwrite(&record,sizeof(record),1,out);total++;local++;
   for(int k=7;k>=0;k--){d=portrait_adjust(d,order[k],1);v[k]++;if(v[k]<count[k])break;v[k]=0;if(k==0)done=1;}
  }
  printf("SPECIES %d: %d complete combinations; cumulative failures %d\n",sp,local,fail);fflush(stdout);
 }
 fclose(out);printf("TOTAL %d combinations, %d native-size renders, %d failures\n",total,total*3,fail);fflush(stdout);return fail;
}
__declspec(dllexport) int ph_exhaustive(const char *path){return ph_exhaustive_version(path,3);}
__declspec(dllexport) int ph_exhaustive_v2(const char *path){return ph_exhaustive_version(path,2);}
