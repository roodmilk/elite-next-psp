/* Approved flight-ui prototype, rendered by the real native game renderer. */
/* Opaque UI pixels keep the engine's existing FF alpha. Zero is transparent.
 * The cache never contains a copy of the world, so it cannot leave world trails. */
static unsigned hc_canvas[STRIDE*H] __attribute__((aligned(64)));
typedef struct{unsigned offset;unsigned short count;} HcSpan;
static HcSpan hc_spans[4096];static int hc_span_count=0,hc_span_overflow=0;
static unsigned*hc_output=0;static int hc_cache_ready=0,hc_cache_clear=1;
static float hc_cache_due=-1;static unsigned hc_geometry_key=~0u;static int hc_last_key[48];
static unsigned hc_overlay_calls=0,hc_paint_count=0;
static int hc_repaint_needed(void){
 int key[48]={0};key[43]=game.shots;key[44]=hud_mode;key[45]=hud_hidden;key[46]=game.dock_stage;key[0]=hc_browser;key[1]=tools_open;key[2]=selected_target;key[3]=hc_highlight;key[4]=hc_category;key[5]=game.active_weapon;key[6]=fit_weapon_item(&game);key[7]=tools_selected;key[8]=game.ship;
 key[9]=game.incoming_missile>0;key[10]=game.collision>0;key[11]=game.attacked>0;key[12]=game.heat>78;key[13]=wanted_level(&game);key[14]=comms_quick;key[15]=comms_quick_choice;
 key[16]=paused;key[17]=pip_sel;key[18]=game.pip_sys;key[19]=game.pip_eng;key[20]=game.pip_wep;key[21]=autoaim;key[22]=game.missiles;key[23]=game.flare_charges;key[24]=game.voice_time>0;key[25]=game.message_time>0;key[26]=route_prejump_active();
 if(IS_NPC_ID(selected_target))key[27]=game.npc[selected_target-NPC_ID_MIN].name_known;
 int at=0;for(int k=0;k<hc_count;k++)if(hc_ids[k]==hc_highlight)at=k;int first=at<4?0:at-3;
 for(int k=0;k<4&&first+k<hc_count;k++){key[28+k*2]=hc_ids[first+k];key[29+k*2]=hc_detected(hc_ids[first+k]);}
 if(game.job_sel>=0&&game.job_sel<game.job_n){key[40]=game.job_sel+1;key[41]=game.expeditions[game.job_sel].stage;key[42]=game.expeditions[game.job_sel].progress;}
 unsigned geometry=(hc_browser?1u:0)|(tools_open?2u:0)|(paused?4u:0)|(comms_quick?8u:0)|(combat_alert_active()?16u:0)|(wanted_level(&game)>0?32u:0)|(speech_active()?64u:0)|(game.message_time>0?128u:0)|(game.heat>78?256u:0)|(game.collision>0?512u:0)|(game.incoming_missile>0?1024u:0)|(game.attacked>0?2048u:0);
 int changed=hc_cache_dirty||!hc_cache_ready||memcmp(key,hc_last_key,sizeof(key));
 if(!changed&&game.time<hc_cache_due)return 0;
 hc_cache_clear=!hc_cache_ready||geometry!=hc_geometry_key;hc_geometry_key=geometry;memcpy(hc_last_key,key,sizeof(key));hc_cache_dirty=0;
 if(changed||game.time-hc_cache_due>.2f)hc_cache_due=game.time+1.f/15;else hc_cache_due+=1.f/15;
 hc_cache_ready=1;return 1;
}
/* Cache opaque row runs once per paint. Bulk writes avoid a full world
 * readback and GPU synchronization on every frame. Transparent gaps remain live. */
