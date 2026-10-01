/* Approved Design 3 at 480x272. Original studio art with live text and bounded
 * animation layers. No heap, PNG decoder, simulation updates or save writes. */
#include "local-tv-art.h"
enum { LOCAL_TV_PROGRAMS=3, LOCAL_TV_BEATS=4, LOCAL_TV_SLOT_SECONDS=300 };
static int local_tv_program=0,local_tv_reveal=0,local_tv_beat=0;
static float local_tv_clock=0;
static double local_tv_now=0,local_tv_test_time=-1;
static int local_tv_clock_valid=0;
static void local_tv_tick(float dt);
#define TV_GOLD RGB(255,211,116)
#define TV_CYAN RGB(75,222,232)
#define TV_TEXT RGB(230,237,248)
#define TV_PANEL RGB(12,23,35)
static const char *local_tv_programs[]={"Evening Orbit","Farmers' Market","Night Stories"};
static const char *local_tv_scripts[3][4]={
 {"Good evening, pilots. Welcome to Lave Local. I'm Mira Vale, keeping you company above the orchards.",
  "From this window, every arrival looks peaceful. Somewhere aboard that freighter, somebody is still trying to find their docking papers.",
  "If you've just arrived, take a little time to look around. There is a whole world beneath those clouds, and a station full of people above them.",
  "Up next: Farmers' Market. Later, Night Stories. Wherever you're heading tonight, leave a light on for the journey home."},
 {"Welcome to Farmers' Market. Tonight: the growers, haulers and small businesses that keep Lave fed. I'm Mira Vale.",
  "One orchard sent us a crate marked HANDLE WITH LOVE. The dock crew asked whether love required a separate customs form.",
  "A reminder from our sponsors: sample the fruit before you buy the orchard. Apparently that advice now applies to second-hand cargo ships too.",
  "That was Farmers' Market. Actual cargo prices and available stock are on the station's market board. We'll leave the bargaining to you."},
 {"Stay a while for Night Stories. Tonight's tale begins with a pilot who kept receiving a docking clearance from a station that wasn't there.",
  "The voice was always polite. Berth six, commander. Mind the lights. He followed it for three nights, finding only empty space and an old beacon.",
  "On the fourth night he answered: I'm home. The beacon went quiet. In his hold, a sealed crate began playing a song his mother used to sing.",
  "He never opened it. Some things, he said, are worth more unopened. A little fiction for the late watch. This is Lave Local. Stay flying."}
};
static const char *local_tv_copy(void){return local_tv_scripts[local_tv_program][local_tv_beat];}
/* A continuous local-clock broadcast: opening or pressing buttons never restarts it. */
static void local_tv_schedule(double seconds){
 seconds=fmod(seconds,86400.0);if(seconds<0)seconds+=86400.0;
 local_tv_now=seconds;
 local_tv_program=((int)seconds/LOCAL_TV_SLOT_SECONDS)%LOCAL_TV_PROGRAMS;
 double phase=fmod(seconds,LOCAL_TV_SLOT_SECONDS),cycle=0;
 for(int b=0;b<LOCAL_TV_BEATS;b++)cycle+=strlen(local_tv_scripts[local_tv_program][b])/28.0+6.0;
 phase=fmod(phase,cycle);
 for(int b=0;b<LOCAL_TV_BEATS;b++){
  int n=(int)strlen(local_tv_scripts[local_tv_program][b]);double duration=n/28.0+6.0;
  if(phase<duration||b==LOCAL_TV_BEATS-1){
   local_tv_beat=b;local_tv_reveal=(int)(phase*28.0);
   if(local_tv_reveal>n)local_tv_reveal=n;break;
  }phase-=duration;
 }
}
static void local_tv_time_label(int ahead,char *out,int size){
 if(!local_tv_clock_valid){snprintf(out,size,"--:--");return;}
 int seconds=ahead?((int)local_tv_now/LOCAL_TV_SLOT_SECONDS+ahead)*LOCAL_TV_SLOT_SECONDS:(int)local_tv_now;
 seconds%=86400;snprintf(out,size,"%02d:%02d",seconds/3600,seconds/60%60);
}
static void local_tv_open(void){local_tv_tick(0);}

