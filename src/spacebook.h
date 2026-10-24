/* A deliberately separate, light social-network skin. Offline fiction only. */
static int opening_net_detail=0,inbox_message_page=0,inbox_last_row=-1,inbox_pages=1;
static int social_glyph(int x,int y,unsigned colour,unsigned char ch,int scale){
 static const unsigned char lower[26][5]={
 {32,84,84,84,120},{127,72,68,68,56},{56,68,68,68,32},{56,68,68,72,127},
 {56,84,84,84,24},{8,126,9,1,2},{12,82,82,82,62},{127,8,4,4,120},
 {0,68,125,64,0},{32,64,68,61,0},{127,16,40,68,0},{0,65,127,64,0},
 {124,4,24,4,120},{124,8,4,4,120},{56,68,68,68,56},{124,20,20,20,8},
 {8,20,20,24,124},{124,8,4,4,8},{72,84,84,84,32},{4,63,68,64,32},
 {60,64,64,32,124},{28,32,64,32,28},{60,64,48,64,60},{68,40,16,40,68},
 {12,80,80,80,60},{68,100,84,76,68}};
 if(ch==' ')return 4*scale;
 if(ch>='a'&&ch<='z'){
  for(int c=0;c<5;c++)for(int r=0;r<7;r++)if(lower[ch-'a'][c]&(1<<r))rect(x+c*scale,y+r*scale,scale,scale,colour);
  return 6*scale;
 }
 if(ch<32||ch>126)ch='?';
 const unsigned char *glyph=font8[ch-32];int lo=7,hi=0;
 for(int c=0;c<8;c++)for(int r=0;r<8;r++)if((glyph[r]>>(7-c))&1){if(c<lo)lo=c;if(c>hi)hi=c;}
 if(lo>hi)return 4*scale;
 for(int c=lo;c<=hi;c++)for(int r=0;r<8;r++)if((glyph[r]>>(7-c))&1)rect(x+(c-lo)*scale,y+r*scale,scale,scale,colour);
 return (hi-lo+2)*scale;
}
static void social_text(int x,int y,int right,unsigned colour,const char *s,int scale){
 for(int i=0;s[i]&&x+8*scale<=right;i++)x+=social_glyph(x,y,colour,(unsigned char)s[i],scale);
}
static int social_char_width(unsigned char ch){
 if(ch==' ')return 4;if(ch>='a'&&ch<='z')return 6;
 if(ch<32||ch>126)ch='?';
 const unsigned char *glyph=font8[ch-32];int lo=7,hi=0;
 for(int c=0;c<8;c++)for(int r=0;r<8;r++)if((glyph[r]>>(7-c))&1){if(c<lo)lo=c;if(c>hi)hi=c;}
 return lo>hi?4:hi-lo+2;
}
static void social_wrap_count(int x,int y,int width,unsigned colour,const char *s,int lines){
 /* Break at words and keep a generous three-line limit inside each card. */
 int at=0;
 for(int line=0;line<lines&&s[at];line++){
  int start=at,last=-1,used=0;
  while(s[at]&&at-start<95&&used+8<=width){if(s[at]==' ')last=at;used+=social_char_width((unsigned char)s[at]);at++;}
  if(s[at]&&last>start)at=last;
  char part[96];int n=at-start;if(n>95)n=95;memcpy(part,s+start,n);part[n]=0;
  social_text(x,y+line*10,x+width,colour,part,1);while(s[at]==' ')at++;
 }
}
static void social_wrap(int x,int y,int width,unsigned colour,const char *s){social_wrap_count(x,y,width,colour,s,3);}
static void spacebook_logo(int x,int y){
 /* Facebook-parody mark: blue tile with a white lowercase-style s. */
 const unsigned blue=RGB(49,82,145),white=RGB(255,255,255);
 rect(x,y,22,22,blue);rect(x+1,y+1,20,20,RGB(66,103,178));
 rect(x+8,y+4,6,2,white);rect(x+6,y+6,3,10,white);rect(x+9,y+11,6,2,white);rect(x+12,y+13,3,4,white);
}
static unsigned spacebook_avatar_seed(const char *name){
 unsigned h=2166136261u;for(int i=0;name&&name[i];i++){unsigned char c=(unsigned char)name[i];if(c>='A'&&c<='Z')c+='a'-'A';h=(h^c)*16777619u;}return h?h:1;
}
static void spacebook_avatar(int x,int y,int size,const char *name){
 {unsigned ph=spacebook_avatar_seed(name);int species=(ph>>3)%PORTRAIT_SPECIES_COUNT,role=(ph>>15)%FACTION_COUNT;portrait_draw(x,y,size,size,portrait_seeded(ph,role,species),role);return;}
 /* Username-keyed 20px portraits: no texture, allocation or save state. */
 unsigned h=spacebook_avatar_seed(name),family=(h>>3)%10;
 static const unsigned palette[][3]={
  {RGB(236,210,165),RGB(139,82,56),RGB(50,40,62)},{RGB(182,215,122),RGB(71,122,74),RGB(36,57,74)},
  {RGB(130,203,212),RGB(56,109,145),RGB(32,40,61)},{RGB(214,151,196),RGB(121,71,125),RGB(48,37,62)},
  {RGB(214,176,104),RGB(139,99,56),RGB(51,44,60)},{RGB(169,174,184),RGB(89,101,116),RGB(32,41,54)},
  {RGB(240,139,104),RGB(147,68,79),RGB(53,37,63)},{RGB(159,145,220),RGB(90,77,155),RGB(41,40,70)}};
 const unsigned *p=palette[(h>>8)&7];unsigned bg=palette[(h>>17)&7][2],skin=p[0],shade=p[1],dark=p[2];
 int s=size/20;if(s<1)s=1;int ox=x+(size-20*s)/2,oy=y+(size-20*s)/2;
#define AVRECT(px,py,pw,ph,c) rect(ox+(px)*s,oy+(py)*s,(pw)*s,(ph)*s,c)
 rect(x,y,size,size,bg);AVRECT(1,1,18,18,p[(h>>22)&1]);AVRECT(2,2,16,16,bg);
 /* Silhouette families: human, reptile, insect, robot, aquatic, fungal,
  * avian, furry, crystalline and ship/logo accounts. */
 if(family==9){
  AVRECT(4,9,12,3,skin);AVRECT(7,6,6,3,shade);AVRECT(2,11,4,2,shade);AVRECT(14,11,4,2,shade);
  AVRECT(9,8,2,2,WHITE);AVRECT(5,14,10,1,dark);
 }else if(family==8){
  AVRECT(8,3,4,3,skin);AVRECT(5,6,10,2,skin);AVRECT(3,8,14,5,shade);AVRECT(6,13,8,4,skin);
  AVRECT(8,7,2,6,WHITE);AVRECT(10,7,2,6,WHITE);
 }else{
  int wide=family==1||family==3||family==7;
  AVRECT(wide?4:5,4,wide?12:10,11,skin);AVRECT(6,14,8,4,shade);
  if(family==0){AVRECT(5,3,10,3,shade);if(h&1){AVRECT(4,5,2,8,shade);AVRECT(14,5,2,8,shade);}}
  if(family==1){AVRECT(2,6,3,5,skin);AVRECT(15,6,3,5,skin);AVRECT(7,13,6,2,shade);}
  if(family==2){AVRECT(2,2,2,7,shade);AVRECT(16,2,2,7,shade);AVRECT(4,7,12,6,skin);}
  if(family==3){AVRECT(4,4,12,11,RGB(135,150,168));AVRECT(6,6,8,5,dark);AVRECT(5,3,3,2,shade);AVRECT(12,3,3,2,shade);}
  if(family==4){AVRECT(3,8,3,5,shade);AVRECT(14,8,3,5,shade);AVRECT(8,2,4,3,skin);}
  if(family==5){AVRECT(3,3,14,5,skin);AVRECT(6,8,8,8,shade);AVRECT(4,5,2,2,WHITE);AVRECT(14,5,2,2,WHITE);}
  if(family==6){AVRECT(5,4,10,8,skin);AVRECT(8,10,4,4,shade);AVRECT(9,12,2,2,dark);}
  if(family==7){AVRECT(3,3,4,5,shade);AVRECT(13,3,4,5,shade);AVRECT(4,6,12,10,skin);}
  /* Eyes and expressions vary independently within every face family. */
  int ey=(family==2)?8:family==5?10:8,span=(h>>12)&1;
  unsigned eye=(h&4)?RGB(255,231,115):RGB(217,251,255);
  AVRECT(span?6:7,ey,2,2,eye);AVRECT(span?12:11,ey,2,2,eye);
  if(h&0x20){AVRECT(7,12,6,1,dark);}else{AVRECT(8,12,4,2,dark);AVRECT(9,12,2,1,skin);}
  if(h&0x40)AVRECT(9,5,2,7,RGB(99,230,215)); /* visor/cyber stripe */
  if(h&0x80){AVRECT(3,15,3,2,RGB(240,198,91));AVRECT(14,15,3,2,RGB(240,198,91));} /* collar pins */
 }
 /* Account-status corner mark gives logos and faces the same social grammar. */
 AVRECT(15,15,4,4,RGB(53,95,165));AVRECT(16,16,2,2,(h&0x100)?RGB(100,232,177):WHITE);
#undef AVRECT
}
/* A Spacebook post keeps the event, ship, author seed and timestamp rather
 * than a full framebuffer dump.  Rebuild a tiny "evidence capture" from that
 * durable data: it reads like a screenshot, costs no save space, and means
 * old posts receive the same treatment as new ones. */
