/* Cosmetic local adverts. No commerce, world RNG, save state or heap buffers. */
#include "rumour-ads-art.h"
typedef struct {const char *title,*copy;unsigned accent,paint;} RumourAdvert;
static const RumourAdvert rumour_ads[]={
 {"A MOON OF YOUR OWN","Rent a moon for less than a cup of tea. Oceans, tides and previous owners sold separately.",RGB(182,67,116),RGB(153,172,189)},
 {"LOST SOCK RESCUE","We find the other sock. We do not ask where it has been. Wormhole postage included.",RGB(45,127,147),RGB(201,90,126)},
 {"YOUR SHIP'S WARRANTY","Your ship's warranty expired yesterday. Even if you bought it tomorrow. That's how space works.",RGB(174,87,36),RGB(71,149,157)},
 {"JUST ADD WATER","All the water. None of the weight. Simply add water. We have thought of everything.",RGB(34,126,158),RGB(67,165,211)},
 {"VACUUM FOR YOUR VACUUM","Space is already a vacuum. Buy another one. Finally, a cleaner that understands your lifestyle.",RGB(146,62,136),RGB(216,115,81)},
 {"CAT-ASSISTED AUTOPILOT","Nine lives. Zero licences. Your cat can fly the ship while you admire the upholstery.",RGB(43,127,116),RGB(230,157,71)},
 {"INFLATABLE CO-PILOT","Never fly alone again. Inflates in seconds. Please keep away from sharp conversations.",RGB(173,76,109),RGB(102,164,182)},
 {"PARK YOUR BLACK HOLE","A discreet parking bay for your black hole. No questions. No refunds. No light.",RGB(120,83,160),RGB(112,83,175)},
 {"ALIEN DENTAL PLAN","Six mouths? One easy payment! We clean the teeth your species admits to having.",RGB(32,132,115),RGB(83,165,116)},
 {"STICK-ON SPEED STRIPES","Go faster emotionally. Actual speed remains exactly the same. Peels off in hyperspace.",RGB(186,72,49),RGB(89,143,179)},
 {"COMET AIR FRESHENER","Make your cockpit smell like a tree nobody has ever seen. Free tiny string.",RGB(72,125,58),RGB(71,154,98)},
 {"PERSONAL POCKET SUN","Cold cockpit? Put a star in your pocket. Do not put your hand in the same pocket.",RGB(181,92,34),RGB(238,173,66)},
 {"SPACE WHALE DATING","Meet gentle giants who share your interests: drifting slowly and being enormous.",RGB(174,71,122),RGB(71,153,178)},
 {"ANTI-GRAVITY TOAST","Finally, toast that falls upwards. Butter included. Ceiling cleaning not included.",RGB(160,96,40),RGB(112,160,169)},
 {"ASTEROID PET ROCKS","Adopt a rock. It will never run away, answer back or remember your birthday.",RGB(58,121,131),RGB(156,140,121)},
 {"HOT SINGLES IN ORBIT","Three local stars want to meet you. They are genuinely hot. Bring heat shields.",RGB(180,66,96),RGB(236,167,72)},
 {"FOLD-UP ESCAPE POD","A full escape pod, folded flat. Some assembly required during the emergency.",RGB(45,126,152),RGB(146,178,193)},
 {"SPACE-SAFE TEA","For a proper cuppa in zero gravity. Now with a lid. We should have started with that.",RGB(147,79,132),RGB(105,159,163)},
 {"GHOST REMOVAL","Haunted freighter? We remove ghosts, spooky noises and embarrassing old flight logs.",RGB(152,74,159),RGB(123,179,165)},
 {"GROW A SPARE CAPTAIN","Grow your own captain. Water twice a week. Keep out of direct command.",RGB(66,124,74),RGB(85,155,105)},
 {"PREMIUM NOTHING","Own absolutely nothing, but in a lovely box. Now with twice as much nothing.",RGB(177,79,113),RGB(141,112,176)},
 {"LUXURY TINFOIL HATS","Keep your thoughts private. Also keeps leftover sandwiches surprisingly fresh.",RGB(62,117,157),RGB(162,184,198)},
 {"REVERSE TRACTOR BEAM","Pull things away from you! Yes, we know. Our engineers have stopped taking calls.",RGB(167,72,53),RGB(81,152,181)},
 {"NEBULA IN A JAR","A genuine nebula in a jar. Please do not open indoors, outdoors or anywhere.",RGB(127,77,166),RGB(115,133,205)}
};
#define RUMOUR_AD_COUNT ((int)(sizeof(rumour_ads)/sizeof(rumour_ads[0])))
#define RUMOUR_AD_OPEN_SECONDS .14f
#define RUMOUR_AD_CLOSE_SECONDS .11f
static int rumour_ad_phase,rumour_ad_index,rumour_ad_last=-1;
static unsigned rumour_ad_sequence;
static float rumour_ad_age,rumour_ad_clock,rumour_ad_wait=8,rumour_ad_close_scale=1,rumour_ad_draw_scale;
static void rumour_ad_reset(void){rumour_ad_phase=0;rumour_ad_age=rumour_ad_clock=0;rumour_ad_wait=8;}
static float rumour_ad_scale(void){
 if(rumour_ad_phase==2)return 1;
 if(rumour_ad_phase==3){float p=fminf(1,rumour_ad_age/RUMOUR_AD_CLOSE_SECONDS);return rumour_ad_close_scale*(1-p)*(1-p);}
 float p=fminf(1,rumour_ad_age/RUMOUR_AD_OPEN_SECONDS);
 if(p<.8f){float q=1-p/.8f;return 1.035f*(1-q*q*q);}
 return 1.035f-(p-.8f)*.175f;
}
/* Scale the authored window around its centre, not a whole-screen snapshot.
 * Text and artwork share integer boundaries; steady-state pixels are native. */
