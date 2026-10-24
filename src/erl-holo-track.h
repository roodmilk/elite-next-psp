/* Derived geometry only: no save fields, allocation, RNG or changed checkpoints.
 * Catmull-Rom ribbon joins the actual ordered course nodes with shared tangents. */
static Vec3 race_track_node(const SpaceRace *r,int n){
 int per=r->laps>0?RACE_GATES/r->laps:RACE_GATES;
 Vec3 closure=sub(r->gates[RACE_GATES-1].pos,r->start);int closed=dot(closure,closure)<100;
 if(closed){n%=per;if(n<0)n+=per;return n?r->gates[n-1].pos:r->start;}
 if(n<0)return sub(mul(r->start,2),r->gates[0].pos);
 if(n>RACE_GATES)return sub(mul(r->gates[RACE_GATES-1].pos,2),r->gates[RACE_GATES-2].pos);
 return n?r->gates[n-1].pos:r->start;
}
static float race_track_bank(const SpaceRace *r,int node){Vec3 a=race_track_node(r,node-1),b=race_track_node(r,node),c=race_track_node(r,node+1),tangent=norm(sub(c,a)),right=norm(race_cross((Vec3){0,1,0},tangent));return fmaxf(-.06f,fminf(.06f,dot(add(sub(c,mul(b,2)),a),right)/fmaxf(1,length(sub(c,a)))*.4f));}
/* Quarter-lap magnetic sector includes its intentional launch gap. */
int race_track_surface(const SpaceRace *r,int section,float t){
 if(r->layout==5){float clock=neon_time(((section%(RACE_GATES/r->laps))+t)/(RACE_GATES/r->laps));if(clock>=50&&clock<53)return 2;return (clock>=10&&clock<25)||(clock>=40&&clock<55);}
 if(r->layout!=4)return 0;int per=RACE_GATES/r->laps;float u=((section%per)+t)/per;
 if(u>=.421875f&&u<.453125f)return 2;
 return u>=.25f&&u<.5f;
}
RaceTrackFrame race_track_frame(const SpaceRace *r,int section,float t){
 if(section<0)section=0;if(section>=RACE_GATES)section=RACE_GATES-1;t=fmaxf(0,fminf(1,t));
 if(r->layout==5)return neon_frame(r,section,t);
 /* The authored round track has an exact analytic frame. Do not run its
  * flat road through the general spline/banking solver every actor tick. */
 if(r->layout==4){int per=RACE_GATES/r->laps;float u=((section%per)+t)/per,angle=u*6.283185307f,c=cosf(angle),sn=sinf(angle),radius=dot(sub(r->gates[per/2-1].pos,r->start),r->right)*.5f;
  RaceTrackFrame f={0};f.section=section;f.t=t;f.centre=add(r->start,add(mul(r->right,radius*(1-c)),mul(r->dir,radius*sn)));f.ahead=add(mul(r->dir,c),mul(r->right,sn));f.right=sub(mul(r->right,c),mul(r->dir,sn));f.up=(Vec3){0,1,0};
  if(u>=.375f&&u<=.421875f){f.centre.y+=1100*(u-.375f)/.046875f;f.ahead=norm(add(f.ahead,(Vec3){0,1100/(radius*6.283185307f*.046875f),0}));f.up=race_cross(f.ahead,f.right);}return f;
 }
 Vec3 a=race_track_node(r,section),b=race_track_node(r,section+1);
 Vec3 m0=mul(sub(b,race_track_node(r,section-1)),.5f),m1=mul(sub(race_track_node(r,section+2),a),.5f);
 float t2=t*t,t3=t2*t;
 RaceTrackFrame f={0};f.section=section;f.t=t;
 f.centre=add(add(mul(a,2*t3-3*t2+1),mul(m0,t3-2*t2+t)),add(mul(b,-2*t3+3*t2),mul(m1,t3-t2)));
 f.ahead=norm(add(add(mul(a,6*t2-6*t),mul(m0,3*t2-4*t+1)),add(mul(b,-6*t2+6*t),mul(m1,3*t2-2*t))));
 if(dot(f.ahead,f.ahead)<.25f)f.ahead=r->gates[section].normal;
 f.right=norm(race_cross((Vec3){0,1,0},f.ahead));if(dot(f.right,f.right)<.25f)f.right=r->gates[section].right;
 f.up=norm(race_cross(f.ahead,f.right));
 float weight=t*t*(3-2*t),bank=race_track_bank(r,section)*(1-weight)+race_track_bank(r,section+1)*weight;
 if(r->layout==4){int per=RACE_GATES/r->laps;float u=((section%per)+t)/per;
  if(u>=.375f&&u<=.421875f){float ramp=(u-.375f)/.046875f;f.centre.y+=1100*ramp;f.ahead=norm(add(f.ahead,(Vec3){0,1100/(r->length/r->laps*.046875f),0}));f.right=norm(race_cross((Vec3){0,1,0},f.ahead));f.up=norm(race_cross(f.ahead,f.right));}
  bank=0;
 }
 Vec3 right=f.right;f.right=add(mul(right,cosf(bank)),mul(f.up,sinf(bank)));f.up=norm(race_cross(f.ahead,f.right));return f;
}
RaceTrackFrame race_track_nearest(const SpaceRace *r,int gate,Vec3 pos){
 if(gate>=RACE_GATES)gate=RACE_GATES-1;if(gate<0)gate=0;
 if(r->layout==5){float best=1.e30f,bt=0;int bk=gate;
  /* Cached right/section/t are redundant: interpolate centre/ahead/up at the
   * unchanged 33 samples, then rebuild the same complete output frame. */
  typedef struct {Vec3 centre,ahead,up;} NeonNearestFrame;
  typedef struct {Vec3 pos;int gate;RaceTrackFrame frame;} NeonNearestMemo;
  static NeonNearestFrame cache[8][33];static NeonNearestMemo memo[32];static unsigned memo_count,memo_next;
  static int owners[8],laps;static Vec3 origin,course_right,course_up,course_dir;static float speed;static unsigned seed;
  if(memcmp(&origin,&r->start,sizeof(origin))||memcmp(&course_right,&r->right,sizeof(course_right))||memcmp(&course_up,&r->up,sizeof(course_up))||memcmp(&course_dir,&r->dir,sizeof(course_dir))||speed!=r->top_speed||laps!=r->laps||seed!=r->seed){origin=r->start;course_right=r->right;course_up=r->up;course_dir=r->dir;speed=r->top_speed;laps=r->laps;seed=r->seed;for(int i=0;i<8;i++)owners[i]=-1;memo_count=memo_next=0;}
  /* Repeated render/support queries share exact positions. Changed physical
   * positions still run the full ordered search; no quantization or lookahead. */
  for(unsigned i=0;i<memo_count;i++)if(memo[i].gate==gate&&!memcmp(&memo[i].pos,&pos,sizeof(pos)))return memo[i].frame;
  for(int k=gate>0?gate-1:0;k<=gate;k++){int slot=k%8;if(owners[slot]!=k){for(int i=0;i<=32;i++){RaceTrackFrame f=neon_frame(r,k,i/32.f);cache[slot][i]=(NeonNearestFrame){f.centre,f.ahead,f.up};}owners[slot]=k;}
   NeonNearestFrame f=cache[slot][0];Vec3 a=add(f.centre,mul(f.up,RACE_TRACK_FLOOR));for(int i=1;i<=32;i++){f=cache[slot][i];Vec3 b=add(f.centre,mul(f.up,RACE_TRACK_FLOOR)),axis=sub(b,a);float denominator=dot(axis,axis);if(denominator<1)denominator=1;float t=dot(sub(pos,a),axis)/denominator;if(t<0)t=0;else if(t>1)t=1;Vec3 d=sub(pos,add(a,mul(axis,t)));float ds=dot(d,d);if(ds<best){best=ds;bk=k;bt=(i-1+t)/32.f;}a=b;}}
  int n=(int)(bt*32);if(n>31)n=31;float blend=bt*32-n;NeonNearestFrame a=cache[bk%8][n],b=cache[bk%8][n+1];RaceTrackFrame f={0};f.t=bt;f.section=bk;f.centre=add(a.centre,mul(sub(b.centre,a.centre),blend));f.ahead=norm(add(a.ahead,mul(sub(b.ahead,a.ahead),blend)));Vec3 up=norm(add(a.up,mul(sub(b.up,a.up),blend)));f.right=norm(race_cross(up,f.ahead));f.up=norm(race_cross(f.ahead,f.right));
  memo[memo_next]=(NeonNearestMemo){pos,gate,f};memo_next=(memo_next+1)&31;if(memo_count<32)memo_count++;return f;
 }
 if(r->layout==4){int per=RACE_GATES/r->laps;float radius=dot(sub(r->gates[per/2-1].pos,r->start),r->right)*.5f;Vec3 delta=sub(pos,r->start);float angle=atan2f(dot(delta,r->dir),radius-dot(delta,r->right));if(angle<0)angle+=6.283185307f;float u=angle*per/6.283185307f;int section=(gate/per)*per+(int)u;float t=u-(int)u;
  int first=gate>0?gate-1:0;if(section<first||section>gate){RaceTrackFrame a=race_track_frame(r,first,0),b=race_track_frame(r,gate,1);Vec3 da=sub(pos,a.centre),db=sub(pos,b.centre);return dot(da,da)<dot(db,db)?a:b;}return race_track_frame(r,section,t);
 }
 int best_section=gate;float best_t=0,distance=1.e30f;
 /* Adjacent ordered sections only; nearby later laps never provide shortcuts. */
 for(int k=gate>0?gate-1:0;k<=gate;k++){
  Vec3 start=race_track_node(r,k),end=race_track_node(r,k+1),m0=mul(sub(end,race_track_node(r,k-1)),.5f),m1=mul(sub(race_track_node(r,k+2),start),.5f),a=start;
  for(int i=1;i<=6;i++){
   float u=i/6.f,u2=u*u,u3=u2*u;
   Vec3 b=add(add(mul(start,2*u3-3*u2+1),mul(m0,u3-2*u2+u)),add(mul(end,-2*u3+3*u2),mul(m1,u3-u2))),axis=sub(b,a);
   float t=fmaxf(0,fminf(1,dot(sub(pos,a),axis)/fmaxf(1,dot(axis,axis))));
   Vec3 delta=sub(pos,add(a,mul(axis,t)));float d=dot(delta,delta);
   if(d<distance){distance=d;best_section=k;best_t=(i-1+t)/6.f;}a=b;
  }
 }
 return race_track_frame(r,best_section,best_t);
}

