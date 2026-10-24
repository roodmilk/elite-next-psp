from pathlib import Path
p=Path('src/intro-terminal.h')
s=Path('src/terminal-review.h').read_text();ship=s[:s.index('static void terminal_panel')];ship=ship.replace('terminal_ship','intro_terminal_ship').replace('terminal_symbol','intro_terminal_symbol')
body='''
static void intro_terminal(int slot,int saved,int browsing){
 int fresh=!browsing&&intro_choice==1;unsigned ink=GOLD;
 int ship=saved&&!fresh?profile_info[slot].ship:0;if(ship<0||ship>=player_ship_count)ship=0;
 intro_strip_panel(258,145,206,99,ink,225);
 text_px(266,151,ink,"SHIP / COMMAND LINK");
 for(int i=0;i<3;i++)rect(435+i*8,153,4,3,i==(int)(intro_time*2)%3?GOLD:RGB(28,63,72));
 rect(266,163,190,13,RGB(36,29,20));rect(266,163,2,13,GOLD);
 char name[25];intro_name(name,sizeof(name),saved?profile_info[slot].name:"");
 text_px(273,166,GOLD,"%.15s",fresh?"NEW COMMANDER":saved?name:"EMPTY SLOT");
 text_px(411,166,CYAN,saved&&!fresh?"LINK":"----");
 for(int k=0;k<4;k++)line(270,184+k*10,343,184+k*10,RGB(10,38,49));
 intro_terminal_ship(268,181,78,38,ship,intro_time,browsing&&!saved);
 int sy=182+(int)(intro_time*15)%36;line(270,sy,343,sy,RGB(20,109,124));line(349,182,349,224,RGB(31,75,83));
 intro_terminal_symbol(358,181,0,GOLD);text_px(376,182,CYAN,"%.10s",saved&&!fresh?player_ships[ship].name:fresh?"UNASSIGNED":"--");
 int system=saved?profile_info[slot].system:7;if(system<0||system>255)system=7;
 intro_terminal_symbol(358,195,1,GOLD);text_px(376,196,WHITE,"%.10s",fresh?"EARTH":saved?profile_info[slot].opening_stage>0&&profile_info[slot].opening_stage<OP_WAKE?"SOL":game.systems[system].name:"NO DATA");
 rect(356,209,99,14,RGB(20,38,42));rect(356,209,2,14,CYAN);
 if(saved&&!fresh)text_px(364,213,GOLD,"%.9g U",profile_info[slot].credits*.1f);else text_px(364,213,GOLD,"-- U");
 text_px(271,222,RGB(85,151,164),fresh?"STANDBY":saved?"RECORDED":"NO LINK");
 line(266,232,456,232,RGB(48,65,66));
 if(fresh)text_px(271,235,GOLD,"X BEGIN YOUR STORY");
 else {text_px(269,235,GOLD,"L <");for(int i=0;i<3;i++){int xx=309+i*25,selected=i==(browsing?intro_load_slot:slot);rect(xx,234,20,8,selected?RGB(60,45,20):RGB(12,31,40));text_px(xx+6,235,selected?GOLD:RGB(62,105,116),"%d",i+1);}text_px(390,235,GOLD,"> R");text_px(428,235,CYAN,"LOAD");}
}
'''
p.write_text(ship+body)
p=Path('src/intro.h');s=p.read_text();s=s.replace('static void intro_screen(void){','#include "intro-terminal.h"\nstatic int intro_first_choice(void){return intro_primary_slot()>=0?0:1;}\nstatic void intro_screen(void){')
s=s.replace('int browsing=!intro_extras&&intro_choice==2;','if(!intro_extras&&intro_choice<intro_first_choice())intro_choice=intro_first_choice();\n int browsing=!intro_extras&&intro_choice==2;')
s=s.replace('char primary[32];if(has_save)snprintf(primary,sizeof(primary),"CONTINUE: %.17s",display);else snprintf(primary,sizeof(primary),"%s",base[0]);','char primary[32];int primary_slot=intro_primary_slot();char primary_name[25];intro_name(primary_name,sizeof(primary_name),primary_slot>=0?profile_info[primary_slot].name:"");snprintf(primary,sizeof(primary),"CONTINUE: %.17s",primary_name);')
s=s.replace('for(int i=0;i<menu_count;i++){\n  int y=154+i*22,','int first=intro_extras?0:intro_first_choice();\n for(int i=first;i<menu_count;i++){\n  int y=154+(i-first)*22,')
s=s.replace('if(intro_extras&&intro_extras_choice==0){','if(!intro_extras){intro_terminal(slot,has_save,browsing);}\n else if(intro_extras&&intro_extras_choice==0){')
p.write_text(s)
p=Path('src/tutorial-runtime.h');s=p.read_text();s=s.replace('if(pressed&PSP_CTRL_UP)intro_choice=(intro_choice+3)%4;\n  if(pressed&PSP_CTRL_DOWN)intro_choice=(intro_choice+1)%4;','int first=intro_first_choice();if(intro_choice<first)intro_choice=first;\n  if(pressed&PSP_CTRL_UP)intro_choice=intro_choice<=first?3:intro_choice-1;\n  if(pressed&PSP_CTRL_DOWN)intro_choice=intro_choice>=3?first:intro_choice+1;')
p.write_text(s)
