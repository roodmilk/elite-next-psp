/* Compiled into the ordinary smoke suite. Generated station exteriors must
 * stay deterministic, varied and safe even when the art profile expands. */
{
 unsigned families=0;int safe_slit=1;float min_radius=10000,max_radius=0,min_half=10000,max_half=0;
 StationProfile first=station_profile_for(&g,0),again=station_profile_for(&g,0);
 CHECK(first.seed==again.seed&&first.radius==again.radius&&first.half==again.half,"Station exterior identity is deterministic");
 for(int sys=0;sys<256;sys++){
  g.system=sys;StationProfile p=station_profile_for(&g,0);families|=1u<<p.family;
  if(p.radius<min_radius)min_radius=p.radius;if(p.radius>max_radius)max_radius=p.radius;
  if(p.half<min_half)min_half=p.half;if(p.half>max_half)max_half=p.half;
  Vec3 a=station_port_corner_for(&g,0,0),b=station_port_corner_for(&g,0,2);
  safe_slit&=a.x==-STATION_PORT_HALF_W&&a.y==-STATION_PORT_HALF_H&&b.x==STATION_PORT_HALF_W&&b.y==STATION_PORT_HALF_H;
 }
 CHECK(safe_slit,"Every system keeps the safe common station flight slit");
 CHECK(families==65535,"All sixteen Rich and eight Poor silhouette families appear across the galaxy");
 CHECK(max_radius/min_radius>2.f&&max_half/min_half>2.f,"Station hulls vary by more than twice their size and depth");
 g.system=7;
}
