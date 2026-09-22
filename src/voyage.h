static void sector_background(void){
 /* Flat sector wash only — nebula / clouds / dust live in space-fx.h.
  * Broad, saturated period-illustration fields: cobalt, violet, teal, ember
  * and coral rather than one black void — still dark enough for cockpit text. */
 const unsigned colors[]={RGB(8,12,27),RGB(27,12,32),RGB(7,30,35),RGB(36,18,10),RGB(15,23,45),RGB(40,17,26),RGB(12,20,23),RGB(32,20,10)};
 unsigned seed=game.bodies[0].seed;float zone=sinf((game.pos.x+game.pos.z)*.00007f+(seed&255)*.03f);
 unsigned raw=colors[game.system%8];float level=.78f+(zone+1)*.12f;
 unsigned base=RGB((int)((raw&255)*level),(int)(((raw>>8)&255)*level),(int)(((raw>>16)&255)*level));
 rect(0,view_top(),W,view_bot()-view_top()+1,base);
}
static Vec3 station_vertex(Vec3 v){return add(rotate(v,0,station_angle(&game)),(Vec3){0,0,3500});}
static void station_model(void){
 shipmesh(mesh_id("CORIOLIS"),(Vec3){0,0,3500},0,station_angle(&game),1,RGB(170,192,202),0);
 Vec3 outer[4]={{0,-160,-160},{160,0,-160},{0,160,-160},{-160,0,-160}};
 Vec3 inner[4];for(int i=0;i<4;i++)inner[i]=station_port_corner(i);
 for(int i=0;i<4;i++){int j=(i+1)%4;Vec3 a=camera(&game,station_vertex(outer[i])),b=camera(&game,station_vertex(outer[j])),c=camera(&game,station_vertex(inner[j])),d=camera(&game,station_vertex(inner[i]));queue_triangle(a,b,c,RGB(109,140,151));queue_triangle(a,c,d,RGB(109,140,151));Vec3 backi=inner[i],backj=inner[j];backi.z=0;backj.z=0;queue_triangle(d,c,camera(&game,station_vertex(backj)),RGB(8,19,24));queue_triangle(d,camera(&game,station_vertex(backj)),camera(&game,station_vertex(backi)),RGB(8,19,24));}
 if(prosperity(&game,game.system)>=4){for(int ring=0;ring<2;ring++){float radius=230+ring*65;for(int i=0;i<24;i++){float a=station_angle(&game)+i*6.2831853f/24,b=station_angle(&game)+(i+1)*6.2831853f/24;Vec3 u=camera(&game,(Vec3){cosf(a)*radius,sinf(a)*radius,3500+(ring?100:-80)}),v=camera(&game,(Vec3){cosf(b)*radius,sinf(b)*radius,3500+(ring?100:-80)});if(u.z>15&&v.z>15){Point p=project(u),q=project(v);line((int)p.x,(int)p.y,(int)q.x,(int)q.y,ring?CYAN:DIM);}}}}
}
/* Small practical windows make the exterior read as a lived-in miniature.
 * Procedural beacon masks; no station crawl geometry or interaction state. */
