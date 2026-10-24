/* Transient, isolated nightmare. Real player flight/weapons, bounded enemy AI.
 * No new save fields and no persistent traffic identities. */
#define DREAM_ARRIVAL 5.f
#define DREAM_ATTACK 10.f
#define DREAM_LIMIT 120.f
typedef struct {Vec3 pos,vel;float life;} DreamBolt;
static DreamBolt dream_bolts[18];
static Vec3 dream_escape[3],dream_burst_pos[3];
static float dream_leg_time[3],dream_burst[3],dream_bank[3],dream_hit_flash;
static int dream_leg[3],dream_last_alive[3],dream_shots_fired,dream_hits;
static const char *dream_radio_script=
 "NIGHT SHIFT: A caller says his kitchen has acquired a jump drive. Breakfast is now three sectors late.     "
 "Traffic: a man in slippers has closed lane four. He insists it is his hallway.     "
 "Our guest has forgotten his trousers. Fortunately, this is radio. Unfortunately, he is the guest's trousers.     "
 "A listener keeps arriving at exams for subjects he has never studied. We recommend a smaller spaceship.     "
 "Weather: low clouds indoors, clearing when someone asks what time it is.     "
 "Lost property: one front door, last seen running away from its owner. Please do not chase it barefoot.     "
 "The studio clock says half past fish. We will bring you the correct time as soon as the fish agrees.     "
 "Coming up: a familiar voice in a room you cannot quite place. First, another record with no beginning.     "
 "If you recognise this song, please tell us its name. The band would love to know.     "
 "You are listening to the Night Shift. Probably. Stay with us a little longer. There is no hurry. Your shoes are still here.     ";
