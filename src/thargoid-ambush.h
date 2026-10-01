#ifndef ARCELITE_THARGOID_AMBUSH_H
#define ARCELITE_THARGOID_AMBUSH_H

/* Allocation-free hyperspace rail encounter. It never enters commander save
 * data: defeat restores the departure system and victory resumes the jump. */
#define THARGOID_MAX 8
#define THARGOID_BOLTS 24
#define THARGOID_MISSILES 8
typedef struct {float x,y,z,phase,shot,entry,flash,age;int hp,alive,kind,pattern,volleys;} ThargoidRaider;
typedef struct {float x,y,vx,vy,life;int alive;} ThargoidBolt;
typedef struct {float x,y,vx,vy,life,trail[6][2];int alive,hp,trail_n;} ThargoidMissile;
static ThargoidRaider thargoid_raider[THARGOID_MAX];
static ThargoidBolt thargoid_bolt[THARGOID_BOLTS];
static ThargoidMissile thargoid_missile[THARGOID_MISSILES];
static int thargoid_active,thargoid_checked,thargoid_failed_flash,thargoid_wave,thargoid_kills,thargoid_wave_kills,thargoid_hp,thargoid_origin,thargoid_destination;
static int thargoid_spawned,thargoid_total,thargoid_surge_mark;
static float thargoid_time,thargoid_cursor_x,thargoid_cursor_y,thargoid_fire_cd,thargoid_enemy_fire_cd,thargoid_wave_delay,thargoid_hit_flash;
static float thargoid_intro,thargoid_laser,thargoid_missile_flash,thargoid_formation_cue;

