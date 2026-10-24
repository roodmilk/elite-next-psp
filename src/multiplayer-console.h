/* Retro Terminal A: native 480x272. Transports remain authoritative.
 * No UI state is serialized into commander saves; no networking in draw. */
#define MP_INK (ship_theme_index()?UI_TEXT:RGB(240,231,212))
#define MP_DIM (ship_theme_index()?UI_MUTED:RGB(175,160,186))
#define MP_TEAL (ship_theme_index()?UI_ACCENT:RGB(182,230,213))
#define MP_RED RGB(237,119,127)
#define MP_AMBER (ship_theme_index()?UI_SIGNAL:RGB(235,189,131))
#define MP_EDGE (ship_theme_index()?UI_EDGE:RGB(166,155,179))
enum { MP_CONNECT,MP_LIST,MP_LEFT,MP_RIGHT };
enum { MP_NONE,MP_ON,MP_OFF,MP_HOST,MP_JOIN,MP_SEARCH,MP_JOIN_ROOM,MP_CANCEL,MP_END,MP_TRACK,MP_RESUME,MP_SAVE };
static struct {int caller,caller_row,focus,choice,peer_id,local_view,local_lost,wlan,profiles,modal,yes;
 float flash,feedback_time;char feedback[80];} mp={.caller=HOME,.peer_id=-1};
/* Only opt-in emulator fixtures install this hook; shipping execution is NULL. */
static int (*mp_transport_hook)(int action)=NULL;
typedef struct {const char *window,*status,*connect,*heading,*empty1,*empty2,*left,*right,*note;
 int connect_action,left_action,right_action,busy,linked,error,rows,is_rooms;} MpView;
