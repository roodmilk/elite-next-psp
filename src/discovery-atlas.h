/* Hierarchical record browser. Views enumerate IDs, never materialise the galaxy.
 * Adding a category requires a provider/count/label; navigation is shared.
 * Existing saved (system, body, slot) identities are the source of truth. */
enum { ATLAS_GALAXY=0,ATLAS_SYSTEM=1,ATLAS_WORLD=2,ATLAS_LORE=3,ATLAS_CATEGORY=4,ATLAS_RECORD=5,ATLAS_SIGNALS=6 };
typedef struct {int scope,system,body,category,entry,selection;} AtlasFrame;
static AtlasFrame atlas_stack[8];static int atlas_depth=0,atlas_category=0,atlas_entry=0;
static int atlas_reward_level=0,atlas_reward_system=0;static float atlas_reward_started=-100.f;
static const char *atlas_categories[]={"FLORA","FAUNA","MINERALS","LOGGED SITES"};
static int atlas_logged(int sys,int body);static int atlas_sites_logged(int sys,int body);
static int atlas_bits(unsigned value){int n=0;while(value){n+=(int)(value&1u);value>>=1;}return n;}
static int atlas_world_count(int sys){int n=0;for(int b=1;b<system_body_slots(sys);b++)n+=system_body_type_slot(sys,b)!=SUN;return n;}
static int atlas_progress_percent(int sys){
 int worlds=atlas_world_count(sys),landed=0,field=0,field_total=worlds*(LIFE_COUNT+8);
 for(int b=1;b<system_body_slots(sys);b++)if(system_body_type_slot(sys,b)!=SUN){landed+=(game.landed_planets[sys]&(1u<<(b-1)))!=0;field+=atlas_logged(sys,b)+atlas_sites_logged(sys,b);}
 int score=10000,total=10000; /* Merely charting a visited system is the first weighted record. */
 if(worlds){score+=30000*landed/worlds;total+=30000;if(field_total){score+=30000*field/field_total;total+=30000;}}
 unsigned signals=anomaly_slots_mask(&game,sys)|(game.rift_logged[sys]&15u);int signal_count=atlas_bits(signals);
 if(signal_count){score+=15000*atlas_bits(game.rift_logged[sys]&signals)/signal_count;total+=15000;}
 score+=10000*atlas_bits(game.bounty_claimed[sys]&31u)/BOUNTY_POSTER_COUNT;total+=10000;
 int pct=score*100/total;if(pct<0)pct=0;if(pct>100)pct=100;return pct;
}
static int atlas_claimed_level(int sys){return (game.landed_planets[sys]>>12)&15;}
static void atlas_set_claimed_level(int sys,int level){game.landed_planets[sys]=(uint16_t)((game.landed_planets[sys]&0x0fffu)|((level&15)<<12));}
static int atlas_reward_available(int sys){int earned=atlas_progress_percent(sys)/10;if(earned>10)earned=10;return earned>atlas_claimed_level(sys);}
static const char *atlas_next_action(int sys){
 static char note[40];
 for(int b=1;b<system_body_slots(sys);b++)if(system_body_type_slot(sys,b)!=SUN&&!(game.landed_planets[sys]&(1u<<(b-1)))){snprintf(note,sizeof(note),"LAND ON %.20s",codex_body_name(sys,b));return note;}
 for(int b=1;b<system_body_slots(sys);b++)if(system_body_type_slot(sys,b)!=SUN){int life=atlas_logged(sys,b),sites=atlas_sites_logged(sys,b);if(life<LIFE_COUNT||sites<8){snprintf(note,sizeof(note),"SURVEY %.18s",codex_body_name(sys,b));return note;}}
 int rifts=atlas_bits(anomaly_slots_mask(&game,sys)&~game.rift_logged[sys]);if(rifts){snprintf(note,sizeof(note),"LOG %d SPACE SIGNAL%s",rifts,rifts==1?"":"S");return note;}
 int bounties=BOUNTY_POSTER_COUNT-atlas_bits(game.bounty_claimed[sys]&31u);if(bounties){snprintf(note,sizeof(note),"IDENTIFY %d BOUNTY TARGET%s",bounties,bounties==1?"":"S");return note;}
 return "SYSTEM FULLY SURVEYED";
}
static int atlas_reward_open(void){return atlas_reward_started>-50.f;}
static void atlas_claim_reward(int sys){
 int level=atlas_claimed_level(sys)+1,earned=atlas_progress_percent(sys)/10;if(level>earned||level>10)return;
 atlas_set_claimed_level(sys,level);game.credits+=level*900;atlas_reward_level=level;atlas_reward_system=sys;atlas_reward_started=preview_time;game.cue=SFX_SELECT;
}
static int codex_system_row(void){return codex_system>=0&&codex_system<256?codex_system:game.system;}
static void atlas_reset(void){atlas_depth=0;atlas_category=atlas_entry=0;codex_scope=0;codex_tab=0;codex_system=game.system;codex_body=0;}
static void atlas_open_surface_record(int system,int body,int slot){
 int kind=field_species_kind(system,body,slot),selection=0;
 for(int i=0;i<slot;i++)if(field_species_logged(&game,system,body,i)&&field_species_kind(system,body,i)==kind)selection++;
 atlas_depth=0;
 atlas_stack[atlas_depth++]=(AtlasFrame){ATLAS_GALAXY,system,0,0,0,0};
 atlas_stack[atlas_depth++]=(AtlasFrame){ATLAS_SYSTEM,system,0,0,0,0};
 atlas_stack[atlas_depth++]=(AtlasFrame){ATLAS_WORLD,system,body+1,kind,0,kind};
 atlas_stack[atlas_depth++]=(AtlasFrame){ATLAS_CATEGORY,system,body+1,kind,slot,selection};
 codex_scope=ATLAS_RECORD;codex_tab=0;codex_system=system;codex_body=body+1;atlas_category=kind;atlas_entry=slot;row=0;
}
static int atlas_entry_at(int index){
 int sys=codex_system_row(),body=codex_body-1,n=0;
 if(body<1||body>=BODY_COUNT)return -1;
 if(atlas_category<3){for(int i=0;i<LIFE_COUNT;i++)if(field_species_logged(&game,sys,body,i)&&field_species_kind(sys,body,i)==atlas_category){if(n++==index)return i;}}
 else for(int i=1;i<10;i++)if(field_site_bit(i)&&(game.surface_progress[sys][body]&field_site_bit(i))){if(n++==index)return i;}
 return -1;
}
static int atlas_entry_count(void){int n=0;while(atlas_entry_at(n)>=0)n++;return n;}
static int atlas_category_count(int kind){int saved=atlas_category;atlas_category=kind;int n=atlas_entry_count();atlas_category=saved;return n;}
static int atlas_rift_at(int index){int n=0;for(int i=0;i<ANOMALY_COUNT;i++)if(game.rift_logged[codex_system_row()]&(1u<<i)){if(n++==index)return i;}return -1;}
static int atlas_rift_count(void){int n=0;while(atlas_rift_at(n)>=0)n++;return n;}
static int codex_rows(void){
 if(codex_scope==ATLAS_LORE)return sizeof(milky_way_topics)/sizeof(*milky_way_topics);
 if(codex_scope==ATLAS_GALAXY)return vis_count();
 if(codex_scope==ATLAS_SYSTEM)return codex_system_body_count(codex_system_row())+1;
 if(codex_scope==ATLAS_WORLD){int b=codex_body-1;return codex_body>1&&system_body_type_slot(codex_system_row(),b)!=SUN&&(game.landed_planets[codex_system_row()]&(1u<<(b-1)))?4:1;}
 if(codex_scope==ATLAS_SIGNALS)return atlas_rift_count()+1;
 if(codex_scope==ATLAS_CATEGORY){int n=atlas_entry_count();return n?n:1;}
 return 1;
}
static void atlas_push(int scope){
 if(atlas_depth>=8)return;
 atlas_stack[atlas_depth++]=(AtlasFrame){codex_scope,codex_system,codex_body,atlas_category,atlas_entry,row};
 codex_scope=scope;row=0;
}
static void atlas_back(void){
 if(!atlas_depth){atlas_reset();menu_back();return;}
 AtlasFrame f=atlas_stack[--atlas_depth];codex_scope=f.scope;codex_system=f.system;codex_body=f.body;atlas_category=f.category;atlas_entry=f.entry;row=f.selection;
}
static void atlas_open(void){
 int selection=row;
 if(codex_scope==ATLAS_SIGNALS){int slot=atlas_rift_at(row);atlas_push(slot>=0?7:8);if(slot>=0)atlas_entry=slot;return;}
 if(codex_scope==ATLAS_GALAXY){int sys=vis_sys(row);atlas_push(ATLAS_SYSTEM);codex_system=sys;}
 else if(codex_scope==ATLAS_SYSTEM){
  int body=codex_system_body_at(codex_system_row(),row);if(body<0)atlas_push(ATLAS_SIGNALS);
  else {atlas_push(ATLAS_WORLD);codex_body=body;}
 }else if(codex_scope==ATLAS_WORLD&&codex_body>1&&system_body_type_slot(codex_system_row(),codex_body-1)!=SUN&&(game.landed_planets[codex_system_row()]&(1u<<(codex_body-2)))){atlas_push(ATLAS_CATEGORY);atlas_category=selection;}
 else if(codex_scope==ATLAS_CATEGORY){int id=atlas_entry_at(row);if(id>=0){atlas_push(ATLAS_RECORD);atlas_entry=id;}}
}
static void atlas_input(unsigned pressed){
 if(atlas_reward_open()){if((pressed&PSP_CTRL_CROSS)&&preview_time-atlas_reward_started>1.1f){atlas_reward_started=-100.f;game.cue=SFX_SELECT;}return;}
 int count=codex_rows();
 if(row<0||row>=count)row=0;
 if(pressed&PSP_CTRL_CIRCLE){atlas_back();return;}
 if(pressed&PSP_CTRL_UP)row=(row+count-1)%count;
 if(pressed&PSP_CTRL_DOWN)row=(row+1)%count;
 if(codex_scope==ATLAS_WORLD&&codex_body>1&&(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT)))row^=1;
 if(pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER)){int delta=pressed&PSP_CTRL_RTRIGGER?7:-7;row+=delta;if(row<0)row=0;if(row>=count)row=count-1;}
 if(codex_scope==ATLAS_GALAXY&&(pressed&PSP_CTRL_TRIANGLE)){int sys=vis_sys(row);if(atlas_reward_available(sys)){atlas_claim_reward(sys);return;}}
 if(pressed&PSP_CTRL_CROSS)atlas_open();
 if(pressed)game.cue=SFX_SELECT;
}
static void atlas_chrome(const char *section){
 rect(0,24,W,224,RGB(5,10,24));
 for(int i=0;i<75;i++)pixel((i*83+17)%W,28+(i*47)%217,i%9?RGB(35,62,85):RGB(119,162,184));
 header("DISCOVERY / STAR ATLAS");
 char trail[80];snprintf(trail,sizeof(trail),"GALAXY");
 if(codex_scope){snprintf(trail,sizeof(trail),"GALAXY > %.12s",game.systems[codex_system_row()].name);}
 if(codex_scope==ATLAS_WORLD||codex_scope==ATLAS_CATEGORY||codex_scope==ATLAS_RECORD){size_t n=strlen(trail);snprintf(trail+n,sizeof(trail)-n," > %s",codex_body>1?codex_body_name(codex_system_row(),codex_body-1):codex_body?"STAR":"STATION");}
 text_px(16,34,UI_CYAN,"%.56s",trail);
 text_px(16,48,UI_GOLD,"%s",section);
 footer("UP/DOWN   L/R PAGE   X OPEN   O BACK");
}
static void atlas_scroll(int count,int visible){
 rect(9,68,3,168,UI_EDGE);int size=count<=visible?168:168*visible/count;if(size<7)size=7;
 int y=count>1?68+row*(168-size)/(count-1):68;rect(9,y,3,size,UI_CYAN);
 text_px(16,238,UI_MUTED,"%d / %d",row+1,count);
}
static void atlas_list_row(int index,int first,const char *label,const char *sub){
 int y=68+(index-first)*24,active=row==index;
 if(active){rect(16,y,216,22,RGB(25,52,68));rect(16,y,2,22,UI_CYAN);}
 text_px(24,y+3,active?UI_GOLD:UI_TEXT,"%.25s",label);
 text_px(24,y+13,UI_MUTED,"%.25s",sub);
}
static void atlas_world_disc(int sys,int body,int x,int y,int radius){
 unsigned col,acc;int type;body_tint(sys,body,&col,&acc,&type);
 draw_planet_disc(x,y,radius,col,acc,body_art_seed(sys,body),field_type(sys,body));
}
static int atlas_logged(int sys,int body){int n=0;for(int i=0;i<LIFE_COUNT;i++)n+=field_species_logged(&game,sys,body,i);return n;}
static int atlas_sites_logged(int sys,int body){int n=0;for(int i=1;i<10;i++)if(field_site_bit(i)&&(game.surface_progress[sys][body]&field_site_bit(i)))n++;return n;}
static int atlas_landed_worlds(int sys){int n=0;for(int b=1;b<system_body_slots(sys);b++)if(system_body_type_slot(sys,b)!=SUN&&(game.landed_planets[sys]&(1u<<(b-1))))n++;return n;}
static int atlas_planet_landed(int sys,int body){return body>0&&system_body_type_slot(sys,body)!=SUN&&(game.landed_planets[sys]&(1u<<(body-1)));}
static const char *atlas_body_type_name(int type){return type==SUN?"STAR":type==GAS?"GAS GIANT":type==OCEAN?"OCEAN WORLD":"ROCKY WORLD";}
static void atlas_station_sprite(int sys,int cx,int cy){
 int kind=station_class_for_system(&game,sys);
 if(kind==STATION_POOR)draw_next_art(almanac_station_poor,ALMANAC_POOR_W,ALMANAC_POOR_H,cx-ALMANAC_POOR_W/2,cy-ALMANAC_POOR_H/2,ALMANAC_POOR_W,ALMANAC_POOR_H);
 else if(kind==STATION_RICH)draw_next_art(almanac_station_rich,ALMANAC_RICH_W,ALMANAC_RICH_H,cx-ALMANAC_RICH_W/2,cy-ALMANAC_RICH_H/2,ALMANAC_RICH_W,ALMANAC_RICH_H);
 else draw_next_art(almanac_station_mega,ALMANAC_MEGA_W,ALMANAC_MEGA_H,cx-ALMANAC_MEGA_W/2,cy-ALMANAC_MEGA_H/2,ALMANAC_MEGA_W,ALMANAC_MEGA_H);
}
static void atlas_orbit(int cx,int cy,int rx,int ry,unsigned ink){
 int px=cx+rx,py=cy;for(int i=1;i<=64;i++){float a=i*6.2831853f/64.f;int x=cx+(int)(cosf(a)*rx),y=cy+(int)(sinf(a)*ry);line(px,py,x,y,ink);px=x;py=y;}
}
static void atlas_station_mini(int cx,int cy){
 rect(cx-10,cy-3,21,7,RGB(116,140,150));rect(cx-5,cy-7,11,15,RGB(35,55,68));rect(cx-2,cy-3,5,7,RGB(4,9,15));pixel(cx-8,cy,UI_CYAN);pixel(cx+8,cy,UI_CYAN);
}
static void atlas_system_orrey(int sys){
 int cx=353,cy=126,worlds=atlas_world_count(sys),wi=0,stars=0;
 for(int b=0;b<system_body_slots(sys);b++)stars+=system_body_type_slot(sys,b)==SUN;
 for(int i=0;i<worlds;i++){int rx=18+(worlds>1?77*i/(worlds-1):34),ry=8+(worlds>1?31*i/(worlds-1):14);atlas_orbit(cx,cy,rx,ry,RGB(39,63,73));}
 int si=0;for(int b=0;b<system_body_slots(sys);b++)if(system_body_type_slot(sys,b)==SUN){unsigned col,acc;int type;body_tint(sys,b,&col,&acc,&type);int ox=stars>1?(si-(stars-1)/2.f)*12:0;draw_planet_disc(cx+ox,cy,stars>1?6:10,col,acc,body_art_seed(sys,b),field_type(sys,b));si++;}
 for(int b=1;b<system_body_slots(sys);b++)if(system_body_type_slot(sys,b)!=SUN){int rx=18+(worlds>1?77*wi/(worlds-1):34),ry=8+(worlds>1?31*wi/(worlds-1):14);float a=(float)(body_art_seed(sys,b)%628)/100.f+preview_time*(.035f+.008f*(worlds-wi));int x=cx+(int)(cosf(a)*rx),y=cy+(int)(sinf(a)*ry);unsigned col,acc;int type;body_tint(sys,b,&col,&acc,&type);draw_planet_disc(x,y,worlds>7?2:worlds>4?3:4,col,acc,body_art_seed(sys,b),field_type(sys,b));wi++;}
 atlas_station_mini(414,108);line(414,116,414,120,UI_CYAN);text_px(402,121,UI_MUTED,"HUB");
}
static float atlas_ease(float t){if(t<=0)return 0;if(t>=1)return 1;return t<.5f?4*t*t*t:1-powf(-2*t+2,3)/2;}
static void atlas_reward_overlay(void){
 if(!atlas_reward_open())return;float age=preview_time-atlas_reward_started,t=age/.68f,e=atlas_ease(t);int x=490+(int)((72-490)*e),y=50;
 for(int yy=24;yy<248;yy+=2)rect(0,yy,W,1,RGB(3,7,14));
 panel(x,y,336,182);rect(x,y,336,3,UI_GOLD);
 if(age>.42f)text_px(x+24,y+13,UI_GOLD,"SYSTEM SURVEY MILESTONE");
 if(age>.68f)text_px(x+40,y+33,UI_TEXT,"WELL DONE! YOU'VE EXPLORED");
 if(age>.80f)text_px(x+56,y+47,UI_CYAN,"%d%% OF THIS SOLAR SYSTEM,",atlas_reward_level*10);
 if(age>.92f)text_px(x+32,y+61,UI_TEXT,"KEEP GOING FOR MORE REWARDS!");
 if(age>.98f){circle(x+57,y+109,24,UI_CYAN);for(int i=0;i<10;i++){float a=i*6.2831853f/10.f;int r=i&1?10:21;line(x+57+(int)(cosf(a)*5),y+109+(int)(sinf(a)*5),x+57+(int)(cosf(a)*r),y+109+(int)(sinf(a)*r),UI_GOLD);}text_px(x+104,y+91,UI_TEXT,"REWARD CLAIMED");text_px(x+104,y+107,UI_GOLD,"+%.1f UNITS",atlas_reward_level*90.f);text_px(x+104,y+123,RGB(82,201,147),"EXPLORER RECOGNITION");}
 if(age>.55f&&age<1.35f){float burst=age-.55f;for(int i=0;i<18;i++){float a=i*2.399963f,r=10+burst*(32+i%5*7);int sx=x+168+(int)(cosf(a)*r),sy=y+91+(int)(sinf(a)*r*.58f);if(sx>2&&sx<W-2&&sy>26&&sy<246){unsigned c=i%3?UI_GOLD:UI_CYAN;pixel(sx,sy,c);if(i%4==0){line(sx-2,sy,sx+2,sy,c);line(sx,sy-2,sx,sy+2,c);}}}}
 if(age>1.22f){button_icon(x+82,y+158,'X',UI_CYAN);text_px(x+100,y+160,UI_CYAN,"CONTINUE");}
}
/* Paint the frame first so it cannot cover the coloured progress interior. */
static void atlas_record_bar(int pct,int claimed){
 if(pct<0)pct=0;if(pct>100)pct=100;
 rect(256,201,196,5,UI_EDGE);rect(257,202,194,3,RGB(8,16,29));
 if(pct)rect(257,202,194*pct/100,3,UI_CYAN);
 for(int level=1;level<=10;level++){int x=256+196*level/10;unsigned ink=level<=claimed?UI_GOLD:level<=pct/10?UI_CYAN:UI_EDGE;line(x,198,x,207,ink);line(x-2,198,x,196,ink);line(x,196,x+2,198,ink);}
}
static void atlas_galaxy(void){
 atlas_chrome("VISITED SYSTEMS");int n=vis_count(),first=row/7*7,sys=vis_sys(row);
 for(int i=first;i<first+7&&i<n;i++){int s=vis_sys(i);char sub[40];snprintf(sub,sizeof(sub),s==game.system?"YOU ARE HERE / %d LANDED":"VISITED / %d LANDED",atlas_landed_worlds(s));atlas_list_row(i,first,game.systems[s].name,sub);}
 atlas_scroll(n,7);panel(244,31,228,207);rect(244,31,228,3,UI_CYAN);text_px(256,41,UI_CYAN,"%.12s / SYSTEM DOSSIER",game.systems[sys].name);atlas_system_orrey(sys);
 int pct=atlas_progress_percent(sys),claimed=atlas_claimed_level(sys);text_px(256,188,UI_CYAN,"SYSTEM RECORD");text_px(424,188,UI_GOLD,"%d%%",pct);atlas_record_bar(pct,claimed);
 text_px(256,213,UI_MUTED,"PLANETS");text_px(360,213,UI_TEXT,"%d / %d LANDED",atlas_landed_worlds(sys),atlas_world_count(sys));text_px(256,226,UI_GOLD,"NEXT: %.25s",atlas_next_action(sys));
 if(atlas_reward_available(sys)){int flash=((int)(preview_time*4.f))&1;rect(326,250,146,20,RGB(7,14,25));button_icon(334,256,'T',flash?UI_GOLD:UI_TEXT);text_px(350,258,flash?UI_GOLD:UI_TEXT,"CLAIM REWARDS");}
}
static void atlas_system(void){
 atlas_chrome("SYSTEM DIRECTORY");int sys=codex_system_row(),n=codex_rows(),signal=codex_system_signal_row(sys),first=row<6?0:row-5,y=68;
 for(int i=first;i<first+6&&i<n;i++){
  if(i==signal+1){line(18,y,230,y,UI_EDGE);text_px(24,y+3,UI_CYAN,"PLANETS");y+=15;}
  int b=codex_system_body_at(sys,i);char label[40],sub[40];
  if(b<0){snprintf(label,40,"SPACE SIGNALS");snprintf(sub,40,"%d LOGGED SIGNALS",atlas_rift_count());}
  else if(!b){snprintf(label,40,"ORBITAL STATION");snprintf(sub,40,"CHARTED INFRASTRUCTURE");}
  else {int body=b-1;snprintf(label,40,"%s",codex_body_name(sys,body));if(system_body_type_slot(sys,body)==SUN)snprintf(sub,40,"CHARTED STAR");else if(atlas_planet_landed(sys,body))snprintf(sub,40,"LANDED / %d FIELD RECORDS",atlas_logged(sys,body));else snprintf(sub,40,"UNDISCOVERED");}
  int active=row==i;if(active){rect(16,y,216,22,RGB(25,52,68));rect(16,y,2,22,UI_CYAN);}text_px(24,y+3,active?UI_GOLD:UI_TEXT,"%.25s",label);text_px(24,y+13,UI_MUTED,"%.25s",sub);y+=24;
 }
 atlas_scroll(n,6);panel(244,64,228,174);
 int selected=codex_system_body_at(sys,row);
 if(selected<0){draw_anomaly_icon(348,82,0);text_px(256,116,UI_CYAN,"SPACE SIGNALS");text_px(256,134,UI_TEXT,"LOGGED %d",atlas_rift_count());text_px(256,150,UI_MUTED,"LOCAL RIFT RECORDS");}
 else if(!selected){atlas_station_sprite(sys,356,105);text_px(256,157,UI_CYAN,"STATION CLASS");text_px(256,172,UI_GOLD,"%s",station_class_name(station_class_for_system(&game,sys)));text_px(256,191,UI_TEXT,"TECH LEVEL %d",game.systems[sys].tech);text_px(256,209,UI_TEXT,"DANGER %d OF 5",danger_rating(&game,sys));}
 else {int body=selected-1,type=system_body_type_slot(sys,body),landed=atlas_planet_landed(sys,body);atlas_world_disc(sys,body,354,105,34);text_px(256,151,UI_CYAN,"%s",atlas_body_type_name(type));if(type==SUN){text_px(256,169,UI_TEXT,"CHARTED NAVIGATION STAR");text_px(256,187,UI_MUTED,"DANGER %d OF 5",danger_rating(&game,sys));}else if(!landed){text_px(256,169,UI_GOLD,"UNDISCOVERED");text_wrap(32,23,26,4,UI_TEXT,"Land on planet to get information.",0);}else {text_px(256,169,UI_GOLD,"LANDED");text_px(256,187,UI_TEXT,"%d LIFE / %d SITES",atlas_logged(sys,body),atlas_sites_logged(sys,body));text_px(256,205,UI_MUTED,"SURFACE ARCHIVE OPEN");}}
}
static void atlas_world(void){
 int sys=codex_system_row(),body=codex_body-1;
 atlas_chrome(codex_body>1?codex_body_name(sys,body):codex_body?"PRIMARY STAR":"ORBITAL STATION");
 int is_star=codex_body>0&&system_body_type_slot(sys,body)==SUN;
 if(!codex_body||is_star){
  if(is_star)atlas_world_disc(sys,body,360,108,43);else atlas_station_sprite(sys,358,108);
  text_wrap(3,10,32,10,UI_TEXT,is_star?"This system star supplies light and heat across its local orbits. Close approaches can overheat and damage your ship. This is a chart record, not a landed world.":"The orbital hub links local trade, outfitting, ship services and station life. This entry records charted infrastructure; it does not claim that you have visited every room or met its residents.",0);
  line(24,174,232,174,UI_EDGE);text_px(24,184,UI_CYAN,is_star?"RECORD TYPE":"STATION CLASS");if(is_star)text_px(24,199,UI_GOLD,"CHARTED STAR");else text_px(24,199,UI_GOLD,"%s",station_class_name(station_class_for_system(&game,sys)));text_px(24,217,UI_TEXT,"TECH %d / DANGER %d OF 5",game.systems[sys].tech,danger_rating(&game,sys));
  return;
 }
 int landed=atlas_planet_landed(sys,body);
 if(!landed){atlas_world_disc(sys,body,103,119,47);text_px(176,78,UI_GOLD,"UNDISCOVERED");text_wrap(22,13,34,5,UI_TEXT,"LAND ON PLANET TO GET INFORMATION",0);text_wrap(22,18,34,5,UI_MUTED,"Surface conditions, flora, fauna, minerals and logged sites will be added after your first landing.",0);footer("O BACK TO SYSTEM DIRECTORY");return;}
 FieldProfile p=field_profile(&game,sys,body);
 atlas_world_disc(sys,body,93,116,45);
 const WorldProfile *world=field_world(&game,sys,body);
 text_px(160,73,UI_CYAN,"%s",world_types[world->type].name);
 text_px(160,91,UI_TEXT,"%s",sys==7?field_lave_region(body):field_culture(p.culture));
 text_px(160,109,UI_MUTED,"%d BIOME%s / GENERATION %d",world->count,world->count==1?"":"S",WORLD_GENERATION_VERSION);
 text_px(160,133,UI_GOLD,"%d FIELD RECORDS",atlas_logged(sys,body));
 text_px(160,149,UI_MUTED,"%d LOGGED SITES",atlas_sites_logged(sys,body));
 for(int i=0;i<4;i++){int x=16+(i%2)*232,y=166+(i/2)*38;
  rect(x,y,224,32,row==i?RGB(26,60,74):RGB(12,28,43));rect(x,y,2,32,row==i?UI_GOLD:UI_EDGE);
  text_px(x+12,y+5,row==i?UI_GOLD:UI_TEXT,"%s",atlas_categories[i]);
  text_px(x+12,y+18,UI_MUTED,"%d %s",atlas_category_count(i),i==3?"COMPLETED":"LOGGED");
 }
}
static void atlas_entry_name(int id,char *out,int cap){
 if(atlas_category<3)field_species_name(codex_system_row(),codex_body-1,id,out,cap);
 else snprintf(out,cap,"%s",surface_site_name(&game,codex_system_row(),codex_body-1,id));
}
static void atlas_picture(int id,int x,int y,int size){
 if(atlas_category<3)field_draw_species(x,y,size,codex_system_row(),codex_body-1,id,preview_time,68,158);
 else {draw_station_badge(x-40,y-38);text_px(x-40,y+22,UI_MUTED,"SITE RECORD");}
}
static void atlas_collection(void){
 atlas_chrome(atlas_categories[atlas_category]);int n=atlas_entry_count(),first=row/7*7;
 if(!n){text_wrap(3,11,50,7,UI_TEXT,atlas_category==3?"No completed sites recorded on this world yet. Explore the surface and finish a field activity to add its record here.":"No scans recorded in this collection yet. Explore this world and scan its life or minerals. Only discoveries from this exact planet appear here.",0);return;}
 for(int i=first;i<first+7&&i<n;i++){char name[64],sub[40];int id=atlas_entry_at(i);atlas_entry_name(id,name,64);snprintf(sub,40,"%03d / %d / %02d",codex_system_row()+1,codex_body-1,id+1);atlas_list_row(i,first,name,sub);}
 atlas_scroll(n,7);int id=atlas_entry_at(row);atlas_picture(id,350,121,60);
 text_wrap(32,22,26,6,UI_TEXT,atlas_category<3?field_species_trait(codex_system_row(),codex_body-1,id):"Completed field activity. Open its record for the local brief.",0);
}
static const char *atlas_field_note(int kind){
 static const char *notes[]={
 "A small life-form can define a whole landscape. Keep its fronds and surrounding ground undisturbed: the specimen's relationship with its habitat is as valuable as the scan itself.",
 "Watch from a little distance and let it resume its routine. A scanner gives you an identity; patience reveals how an animal moves through the world it calls home.",
 "This sample is a small piece of a much older world. Its structure gives surveyors a reference for comparing deposits without treating every familiar-looking stone as the same discovery."
 };
 return notes[kind<0||kind>2?0:kind];
}
static void atlas_record(void){
 char name[64];atlas_entry_name(atlas_entry,name,64);atlas_chrome(name);
 atlas_picture(atlas_entry,98,128,65);
 text_px(24,185,UI_CYAN,"%03d-%d-%02d",codex_system_row()+1,codex_body-1,atlas_entry+1);
 text_px(176,72,UI_GOLD,"%s / LOGGED",atlas_categories[atlas_category]);
 char description[512];
 if(atlas_category<3)snprintf(description,sizeof(description),"%s %s",field_species_trait(codex_system_row(),codex_body-1,atlas_entry),atlas_field_note(atlas_category));
 else if(atlas_entry==observatory_site(&game,codex_system_row(),codex_body-1))snprintf(description,sizeof(description),"%s",observatory_report(observatory_choice(&game,codex_system_row(),codex_body-1)));
 else snprintf(description,sizeof(description),"%s",surface_site_brief(&game,codex_system_row(),codex_body-1,atlas_entry));
 text_wrap(22,12,35,10,UI_TEXT,description,0);
 FieldProfile p=field_profile(&game,codex_system_row(),codex_body-1);
 char context[192];snprintf(context,sizeof(context),"RECORDED ON %s. %s. %s.",codex_body_name(codex_system_row(),codex_body-1),field_weather(p.weather),atlas_category<3?"SURFACE SCAN CONFIRMED":"FIELD ACTIVITY COMPLETED");
 text_wrap(22,24,35,6,UI_MUTED,context,0);
 footer("O BACK TO COLLECTION");
}
static void atlas_rifts(void){
 atlas_chrome("SPACE SIGNALS");int n=atlas_rift_count(),count=n+1,first=row/7*7;
 for(int i=first;i<first+7&&i<count;i++){int slot=atlas_rift_at(i);char sub[48];
  if(slot>=0){snprintf(sub,48,"%03d / RIFT %02d",codex_system_row()+1,slot+1);atlas_list_row(i,first,rift_names[rift_type(codex_system_row(),slot)],sub);}
  else atlas_list_row(i,first,"EARLIER SCAN TOTALS","LOCATION DATA UNAVAILABLE");
 }
 atlas_scroll(count,7);
 int slot=atlas_rift_at(row);
 if(slot>=0){rift_portrait(350,120,65,rift_type(codex_system_row(),slot),preview_time,68,172);text_wrap(32,23,26,5,UI_TEXT,"A saved field identity in this system. Open its instrument report and survey lore.",0);}
 else text_wrap(32,11,26,14,UI_TEXT,"Older scan totals have no stored locations. New scans are filed here with their real system and field identity.",0);
}
static void atlas_rift_record(void){
 int type=rift_type(codex_system_row(),atlas_entry);atlas_chrome(rift_names[type]);
 rift_portrait(91,131,69,type,preview_time,68,222);char prose[1024];snprintf(prose,sizeof(prose),"%s %s",rift_reports[type],rift_lore[type]);
 text_wrap(22,9,35,21,UI_TEXT,prose,0);
 text_px(16,231,UI_CYAN,"%03d / RIFT %02d",codex_system_row()+1,atlas_entry+1);footer("O BACK TO SIGNALS");
}
static void codex_screen(void){
 if(codex_scope==ATLAS_LORE){galactic_lore_screen();return;}
 int n=codex_rows();if(row<0||row>=n)row=0;
 if(codex_scope==ATLAS_GALAXY)atlas_galaxy();
 else if(codex_scope==ATLAS_SYSTEM)atlas_system();
 else if(codex_scope==ATLAS_WORLD)atlas_world();
 else if(codex_scope==ATLAS_CATEGORY)atlas_collection();
 else if(codex_scope==ATLAS_RECORD)atlas_record();
 else if(codex_scope==ATLAS_SIGNALS)atlas_rifts();
 else if(codex_scope==7)atlas_rift_record();
 else {atlas_chrome("SPACE SIGNALS / COVERAGE");draw_anomaly_icon(340,78,0);text_wrap(3,10,33,16,UI_TEXT,"Older builds kept scan totals without locations. Those earlier discoveries cannot be assigned to a particular system. New rift scans now retain their exact system and field identity in Space Signals.",0);text_px(24,222,UI_CYAN,"%d SPACE SCANS / ALL SYSTEMS",game.scanned_anomalies);}
 atlas_reward_overlay();
}