/* The tunnel's floor and axes agree with the curved road, for rendering,
 * swept passage detection and rival steering. Its bottom sits on the road. */
RaceTrackFrame race_repair_frame(const SpaceRace *r,int section){RaceTrackFrame f=race_track_frame(r,section,.5f);f.centre=add(f.centre,add(mul(f.right,-4300),mul(f.up,RACE_TRACK_FLOOR+1000)));return f;}
/* One curved footprint for rendered pads and every actor's swept trigger. */
Vec3 race_pad_point(const SpaceRace *r,int section,float t,float lane){RaceTrackFrame f=race_track_frame(r,section,t);return add(f.centre,add(mul(f.right,((r->layout==4||r->layout==5)?0:2500)+lane),mul(f.up,RACE_TRACK_FLOOR+4)));}
/* Authored full-lap pad data drives both visible chevrons and swept contact.
 * The generated normalized arcs invert the same 129-entry route-clock LUT
 * as the road; each anchor owns one existing per-section depletion bit. */
int __attribute__((noinline)) race_neon_pad(const SpaceRace *r,int section,float *centre,float *lane){
 if(r->layout!=5||r->laps<=0||section<0||section>=RACE_GATES)return 0;
 int per=RACE_GATES/r->laps;if(per<=0)return 0;int local=section%per;
 for(int i=0;i<(int)(sizeof(neon_pads)/sizeof(neon_pads[0]));i++){
  float q=neon_pads[i][0]*per;if((int)q!=local)continue;
  if(centre)*centre=q-(int)q;if(lane)*lane=neon_pads[i][1]*2.5f*race_neon_scale(r);return 1;
 }return 0;
}
float race_neon_pad_lane(const SpaceRace *r,int section){float lane=0;race_neon_pad(r,section,0,&lane);return lane;}
/* Deliberately unbounded: pads near a section join extend into its neighbour.
 * Consumers must normalize section + t before calling race_track_frame. */
