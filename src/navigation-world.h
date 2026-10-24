/* Shared geometry/provenance. Coordinates never override live detection. */
int navigation_enabled(const Game*g){return g&&!opening_active(g)&&g->story==STORY_FREE&&(g->tutorial_step==0||g->tutorial_step>TUTORIAL_COUNT);}
Vec3 exploration_discovery_position(const Game*g,int slot){
 unsigned h=((unsigned)g->system+1)*2654435761u+(unsigned)(slot+1)*2246822519u;const Body*b=&g->bodies[1+slot%4];
 if(!body_active(b)||b->type==SUN){int active[10],n=0;for(int i=1;i<BODY_COUNT;i++)if(body_active(&g->bodies[i])&&g->bodies[i].type!=SUN)active[n++]=i;if(n)b=&g->bodies[active[slot%n]];}
 float a=(h&65535)*.0000958738f,r=b->radius+planet_ring_outer(b)+7000+(h>>17)%5000;Vec3 anchor=body_active(b)&&b->type!=SUN?b->pos:hub_position(g,0);if(!body_active(b)||b->type==SUN)r=32000+(h>>17)%23000;Vec3 out=add(anchor,(Vec3){cosf(a)*r,((int)((h>>11)&31)-15)*250.f,sinf(a)*r});
 for(int attempt=0;attempt<16;attempt++){int safe=1;for(int j=0;j<BODY_COUNT;j++)if(body_active(&g->bodies[j])&&length(sub(out,g->bodies[j].pos))<fmaxf(g->bodies[j].radius,g->bodies[j].type==GAS?planet_ring_outer(&g->bodies[j]):0)+2000)safe=0;for(int j=0;j<HUB_COUNT;j++)if(length(sub(out,hub_position(g,j)))<6000)safe=0;if(safe)return out;out.y+=12000;}return out;
}
static Vec3 navigation_safe(const Game*g,Vec3 p,float margin){
 for(int pass=0;pass<12;pass++){int safe=1;for(int b=0;b<BODY_COUNT;b++){float radius=g->bodies[b].radius;if(body_active(&g->bodies[b])&&length(sub(p,g->bodies[b].pos))<radius+margin){p.y+=radius+margin+1500;safe=0;}}if(safe)return p;}return p;
}
Vec3 navigation_position(const Game*g,int key){
 if(key>=33&&key<NAV_KEYS)return g->bodies[key-32].pos;
 if(key<3)return hub_position(g,key<0?0:key);
 if(key<11)return exploration_discovery_position(g,key-3);
 if(key<15)return g->anomaly[key-11].pos;
 unsigned seed=sector_hash((g->system+1)*17011u);float angle=(seed%6283)*.001f;Vec3 hub=hub_position(g,0),outer=add(hub,(Vec3){cosf(angle)*28000,1400,sinf(angle)*28000});
 if(key<21){int b=key-15;Vec3 p=outer;
  if(b==1)p=add(hub,mul(sub(outer,hub),.43f));
  if(b==2)p=add(outer,mul(sub(exploration_discovery_position(g,0),outer),.5f));
  if(b==3)p=add(exploration_discovery_position(g,0),(Vec3){2600,900,-1800});
  if(b==4){if(body_active(&g->bodies[1])&&g->bodies[1].type!=SUN){Vec3 body=g->bodies[1].pos;p=add(body,mul(norm(sub(outer,body)),g->bodies[1].radius+5000));}else p=add(outer,(Vec3){8000,3400,-6000});}
  if(b==5)p=add(hub_position(g,1),(Vec3){2400,1200,-2400});
  return navigation_safe(g,p,3500);
 }
 int region=key-21;Vec3 p;
 if(region==0)p=add(navigation_position(g,NAV_BUOY(2)),(Vec3){3000,600,1800});
 else if(region==1)p=add(navigation_position(g,NAV_BUOY(5)),(Vec3){11000,2800,-3500});
 else if(region==2)p=add(body_active(&g->bodies[3])?g->bodies[3].pos:outer,(Vec3){g->bodies[3].radius+11000,17000,0});
 else if(region==3)p=add(exploration_discovery_position(g,1),(Vec3){6500,1200,1500});
 else if(region==4)p=add(hub,(Vec3){-52000,14000,-21000});
 else p=add(body_active(&g->bodies[4])?g->bodies[4].pos:outer,(Vec3){9000,-18000,12000});
 return navigation_safe(g,p,12000);
}
int navigation_source(const Game*g,int key){
 if(key<0||key>=NAV_KEYS)return 0;if(key>=33){int b=key-32;return body_active(&g->bodies[b])?NAV_CHART:0;}if(key>=NAV_FIXED_KEYS)return 0;
 unsigned bit=1u<<key;int source=0;if(g->navigation.visited[g->system]&bit)source|=NAV_VISITED;if(g->navigation.supplied[g->system]&bit)source|=NAV_BUOY_SOURCE;
 if(key<3&&is_mission_target(g,STATION_TARGET_ID(key)))source|=NAV_MISSION;
 if(key==0&&g->campaign_stage>=6&&g->saga_chapter<SAGA_COUNT&&g->saga_step>0&&g->saga_dest==g->system){int kind=saga_beats[g->saga_chapter].kind;if(kind==SAGA_DOCK||kind==SAGA_HOME||kind==SAGA_CHOICE||saga_ready(g))source|=NAV_MISSION;}
 /* Paid searches are deliberately not exact destination knowledge. */
 return source;
}
int navigation_select(Game*g,int key){if(!navigation_source(g,key))return 0;g->navigation.waypoint_system=(uint16_t)g->system;g->navigation.waypoint_key=(uint16_t)(key+1);return 1;}
void navigation_visit(Game*g,int key){if(key>=0&&key<NAV_FIXED_KEYS)g->navigation.visited[g->system]|=1u<<key;}
int navigation_links(const Game*g,int buoy,int*keys,int capacity){
 if(buoy<0||buoy>=NAV_BUOYS||!(g->navigation.scanned[g->system]&(1u<<buoy)))return 0;
 static const int links[6][4]={{16,17,33,-1},{15,0,20,-1},{15,18,-1,-1},{17,3,4,-1},{15,33,-1,-1},{16,1,-1,-1}};
 int n=0;for(int i=0;i<4&&n<capacity;i++){int k=links[buoy][i];if(k<0||(k>=33&&!navigation_source(g,k)))continue;/* A buoy contains only its own authored local records. */keys[n++]=k;}return n;
}
int navigation_buoy_scan(Game*g,int buoy){
 if(!navigation_enabled(g)||buoy<0||buoy>=NAV_BUOYS||g->planet>=0||g->docked||length(sub(g->pos,navigation_position(g,NAV_BUOY(buoy))))>700||g->speed>120)return 0;
 navigation_visit(g,NAV_BUOY(buoy));g->navigation.scanned[g->system]|=1u<<buoy;int keys[4],n=navigation_links(g,buoy,keys,4);for(int i=0;i<n;i++)if(keys[i]<NAV_FIXED_KEYS)g->navigation.supplied[g->system]|=1u<<keys[i];g->cue=SFX_SCAN;return n;
}
int navigation_rumour_buy(Game*g,int offer){
 if(!navigation_enabled(g)||offer<0||offer>2)return -1;static const int buoys[3]={0,2,4};int cost=(25+offer*10)*10,owned=((g->navigation.paid[g->system]>>offer)&1)||((g->navigation.scanned[g->system]>>buoys[offer])&1);
 if(!owned){if(g->credits<cost)return -2;g->credits-=cost;g->navigation.paid[g->system]|=1u<<offer;}
 g->cue=SFX_SELECT;return owned?2:1;
}
Vec3 navigation_search_position(const Game*g){int offer=g->navigation.search_key?g->navigation.search_key-1:0;static const int buoys[3]={0,2,4};return navigation_safe(g,add(navigation_position(g,NAV_BUOY(buoys[offer%3])),(Vec3){700,450,-700}),1500);}
int navigation_planet_job(Game*g){
 if(!navigation_enabled(g)||g->planet<1||g->surface!=1||g->bodies[g->planet].type!=OCEAN)return 0;
 for(int i=0;i<g->job_n;i++)if(g->jobs[i].type==MISSION_DELIVERY&&g->jobs[i].dest==g->system)return 2;
 if(g->job_n>=MISSION_SLOTS||cargo_used(g)>=cargo_capacity(g)){message(g,"A free contract and one tonne of cargo space are required.");return -1;}
 Job*j=&g->jobs[g->job_n++];*j=(Job){g->system,MISSION_DELIVERY,0,-1,1,g->system,650,1800};g->cargo[0]++;g->job_sel=g->job_n-1;jobs_sync(g);message(g,"Local courier accepted. Station coordinates supplied.");return 1;
}
int spatial_region_kind(const Game*g,int region){(void)g;return region;}
float spatial_region_radius(int region){static const float radii[6]={12500,10000,14500,8500,18000,11500};return radii[region%6];}
float spatial_density(const Game*g,int region,Vec3 p){
 if(!navigation_enabled(g)||region<0||region>=6)return 0;Vec3 d=sub(p,navigation_position(g,NAV_REGION(region)));float r=spatial_region_radius(region);float q=sqrtf(d.x*d.x+d.y*d.y*1.8f+d.z*d.z)/r;float v=fmaxf(0,fminf(1,(1-q)/.55f));v=v*v*(3-2*v);
 if(region==1){float age=fmodf(g->world_clock+g->system*120.f,1080.f);float weather=fmaxf(0,fminf(1,(age-240)/12))*fmaxf(0,fminf(1,(1080-age)/12));v*=weather*(.75f+.25f*sinf(g->world_clock*.015f));}
 return v;
}
float spatial_engine_factor(const Game*g){if(!navigation_enabled(g)||g->planet>=0||g->docked||race_is_active(g))return 1;return 1-.25f*spatial_density(g,1,g->pos);}
void navigation_tick(Game*g,float dt){
 (void)dt;if(!navigation_enabled(g)||g->dead)return;
 if(g->docked){navigation_visit(g,g->station_variant);return;}if(g->planet>=0||g->jump>0||race_is_active(g))return;
 for(int k=0;k<21;k++){if(k>=11&&k<15&&!g->anomaly[k-11].alive)continue;float reach=k<3?1600:k<11?(k==4?3600:k==3?1800:2400):k<15?600:450;if(length(sub(g->pos,navigation_position(g,k)))<reach)navigation_visit(g,k);}
 int near=-1;float distance=701;
 for(int b=0;b<NAV_BUOYS;b++){float d=length(sub(g->pos,navigation_position(g,NAV_BUOY(b))));if(d<distance){distance=d;near=b;}}
 if(near>=0&&g->speed<=120&&!(g->navigation.scanned[g->system]&(1u<<near))){
  if(g->buoy_scan_slot!=near){g->buoy_scan_slot=near;g->buoy_scan_time=0;}
  g->buoy_scan_time+=dt;if(g->buoy_scan_time>=2){int n=navigation_buoy_scan(g,near);if(n){message(g,"Buoy read. Hold Square / NAV LOG to choose a local lead.");g->buoy_scan_time=0;}}
 }else{g->buoy_scan_slot=-1;g->buoy_scan_time=0;}
 int region=-1;float density=0;for(int r=0;r<6;r++){float d=spatial_density(g,r,g->pos);if(d>density){density=d;region=r;}if(d>.25f)navigation_visit(g,NAV_REGION(r));}
 if(region!=g->spatial_region&&density>.08f){if(g->voice_time<=0&&g->message_time<=0)message(g,region==1?"Magnetic interference. Engine output reduced.":region==0||region==5?"Cloud ahead. Visibility falling.":"Spatial field ahead. Watch your instruments.");g->spatial_region=region;}
 else if(density<.025f&&g->spatial_region>=0){if(g->voice_time<=0&&g->message_time<=0)message(g,"Field strength falling. Systems recovering.");g->spatial_region=-1;}g->spatial_level=density;
 /* Mission-only waypoints expire; independent and visited providers survive. */
 if(g->navigation.waypoint_key&&g->navigation.waypoint_system==g->system&&!navigation_source(g,g->navigation.waypoint_key-1))g->navigation.waypoint_key=0;
}

/* Arrival is cleared against the NEW system after game_spawn builds its bodies.
 * Keep the separately seeded heading; never aim the player at civilisation. */
void navigation_arrival_clearance(Game*g){
 if(!navigation_enabled(g))return;
 for(int pass=0;pass<12;pass++){int clear=1;
  for(int b=0;b<BODY_COUNT;b++)if(body_active(&g->bodies[b])){float radius=g->bodies[b].type==GAS?planet_ring_outer(&g->bodies[b]):g->bodies[b].radius;if(length(sub(g->pos,g->bodies[b].pos))<radius+5000){g->pos.y+=radius+6500;clear=0;}}
  for(int h=0;h<HUB_COUNT;h++)if(length(sub(g->pos,hub_position(g,h)))<18000){g->pos.y+=19000;clear=0;}
  for(int d=0;d<8;d++)if(length(sub(g->pos,exploration_discovery_position(g,d)))<5000){g->pos.y+=6000;clear=0;}
  if(clear)return;
 }
}