static void hc_build_spans(void){
 hc_span_count=hc_span_overflow=0;
 for(int y=0;y<H;y++)for(int x=0;x<W;){
  while(x<W&&!(hc_canvas[y*STRIDE+x]>>24))x++;
  int start=x;while(x<W&&(hc_canvas[y*STRIDE+x]>>24))x++;
  if(x>start){if(hc_span_count==4096){hc_span_overflow=1;return;}hc_spans[hc_span_count++]=(HcSpan){y*STRIDE+start,(unsigned short)(x-start)};}
 }
}
static void hc_overlay_draw(unsigned*destination){
 uint64_t begin=sceKernelGetSystemTimeWide();
 if(hc_span_overflow){for(int y=0;y<H;y++)for(int x=0;x<W;x++){unsigned c=hc_canvas[y*STRIDE+x];if(c>>24)destination[y*STRIDE+x]=c;}}
 else for(int k=0;k<hc_span_count;k++){const HcSpan*s=&hc_spans[k];memcpy(destination+s->offset,hc_canvas+s->offset,s->count*sizeof(unsigned));}
 hc_overlay_calls++;hc_copy_us+=sceKernelGetSystemTimeWide()-begin;
}
static void hc_present_begin(void){
 if(hc_output)return;hc_output=fb;if(hc_cache_clear)memset(hc_canvas,0,STRIDE*H*sizeof(unsigned));fb=hc_canvas;
}
static void hc_present_finish(void){
 if(!hc_output)return;unsigned*destination=hc_output;hc_output=0;fb=destination;if(hc_cache_clear||speech_active())hc_build_spans();hc_overlay_draw(destination);hc_paint_count++;
}
#include "cockpit-detail-art.h"
static void hc_indicator_anchor(float*x,float*y){
 if(!hc_full_console_visible())return;
 float left=hc_browser?172:32,right=tools_open?224:446,top=tools_open?72:58,bottom=tools_open?130:150;
 *x=fmaxf(left,fminf(right,*x));*y=fmaxf(top,fminf(bottom,*y));
 if(!hc_browser&&*x<112&&*y>132)*y=132;
}
static void hc_idle_reticle(void){
 int cx=83,cy=216,gap=14+(int)(1+sinf(game.time*2));unsigned ink=RGB(65,131,139);
 hc_circle_fast(cx,cy,19,RGB(22,47,61));
 for(int k=0;k<2;k++){float a=game.time*.8f+k*3.14159265f;int x=cx+(int)(cosf(a)*19),y=cy+(int)(sinf(a)*19);line(x,y,x+(int)(-sinf(a)*4),y+(int)(cosf(a)*4),HC_CYAN);}
 for(int sx=-1;sx<=1;sx+=2)for(int sy=-1;sy<=1;sy+=2){int x=cx+sx*gap,y=cy+sy*gap;line(x-sx*5,y,x,y,ink);line(x,y,x,y-sy*5,ink);}
 line(cx-7,cy,cx-3,cy,HC_EDGE);line(cx+3,cy,cx+7,cy,HC_EDGE);line(cx,cy-7,cx,cy-3,HC_EDGE);line(cx,cy+3,cx,cy+7,HC_EDGE);
 text_px(47,242,HC_DIM,"NO TARGET");for(int k=0;k<3;k++)rect(75+k*7,256,3,2,k==(int)(game.time*1.3f)%3?HC_CYAN:HC_EDGE);
}
static void hc_target_shape(int id){
 if(IS_NPC_ID(id)&&scanner_known(id)){
  int mesh=game.npc[id-NPC_ID_MIN].mesh;if(mesh<0||mesh>=mesh_count||mesh>=256)return;const Mesh*m=&meshes[mesh];if(m->vertices>256)return;
  static float radius_cache[256];float radius=radius_cache[mesh];
  if(radius<=0){radius=1;for(int k=0;k<m->vertices;k++)radius=fmaxf(radius,length(m->v[k]));radius_cache[mesh]=radius;}
  float scale=24/radius,c=cosf(game.time*.15f),s=sinf(game.time*.15f);short points[256][2];
  for(int k=0;k<m->vertices;k++){Vec3 v=m->v[k];float x=v.x*c+v.z*s,z=-v.x*s+v.z*c,xx=x*.96891242f-v.y*.24740396f,yy=x*.24740396f+v.y*.96891242f;points[k][0]=(short)(50+xx*scale);points[k][1]=(short)(233-z*scale*.38f-yy*scale*.4f);}
  short order[128];int depth[128],count=0;
  for(int k=0;k<m->triangles&&k<128;k++){const MeshTri*t=&m->t[k];int ax=points[t->a][0],ay=points[t->a][1],bx=points[t->b][0],by=points[t->b][1],cx=points[t->c][0],cy=points[t->c][1];if((bx-ax)*(cy-ay)-(by-ay)*(cx-ax)>=0)continue;
   float z=0;int ids[3]={t->a,t->b,t->c};for(int v=0;v<3;v++){Vec3 p=m->v[ids[v]];z+=-p.x*s+p.z*c;}
   int d=(int)(z*100),j=count++;while(j&&depth[j-1]<d){order[j]=order[j-1];depth[j]=depth[j-1];j--;}order[j]=k;depth[j]=d;
  }
  for(int k=0;k<count;k++){const MeshTri*t=&m->t[order[k]];HcPoint p[3]={{points[t->a][0],points[t->a][1]},{points[t->b][0],points[t->b][1]},{points[t->c][0],points[t->c][1]}};Vec3 normal=norm(t->normal);int light=45+(int)(fabsf(normal.y)*32+fabsf(normal.x*c+normal.z*s)*24);hc_edge_poly(p,3,dim_rgb(HC_GOLD,light,140),dim_rgb(HC_GOLD,3,5));}


 }else {int kind=IS_STATION_ID(id)?0:id>0&&id<=BODY_COUNT?1:3;hud_pixel_icon(47,227,kind,HC_CYAN);hc_circle_fast(53,233,19,HC_EDGE);}
}
static int hc_motion_target=-2,hc_motion_tool=-1,hc_motion_gun=-1,hc_motion_browser=0,hc_motion_tools=0;
static float hc_acquired_at=-10,hc_tool_at=-10,hc_gun_at=-10,hc_panel_at=-10;
static void hc_motion_tick(void){
 if(selected_target!=hc_motion_target){hc_motion_target=selected_target;hc_acquired_at=game.time;}
 if(tools_selected!=hc_motion_tool){hc_motion_tool=tools_selected;hc_tool_at=game.time;}
 if(game.active_weapon!=hc_motion_gun){hc_motion_gun=game.active_weapon;hc_gun_at=game.time;}
 if(hc_browser!=hc_motion_browser||tools_open!=hc_motion_tools){hc_motion_browser=hc_browser;hc_motion_tools=tools_open;hc_panel_at=game.time;}
}
static void hc_motion_draw(void){
 float age=game.time-hc_acquired_at;
 if(selected_target>=0&&age>=0&&age<.6f){int radius=17+(int)(7*(1-age/.6f));hc_circle_fast(53,233,radius,HC_CYAN);}
 age=game.time-hc_tool_at;if(age>=0&&age<.32f){int width=4+(int)(125*age/.32f);line(338,207,338+width,207,HC_GOLD);}
 age=game.time-hc_gun_at;if(age>=0&&age<.32f){int width=4+(int)(124*age/.32f);line(338,230,338+width,230,HC_CYAN);}
 age=game.time-hc_panel_at;if(age>=0&&age<.24f){if(hc_browser){int y=62+(int)(103*age/.24f);line(14,y,15,y+6,HC_CYAN);}if(tools_open){int y=76+(int)(54*age/.24f);line(470,y,470,y+6,HC_GOLD);}}
 if(tools_open&&tools_confirm_time>0){int radius=14+(int)(5*fminf(1,tools_confirm_time/.36f));hc_circle_fast(359,112,radius,tools_weapon_result<0?RED:HC_GOLD);}
 unsigned signal=(game.voice_time>0||game.message_time>0)?HC_CYAN:HC_EDGE;for(int k=0;k<3;k++)rect(84+k*5,148,3,2,signal==HC_CYAN&&((int)(game.time*9)+k)%3?HC_CYAN:HC_EDGE);
 if(game.heat>78||game.incoming_missile>0||game.collision>0||game.attacked>0){unsigned warn=((int)(game.time*5)&1)?HC_GOLD:RGB(108,51,36);line(333,268,469,268,warn);}
}
static void hc_target_card(void){
 hc_frame(8,hc_browser?40:164,152,hc_browser?229:105,HC_CYAN);
 if(hc_browser){
  hc_key(13,43,'S',0);text_px(30,47,HC_CYAN,"TARGET COMPUTER");line(16,60,151,60,HC_EDGE);
  text_px(18,66,HC_CYAN,"<");text_px(48,66,WHITE,"%s",scan_cat_names[hc_category]);text_px(143,66,HC_CYAN,">");
  int index=0;for(int k=0;k<hc_count;k++)if(hc_ids[k]==hc_highlight)index=k;int start=index<4?0:index-3;
  for(int k=start;k<hc_count&&k<start+4;k++){
   int id=hc_ids[k],y=80+(k-start)*17;if(hc_category!=5&&!hc_detected(id))continue;unsigned ink=id==hc_highlight?WHITE:HC_DIM;
   if(id==hc_highlight){rect(14,y-1,139,17,RGB(17,47,56));rect(14,y-1,1,17,HC_CYAN);}
   text_px(20,y,ink,"%.16s",hc_category==5?nav_ui_name(id):scanner_known(id)?target_name(id):"UNKNOWN CONTACT");char range[24];target_range_label(length(sub(hc_category==5?nav_ui_position(id):target_raw_position(id),game.pos)),range,sizeof(range));text_px(95,y+8,HC_CYAN,"%7s",range);
  }
  if(!hc_count)text_px(20,94,HC_DIM,"NO CONTACTS");
  hc_key(15,150,'X',0);text_px(32,154,WHITE,hc_category==5?"PLOT":"TARGET");hc_key(95,150,'r',0);text_px(112,154,HC_DIM,hc_category==5?"ALIGN":"TRACK");line(16,166,151,166,HC_EDGE);
 }else {hc_key(13,167,'S',0);}
 text_px(hc_browser?18:30,172,HC_CYAN,hc_browser?"CURRENT TARGET":"TARGET COMPUTER");line(31,181,150,181,RGB(27,71,76));int scan=31+(int)(game.time*24)%108;line(scan,181,scan+10,181,HC_CYAN);
 int id=hc_detected(selected_target)?selected_target:-1;
 if(id<0){hc_idle_reticle();return;}
 const char *name=scanner_known(id)?target_name(id):"UNKNOWN CONTACT";hc_words(18,185,136,HC_GOLD,name);
 char distance[24];target_range_label(length(sub(target_raw_position(id),game.pos)),distance,sizeof(distance));text_px(88,208,WHITE,"%s",distance);
 text_px(88,219,HC_CYAN,autoaim?"ALIGNING":"LOCKED");hc_target_shape(id);
 if(IS_NPC_ID(id)&&scanner_known(id)){
  NPC*n=&game.npc[id-NPC_ID_MIN];float mh=n->bounty_slot>=0?110+danger_rating(&game,game.system)*35:n->freighter?900:n->role==LAW?110:80;
  float ms=n->bounty_slot>=0?55+danger_rating(&game,game.system)*15:n->freighter?100:n->role==LAW?60:40;
  text_px(88,233,HC_DIM,"SHLD");hc_segment_meter(88,243,60,(int)(100*n->shield/ms),HC_CYAN);
  text_px(88,250,HC_DIM,"HULL");hc_segment_meter(88,260,60,(int)(100*n->health/mh),HC_GOLD);
 }else {hc_words(18,253,136,HC_DIM,scanner_known(id)?(IS_STATION_ID(id)?"STATION":id>0&&id<=BODY_COUNT?"WORLD":"SIGNAL"):"UNIDENTIFIED");}
}
static void hc_speed_ticker(void){
 /* Only this 32x8 aperture is updated at display cadence; never repaint the
    whole cockpit just to roll four digits. Use the supplied game glyphs. */
 float speed=fmaxf(0,fminf(9999,hud_smooth[4]));int value=(int)speed;unsigned ink=game.boost?HC_GOLD:WHITE;
 rect(205,168,32,10,HC_BG);
 int place=1000;for(int k=0;k<4;k++,place/=10){int digit=value/place%10;float phase=fmaxf(0,fmodf(speed,(float)place)-place+1);if(place==1)phase=speed-value;int offset=(int)(phase*8);
  if(k<3&&value<place)continue;
  for(int next=0;next<2;next++){const unsigned char*g=font8['0'+(digit+next)%10-32];int top=169-offset+next*8;
   for(int y=0;y<8;y++){int yy=top+y;if(yy<169||yy>176)continue;for(int x=0;x<8;x++)if(g[y]&(128u>>x))fb[yy*STRIDE+205+k*8+x]=ink;}
  }
 }
}
static void hc_ship_instruments(void){
 hc_frame(164,164,156,105,HC_EDGE);hc_frame(328,164,146,105,HC_GOLD);
 text_px(173,169,HC_DIM,"SPD");hc_segment_meter(246,170,63,(int)(100*game.speed/fmaxf(1,player_ships[game.ship].speed)),game.boost?HC_GOLD:HC_CYAN);
 line(171,180,312,180,HC_EDGE);text_px(172,184,HC_GOLD,"%.10s",game.systems[game.system].name);
 text_px(260,184,HC_DIM,"%3dKM",(int)(sensor_range()*.001f));
 if(navigation_enabled(&game)){
  Vec3 point;int active=nav_active_point(&point);
  if(game.buoy_scan_time>0){text_px(173,195,HC_CYAN,"BUOY SCAN %3d%%",(int)(game.buoy_scan_time*50));}
  else if(active==1||active==3){char label[20];target_range_label(length(sub(point,game.pos)),label,sizeof(label));text_px(173,195,HC_GOLD,"NAV %s",label);}
  else if(active==2){text_px(173,195,HC_GOLD,"SEARCH / 2KM AREA");}
  else if(nav_trace_key>=0){Vec3 v=camera(&game,add(game.pos,mul(nav_trace_bearing,1000)));int sector=((int)floorf((atan2f(v.x,v.z)*57.2958f+360)/30+.5f)%12)*30;text_px(173,195,HC_CYAN,"TRACE %03d %s %d",sector,v.y>120?"UP":v.y< -120?"DN":"--",1+(int)(nav_trace_strength*4));}
 }
 /* Existing radar and law-scan echoes retain their real ship-relative data. */
 int radar_x0=clipx0,radar_x1=clipx1,radar_y0=clipy0,radar_y1=clipy1;clipx0=172;clipx1=312;clipy0=202;clipy1=252;
 static HcPoint radar_ring[2][33];static int radar_ring_ready;
 if(!radar_ring_ready){for(int ring=0;ring<2;ring++)for(int k=0;k<=32;k++){float a=k*.19634954f;radar_ring[ring][k]=(HcPoint){(int)(cosf(a)*(ring?57:28)),(int)(sinf(a)*(ring?21:10))};}radar_ring_ready=1;}
 for(int ring=0;ring<2;ring++)for(int k=0;k<32;k++)line(240+radar_ring[ring][k].x,230+radar_ring[ring][k].y,240+radar_ring[ring][k+1].x,230+radar_ring[ring][k+1].y,RGB(34,76,81));
 line(240,205,240,251,RGB(34,76,81));float sweep=game.time*.7f;line(240,230,240+(int)(cosf(sweep)*53),230+(int)(sinf(sweep)*19),RGB(43,110,108));
 if(fit_find(&game,LAW_SCANNER_ITEM)>=0&&game.law_scan_valid){law_radar();rect(172,195,139,12,HC_BG);int age=(int)(game.time-game.law_scan_time);if(age>=0&&age<12)text_px(176,196,HC_CYAN,"LAW %02dS / 20KM",age);}
 for(int pass=0;pass<2;pass++)for(int id=0;id<=FLIGHT_TARGET_MAX;id++)if(((id==selected_target)==pass)&&hc_detected(id)){
  unsigned ink=id==RUMOUR_TARGET_ID?RGB(239,179,98):id==WHALE_TARGET_ID?RGB(115,201,192):id==ROUTE_TARGET_ID?CYAN:IS_NPC_ID(id)?faction_colors[game.npc[id-NPC_ID_MIN].role]:IS_STATION_ID(id)?UI_SIGNAL:id>0&&id<=BODY_COUNT?RGB(125,122,91):HC_DIM;
  radar_dot(camera(&game,target_position(id)),ink,IS_STATION_ID(id)?2:id>0&&id<=BODY_COUNT?1:0,pass);
 }
 if(navigation_enabled(&game)){
  Vec3 point;int active=nav_active_point(&point);
  if(active==1||active==3)radar_dot(camera(&game,point),HC_GOLD,2,0);
  else if(active==2){RadarPoint q=radar_point(camera(&game,point));hc_circle_fast(q.x,q.y-q.lift,4,HC_GOLD);}
  else if(nav_trace_key>=0){Vec3 bearing=camera(&game,add(game.pos,mul(nav_trace_bearing,12000)));float angle=atan2f(bearing.x,bearing.z);int x=240+sinf(angle)*58,y=230-cosf(angle)*18;line(x-2,y,x+2,y,HC_CYAN);pixel(x,y-2,HC_CYAN);pixel(x,y+2,HC_CYAN);}
 }
 hud_pixel_icon(237,227,2,WHITE);clipx0=radar_x0;clipx1=radar_x1;clipy0=radar_y0;clipy1=radar_y1;
 for(int bank=0;bank<3;bank++){
  int x=174+bank*48,n=bank==0?game.pip_sys:bank==1?game.pip_eng:game.pip_wep,active=paused&&pip_sel==bank;
  if(active)rect(x-2,251,40,15,RGB(67,57,32));text_px(x,253,active?WHITE:HC_GOLD,bank==0?"SYS":bank==1?"ENG":"WEP");
  for(int k=0;k<4;k++)rect(x+k*7,264,5,2,k<n?(active?WHITE:HC_CYAN):HC_EDGE);
 }
 rect(333,168,136,11,HC_GOLD);hc_key(333,166,'O',0);text_px(347,170,RGB(9,22,29),"WEAPON SYSTEMS");
 const char*tool=tool_names[tools_selected];text_px(354,183,HC_GOLD,"%s",tool);hc_instrument_icon(338,191,tools_selected,tools_selected==1?RGB(255,146,106):tools_selected==2?HC_CYAN:tools_selected==3?RGB(161,202,255):tools_selected==4?RGB(201,165,255):HC_GOLD);line(340,181,340,186,HC_GOLD);line(340,186,348,186,HC_GOLD);
 if(tools_selected==0)text_px(354,196,game.missiles?HC_GOLD:HC_DIM,"%d LEFT",game.missiles);
 else if(tools_selected==1)text_px(354,196,HC_GOLD,"%d/%d LEFT",game.flare_charges,flare_capacity(&game));
 else text_px(354,196,HC_DIM,"%s",hc_tool_reason(tools_selected));
 if(tools_selected==2)text_px(418,183,HC_DIM,"%dM",(int)tech_tractor_range(&game));
 text_px(426,196,HC_DIM,"M %d",game.missiles);
 unsigned gun_icon_ink=primary_visual_event.owner==&game&&primary_visual_event.serial==game.shots&&game.time-primary_visual_event.time>=0&&game.time-primary_visual_event.time<.18f?twin_weapon_ink(primary_visual_event.weapon):HC_CYAN;line(337,209,465,209,HC_EDGE);line(338,227,362,227,gun_icon_ink);line(338,225,343,223,HC_DIM);line(343,223,356,225,HC_DIM);rect(339,225,5,4,RGB(42,76,85));int item=fit_weapon_item(&game);text_px(338,214,HC_DIM,"GUN");hc_words(370,214,96,tools_confirm_time>0&&tools_weapon_result>0?HC_GOLD:HC_CYAN,fit_weapon_short_name(item));
 const char*labels[]={"SHLD","HULL","HEAT","FUEL"};
 unsigned inks[]={game.energy<30?RED:HC_CYAN,game.hull<35?RED:HC_CYAN,game.heat>70?RED:HC_GOLD,HC_GOLD};
 for(int k=0;k<4;k++){text_px(338,234+k*8,HC_DIM,"%s",labels[k]);hc_segment_meter(377,235+k*8,88,(int)hud_smooth[k],inks[k]);}
}
static void hc_header(void){
 hc_frame(8,2,236,31,HC_EDGE);hc_frame(250,2,224,31,HC_EDGE);if(!hc_output)rear_view_mirror(257,4,209,12);
 int station=radio_display_station();if(station<0||station>=RADIO_STATION_COUNT)station=0;
 const char*name=radio_off?"RADIO OFF":radio_station_name(station);
 text_px(15,7,HC_DIM,"NOW PLAYING:");hc_scroll(119,7,60,HC_GOLD,name,game.time*.5f);
 /* Segmented receiver activity decoration, not a fictitious audio analyser. */
 for(int j=0;j<16;j++){int level=radio_off?0:1+(int)(3.49f*(.5f+.3f*sinf(game.time*6.3f+j*.43f)+.2f*sinf(game.time*3-j*.65f))),peak=level;for(int age=1;age<7&&!radio_off;age++){float t=game.time-age*.033f;int old=1+(int)(3.49f*(.5f+.3f*sinf(t*6.3f+j*.43f)+.2f*sinf(t*3-j*.65f)))-age/2;if(old>peak)peak=old;}for(int band=0;band<4;band++)rect(184+j*3,17-band*3,2,2,band<level?(band==3?HC_GOLD:HC_CYAN):RGB(15,34,41));if(peak>0)line(184+j*3,15-(peak-1)*3,185+j*3,15-(peak-1)*3,RGB(233,222,171));}
 const char*broadcast="";
 if(!radio_off&&!quiet_comms&&comms_captions){
  if(station==5||station==6)broadcast=radio_voice_script(station,radio_voice_station==station?radio_voice_line:0);
  else broadcast=radio_talk_lines[station][((int)(game.time/16.5f)*5+station*3)%24];
 }
 if(!broadcast)broadcast=name;
 char progress[64];if(expedition_header_info(progress,sizeof(progress)))hc_scroll(15,21,216,HC_CYAN,progress,game.time*.6f);else hc_scroll(15,21,216,HC_DIM,broadcast,game.time*.6f);
 if(route_prejump_active()){float p=route_prejump_progress();text_px(257,20,HC_CYAN,"HYPERDRIVE %3d%%",(int)(p*100));route_electric_bar(257,29,208,2,p);}
 else hc_scroll(257,21,208,HC_CYAN,nav_header_cue(),game.time*.4f);

}
static void hc_alerts_and_comms(void){
 int computers=hc_browser||tools_open;
 if(!computers){speech_box(8,35,464);quick_comms_box();}
 else {
  const char*urgent=game.incoming_missile>0?"MISSILE":game.collision>0?"IMPACT":game.attacked>0?"ATTACK":game.heat>78?"HEAT":wanted_level(&game)>0?"LAW":speech_active()?"COMMS":0;
  if(urgent){rect(166,35,74,12,RGB(56,15,22));text_px(170,37,HC_GOLD,"%s",urgent);}

 }
 if(!computers&&combat_alert_active()){rect(166,143,176,12,RGB(56,15,22));text_px(170,145,WHITE,game.incoming_missile>0?"MISSILE INBOUND":"RED ALERT");}
 if(!hc_browser){hc_key(29,139,'T',0);text_px(46,143,HC_GOLD,"COMMS");}
 if(!computers&&wanted_level(&game)>0&&!combat_alert_active()){rect(166,143,176,11,RGB(56,15,22));text_px(170,145,RED,"LAW WANTED %d",wanted_level(&game));}
 if(comms_quick){
  hc_frame(166,70,308,78,HC_GOLD);text_px(180,78,HC_GOLD,"INCOMING COMMS");
  rect(178,98,112,17,comms_quick_choice==0?RGB(46,61,66):HC_BG);rect(314,98,144,17,comms_quick_choice==1?RGB(46,61,66):HC_BG);
  text_px(186,103,HC_DIM,"IGNORE");text_px(322,103,HC_CYAN,"RESPOND");hc_key(180,125,'X',0);text_px(200,128,WHITE,"CONFIRM   D-PAD CHOOSE");
 }
 if(paused){rect(166,35,176,12,RGB(24,43,52));text_px(170,37,HC_GOLD,"POWER: D-PAD ADJUST");}
}
static void hc_canopy_frame(void){hc_material_shell();}
static void cockpit(void){
 if(!hc_presentation_context()){cockpit_legacy();return;}
 if(!hc_full_console_visible())return;
 hc_motion_tick();hc_validate_lock();float previous=sensor_hud_range;sensor_hud_range=-1;sensor_hud_range=sensor_range();if(!hc_output)hud_animation_tick();
 if(!hc_output)hud_canopy_feedback();hc_canopy_frame();uint64_t stamp=sceKernelGetSystemTimeWide();hc_header();hc_profile_us[0]+=sceKernelGetSystemTimeWide()-stamp;
 stamp=sceKernelGetSystemTimeWide();hc_target_card();hc_profile_us[1]+=sceKernelGetSystemTimeWide()-stamp;
 stamp=sceKernelGetSystemTimeWide();hc_ship_instruments();hc_profile_us[2]+=sceKernelGetSystemTimeWide()-stamp;
 stamp=sceKernelGetSystemTimeWide();hc_alerts_and_comms();hc_profile_us[3]+=sceKernelGetSystemTimeWide()-stamp;hc_profile_frames++;sensor_hud_range=previous;
}