float race_neon_pad_t(const SpaceRace *r,int section,int station){float centre=.5f;race_neon_pad(r,section,&centre,0);return centre+(station-2)*56*(RACE_GATES/r->laps)/NEON_SOURCE_LENGTH;}
int race_pad_crossing(const SpaceRace *r,int section,Vec3 from,Vec3 to,float support){if(r->layout==5&&!race_neon_pad(r,section,0,0))return 0;
 if(r->layout==4){int per=RACE_GATES/r->laps;int k=section%per;if(k!=per/8&&k!=per*3/4)return 0;}
 /* Conservative swept-volume rejection before the ten spline evaluations.
  * The entire authored bed, legal support heights and its curved stations
  * fit comfortably inside this sphere. Never reject a crossing just because
  * a high-speed endpoint has already passed the pad. */
 if(r->layout==5){float t=.5f,lane=0;race_neon_pad(r,section,&t,&lane);RaceTrackFrame f=race_track_frame(r,section,t);
  Vec3 c=add(f.centre,add(mul(f.right,lane),mul(f.up,RACE_TRACK_FLOOR+4))),axis=sub(to,from);
  float length2=dot(axis,axis),along=length2>1?dot(sub(c,from),axis)/length2:0;along=fmaxf(0,fminf(1,along));
  Vec3 d=sub(add(from,mul(axis,along)),c);float bound=900* race_neon_scale(r)+support;
  if(dot(d,d)>bound*bound)return 0;
 }
 for(int i=0;i<5;i++){float u=r->layout==5?race_neon_pad_t(r,section,i):.32f+i*.075f;int frame_section=section;
  if(r->layout==5){while(u<0){u+=1;frame_section--;}while(u>1){u-=1;frame_section++;}}
  RaceTrackFrame f=race_track_frame(r,frame_section,u);float lane=r->layout==5?race_neon_pad_lane(r,section):0;Vec3 c=race_pad_point(r,frame_section,u,lane);
  float a=dot(sub(from,c),f.ahead),b=dot(sub(to,c),f.ahead);if(a>=0||b<=0||b-a<.0001f)continue;
  Vec3 hit=sub(add(from,mul(sub(to,from),-a/(b-a))),c);float height=dot(hit,f.up);
  float half_width=r->layout==5?ERL_PAD_HALF_WIDTH*2.5f*race_neon_scale(r):1100;
  if(fabsf(dot(hit,f.right))<half_width&&height>=support-25&&height<=support+230)return 1;
 }return 0;
}
/* All ERL entries use the same conservative racer envelope. It contains
 * every roster hull/fin and is independent of the parked spacefaring ship. */
float race_floor_support(const Game *g,int id,RaceTrackFrame f){
 Vec3 ahead=id?g->race.pilot[id-1].dir:forward(g),right=norm(race_cross((Vec3){0,1,0},ahead)),up=norm(race_cross(ahead,right));
 if(id&&g->race.layout==5){up=norm(sub(f.up,mul(ahead,dot(f.up,ahead))));right=norm(race_cross(up,ahead));}
 if(!id){Vec3 base=right;right=add(mul(base,cosf(g->roll)),mul(up,sinf(g->roll)));up=norm(race_cross(ahead,right));}
 return 8+(fabsf(dot(right,f.up))*74+fabsf(dot(up,f.up))*36+fabsf(dot(ahead,f.up))*158)*(id?RACE_SHIP_SCALE:1.2f);
}
float race_neon_body_radius(const Game *g){(void)g;return 225;}

