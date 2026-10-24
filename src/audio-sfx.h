/* Allocation-free event sounds at 44.1 kHz. Essential cockpit feedback uses
 * the licensed resident metal transient included before this header. */
#ifndef ELITE_AUDIO_SFX_H
#define ELITE_AUDIO_SFX_H
static int audio_sfx_length(int id){
 switch(id){
 case SFX_AD_OPEN:return 6174; /* 140 ms playful rising whoop */
 case SFX_AD_CLOSE:return 4851; /* 110 ms descending airy thwip */
 case SFX_POWER_BUS:return 17640;
 case SFX_POWER_RELAY:return 13230;
 case SFX_POWER_READY:return 19845;
 case SFX_OBSERVATORY:return 10584; /* 240ms quiet receiver handshake */
 case SFX_PAPER:return 5292; /* 120 ms paper brush */
 case SFX_SELECT:return 882;       /* 20 ms: quiet navigation tick */
 case SFX_UI:return 1764;
 case SFX_LASER:return 3528;
 case SFX_HIT:return 4410;
 case SFX_SCAN:return 7938;        /* 180 ms: rising confirmation */
 case SFX_COMM:return 5292;
 case SFX_TALK:return 6174;
 case SFX_LAND:return 8820;
 case SFX_DOCK:return 10584;
 case SFX_MINE:return 4410;
 case SFX_BOOST:return 7056;
 case SFX_MISSILE:return 7938;
 case SFX_ALERT:return 13230;      /* two softly edged warning pulses */
 case SFX_WARP:return 13230;
 case SFX_DEATH:return 13230;
 case SFX_ALIEN_PLAYER:return 2205;
 case SFX_ALIEN_FIRE:return 3087;
 case SFX_ALIEN_MISSILE:return 5292;
 case SFX_ALIEN_KILL:return 6174;
 case SFX_SPEED_SURGE:return 8820;
 default:return 0;
 }
}
static int audio_sfx_sample(int id,int age,unsigned *phase,unsigned *noise){
 int length=audio_sfx_length(id);if(age<0||age>=length)return 0;
 int remaining=length-1-age;
 *noise=*noise*1664525u+1013904223u;
 int n=(int)(*noise>>24)-128;
 /* Essential feedback shares one real metal mechanism, reshaped into switch,
  * relay, latch and warning gestures.  Quiet filtered contact noise supplies
  * material variation; no exposed oscillator is used in this family. */
 if(id==SFX_SELECT||id==SFX_UI||id==SFX_SCAN||id==SFX_COMM||id==SFX_DOCK||id==SFX_MISSILE||id==SFX_ALERT){
  int state=(int)*phase,divisor=id==SFX_SELECT?4:id==SFX_UI?8:id==SFX_MISSILE?10:14;
  state+=(n*24-state)/divisor;*phase=(unsigned)state;
  int local=age;
  if(id==SFX_ALERT){local=age%6615;if(local>=3528)return 0;}
  int source_rate=id==SFX_SELECT?6200:id==SFX_UI?7000:id==SFX_DOCK?5200:id==SFX_SCAN?9000:id==SFX_COMM?7600:8000;
  int si=local*source_rate/44100;if(si>=1760)si=1759;
  int recorded=(int)flight_dock_clamp_pcm[si];
  int attack=local<80?local:80,local_left=(id==SFX_ALERT?3527-local:remaining);
  int edge=local_left<attack?local_left:attack;if(remaining<edge)edge=remaining;if(edge<0)edge=0;
  int gain=id==SFX_SELECT?5:id==SFX_UI?7:id==SFX_SCAN?8:id==SFX_COMM?6:id==SFX_DOCK?12:id==SFX_MISSILE?10:9;
  int body=recorded*gain+state*(id==SFX_MISSILE||id==SFX_ALERT?2:1);
  return body*edge/80;
 }
 /* Cold-start cues are layers of the ship that is actually coming alive:
  * generator mass, cockpit machinery and a recorded relay contact. */
 if(id==SFX_POWER_BUS||id==SFX_POWER_RELAY||id==SFX_POWER_READY){
  unsigned ei=(unsigned)age/2u%(unsigned)(sizeof(flight_engine_industrial_pcm)/sizeof(flight_engine_industrial_pcm[0]));
  unsigned mi=(unsigned)age/2u%(unsigned)(sizeof(flight_cockpit_machinery_pcm)/sizeof(flight_cockpit_machinery_pcm[0]));
  int source=id==SFX_POWER_BUS?flight_engine_industrial_pcm[ei]:flight_cockpit_machinery_pcm[mi];
  int state=(int)*phase;state+=(source-state)/(id==SFX_POWER_RELAY?10:22);*phase=(unsigned)state;
  int ci=age*(id==SFX_POWER_RELAY?7200:5200)/44100,contact=ci<1760?(int)flight_dock_clamp_pcm[ci]:0;
  int attack=age<192?age:192,envelope=remaining<attack?remaining:attack;if(envelope<0)envelope=0;
  int body=id==SFX_POWER_BUS?state/10+contact*2:id==SFX_POWER_RELAY?state/15+contact*8:state/18+flight_engine_industrial_pcm[ei]/22+contact*3;
  return body*envelope/192*remaining/length;
 }
 /* Combat is heard through the player's structure. Discharge has a short
  * contact plus turbulent energy; impact has metal and filtered hull force;
  * destruction is one continuous machinery collapse. None is a pitched beep. */
 if(id==SFX_LASER||id==SFX_HIT||id==SFX_MINE||id==SFX_DEATH){
  int state=(int)*phase;state+=(n*32-state)/(id==SFX_LASER?5:id==SFX_DEATH?18:10);*phase=(unsigned)state;
  int attack=age<96?age:96,envelope=remaining<attack?remaining:attack;if(envelope<0)envelope=0;
  int rate=id==SFX_LASER?10500:id==SFX_HIT?7200:id==SFX_MINE?5900:4800;
  int ci=age*rate/44100,contact=ci<1760?(int)flight_dock_clamp_pcm[ci]:0;
  if(id==SFX_LASER){int turbulent=n*12-state/2;return (contact*5+turbulent)*envelope/96*remaining/length;}
  if(id==SFX_HIT)return (contact*8+state/2)*envelope/96*remaining/length;
  if(id==SFX_MINE)return (contact*6+state/2)*envelope/96*remaining/length;
  unsigned ei=(unsigned)age/2u,cl=(unsigned)(sizeof(flight_engine_industrial_pcm)/sizeof(flight_engine_industrial_pcm[0]));if(ei>=cl)ei=cl-1;
  unsigned mi=(unsigned)age/2u%(unsigned)(sizeof(flight_cockpit_machinery_pcm)/sizeof(flight_cockpit_machinery_pcm[0]));
  int collapse=flight_engine_industrial_pcm[ei]/16+flight_cockpit_machinery_pcm[mi]/24+contact*3+state/2;
  return collapse*envelope/96*remaining/length;
 }
 /* Atmosphere capture and touchdown are heard as compression through the
  * hull: recorded engine mass, damped machinery and one buried contact. */
 if(id==SFX_LAND){
  unsigned el=(unsigned)(sizeof(flight_engine_industrial_pcm)/sizeof(flight_engine_industrial_pcm[0]));
  unsigned ml=(unsigned)(sizeof(flight_cockpit_machinery_pcm)/sizeof(flight_cockpit_machinery_pcm[0]));
  unsigned ei=((unsigned)age*11800u/44100u)%el,mi=((unsigned)age*8600u/44100u)%ml;
  int body=flight_engine_industrial_pcm[ei]*3/4+flight_cockpit_machinery_pcm[mi]/4;
  int state=(int)*phase;state+=(body-state)/18;*phase=(unsigned)state;
  int ci=age*4800/44100,contact=ci<1760?(int)flight_dock_clamp_pcm[ci]:0;
  int attack=age<160?age:160,envelope=remaining<attack?remaining:attack;if(envelope<0)envelope=0;
  return (state*2+contact*16)*envelope/5/160*remaining/length;
 }
 /* Hostile technology uses the selected Deep Bioelectric Pressure identity:
  * slowed recorded propulsion under living machinery movement. Short metal
  * excitation is heard through that body rather than exposed as a tap. */
 if(id==SFX_ALIEN_PLAYER||id==SFX_ALIEN_FIRE||id==SFX_ALIEN_MISSILE||id==SFX_ALIEN_KILL){
  unsigned el=(unsigned)(sizeof(flight_engine_industrial_pcm)/sizeof(flight_engine_industrial_pcm[0]));
  unsigned ml=(unsigned)(sizeof(flight_cockpit_machinery_pcm)/sizeof(flight_cockpit_machinery_pcm[0]));
  unsigned er=id==SFX_ALIEN_PLAYER?12800:id==SFX_ALIEN_FIRE?17600:id==SFX_ALIEN_MISSILE?14200:10500;
  unsigned mr=id==SFX_ALIEN_PLAYER?9600:id==SFX_ALIEN_FIRE?15100:id==SFX_ALIEN_MISSILE?11200:8200;
  unsigned ei=((unsigned)age*er/44100u)%el,mi=((unsigned)age*mr/44100u)%ml;
  int body=flight_engine_industrial_pcm[ei]*7/10+flight_cockpit_machinery_pcm[mi]*3/10;
  int state=(int)*phase;state+=(body-state)/(id==SFX_ALIEN_FIRE?9:15);*phase=(unsigned)state;
  int ci=age*(id==SFX_ALIEN_FIRE?7800:id==SFX_ALIEN_KILL?5200:4100)/44100;
  int contact=ci<1760?(int)flight_dock_clamp_pcm[ci]:0;
  int attack=age<128?age:128,envelope=remaining<attack?remaining:attack;if(envelope<0)envelope=0;
  int gain=id==SFX_ALIEN_PLAYER?5:id==SFX_ALIEN_FIRE?9:id==SFX_ALIEN_MISSILE?7:8;
  int excitation=(id==SFX_ALIEN_FIRE||id==SFX_ALIEN_KILL)?contact*(id==SFX_ALIEN_FIRE?4:3):0;
  return (state*gain/8+excitation)*envelope/128*remaining/length;
 }
 /* Remaining world and flight gestures are shaped only from recorded ship
  * material. Slow filtering hides individual mechanical knocks so these read
  * as mass, pressure and motion rather than taps or retro synthesizer tones. */
 if(id==SFX_AD_OPEN||id==SFX_AD_CLOSE||id==SFX_OBSERVATORY||id==SFX_PAPER||id==SFX_TALK||id==SFX_BOOST||id==SFX_WARP||id==SFX_SPEED_SURGE){
  unsigned el=(unsigned)(sizeof(flight_engine_industrial_pcm)/sizeof(flight_engine_industrial_pcm[0]));
  unsigned ml=(unsigned)(sizeof(flight_cockpit_machinery_pcm)/sizeof(flight_cockpit_machinery_pcm[0]));
  unsigned rate=4700u+(unsigned)id*317u;
  if(id==SFX_WARP||id==SFX_SPEED_SURGE)rate+=(unsigned)age*(id==SFX_WARP?17600u:7200u)/(unsigned)length;
  unsigned ei=((unsigned)age*rate/44100u)%el;
  unsigned mi=((unsigned)age*(rate*3u/4u+1700u)/44100u)%ml;
  int physical=flight_engine_industrial_pcm[ei]*3/5+flight_cockpit_machinery_pcm[mi]*2/5;
  int state=(int)*phase;state+=(physical-state)/20;*phase=(unsigned)state;
  int attack=age<160?age:160,envelope=remaining<attack?remaining:attack;if(envelope<0)envelope=0;
  int body=state/3+flight_engine_industrial_pcm[ei]/22;
  if(id==SFX_BOOST){unsigned bi=(unsigned)age*8800u/44100u;if(bi>=4400u)bi=4399u;body=state*2/5+(int)flight_boost_warm_gravity_pcm[bi]*7;}
  return body*envelope/160*remaining/length;
 }
 return 0;
}
#endif
