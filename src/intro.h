static float intro_time=0;
static int intro_choice=0;
static int intro_load_slot=0;
static int intro_extras=0,intro_extras_choice=0;
#include "intro-movie.h"
static float intro_menu_age=-1;
static int intro_boot_pending=1,intro_wait_release=0;
static unsigned intro_mix(unsigned from,unsigned to,int amount){
 int inverse=256-amount;
 return RGB((((from&255)*inverse+(to&255)*amount)>>8),(((((from>>8)&255)*inverse+((to>>8)&255)*amount)>>8)),(((((from>>16)&255)*inverse+((to>>16)&255)*amount)>>8)));
}
static void intro_enter(void){
 /* Setup creates a hidden story greeting. Never play its queued comms blip
  * over the title, including when Select replays the intro. */
 game.cue=SFX_NONE;game.voice_time=game.message_time=0;
 intro_load_slot=commander_active_slot>=0&&commander_active_slot<3?commander_active_slot:0;
 intro_movie_attempted=0;intro_choice=0;intro_extras=0;intro_extras_choice=0;intro_wait_release=1;
 if(intro_boot_pending){intro_time=0;intro_menu_age=-1;intro_boot_pending=0;}
 else {intro_time=40;intro_menu_age=1;}
}
static void intro_leave(void){intro_movie_close();intro_movie_attempted=0;}
static void intro_suspend(void){intro_movie_close();intro_movie_attempted=0;}
static void intro_tick(float dt){
 intro_time+=dt;
 if(intro_time>=36.34f&&intro_menu_age<0)intro_menu_age=0;
 if(intro_menu_age>=0)intro_menu_age=fminf(1,intro_menu_age+dt);
}
static int intro_gate(unsigned pressed,unsigned held){
 if(intro_menu_age<0){if(pressed&PSP_CTRL_CROSS){intro_menu_age=0;intro_wait_release=1;}return 1;}
 if(!(held&PSP_CTRL_CROSS))intro_wait_release=0;
 return intro_menu_age<.8f||intro_wait_release;
}
static void intro_alpha_text_at(const char *s,int x,int y,unsigned ink,float opacity){
 int alpha=(int)(fminf(1,fmaxf(0,opacity))*256);
 for(int n=0;s[n];n++){
  int ch=(unsigned char)s[n];if(ch<32||ch>126)ch='?';
  for(int yy=0;yy<8;yy++)for(int xx=0;xx<8;xx++)if(font8[ch-32][yy]&(1u<<(7-xx))){int px=x+n*8+xx,py=y+yy;if(px>=0&&px<W&&py>=0&&py<H)fb[py*STRIDE+px]=intro_mix(fb[py*STRIDE+px],ink,alpha);}
 }
}
static void intro_alpha_text(const char *s,int y,unsigned ink,float opacity){
 intro_alpha_text_at(s,(W-(int)strlen(s)*8)/2,y,ink,opacity);
}
static float intro_present_opacity(float time){
 return fminf(1,fmaxf(0,time))*fminf(1,fmaxf(0,(3.2f-time)/.8f));
}
#define INTRO_PRESENT_TITLE "CuLa Games presents..."
static void intro_present(float opacity){
 /* Two native 16x16 tiles: only Cu and La have coloured backgrounds.
  * Blend the tiles and lettering together over the moving cinematic. */
 int x=(W-(int)strlen(INTRO_PRESENT_TITLE)*8)/2,y=113,amount=(int)(fminf(1,fmaxf(0,opacity))*256);
 for(int yy=y-4;yy<y+12;yy++)for(int xx=x;xx<x+32;xx++){
  unsigned ink=xx<x+16?RGB(195,41,51):RGB(42,77,184);
  fb[yy*STRIDE+xx]=intro_mix(fb[yy*STRIDE+xx],ink,amount);
 }
 intro_alpha_text_at("CuLa",x,y,WHITE,opacity);
 intro_alpha_text_at(INTRO_PRESENT_TITLE+4,x+32,y,GOLD,opacity);
}
static void intro_logo(float alpha){
 int amount=(int)(fminf(1,fmaxf(0,alpha))*256);if(amount<=0)return;
 for(int y=0;y<53;y++)for(int x=0;x<236;x++){
  unsigned p=next_logo[(y*72/53)*320+x*320/236];if(!(p&0x8000))continue;
  int r=(p>>10)&31,g=(p>>5)&31,b=p&31;unsigned ink=RGB((r<<3)|(r>>2),(g<<3)|(g>>2),(b<<3)|(b>>2));int at=(y+15)*STRIDE+x+122;
  if(intro_menu_age>=0){
   float beam=fmodf(intro_time*.24f,1.f)*300-32;float dist=fabsf(x-beam);int shine=dist<18?(int)((1-dist/18)*125):0;
   if(shine)ink=intro_mix(ink,RGB(235,255,255),shine);
   if(x%2==0&&y%2==0){int glow=10+(int)(7*(.5f+.5f*sinf(intro_time*2)));for(int side=-1;side<=1;side+=2){int q=at+side*2*STRIDE;if(q>=0&&q<STRIDE*H)fb[q]=intro_mix(fb[q],RGB(40,160,210),glow);}}
  }
  fb[at]=intro_mix(fb[at],ink,amount);
 }
}
static void intro_strip_panel(int x,int y,int w,int h,unsigned edge,int alpha){
 for(int yy=y;yy<y+h&&yy<H;yy++)for(int xx=x;xx<x+w&&xx<W;xx++)if(xx>=0&&yy>=0)fb[yy*STRIDE+xx]=intro_mix(fb[yy*STRIDE+xx],RGB(3,14,25),alpha);
 line(x,y,x+w-1,y,edge);line(x,y+h-1,x+w-1,y+h-1,RGB(27,65,78));
}
static int intro_primary_slot(void){
 int active=commander_active_slot;if(active>=0&&active<3&&(profile_info[active].state==1||profile_info[active].state==3))return active;
 for(int i=0;i<3;i++)if(profile_info[i].state==1||profile_info[i].state==3)return i;
 return -1;
}
static void intro_name(char *out,int cap,const char *name){
 int word=1,n=0;for(;name&&name[n]&&n<cap-1;n++){unsigned char c=name[n];if(c>='A'&&c<='Z')c+='a'-'A';if(word&&c>='a'&&c<='z')c-='a'-'A';out[n]=(char)c;word=c==' '||c=='-'||c=='\'';}out[n]=0;
}
static void intro_frame(int x,int y,int w,int h,unsigned edge){
 rect(x,y,w,h,RGB(4,13,24));rect(x,y,w,1,edge);rect(x,y+h-1,w,1,RGB(37,65,78));rect(x,y,1,h,RGB(37,65,78));rect(x+w-1,y,1,h,RGB(37,65,78));
}
static void intro_ship_icon(int cx,int cy,int ship){
 unsigned hull=RGB(174,183,180),light=RGB(242,191,78),shade=RGB(55,67,73),engine=RGB(63,201,223);int style=ship%4,wing=7+(style&1)*2;
 /* Compact authored top view: recognisable at 1:1 PSP scale and varied by
  * hull family without pretending that this tiny card is a 3D viewport. */
 rect(cx-22,cy-2,42,5,shade);rect(cx-17,cy-3,37,7,hull);
 for(int i=0;i<wing;i++){line(cx-12+i,cy-4-i,cx+4+i,cy-4-i,i<2?light:hull);line(cx-12+i,cy+4+i,cx+4+i,cy+4+i,i<2?light:hull);}
 rect(cx+20,cy-1,5,3,light);pixel(cx+25,cy,light);rect(cx-21,cy-1,4,3,engine);pixel(cx-24,cy,engine);
 rect(cx+7,cy-1,8,2,RGB(35,99,119));if(style==2){rect(cx-15,cy-5,4,11,shade);}else if(style==3){rect(cx-5,cy-7,4,15,light);}
}
#include "intro-terminal.h"
static int intro_first_choice(void){return intro_primary_slot()>=0?0:1;}
static void intro_screen(void){
 if(!intro_profile_ready){profile_from_intro=1;profile_refresh();profile_from_intro=0;intro_profile_ready=1;}
 if(!intro_extras&&intro_choice<intro_first_choice())intro_choice=intro_first_choice();
 int browsing=!intro_extras&&intro_choice==2;
 int slot=browsing?intro_load_slot:intro_primary_slot(),has_save=slot>=0&&(profile_info[slot].state==1||profile_info[slot].state==3),tutorial_saved=profile_file_exists("tutorial.sav");char display[25];
 if(has_save)intro_name(display,sizeof(display),profile_info[slot].name);else snprintf(display,sizeof(display),"New pilot");
 int film=intro_movie_draw(intro_time);
 if(!film)draw_next_art(intro_backdrop,INTRO_BACKDROP_W,INTRO_BACKDROP_H,0,0,W,H);
 if(intro_menu_age<0&&intro_time<7){
  if(intro_time<3.2f)intro_present(intro_present_opacity(intro_time));
  else {float a=fminf(1,(intro_time-3.2f)/.8f)*fminf(1,(7-intro_time)/.8f);intro_alpha_text("A re-imagining of",103,WHITE,a);intro_alpha_text("David Braben and Ian Bell's",117,WHITE,a);intro_alpha_text("seminal Elite game",131,WHITE,a);}
 }
 /* An early X reveals the title with the menu, without jumping the film. */
 intro_logo(fmaxf((intro_time-27)/1.5f,intro_menu_age/.8f));
 if(intro_menu_age<0)return;
 intro_strip_panel(258,149,206,95,CYAN,210);
 const char *base[]={has_save?"":"BEGIN NEW COMMANDER","NEW COMMANDER","LOAD COMMANDER","EXTRAS"};
 const char *extras[]={tutorial_saved?"CONTINUE FIRST LIGHT":"FIRST LIGHT TUTORIAL","ERL RACE 1 TEST"};
 char primary[32];int primary_slot=intro_primary_slot();char primary_name[25];intro_name(primary_name,sizeof(primary_name),primary_slot>=0?profile_info[primary_slot].name:"");snprintf(primary,sizeof(primary),"CONTINUE: %.17s",primary_name);
 int menu_count=intro_extras?2:4;
 int first=intro_extras?0:intro_first_choice();
 for(int i=first;i<menu_count;i++){
  int y=154+(i-first)*22,selected=i==(intro_extras?intro_extras_choice:intro_choice);const char *label=intro_extras?extras[i]:(i?base[i]:primary);
  float reveal=fminf(1,fmaxf(0,(intro_menu_age-i*.075f)/.32f));
  int slide=(int)((1-reveal)*(1-reveal)*26);y+=slide;
  intro_strip_panel(20,y-4,228,20,selected?CYAN:RGB(30,72,83),selected?225:180);
  if(selected){
   int pulse=90+(int)(45*(.5f+.5f*sinf(intro_time*3)));
   for(int d=1;d<=3;d++){int alpha=pulse/(d*2);for(int xx=21;xx<247;xx++){int yy=y-4-d;if(yy>=0)fb[yy*STRIDE+xx]=intro_mix(fb[yy*STRIDE+xx],CYAN,alpha);yy=y+15+d;if(yy<H)fb[yy*STRIDE+xx]=intro_mix(fb[yy*STRIDE+xx],CYAN,alpha);}}
   int sweep=24+(int)(fmodf(intro_time*.32f,1.f)*215);line(sweep,y-4,sweep+12<247?sweep+12:247,y-4,WHITE);
   rect(20,y-4,2,20,GOLD);
  }
  text_px(33,y,selected?GOLD:WHITE,"%s",label);
  if(selected){int shift=(int)(sinf(intro_time*4)*1.5f);line(24+shift,y+2,27+shift,y+5,GOLD);line(27+shift,y+5,24+shift,y+8,GOLD);}
 }
 if(intro_extras)text_px(29,224,UI_MUTED,"O  BACK");
 if(!intro_extras){intro_terminal(slot,has_save,browsing);}
 else if(intro_extras&&intro_extras_choice==0){
  text_px(273,160,GOLD,"FIRST LIGHT");text_px(273,181,CYAN,"FLIGHT LICENCE");
  text_px(273,201,WHITE,"REPLAY THE TUTORIAL");text_px(273,224,UI_MUTED,tutorial_saved?"CHECKPOINT AVAILABLE":"START FROM LESSON 1");
 }else if(intro_extras&&intro_extras_choice==1){
  text_px(273,160,GOLD,"ERL / RACE 1");text_px(273,181,CYAN,"NEON INTERCHANGE");
  text_px(273,201,WHITE,"QUICK RACE TEST");text_px(273,224,UI_MUTED,"PRACTICE / NO SAVE");
 }else if(browsing&&!has_save){
  text_px(273,160,GOLD,"SAVE SLOT %d / 3",intro_load_slot+1);text_px(273,184,WHITE,"EMPTY SLOT");text_px(273,204,UI_MUTED,"NO COMMANDER SAVED");text_px(273,225,CYAN,"L / R SWITCH SLOT");
 }else if(has_save){
  int system=profile_info[slot].system,ship=profile_info[slot].ship;if(system<0||system>255)system=7;if(ship<0||ship>=player_ship_count)ship=0;
  draw_commander_portrait(266,157,58,profile_info[slot].portrait);
  text_px(332,157,GOLD,"%.16s",display);text_px(332,174,CYAN,"%.16s",profile_info[slot].opening_stage>0&&profile_info[slot].opening_stage<OP_WAKE?"SOL / EARTH":game.systems[system].name);text_px(332,190,WHITE,"%.15s",player_ships[ship].name);text_px(332,207,GOLD,"%.1f U",profile_info[slot].credits*.1f);
  if(browsing)text_px(268,225,CYAN,"L < SLOT %d / 3 > R",intro_load_slot+1);else intro_ship_icon(423,227,ship);
 }else{
  draw_commander_portrait(266,157,58,portrait_seeded(7,EXPLORERS,0));text_px(332,157,GOLD,"NEW PILOT");text_px(332,174,CYAN,"EARTH / 2050");text_px(332,190,WHITE,"CHOOSE A SHIP");text_px(332,207,GOLD,"FIRST FLIGHT");
  text_px(268,224,UI_MUTED,"NO COMMANDER SAVE");
 }
 rect(0,248,W,24,RGB(7,15,25));text_px(8,257,DIM,"D-PAD");button_icon(58,253,'X',CYAN);text_px(74,257,WHITE,"OPEN");if(intro_extras){button_icon(124,253,'O',CYAN);text_px(140,257,WHITE,"BACK");}else{button_icon(124,253,'S',CYAN);text_px(140,257,WHITE,"MULTIPLAYER");button_icon(246,253,'E',CYAN);text_px(286,257,WHITE,"REPLAY INTRO");}
 if(intro_menu_age<.8f){
  int a=(int)(intro_menu_age/.8f*256);
  for(int y=149;y<H;y++)for(int x=0;x<W;x++){
   unsigned background;if(film)background=intro_movie_pixels[y*W+x];else {unsigned p=intro_backdrop[y*W+x];int r=(p>>10)&31,g=(p>>5)&31,b=p&31;background=RGB((r<<3)|(r>>2),(g<<3)|(g>>2),(b<<3)|(b>>2));}
   fb[y*STRIDE+x]=intro_mix(background,fb[y*STRIDE+x],a);
  }
 }
}
