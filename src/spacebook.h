/* A deliberately separate, light social-network skin. Offline fiction only. */
static unsigned char spacebook_likes[256];
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
static void social_wrap(int x,int y,int width,unsigned colour,const char *s){
 /* Break at words and keep a generous three-line limit inside each card. */
 int at=0;
 for(int line=0;line<3&&s[at];line++){
  int start=at,last=-1,used=0;
  while(s[at]&&used+8<width){if(s[at]==' ')last=at;used+=s[at]==' '?4:6;at++;}
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
static void spacebook_screen(int messages){
 const unsigned blue=RGB(49,82,145),ink=RGB(34,43,58),muted=RGB(91,107,127),paper=RGB(238,242,247);
 rect(0,0,W,H,paper);
 spacebook_logo(8,44);
 social_text(34,50,158,blue,"Spacebook",1);
 char context[64];snprintf(context,sizeof(context),messages?"Messages / System: %s":"Local feed / System: %s",game.systems[game.system].name);
 social_text(160,50,470,muted,context,1);
 rect(8,68,94,170,RGB(221,229,240));draw_portrait(16,74,28,28,game.system,EXPLORERS);
 social_text(15,108,101,ink,"Commander",1);
 social_text(15,122,101,blue,messages?"Inbox":"Your feed",1);
 social_text(15,137,101,muted,messages?"Unread: 3":"Friends: 7",1);
 social_text(15,152,101,muted,messages?"Archived: 12":"Pokes: 42",1);
 social_text(15,172,101,muted,"Sponsored",1);
 social_text(15,186,101,blue,"SPACE VPN",1);
 social_wrap(15,200,83,ink,"Hide from ads. Not the police.");
 int first=(row/2)*2,total=messages?5:7;
 for(int j=0;j<2&&first+j<total;j++){
  int i=first+j,y=68+j*86;char author[40],body[96],meta[96];galnet_post(i,author,sizeof(author),body,sizeof(body));
  rect(111,y,361,82,i==row?RGB(166,190,223):RGB(210,219,230));rect(113,y+2,357,78,RGB(255,255,255));
  galnet_avatar(121,y+6,22,i);
  social_text(151,y+6,462,blue,author,1);
  snprintf(meta,sizeof(meta),"%d min ago / public / near %s",i*7+2,game.systems[game.system].name);social_text(151,y+18,462,muted,meta,1);
  social_wrap(121,y+32,340,ink,body);
  int liked=(spacebook_likes[game.system]>>i)&1;
  if(messages)snprintf(meta,sizeof(meta),"Reply       Archive       Mark unread");
  else snprintf(meta,sizeof(meta),"%s  %d       Reply       Share oxygen",liked?"Liked":"Like",12+i*9+liked);
  social_text(121,y+64,462,blue,meta,1);
  if(i==row&&spacebook_comments){rect(118,y+48,347,14,paper);social_text(122,y+51,461,muted,"Pip: I have concerns about your privacy settings.",1);}
 }
 rect(0,244,W,28,RGB(215,225,239));
 social_text(10,253,474,blue,messages?"Up/down  X open  Triangle reply  L/R tab  O back":"Up/down  X like  Triangle comment  L/R tab  O back",1);
}
