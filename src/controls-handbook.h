/* Native 480x272 pilot's handbook. No scaled artwork or alternate font. */
enum { HB_STEER,HB_SPEED,HB_BOOST,HB_BRAKE,HB_ROLL,HB_FIRE,HB_TOOLS,HB_POWER,
 HB_TARGET,HB_CATEGORY,HB_CONTACT,HB_FRONT,HB_LOCK,HB_CANCEL,HB_HAIL,HB_ROUTE,
 HB_COMMS,HB_DOCK,HB_WORLD,HB_ENTER,HB_BOARD,HB_LAUNCH,HB_OUTSIDE,HB_BACK,
 HB_DECK,HB_TABS,HB_CONFIRM,HB_HUD,HB_CAMERA,HB_ANALOG,HB_LOOK,HB_WALK,
 HB_STRAFE,HB_JUMP,HB_SCAN,HB_SITE,HB_MAP,HB_ROVER };
typedef struct {const char *label,*keys,*instruction;int picture;} HandbookLesson;
static const HandbookLesson handbook_lessons[5][8]={
 {{"Steer","NP","Use the nub or D-pad to steer.",HB_STEER},
  {"Speed up","r","Hold R to accelerate. Hold L to slow down.",HB_SPEED},
  {"Boost","r","Double press R + keep it held.",HB_BOOST},
  {"Hard brake","l","Double press L to brake hard at speed.",HB_BRAKE},
  {"Roll","l","Hold L + D-pad Left or Right to roll.",HB_ROLL},
  {"Fire laser","X","Hold X to fire the fitted gun immediately.",HB_FIRE},
  {"Ship tools","O","Hold O: L missiles; Triangle flares; Up scan; Down heat sink; Left tractor; Right next gun. Release; tap O uses tool.",HB_TOOLS},
  {"Power banks","A","Hold Start. Left/Right selects a bank; Up/Down changes its power.",HB_POWER}},
 {{"Target computer","S","Hold Square to open the computer; release to close. Flight continues normally.",HB_TARGET},
  {"Target browser","S","Square opens one screen: browse above, current target below.",HB_CONTACT},
  {"Category","P","With the computer open, Left/Right changes category without changing the lock.",HB_CATEGORY},
  {"Choose contact","P","Up/Down highlights detected contacts, nearest first. X confirms the target.",HB_CONTACT},
  {"Confirm target","X","X locks the highlighted contact. Browsing alone never changes your target.",HB_FRONT},
  {"Lock on","r","With the computer open, hold R and press X to confirm and turn towards the contact.",HB_LOCK},
  {"Take control","N","Steer manually to cancel the automatic turn.",HB_CANCEL},
  {"Hail target","T","Tap Triangle to contact your target or answer an incoming call.",HB_HAIL}},
 {{"Comms panel","T","Hold Triangle to open the Comms Panel and pause flight.",HB_COMMS},
  {"Request docking","T","Near a station, open the Comms Panel and choose Request Docking.",HB_DOCK},
  {"Approach world","T","Target a nearby planet and tap Triangle to ask to approach.",HB_WORLD},
  {"Enter or leave","XO","At the planet prompt, press X to enter or Circle to turn away.",HB_ENTER},
  {"Board ship","T","Walk beside your parked ship and press Triangle to board.",HB_BOARD},
  {"Launch","r","When seated on a planet, press R to launch your ship.",HB_LAUNCH},
  {"Step outside","X","When seated on a planet, press X to step outside.",HB_OUTSIDE},
  {"Return","O","Press Circle to return from a menu.",HB_BACK}},
 {{"Command deck","E","Press Select in flight to open the paused Command Deck.",HB_DECK},
  {"Deck categories","lr","On the Command Deck, use L/R or Left/Right to change category.",HB_TABS},
  {"Select / open","PX","Use the D-pad to select. Press X to open or confirm.",HB_CONFIRM},
  {"Go back","O","Press Circle to return to the previous screen.",HB_BACK},
  {"Comms settings","T","Hold Triangle in flight for radio, captions and display settings.",HB_COMMS},
  {"HUD mode","lE","Hold L + press Select in flight to cycle the HUD mode.",HB_HUD},
  {"Camera view","T","Open the Comms Panel and use its camera selector.",HB_CAMERA},
  {"Analog steering","S","On this page, press Square to toggle analog steering. Centre the nub after enabling it.",HB_ANALOG}},
 {{"Look around","N","Use the nub to look. Hold L + use the D-pad to look without walking.",HB_LOOK},
  {"Walk / strafe","P","D-pad Up/Down walks forward or back. Left/Right moves sideways.",HB_WALK},
  {"Surface targets","S","Tap Square to cycle visible targets. Hold Square for the surface target computer.",HB_CONTACT},
  {"Jump / run","r","Tap R for a short jump. Hold R to run.",HB_JUMP},
  {"Scan / Codex","OT","Press Circle to scan nearby targets. Triangle opens the Codex or boards your nearby ship.",HB_SCAN},
  {"Use a site","X","Press X at an interactive site or your parked rover.",HB_SITE},
  {"Local map","A","Press Start to open the local surface map.",HB_MAP},
  {"Drive rover","rl","R accelerates; L brakes. Circle drifts, X boosts. Stop, then Triangle exits.",HB_ROVER}}
};
static int handbook_row[5];
static float handbook_started;
#define HB_INK RGB(53,47,37)
#define HB_PAPER RGB(225,213,178)
#define HB_NAVY (ship_theme_index()?UI_PANEL:RGB(19,33,43))
#define HB_GOLD (ship_theme_index()?UI_EDGE:RGB(203,143,62))
#define HB_TEAL (ship_theme_index()?UI_ACCENT:RGB(108,192,185))
/* Returns consumed bytes, so the native review can prove that copy fits. */
static int handbook_copy(int x,int y,int cols,int rows,unsigned ink,const char *copy,int draw){
 const char *p=copy;int n=0;
 for(int r=0;r<rows&&*p;r++){
  while(*p==' ')p++;int len=(int)strlen(p),take=len<cols?len:cols;
  if(len>cols&&p[take]!=' '){int br=take;while(br>0&&p[br]!=' ')br--;if(br)take=br;}
  if(draw&&take)text_px(x,y+r*10,ink,"%.*s",take,p);
  p+=take;while(*p==' ')p++;n=(int)(p-copy);
 }
 return n;
}
static void handbook_arrow(int x,int y,int dx,int dy,unsigned c){
 line(x,y,x+dx,y+dy,c);int xx=x+dx,yy=y+dy;
 if(dx){line(xx,yy,xx-(dx>0?5:-5),yy-4,c);line(xx,yy,xx-(dx>0?5:-5),yy+4,c);}
 else{line(xx,yy,xx-4,yy-(dy>0?5:-5),c);line(xx,yy,xx+4,yy-(dy>0?5:-5),c);}
}
static void handbook_ship(int x,int y,int flames,int roll){
 unsigned hull=RGB(206,219,199),edge=RGB(107,151,155);
 /* Hand-authored silhouette, 54x36 native pixels. */
 for(int i=0;i<26;i++)line(x-i,y-11+i,x+i,y-11+i,hull);
 line(x,y-11,x-25,y+14,edge);line(x,y-11,x+25,y+14,edge);
 rect(x-4,y-4,8,12,HB_NAVY);rect(x-2,y-3,4,6,HB_TEAL);
 line(x-19,y+10,x-8,y+5,edge);line(x+19,y+10,x+8,y+5,edge);
 for(int i=-1;i<=1;i+=2){rect(x+i*13-3,y+13,6,4,edge);if(flames){int f=flames+(int)(sinf(preview_time*12+i)*3);for(int j=0;j<f;j++)line(x+i*13-2+j/6,y+17+j,x+i*13+2-j/6,y+17+j,j<5?RGB(255,219,133):HB_GOLD);}}
 if(roll){handbook_arrow(x-39,y+12,0,-25,HB_GOLD);handbook_arrow(x+39,y-12,0,25,HB_GOLD);}
}
static void handbook_pad(int x,int y,int horizontal){
 rect(x-6,y-17,13,35,RGB(76,94,99));rect(x-17,y-6,35,13,RGB(76,94,99));
 button_icon(x-5,y-17,'U',WHITE);button_icon(x-5,y+7,'D',WHITE);
 button_icon(x-17,y-5,'L',WHITE);button_icon(x+7,y-5,'R',WHITE);
 if(horizontal){rect(x-18,y-7,2,15,HB_GOLD);rect(x+17,y-7,2,15,HB_GOLD);}else{rect(x-7,y-18,15,2,HB_GOLD);rect(x-7,y+17,15,2,HB_GOLD);}
}
static void handbook_picture(int kind){
 rect(208,91,256,94,HB_NAVY);
 for(int x=216;x<464;x+=16)line(x,95,x,180,RGB(26,44,53));
 for(int y=100;y<185;y+=16)line(212,y,459,y,RGB(26,44,53));
 float t=fmaxf(0,preview_time-handbook_started);int pulse=(int)(t*15)%18;
 if(kind==HB_DOCK||kind==HB_WORLD||kind==HB_ENTER||kind==HB_LAUNCH||kind==HB_BOARD||kind==HB_OUTSIDE){
  if(kind==HB_WORLD||kind==HB_ENTER){for(int r=26;r>0;r--)circle(405,135,r,r>23?HB_TEAL:RGB(63,100+r*2,109));line(386,123,411,114,HB_PAPER);line(397,146,426,139,HB_PAPER);}
  else{rect(382,106,62,58,RGB(90,108,111));rect(388,113,50,44,RGB(9,17,26));rect(393,119,40,2,HB_TEAL);rect(393,151,40,2,HB_TEAL);for(int i=0;i<4;i++){rect(384,112+i*14,2,5,HB_GOLD);rect(440,112+i*14,2,5,HB_GOLD);}}
  handbook_ship(275+(kind==HB_DOCK?pulse:0),134,6,0);handbook_arrow(311,136,57,0,HB_GOLD);
 }else if(kind==HB_DECK||kind==HB_TABS||kind==HB_CONFIRM||kind==HB_BACK||kind==HB_COMMS||kind==HB_HUD||kind==HB_CAMERA||kind==HB_POWER||kind==HB_CATEGORY||kind==HB_CONTACT||kind==HB_MAP){
  rect(258,101,155,74,RGB(97,108,107));rect(263,106,145,61,RGB(8,22,30));rect(268,111,135,9,HB_TEAL);text_px(274,112,HB_NAVY,kind==HB_POWER?"SYS ENG WEP":kind==HB_COMMS?"COMMS PANEL":kind==HB_MAP?"SURFACE MAP":kind==HB_CONTACT?"CONTACTS":"SHIP COMPUTER");
  if(kind==HB_POWER){for(int i=0;i<3;i++){for(int k=0;k<4;k++)rect(278+i*43,154-k*8,24,5,k<2+(i==1)?HB_GOLD:RGB(44,61,66));}}
  else if(kind==HB_MAP){circle(336,143,17,HB_TEAL);line(277,143,394,143,HB_TEAL);line(335,124,335,160,HB_TEAL);rect(352,133,4,4,HB_GOLD);}
  else for(int i=0;i<3;i++){rect(273,126+i*12,123,9,i==(int)(t*.65f)%3?HB_GOLD:RGB(45,66,72));line(277,130+i*12,343+i*12,130+i*12,HB_PAPER);}
  rect(320,175,30,3,RGB(97,108,107));
 }else if(kind==HB_LOOK||kind==HB_WALK||kind==HB_STRAFE||kind==HB_JUMP||kind==HB_SCAN||kind==HB_SITE||kind==HB_ROVER){
  line(215,163,456,163,HB_TEAL);for(int i=0;i<6;i++)line(222+i*45,163,211+i*47,179,RGB(48,71,74));
  int yy=kind==HB_JUMP?-(int)(fabsf(sinf(t*2))*16):0;
  circle(318,118+yy,5,HB_PAPER);line(318,123+yy,318,145+yy,HB_PAPER);line(318,130+yy,307,137+yy,HB_PAPER);line(318,130+yy,329,137+yy,HB_PAPER);line(318,145+yy,307,161+yy,HB_PAPER);line(318,145+yy,329,161+yy,HB_PAPER);
  if(kind==HB_ROVER){rect(358,140,45,12,HB_PAPER);rect(370,133,20,8,HB_TEAL);circle(365,154,6,HB_GOLD);circle(397,154,6,HB_GOLD);}
  else if(kind==HB_SCAN||kind==HB_SITE){rect(386,132,18,29,HB_TEAL);for(int i=0;i<3;i++)line(339,136+i*3,381,125+pulse+i*4,HB_GOLD);}
  else handbook_pad(400,132,kind==HB_STRAFE);
 }else{
  int boost=kind==HB_BOOST,flame=boost?20:kind==HB_SPEED?10:kind==HB_FIRE?4:0;
  handbook_ship(333,132,flame,kind==HB_ROLL);
  if(boost||kind==HB_SPEED||kind==HB_BRAKE){for(int i=0;i<6;i++){int x=230+i*40,y=100+(pulse+i*9)%38;line(x,y,x,y+(boost?27:10),HB_TEAL);}if(kind==HB_BRAKE){line(296,166,370,166,HB_GOLD);line(306,171,360,171,HB_GOLD);}}
  else if(kind==HB_FIRE){line(320,131,312,98,HB_GOLD);line(346,131,354,98,HB_GOLD);rect(309,98,7,2,HB_PAPER);rect(351,98,7,2,HB_PAPER);}
  else if(kind==HB_TARGET||kind==HB_FRONT||kind==HB_LOCK||kind==HB_CANCEL){for(int i=-1;i<=1;i+=2){int x=333+i*35;line(x,107,x-i*9,107,HB_GOLD);line(x,107,x,116,HB_GOLD);line(x,153,x-i*9,153,HB_GOLD);line(x,144,x,153,HB_GOLD);}if(kind==HB_LOCK)line(334,100,334,105,HB_TEAL);}
  else if(kind==HB_HAIL){for(int i=0;i<3;i++){int r=8+i*7+pulse/3;line(375+r,121,380+r,129,HB_TEAL);line(380+r,129,375+r,137,HB_TEAL);}}
  else handbook_pad(260,132,kind==HB_ROLL);
 }
 /* Page-number stamp reads as printed ink rather than another HUD. */
 text_px(422,172,HB_TEAL,"%02d",handbook_row[help_tab]+1);
}
static void help(void){
 static const char *tabs[]={"FLIGHT","TARGETS","DOCKING","MENUS","ON FOOT"};
 if(help_tab<0||help_tab>4)help_tab=0;int selected=handbook_row[help_tab];if(selected<0||selected>7)selected=handbook_row[help_tab]=0;
 rect(0,0,480,272,RGB(75,60,48));rect(0,0,480,24,HB_NAVY);
 text_px(10,8,HB_GOLD,"ELITE: NEXT");text_px(232,8,HB_PAPER,"PILOT'S FIELD GUIDE");
 for(int i=0;i<5;i++){int x=8+i*94;rect(x,30,88,22,i==help_tab?HB_GOLD:HB_NAVY);text_px(x+(88-(int)strlen(tabs[i])*8)/2,37,i==help_tab?HB_NAVY:HB_PAPER,"%s",tabs[i]);}
 rect(8,59,184,183,RGB(213,198,159));rect(200,59,272,183,HB_PAPER);
 for(int y=65;y<239;y+=18){line(194,y,198,y+5,RGB(172,145,104));line(194,y+5,198,y,RGB(172,145,104));}
 text_px(18,71,HB_INK,"SELECT A CONTROL");line(18,83,182,83,RGB(162,139,98));
 for(int i=0;i<8;i++){int y=90+i*18;const HandbookLesson *l=&handbook_lessons[help_tab][i];
  if(i==selected){rect(13,y-2,174,17,HB_NAVY);rect(13,y-2,3,17,HB_GOLD);}
  rect(18,y,30,14,HB_NAVY);
  int n=(int)strlen(l->keys);for(int k=0;k<n;k++)button_icon(19+k*11,y+1,l->keys[k],HB_PAPER);
  text_px(55,y+2,i==selected?HB_PAPER:HB_INK,"%s",l->label);
 }
 const HandbookLesson *l=&handbook_lessons[help_tab][selected];
 text_px(212,71,HB_INK,"%s",l->label);handbook_picture(l->picture);
 handbook_copy(212,195,30,4,HB_INK,l->instruction,1);
 rect(0,248,480,24,HB_NAVY);line(0,248,479,248,HB_GOLD);
 button_icon(8,256,'U',HB_PAPER);button_icon(19,256,'D',HB_PAPER);text_px(33,257,HB_PAPER,"CHOOSE");
 button_icon(94,256,'l',HB_PAPER);button_icon(107,256,'r',HB_PAPER);text_px(122,257,HB_PAPER,"TOPIC");
 button_icon(182,256,'S',HB_PAPER);text_px(197,257,HB_PAPER,"ANALOG %s",analog_enabled?"ON":"OFF");
 button_icon(378,256,'O',HB_PAPER);text_px(394,257,HB_PAPER,"BACK");
}
static void handbook_input(unsigned pressed){
 if(help_tab<0||help_tab>4)help_tab=0;
 if(pressed&PSP_CTRL_CIRCLE){menu_back();return;}
 if(pressed&(PSP_CTRL_LEFT|PSP_CTRL_LTRIGGER|PSP_CTRL_RIGHT|PSP_CTRL_RTRIGGER))help_tab=(help_tab+((pressed&(PSP_CTRL_RIGHT|PSP_CTRL_RTRIGGER))?1:4))%5;
 else if(pressed&PSP_CTRL_UP)handbook_row[help_tab]=(handbook_row[help_tab]+7)%8;
 else if(pressed&PSP_CTRL_DOWN)handbook_row[help_tab]=(handbook_row[help_tab]+1)%8;
 else if(pressed&PSP_CTRL_SQUARE){analog_enabled=!analog_enabled;analog_ready=0;}
 else return;
 handbook_started=preview_time;game.cue=SFX_SELECT;
}
