static void sector_background(void){
 /* Shared 1950s-cover field: broad stepped colour planes first, then the
  * softer nebula/star passes. Keep the centre quiet for the reticle and HUD;
  * the large side mass and tiny relay give the frame a period illustration
  * composition without becoming gameplay geometry. */
 const unsigned colors[]={RGB(8,12,27),RGB(27,12,32),RGB(7,30,35),RGB(36,18,10),RGB(15,23,45),RGB(40,17,26),RGB(12,20,23),RGB(32,20,10)};
 unsigned seed=game.bodies[0].seed^(unsigned)game.system*0x9e3779b9u;
 unsigned raw=colors[game.system%8];
 float zone=sinf((game.pos.x+game.pos.z)*.00007f+(seed&255)*.03f);
 unsigned base=RGB((int)((raw&255)*(.78f+(zone+1)*.12f)),(int)(((raw>>8)&255)*(.78f+(zone+1)*.12f)),(int)(((raw>>16)&255)*(.78f+(zone+1)*.12f)));
 int top=view_top(),bot=view_bot();
 rect(0,top,W,bot-top+1,base);
 /* Three stepped bands create the inked sky/painted wash of a cover, while
    staying cheap enough for the PSP framebuffer and high-contrast mode. */
 unsigned upper=RGB((base&255)/2,((base>>8)&255)/2,((base>>16)&255)/2);
 unsigned lower=RGB((int)((base&255)*1.12f),(int)(((base>>8)&255)*1.08f),(int)(((base>>16)&255)*.94f));
 int horizon=top+(bot-top)*3/5;
 for(int y=top;y<horizon;y+=4)rect(0,y,W,4,mix_rgb(upper,base,(y-top)/(float)fmaxf(1,horizon-top)));
 for(int y=horizon;y<=bot;y+=4)rect(0,y,W,4,mix_rgb(base,lower,(y-horizon)/(float)fmaxf(1,bot-horizon)));
}
static Vec3 station_vertex(Vec3 v){return add(rotate(v,0,station_angle(&game)),(Vec3){0,0,3500});}
static void station_model(void){
 shipmesh(mesh_id("CORIOLIS"),(Vec3){0,0,3500},0,station_angle(&game),1,RGB(170,192,202),0);
 Vec3 outer[4]={{0,-160,-160},{160,0,-160},{0,160,-160},{-160,0,-160}};
 Vec3 inner[4];for(int i=0;i<4;i++)inner[i]=station_port_corner(i);
 for(int i=0;i<4;i++){int j=(i+1)%4;Vec3 a=camera(&game,station_vertex(outer[i])),b=camera(&game,station_vertex(outer[j])),c=camera(&game,station_vertex(inner[j])),d=camera(&game,station_vertex(inner[i]));queue_triangle(a,b,c,RGB(109,140,151));queue_triangle(a,c,d,RGB(109,140,151));Vec3 backi=inner[i],backj=inner[j];backi.z=0;backj.z=0;queue_triangle(d,c,camera(&game,station_vertex(backj)),RGB(8,19,24));queue_triangle(d,camera(&game,station_vertex(backj)),camera(&game,station_vertex(backi)),RGB(8,19,24));}
  if(prosperity(&game,game.system)>=4){for(int ring=0;ring<2;ring++){float radius=230+ring*65;for(int i=0;i<24;i++){float a=station_angle(&game)+i*6.2831853f/24,b=station_angle(&game)+(i+1)*6.2831853f/24;Vec3 u=camera(&game,(Vec3){cosf(a)*radius,sinf(a)*radius,3500+(ring?100:-80)}),v=camera(&game,(Vec3){cosf(b)*radius,sinf(b)*radius,3500+(ring?100:-80)});if(u.z>15&&v.z>15){Point p=project(u),q=project(v);line((int)p.x,(int)p.y,(int)q.x,(int)q.y,ring?RGB(85,212,212):RGB(155,154,165));}}}}
}
/* Practical station windows — soft warm lamps only (no glitter masks). */
static void station_window_animation(void){
 if(high_contrast)return;
 int top=clipy0>=0?clipy0:view_top(),bot=clipy1>=0?clipy1-1:view_bot();
 int count=prosperity(&game,game.system)>=4?12:6;
 for(int i=0;i<count;i++){
  float a=station_angle(&game)+(i+.5f)*6.2831853f/count;
  float radius=214.f+(i&1)*38.f;
  Vec3 v=camera(&game,(Vec3){cosf(a)*radius,sinf(a)*radius,3500.f+((i&3)-1)*72.f});
  if(v.z<20)continue;
  Point p=project(v);if(p.x<2||p.x>=W-2||p.y<top||p.y>bot)continue;
  unsigned c=((i+(int)(game.time*1.4f))&3)?RGB(60,40,22):RGB(90,58,28);
  sfx_add((int)p.x,(int)p.y,c,top,bot);
 }
}
static void secondary_hubs(void);
#include "menu-ship-preview.h"
static void ambient_space(void){
 if(system_whales(game.system)){Body *b=&game.bodies[3];for(int i=0;i<3;i++){float a=game.time*.028f+i*.62f;Vec3 pos=add(b->pos,(Vec3){cosf(a)*(b->radius+5600),700+sinf(a+i)*.5f*480,sinf(a)*(b->radius+5600)});if(length(sub(pos,game.pos))<14000)shipmesh(mesh_id("WORM"),pos,a+1.57f,sinf(game.time*.35f+i)*.16f,7.2f+i*1.3f,RGB(96,186,198),0);}}
 if(system_comet(game.system)){float a=game.time*.018f;Vec3 pos={cosf(a)*17000,1800,sinf(a)*17000};if(length(sub(pos,game.pos))<12000){shipmesh(mesh_id("BOULDER"),pos,a,a*.3f,1.4f,RGB(210,230,240),0);Vec3 tail=add(pos,(Vec3){sinf(a)*900,-200,-cosf(a)*900});Vec3 u=camera(&game,pos),v=camera(&game,tail);if(u.z>30&&v.z>30){Point p=project(u),q=project(v);if(p.y>view_top()&&p.y<view_bot()&&q.y>view_top()&&q.y<view_bot()){line((int)p.x,(int)p.y,(int)q.x,(int)q.y,RGB(85,160,200));if(!high_contrast){int top=view_top(),bot=view_bot();for(int k=0;k<5;k++){float t=k/4.f;int x=(int)(p.x+(q.x-p.x)*t),y=(int)(p.y+(q.y-p.y)*t);sfx_add(x,y,RGB(50,90,120),top,bot);}}}}}}
}
/* Speed lines scale with actual velocity: a quiet hint at cruise, a dense
 * tunnel of streaks during boost. */
