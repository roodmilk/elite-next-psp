/* Modal timing, money/input isolation, actual native art and synthesized cues. */
static void rumour_ad_wav(const char *name,int id){
 FILE *f=fopen(name,"wb");if(!f)return;int n=audio_sfx_length(id);unsigned char h[44]={0};
 memcpy(h,"RIFF",4);memcpy(h+8,"WAVEfmt ",8);memcpy(h+36,"data",4);
 unsigned values[]={36+n*2,16,44100,88200,n*2};const int offsets[]={4,16,24,28,40};
 for(int i=0;i<5;i++)for(int k=0;k<4;k++)h[offsets[i]+k]=(unsigned char)(values[i]>>(k*8));
 h[20]=h[22]=1;h[32]=2;h[34]=16;fwrite(h,1,44,f);unsigned phase=0,noise=7;
 for(int age=0;age<n;age++){int sample=audio_sfx_sample(id,age,&phase,&noise);unsigned char b[2]={(unsigned char)sample,(unsigned char)(sample>>8)};fwrite(b,1,2,f);}fclose(f);
}
static int rumour_ads_review(void){
 FILE *flag=fopen("rumour-ads-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *out=fopen("rumour-ads-review.txt","w");if(!out)return 1;int failures=0;
 unsigned *oldfb=fb,*allocation=malloc((STRIDE*H+32)*sizeof(unsigned));if(!allocation){fprintf(out,"RESULT 1 failures\n");fclose(out);return 1;}
 for(int i=0;i<16;i++)allocation[i]=allocation[16+STRIDE*H+i]=0xdeadbeefu;fb=allocation+16;
#define ADCHECK(c,label) do{int ok=!!(c);fprintf(out,"%s %s\n",ok?"PASS":"FAIL",label);failures+=!ok;fflush(out);}while(0)
 game_init(&game);story_complete(&game);game.opening.stage=OP_FREE;game.system=7;game_spawn(&game);game.docked=1;game.credits=100000;
 identity_slot=-99;identity_sync();page=GALNET;galnet_tab=5;row=0;rumour_page_system=-1;rumour_layout();deck_open_cancel();preview_time=20;spacepay_wait_release=0;
 int fit=RUMOUR_AD_COUNT==24;unsigned hashes[24];
 for(int a=0;a<RUMOUR_AD_COUNT;a++){
  fit&=strlen(rumour_ads[a].title)<=34&&rumour_ad_copy(rumour_ads[a].copy,0)<=5;
  unsigned hash=0;for(int y=0;y<16;y++){fit&=strlen(rumour_ad_art[a][y])==16;for(int x=0;x<16;x++){char p=rumour_ad_art[a][y][x];fit&=strchr(".#BALW",p)!=0;hash=hash*33+p;}}
  hashes[a]=hash;for(int k=0;k<a;k++)fit&=hashes[k]!=hash;
 }
 ADCHECK(fit,"24 unique authored pictures, bounded titles and fully fitting five-line copy");
 int cash=game.credits,focus=rumour_focus;float world_time=game.time;Vec3 pos=game.pos;
 for(int i=0;i<140;i++)input(0,0,.05f,0,0);
 ADCHECK(!rumour_ad_phase&&rumour_ad_clock>6.9f,"browsing begins with seven clear seconds, no immediate pop-up");
 for(int i=0;i<24;i++)input(0,0,.05f,0,0);
 ADCHECK(rumour_ad_phase&&game.cue==SFX_AD_OPEN,"real idle input shows the first advert after eight seconds with the whoop cue");
 input(0,0,.1f,0,0);input(0,0,.1f,0,0);
 ADCHECK(rumour_ad_phase==2&&rumour_ad_scale()==1,"opening settles to exact native pixels within 140ms");
 int active_before=game.rumours.active;
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);input(PSP_CTRL_DOWN,PSP_CTRL_DOWN,.016f,0,0);input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE,.016f,0,0);
 ADCHECK(!spacepay_stage&&rumour_focus==focus&&game.credits==cash&&game.rumours.active==active_before,"modal advert blocks payment, browse and tracking actions underneath");
 ADCHECK(game.time==world_time&&length(sub(pos,game.pos))<.001f,"advert browsing keeps the world paused");
 input(PSP_CTRL_TRIANGLE,PSP_CTRL_TRIANGLE,.016f,0,0);
 ADCHECK(rumour_ad_phase==3&&game.cue==SFX_AD_CLOSE,"Triangle begins shrinking with a distinct thwip cue");
 input(PSP_CTRL_TRIANGLE,PSP_CTRL_TRIANGLE,.04f,0,0);
 ADCHECK(rumour_ad_phase==3&&rumour_ad_age>.03f&&rumour_ad_scale()<1,"repeated Triangle does not restart or reverse the close animation");
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.1f,0,0);
 ADCHECK(!rumour_ad_phase&&!spacepay_stage&&rumour_ad_wait>=22&&rumour_ad_wait<=35,"last shrinking frame consumes X and schedules a generous 22-35 second cooldown");
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);ADCHECK(!spacepay_stage,"held X cannot click through after dismissal");
 input(0,0,.016f,0,0);input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 ADCHECK(spacepay_stage==1&&game.credits==cash,"a fresh X still opens the normal SpacePay review without charging");
 for(int i=0;i<400;i++)input(0,0,.1f,0,0);
 ADCHECK(!rumour_ad_phase&&!rumour_ad_clock&&spacepay_stage==1,"forty seconds in payment review never spawns an advert");
 input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);rumour_ad_clock=rumour_ad_wait;
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 ADCHECK(spacepay_stage==1&&!rumour_ad_phase,"a purchase request takes priority when an advert is due on the same frame");
 input(0,0,.016f,0,0);input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);rumour_ad_reset();
 for(int i=0;i<200;i++)input(0,0,.1f,0,0); /* Show another advert, then test exits. */
 input(PSP_CTRL_LTRIGGER,PSP_CTRL_LTRIGGER,.016f,0,0);
 ADCHECK(galnet_tab==4&&!rumour_ad_phase&&!rumour_ad_clock,"L can leave an advert for Inbox and clears its overlay");
 galnet_tab=5;rumour_ad_clock=8;input(0,0,.016f,0,0);input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);
 ADCHECK(galnet_tab==0&&!rumour_ad_phase,"R leaves an advert for NEWS without a stranded overlay");
 galnet_tab=5;rumour_ad_clock=8;input(0,0,.016f,0,0);input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);
 ADCHECK(page==HOME&&!rumour_ad_phase,"Circle returns home even while an advert is visible");
 change_page(GALNET);galnet_tab=5;deck_open_cancel();input(0,0,.016f,0,0);
 ADCHECK(!rumour_ad_phase&&rumour_ad_clock<.1f,"reopening GalacticNet starts a fresh clear browsing interval");
 rumour_ad_sequence=0;rumour_ad_last=-1;unsigned seen=0;
 for(int i=0;i<24;i++){rumour_ad_reset();rumour_ad_clock=8;input(0,0,.016f,0,0);seen|=1u<<rumour_ad_index;}
 ADCHECK(seen==0xffffffu,"all 24 adverts rotate before repeating in a local browsing session");
 rumour_ad_reset();int guards=1,visible_art=1;u64 start,end;double sum=0;
 for(int a=0;a<24;a++){
  rumour_ad_index=a;rumour_ad_phase=2;rect(0,0,W,H,BG);sceRtcGetCurrentTick(&start);galnet_screen();sceRtcGetCurrentTick(&end);
  sum+=(end-start)*1000.0/sceRtcGetTickResolution();char name[48];snprintf(name,sizeof(name),"rumour-ad-%02d.bmp",a);dump_native_bmp(name);
  const RumourAdvert *ad=&rumour_ads[a];for(int yy=0;yy<16;yy++)for(int xx=0;xx<16;xx++){char p=rumour_ad_art[a][yy][xx];if(p=='.')continue;unsigned expected=p=='#'?RGB(31,35,49):p=='B'?ad->paint:p=='A'?ad->accent:p=='L'?RGB(255,216,111):RGB(255,248,229);visible_art&=fb[(116+yy*4)*STRIDE+64+xx*4]==expected;}
  for(int i=0;i<16;i++)guards&=allocation[i]==0xdeadbeefu&&allocation[16+STRIDE*H+i]==0xdeadbeefu;
 }
 ADCHECK(guards,"all 24 native advert renders preserve framebuffer allocation guards");
 ADCHECK(visible_art,"all 24 actual on-screen pictures match the authored pixels inside their own illustration aperture");
 ADCHECK(sum/24<33.34,"mean full website plus advert render stays within the 30fps emulator budget");fprintf(out,"INFO advert render mean %.3f ms\n",sum/24);
 const float open_frames[]={0,.025f,.06f,.105f,.13f,.14f};rumour_ad_index=5;rumour_ad_phase=1;
 for(int i=0;i<6;i++){rumour_ad_age=open_frames[i];rect(0,0,W,H,BG);galnet_screen();char name[48];snprintf(name,sizeof(name),"rumour-ad-open-%d.bmp",i);dump_native_bmp(name);}
 rumour_ad_phase=3;rumour_ad_close_scale=1;const float close_frames[]={0,.025f,.06f,.10f,.11f};
 for(int i=0;i<5;i++){rumour_ad_age=close_frames[i];rect(0,0,W,H,BG);galnet_screen();char name[48];snprintf(name,sizeof(name),"rumour-ad-close-%d.bmp",i);dump_native_bmp(name);}
 rumour_ad_phase=1;rumour_ad_age=.025f;float early=rumour_ad_scale();input(PSP_CTRL_TRIANGLE,PSP_CTRL_TRIANGLE,0,0,0);
 ADCHECK(rumour_ad_phase==3&&fabsf(rumour_ad_scale()-early)<.0001f,"early dismissal shrinks from the currently visible size without jumping");
 for(int id=SFX_AD_OPEN;id<=SFX_AD_CLOSE;id++){
  unsigned phase=0,noise=17;int n=audio_sfx_length(id),peak=0;u64 energy=0;
  for(int age=0;age<n;age++){int sample=abs(audio_sfx_sample(id,age,&phase,&noise));if(sample>peak)peak=sample;energy+=(u64)sample*sample;}
  ADCHECK(n>=4800&&n<=6200&&energy>1000000&&peak<5000&&audio_sfx_sample(id,n,&phase,&noise)==0,"new sweep is short, audible, bounded and ends cleanly");
  unsigned p=0,s=1;audio_sfx_sample(id,0,&p,&s);unsigned first=p;p=0;audio_sfx_sample(id,n-2,&p,&s);
  ADCHECK(id==SFX_AD_OPEN?p>first:p<first,"open whoop rises and close thwip falls in pitch");
 }
 rumour_ad_wav("rumour-ad-whoop.wav",SFX_AD_OPEN);rumour_ad_wav("rumour-ad-thwip.wav",SFX_AD_CLOSE);
 ADCHECK(game.credits==cash,"every advert, dismissal, transition and rotation leaves units unchanged");
 rumour_ad_reset();fprintf(out,"RESULT %d failures\n",failures);fclose(out);fb=oldfb;free(allocation);
#undef ADCHECK
 return 1;
}