static int thargoid_route_section(void){return ((int)(thargoid_time/9.f))%5;}
static float thargoid_route_power(void){float p=fmodf(thargoid_time,12.f);if(p>8.f&&p<10.5f){float q=(p-8.f)/2.5f;return 1.f+sinf(q*3.14159265f)*2.6f;}return 1.f;}
static void thargoid_reset(void){
 memset(thargoid_raider,0,sizeof(thargoid_raider));memset(thargoid_bolt,0,sizeof(thargoid_bolt));memset(thargoid_missile,0,sizeof(thargoid_missile));
 thargoid_active=thargoid_checked=thargoid_failed_flash=0;thargoid_wave=thargoid_kills=thargoid_wave_kills=0;thargoid_hp=100;
 thargoid_time=thargoid_fire_cd=thargoid_enemy_fire_cd=thargoid_wave_delay=thargoid_hit_flash=0;thargoid_cursor_x=240;thargoid_cursor_y=126;
 thargoid_intro=thargoid_laser=thargoid_missile_flash=thargoid_formation_cue=0;thargoid_spawned=thargoid_total=0;thargoid_surge_mark=-1;
}
static Vec3 thargoid_position(const ThargoidRaider *e){
 float local=fmodf(e->age+e->phase*.11f,7.6f),hold,depth;
 if(local<1.15f){hold=local/1.15f;depth=1700.f-hold*930.f;}else if(local<5.f){hold=(local-1.15f)/3.85f;depth=770.f+sinf(hold*6.283f+e->phase)*75.f;}else {hold=(local-5.f)/2.6f;depth=770.f-hold*650.f;}
 float lane=e->x*2.7f,weave=sinf(thargoid_time*(.72f+e->pattern*.12f)+e->phase),x=lane+weave*(42.f+e->pattern*16.f),y=e->y*2.f+cosf(thargoid_time*(.85f+e->kind*.06f)+e->phase)*55.f;
 if(e->pattern==1){x+=sinf(thargoid_time*1.7f+e->phase)*95.f;y+=cosf(thargoid_time*1.7f+e->phase)*55.f;}else if(e->pattern==2){float side=e->x<0?-1.f:1.f;x+=side*sinf(hold*3.14159f)*150.f;y+=fabsf(weave)*70.f-30.f;}else if(e->pattern==3){x+=sinf(thargoid_time*2.2f+e->phase)*125.f;y+=sinf(thargoid_time*1.1f+e->phase)*90.f;}
 if(local>=5.f){float dive=(local-5.f)/2.6f;x+=(e->x<0?-1.f:1.f)*dive*520.f;y+=sinf(e->phase)*dive*260.f;}
 return (Vec3){x,y,depth};
}
static int thargoid_attack_window(const ThargoidRaider *e){float local=fmodf(e->age+e->phase*.11f,7.6f);return e->entry<=0&&local>1.2f&&local<5.5f;}
static void thargoid_project(const ThargoidRaider *e,float *x,float *y){Vec3 p=thargoid_position(e);*x=240+p.x*240/p.z;*y=126-p.y*240/p.z;}
static void thargoid_spawn_raider(int slot){
 ThargoidRaider *e=&thargoid_raider[slot];memset(e,0,sizeof(*e));int serial=thargoid_spawned++;
 e->alive=1;e->kind=(serial+thargoid_wave*2)%5;e->pattern=(thargoid_wave-1)%4;e->hp=2+thargoid_wave/2+(e->kind==4);
 float lane=slot-(THARGOID_MAX-1)*.5f;e->x=lane*72.f;
 if(e->pattern==0)e->y=fabsf(lane)*34.f-58.f;else if(e->pattern==1)e->y=(slot&1?1.f:-1.f)*(38.f+fabsf(lane)*9.f);else e->y=((slot%3)-1)*54.f;
 e->z=.9f;e->phase=serial*.24f+thargoid_wave*.41f;e->shot=2.4f+(slot%3)*.7f;e->entry=serial<6?slot*.22f:1.f;
}
static void thargoid_spawn_wave(void){memset(thargoid_raider,0,sizeof(thargoid_raider));memset(thargoid_bolt,0,sizeof(thargoid_bolt));memset(thargoid_missile,0,sizeof(thargoid_missile));thargoid_wave_kills=0;thargoid_spawned=0;thargoid_total=9+thargoid_wave*3;thargoid_enemy_fire_cd=2.4f;thargoid_formation_cue=2.2f;for(int i=0;i<6;i++)thargoid_spawn_raider(i);thargoid_wave_delay=0;}
static void thargoid_begin(void){
 thargoid_active=1;thargoid_checked=1;thargoid_failed_flash=0;thargoid_wave=1;thargoid_kills=0;thargoid_hp=100;thargoid_time=0;thargoid_origin=game.system;thargoid_destination=game.destination;thargoid_cursor_x=240;thargoid_cursor_y=126;thargoid_fire_cd=thargoid_hit_flash=0;thargoid_surge_mark=-1;thargoid_spawn_wave();game.speed=0;game.boost=0;game.cue=SFX_ALERT;message(&game,"THARGOID INTERDICTION. Hold Cross to fire. Shoot down incoming missiles.");thargoid_intro=4.f;thargoid_laser=thargoid_missile_flash=0;
}
static int thargoid_maybe_start(void){if(game.jump<=0){thargoid_checked=0;return 0;}if(game.jump>7.5f&&!thargoid_active)thargoid_checked=0;if(game.jump>4.9f)return 0;if(thargoid_checked)return thargoid_active;thargoid_checked=1;unsigned h=field_hash((unsigned)(game.system+1)*31337u^(unsigned)(game.destination+7)*7919u^(unsigned)(game.world_clock*10.f)^game.rng);if(!debug_force_thargoid&&h%100u>=14u)return 0;thargoid_begin();return 1;}
static void thargoid_finish(int won){
 thargoid_active=0;memset(thargoid_bolt,0,sizeof(thargoid_bolt));memset(thargoid_missile,0,sizeof(thargoid_missile));
 if(won){game.jump=2.6f;game.cue=SFX_WARP;char note[96];snprintf(note,sizeof(note),"INTERDICTION CLEARED. %d kills / %.1f units. Jump continuing.",thargoid_kills,thargoid_kills*20.f);message(&game,note);}else {game.jump=0;game.speed=0;game.boost=0;game.system=thargoid_origin;game.destination=thargoid_destination;thargoid_failed_flash=1;game.cue=SFX_ALERT;message(&game,"INTERDICTION FAILED. Returned to departure system; no jump fuel spent.");}
}
static void thargoid_damage(int amount){thargoid_hp-=amount;if(thargoid_hp<0)thargoid_hp=0;thargoid_hit_flash=.24f;game.cue=SFX_HIT;if(!thargoid_hp)thargoid_finish(0);}
static void thargoid_kill(ThargoidRaider *e){e->alive=0;thargoid_kills++;thargoid_wave_kills++;game.kills++;game.credits+=200;game.cue=SFX_ALIEN_KILL;if(thargoid_spawned<thargoid_total)thargoid_spawn_raider((int)(e-thargoid_raider));int alive=0;for(int i=0;i<THARGOID_MAX;i++)alive+=thargoid_raider[i].alive;if(!alive){if(thargoid_wave>=3)thargoid_finish(1);else {memset(thargoid_missile,0,sizeof(thargoid_missile));thargoid_wave_delay=5.5f;}}}
static void thargoid_enemy_bolt(float x,float y){for(int i=0;i<THARGOID_BOLTS;i++)if(!thargoid_bolt[i].alive){ThargoidBolt *b=&thargoid_bolt[i];float travel=1.35f;b->alive=1;b->x=x;b->y=y;b->vx=(thargoid_cursor_x-x)/travel;b->vy=(thargoid_cursor_y-y)/travel;b->life=travel;game.cue=SFX_ALIEN_FIRE;return;}}
static void thargoid_enemy_missile(float x,float y){int active=0,limit=thargoid_wave>=3?3:2;for(int i=0;i<THARGOID_MISSILES;i++)active+=thargoid_missile[i].alive;if(active>=limit)return;for(int i=0;i<THARGOID_MISSILES;i++)if(!thargoid_missile[i].alive){ThargoidMissile *m=&thargoid_missile[i];memset(m,0,sizeof(*m));m->alive=1;m->hp=1+thargoid_wave/2;m->x=x;m->y=y;m->life=4.4f;m->trail_n=1;m->trail[0][0]=x;m->trail[0][1]=y;game.cue=SFX_ALIEN_MISSILE;return;}}
static int thargoid_shoot_missile(void){ThargoidMissile *best=0;float nearest=9999;for(int i=0;i<THARGOID_MISSILES;i++)if(thargoid_missile[i].alive){float dx=thargoid_missile[i].x-thargoid_cursor_x,dy=thargoid_missile[i].y-thargoid_cursor_y,d=sqrtf(dx*dx+dy*dy);if(d<nearest){nearest=d;best=&thargoid_missile[i];}}float hit=best?17.f+(4.4f-best->life)*1.7f:0;if(!best||nearest>hit)return 0;if(--best->hp<=0){best->alive=0;thargoid_missile_flash=.18f;game.cue=SFX_ALIEN_KILL;}else game.cue=SFX_HIT;return 1;}
static void thargoid_fire(void){
 if(thargoid_fire_cd>0||!thargoid_active||thargoid_intro>0||thargoid_wave_delay>0)return;thargoid_fire_cd=.16f;thargoid_laser=.075f;game.cue=SFX_ALIEN_PLAYER;if(thargoid_shoot_missile())return;
 ThargoidRaider *best=0;float best_d=9999;for(int i=0;i<THARGOID_MAX;i++)if(thargoid_raider[i].alive&&thargoid_attack_window(&thargoid_raider[i])){float x,y;thargoid_project(&thargoid_raider[i],&x,&y);float dx=x-thargoid_cursor_x,dy=y-thargoid_cursor_y,d=sqrtf(dx*dx+dy*dy);if(d<best_d){best_d=d;best=&thargoid_raider[i];}}
 if(best){Vec3 p=thargoid_position(best);float hit=18+fmaxf(0,850-p.z)*.035f;if(best_d<hit){best->hp--;best->flash=.12f;if(best->hp<=0)thargoid_kill(best);}}
}
static void thargoid_tick(float dt,float ax,float ay,unsigned held){
 if(!thargoid_active)return;if(dt<=0||dt>.1f)dt=1.f/60;thargoid_time+=dt;thargoid_fire_cd=fmaxf(0,thargoid_fire_cd-dt);thargoid_enemy_fire_cd=fmaxf(0,thargoid_enemy_fire_cd-dt);thargoid_formation_cue=fmaxf(0,thargoid_formation_cue-dt);thargoid_hit_flash=fmaxf(0,thargoid_hit_flash-dt);thargoid_missile_flash=fmaxf(0,thargoid_missile_flash-dt);thargoid_laser=fmaxf(0,thargoid_laser-dt);
 if(thargoid_intro>0){thargoid_intro=fmaxf(0,thargoid_intro-dt);return;}int surge=(int)(thargoid_time/12.f);float surge_phase=fmodf(thargoid_time,12.f);if(surge_phase>8.f&&surge!=thargoid_surge_mark){thargoid_surge_mark=surge;game.cue=SFX_SPEED_SURGE;}
 float dx=ax,dy=ay;if(held&PSP_CTRL_LEFT)dx=-1;if(held&PSP_CTRL_RIGHT)dx=1;if(held&PSP_CTRL_UP)dy=-1;if(held&PSP_CTRL_DOWN)dy=1;thargoid_cursor_x+=dx*dt*210;thargoid_cursor_y+=dy*dt*160;if(thargoid_cursor_x<34)thargoid_cursor_x=34;if(thargoid_cursor_x>446)thargoid_cursor_x=446;if(thargoid_cursor_y<48)thargoid_cursor_y=48;if(thargoid_cursor_y>218)thargoid_cursor_y=218;
 if(held&PSP_CTRL_CROSS)thargoid_fire();if(!thargoid_active)return;if(thargoid_wave_delay>0){thargoid_wave_delay-=dt;if(thargoid_wave_delay<=0){thargoid_wave++;thargoid_spawn_wave();game.cue=SFX_ALERT;}return;}
 for(int i=0;i<THARGOID_MAX;i++)if(thargoid_raider[i].alive){ThargoidRaider *e=&thargoid_raider[i];if(e->entry>0){e->entry-=dt;continue;}e->age+=dt;e->flash=fmaxf(0,e->flash-dt);e->shot-=dt;if(e->shot<=0&&thargoid_enemy_fire_cd<=0&&thargoid_attack_window(e)){float x,y;thargoid_project(e,&x,&y);e->volleys++;thargoid_enemy_missile(x,y);e->shot=5.2f+(i%3)*.65f;thargoid_enemy_fire_cd=fmaxf(1.7f,2.35f-thargoid_wave*.16f);}}
 for(int i=0;i<THARGOID_BOLTS;i++)if(thargoid_bolt[i].alive){ThargoidBolt *b=&thargoid_bolt[i];b->x+=b->vx*dt;b->y+=b->vy*dt;b->life-=dt;if(b->life<=0){float bx=b->x-thargoid_cursor_x,by=b->y-thargoid_cursor_y;b->alive=0;if(bx*bx+by*by<24*24){thargoid_damage(6+thargoid_wave*2);if(!thargoid_active)return;}}}
 for(int i=0;i<THARGOID_MISSILES;i++)if(thargoid_missile[i].alive){ThargoidMissile *m=&thargoid_missile[i];float travel=fmaxf(.2f,m->life),wantx=(thargoid_cursor_x-m->x)/travel,wanty=(thargoid_cursor_y-m->y)/travel;m->vx+=(wantx-m->vx)*fminf(1,dt*3.4f);m->vy+=(wanty-m->vy)*fminf(1,dt*3.4f);m->x+=m->vx*dt;m->y+=m->vy*dt;m->life-=dt;if(m->trail_n<6)m->trail_n++;for(int k=m->trail_n-1;k>0;k--){m->trail[k][0]=m->trail[k-1][0];m->trail[k][1]=m->trail[k-1][1];}m->trail[0][0]=m->x;m->trail[0][1]=m->y;if(m->life<=0){float bx=m->x-thargoid_cursor_x,by=m->y-thargoid_cursor_y;m->alive=0;if(bx*bx+by*by<34*34){thargoid_damage(14+thargoid_wave*3);if(!thargoid_active)return;}}}
}
static void thargoid_input(unsigned pressed,unsigned held,float dt,float ax,float ay){if(thargoid_active&&(pressed&PSP_CTRL_SELECT)){thargoid_finish(1);thargoid_checked=1;audio_battle=0;memset(thargoid_raider,0,sizeof(thargoid_raider));memset(thargoid_missile,0,sizeof(thargoid_missile));thargoid_intro=thargoid_laser=thargoid_wave_delay=thargoid_hit_flash=0;message(&game,"Thargoid encounter skipped. Hyperspace jump continuing.");return;}thargoid_tick(dt,ax,ay,held);}

