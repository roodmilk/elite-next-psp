#include "generated/planet-site-pixels.h"
#include "planet-site-dialogue.h"
/* Planetary site overlay: no second world simulation, no duplicated reward path.
 * Completion remains in surface_progress, preserving existing saves. */
static int ps_open=0,ps_site=0,ps_row=0,ps_reply=0,ps_ready=0,ps_release=0;
static int ps_system=-1,ps_body=-1,ps_inspected=0;
static unsigned ps_seed;
static float ps_anim=0;
static const char *ps_objects[12][3]={
 {"SEALED CRATE","INVENTORY","EXPEDITION NOTES"},
 {"INSCRIPTIONS","SURVEY TABLE","EXCAVATION NOTES"},
 {"TELESCOPE","SIGNAL CONSOLE","OBSERVATION LOG"},
 {"TRANSMITTER","SUPPLY PACK","DISTRESS LOG"},
 {"SEED BEDS","IRRIGATION","GROWER'S NOTES"},
 {"EXPOSED FOSSIL","SAMPLE BENCH","FIELD JOURNAL"},
 {"REGULATOR","PRESSURE GAUGE","SERVICE RECORD"},
 {"DRONE HULL","RECORDER","FLIGHT RECORD"},
 {"LOOKOUT","TRACKING DESK","MIGRATION LOG"},
 {"CRYSTAL FACES","SURVEY RIG","PROSPECTOR LOG"},
 {"RECORD TERMINAL","TAPE CABINET","SETTLER JOURNAL"},
 {"SENSOR MAST","FORECAST DESK","WEATHER LOG"}
};
static const char *ps_keepers[]={"QUARTERMASTER","ARCHAEOLOGIST","OBSERVER","SURVIVOR","BOTANIST","RESEARCHER","TECHNICIAN","DRONE OPERATOR","RANGER","PROSPECTOR","ARCHIVIST","METEOROLOGIST"};
static const char *ps_observations[]={
 "The crate has survived better than the shelter around it. Its seal lists mineral samples rather than food. You check the transfer label before opening anything; your ship will need a free tonne of cargo space to take the contents.",
 "The marks repeat at different heights along the stones. They were not carved as decoration: someone was recording a cycle. Your scanner can preserve their arrangement without disturbing the original surface.",
 "The telescope still moves, but its receiver has drifted away from the reference signal. A handwritten correction sits beside the controls. The instruments need careful calibration before their readings can be trusted.",
 "The beacon is transmitting into a damaged antenna. The distress recording keeps restarting before it reaches the location code. Restoring the link would let the spaceport organise a recovery flight.",
 "Each tray has a different label and a carefully measured water supply. The missing information is not in the greenhouse: the growers need records of at least two local species to compare with their cultivated specimens.",
 "A pale outline runs through the darker stone. It is easy to mistake a fracture for a limb until you follow the pattern into the next layer. A recorded survey will preserve the find without lifting it from its bed.",
 "The pressure needle rises slowly, then drops with a shudder. This installation supplies distant shelters. The regulator can be reset locally, but first you check the service markings to identify the correct release sequence.",
 "The drone's casing is buckled around its recorder. Its last route may still be intact. Recovering the survey data is more useful than hauling away a shell that will never fly again.",
 "The tracking desk contains long gaps between sightings. A useful report needs an actual local animal record, not a guess based on distant movement. Your discovery log can supply the missing evidence.",
 "Several crystal faces reflect the same light in different colours. The nearby survey rig measures their structure without breaking them. Recording the formation leaves it available for the next expedition.",
 "The terminal indexes voices rather than cargo or accounts. Some recordings have lost their dates, but the original tapes remain in order. Restoring that index will preserve a small part of the settlement's history.",
 "The array's pressure baseline no longer agrees with its reference instrument. Local flight control needs a corrected reading. You can reset the baseline here and transmit a new measurement without replacing the whole installation."
};
static int ps_kind(void){return ps_site<=1?FIELD_WEATHER:field_site_kind(&game,ps_system,ps_body,ps_site);}
static const char *ps_person(void){static const char *names[]={"IONA SENN","TAVI ORREN","MARA VEY","ELI KEST"};if(ps_site>1&&ps_kind()==FIELD_DISH)return names[ps_seed%4];return ps_site==0?"PORT OFFICER":ps_site==1?"FIELD TECHNICIAN":ps_keepers[ps_kind()];}
static const char *ps_object(int index){
 static const char *port[]={"SERVICE DESK","SUIT CHARGER","CONTRACT RECORD"};
 static const char *contract[3][3]={{"POWER RELAY","REGULATOR","SERVICE RECORD"},{"BIOLOGY ARRAY","SAMPLE DESK","SPECIES RECORD"},{"ARCHIVE UPLINK","TRANSMITTER","ARCHIVE RECORD"}};
 if(ps_site==0)return port[index];
 if(ps_site==1)return contract[field_profile(&game,ps_system,ps_body).job][index];
 return ps_objects[ps_kind()][index];
}
static int ps_done(void){unsigned bit=ps_site==0?64u:field_site_bit(ps_site);return (game.surface_progress[ps_system][ps_body]&bit)!=0;}
static int ps_staffed(void){return ps_site<=1||ps_kind()==FIELD_DISH||(ps_kind()==FIELD_RESCUE?!ps_done():ps_seed%3!=0);}
 #include "observatory-scene.h"