static void speed_lines(void){
 if(game.speed<player_ships[game.ship].speed*.18f||game.dock_stage||game.jump>0||game.dead)return;
 float normal=game.speed/fmaxf(1,player_ships[game.ship].speed),power=fminf(1,normal/(game.boost?20.f:1.15f));
 int top=view_top(),bottom=view_bot();
 int count=12+(int)(power*36.f);
 unsigned edge=game.boost?RGB(50,130,170):RGB(48,86,112);
 for(int i=0;i<count;i++){
  float a=i*2.39996f;float r=110+fmodf(i*37+game.time*(game.boost?520.f:180.f),160);float trail=6+power*46;
  int x=240+(int)(cosf(a)*r),y=110+(int)(sinf(a)*r*.5f);
  int xx=240+(int)(cosf(a)*(r+trail)),yy=110+(int)(sinf(a)*(r+trail)*.5f);
  if(y>top&&y<bottom&&yy>top&&yy<bottom){sfx_add(x,y,edge,top,bottom);sfx_add(xx,yy,game.boost?RGB(85,180,205):RGB(60,110,135),top,bottom);}
 }
}
static void engine_flare(void){
 if(game.speed<80||game.dock_stage||game.jump>0||game.dead)return;
 int bottom=view_bot();if(bottom<150)return;float normal=game.speed/fmaxf(1,player_ships[game.ship].speed),power=game.boost?fminf(1,normal/20):fminf(1,normal);
 int pulse=(int)(sinf(game.time*(game.boost?18:9))*2),length=(game.boost?18:7)+(int)(power*10)+pulse;if(length<3)length=3;
 unsigned core=game.boost?CYAN:AMBER,edge=game.boost?RGB(45,120,170):RGB(150,92,28);int y=bottom-6;
 for(int i=0;i<length;i++){int w=game.boost?2+(i%3):1+(i%2),yy=y+i,spread=(i*2)/3+1;rect(240-w-spread,yy,w,1,edge);rect(240+spread,yy,w,1,edge);if((i&1)==0)pixel(240,yy,core);}line(240-length/2,y-1,240+length/2,y-1,edge);
 space_anim_draw(SPACE_ANIM_PLUME,240,y+length/2,(int)(game.time*12.f),core);
}
static void secondary_hubs(void){
 for(int i=1;i<HUB_COUNT;i++){
  Vec3 p=hub_position(&game,i);float d=length(sub(p,game.pos));if(d>30000||occluded(p))continue;
  unsigned c=i==1?RGB(170,192,202):RGB(150,110,180);
  shipmesh(mesh_id("CORIOLIS"),p,0,station_angle(&game)+(i*1.7f),.62f,c,0);
 }
}
/* Station-local traffic glitter removed — read as hull lights, not sparkle dots. */
static void station_traffic_glints(void){}
static void docking_view(void){
 if(game.dock_stage==3){
  rect(0,23,W,195,RGB(8,13,24));
  rect(40,70,400,2,RGB(193,139,77));
  text(18,10,RGB(229,210,163),"DOCKING COMPLETE");
  text(12,14,WHITE,"Welcome to %s",station_name(&game));
  text(14,18,RGB(155,154,165),"Opening station services...");
  return;
 }
 Vec3 oldpos=game.pos;float oldyaw=game.yaw,oldpitch=game.pitch,oldroll=game.roll;
 game.pos=(Vec3){260,120,2820};Vec3 aim=norm(sub((Vec3){0,0,3400},game.pos));
 game.yaw=atan2f(aim.x,aim.z);game.pitch=asinf(aim.y);game.roll=0;
 sector_background();space_fx_nebula();starfield();
 station_model();station_window_animation();
 float t=fminf(1,game.dock_timer/3);
 shipmesh(mesh_id(player_ships[game.ship].name),(Vec3){0,0,3070+t*470},0,station_angle(&game),.7f,RGB(193,139,77),0);
 flush_meshes();
 station_entrance();
 /* Soft approach haze only — no glitter stars in the corridor. */
 if(!high_contrast){
  int top=view_top(),bot=view_bot();
  for(int i=0;i<8;i++){
   float a=i*.52f+game.time*.5f;
   int x=240+(int)(cosf(a)*(20+t*40)),y=110+(int)(sinf(a)*(10+t*18));
   sfx_add(x,y,RGB(40,32,22),top,bot);
  }
 }
 text(2,5,RGB(229,210,163),"ARRIVAL / %.16s",station_name(&game));
 game.pos=oldpos;game.yaw=oldyaw;game.pitch=oldpitch;game.roll=oldroll;
}
/* Edge-band captions. Body text is word wrapped to actual 8px cell capacity. */
static void speech_box(int x,int y,int w){
 const char *s=0;int who=VOICE_COMP;
 if(!quiet_comms&&speech_active()){s=game.voice;who=game.voice_who;}
 /* During combat, status text stays on the bottom RED ALERT banner — not here. */
 else if(game.message_time>0&&game.message[0]&&!combat_alert_active())s=game.message;
 if(!s)return;
 if(who<VOICE_KEI||who>VOICE_CONTACT)who=VOICE_COMP;
 int role=who==VOICE_LAW?LAW:who==VOICE_KEI?EXPLORERS:TRADERS;
 if(who==VOICE_CONTACT)role=game.voice_role>=0&&game.voice_role<FACTION_COUNT?game.voice_role:EXPLORERS;
 static const char *names[]={"","KEI","VENN","DOCKHAND","LOCAL LAW","COMPUTER","CONTACT"};
 /* Computer voice uses soft cyan; named speakers keep faction ink on charcoal plate. */
 unsigned ink=who==VOICE_COMP?RGB(85,212,212):faction_colors[role];
 int portrait=who!=VOICE_COMP,tx=x+(portrait?40:8),col=tx/8,cap=(x+w-8-tx)/8;
 /* Three body rows under the name plate stay inside the top caption band (y<56). */
 int box_h=32;
 rect(x,y,w,box_h,RGB(21,28,39));rect(x,y,w,1,RGB(193,139,77));rect(x,y+box_h-1,w,1,RGB(41,54,70));
 rect(x,y,2,box_h,ink);
 if(portrait){if(who==VOICE_KEI)draw_kei(x+4,y,32,0);else draw_portrait(x+4,y,32,32,who==VOICE_CONTACT?game.voice_seed:who*37,role);}
 speaker_name_tag(col,y/8,who==VOICE_CONTACT?faction_names[role]:names[who],ink);
 button_icon(x+w-16,y,'T',ink);
 if(encounter_requires_reply(&game))text((x+w-56)/8,y/8,RGB(240,180,91),"HOLD");
 else text((x+w-56)/8,y/8,RGB(120,245,220),"CLOSE");
 text_wrap(col,y/8+1,cap,3,RGB(229,210,163),s,0);
}
static void quick_comms_box(void){
 if(!comms_quick||game.encounter_kind==ENCOUNTER_NONE)return;
 int x=96,y=78,w=288,h=30;rect(x,y,w,h,RGB(10,18,35));rect(x,y,w,2,RGB(240,180,91));rect(x,y+h-2,w,2,RGB(41,54,70));
 rect(112,y+8,104,14,comms_quick_choice==0?RGB(66,45,38):RGB(25,35,45));
 rect(264,y+8,104,14,comms_quick_choice==1?RGB(25,65,77):RGB(25,35,45));
 text(16,11,comms_quick_choice==0?RGB(240,180,91):RGB(155,154,165),"[ IGNORE ]");
 text(35,11,comms_quick_choice==1?RGB(240,180,91):RGB(155,154,165),"[ RESPOND ]");
}
static void pip_bar(int x,int y,int w,int h,int fill,unsigned c){
 if(fill<0)fill=0;
 if(fill>100)fill=100;
 rect(x,y,w,h,RGB(32,14,6));int fw=w*fill/100;if(fw>0)rect(x,y,fw,h,c);rect(x,y,w,1,AMBDIM);
}
static void target_segments(int x,int y,int segments,int filled,unsigned ink){
 if(filled<0)filled=0;if(filled>segments)filled=segments;
 for(int i=0;i<segments;i++){
  unsigned c=i<filled?ink:RGB(41,54,70);
  rect(x+i*7,y,5,4,c);
  if(i<segments-1)pixel(x+i*7+5,y+1,RGB(21,28,39));
 }
}
static void target_condition(int x,int y,int id){
 if(!IS_NPC_ID(id)||!scanner_known(id))return;
 NPC *n=&game.npc[id-BODY_COUNT-1];
 float max_h=n->freighter?900.f:n->role==LAW?110.f:80.f;
 float max_s=n->freighter?100.f:n->role==LAW?60.f:40.f;
 int hull=(int)fmaxf(0,fminf(100,100.f*n->health/max_h));
 int shield=(int)fmaxf(0,fminf(100,100.f*n->shield/fmaxf(1.f,max_s)));
 text(x/8,y/8,RGB(155,154,165),"H");target_segments(x+10,y,5,(hull+19)/20,hull<30?RED:RGB(85,212,212));
 text((x+54)/8,y/8,RGB(155,154,165),"S");target_segments(x+64,y,5,(shield+19)/20,RGB(85,212,212));
}
static unsigned target_overlay_color(int id){
 if(is_mission_target(&game,id))return WHITE;
 if(IS_NPC_ID(id))return faction_colors[game.npc[id-BODY_COUNT-1].role];
 if(IS_ANOMALY_ID(id))return GOLD;
 if(IS_DEBRIS_ID(id))return DIM;
 return id==0?CYAN:GOLD;
}
static void targeting_overlay(int x,int y,int w,int h,int detailed){
 int ids[1+BODY_COUNT+NPC_COUNT+DEBRIS_COUNT+ANOMALY_COUNT],n=collect_scan_ids(ids,scan_cat);
 int shown=n>5?5:n;
 rect(x,y,w,h,RGB(10,18,29));rect(x,y,w,1,RGB(193,139,77));rect(x,y+h-1,w,1,RGB(41,54,70));
 text(x/8+1,y/8+1,RGB(229,210,163),detailed?"TARGET COMPUTER":"TARGETS");
 text(x/8+1,y/8+3,RGB(240,180,91),"< BAND: %.10s >",scan_cat_names[scan_cat<0?0:scan_cat>4?4:scan_cat]);
 if(!n){text(x/8+1,y/8+6,DIM,"NO CONTACTS");return;}
 int first=0;
 for(int i=0;i<n;i++)if(ids[i]==selected_target){first=i-2;break;}
 if(first<0)first=0;if(first>n-shown)first=n-shown;
 if(n>shown){
  int track_y=y+42,track_h=80;
  int thumb_h=(track_h*shown)/n;if(thumb_h<8)thumb_h=8;
  int thumb_y=track_y+(n==shown?0:(track_h-thumb_h)*first/(n-shown));
  /* Keep the scroll rail on the bezel side; the right edge is reserved for
   * each contact's distance readout. */
  rect(x+4,track_y,3,track_h,RGB(41,54,70));
  rect(x+4,thumb_y,3,thumb_h,RGB(240,180,91));
 }
 for(int i=0;i<shown;i++){
  int id=ids[first+i],yy=y+42+i*16,row_y=(yy/8)*8;
  /* Text is snapped to 8px font rows. Use that exact snapped origin for
   * the selection rule; otherwise 14px list spacing makes it drift. */
  if(id==selected_target)rect(x+8,row_y-2,w-11,12,RGB(25,65,77));
  unsigned ink=id==selected_target?RGB(240,180,91):target_overlay_color(id);
  text(x/8+2,row_y/8,ink,"%c %.13s",id==selected_target?'>':' ',scanner_known(id)?target_name(id):"UNKNOWN");
  text((x+w-43)/8,row_y/8,RGB(229,210,163),"%4dm",(int)length(sub(target_position(id),game.pos)));
 }
 if(detailed&&valid_target(selected_target)){
  int cy=y+h-37;
  text(x/8+1,cy/8,RGB(240,180,91),"%.21s",target_name(selected_target));
  target_condition(x+8,cy+10,selected_target);
 }
}
static unsigned dim_rgb(unsigned c,int num,int den){
 int r=((c&255)*num)/den,g=(((c>>8)&255)*num)/den,b=(((c>>16)&255)*num)/den;
 if(r>255)r=255;
 if(g>255)g=255;
 if(b>255)b=255;
 return RGB(r,g,b);
}
/* Soft bloom on the space canopy only — never after UI glyphs.
 * Header/instrument chrome stays crisp; bloom is for suns/engines/haze. */
