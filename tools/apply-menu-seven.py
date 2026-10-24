from pathlib import Path
p=Path('src/intro.h');s=p.read_text()
s=s.replace('static int intro_choice=0;','static int intro_choice=0;\nstatic int intro_load_slot=0;')
s=s.replace('intro_movie_attempted=0;intro_choice=0;','intro_load_slot=commander_active_slot>=0&&commander_active_slot<3?commander_active_slot:0;\n intro_movie_attempted=0;intro_choice=0;')
s=s.replace('int slot=intro_primary_slot(),has_save=slot>=0,tutorial_saved=', 'int browsing=!intro_extras&&intro_choice==2;\n int slot=browsing?intro_load_slot:intro_primary_slot(),has_save=slot>=0&&(profile_info[slot].state==1||profile_info[slot].state==3),tutorial_saved=')
s=s.replace('intro_frame(16,149,236,95,GOLD);intro_frame(258,149,206,95,CYAN);','intro_strip_panel(258,149,206,95,CYAN,210);')
a=s.index('  if(selected){rect(20,y-3,228,19,RGB(12,54,72));')
b=s.index('\n }',a)
s=s[:a]+'''  float reveal=fminf(1,fmaxf(0,(intro_menu_age-i*.075f)/.32f));
  int slide=(int)((1-reveal)*(1-reveal)*26);y+=slide;
  intro_strip_panel(20,y-4,228,20,selected?CYAN:RGB(30,72,83),selected?225:180);
  if(selected){
   int pulse=90+(int)(45*(.5f+.5f*sinf(intro_time*3)));
   for(int d=1;d<=3;d++){int alpha=pulse/(d*2);for(int xx=21;xx<247;xx++){int yy=y-4-d;if(yy>=0)fb[yy*STRIDE+xx]=intro_mix(fb[yy*STRIDE+xx],CYAN,alpha);yy=y+15+d;if(yy<H)fb[yy*STRIDE+xx]=intro_mix(fb[yy*STRIDE+xx],CYAN,alpha);}}
   int sweep=24+(int)(fmodf(intro_time*.32f,1.f)*215);line(sweep,y-4,sweep+12<247?sweep+12:247,y-4,WHITE);
   rect(20,y-4,2,20,GOLD);
  }
  text_px(33,y,selected?GOLD:WHITE,"%s",label);
  if(selected){int shift=(int)(sinf(intro_time*4)*1.5f);line(24+shift,y+2,27+shift,y+5,GOLD);line(27+shift,y+5,24+shift,y+8,GOLD);}''' +s[b:]
s=s.replace('}else if(has_save){','}else if(browsing&&!has_save){\n  text_px(273,160,GOLD,"SAVE SLOT %d / 3",intro_load_slot+1);text_px(273,184,WHITE,"EMPTY SLOT");text_px(273,204,UI_MUTED,"NO COMMANDER SAVED");text_px(273,225,CYAN,"L / R SWITCH SLOT");\n }else if(has_save){')
s=s.replace('intro_ship_icon(423,227,ship);','if(browsing)text_px(268,225,CYAN,"L < SLOT %d / 3 > R",intro_load_slot+1);else intro_ship_icon(423,227,ship);')
s=s.replace('static int intro_primary_slot(void){','''static void intro_strip_panel(int x,int y,int w,int h,unsigned edge,int alpha){
 for(int yy=y;yy<y+h&&yy<H;yy++)for(int xx=x;xx<x+w&&xx<W;xx++)if(xx>=0&&yy>=0)fb[yy*STRIDE+xx]=intro_mix(fb[yy*STRIDE+xx],RGB(3,14,25),alpha);
 line(x,y,x+w-1,y,edge);line(x,y+h-1,x+w-1,y+h-1,RGB(27,65,78));
}
static int intro_primary_slot(void){''')
s=s.replace('fb[at]=intro_mix(fb[at],ink,amount);','''if(intro_menu_age>=0){
   float beam=fmodf(intro_time*.24f,1.f)*300-32;float dist=fabsf(x-beam);int shine=dist<18?(int)((1-dist/18)*125):0;
   if(shine)ink=intro_mix(ink,RGB(235,255,255),shine);
   if(x%2==0&&y%2==0){int glow=10+(int)(7*(.5f+.5f*sinf(intro_time*2)));for(int side=-1;side<=1;side+=2){int q=at+side*2*STRIDE;if(q>=0&&q<STRIDE*H)fb[q]=intro_mix(fb[q],RGB(40,160,210),glow);}}
  }
  fb[at]=intro_mix(fb[at],ink,amount);''')
p.write_text(s)
p=Path('src/tutorial-runtime.h');s=p.read_text().replace('if(intro_extras){\n   if(pressed','if(intro_extras){\n   intro_tick(dt);\n   if(pressed',1)
s=s.replace('if(pressed&PSP_CTRL_CROSS){\n   if(intro_choice==0)', '''if(intro_choice==2){
   if(pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_LEFT))intro_load_slot=(intro_load_slot+2)%3;
   if(pressed&(PSP_CTRL_RTRIGGER|PSP_CTRL_RIGHT))intro_load_slot=(intro_load_slot+1)%3;
  }
  if(pressed&PSP_CTRL_CROSS){
   if(intro_choice==0)''',1)
s=s.replace('else if(intro_choice==2)profile_open_load();','else if(intro_choice==2){int selected_slot=intro_load_slot;profile_open_load();profile_slot=selected_slot;}')
p.write_text(s)
