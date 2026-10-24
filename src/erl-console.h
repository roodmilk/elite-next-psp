/* Broadcast console. All positions and glyphs are native 480x272. */
enum {ERL_HOME,ERL_BRIEF,ERL_SEASON,ERL_DRIVER,ERL_RIVALS,ERL_GARAGE,ERL_STANDINGS,ERL_REWARDS,ERL_MECHANIC,ERL_TALK,ERL_EVENTS,ERL_RIVAL_TALK,ERL_WELCOME};
static int erl_board_brief;
static int erl_requested_kind,erl_requested_layout,erl_requested_type;
static int erl_screen,erl_choice,erl_rival_id,erl_garage_row,erl_confirm,erl_confirm_yes;
static float erl_motion_entry_age=10;
static float erl_anim,erl_reveal,erl_rtap=10;static unsigned erl_result_serial;
static float erl_vertical_speed,erl_drift_time;static int erl_drift_direction,erl_rear_latch;static unsigned erl_vertical_serial,erl_drift_serial,erl_rear_serial;
static char erl_notice[96];
#define EB RGB(16,11,38)
#define EP RGB(39,26,72)
#define EC RGB(255,240,203)
#define ET RGB(123,255,240)
#define ED RGB(146,164,181)
#define ER RGB(255,96,206)
static const char *erl_division_name(int n){static const char *names[]={"ROOKIE","CLUB","PRO","ELITE"};return names[n<0?0:n>3?3:n];}
static const char *erl_driver_name(int id){
 if(!id)return game.commander_name;if(id==1)return "RAFE VOSS";
 static const char *first[]={"MILO","NIA","ORLA","TARO","IVO","MARA","KEZ","JUNO","ELI","ADA","LENA","RORY"};static const char *last[]={"QUINN","SOL","KEEN","VALE","REED","NORTH","AVEN","CAIRN","DALE","WREN","ASH","PARK","MOSS","HOLT","SHAW","LYNN"};
 static char names[4][24];static int slot;slot=(slot+1)&3;int roster=game.erl.event_kind==ERL_OFFICIAL?game.erl.champ_system:game.system;unsigned hash=station_profile_hash(roster,397);snprintf(names[slot],24,"%s %s",first[(roster+id*3)%12],last[((hash>>8)+id*7)&15]);return names[slot];
}
static unsigned erl_portrait(int id){unsigned d=portrait_expand_dna(portrait_parts_dna(portrait_seeded(id?0xe710u+(id-1)*101u+(id>1?(game.erl.event_kind==ERL_OFFICIAL?game.erl.champ_system:game.system)*419u:0):0x70bb,TRADERS,0)));if((id==0||id==1)&&portrait_part_value(d,11))d=portrait_adjust(d,11,1);return d;}
static void erl_text(int x,int y,unsigned ink,const char *s,int scale){
 for(int i=0;s[i];i++){unsigned ch=(unsigned char)s[i];if(ch>='a'&&ch<='z')ch-=32;if(ch<32||ch>126)ch='?';
  for(int yy=0;yy<8;yy++)for(int xx=0;xx<8;xx++)if(font8[ch-32][yy]&(128>>xx))rect(x+i*8*scale+xx*scale,y+yy*scale,scale,scale,ink);
 }
}
static void erl_fit(int x,int y,int width,unsigned ink,const char *s){
 int n=(int)strlen(s),cols=width/8;if(n<=cols){erl_text(x,y,ink,s,1);return;}
 float t=fmaxf(0,erl_anim-1);int offset=(int)(t*13)%(n*8+32);int oldx0=clipx0,oldx1=clipx1; /* Text has its own exact clipping. */
 for(int i=0;i<n;i++){int px=x+i*8-offset;unsigned ch=(unsigned char)s[i];if(ch<32||ch>126)ch='?';for(int yy=0;yy<8;yy++)for(int xx=0;xx<8;xx++)if(px+xx>=x&&px+xx<x+width&&(font8[ch-32][yy]&(128>>xx)))pixel(px+xx,y+yy,ink);}
 (void)oldx0;(void)oldx1;
}
static void erl_box(int x,int y,int w,int h,unsigned fill,unsigned edge){rect(x,y,w,h,fill);line(x,y,x+w-1,y,edge);line(x,y+h-1,x+w-1,y+h-1,edge);line(x,y,x,y+h-1,edge);line(x+w-1,y,x+w-1,y+h-1,edge);}
static void erl_stars(int top,int bottom){
 for(int i=0;i<530;i++){unsigned h=station_profile_hash(game.system,i+177);int x=2+h%476,y=top+((h>>10)%(bottom-top));int light=(h>>24)&127;unsigned c=i%19?RGB(30+light/2,47+light/2,66+light/2):RGB(164,216,229);pixel(x,y,c);if(i%53==0&&y>top+1&&y<bottom-2){pixel(x-1,y,RGB(41,87,106));pixel(x+1,y,RGB(41,87,106));pixel(x,y-1,RGB(41,87,106));pixel(x,y+1,RGB(41,87,106));}}
}
#include "erl-hyper-chrome.h"
static void erl_header(const char *title,const char *right){
 erl_backdrop();rect(0,0,W,24,RGB(16,20,28));for(int yy=0;yy<3;yy++)for(int xx=0;xx<4;xx++)rect(8+xx*3,7+yy*3,3,3,(xx+yy)&1?EC:ER);erl_text(29,8,ET,title,1);int n=strlen(right);if(n<=20)erl_text(n<=9?400:472-n*8,8,ET,right,1);else erl_fit(312,8,160,ET,right);
}
static void erl_footer(const char *action){(void)action;} /* No redundant control legend on ERL pages. */
static void erl_button(int x,int y,int w,const char *name,int chosen,int enabled){
 unsigned fill=chosen&&enabled?ER:RGB(16,20,28),edge=chosen?ET:planet_rgb_mix(ER,EC,60);erl_chamfer(x,y,w,24,fill,edge);
 line(x+10,y+2,x+w-11,y+2,chosen?EC:planet_rgb_mix(ER,EB,90));if(!enabled)line(x+8,y+6,x+15,y+13,RED);
 int px=x+(w-(int)strlen(name)*8)/2;erl_text(px,y+8,chosen&&enabled?RGB(16,20,28):enabled?EC:ED,name,1);
}
#include "erl-card-icons.h"
static void erl_card_value(int x,int y,int width,unsigned ink,const char *value){int n=strlen(value);if(n*16<=width)erl_text(x,y,ink,value,2);else erl_fit(x,y+4,width,ink,value);}
static void erl_flag(int x,int y,int cell){for(int yy=0;yy<3;yy++)for(int xx=0;xx<4;xx++)rect(x+xx*cell,y+yy*cell,cell,cell,(xx+yy)&1?EB:EC);}
static unsigned erl_preview_builds;
static void erl_preview(int x,int y,int w,int h,int layout){
 /* Cache the native render, never a stretched screenshot or invented doodle.
  * Each card renders the real authored course and racing ship meshes once.
  * Menu animation therefore does not rebuild a 3D world every frame. */
 static unsigned char images[464*159*3];static unsigned key_saved;static int width_saved,height_saved,layout_saved=-1;layout&=3;if(w<1||w>464||h<1||h>159)return;
 int picker=erl_screen==ERL_EVENTS;int venue=game.erl.event_kind?game.erl.event_system:game.erl.championship&&(erl_screen==ERL_HOME||(erl_screen==ERL_BRIEF&&erl_requested_kind==ERL_OFFICIAL))?erl_round_venue(&game,layout):game.system;unsigned key=1+picker*7919u+venue*73u+game.station_variant*5477u+game.ship*997u+ship_paint[game.ship]*17u;
 if(key_saved!=key||width_saved!=w||height_saved!=h||layout_saved!=layout){erl_preview_builds++;
  SpaceRace saved=game.race;Vec3 position=game.pos;int kind=game.erl.event_kind,type=game.erl.event_type,oldlayout=game.erl.event_layout;
  game.erl.event_kind=ERL_PRACTICE;game.erl.event_type=layout==1?ERL_SPRINT:ERL_CIRCUIT;game.erl.event_layout=layout;
  if(venue==game.system)race_course_build(&game,game.station_variant,&game.race);else {Game *view=malloc(sizeof(*view));if(view){*view=game;view->system=venue;view->station_variant=0;system_bodies(view);race_course_build(view,0,&game.race);free(view);}else race_course_build(&game,game.station_variant,&game.race);}SpaceRace *r=&game.race;r->gate=8;r->phase=RACE_RUNNING;
  RaceGate *q=&r->gates[8];Vec3 eye=add(q->pos,add(mul(q->normal,picker?-12000:-15000),add(mul(q->right,-1100),mul(q->up,3200))));Vec3 look=norm(sub(add(q->pos,mul(q->normal,5000)),eye));
  game.pos=eye;camera_override_set(eye,atan2f(look.x,look.z),asinf(look.y),-.035f);
  erl_box(x,y,w,h,EB,ED);preview_clip(picker?x+55:x+w/2,y+h/2,x+1,y+1,x+w-1,y+h-1);race_starfield();drawcount=0;
  race_scenery();for(int i=8;i<11;i++){RaceGate *gate=&r->gates[i];race_ring_draw(gate,gate->pos,gate->radius,0,0);if(i>8)race_ring_draw(gate,gate->booster,1150,1,0);}
  const char *hulls[]={"KRAIT","MAMBA","COBRA MK 3"};for(int i=0;i<3;i++){RaceRival p={0};p.mesh=mesh_id(hulls[i]);p.dir=norm(add(q->normal,mul(q->up,.35f)));p.radius=200;p.pos=add(eye,add(mul(q->normal,950+i*1450),add(mul(q->right,(i-1)*210.f),mul(q->up,-100-i*120.f))));race_ship_draw(&p,i);}
  flush_meshes();preview_reset();camera_override_clear();game.pos=position;game.race=saved;game.erl.event_kind=kind;game.erl.event_type=type;game.erl.event_layout=oldlayout;
  for(int yy=0;yy<h;yy++)for(int xx=0;xx<w;xx++){unsigned c=fb[(y+yy)*STRIDE+x+xx];int at=(yy*w+xx)*3;images[at]=c;images[at+1]=c>>8;images[at+2]=c>>16;}key_saved=key;width_saved=w;height_saved=h;layout_saved=layout;
 }else for(int yy=0;yy<h;yy++)for(int xx=0;xx<w;xx++){int at=(yy*w+xx)*3;fb[(y+yy)*STRIDE+x+xx]=RGB(images[at],images[at+1],images[at+2]);}
 erl_flag(x+9,y+8,2);erl_text(x+23,y+8,EC,"ERL",1);
}
static int erl_commit(void){
 if(erl_quick_race){game.erl.dirty=0;return 1;}
 if(erl_checkpoint_collect(1)<0)game.erl.dirty=1;
 if(!game.erl.dirty)return 1;if(!opening_save_allowed(&game))return 0;
 if(!game.erl.event_kind||game.erl.result)erl_ship_capture(&game,ship_paint[game.ship]);
 char note[160];snprintf(note,sizeof(note),"%s",game.message);float age=game.message_time;int cue=game.cue;
 if(save_game(&game,commander_save_path(&game))){game.erl.dirty=0;game.erl.save_age=0;}else snprintf(erl_notice,sizeof(erl_notice),"SAVE FAILED / KEEP THIS SESSION OPEN");
 snprintf(game.message,sizeof(game.message),"%s",note);game.message_time=age;game.cue=cue;return !game.erl.dirty;
}
static int erl_brief_kind(void){return erl_screen==ERL_MECHANIC?ERL_INTRO:!game.erl.referral?0:!game.erl.licence?ERL_QUALIFIER:game.erl.championship?ERL_OFFICIAL:erl_choice==2?ERL_PRACTICE:ERL_QUALIFIER;}
static int erl_layout(void){return game.erl.championship?game.erl.round:game.erl.event_layout&3;}
static int erl_discipline(int layout){return layout==2?ERL_RUSH:(layout==0||layout==3)?ERL_CIRCUIT:ERL_SPRINT;}
#include "erl-hyper-ui.h"
static void erl_rival_talk_draw(void){
 int oldrow=row;row=0;dialogue_begin("ERL / PADDOCK CHANNEL");const char *speech;
 if(erl_rival_id!=1)speech=erl_rival_id%3==1?"Keep your nose clean through the tight section. I save my boost for the exit; there is no prize for being first into a wall.":erl_rival_id%3==2?"Those cyan pads make a difference. I pick a line before the turn, then save my manual boost for a clear exit.":"I like a bit of contact racing. Give me a proper shoulder check, though. We are here for a race, not a parking dispute.";
 else if(game.erl.season_place==1)speech="A championship trophy. Fair enough, Commander; you earned that one. Enjoy the prize ship. I will see you in the next league.";
 else if(game.erl.result_takedowns)speech="I saw those takedowns. You have certainly made an impression on the repair crew. Next time, let us see if you can catch me through the corners.";
 else if(game.erl.result_place==1)speech="All right, that was proper flying. You kept enough boost for the finish. Enjoy it while it lasts; I am learning your lines.";
 else if(game.erl.result_place)speech="Rough round? Happens to everyone. Brake a little earlier and keep your eyes on the next gate. I want a proper fight next time.";
 else speech="Rafe Voss. You must be Torr's new prospect. I hope you can turn as well as you can hold the throttle. See you on the grid.";
 dialogue_speech(erl_driver_name(erl_rival_id),TRADERS,(int)erl_portrait(erl_rival_id),speech,0);dialogue_reply(0,"Back to the paddock");narrative_footer();row=oldrow;
}
#include "erl-motion.h"
#include "erl-replay.h"
static void erl_console_draw_raw(void){
 if(erl_screen==ERL_RIVAL_TALK){erl_rival_talk_draw();return;}
 if(erl_screen==ERL_EVENTS){erl_events_draw();return;}
 if(erl_screen==ERL_WELCOME){
  int oldrow=row;row=0;dialogue_begin("ERL / WELCOME TO THE LEAGUE");
  dialogue_speech(erl_organiser(&game,game.system),TRADERS,(int)portrait_seeded(game.system*173u+61u,TRADERS,0),
   "Welcome, Commander! Torr sent your referral ahead. This is the Elite Racing League. Register a Racer and choose your livery; we regulate performance so the racing stays fair. Your first event is the rookie qualifier: two laps against six other pilots. Fly through the checkpoints in order and finish first to earn your licence. Blue rings and cyan pads give a short speed burst. Release R to recharge your manual reserve, then use it for overtaking and clear exits! There is no entry fee, and you can retry. Once you have your licence, our four-round championships offer units, racing equipment and prize ships. Ready? Let me show you the league desk and your rookie qualifier.",0);
  dialogue_objective("Win the rookie qualifier to earn your licence. Entry is free.");
  dialogue_reply(0,dialogue_page+1<dialogue_pages?"Tell me more":"Show me the ERL league");narrative_footer();row=oldrow;return;
 }
 if(erl_screen==ERL_TALK){
  int oldrow=row;row=erl_choice;int mechanic=game.system==7&&!erl_hq(&game),referred=mechanic&&game.erl.referral;char speech[384],objective[256];
  dialogue_begin("INTRODUCTION TO THE ERL");const char *speaker=mechanic?"TORR BELL":erl_organiser(&game,game.system);
  if(referred){if(game.erl.trophies)snprintf(speech,sizeof(speech),"A championship trophy! I saw the broadcast. I knew you had it in you. Your original ship and your prize ships are safe in the ERL Garage. Keep racing; I want to see what you do in the next league.");else if(game.erl.licence)snprintf(speech,sizeof(speech),"A proper racing licence! Lovely work. The Capitol runs four-round seasons. Take your time learning those corners, and save a bit of boost for the exit. I will be watching the broadcast.");else snprintf(speech,sizeof(speech),"Lovely flying, Commander. Your referral is with %s at %s Mega Capitol. Go and see them for your qualifier. There is no fee, and you can retry. Tell them Torr sent you!",erl_organiser(&game,game.erl.ref_system),game.systems[game.erl.ref_system].name);erl_objective(&game,objective,sizeof(objective));}
  else if(!mechanic&&!game.erl.referral){snprintf(speech,sizeof(speech),"Welcome to the league. Before I can arrange your qualifier, meet Torr Bell at Lave Hub. He runs a free one-on-one test. Win that and bring his referral back to an ERL headquarters.");snprintf(objective,sizeof(objective),"Find Introduction to the ERL on Lave Hub\'s Mission Board. Meet Torr Bell.");}
  else{snprintf(speech,sizeof(speech),"%s",mechanic?"Nice flying out there. I am Torr, the mechanic. I used to look after league racers. Fancy a test flight against me? No fee, no paperwork. Beat me and I will put in a word at the Capitol.":"Torr said you could fly. I am glad you made it. Win our qualifier and I will issue your rookie racing licence. Choose an ERL Racer with Triangle. Your spacefaring ship stays parked during races.");snprintf(objective,sizeof(objective),"%s",mechanic?"A short one-on-one race. Pass every gate in order. You can retry if you lose.":"Finish first to earn your racing licence. Then enter our four-round championship.");}
  dialogue_speech(speaker,TRADERS,(int)(mechanic?erl_portrait(0):portrait_seeded(game.system*173u+61u,TRADERS,0)),speech,0);dialogue_objective(objective);dialogue_reply(0,!mechanic&&!game.erl.referral?"Track Torr Bell":referred&&!game.erl.licence?"Plot route to ERL HQ":referred?"Go to mission log":mechanic?"Let's see the test course":"Show me the qualifier");dialogue_reply(1,"Maybe another time");narrative_footer();row=oldrow;return;
 }
 switch(erl_screen){case ERL_GARAGE:erl_garage_draw();break;case ERL_SEASON:erl_season_draw();break;case ERL_STANDINGS:erl_standings_draw();break;case ERL_DRIVER:erl_driver_draw();break;case ERL_RIVALS:erl_rivals_draw();break;case ERL_BRIEF:case ERL_MECHANIC:erl_brief_draw();break;default:erl_home_draw();}
 if(erl_notice[0]&&erl_screen!=ERL_GARAGE){rect(8,226,464,22,EB);erl_fit(16,233,448,GOLD,erl_notice);}
 if(erl_confirm){erl_chamfer(62,82,356,106,EP,ER);erl_text(78,94,GOLD,"WITHDRAW FROM THIS CHAMPIONSHIP?",1);erl_text(78,114,EC,"RECORDED ROUNDS AND LOSSES STAY.",1);erl_text(78,130,ED,"YOUR SHIP WILL BE UNLOCKED.",1);erl_button(80,155,148,"YES",erl_confirm_yes,1);erl_button(244,155,148,"NO",!erl_confirm_yes,1);}
}