static void hud_postfx(void){
 if(high_contrast)return;
 /* Keep bloom off the 24px header band so mission cue / system text stay sharp. */
 int top=view_top();if(top<24&&!hud_hidden)top=24;
 int bot=view_bot();if(bot>191&&hud_mode==0)bot=191;
 if(bot<=top+6)return;
 for(int y=top;y<=bot;y++)for(int x=0;x<8;x++){
  fb[y*STRIDE+x]=dim_rgb(fb[y*STRIDE+x],24+x,32);
  fb[y*STRIDE+W-1-x]=dim_rgb(fb[y*STRIDE+W-1-x],24+x,32);
 }
 /* Sparse bloom — high threshold so cream/amber HUD ink never blooms. */
 for(int y=top+3;y<=bot-3;y+=3)for(int x=3;x<W-3;x+=3){
  unsigned c=fb[y*STRIDE+x];int r=c&255,g=(c>>8)&255,b=(c>>16)&255,lum=r+g+b;
  if(lum<620)continue;
  for(int dy=-2;dy<=2;dy++)for(int dx=-2;dx<=2;dx++){
   int dist=dx*dx+dy*dy;if(dist==0||dist>8)continue;
   int yy=y+dy,xx=x+dx;if(yy<top||yy>bot)continue;unsigned d=fb[yy*STRIDE+xx];
   int fall=5-dist/2;if(fall<1)fall=1;
   int nr=((d&255)*6+(r*fall)/5)/6,ng=(((d>>8)&255)*6+(g*fall)/5)/6,nb=(((d>>16)&255)*6+(b*fall)/5)/6;
   if(nr>255)nr=255;if(ng>255)ng=255;if(nb>255)nb=255;
   fb[yy*STRIDE+xx]=RGB(nr,ng,nb);
  }
 }
}
/* Small decorative flight display. It replaces the old heading/danger glyph
 * cluster without adding a second HUD panel. All marks are native pixels and
 * the pattern state is derived from game.time, so there is no allocation or
 * per-frame state to save. */
