from pathlib import Path
p=Path('src/ship-art-raster.h');s=p.read_text();start=s.index(' for(int y=y0;y<=y1;y++)for(int x=x0;x<=x1;x++){',s.index('if(racer){'));end=s.index('\n }\n}',start)+3
s=s[:start]+''' /* Incremental perspective planes retain the same UV/depth mapping while
  * removing repeated edge calls and redundant per-pixel divisions. */
 float dx0=c.y-b.y,dx1=a.y-c.y,dx2=b.y-a.y;
 float di=dx0*ia+dx1*ib+dx2*ic;
 float ui[3]={ia*uv[0],ib*uv[2],ic*uv[4]},vi[3]={ia*uv[1],ib*uv[3],ic*uv[5]};
 float du=dx0*ui[0]+dx1*ui[1]+dx2*ui[2],dv=dx0*vi[0]+dx1*vi[1]+dx2*vi[2];
 for(int y=y0;y<=y1;y++){
  float e0=edge(b,c,x0+.5f,y+.5f),e1=edge(c,a,x0+.5f,y+.5f),e2=edge(a,b,x0+.5f,y+.5f);
  float inv=e0*ia+e1*ib+e2*ic,uq=e0*ui[0]+e1*ui[1]+e2*ui[2],vq=e0*vi[0]+e1*vi[1]+e2*vi[2];
  for(int x=x0;x<=x1;x++,e0+=dx0,e1+=dx1,e2+=dx2,inv+=di,uq+=du,vq+=dv){
   if(!((e0>=-.05f&&e1>=-.05f&&e2>=-.05f)||(e0<=.05f&&e1<=.05f&&e2<=.05f))||inv<=0)continue;
   if(surface_depth_on){unsigned depth=surface_encode_depth(inv);if(depth>surface_depth[y*W+x])continue;surface_depth[y*W+x]=depth;}
   float reciprocal=1.f/inv;int u=(int)(uq*reciprocal),v=(int)(vq*reciprocal);if(u<0)u=0;if(u>31)u=31;if(v<0)v=0;if(v>31)v=31;
   int at=(v>>mip)*size+(u>>mip);unsigned char packed=(fleet||quad)?fleet_tiles[tile][offset+(at>>1)]:ship_art_pixels[ship_art_panel[face]][at>>1];
   fb[y*STRIDE+x]=palette[(packed>>((at&1)*4))&15];
  }
 }''' +s[end:];p.write_text(s)
p=Path('src/fleet-ge.h');s=p.read_text().replace(' return third_person!=0;',' for(int hub=1;hub<HUB_COUNT;hub++){Vec3 p=hub_position(&game,hub);if(length(sub(p,game.pos))<18000&&render_sphere_visible(p,station_profile_for(&game,hub).radius*2)&&!occluded(p))return 1;}\n return third_person!=0;');p.write_text(s)
p=Path('src/mega-city-render.h');s=p.read_text().replace(' mc_depth_active=!mc_legacy_review',' for(int hub=1;hub<HUB_COUNT&&!fleet;hub++){Vec3 p=hub_position(&game,hub);fleet=length(sub(p,game.pos))<18000&&render_sphere_visible(p,station_profile_for(&game,hub).radius*2);}\n mc_depth_active=!mc_legacy_review');p.write_text(s)
p=Path('src/encounter-upgrade-review.h');s=p.read_text().replace('float a=.30f+sinf(frame*.1f)*.28f;','float orient=slots[scene]<0?-slots[scene]*.47f:(discovery_seed(slots[scene])&255)*.006f;float a=orient+.30f+sinf(frame*.1f)*.12f;');p.write_text(s)
p=Path('src/encounter-custom.h');s=p.read_text().replace('RGB(137,155,166)','RGB(76,104,118)');p.write_text(s)
