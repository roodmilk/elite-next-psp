/* First-person transfers use the actual planet renderer. Simulation commits
 * only at phase boundaries; rendering borrows and restores the small camera. */
static int planet_landing_menu=0,planet_controls_ready=0,planet_first_frame=0,planet_first_tick=0;
static int planet_entry_body=0,planet_orbit_release=0;
static float planet_entry_time=0,planet_orbit_veil=0,planet_entry_cover=.85f;
static Vec3 planet_entry_dir;
static float planet_entry_speed;
static float planet_launch_yaw=0,planet_launch_pitch=0;
static unsigned planet_veil_ink=0;
typedef struct {Vec3 eye;float yaw,pitch;} PlanetPilotCamera;
static struct {int active,system,body,rover;float time,from_canopy,to_canopy;PlanetPilotCamera from,to;} planet_seat;
static void space(void);
static float pilot_clamp(float t){return fmaxf(0,fminf(1,t));}
static float pilot_ease(float t){t=pilot_clamp(t);return t*t*t*(t*(t*6-15)+10);}
static float pilot_mix(float a,float b,float t){return a+(b-a)*t;}
static void planet_prepare_veil(int body){float h=field_local_hour(&game,game.system,body);float day=pilot_clamp(sinf((h-6)*.2617994f)*2+.15f);planet_veil_ink=mix_rgb(RGB(35,49,66),RGB(188,210,220),day);}
static void planet_trace(const char *phase,int reset){
 static int count=0;if(smoke)return;if(reset||count>=64){reset=1;count=0;}count++;FILE *f=fopen("landing-trace.txt",reset?"w":"a");if(!f)return;
 fprintf(f,"2.5.240 %s system=%d planet=%d surface=%d sequence=%d stack=%d\n",phase,game.system,game.planet,game.surface,game.planet_sequence,sceKernelCheckThreadStack());fclose(f);
}
static float planet_sequence_duration(int kind){return kind==1?10.f:kind==2?2.2f:8.5f;}
static int planet_sequence_valid(int kind){return game.planet>=1&&game.planet<BODY_COUNT&&!game.dead&&!game.docked&&((kind==2&&game.surface==1)||((kind==1||kind==3)&&game.surface==0));}
static PlanetPilotCamera planet_current_eye(void){PlanetPilotCamera c={game.pos,game.yaw,game.pitch};if(game.surface==1)c.eye.y+=5;return c;}
static void planet_seat_start(PlanetPilotCamera from,int was_vehicle){
 planet_seat.active=1;planet_seat.system=game.system;planet_seat.body=game.planet;planet_seat.time=0;
 planet_seat.from=from;planet_seat.to=planet_current_eye();planet_seat.from_canopy=was_vehicle?1:0;
 planet_seat.to_canopy=(game.surface==1||game.rover_driving)?1:0;planet_seat.rover=game.surface!=1;
 game.voice_time=game.message_time=0;game.boost=0;game.eva_running=game.eva_run_arm=0;game.eva_run_hold=0;
 surface_target_open=0;surface_turn_active=0;planet_controls_ready=0;
}
static void planet_sequence_begin(int kind){
 if(!planet_sequence_valid(kind)){game.planet_sequence=0;return;}
 game.planet_sequence=kind;game.planet_sequence_time=0;game.planet_sequence_anchor=game.ship_pos;
 if(kind==3){planet_launch_yaw=planet_launch_pitch=0;}
 planet_prepare_veil(game.planet);
 planet_seat.active=0;game.voice_time=game.message_time=0;game.boost=0;game.eva_run_arm=game.eva_running=0;game.eva_run_hold=0;autoaim=0;
 planet_landing_menu=0;planet_controls_ready=0;planet_trace(kind==1?"approach-start":kind==2?"touchdown-start":"departure-start",kind==1);
}
static void planet_disembark(void){
 PlanetPilotCamera from=planet_current_eye();
 if(disembark_planet(&game)){planet_first_frame=planet_first_tick=1;surface_target_open=0;surface_target_lock=-1;surface_turn_active=0;surface_nav_poi=-1;planet_seat_start(from,1);}
 planet_controls_ready=0;autoaim=0;planet_trace("airlock",0);
}
static void planet_sequence_finish(int kind){
 game.planet_sequence=0;game.planet_sequence_time=0;planet_controls_ready=0;
 if(kind==3){leave_planet(&game);planet_orbit_veil=1.3f;planet_orbit_release=1;autoaim=0;planet_trace("orbit-restored",0);return;}
 if(kind==2){planet_disembark();return;}
 game.pos=surface_site(&game,1);game.speed=0;game.yaw=game.pitch=game.roll=0;
 if(land_planet(&game))planet_sequence_begin(2);else {planet_landing_menu=1;planet_trace("landing-refused",0);}
}
static void planet_entry_begin(void){
 if(game.approach<1||game.approach>=BODY_COUNT||game.planet>=0||game.dead||game.docked)return;
 planet_entry_body=game.approach;planet_entry_time=0;
 planet_entry_speed=fmaxf(0,game.speed);planet_entry_dir=forward(&game);
 float gap=length(sub(game.pos,game.bodies[planet_entry_body].pos))-game.bodies[planet_entry_body].radius-70;
 planet_entry_cover=planet_entry_speed>1?fmaxf(.08f,fminf(.85f,gap/planet_entry_speed*.65f)):.85f;
 game.boost=0;game.voice_time=game.message_time=0;autoaim=0;planet_controls_ready=0;
 planet_prepare_veil(planet_entry_body);
}
/* Entry is intentional inward flight, not merely flying alongside a world. */
static void planet_auto_entry(void){
 if(game.planet>=0||game.dead||game.docked||game.dock_stage||game.police_stop||game.jump>0||paused||planet_entry_body||planet_orbit_release||planet_orbit_veil>0)return;
 int body=game.approach;
 if(body<1){
  if(game.speed<=1)return;
  float nearest=1e30f;
  for(int i=1;i<BODY_COUNT;i++){
   Body *b=&game.bodies[i];Vec3 d=sub(b->pos,game.pos);float length0=length(d),gap=length0-b->radius;
   if(b->type==SUN||gap>1000+game.speed*.1f||gap<0||dot(forward(&game),norm(d))<.7f)continue;
   if(gap<nearest){nearest=gap;body=i;}
  }
  if(body<1||!approach_planet(&game,body))return;
 }
 if(!story_landing_ready(&game)){
  turn_back(&game);message(&game,"Landing equipment required. Holding outside the atmosphere.");return;
 }
 planet_entry_begin();
}
static int planet_transfer_busy(void){return planet_entry_body||game.planet_sequence||planet_seat.active||planet_orbit_veil>0;}
static int planet_sequence_update(float dt,unsigned pressed){
 (void)pressed;dt=fmaxf(0,fminf(.1f,dt));
 if(planet_entry_body){
  if(game.dead||game.docked||game.approach!=planet_entry_body){planet_entry_body=0;return 1;}
  game.time+=dt;game.world_clock=fmodf(game.world_clock+dt,86400.f);planet_entry_time+=dt;
  /* Clouds become opaque before the safety shell; retain incoming velocity. */
  if(planet_entry_time<planet_entry_cover){
   Body *b=&game.bodies[planet_entry_body];float remaining=fmaxf(0,length(sub(game.pos,b->pos))-b->radius-65);
   game.pos=add(game.pos,mul(planet_entry_dir,fminf(planet_entry_speed*dt,remaining)));game.speed=planet_entry_speed;
  }
  /* A fully opaque frame precedes synchronous world initialisation. */
  if(planet_entry_time>=planet_entry_cover+.35f){planet_entry_body=0;if(enter_planet(&game))planet_sequence_begin(1);}
  return 1;
 }
 if(planet_orbit_veil>0){planet_orbit_veil=fmaxf(0,planet_orbit_veil-dt);game.time+=dt;return 1;}
 if(planet_seat.active){
  if(game.dead||game.docked||game.system!=planet_seat.system||game.planet!=planet_seat.body){planet_seat.active=0;return 1;}
  game.time+=dt;game.world_clock=fmodf(game.world_clock+dt,86400.f);planet_seat.time+=dt;
  if(planet_seat.time>=1.05f){planet_seat.active=0;planet_controls_ready=0;}
  return 1;
 }
 if(!game.planet_sequence)return 0;
 if(!planet_sequence_valid(game.planet_sequence)){game.planet_sequence=0;planet_controls_ready=0;return 1;}
 int kind=game.planet_sequence;game.time+=dt;game.world_clock=fmodf(game.world_clock+dt,86400.f);game.planet_sequence_time+=dt;
 if(game.planet_sequence_time>=planet_sequence_duration(kind))planet_sequence_finish(kind);
 return 1;
}
/* Hermite segments retain velocity through approach, braking and touchdown. */
static float pilot_curve(float a,float b,float va,float vb,float t,float seconds){
 t=pilot_clamp(t);float t2=t*t,t3=t2*t;return (2*t3-3*t2+1)*a+(t3-2*t2+t)*seconds*va+(-2*t3+3*t2)*b+(t3-t2)*seconds*vb;
}
static PlanetPilotCamera planet_pilot_sample(int kind,float time){
 Vec3 pad=surface_site(&game,1);pad.y=terrain_height(&game,pad.x,pad.z);
 PlanetPilotCamera c={pad,0,0};float t=pilot_clamp(time/planet_sequence_duration(kind)),x=0,y=23,z=0;
 if(kind==1){
  if(time<5){float q=time/5;x=pilot_curve(-170,0,35,0,q,5);z=pilot_curve(-1400,-500,210,150,q,5);y=pilot_curve(350,125,-45,-18,q,5);}
  else {float q=(time-5)/5;z=pilot_curve(-500,0,150,0,q,5);y=pilot_curve(125,58,-18,0,q,5);}
  c.yaw=.12f*(1-pilot_ease(time/5));c.pitch=-.12f*(1-pilot_ease((time-4)/6));
 }else if(kind==2){y=pilot_mix(58,23,pilot_ease(t));}
 else {
  if(time<1.5f){float q=pilot_ease(time/1.5f);y=pilot_mix(23,80,q);z=0;c.yaw=planet_launch_yaw*(1-q);c.pitch=planet_launch_pitch*(1-q);}
  else if(time<4.5f){float q=(time-1.5f)/3;z=pilot_curve(0,330,0,210,q,3);y=pilot_curve(80,110,0,30,q,3);c.pitch=pilot_mix(0,.13f,pilot_ease(q));}
  else {float q=(time-4.5f)/4;z=pilot_curve(330,1450,210,310,q,4);y=pilot_curve(110,1450,30,540,q,4);c.pitch=pilot_mix(.13f,1.12f,pilot_ease(q));}
 }
 c.eye=add(pad,(Vec3){x,y,z});
 /* Seeded tall landmarks can cross an otherwise clear flight corridor.
  * A smooth altitude envelope clears them without moving their identities. */
 if(kind!=2)for(int id=1;id<10;id++)if(id!=6){
  Vec3 p=surface_poi(&game,id);FieldBuilding b=field_site_building(&game,id);
  float dx=fmaxf(0,fabsf(c.eye.x-p.x)-b.w-24),dz=fmaxf(0,fabsf(c.eye.z-p.z)-b.d-24);
  float weight=pilot_ease(1-fmaxf(dx,dz)/200.f),safe=terrain_height(&game,p.x,p.z)+b.height+40;
  if(safe>c.eye.y)c.eye.y=pilot_mix(c.eye.y,safe,weight);
 }
 return c;
}
/* Narrow canopy edges preserve the landscape; no fake opaque windshield. */
static void planet_pilot_canopy(int rover,float amount){
 amount=pilot_clamp(amount);if(amount<.01f)return;int drop=(int)((1-amount)*48),base=244+drop;
 unsigned edge=RGB(52,72,83),dark=RGB(12,21,30),lamp=rover?RGB(232,177,92):RGB(90,209,218);
 for(int side=0;side<2;side++){int x=side?W-1:0;for(int i=0;i<5;i++)line(x+(side?-i:i),H-1,x+(side?-35:35),base-26,edge);}
 if(base<H){rect(0,base,W,H-base,dark);line(0,base,W,base,edge);}
 if(base<264){rect(32,base+6,65,2,lamp);rect(W-97,base+6,65,2,lamp);text(25,(base+5)/8,UI_MUTED,rover?"ROAMER":"FLIGHT CONTROL");}
}
/* Coherent drifting veil, continuous opacity; one means completely covered. */
static void planet_cloud_cover(float amount){
 amount=pilot_clamp(amount);if(amount<=0)return;
 unsigned ink=planet_veil_ink?planet_veil_ink:RGB(188,210,220);
 /* Separable waves: avoid thousands of software sin calls per frame. */
 static float xs[2][120],xc[2][120];static int ready;
 if(!ready){for(int x=0;x<120;x++){xs[0][x]=sinf(x*4*.026f);xc[0][x]=cosf(x*4*.026f);xs[1][x]=sinf(-x*4*.011f);xc[1][x]=cosf(-x*4*.011f);}ready=1;}
 for(int y=0;y<H;y+=4){
  float a=y*.015f+game.time*.8f,b=y*.045f-game.time*.45f;
  float sa=sinf(a),ca=cosf(a),sb=sinf(b),cb=cosf(b);
  for(int x=0;x<W;x+=4){
   int k=x/4;float wave=.5f+.25f*(xs[0][k]*ca+xc[0][k]*sa)+.25f*(xs[1][k]*cb+xc[1][k]*sb);
   unsigned c=mix_rgb(ink,RGB(226,237,240),wave*.15f);
   if(amount>=.999f){rect(x,y,4,4,c);continue;}
   float alpha=pilot_clamp(amount*(.7f+.4f*wave)+amount*amount*.3f);
   int weight=(int)(alpha*256),inv=256-weight;unsigned rb=(c&0xff00ff)*weight,g=(c&0x00ff00)*weight;
   for(int yy=y;yy<y+4&&yy<H;yy++)for(int xx=x;xx<x+4&&xx<W;xx++){unsigned p=fb[yy*STRIDE+xx];fb[yy*STRIDE+xx]=0xff000000|((((p&0xff00ff)*inv+rb)>>8)&0xff00ff)|((((p&0x00ff00)*inv+g)>>8)&0x00ff00);}
  }
 }
}
static void planet_render_pilot(PlanetPilotCamera c){
 Vec3 pos=game.pos;float yaw=game.yaw,pitch=game.pitch,roll=game.roll;
 int flag=planet_scene_camera,ox=proj_ox,oy=proj_oy,x0=clipx0,y0=clipy0,x1=clipx1,y1=clipy1;
 game.pos=c.eye;game.yaw=c.yaw;game.pitch=c.pitch;game.roll=0;planet_scene_camera=planet_seat.active?2:1;
 preview_clip(240,110,0,0,W,H);drawcount=0;planet_view();
 game.pos=pos;game.yaw=yaw;game.pitch=pitch;game.roll=roll;planet_scene_camera=flag;preview_clip(ox,oy,x0,y0,x1,y1);
}
static void planet_sequence_view(void){
 if(!planet_sequence_valid(game.planet_sequence)){rect(0,0,W,H,BG);return;}
 int kind=game.planet_sequence;float time=game.planet_sequence_time;
 float cloud=kind==1?1-pilot_ease(time/2.f):kind==3?pilot_ease((time-6.2f)/1.4f):0;
 if(cloud<.999f)planet_render_pilot(planet_pilot_sample(kind,time));else rect(0,0,W,H,planet_veil_ink);
 planet_cloud_cover(cloud);planet_pilot_canopy(0,1);
 text(2,1,UI_MUTED,kind==3?(time<4.5f?"DEPARTURE CORRIDOR":"ORBITAL ASCENT"):kind==2?"LANDING GEAR / TOUCHDOWN":time<2?"CLOUD LAYER":"SPACEPORT APPROACH");
}
static void planet_seat_view(void){
 float t=pilot_clamp(planet_seat.time/1.05f),q=pilot_ease(t);PlanetPilotCamera c;
 c.eye=add(planet_seat.from.eye,mul(sub(planet_seat.to.eye,planet_seat.from.eye),q));c.eye.y+=sinf(t*3.14159265f)*5;
 float angle=atan2f(sinf(planet_seat.to.yaw-planet_seat.from.yaw),cosf(planet_seat.to.yaw-planet_seat.from.yaw));
 c.yaw=planet_seat.from.yaw+angle*q;c.pitch=pilot_mix(planet_seat.from.pitch,planet_seat.to.pitch,q)-sinf(t*3.14159265f)*.045f;
 planet_render_pilot(c);planet_pilot_canopy(planet_seat.rover,pilot_mix(planet_seat.from_canopy,planet_seat.to_canopy,q));
}
static void planet_boarded_view(void){
 planet_render_pilot(planet_current_eye());planet_pilot_canopy(0,1);
 button_icon(14,224,'r',UI_GOLD);text(4,28,WHITE,"LAUNCH");button_icon(330,224,'X',UI_CYAN);text(44,28,WHITE,"STEP OUT");
}
static void planet_landing_prompt_view(void){
 planet_render_pilot(planet_current_eye());planet_pilot_canopy(0,1);
 rect(80,167,320,59,RGB(13,26,36));text(12,22,UI_GOLD,"SPACEPORT LANDING CLEARANCE");
 button_icon(100,199,'X',UI_CYAN);text(15,25,WHITE,"LAND");button_icon(280,199,'O',UI_MUTED);text(38,25,WHITE,"CANCEL");
}
