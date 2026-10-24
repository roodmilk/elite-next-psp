/* Paid receipts are presentation only: this modal never awards credits. */
static int mission_card_wrap(int x,int y,int cols,int rows,unsigned ink,const char *source);
#include "mission-expansion-ui.h"
static unsigned mission_card_serial;
static float mission_card_age;
static int mission_card_active,mission_card_released,mission_card_release;
static void mission_card_reset(void){expedition_call=-1;expedition_history=expedition_call_exit=0;expedition_call_key=0;mission_card_serial=0;mission_card_age=0;mission_card_active=mission_card_released=mission_card_release=0;}
static int mission_card_safe(void){return page!=INTRO&&!game.dead&&!game.police_stop&&!game.dock_stage&&game.jump<=0;}
static void mission_card_sync(void){
 expedition_call_sync();
 if(page==INTRO){mission_card_reset();return;}
 if(!game.mission_receipt_n){mission_card_active=0;return;}
 if(!mission_card_safe())return;
 MissionReceipt *r=&game.mission_receipts[0];
 if(!mission_card_active||mission_card_serial!=r->serial){
  mission_card_serial=r->serial;mission_card_active=1;mission_card_age=0;mission_card_released=0;
  game.message_time=game.voice_time=0;game.message[0]=0;game.cue=SFX_OBSERVATORY;
 }
}
static int mission_card_open(void){return expedition_call_open()||(mission_card_active&&game.mission_receipt_n>0&&mission_card_safe());}
static int mission_card_input(unsigned pressed,unsigned held,float dt){
 mission_card_sync();
 if(expedition_call_input(pressed,held,dt))return 1;
 if(mission_card_open()){
  mission_card_age+=dt;
  if(!(held&PSP_CTRL_CROSS))mission_card_released=1;
  if(mission_card_age>=.65f&&mission_card_released&&(pressed&PSP_CTRL_CROSS)){
   mission_receipt_pop(&game);mission_card_active=0;mission_card_release=1;
   fire_blocked=1;tools_block_until_release();game.cue=SFX_SELECT;
  }
  return 1;
 }
 if(mission_card_release){if(!held)mission_card_release=0;return 1;}
 return 0;
}
static int mission_card_role(const MissionReceipt *r){
 if(r->kind==RECEIPT_ACCEPTED)return r->role>=TRADERS&&r->role<=EXPLORERS?r->role:TRADERS;
 if(r->kind==RECEIPT_BOARD&&expedition_def(r->type))return expedition_def(r->type)->role;
 if(r->kind==RECEIPT_SAGA&&r->type>=0&&r->type<SAGA_COUNT)return saga_speaker_role(&saga_beats[r->type]);
 if(r->kind==RECEIPT_STATION)return r->type;
 if(r->kind==RECEIPT_GUILD||r->kind==RECEIPT_CAMPAIGN||r->kind==RECEIPT_STORY)return EXPLORERS;
 return TRADERS;
}
static int mission_card_seed(const MissionReceipt *r){
 if(r->kind==RECEIPT_ACCEPTED)return r->portrait_seed;
 if(r->kind==RECEIPT_STATION)return r->system*37+mission_card_role(r)*17+(unsigned char)r->sender[0]*3;
 unsigned seed=0;for(const char *p=r->sender;*p;p++)seed=seed*131u+(unsigned char)*p;return (int)(seed%2000)+mission_card_role(r)*37;}
