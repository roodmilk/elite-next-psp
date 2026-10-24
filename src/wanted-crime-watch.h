/* Crime Watch: native broadcast layout, corrected source portraits, UI-only motion. */
static int wanted_record=-1,wanted_system=-1,wanted_cache_system=-1,wanted_direction=1;
static float wanted_enter,wanted_slide,wanted_clock,wanted_claim_age,wanted_exit=-1;
static int wanted_seen_claim;
static unsigned char wanted_stamped[256];
static unsigned wanted_dna[BOUNTY_POSTER_COUNT];
static void wanted_ui_reset(void){wanted_record=wanted_system=-1;wanted_enter=wanted_slide=wanted_clock=wanted_claim_age=0;wanted_exit=-1;wanted_seen_claim=0;}
static void wanted_cache(void){
 if(wanted_cache_system==game.system)return;
 for(int i=0;i<BOUNTY_POSTER_COUNT;i++)wanted_dna[i]=portrait_seeded((unsigned)(game.system*31+i*19),PIRATES,-1);
 wanted_cache_system=game.system;
}
static void wanted_sync(void){
 wanted_cache();if(row<0||row>=BOUNTY_POSTER_COUNT)row=0;
 if(wanted_system!=game.system||wanted_record<0){wanted_system=game.system;wanted_record=row;wanted_enter=wanted_slide=0;wanted_seen_claim=0;}
 else if(wanted_record!=row){wanted_direction=(row==(wanted_record+1)%BOUNTY_POSTER_COUNT)?1:-1;wanted_record=row;wanted_slide=0;wanted_seen_claim=0;}
 int claimed=bounty_target_taken(&game,row);
 if(claimed&&!wanted_seen_claim){unsigned bit=1u<<row;wanted_claim_age=(wanted_stamped[game.system]&bit)?1:0;wanted_stamped[game.system]|=bit;}
 wanted_seen_claim=claimed;
}
static int wanted_input(unsigned pressed,float dt){
 if(page!=GALNET||galnet_tab!=2)return 0;
 wanted_sync();dt=fmaxf(0,fminf(.1f,dt));wanted_enter+=dt;wanted_slide+=dt;wanted_clock+=dt;wanted_claim_age+=dt;
 if(pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER)){
  galnet_tab=(galnet_tab+(pressed&PSP_CTRL_RTRIGGER?1:5))%6;row=0;wanted_ui_reset();game.cue=SFX_SELECT;return 1;
 }
 if(pressed&PSP_CTRL_CIRCLE){if(wanted_exit<0){wanted_exit=0;game.cue=SFX_UI;}}
 if(wanted_exit>=0){wanted_exit+=dt;if(wanted_exit>=.12f){wanted_ui_reset();change_page(HOME);}return 1;}
 if(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT)){
  wanted_direction=(pressed&PSP_CTRL_RIGHT)?1:-1;row=(row+BOUNTY_POSTER_COUNT+wanted_direction)%BOUNTY_POSTER_COUNT;
  wanted_record=row;wanted_slide=0;wanted_seen_claim=0;wanted_sync();game.cue=SFX_SELECT;
 }return 1; /* Informational only: X, Up/Down and tracking never act. */
}
static void wanted_text(int x,int y,unsigned ink,const char *s,int scale){
 for(;*s;s++,x+=8*scale){unsigned ch=(unsigned char)*s;if(ch<32||ch>126)ch='?';
  for(int yy=0;yy<8;yy++)for(int xx=0;xx<8;xx++)if(font8[ch-32][yy]&(128>>xx))rect(x+xx*scale,y+yy*scale,scale,scale,ink);
 }
}
static unsigned wanted_rgb(uint16_t p){return RGB(((p>>10)&31)*255/31,((p>>5)&31)*255/31,(p&31)*255/31);}
/* Fixed viewport clipping. Only the new identity moves in; old faces never sit
 * beside a newly selected name/reward. Source rows and scanlines never scroll. */
