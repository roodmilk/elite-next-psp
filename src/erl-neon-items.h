/* Real rotating world geometry; existing pickup positions, cooldowns and
 * item rules remain authoritative. Bounded eight-box frustum budget. */
static Vec3 neon_item_uv(Vec3 a,Vec3 b,Vec3 d,float u,float v){return add(a,add(mul(sub(b,a),u),mul(sub(d,a),v)));}
static void __attribute__((noinline,optimize("Os"))) race_neon_pickups(void){
 static const unsigned char faces[6][4]={{0,1,2,3},{5,4,7,6},{4,0,3,7},{1,5,6,2},{3,2,6,7},{4,5,1,0}};
 static const unsigned palette[4]={RGB(149,243,255),RGB(45,183,255),RGB(23,104,224),RGB(18,49,133)};
 static const unsigned char question[7]={14,17,1,2,4,0,4};
 SpaceRace *r=&game.race;int visible=0;
 for(int i=0;i<RA_PICKUPS&&visible<8;i++){
  RacePickup *p=&r->arcade.pickup[i];if(p->cooldown>0||length(sub(p->pos,neon_eye))>14000||!neon_visible(p->pos,250))continue;
  Vec3 view=neon_camera(p->pos);if(view.z<64)continue;visible++;
  RaceTrackFrame f=race_track_frame(r,p->section,.5f);float phase=r->elapsed*.9f+i*.37f,c=cosf(phase),s=sinf(phase);
  Vec3 right=add(mul(f.right,c),mul(f.ahead,s)),ahead=sub(mul(f.ahead,c),mul(f.right,s)),up=f.up;
  Vec3 points[8];for(int n=0;n<8;n++){float x=(n==1||n==2||n==5||n==6)?150:-150,y=(n==2||n==3||n==6||n==7)?150:-150,z=n>=4?150:-150;points[n]=add(p->pos,add(mul(right,x),add(mul(up,y),mul(ahead,z))));}
  Vec3 normal[6]={mul(ahead,-1),ahead,mul(right,-1),right,up,mul(up,-1)};
  for(int side=0;side<6;side++){
   if(dot(normal[side],sub(neon_eye,p->pos))<=0)continue;
   Vec3 a=points[faces[side][0]],b=points[faces[side][1]],d=points[faces[side][3]];
   for(int band=0;band<4;band++){float lo=band*.25f,hi=lo+.25f;unsigned ink=neon_road_ink(palette[band],side==4?1:(side==1||side==2?.65f:.84f));
    neon_quad(neon_item_uv(a,b,d,0,lo),neon_item_uv(a,b,d,1,lo),neon_item_uv(a,b,d,1,hi),neon_item_uv(a,b,d,0,hi),ink);
   }
   /* Face-space glyphs rotate with the box, and disappear below readable LOD. */
   if(150*proj_focal/view.z<7)continue;
   a=add(a,mul(normal[side],3));b=add(b,mul(normal[side],3));d=add(d,mul(normal[side],3));
   for(int y=0;y<7;y++)for(int x=0;x<5;x++)if(question[y]&(16>>x)){
    float lo=.25f+x*.1f,hi=lo+.1f,bottom=.85f-(y+1)*.1f,top=bottom+.1f;
    neon_quad(neon_item_uv(a,b,d,lo,bottom),neon_item_uv(a,b,d,hi,bottom),neon_item_uv(a,b,d,hi,top),neon_item_uv(a,b,d,lo,top),RGB(255,247,229));
   }
  }
  neon_flush();
  if(view.z<5000){static const unsigned char edges[12][2]={{0,1},{1,2},{2,3},{3,0},{4,5},{5,6},{6,7},{7,4},{0,4},{1,5},{2,6},{3,7}};
   for(int e=0;e<12;e++){NeonStrokeSegment line;if(neon_stroke_camera(neon_camera(points[edges[e][0]]),neon_camera(points[edges[e][1]]),&line)){unsigned glow=RGB(35,174,255);neon_stroke_draw(&line,8,glow,28);neon_stroke_draw(&line,3,RGB(192,246,255),70);}}
  }
 }
}
