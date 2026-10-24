/* Approved native 480x272 Vector HUD. All instruments read authoritative
 * temporary race state. Mirror renders the real scene, never a canned image. */
static void race_type(int x,int y,int scale,unsigned ink,const char *label){
 for(int i=0;label[i];i++){unsigned char ch=label[i];if(ch<32||ch>126)ch='?';const unsigned char *glyph=font8[ch-32];
  for(int yy=0;yy<8;yy++)for(int xx=0;xx<8;xx++)if(glyph[yy]&(128>>xx))rect(x+i*8*scale+xx*scale,y+yy*scale,scale,scale,ink);
 }
}
static void race_corners(int x,int y,int w,int h,int size,unsigned ink){
 line(x,y,x+size,y,ink);line(x,y,x,y+size,ink);line(x+w,y,x+w-size,y,ink);line(x+w,y,x+w,y+size,ink);
 line(x,y+h,x+size,y+h,ink);line(x,y+h,x,y+h-size,ink);line(x+w,y+h,x+w-size,y+h,ink);line(x+w,y+h,x+w,y+h-size,ink);
}
static void race_world_line(Vec3 a,Vec3 b,unsigned ink){
 a=camera(&game,a);b=camera(&game,b);if(a.z<15&&b.z<15)return;
 if(a.z<15)a=add(a,mul(sub(b,a),(15-a.z)/(b.z-a.z)));
 if(b.z<15)b=add(b,mul(sub(a,b),(15-b.z)/(a.z-b.z)));
 Point p=project(a),q=project(b);line(p.x,p.y,q.x,q.y,ink);
}
static void race_neon_projectile_art(void);
static void race_projectile_art(void){
 if(game.race.layout==5&&neon_depth_mode){race_neon_projectile_art();return;}
 RaceArcade *a=&game.race.arcade;
 for(int i=0;i<RA_PROJECTILES;i++){RaceProjectile *p=&a->shot[i];if(!p->live)continue;
  race_world_line(p->pos,add(p->pos,mul(p->dir,p->kind==RA_MISSILE?-430:-700)),p->kind==RA_MISSILE?RGB(255,111,81):RGB(255,239,160));
 }
}
#if 0 /* Superseded by the lighter authoritative missile-direction alert. */
static void race_rear_view(void){
 static unsigned cache[204*46],cache_seed,cache_frame;static int cache_valid;
 /* The mirror remains a real second camera, but a 30 Hz instrument is ample
  * at this size. Reuse its previous complete panel on alternate main frames
  * so the 480x272 view keeps the motion budget. */
 if(cache_valid&&cache_seed==game.race.seed&&(cache_frame++&1)){
  for(int y=0;y<46;y++)memcpy(fb+(y+6)*STRIDE+138,cache+y*204,204*sizeof(unsigned));
  return;
 }
 Vec3 eye=camera_origin(&game),right,up,ahead;camera_axes(&game,&right,&up,&ahead);
 float yaw=atan2f(ahead.x,ahead.z),pitch=asinf(fmaxf(-1,fminf(1,ahead.y)));
 Vec3 br={cosf(yaw),0,-sinf(yaw)},bu={-sinf(yaw)*sinf(pitch),cosf(pitch),-cosf(yaw)*sinf(pitch)};
 float roll=atan2f(-dot(up,br),dot(up,bu));
 int ox=proj_ox,oy=proj_oy,x0=clipx0,x1=clipx1,y0=clipy0,y1=clipy1;
 float focal=proj_focal,light=surface_light;int ymin=surface_y_min,ymax=surface_y_max,depth=surface_depth_on,encoding=mega_depth_encoding,material=surface_material;
 rect(138,6,204,46,RGB(18,12,34));preview_clip(240,29,140,9,340,49);proj_focal=125;
 camera_override_set(game.pos,game.yaw+3.14159265f,-game.pitch,0);
 surface_y_min=9;surface_y_max=48;surface_depth_on=1;mega_depth_encoding=1;surface_material=0;surface_light=1;
 for(int y=9;y<49;y++)memset(surface_depth+y*W+140,255,200*sizeof(surface_depth[0]));
 race_starfield();
 if(game.race.layout==4){for(int y=24;y<49;y++){float t=(y-24)/24.f,half=38+t*t*62;int left=240-half,right=240+half;for(int band=0;band<4;band++){int a=left+(right-left)*band/4,b=left+(right-left)*(band+1)/4;rect(a,y,b-a+1,1,(band&1)?RGB(48,29,61):RGB(33,22,50));}pixel(240,y,RGB(220,99,195));if(!((y-24)%8))line(left,y,right,y,RGB(87,44,88));}}
 race_scenery();race_service_art();race_pickup_art();
 for(int i=0;i<game.race.rivals;i++)race_ship_draw(&game.race.pilot[i],i);flush_solid_meshes();if(game.race.layout!=4)race_holo_track_draw();race_pad_art();race_boundary_glass();race_ghost_art();race_projectile_art();race_dressing_art();race_apex_effects(1);
 camera_override_set(eye,yaw,pitch,roll);proj_ox=ox;proj_oy=oy;proj_focal=focal;clipx0=x0;clipx1=x1;clipy0=y0;clipy1=y1;
 surface_y_min=ymin;surface_y_max=ymax;surface_depth_on=depth;mega_depth_encoding=encoding;surface_material=material;surface_light=light;
 line(138,6,341,6,RGB(220,106,204));line(138,51,341,51,RGB(59,142,153));line(138,6,138,51,RGB(61,80,105));line(341,6,341,51,RGB(61,80,105));
 if(game.race.arcade.warning>=0&&game.race.arcade.warning_side<0)race_corners(139,8,201,41,5,RGB(255,89,78));
 for(int y=0;y<46;y++)memcpy(cache+y*204,fb+(y+6)*STRIDE+138,204*sizeof(unsigned));cache_seed=game.race.seed;cache_valid=1;
}
#endif
static void race_item_glyph(int x,int y,int item,unsigned ink){
 /* Filled 28px authored silhouettes, not rectangular recolour overlays. */
 const char *shapes[]={"", "00000111000000/00001110000000/00011100000000/00111000000000/01111111111000/00000111110000/00000111100000/00001111000000/00001110000000/00011100000000/00111000000000/00110000000000/01100000000000/01000000000000",
 "00000011000000/00000111100000/00001111110000/00011111111000/00011111111000/00011111111000/00001111110000/00001111110000/00001111110000/00011111111000/00111100111100/00111000011100/00001000010000/00000000000000",
 "00100010001000/01110111011100/01110111011100/01110111011100/01110111011100/01110111011100/01110111011100/01110111011100/01110111011100/01110111011100/01110111011100/00000000000000/00000000000000/00000000000000",
 "00000011000000/00011111111000/01111111111110/01111111111110/01111111111110/01111111111110/01111111111110/01111111111110/00111111111100/00111111111100/00011111111000/00001111110000/00000111100000/00000011000000"};
 if(item<=RA_EMPTY||item>=RA_ITEM_COUNT)return;const char *shape=shapes[item];for(int row=0;row<14;row++)for(int col=0;col<14;col++)if(shape[row*15+col]=='1')rect(x+col*2,y+row*2,2,2,ink);
}
#include "erl-broadcast-ticker.h"
static void race_speed_type(int x,int y,int scale,unsigned ink,const char *label){
 for(int i=0;label[i];i++){const unsigned char *glyph=font8[(unsigned char)label[i]-32];for(int row=0;row<8;row++)for(int column=0;column<8;column++)if(glyph[row]&(128>>column)){int dx=i*8*scale+column*scale;rect(x+dx+(7-row)*scale/3,y+row*scale-dx/40,scale,scale,ink);}}
}
/* Sweep only the authored glyph pixels: no full-screen glow pass or buffers. */
static void race_position_type(int value,int total){
 static unsigned seed;static int previous;static float changed,last;SpaceRace *r=&game.race;
 if(seed!=r->seed||r->elapsed<last){seed=r->seed;previous=value;changed=-10;}if(previous!=value){previous=value;changed=r->elapsed;}last=r->elapsed;
 float age=r->elapsed-changed,bump=age<.28f?sinf(age/.28f*3.14159265f):0;int scale=value>9?2:3;char label[12];snprintf(label,sizeof(label),"%d",value);int x=value>9?9:13,y=(value>9?23:17)-(int)(bump*2),lastpx=x;
 unsigned soft=value==1?RGB(108,61,78):RGB(59,50,96),ink=value==1?RGB(255,237,181):RGB(245,222,240);
 for(int i=0;label[i];i++){const unsigned char *glyph=font8[label[i]-32];for(int row=0;row<8;row++)for(int col=0;col<8;col++)if(glyph[row]&(128>>col)){int px=x+i*8*scale+col*scale,py=y+row*scale;rect(px-1,py-1,scale+2,scale+2,soft);lastpx=lastpx>px+scale?lastpx:px+scale;}}
 int grow=(int)(bump*1.5f);for(int i=0;label[i];i++){const unsigned char *g=font8[label[i]-32];for(int row=0;row<8;row++)for(int col=0;col<8;col++)if(g[row]&(128>>col))rect(x+i*8*scale+col*scale+(col>=4?grow:0),y+row*scale+(row>=4?grow:0),scale,scale,ink);}
 int sweep=(int)(r->elapsed*8)%16;for(int i=0;label[i];i++){const unsigned char *g=font8[label[i]-32];for(int row=0;row<8;row++)for(int col=0;col<8;col++)if(row+col==sweep&&(g[row]&(128>>col)))rect(x+i*8*scale+col*scale,y+row*scale,scale,scale,WHITE);}
 if(total>0){char counter[12];snprintf(counter,sizeof(counter),"/%d",total);for(int i=0;counter[i];i++){const unsigned char *g=font8[counter[i]-32];for(int y=0;y<12;y++)for(int x=0;x<12;x++)if(g[y*8/12]&(128>>(x*8/12)))pixel(38+i*12+x,30+y,RGB(91,224,210));}}
 if(age>=0&&age<.6f)for(int i=0;i<8;i++){float phase=i*.785398f;int sx=13+10+(int)(cosf(phase)*(6+age*22)),sy=24+(int)(sinf(phase)*(4+age*12));if(sx>8&&sx<65&&sy>19&&sy<43){pixel(sx,sy,value==1?GOLD:RGB(255,133,225));if(age<.25f)pixel(sx,sy-1,WHITE);}}

}
static void race_apex_panel(int x,int y,int w,int h){rect(x,y,w,h,RGB(24,16,43));line(x+4,y,x+w-4,y,RGB(72,45,81));line(x,y+4,x,y+h-4,RGB(69,42,80));}
static void race_reserve_fire(float reserve){
 static float previous,last,started,refilled;static unsigned seed;static int edge;
 SpaceRace *r=&game.race;if(seed!=r->seed||r->elapsed<last){seed=r->seed;previous=reserve;started=refilled=-10;}if(reserve<previous-.1f){edge=(int)(65*previous/100);started=r->elapsed;}if(reserve>previous+10)refilled=r->elapsed;last=r->elapsed;previous=reserve;
 int fill=(int)(65*fmaxf(0,fminf(100,reserve))/100);rect(399,237,65,4,RGB(27,41,55));rect(399,237,fill,4,RGB(255,161,61));
 float age=r->elapsed-started;if(reserve>0&&age>=0&&age<.4f)for(int i=0;i<4;i++){int x=399+edge-2+i-(int)(age*(7+i*2)),y=234-(int)(age*(12+i*9));if(x>=399&&x<464&&y>=220)pixel(x,y,age<.15f?RGB(255,238,161):RGB(232,115,51));}
 /* Eleven irregular, extinguishing pixel tongues. Actual reserve controls
  * visible fuel; actual drive state controls the nitrous cores. */
 /* Frame background is already fresh; never paint a black flame rectangle. */
 float refill_age=r->elapsed-refilled,kick=refill_age>=0&&refill_age<.55f?sinf(refill_age/.55f*3.14159265f):0;
 int boost=reserve>0&&(game.boost||r->boost_time>0||r->arcade.actor[0].rocket>0);
 for(int i=0;i<11;i++){int x=400+i*6;if(x>=399+fill)continue;
  unsigned flicker=i*73u;int extinguished=0,height=5+(int)(4+4*sinf(r->elapsed*9+i*2.3f)+2*sinf(r->elapsed*17+i))+boost*5+(int)(kick*10),lean=(int)(2*sinf(r->elapsed*6+i));
  for(int y=0;y<height;y++){int cx=x+lean*y/height,half=y<height/3?2:y<height*2/3?1:0;for(int dx=-half;dx<=half;dx++)pixel(cx+dx,236-y,RGB(240,86,47));if(y<height*3/4)pixel(cx,236-y,RGB(255,184,74));if(y<height/2)pixel(cx,236-y,boost?RGB(144,255,255):RGB(255,240,171));}
  if(!extinguished&&((flicker>>12)&3)==0)pixel(x+lean,233-height,RGB(252,163,82));
 }
 rect(399,237,fill,4,RGB(102,232,215));

}
static void race_hud(void){
 SpaceRace *r=&game.race;RaceArcade *arc=&r->arcade;RaceItemActor *actor=&arc->actor[0];
 if(!race_is_active(&game)||game.docked||game.dock_stage)return;
 unsigned cream=RGB(243,232,199),teal=RGB(91,224,210),panel=RGB(7,22,32),dim=RGB(31,59,70),red=RGB(255,89,78);
 if(r->layout!=5&&r->phase==RACE_COUNTDOWN&&r->countdown>3.9f)return;
 char buf[32];race_apex_panel(7,6,108,38);line(69,11,69,41,RGB(67,50,89));text_px(40,11,teal,arc->rules.mode==RA_TIME_TRIAL?"GOLD":arc->rules.objective==RA_TAKEDOWNS?"K.O":"POS");line(40,21,62,21,teal);text_px(78,11,teal,arc->rules.objective==RA_TAKEDOWNS&&arc->rules.seconds?"TIME":"LAP");line(78,18,125,18,teal);
 if(arc->ready&&arc->rules.mode==RA_TIME_TRIAL){float delta=race_gold_delta(&game);snprintf(buf,sizeof(buf),"%+.1f",delta);race_type(12,25,1,delta<=0?teal:red,buf);}
 else if(arc->rules.objective==RA_TAKEDOWNS)race_position_type(r->takedowns,arc->rules.required);
 else race_position_type(r->place,r->rivals+1);
 int lap=r->gate*r->laps/RACE_GATES+1;if(lap>r->laps)lap=r->laps;
 if(arc->rules.objective==RA_TAKEDOWNS&&arc->rules.seconds){int seconds=(int)fmaxf(0,arc->rules.seconds-r->elapsed);snprintf(buf,sizeof(buf),"%d:%02d",seconds/60,seconds%60);}else snprintf(buf,sizeof(buf),"%d/%d",lap,r->laps);race_type(78,29,1,cream,buf);
 text_px(400,10,RGB(255,120,220),"ERL LIVE");text_px(400,23,cream,"%02d:%05.2f",(int)r->elapsed/60,fmodf(r->elapsed,60));circle(391,13,2,GOLD);race_broadcast_draw();
 if(arc->warning>=0){int rear=arc->warning_side<0,ay=54,ax=rear?198:189,tip=rear?ay+10:ay,base=rear?ay+4:ay+6;unsigned alert=((int)(r->elapsed*8)&1)?red:RGB(155,55,76);line(ax-4,base,ax,tip,alert);line(ax,tip,ax+4,base,alert);text_px(rear?207:198,ay,alert,rear?"MISSILE / REAR":"MISSILE / AHEAD");}
 if(r->phase==RACE_COUNTDOWN&&r->layout==5){char digit[8];if(r->countdown>4){race_apex_panel(192,74,96,32);race_type(200,80,2,teal,"READY");}else{race_apex_panel(216,68,48,44);snprintf(digit,sizeof(digit),"%d",(int)ceilf(r->countdown));race_type(224,74,4,cream,digit);}return;}
 if(r->phase==RACE_COUNTDOWN&&r->countdown>3.8f)return;
 race_apex_rush();
 if(arc->last_hit==4&&arc->cue_age>0&&r->boost_time<=0){float age=1-arc->cue_age/.45f;for(int i=0;i<10;i++){float angle=i*.628319f,radius=65+age*170;int x=240+cosf(angle)*radius*1.7f,y=136+sinf(angle)*radius;unsigned glass=planet_rgb_mix(teal,panel,(int)(age*170));line(x,y,x+11,y-3,glass);line(x+11,y-3,x+4,y+8,glass);line(x+4,y+8,x,y,glass);}}
 if(arc->last_hit==5&&arc->cue_age>0){float age=1-arc->cue_age/.25f;unsigned ink=planet_rgb_mix(cream,panel,(int)(age*200));for(int side=0;side<2;side++)for(int i=0;i<3;i++){int x=side?477:2,dx=side?-1:1,y=98+i*35;line(x,y,x+dx*(10+(int)(age*20)),y+8,ink);}}
 if(arc->last_hit==3&&arc->cue_age>0){float age=1-arc->cue_age/.6f;unsigned pink=RGB(255,98,218);for(int side=0;side<2;side++){int x=side?466:13,dir=side?-1:1;line(x-5,170,x+5,170,pink);line(x,165,x,175,pink);for(int i=0;i<5;i++){float t=fmodf(age*2+i*.2f,1);int sx=x+dir*(int)(t*75),sy=174+(int)(t*35);line(sx,sy,sx+dir*6,sy+3,pink);line(sx,sy+1,sx+dir*6,sy+4,cream);}}}
 if(actor->shield>0||actor->shield_break>0){
  float age=actor->shield_break>0?1-actor->shield_break/.35f:1-fminf(1,actor->shield/3);
  unsigned blue=RGB(75,156,255);race_corners(1,80,477,128,actor->shield_break>0?8+(int)(age*35):16,blue);
  if(actor->shield>0){unsigned rim=planet_rgb_mix(blue,panel,45+(int)(20*sinf(r->elapsed*8)));line(0,80,0,208,rim);line(479,80,479,208,rim);line(1,208,478,208,rim);}
  if(actor->shield>2.6f){float radius=(3-actor->shield)*330;for(int i=0;i<24;i++){float t=i*6.2831853f/24,n=(i+1)*6.2831853f/24;line(240+cosf(t)*radius,130+sinf(t)*radius*.65f,240+cosf(n)*radius,130+sinf(n)*radius*.65f,blue);}}
  if(actor->shield_break>0)for(int corner=0;corner<4;corner++)for(int shard=0;shard<3;shard++){int sx=corner&1?-1:1,sy=corner&2?-1:1,x=(corner&1?478:1)+sx*(18+shard*14)-(int)(sx*age*65),y=(corner&2?208:80)+sy*(9+shard*7)-(int)(sy*age*42);line(x,y,x+sx*11,y+sy*3,blue);line(x+sx*11,y+sy*3,x+sx*4,y+sy*10,blue);line(x+sx*4,y+sy*10,x,y,blue);}
 }
 race_apex_panel(7,228,124,36);race_apex_panel(177,232,128,32);race_apex_panel(342,230,131,34);
 int item=actor->shuffle>0?ra_shuffle_icon(arc->rules.pool,actor->shuffle):actor->item;
 int usable=r->phase==RACE_RUNNING&&!game.erl.paused&&!actor->eliminated&&actor->health>0&&actor->protection<=0&&actor->spin<=0&&r->repair<=0&&r->handover<=0&&r->crash_time<=0&&actor->shuffle<=0&&item!=RA_EMPTY&&(item!=RA_MINIGUN||actor->ammo>0)&&(item!=RA_MISSILE||r->rear_input||(arc->lock>=1&&arc->lock<=r->rivals&&!arc->actor[arc->lock].eliminated&&!r->pilot[arc->lock-1].disabled))&&!actor->require_release;
 race_item_glyph(12,232,item,usable?item==RA_BOOST?RGB(51,157,255):teal:dim);
 if(usable){float pulse=.5f+.5f*sinf(r->elapsed*4.6542f);int dx=(int)(r->elapsed*12)%18;line(14+dx,240,17+dx,237,RGB(164,250,255));pixel(20,247-(int)(pulse*4),WHITE);}
 const char *names[]={"EMPTY","BOOST","MISSILE","MINIGUN","SHIELD"};text_px(46,234,teal,"%s",actor->shuffle>0?"ROLL":item==RA_MISSILE&&r->rear_input?"REAR":names[item]);
 if(item==RA_MINIGUN&&actor->shuffle<=0)text_px(46,249,cream,"%02d",actor->ammo);else text_px(46,249,usable?cream:dim,usable?"READY":item==RA_MISSILE&&arc->lock<0?"LOCK?":actor->shuffle>0?"...":"WAIT");
 circle(112,247,9,RGB(40,57,80));circle(112,247,5,RGB(255,132,174));
 snprintf(buf,sizeof(buf),"%d",(int)(game.speed*(game.race.layout==5?.228571429f:game.race.layout==4?.121428571f:2.236936f)));int scale=strlen(buf)>5?1:2;race_type(186,239,scale,cream,buf);text_px(270,252,teal,"MPH");
 text_px(350,236,teal,"BOOST");race_reserve_fire(r->reserve);text_px(350,251,teal,"HULL");rect(399,252,65,4,dim);rect(399,252,65*fmaxf(0,actor->health)/RA_HEALTH,4,actor->health<RA_HEALTH/4?red:teal);
 if(actor->health>0&&actor->health<RA_HEALTH/4&&((int)(r->elapsed*9)&3)==0){line(8,215,70,215,dim);line(407,215,472,215,dim);}
 if(actor->health>0&&actor->health<RA_HEALTH/4){line(1,90,12,101,red);line(12,101,5,112,red);line(478,170,463,185,red);line(463,185,472,197,red);}
 if(r->layout==5&&actor->decision>0)text_px(188,216,RGB(148,255,198),"REPAIR / HULL");
 if(arc->lock>=1&&item==RA_MISSILE&&!r->rear_input){Vec3 v=camera(&game,r->pilot[arc->lock-1].pos);if(v.z>15){Point p=project(v);int size=12+(int)(6*(1-fminf(1,arc->lock_age/.15f)));race_corners(p.x-size,p.y-size,size*2,size*2,5,red);}}
 if((arc->rules.objective==RA_HUNTER||arc->rules.objective==RA_ESCAPE)&&arc->warning<0){text_px(188,184,teal,"CAPTURE");rect(180,198,120,5,dim);rect(180,198,(int)(120*arc->actor[arc->rules.objective==RA_HUNTER?0:1].capture/4),5,arc->rules.objective==RA_HUNTER?teal:red);}
 if(actor->protection>0){float fade=fminf(1,actor->protection/3);unsigned white=planet_rgb_mix(cream,panel,(int)(180*(1-fade)));for(int i=0;i<8;i++){int x=6+i*66,y=i&1?210:82;line(x,y,x+5,y,white);if(((int)(r->elapsed*12)+i)%4==0)pixel(x+2,y-2,cream);}}
 if(arc->rules.sponsor&&!arc->sponsor_done&&arc->warning<0){int count=0;unsigned bits=arc->branch_boost[0];while(bits){count+=bits&1;bits>>=1;}int value=arc->rules.sponsor==1?actor->health*100/RA_HEALTH:arc->rules.sponsor==2?r->takedowns:arc->rules.sponsor==3?count:!r->knockouts,target=arc->rules.sponsor==1?75:arc->rules.sponsor==2?2:arc->rules.sponsor==3?3:1;text_px(14,202,teal,"SPONSOR %d/%d",value,target);rect(14,213,106,2,dim);rect(14,213,106*fminf(1,(float)value/target),2,teal);}
 if(arc->sponsor_age>0&&arc->warning<0){int sx=14+(int)(fmaxf(0,.4f-arc->sponsor_age)*-200);line(sx,202,sx+3,205,GOLD);line(sx+3,205,sx+8,198,GOLD);text_px(sx+13,204,GOLD,"SPONSOR COMPLETE %s",erl_sponsor_module(&arc->rules)?erl_sponsor_room(&game)?"400U + MODULE":"800U":"400U");}
 if(r->phase==RACE_GRID){text_px(152,99,cream,"START GRID");return;}
 if(r->phase==RACE_COUNTDOWN)return; /* Three lights live on the real gantry. */
 if(r->elapsed<.8f){int length=(int)(200*fminf(1,r->elapsed/.4f));line(240-length,214,240+length,214,RGB(104,255,159));return;}
 if(r->layout==5){RaceTrackFrame f=race_track_nearest(r,r->gate,game.pos);if(dot(forward(&game),f.ahead)<-.1f)text_px(204,65,red,"WRONG WAY");}else{Vec3 v=camera(&game,race_waypoint(&game));if(v.z<0)text_px(204,65,red,"WRONG WAY");}
}