static void thargoid_craft(const ThargoidRaider *e){
 static const char *shape[]={"THARGOID","KRAIT","MAMBA","GECKO","SIDEWINDER"};static const unsigned colour[]={RGB(91,210,124),RGB(178,110,225),RGB(62,205,205),RGB(231,103,143),RGB(205,224,84)};
 Vec3 p=thargoid_position(e);float bank=sinf(thargoid_time*1.3f+e->phase)*.18f,scale=.48f+e->kind*.035f;shipmesh(mesh_id(shape[e->kind]),p,3.14159265f+sinf(thargoid_time*.7f+e->phase)*.08f,bank,scale,e->flash>0?WHITE:colour[e->kind],1);
}
static void thargoid_star(int i,float time,int *x,int *y){unsigned h=field_hash((unsigned)i*977u+71u);float angle=(h%6283)*.001f,speed=thargoid_route_power(),radius=12.f+fmodf((float)((h>>13)%240)+time*(10.f+(h%8))*speed,240.f);*x=240+(int)(cosf(angle)*radius);*y=126+(int)(sinf(angle)*radius*.43f);}
static void thargoid_tunnel(float speed){
 for(int ring=0;ring<8;ring++){float z=330+fmodf(ring*215.f-thargoid_time*700.f*speed,1720.f),r=430.f;int lastx=0,lasty=0;for(int spoke=0;spoke<=12;spoke++){float a=spoke*6.2831853f/12.f+(ring&1)*.18f;int x=240+(int)(cosf(a)*r*240/z),y=126+(int)(sinf(a)*r*.58f*240/z);if(spoke&&((spoke+ring)%4)!=0)line(lastx,lasty,x,y,ring<2?RGB(145,102,74):RGB(75,61,58));lastx=x;lasty=y;}}
 for(int i=0;i<14;i++){unsigned h=field_hash(i*3571u+91u);float z=420+fmodf(i*173.f-thargoid_time*760.f*speed,1700.f),a=i*2.399f+(h&31)*.02f;Vec3 p={cosf(a)*430,sinf(a)*250,z};shipmesh(mesh_id(i%3?"BOULDER":"ASTEROID"),p,a+thargoid_time*.2f,a*.3f,.34f+(i%3)*.08f,RGB(83,70,65),1);}
}
static void thargoid_crystal_route(float speed){for(int i=0;i<28;i++){unsigned h=field_hash(i*4591u+17u);float z=260+fmodf(i*83.f-thargoid_time*900.f*speed,1900.f),side=i&1?1.f:-1.f,x=side*(330+(h%500)),y=((int)((h>>10)%700)-350.f);int sx=240+(int)(x*240/z),sy=126+(int)(y*240/z),tipx=240+(int)((x-side*90)*240/z),tipy=126+(int)((y-70)*240/z);line(sx,sy,tipx,tipy,i%3?RGB(63,178,210):RGB(191,86,230));line(sx+1,sy,tipx+1,tipy,RGB(210,235,255));}}
static void thargoid_megastructure(float speed){
 for(int ring=0;ring<7;ring++){float z=350+fmodf(ring*290.f-thargoid_time*520.f*speed,2050.f),r=510.f+(ring&1)*100;int rx=(int)(r*240/z),ry=(int)(r*.42f*240/z);for(int s=0;s<12;s++){float a=s*6.283f/12.f,b=(s+1)*6.283f/12.f;line(240+(int)(cosf(a)*rx),126+(int)(sinf(a)*ry),240+(int)(cosf(b)*rx),126+(int)(sinf(b)*ry),s%3?RGB(64,84,109):RGB(185,76,118));}}
 for(int i=0;i<4;i++){float z=600+i*360-fmodf(thargoid_time*260.f,360.f);shipmesh(mesh_id(i&1?"ANACONDA":"PYTHON"),(Vec3){(i&1?1:-1)*(500+i*80.f),(i-2)*100.f,z},1.57f,i*.4f,.38f,RGB(54,66,82),1);}
}
static void thargoid_void_storm(float speed){for(int i=0;i<70;i++){unsigned h=field_hash(i*811u+303u);float a=(h%6283)*.001f+thargoid_time*(i&1?.22f:-.18f),r=60+(h>>12)%1100,z=500+fmodf(i*71.f-thargoid_time*1000.f*speed,1600.f);int x=240+(int)(cosf(a)*r*240/z),y=126+(int)(sinf(a)*r*.48f*240/z);line(x,y,x+(int)(cosf(a)*10*speed),y+(int)(sinf(a)*5*speed),i%5?RGB(91,57,144):RGB(55,202,190));}}
static void thargoid_transition_route(void){
 if(thargoid_wave_delay<=0)return;float elapsed=5.5f-thargoid_wave_delay,turn=sinf(elapsed*.86f+thargoid_wave*1.7f),lift=cosf(elapsed*.62f)*18.f;int cx=240+(int)(turn*82.f),cy=126+(int)lift;
 /* Curved gate rings make the camera appear to bank through a route rather
  * than merely moving straight ahead while the next formation loads. */
 for(int ring=0;ring<7;ring++){float z=310+fmodf(ring*260.f-elapsed*880.f,1820.f),r=360.f+(ring&1)*45.f;int rx=(int)(r*240/z),ry=(int)(r*.48f*240/z),ox=240+(int)(turn*(1.f-z/2200.f)*95.f),oy=126+(int)(lift*(1.f-z/2200.f));for(int s=0;s<12;s++){float a=s*6.2831853f/12.f,b=(s+1)*6.2831853f/12.f;if((s+ring)%4)line(ox+(int)(cosf(a)*rx),oy+(int)(sinf(a)*ry),ox+(int)(cosf(b)*rx),oy+(int)(sinf(b)*ry),thargoid_wave==1?RGB(103,82,68):RGB(65,96,122));}}
 preview_clip(cx,cy,0,24,W,232);
 if(thargoid_wave==1){for(int i=0;i<11;i++){float z=360+fmodf(i*211.f-elapsed*930.f,1900.f),side=i&1?1.f:-1.f;Vec3 p={side*(310.f+(i%4)*92.f),((i%3)-1)*135.f,z};shipmesh(mesh_id(i%3?"BOULDER":"ASTEROID"),p,elapsed*.55f+i,elapsed*.31f+i*.2f,.8f+(i%3)*.32f,RGB(91,77,68),1);}}
 else {for(int i=0;i<5;i++){float z=470+i*420-fmodf(elapsed*470.f,420.f),side=i&1?1.f:-1.f;Vec3 p={side*(410.f+i*75.f),(i-2)*115.f,z};shipmesh(mesh_id(i&1?"ANACONDA":"PYTHON"),p,side*1.48f,turn*.28f,1.05f+(i%2)*.34f,i&1?RGB(48,78,101):RGB(92,48,90),1);}}
 preview_clip(240,126,0,24,W,232);
}
static void thargoid_corridor(void){
 float speed=thargoid_route_power(),lookx=(thargoid_cursor_x-240)*.10f,looky=(thargoid_cursor_y-126)*.08f;for(int i=0;i<86;i++){unsigned h=field_hash(i*7919u+125u);float z=1800.f-fmodf((float)(h%1600)+thargoid_time*1100.f*speed,1600.f);float x=(float)((h>>10)%2600)-1300-lookx,y=(float)((h>>20)%1000)-500+looky;if(fabsf(x)<95&&fabsf(y)<55)x+=160;int a=240+(int)(x*240/z),b=126+(int)(y*240/z),c=240+(int)(x*240/(z+120*speed)),d=126+(int)(y*240/(z+120*speed));line(c,d,a,b,i%9?RGB(50,82,112):RGB(150,196,220));}
 int section=thargoid_route_section();if(section==1)thargoid_tunnel(speed);else if(section==2)thargoid_crystal_route(speed);else if(section==3)thargoid_megastructure(speed);else if(section==4)thargoid_void_storm(speed);else for(int i=0;i<9;i++){unsigned h=field_hash(i*1777u+34u);float z=2400.f-fmodf(i*287.f+thargoid_time*820.f*speed,2240.f);Vec3 p={(i&1?1:-1)*(580.f+(h%440)),((int)((h>>12)%700)-350.f),z};shipmesh(mesh_id(i&1?"BOULDER":"ASTEROID"),p,thargoid_time*.25f+i,thargoid_time*.18f,.38f,RGB(72,81,94),1);}
}
static void thargoid_view(void){
 rect(0,0,W,H,RGB(3,5,18));preview_clip(240,126,0,24,W,232);float power=thargoid_route_power();for(int i=0;i<48;i++){int x,y;thargoid_star(i,thargoid_time,&x,&y);int len=1+(int)(power*3);line(x,y,x+(x-240)*len/80,y+(y-126)*len/80,power>2?WHITE:RGB(56,78,112));}
 thargoid_corridor();thargoid_transition_route();for(int i=0;i<THARGOID_MAX;i++)if(thargoid_raider[i].alive&&thargoid_raider[i].entry<=0)thargoid_craft(&thargoid_raider[i]);flush_meshes();
 for(int i=0;i<THARGOID_BOLTS;i++)if(thargoid_bolt[i].alive){ThargoidBolt *b=&thargoid_bolt[i];line((int)b->x,(int)b->y,(int)(b->x-b->vx*.08f),(int)(b->y-b->vy*.08f),RGB(255,76,164));circle((int)b->x,(int)b->y,2,WHITE);}
 for(int i=0;i<THARGOID_MISSILES;i++)if(thargoid_missile[i].alive){ThargoidMissile *m=&thargoid_missile[i];for(int k=m->trail_n-1;k>0;k--)line((int)m->trail[k][0],(int)m->trail[k][1],(int)m->trail[k-1][0],(int)m->trail[k-1][1],k>3?RGB(82,36,112):RGB(255,83,188));int x=(int)m->x,y=(int)m->y,size=4+(int)((4.4f-m->life)*2.5f);if(size<4)size=4;if(size>15)size=15;line(x-size,y,x,y-size,RGB(255,85,190));line(x,y-size,x+size,y,RGB(255,85,190));line(x+size,y,x,y+size,RGB(255,85,190));line(x,y+size,x-size,y,RGB(255,85,190));circle(x,y,size>9?4:2,WHITE);if(m->life<1.7f){circle(x,y,size+5,RGB(255,185,72));line(x,y,(int)thargoid_cursor_x,(int)thargoid_cursor_y,RGB(105,52,82));}}
 int cx=(int)thargoid_cursor_x,cy=(int)thargoid_cursor_y;if(thargoid_laser>0){line(62,230,cx,cy,UI_CYAN);line(418,230,cx,cy,UI_CYAN);pixel(cx,cy,WHITE);}line(cx-11,cy,cx-4,cy,UI_GOLD);line(cx+4,cy,cx+11,cy,UI_GOLD);line(cx,cy-11,cx,cy-4,UI_GOLD);line(cx,cy+4,cx,cy+11,UI_GOLD);preview_reset();for(int k=0;k<4;k++){line(k,178,64+k,232,RGB(36,48,61));line(479-k,178,415-k,232,RGB(36,48,61));}line(0,173,70,232,RGB(81,113,134));line(479,173,409,232,RGB(81,113,134));
 if(thargoid_hit_flash>0){rect(0,28,3,201,RED);rect(477,28,3,201,RED);}if(thargoid_missile_flash>0){circle(cx,cy,18,RGB(255,110,210));circle(cx,cy,23,RGB(91,220,235));}
 rect(0,0,W,24,UI_PANEL);text_px(12,8,UI_CYAN,"HYPERSPACE INTERCEPT");text_px(368,8,UI_GOLD,"+%d U",thargoid_kills*20);rect(0,231,W,19,UI_PANEL);text_px(8,236,WHITE,"HULL");rect(52,237,116,6,RGB(45,22,34));rect(52,237,116*thargoid_hp/100,6,thargoid_hp<30?RED:UI_CYAN);text_px(200,236,UI_MUTED,"TO %.11s",game.systems[thargoid_destination].name);
 int incoming=0;for(int i=0;i<THARGOID_MISSILES;i++)incoming+=thargoid_missile[i].alive;if(incoming)text_px(350,216,RGB(255,104,196),"MISSILE %d",incoming);if(power>2)text_px(190,30,WHITE,"VELOCITY SURGE");if(thargoid_formation_cue>0&&thargoid_wave_delay<=0){static const char *form[]={"VANGUARD WEDGE","CROSSING LANCES","PINCER FORMATION","HUNTER SWARM"};text_px(176,31,UI_GOLD,"%s",form[(thargoid_wave-1)%4]);}if(thargoid_wave_delay>0)text_px(thargoid_wave==1?164:150,31,WHITE,thargoid_wave==1?"ROCK CANYON - BANKING":"CAPITAL GRAVEYARD - TURNING");
 if(thargoid_intro>0){rect(48,61,384,128,UI_PANEL);text_px(80,72,UI_GOLD,"HYPERSPACE INTERCEPTED");text_px(72,93,WHITE,"Alien formations are entering the corridor.");text_px(72,113,UI_CYAN,"Incoming missiles grow as they approach.");button_icon(72,134,'X',WHITE);text_px(96,134,WHITE,"Hold to fire; shoot missiles before impact.");text_px(72,154,UI_MUTED,"Enemy kills earn 20U. Select always skips.");text_px(184,174,UI_GOLD,"READY IN %d",(int)ceilf(thargoid_intro));}footer("NUB / D-PAD AIM + DODGE    X FIRE    SELECT SKIP");
}

#endif