static void ps_intro(void){
 if(ps_is_observatory()){ps_obs_intro();return;}
 char body[1024];FieldProfile p=field_profile(&game,ps_system,ps_body);
 snprintf(body,sizeof(body),"%s. %s. %s %s",field_culture(p.culture),field_weather(p.weather),
  ps_staffed()?"Someone is working here. You can inspect the equipment before speaking to them.":"No one is here to greet you. The equipment and field records may explain what this place was used for.",
  ps_done()?"Your previous work is already recorded; there is no unclaimed payment waiting here.":surface_site_brief(&game,ps_system,ps_body,ps_site));
 sc_read_set(surface_site_name(&game,ps_system,ps_body,ps_site),body);
}
static void ps_begin(int id){
 if(id<0||id>9||id==6||game.planet<1||game.surface!=2||game.rover_driving||game.dead)return;
 ps_site=id;ps_system=game.system;ps_body=game.planet;ps_seed=field_hash(field_seed(ps_system,ps_body)+id*7159u);
 ps_open=1;ps_ready=0;ps_anim=0;ps_obs_pending=0;ps_row=ps_reply=ps_inspected=0;game.eva_running=game.eva_run_arm=0;game.eva_run_hold=0;game.speed=0;
 sc_menu=SC_MENU_NONE;ps_intro();game.cue=ps_is_observatory()?SFX_OBSERVATORY:SFX_UI;
}
static void ps_action(void){
 if(ps_is_observatory()){ps_obs_decide();return;}
 if(!ps_inspected&&ps_site>1){sc_read_set("CHECK THE EQUIPMENT FIRST","Before you commit to the work, inspect the main feature. That will tell you what is required and whether you have the right records or cargo space.");return;}
 /* The overlay never moves the player. Reject stale contexts instead of
  * completing a different nearby site after a load or transition. */
 if(game.system!=ps_system||game.planet!=ps_body||surface_nearest_site(&game,55)!=ps_site){ps_open=0;ps_release=1;return;}
 surface_interact(&game);sc_read_set("SITE REPORT",game.message);ps_reply=0;ps_row=0;
}
static void ps_choose(void){
 if(ps_is_observatory()){ps_obs_choose();return;}
 int k=ps_kind();
 if(ps_reply){
  if(ps_row==0){sc_read_pair(ps_person(),ps_site<=1?surface_site_brief(&game,ps_system,ps_body,ps_site):ps_observations[k],ps_done()?"The report has already been accepted. Thank you for taking the time to come back.":"Take a look around before you decide. I would rather have a careful report than a quick one.");ps_inspected=1;}
  else if(ps_row==1)sc_read_set(ps_person(),ps_site<=1?"The port and these field installations depend on each other. Reports come back here, supplies go out, and somebody has to notice when a routine job stops being routine. You can help with the local contract, but there is no obligation to accept it just because you came in to talk.":ps_personal[k]);
  else if(ps_row==2)ps_action();
  else {ps_reply=0;ps_row=0;sc_read_set("CONVERSATION ENDED","You step away from the conversation. The rest of the site is still available to inspect.");}
  return;
 }
 if(ps_row==0){ps_inspected=1;sc_read_set(ps_site<=1?"LOCAL CONTRACT":ps_object(0),ps_site<=1?surface_site_brief(&game,ps_system,ps_body,ps_site):ps_observations[k]);}
 else if(ps_row==1)sc_read_pair(ps_object(1),ps_site<=1?surface_site_brief(&game,ps_system,ps_body,ps_site):ps_secondary[k],ps_done()?"The completed work is already recorded here. There is no second reward to collect.":"You can inspect freely before deciding whether to carry out the work.");
 else if(ps_row==2){sc_read_pair(ps_object(2),surface_site_brief(&game,ps_system,ps_body,ps_site),ps_done()?"The final entry confirms that this objective has been completed.":"The final entry is still waiting for an expedition report.");}
 else if(ps_row==3){if(ps_staffed()){ps_reply=1;ps_row=0;sc_read_set(ps_person(),ps_done()?"I recognise your report. There is no need to do the same work twice, but you are welcome to look around.":"Hello. I wasn't expecting company out here. If you have a moment, I can explain what we are working on. You don't have to commit to anything just to ask.");}else sc_read_set("EMPTY WORKSTATION","An empty chair faces the equipment. No voice answers you. You can still inspect the records and carry out any work that does not require someone to be present.");}
 else ps_action();
}
static void ps_input(unsigned pressed,unsigned held){
 if(!ps_ready){if(!held)ps_ready=1;return;}
 if(pressed&PSP_CTRL_CIRCLE){if(ps_reply){ps_reply=0;ps_row=0;ps_intro();}else{ps_open=0;ps_release=1;}return;}
 int count=ps_is_observatory()?ps_obs_count():ps_reply?4:5;
 if(pressed&PSP_CTRL_UP)ps_row=(ps_row+count-1)%count;
 if(pressed&PSP_CTRL_DOWN)ps_row=(ps_row+1)%count;
 if(pressed&PSP_CTRL_LEFT&&sc_read_page>0)sc_read_page--;
 if(pressed&PSP_CTRL_RIGHT&&sc_read_page+1<sc_read_pages())sc_read_page++;
 if(pressed&PSP_CTRL_CROSS)ps_choose();
}
static void ps_draw(void){
 int k=ps_kind();
 unsigned tint=game.bodies[ps_body].accent,ground=game.bodies[ps_body].color;
 rect(0,0,W,H,SC_CHAR);text(1,1,SC_CREAM,"%.36s",surface_site_name(&game,ps_system,ps_body,ps_site));
 char cash[24];snprintf(cash,sizeof(cash),"%.1f U",game.credits*.1f);text_px(W-8-(int)strlen(cash)*8,8,SC_AMBER,cash);
 /* Native pixel plates and interaction anchors share one design source. */
 int plate=ps_site==0?12:ps_site==1?13+field_profile(&game,ps_system,ps_body).job:k;
 unsigned pal[24];float hour=field_local_hour(&game,ps_system,ps_body);int night=hour<6||hour>19;
 for(int i=0;i<24;i++){
  pal[i]=ps_palette[i];
  if(i>=12&&i<=15)pal[i]=mix_rgb(pal[i],ground,.32f);
  if(i==16||i==17)pal[i]=mix_rgb(pal[i],night?SC_VOID:tint,night?.65f:.25f);
  if(i>=2&&i<=4)pal[i]=mix_rgb(pal[i],tint,.12f);
 }
 for(int y=0;y<168;y++)for(int x=0;x<340;x++)fb[(y+22)*STRIDE+x]=pal[ps_plates[plate][y*340+x]];
 if(ps_is_observatory()){ps_obs_window(pal);ps_obs_instruments();}
 if(ps_staffed())sc_raster_sprite(lave_crew_pixels[ps_seed%8],ps_anchor[3][0],22+ps_anchor[3][1],32,64);
 /* Small console lights animate using UI time, never advancing outdoor time. */
 rect(236+(int)(ps_anim*3)%4*4,130,2,3,ps_done()?SC_OLIVE:SC_CYAN);
 if(!ps_reply){
  int a=ps_row<4?ps_row:0,x=ps_anchor[a][0],y=22+ps_anchor[a][1],w=ps_anchor[a][2],h=ps_anchor[a][3];
  unsigned glow=mix_rgb(SC_AMBER,SC_CREAM,.3f+.2f*sinf(ps_anim*4));
  station_shell_frame(x-2,y-2,w+4,h+4,1);
  rect(x-1,y-1,w+2,1,glow);rect(x-1,y+h,w+2,1,glow);
  rect(x-1,y,1,h,glow);rect(x+w,y,1,h,glow);
 }
 rect(340,22,140,168,SC_VOID);
 for(int i=0;i<(ps_is_observatory()?ps_obs_count():ps_reply?4:5);i++){
  int y=38+i*27;const char *label;
  if(ps_is_observatory())label=ps_obs_label(i);
  else if(ps_reply)label=i==0?"WHAT'S NEEDED?":i==1?"LIFE OUT HERE?":i==2?"I'LL DO IT":"GOODBYE";
  else label=i<3?ps_object(i):i==3?(ps_staffed()?ps_person():"EMPTY CHAIR"):ps_site==0?"PORT SERVICE":"COMPLETE WORK";
  if(i==ps_row)rect(342,y-3,136,19,ps_reply?SC_AMBER:SC_SLATE);
  text_px(346,y,i==ps_row&&ps_reply?SC_VOID:SC_CREAM,"%.16s",label);
 }
 sc_menu=SC_MENU_NONE;sc_draw_text_box();
 /* No station-only ship shortcut: exit must return to the surface. */
 rect(0,256,W,16,SC_VOID);button_icon(8,259,'U',SC_CREAM);button_icon(20,259,'D',SC_CREAM);text_px(36,260,SC_LAV,"SELECT");
 button_icon(100,259,'X',SC_CREAM);text_px(116,260,SC_LAV,"CHOOSE");button_icon(180,259,'L',SC_CREAM);button_icon(192,259,'R',SC_CREAM);text_px(208,260,SC_LAV,"READ");button_icon(280,259,'O',SC_CREAM);text_px(296,260,SC_LAV,ps_reply?"BACK":"OUTSIDE");
}