static void wanted_portrait(int poster,int x,int y,int w,int h,int shift,int signal){
 int side=w<h?w:h,ox=x+(w-side)/2+shift,oy=y+(h-side)/2;
 const uint16_t *source=portrait_legacy_source(portrait_pulp_base(wanted_dna[poster]));
 for(int yy=y;yy<y+h;yy++){
  int sy=(yy-oy)*64/side;int scan=signal&&((yy-y-(int)(wanted_clock*4))&3)==0;
  for(int xx=x;xx<x+w;xx++){
   unsigned c=RGB(7,16,21);
   if(xx>=ox&&xx<ox+side&&sy>=0&&sy<64){uint16_t p=source[sy*64+(xx-ox)*64/side];if(p&0x8000)c=wanted_rgb(p);}
   if(signal){c=art_mix(c,RGB(69,201,138),20);if(scan)c=art_mix(c,RGB(6,33,20),20);}
   fb[yy*STRIDE+xx]=c;
  }
 }
}
static void wanted_wipe(float progress){
 if(progress>=1)return;
 progress=fmaxf(0,progress);float q=1-progress;int edge=(int)(-65+610*(1-q*q*q));
 for(int y=42;y<248;y++){int x=edge+60-(y-42)*60/205;if(x<0)x=0;if(x<W)rect(x,y,W-x,1,RGB(12,20,38));}
}
static void galnet_bounties(void){
 wanted_sync();const unsigned navy=RGB(21,33,60),pink=RGB(255,75,131),cream=RGB(244,231,201),cyan=RGB(118,220,233),gold=RGB(255,227,136);
 /* Reuse the compiled authored space plate. No runtime image decode or buffer. */
 for(int y=42;y<248;y++)for(int x=0;x<W;x++)fb[y*STRIDE+x]=wanted_rgb(intro_backdrop[y*W+x]);
 for(int y=42;y<=194;y++){
  int edge=307-(y-42)*61/152;rect(0,y,edge,1,navy);
  int slash=280-(y-42)*59/152;rect(slash,y,10,1,pink);
 }
 for(int y=42;y<62;y++){int x=306-(y-42)*8/20;rect(x,y,W-x,1,RGB(242,215,125));}
 char label[32],reward[24],ribbon[24],risk[16];bounty_target_label(&game,row,label,sizeof(label));
 snprintf(reward,sizeof(reward),"%.1f U",bounty_target_reward(&game,row)*.1f);
 snprintf(ribbon,sizeof(ribbon),"%.8s / LOCAL LAW",game.systems[game.system].name);
 snprintf(risk,sizeof(risk),"RISK %d/5",danger_rating(&game,game.system));
 wanted_text(320,49,RGB(52,43,38),ribbon,1);
 wanted_text(13,52,cyan,"GALACTICNET CRIME WATCH",1);wanted_text(13,72,cream,"WANTED",3);rect(15,104,120,3,pink);
 wanted_text(14,116,cream,label,1);wanted_text(14,136,cyan,"BOUNTY",1);
 /* Integer 3x real game glyphs keep the reward prominent and fully fitting. */
 wanted_text(12,150,gold,reward,3);wanted_text(15,181,cyan,risk,1);
 rect(298,71,173,135,cyan);rect(299,72,171,133,RGB(5,12,19));
 float p=fminf(1,wanted_slide/.18f),q=1-p;int shift=(int)(169*q*q*q)*wanted_direction;
 wanted_portrait(row,300,73,169,112,shift,1);
 int claimed=bounty_target_taken(&game,row);rect(302,186,165,17,claimed?RGB(42,101,75):RGB(194,68,98));
 const char *status=claimed?"TARGET TAKEN":"LIVE WARRANT";wanted_text(302+(165-(int)strlen(status)*8)/2,192,cream,status,1);
 for(int i=0;i<3;i++)rect(306+i*5,188,3,2,((int)(wanted_clock*3)%3==i)?gold:RGB(89,45,61));
 if(claimed&&wanted_claim_age<.24f){int a=(int)(wanted_claim_age/.24f*12);rect(302+a,186+a/4,165-a*2,1,gold);}
 rect(0,209,480,39,RGB(16,26,41));wanted_text(10,217,RGB(139,159,169),"LOCAL",1);wanted_text(10,230,RGB(139,159,169),"FILES",1);
 for(int i=0;i<BOUNTY_POSTER_COUNT;i++){
  int x=62+i*82;unsigned edge=i==row?RGB(255,191,84):RGB(55,83,105);
  rect(x,214,76,29,edge);rect(x+1,215,74,27,i==row?RGB(23,53,64):RGB(23,35,45));
  wanted_portrait(i,x+2,216,31,24,0,0);
  char number[3];snprintf(number,sizeof(number),"%02d",i+1);wanted_text(x+39,218,i==row?gold:cream,number,1);
  if(bounty_target_taken(&game,i)){line(x+41,235,x+44,238,cyan);line(x+44,238,x+52,230,cyan);}
  else if(i==row)rect(x+39,232,22,2,RGB(255,191,84));
  if(i==row)rect(x,244,76,2,RGB(255,191,84));
 }
 /* Tiny distant lamps only; the space plate and meaningful text stay still. */
 for(int i=0;i<4;i++){int x=250+i*11,y=202;unsigned c=(i==(int)(wanted_clock*.5f)%4)?cyan:RGB(44,77,91);pixel(x,y,c);}
 wanted_wipe(wanted_exit>=0?1-wanted_exit/.12f:wanted_enter/.26f);
 footer("L/R SECTION   LEFT/RIGHT WARRANT   O BACK");
}