static void mp_feedback(const char *s){snprintf(mp.feedback,sizeof(mp.feedback),"%s",s?s:"");mp.feedback_time=3.f;}
static void multiplayer_enter(int caller){
 mp.caller=caller;mp.caller_row=row;mp.focus=MP_CONNECT;mp.choice=0;mp.peer_id=-1;mp.modal=mp.yes=0;mp.flash=0;mp.feedback_time=0;
 mp.wlan=sceWlanGetSwitchState()!=0;mp.profiles=0;
 mp.local_lost=wifi_ended;
 if(online.stage)mp.profiles=1;else for(int i=1;i<=100;i++)if(!sceUtilityCheckNetParam(i)){mp.profiles=1;break;}
 online_return_page=caller;change_page(MULTIPLAYER);
}
static int mp_ids(int ids[ONLINE_LIMIT]){
 if(wifi.state){if(wifi.state==WIFI_SCAN&&wifi.scan_done)return wifi.rooms_n;
  if(wifi.ready&&wifi.quiet<60){ids[0]=-1;return 1;}return 0;}
 return online_list(ids);
}
static void mp_clamp(void){
 int ids[ONLINE_LIMIT],n=mp_ids(ids);if(mp.choice<0)mp.choice=0;if(mp.choice>=n)mp.choice=n>0?n-1:0;
 if(!wifi.state&&n){if(mp.peer_id>=0)for(int i=0;i<n;i++)if(ids[i]==mp.peer_id){mp.choice=i;break;}mp.peer_id=ids[mp.choice];}
 if(!n){mp.peer_id=-1;if(mp.focus==MP_LIST)mp.focus=MP_CONNECT;}
}
static MpView mp_view(void){
 MpView v={.window="WLAN COMMUNICATOR",.status="OFFLINE",.connect="CONNECT ONLINE",.heading="ONLINE PILOTS",.empty1="NOT CONNECTED",.empty2="",.left="HOST LOCAL",.right="JOIN LOCAL",.note="",.connect_action=MP_ON,.left_action=MP_HOST,.right_action=MP_JOIN};
 int ids[ONLINE_LIMIT];v.rows=mp_ids(ids);
 if(wifi.state){
  v.heading="LOCAL PILOTS";
  if(wifi.ready){v.window="LOCAL WI-FI / SESSION";v.status="LOCAL LINK";v.linked=1;v.left="RESUME FLIGHT";v.left_action=MP_RESUME;v.right="END SESSION";v.right_action=MP_END;}
  else if(wifi.host){v.window="LOCAL WI-FI / HOST";v.status="WAITING FOR PILOT";v.connect="CANCEL HOSTING";v.connect_action=MP_CANCEL;v.note="Friend: select Join on PSP";v.busy=1;v.empty1="WAITING FOR";v.empty2="A FRIEND...";v.left="RETURN TO FLIGHT";v.left_action=wifi.playing?MP_RESUME:MP_NONE;v.right="END SESSION";v.right_action=MP_END;}
  else if(wifi.state==WIFI_SCAN){
   v.window="LOCAL WI-FI / JOIN";v.heading="LOCAL FLIGHTS";v.is_rooms=1;
   if(!wifi.scan_done){v.status="SEARCHING...";v.connect="CANCEL SEARCH";v.connect_action=MP_CANCEL;v.busy=1;v.empty1="SEARCHING...";}
   else if(wifi.rooms_n){v.status="CHOOSE A FLIGHT";v.connect="JOIN FLIGHT";v.connect_action=MP_JOIN_ROOM;}
   else {v.status="NO LOCAL HOSTS";v.connect="SEARCH AGAIN";v.connect_action=MP_SEARCH;v.note="Ask your friend to host.";v.empty1="NO FLIGHTS FOUND";}
   v.right="SEARCH AGAIN";v.right_action=wifi.scan_done?MP_SEARCH:MP_NONE;
  }else {v.window="LOCAL WI-FI / JOIN";v.heading="LOCAL FLIGHTS";v.status="JOINING...";v.connect="CANCEL JOIN";v.connect_action=MP_CANCEL;v.busy=1;v.is_rooms=2;v.right="SEARCH AGAIN";v.right_action=MP_NONE;}
 }else if(mp.local_lost){v.window="LOCAL WI-FI / SESSION";v.heading="LOCAL PILOTS";v.status="LOCAL LINK LOST";v.connect="HOST LOCAL";v.connect_action=MP_HOST;v.error=1;v.note="Local connection has ended.";}
 else if(online.stage==3){v.status="ONLINE";v.connect="CONNECTED";v.connect_action=MP_OFF;v.linked=1;v.empty1="NO OTHER PILOTS";v.empty2="CONNECTED YET";}
 else if(online.stage==1){v.status="CONNECTING...";v.connect="CANCEL CONNECT";v.connect_action=MP_OFF;v.busy=1;v.note=online.note;}
 else if(online.stage==2){v.status="WI-FI CONNECTED";v.connect="CANCEL CONNECT";v.connect_action=MP_OFF;v.busy=1;v.note="Connecting to the game server.";}
 else if(online.enabled&&online.retry!=UINT64_MAX){v.status="CONNECTING...";v.connect="CANCEL CONNECT";v.connect_action=MP_OFF;v.busy=1;}
 else if(online_error){
  v.error=1;v.connect="TRY AGAIN";
  switch(online_error){
   case ONLINE_WLAN:v.status="WLAN OFF";v.note="Turn on the PSP WLAN switch.";break;
   case ONLINE_PROFILE:v.status="NO WI-FI PROFILE";v.note="Save Wi-Fi in PSP settings.";break;
   case ONLINE_FULL:v.status="SERVER FULL";v.note="Server is full. Try later.";break;
   case ONLINE_LOST:v.status="CONNECTION LOST";v.connect="RECONNECT";v.note="Offline play can continue.";break;
   case ONLINE_SERVER:v.status="SERVER UNAVAILABLE";v.note="Could not reach the server.";break;
   default:v.status="WI-FI UNAVAILABLE";v.note="Could not start Wi-Fi. Retry.";break;
  }
 }
 if(!mp.wlan&&!wifi.state&&!online.stage){v.status="WLAN OFF";v.note="Turn on the PSP WLAN switch.";v.error=1;}
 else if(!mp.profiles&&!wifi.state&&!online.stage&&!online_error&&!online_save_dirty){v.status="NO WI-FI PROFILE";v.note="Save Wi-Fi in PSP settings.";v.error=1;}
 if(online_save_dirty){v.note="Setting not saved. Retry save.";if(!v.linked&&!v.busy){v.status="SETTING NOT SAVED";v.connect="TRY AGAIN";v.connect_action=MP_SAVE;v.error=1;v.note="Check free space on the PSP.";}}
 else if(mp.feedback_time>0)v.note=mp.feedback;
 return v;
}
static const char *mp_unavailable(int action){
 if(action==MP_NONE)return "Please wait for the connection.";
 if(action==MP_ON||action==MP_HOST||action==MP_JOIN||action==MP_SEARCH||action==MP_JOIN_ROOM){
  if(!mp.wlan)return "Turn on the PSP WLAN switch.";
  if(action==MP_ON&&!mp.profiles)return "Save Wi-Fi in PSP settings.";
  if((action==MP_HOST||action==MP_JOIN)&&(!wifi_available()||mp.caller==INTRO))return mp.caller==INTRO?"Load a commander to fly locally.":"Use local Wi-Fi in normal space.";
 }
 if(action==MP_JOIN_ROOM&&(!wifi.scan_done||!wifi.rooms_n))return "No local flight selected.";
 if(action==MP_TRACK){int ids[ONLINE_LIMIT];if(!mp_ids(ids))return "No pilot selected.";if(mp.caller==INTRO)return "Load a commander to track pilots.";}
 return NULL;
}
static int mp_focused_action(MpView v){return mp.focus==MP_CONNECT?v.connect_action:mp.focus==MP_LIST?(v.is_rooms?MP_JOIN_ROOM:MP_TRACK):mp.focus==MP_LEFT?v.left_action:v.right_action;}
static void mp_return(void){
 int caller=mp.caller,saved_row=mp.caller_row;mp.modal=0;nav_back=1;change_page(caller);nav_back=0;row=saved_row;analog_ready=0;
}
static void mp_local_close(const char *note){wifi_close(NULL);wifi_ended=0;wifi_release=0;page=MULTIPLAYER;mp.local_view=mp.local_lost=0;mp.choice=0;mp_feedback(note);}
static void mp_begin_local(int host){
 if(online.enabled||online.stage)online_disable("Using local Wi-Fi");
 if(wifi.state)mp_local_close(NULL);
 mp.local_view=host?1:2;mp.local_lost=0;mp.choice=0;mp.peer_id=-1;
 if(mp_transport_hook){mp_transport_hook(host?MP_HOST:MP_JOIN);return;}
 int ok=wifi_init(host);page=MULTIPLAYER;wifi_release=0;
 if(!ok)mp_feedback(game.message[0]?game.message:"Could not start local Wi-Fi.");
}
static void mp_execute(int action){
 const char *reason=mp_unavailable(action);if(reason){mp_feedback(reason);return;}
 mp.flash=.16f;game.cue=SFX_SELECT;
 if(action==MP_ON){
  if(wifi.state){mp.modal=MP_ON;mp.yes=0;return;}
  mp.local_view=mp.local_lost=0;if(mp_transport_hook){mp_transport_hook(action);return;}online_request();
 }else if(action==MP_OFF){if(mp_transport_hook)mp_transport_hook(action);else online_disable("Disconnected.");mp_feedback("Disconnected.");mp.choice=0;mp.peer_id=-1;}
 else if(action==MP_SAVE){if(mp_transport_hook)mp_transport_hook(action);else online_retry_save();}
 else if(action==MP_HOST||action==MP_JOIN){if(online.stage||online.enabled){mp.modal=action;mp.yes=0;return;}mp_begin_local(action==MP_HOST);}
 else if(action==MP_CANCEL){if(mp_transport_hook)mp_transport_hook(action);mp_local_close("Local connection cancelled.");}
 else if(action==MP_END){mp.modal=MP_END;mp.yes=0;}
 else if(action==MP_RESUME)mp_return();
 else if(action==MP_SEARCH){
  if(mp_transport_hook){mp_transport_hook(action);return;}
  if(wifi.state==WIFI_SCAN&&wifi.scan_done){if(sceNetAdhocctlScan()>=0){wifi.scan_done=0;wifi.scan_age=0;wifi.rooms_n=0;mp.choice=wifi.choice=0;}else mp_feedback("Could not search. Try again.");}
  else if(!wifi.state)mp_begin_local(0);
 }else if(action==MP_JOIN_ROOM){
  wifi.choice=mp.choice;if(mp_transport_hook){mp_transport_hook(action);return;}
  memcpy(wifi.group,wifi.rooms[wifi.choice].group,9);
  if(sceNetAdhocctlConnect(wifi.group)<0)mp_feedback("Could not join. Search again.");else {wifi.state=WIFI_CONNECT;wifi.age=wifi.quiet=0;}
 }else if(action==MP_TRACK){
  int ids[ONLINE_LIMIT],n=mp_ids(ids);if(mp.choice>=n)return;if(!wifi.state)online_apply(ids[mp.choice]);
  selected_target=PLAYER_TARGET_ID;scan_cat=2;autoaim=1;wifi_contact_route();
  if(game.planet>=1&&game.surface==2){surface_target_lock=19;surface_turn_active=wifi_surface_contact_local();autoaim=0;}
  char note[80];snprintf(note,sizeof(note),game.docked?"Tracked. Launch when ready.":wifi_contact_local()?"Pilot selected.":"Route set to %.12s.",game.systems[wifi.remote.system].name);mp_feedback(note);
 }
}
static void mp_confirm(void){
 int action=mp.modal;mp.modal=0;if(!mp.yes)return;
 /* State may have changed during the dialog. Recheck before either teardown. */
 const char *reason=mp_unavailable(action);if(reason){mp_feedback(reason);return;}
 if(action==MP_HOST||action==MP_JOIN){mp_begin_local(action==MP_HOST);}
 else if(action==MP_ON){mp_local_close(NULL);if(mp_transport_hook)mp_transport_hook(MP_ON);else online_request();}
 else if(action==MP_END){if(mp_transport_hook)mp_transport_hook(MP_END);mp_local_close("Local flight ended.");}
}
static void multiplayer_suspend(void){
 if(online.stage)online_fail(ONLINE_LOST,"Online paused for sleep");
 if(wifi.state){int lobby=page==MULTIPLAYER;wifi_close("Wi-Fi ended for sleep. Your commander continues.");mp.local_lost=1;if(lobby){page=MULTIPLAYER;wifi_release=0;}}
 mp.modal=0;
}
static void multiplayer_input(unsigned pressed,float dt){
 int active=wifi.state;if(!mp_transport_hook){
  int was_wlan=mp.wlan;mp.wlan=sceWlanGetSwitchState()!=0;
  if(!was_wlan&&mp.wlan&&!online.stage&&!wifi.state){mp.profiles=0;for(int i=1;i<=100;i++)if(!sceUtilityCheckNetParam(i)){mp.profiles=1;break;}}
 }
 if(active&&!mp_transport_hook){wifi_poll(dt);if(!wifi.state){mp.local_lost=1;mp.local_view=3;}page=MULTIPLAYER;wifi_release=0;}
 mp.flash=fmaxf(0,mp.flash-dt);mp.feedback_time=fmaxf(0,mp.feedback_time-dt);mp_clamp();
 if(mp.modal){
  if(pressed&PSP_CTRL_CIRCLE){mp.modal=0;return;}
  if(pressed&PSP_CTRL_LEFT)mp.yes=1;else if(pressed&PSP_CTRL_RIGHT)mp.yes=0;
  if(pressed&PSP_CTRL_CROSS)mp_confirm();
  return;
 }
 if(pressed&PSP_CTRL_CIRCLE){mp_return();return;}
 MpView v=mp_view();
 if(pressed&PSP_CTRL_LEFT){if(mp.focus==MP_LIST)mp.focus=MP_CONNECT;else if(mp.focus==MP_RIGHT)mp.focus=MP_LEFT;}
 else if(pressed&PSP_CTRL_RIGHT){if(mp.focus==MP_CONNECT&&v.rows)mp.focus=MP_LIST;else if(mp.focus==MP_LEFT)mp.focus=MP_RIGHT;}
 else if(pressed&PSP_CTRL_DOWN){if(mp.focus==MP_CONNECT)mp.focus=MP_LEFT;else if(mp.focus==MP_LIST){if(mp.choice+1<v.rows)mp.choice++;else mp.focus=MP_RIGHT;}}
 else if(pressed&PSP_CTRL_UP){if(mp.focus==MP_LIST){if(mp.choice>0)mp.choice--;}else if(mp.focus==MP_LEFT)mp.focus=MP_CONNECT;else if(mp.focus==MP_RIGHT)mp.focus=v.rows?MP_LIST:MP_CONNECT;}
 if(mp.focus==MP_LIST&&!wifi.state&&v.rows){int ids[ONLINE_LIMIT];online_list(ids);mp.peer_id=ids[mp.choice];}
 if(pressed&PSP_CTRL_SQUARE){if(wifi.state==WIFI_SCAN&&wifi.scan_done)mp_execute(MP_SEARCH);return;}
 if(pressed&PSP_CTRL_CROSS)mp_execute(mp_focused_action(v));
}
static void mp_box(int x,int y,int w,int h,unsigned fill,unsigned edge){rect(x,y,w,h,fill);line(x,y,x+w-1,y,edge);line(x,y+h-1,x+w-1,y+h-1,edge);line(x,y,x,y+h-1,edge);line(x+w-1,y,x+w-1,y+h-1,edge);}
static void mp_text(int x,int y,unsigned ink,const char *s){
 for(int i=0;s&&s[i];i++){unsigned ch=(unsigned char)s[i];if(ch<32||ch>126)ch='?';for(int yy=0;yy<8;yy++)for(int xx=0;xx<8;xx++)if(font8[ch-32][yy]&(128>>xx))pixel(x+i*8+xx,y+yy,ink);}
}
static void mp_bounded(int x,int y,int cells,unsigned ink,const char *s){char b[64];snprintf(b,sizeof(b),"%.*s",cells,s?s:"");mp_text(x,y,ink,b);}
static void mp_center(int x,int y,int w,unsigned ink,const char *s){mp_text(x+(w-(int)strlen(s)*8)/2,y,ink,s);}
static void mp_button(int x,int y,int w,const char *label,int focus,int enabled){
 unsigned fill=!enabled?ui_paint_surface(RGB(50,49,65)):focus?ui_paint_surface(RGB(68,105,108)):ui_paint_surface(RGB(80,81,102));if(focus&&mp.flash>0)fill=ui_paint_surface(RGB(91,124,123));
 mp_box(x,y,w,46,fill,focus&&enabled?MP_TEAL:MP_EDGE);line(x+2,y+2,x+w-3,y+2,focus?ui_paint_surface(RGB(125,158,158)):ui_paint_surface(RGB(152,144,165)));
 if(label)mp_center(x,y+18,w,enabled?MP_INK:MP_DIM,label);
}
static int mp_two_lines(const char *s,char lines[2][32],int cells){
 int n=(int)strlen(s);if(n<=cells){snprintf(lines[0],32,"%s",s);lines[1][0]=0;return 1;}
 int cut=cells;while(cut>0&&s[cut]!=' ')cut--;if(!cut)cut=cells;snprintf(lines[0],32,"%.*s",cut,s);while(s[cut]==' ')cut++;
 if((int)strlen(s+cut)<=cells)snprintf(lines[1],32,"%s",s+cut);
 else {int limit=(int)strlen(s+cut)-cells,offset=(int)(fmaxf(0,preview_time-2)*2)%(limit+5);if(offset>limit)offset=limit;snprintf(lines[1],32,"%.*s",cells,s+cut+offset);}
 return 2;
}
static void mp_location(const WifiPilot *p,char *out,size_t cap){
 if(p->jumping){snprintf(out,cap,"IN HYPERSPACE");return;}
 const char *state=p->docked?"DOCKED":p->planet>=1?(p->rover?"ROAMER":p->surface==2?"ON FOOT":"LANDING"):"SPACE";
 static const char *roman[]={"","I","II","III","IV","V","VI","VII","VIII","IX"};
 if(p->planet>=1)snprintf(out,cap,"%s %s / %s",game.systems[p->system].name,p->planet<10?roman[p->planet]:"?",state);
 else snprintf(out,cap,"%s / %s",game.systems[p->system].name,state);
}
static void mp_roster(MpView v){
 mp_box(288,68,176,96,ui_paint_surface(RGB(32,43,57)),MP_EDGE);mp_text(298,77,MP_DIM,v.heading);line(296,90,455,90,ui_paint_surface(RGB(129,113,142)));
 if(v.is_rooms==2){
  rect(294,97,164,16,ui_paint_surface(RGB(65,87,100)));rect(294,97,2,16,MP_TEAL);char b[32];snprintf(b,sizeof(b),"FLIGHT %.8s",wifi.group);mp_text(302,102,MP_TEAL,b);
  if(wifi.choice>=0&&wifi.choice<wifi.rooms_n){snprintf(b,sizeof(b),"CHANNEL %d",wifi.rooms[wifi.choice].channel);mp_bounded(298,129,19,MP_INK,b);}
 }else if(v.is_rooms&&v.rows){
  int first=mp.choice/3*3;for(int i=first;i<v.rows&&i<first+3;i++){int y=97+(i-first)*20;if(i==mp.choice){rect(294,y,164,16,ui_paint_surface(RGB(65,87,100)));rect(294,y,2,16,MP_TEAL);}char b[32];snprintf(b,sizeof(b),"FLIGHT %.8s",wifi.rooms[i].group);mp_text(302,y+5,MP_INK,b);}
 }else if(v.rows){
  int ids[ONLINE_LIMIT];mp_ids(ids);const WifiPilot *p=wifi.state?&wifi.remote:&online.peer[ids[mp.choice]].pilot;char lines[2][32],place[64];int rows=mp_two_lines(p->name,lines,19);
  rect(294,97,164,rows==2?26:16,ui_paint_surface(RGB(65,87,100)));rect(294,97,2,rows==2?26:16,MP_TEAL);
  for(int i=0;i<rows;i++)mp_text(302,102+i*10,MP_TEAL,lines[i]);
  mp_location(p,place,sizeof(place));int y=rows==2?139:129,n=mp_two_lines(place,lines,19);for(int i=0;i<n;i++)mp_text(298,y+i*10,MP_INK,lines[i]);
 }else {mp_bounded(298,106,19,MP_INK,v.empty1);mp_bounded(298,122,19,MP_INK,v.empty2);}
 if(v.rows>1){rect(460,97,2,62,ui_paint_surface(RGB(64,62,79)));rect(460,97+(54*mp.choice/(v.rows-1)),2,8,MP_TEAL);}
}
static const char *mp_footer_action(int a){
 switch(a){case MP_ON:return "CONNECT";case MP_OFF:return online.stage==3?"DISCONNECT":"CANCEL";case MP_HOST:return "HOST";case MP_JOIN:case MP_JOIN_ROOM:return "JOIN";case MP_SEARCH:return "SEARCH";case MP_CANCEL:return "CANCEL";case MP_END:return "END";case MP_TRACK:return "TRACK";case MP_RESUME:return "RESUME";case MP_SAVE:return "RETRY";default:return "CHOOSE";}
}
static void multiplayer_draw(void){
 mp_clamp();MpView v=mp_view();rect(0,0,W,H,ui_paint_surface(RGB(33,29,57)));unsigned seed=218;
 for(int i=0;i<90;i++){seed=seed*1664525u+1013904223u;int x=seed%480;seed=seed*1664525u+1013904223u;int y=24+seed%228;unsigned c=i%3==0?ui_paint_surface(RGB(75,59,97)):i%3==1?ui_paint_surface(RGB(100,81,113)):ui_paint_surface(RGB(140,125,153));pixel(x,y,c);}
 rect(0,0,480,24,ui_paint_surface(RGB(17,31,42)));draw_next_art(next_logo_small,100,18,5,2,100,18);mp_text(112,8,MP_INK,"/ MULTIPLAYER");line(0,23,479,23,MP_TEAL);
 mp_box(8,36,464,204,ui_paint_surface(RGB(64,55,78)),MP_EDGE);line(10,38,469,38,RGB(209,191,212));rect(16,44,448,16,ui_paint_surface(RGB(109,86,132)));mp_text(24,49,MP_INK,v.window);
 unsigned lamp=v.linked?RGB(182,230,213):v.busy?((int)(preview_time*3)&1?MP_AMBER:ui_paint_surface(RGB(141,127,159))):ui_paint_surface(RGB(141,127,159));rect(449,49,5,5,lamp);
 mp_box(16,68,256,96,ui_paint_surface(RGB(41,43,61)),MP_EDGE);int available=!mp_unavailable(v.connect_action);mp_button(24,78,240,NULL,mp.focus==MP_CONNECT,available);
 unsigned icon=available?MP_TEAL:MP_DIM;line(32,94,37,90,icon);line(37,90,51,90,icon);line(51,90,56,94,icon);line(36,98,40,95,icon);line(40,95,48,95,icon);line(48,95,52,98,icon);line(40,102,44,99,icon);line(44,99,48,102,icon);rect(43,106,3,3,icon);
 mp_text(68,98,available?MP_INK:MP_DIM,v.connect);mp_box(232,89,24,24,ui_paint_surface(RGB(23,34,43)),online.stage==3&&!wifi.state?MP_RED:ui_paint_surface(RGB(139,171,174)));
 if(online.stage==3&&!wifi.state){for(int i=0;i<2;i++){line(239+i,96,248+i,105,MP_RED);line(248+i,96,239+i,105,MP_RED);}}
 else if(v.busy)rect(240,97,8,8,MP_AMBER);else {line(240,97,246,103,icon);line(246,97,240,103,icon);}
 unsigned sc=v.linked?MP_TEAL:v.busy?MP_AMBER:v.error?MP_RED:MP_DIM;rect(29,142,4,4,sc);mp_bounded(44,141,27,sc,v.status);
 const char *reason=mp_unavailable(mp_focused_action(v));const char *note=reason?reason:v.note;
 if(note&&*note){
  char shortnote[64];if(online.stage==1&&!reason&&!mp.feedback_time&&!online_save_dirty){snprintf(shortnote,sizeof(shortnote),"Trying saved Wi-Fi profile %d.",online.active_profile);note=shortnote;}mp_bounded(24,154,30,v.error||reason?MP_AMBER:MP_DIM,note);
 }else for(int k=0;k<7;k++){int h=3+(int)((sinf(preview_time*2+k*1.7f)+1)*4);rect(202+k*7,154-h,3,h,ui_paint_surface(RGB(120,158,157)));}
 mp_roster(v);mp_button(16,180,216,v.left,mp.focus==MP_LEFT,!mp_unavailable(v.left_action));mp_button(248,180,216,v.right,mp.focus==MP_RIGHT,!mp_unavailable(v.right_action));
 if(mp.modal){
  mp_box(38,80,404,120,ui_paint_surface(RGB(17,19,32)),ui_paint_surface(RGB(17,19,32)));mp_box(42,84,396,112,ui_paint_surface(RGB(48,55,70)),MP_TEAL);
  const char *title=mp.modal==MP_HOST||mp.modal==MP_JOIN?"SWITCH TO LOCAL WI-FI?":mp.modal==MP_ON?"CONNECT ONLINE?":"END LOCAL FLIGHT?";
  const char *a=mp.modal==MP_HOST||mp.modal==MP_JOIN?"This disconnects online.":mp.modal==MP_ON?"This ends your local flight.":"Disconnect from the other pilot?";
  const char *b=mp.modal==MP_HOST?"Start hosting a local flight?":mp.modal==MP_JOIN?"Join a nearby local flight?":mp.modal==MP_ON?"Connect to the game server?":"You can keep playing offline.";
  mp_center(42,96,396,MP_AMBER,title);mp_center(42,118,396,MP_INK,a);mp_center(42,132,396,MP_INK,b);
  mp_box(100,154,128,28,mp.yes?ui_paint_surface(RGB(68,105,108)):ui_paint_surface(RGB(80,81,102)),mp.yes?MP_TEAL:MP_EDGE);mp_center(100,164,128,MP_INK,"YES");mp_box(252,154,128,28,mp.yes?ui_paint_surface(RGB(80,81,102)):ui_paint_surface(RGB(68,105,108)),mp.yes?MP_EDGE:MP_TEAL);mp_center(252,164,128,MP_INK,"NO");
 }
 rect(0,252,480,20,ui_paint_surface(RGB(33,29,57)));line(8,252,471,252,MP_EDGE);
 /* Proper native PSP icons, never shoulder glyphs for the D-pad. */
 button_icon(8,256,'U',MP_DIM);button_icon(18,256,'D',MP_DIM);mp_text(30,260,MP_DIM,"SELECT");button_icon(146,256,'X',MP_TEAL);
 mp_text(162,260,MP_INK,mp.modal?"CONFIRM":reason?"CHOOSE":mp_footer_action(mp_focused_action(v)));
 if(!mp.modal&&wifi.state==WIFI_SCAN&&wifi.scan_done){button_icon(266,256,'S',MP_TEAL);mp_text(282,260,MP_DIM,"SEARCH");}
 button_icon(396,256,'O',MP_RED);mp_text(410,260,MP_INK,"BACK");
}
