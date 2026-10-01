/* Real space is drawn through the hangar mouth, then revealed as it passes
 * the canopy. Masks cover stars/flare outside the opening while still inside. */
static void departure_view(void){
 sector_background();space_fx_nebula();starfield();celestial_rings(0);celestial_rims();draw_bodies();celestial_rings(1);lens_flares();
 float travelled=dot(sub(game.pos,game.dock_from),game.dock_to);
 float distance=station_half_for(&game,game.station_variant)-travelled;
 int top=view_top(),bot=view_bot(),cy=(top+bot)/2;
 if(distance>15){
  int hw=(int)(70*280/distance),hh=(int)(32*280/distance);
  int l=240-hw,r=240+hw,t=cy-hh,b=cy+hh;
  if(l<0)l=0;
  if(r>W)r=W;
  if(t<top)t=top;
  if(b>bot)b=bot;
  rect(0,top,W,t-top,RGB(16,20,27));rect(0,b,W,bot-b,RGB(23,25,30));
  rect(0,t,l,b-t,RGB(20,27,35));rect(r,t,W-r,b-t,RGB(20,27,35));
  unsigned rim=RGB(237,177,83),rail=RGB(63,95,111);
  line(l,t,r,t,rim);line(l,b,r,b,rim);line(l,t,l,b,UI_CYAN);line(r,t,r,b,UI_CYAN);
  line(0,top,l,t,rail);line(W-1,top,r,t,rail);line(0,bot,l,b,rail);line(W-1,bot,r,b,rail);
  for(int i=1;i<=4;i++){float k=i*.2f;int xl=l*(1-k),xr=r+(W-r)*k,yt=t+(top-t)*k,yb=b+(bot-b)*k;
   line(xl,yt,xr,yt,rail);line(xl,yb,xr,yb,rail);
   rect(xl,yt,4,5,UI_CYAN);rect(xr-4,yt,4,5,UI_CYAN);
  }
 }else speed_lines();
 cockpit();
 footer("DEPARTURE GUIDANCE / CONTROLS LOCKED");
 text_px(160,34,UI_CYAN,game.dock_timer<2.1f?"DEPARTURE / PORT TUNNEL":game.dock_timer<3?"DEPARTURE / ACCELERATING":"DEPARTURE / CLEAR OF STATION");
}