static void flight_activity_display(void){
 float vmax=fmaxf(1.f,(float)player_ships[game.ship].speed);
 float ratio=game.speed/vmax;
 float rate=0.35f+fminf(2.4f,ratio*1.15f)+(game.boost?3.2f:0.f);
 float phase=game.time*rate;
 int pattern=((int)(game.time/9.f))%5;
 int x0=128,y0=3,w=128,h=16,cx=192,cy=11;
 int danger=danger_rating(&game,game.system);
 unsigned ink=game.boost?RGB(85,212,212):ratio>1.05f?RGB(240,180,91):RGB(139,184,198);
 unsigned dim=dim_rgb(ink,2,5);
 unsigned edge=danger>=4?RED:RGB(41,54,70);
 rect(x0,y0,w,1,edge);rect(x0,y0+h,w,1,RGB(41,54,70));
 for(int i=0;i<5;i++)pixel(x0+3+i*7,y0+4,(danger>=4&&i<danger)?RED:(i==pattern)?ink:dim);
 if(game.boost){
  for(int i=0;i<14;i++){int sx=x0+10+(int)fmodf(i*19+phase*28,108.f);int sy=y0+3+(i*7)%10;line(sx,sy,sx-5-(int)(ratio*2),sy,dim_rgb(ink,1+(i&1),3));}
  return;
 }
 if(pattern==0){ /* Drifting star scan. */
  for(int i=0;i<15;i++){int sx=x0+8+(int)fmodf(i*23+phase*(4+i%3),112.f);int sy=y0+3+(i*11)%10;pixel(sx,sy,i%5==0?WHITE:ink);if(i%6==0)pixel(sx-1,sy,dim);}
 }else if(pattern==1){ /* Radar sweep and contact dots. */
  circle(cx,cy,7,dim);line(cx,cy,cx+(int)(cosf(phase)*7),cy+(int)(sinf(phase)*7),ink);
  for(int i=0;i<5;i++){float a=i*1.37f;int px=cx+(int)(cosf(a)*((i&1)?5:3)),py=cy+(int)(sinf(a)*((i&1)?5:3));pixel(px,py,i==((int)phase%5)?WHITE:ink);}
 }else if(pattern==2){ /* Tiny telemetry waveform. */
  int lastx=x0+8,lasty=cy+(int)(sinf(phase)*3);
  for(int i=1;i<28;i++){int px=x0+8+i*4,py=cy+(int)(sinf(phase+i*.72f)*((i%7==0)?5:3));line(lastx,lasty,px,py,ink);lastx=px;lasty=py;}
 }else if(pattern==3){ /* Rotating planet / galaxy schematic. */
  circle(cx,cy,6,dim);int ex=cx+(int)(cosf(phase)*6),ey=cy+(int)(sinf(phase)*3);line(cx-8,cy,cx+8,cy,dim);line(cx,cy,ex,ey,ink);pixel(cx+(int)(cosf(phase*1.7f)*9),cy+(int)(sinf(phase*1.7f)*5),WHITE);
 }else { /* Slow ship silhouette with engine pulse. */
  line(cx-12,cy,cx-4,cy-3,ink);line(cx-4,cy-3,cx+10,cy,ink);line(cx+10,cy,cx-4,cy+3,ink);line(cx-4,cy+3,cx-12,cy,ink);line(cx-4,cy-3,cx-4,cy+3,dim);pixel(cx-15-(int)(fabsf(sinf(phase))*4),cy,game.boost?WHITE:dim);
 }
}
/* Session-long text radio: a cheap marquee beside the activity strip. The
 * station voices are deliberately short, strange and varied so the cockpit
 * feels inhabited without needing another audio stream or PSP UI panel. */
