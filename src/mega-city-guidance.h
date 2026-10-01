/* On-demand 3D routing, fixed 5083-node scratch (~30 KiB), no per-frame search.
 * All links are swept against the SAME buildings the pilot sees. An unavailable
 * route leaves the ship under player control; never teleport through a district. */
#define MC_NX 23
#define MC_NY 17
#define MC_NZ 13
#define MC_NODES (MC_NX*MC_NY*MC_NZ)
static short mc_parent[MC_NODES];
static unsigned short mc_queue[MC_NODES];
static Vec3 mc_path[96];static int mc_path_n,mc_path_at,mc_system;
static float mc_spacing=900;
static Vec3 mc_node(int i,float front){return (Vec3){((i%MC_NX)-11)*mc_spacing,((i/MC_NX)%MC_NY-8)*mc_spacing,front+(i/(MC_NX*MC_NY))*mc_spacing};}
static int mc_clear(const Game *g,Vec3 a,Vec3 b){return !station_architecture_hit(g,a,b,80,0);}
static int mc_plan(const Game *g,Vec3 start){
 float front=-station_half_for(g,0)-1000;Vec3 goal={0,0,front};
 mc_spacing=station_class(g)==STATION_MEGA?900:station_class(g)==STATION_RICH?500:200;
 mc_path_n=mc_path_at=0;mc_system=g->system;
 if(mc_clear(g,start,goal)){mc_path[mc_path_n++]=goal;return 1;}
 for(int i=0;i<MC_NODES;i++)mc_parent[i]=-2;
 int sx=(int)floorf(start.x/mc_spacing+11.5f),sy=(int)floorf(start.y/mc_spacing+8.5f),sz=(int)floorf((start.z-front)/mc_spacing+.5f);
 if(sx<0)sx=0;if(sx>=MC_NX)sx=MC_NX-1;if(sy<0)sy=0;if(sy>=MC_NY)sy=MC_NY-1;if(sz<0)sz=0;if(sz>=MC_NZ)sz=MC_NZ-1;
 int root=-1;float best=1e30f;
 for(int dz=-2;dz<=2;dz++)for(int dy=-2;dy<=2;dy++)for(int dx=-2;dx<=2;dx++){
  int x=sx+dx,y=sy+dy,z=sz+dz;if(x<0||x>=MC_NX||y<0||y>=MC_NY||z<0||z>=MC_NZ)continue;
  int at=x+MC_NX*(y+MC_NY*z);Vec3 p=mc_node(at,front);float d=dot(sub(p,start),sub(p,start));
  if(d<best&&mc_clear(g,start,p)){best=d;root=at;}
 }
 if(root<0)return 0;
 int head=0,tail=0,end=-1;mc_queue[tail++]=root;mc_parent[root]=-1;
 while(head<tail){int at=mc_queue[head++],x=at%MC_NX,y=at/MC_NX%MC_NY,z=at/(MC_NX*MC_NY);Vec3 p=mc_node(at,front);
  if(z==0){end=at;break;}
  int next[6]={x?at-1:-1,x+1<MC_NX?at+1:-1,y?at-MC_NX:-1,y+1<MC_NY?at+MC_NX:-1,z?at-MC_NX*MC_NY:-1,z+1<MC_NZ?at+MC_NX*MC_NY:-1};
  for(int k=0;k<6;k++){int n=next[k];if(n<0||mc_parent[n]!=-2)continue;
   if(mc_clear(g,p,mc_node(n,front))){mc_parent[n]=at;mc_queue[tail++]=n;}
  }
 }
 if(end<0)return 0;
 int count=0;for(int n=end;n>=0;n=mc_parent[n]){if(count>=94)return 0;mc_path[count++]=mc_node(n,front);}
 for(int i=0;i<count/2;i++){Vec3 t=mc_path[i];mc_path[i]=mc_path[count-1-i];mc_path[count-1-i]=t;}
 mc_path[count++]=goal;
 /* Pull a taut line through the lattice; no robotic right-angle flight. */
 int written=0,at=0;Vec3 p=start;
 while(at<count){int far=count-1;while(far>at&&!mc_clear(g,p,mc_path[far]))far--;
  if(!mc_clear(g,p,mc_path[far]))return 0;p=mc_path[far];mc_path[written++]=p;at=far+1;}
 mc_path_n=written;return 1;
}
static int mega_guidance_start(Game *g){
 Vec3 start=station_local(g,g->pos);
 if(!mc_plan(g,start)){message(g,"Guidance needs clearance. Move away from the nearest building, then hail again.");return 0;}
 mc_path[mc_path_n++]=(Vec3){0,0,-station_half_for(g,0)+2};
 g->dock_stage=1;g->dock_phase=10;g->dock_timer=0;g->station_variant=0;g->speed=0;g->boost=0;g->approach=-1;
 message(g,station_class(g)==STATION_MEGA?"Capital traffic control: city route cleared. Guiding you to the docking core.":"Traffic control: approach cleared. Guiding you around the station hull.");campaign_event(g,CP_GUIDANCE);return 1;
}
static void mega_guidance_tick(Game *g,float dt){
 if(mc_system!=g->system||mc_path_at>=mc_path_n){g->dock_stage=0;message(g,"Guidance ended. Please hail traffic control again.");return;}
 float angle=station_angle(g);Vec3 local=mc_path[mc_path_at];
 Vec3 goal={local.x*cosf(angle)-local.y*sinf(angle),local.x*sinf(angle)+local.y*cosf(angle),local.z+STATION_Z},v=sub(goal,g->pos);float d=length(v);
 int final=mc_path_at==mc_path_n-1;
 Vec3 dir=d>.01f?mul(v,1/d):(Vec3){0,0,1};float yaw=atan2f(dir.x,dir.z),delta=atan2f(sinf(yaw-g->yaw),cosf(yaw-g->yaw));
 float ease=fminf(1,dt*3);g->yaw+=delta*ease;g->pitch+=(asinf(fmaxf(-1,fminf(1,dir.y)))-g->pitch)*ease;g->roll+=atan2f(sinf(angle-g->roll),cosf(angle-g->roll))*ease;
 float speed=final?140.f:fminf(950.f,180.f+d*.55f);g->speed=speed;
 Vec3 previous=g->pos;g->pos=add(previous,mul(dir,fminf(d,speed*dt)));
 if(!final){Vec3 hit;if(station_intersection(g,previous,g->pos,&hit)){g->pos=previous;g->dock_stage=0;g->speed=0;message(g,"Guidance paused for an obstruction. Move clear and hail again.");return;}}
 else if(station_collision(g,previous))return;
 if(d<=speed*dt+.1f)mc_path_at++;
}
