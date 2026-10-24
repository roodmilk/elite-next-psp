/* Docked legal terminal. Warrants remain local to their issuing system. */
static int law_call_open=0,law_choice=0;
static int law_level_points(int points){return points>0?(points>=25?5:1+(points-1)/5):0;}
static int law_fine_system(int sys){int level=law_level_points(game.wanted[sys]);return (level>0?level:1)*500;}
static int law_count(void){int n=0;for(int i=0;i<256;i++)if(game.wanted[i]>0)n++;return n;}
static int law_system_at(int at){for(int i=0,n=0;i<256;i++)if(game.wanted[i]>0){if(n++==at)return i;}return -1;}
static void law_enter(void){law_call_open=law_choice=0;row=0;game.wanted[game.system]=game.legal;}
static const char *law_charge_name(int sys){unsigned c=game.crime_record[sys]&255;if(c&CRIME_LAW_DESTRUCTION)return "PATROL DESTRUCTION";if(c&CRIME_DESTRUCTION)return "VESSEL DESTRUCTION";if(c&CRIME_LAW_ASSAULT)return "ASSAULT ON LAW";if(c&CRIME_ASSAULT)return "VESSEL ASSAULT";if(c&CRIME_ESCAPE)return "FLEEING A STOP";if(c&CRIME_REFUSAL)return "INSPECTION REFUSAL";if(c&CRIME_CARGO)return "RESTRICTED CARGO";return "ARCHIVED WARRANT";}
static void law_input(unsigned pressed){
 int n=law_count();if(law_call_open){if(pressed&(PSP_CTRL_UP|PSP_CTRL_DOWN))law_choice=1-law_choice;if(pressed&PSP_CTRL_CIRCLE){law_call_open=0;return;}if(pressed&PSP_CTRL_CROSS){if(law_choice){law_call_open=0;return;}if(police_pay_desk(&game)){law_call_open=0;row=0;} }return;}
 if(pressed&PSP_CTRL_CIRCLE){menu_back();return;}if(n&&pressed&PSP_CTRL_UP)row=(row+n-1)%n;if(n&&pressed&PSP_CTRL_DOWN)row=(row+1)%n;
 if(pressed&PSP_CTRL_CROSS){int sys=law_system_at(row);if(sys<0){message(&game,"No warrants are recorded against this commander.");return;}if(sys!=game.system){char note[96];snprintf(note,sizeof(note),"This warrant is local. Dock in %s to settle it.",game.systems[sys].name);message(&game,note);return;}law_call_open=1;law_choice=0;game.cue=SFX_TALK;}
}
static void law_screen(void){
 header("LAW / WARRANT LEDGER");unsigned metal=RGB(17,29,39),edge=RGB(48,73,81),lawblue=RGB(82,155,214);rect(8,32,464,214,metal);rect(8,32,464,2,lawblue);
 for(int i=0;i<6;i++){int x=14+i*90;rect(x,38,3,3,edge);pixel(x+1,39,WHITE);}text_px(22,45,lawblue,"GALACTIC CO-OPERATIVE / LOCAL JURISDICTION LINK");
 int n=law_count();if(law_call_open){int fine=police_fine(&game);rect(28,70,424,132,RGB(8,20,30));rect(28,70,424,2,lawblue);draw_portrait(42,86,58,58,game.system,LAW);text_px(116,84,lawblue,"INCOMING / %s LOCAL LAW",game.systems[game.system].name);text_wrap(15,13,39,4,WHITE,"Commander, your local warrant is ready for settlement. Payment clears this jurisdiction's record immediately.",0);text_px(116,150,UI_GOLD,"FINE %.1f U",fine*.1f);for(int i=0;i<2;i++){int y=170+i*22;rect(116,y,310,18,law_choice==i?UI_RAISED:metal);text_px(126,y+5,law_choice==i?UI_GOLD:WHITE,i?"END CALL":"PAY FINE / CLEAR WARRANT");}footer("UP/DOWN CHOOSE   X CONFIRM   O END CALL");return;}
 rect(20,62,218,152,RGB(8,20,30));rect(246,62,214,152,RGB(8,20,30));text_px(30,70,UI_MUTED,"SYSTEM WARRANTS");text_px(256,70,UI_MUTED,"SELECTED RECORD");
 if(!n){text_px(38,112,RGB(82,214,191),"RECORD CLEAR");text_wrap(5,17,22,4,WHITE,"No system currently holds an active warrant for this commander.",0);text_px(260,116,lawblue,"SAFE FLIGHT, COMMANDER");footer("O BACK");return;}
 int first=row-3;if(first<0)first=0;if(first>n-7)first=n-7;if(first<0)first=0;
 for(int i=first;i<n&&i<first+7;i++){int sys=law_system_at(i),y=86+(i-first)*17;if(i==row){rect(26,y-2,204,15,UI_RAISED);rect(26,y-2,3,15,UI_GOLD);}text_px(34,y,i==row?UI_GOLD:WHITE,"%.15s",game.systems[sys].name);text_px(176,y,RED,"%d/5",law_level_points(game.wanted[sys]));}
 int sys=law_system_at(row),fine=law_fine_system(sys);text_px(258,90,UI_GOLD,"%.22s",game.systems[sys].name);text_px(258,108,RED,"WANTED %d / 5",law_level_points(game.wanted[sys]));text_px(258,124,WHITE,"%s",law_charge_name(sys));text_px(258,140,UI_GOLD,"FINE %.1f U",fine*.1f);text_wrap(32,20,24,3,UI_MUTED,sys==game.system?"Local channel available. Call law to settle this record.":"Remote jurisdiction. Travel there and dock before payment.",0);
 rect(258,184,184,22,sys==game.system?UI_RAISED:metal);text_px(270,191,sys==game.system?lawblue:UI_MUTED,sys==game.system?"CALL LOCAL LAW":"CHANNEL UNAVAILABLE");footer("UP/DOWN WARRANT   X CALL / INFO   O BACK");
}
