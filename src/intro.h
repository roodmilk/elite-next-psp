static float intro_time=0;
static int intro_choice=0;
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
static void intro_screen(void){
 if(!intro_profile_ready){profile_from_intro=1;profile_refresh();profile_from_intro=0;intro_profile_ready=1;}
 int slot=intro_primary_slot(),has_save=slot>=0,tutorial_saved=profile_file_exists("tutorial.sav");char display[25];
 if(has_save)intro_name(display,sizeof(display),profile_info[slot].name);else snprintf(display,sizeof(display),"New pilot");
 if(high_contrast)rect(0,0,W,H,RGB(3,8,19));else draw_next_art(intro_backdrop,INTRO_BACKDROP_W,INTRO_BACKDROP_H,0,0,W,H);
 for(int i=0;i<10;i++){unsigned h=(unsigned)(i+9)*2654435761u;int x=20+(h%440),y=8+((h>>12)%128);if(((int)(intro_time*2)+(int)i)%9<2)pixel(x,y,i&1?WHITE:CYAN);}
 draw_next_art(next_logo,320,72,108,84,264,59);
 int glint=108+(int)fmodf(intro_time*25,264);rect(108,143,264,1,RGB(19,74,95));rect(glint,143,3,1,CYAN);
 intro_frame(16,149,236,95,GOLD);intro_frame(258,149,206,95,CYAN);
 const char *base[]={has_save?"":"BEGIN NEW COMMANDER","NEW COMMANDER","LOAD COMMANDER",tutorial_saved?"CONTINUE TUTORIAL":"FIRST FLIGHT TUTORIAL"};
 char primary[32];if(has_save)snprintf(primary,sizeof(primary),"CONTINUE: %.17s",display);else snprintf(primary,sizeof(primary),"%s",base[0]);
 for(int i=0;i<4;i++){
  int y=154+i*22,selected=i==intro_choice;const char *label=i?base[i]:primary;
  if(selected){rect(20,y-3,228,19,RGB(12,54,72));rect(20,y-3,3,19,CYAN);line(20,y-3,248,y-3,CYAN);line(20,y+15,248,y+15,RGB(37,103,120));}
  text_px(29,y,selected?GOLD:WHITE,"%s",label);
  if(selected){line(23,y+3,26,y+6,GOLD);line(26,y+6,23,y+9,GOLD);}
 }
 if(tutorial_saved)text_px(204,224,UI_CYAN,"SAVED");
 if(has_save){
  int system=profile_info[slot].system,ship=profile_info[slot].ship;if(system<0||system>255)system=7;if(ship<0||ship>=player_ship_count)ship=0;
  draw_commander_portrait(266,157,58,profile_info[slot].portrait);
  text_px(332,157,GOLD,"%.16s",display);text_px(332,174,CYAN,"%.16s",game.systems[system].name);text_px(332,190,WHITE,"%.15s",player_ships[ship].name);text_px(332,207,GOLD,"%.1f U",profile_info[slot].credits*.1f);
  intro_ship_icon(423,227,ship);
 }else{
  draw_commander_portrait(266,157,58,portrait_seeded(7,EXPLORERS,0));text_px(332,157,GOLD,"NEW PILOT");text_px(332,174,CYAN,"LAVE");text_px(332,190,WHITE,"ADDER");text_px(332,207,GOLD,"100.0 U");
  text_px(268,224,UI_MUTED,"NO COMMANDER SAVE");
 }
 footer("D-PAD CHOOSE   X OPEN");
 if(game.message_time>0)text(2,30,GOLD,"%.56s",game.message);
}
