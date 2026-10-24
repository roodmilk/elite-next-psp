#include "newspaper-picross-data.h"
/* Independent sidecar: existing Sudoku/crossword files and commander saves
 * are unchanged. Rewards reuse this edition's original puzzle claim bit. */
typedef struct {unsigned magic,version,system,checksum;unsigned char cells[100];} NewspaperPicross;
static NewspaperPicross newspaper_picross;
static int newspaper_picross_dirty;
static unsigned newspaper_picross_checksum(const NewspaperPicross *p){
 unsigned h=2166136261u;for(int i=0;i<100;i++)h=(h^p->cells[i])*16777619u;return h^p->system^p->version;
}
static int newspaper_picross_save(void){
 if(!newspaper_picross_dirty)return 1;
 char path[64],tmp[64],bak[64];newspaper_puzzle_path(path,sizeof(path),"pic");newspaper_puzzle_path(tmp,sizeof(tmp),"pit");newspaper_puzzle_path(bak,sizeof(bak),"pib");
 newspaper_picross.magic=0x50494331u;newspaper_picross.version=1;newspaper_picross.system=newspaper_loaded_system;newspaper_picross.checksum=newspaper_picross_checksum(&newspaper_picross);
 FILE *f=fopen(tmp,"wb");int ok=f&&fwrite(&newspaper_picross,1,sizeof(newspaper_picross),f)==sizeof(newspaper_picross);if(f&&fclose(f))ok=0;
 if(ok){remove(bak);if(rename(path,bak)){FILE *old=fopen(path,"rb");if(old){fclose(old);remove(tmp);newspaper_save_error=1;return 0;}}if(rename(tmp,path)){rename(bak,path);ok=0;}}
 if(!ok)remove(tmp);else newspaper_picross_dirty=0;
 newspaper_save_error=!ok;return ok;
}
static int newspaper_picross_read(const char *path){
 NewspaperPicross p;FILE *f=fopen(path,"rb");if(!f)return 0;
 int ok=fread(&p,1,sizeof(p),f)==sizeof(p)&&fgetc(f)==EOF;fclose(f);
 ok=ok&&p.magic==0x50494331u&&p.version==1&&p.system==(unsigned)newspaper_loaded_system&&p.checksum==newspaper_picross_checksum(&p);
 for(int i=0;ok&&i<100;i++)ok=p.cells[i]<=2;
 if(ok)newspaper_picross=p;
 return ok;
}
static void newspaper_picross_load(void){
 memset(&newspaper_picross,0,sizeof(newspaper_picross));newspaper_picross_dirty=0;
 char path[64];newspaper_puzzle_path(path,sizeof(path),"pic");if(!newspaper_picross_read(path)){newspaper_puzzle_path(path,sizeof(path),"pib");newspaper_picross_read(path);}
}
static int newspaper_picross_complete(void){
 for(int i=0;i<100;i++)if((newspaper_picross.cells[i]==1)!=newspaper_picross_solution(newspaper_loaded_system,i))return 0;
 return 1;
}
static void newspaper_picross_preview(int x,int y,int step,const unsigned *c){
 for(int i=0;i<100;i++){
  int xx=x+i%10*step,yy=y+i/10*step;
  rect(xx+1,yy+1,step-1,step-1,newspaper_picross.cells[i]==1?c[1]:c[0]);
  if(newspaper_picross.cells[i]==2&&step>3)pixel(xx+step/2,yy+step/2,c[2]);
 }
 for(int i=0;i<=10;i++){rect(x+i*step,y,1,10*step+1,c[2]);rect(x,y+i*step,10*step+1,1,c[2]);}
}
static void newspaper_picross_screen(const unsigned *c){
 rect(8,45,464,201,c[0]);newspaper_ink_text(20,49,c[1],"PIXEL PICTURE / PICROSS");
 const int gx=72,gy=82,step=16;int solved=newspaper_picross_complete();
 for(int i=0;i<10;i++){
  int clue[5],n=newspaper_picross_clues(newspaper_loaded_system,0,i,clue);
  for(int j=0;j<n;j++)newspaper_ink_text(gx-4-(n-j)*10-(clue[j]==10?6:0),gy+i*step+4,c[1],"%d",clue[j]);
  n=newspaper_picross_clues(newspaper_loaded_system,1,i,clue);
  for(int j=0;j<n;j++)newspaper_ink_text(gx+i*step+(clue[j]==10?0:4),gy-3-(n-j)*8,c[1],"%d",clue[j]);
 }
 for(int i=0;i<100;i++){
  int x=gx+i%10*step,y=gy+i/10*step;unsigned fill=newspaper_picross.cells[i]==1?c[1]:c[0];rect(x+1,y+1,step-1,step-1,fill);
  if(newspaper_picross.cells[i]==2){line(x+5,y+5,x+10,y+10,c[2]);line(x+10,y+5,x+5,y+10,c[2]);}
 }
 for(int i=0;i<=10;i++){int w=i%5==0?2:1;rect(gx+i*step,gy,w,161,c[2]);rect(gx,gy+i*step,161,w,c[2]);}
 int x=gx+newspaper_cell%10*step,y=gy+newspaper_cell/10*step;
 rect(x,y,16,2,c[4]);rect(x,y+14,16,2,c[4]);rect(x,y,2,16,c[4]);rect(x+14,y,2,16,c[4]);
 newspaper_ink_text(244,84,c[4],solved?"PICTURE REVEALED!":"REVEAL THE PICTURE");
 if(solved)newspaper_ink_text(244,103,c[1],"%s",newspaper_picross_titles[((unsigned)newspaper_loaded_system/3)%6]);
 else {newspaper_ink_text(244,106,c[1],"NUMBERS GIVE RUNS");newspaper_ink_text(244,119,c[1],"OF FILLED SQUARES.");newspaper_ink_text(244,143,c[1],"LEAVE ONE EMPTY");newspaper_ink_text(244,156,c[1],"BETWEEN EACH RUN.");}
 newspaper_ink_text(244,185,c[2],"X FILL / UNFILL");newspaper_ink_text(244,200,c[2],"SQUARE MARK EMPTY");
 if(newspaper_save_error)newspaper_ink_text(244,225,c[5],"SAVE FAILED. RETRY.");
 rect(0,248,480,24,BG);newspaper_ink_text(10,256,WHITE,"D-PAD MOVE");button_icon(144,254,'X',UI_CYAN);newspaper_ink_text(160,256,WHITE,"FILL");button_icon(214,254,'S',UI_CYAN);newspaper_ink_text(230,256,WHITE,"MARK");button_icon(392,254,'O',UI_CYAN);newspaper_ink_text(408,256,WHITE,"BACK");
}
static void newspaper_picross_input(unsigned pressed){
 if(pressed&PSP_CTRL_CIRCLE){newspaper_mode=0;newspaper_save();return;}
 int x=newspaper_cell%10,y=newspaper_cell/10;
 if(pressed&PSP_CTRL_LEFT)x=(x+9)%10;
 if(pressed&PSP_CTRL_RIGHT)x=(x+1)%10;
 if(pressed&PSP_CTRL_UP)y=(y+9)%10;
 if(pressed&PSP_CTRL_DOWN)y=(y+1)%10;
 newspaper_cell=y*10+x;
 unsigned char *cell=&newspaper_picross.cells[newspaper_cell];
 if(pressed&PSP_CTRL_CROSS){*cell=*cell==1?0:1;newspaper_picross_dirty=1;}
 if(pressed&PSP_CTRL_SQUARE){*cell=*cell==2?0:2;newspaper_picross_dirty=1;}
 if(newspaper_picross_dirty)newspaper_save();
}
