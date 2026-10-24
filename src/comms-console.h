/* Native 480x272 console. IDs retain legacy actions for tutorial compatibility. */
typedef struct {int x,y,w,h;} CommsKey;
static const CommsKey comms_keys[16]={
 {12,134,108,34},{12,92,222,36},{126,134,108,34},{242,92,226,36},
 {12,214,222,22},{164,214,144,22},{242,214,226,22},{240,134,112,34},
 {12,174,108,34},{240,174,112,34},{12,44,270,42},{358,174,110,34},
 {358,134,110,34},{290,44,86,42},{126,174,108,34},{382,44,86,42}
};
static float comms_anim=0,comms_press=0;static int comms_pressed=-1;
static int comms_prior_music=0,comms_prior_chatter=0;
static unsigned comms_mix(unsigned a,unsigned b,int t){int u=255-t;return RGB(((a&255)*u+(b&255)*t)/255,(((a>>8)&255)*u+((b>>8)&255)*t)/255,(((a>>16)&255)*u+((b>>16)&255)*t)/255);}
static const char *comms_labels[16]={"DISMISS","CONTACT TARGET","CLEAR","REQUEST DOCKING","MISSION LOG","STATION DECK","SALVAGE EVA","HUD","CHATTER","CONTRAST","RADIO TUNER","CONTROLS","CAMERA","MUSIC","CAPTIONS","QUIET"};
static int comms_has_target(void){return (selected_target!=0||target_explicit)&&valid_target(selected_target);}
static int comms_incoming(void){return incoming_reply_ready()||night_ready();}
static const char *comms_unavailable(int id){
 if(id==0&&game.message_time<=0&&game.voice_time<=0)return "No active message.";
 if(id==1&&!comms_incoming()&&(!comms_has_target()||(!IS_NPC_ID(selected_target)&&!IS_STATION_ID(selected_target)&&!IS_DISCOVERY_ID(selected_target)&&selected_target!=PLAYER_TARGET_ID&&selected_target!=EXPEDITION_TARGET_ID&&selected_target!=DOCKBAY_TARGET_ID)))return "Select a ship or station.";
 if(id==1&&IS_DISCOVERY_ID(selected_target)&&!comms_incoming()&&length(sub(target_position(selected_target),game.pos))>12000)return "Move within 12,000 m to contact.";
 if(id==1&&selected_target==EXPEDITION_TARGET_ID&&!comms_incoming()){
  const ExpeditionStage *s=expedition_stage(&game,game.job_sel);if(!s||s->kind!=XP_HAIL&&s->kind!=XP_PATROL)return "Navigation waypoint; select a contact.";
  int near=0;ExpeditionRuntime *r=&game.expedition_runtime;for(int k=0;k<6;k++)if(r->built&&r->npc[k]>=0&&game.npc[r->npc[k]].alive&&length(sub(game.npc[r->npc[k]].pos,game.pos))<600)near=1;
  if(!near)return "Move within 600 m of the contact.";
 }
 if(id==1&&!comms_incoming()&&IS_NPC_ID(selected_target)&&(!target_detected(selected_target)||length(sub(target_raw_position(selected_target),game.pos))>module_scan_range(&game)))return "Move closer to hail this contact.";
 if(id==1&&!comms_incoming()&&(selected_target==DOCKBAY_TARGET_ID||IS_NPC_ID(selected_target)&&dockbay_actor(&game,selected_target-NPC_ID_MIN)>=0)){
  int npc=selected_target==DOCKBAY_TARGET_ID?game.dockbay_npc:selected_target-NPC_ID_MIN;
  const char *unavailable=dockbay_contact_unavailable(&game,npc);if(unavailable)return unavailable;
 }
 if(id==2&&!comms_has_target())return "No target selected.";
 if(id==3){if(game.docked)return "Already docked.";if(game.planet>=0)return "Return to orbit first.";if(game.dead||game.jump>0||game.dock_stage||game.police_stop)return "Docking unavailable now.";int hub=docking_target_hub();if(length(sub(game.pos,hub_position(&game,hub)))>station_comms_range(&game,hub))return "Move closer to a station.";}
 if(id==5&&!game.docked)return "Dock to walk the station.";
 if(id==6&&(game.docked||game.planet>=0||!comms_has_target()||!IS_ANOMALY_ID(selected_target)||length(sub(target_position(selected_target),game.pos))>=1400))return "Select a nearby derelict echo.";
 return 0;
}
static void comms_toggle_quiet(void){
 if(!comms_quiet){comms_prior_music=radio_off;comms_prior_chatter=quiet_comms;comms_quiet=1;radio_off=quiet_comms=1;}
 else {comms_quiet=0;radio_off=comms_prior_music;quiet_comms=comms_prior_chatter;}
 radio_dirty=1;
}
static void comms_cycle_hud(void){hud_mode=(hud_mode+1)%3;hud_hidden=hud_mode==2;}
/* Independent versioned preferences; legacy radio.cfg remains compatible. */
static int comms_settings_save(const char *path){
 uint32_t s[4]={0x434f4e31u,(unsigned)(hud_mode|(high_contrast<<2)|((third_person==1)<<3)|(comms_captions<<4)|(comms_quiet<<5)|(comms_prior_music<<6)|(comms_prior_chatter<<7)|((third_person==2)<<8)),0,0};s[2]=1;s[3]=s[0]^s[1]^s[2]^0x937adf31u;
 char tmp[256];if(snprintf(tmp,sizeof(tmp),"%s.tmp",path)>=(int)sizeof(tmp))return 0;
 FILE *f=fopen(tmp,"wb");if(!f)return 0;int ok=fwrite(s,1,sizeof(s),f)==sizeof(s);if(fflush(f))ok=0;if(fclose(f))ok=0;if(!ok)return 0;
 /* Keep a valid recovery copy while replacing the primary. */
 char bak[256];if(snprintf(bak,sizeof(bak),"%s.bak",path)>=(int)sizeof(bak))return 0;remove(bak);f=fopen(path,"rb");if(f){fclose(f);if(rename(path,bak))return 0;}return rename(tmp,path)==0;
}
static int comms_settings_load(const char *path){
 uint32_t s[4];char bak[256];snprintf(bak,sizeof(bak),"%s.bak",path);
 for(int i=0;i<2;i++){FILE *f=fopen(i?bak:path,"rb");if(!f)continue;int ok=fread(s,1,sizeof(s),f)==sizeof(s);fclose(f);if(!ok||s[0]!=0x434f4e31u||s[2]!=1||s[1]>511||(s[1]&3)>2||s[3]!=(s[0]^s[1]^s[2]^0x937adf31u))continue;
 hud_mode=s[1]&3;hud_hidden=hud_mode==2;high_contrast=(s[1]>>2)&1;third_person=(s[1]&(1u<<8))?2:(int)((s[1]>>3)&1);comms_captions=(s[1]>>4)&1;comms_quiet=(s[1]>>5)&1;comms_prior_music=(s[1]>>6)&1;comms_prior_chatter=(s[1]>>7)&1;if(comms_quiet)radio_off=quiet_comms=1;return 1;}return 0;
}
static int comms_neighbour(int id,int dx,int dy){
 if(id<0||id>=16)id=10;CommsKey a=comms_keys[id];float ax=a.x+a.w*.5f,ay=a.y+a.h*.5f,best=1e9f;int found=id;
 for(int i=0;i<16;i++)if(i!=id&&i!=5&&i!=9){CommsKey b=comms_keys[i];float x=b.x+b.w*.5f-ax,y=b.y+b.h*.5f-ay,along=x*dx+y*dy,across=fabsf(x*dy-y*dx);if(along<=1)continue;int overlap=dy?(a.x<b.x+b.w&&b.x<a.x+a.w):(a.y<b.y+b.h&&b.y<a.y+a.h);float score=along+across*.1f+(overlap?0.f:10000.f);if(score<best){best=score;found=i;}}return found;
}
/* Deliberately tiny pixel glyphs drawn at their final size, never scaled art. */
static void comms_glyph(int id,int x,int y,unsigned c,int disabled){
 unsigned inset=comms_mix(UI_PANEL,RGB(2,7,12),150);
 switch(id){
 case 13:case 15:rect(x,y+5,4,6,c);line(x+4,y+5,x+8,y+2,c);line(x+8,y+2,x+8,y+14,c);line(x+8,y+14,x+4,y+10,c);line(x+11,y+4,x+14,y+7,c);line(x+14,y+7,x+11,y+11,c);break;
 case 8:case 14:rect(x,y+2,14,9,c);rect(x+1,y+3,12,7,inset);line(x+3,y+11,x+3,y+14,c);line(x+3,y+14,x+7,y+11,c);rect(x+3,y+5,8,1,c);break;
 case 1:line(x,y+12,x+7,y+3,c);line(x+7,y+3,x+14,y+12,c);line(x,y+12,x+14,y+12,c);line(x+10,y+1,x+14,y+1,c);line(x+14,y+1,x+14,y+5,c);break;
 case 3:rect(x,y,15,15,c);rect(x+2,y+2,11,11,inset);rect(x+5,y+5,5,10,c);break;
 case 0:rect(x,y+2,14,11,c);rect(x+1,y+3,12,9,inset);line(x+3,y+5,x+10,y+10,c);line(x+10,y+5,x+3,y+10,c);break;
 case 2:case 7:line(x,y+4,x,y,c);line(x,y,x+4,y,c);line(x+10,y,x+14,y,c);line(x+14,y,x+14,y+4,c);line(x,y+10,x,y+14,c);line(x,y+14,x+4,y+14,c);line(x+10,y+14,x+14,y+14,c);line(x+14,y+14,x+14,y+10,c);if(id==2)line(x+2,y+12,x+12,y+2,c);else {line(x+7,y+4,x+7,y+10,c);line(x+4,y+7,x+10,y+7,c);}break;
 case 12:rect(x,y+3,12,10,c);rect(x+2,y+5,8,6,inset);rect(x+4,y+6,4,4,c);line(x+12,y+6,x+15,y+3,c);line(x+15,y+3,x+15,y+13,c);break;
 default:rect(x+2,y+1,11,13,c);rect(x+3,y+2,9,11,inset);for(int j=0;j<3;j++)rect(x+5,y+4+j*3,5,1,c);break;
 }if(disabled)line(x-1,y+15,x+16,y-1,RGB(115,86,70));
}
static const char *comms_state(int id){switch(id){case 7:return hud_mode==0?"FULL":hud_mode==1?"MINIMAL":"HIDDEN";case 8:return quiet_comms?"OFF":"ON";case 9:return high_contrast?"ON":"OFF";case 12:return third_person==0?"FIRST":third_person==1?"THIRD":"CINEMATIC";case 13:return radio_off?"OFF":"ON";case 14:return comms_captions?"ON":"OFF";case 15:return comms_quiet?"ON":"OFF";default:return "";}}
static void comms_console_draw(void){
 unsigned voidc=comms_mix(UI_PANEL,RGB(1,5,10),185),frame=comms_mix(UI_RAISED,UI_PANEL,90),well=comms_mix(UI_PANEL,RGB(1,5,10),135);
 preview_reset();rect(0,0,W,H,voidc);rect(4,4,472,264,frame);rect(7,7,466,258,UI_PANEL);
 for(int i=0;i<4;i++){int x=i&1?470:9,y=i&2?263:9;rect(x-2,y-2,5,5,comms_mix(UI_EDGE,UI_MUTED,105));line(x-1,y+1,x+1,y-1,voidc);}
 text_px(20,12,UI_TEXT,"COMMS / FLIGHT CONSOLE");text_px(364,12,UI_SIGNAL,game.docked?"DOCKED":"PAUSED");
 rect(12,26,456,14,well);text_px(20,29,UI_SIGNAL,"TARGET");
 char target[48];snprintf(target,sizeof(target),"%.45s",comms_has_target()?target_name(selected_target):"NO TARGET SELECTED");text_px(80,29,UI_TEXT,"%.47s",target);
 for(int i=0;i<16;i++){if(i==5||i==9)continue;CommsKey k=comms_keys[i];int disabled=comms_unavailable(i)!=0,selected=row==i,down=comms_pressed==i&&comms_press>0;int y=k.y+down;
 int dock_ready=i==3&&!disabled;unsigned dock_green=RGB(67,186,126);int dock_pulse=dock_ready?(int)(58+38*(.5f+.5f*sinf(comms_anim*4.2f))):0;
 unsigned edge=selected?UI_ACCENT:dock_ready?comms_mix(UI_EDGE,dock_green,150+dock_pulse/2):UI_EDGE,ink=disabled?UI_MUTED:dock_ready?comms_mix(UI_TEXT,dock_green,72):UI_TEXT;
 unsigned face=dock_ready?comms_mix(UI_RAISED,dock_green,dock_pulse):UI_RAISED;if(selected)face=comms_mix(face,UI_ACCENT,48);
 rect(k.x,y,k.w,k.h,selected?UI_ACCENT:voidc);rect(k.x+1,y+1,k.w-2,k.h-2,edge);rect(k.x+2,y+2,k.w-4,k.h-4,face);rect(k.x+3,y+3,k.w-6,1,dock_ready?comms_mix(UI_EDGE,dock_green,130):comms_mix(UI_EDGE,UI_TEXT,55));
 if(dock_ready){rect(k.x+k.w-12,y+7,5,5,comms_mix(dock_green,WHITE,dock_pulse));rect(k.x+k.w-11,y+8,3,3,dock_green);}
 if(selected){
  rect(k.x+2,y+2,3,k.h-4,UI_ACCENT);
  unsigned corner=comms_mix(UI_ACCENT,UI_TEXT,160);
  rect(k.x+1,y+1,7,1,corner);rect(k.x+1,y+1,1,7,corner);rect(k.x+k.w-8,y+1,7,1,corner);rect(k.x+k.w-2,y+1,1,7,corner);
  rect(k.x+1,y+k.h-2,7,1,corner);rect(k.x+1,y+k.h-8,1,7,corner);rect(k.x+k.w-8,y+k.h-2,7,1,corner);rect(k.x+k.w-2,y+k.h-8,1,7,corner);
 }
 if(i==10){rect(k.x+7,y+7,k.w-14,27,well);button_icon(k.x+10,y+11,'l',ink);button_icon(k.x+k.w-23,y+11,'r',ink);text_px(k.x+30,y+10,ink,"%.24s",radio_station_name(radio_display_station()));
 for(int n=0;n<100;n++){int h=radio_off||comms_quiet?0:(int)(2+2*sinf(comms_anim*4+n*.31f));rect(k.x+34+n*2,y+27-h,1,h+1,UI_SIGNAL);}continue;}
 comms_glyph(i,k.x+8,y+(k.h<=22?4:8),ink,disabled);
 if(k.h<=22){text_px(k.x+28,y+7,ink,"%s",comms_labels[i]);continue;}
 const char *label=i==1&&comms_incoming()?"ANSWER INCOMING":comms_labels[i];
 text_px(k.x+28,y+7,ink,"%s",label);
 const char *state=comms_state(i);if(disabled)state=i==0?"NO MESSAGE":i==2?"NO TARGET":i==3&&game.docked?"DOCKED":"NO SIGNAL";
 if(!state[0])state=i==1?"OPEN CHANNEL":i==3?"GUIDED APPROACH":"";
 text_px(k.x+28,y+21,disabled?UI_MUTED:UI_SIGNAL,"%s",state);
 if(i==13||i==15||i==8||i==14){int on=comms_state(i)[1]=='N';rect(k.x+k.w-25,y+20,18,8,well);rect(k.x+k.w-24+(on?9:0),y+21,8,6,on?UI_SIGNAL:UI_MUTED);}
 if(i==7||i==12){int active=i==7?hud_mode:third_person;for(int j=0;j<3;j++)rect(k.x+k.w-30+j*7,y+8,4,5,j==active?UI_ACCENT:well);}
 }
 rect(240,174,112,34,well);text_px(250,180,UI_SIGNAL,"SAFETY");text_px(250,193,UI_TEXT,"ALWAYS ON");
 const char *why=comms_unavailable(row);const char *info=why?why:row==10?"X next station. Shoulder buttons tune either way.":row==15?"Silence music and chatter; safety warnings remain.":row==14?"Optional captions; mission and danger alerts remain.":row==1?"Open a channel while flight remains paused.":row==3?"Request a guided approach to the nearby station.":"D-pad selects a control. X operates it.";
 if(game.message_time>0&&!strncmp(game.message,"Settings could",14))info=game.message;
 rect(12,240,456,14,well);text_px(20,243,why?RGB(217,159,105):UI_MUTED,"%.55s",info);
 button_icon(20,258,'U',UI_TEXT);button_icon(32,258,'D',UI_TEXT);button_icon(44,258,'L',UI_TEXT);button_icon(56,258,'R',UI_TEXT);text_px(72,260,UI_TEXT,"SELECT");button_icon(160,258,'X',UI_TEXT);text_px(176,260,UI_TEXT,"OPERATE");button_icon(288,258,'O',UI_TEXT);text_px(304,260,UI_TEXT,"RETURN");
 button_icon(376,258,'l',UI_TEXT);button_icon(388,258,'r',UI_TEXT);text_px(404,260,UI_TEXT,"RADIO");
}
static void comms_answer_incoming(void){
 int caller=night_ready()?night_npc:incoming_reply_ready()?game.encounter_npc:-1;if(caller>=0&&caller<NPC_COUNT&&game.npc[caller].alive&&length(sub(game.npc[caller].pos,game.pos))<=module_scan_range(&game))game.npc[caller].name_known=1;
 comms_quick=0;if(night_ready()){night_open=1;change_page(COMMS_PANEL);return;}
 if(incoming_reply_ready()){comms_channel_capture(1);comms_trade_stage=0;row=0;change_page(COMMS_PANEL);}
}
static int comms_console_input(unsigned pressed,float dt){
 if(page!=COMMS_PANEL||comms_encounter_conversation||night_open)return 0;
 if(nav_depth>0&&nav_pages[nav_depth-1]==HOME)comms_return=HOME;
 comms_anim+=dt;comms_press=fmaxf(0,comms_press-dt);if(row<0||row>=16||row==5||row==9)row=10;
 if(pressed&PSP_CTRL_CIRCLE){if((radio_dirty&&!radio_save_settings("radio.cfg"))||!comms_settings_save("comms-console.cfg")){message(&game,"Settings could not be saved.");return 1;}change_page(comms_return);return 1;}
 if(pressed&PSP_CTRL_UP)row=comms_neighbour(row,0,-1);if(pressed&PSP_CTRL_DOWN)row=comms_neighbour(row,0,1);if(pressed&PSP_CTRL_LEFT)row=comms_neighbour(row,-1,0);if(pressed&PSP_CTRL_RIGHT)row=comms_neighbour(row,1,0);
 if(pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER)){int dir=!!(pressed&PSP_CTRL_RTRIGGER)-!!(pressed&PSP_CTRL_LTRIGGER);if(dir){radio_tune((radio_station+RADIO_STATION_COUNT+dir)%RADIO_STATION_COUNT);if(comms_quiet)radio_off=1;game.cue=SFX_SELECT;}return 1;}
 if(!(pressed&PSP_CTRL_CROSS))return 1;
 comms_pressed=row;comms_press=.12f;game.cue=SFX_UI;if(comms_unavailable(row))return 1;
 switch(row){
 case 0:speech_ok();break;
 case 1:if(comms_incoming())comms_answer_incoming();else {unsigned before=game.voice_serial;hail_target();if(page==COMMS_PANEL&&game.voice_time>0&&game.voice_serial!=before){comms_channel_capture(0);comms_trade_stage=0;row=0;}}break;
 case 2:selected_target=0;target_explicit=0;autoaim=0;look_target=-1;break;
 case 3:if(dock_selected_station()){selected_target=STATION_TARGET_ID(game.station_variant);change_page(FLIGHT);}break;
 case 4:change_page(MISSIONLOG);break;
 case 6:walk_kind=2;walk_x=walk_z=walk_yaw=0;walk_oxygen=walk_integrity=100;walk_salvaged=0;change_page(WALK);break;
 case 7:comms_cycle_hud();break;
 case 8:if(comms_quiet)comms_toggle_quiet();quiet_comms=!quiet_comms;radio_dirty=1;break;
 case 10:radio_tune((radio_station+1)%RADIO_STATION_COUNT);if(comms_quiet)radio_off=1;break;
 case 11:change_page(HELP);break;
 case 12:third_person=(third_person+1)%3;break;
 case 13:if(comms_quiet)comms_toggle_quiet();radio_off=!radio_off;radio_dirty=1;break;
 case 14:comms_captions=!comms_captions;break;
 case 15:comms_toggle_quiet();break;
 }if(row>=7){if((radio_dirty&&!radio_save_settings("radio.cfg"))||!comms_settings_save("comms-console.cfg"))message(&game,"Settings could not be saved.");}return 1;
}