static void rumour_ad_rect(int x,int y,int w,int h,unsigned ink){
 float s=rumour_ad_draw_scale;
 if(s==1){if(w==1&&h==1)pixel(48+x,74+y,ink);else rect(48+x,74+y,w,h,ink);return;}
 int x0=(int)roundf(240+(x-192)*s),y0=(int)roundf(152+(y-78)*s);
 int x1=(int)roundf(240+(x+w-192)*s),y1=(int)roundf(152+(y+h-78)*s);
 if(x1>x0&&y1>y0)rect(x0,y0,x1-x0,y1-y0,ink);
}
static void rumour_ad_text(int x,int y,unsigned ink,const char *s){
 for(;*s;s++,x+=8){unsigned ch=(unsigned char)*s;if(ch<32||ch>126)ch='?';
  for(int yy=0;yy<8;yy++)for(int xx=0;xx<8;xx++)if(font8[ch-32][yy]&(128>>xx))rumour_ad_rect(x+xx,y+yy,1,1,ink);
 }
}
static int rumour_ad_copy(const char *s,int draw){
 int n=0;while(*s){int len=(int)strlen(s),cut=len<34?len:34;
  if(len>34){for(int i=34;i>0;i--)if(s[i]==' '){cut=i;break;}}
  if(draw&&n<5){char linebuf[35];memcpy(linebuf,s,cut);linebuf[cut]=0;rumour_ad_text(94,55+n*12,RGB(31,35,49),linebuf);}
  s+=cut;while(*s==' ')s++;n++;
 }return n;
}
static int rumour_ad_input(unsigned pressed,float dt){
 dt=fmaxf(0,fminf(.1f,dt));
 if(rumour_ad_phase){
  spacepay_wait_release=1; /* Includes the final closing frame, even if X is held. */
  if((pressed&PSP_CTRL_TRIANGLE)&&rumour_ad_phase!=3){rumour_ad_close_scale=rumour_ad_scale();rumour_ad_phase=3;rumour_ad_age=0;game.cue=SFX_AD_CLOSE;spacepay_wait_release=1;return 1;}
  rumour_ad_age+=dt;
  if(rumour_ad_phase==1&&rumour_ad_age>=RUMOUR_AD_OPEN_SECONDS){rumour_ad_phase=2;rumour_ad_age=0;}
  if(rumour_ad_phase==3&&rumour_ad_age>=RUMOUR_AD_CLOSE_SECONDS){rumour_ad_phase=0;rumour_ad_clock=0;rumour_ad_age=0;rumour_ad_wait=22+(rumour_ad_sequence*11u)%14;}
  return 1; /* Never click through, including the last shrinking frame. */
 }
 if(pressed&PSP_CTRL_CROSS){rumour_ad_clock=0;return 0;} /* Payment wins over a scheduled advert. */
 rumour_ad_clock+=dt;
 if(rumour_ad_clock>=rumour_ad_wait){
  rumour_ad_index=(game.system*11+(rumour_ad_sequence%24)*7)%RUMOUR_AD_COUNT;
  if(rumour_ad_index==rumour_ad_last)rumour_ad_index=(rumour_ad_index+1)%RUMOUR_AD_COUNT;
  rumour_ad_last=rumour_ad_index;rumour_ad_sequence++;rumour_ad_phase=1;rumour_ad_age=0;
  spacepay_wait_release=1;game.cue=SFX_AD_OPEN;return 1;
 }
 return 0;
}
static void rumour_ad_draw(void){
 if(!rumour_ad_phase)return;
 for(int y=42;y<248;y++)for(int x=0;x<W;x++)fb[y*STRIDE+x]=((fb[y*STRIDE+x]&0xfefefeu)>>1)|0xff000000u;
 rumour_ad_draw_scale=rumour_ad_scale();const RumourAdvert *ad=&rumour_ads[rumour_ad_index];
 unsigned ink=RGB(31,35,49),paper=RGB(247,235,208);
 rumour_ad_rect(3,4,384,156,RGB(12,16,28));rumour_ad_rect(0,0,384,156,ink);
 rumour_ad_rect(2,2,380,152,paper);rumour_ad_rect(3,3,378,22,ad->accent);
 rumour_ad_rect(3,3,378,1,RGB(255,218,170));rumour_ad_text(12,10,paper,"TOTALLY NORMAL ADVERT");
 for(int i=0;i<3;i++)rumour_ad_rect(350+i*9,11,4,4,paper);
 rumour_ad_rect(12,38,72,72,ink);rumour_ad_rect(14,40,68,68,RGB(228,216,193));
 for(int yy=0;yy<16;yy++)for(int xx=0;xx<16;xx++){
  char p=rumour_ad_art[rumour_ad_index][yy][xx];if(p=='.')continue;
  unsigned c=p=='#'?ink:p=='B'?ad->paint:p=='A'?ad->accent:p=='L'?RGB(255,216,111):RGB(255,248,229);
  rumour_ad_rect(16+xx*4,42+yy*4,4,4,c);
 }
 rumour_ad_text(94,37,ad->accent,ad->title);rumour_ad_copy(ad->copy,1);
 rumour_ad_rect(12,127,360,1,ad->accent);rumour_ad_text(16,138,ad->accent,"SPONSORED NONSENSE");
 rumour_ad_rect(286,131,86,20,ink);
 for(int y=0;y<7;y++){int half=y*4/6;rumour_ad_rect(302-half,135+y,1,1,paper);rumour_ad_rect(302+half,135+y,1,1,paper);}
 rumour_ad_rect(298,141,9,1,paper);rumour_ad_text(318,138,paper,"CLOSE");
 footer("TRI CLOSE AD   L/R SECTIONS   O BACK");
}
