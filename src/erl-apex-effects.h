/* Apex effects: fixed history/particle budgets, no gameplay RNG or heap use. */
static uint64_t race_fx_us;static unsigned race_fx_frames;
#define APEX_HISTORY 16
#define APEX_SPARKS 96
typedef struct {Vec3 pos,right,up,ahead;} ApexPose;
typedef struct {Vec3 root[8];int count;} ApexEngines;
typedef struct {Vec3 pos,vel,up,floor;float life,total;int live,ember;} ApexSpark;
static struct {ApexPose history[RA_ACTORS][APEX_HISTORY];int head,count,valid;unsigned seed;float last,sample,emission,embers;ApexSpark spark[APEX_SPARKS];unsigned serial;int pad_seen[RA_ACTORS];} apex_fx;
static ApexPose race_apex_pose(int id){
 SpaceRace *r=&game.race;ApexPose p={0};p.pos=id?r->pilot[id-1].pos:game.pos;p.ahead=id?r->pilot[id-1].dir:forward(&game);p.right=norm(race_ui_cross((Vec3){0,1,0},p.ahead));p.up=norm(race_ui_cross(p.ahead,p.right));
 if(id&&r->layout==5){RaceTrackFrame f=race_track_nearest(r,r->pilot[id-1].gate,p.pos);Vec3 n=sub(f.up,mul(p.ahead,dot(f.up,p.ahead)));if(dot(n,n)>.00001f){p.up=norm(n);p.right=norm(race_ui_cross(p.up,p.ahead));p.up=norm(race_ui_cross(p.ahead,p.right));}}
 if(!id){Vec3 right=p.right;p.right=add(mul(right,cosf(game.roll)),mul(p.up,sinf(game.roll)));p.up=norm(race_ui_cross(p.ahead,p.right));}return p;
}
static Vec3 race_apex_local(ApexPose p,Vec3 local,float scale){return add(p.pos,add(mul(p.right,local.x*scale),add(mul(p.up,local.y*scale),mul(p.ahead,local.z*scale))));}
static ApexEngines race_apex_engines(int actor){
 int id=erl_racer_id(&game,actor);ApexEngines e={0};e.count=2;e.root[0]=erl_racer_engine(id,-1);e.root[1]=erl_racer_engine(id,1);return e;
}
static void race_apex_tick(void){
 SpaceRace *r=&game.race;if(!race_is_active(&game)){apex_fx.valid=0;return;}
 if(!apex_fx.valid||apex_fx.seed!=r->seed||r->elapsed<apex_fx.last){memset(&apex_fx,0,sizeof(apex_fx));apex_fx.valid=1;apex_fx.seed=r->seed;apex_fx.last=r->elapsed;for(int id=0;id<=r->rivals;id++)apex_fx.history[id][0]=race_apex_pose(id);apex_fx.count=1;}
 float dt=fmaxf(0,fminf(.1f,r->elapsed-apex_fx.last));if(dt<=0||r->phase!=RACE_RUNNING||game.erl.paused)return;apex_fx.last=r->elapsed;apex_fx.sample+=dt;
 if(apex_fx.sample>=.033333f){apex_fx.sample=fmodf(apex_fx.sample,.033333f);apex_fx.head=(apex_fx.head+1)%APEX_HISTORY;for(int id=0;id<=r->rivals;id++)apex_fx.history[id][apex_fx.head]=race_apex_pose(id);if(apex_fx.count<APEX_HISTORY)apex_fx.count++;}
 for(int i=0;i<APEX_SPARKS;i++){ApexSpark *p=&apex_fx.spark[i];if(!p->live)continue;p->life-=dt;if(p->life<=0){p->live=0;continue;}p->vel=sub(p->vel,mul(p->up,(p->ember?80:950)*dt));p->pos=add(p->pos,mul(p->vel,dt));float height=dot(sub(p->pos,p->floor),p->up);if(height<0&&!p->ember){p->pos=sub(p->pos,mul(p->up,height));float down=dot(p->vel,p->up);if(down<0)p->vel=sub(mul(p->vel,.65f),mul(p->up,down*.95f));}}
 ApexPose pose=race_apex_pose(0);RaceTrackFrame f=race_track_nearest(r,r->gate,game.pos);float height=dot(sub(game.pos,f.centre),f.up),support=race_floor_support(&game,0,f);int contact=height<RACE_TRACK_FLOOR+support+65&&fabsf(dot(sub(game.pos,f.centre),f.right))<RACE_TRACK_WIDTH;
 int wall=r->arcade.wall_ripple>.25f;
 if((contact||wall)&&game.speed>100){apex_fx.emission+=dt*(wall?100:r->boost_time>0?180:45);int n=(int)apex_fx.emission;apex_fx.emission-=n;const ErlRacer *s=&erl_racers[erl_racer_id(&game,0)];
  for(int e=0;e<n;e++){unsigned h=(++apex_fx.serial*1664525u+1013904223u);int slot=-1;for(int i=0;i<APEX_SPARKS;i++)if(!apex_fx.spark[i].live){slot=i;break;}if(slot<0)break;Vec3 local={(h&1?1:-1)*s->pod,-6,-s->tail+(int)(h%70)};Vec3 point=race_apex_local(pose,local,1.2f);point=sub(point,mul(f.up,dot(sub(point,f.centre),f.up)-RACE_TRACK_FLOOR-5));
   if(wall)point=r->arcade.wall_impact;
   ApexSpark *p=&apex_fx.spark[slot];*p=(ApexSpark){.pos=point,.vel=add(mul(f.up,230+(h%220)),add(mul(f.right,((int)(h>>8)%301-150)),mul(f.ahead,-game.speed*.28f))),.up=f.up,.floor=point,.life=.35f+(h%100)*.002f,.total=.55f,.live=1};}
 }
 apex_fx.embers+=dt*60;int fragments=(int)apex_fx.embers;apex_fx.embers-=fragments;
 for(int e=0;e<fragments;e++){unsigned h=++apex_fx.serial*1664525u+1013904223u;int id=(h>>12)%(r->rivals+1);if(r->arcade.actor[id].eliminated||(id&&r->pilot[id-1].disabled))continue;
  int mesh=id?r->pilot[id-1].mesh:mesh_id(player_ships[game.ship].name);ApexEngines eng=race_apex_engines(id);if(!eng.count)continue;int j=id?r->pilot[id-1].boosting?16:7:game.boost||r->boost_time>0||r->arcade.actor[0].rocket>0?16:7;if(j>=apex_fx.count)continue;
  ApexPose p=apex_fx.history[id][(apex_fx.head-j+APEX_HISTORY)%APEX_HISTORY];if(length(sub(p.pos,game.pos))>14000)continue;
  for(int i=0;i<APEX_SPARKS;i++)if(!apex_fx.spark[i].live){Vec3 point=race_apex_local(p,eng.root[h%eng.count],id?RACE_SHIP_SCALE:1.2f);apex_fx.spark[i]=(ApexSpark){.pos=point,.vel=add(mul(p.up,70+(h%80)),mul(p.right,(int)((h>>8)%121)-60)),.up=p.up,.floor=point,.life=.28f+(h%80)*.002f,.total=.44f,.live=1,.ember=1};break;}
 }
}
static void race_apex_glint(Vec3 world,float power){
 Vec3 v=camera(&game,world);if(v.z<15)return;Point p=project(v);int x=p.x,y=p.y;if(x<clipx0+4||x>=clipx1-4||y<(clipy0<0?surface_y_min:clipy0)+3||y>=(clipy1<0?surface_y_max:clipy1)-3)return;
 if(surface_encode_depth(1/v.z)>surface_depth[y*W+x]+8)return;
 if(power>=.25f){unsigned depth=surface_encode_depth(1/v.z);for(int dx=-7;dx<=7;dx++)for(int dy=-1;dy<=1;dy++){
  int px=x+dx,py=y+dy;if(px<clipx0||px>=clipx1||py<(clipy0<0?surface_y_min:clipy0)||py>(clipy1<0?surface_y_max:clipy1-1)||depth>surface_depth[py*W+px]+8)continue;
  unsigned old=fb[py*STRIDE+px],mask=0xf0f0f0;int shift=dy?4:3;if(!dy)mask=0xf8f8f8;fb[py*STRIDE+px]=old-((old&mask)>>shift)+((RGB(228,97,169)&mask)>>shift);
 }}
 int radius=1+(int)(power*3);for(int i=-radius;i<=radius;i++){pixel(x+i,y,abs(i)<2?RGB(255,230,173):RGB(175,82,111));if(abs(i)<2)pixel(x,y+i,RGB(255,174,115));}pixel(x,y,WHITE);
}
static void race_apex_shadow(int id,ApexPose pose){
 SpaceRace *r=&game.race;int gate=id?r->pilot[id-1].gate:r->gate;if(gate>=RACE_GATES)return;RaceTrackFrame f=race_track_nearest(r,gate,pose.pos);float h=dot(sub(pose.pos,f.centre),f.up)-RACE_TRACK_FLOOR;if(h<0||h>1500)return;
 Vec3 c=sub(pose.pos,mul(f.up,h-12)),a=add(c,mul(f.ahead,190)),b=add(c,mul(f.right,90)),d=sub(c,mul(f.ahead,110)),e=sub(c,mul(f.right,90));unsigned old=race_glass_ink;race_glass_ink=RGB(8,4,20);race_wall_triangle(a,b,d,90);race_wall_triangle(a,d,e,90);race_glass_ink=old;
}
static void race_apex_effects(int rear){
 SpaceRace *r=&game.race;race_apex_tick();if(r->phase!=RACE_RUNNING)return;uint64_t begin=sceKernelGetSystemTimeWide();int old_encoding=mega_depth_encoding,old_depth=surface_depth_on;mega_depth_encoding=surface_depth_on=1;
 for(int id=0;id<=r->rivals;id++){if(r->arcade.actor[id].eliminated||(id&&r->pilot[id-1].disabled))continue;if(!id&&(!third_person||rear))continue;
  ApexPose pose=race_apex_pose(id);float distance=length(sub(pose.pos,camera_origin(&game)));if(distance>15000)continue;
  /* Exhaust, shadows and glow are expensive translucent geometry.  A rival
   * wholly outside the camera cannot contribute a visible pixel, so reject
   * it before building its shadow mesh and trail.  Keep the player path
   * unconditional because the close chase camera may sit inside its bound. */
  if(id&&!render_sphere_visible(pose.pos,900))continue;
  race_apex_shadow(id,pose);int mesh=id?r->pilot[id-1].mesh:mesh_id(player_ships[game.ship].name);ApexEngines engines=race_apex_engines(id);float scale=id?RACE_SHIP_SCALE:1.2f;
  int boosting=id?r->pilot[id-1].boosting||r->arcade.actor[id].rocket>0:game.boost||r->boost_time>0||r->arcade.actor[0].rocket>0;
  int samples=id?(boosting?6:3):(boosting?18:8);if(samples>=apex_fx.count)samples=apex_fx.count-1;int step=distance>6000?3:2;if(samples<1)samples=1;
  for(int e=0;e<engines.count;e++){Vec3 root=race_apex_local(pose,engines.root[e],scale),previous=root;
   for(int j=1;j<=samples;j+=step){int at=(apex_fx.head-j+APEX_HISTORY)%APEX_HISTORY;Vec3 tail=race_apex_local(apex_fx.history[id][at],engines.root[e],scale);if(apex_fx.count<=1)tail=add(root,mul(pose.ahead,-40));Vec3 rawtail=tail;float fade=1-(float)j/(samples+2);
    Vec3 ca=camera(&game,previous),cb=camera(&game,tail);if(length(sub(previous,tail))>2400||(ca.z<18&&cb.z<18)){previous=rawtail;continue;}
    /* Preserve the hot wake right down to the chase camera instead of
     * dropping the whole segment.  Clip in world space before building the
     * translucent quad so no near-plane triangle can explode over the HUD. */
    if(ca.z<18){float t=(18-ca.z)/fmaxf(.0001f,cb.z-ca.z);previous=add(previous,mul(sub(tail,previous),t));ca=camera(&game,previous);}
    if(cb.z<18){float t=(ca.z-18)/fmaxf(.0001f,ca.z-cb.z);tail=add(previous,mul(sub(tail,previous),t));cb=camera(&game,tail);}
    float width=(boosting?7:4)*fade*(.8f+.2f*sinf(r->elapsed*25+j+e)),oldwidth=(boosting?7:4)*fminf(1,fade+.16f);
    oldwidth=fminf(oldwidth,fmaxf(15,camera(&game,previous).z)*4/proj_focal);width=fminf(width,fmaxf(15,camera(&game,tail).z)*4/proj_focal);
    Vec3 side=apex_fx.history[id][at].right,pa=sub(previous,mul(pose.right,oldwidth)),pb=add(previous,mul(pose.right,oldwidth)),ta=sub(tail,mul(side,width)),tb=add(tail,mul(side,width));
    unsigned oldink=race_glass_ink;int budget=race_wall_samples;race_glass_ink=RGB(255,81,160);race_wall_samples=0;race_wall_triangle(pa,pb,tb,45);race_wall_triangle(pa,tb,ta,45);race_glass_ink=oldink;race_wall_samples=budget;
    if(ca.z>=15||cb.z>=15){Vec3 cr=camera(&game,add(previous,pose.right));cr=sub(cr,ca);if(ca.z<15)ca=add(ca,mul(sub(cb,ca),(15-ca.z)/(cb.z-ca.z)));if(cb.z<15)cb=add(cb,mul(sub(ca,cb),(15-cb.z)/(ca.z-cb.z)));
     for(int layer=0;layer<2;layer++){float w=layer?.18f:.45f,wa=fminf(oldwidth*w,ca.z*3/proj_focal),wb=fminf(width*w,cb.z*3/proj_focal);unsigned warm=layer?RGB((int)(255*fade),(int)(225*fade),(int)(174*fade)):RGB((int)(255*fade),(int)(135*fade),(int)(81*fade));Vec3 a=sub(ca,mul(cr,wa)),b=add(ca,mul(cr,wa)),c=add(cb,mul(cr,wb)),d=sub(cb,mul(cr,wb));queue_triangle(a,b,c,warm);queue_triangle(a,c,d,warm);}}
    flush_solid_meshes();race_boundary_line(previous,tail,RGB((int)(255*fade),(int)(227*fade),(int)(174*fade)));previous=rawtail;
   }
   race_apex_glint(root,boosting?1:.25f);if(apex_fx.count>5){int at=(apex_fx.head-samples+APEX_HISTORY)%APEX_HISTORY;Vec3 tail=race_apex_local(apex_fx.history[id][at],engines.root[e],scale);tail=add(tail,mul(pose.up,sinf(r->elapsed*19+e+id)*8));race_apex_glint(tail,.1f);}
  }
 }
 for(int i=0;i<APEX_SPARKS;i++){ApexSpark *p=&apex_fx.spark[i];if(!p->live)continue;race_boundary_line(p->pos,add(p->pos,mul(p->vel,-.008f)),p->life>.25f?RGB(255,229,225):RGB(211,81,146));}
 mega_depth_encoding=old_encoding;surface_depth_on=old_depth;race_fx_us+=sceKernelGetSystemTimeWide()-begin;race_fx_frames++;
}
static void race_apex_rush(void){
 SpaceRace *r=&game.race;if(r->phase!=RACE_RUNNING)return;float speed=game.speed/fmaxf(1,r->top_speed),mega=r->boost_time>0?1:0;int count=r->layout==5?(speed>1.05f||game.boost?12:6):18+(int)(fminf(2.5f,speed)*14);
 for(int i=0;i<count;i++){float phase=fmodf(r->elapsed*(1.65f+speed*1.35f)+i*.173f,1),y=61+(i*47%154);int side=i&1?1:-1;float x=240+side*(184+phase*51),length=10+phase*18+speed*11+mega*34;
  unsigned colors[]={RGB(128,59,156),RGB(227,100,199),RGB(255,193,231),RGB(172,63,162)};line(x,y,x+side*length,y+(y-136)*length/210,colors[i%4]);}
 if(!third_person){line(3,218,78,222,RGB(88,35,85));line(402,222,476,218,RGB(88,35,85));if(r->arcade.actor[0].contact>0)for(int i=0;i<6;i++){int x=i&1?478:1,y=188-(int)(fmodf(r->elapsed*8+i*.17f,1)*45);line(x,y,x+(i&1?-1:1)*(6+i),y-9,RGB(255,181,226));}}
}