static const char *radio_talk_lines[6][24]={
 {"DEEP FIELD: Tonight, moons answer questions with gravity.","DEEP FIELD: The quiet between stars is not empty. It is listening.","DEEP FIELD: Our guest insists comets have excellent memories.","DEEP FIELD: Please do not name a wormhole after your ex.","DEEP FIELD: Scientists confirm the signal was definitely not a sneeze.","DEEP FIELD: Three stars, one cup, and a very long night ahead.","DEEP FIELD: We now take calls from anyone outside normal space.","DEEP FIELD: If the sky blinks, remain calm and write it down.","DEEP FIELD: Today's forecast is radiant with a chance of radiation.","DEEP FIELD: The universe is expanding. Please keep your elbows in.","DEEP FIELD: A black hole called in to complain about its personal space.","DEEP FIELD: Tonight's caller says destiny is just very confident navigation.","DEEP FIELD: Research update: nobody has found the end of Tuesday.","DEEP FIELD: The observatory has upgraded its telescope and its snacks.","DEEP FIELD: If your moon follows you home, contact a professional.","DEEP FIELD: Guest debate: are stars burning, or simply overachieving?","DEEP FIELD: We apologise for the earlier eclipse. It was a scheduling error.","DEEP FIELD: A wormhole is not a shortcut if you forget where you parked.","DEEP FIELD: Listeners report a mysterious hum. Experts report a second hum.","DEEP FIELD: Tonight's prize is a certificate proving you saw something.","DEEP FIELD: The galaxy is vast, but the studio cupboard is somehow smaller.","DEEP FIELD: Please keep all existential questions after the station ident.","DEEP FIELD: Our guest claims aliens invented awkward silence.","DEEP FIELD: Stay tuned for the weather, traffic and one impossible constellation."},
 {"NEON TRANSIT: Welcome back, night pilots and daytime smugglers.","NEON TRANSIT: Our traffic report says the fast lane is mostly pirates.","NEON TRANSIT: Listener poll: best planet name? We accept bribes.","NEON TRANSIT: A station clerk has declared war on loose paperwork.","NEON TRANSIT: Today's advice: never race a courier with nothing to lose.","NEON TRANSIT: We play the hits, the misses, and one suspicious distress call.","NEON TRANSIT: Someone left a goldfish in dock seven. It wants a pilot.","NEON TRANSIT: Local law says this joke is still under investigation.","NEON TRANSIT: The next song is sponsored by three identical moon shops.","NEON TRANSIT: Keep your engines cool and your opinions warmer.","NEON TRANSIT: Live now: two aliens, one couch and a disagreement about parking.","NEON TRANSIT: Advert break: buy a moon, regret the paperwork later.","NEON TRANSIT: Our caller says their ex stole the family cargo hauler.","NEON TRANSIT: The audience votes to forgive the pirate. The pirate votes no.","NEON TRANSIT: Sponsored by Emergency Apology Insurance for accidental lasers.","NEON TRANSIT: Tonight's guest brought receipts, witnesses and three lawyers.","NEON TRANSIT: The station lift is stuck again. Please use the emotional lift.","NEON TRANSIT: Call now if your neighbour is secretly a disguised moon.","NEON TRANSIT: New dating service matches pilots by fuel efficiency.","NEON TRANSIT: A listener asks if fines count as souvenirs. Law says no.","NEON TRANSIT: We interrupt this argument for a very short advertisement.","NEON TRANSIT: Buy one docking permit, get the second one emotionally free.","NEON TRANSIT: Our host has been advised not to mention the incident. Here it is.","NEON TRANSIT: Keep your engines cool and your opinions warmer."},
 {"PIXEL COMET: Tiny rocks, enormous consequences, excellent radio.","PIXEL COMET: Mining tip: the shiny one is rarely the friendly one.","PIXEL COMET: We asked an asteroid how it felt. It gave us a hard answer.","PIXEL COMET: Tonight's guest is a mineral with a very low voice.","PIXEL COMET: Space dust gets everywhere. Especially in the microphone.","PIXEL COMET: A rock and a hard place walk into a docking bay.","PIXEL COMET: Comet etiquette: wave first, scoop later.","PIXEL COMET: We are broadcasting from somewhere with no return address.","PIXEL COMET: If your scanner says nothing, ask the rock again.","PIXEL COMET: Today's forecast: crunchy with pockets of vacuum.","PIXEL COMET: Advertisement: rent a mining laser, return it mostly intact.","PIXEL COMET: Our guest claims asteroids have no feelings. It then threw a moon.","PIXEL COMET: A prospector called in from a hole with excellent acoustics.","PIXEL COMET: Mineral dating advice: never say you are looking for something shiny.","PIXEL COMET: The rock cycle is just geology's longest talk show.","PIXEL COMET: Today's caller wants to know if cargo can be emotionally fragile.","PIXEL COMET: Buy a crate of ore, receive a free warning label.","PIXEL COMET: We asked a crystal for advice. It was remarkably clear.","PIXEL COMET: Asteroid etiquette update: no tailgating in the belt.","PIXEL COMET: A miner's helmet is not formal wear, despite what the advert says.","PIXEL COMET: Our studio wall is now technically a mineral sample.","PIXEL COMET: If it sparkles, scan it. If it screams, leave.","PIXEL COMET: Tonight's debate: rock, stone or very patient planet?","PIXEL COMET: Stay tuned for more hard news from soft microphones."},
 {"VELVET ORBIT: Slow down, breathe out, and admire that gas giant.","VELVET ORBIT: Our guest says luxury is having a working cooling fan.","VELVET ORBIT: A gentle reminder: docking is a dance, not a collision.","VELVET ORBIT: Tonight we discuss poetry, propulsion, and bad insurance.","VELVET ORBIT: Someone has put a tiny hat on the station beacon.","VELVET ORBIT: The calmest pilot is usually the one with fuel left.","VELVET ORBIT: We accept dedications from ships still in one piece.","VELVET ORBIT: Beauty tip: polished hulls reflect fewer regrets.","VELVET ORBIT: Our horoscope says avoid suspicious cargo today.","VELVET ORBIT: Stay soft, stay curious, and mind the approach vector.","VELVET ORBIT: Tonight's panel asks: is a yacht still a yacht with tractor damage?","VELVET ORBIT: Advertise your luxury cabin before the neighbours do.","VELVET ORBIT: We welcome a caller who has feelings about station carpet.","VELVET ORBIT: Fine dining tip: never ask what the sauce was before docking.","VELVET ORBIT: A host, a diplomat and a space slug enter a quiet lounge.","VELVET ORBIT: Sponsored by Soft Landing, the apology service for hard arrivals.","VELVET ORBIT: Your aura is calm. Your engine temperature disagrees.","VELVET ORBIT: Listener confession: they polished the cargo bay instead of sleeping.","VELVET ORBIT: Today's meditation is sponsored by a very loud compressor.","VELVET ORBIT: Our guest says romance is sharing the last fuel scoop.","VELVET ORBIT: The station beacon is wearing the hat again.","VELVET ORBIT: Luxury is a clean visor and a dock that says welcome.","VELVET ORBIT: Please remember: serenity does not stop missiles.","VELVET ORBIT: Stay soft, stay curious, and mind the approach vector."},
 {"FAR HORIZONS: Greetings, travellers. Your stars are behaving beautifully.","FAR HORIZONS: Tonight's alien panel asks whether humans dream in maps.","FAR HORIZONS: A distant voice says hello. It may be three systems away.","FAR HORIZONS: We discuss old Earth recipes and very new black holes.","FAR HORIZONS: The best route is not always the shortest. Sometimes it sings.","FAR HORIZONS: Listener question: can a nebula be homesick? We think yes.","FAR HORIZONS: Our guest has crossed a thousand suns and lost one shoe.","FAR HORIZONS: Please enjoy the quiet glow of the next horizon.","FAR HORIZONS: A pilot reports finding hope between two unremarkable stars.","FAR HORIZONS: Keep exploring. The dark has more stories than maps.","FAR HORIZONS: Live from the void: an alien couple argues about who named the moon.","FAR HORIZONS: Advertisement: insure your memories before visiting a time anomaly.","FAR HORIZONS: Tonight's guest says humans are adorable when they over-explain maps.","FAR HORIZONS: A three-eyed caller asks whether destiny accepts return journeys.","FAR HORIZONS: We discuss old Earth recipes, new black holes and a missing spoon.","FAR HORIZONS: Listener mail: can a star be lonely? Our panel says it depends.","FAR HORIZONS: Sponsored by Galactic Mediation for disputes between moons.","FAR HORIZONS: An alien poet joins us after accidentally inventing a new colour.","FAR HORIZONS: The quietest signal in space may simply be a very polite caller.","FAR HORIZONS: Tonight's debate: are humans brave, or just curious near danger?","FAR HORIZONS: A traveller found a shortcut and returned with excellent eyebrows.","FAR HORIZONS: If the horizon calls your name, ask which horizon.","FAR HORIZONS: Our studio guest has seven opinions and no visible mouth.","FAR HORIZONS: Keep exploring. The dark has more stories than maps."},
 {"CROSS-LING: Krru-vaa. Krru-vaa. The channel is awake.","CROSS-LING: Shaa? Tekk-takk. No, that was not a distress call.","CROSS-LING: Vrr-oo, vrr-oo, kha. Three mouths, one translator.","CROSS-LING: We asked the nest for traffic news. It answered in clicks.","CROSS-LING: Oruu-eh. A soft call from something with too many eyes.","CROSS-LING: The translator agrees this means hello, probably.","CROSS-LING: Chik-chik-raa. Please keep your antennae inside the ship.","CROSS-LING: Listener sample: mrr-ah, mrr-ah, gruu. Beautifully inconclusive.","CROSS-LING: A long-distance purr is crossing the static now.","CROSS-LING: Kha-kha-voom. The local flock has opinions about your engine.","CROSS-LING: No words today. Just beaks, throats and a little weather.","CROSS-LING: Sss-ora-ket. That phrase translates as watch the bright moon.","CROSS-LING: Two callers overlap. One is chirping. One is definitely aquatic.","CROSS-LING: The brood signal rises, folds and forgets what it meant.","CROSS-LING: Rruu-rru-rru. A lullaby from a station nobody charts.","CROSS-LING: Translator note: emotional clicking is not a navigation command.","CROSS-LING: Kett-oi, kett-oi. The reply came from inside the asteroid.","CROSS-LING: A cave-dweller has joined the broadcast. Please do not feed it.","CROSS-LING: Vaa-tek-shuu. We think that was a joke about humans.","CROSS-LING: The signal has feathers, scales or both. We remain respectful.","CROSS-LING: Soft trill, hard rattle, sudden squeak. That is the whole bulletin.","CROSS-LING: A distant pack is harmonising across three light-minutes.","CROSS-LING: Krru-vaa returns your greeting with seventeen corrections.","CROSS-LING: Stay curious. The galaxy is making sounds before it makes sense."}
};
static void radio_ticker_display(void){
 int x=260,w=216;rect(x,1,w,21,RGB(21,28,39));rect(x,1,w,1,RGB(41,54,70));
 if(radio_off){text(33,1,DIM,"RADIO OFF");return;}
 int station=radio_station<0?0:radio_station>=RADIO_STATION_COUNT?RADIO_STATION_COUNT-1:radio_station;
 int segment=(int)(game.time/16.5f),line_index=(segment*5+station*3)%24;const char *broadcast=radio_talk_lines[station][line_index];int len=(int)strlen(broadcast),cycle=len+28;float in=fmodf(game.time,16.5f),pause=.45f+((segment*11+station*7)%5)*.22f;int pos=in<pause?-28:(int)((in-pause)*7.f)%cycle;char shown[29];
 for(int i=0;i<28;i++){int src=pos-28+i;shown[i]=(src>=0&&src<len)?broadcast[src]:' ';}shown[28]=0;
 /* Keep the scrolling ticker on its own line. The visualizer lives below it
  * so the moving copy is never crossed by a waveform. */
 unsigned ink=station==5?RGB(218,142,255):station==4?RGB(85,212,212):RGB(229,210,163);
 /* Tiny but readable talking-host icon sits directly left of the ticker.
  * Four little expressions sell the illusion of a live presenter. */
 int face=(int)(game.time*2.2f)%4;
 /* The host sits on the ticker baseline: five pixels lower keeps its mouth
  * level with the moving copy instead of floating above it. */
 rect(280,7,12,9,RGB(14,18,28));rect(281,8,10,7,ink);
 if(face==3){line(283,10,285,10,RGB(14,18,28));line(287,10,289,10,RGB(14,18,28));}
 else {rect(283,10,2,2,RGB(14,18,28));rect(287,10,2,2,RGB(14,18,28));}
 if(face==0){rect(284,13,4,1,RGB(14,18,28));}
 else if(face==1){rect(284,12,4,2,RGB(14,18,28));}
 else if(face==2){rect(285,12,2,2,RGB(14,18,28));}
 else {line(284,13,288,13,RGB(14,18,28));}
 /* A longer, centred rail lives beneath the ticker. Bars breathe slowly and
  * crossfade between station colours without touching the scrolling text. */
 unsigned eq_a=station==5?RGB(218,142,255):station==4?RGB(85,212,212):RGB(90,165,255),eq_b=station==5?RGB(110,220,185):station==4?RGB(190,125,245):RGB(240,180,91);
 for(int i=0;i<28;i++){int h=2+(int)((sinf(game.time*2.4f+i*1.35f+station)*.5f+.5f)*5.f);int y=23-h;float fade=sinf(game.time*.42f+i*.11f+station)*.5f+.5f;line(344+i*3,y,346+i*3,23,mix_rgb(eq_a,eq_b,fade));}
 text(37,1,ink,"%.23s",shown);
}
/* Ship-relative plan radar: up is ahead, down is behind. Full 360 degrees;
 * logarithmic range keeps both nearby craft and remote worlds visible. */
