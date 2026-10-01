/* A deliberately separate, light social-network skin. Offline fiction only. */
static int spacebook_comments=0;
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
static void social_wrap(int x,int y,int width,unsigned colour,const char *s){
 /* Break at words and keep a generous three-line limit inside each card. */
 int at=0;
 for(int line=0;line<3&&s[at];line++){
  int start=at,last=-1,used=0;
  while(s[at]&&at-start<95&&used+8<=width){if(s[at]==' ')last=at;used+=social_char_width((unsigned char)s[at]);at++;}
  if(s[at]&&last>start)at=last;
  char part[96];int n=at-start;if(n>95)n=95;memcpy(part,s+start,n);part[n]=0;
  social_text(x,y+line*10,x+width,colour,part,1);while(s[at]==' ')at++;
 }
}
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
static void social_screen(void){
 const unsigned blue=RGB(49,82,145),ink=RGB(34,43,58),muted=RGB(91,107,127),paper=RGB(238,242,247);
 rect(0,0,W,H,paper);spacebook_logo(8,42);social_text(34,49,155,blue,"Spacebook",1);
 char name[25],label[80];social_name(name,sizeof(name),game.commander_name);
 snprintf(label,sizeof(label),"%.24s / %d posts",name,social_count());social_text(162,49,474,ink,label,1);
 int total=social_rows();if(row<0)row=0;if(row>=total)row=total-1;int first=(row/2)*2;
 galnet_scrollbar(4,68,170,first,2,total,RGB(210,219,230),blue);
 for(int j=0;j<2&&first+j<total;j++){
  int i=first+j,y=68+j*86;char author[40],body[192],date[96];galnet_post(i,author,sizeof(author),body,sizeof(body));
  rect(12,y,460,82,i==row?blue:RGB(210,219,230));rect(15,y+3,454,76,RGB(255,255,255));
  unsigned code=social_code(i);spacebook_avatar(20,y+6,20,author);
  social_text(48,y+6,462,blue,author,1);social_date(i,date,sizeof(date));social_text(48,y+18,462,muted,date,1);
  /* Each post’s evidence strip is a reconstructed capture of that event,
     leaving enough width for the short conversational post beside it. */
  spacebook_snapshot(322,y+31,137,31,code);
  social_wrap(22,y+32,287,ink,body);
  int reaction=social_reaction(i);
  if(code||(social_offset()&&i==0)){snprintf(label,sizeof(label),"%s     %s",reaction==1?"[Liked]":"Like",reaction==2?"[Disliked]":"Dislike");social_text(22,y+65,350,blue,label,1);}
  snprintf(label,sizeof(label),"%d/%d",i+1,total);social_text(386,y+65,466,muted,label,1);
 }
 rect(0,244,W,28,RGB(215,225,239));button_icon(8,251,'X',blue);social_text(22,253,72,blue,"Like",1);
 button_icon(70,251,'S',blue);social_text(84,253,141,blue,"Dislike",1);
 button_icon(145,251,'L',blue);button_icon(158,251,'R',blue);social_text(172,253,254,blue,"10 posts",1);
 social_text(267,253,405,blue,"L/R tabs",1);button_icon(420,251,'O',blue);social_text(434,253,478,blue,"Back",1);
}
static void spacebook_screen(int messages){
 if(!messages){social_screen();return;}

 const unsigned blue=RGB(49,82,145),ink=RGB(34,43,58),muted=RGB(91,107,127),paper=RGB(238,242,247);
 rect(0,0,W,H,paper);
 spacebook_logo(8,44);
 social_text(34,50,158,blue,"Spacebook",1);
 char context[64];snprintf(context,sizeof(context),messages?"Messages / System: %s":"Local feed / System: %s",game.systems[game.system].name);
 social_text(160,50,470,muted,context,1);
 rect(8,68,94,170,RGB(221,229,240));draw_commander_portrait(16,74,32,game.commander_portrait);
 social_text(15,108,101,ink,game.commander_name,1);
 social_text(15,122,101,blue,messages?"Inbox":"Your feed",1);
 social_text(15,137,101,muted,messages?"Unread: 3":"Local history",1);
 social_text(15,152,101,muted,messages?"Archived: 12":"16 latest posts",1);
 social_text(15,172,101,muted,"Sponsored",1);
 social_text(15,186,101,blue,"SPACE VPN",1);
 social_wrap(15,200,83,ink,"Hide from ads. Not the police.");
 int first=(row/2)*2,total=galnet_rows();
 galnet_scrollbar(105,68,168,first,2,total,RGB(210,219,230),blue);
 for(int j=0;j<2&&first+j<total;j++){
  int i=first+j,y=68+j*86;char author[40],body[192],meta[96];galnet_post(i,author,sizeof(author),body,sizeof(body));
  rect(111,y,361,82,i==row?RGB(166,190,223):RGB(210,219,230));rect(113,y+2,357,78,RGB(255,255,255));
  unsigned code=messages?0:social_code(i);spacebook_avatar(121,y+6,22,author);
  social_text(151,y+6,462,blue,author,1);
  if(messages)snprintf(meta,sizeof(meta),"Local inbox / %s",game.systems[game.system].name);else social_date(i,meta,sizeof(meta));social_text(151,y+18,462,muted,meta,1);
  social_wrap(121,y+32,340,ink,body);
  int reaction=messages?0:social_reaction(i);
  if(messages)snprintf(meta,sizeof(meta),"Reply       Archive       Mark unread");
  else snprintf(meta,sizeof(meta),"%s     %s",reaction==1?"[Liked]":"Like",reaction==2?"[Disliked]":"Dislike");
  social_text(121,y+64,462,blue,meta,1);
  if(messages&&i==row&&spacebook_comments){rect(118,y+48,347,14,paper);social_text(122,y+51,461,muted,code?"OrbitMoth: Local feed never sleeps. Unlike me.":"Pip: I have concerns about your privacy settings.",1);}
 }
 rect(0,244,W,28,RGB(215,225,239));
 if(messages)social_text(10,253,474,blue,"Up/down  X open  Triangle reply  L/R tab  O back",1);
 else {button_icon(10,250,'X',blue);social_text(24,252,98,blue,"Like",1);button_icon(92,250,'S',blue);social_text(106,252,180,blue,"Dislike",1);social_text(270,252,417,blue,"L/R tab  Up/down",1);button_icon(420,250,'O',blue);social_text(434,252,477,blue,"Back",1);}
}