static void station_window_animation(void){
 if(high_contrast)return;
 int top=clipy0>=0?clipy0:view_top(),bot=clipy1>=0?clipy1-1:view_bot();
 unsigned seed=game.bodies[0].seed^(unsigned)game.system*97u;
 int count=prosperity(&game,game.system)>=4?20:10;
 for(int i=0;i<count;i++){
  float a=station_angle(&game)+(i+.5f)*6.2831853f/count;
  float radius=214.f+(i&1)*38.f;
  Vec3 v=camera(&game,(Vec3){cosf(a)*radius,sinf(a)*radius,3500.f+((i&3)-1)*72.f});
  if(v.z<20)continue;
  Point p=project(v);if(p.x<2||p.x>=W-2||p.y<top||p.y>bot)continue;
  unsigned c=((i+(int)(game.time*2.f))&5)==0?RGB(255,183,76):RGB(211,145,65);
  space_anim_draw(SPACE_ANIM_BEACON,(int)p.x,(int)p.y,(int)(game.time*4.f)+i,c);
 }
 (void)seed;
}
static void secondary_hubs(void);
static void menu_space_view(int x,int y,int w,int h){
 rect(x,y,w,h,RGB(4,8,18));
 rect(x,y,w,1,CYAN);rect(x,y+h-1,w,1,RGB(32,57,69));
 Vec3 oldpos=game.pos;float oldyaw=game.yaw,oldpitch=game.pitch,oldroll=game.roll;
 preview_clip(x+w/2,y+h/2+2,x+1,y+1,x+w-1,y+h-1);
 /* Third-person orbit of the fitted hull against local space — pulled back so the full silhouette reads. */
 {
  float phase=preview_time*.4f;
  Vec3 ship=game.docked?(Vec3){0,40,3180}:game.planet>=0?add(game.pos,(Vec3){0,80,0}):game.pos;
  float dist=310.f;Vec3 cam=add(ship,(Vec3){sinf(phase)*dist,88.f+sinf(phase*.7f)*36.f,cosf(phase)*dist});
  game.pos=cam;Vec3 aim=norm(sub(ship,cam));game.yaw=atan2f(aim.x,aim.z);float ap=aim.y;if(ap>1)ap=1;if(ap<-1)ap=-1;game.pitch=asinf(ap);game.roll=0;
  starfield();space_fx_nebula();space_fx_meteors();
  /* Local scenery so the inset reads as “ship in this system,” not a void studio. */
  if(game.docked||length(sub(ship,(Vec3){0,0,3500}))<14000){station_model();station_window_animation();}
  else {
   Body *b=&game.bodies[1];
   Vec3 bp=camera(&game,b->pos);if(bp.z>80){Point p=project(bp);int r=(int)fminf(28,b->radius*240.f/bp.z);if(r>3&&p.x>x&&p.x<x+w&&p.y>y&&p.y<y+h)circle((int)p.x,(int)p.y,r,b->color);}
  }
  float yaw=game.docked?station_angle(&game)*.15f+phase*.2f:oldyaw;
  shipmesh(mesh_id(player_ships[game.ship].name),ship,yaw,oldroll*.25f,1.85f,GOLD,0);
  flush_meshes();
 }
 preview_reset();game.pos=oldpos;game.yaw=oldyaw;game.pitch=oldpitch;game.roll=oldroll;
 text((x+6)/8,(y+h-10)/8,DIM,"3RD / %.10s",player_ships[game.ship].name);
}
static void ambient_space(void){
 if(system_whales(game.system)){Body *b=&game.bodies[3];for(int i=0;i<3;i++){float a=game.time*.028f+i*.62f;Vec3 pos=add(b->pos,(Vec3){cosf(a)*(b->radius+5600),700+sinf(a+i)*.5f*480,sinf(a)*(b->radius+5600)});if(length(sub(pos,game.pos))<14000)shipmesh(mesh_id("WORM"),pos,a+1.57f,sinf(game.time*.35f+i)*.16f,7.2f+i*1.3f,RGB(96,186,198),0);}}
 if(system_comet(game.system)){float a=game.time*.018f;Vec3 pos={cosf(a)*17000,1800,sinf(a)*17000};if(length(sub(pos,game.pos))<12000){shipmesh(mesh_id("BOULDER"),pos,a,a*.3f,1.4f,RGB(210,230,240),0);Vec3 tail=add(pos,(Vec3){sinf(a)*900,-200,-cosf(a)*900});Vec3 u=camera(&game,pos),v=camera(&game,tail);if(u.z>30&&v.z>30){Point p=project(u),q=project(v);if(p.y>view_top()&&p.y<view_bot()&&q.y>view_top()&&q.y<view_bot()){line((int)p.x,(int)p.y,(int)q.x,(int)q.y,CYAN);if(!high_contrast){int top=view_top(),bot=view_bot();for(int k=0;k<8;k++){float t=k/7.f;int x=(int)(p.x+(q.x-p.x)*t),y=(int)(p.y+(q.y-p.y)*t);sfx_add(x+(k&1),y,RGB(90,140,180),top,bot);space_anim_draw(SPACE_ANIM_SPARK,x,y,((int)(game.time*6)+k)&3,RGB(180,220,255));}}}}}}
}
static void speed_lines(void){float normal=game.speed/player_ships[game.ship].speed;if(normal<.75f||game.dock_stage||game.jump>0)return;int count=game.boost?65:18;float power=game.boost?fminf(1,normal/20):fminf(1,(normal-.75f)*4);for(int i=0;i<count;i++){float a=i*2.39996f;float r=95+fmodf(i*37+game.time*(game.boost?650:130),190);float trail=(game.boost?20+power*95:3+power*12);int x=240+(int)(cosf(a)*r),y=110+(int)(sinf(a)*r*.5f),xx=240+(int)(cosf(a)*(r+trail)),yy=110+(int)(sinf(a)*(r+trail)*.5f);int top=view_top(),bottom=view_bot();if(y>top&&y<bottom&&yy>top&&yy<bottom)line(x,y,xx,yy,game.boost?CYAN:RGB(95,128,145));}}
static void engine_flare(void){
 if(game.speed<80||game.dock_stage||game.jump>0||game.dead)return;
 int bottom=view_bot();if(bottom<150)return;float normal=game.speed/fmaxf(1,player_ships[game.ship].speed),power=game.boost?fminf(1,normal/20):fminf(1,normal);
 int pulse=(int)(sinf(game.time*(game.boost?18:9))*2),length=(game.boost?18:7)+(int)(power*10)+pulse;if(length<3)length=3;
 unsigned core=game.boost?CYAN:AMBER,edge=game.boost?RGB(45,120,170):RGB(150,92,28);int y=bottom-6;
 for(int i=0;i<length;i++){int w=game.boost?2+(i%3):1+(i%2),yy=y+i,spread=(i*2)/3+1;rect(240-w-spread,yy,w,1,edge);rect(240+spread,yy,w,1,edge);if((i&1)==0)pixel(240,yy,core);}line(240-length/2,y-1,240+length/2,y-1,edge);
 space_anim_draw(SPACE_ANIM_PLUME,240,y+length/2,(int)(game.time*12.f),core);
}
static void secondary_hubs(void){
 for(int i=1;i<HUB_COUNT;i++){Vec3 p=hub_position(&game,i);float d=length(sub(p,game.pos));if(d>30000||occluded(p))continue;unsigned c=i==1?CYAN:RGB(150,110,210);shipmesh(mesh_id("CORIOLIS"),p,0,station_angle(&game)+(i*1.7f),.62f,c,0);}
}
static void docking_view(void){
 if(game.dock_stage==3){rect(0,23,W,195,BG);text(22,10,CYAN,"DOCKING COMPLETE");text(12,14,WHITE,"Welcome to %s",station_name(&game));text(17,18,DIM,"Opening station services...");return;}
 Vec3 oldpos=game.pos;float oldyaw=game.yaw,oldpitch=game.pitch,oldroll=game.roll;game.pos=(Vec3){260,120,2820};Vec3 aim=norm(sub((Vec3){0,0,3400},game.pos));game.yaw=atan2f(aim.x,aim.z);game.pitch=asinf(aim.y);game.roll=0;sector_background();space_fx_nebula();starfield();space_fx_meteors();station_model();station_window_animation();float t=fminf(1,game.dock_timer/3);shipmesh(mesh_id(player_ships[game.ship].name),(Vec3){0,0,3070+t*470},0,station_angle(&game),.7f,GOLD,0);flush_meshes();station_entrance();text(2,5,CYAN,"ARRIVAL CAMERA / %s",station_name(&game));game.pos=oldpos;game.yaw=oldyaw;game.pitch=oldpitch;game.roll=oldroll;
}
/* Edge-band captions. Body text is word wrapped to actual 8px cell capacity. */
static void speech_box(int x,int y,int w){
 const char *s=0;int who=VOICE_COMP;
 if(!quiet_comms&&game.voice_time>0&&game.voice[0]){s=game.voice;who=game.voice_who;}
 /* During combat, status text stays on the bottom RED ALERT banner — not here. */
 else if(game.message_time>0&&game.message[0]&&!combat_alert_active())s=game.message;
 if(!s)return;
 if(who<VOICE_KEI||who>VOICE_CONTACT)who=VOICE_COMP;
 int role=who==VOICE_LAW?LAW:who==VOICE_KEI?EXPLORERS:TRADERS;
 if(who==VOICE_CONTACT)role=game.voice_role>=0&&game.voice_role<FACTION_COUNT?game.voice_role:EXPLORERS;
 static const char *names[]={"","KEI","VENN","DOCKHAND","LOCAL LAW","COMPUTER","CONTACT"};
 unsigned ink=who==VOICE_COMP?CYAN:faction_colors[role];
 int portrait=who!=VOICE_COMP,tx=x+(portrait?40:8),col=tx/8,cap=(x+w-8-tx)/8;
 /* Three body rows under the name plate stay inside the top caption band (y<56). */
 int box_h=32;
 rect(x,y,w,box_h,RGB(8,18,28));rect(x,y,2,box_h,ink);
 if(portrait){if(who==VOICE_KEI)draw_kei(x+4,y,32,0);else draw_portrait(x+4,y,32,32,who==VOICE_CONTACT?game.voice_seed:who*37,role);}
 speaker_name_tag(col,y/8,who==VOICE_CONTACT?faction_names[role]:names[who],ink);
 button_icon(x+w-16,y,'T',ink);
 text_wrap(col,y/8+1,cap,3,WHITE,s,0);
}
static void pip_bar(int x,int y,int w,int h,int fill,unsigned c){
 if(fill<0)fill=0;
 if(fill>100)fill=100;
 rect(x,y,w,h,RGB(32,14,6));int fw=w*fill/100;if(fw>0)rect(x,y,fw,h,c);rect(x,y,w,1,AMBDIM);
}
static unsigned dim_rgb(unsigned c,int num,int den){
 int r=((c&255)*num)/den,g=(((c>>8)&255)*num)/den,b=(((c>>16)&255)*num)/den;
 if(r>255)r=255;
 if(g>255)g=255;
 if(b>255)b=255;
 return RGB(r,g,b);
}
/* Applied to the world before any UI. Vignette + threshold bloom that never
 * reallocates and only samples a sparse grid — stays glitch-free on PSP. */
