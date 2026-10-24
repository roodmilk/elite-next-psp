/* Native print primitives deliberately bypass the ship-paint text filter. */
static const NewspaperPrintIdentity *newspaper_print_local(void){
 if(opening_sol(&game))return NULL;
 for(unsigned i=0;i<sizeof(newspaper_print_identity)/sizeof(*newspaper_print_identity);i++)if(newspaper_print_identity[i].system==newspaper_system())return &newspaper_print_identity[i];
 return NULL;
}
static const NewspaperComposition *newspaper_composed_page(void){
 if(opening_sol(&game))return NULL;
 for(unsigned i=0;i<sizeof(newspaper_compositions)/sizeof(*newspaper_compositions);i++)if(newspaper_compositions[i].system==newspaper_system()&&newspaper_compositions[i].page==row)return &newspaper_compositions[i];
 return NULL;
}
static void newspaper_print_text(int x,int y,const char *s,int scale,unsigned ink){
 for(int i=0;s[i];i++){unsigned char ch=s[i];if(ch<32||ch>126)ch='?';for(int yy=0;yy<8;yy++)for(int xx=0;xx<8;xx++)if(font8[ch-32][yy]&(128>>xx))rect(x+(i*8+xx)*scale,y+yy*scale,scale,scale,ink);}
}
static void newspaper_ink_text(int x,int y,unsigned ink,const char *fmt,...){
 char text[256];va_list args;va_start(args,fmt);vsnprintf(text,sizeof(text),fmt,args);va_end(args);newspaper_print_text(x,y,text,1,ink);
}
static int newspaper_puzzle_kind(void){
 const NewspaperComposition *c=newspaper_composed_page();int kind=c?c->puzzle:newspaper_current()->puzzle;
 /* Rotate a third of draft editions into Picross. Authored accepted papers
  * and the Earth tutorial retain their printed puzzle and instructions. */
 return kind&&!opening_sol(&game)&&!newspaper_print_local()&&newspaper_system()%3==0?3:kind;
}
static unsigned newspaper_reward_bit(int kind){
 if(kind==3){const NewspaperComposition *c=newspaper_composed_page();kind=c?c->puzzle:newspaper_current()->puzzle;}
 return kind>=1&&kind<=2?1u<<(kind-1):0;
}
static void newspaper_puzzle_aperture(NewspaperBlock *out){
 memset(out,0,sizeof(*out));const NewspaperComposition *c=newspaper_composed_page();
 if(c){for(int i=0;i<c->count;i++){const NewspaperPrintOp *op=&newspaper_print_ops[c->first+i];if(op->kind==3){out->x=op->x;out->y=op->y;out->w=op->w;out->h=op->h;return;}}}
 else{const NewspaperPage *p=newspaper_current();for(int i=0;i<p->count;i++)if(newspaper_blocks[p->first+i].kind==2){*out=newspaper_blocks[p->first+i];return;}}
}
static void newspaper_print_mark(const NewspaperPrintIdentity *id){
 for(int y=0;y<16;y++)for(int x=0;x<16;x++){char ch=newspaper_print_marks[id->mark][y][x];if(ch!='.')rect(14+x*2,46+y*2,2,2,ch=='#'?id->palette[1]:ch=='+'?id->palette[2]:ch=='o'?id->palette[3]:id->palette[0]);}
}
static void newspaper_print_picture(const NewspaperPrintOp *op){
 const NewspaperPrintArt *a=&newspaper_print_art[op->arg];
 for(int yy=0;yy<a->h;yy++)for(int xx=0;xx<a->w;xx++){unsigned p=a->pixels[yy*a->w+xx];unsigned r=(p>>11)&31,g=(p>>5)&63,b=p&31;fb[(op->y+yy)*STRIDE+op->x+xx]=RGB((r<<3)|(r>>2),(g<<2)|(g>>4),(b<<3)|(b>>2));}
}
