static void erl_backdrop(void); /* ERL draws its own native Hyper Circuit chrome. */
/* Portrait Broadcast / style 15. Native 480x272, resident art, real font.
 * Collect calls until footer: layout needs the actual reply count/context. */
static int dialogue_page=0,dialogue_pages=1;
static char dialogue_source[1536];
static float dialogue_focus_start;
static unsigned dialogue_focus_key;
static struct {int pending,role,seed,count,context_page;char title[96],speaker[96],body[1024],label[96],context[512],reply[4][256];} dialogue;
static void dialogue_ascii(char *out,int cap,const char *s){
 int n=0;const unsigned char *p=(const unsigned char *)(s?s:"");
 while(*p&&n<cap-1){
  if(p[0]==0xe2&&p[1]==0x80&&p[2]){unsigned c=p[2];out[n++]=(c==0x98||c==0x99)?'\'':(c==0x9c||c==0x9d)?'"':c==0xa6?'.':'-';p+=3;}
  else out[n++]=(char)*p++;
 }out[n]=0;
}
/* Identical wrapping for measurement, pagination and drawing. */
static const char *dialogue_line(const char *s,int cols,int *length){
 int n=0;while(s[n]&&s[n]!='\n'&&n<cols)n++;
 int cut=n;if(n==cols&&s[n]&&s[n]!=' '&&s[n]!='\n')for(int k=n;k>0;k--)if(s[k]==' '){cut=k;break;}
 *length=cut;s+=cut;if(*s=='\n')s++;else while(*s==' ')s++;return s;
}
static const char *dialogue_skip(const char *s,int rows,int cols){for(int r=0;r<rows&&*s;r++){int n;s=dialogue_line(s,cols,&n);}return s;}
static int dialogue_lines(const char *s,int cols){int rows=0;while(*s){int n;s=dialogue_line(s,cols,&n);rows++;}return rows;}
static void dialogue_text(int x,int y,int cols,int rows,int pitch,unsigned ink,const char *s){
 for(int r=0;r<rows&&*s;r++){int n;const char *next=dialogue_line(s,cols,&n);char buf[64];if(n>63)n=63;memcpy(buf,s,n);buf[n]=0;text_px(x,y+r*pitch,ink,"%s",buf);s=next;}
}
static void dialogue_begin(const char *title){preview_reset();memset(&dialogue,0,sizeof(dialogue));dialogue.pending=1;dialogue_ascii(dialogue.title,sizeof(dialogue.title),title);}
static void dialogue_speech(const char *speaker,int role,int seed,const char *a,const char *b){
 dialogue_ascii(dialogue.speaker,sizeof(dialogue.speaker),speaker);dialogue.role=role;dialogue.seed=seed;
 snprintf(dialogue.body,sizeof(dialogue.body),"%s%s%s",a?a:"",a&&a[0]&&b&&b[0]?" ":"",b?b:"");dialogue_ascii(dialogue.body,sizeof(dialogue.body),dialogue.body);
}
static void dialogue_named(const char *speaker,int role,const char *a,const char *b){unsigned seed=0;for(const char *p=speaker;*p;p++)seed=seed*131u+(unsigned char)*p;dialogue_speech(speaker,role,(int)(seed%2000)+role*37,a,b);}
static void dialogue_context(const char *label,const char *body){dialogue_ascii(dialogue.label,sizeof(dialogue.label),label);dialogue_ascii(dialogue.context,sizeof(dialogue.context),body);}
static void dialogue_objective(const char *body){dialogue_context("CURRENT OBJECTIVE",body);}
static void dialogue_reply(int index,const char *label){if(index<0||index>=4)return;dialogue_ascii(dialogue.reply[index],sizeof(dialogue.reply[index]),label);if(dialogue.count<=index)dialogue.count=index+1;}
static int dialogue_move(int selected,int count,unsigned pressed){
 if(count<2)return 0;
 if(count==4){if(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT))selected^=1;else if(pressed&(PSP_CTRL_UP|PSP_CTRL_DOWN))selected^=2;}
 else if(pressed&(PSP_CTRL_LEFT|PSP_CTRL_UP))selected=(selected+count-1)%count;
 else if(pressed&(PSP_CTRL_RIGHT|PSP_CTRL_DOWN))selected=(selected+1)%count;
 return selected;
}
static unsigned dialogue_edge(void){return page==ERL_PAGE?RGB(123,255,240):ship_theme_index()?UI_SIGNAL:RGB(93,191,193);}
static void dialogue_box(int x,int y,int w,int h,unsigned fill,unsigned edge){rect(x,y,w,h,fill);line(x,y,x+w-1,y,edge);line(x,y+h-1,x+w-1,y+h-1,edge);line(x,y,x,y+h-1,edge);line(x+w-1,y,x+w-1,y+h-1,edge);}
static void dialogue_key(int index,int x,int y,int w,int h){
 int active=row==index,compact=h<32;unsigned ink=RGB(16,24,32),cream=RGB(250,235,192),fill=active?RGB(255,194,70):cream;
 if(page==ERL_PAGE){ink=RGB(245,233,199);fill=active?RGB(255,96,206):RGB(48,24,70);if(active)ink=RGB(10,8,25);}
 rect(x+2,y+3,w-2,h-3,RGB(5,12,19));rect(x+3,y,w-6,h-3,fill);rect(x,y+3,w,h-9,fill);
 line(x+3,y,x+w-4,y,page==ERL_PAGE?dialogue_edge():active?RGB(255,231,149):RGB(255,251,226));line(x,y+3,x+3,y,dialogue_edge());line(x+3,y+h-4,x+w-4,y+h-4,page==ERL_PAGE?RGB(255,96,206):RGB(157,120,67));
 int nx=x+8,ny=y+(compact?3:8);rect(nx,ny,16,10,page==ERL_PAGE?RGB(10,8,25):ink);text_px(nx+4,ny+1,cream,"%d",index+1);
 int cols=(w-44)/8,pitch=compact?8:10,lines=dialogue_lines(dialogue.reply[index],cols),maxrows=(h-(compact?2:10))/pitch;
 int first=0;if(lines>maxrows&&active)first=((int)(fmaxf(0,preview_time-dialogue_focus_start)*.45f)%(1+(lines-1)/maxrows))*maxrows;
 int shown=lines-first;if(shown>maxrows)shown=maxrows;
 dialogue_text(x+32,y+(h-3-shown*pitch)/2,cols,shown,pitch,ink,dialogue_skip(dialogue.reply[index],first,cols));
 if(lines>maxrows)text_px(x+w-10,y+h-12,ink,active?">":"+");
}
static const char *dialogue_flush_footer(const char *s){
 if(!dialogue.pending)return s;dialogue.pending=0;
 unsigned ink=RGB(11,27,39),cream=RGB(250,235,192),edge=dialogue_edge();
 unsigned focus=(unsigned)row;for(const char *p=dialogue.title;*p;p++)focus=focus*33u+(unsigned char)*p;
 if(row>=0&&row<dialogue.count)for(const char *p=dialogue.reply[row];*p;p++)focus=focus*33u+(unsigned char)*p;
 if(focus!=dialogue_focus_key){dialogue_focus_key=focus;dialogue_focus_start=preview_time;}
 if(page==ERL_PAGE)erl_backdrop();else draw_next_art(intro_backdrop,INTRO_BACKDROP_W,INTRO_BACKDROP_H,0,0,W,H);
 for(int y=28;y<245;y++){rect(5,y,5,1,ink);rect(470,y,5,1,ink);}line(5,28,474,28,edge);line(5,244,474,244,edge);
 rect(0,0,W,24,ink);line(0,24,479,24,edge);
 const char *title=dialogue.title;int tlen=strlen(title);if(tlen>53)title+=(int)(fmaxf(0,preview_time-dialogue_focus_start)*4)%(tlen-53+1);
 text_px(12,8,cream,"%.53s",title);
 for(int i=0;i<4;i++)rect(445+i*5,16-i*3,3,3+i*3,edge);
 dialogue_box(16,36,334,46,ink,edge);
 const char *name=dialogue.speaker;int namecols=39,nlen=(int)strlen(name);
 if(nlen>namecols){int span=nlen-namecols,phase=(int)(preview_time*4)%(span+12);name+=phase<6?0:phase>=span+6?span:phase-6;}
 text_px(28,45,cream,"%.*s",namecols,name);
 for(int x=0;x<146;x+=3){int v=1+(int)(fabsf(sinf(x*.31f+preview_time*3)*cosf(x*.11f-preview_time*2))*6);line(28+x,70-v,28+x,70+v,edge);}
 dialogue_box(362,31,108,102,ink,edge);
 if(!strcmp(dialogue.speaker,"KEI")||!strcmp(dialogue.speaker,"RYN"))draw_kei(368,34,96,!strcmp(dialogue.speaker,"RYN"));
 else if(strstr(dialogue.speaker,"COMPUTER"))ship_computer_portrait(368,34,96,96);
 else draw_portrait(368,34,96,96,dialogue.seed,dialogue.role);
 rect(365,134,102,3,edge);
 /* Four replies use a 2x2 grid. Oversize context joins the paged speech,
  * keeping all words readable rather than squeezing them behind controls. */
 int ctx=dialogue.context[0]!=0,context_inline=ctx&&(dialogue.count==4||dialogue_lines(dialogue.context,54)>2);
 char body[1536];snprintf(body,sizeof(body),"%s%s%s",dialogue.body,context_inline?"\n":"",context_inline?dialogue.context:"");
 if(strcmp(body,dialogue_source)){snprintf(dialogue_source,sizeof(dialogue_source),"%s",body);dialogue_page=0;}
 dialogue_pages=(dialogue_lines(body,39)+3)/4;if(dialogue_pages<1)dialogue_pages=1;
 if(dialogue_page<0)dialogue_page=0;if(dialogue_page>=dialogue_pages)dialogue_page=dialogue_pages-1;
 dialogue.context_page=ctx&&!context_inline;
 int top=dialogue.count==4?88:94,bottom=dialogue.count==4?140:146;
 rect(12,top,338,bottom-top,cream);
 /* Tail points right/up to the caller, not down/left to the player. */
 for(int y=87;y<116;y++){int end=y<100?350+(y-87)*10/13:360-(y-100)*10/16;if(y>=top||y>=94)line(350,y,end,y,cream);}
 line(350,100,360,87,cream);line(360,87,350,116,cream);
 dialogue_text(24,top+5,39,4,9,ink,dialogue_skip(body,dialogue_page*4,39));
 if(dialogue_pages>1){
  if(dialogue_page+1<dialogue_pages){button_icon(312,bottom-11,'r',ink);line(329,bottom-7,341,bottom-7,ink);line(338,bottom-10,341,bottom-7,ink);line(338,bottom-4,341,bottom-7,ink);}
  if(dialogue_page>0){button_icon(38,bottom-11,'l',ink);line(24,bottom-7,35,bottom-7,ink);line(24,bottom-7,27,bottom-10,ink);line(24,bottom-7,27,bottom-4,ink);}
 }
 if(dialogue.context_page){unsigned green=RGB(126,233,180);int cy=dialogue.count==3?148:152,ch=dialogue.count==3?34:36;
  dialogue_box(12,cy,456,ch,RGB(17,49,42),green);rect(12,cy,3,ch,green);
  const char *label=dialogue.label;int lc=(int)strlen(label);if(lc>54)label+=((int)(preview_time*3)%(lc-54+1));
  text_px(22,cy+4,green,"%.54s",label);dialogue_text(22,cy+16,54,2,9,cream,dialogue.context);
 }
 for(int i=0;i<dialogue.count;i++){
  if(dialogue.count==4)dialogue_key(i,12+(i%2)*232,146+(i/2)*52,224,44);
  else if(dialogue.context_page&&dialogue.count==3)dialogue_key(i,12,184+i*20,456,19);
  else {int gap=8,w=(456-gap*(dialogue.count-1))/dialogue.count;dialogue_key(i,12+i*(w+gap),dialogue.context_page?194:154,w,dialogue.context_page?48:88);}
 }
 static char hint[96];if(!strncmp(s,"UP/DOWN",7)){snprintf(hint,sizeof(hint),"%s%s",dialogue.count==4?"D-PAD":dialogue.context_page&&dialogue.count==3?"UP/DOWN":"LEFT/RIGHT",s+7);return hint;}return s;
}
static void dialogue_notice(void){
 /* Independent status gutter: never replace objectives or reply hit targets. */
 if(game.message_time>0&&game.message[0]){char copy[256];dialogue_ascii(copy,sizeof(copy),game.message);rect(16,83,328,9,RGB(11,27,39));int len=strlen(copy),off=0;if(len>40)off=(int)(preview_time*5)%(len-40+1);text_px(20,84,RGB(250,205,119),"%.40s",copy+off);}
}