/* Retain the original full-height glyphs; trim only blank side bearings. */
static int local_tv_glyph(int ch,int *first){
 if(ch<32||ch>126)ch='?';
 if(ch==' '){*first=0;return 3;}
 unsigned bits=0;for(int y=0;y<8;y++)bits|=font8[ch-32][y];
 int a=0,b=7;while(a<7&&!(bits&(128u>>a)))a++;while(b>a&&!(bits&(128u>>b)))b--;
 *first=a;return b-a+2;
}
static int local_tv_width(const char *s){int w=0,a;for(;*s;s++)w+=local_tv_glyph((unsigned char)*s,&a);return w;}
static void local_tv_text(int x,int y,unsigned ink,const char *s){
 for(;*s;s++){int ch=(unsigned char)*s,a;if(ch<32||ch>126)ch='?';int advance=local_tv_glyph(ch,&a);
  if(ch!=' ')for(int yy=0;yy<8;yy++)for(int xx=0;xx<advance-1;xx++)if(font8[ch-32][yy]&(128u>>(xx+a)))pixel(x+xx,y+yy,ink);
  x+=advance;
 }
}
static void local_tv_center(int x,int y,int width,unsigned ink,const char *s){local_tv_text(x+(width-local_tv_width(s))/2,y,ink,s);}
/* Wrap the full caption first, then reveal into that stable layout. */
static int local_tv_caption(int reveal,int draw){
 const char *s=local_tv_copy();int row=0,start=0,n=(int)strlen(s);
 while(start<n&&row<3){int end=start,width=0,a,last_space=-1;
  while(end<n){int w=local_tv_glyph((unsigned char)s[end],&a);if(width+w>450)break;width+=w;if(s[end]==' ')last_space=end;end++;}
  if(end<n&&last_space>start)end=last_space;
  if(draw){char buf[192];int count=reveal-start;if(count<0)count=0;if(count>end-start)count=end-start;
   memcpy(buf,s+start,count);buf[count]=0;local_tv_text(15,212+row*10,TV_TEXT,buf);}
  start=end;while(s[start]==' ')start++;row++;
 }
 return start==n;
}
static unsigned local_tv_color(int x,int y){unsigned p=local_tv_art[y*480+x];int r=(p>>10)&31,g=(p>>5)&31,b=p&31;return RGB((r<<3)|(r>>2),(g<<3)|(g>>2),(b<<3)|(b>>2));}
static int local_tv_speaking(void){
 if(local_tv_reveal>=(int)strlen(local_tv_copy()))return 0;
 int c=local_tv_copy()[local_tv_reveal];
 return (c>='A'&&c<='Z')||(c>='a'&&c<='z');
}
static void local_tv_audio_update(void){
 audio_tv_on=page==LOCALTV&&!paused;
 audio_tv_code=audio_tv_on&&!quiet_comms&&local_tv_speaking()?
  (unsigned char)local_tv_copy()[local_tv_reveal]|(local_tv_program==2?256:0):0;
}
/* Compress only the tilted smile's texels. Surrounding skin and the entire
 * face remain registered to the source art; no floating mouth rectangle. */
