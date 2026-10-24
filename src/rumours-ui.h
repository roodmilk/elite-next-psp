/* One logical scroll document; only visible modules draw. No large framebuffer. */
#include "generated/rumours-pixels.h"
typedef struct {int slot,x,y,w,h,style;} RumourLink;
static RumourLink rumour_links[16];static int rumour_links_n,rumour_page_system=-1,rumour_focus,rumour_doc_height;
static float rumour_scroll,rumour_scroll_goal;
static int spacepay_stage,spacepay_choice,spacepay_slot,spacepay_latched,spacepay_wait_release;
static float spacepay_started;
static const char *spacepay_error;
static void opening_advance(void);
#define RCREAM RGB(235,223,196)
#define RTEAL RGB(115,201,192)
#define RPURPLE RGB(74,52,90)
#define RGOLD RGB(239,179,98)
#include "rumour-ads.h"
static void rumour_box(int x,int y,int w,int h,unsigned fill,unsigned edge){
 /* Main rect() is a full-frame fast fill; website primitives explicitly clip. */
 for(int yy=y;yy<y+h;yy++)for(int xx=x;xx<x+w;xx++)pixel(xx,yy,(xx==x||yy==y||xx==x+w-1||yy==y+h-1)?edge:fill);
}
static void rumour_text(int x,int y,unsigned c,const char *s){
 for(;*s;s++,x+=8){unsigned ch=(unsigned char)*s;if(ch<32||ch>126)ch='?';
  for(int r=0;r<8;r++)for(int k=0;k<8;k++)if(font8[ch-32][r]&(128>>k))pixel(x+k,y+r,c);
 }
}
static int rumour_wrap(int x,int y,int width,unsigned c,const char *s,int maxlines){
 int cols=width/8,n=0;char buf[60];if(cols>59)cols=59;
 while(*s&&n<maxlines){int len=(int)strlen(s),cut=len<cols?len:cols;
  if(len>cols){for(int i=cols;i>0;i--)if(s[i]==' '){cut=i;break;}}
  memcpy(buf,s,cut);buf[cut]=0;rumour_text(x,y+n*12,c,buf);s+=cut;while(*s==' ')s++;n++;
 }return n;
}
static void rumour_art(int x,int y,int kind,int animate){
 const unsigned char *pixels=kind==1?rumour_wreck_pixels:kind==2?rumour_whales_pixels:rumour_beacon_pixels;
 const unsigned *pal=kind==1?rumour_wreck_pal:kind==2?rumour_whales_pal:rumour_beacon_pal;
 for(int yy=0;yy<100;yy++)if(y+yy>=60&&y+yy<248)for(int xx=0;xx<164;xx++)pixel(x+xx,y+yy,pal[pixels[yy*164+xx]]);
 if(!animate)return;
 int tick=(int)(preview_time*6);
 if(kind==1){for(int k=0;k<3;k++)rumour_box(x+28+k*39,y+53+k*7,2,2,((tick/5+k)&1)?RTEAL:RGOLD,RTEAL);}
 else if(kind==2){for(int k=0;k<5;k++){int dx=(tick/2+k*31)%150;pixel(x+dx+5,y+10+k*17,RTEAL);}
  /* Tiny moving wake flecks, drawn for this illustration rather than scaling it. */
  int a=tick%4;line(x+40-a,y+58,x+47-a,y+58,RTEAL);line(x+108-a,y+76,x+113-a,y+76,RTEAL);
  int fin=(tick/2)%4;fin=fin==3?1:fin;line(x+116,y+52,x+128,y+45+fin,RTEAL);
 }else{int p=tick%20;rumour_box(x+79,y+45,3,3,p<4?RGOLD:RTEAL,RTEAL);}
}
static void rumour_layout(void){
 if(rumour_page_system==game.system*2+!!opening_active(&game))return;
 rumour_page_system=game.system*2+!!opening_active(&game);rumour_links_n=0;rumour_focus=0;rumour_scroll=rumour_scroll_goal=0;
 rumour_ad_reset();
 int slots[RUMOUR_SLOTS+3],n=0;
 if(navigation_enabled(&game))for(int k=RUMOUR_SLOTS;k<RUMOUR_SLOTS+3;k++)slots[n++]=k;
 /* Classifications first, then quiet destinations. No shuffled offers on focus. */
 for(int pass=0;pass<2;pass++)for(int k=0;k<RUMOUR_SLOTS;k++)if((!navigation_enabled(&game)||rumour_owned(k)||rumour_found(k))&&rumour_valid(k)&&((rumour_kind(k)==1||k==12)==(pass==0)))slots[n++]=k;
 int y=158;
 for(int i=0;i<n;i++){
  RumourLink *l=&rumour_links[rumour_links_n++];l->slot=slots[i];
  if(i<2){l->x=i?316:10;l->y=46;l->w=i?142:296;l->h=104;l->style=i?1:0;}
  else {l->x=10;l->y=y;l->w=448;l->h=188;l->style=2+(i-2)%3;y+=200;}
 }
 /* Two honest entrances to the same pod share its one ownership record. */
 if(!navigation_enabled(&game)&&rumour_valid(12)&&rumour_links_n<16){rumour_links[rumour_links_n++]=(RumourLink){12,10,y,448,188,3};y+=200;}
 rumour_doc_height=188;
 for(int i=0;i<rumour_links_n;i++){
  int bottom=rumour_links[i].y+rumour_links[i].h+58;
  if(bottom>rumour_doc_height)rumour_doc_height=bottom;
 }
 spacepay_stage=0;spacepay_error=0;
}
static const char *rumour_status(int k){
 if(k<RUMOUR_SLOTS&&(game.rumours.refunded[game.system]&(1u<<k)))return "REFUNDED / UNAVAILABLE";
 if(rumour_found(k))return "DISCOVERED / DIRECTIONS";
 if(game.rumours.active==(unsigned)(1+game.system*RUMOUR_SLOTS+k))return rumour_arrived_key==(int)game.rumours.active?"AREA REACHED / WATCH SCANNER":"TRACKING / VIEW DIRECTIONS";
 if(rumour_owned(k))return "PURCHASED / DIRECTIONS";
 return 0;
}
static void rumour_focus_scroll(void){
 if(!rumour_links_n)return;RumourLink *l=&rumour_links[rumour_focus];
 int top=l->y,bottom=top+l->h;
 if(l->h>=188)rumour_scroll_goal=top;
 else if(top<rumour_scroll_goal)rumour_scroll_goal=top;
 else if(bottom>rumour_scroll_goal+188)rumour_scroll_goal=bottom-188;
 float limit=fmaxf(0,rumour_doc_height-188);rumour_scroll_goal=fmaxf(0,fminf(limit,rumour_scroll_goal));
}
static void rumour_link_draw(RumourLink *l,int index){
 int x=l->x,y=60+l->y-(int)rumour_scroll,k=l->slot,kind=rumour_kind(k),selected=index==rumour_focus;
 if(y>=248||y+l->h<=60)return;
 unsigned edge=selected?RGOLD:RGB(123,131,155),fill=l->style==3?RGB(53,68,77):l->style==4?RGB(52,40,63):RGB(28,37,49);
 rumour_box(x,y,l->w,l->h,fill,edge);
 if(k>=RUMOUR_SLOTS){
  rumour_text(x+12,y+9,RGOLD,l->style==1?"LOCAL BEARING":"BEYOND THE LANES");
  if(l->style==1){rumour_wrap(x+12,y+28,120,RCREAM,"A buoy was heard off-route. Scan for local leads.",4);}
  else{rumour_wrap(x+12,y+28,l->w-24,RCREAM,rumour_name(k),2);rumour_wrap(x+12,y+58,l->w-24,RTEAL,"Search area, not a live lock. Follow faint reception. Slow below 120 m/s within 700m; scan takes two seconds.",l->h>140?5:2);}
  char price[32];snprintf(price,sizeof(price),rumour_owned(k)||rumour_found(k)?"VIEW OWNED DATA":"SEARCH AREA / %d U",rumour_price(k));if(l->style==1)snprintf(price,sizeof(price),rumour_owned(k)||rumour_found(k)?"VIEW DATA":"%d U / DATA",rumour_price(k));rumour_text(x+12,y+l->h-15,selected?RGOLD:RTEAL,price);return;
 }
 char label[56];const char *status=rumour_status(k);
 snprintf(label,sizeof(label),"%s",status?status:"");
 if(!status)snprintf(label,sizeof(label),"SEARCH AREA / %d U",rumour_price(k));
 if(l->style==0){
  rumour_text(x+12,y+10,RGOLD,kind==1?"ONE FREIGHTER.":kind==2?"GIANT LOCALS.":"OFF THE MAIN LANE.");
  rumour_wrap(x+12,y+27,136,RCREAM,kind==1?"Pre-loved. Previous owner unavailable.":"Take a detour. Find something worth seeing.",3);
  /* Small advert art is drawn at its displayed size, never a shrunken photo. */
  if(kind==1){
   rumour_box(x+176,y+39,94,20,RGB(73,92,103),RTEAL);
   rumour_box(x+211,y+29,29,12,RGB(85,101,112),DIM);
   for(int i=0;i<4;i++)rumour_box(x+181+i*19,y+42,13,13,RGB(44,60,72),DIM);
   int lit=(int)(preview_time*3)%4;rumour_box(x+217+lit*4,y+31,3,3,RGOLD,RGOLD);
   line(x+178,y+64,x+192,y+68,DIM);line(x+260,y+32,x+271,y+29,DIM);
  }else if(kind==2){
   for(int i=0;i<70;i++)line(x+170+i,y+45-abs(i-35)/6,x+170+i,y+45+abs(i-35)/8,RTEAL);
   line(x+239,y+45,x+257,y+31,RTEAL);line(x+205,y+45,x+198,y+61,RTEAL);
  }else{circle(x+221,y+45,16,RTEAL);line(x+221,y+22,x+221,y+69,RGOLD);line(x+199,y+45,x+243,y+45,RTEAL);}
  rumour_text(x+12,y+86,selected?RGOLD:RTEAL,label);
 }else if(l->style==1){
  rumour_text(x+12,y+9,RGOLD,kind==2?"MEET LOCALS":"CURIOUS?");
  /* Native small icon, separate from the full-size artwork. */
  if(kind==2){for(int a=0;a<34;a++)line(x+30+a,y+45-abs(a-17)/4,x+30+a,y+45+abs(a-17)/5,RTEAL);line(x+63,y+45,x+76,y+35,RTEAL);}
  else {circle(x+66,y+43,13,RTEAL);line(x+66,y+26,x+66,y+60,RGOLD);line(x+50,y+43,x+82,y+43,RTEAL);}
  rumour_wrap(x+12,y+64,120,RCREAM,status?"Your directions are here.":"A real local sighting.",2);
  char shortprice[18];snprintf(shortprice,sizeof(shortprice),status?"VIEW DATA":"%d U / DATA",rumour_price(k));rumour_text(x+12,y+89,RTEAL,shortprice);
 }else{
  const char *title=l->style==2?"CLASSIFIED / PRE-LOVED SPACE":l->style==3?"LOCAL EXPERIENCES / THE QUIET LIFE":"STRANGE SIGNALS / FIELD NOTES";
  rumour_text(x+12,y+10,RGOLD,title);
  if(l->style==3){
   rumour_art(x+146,y+29,kind,selected);rumour_text(x+12,y+38,RTEAL,"CAPTAIN VENN");
   for(int i=0;i<5;i++){int sx=x+16+i*14;line(sx-4,y+58,sx+4,y+58,RGOLD);line(sx,y+54,sx,y+62,RGOLD);}
   rumour_wrap(x+12,y+75,112,RCREAM,"Worth missing work for.",3);
   rumour_wrap(x+328,y+47,112,RTEAL,"Come see for yourself.",3);
   rumour_wrap(x+12,y+137,424,RCREAM,k==12?"Come fly alongside these amazing space whales! We'll send you the search area. You bring the camera!":"Take the scenic route for once. We'll send you a local sighting. You bring the camera!",2);
  }else if(l->style==2){
   rumour_art(x+12,y+29,kind,selected);
   rumour_wrap(x+192,y+31,240,RCREAM,kind==1?"Someone left a giant ship out here. We have its last sighting; you can take a closer look. The ship itself is not for sale.":"A traveller found something worth the detour. Buy their search area, then follow your scanner to the real thing.",6);
   rumour_wrap(x+12,y+137,424,RTEAL,rumour_name(k),2);
  }else {
   for(int i=0;i<95;i++){int sx=x+14+i*4,h=3+(i*13+(int)(preview_time*6))%17;line(sx,y+45-h/2,sx,y+45+h/2,i%3?RTEAL:RGOLD);}
   rumour_wrap(x+12,y+70,424,RCREAM,k>=8&&k<12?"I switched my radio off. The knocking carried on. I've saved the area, if you'd like to take a look. Bring your scanner; the source is easy to miss.":"There's more out here than passing traffic. Someone sent us a quiet sighting from beyond the usual lanes. We'll pass the search area on to you.",5);
  }
  rumour_box(x+12,y+162,424,20,selected?RGB(75,96,101):RGB(39,51,65),edge);
  rumour_text(x+12+(424-(int)strlen(label)*8)/2,y+168,RCREAM,label);
 }
}
static void spacepay_draw(void){
 rect(0,42,480,206,RGB(20,15,31));int k=spacepay_slot;
 rect(20,57,440,177,RGB(38,31,53));line(20,57,459,57,RGOLD);
 text_px(38,70,RGOLD,"SpacePay");text_px(254,70,RTEAL,"SMALL FEES. BIG SPACE.");
 text_px(38,92,RCREAM,"%s",spacepay_stage==1?"REVIEW NAV DATA":"NAV DATA RECEIVED");
 char desc[80];snprintf(desc,sizeof(desc),"%s / %s",game.systems[game.system].name,rumour_name(k));
 rumour_wrap(38,108,400,RCREAM,desc,2);
 if(spacepay_stage==1){
  int cost=(rumour_owned(k)||rumour_found(k))?0:rumour_price(k)*10;
  text_px(38,138,RTEAL,"COST %.1f U",cost*.1f);text_px(242,138,RCREAM,"BALANCE %.1f U",game.credits*.1f);
  text_px(38,154,DIM,"AFTER PURCHASE: %.1f U",(game.credits-cost)*.1f);
  rumour_wrap(38,170,400,spacepay_error?RED:DIM,spacepay_error?spacepay_error:(rumour_kind(k)==3?"Search area only. Find the source; Triangle scans within 1,100m.":"Search area only. Find the contact; Triangle inspects it up close."),2);
  const char *a=rumour_owned(k)||rumour_found(k)?"VIEW DIRECTIONS":"CONFIRM";
  rumour_box(38,204,190,21,spacepay_choice==0?RPURPLE:RGB(22,25,37),spacepay_choice==0?RGOLD:DIM);
  rumour_box(242,204,190,21,spacepay_choice==1?RPURPLE:RGB(22,25,37),spacepay_choice==1?RGOLD:DIM);
  text_px(38+(190-(int)strlen(a)*8)/2,211,RCREAM,"%s",a);text_px(313,211,RCREAM,"CANCEL");
 }else {
  float t=fmaxf(0,preview_time-spacepay_started),p=fminf(1,t/.75f);
  if(spacepay_stage==2){
   int cx=52+(int)(p*350);rumour_box(cx,140,12,10,RGOLD,RTEAL);rumour_text(cx+2,141,RGB(38,31,53),"U");
   rect(38,165,394,8,RGB(15,20,32));rect(38,165,(int)(394*p),8,RTEAL);
   text_px(38,185,RCREAM,"RECEIVING YOUR NAV DATA...");
  }else {
   text_px(38,138,RTEAL,spacepay_latched==2?"DIRECTIONS READY":"PAYMENT COMPLETE");
   int paid=k>=RUMOUR_SLOTS?(rumour_owned(k)?rumour_price(k)*10:0):game.rumours.paid[game.system][k];text_px(38,154,RCREAM,"PAID %.1f U",paid*.1f);text_px(242,154,RGOLD,"BALANCE %.1f U",game.credits*.1f);
   for(int i=0;i<6&&t<1.3f;i++){float a=i*1.0472f,r=(t-.6f)*40;pixel(410+(int)(cosf(a)*r),138+(int)(sinf(a)*r),RTEAL);}
   rumour_box(38,181,394,20,spacepay_choice==0?RPURPLE:RGB(22,25,37),spacepay_choice==0?RGOLD:DIM);
   text_px(165,187,RCREAM,"TRACK SEARCH AREA");
   rumour_box(38,207,394,20,spacepay_choice==1?RPURPLE:RGB(22,25,37),spacepay_choice==1?RGOLD:DIM);
   text_px(177,213,RCREAM,"KEEP BROWSING");
  }
 }
 footer("X CHOOSE   O BACK");
}
static void rumours_screen(void){
 rumour_layout();
 if(spacepay_stage){spacepay_draw();return;}
 rect(0,42,480,206,RPURPLE);rect(4,44,460,12,RGB(32,42,58));
 text_px(12,46,RTEAL,"WWW.SPACESECRETS.NET");
 text_px(300,46,RCREAM,"LOCAL: %s",opening_sol(&game)?"EARTH":game.systems[game.system].name);
 int oldx0=clipx0,oldx1=clipx1,oldy0=clipy0,oldy1=clipy1;
 clipx0=0;clipx1=464;clipy0=60;clipy1=248;
 int y=60-(int)rumour_scroll;rumour_box(10,y,448,38,RGB(44,31,59),RGB(130,95,146));
 int blink=((int)(preview_time*10)%73)>70;
 line(22,y+18,40,y+(blink?18:10),RTEAL);line(40,y+(blink?18:10),58,y+18,RTEAL);line(22,y+18,40,y+(blink?18:26),RTEAL);line(40,y+(blink?18:26),58,y+18,RTEAL);
 if(!blink)rumour_box(38,y+14,5,8,RGOLD,RGOLD);
 rumour_text(68,y+4,RGOLD,"SPACE SECRETS!");
 rumour_text(68,y+15,RCREAM,"PAY UNITS FOR NAVIGATION DATA.");
 rumour_text(68,y+26,RTEAL,"DISCOVER HIDDEN PLACES IN SPACE.");
 if(!rumour_links_n)rumour_wrap(22,110,424,RCREAM,"Quiet skies today. No new local sightings, but you can still find things the old-fashioned way.",5);
 for(int i=0;i<rumour_links_n;i++)rumour_link_draw(&rumour_links[i],i);
 if(rumour_links_n){int fy=60+rumour_doc_height-43-(int)rumour_scroll;rumour_text(22,fy,RTEAL,"100% LOCAL. ZERO MAILING LISTS.");rumour_text(22,fy+16,DIM,"SPACEPAY ACCEPTED. TEA OPTIONAL.");}
 clipx0=oldx0;clipx1=oldx1;clipy0=oldy0;clipy1=oldy1;
 rect(468,62,3,184,RGB(41,31,53));int thumb=(int)fmaxf(12,184.f*188/fmaxf(188,rumour_doc_height));
 rect(468,62+(int)((184-thumb)*rumour_scroll/fmaxf(1,rumour_doc_height-188)),3,thumb,RGOLD);
 footer(game.rumours.active?"UP/DOWN BROWSE   X REVIEW   SQ STOP   O BACK":"UP/DOWN BROWSE   X REVIEW   O BACK");
 rumour_ad_draw();
}
static int rumours_input(unsigned pressed,unsigned held,float dt){
 rumour_layout();
 if(spacepay_wait_release){if(!(held&PSP_CTRL_CROSS))spacepay_wait_release=0;pressed&=~PSP_CTRL_CROSS;}
 if(spacepay_stage)rumour_ad_clock=0;
 else if(pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER|PSP_CTRL_CIRCLE))rumour_ad_reset();
 else if(rumour_ad_input(pressed,dt))return 1;
 if(spacepay_stage){
  if(pressed&PSP_CTRL_CIRCLE){spacepay_stage=0;spacepay_error=0;return 1;}
  if(spacepay_stage==2){if(preview_time-spacepay_started>=.75f){spacepay_stage=3;spacepay_choice=1;}
   else if(pressed&PSP_CTRL_CROSS){spacepay_stage=3;spacepay_choice=1;spacepay_wait_release=1;}return 1;}
  if(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT|PSP_CTRL_UP|PSP_CTRL_DOWN))spacepay_choice^=1;
  if(pressed&PSP_CTRL_CROSS){
   spacepay_wait_release=1;
   if(spacepay_choice==1){spacepay_stage=0;spacepay_error=0;}
   else if(spacepay_stage==1){
    int result=rumour_purchase(spacepay_slot);spacepay_latched=result;
    if(result<0){spacepay_error=result==-2?"Not enough units. Nothing was charged.":"Sighting unavailable. Nothing was charged.";spacepay_choice=1;}
    else{spacepay_stage=result==1?2:3;spacepay_started=preview_time;spacepay_choice=1;}
   }else if(rumour_track(spacepay_slot)){spacepay_stage=0;}
   else spacepay_error="Directions are no longer available.";
  }return 1;
 }
 /* Shoulder switching stays on GalacticNet; never enter another modal. */
 if(pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER)){galnet_tab=(galnet_tab+(pressed&PSP_CTRL_RTRIGGER?1:5))%6;if(opening_sol(&game)&&galnet_tab==2)galnet_tab=3;row=0;return 1;}
 if(pressed&PSP_CTRL_CIRCLE){int stage=game.opening.stage;change_page(HOME);if(stage==OP_EARTH_NET||stage==OP_DAD_INBOX)opening_advance();return 1;}
 if(pressed&PSP_CTRL_SQUARE){game.rumours.active=0;game.navigation.search_key=0;game.cue=SFX_SELECT;return 1;}
 int prev=rumour_focus;
 if(rumour_links_n){
  if(pressed&PSP_CTRL_DOWN)rumour_focus=rumour_focus<2&&rumour_links_n>2?2:rumour_focus+1<rumour_links_n?rumour_focus+1:rumour_focus;
  if(pressed&PSP_CTRL_UP)rumour_focus=rumour_focus==2?0:rumour_focus>0?rumour_focus-1:0;
  if((pressed&PSP_CTRL_LEFT)&&rumour_focus==1)rumour_focus=0;
  if((pressed&PSP_CTRL_RIGHT)&&rumour_focus==0&&rumour_links_n>1&&rumour_links[1].y==rumour_links[0].y)rumour_focus=1;
  if(pressed&PSP_CTRL_CROSS){spacepay_slot=rumour_links[rumour_focus].slot;spacepay_stage=1;spacepay_choice=1;spacepay_error=0;spacepay_wait_release=1;}
 }
 if(prev!=rumour_focus){rumour_focus_scroll();game.cue=SFX_SELECT;}
 if(fabsf(rumour_scroll-rumour_scroll_goal)<1)rumour_scroll=rumour_scroll_goal;
 else rumour_scroll+=(rumour_scroll_goal-rumour_scroll)*(1-expf(-dt*24));
 return 1;
}