static void erl_console_draw(void){erl_console_draw_raw();erl_motion_draw();}

static float erl_smooth(float t){t=fmaxf(0,fminf(1,t));return t*t*(3-2*t);}
#include "erl-shockwave.h"
static void erl_result_draw_raw(void){
 ErlCareer *e=&game.erl;if(!e->event_kind||!e->result)return;float t=erl_reveal;int won=e->result_place==1&&e->result==1;
 if(e->result==1){erl_shockwave_draw(t);return;}
 erl_backdrop();
 if(t>=2.2f||!won){rect(0,0,W,24,RGB(16,20,28));erl_flag(8,7,3);erl_text(29,8,ET,won?"ERL / RACE WON":"ERL / RACE RESULT",1);}
 if(!won&&t>.2f){char place[24];snprintf(place,sizeof(place),"PLACE %d",e->result_place);erl_text(16,42,EC,e->result==2?"WITHDRAWN":place,3);erl_text(16,82,ER,"RACE COMPLETE",2);}
 if((won&&t>3.4f)||(!won&&t>.5f)){
  erl_fit(16,119,288,ET,game.commander_name);erl_fit(16,135,288,ED,erl_course_name(&game,e->event_layout));erl_icon(ERLI_CLOCK,337,120);text_px(366,127,EC,"%d:%05.2f",(int)game.race.elapsed/60,fmodf(game.race.elapsed,60));
  float count=won?erl_smooth((t-3.95f)/.75f):erl_smooth((t-.5f)/.7f);const int xs[]={8,166,324};const char *labels[]={"UNITS","LEAGUE XP",""};char values[3][32];snprintf(values[0],32,"+%d",(int)(e->result_units*count)/10);snprintf(values[1],32,"+%d",(int)(e->result_xp*count));int icon=ERLI_FLAG;const char *detail="";
  int newship=e->event_kind==ERL_OFFICIAL&&e->round==4&&e->season_place==1&&e->result_units!=102000*(e->division+1);
  if(newship){snprintf(values[2],32,"%s",player_ships[erl_prizes[e->division]].name);labels[2]="PRIZE SHIP";detail="IN YOUR GARAGE";icon=ERLI_SHIP;}
  else if(e->event_kind==ERL_OFFICIAL){snprintf(values[2],32,"%d",e->round==4?e->season_place:e->result_points);labels[2]=e->round==4?"CUP PLACE":"POINTS";}
  else if(e->event_kind==ERL_QUALIFIER&&won){snprintf(values[2],32,"%s",erl_division_name(e->licence-1));labels[2]="LICENCE";}
  else if(e->event_kind==ERL_INTRO&&won){snprintf(values[2],32,"ERL HQ");labels[2]="REFERRAL";}
  else{snprintf(values[2],32,"%d",e->result_takedowns);labels[2]="TAKEDOWNS";}
  for(int i=0;i<3;i++){erl_chamfer(xs[i],160,148,66,EP,ER);erl_icon(i==0?ERLI_UNITS:i==1?ERLI_XP:icon,xs[i]+10,171);erl_card_value(xs[i]+43,176,98,i==0?GOLD:i==1?ET:ER,values[i]);erl_text(xs[i]+10,207,ED,labels[i],1);}if(detail[0])erl_fit(327,231,144,ET,detail);
 }
 float ready=won?4.75f:1.2f;if(t>ready){int retry=e->event_kind!=ERL_OFFICIAL&&e->result_place!=1;if(e->event_kind==ERL_OFFICIAL||retry){erl_button(8,240,228,retry?"RETRY":"STANDINGS",!erl_choice,1);erl_button(244,240,228,"CONTINUE",erl_choice,1);}else erl_button(8,240,464,"CONTINUE",1,1);}
}
static void erl_result_draw(void){if(game.race.layout==5&&game.race.crash_time>0)return;erl_result_draw_raw();if(game.erl.result&&erl_reveal>=(game.erl.result==1?SW_READY:1.2f)){if(erl_quick_race){rect(0,236,W,36,EB);erl_button(8,240,228,"RACE AGAIN",!erl_choice,1);erl_button(244,240,228,"MAIN MENU",erl_choice,1);}else erl_motion_draw();}}
/* Sleep pauses only an actual countdown/race. Also recover old pre-start
 * flags without withdrawing the accepted event or changing the flight state. */
