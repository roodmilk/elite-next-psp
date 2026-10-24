/* Standalone PSP diagnostics: uses the production Picross input/save/renderer.
 * Run only in an empty review directory, never beside commander saves. */
#include <pspkernel.h>
#include <pspctrl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "../src/font8.h"
PSP_MODULE_INFO("Picross review",0,1,0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);
#define RGB(r,g,b) (0xff000000u|(r)|((g)<<8)|((b)<<16))
#define BG RGB(8,15,22)
#define WHITE RGB(240,240,240)
#define UI_CYAN RGB(80,230,225)
static unsigned fb[512*272];
static int clipped,failures,newspaper_cell,newspaper_mode,newspaper_save_error,newspaper_loaded_system,newspaper_loaded_slot;
static void pixel(int x,int y,unsigned c){if(x<0||x>=480||y<0||y>=272){clipped++;return;}fb[y*512+x]=c;}
static void rect(int x,int y,int w,int h,unsigned c){for(int yy=y;yy<y+h;yy++)for(int xx=x;xx<x+w;xx++)pixel(xx,yy,c);}
static void line(int x,int y,int xx,int yy,unsigned c){int dx=abs(xx-x),sx=x<xx?1:-1,dy=-abs(yy-y),sy=y<yy?1:-1,e=dx+dy;for(;;){pixel(x,y,c);if(x==xx&&y==yy)break;int e2=e*2;if(e2>=dy){e+=dy;x+=sx;}if(e2<=dx){e+=dx;y+=sy;}}}
static void newspaper_ink_text(int x,int y,unsigned c,const char *fmt,...){char s[256];va_list args;va_start(args,fmt);vsnprintf(s,sizeof(s),fmt,args);va_end(args);for(int i=0;s[i];i++){int ch=(unsigned char)s[i];if(ch<32||ch>126)ch='?';for(int yy=0;yy<8;yy++)for(int xx=0;xx<8;xx++)if(font8[ch-32][yy]&(128>>xx))pixel(x+i*8+xx,y+yy,c);}}
static void button_icon(int x,int y,char b,unsigned c){if(b=='X'){line(x+2,y+2,x+8,y+8,c);line(x+8,y+2,x+2,y+8,c);}else if(b=='S'){rect(x+2,y+2,6,6,c);rect(x+4,y+4,2,2,RGB(13,20,28));}else{for(int i=0;i<8;i++){static const int dx[]={3,2,0,-2,-3,-2,0,2},dy[]={0,2,3,2,0,-2,-3,-2};line(x+5+dx[i],y+5+dy[i],x+5+dx[(i+1)%8],y+5+dy[(i+1)%8],c);}}}
static void newspaper_puzzle_path(char *p,int n,const char *suffix){snprintf(p,n,"news-puzzle-%d-%03d.%s",newspaper_loaded_slot,newspaper_loaded_system,suffix);}
static int newspaper_save(void);
#include "../src/newspaper-picross.h"
static int newspaper_save(void){return newspaper_picross_save();}
static FILE *report;
#define CHECK(ok,label) do{int pass=!!(ok);fprintf(report,"%s %s\n",pass?"PASS":"FAIL",label);failures+=!pass;fflush(report);}while(0)
static void dump(const char *path){FILE *f=fopen(path,"wb");if(!f){failures++;return;}unsigned size=54+480*272*3;unsigned char h[54]={0};h[0]='B';h[1]='M';memcpy(h+2,&size,4);h[10]=54;h[14]=40;h[18]=224;h[19]=1;h[22]=16;h[23]=1;h[26]=1;h[28]=24;fwrite(h,1,54,f);for(int y=271;y>=0;y--)for(int x=0;x<480;x++){unsigned c=fb[y*512+x];unsigned char b[3]={c>>16,c>>8,c};fwrite(b,1,3,f);}fclose(f);}
int main(void){
 report=fopen("picross-review.txt","w");if(!report){sceKernelExitGame();return 1;}
 newspaper_loaded_slot=99;newspaper_loaded_system=0;newspaper_picross_load();memset(&newspaper_picross,0,sizeof(newspaper_picross));
 newspaper_cell=0;newspaper_picross_input(PSP_CTRL_LEFT);CHECK(newspaper_cell==9,"left wraps within row");newspaper_picross_input(PSP_CTRL_UP);CHECK(newspaper_cell==99,"up wraps within column");
 newspaper_cell=0;newspaper_picross_input(PSP_CTRL_CROSS);CHECK(newspaper_picross.cells[0]==1,"X fills selected cell");newspaper_picross_input(PSP_CTRL_CROSS);CHECK(!newspaper_picross.cells[0],"X un-fills selected cell");
 newspaper_picross_input(PSP_CTRL_SQUARE);CHECK(newspaper_picross.cells[0]==2,"Square marks empty");newspaper_picross_input(PSP_CTRL_SQUARE);CHECK(!newspaper_picross.cells[0],"Square removes empty mark");
 newspaper_picross_input(PSP_CTRL_CROSS);memset(&newspaper_picross,0,sizeof(newspaper_picross));newspaper_picross_load();CHECK(newspaper_picross.cells[0]==1,"progress reload preserves filled cell");
 newspaper_loaded_system=3;newspaper_picross_load();CHECK(!newspaper_picross.cells[0],"system progress is isolated");newspaper_loaded_system=0;newspaper_loaded_slot=98;newspaper_picross_load();CHECK(!newspaper_picross.cells[0],"commander slots are isolated");newspaper_loaded_slot=99;newspaper_picross_load();
 char path[64];newspaper_puzzle_path(path,sizeof(path),"pic");FILE *bad=fopen(path,"wb");fputs("broken",bad);fclose(bad);memset(&newspaper_picross,0,sizeof(newspaper_picross));CHECK(!newspaper_picross_read(path),"truncated sidecar is rejected");newspaper_picross_load();CHECK(!newspaper_picross.cells[0],"valid backup is restored after file corruption");
 int all_clues=1;for(int s=0;s<256;s++)for(int d=0;d<2;d++)for(int i=0;i<10;i++){int a[5];int n=newspaper_picross_clues(s,d,i,a),sum=0;for(int j=0;j<n;j++)sum+=a[j];int bits=0;for(int j=0;j<10;j++)bits+=newspaper_picross_solution(s,d?j*10+i:i*10+j);all_clues&=n>=1&&n<=5&&sum==bits&&sum+n-1<=10;}
 CHECK(all_clues,"all 256 editions have bounded exact row and column clues");
 const unsigned c[]={RGB(240,228,201),RGB(37,56,59),RGB(148,96,48),RGB(162,139,104),RGB(199,85,24),RGB(190,40,30)};
 unsigned long long start=sceKernelGetSystemTimeWide();
 for(int i=0;i<6;i++){newspaper_loaded_system=i*3;memset(&newspaper_picross,0,sizeof(newspaper_picross));CHECK(!newspaper_picross_complete(),"blank picture is incomplete");rect(0,0,480,272,BG);newspaper_picross_screen(c);char name[64];snprintf(name,sizeof(name),"picross-%d-empty.bmp",i);dump(name);
  for(int k=0;k<100;k++)newspaper_picross.cells[k]=newspaper_picross_solution(newspaper_loaded_system,k)?1:2;
  CHECK(newspaper_picross_complete(),"exact solution completes picture");rect(0,0,480,272,BG);newspaper_picross_screen(c);snprintf(name,sizeof(name),"picross-%d-solved.bmp",i);dump(name);
  newspaper_picross_preview(20,90,10,c);
  int k=0;while(!newspaper_picross_solution(newspaper_loaded_system,k))k++;newspaper_picross.cells[k]=2;CHECK(!newspaper_picross_complete(),"missing fill never completes picture");
  newspaper_picross.cells[k]=1;k=0;while(newspaper_picross_solution(newspaper_loaded_system,k))k++;newspaper_picross.cells[k]=1;CHECK(!newspaper_picross_complete(),"extra fill never completes picture");
 }
 CHECK(!clipped,"all six native layouts and previews fit 480x272 without clipping");
 start=sceKernelGetSystemTimeWide();for(int i=0;i<120;i++)newspaper_picross_screen(c);fprintf(report,"Mean shared renderer draw: %llu us (emulator)\n",(sceKernelGetSystemTimeWide()-start)/120);
 newspaper_picross_dirty=0;newspaper_mode=1;newspaper_picross_input(PSP_CTRL_CIRCLE);CHECK(!newspaper_mode,"Circle returns to newspaper");
 fprintf(report,"RESULT %d failures\n",failures);fclose(report);sceKernelExitGame();return failures;
}