static const char *mission_card_thanks(const MissionReceipt *r){
 if(r->kind==RECEIPT_ACCEPTED&&r->type==MISSION_PROCUREMENT)return "Exchange registered. The exact cargo and meeting system are in your Mission Log. Find this trader again and hail when your hold is ready.";
 if(r->kind==RECEIPT_ACCEPTED)return "Call logged. Protect the distress vessel and stop the marked attacker. The verified reward will be paid when they are clear.";
 if(r->kind==RECEIPT_FAILED&&r->type==MISSION_PROCUREMENT)return "This cargo exchange is closed. No goods or credits changed hands. You may accept another offer whenever you are ready.";
 if(r->kind==RECEIPT_FAILED)return "The distress encounter ended before the vessel reached safety. The contract is closed and no reward has been paid.";
 if(r->kind==RECEIPT_BOARD&&r->type==MISSION_PROCUREMENT)return "Clean exchange confirmed. The agreed cargo has moved between both holds. No credits were charged and the appointment is now closed.";
 if(r->kind==RECEIPT_BOARD&&r->type==MISSION_DISTRESS)return "We're clear, Commander. You stayed when we needed you. The verified rescue reward has been transferred in full.";
 if(r->kind==RECEIPT_BOARD&&r->type==MISSION_RACE)return "Contract achieved! That was some flying, Commander. Your agreed prize is in your account. See you at the next grid!";
 if(r->kind==RECEIPT_PASSENGER)return "That was a good flight. Thank you for getting me here safely. Your fare is paid. Safe travels, Commander!";
 if(r->kind==RECEIPT_CAMPAIGN)return "A safe launch and a safe return. That's a first flight to be proud of. Your harbour badge and payment are yours!";
 if(r->kind==RECEIPT_STORY||r->kind==RECEIPT_SAGA)return "You made a difference out there. We've sent your payment, with our thanks. Keep that channel open, Commander.";
 if(r->kind==RECEIPT_GUILD)return "Good work, Commander. The Guild has recorded your assignment and sent your reward. Here's to your next adventure!";
 if(r->kind==RECEIPT_STATION&&!r->amount)return "You're one of the familiar faces here now. Thanks for stopping by. There'll always be a seat for you, Commander.";
 if(r->kind==RECEIPT_STATION)return "That's just what we needed. Thank you for seeing it through. Your payment has arrived. Drop by again sometime!";
 switch(mission_base_type(r->type)){
 case MISSION_DELIVERY:return r->serial&1?"The shipment is here, safe and sound. Thank you, Commander! Your payment is through. We'd fly with you again.":"Delivery confirmed! You got it here when we needed it. The agreed reward is yours. Safe skies, Commander.";
 case MISSION_BOUNTY:return "Target confirmed. You've made these lanes safer for everyone. Your contract reward has been paid. Good hunting, Commander.";
 case MISSION_EXPLORATION:return "Your field report just came through. There's a little less unknown out there now. Thank you! Your reward has arrived.";
 case MISSION_RESCUE:return "They're home. You can't put a price on that, but we can at least pay what we promised. Thank you for bringing them back.";
 default:return "The job is done and your payment is through. You kept your word, Commander. We won't forget it. Safe travels out there.";
 }
}
/* Native 8x8 glyphs; wrapping shares the dialogue's ASCII normalization. */
static int mission_card_wrap(int x,int y,int cols,int rows,unsigned ink,const char *source){
 char ascii[256];dialogue_ascii(ascii,sizeof(ascii),source);const char *p=ascii;
 for(int row_id=0;row_id<rows&&*p;row_id++){
  int len=(int)strlen(p),cut=len<cols?len:cols;
  if(len>cols)for(int k=cut;k>0;k--)if(p[k]==' '){cut=k;break;}
  char line_text[64];memcpy(line_text,p,cut);line_text[cut]=0;text_px(x,y+row_id*12,ink,"%s",line_text);
  p+=cut;while(*p==' ')p++;
 }
 return !*p;
}
static void mission_card_draw(void){
 if(expedition_call_open()){expedition_call_draw();return;}
 if(!mission_card_open())return;
 const MissionReceipt *r=&game.mission_receipts[0];
 unsigned dark=RGB(5,10,18),metal=ui_paint_surface(RGB(23,34,46)),edge=UI_SIGNAL,cream=RGB(234,232,211),muted=RGB(151,174,186);
 float t=fminf(1.f,mission_card_age/.62f);int ox=(int)(480*(1-t)*(1-t)*(1-t));
 rect(0,0,W,H,dark);
 /* Warp arrival lines occupy the background rather than obscuring the call. */
 if(t<1)for(int i=0;i<12;i++){int yy=8+i*23;line(0,yy,(int)(480*t),yy,mix_rgb(dark,edge,(1-t)*.55f));}
 rect(12+ox,14,456,242,edge);rect(14+ox,16,452,238,metal);
 rect(24+ox,26,432,20,RGB(8,18,25));text_px(32+ox,32,cream,r->kind==RECEIPT_ACCEPTED?"MISSION ACCEPTED":r->kind==RECEIPT_FAILED?"MISSION FAILED":"TRANSMISSION RECEIVED");
 circle(340+ox,36,2,RGB(102,226,168));text_px(350+ox,32,muted,"VERIFIED");
 for(int i=0;i<4;i++)rect(428+ox+i*5,39-i*2,3,3+i*2,(int)(mission_card_age*8)%5>i?cream:edge);
 mission_card_wrap(32+ox,56,52,2,cream,r->title);
 rect(30+ox,89,108,108,edge);rect(32+ox,91,104,104,dark);
 if(!strcmp(r->sender,"KEI")||!strcmp(r->sender,"RYN"))draw_kei(36+ox,95,96,!strcmp(r->sender,"RYN"));
 else draw_portrait(36+ox,95,96,96,mission_card_seed(r),mission_card_role(r));
 rect(34+ox,180,100,13,RGB(8,18,25));text_px(40+ox,183,cream,"VIDEO LINK");
 text_px(152+ox,96,edge,"%s",r->sender);
 text_px(152+ox,113,muted,r->kind==RECEIPT_ACCEPTED?"%s / ACTIVE CONTRACT":r->kind==RECEIPT_FAILED?"%s / CONTRACT CLOSED":"%s / MISSION COMPLETE",game.systems[r->system].name);
 line(152+ox,127,448+ox,127,edge);
 mission_card_wrap(152+ox,136,37,5,cream,mission_card_thanks(r));
 rect(30+ox,202,420,30,RGB(8,18,25));rect(30+ox,202,3,30,edge);
 int barter=r->type==MISSION_PROCUREMENT&&!r->amount;text_px(42+ox,213,muted,barter?"DIRECT CARGO EXCHANGE":r->kind==RECEIPT_ACCEPTED?"REWARD ON COMPLETION":r->kind==RECEIPT_FAILED?"NO PAYMENT":r->amount?"PAYMENT RECEIVED":"NO CREDIT TRANSFER");
 char money[32];if(barter)snprintf(money,sizeof(money),"NO UNITS");else if(r->kind==RECEIPT_FAILED)snprintf(money,sizeof(money),"--");else snprintf(money,sizeof(money),r->kind==RECEIPT_ACCEPTED?"%.1f U":"+%.1f U",r->amount*.1f);text_px(438+ox-(int)strlen(money)*8,213,cream,"%s",money);
 button_icon(198+ox,238,'X',RGB(108,195,255));text_px(216+ox,240,cream,"CONTINUE");
 if(game.mission_receipt_n>1)text_px(32+ox,240,muted,"%d CALLS",game.mission_receipt_n);
 for(int i=0;i<4;i++){int sx=20+ox+(i&1)*440,sy=22+(i/2)*226;circle(sx,sy,2,muted);line(sx-1,sy,sx+1,sy,dark);}
 /* Completion celebrates; acceptance uses the quieter incoming space card. */
 if(t>=1&&r->kind!=RECEIPT_ACCEPTED&&r->kind!=RECEIPT_FAILED)for(int b=0;b<4;b++){
  float f=fmodf(mission_card_age-.62f+b*.47f,2.6f)/1.6f;if(f>1)continue;
  int cx=b&1?465:15,cy=b<2?74:191;unsigned ink=mix_rgb(b&1?RGB(255,184,80):RGB(104,223,208),dark,f);
  for(int i=0;i<12;i++){float a=i*.523599f;int x=cx+(int)(cosf(a)*f*20),y=cy+(int)(sinf(a)*f*20+f*f*5);if(x<28||x>452){pixel(x,y,ink);pixel(x,y+1,ink);}}
 }
}
