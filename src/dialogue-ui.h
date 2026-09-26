/* One 480x272 conversation layout for every mission and incoming channel. */
static int dialogue_page=0,dialogue_pages=1;
static char dialogue_source[1024];
/* The PSP font is ASCII: preserve authored punctuation without ??? glyphs. */
static void dialogue_ascii(char *out,int cap,const char *s){
 int n=0;const unsigned char *p=(const unsigned char *)(s?s:"");
 while(*p&&n<cap-1){
  if(p[0]==0xe2&&p[1]==0x80&&p[2]){
   unsigned c=p[2];out[n++]=(c==0x98||c==0x99)?'\'':(c==0x9c||c==0x9d)?'"':c==0xa6?'.':'-';p+=3;
  }else out[n++]=(char)*p++;
 }
 out[n]=0;
}
static const char *dialogue_skip(const char *s,int rows,int cols){
 for(int r=0;r<rows&&*s;r++){
  int len=(int)strlen(s),cut=len<cols?len:cols;
  if(len>cols)for(int k=cut;k>cols/3;k--)if(s[k]==' '){cut=k;break;}
  s+=cut;while(*s==' ')s++;
 }
 return s;
}
static void dialogue_begin(const char *title){
 preview_reset();
 header("CONVERSATION");panel(8,32,464,214);text(2,5,GOLD,"%.56s",title);
}
static void dialogue_speech(const char *speaker,int role,int seed,const char *a,const char *b){
 char body[1024];snprintf(body,sizeof(body),"%s%s%s",a?a:"",a&&a[0]&&b&&b[0]?" ":"",b?b:"");
 dialogue_ascii(body,sizeof(body),body);
 if(strcmp(body,dialogue_source)){snprintf(dialogue_source,sizeof(dialogue_source),"%s",body);dialogue_page=0;}
 const char *p=body;dialogue_pages=0;do{dialogue_pages++;p=dialogue_skip(p,6,46);}while(*p);
 if(dialogue_page>=dialogue_pages)dialogue_page=dialogue_pages-1;
 unsigned edge=!strcmp(speaker,"COMMANDER")?RGB(245,157,62):RGB(76,181,190),fill=RGB(14,29,39);
 if(!strcmp(speaker,"KEI")||!strcmp(speaker,"RYN"))draw_kei(16,64,48,!strcmp(speaker,"RYN"));
 else if(!strcmp(speaker,"COMMANDER"))draw_portrait(16,64,48,48,game.system,EXPLORERS);
 else draw_portrait(16,64,48,48,seed,role);
 rect(76,56,388,80,fill);rect(76,56,388,2,edge);rect(76,134,388,2,edge);rect(462,56,2,80,edge);
 line(76,76,64,84,edge);line(64,84,76,92,edge);rect(73,78,4,13,fill);
 speaker_name_tag(11,8,speaker,edge);
 text_wrap(11,10,46,6,WHITE,dialogue_skip(body,dialogue_page*6,46),0);
 if(dialogue_pages>1)text(39,8,CYAN,"L/R %d/%d",dialogue_page+1,dialogue_pages);
}
static void dialogue_named(const char *speaker,int role,const char *a,const char *b){
 unsigned seed=0;for(const char *p=speaker;*p;p++)seed=seed*131u+(unsigned char)*p;
 dialogue_speech(speaker,role,(int)(seed%2000)+role*37,a,b);
}
static void dialogue_context(const char *label,const char *body){
 char copy[512];dialogue_ascii(copy,sizeof(copy),body);
 rect(16,140,448,38,RGB(13,36,43));text(3,18,GOLD,"%.54s",label);
 text_wrap(3,20,54,2,WHITE,copy,0);
}
static void dialogue_reply(int index,const char *label){
 char copy[256];dialogue_ascii(copy,sizeof(copy),label);
 int y=23+index*3,py=y*8-2,active=row==index;
 unsigned edge=active?RGB(245,157,62):RGB(119,71,38),fill=active?RGB(62,36,24):RGB(28,24,23);
 rect(16,py,448,18,fill);rect(16,py,448,1,edge);rect(16,py+17,448,1,edge);rect(16,py,2,18,edge);rect(462,py,2,18,edge);
 line(464,py+5,470,py+9,edge);line(470,py+9,464,py+13,edge);
 text(3,y,active?GOLD:AMBDIM,active?">":" ");
 text_wrap(5,y,51,2,active?GOLD:AMBDIM,copy,0);
}
static void dialogue_notice(void){
 /* Feedback has a reserved area; it must never paint over reply hit targets. */
 if(game.message_time>0&&game.message[0])dialogue_context("UPDATE",game.message);
}