static void local_tv_mouth(void){
 if(!local_tv_speaking())return;
 int phase=(int)(local_tv_clock*(local_tv_program==2?5.f:6.6667f))%7;
 if(phase==0||phase==4)return;
 for(int x=128;x<=142;x++){
  float mid=92.f-(x-128)*.24f;int top=(int)floorf(mid-2.2f),bottom=(int)ceilf(mid+2.2f);
  for(int y=top;y<=bottom;y++){
   float offset=y-mid,scale=(phase==2||phase==5)?.45f:.7f;
   int sy=(int)floorf(mid+offset/scale+.5f);
   if(sy<top)sy=top;
   if(sy>bottom)sy=bottom;
   pixel(x,y,local_tv_color(x,sy));
  }
 }
}
static int local_tv_window_mask(int x,int y){
 if(x<199||x>311||y<40||y>143)return 0;
 if((x>=204&&x<=287&&y>=74&&y<=93)||(x>=249&&x<=261&&y>=43&&y<=137))return 0;
 return 1;
}
static void local_tv_window(void){
 int glow=(int)(3.f+3.f*sinf(local_tv_clock*1.1f));
 for(int y=41;y<98;y++)for(int x=166;x<198;x++){
  unsigned c=local_tv_color(x,y);int r=c&255,g=(c>>8)&255,b=(c>>16)&255;
  if(r>235&&g>140&&b<170){g+=glow;if(g>255)g=255;pixel(x,y,RGB(r,g,b));}
 }
 /* Explicit native-pixel hulls: sampled highlights from the old reference
  * were too faint to read as traffic. Keep all pixels behind station/masks. */
 static const char hull[3][13]={"000001110000","344222222110","000001110000"};
 static const unsigned colors[5]={0,RGB(88,109,130),RGB(225,212,177),RGB(54,162,214),RGB(116,220,240)};
 for(int i=0;i<3;i++){
  int offset=(int)fmodf(local_tv_clock*(2.4f+i*.8f)+i*39.f,136.f);
  int xx=i==1?315-offset:187+offset;
  int yy=i==0?60:i==1?111:128;
  for(int y=0;y<3;y++)for(int x=0;x<12;x++){
   int ink=hull[y][i==1?11-x:x]-'0';
   if(ink&&local_tv_window_mask(xx+x,yy+y))pixel(xx+x,yy+y,colors[ink]);
  }
 }
 for(int i=0;i<7;i++){
  int x=210+i*13,y=46+(i*23)%91;unsigned c=local_tv_color(x,y);
  if(local_tv_window_mask(x,y)&&(c&255)<75&&((c>>8)&255)<75){int k=95+(int)(55*sinf(local_tv_clock*.8f+i));pixel(x,y,RGB(k,k,k+25));}
 }
}
static void local_tv_card(int y,const char *label,const char *programme,int current,int ahead){
 rect(371,y,101,30,TV_PANEL);
 unsigned edge=current?TV_GOLD:RGB(47,74,94);
 line(371,y,471,y,edge);line(371,y+29,471,y+29,edge);line(371,y,371,y+29,edge);line(471,y,471,y+29,edge);
 local_tv_text(382,y+4,TV_CYAN,label);
 if(ahead){char when[12];local_tv_time_label(ahead,when,sizeof(when));local_tv_text(468-local_tv_width(when),y+4,TV_GOLD,when);}
 if(local_tv_width(programme)<=87)local_tv_text(382,y+17,current?TV_GOLD:TV_TEXT,programme);
 else {char a[32],b[32];const char *space=strchr(programme,' ');int n=space?(int)(space-programme):(int)strlen(programme);snprintf(a,sizeof(a),"%.*s",n,programme);snprintf(b,sizeof(b),"%s",space?space+1:"");local_tv_text(382,y+12,TV_TEXT,a);local_tv_text(382,y+21,TV_TEXT,b);}
 if(current){line(375,y+13,378,y+16,TV_GOLD);line(378,y+16,375,y+19,TV_GOLD);}
}
static void local_tv_ident(void){
 /* A single station retains its CH8 identity throughout the schedule. */
 rect(375,83,94,16,TV_PANEL);
 local_tv_center(375,83,94,TV_TEXT,"LAVE LOCAL");local_tv_center(375,92,94,TV_TEXT,"TELEVISION");
}
static void local_tv_screen(void){
 draw_next_art(local_tv_art,480,272,0,0,480,272);
 local_tv_window();local_tv_mouth();
 char title[64];snprintf(title,sizeof(title),"%.16s LOCAL / CHANNEL 8",game.systems[game.system].name);
 rect(127,3,350,16,RGB(11,21,33));local_tv_text(129,7,TV_GOLD,title);
 char clock_label[12];local_tv_time_label(0,clock_label,sizeof(clock_label));local_tv_text(472-local_tv_width(clock_label),7,TV_CYAN,clock_label);
 local_tv_ident();
 local_tv_card(106,"NOW:",local_tv_programs[local_tv_program],1,0);
 local_tv_card(139,"NEXT:",local_tv_programs[(local_tv_program+1)%3],0,1);
 local_tv_card(173,"LATER:",local_tv_programs[(local_tv_program+2)%3],0,2);
 rect(13,191,163,12,TV_PANEL);local_tv_text(15,194,TV_GOLD,"MIRA VALE");local_tv_text(83,194,TV_TEXT,"-");local_tv_text(95,194,TV_CYAN,"LAVE LOCAL");
 rect(9,207,461,35,TV_PANEL);local_tv_caption(local_tv_reveal,1);
 rect(0,249,480,23,RGB(11,21,33));line(0,248,479,248,TV_GOLD);
 button_icon(9,253,'O',TV_TEXT);local_tv_text(23,257,TV_TEXT,"BACK");
 local_tv_text(78,257,TV_CYAN,"LIVE / LOCAL TIME");
 local_tv_text(235,257,TV_GOLD,"LAVE LOCAL / A BRIGHTER TOMORROW");
}
static void local_tv_tick(float dt){
 if(!isfinite(dt)||dt<0)return;
 local_tv_clock+=dt;if(local_tv_clock>3600)local_tv_clock=fmodf(local_tv_clock,3600);
 ScePspDateTime rtc;
 if(local_tv_test_time>=0){local_tv_clock_valid=1;local_tv_schedule(local_tv_test_time);}
 else if(sceRtcGetCurrentClockLocalTime(&rtc)>=0&&rtc.hour<24&&rtc.minute<60&&rtc.second<60){
  local_tv_clock_valid=1;local_tv_schedule(rtc.hour*3600.0+rtc.minute*60.0+rtc.second+rtc.microsecond/1000000.0);
 }else{local_tv_clock_valid=0;local_tv_schedule(local_tv_now+dt);}
}

