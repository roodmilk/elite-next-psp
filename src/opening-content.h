/* Story correspondence stays in Inbox; the feed has one shared local model. */
static int opening_net_count(void){if(galnet_tab==3)return social_rows();if(galnet_tab==4)return story_inbox_count();return galnet_tab==0?galnet_rows():opening_sol(&game)?4:galnet_rows();}
static const char *opening_net_author(int i){
 static const char *news[]={"EARTH DAILY / 2050","SOL TRAFFIC NEWS","CITY DESK","LOCAL WEATHER"};
 static const char *market[]={"ORCHARD RETAIL / OCH","BLUE ARC AEROSPACE / BAA","NORTHSTAR TRANSIT / NST","KINETIC ELECTRONICS / KEL"};
 static const char *jobs[]={"Accounts Assistant","Orbital Pizza Driver","Lunar Warehouse Operative","Mars Office Administrator"};
 return galnet_tab==0?news[i]:galnet_tab==1?market[i]:jobs[i];
}
static const char *opening_net_body(int i){
 static const char *news[]={"Earth launch permits rise as family-sized ships become more affordable. Orbital controllers remind new pilots to request docking clearance.","Earth orbital station open for engineering, refuelling and cargo. Expect light traffic in the approach lanes.","Town square prepares for this weekend's summer fair. Evening buses will run late. Residents asked to keep landing fields clear.","Clear skies across the countryside. Good visibility over the hills. A lovely afternoon to stretch your legs."};
 static const char *market[]={"18.40 U  +0.12 | Retail and groceries. Earth retail and groceries. Last updated 08:45.","92.10 U  -0.80 | Ship components and aerospace. EARTH EXCHANGE / Last updated 08:45.","31.70 U  +0.24 | Public transport and orbital routes. EARTH EXCHANGE / Last updated 08:45.","46.20 U  +0.05 | Consumer electronics. EARTH EXCHANGE / Last updated 08:45."};
 static const char *jobs[]={"Monday-Friday, 9-5. Spreadsheet experience essential. Interest in space irrelevant. Salary negotiable. 37.5 hours a week. Pension and unlimited office biscuits.","Own ship required. Mileage allowance under review. Must deliver before cheese enters low orbit. Apply with CV and references.","Safety boots required despite reduced gravity. Competitive breaks. Applicants must enjoy boxes. Apply with CV and references.","This is an on-site role. Please stop asking. Relocation costs not included. Apply with CV and references."};
 return galnet_tab==0?news[i]:galnet_tab==1?market[i]:jobs[i];
}
static int opening_net_screen(void){
 if(!opening_exists(&game))return 0;
 int sol=opening_sol(&game);if(sol&&galnet_tab==2)galnet_tab=3;
 if(galnet_tab==3||galnet_tab==4){spacebook_screen(galnet_tab==4);galnet_chrome();return 1;}
 int n=opening_net_count();if(row<0||row>=n)row=0;
 if(sol&&galnet_tab==0){news_screen();galnet_chrome();return 1;}
 if(!sol)return 0;
 if(sol&&galnet_tab==1){
  rect(0,42,W,206,RGB(5,16,18));text(2,6,UI_CYAN,"EARTH EXCHANGE / DELAYED PRICES");
  text(2,8,DIM,"COMPANY                   PRICE       TREND");
  for(int i=0;i<4;i++){int y=82+i*34;if(i==row)rect(8,y-5,464,28,RGB(15,47,47));text_px(16,y,i==row?WHITE:DIM,"%s",opening_net_author(i));text_px(275,y,UI_GOLD,"%s",i==0?"18.40":i==1?"92.10":i==2?"31.70":"46.20");for(int j=0;j<7;j++)line(368+j*12,y+10-(j+i)%4*3,380+j*12,y+10-(j+1+i)%4*3,i==1?RED:UI_CYAN);}
  galnet_chrome();footer("L/R SECTION   UP/DOWN TICKER   O BACK");return 1;
 }
 if(sol&&galnet_tab==5){
  rect(0,42,W,206,BG);text(2,6,UI_CYAN,"EARTH EMPLOYMENT / LOCAL JOBS");int first=row/3*3;
  for(int j=0;j<3&&first+j<n;j++){int i=first+j,y=8+j*5;panel(14,y*8-3,458,34);if(i==row)rect(14,y*8-3,3,34,UI_GOLD);galnet_avatar(20,y*8-1,28,i);text(7,y,i==row?UI_GOLD:UI_CYAN,"%s",opening_net_author(i));text(7,y+2,WHITE,"%.50s",opening_net_body(i));}
  galnet_chrome();footer("L/R SECTION   UP/DOWN JOB   O BACK");return 1;
 }
 return 0;
}
static int opening_net_input(unsigned pressed){
 if(page!=GALNET||!opening_exists(&game))return 0;
 if(opening_net_detail){spacebook_actions(pressed);return 1;}
 if(pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER)){
  int dir=(pressed&PSP_CTRL_RTRIGGER)?1:-1;galnet_tab=(galnet_tab+6+dir)%6;
  if(opening_sol(&game)&&galnet_tab==2)galnet_tab=(galnet_tab+6+dir)%6;row=0;inbox_message_page=0;inbox_last_row=-1;
 }
 int n=opening_net_count();if(n<1)n=1;
 if(pressed&PSP_CTRL_DOWN){row=(row+1)%n;inbox_message_page=0;}
 if(pressed&PSP_CTRL_UP){row=(row+n-1)%n;inbox_message_page=0;}
 if(galnet_tab==3&&(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT))){row+=(pressed&PSP_CTRL_RIGHT)?10:-10;if(row<0)row=0;if(row>=n)row=n-1;}
 if(galnet_tab==3||galnet_tab==4)spacebook_actions(pressed);
 if(pressed&PSP_CTRL_CIRCLE)change_page(HOME);
 return 1;
}
