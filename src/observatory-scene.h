/* Sky Observatory: three durable observations, deliberate irreversible choice.
 * Reply states: 1 conversation, 2 decision, 3 confirmation. */
static int ps_obs_pending=0;
static int ps_is_observatory(void){return ps_site>1&&ps_kind()==FIELD_DISH;}
static uint32_t *ps_obs_flags(void){return &game.surface_progress[ps_system][ps_body];}
static void ps_obs_intro(void){
 if(ps_done()){sc_read_pair(ps_person(),observatory_report(observatory_choice(&game,ps_system,ps_body)),"The instruments are running. You are welcome to look around; the completed survey will not pay again.");return;}
 sc_read_set("THE SIGNAL BETWEEN", "A telescope turns slowly beneath the roof aperture. The observer is listening to a receiver that keeps repeating the same uneven pulse. A clean calibration tone lies underneath it. Someone has put a mug beside the controls and forgotten it there. You can examine the telescope, the signal console and the observing log before deciding whether to help.");
}
static void ps_obs_decide(void){
 if(ps_done()){ps_reply=0;sc_read_set("OBSERVATORY REPORT",observatory_report(observatory_choice(&game,ps_system,ps_body)));return;}
 if((*ps_obs_flags()&OBS_CLUES)!=OBS_CLUES){
  sc_read_set(ps_person(),"We shouldn't reset anything on a hunch. Check the telescope to rule out a mechanical fault, compare the two channels at the console, and read the observing log. Then we can choose what to do without pretending we know more than we do.");return;
 }
 ps_reply=2;ps_row=0;sc_read_set(ps_person(),"The beacon reset will clear the interference, but it will also erase the receiver's volatile trace. The port pays a 20-unit service bonus for that reset. Or we can record the trace and recalibrate without the reset: no service bonus, but an extra discovery for your Codex. Either way, the ordinary survey payment is yours. I can't tell you the trace is alien. I can only tell you it is worth a careful decision.");
}
static int ps_obs_count(void){return ps_reply==2?3:ps_reply==3?2:ps_reply?4:5;}
static const char *ps_obs_label(int row){
 if(ps_reply==2)return row==0?"RESTORE BEACON":row==1?"PRESERVE TRACE":"NOT YET";
 if(ps_reply==3)return row==0?"CONFIRM CHOICE":"GO BACK";
 if(ps_reply)return row==0?"WHAT HAPPENED?":row==1?"LIFE OUT HERE?":row==2?"LET'S DECIDE":"GOODBYE";
 return row<3?ps_object(row):row==3?ps_person():ps_done()?"VIEW OUTCOME":"RESOLVE SIGNAL";
}
static void ps_obs_choose(void){
 if(ps_reply==3){
  if(ps_row==1){ps_reply=2;ps_row=0;ps_obs_decide();return;}
  if(game.system!=ps_system||game.planet!=ps_body||!observatory_resolve(&game,ps_site,ps_obs_pending)){
   sc_read_set("NO CHANGE MADE","The observatory could not accept that operation. The work may already be complete, or you may no longer be at this site. No second reward was claimed.");
  }else{
   sc_read_pair(ps_person(),ps_obs_pending==1?"There. A clean reference tone. The port can use its beacon again, and the bonus has been transferred. I wish we could have kept every last sound in that receiver, but we made the choice with our eyes open. Thank you for checking before you touched the reset.":"The recording is safe. Now I can recalibrate without wiping the trace. It might be an old transmitter, an atmospheric effect, or something we haven't recognised. Your archive says exactly that, not a more exciting story we can't support. Thank you for leaving us something to study.",observatory_report(ps_obs_pending));
  }
  ps_reply=0;ps_row=3;ps_obs_pending=0;return;
 }
 if(ps_reply==2){
  if(ps_row==2){ps_reply=0;ps_row=0;sc_read_set(ps_person(),"Take your time. Nothing has been reset or claimed. You can leave and come back; the observations you have already made will remain in your field record.");return;}
  ps_obs_pending=ps_row+1;ps_reply=3;ps_row=0;
  sc_read_set("CONFIRM OBSERVATORY CHOICE",ps_obs_pending==1?"Restore the public beacon? This permanently clears the faint trace and awards the usual survey payment plus 20 units. The choice cannot be changed on a later visit.":"Preserve the trace? This records one additional discovery and awards the usual survey payment, but no 20-unit beacon bonus. The choice cannot be changed on a later visit.");return;
 }
 if(ps_reply){
  if(ps_row==0)sc_read_set(ps_person(),ps_done()?observatory_report(observatory_choice(&game,ps_system,ps_body)):"I came in to align the receiver and heard that pulse. The port wants its reference channel restored. Usually I would reset the receiver and send the report, but this time there is something mixed into the signal. Please check the instrument, the console and my log. I want another pair of eyes before we erase anything.");
  else if(ps_row==1)sc_read_set(ps_person(),ps_personal[FIELD_DISH]);
  else if(ps_row==2)ps_obs_decide();
  else {ps_reply=0;ps_row=0;sc_read_set(ps_person(),"I'll be here if you want to talk again. The telescope has been better company than the transmitter tonight.");}
  return;
 }
 if(ps_row<3){
  if(!ps_done())*ps_obs_flags()|=1u<<(22+ps_row);
  static const char *clues[]={
   "The tracking gears hold their position and the optical reference stays centred. There is no loose mount to explain the pulse. The receiver is hearing something real, although that does not tell you what produced it. You record the mechanical check: the telescope itself is sound.",
   "Two traces cross the screen. One is the expected calibration tone; the other rises at irregular intervals. The reset switch clears the receiver's working memory. A separate recording channel could preserve the faint trace before recalibration, but that procedure does not qualify for the port's rapid-service bonus.",
   "The observer has recorded three nights of interruptions, not three nights of answers. One entry suggests an old transmitter. Another mentions atmospheric reflections. The final line reads: 'Don't call it a message until we can show that somebody sent it.' The port's service terms confirm the 20-unit reset bonus. Preserving the trace instead would create a separate discovery record."
  };
  sc_read_set(ps_object(ps_row),ps_done()?observatory_report(observatory_choice(&game,ps_system,ps_body)):clues[ps_row]);game.cue=SFX_SCAN;
 }else if(ps_row==3){ps_reply=1;ps_row=0;sc_read_set(ps_person(),ps_done()?observatory_report(observatory_choice(&game,ps_system,ps_body)):"Hello. Mind the cable by the telescope. I'm trying to decide whether this receiver needs a routine reset or a little more patience. If you're willing to look, I'd appreciate an independent opinion.");game.cue=SFX_TALK;}
 else ps_obs_decide();
}
/* Repaint only window pixels; foreground telescope, mullions and furniture
 * are protected by a mask authored on the same native pixel canvas. */
