/* An analytic footprint scales to the expanded survey radius without a huge
 * PSP-side bitmap. Renderer, walker, rover and fauna still agree on void. */
int surface_cloud_deck(const Game *g,float x,float z){
 if(g->planet<1||g->planet>=BODY_COUNT||g->bodies[g->planet].type!=GAS)return 0;
 Vec3 pad=surface_site(g,1);float px=(floorf((x-pad.x)/40)+.5f)*40,pz=(floorf((z-pad.z)/40)+.5f)*40;
 if(fabsf(px)<FIELD_PORT_CLEAR&&fabsf(pz)<FIELD_PORT_CLEAR)return 1;
 for(int i=1;i<10;i++)if(i!=6){
  Vec3 site=sub(surface_poi(g,i),pad);FieldBuilding bounds=field_site_building(g,i);
  if(fabsf(px-site.x)<bounds.w+85&&fabsf(pz-site.z)<bounds.d+85)return 1;
  if(px>=fminf(0,site.x)-60&&px<=fmaxf(0,site.x)+60&&fabsf(pz)<60)return 1;
  if(fabsf(px-site.x)<60&&pz>=fminf(0,site.z)-60&&pz<=fmaxf(0,site.z)+60)return 1;
 }
 return 0;
}
