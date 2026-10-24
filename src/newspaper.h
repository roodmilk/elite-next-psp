/* Print copy is immutable compiled data. Answers use separate small files;
 * prize claims save atomically with commander credits in the V32 record. */
#include "newspaper-data.h"
#include "newspaper-composition-data.h"
static void change_page(int p);
static void galnet_chrome(void);
typedef struct {unsigned magic,version,system,checksum;unsigned char sudoku[81],crossword[49];unsigned short notes[81];} NewspaperPuzzle;
typedef struct {short x,y;unsigned char ink;char text[60];} NewspaperLine;
static NewspaperPuzzle newspaper_answers;
static NewspaperLine newspaper_lines[160];
static int newspaper_line_count,newspaper_cached_system=-1,newspaper_cached_page=-1,newspaper_fit=1;
static int newspaper_mode=0,newspaper_cell=0,newspaper_number=1,newspaper_notes=0,newspaper_entry=0,newspaper_letter=0,newspaper_slot=0,newspaper_down=0,newspaper_dirty=0,newspaper_save_error=0,newspaper_loaded_slot=-1,newspaper_loaded_system=-1;
static float newspaper_turn_started=-10.f,newspaper_zoom_started=-10.f,newspaper_bad_until=-10.f;
static int newspaper_turn_direction=1,newspaper_reward_card=0;
static float newspaper_reward_started=-10.f;
static int newspaper_system(void){return game.system>=0&&game.system<256?game.system:7;}
static const NewspaperIdentity *newspaper_local_identity(void){return opening_sol(&game)?&newspaper_earth_identity:&newspaper_identity[newspaper_system()];}
static int newspaper_local_offset(void){return opening_sol(&game)?newspaper_earth_offset:newspaper_offsets[newspaper_system()];}
static const NewspaperPage *newspaper_current(void){int n=newspaper_local_identity()->page_count;int r=row>=0&&row<n?row:0;return &newspaper_pages[newspaper_local_offset()+r];}
#include "newspaper-compositions.h"
static unsigned newspaper_checksum(const NewspaperPuzzle *p){const unsigned char *b=(const unsigned char*)p;unsigned h=2166136261u;for(unsigned i=16;i<sizeof(*p);i++)h=(h^b[i])*16777619u;return h^p->system^p->version;}
static void newspaper_puzzle_path(char *p,int size,const char *suffix){snprintf(p,size,"news-puzzle-%d-%03d.%s",newspaper_loaded_slot,newspaper_loaded_system,suffix);}
static int newspaper_save(void);
#include "newspaper-picross.h"
static int newspaper_save(void){
 if(!newspaper_dirty)return newspaper_picross_save();
 char path[64],tmp[64],bak[64];newspaper_puzzle_path(path,sizeof(path),"dat");newspaper_puzzle_path(tmp,sizeof(tmp),"tmp");newspaper_puzzle_path(bak,sizeof(bak),"bak");
 newspaper_answers.magic=0x4e505a31;newspaper_answers.version=1;newspaper_answers.system=newspaper_loaded_system;newspaper_answers.checksum=newspaper_checksum(&newspaper_answers);
 FILE *f=fopen(tmp,"wb");int ok=f&&fwrite(&newspaper_answers,1,sizeof(newspaper_answers),f)==sizeof(newspaper_answers);if(f&&fclose(f))ok=0;
 if(ok){remove(bak);rename(path,bak);if(rename(tmp,path)){rename(bak,path);ok=0;}}
 if(!ok)remove(tmp);
 newspaper_save_error=!ok;if(ok)newspaper_dirty=0;return ok&&newspaper_picross_save();
}
static int newspaper_read_puzzle(const char *path){
 NewspaperPuzzle p;FILE *f=fopen(path,"rb");if(!f)return 0;int ok=fread(&p,1,sizeof(p),f)==sizeof(p)&&fgetc(f)==EOF;fclose(f);
 ok=ok&&p.magic==0x4e505a31&&p.version==1&&p.system==(unsigned)newspaper_loaded_system&&p.checksum==newspaper_checksum(&p);
 for(int i=0;ok&&i<81;i++)ok=p.sudoku[i]<=9&&p.notes[i]<=511;
 for(int i=0;ok&&i<49;i++)ok=!p.crossword[i]||(p.crossword[i]>='A'&&p.crossword[i]<='Z');
 if(ok)newspaper_answers=p;
 return ok;
}
static void newspaper_load(void){
 int s=newspaper_system();if(newspaper_loaded_slot==commander_active_slot&&newspaper_loaded_system==s)return;
 if(!newspaper_save())return;
 newspaper_loaded_slot=commander_active_slot;newspaper_loaded_system=s;memset(&newspaper_answers,0,sizeof(newspaper_answers));
 char path[64];newspaper_puzzle_path(path,sizeof(path),"dat");if(!newspaper_read_puzzle(path)){newspaper_puzzle_path(path,sizeof(path),"bak");newspaper_read_puzzle(path);}newspaper_picross_load();newspaper_save_error=0;
}
static void newspaper_reset(void){newspaper_save();newspaper_mode=newspaper_entry=newspaper_reward_card=0;newspaper_cached_page=-1;newspaper_cached_system=-1;newspaper_turn_started=-10.f;}
static void newspaper_add_text(int x,int y,int w,int h,unsigned ink,const char *s){
 int cols=w/8,lines=h/9,used=0;if(cols<1){newspaper_fit=0;return;}
 while(*s&&used<lines){int len=(int)strlen(s),cut=len<cols?len:cols;if(len>cols){int k=cut;while(k>0&&s[k]!=' ')k--;if(k>0)cut=k;}
  if(newspaper_line_count>=160||cut>=60){newspaper_fit=0;return;}NewspaperLine *l=&newspaper_lines[newspaper_line_count++];l->x=x;l->y=y+used*9;l->ink=ink;memcpy(l->text,s,cut);l->text[cut]=0;s+=cut;while(*s==' ')s++;used++;
 }if(*s)newspaper_fit=0;
}
static int newspaper_wrap_rows(const char *s,int cols){int n=0;while(*s){int cut=(int)strlen(s);if(cut>cols){cut=cols;int k=cut;while(k>0&&s[k]!=' ')k--;if(k>0)cut=k;}s+=cut;while(*s==' ')s++;n++;}return n;}
static void newspaper_cache(void){
 int s=newspaper_system()+(opening_sol(&game)?256:0);if(newspaper_cached_system==s&&newspaper_cached_page==row)return;
 newspaper_line_count=0;newspaper_fit=1;if(newspaper_composed_page()){newspaper_cached_system=s;newspaper_cached_page=row;return;}const NewspaperPage *p=newspaper_current();
 for(int i=0;i<p->count;i++){const NewspaperBlock *b=&newspaper_blocks[p->first+i];if(b->kind==2)continue;if(b->kind==1){newspaper_add_text(b->x+4,b->y+b->h-24,b->w-8,18,0,newspaper_strings[b->title]);continue;}
  int x=b->x+4,y=b->y+4,w=b->w-8;const char *title=newspaper_strings[b->title];int title_h=newspaper_wrap_rows(title,w/8)*9;
  newspaper_add_text(x,y,w,title_h,0,title);y+=title_h;
  newspaper_add_text(x,y,w,9,1,newspaper_strings[b->label]);y+=9;
  newspaper_add_text(x,y,w,9,1,newspaper_strings[b->byline]);y+=9;
  newspaper_add_text(x,y,w,b->y+b->h-4-y,0,newspaper_strings[b->body]);
 }
 newspaper_cached_system=s;newspaper_cached_page=row;
}
/* Every mark is 24x24, with its own eight-bit heraldic register. No scaling. */
static void newspaper_mark(int x,int y,int s,const unsigned *c){
 rect(x,y,24,24,c[0]);circle(x+12,y+12,10,c[1]);
 for(int k=0;k<8;k++){int px=x+5+(k%4)*4,py=y+7+(k/4)*8;if(s&(1<<k)){rect(px,py,3,3,c[1]);pixel(px+1,py-1,c[4]);}else {line(px,py+2,px+2,py,c[2]);}}
 line(x+3,y+22,x+21,y+22,c[4]);
}
static void newspaper_masthead(const NewspaperIdentity *id,const unsigned *c){
 int scale=id->masthead_style==1&&strlen(id->masthead)<=25?2:1;
 if(id->masthead_style==2){rect(46,46,417,20,c[1]);}
 unsigned ink=id->masthead_style==2?c[0]:c[1];
 for(int i=0;id->masthead[i];i++){unsigned char ch=(unsigned char)id->masthead[i];if(ch<32||ch>126)ch='?';for(int y=0;y<8;y++)for(int x=0;x<8;x++)if(font8[ch-32][y]&(128>>x))rect(50+i*8*scale+x*scale,47+y*scale,scale,scale,ink);}
 newspaper_ink_text(50,67,c[2],"%s",id->motto);
 /* Contextual untranslated glyphs occupy their own strip, never English copy. */
 if(id->publication_family==11){static const unsigned char glyph[6][6]={{14,17,4,10,17,14},{4,14,21,4,10,17},{31,4,14,17,14,4},{17,10,31,4,14,4},{14,21,14,4,17,10},{4,14,31,17,10,4}};for(int k=0;k<6;k++)for(int y=0;y<6;y++)for(int x=0;x<5;x++)if(glyph[(k+newspaper_system())%6][y]&(16>>x))pixel(403+k*9+x,67+y,c[4]);}
}
#include "newspaper-art.h"
static int newspaper_sudoku_solution(int cell){static const char sol[]="534678912672195348198342567859761423426853791713924856961537284287419635345286179";return (sol[cell]-'1'+newspaper_system()%9)%9+1;}
static int newspaper_sudoku_given(int cell){static const char givens[]="530070000600195000098000060800060003400803001700020006060000280000419005000080079";return givens[cell]!='0'?newspaper_sudoku_solution(cell):0;}
static int newspaper_sudoku_value(int cell){int g=newspaper_sudoku_given(cell);return g?g:newspaper_answers.sudoku[cell];}
static int newspaper_cross_white(int cell){int x=cell%7,y=cell/7;return (x<3&&y<3)||(x>3&&y>3);}
static int newspaper_cross_solution(int cell){static const char *words[]={"SUN","UFO","NOW","CAT","ARE","TEN"};int x=cell%7,y=cell/7;return newspaper_cross_white(cell)?words[y<3?y:y-1][x<3?x:x-4]:0;}
static int newspaper_complete(int kind){if(kind==3)return newspaper_picross_complete();for(int i=0;i<(kind==1?81:49);i++)if(kind==1?newspaper_sudoku_value(i)!=newspaper_sudoku_solution(i):newspaper_cross_white(i)&&newspaper_answers.crossword[i]!=newspaper_cross_solution(i))return 0;return 1;}
static int newspaper_reward_index(void){return opening_sol(&game)?256:newspaper_system();}
static void newspaper_claim_reward(int kind){
 if(kind<1||kind>3||!newspaper_complete(kind))return;
 unsigned bit=newspaper_reward_bit(kind);int sys=newspaper_reward_index();
 if(game.newspaper_rewards[sys]&bit)return;
 /* Credits use tenths of a unit. Never mark a prize that cannot be paid. */
 if(game.credits>100000000-10000)return;
 game.credits+=10000;game.newspaper_rewards[sys]|=bit;
 newspaper_reward_card=1;newspaper_reward_started=preview_time;game.cue=SFX_SELECT;
}
static void newspaper_reward_screen(void){
 float age=preview_time-newspaper_reward_started,t=age/.24f;if(t<0)t=0;if(t>1)t=1;
 float ease=1.f-(1.f-t)*(1.f-t)*(1.f-t);int size=32+(int)(158*ease),x=240-size/2,y=147-size/2;
 rect(x+4,y+4,size,size,RGB(4,9,16));rect(x,y,size,size,RGB(14,27,40));
 line(x,y,x+size-1,y,UI_GOLD);line(x,y,x,y+size-1,UI_GOLD);line(x+size-1,y,x+size-1,y+size-1,UI_CYAN);line(x,y+size-1,x+size-1,y+size-1,UI_CYAN);
 if(t<1.f)return;
 for(int k=0;k<18;k++){
  float a=k*6.283185f/18.f,phase=fmodf(age*.55f+k*.071f,1.f),r=17+phase*58;
  int px=240+(int)(cosf(a)*r),py=118+(int)(sinf(a)*r*.25f);
  unsigned ink=k%3==0?UI_GOLD:k%3==1?UI_CYAN:RGB(229,127,129);
  if(phase<.84f){rect(px,py,2,2,ink);if(phase<.35f){pixel(px-2,py,ink);pixel(px,py-2,ink);}}
 }
 circle(240,118,17,UI_GOLD);circle(240,118,13,UI_CYAN);
 for(int k=0;k<8;k++){float a=k*.785398f;line(240,118,240+(int)(cosf(a)*(k%2?7:11)),118+(int)(sinf(a)*(k%2?7:11)),UI_GOLD);}
 newspaper_ink_text(180,69,UI_GOLD,"CONGRATULATIONS!");newspaper_ink_text(180,82,WHITE,"PUZZLE COMPLETE");
 newspaper_ink_text(192,153,UI_GOLD,"+1,000 UNITS");newspaper_ink_text(188,174,UI_CYAN,"PRIZE CLAIMED");
 line(162,192,318,192,RGB(45,65,78));
 if(age>.65f){button_icon(192,211,'X',UI_CYAN);newspaper_ink_text(212,213,WHITE,"CONTINUE");}
}
static void newspaper_puzzle_grid(int x,int y,int step,int kind,const unsigned *c,int interactive){
 if(kind==3){newspaper_picross_preview(x,y,step,c);return;}
 int size=kind==1?9:7;
 for(int yy=0;yy<size;yy++)for(int xx=0;xx<size;xx++){int cell=yy*size+xx,px=x+xx*step,py=y+yy*step,white=kind==1||newspaper_cross_white(cell);
  unsigned fill=white?c[0]:c[1];if(interactive&&cell==newspaper_cell)fill=preview_time<newspaper_bad_until?RGB(218,105,99):RGB(245,184,89);
  rect(px+1,py+1,step-1,step-1,fill);int value=kind==1?newspaper_sudoku_value(cell):newspaper_answers.crossword[cell];
  if(value)newspaper_ink_text(px+(step-8)/2,py+(step-8)/2,c[1],"%c",kind==1?'0'+value:value);
  else if(kind==1&&step>=18&&newspaper_answers.notes[cell]){
   static const unsigned char small_digit[9][5]={{2,6,2,2,7},{6,1,2,4,7},{6,1,2,1,6},{5,5,7,1,1},{7,4,6,1,6},{3,4,6,5,2},{7,1,2,2,2},{2,5,2,5,2},{2,5,3,1,6}};
   for(int n=0;n<9;n++)if(newspaper_answers.notes[cell]&(1<<n))for(int yy=0;yy<5;yy++)for(int xx=0;xx<3;xx++)if(small_digit[n][yy]&(4>>xx))pixel(px+2+n%3*5+xx,py+1+n/3*5+yy,c[4]);
  }
 }
 for(int i=0;i<=size;i++){int thick=kind==1&&i%3==0?2:1;rect(x+i*step,y,thick,size*step+1,c[1]);rect(x,y+i*step,size*step+1,thick,c[1]);}
}
static void newspaper_puzzle_invite(const NewspaperBlock *b,int kind,const unsigned *c){
 int size=kind==3?10:kind==1?9:7,step=(b->h-40)/size;int x=b->x+12,y=b->y+20;
 newspaper_ink_text(b->x+4,b->y+4,c[1],kind==3?"PIXEL PICTURE":kind==1?"NUMBER GARDEN":"WORD DOCK");newspaper_puzzle_grid(x,y,step,kind,c,0);
 float age=preview_time-newspaper_turn_started,t=age/.25f;if(t<0)t=0;if(t>1)t=1;int last=(int)(56*t);
 int cx=x+size*step/2,cy=y+size*step/2,rx=size*step/2+5,ry=size*step/2+5;
 for(int k=1;k<=last;k++){float a=(k-1)*6.28318f/56,aa=k*6.28318f/56;line(cx+(int)(cosf(a)*rx),cy+(int)(sinf(a)*ry),cx+(int)(cosf(aa)*rx),cy+(int)(sinf(aa)*ry),c[4]);}
 button_icon(b->x+b->w-25,b->y+b->h-22,'X',c[4]);
}
static void newspaper_puzzle_screen(const unsigned *c){
 int kind=newspaper_puzzle_kind();rect(8,45,464,201,c[0]);rect(12,49,456,2,c[1]);
 newspaper_ink_text(22,52,c[1],kind==1?"NUMBER GARDEN":"WORD DOCK");
 newspaper_puzzle_grid(26,70,kind==1?18:22,kind,c,1);
 if(kind==1){
  newspaper_ink_text(212,76,c[1],"ROWS, COLUMNS, BOXES");newspaper_ink_text(212,88,c[1],"USE EACH DIGIT ONCE.");
  newspaper_ink_text(212,112,c[4],newspaper_notes?"PENCIL NOTES ON":"PENCIL NOTES OFF");
  if(newspaper_entry){for(int n=1;n<=9;n++){int x=212+(n-1)*27;rect(x,144,25,25,n==newspaper_number?c[4]:c[3]);newspaper_ink_text(x+8,152,n==newspaper_number?c[0]:c[1],"%d",n);}}
  else {newspaper_ink_text(212,144,c[1],"X CHOOSE A NUMBER");newspaper_ink_text(212,162,c[1],"SQUARE ERASE");newspaper_ink_text(212,180,c[1],"TRIANGLE PENCIL NOTES");}
 }else{
  int cell=newspaper_cell,x=cell%7,y=cell/7,start=x<3&&y<3?0:4,which=newspaper_down?x-start:y-start;int group=start?3:0;
  static const char *clue[]={"Our nearest kind of star","Unidentified flying object","The present moment","A small purring companion","Plural form of 'is'","Five plus five"};
  newspaper_ink_text(204,76,c[4],newspaper_down?"DOWN":"ACROSS");newspaper_ink_text(204,90,c[1],"%s",newspaper_cross_white(cell)?clue[group+which]:"Select a white cell.");
  /* The longest clue is 26 letters: wrap it rather than clipping at x=480. */
  if(newspaper_entry){for(int k=0;k<3;k++){int pos=newspaper_down?(start+k)*7+x:y*7+start+k;rect(234+k*40,108,32,22,k==newspaper_slot?c[4]:c[3]);if(newspaper_answers.crossword[pos])newspaper_ink_text(246+k*40,115,c[1],"%c",newspaper_answers.crossword[pos]);}
   for(int k=0;k<26;k++){int xx=210+k%9*27,yy=142+k/9*25;rect(xx,yy,25,23,k==newspaper_letter?c[4]:c[0]);newspaper_ink_text(xx+8,yy+7,c[1],"%c",'A'+k);}
  }else {newspaper_ink_text(204,130,c[1],"X ENTER THIS ANSWER");newspaper_ink_text(204,150,c[1],"TRIANGLE ACROSS/DOWN");newspaper_ink_text(204,170,c[1],"SQUARE ERASE LETTER");}
 }
 if(newspaper_complete(kind))newspaper_ink_text(212,220,c[4],game.newspaper_rewards[newspaper_reward_index()]&newspaper_reward_bit(kind)?"COMPLETE. PRIZE CLAIMED.":"COMPLETE. BALANCE FULL.");else if(newspaper_save_error)newspaper_ink_text(204,220,c[5],"SAVE FAILED; TRY AGAIN.");
 rect(0,248,480,24,BG);button_icon(10,254,'X',UI_CYAN);newspaper_ink_text(26,256,WHITE,newspaper_entry?"ENTER":"EDIT");button_icon(108,254,'S',UI_CYAN);newspaper_ink_text(124,256,WHITE,"ERASE");button_icon(208,254,'T',UI_CYAN);newspaper_ink_text(224,256,WHITE,kind==1?"NOTES":"DIRECTION");button_icon(392,254,'O',UI_CYAN);newspaper_ink_text(408,256,WHITE,"BACK");
}
static void newspaper_draw_composition(const NewspaperComposition *page,const NewspaperPrintIdentity *id){
 const unsigned *c=id->palette;rect(0,42,480,206,c[0]);newspaper_print_mark(id);
 newspaper_print_text(56,47,id->masthead,2,c[1]);newspaper_print_text(57,69,id->motto,1,c[2]);newspaper_print_text(392,50,game.systems[id->system].name,1,c[2]);rect(14,80,452,1,c[1]);
 for(int i=0;i<page->count;i++){const NewspaperPrintOp *op=&newspaper_print_ops[page->first+i];if(op->kind==0)rect(op->x,op->y,op->w,op->h,op->ink);else if(op->kind==1)newspaper_print_text(op->x,op->y,op->text,op->arg,op->ink);else if(op->kind==2)newspaper_print_picture(op);else if(op->kind==3){NewspaperBlock b={0};b.x=op->x;b.y=op->y;b.w=op->w;b.h=op->h;newspaper_puzzle_invite(&b,newspaper_puzzle_kind(),c);}}
}
static int news_screen(void){
 newspaper_load();int s=newspaper_system();const NewspaperIdentity *id=newspaper_local_identity();if(row<0||row>=(int)id->page_count)row=0;newspaper_cache();const NewspaperPage *p=newspaper_current();int kind=newspaper_puzzle_kind();const NewspaperPrintIdentity *print_id=newspaper_print_local();const NewspaperComposition *composition=newspaper_composed_page();const unsigned *c=print_id?print_id->palette:newspaper_palette[id->palette];
 if(composition)newspaper_draw_composition(composition,print_id);else {
 rect(6,43,468,205,c[0]);newspaper_mark(17,48,s,c);newspaper_masthead(id,c);rect(18,77,444,id->border_style%2+1,c[1]);
 if(id->wear_level){line(468,47,468,243,c[3]);line(467,47,467,243,c[3]);rect(8,45,4,3,c[3]);rect(469,244,3,2,c[3]);}
 for(int i=0;i<p->count;i++){const NewspaperBlock *b=&newspaper_blocks[p->first+i];rect(b->x-1,b->y-1,b->w+2,1,c[3]);if(b->kind==1)newspaper_art(b,s,c);else if(b->kind==2)newspaper_puzzle_invite(b,kind,c);}
 for(int i=0;i<newspaper_line_count;i++){NewspaperLine *l=&newspaper_lines[i];newspaper_ink_text(l->x,l->y,c[l->ink?2:1],"%s",l->text);}
 newspaper_ink_text(7,86,c[4],"^");rect(9,100,2,117,c[3]);rect(8,100+row*105/(id->page_count-1),4,10,c[4]);newspaper_ink_text(7,230,c[4],"v");
 }
 float turn=preview_time-newspaper_turn_started;if(turn>=0&&turn<.14f){int w=(int)(28*(1-turn/.14f));int x=newspaper_turn_direction>0?474-w:6;rect(x,44,w,203,c[3]);line(x,44,x,246,c[1]);}
 rect(0,248,480,24,BG);button_icon(10,254,'L',UI_CYAN);button_icon(26,254,'R',UI_CYAN);newspaper_ink_text(42,256,WHITE,"PAGES");button_icon(126,254,'l',UI_CYAN);button_icon(146,254,'r',UI_CYAN);newspaper_ink_text(166,256,WHITE,"SECTIONS");if(kind){button_icon(256,254,'X',UI_CYAN);newspaper_ink_text(272,256,WHITE,"PUZZLE");}button_icon(392,254,'O',UI_CYAN);newspaper_ink_text(408,256,WHITE,"BACK");
 
 if(newspaper_mode&&kind==3)newspaper_picross_screen(c);
 else if(newspaper_mode){
  float age=(preview_time-newspaper_zoom_started)/.13f,zoom=newspaper_mode==2?1.f-age:age;
  if(newspaper_mode==2&&age>=1.f)newspaper_mode=0;
  else if(zoom>=0&&zoom<1.f){NewspaperBlock aperture;newspaper_puzzle_aperture(&aperture);const NewspaperBlock *b=&aperture;
   int size=kind==1?9:7,step=(b->h-40)/size,xx=b->x+12,yy=b->y+20;
   rect((int)(b->x+(8-b->x)*zoom),(int)(b->y+(45-b->y)*zoom),(int)(b->w+(464-b->w)*zoom),(int)(b->h+(201-b->h)*zoom),c[0]);
   newspaper_puzzle_grid((int)(xx+(26-xx)*zoom),(int)(yy+(70-yy)*zoom),(int)(step+((kind==1?18:22)-step)*zoom),kind,c,0);
  }else newspaper_puzzle_screen(c);
 }
 if(newspaper_reward_card)newspaper_reward_screen();
 return newspaper_fit;
}
static int newspaper_input(unsigned pressed){
 if(page!=GALNET||galnet_tab!=0)return 0;
 newspaper_load();int kind=newspaper_puzzle_kind();
 if(newspaper_loaded_system!=newspaper_system()||newspaper_loaded_slot!=commander_active_slot)return 1;
 if(newspaper_reward_card){if(preview_time-newspaper_reward_started>.65f&&(pressed&(PSP_CTRL_CROSS|PSP_CTRL_CIRCLE)))newspaper_reward_card=0;return 1;}
 if(newspaper_mode&&kind==3){newspaper_picross_input(pressed);newspaper_claim_reward(kind);return 1;}
 if(newspaper_mode){
  if(newspaper_mode==2)return 1;
  if(pressed&PSP_CTRL_CIRCLE){if(newspaper_entry)newspaper_entry=0;else {newspaper_mode=2;newspaper_zoom_started=preview_time;newspaper_save();}return 1;}
  if(pressed&PSP_CTRL_TRIANGLE){if(kind==1)newspaper_notes=!newspaper_notes;else {newspaper_down=!newspaper_down;newspaper_slot=0;}return 1;}
  if(newspaper_entry){
   if(kind==1){if(pressed&PSP_CTRL_LEFT)newspaper_number=(newspaper_number+7)%9+1;if(pressed&PSP_CTRL_RIGHT)newspaper_number=newspaper_number%9+1;}
   else{int yy=newspaper_letter/9,xx=newspaper_letter%9,cols=yy==2?8:9;if(pressed&PSP_CTRL_LEFT)xx=(xx+cols-1)%cols;if(pressed&PSP_CTRL_RIGHT)xx=(xx+1)%cols;if(pressed&PSP_CTRL_UP)yy=(yy+2)%3;if(pressed&PSP_CTRL_DOWN)yy=(yy+1)%3;if(yy==2&&xx>7)xx=7;newspaper_letter=yy*9+xx;if(pressed&PSP_CTRL_LTRIGGER)newspaper_slot=(newspaper_slot+2)%3;if(pressed&PSP_CTRL_RTRIGGER)newspaper_slot=(newspaper_slot+1)%3;}
  }else if(pressed&(PSP_CTRL_UP|PSP_CTRL_DOWN|PSP_CTRL_LEFT|PSP_CTRL_RIGHT)){
   int size=kind==1?9:7,dx=(pressed&PSP_CTRL_RIGHT)?1:(pressed&PSP_CTRL_LEFT)?-1:0,dy=(pressed&PSP_CTRL_DOWN)?1:(pressed&PSP_CTRL_UP)?-1:0;
   int x=newspaper_cell%size,y=newspaper_cell/size;x=(x+dx+size)%size;y=(y+dy+size)%size;newspaper_cell=y*size+x;
  }
  if(pressed&PSP_CTRL_CROSS){
   if(!newspaper_entry){if((kind==1&&!newspaper_sudoku_given(newspaper_cell))||(kind==2&&newspaper_cross_white(newspaper_cell))){newspaper_entry=1;newspaper_slot=0;}else newspaper_bad_until=preview_time+.25f;}
   else if(kind==1){
    int valid=1,cell=newspaper_cell,v=newspaper_number,x=cell%9,y=cell/9;
    for(int i=0;i<81;i++)if(i!=cell&&(i%9==x||i/9==y||(i%9/3==x/3&&i/9/3==y/3))&&newspaper_sudoku_value(i)==v)valid=0;
    if(newspaper_notes){newspaper_answers.notes[cell]^=1<<(v-1);newspaper_dirty=1;newspaper_entry=0;}else if(valid){newspaper_answers.sudoku[cell]=v;newspaper_answers.notes[cell]=0;newspaper_dirty=1;newspaper_entry=0;}else newspaper_bad_until=preview_time+.25f;
   }else{int x=newspaper_cell%7,y=newspaper_cell/7,start=x<3&&y<3?0:4,pos=newspaper_down?(start+newspaper_slot)*7+x:y*7+start+newspaper_slot;newspaper_answers.crossword[pos]='A'+newspaper_letter;if(newspaper_answers.crossword[pos]!=newspaper_cross_solution(pos))newspaper_bad_until=preview_time+.25f;newspaper_slot=(newspaper_slot+1)%3;newspaper_dirty=1;}
  }
  if(pressed&PSP_CTRL_SQUARE){if(kind==1&&!newspaper_sudoku_given(newspaper_cell)){newspaper_answers.sudoku[newspaper_cell]=0;newspaper_answers.notes[newspaper_cell]=0;newspaper_dirty=1;}else if(kind==2){int pos=newspaper_cell;if(newspaper_entry){newspaper_slot=(newspaper_slot+2)%3;int x=pos%7,y=pos/7,start=x<3&&y<3?0:4;pos=newspaper_down?(start+newspaper_slot)*7+x:y*7+start+newspaper_slot;}newspaper_answers.crossword[pos]=0;newspaper_dirty=1;}}
  newspaper_save();newspaper_claim_reward(kind);return 1;
 }
 if(pressed&(PSP_CTRL_UP|PSP_CTRL_DOWN|PSP_CTRL_LEFT|PSP_CTRL_RIGHT)){int n=newspaper_local_identity()->page_count;newspaper_turn_direction=pressed&(PSP_CTRL_DOWN|PSP_CTRL_RIGHT)?1:-1;row=(row+n+newspaper_turn_direction)%n;newspaper_turn_started=preview_time;game.cue=SFX_PAPER;}
 if(pressed&PSP_CTRL_CROSS&&kind){newspaper_mode=1;newspaper_cell=newspaper_entry=newspaper_slot=newspaper_down=0;newspaper_zoom_started=preview_time;newspaper_claim_reward(kind);}
 if(pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER)){newspaper_save();int dir=pressed&PSP_CTRL_RTRIGGER?1:5;galnet_tab=(galnet_tab+dir)%6;row=0;newspaper_reset();}
 if(pressed&PSP_CTRL_CIRCLE){newspaper_save();newspaper_reset();change_page(HOME);}return 1;
}

