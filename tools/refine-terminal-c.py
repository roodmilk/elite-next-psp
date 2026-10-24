from pathlib import Path
p=Path('src/terminal-review.h');s=p.read_text();a=s.index('  terminal_ship(266,165,100,59');b=s.index('\n }\n for(int side',a)
s=s[:a]+'''  /* Compact dossier: identity, ship scan, numbered records, wallet and slots. */
  rect(266,163,190,13,RGB(36,29,20));rect(266,163,2,13,GOLD);
  text_px(273,166,GOLD,state==1?"CMDR BEN":state==0?"NEW COMMANDER":"UNASSIGNED RECORD");
  if(state==1)text_px(403,166,CYAN,"ONLINE");
  terminal_ship(268,181,78,42,0,t,state==2);
  for(int k=0;k<4;k++){int yy=184+k*10;line(270,yy,343,yy,RGB(10,38,49));}
  int sy=182+(int)(t*15)%39;line(270,sy,343,sy,RGB(20,109,124));
  line(349,182,349,224,RGB(31,75,83));
  text_px(356,182,GOLD,"01");text_px(380,182,CYAN,state==2?"--":"ADDER");
  text_px(356,196,GOLD,"02");text_px(380,196,WHITE,state==1?"LAVE":state==0?"EARTH":"NO DATA");
  rect(356,209,99,14,RGB(20,38,42));rect(356,209,2,14,CYAN);
  text_px(364,213,GOLD,state==1?"1250.0 U":"-- U");
  text_px(271,224,RGB(85,151,164),state==1?"DOCKED":state==0?"STANDBY":"NO LINK");
  if(state==1){line(327,222,343,222,GOLD);pixel(343,221,GOLD);}
''' +s[b:]
s=s.replace('for(int side=0;side<2;side++){int x=side?341:267;', 'for(int side=0;side<2;side++){int x=side?341:267;')
s=s.replace('text_px(267,234,ink,state==0?"X CREATE YOUR COMMANDER":"L < SLOT 1 / 3 > R");','''if(style==2){
  line(266,233,456,233,RGB(48,65,66));
  if(state==0)text_px(271,237,GOLD,"X BEGIN YOUR STORY");
  else {text_px(269,237,GOLD,"L <");for(int i=0;i<3;i++){int xx=309+i*25;rect(xx,236,20,8,i==0?RGB(60,45,20):RGB(12,31,40));text_px(xx+6,237,i==0?GOLD:RGB(62,105,116),"%d",i+1);}text_px(390,237,GOLD,"> R");text_px(428,237,CYAN,"LOAD");}
 }else text_px(267,234,ink,state==0?"X CREATE YOUR COMMANDER":"L < SLOT 1 / 3 > R");''')
# Capture just the chosen design, all three states.
s=s.replace('for(int s=0;s<3;s++)','for(int s=2;s<3;s++)')
p.write_text(s)