static void spacebook_photo_ship(int x,int y,unsigned paint,int flip){
 unsigned shade=livery_tint(paint,-58),bright=livery_tint(paint,26);
 if(flip){
  rect(x+4,y+8,23,5,shade);rect(x+8,y+5,14,11,paint);rect(x+11,y+3,8,3,bright);
  rect(x,y+9,8,3,paint);rect(x+27,y+9,8,3,paint);rect(x+10,y+16,7,2,shade);rect(x+20,y+16,7,2,shade);
 }else{
  rect(x+8,y+8,23,5,shade);rect(x+8,y+5,14,11,paint);rect(x+16,y+3,8,3,bright);
  rect(x+27,y+9,8,3,paint);rect(x,y+9,8,3,paint);rect(x+18,y+16,7,2,shade);rect(x+8,y+16,7,2,shade);
 }
}
static void spacebook_snapshot(int x,int y,int w,int h,unsigned code){
 unsigned seed=(code>>9)&0x1fffffu,kind=code&31,ship=(code>>5)&15;
 unsigned sky=RGB(13+(seed&7),21+((seed>>3)&9),40+((seed>>7)&12));
 unsigned paint=ship<16?ship_paint[ship]:GOLD;
 rect(x,y,w,h,RGB(71,91,116));rect(x+2,y+2,w-4,h-4,sky);
 /* Sparse, repeatable stars make every capture feel like a specific moment. */
 for(int i=0;i<8;i++){unsigned q=seed+(unsigned)i*0x9e3779b9u;int sx=x+4+(q%(unsigned)(w-8)),sy=y+4+((q>>9)%(unsigned)(h-8));pixel(sx,sy,(i&2)?RGB(127,170,204):RGB(224,232,220));}
 if(kind==SB_DOCK||kind==SB_LAUNCH){
  unsigned hull=RGB(70,88,102);rect(x+7,y+5,20,h-10,hull);rect(x+10,y+7,14,h-14,RGB(7,13,20));
  rect(x+14,y+9,6,h-18,RGB(28,42,48));line(x+4,y+h-6,x+w-5,y+h-6,RGB(171,126,72));
  if(kind==SB_LAUNCH){rect(x+w-16,y+4,9,9,RGB(238,186,86));spacebook_photo_ship(x+27,y+10,paint,0);}
  else spacebook_photo_ship(x+31,y+10,paint,1);
 }else if(kind==SB_BOUNTY||kind==SB_DESTROY){
  spacebook_photo_ship(x+12,y+10,paint,0);spacebook_photo_ship(x+w-46,y+7,kind==SB_BOUNTY?RGB(175,65,61):RGB(113,119,132),1);
  line(x+39,y+13,x+w-29,y+13,RGB(248,142,85));line(x+42,y+16,x+w-32,y+16,RGB(255,221,121));
  for(int i=0;i<5;i++)pixel(x+w-22+(i&1)*3,y+14+(i/2)*3,RGB(255,189,83));
 }else if(kind==SB_ARREST||kind==SB_FINE||kind==SB_FLEE){
  spacebook_photo_ship(x+14,y+11,paint,0);spacebook_photo_ship(x+w-42,y+6,RGB(76,142,231),1);
  line(x+w-20,y+4,x+w-20,y+9,RGB(240,84,84));line(x+w-23,y+7,x+w-17,y+7,RGB(240,84,84));
  if(kind==SB_FINE)rect(x+47,y+7,16,13,RGB(50,104,87));
 }else if(kind==SB_LAND||kind==SB_FLORA||kind==SB_FAUNA){
  rect(x+2,y+h-13,w-4,11,kind==SB_LAND?RGB(111,87,59):RGB(56,104,67));
  line(x+2,y+h-14,x+w-3,y+h-14,RGB(177,139,82));
  if(kind==SB_LAND)spacebook_photo_ship(x+45,y+8,paint,0);
  else if(kind==SB_FLORA){rect(x+30,y+13,3,11,RGB(86,156,74));rect(x+24,y+10,7,6,RGB(107,188,90));rect(x+32,y+7,8,10,RGB(84,145,80));rect(x+38,y+12,8,6,RGB(117,203,94));}
  else {rect(x+28,y+13,15,7,RGB(188,153,94));rect(x+25,y+10,7,6,RGB(206,180,112));rect(x+40,y+9,5,5,RGB(206,180,112));pixel(x+27,y+12,RGB(12,18,22));}
 }else if(kind==SB_RIFT){
  for(int r=0;r<4;r++){int xx=x+42-r*5,yy=y+13-r*3;rect(xx,yy,18+r*10,3,RGB(75+r*24,95+r*18,178+r*12));}
  rect(x+48,y+10,8,11,RGB(191,116,237));pixel(x+51,y+14,WHITE);
 }else if(kind==SB_SALVAGE){
  spacebook_photo_ship(x+13,y+9,paint,0);rect(x+w-31,y+13,9,8,RGB(178,154,84));line(x+39,y+15,x+w-31,y+15,RGB(79,214,206));
 }else if(kind==SB_PAINT||kind==SB_SHIP){
  rect(x+12,y+5,w-24,h-10,RGB(25,34,45));rect(x+14,y+7,w-28,h-14,RGB(42,54,64));spacebook_photo_ship(x+43,y+8,paint,0);
  line(x+18,y+9,x+25,y+9,RGB(180,211,230));line(x+18,y+12,x+23,y+12,RGB(180,211,230));
 }else{
  /* Arrivals, departures, contracts and idle gossip still get a real scene. */
  spacebook_photo_ship(x+40,y+9,paint,(seed>>6)&1);
  if(kind==SB_ARRIVE||kind==SB_RETURN){line(x+10,y+17,x+35,y+17,RGB(93,170,238));line(x+13,y+20,x+35,y+20,RGB(69,113,186));}
  else if(kind==SB_LEAVE){line(x+70,y+14,x+w-11,y+14,RGB(184,112,219));line(x+73,y+17,x+w-8,y+17,RGB(102,157,229));}
  else if(kind==SB_JOB){rect(x+15,y+8,19,15,RGB(157,118,63));rect(x+18,y+11,13,2,RGB(225,203,151));rect(x+18,y+16,10,2,RGB(225,203,151));}
 }
 /* The tiny REC marker gives the shots a distinct in-world camera grammar. */
 rect(x+5,y+5,3,3,RGB(239,83,83));rect(x+10,y+5,14,2,RGB(205,217,226));
}
/* Page-sized thumb follows the visible cards, including partial last pages. */
static void galnet_scrollbar(int x,int y,int height,int first,int visible,int total,unsigned track,unsigned thumb){
 if(total<=0||visible<=0||height<=0)return;
 if(first<0)first=0;if(first>=total)first=total-1;
 int end=first+visible;if(end>total)end=total;
 int top=height*first/total,bottom=height*end/total;
 rect(x,y,3,height,track);if(bottom<=top)bottom=top+1;if(bottom>height)bottom=height;rect(x,y+top,3,bottom-top,thumb);
}
static void spacebook_masthead(const char *section){
 const unsigned blue=RGB(59,89,152);rect(0,42,W,26,blue);spacebook_logo(8,44);
 social_text(36,47,160,WHITE,"spacebook",2);
 rect(177,48,139,14,RGB(245,247,251));social_text(183,51,311,RGB(85,104,139),section,1);
 char name[25];social_name(name,sizeof(name),game.commander_name);portrait_draw(326,46,18,18,game.commander_portrait,EXPLORERS);social_wrap_count(350,46,126,WHITE,name,2);
}
static void social_screen(void){
 const unsigned blue=RGB(59,89,152),ink=RGB(34,43,58),muted=RGB(102,117,138),paper=RGB(233,235,238),rail=RGB(213,219,229);
 rect(0,42,W,206,paper);spacebook_masthead("News Feed");
 int total=social_rows();if(row<0)row=0;if(row>=total)row=total-1;
 char name[25],label[96];social_name(name,sizeof(name),game.commander_name);
 portrait_draw(10,79,26,26,game.commander_portrait,EXPLORERS);social_text(10,111,87,ink,"Your profile",1);
 rect(7,130,80,21,RGB(215,224,238));rect(10,135,10,2,blue);rect(10,140,10,2,blue);rect(10,145,10,2,blue);social_text(26,137,86,blue,"News Feed",1);
 rect(10,164,11,8,muted);line(10,164,15,168,WHITE);line(15,168,20,164,WHITE);social_text(26,164,87,blue,"Inbox",1);button_icon(10,178,'T',blue);social_text(24,180,87,muted,"Open inbox",1);
 social_text(10,209,87,muted,"Your flights.",1);social_text(10,222,87,muted,"Their gossip.",1);
 rect(96,75,280,20,WHITE);social_text(105,82,367,muted,"Local pilots are talking...",1);
 char author[40],body[192],date[96];galnet_post(row,author,sizeof(author),body,sizeof(body));social_date(row,date,sizeof(date));unsigned code=social_code(row);
 rect(96,100,280,141,rail);rect(97,101,278,139,WHITE);rect(96,100,3,141,blue);
 spacebook_avatar(104,107,22,author);social_text(133,108,367,blue,author,1);social_text(133,121,367,muted,code?date:"Waiting for local sightings",1);
 /* 262px x 50px accommodates every existing 170-character event body. */
 int at=0;for(int ln=0;ln<5&&body[at];ln++){
  int begin=at,last=-1,used=0;while(body[at]&&used+8<262){if(body[at]==' ')last=at;used+=social_char_width(body[at]);at++;}if(body[at]&&last>begin)at=last;
  char part[192];int len=at-begin;memcpy(part,body+begin,len);part[len]=0;social_text(105,136+ln*10,368,ink,part,1);while(body[at]==' ')at++;
 }
 if(code)spacebook_snapshot(105,188,262,27,code);
 else social_text(105,196,368,muted,"Go flying. Someone will notice.",1);
 line(104,219,367,219,rail);int reaction=social_reaction(row);
 social_text(105,226,197,blue,reaction==1?"Liked":"Like",1);social_text(204,226,292,blue,reaction==2?"Disliked":"Dislike",1);snprintf(label,sizeof(label),"%d/%d",row+1,total);social_text(309,226,369,muted,label,1);
 social_text(386,80,475,muted,"On this wire",1);line(385,94,470,94,rail);
 for(int j=0;j<3&&j<total;j++){int r=(row+j)%total;char who[40],copy[192];galnet_post(r,who,sizeof(who),copy,sizeof(copy));if(!social_code(r))break;int y=104+j*40;spacebook_avatar(386,y,18,who);social_text(386,y+22,474,blue,who,1);}
 social_text(386,229,474,muted,"Not a vacuum.",1);galnet_scrollbar(474,100,141,row,1,total,rail,blue);
 rect(0,248,W,24,RGB(215,225,239));button_icon(5,251,'U',blue);button_icon(17,251,'D',blue);social_text(31,253,74,blue,"Posts",1);
 button_icon(78,251,'X',blue);social_text(92,253,127,blue,"Like",1);button_icon(135,251,'S',blue);social_text(149,253,205,blue,"Dislike",1);
 button_icon(217,251,'T',blue);social_text(231,253,277,blue,"Inbox",1);button_icon(292,251,'l',blue);button_icon(305,251,'r',blue);social_text(319,253,359,blue,"Tabs",1);button_icon(420,251,'O',blue);social_text(434,253,478,blue,"Back",1);
}
/* Pixel-measured wrapping handles paragraphs and paging without truncating the
 * archived letter, even when a chapter or speaker has a long name. */