typedef struct {int x,y,lift;} RadarPoint;
static RadarPoint radar_point(Vec3 p){
 float d=length(p),h=sqrtf(p.x*p.x+p.z*p.z),r=fminf(1,logf(1+d/100)/7.f);
 RadarPoint q={240,230,0};
 if(h>.001f){q.x+=(int)(p.x/h*r*62);q.y-=(int)(p.z/h*r*16);}
 q.lift=(int)fmaxf(-4,fminf(4,p.y/fmaxf(1,d)*4));return q;
}
static void radar_dot(Vec3 p,unsigned ink,int kind,int focus){
 RadarPoint q=radar_point(p);line(q.x,q.y,q.x,q.y-q.lift,ink);q.y-=q.lift;
 if(focus){line(q.x-3,q.y-3,q.x+3,q.y-3,WHITE);line(q.x-3,q.y+3,q.x+3,q.y+3,WHITE);}
 if(kind==1){pixel(q.x,q.y-2,ink);line(q.x-2,q.y,q.x+2,q.y,ink);pixel(q.x,q.y+2,ink);}
 else if(kind==2){rect(q.x-2,q.y-2,5,5,ink);rect(q.x-1,q.y-1,3,3,RGB(21,28,39));}
 else rect(q.x-1,q.y-1,3,3,ink);
}
static const char *tracked_hud_cue(void){
 static char out[40];
 if(tracked_mission==TRACK_STATION_TOUR&&station_tour_stage>=STATION_TOUR_ROUTE&&station_tour_stage<STATION_TOUR_DONE){
  if(station_tour_stage==STATION_TOUR_ROUTE||station_tour_stage==STATION_TOUR_DOCK){
   if(game.system!=STATION_TOUR_DEST)snprintf(out,sizeof(out),"JUMP: REORTE");
   else snprintf(out,sizeof(out),"DOCK: REORTE HUB");
  }else if(station_tour_stage==STATION_TOUR_WALK)snprintf(out,sizeof(out),"WALK: CANTEEN");
  else if(station_tour_stage==STATION_TOUR_BAR)snprintf(out,sizeof(out),"TALK: LYSA KEST");
  else snprintf(out,sizeof(out),"TALK: LYSA KEST");
  return out;
 }
 if(tracked_mission==0){if(game.campaign_stage>=6&&game.saga_chapter<SAGA_COUNT){if(!game.saga_step)snprintf(out,sizeof(out),"OPEN: STORY BRIEF");else if(saga_ready(&game))snprintf(out,sizeof(out),"OPEN: TRACKED MISSION");else if(game.system!=game.saga_dest){int jumps=0,hop=saga_next_hop(&game,&jumps);snprintf(out,sizeof(out),hop>=0?"JUMP: %.18s":"OPEN: GALAXY MAP",hop>=0?game.systems[hop].name:"");}else if(saga_beats[game.saga_chapter].kind==SAGA_SCAN)snprintf(out,sizeof(out),"SCAN: LOCAL SIGNAL");else if(saga_beats[game.saga_chapter].kind==SAGA_HUNT)snprintf(out,sizeof(out),"CLEAR: HOSTILE SHIPS");else snprintf(out,sizeof(out),"DOCK: LOCAL HUB");return out;}if(game.system!=7)snprintf(out,sizeof(out),"JUMP: LAVE");else if(game.docked)snprintf(out,sizeof(out),game.campaign_stage==1?"LAUNCH: FIRST FLIGHT":"OPEN: TRACKED MISSION");else if(game.campaign_stage==2&&!(game.campaign_flags&CP_LOCKED))snprintf(out,sizeof(out),"LOCK: LAVE HUB");else if(game.campaign_stage==3&&!(game.campaign_flags&CP_FLEW))snprintf(out,sizeof(out),"FLY: %.0f/600 M",game.campaign_distance);else snprintf(out,sizeof(out),"DOCK: LAVE HUB");return out;}
 if(tracked_mission==1){int ji=-1,type=guild_required_contract(&game);for(int i=0;i<game.job_n;i++)if(game.jobs[i].type==type){ji=i;break;}if(guild_ready(&game)){snprintf(out,sizeof(out),"DOCK: %.14s HUB",game.systems[game.system].name);return out;}if(ji>=0){Job *j=&game.jobs[ji];if(game.system!=j->dest)snprintf(out,sizeof(out),"JUMP: %.18s",game.systems[j->dest].name);else snprintf(out,sizeof(out),j->type==MISSION_RESCUE&&!j->stage?"FIND: RESCUE SIGNAL":"DOCK: %.14s HUB",game.systems[game.system].name);return out;}if(game.guild_chapter==2){snprintf(out,sizeof(out),"SCAN: LOCAL SIGNAL");return out;}if(type>=0){int station=guild_contract_station(&game);if(game.system!=station)snprintf(out,sizeof(out),"JUMP: %.18s",game.systems[station].name);else if(!game.docked)snprintf(out,sizeof(out),"DOCK: %.14s HUB",game.systems[station].name);else snprintf(out,sizeof(out),"OPEN: MISSION BOARD");return out;}snprintf(out,sizeof(out),game.docked?"LAUNCH: TEST FLIGHT":"DOCK: LOCAL HUB");return out;}
 int ji=tracked_mission-2;if(ji<0||ji>=game.job_n){snprintf(out,sizeof(out),"OPEN: MISSION LOG");return out;}Job *j=&game.jobs[ji];if(game.system!=j->dest)snprintf(out,sizeof(out),"JUMP: %.18s",game.systems[j->dest].name);else if(j->type==MISSION_DELIVERY||j->type==MISSION_SMUGGLING||(j->type==MISSION_RESCUE&&j->stage))snprintf(out,sizeof(out),"DOCK: %.14s HUB",game.systems[game.system].name);else if(j->type==MISSION_EXPLORATION&&j->item>=1&&j->item<BODY_COUNT)snprintf(out,sizeof(out),"FLY TO: %.18s",game.bodies[j->item].name);else if(j->type==MISSION_BOUNTY)snprintf(out,sizeof(out),"HUNT: MISSION TARGET");else if(j->type==MISSION_RESCUE)snprintf(out,sizeof(out),"FIND: RESCUE SIGNAL");else snprintf(out,sizeof(out),"FIND: MISSION CONTACT");return out;
}
static void cockpit(void){
 if(hud_hidden||hud_mode==2)return;
 /* Top 24px: system, heading, route. Bottom 80px: all instruments.
  * Charcoal + ochre instrument bands — McQuarrie chrome, not cyan debug boxes. */
 rect(0,0,W,24,RGB(21,28,39));rect(0,23,W,1,RGB(193,139,77));
 text(1,0,RGB(85,212,212),"System: %.11s",game.systems[game.system].name);
 {int wl=wanted_level(&game);if(wl){unsigned ink=wl>=4?(((int)(game.time*8.f)&1)?RED:RGB(255,120,50)):wl>=2?RED:RGB(240,180,91);text(1,1,ink,"LAW IS AFTER YOU!");}}
 flight_activity_display();
 radio_ticker_display();
 /* Mission cue top-right in the header band with a 2-col margin — not flush
  * to the screen edge. ART_AMBER objective ink (ART DIRECTOR palette); clear of danger badge. */
 {
  const char *cue=tracked_hud_cue();char route[52];snprintf(route,sizeof(route),">> %s >>",cue);
  int cols=W/8,inset=1,left=33,clen=(int)strlen(route),max=cols-inset-left;
  if(max<8)max=8;if(clen>max)clen=max;
  text(cols-inset-clen,0,RGB(100,235,150),"%.*s",clen,route);
 }
 if(game.dock_stage==1){rect(8,24,464,16,RGB(21,28,39));rect(8,24,464,1,RGB(193,139,77));text(2,4,RGB(85,212,212),"DOCKING GUIDANCE ACTIVE");}
 else if(square_held){
  /* Hold-Square is the expanded tactical browser. Keep it pinned to the
   * left edge so the central flight view remains readable and playable. */
  targeting_overlay(8,28,192,156,0);
  rect(8,204,132,16,RGB(24,63,73));rect(8,204,132,1,RGB(85,212,212));
  text(2,25,RGB(120,245,220),"(R = LOCK)");
 }
 else if(game.approach<0&&!game.police_stop&&!game.dead&&!game.dock_stage&&game.jump<=0){speech_box(8,24,464);quick_comms_box();}
 combat_alert_banner();
 rect(0,192,W,80,RGB(21,28,39));rect(0,192,W,1,RGB(193,139,77));
 line(155,198,155,258,RGB(41,54,70));line(323,198,323,258,RGB(41,54,70));
 int id=valid_target(selected_target)?selected_target:valid_target(look_target)?look_target:-1;
 text(4,25,RGB(155,154,165),game.surface==2?"PARKED SHIP":game.planet>=0?"LANDING PAD":"TARGET");
 hud_pixel_icon(8,200,id<0?3:id==0?0:id<=BODY_COUNT?1:2,RGB(85,212,212));
 if(id>=0||game.planet>=0){
  Vec3 wp=game.surface==2?game.ship_pos:game.planet>=0?surface_site(&game,1):target_position(id),p=camera(&game,wp);
  const char *name=game.surface==2?player_ships[game.ship].name:game.planet>=0?game.bodies[game.planet].name:scanner_known(id)?target_name(id):"UNKNOWN CONTACT";
  int cut=(int)strlen(name);if(cut>18){cut=18;for(int k=18;k>8;k--)if(name[k]==' '){cut=k;break;}}
  text(1,27,RGB(240,180,91),"%.*s",cut,name);
  const char *tail=name+cut;while(*tail==' ')tail++;
  if(*tail)text(1,28,RGB(240,180,91),"%.18s",tail);
  text(1,30,RGB(229,210,163),"%d M",(int)length(p));
  if(IS_NPC_ID(id)&&scanner_known(id)){
   NPC *n=&game.npc[id-BODY_COUNT-1];
   float mh=n->freighter?900.f:n->role==LAW?110.f:80.f,ms=n->freighter?100.f:n->role==LAW?60.f:40.f;
   int hull=(int)fmaxf(0,fminf(100,100.f*n->health/mh)),shld=(int)fmaxf(0,fminf(100,100.f*n->shield/fmaxf(1.f,ms)));
   /* Stack the two compact target bars vertically. They share the target card
    * but never compete for the same row or overlap the radar/power columns. */
   text(1,31,RGB(155,154,165),"HULL");pip_bar(40,248,72,4,hull,hull<30?RED:(n->freighter?RGB(240,180,91):RGB(85,212,212)));
   text(1,32,RGB(155,154,165),"SHLD");pip_bar(40,258,72,3,shld,RGB(85,212,212));
  }else if(autoaim)text(1,31,RGB(85,212,212),"LOCKED / ALIGNING");
 }
 else {text(1,27,RGB(155,154,165),"NO TARGET");text(1,30,RGB(229,210,163),"SQUARE TO SELECT");}
 text(28,25,RGB(155,154,165),"AHEAD");text(29,31,RGB(155,154,165),"AFT");
 line(171,230,309,230,RGB(41,54,70));line(240,211,240,249,RGB(41,54,70));
 line(177,214,303,214,RGB(41,54,70));line(177,246,303,246,RGB(41,54,70));
 if(game.planet>=0){for(int i=0;i<3;i++)radar_dot(camera(&game,surface_site(&game,i)),i==1?RGB(85,212,212):RGB(240,180,91),2,0);if(game.surface==2)radar_dot(camera(&game,game.ship_pos),RGB(85,212,212),0,1);}
 else {
  /* Draw faint scenery first, selected contact last. */
  for(int pass=0;pass<2;pass++)for(int tid=0;tid<=ANOMALY_ID_MAX;tid++)if(valid_target(tid)&&((tid==selected_target)==pass)){
   int ship=IS_NPC_ID(tid);unsigned ink=ship?faction_colors[game.npc[tid-BODY_COUNT-1].role]:tid==0?RGB(85,212,212):tid<=BODY_COUNT?RGB(125,122,91):RGB(155,154,165);
   radar_dot(camera(&game,target_position(tid)),ink,tid==0?2:tid<=BODY_COUNT?1:0,pass);
  }
  for(int hub=1;hub<HUB_COUNT;hub++)if(hub!=nearest_hub(&game))radar_dot(camera(&game,hub_position(&game,hub)),RGB(85,212,212),2,0);
 }
 hud_pixel_icon(237,227,2,RGB(229,210,163));
 for(int i=0;i<3;i++){
  int x=336+i*46;int n=i==0?game.pip_sys:i==1?game.pip_eng:game.pip_wep;
  unsigned ink=paused&&i==pip_sel?RGB(240,180,91):RGB(155,154,165);
  text(x/8,25,ink,"%s",i==0?"SYS":i==1?"ENG":"WEP");
  /* The orange label and selected pips already identify the active bank;
   * avoid an extra underline crossing the instrument labels below. */
  for(int k=0;k<4;k++)rect(x+k*5,210,3,2,k<n?(i==0?RGB(85,212,212):i==1?RGB(240,180,91):RED):RGB(41,54,70));
 }
 const char *labels[]={"SHLD","HULL","HEAT","FUEL"};
 int vmax=player_ships[game.ship].speed;if(vmax<1)vmax=1;
 int values[]={(int)game.energy,(int)game.hull,(int)game.heat,(int)(100*game.fuel/fmaxf(1,player_ships[game.ship].range))};
 unsigned cols[]={game.energy<30?RED:RGB(85,212,212),game.hull<35?RED:RGB(85,212,212),game.heat>70?RED:RGB(139,106,72),RGB(240,180,91)};
 /* Compact speed readout sits above the power pips, clear of radar and bars. */
 /* Keep the speed fill pegged to the normal cruise scale. Boost is shown as
  * a state effect (colour/pulse), rather than making the bar appear to empty. */
 int speed_pct=(int)fminf(100.f,100.f*game.speed/fmaxf(1.f,(float)vmax));
 for(int i=0;i<4;i++){text(42,27+i,RGB(155,154,165),"%s",labels[i]);pip_bar(378,216+i*8,90,5,values[i],cols[i]);}
 text(42,31,game.boost?RGB(240,120,96):RGB(155,154,165),"SPD %3d",(int)game.speed);
 {int sx=game.boost?400+(int)(sinf(game.time*18.f)*2.f):400;
  pip_bar(sx,248,68,5,speed_pct,game.boost?RGB(240,120,96):RGB(139,184,198));
  if(game.boost){rect(sx-2,246,72,1,RGB(240,180,91));rect(sx-2,254,72,1,RGB(240,180,91));}
 }
 rect(0,262,W,10,RGB(21,28,39));rect(0,262,W,1,RGB(193,139,77));
 button_icon(8,263,'T',RGB(85,212,212));text(3,33,game.damaged?RED:RGB(155,154,165),game.damaged?"HULL DAMAGE / ENGINEERS REQUIRED":"HOLD: COMMS");
 if(game.dead)text(20,33,RED,"START: RECOVER");
 else if(paused)text(20,33,RGB(240,180,91),"HOLD START  L/R BANK  U/D POWER");
 else if(game.police_stop)text(16,33,RGB(240,180,91),game.police_phase?"X CONFIRM SCAN MENU":"X CONFIRM SETTLE MENU");
 else if(game.approach>=0)text(20,33,RGB(240,180,91),game.bodies[game.approach].type==GAS?"O TURN BACK":"X ENTER   O TURN BACK");
 else if(game.surface==2)text(20,33,RGB(155,154,165),"O BOARD   SQUARE SCAN   HOLD R JET");
 else if(game.planet>=0)text(20,33,RGB(155,154,165),game.surface==1?"O WALK   TRIANGLE TAKE OFF":"O LAND   TRIANGLE ORBIT");
 else {
  int action=valid_target(look_target)?look_target:id;
  int rock=IS_DEBRIS_ID(action)&&game.debris[action-DEBRIS_ID_MIN].rock;
  const char *verb=rock?"MINE":IS_DEBRIS_ID(action)?"COLLECT":IS_NPC_ID(action)?"LOCK":IS_ANOMALY_ID(action)?"SCAN":action==0?"DOCK":"APPROACH";
  button_icon(158,263,'S',RGB(240,180,91));text(22,33,RGB(155,154,165),"TARGETS");button_icon(264,263,rock?'X':'O',RGB(85,212,212));text(35,33,RGB(155,154,165),"%s",verb);text(46,33,RGB(155,154,165),"MS %d",game.missiles);
 }
}
