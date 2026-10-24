/* Decorative holograms outside the legal driving volume. No targets,
 * collisions, rewards, RNG, allocations or asset loads. Shared camera path. */
static void race_ad_copy(int x,int y,const char *s,unsigned ink,unsigned depth){
 for(int n=0;s[n];n++){unsigned ch=(unsigned char)s[n];if(ch<32||ch>127)continue;for(int row=0;row<8;row++)for(int col=0;col<8;col++)if(font8[ch-32][row]&(128>>col)){
  int px=x+n*8+col,py=y+row;if(px>=clipx0&&px<clipx1&&py>=surface_y_min&&py<=surface_y_max&&py>=0&&py<H&&px>=0&&px<W&&depth<=surface_depth[py*W+px]+8)pixel(px,py,ink);
 }}
}
static void race_dressing_art(void){
 SpaceRace *r=&game.race;if(!race_ready(&game))return;
 int first=r->gate>0?r->gate-1:0;
 for(int k=first;k<RACE_GATES&&k<first+4;k++){
  const RaceGate *q=&r->gates[k];Vec3 prev=k?r->gates[k-1].pos:r->start,mid=mul(add(prev,q->pos),.5f);
  if(length(sub(mid,camera_origin(&game)))>19000)continue;
  /* Spectators are scenery only. Local geometry never enters a pilot, actor,
   * target or collision pool; their slow orbit freezes with race time. */
  if(k%8==3){
   float angle=r->elapsed*.055f+(r->seed&31)*.19f;
   Vec3 crowd=add(mid,add(mul(q->right,12400+cosf(angle)*700),add(mul(q->up,3500+sinf(angle)*650),mul(q->normal,-800))));
   for(int ship=0;ship<2;ship++){
    Vec3 c=add(crowd,add(mul(q->normal,ship*1700),mul(q->up,ship*550)));
    Vec3 nose=add(c,mul(q->normal,430)),tail=add(c,mul(q->normal,-270));
    Vec3 left=add(tail,mul(q->right,-360)),right=add(tail,mul(q->right,360)),top=add(c,mul(q->up,130));
    queue_triangle(camera(&game,nose),camera(&game,left),camera(&game,top),RGB(60,79,94));
    queue_triangle(camera(&game,nose),camera(&game,top),camera(&game,right),RGB(94,113,126));
    queue_triangle(camera(&game,left),camera(&game,right),camera(&game,top),RGB(46,61,75));
    race_boundary_line(tail,add(tail,mul(q->normal,-150)),RGB(102,223,225));
   }
   flush_solid_meshes();
  }
  if(k%4==1){
   Vec3 centre=add(mid,add(mul(q->right,k&4?10500:-10500),mul(q->up,2400))),v=camera(&game,centre);
   if(v.z<150)continue;Point p=project(v);int width=fminf(104,2600*proj_focal/v.z),height=fminf(32,800*proj_focal/v.z);
   if(width<24||height<8||p.x-width/2<clipx0||p.x+width/2>=clipx1||p.y-height/2<surface_y_min||p.y+height/2>surface_y_max)continue;
   unsigned depth=surface_encode_depth(1.f/v.z),ink=RGB(235,78,187),accent=RGB(106,240,219);int left=p.x-width/2,top=p.y-height/2;
   for(int side=-1;side<=1;side+=2){race_boundary_line(add(centre,add(mul(q->right,-1300),mul(q->up,side*400))),add(centre,add(mul(q->right,1300),mul(q->up,side*400))),ink);}
   if(width>=80&&height>=22){static const char *brands[]={"BOVVA","ZAP!","THARG","ERL"},*copy[]={"MOOOVE!","NO NAPS","CUP GO","FULL SEND"};int brand=(r->seed+k/4)%4;race_ad_copy(left+4,top+3,brands[brand],accent,depth);race_ad_copy(left+4,top+14,copy[brand],RGB(255,221,174),depth);
    /* Little animated sponsor checkerboard; all changes follow race time. */
    for(int y=0;y<2;y++)for(int x=0;x<2;x++)if((x+y+(int)(r->elapsed*2))&1)for(int py=0;py<3;py++)for(int px=0;px<3;px++){int sx=left+width-11+x*3+px,sy=top+3+y*3+py;if(depth<=surface_depth[sy*W+sx]+8)pixel(sx,sy,ink);}
   }
  }
  /* Sparse dust/ice/light veils remain beyond the course edge, never a wall. */
  if(k%8==0){int theme=(r->seed>>4)%3;unsigned ink=theme==0?RGB(77,58,60):theme==1?RGB(45,77,96):RGB(73,43,94);
   for(int ribbon=0;ribbon<2;ribbon++){Vec3 old={0};for(int knot=0;knot<4;knot++){float drift=sinf(r->elapsed*.15f+knot*.9f+ribbon)*250;Vec3 at=add(mid,add(mul(q->right,(ribbon?1:-1)*(12000+knot*400)),add(mul(q->up,2200+drift),mul(q->normal,(knot-1.5f)*1700))));if(knot)race_boundary_line(old,at,ink);old=at;}}
  }
 }
}