static void ps_obs_window(unsigned *pal){
 int ridge[340];unsigned sky[168];FieldProfile p=field_profile(&game,ps_system,ps_body);
 float hour=field_local_hour(&game,ps_system,ps_body);int night=hour<6||hour>19,dusk=!night&&(hour<8||hour>17);
 unsigned tint=game.bodies[ps_body].color,accent=game.bodies[ps_body].accent;
 for(int y=0;y<168;y++)sky[y]=mix_rgb(night?RGB(8,15,34):dusk?RGB(91,55,82):RGB(38,88,128),night?RGB(21,38,54):mix_rgb(tint,dusk?RGB(234,153,92):RGB(146,185,184),.65f),fminf(1,y/100.f));
 for(int x=0;x<340;x++)ridge[x]=67+(int)(sinf(x*.045f+(ps_seed%53))*7+sinf(x*.091f)*4);
 for(int y=23;y<87;y++)for(int x=20;x<276;x++)if(ps_observatory_window_mask[y*340+x]){
  unsigned c=sky[y];
  if(p.biome==5){if(y>64)c=mix_rgb(sky[y],SC_CREAM,.22f);}
  else if(y>=ridge[x])c=mix_rgb(tint,night?SC_VOID:pal[3],night?.75f:.6f);
  if(p.biome==0&&y>78)c=mix_rgb(accent,sky[y],.65f);
  if(night&&y<ridge[x]-5&&field_hash(ps_seed+(unsigned)(x+y*340))%613==0)c=SC_CREAM;
  if(!night&&y<60&&((x+(int)(ps_anim*1.5f)+(ps_seed%107))%117)<32&&y==39+(x/117)*7)c=mix_rgb(c,SC_CREAM,.4f);
  fb[(y+22)*STRIDE+x]=c;
 }
}
static void ps_obs_instruments(void){
 int choice=observatory_choice(&game,ps_system,ps_body);
 rect(169,127,29,8,SC_VOID);
 for(int x=0;x<28;x++){
  int y=choice==1?131:131+(int)(sinf(x*.7f+ps_anim*3)*2);
  pixel(170+x,y,choice==2?SC_AMBER:SC_CYAN);
 }
 if(choice==2){rect(211,151,15,3,SC_AMBER);}
}