static float dream_ease(float t){t=fmaxf(0,fminf(1,t));return t*t*(3-2*t);}
static float dream_eye_open(float t){
 if(t<.12f)return 0;
 if(t<.57f)return .48f*dream_ease((t-.12f)/.45f);
 if(t<.78f)return .48f*(1-dream_ease((t-.57f)/.21f));
 return dream_ease((t-.78f)/.82f);
}
static void dream_reset(void){
 memset(dream_bolts,0,sizeof(dream_bolts));memset(dream_leg,0,sizeof(dream_leg));
 memset(dream_leg_time,0,sizeof(dream_leg_time));memset(dream_burst,0,sizeof(dream_burst));
 memset(dream_bank,0,sizeof(dream_bank));memset(dream_last_alive,0,sizeof(dream_last_alive));
 dream_hit_flash=0;dream_shots_fired=dream_hits=0;
 selected_target=-1;autoaim=0;game.legal=0;game.police_stop=game.police_warned=0;
 game.voice_time=game.message_time=game.attacked=game.damage_fx=0;
 game.planet=-1;game.surface=0;game.approach=-1;game.planet_sequence=game.dock_stage=0;
 game.jump=0;game.boost=0;game.energy=game.hull=100;game.damaged=game.dead=0;
 game.heat=game.shot=0;game.pos=(Vec3){1000000,1000000,1000000};game.speed=45;
 game.yaw=game.pitch=game.roll=0;game.missile_time=game.incoming_missile=0;
 memset(game.npc,0,sizeof(game.npc));memset(game.debris,0,sizeof(game.debris));
 for(int i=0;i<NPC_COUNT;i++){game.npc[i].traveller=-1;game.npc[i].bounty_slot=-1;}
}
static Vec3 dream_turn(Vec3 current,Vec3 desired,float dt){
 float c=fmaxf(-1,fminf(1,dot(current,desired))),a=acosf(c),step=fminf(a,dt*1.35f);
 if(a<.0001f)return desired;
 Vec3 side=sub(desired,mul(current,c));
 if(length(side)<.001f)side=fabsf(current.y)<.9f?norm((Vec3){current.z,0,-current.x}):norm((Vec3){0,current.z,-current.y});
 return norm(add(mul(current,cosf(step)),mul(norm(side),sinf(step))));
}
static float dream_segment_distance(Vec3 a,Vec3 b){
 Vec3 d=sub(b,a);float l=dot(d,d),t=l>.001f?fmaxf(0,fminf(1,-dot(a,d)/l)):0;
 return length(add(a,mul(d,t)));
}
static void dream_fire_bolt(NPC *n,int i){
 for(int b=0;b<18;b++)if(dream_bolts[b].life<=0){
  float travel=length(sub(game.pos,n->pos))/1800.f;
  Vec3 aim=add(game.pos,mul(forward(&game),game.speed*travel*.8f));
  /* Small fixed salvo offsets make near misses readable and evasive turns useful. */
  aim.y+=(i-1)*12;dream_bolts[b].pos=add(n->pos,mul(n->dir,n->radius));
  dream_bolts[b].vel=mul(norm(sub(aim,dream_bolts[b].pos)),1800);
  dream_bolts[b].life=2.4f;n->cooldown=1.1f+i*.24f;dream_shots_fired++;
  break;
 }
}
static void dream_fleet_tick(float dt,Vec3 previous_player){
 dream_hit_flash=fmaxf(0,dream_hit_flash-dt);
 if(!dream_spawned&&dream_time>=DREAM_ARRIVAL){
  /* One coherent arrowhead, placed once in world space ahead of the pilot. */
  Vec3 f=forward(&game),right=norm((Vec3){f.z,0,-f.x});
  if(length(right)<.1f)right=(Vec3){1,0,0};
  for(int i=0;i<3;i++){
   NPC *n=&game.npc[i];float side=i==1?-340:i==2?340:0;
   n->alive=1;n->role=PIRATES;n->health=72;n->shield=0;n->mesh=mesh_id("THARGOID");
   n->scale=.61f;n->radius=100;n->target=-2;n->cooldown=.35f+i*.42f;
   n->pos=add(game.pos,add(mul(f,i?2650:2370),add(mul(right,side),(Vec3){0,i?90:0,0})));
   n->dir=mul(f,-1);n->cruise=245;dream_last_alive[i]=1;
  }dream_spawned=3;game.cue=SFX_ALERT;
 }
 for(int i=0;i<3;i++){
  NPC *n=&game.npc[i];dream_burst[i]=fmaxf(0,dream_burst[i]-dt);
  if(dream_last_alive[i]&&!n->alive){dream_burst_pos[i]=n->pos;dream_burst[i]=1.3f;}
  dream_last_alive[i]=n->alive;if(!n->alive)continue;
  n->flash=fmaxf(0,n->flash-dt);n->cooldown-=dt;
  if(dream_time>=DREAM_ATTACK){
   Vec3 to_player=sub(game.pos,n->pos);float dist=length(to_player);
   dream_leg_time[i]+=dt;Vec3 desired;
   if(!dream_leg[i]&&(dist<420||(dist<850&&dot(n->dir,norm(to_player))<-.2f)||dream_leg_time[i]>9)){
    dream_leg[i]=1;dream_leg_time[i]=0;
    Vec3 side=norm((Vec3){n->dir.z,(i-1)*.45f,-n->dir.x});
    dream_escape[i]=add(n->pos,add(mul(n->dir,1150),mul(side,i==1?-780:780)));
   }
   if(dream_leg[i]){
    desired=norm(sub(dream_escape[i],n->pos));n->cruise=340;
    if(dream_leg[i]==1&&(dream_leg_time[i]>3.5f+i*.3f||length(sub(dream_escape[i],n->pos))<130)){
     /* Flank back ahead before the next run, rather than spending the battle
      * glued behind a novice who has not learned to turn around yet. The
      * rendezvous is fixed once per pass; ships still fly every metre. */
     Vec3 ahead=forward(&game),side=norm((Vec3){ahead.z,0,-ahead.x});
     dream_escape[i]=add(game.pos,add(mul(ahead,1150),add(mul(side,i==1?-560:i==2?560:190),(Vec3){0,(i-1)*100.f,0})));
     dream_leg[i]=2;dream_leg_time[i]=0;
    }else if(dream_leg[i]==2&&(dream_leg_time[i]>8||length(sub(dream_escape[i],n->pos))<150)){dream_leg[i]=0;dream_leg_time[i]=0;}
   }else {
    Vec3 lead=add(game.pos,mul(forward(&game),game.speed*.5f));
    desired=norm(sub(lead,n->pos));n->cruise=285+i*15;
    if(dist<2150&&dist>180&&dot(n->dir,norm(to_player))>.94f&&n->cooldown<=0)dream_fire_bolt(n,i);
   }
   Vec3 old=n->dir;n->dir=dream_turn(n->dir,desired,dt);
   float bank=fmaxf(-.6f,fminf(.6f,(old.z*n->dir.x-old.x*n->dir.z)*10));
   dream_bank[i]+=(bank-dream_bank[i])*fminf(1,dt*4);
  }
  n->pos=add(n->pos,mul(n->dir,n->cruise*dt));
  /* Gentle separation instead of teleporting an attacker onto the camera. */
  for(int j=0;j<i;j++)if(game.npc[j].alive){Vec3 gap=sub(n->pos,game.npc[j].pos);float d=length(gap);if(d<180)n->dir=dream_turn(n->dir,norm(add(n->dir,mul(norm(gap),2))),dt);}
 }
 for(int b=0;b<18;b++){
  DreamBolt *p=&dream_bolts[b];if(p->life<=0)continue;Vec3 previous=p->pos;
  p->pos=add(p->pos,mul(p->vel,dt));p->life-=dt;
  /* Swept relative motion: a player can dodge a shot after it leaves the ship. */
  if(dream_segment_distance(sub(previous,previous_player),sub(p->pos,game.pos))<23){
   p->life=0;float absorbed=fminf(game.energy,8);game.energy-=absorbed;
   if(absorbed<8){game.hull-=8-absorbed;game.damaged=1;}
   dream_hit_flash=.18f;dream_hits++;game.attacked=1;game.cue=SFX_HIT;
  }
 }
}
static void dream_world_line(Vec3 a,Vec3 b,unsigned ink){
 a=camera(&game,a);b=camera(&game,b);if(a.z<=15&&b.z<=15)return;
 if(a.z<15)a=add(a,mul(sub(b,a),(15-a.z)/(b.z-a.z)));
 if(b.z<15)b=add(b,mul(sub(a,b),(15-b.z)/(a.z-b.z)));
 Point p=project(a),q=project(b);line((int)p.x,(int)p.y,(int)q.x,(int)q.y,ink);
}
static void dream_radio_draw(void){
 /* Text-only scripted host; no changes to the player's station or saved settings.
  * 8 characters/sec supplies fresh copy throughout the maximum 120s dream. */
 int len=(int)strlen(dream_radio_script),offset=(int)(dream_time*8),face=(int)(dream_time*5)%3;
 char shown[27];for(int i=0;i<26;i++)shown[i]=dream_radio_script[(offset+i)%len];shown[26]=0;
 rect(243,1,233,23,RGB(9,19,28));unsigned ink=RGB(173,200,179);
 rect(250,7,12,10,ink);rect(252,9,2,2,BG);rect(258,9,2,2,BG);rect(254,13,4,face==1?2:1,BG);
 text_px(266,8,ink,"%s",shown);
 for(int i=0;i<26;i++){int h=1+(int)(2*(.5f+.5f*sinf(dream_time*3+i)));line(292+i*6,23-h,294+i*6,23,RGB(53,90,96));}
}
static void opening_dream_view(void){
 /* No normal system renderer, cockpit ticker, mission navigation or law UI. */
 rect(0,0,W,H,RGB(2,4,10));preview_clip(240,110,0,25,W,237);
 for(int y=25;y<237;y++){unsigned c=RGB(3,5+(y<130?y:260-y)/35,12+(y<130?y:260-y)/20);line(0,y,W-1,y,c);}
 starfield();drawcount=0;
 for(int i=0;i<3;i++){NPC *n=&game.npc[i];if(!n->alive)continue;
  float yaw=atan2f(n->dir.x,n->dir.z);unsigned ink=n->flash>0?WHITE:RGB(132,180,107);
  /* The imported octagon's nose is +X, unlike the ordinary +Z ship meshes. */
  shipmesh(n->mesh,n->pos,yaw-1.5707963f,dream_bank[i]+.12f*sinf(dream_time*.8f+i),n->scale,ink,0);
 }flush_meshes();
 for(int i=0;i<3;i++){
  NPC *n=&game.npc[i];if(n->alive){
   float yaw=atan2f(n->dir.x,n->dir.z)-1.5707963f,bank=dream_bank[i]+.12f*sinf(dream_time*.8f+i);
   if(dot(n->dir,norm(sub(game.pos,n->pos)))>.1f){
    /* Eight native geometric rim segments keep the alien silhouette readable. */
    const Mesh *m=&meshes[n->mesh];
    for(int e=0;e<8;e++){Vec3 a=add(n->pos,mul(rotate(m->v[8+e],yaw,bank),n->scale)),b=add(n->pos,mul(rotate(m->v[8+(e+1)%8],yaw,bank),n->scale));dream_world_line(a,b,RGB(86,140,100));}
    Vec3 core=camera(&game,add(n->pos,mul(rotate((Vec3){33,0,0},yaw,bank),n->scale)));
    if(core.z>30){Point p=project(core);pixel(p.x,p.y,RGB(179,250,135));pixel(p.x+1,p.y,RGB(179,250,135));}
   }
   Vec3 tail=sub(n->pos,mul(n->dir,55));dream_world_line(tail,sub(tail,mul(n->dir,75)),RGB(99,199,156));
   dream_world_line(sub(tail,mul(n->dir,75)),sub(tail,mul(n->dir,170)),RGB(29,67,64));
   Vec3 v=camera(&game,n->pos);if(v.z>30){Point p=project(v);if(p.x>15&&p.x<465&&p.y>30&&p.y<221){int r=(int)fmaxf(8,18000/v.z);line(p.x-r,p.y-r,p.x-r+4,p.y-r,RGB(136,92,56));line(p.x+r-4,p.y-r,p.x+r,p.y-r,RGB(136,92,56));}}
  }
  if(dream_burst[i]>0)for(int k=0;k<20;k++){
   float age=1.3f-dream_burst[i],a=k*2.39996f;
   Vec3 d={cosf(a)*(50+k*5),sinf(a)*(60+k*4),(k-10)*13.f};
   Vec3 p=add(dream_burst_pos[i],mul(d,age));dream_world_line(p,add(p,mul(d,.09f)),dream_burst[i]>.75f?RGB(244,223,139):RGB(145,88,57));
  }
 }
 for(int b=0;b<18;b++)if(dream_bolts[b].life>0){DreamBolt *p=&dream_bolts[b];dream_world_line(p->pos,sub(p->pos,mul(p->vel,.055f)),RGB(98,255,150));}
 /* The standard weapon effect assumes the ordinary reticle at y=110. */
 weapon_fx_fire();
 line(226,110,236,110,AMBER);line(244,110,254,110,AMBER);line(240,97,240,106,AMBER);line(240,114,240,123,AMBER);
 if(dream_hit_flash>0){line(3,28,3,233,RGB(101,155,177));line(476,28,476,233,RGB(101,155,177));line(3,233,476,233,RGB(58,101,136));}
 preview_reset();
 rect(0,0,240,24,RGB(7,15,24));text_px(10,8,RGB(143,161,178),"NO NAVIGATION SIGNAL");dream_radio_draw();
 rect(0,237,W,35,RGB(9,19,28));line(0,237,W-1,237,RGB(56,88,98));
 int alive=0;for(int i=0;i<3;i++)alive+=game.npc[i].alive;
 text_px(10,243,RGB(163,189,194),"CONTACTS %d",alive);text_px(140,243,RGB(163,189,194),"SPD %d",(int)game.speed);
 const char *labels[]={"SHLD","HULL","HEAT"};float values[]={game.energy,game.hull,game.heat};
 for(int i=0;i<3;i++){int x=246+i*78;text_px(x,243,DIM,"%s",labels[i]);rect(x,256,64,4,BG);rect(x,256,(int)(64*fmaxf(0,fminf(1,values[i]/100))),4,i==2?AMBER:CYAN);}
 button_icon(10,256,'r',WHITE);text_px(26,258,WHITE,"FLY");button_icon(68,256,'l',WHITE);text_px(84,258,WHITE,"BRAKE");button_icon(134,256,'X',AMBER);text_px(150,258,WHITE,"FIRE");
 /* Three contacts only, including bearings behind the player. */
 for(int i=0;i<3;i++)if(game.npc[i].alive){Vec3 v=camera(&game,game.npc[i].pos);float d=fmaxf(1,length(v));int x=220+(int)(v.x/d*13),y=251-(int)(v.z/d*9);rect(x,y,2,2,RGB(160,210,119));}
 pixel(220,251,WHITE);
}
static void dream_overlay(void){
 const char *copy=dream_time<3?"Huh...? Where am I?":dream_time<6?"Warning. Three contacts incoming.":dream_time<DREAM_ATTACK?"Thargoids. They are closing in.":dream_time<15?"They're attacking! Steer clear and fire!":dream_time<22?"D-pad or nub to steer. Keep moving.":0;
 if(copy&&dream_time>=1.6f&&!dream_ending){
  const char *speaker=dream_time<3?"YOU":"COMPUTER";
  rect(8,29,464,36,RGB(8,19,30));opening_portrait(speaker,12,31,32);text_px(52,33,dream_time<3?AMBER:CYAN,"%s",speaker);text_px(52,49,WHITE,"%s",copy);
 }
 if(dream_time<1.6f){float open=dream_eye_open(dream_time);
  for(int x=0;x<W;x++){float nx=(x-240)/240.f;int half=(int)(open*(150+65*(1-nx*nx))),top=136-half,bottom=136+half;
   for(int y=0;y<H;y++)if(y<top||y>bottom)fb[y*STRIDE+x]=RGB(0,0,0);else if(y<top+3||y>bottom-3)fb[y*STRIDE+x]=dim_rgb(fb[y*STRIDE+x],1,3);
  }
 }
 if(dream_ending){float a=dream_ease(dream_wake/4.f);for(int y=0;y<H;y++)for(int x=0;x<W;x++)fb[y*STRIDE+x]=mix_rgb(fb[y*STRIDE+x],WHITE,a);if(dream_wake>4.8f)text_px(166,130,RGB(30,35,44),"WAKE UP!");}
}