static void erl_prestart_recover(void){
 if(!game.erl.result&&!race_is_active(&game)&&(game.erl.paused||game.erl.resume)){
  game.erl.paused=game.erl.resume=0;game.erl.dirty=1;erl_choice=erl_confirm=0;
 }
}
static void erl_sleep_pause(int checkpoint){
 erl_prestart_recover();
 if(game.erl.result||!race_is_active(&game))return;
 game.erl.paused=1;game.erl.dirty=1;erl_choice=erl_confirm=0;
 if(checkpoint)erl_commit();
}
static void erl_pause_draw_raw(void){
 if(game.erl.result||!game.erl.paused||!race_is_active(&game))return;
 erl_chamfer(62,77,356,119,EP,ER);erl_text(166,89,EC,"ERL / PAUSED",1);erl_text(78,110,ED,erl_quick_race?"PRACTICE / NO CAREER SAVES.":"YOUR RACE PROGRESS IS SAVED.",1);erl_text(78,126,ET,"X GO / L BRAKE / R BOOST / SQ DRIFT / O FIRE",1);
 if(game.race.layout==5)erl_text(78,140,ET,"NUB HEIGHT / DOWN + O REAR ROCKET",1);
 erl_button(80,155,148,"RESUME",!erl_choice,1);erl_button(244,155,148,erl_quick_race?"MAIN MENU":"WITHDRAW",erl_choice,1);
 if(erl_confirm){erl_chamfer(54,70,372,133,EP,ER);erl_text(70,87,GOLD,game.erl.event_kind?"WITHDRAW? THIS ROUND RECORDS A LOSS.":"WITHDRAW? THIS CONTRACT WILL FAIL.",1);erl_button(80,152,148,"YES",erl_confirm_yes,1);erl_button(244,152,148,"NO",!erl_confirm_yes,1);}
}
static void erl_pause_draw(void){erl_pause_draw_raw();if(game.erl.paused&&!game.erl.result&&race_is_active(&game))erl_motion_draw();}
static void erl_open_hq(void){
 erl_local_sync(&game);erl_season_offer(&game);
 erl_choice=0;erl_notice[0]=0;dialogue_source[0]=0;dialogue_page=0;
 erl_screen=!game.erl.referral?ERL_TALK:!game.erl.licence&&!(game.erl.inbox&8)?ERL_WELCOME:ERL_HOME;
 change_page(ERL_PAGE);
}
static void erl_return_hq(void){
 if(erl_quick_race){erl_quick_race_leave();return;}
 ErlCareer *e=&game.erl;int system=e->event_system,hub=e->event_hub,intro=e->event_kind==ERL_INTRO,away=e->event_kind==ERL_OFFICIAL&&e->event_system!=e->champ_system;e->event_kind=0;e->result=0;e->resume=e->paused=0;memset(&game.race,0,sizeof(game.race));game.system=system;game.station_variant=hub;game.docked=1;game.dock_stage=0;game.speed=game.boost=0;system_bodies(&game);game_spawn(&game);game.voice_time=game.message_time=0;selected_target=0;autoaim=square_held=0;erl_screen=ERL_HOME;erl_choice=0;erl_confirm=0;paused=0;change_page(away||(intro&&!e->referral)?HOME:ERL_PAGE);if(away)tracked_mission=TRACK_ERL;if(intro&&e->referral){erl_screen=ERL_TALK;dialogue_source[0]=0;dialogue_page=0;tracked_mission=TRACK_ERL;}if(!away&&e->licence&&tracked_mission==TRACK_ERL)tracked_mission=0;erl_ship_capture(&game,ship_paint[game.ship]);erl_commit();
}
static void erl_start_event(int kind,int layout,int discipline,int qualifier){
 erl_ship_capture(&game,ship_paint[game.ship]);if(!erl_begin(&game,kind,layout,discipline,qualifier)){snprintf(erl_notice,sizeof(erl_notice),"Finish other racing work or meet the entry requirements.");return;}
 game.erl.resume=0;game.erl.paused=0;game.erl.track=1;erl_choice=0;erl_result_serial=0;selected_target=RACE_TARGET_ID;tracked_mission=TRACK_ERL;change_page(HOME);message(&game,"Race accepted. Fly to the start beacon and contact it to get READY.");erl_commit();
}
static int erl_input_raw(unsigned pressed,unsigned held,float dt,float ax,float ay){
 /* A restored title session must not be pulled into an old accepted event. */
 if(page==INTRO)return 0;
 if(erl_quick_race&&erl_quick_race_release){if(!held)erl_quick_race_release=0;pressed=held=0;ax=ay=0;}
 ErlCareer *e=&game.erl;erl_prestart_recover();erl_rtap+=dt;
 if(!e->paused&&page==FLIGHT&&race_ready(&game)&&game.race.phase==RACE_COUNTDOWN&&game.race.countdown>4&&(pressed&PSP_CTRL_CROSS)){game.race.countdown=4;race_item_input(&game,1);return 1;}
 if(!e->event_kind&&race_ready(&game)&&game.race.phase==RACE_GRID){
  if(page==FLIGHT&&(pressed&PSP_CTRL_TRIANGLE)&&length(sub(game.pos,game.race.start))<5000){erl_board_brief=1;erl_screen=ERL_BRIEF;erl_choice=0;change_page(ERL_PAGE);return 1;}
  if(erl_board_brief&&page==ERL_PAGE){if(pressed&PSP_CTRL_CIRCLE){erl_board_brief=0;erl_notice[0]=0;change_page(FLIGHT);return 1;}if(pressed&PSP_CTRL_CROSS){const char *blocked=race_start_blocker(&game);if(blocked){snprintf(erl_notice,sizeof(erl_notice),"%s",blocked);return 1;}if(!race_ready(&game)||length(sub(game.pos,game.race.start))>=5000){erl_board_brief=0;change_page(FLIGHT);message(&game,"Contact the start beacon before READY.");return 1;}erl_board_brief=0;erl_notice[0]=0;race_restart(&game);race_item_input(&game,1);erl_commit();change_page(FLIGHT);return 1;}return 1;}
 }

 if(e->event_kind&&game.race.phase==RACE_GRID&&!e->result){
   if(page==FLIGHT&&race_ready(&game)&&(pressed&PSP_CTRL_TRIANGLE)&&length(sub(game.pos,game.race.start))<5000){erl_screen=ERL_BRIEF;erl_choice=0;change_page(ERL_PAGE);return 1;}
   if(page==ERL_PAGE&&erl_screen==ERL_BRIEF){
    if(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT))erl_choice=!erl_choice;
    if(pressed&PSP_CTRL_CIRCLE){change_page(game.docked?HOME:FLIGHT);return 1;}
    if(pressed&PSP_CTRL_CROSS){if(erl_choice){change_page(game.docked?HOME:FLIGHT);return 1;}
     if(game.docked||!race_ready(&game)||length(sub(game.pos,game.race.start))>=5000){selected_target=RACE_TARGET_ID;tracked_mission=TRACK_ERL;change_page(game.docked?HOME:FLIGHT);message(&game,"Race accepted. Fly to the start beacon and contact it to get READY.");return 1;}
     const char *blocked=race_start_blocker(&game);if(blocked){snprintf(erl_notice,sizeof(erl_notice),"%s",blocked);return 1;}
     erl_notice[0]=0;e->real_hull=game.hull;e->real_shield=game.energy;e->real_heat=game.heat;e->real_fuel=game.fuel;e->real_damaged=game.damaged;race_restart(&game);race_item_input(&game,1);change_page(FLIGHT);erl_commit();return 1;}
    return 1;
   }
 }
 /* Pending contracts do not own headquarters navigation. Only a started
  * race/result may intercept ordinary menu input. */
 if((e->event_kind&&(game.race.phase!=RACE_GRID||e->result))||(!e->event_kind&&race_is_active(&game))){
  if(e->result){
   if(game.race.layout==5&&game.race.crash_time>0){race_crash_present_tick(&game,dt);return 1;}
   if(erl_result_serial!=e->serial){erl_result_serial=e->serial;erl_reveal=0;erl_choice=0;erl_commit();}
   /* Keep the recording for LocalTV highlights, but never delay race results
    * with an automatic replay. The finish flyby starts immediately. */
   erl_replay.active=0;erl_replay.played=e->serial;
   float previous_reveal=erl_reveal;int winning=e->result_place==1&&e->result==1;
   int finished=e->result==1;erl_reveal=finished?erl_reveal+dt:fminf(7.2f,erl_reveal+dt);float ready=finished?SW_READY:1.2f;
   if(finished){const float at[]={0,.25f,2.14f,2.38f,2.71f,5.46f,6.71f};const int cues[]={SFX_SPEED_SURGE,SFX_BOOST,SFX_SCAN,SFX_SCAN,SFX_HIT,SFX_OBSERVATORY,SFX_SCAN};for(int i=0;i<7;i++)if((previous_reveal<=at[i]&&erl_reveal>at[i]))game.cue=cues[i];}
   if(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT))erl_choice=!erl_choice;
   if(pressed&PSP_CTRL_CROSS){if(erl_reveal<ready){erl_reveal=finished?9.8f:7.2f;return 1;}if(erl_quick_race){if(!erl_choice)erl_quick_race_start();else erl_quick_race_leave();return 1;}int retry=!erl_choice&&e->event_kind!=ERL_OFFICIAL&&e->result_place!=1,kind=e->event_kind,layout=e->event_layout,type=e->event_type,q=e->qualifier,standings=!erl_choice;erl_return_hq();if(retry)erl_start_event(kind,layout,type,q);else if(kind==ERL_OFFICIAL&&standings){erl_screen=ERL_STANDINGS;}}
   else if((pressed&PSP_CTRL_CIRCLE)&&erl_reveal>=ready)erl_return_hq();return 1;
  }
  if(page!=FLIGHT)change_page(FLIGHT);
  if(pressed&PSP_CTRL_START){if(e->paused){e->paused=0;game.race.handover=0;game.race.protection=3;}else{e->paused=1;erl_choice=0;erl_confirm=0;erl_commit();}return 1;}
  if(e->paused){
   if(erl_quick_race&&erl_choice&&(pressed&PSP_CTRL_CROSS)){erl_quick_race_leave();return 1;}
   if(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT|PSP_CTRL_UP|PSP_CTRL_DOWN)){if(erl_confirm)erl_confirm_yes=!erl_confirm_yes;else erl_choice=!erl_choice;}
   if(erl_screen==ERL_EVENTS&&erl_choice&&pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER)){game.erl_local.selection=(game.erl_local.selection+180+(pressed&PSP_CTRL_RTRIGGER?1:-1))%180;game.cue=SFX_SELECT;return 1;}
 if(pressed&PSP_CTRL_CIRCLE){if(erl_confirm)erl_confirm=0;else {e->paused=0;game.race.handover=0;game.race.protection=3;}return 1;}
   if(pressed&PSP_CTRL_CROSS){if(erl_confirm){if(erl_confirm_yes){if(e->event_kind)erl_withdraw(&game);else {race_withdraw(&game);e->paused=e->resume=0;erl_commit();}erl_result_serial=0;}erl_confirm=0;}else if(erl_choice){erl_confirm=1;erl_confirm_yes=0;}else {e->paused=0;game.race.handover=0;game.race.protection=3;}}
   return 1;
  }
  if(pressed&PSP_CTRL_TRIANGLE){third_person=third_person?0:1;flight_camera_seen=-1;}
  /* Square is a tap-to-drift command. The tap arms a short controlled slide;
   * steering still shapes it, as in arcade drift racers. */
  if(erl_drift_serial!=game.erl.serial||game.race.phase!=RACE_RUNNING){erl_drift_serial=game.erl.serial;erl_drift_time=0;erl_drift_direction=0;}
  if(game.race.layout==5&&game.race.crash_time>0){erl_drift_time=0;game.race.drift_input=0;}
  if((pressed&PSP_CTRL_SQUARE)&&game.race.crash_time<=0){float steer=ax;if(held&PSP_CTRL_LEFT)steer=-1;if(held&PSP_CTRL_RIGHT)steer=1;
   if(fabsf(steer)<.15f&&game.race.layout==5){RaceTrackFrame now=race_track_nearest(&game.race,game.race.gate,game.pos);float t=now.t+.22f;int section=now.section;if(t>1){section++;t-=1;}RaceTrackFrame ahead=race_track_frame(&game.race,section,t);steer=dot(ahead.ahead,now.right);}
   erl_drift_direction=steer<0?-1:1;erl_drift_time=.82f;
  }
  erl_drift_time=fmaxf(0,erl_drift_time-dt);game.race.drift_input=erl_drift_time>0?erl_drift_direction:0;
  selected_target=RACE_TARGET_ID;autoaim=square_held=0;game.shot=0;
  /* ERL has a dedicated racing layout: X drives, L brakes, R is an
   * immediate held boost, Square drifts and Circle uses the current item. */
  int braking=!!(held&PSP_CTRL_LTRIGGER),boosting=!!(held&PSP_CTRL_RTRIGGER);
  game.race.manual_input=boosting;
  if(erl_rear_serial!=game.erl.serial){erl_rear_serial=game.erl.serial;erl_rear_latch=0;}
  if(!(held&PSP_CTRL_DOWN))erl_rear_latch=0;
  else if(game.race.layout==5&&game.race.arcade.actor[0].item==RA_MISSILE)erl_rear_latch=1;
  if(erl_rear_latch&&!game.race.rear_input)erl_vertical_speed=0;
  game.race.rear_input=erl_rear_latch;
  int throttle=braking?-1:((held&PSP_CTRL_CROSS)||boosting?1:0);
  if(braking)game.boost=0;
  else if(boosting&&!game.race.boost_lock)game.boost=1;
  else if(!boosting){game.boost=0;game.race.boost_lock=0;}
  float turn=ax,pitch=fmaxf(0,-ay);if(held&PSP_CTRL_LEFT)turn=-1;if(held&PSP_CTRL_RIGHT)turn=1;if(held&PSP_CTRL_UP)pitch=1;if(held&PSP_CTRL_DOWN)pitch=0;
  if(game.race.layout==5){
   /* Up/Down are track-local lift controls. They translate the craft while
    * its nose and chase camera stay aimed down-course. Normal pink road and
    * the jump permit lift; magnetic road still holds the hull to its skin. */
   RaceTrackFrame vf=race_track_nearest(&game.race,game.race.gate,game.pos);int surface=race_track_surface(&game.race,vf.section,vf.t);float vertical=fmaxf(-1,fminf(1,-ay));
   if(held&PSP_CTRL_UP)vertical=1;if((held&PSP_CTRL_DOWN)&&!erl_rear_latch)vertical=-1;if(surface==1)vertical=0;
   if(erl_vertical_serial!=game.erl.serial||game.race.phase!=RACE_RUNNING){erl_vertical_serial=game.erl.serial;erl_vertical_speed=0;}
   float target=vertical*game.race.top_speed*.26f,step=game.race.top_speed*1.8f*dt,difference=target-erl_vertical_speed;
   erl_vertical_speed+=fmaxf(-step,fminf(step,difference));
   float movement=erl_vertical_speed*dt;
   if(surface==0){float height=dot(sub(game.pos,vf.centre),vf.up),floor=RACE_TRACK_FLOOR+race_floor_support(&game,0,vf)+35;
    if(movement<0)movement=fmaxf(movement,floor-height);
    float scale=race_neon_scale(&game.race),edge=190.f*2.5f*scale,body=race_neon_body_radius(&game),lane=dot(sub(game.pos,vf.centre),vf.right),tunnel=fmaxf(1,edge-body);
    float roof=RACE_TRACK_FLOOR+sqrtf(fmaxf(0,tunnel*tunnel-lane*lane));if(movement>0)movement=fminf(movement,roof-height);
   }
   if(surface==1||game.race.crash_time>0||game.race.repair>0)erl_vertical_speed=movement=0;game.pos=add(game.pos,mul(vf.up,movement));pitch=0;
  }
  int board=!e->event_kind;game_tick(&game,dt,turn,pitch,throttle,!!(held&PSP_CTRL_CIRCLE));erl_replay_capture(dt);if(e->result||(board&&!race_is_active(&game)))erl_commit();return 1;
 }
 if(page==HOME&&row==31&&erl_hq(&game)&&(pressed&PSP_CTRL_CROSS)){erl_ship_capture(&game,ship_paint[game.ship]);erl_open_hq();return 1;}
 if(page==MISSIONS&&game.system==7&&row==0&&(pressed&PSP_CTRL_CROSS)){erl_screen=ERL_TALK;erl_choice=0;erl_notice[0]=0;game.erl.track=1;change_page(ERL_PAGE);return 1;}
 if(page==CAMPAIGN&&tracked_mission==TRACK_ERL){
  if(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT|PSP_CTRL_UP|PSP_CTRL_DOWN))row=!row;
  if(pressed&PSP_CTRL_CIRCLE){change_page(MISSIONLOG);return 1;}
  if(pressed&PSP_CTRL_CROSS){if(row){change_page(MISSIONLOG);return 1;}int dest=erl_destination();if(game.system==dest){if(game.docked){if(!e->referral&&game.system==7){erl_screen=ERL_MECHANIC;erl_choice=0;change_page(ERL_PAGE);}else if(erl_hq(&game)){erl_open_hq();}else{selected_target=0;change_page(HOME);}}else{selected_target=(e->championship&&e->champ_hub?ROUTE_TARGET_ID+e->champ_hub:0);scan_cat=0;change_page(FLIGHT);}}else{route_set_goal(&game,dest);int hops=0,hop=route_next_hop(&game,dest,&hops);if(hop>=0){game.destination=hop;change_page(CHART);}else message(&game,"Refuel before plotting the racing route.");}}
  return 1;
 }
 if(page!=ERL_PAGE)return 0;
 if(pressed&PSP_CTRL_TRIANGLE){erl_racer_open(0);return 1;}
 if((erl_screen==ERL_TALK||erl_screen==ERL_RIVAL_TALK||erl_screen==ERL_WELCOME)&&dialogue_pages>1&&(pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER))){if((pressed&PSP_CTRL_LTRIGGER)&&dialogue_page>0)dialogue_page--;if((pressed&PSP_CTRL_RTRIGGER)&&dialogue_page+1<dialogue_pages)dialogue_page++;game.cue=SFX_SELECT;return 1;}
 if(erl_screen==ERL_EVENTS&&erl_choice&&pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER)){game.erl_local.selection=(game.erl_local.selection+180+(pressed&PSP_CTRL_RTRIGGER?1:-1))%180;game.cue=SFX_SELECT;return 1;}
 if(pressed&PSP_CTRL_CIRCLE){if(erl_confirm)erl_confirm=0;else if(!erl_hq(&game)||erl_screen==ERL_WELCOME){change_page(HOME);}else if(erl_screen==ERL_RIVAL_TALK){erl_screen=ERL_RIVALS;erl_choice=erl_rival_id-1;}else if(erl_screen!=ERL_HOME){erl_screen=ERL_HOME;erl_choice=0;}else change_page(HOME);erl_notice[0]=0;return 1;}
 if(erl_confirm){if(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT))erl_confirm_yes=!erl_confirm_yes;if(pressed&PSP_CTRL_CROSS){if(erl_confirm_yes){e->championship=0;e->dirty=1;erl_commit();}erl_confirm=0;}return 1;}
 if(pressed&(PSP_CTRL_LEFT|PSP_CTRL_UP|PSP_CTRL_RIGHT|PSP_CTRL_DOWN)){
  int d=pressed&(PSP_CTRL_LEFT|PSP_CTRL_UP)?-1:1;if(erl_screen==ERL_RIVALS&&(pressed&(PSP_CTRL_UP|PSP_CTRL_DOWN)))d*=3;
  if(erl_screen==ERL_GARAGE)erl_garage_row=(erl_garage_row+e->fleet_n+d)%e->fleet_n;
  else{int n=erl_screen==ERL_HOME?(e->championship?6:5):erl_screen==ERL_EVENTS?3:erl_screen==ERL_RIVALS?6:erl_screen==ERL_SEASON||erl_screen==ERL_BRIEF||erl_screen==ERL_MECHANIC||erl_screen==ERL_TALK?2:1;erl_choice=(erl_choice+n+d)%n;}erl_anim=0;erl_notice[0]=0;game.cue=SFX_SELECT;
 }
 if(pressed&PSP_CTRL_SQUARE){if(e->module_pending||game.erl_local.pending_count){Game *prior=malloc(sizeof(Game));if(!prior){snprintf(erl_notice,sizeof(erl_notice),"Cannot allocate safe prize transaction.");return 1;}*prior=game;if(erl_claim_module(&game,module_store_capacity())){if(erl_commit())snprintf(erl_notice,sizeof(erl_notice),"Exact prize delivered to Item Storage.");else {game=*prior;snprintf(erl_notice,sizeof(erl_notice),"Save failed. Prize remains unclaimed.");}}else snprintf(erl_notice,sizeof(erl_notice),"Make room in Item Storage, then claim here.");free(prior);}else if(erl_screen==ERL_HOME&&e->licence){erl_start_event(ERL_PRACTICE,erl_layout(),erl_discipline(erl_layout()),0);}return 1;}
 if(!(pressed&PSP_CTRL_CROSS))return 1;
 if(erl_screen==ERL_WELCOME){if(dialogue_page+1<dialogue_pages){dialogue_page++;return 1;}e->inbox|=8;e->dirty=1;erl_commit();erl_screen=ERL_HOME;erl_choice=0;return 1;}
 if(erl_screen==ERL_EVENTS){if(erl_choice==0&&!erl_next_qualifier(&game))return 1;erl_requested_kind=erl_choice==0?ERL_QUALIFIER:erl_choice==1?ERL_PRACTICE:ERL_TRIAL;erl_requested_layout=erl_choice==2?2:0;erl_requested_type=erl_choice==2?ERL_TARGET:ERL_CIRCUIT;erl_screen=ERL_BRIEF;erl_choice=0;}
 else if(erl_screen==ERL_TALK){if(!e->referral&&game.system!=7&&!erl_choice){tracked_mission=TRACK_ERL;e->track=1;change_page(MISSIONLOG);row=mission_track_row();return 1;}if(game.system==7&&!erl_hq(&game)&&e->referral){if(!erl_choice){tracked_mission=TRACK_ERL;if(!e->licence&&e->ref_system>=0){route_set_goal(&game,e->ref_system);int hops=0,hop=route_next_hop(&game,e->ref_system,&hops);if(hop>=0)game.destination=hop;change_page(CHART);}else{change_page(MISSIONLOG);row=mission_track_row();}}else{erl_screen=ERL_HOME;erl_choice=0;change_page(HOME);}return 1;}if(erl_choice){erl_screen=ERL_HOME;erl_choice=0;}else {erl_requested_kind=ERL_QUALIFIER;erl_requested_layout=0;erl_requested_type=ERL_CIRCUIT;erl_screen=!e->referral?ERL_MECHANIC:ERL_BRIEF;}}
 else if(erl_screen==ERL_HOME){if(erl_choice==5){erl_requested_kind=ERL_OFFICIAL;erl_requested_layout=e->round;erl_screen=ERL_BRIEF;erl_choice=0;}else if(erl_choice==0){erl_requested_kind=0;erl_screen=!e->licence?ERL_BRIEF:ERL_EVENTS;erl_choice=0;if(!e->referral&&game.system!=7)snprintf(erl_notice,sizeof(erl_notice),"Find TORR BELL on Lave Hub's Mission Board.");}else {erl_screen=erl_choice==1?ERL_SEASON:erl_choice==2?ERL_DRIVER:erl_choice==3?ERL_RIVALS:ERL_GARAGE;erl_choice=0;}}
 else if(erl_screen==ERL_GARAGE){if(erl_swap(&game,erl_garage_row,ship_paint[game.ship],erl_notice,sizeof(erl_notice))){ship_paint[game.ship]=e->fleet[e->active_ship].paint;erl_commit();}}
 else if(erl_screen==ERL_SEASON){if(erl_choice&&e->championship){erl_confirm=1;erl_confirm_yes=0;}else if(!erl_choice){if(e->championship)erl_screen=ERL_STANDINGS;else if(erl_season_start(&game)){erl_commit();erl_requested_kind=ERL_OFFICIAL;erl_screen=ERL_BRIEF;}else snprintf(erl_notice,sizeof(erl_notice),"Win your qualifier before entering a season.");erl_choice=0;}}
 else if(erl_screen==ERL_STANDINGS){erl_requested_kind=e->championship?ERL_OFFICIAL:0;erl_screen=e->championship?ERL_BRIEF:ERL_HOME;erl_choice=0;}
 else if(erl_screen==ERL_RIVALS){erl_rival_id=erl_choice+1;erl_screen=ERL_RIVAL_TALK;erl_choice=0;}
 else if(erl_screen==ERL_RIVAL_TALK){erl_screen=ERL_RIVALS;erl_choice=erl_rival_id-1;}
 else if(erl_screen==ERL_DRIVER){if(erl_next_qualifier(&game)){erl_requested_kind=ERL_QUALIFIER;erl_requested_layout=0;erl_requested_type=ERL_CIRCUIT;erl_screen=ERL_BRIEF;erl_choice=0;}}
 else if(erl_screen==ERL_BRIEF||erl_screen==ERL_MECHANIC){if(erl_choice){erl_screen=ERL_HOME;erl_choice=0;}else {int kind=erl_screen==ERL_MECHANIC?ERL_INTRO:erl_requested_kind?erl_requested_kind:ERL_QUALIFIER;int layout=kind==ERL_OFFICIAL?e->round:erl_requested_layout,type=kind==ERL_INTRO?ERL_SPRINT:kind==ERL_OFFICIAL?erl_discipline(layout):erl_requested_type;
 if(kind==ERL_PRACTICE||kind==ERL_TRIAL){game.erl_local.event_rules=erl_catalogue_rules(station_profile_hash(game.system,813),e->licence,game.erl_local.selection);game.erl_local.rules_pending=1;type=game.erl_local.event_rules.mode==RA_COMBAT?ERL_RUSH:ERL_CIRCUIT;}erl_start_event(kind,layout,type,kind==ERL_QUALIFIER?erl_next_qualifier(&game):0);}}
 return 1;
}
static int erl_input(unsigned pressed,unsigned held,float dt,float ax,float ay){
 erl_motion_sync();int handled;
 /* Account for a slow displayed frame without sending a large step through
  * steering, jump guidance or swept weapon/contact physics. Edge presses run
  * once; pause/resume/menu transitions never receive catch-up input. */
 if(page==FLIGHT&&game.race.layout==5&&race_is_active(&game)&&!game.erl.paused&&!paused&&!(pressed&PSP_CTRL_START)&&dt>.034f){
  int steps=(int)ceilf(dt*30);if(steps>6)steps=6;float step=dt/steps;
  handled=erl_input_raw(pressed,held,step,ax,ay);
  for(int i=1;i<steps&&handled&&page==FLIGHT&&!game.erl.paused&&!game.erl.result&&race_is_active(&game);i++)erl_input_raw(0,held,step,ax,ay);
 }else handled=erl_input_raw(pressed,held,dt,ax,ay);
 erl_motion_sync();
 if(handled&&(pressed&PSP_CTRL_CROSS)&&erl_motion_view()>=0){erl_motion.pulse=0;if(!game.cue)game.cue=SFX_UI;}return handled;
}
static void erl_ui_tick(float dt){
 erl_prestart_recover();
 erl_motion_tick(dt);
 erl_anim+=dt;ErlCareer *e=&game.erl;
 if(erl_quick_race||page==INTRO)return;
 if(erl_checkpoint_collect(0)<0){e->dirty=1;snprintf(erl_notice,sizeof(erl_notice),"CHECKPOINT FAILED / PAUSE TO RETRY");}
 /* Board completion has no league result screen to trigger its commit.
  * Keep retrying after an older immutable checkpoint finishes. */
 if(!e->event_kind&&game.race.arcade.ready&&game.race.arcade.rules.context==2&&game.race.phase>=RACE_LOST&&game.race.job>=0){
  if(e->dirty&&opening_save_allowed(&game)&&erl_checkpoint_queue())e->dirty=0;
  return;
 }
 if((!e->event_kind&&!race_is_active(&game))||e->result||e->paused)return;
 e->save_age+=dt;if(e->save_age>=5&&opening_save_allowed(&game)){e->save_age=0;if(erl_checkpoint_queue())e->dirty=0;else e->dirty=1;}
}
