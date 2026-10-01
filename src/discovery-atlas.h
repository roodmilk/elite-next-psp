/* Hierarchical record browser. Views enumerate IDs, never materialise the galaxy.
 * Adding a category requires a provider/count/label; navigation is shared.
 * Existing saved (system, body, slot) identities are the source of truth. */
enum { ATLAS_GALAXY=0,ATLAS_SYSTEM=1,ATLAS_WORLD=2,ATLAS_LORE=3,ATLAS_CATEGORY=4,ATLAS_RECORD=5,ATLAS_SIGNALS=6 };
typedef struct {int scope,system,body,category,entry,selection;} AtlasFrame;
static AtlasFrame atlas_stack[8];static int atlas_depth=0,atlas_category=0,atlas_entry=0;
static const char *atlas_categories[]={"FLORA","FAUNA","MINERALS","FIELD SITES"};
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
 if(codex_scope==ATLAS_WORLD)return codex_body>1?4:1;
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
  if(row==codex_rows()-1)atlas_push(ATLAS_SIGNALS);
  else {int body=codex_system_body_at(codex_system_row(),row);atlas_push(ATLAS_WORLD);codex_body=body;}
 }else if(codex_scope==ATLAS_WORLD&&codex_body>1){atlas_push(ATLAS_CATEGORY);atlas_category=selection;}
 else if(codex_scope==ATLAS_CATEGORY){int id=atlas_entry_at(row);if(id>=0){atlas_push(ATLAS_RECORD);atlas_entry=id;}}
}
static void atlas_input(unsigned pressed){
 int count=codex_rows();
 if(row<0||row>=count)row=0;
 if(pressed&PSP_CTRL_CIRCLE){atlas_back();return;}
 if(pressed&PSP_CTRL_UP)row=(row+count-1)%count;
 if(pressed&PSP_CTRL_DOWN)row=(row+1)%count;
 if(codex_scope==ATLAS_WORLD&&codex_body>1&&(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT)))row^=1;
 if(pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER)){int delta=pressed&PSP_CTRL_RTRIGGER?7:-7;row+=delta;if(row<0)row=0;if(row>=count)row=count-1;}
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
static void atlas_galaxy(void){
 atlas_chrome("VISITED SYSTEMS");int n=vis_count(),first=row/7*7,sys=vis_sys(row);
 for(int i=first;i<first+7&&i<n;i++){int s=vis_sys(i);char sub[40];snprintf(sub,sizeof(sub),s==game.system?"YOU ARE HERE / %d WORLDS":"VISITED / %d WORLDS",codex_system_body_count(s)-2);atlas_list_row(i,first,game.systems[s].name,sub);}
 atlas_scroll(n,7);
 panel(244,64,228,174);
 for(int i=0;i<n;i++){int s=vis_sys(i),x=258+game.systems[s].x*198/255,y=77+game.systems[s].y*112/255;pixel(x,y,UI_MUTED);}
 int x=258+game.systems[sys].x*198/255,y=77+game.systems[sys].y*112/255;
 circle(x,y,5,UI_CYAN);line(x-8,y,x+8,y,UI_CYAN);line(x,y-8,x,y+8,UI_CYAN);
 text_px(256,200,UI_GOLD,"%.12s",game.systems[sys].name);
 text_px(256,215,UI_TEXT,"%d LANDED WORLDS",codex_system_body_count(sys)-2);
 text_px(256,229,UI_MUTED,"X EXPLORE THE ARCHIVE");
}
static void atlas_system(void){
 atlas_chrome("SYSTEM DIRECTORY");int sys=codex_system_row(),n=codex_rows(),first=row/7*7;
 for(int i=first;i<first+7&&i<n;i++){
  int b=codex_system_body_at(sys,i);char label[40],sub[40];
  if(i==n-1){snprintf(label,40,"SPACE SIGNALS");snprintf(sub,40,"%d LOGGED FIELDS",atlas_rift_count());}
  else if(!b){snprintf(label,40,"ORBITAL STATION");snprintf(sub,40,"CHARTED INFRASTRUCTURE");}
  else {snprintf(label,40,"%s",codex_body_name(sys,b-1));snprintf(sub,40,b==1?"PRIMARY STAR":"LANDED / %d FIELD RECORDS",b==1?0:atlas_logged(sys,b-1));}
  atlas_list_row(i,first,label,sub);
 }
 atlas_scroll(n,7);panel(244,64,228,174);
 int selected=codex_system_body_at(sys,row);
 if(row==n-1)draw_anomaly_icon(300,80,0);
 else if(!selected)draw_station_badge(282,76);
 else atlas_world_disc(sys,selected-1,354,111,37);
 text_px(256,161,UI_CYAN,"%d / %d WORLDS LANDED",n-3,BODY_COUNT-1);
 text_wrap(32,23,26,5,UI_TEXT,"Land on a world to open its archive. Scan life and minerals to fill its collections.",0);
}
static void atlas_world(void){
 int sys=codex_system_row(),body=codex_body-1;
 atlas_chrome(codex_body>1?codex_body_name(sys,body):codex_body?"PRIMARY STAR":"ORBITAL STATION");
 if(codex_body<=1){
  if(codex_body)atlas_world_disc(sys,0,360,119,49);else draw_station_badge(318,76);
  text_wrap(3,10,32,15,UI_TEXT,codex_body?"The system's primary star supplies the light and heat that shape your journey. Close approaches can overheat and damage your ship. This is a chart record, not a landed world.":"The orbital hub links local trade, outfitting, ship services and station life. This entry records charted infrastructure; it does not claim that you have visited every room or met its residents.",0);
  return;
 }
 FieldProfile p=field_profile(&game,sys,body);
 atlas_world_disc(sys,body,93,116,45);
 text_px(160,73,UI_CYAN,"%s",field_type(sys,body)==GAS?"GAS / SKYPORT":field_type(sys,body)==OCEAN?"OCEAN WORLD":"ROCKY WORLD");
 text_px(160,91,UI_TEXT,"%s",sys==7?field_lave_region(body):field_culture(p.culture));
 text_px(160,109,UI_MUTED,"%s",field_weather(p.weather));
 text_px(160,133,UI_GOLD,"%d FIELD RECORDS",atlas_logged(sys,body));
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
}