static int inbox_letter(const char *body,int page){
 int at=0,line_no=0;while(body[at]){
  if(body[at]=='\n'){at++;line_no++;continue;}
  int begin=at,last=-1,used=0;while(body[at]&&body[at]!='\n'&&used+8<292){if(body[at]==' ')last=at;used+=social_char_width(body[at]);at++;}if(body[at]&&body[at]!='\n'&&last>begin)at=last;
  char part[256];int len=at-begin;if(len>255)len=255;memcpy(part,body+begin,len);part[len]=0;
  if(line_no>=page*9&&line_no<(page+1)*9)social_text(170,124+(line_no%9)*10,462,RGB(34,43,58),part,1);
  line_no++;while(body[at]==' ')at++;if(body[at]=='\n')at++;
 }return (line_no+8)/9;
}
static void inbox_avatar(int x,int y,int size,const char *name){
 if(!strcmp(name,"TORR BELL")){portrait_draw(x,y,size,size,erl_portrait(0),TRADERS);return;}
 int erl_sys=game.erl.ref_system>=0?game.erl.ref_system:7;
 if(!strcmp(name,erl_organiser(&game,erl_sys))){portrait_draw(x,y,size,size,portrait_seeded(erl_sys*173u+61u,TRADERS,0),TRADERS);return;}
 if(!strcmp(name,"KEI")||!strcmp(name,"Kei Aven")){draw_kei(x,y,size,0);return;}
 if(!strcmp(name,"RYN")){draw_kei(x,y,size,1);return;}
 if(!strcmp(name,"Dad")||!strcmp(name,"Mum")||!strcmp(name,"Kat Herrington")||!strcmp(name,"Scott Winters")||!strcmp(name,"Ship Computer")){opening_portrait(name,x,y,size);return;}
 if(!strcmp(name,"Flight Journal")){portrait_draw(x,y,size,size,game.commander_portrait,EXPLORERS);return;}
 spacebook_avatar(x,y,size,name);
}
static void spacebook_screen(int messages){
 if(!messages){social_screen();return;}
 const unsigned blue=RGB(59,89,152),ink=RGB(34,43,58),muted=RGB(102,117,138),rail=RGB(213,219,229);
 rect(0,42,W,206,RGB(233,235,238));spacebook_masthead("Inbox / story archive");
 int total=story_inbox_count();if(row<0||row>=total)row=0;if(inbox_last_row!=row){inbox_message_page=0;inbox_last_row=row;}
 if(opening_net_detail){opening_bitmap_draw(&opening_party_bitmap,96,72,288,162);footer("O CLOSE PHOTO");return;}
 rect(8,75,146,166,RGB(224,229,236));int first=row/5*5;
 for(int j=0;j<5&&first+j<total;j++){int i=first+j,y=79+j*32;char author[40],subject[128],body[768];story_inbox_text(i,author,sizeof(author),subject,sizeof(subject),body,sizeof(body));
  if(i==row){rect(9,y-2,144,30,RGB(199,215,237));rect(9,y-2,3,30,blue);}inbox_avatar(16,y+1,20,author);social_text(41,y+2,151,blue,author,1);social_text(41,y+15,151,muted,subject,1);
 }galnet_scrollbar(4,75,166,first,5,total,rail,blue);
 rect(160,75,312,166,WHITE);char author[40],subject[128],body[768];story_inbox_text(row,author,sizeof(author),subject,sizeof(subject),body,sizeof(body));
 inbox_avatar(167,80,22,author);social_text(196,80,463,blue,author,1);social_wrap_count(196,94,267,muted,subject,2);line(167,117,464,117,rail);
 inbox_pages=inbox_letter(body,-1);if(inbox_pages<1)inbox_pages=1;if(inbox_message_page>=inbox_pages||inbox_message_page<0)inbox_message_page=0;inbox_letter(body,inbox_message_page);
 int index,kind=story_inbox_record(row,&index);char status[96];
 if(kind==INBOX_HOME){snprintf(status,sizeof(status),"%s",opening_sol(&game)?"Earth / private correspondence":"Earth cache / relay offline");if(index<4&&!opening_sol(&game)&&(game.opening.flags&(1<<(16+index))))snprintf(status,sizeof(status),"Undelivered: Earth relay offline");}
 else snprintf(status,sizeof(status),"%s",kind==INBOX_SAGA&&index<game.saga_chapter?"Filed chapter / kept for reference":"Story correspondence / saved with progress");
 social_text(168,221,463,muted,status,1);snprintf(status,sizeof(status),"%d/%d",inbox_message_page+1,inbox_pages);social_text(432,233,468,muted,status,1);
 social_text(168,233,425,blue,kind==INBOX_HOME?(index==3?"Square: Friday's photo":index<4&&!opening_sol(&game)?"X: Send 'Are you there?'":"Saved personal note"):"X: Open Mission Log",1);
 rect(0,248,W,24,RGB(215,225,239));button_icon(5,251,'U',blue);button_icon(17,251,'D',blue);social_text(31,253,90,blue,"Messages",1);
 if(kind!=INBOX_HOME||(!opening_sol(&game)&&index<4)){button_icon(101,251,'X',blue);social_text(115,253,172,blue,kind==INBOX_HOME?"Send":"Log",1);}button_icon(181,251,'S',blue);social_text(195,253,248,blue,index==3&&kind==INBOX_HOME?"Photo":"Page",1);
 button_icon(268,251,'l',blue);button_icon(281,251,'r',blue);social_text(295,253,336,blue,"Tabs",1);button_icon(351,251,'T',blue);social_text(365,253,404,blue,"Feed",1);button_icon(420,251,'O',blue);social_text(434,253,478,blue,"Back",1);
}
static void spacebook_actions(unsigned pressed){
 if(galnet_tab==3){if(pressed&PSP_CTRL_CROSS)social_react(row,1);if(pressed&PSP_CTRL_SQUARE)social_react(row,2);if(pressed&PSP_CTRL_TRIANGLE){galnet_tab=4;row=0;inbox_message_page=0;inbox_last_row=-1;}return;}
 if(galnet_tab!=4)return;
 if(opening_net_detail){if(pressed&PSP_CTRL_CIRCLE)opening_net_detail=0;return;}
 if(pressed&PSP_CTRL_TRIANGLE){galnet_tab=3;row=0;return;}
 int index,kind=story_inbox_record(row,&index);
 if(pressed&PSP_CTRL_SQUARE){if(kind==INBOX_HOME&&index==3)opening_net_detail=1;else{char a[40],s[128],b[768];story_inbox_text(row,a,sizeof(a),s,sizeof(s),b,sizeof(b));inbox_pages=inbox_letter(b,-1);if(inbox_pages<1)inbox_pages=1;inbox_message_page=(inbox_message_page+1)%inbox_pages;}}
 if(pressed&PSP_CTRL_CROSS){if(kind==INBOX_HOME){if(index<4&&!opening_sol(&game))game.opening.flags|=1<<(16+index);}else{tracked_mission=kind==INBOX_ERL?TRACK_ERL:0;change_page(MISSIONLOG);row=mission_track_row();}}
}
