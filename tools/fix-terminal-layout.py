from pathlib import Path
p=Path('src/terminal-review.h');s=p.read_text();at=s.index('static void terminal_panel(')
s=s[:at]+'''/* Native pixel symbols, bounded to a 12 by 10 cell. */
static void terminal_symbol(int x,int y,int location,unsigned ink){
 if(location){line(x+3,y,x+8,y,ink);line(x+1,y+2,x+1,y+5,ink);line(x+10,y+2,x+10,y+5,ink);line(x+2,y+6,x+5,y+9,ink);line(x+9,y+6,x+6,y+9,ink);rect(x+4,y+3,3,3,ink);}
 else {line(x+1,y+7,x+5,y+1,ink);line(x+5,y+1,x+10,y+7,ink);line(x+1,y+7,x+5,y+5,ink);line(x+10,y+7,x+5,y+5,ink);rect(x+4,y+7,3,2,CYAN);}
}
''' +s[at:]
s=s.replace('terminal_ship(268,181,78,42,0,t,state==2);','')
s=s.replace('int sy=182+(int)(t*15)%39;line(270,sy,343,sy,RGB(20,109,124));','terminal_ship(268,181,78,38,0,t,state==2);\n  int sy=182+(int)(t*15)%36;line(270,sy,343,sy,RGB(20,109,124));')
s=s.replace('text_px(356,182,GOLD,"01");text_px(380,182,CYAN,','terminal_symbol(358,181,0,GOLD);text_px(376,182,CYAN,')
s=s.replace('text_px(356,196,GOLD,"02");text_px(380,196,WHITE,','terminal_symbol(358,195,1,GOLD);text_px(376,196,WHITE,')
s=s.replace('text_px(271,224,','text_px(271,222,')
s=s.replace('for(int side=0;side<2;side++){int x=side?341:267;line(x,166,x+6,166,ink);line(x,166,x,172,ink);}','if(style!=2)for(int side=0;side<2;side++){int x=side?341:267;line(x,166,x+6,166,ink);line(x,166,x,172,ink);}')
s=s.replace('line(266,233,456,233,','line(266,232,456,232,').replace('271,237','271,235').replace('269,237','269,235').replace('xx,236,20,8','xx,234,20,8').replace('xx+6,237','xx+6,235').replace('390,237','390,235').replace('428,237','428,235')
p.write_text(s)