static void hud_postfx(void){
 if(high_contrast)return;
 int top=view_top(),bot=view_bot();
 for(int y=top;y<=bot;y++)for(int x=0;x<8;x++){
  fb[y*STRIDE+x]=dim_rgb(fb[y*STRIDE+x],24+x,32);
  fb[y*STRIDE+W-1-x]=dim_rgb(fb[y*STRIDE+W-1-x],24+x,32);
 }
 /* Two-pass sparse bloom: gather bright samples, then soft-add to neighbours. */
 for(int y=top+3;y<=bot-3;y+=3)for(int x=3;x<W-3;x+=3){
  unsigned c=fb[y*STRIDE+x];int r=c&255,g=(c>>8)&255,b=(c>>16)&255,lum=r+g+b;
  if(lum<540)continue;
  int boost=(lum-540)/24;if(boost>6)boost=6;
  for(int dy=-2;dy<=2;dy++)for(int dx=-2;dx<=2;dx++){
   int dist=dx*dx+dy*dy;if(dist==0||dist>8)continue;
   int yy=y+dy,xx=x+dx;unsigned d=fb[yy*STRIDE+xx];
   int fall=5-dist/2;if(fall<1)fall=1;
   int nr=((d&255)*6+(r*fall)/5)/6,ng=(((d>>8)&255)*6+(g*fall)/5)/6,nb=(((d>>16)&255)*6+(b*fall)/5)/6;
   if(nr>255)nr=255;if(ng>255)ng=255;if(nb>255)nb=255;
   /* Never write the sample pixel itself — avoids strobing the source. */
   fb[yy*STRIDE+xx]=RGB(nr,ng,nb);
  }
  (void)boost;
 }
}
static void danger_badge(int x,int y,int level){
 int n=level<1?1:level>5?5:level;
 for(int i=0;i<5;i++){int cx=x+i*8;unsigned c=i<n?(level>=4?RED:AMBER):RGB(35,45,55);rect(cx+3,y,2,2,c);rect(cx+1,y+2,6,2,c);rect(cx,y+4,8,2,c);rect(cx+2,y+6,4,2,c);}
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
 else if(kind==2){rect(q.x-2,q.y-2,5,5,ink);rect(q.x-1,q.y-1,3,3,RGB(8,18,28));}
 else rect(q.x-1,q.y-1,3,3,ink);
}
static const char *tracked_hud_cue(void){
 static char out[40];
 if(tracked_mission==0){if(game.campaign_stage>=6&&game.saga_chapter<SAGA_COUNT){if(!game.saga_step)snprintf(out,sizeof(out),"OPEN: STORY BRIEF");else if(saga_ready(&game))snprintf(out,sizeof(out),"OPEN: TRACKED MISSION");else if(game.system!=game.saga_dest){int jumps=0,hop=saga_next_hop(&game,&jumps);snprintf(out,sizeof(out),hop>=0?"JUMP: %.18s":"OPEN: GALAXY MAP",hop>=0?game.systems[hop].name:"");}else if(saga_beats[game.saga_chapter].kind==SAGA_SCAN)snprintf(out,sizeof(out),"SCAN: LOCAL SIGNAL");else if(saga_beats[game.saga_chapter].kind==SAGA_HUNT)snprintf(out,sizeof(out),"CLEAR: HOSTILE SHIPS");else snprintf(out,sizeof(out),"DOCK: LOCAL HUB");return out;}if(game.system!=7)snprintf(out,sizeof(out),"JUMP: LAVE");else snprintf(out,sizeof(out),"DOCK: LAVE HUB");return out;}
 if(tracked_mission==1){int ji=-1,type=guild_required_contract(&game);for(int i=0;i<game.job_n;i++)if(game.jobs[i].type==type){ji=i;break;}if(guild_ready(&game)){snprintf(out,sizeof(out),"DOCK: %.14s HUB",game.systems[game.system].name);return out;}if(ji>=0){Job *j=&game.jobs[ji];if(game.system!=j->dest)snprintf(out,sizeof(out),"JUMP: %.18s",game.systems[j->dest].name);else snprintf(out,sizeof(out),j->type==MISSION_RESCUE&&!j->stage?"FIND: RESCUE SIGNAL":"DOCK: %.14s HUB",game.systems[game.system].name);return out;}if(game.guild_chapter==2){snprintf(out,sizeof(out),"SCAN: LOCAL SIGNAL");return out;}if(type>=0){int station=guild_contract_station(&game);if(game.system!=station)snprintf(out,sizeof(out),"JUMP: %.18s",game.systems[station].name);else if(!game.docked)snprintf(out,sizeof(out),"DOCK: %.14s HUB",game.systems[station].name);else snprintf(out,sizeof(out),"OPEN: MISSION BOARD");return out;}snprintf(out,sizeof(out),game.docked?"LAUNCH: TEST FLIGHT":"DOCK: LOCAL HUB");return out;}
 int ji=tracked_mission-2;if(ji<0||ji>=game.job_n){snprintf(out,sizeof(out),"OPEN: MISSION LOG");return out;}Job *j=&game.jobs[ji];if(game.system!=j->dest)snprintf(out,sizeof(out),"JUMP: %.18s",game.systems[j->dest].name);else if(j->type==MISSION_DELIVERY||j->type==MISSION_SMUGGLING||(j->type==MISSION_RESCUE&&j->stage))snprintf(out,sizeof(out),"DOCK: %.14s HUB",game.systems[game.system].name);else if(j->type==MISSION_EXPLORATION&&j->item>=1&&j->item<BODY_COUNT)snprintf(out,sizeof(out),"FLY TO: %.18s",game.bodies[j->item].name);else if(j->type==MISSION_BOUNTY)snprintf(out,sizeof(out),"HUNT: MISSION TARGET");else if(j->type==MISSION_RESCUE)snprintf(out,sizeof(out),"FIND: RESCUE SIGNAL");else snprintf(out,sizeof(out),"FIND: MISSION CONTACT");return out;
}
static void cockpit(void){
 if(hud_hidden||hud_mode==2)return;
 /* Top 24px: system, heading, route. Bottom 80px: all instruments. */
 rect(0,0,W,24,RGB(6,15,24));rect(0,23,W,1,RGB(43,77,89));
 text(1,0,CYAN,"System: %.11s",game.systems[game.system].name);
 {int wl=wanted_level(&game);text(1,1,wl?RED:DIM,wl?"Wanted %d/5":"Wanted 0/5",wl);}
 int heading=(int)(game.yaw*57.29578f)%360;if(heading<0)heading+=360;
 text(24,0,DIM,"%03d",heading);danger_badge(224,4,danger_rating(&game,game.system));
 /* Mission cue flush to the top-right (1-col inset). Keep past the danger badge. */
 {
  const char *cue=tracked_hud_cue();
  int cols=W/8,inset=1,left=34,clen=(int)strlen(cue),max=cols-inset-left;
  if(max<8)max=8;if(clen>max)clen=max;
  text(cols-inset-clen,0,GOLD,"%.*s",clen,cue);
 }
 if(game.dock_stage==1){rect(8,24,464,16,RGB(8,24,32));text(2,4,CYAN,"DOCKING GUIDANCE ACTIVE");}
 else if(square_held){rect(8,24,464,32,RGB(8,24,32));for(int i=0;i<5;i++)text(1+i*11,4,i==scan_cat?GOLD:DIM,"%s",scan_cat_names[i]);text(2,6,CYAN,"D-PAD BANDS   L CYCLE VIEW");text(35,6,GOLD,"R LOCK");}
 else if(game.approach<0&&!game.police_stop&&!game.dead&&!game.dock_stage&&game.jump<=0)speech_box(8,24,464);
 combat_alert_banner();
 rect(0,192,W,80,RGB(6,15,24));rect(0,192,W,1,CYAN);
 line(155,198,155,258,RGB(32,57,69));line(323,198,323,258,RGB(32,57,69));
 int id=valid_target(selected_target)?selected_target:valid_target(look_target)?look_target:-1;
 text(4,25,DIM,game.surface==2?"PARKED SHIP":game.planet>=0?"LANDING PAD":"TARGET");
 hud_pixel_icon(8,200,id<0?3:id==0?0:id<=BODY_COUNT?1:2,CYAN);
 if(id>=0||game.planet>=0){
  Vec3 wp=game.surface==2?game.ship_pos:game.planet>=0?surface_site(&game,1):target_position(id),p=camera(&game,wp);
  const char *name=game.surface==2?player_ships[game.ship].name:game.planet>=0?game.bodies[game.planet].name:scanner_known(id)?target_name(id):"UNKNOWN CONTACT";
  int cut=(int)strlen(name);if(cut>18){cut=18;for(int k=18;k>8;k--)if(name[k]==' '){cut=k;break;}}
  text(1,27,GOLD,"%.*s",cut,name);
  const char *tail=name+cut;while(*tail==' ')tail++;
  if(*tail)text(1,28,GOLD,"%.18s",tail);
  text(1,30,WHITE,"%d M",(int)length(p));
  if(IS_NPC_ID(id)&&scanner_known(id)){
   NPC *n=&game.npc[id-BODY_COUNT-1];
   float mh=n->freighter?900.f:n->role==LAW?110.f:80.f,ms=n->freighter?100.f:n->role==LAW?60.f:40.f;
   int hull=(int)fmaxf(0,fminf(100,100.f*n->health/mh)),shld=(int)fmaxf(0,fminf(100,100.f*n->shield/fmaxf(1.f,ms)));
   text(1,31,DIM,"HULL");pip_bar(40,250,70,4,hull,hull<30?RED:(n->freighter?GOLD:CYAN));
   text(15,31,DIM,"SHLD");pip_bar(128,250,50,4,shld,CYAN);
  }else text(1,31,autoaim?CYAN:DIM,"%s",autoaim?"LOCKED / ALIGNING":"HOLD SQ + R: LOCK");
 }
 else {text(1,27,DIM,"NO TARGET");text(1,30,WHITE,"SQUARE TO SELECT");}
 text(28,25,DIM,"AHEAD");text(29,31,DIM,"AFT");
 line(171,230,309,230,RGB(25,49,60));line(240,211,240,249,RGB(25,49,60));
 line(177,214,303,214,RGB(19,37,49));line(177,246,303,246,RGB(19,37,49));
 if(game.planet>=0){for(int i=0;i<3;i++)radar_dot(camera(&game,surface_site(&game,i)),i==1?CYAN:GOLD,2,0);if(game.surface==2)radar_dot(camera(&game,game.ship_pos),CYAN,0,1);}
 else {
  /* Draw faint scenery first, selected contact last. */
  for(int pass=0;pass<2;pass++)for(int tid=0;tid<=ANOMALY_ID_MAX;tid++)if(valid_target(tid)&&((tid==selected_target)==pass)){
   int ship=IS_NPC_ID(tid);unsigned ink=ship?faction_colors[game.npc[tid-BODY_COUNT-1].role]:tid==0?CYAN:tid<=BODY_COUNT?RGB(125,122,91):DIM;
   radar_dot(camera(&game,target_position(tid)),ink,tid==0?2:tid<=BODY_COUNT?1:0,pass);
  }
  for(int hub=1;hub<HUB_COUNT;hub++)if(hub!=nearest_hub(&game))radar_dot(camera(&game,hub_position(&game,hub)),CYAN,2,0);
 }
 hud_pixel_icon(237,227,2,WHITE);
 for(int i=0;i<3;i++){
  int x=336+i*46;int n=i==0?game.pip_sys:i==1?game.pip_eng:game.pip_wep;
  unsigned ink=paused&&i==pip_sel?GOLD:DIM;
  text(x/8,25,ink,"%s",i==0?"SYS":i==1?"ENG":"WEP");
  if(paused&&i==pip_sel){line(x,218,x+22,218,GOLD);line(x,219,x+22,219,CYAN);}
  for(int k=0;k<4;k++)rect(x+k*5,210,3,2,k<n?(i==0?CYAN:i==1?AMBER:RED):RGB(29,46,57));
 }
 const char *labels[]={"SHLD","SPD","HEAT","FUEL"};
 int vmax=player_ships[game.ship].speed;if(vmax<1)vmax=1;
 int values[]={(int)game.energy,(int)(100*game.speed/vmax),(int)game.heat,(int)(100*game.fuel/fmaxf(1,player_ships[game.ship].range))};
 unsigned cols[]={game.energy<30?RED:CYAN,GOLD,game.heat>70?RED:RGB(139,106,72),GOLD};
 for(int i=0;i<4;i++){text(42,27+i,DIM,"%s",labels[i]);pip_bar(378,216+i*8,90,5,values[i],cols[i]);}
 rect(0,262,W,10,RGB(10,24,33));
 button_icon(8,263,'T',CYAN);text(3,33,DIM,"HOLD: COMMS");
 if(game.dead)text(20,33,RED,"START: RECOVER");
 else if(paused)text(20,33,GOLD,"HOLD START  L/R BANK  U/D POWER");
 else if(game.police_stop)text(20,33,GOLD,"X PAY   O CUSTODY");
 else if(game.approach>=0)text(20,33,GOLD,"X ENTER   O TURN BACK");
 else if(game.surface==2)text(20,33,DIM,"O BOARD   SQUARE SCAN   2xR JET");
 else if(game.planet>=0)text(20,33,DIM,game.surface==1?"O WALK   TRIANGLE TAKE OFF":"O LAND   TRIANGLE ORBIT");
 else {
  int action=valid_target(look_target)?look_target:id;
  int rock=IS_DEBRIS_ID(action)&&game.debris[action-DEBRIS_ID_MIN].rock;
  const char *verb=rock?"MINE":IS_DEBRIS_ID(action)?"COLLECT":IS_NPC_ID(action)?"LOCK":IS_ANOMALY_ID(action)?"SCAN":action==0?"DOCK":"APPROACH";
  button_icon(158,263,'S',GOLD);text(22,33,DIM,"TARGETS");button_icon(264,263,rock?'X':'O',CYAN);text(35,33,DIM,"%s",verb);text(46,33,DIM,"MS %d",game.missiles);
 }
}
