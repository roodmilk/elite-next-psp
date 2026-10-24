/* Native printed illustration kit. Coordinates are local to each exact art
 * aperture; plot is clipped before touching the framebuffer. No enlarged
 * source bitmap, animation, or mutable world-state input is used here. */
typedef struct {int x,y,w,h;unsigned paper,ink,shade,spot,rule;} NewsCanvas;
static unsigned news_hash(unsigned h){h^=h>>16;h*=0x7feb352du;h^=h>>15;h*=0x846ca68bu;return h^(h>>16);}
static void news_dot(const NewsCanvas *a,int x,int y,unsigned c){if(x>=0&&y>=0&&x<a->w&&y<a->h)pixel(a->x+x,a->y+y,c);}
static void news_box(const NewsCanvas *a,int x,int y,int w,int h,unsigned c){int x0=x<0?0:x,y0=y<0?0:y,x1=x+w>a->w?a->w:x+w,y1=y+h>a->h?a->h:y+h;if(x1>x0&&y1>y0)rect(a->x+x0,a->y+y0,x1-x0,y1-y0,c);}
static void news_stroke(const NewsCanvas *a,int x,int y,int xx,int yy,unsigned c){int dx=abs(xx-x),sx=x<xx?1:-1,dy=-abs(yy-y),sy=y<yy?1:-1,e=dx+dy;for(;;){news_dot(a,x,y,c);if(x==xx&&y==yy)break;int k=e*2;if(k>=dy){e+=dy;x+=sx;}if(k<=dx){e+=dx;y+=sy;}}}
static void news_disc(const NewsCanvas *a,int cx,int cy,int r,unsigned c){for(int y=-r;y<=r;y++)for(int x=-r;x<=r;x++)if(x*x+y*y<=r*r)news_dot(a,cx+x,cy+y,c);}
static void news_person(const NewsCanvas *a,int x,int y,unsigned coat,int seated){
 news_box(a,x+2,y,5,2,a->ink);news_box(a,x+1,y+2,7,5,a->paper);news_dot(a,x+6,y+3,a->ink);
 news_box(a,x+2,y+7,5,2,a->shade);news_box(a,x,y+9,9,10,coat);news_box(a,x-1,y+11,2,9,coat);news_box(a,x+8,y+11,2,8,coat);
 news_box(a,x+1,y+19,3,seated?5:10,a->ink);news_box(a,x+5,y+19,3,seated?5:10,a->ink);
 if(seated){news_box(a,x+2,y+22,10,3,a->ink);news_box(a,x+10,y+22,3,7,a->ink);}else{news_box(a,x,y+28,4,2,a->paper);news_box(a,x+5,y+28,5,2,a->paper);}
}
static void news_crate(const NewsCanvas *a,int x,int y,int w,int h){news_box(a,x+2,y,w,h,a->ink);news_box(a,x,y+2,w-2,h-2,a->paper);news_stroke(a,x,y+2,x+w-2,y+h-1,a->rule);news_box(a,x+3,y+5,w-8,2,a->spot);news_box(a,x+3,y+h-5,w-8,1,a->shade);}
static void news_hull(const NewsCanvas *a,int x,int y,int r){
 /* Cobra Mk III: broad triangular body, split engine deck and small bridge. */
 for(int yy=0;yy<=r/2;yy++)news_box(a,x-yy*2,y+yy,yy*4+1,1,yy<r/4?a->paper:a->rule);
 news_stroke(a,x,y,x-r,y+r/2,a->paper);news_stroke(a,x,y,x+r,y+r/2,a->paper);
 news_stroke(a,x,y+4,x-r/2,y+r/2,a->shade);news_stroke(a,x,y+4,x+r/2,y+r/2,a->shade);
 news_box(a,x-4,y+5,9,3,a->spot);news_box(a,x-r+3,y+r/2,8,3,a->ink);news_box(a,x+r-10,y+r/2,8,3,a->ink);
 news_box(a,x-r+4,y+r/2+1,6,1,a->spot);news_box(a,x+r-9,y+r/2+1,6,1,a->spot);
}
static void news_starfield(const NewsCanvas *a,unsigned seed){
 for(int k=0;k<a->w*a->h/190;k++){seed=seed*1664525u+1013904223u;int x=seed%(unsigned)a->w;seed=seed*1664525u+1013904223u;int y=seed%(unsigned)a->h;news_dot(a,x,y,k%7?a->rule:a->paper);if(k%13==0){news_dot(a,x-1,y,a->rule);news_dot(a,x+1,y,a->rule);}}
}
static void news_station(const NewsCanvas *a,int s){
 int cx=a->w/2,cy=a->h/2,r=(a->h<a->w?a->h:a->w)/3;if(r>38)r=38;
 int mega=s!=7&&station_profile_hash(s,0)%17u==0,poor=newspaper_background_economy[s]==2||newspaper_background_economy[s]==6;
 if(mega){news_box(a,cx-r,cy-9,2*r,18,a->rule);news_box(a,cx-10,cy-r,20,2*r,a->rule);news_box(a,cx-7,cy-r,14,2*r,a->paper);for(int k=-1;k<=1;k+=2){news_box(a,cx+k*(r-5)-4,cy-r/2,8,r,a->paper);news_box(a,cx+k*(r-5)-3,cy-r/2+3,6,2,a->spot);}}
 else if(poor){news_box(a,cx-r,cy-8,2*r,16,a->rule);news_box(a,cx-8,cy-16,16,32,a->paper);news_box(a,cx-r+2,cy-6,2*r-4,3,a->paper);for(int k=-1;k<=1;k+=2)news_box(a,cx+k*(r-4)-2,cy-17,4,34,a->shade);}
 else {for(int yy=-r;yy<=r;yy++){int inset=abs(yy)/2;news_box(a,cx-r+inset,cy+yy,2*(r-inset),1,yy<0?a->paper:a->rule);}news_stroke(a,cx-r/2,cy-r,cx,cy,a->shade);news_stroke(a,cx+r/2,cy-r,cx,cy,a->shade);news_stroke(a,cx,cy,cx-r/2,cy+r,a->shade);news_stroke(a,cx,cy,cx+r/2,cy+r,a->shade);}
 news_box(a,cx-7,cy-4,14,8,a->spot);news_box(a,cx-5,cy-2,10,4,a->ink);
 for(int k=0;k<5;k++){news_box(a,cx-r+7+k*6,cy+8,2,1,a->spot);news_box(a,cx-2,cy-r+6+k*6,2,1,a->spot);}
 if(a->w>130&&a->h>55){news_hull(a,a->w-25,a->h-22,12);news_stroke(a,9,a->h-12,32,a->h-21,a->rule);news_disc(a,8,a->h-12,2,a->spot);}
}
static void news_miniature(const NewsCanvas *a,int art,int system){
 int x=a->w/2,y=a->h/2;
 /* Each strip gets an actual native miniature of its captioned subject. It
  * must not silently substitute an unrelated crate or generic placeholder. */
 switch(art){
 case 0:for(int k=0;k<3;k++){news_disc(a,x-21+k*21,y,5,k?a->paper:a->spot);if(k==2)news_stroke(a,x+14,y+3,x+29,y-3,a->rule);}break;
 case 1:news_box(a,x-12,y-7,24,14,a->rule);news_box(a,x-7,y-9,14,18,a->paper);news_box(a,x-4,y-2,8,4,a->ink);news_box(a,x-11,y+3,3,1,a->spot);news_box(a,x+8,y+3,3,1,a->spot);break;
 case 2:news_hull(a,x,y-5,17);break;
 case 3:news_box(a,x-4,y-7,8,2,a->rule);news_box(a,x-5,y-5,10,8,a->paper);news_dot(a,x-2,y-2,a->ink);news_dot(a,x+2,y-2,a->ink);news_box(a,x-2,y+2,4,1,a->spot);news_box(a,x-9,y+4,18,5,a->rule);break;
 case 4:news_disc(a,x,y,8,a->paper);news_disc(a,x,y,5,a->ink);news_box(a,x-1,y-4,3,9,a->spot);news_box(a,x-4,y-1,9,3,a->spot);break;
 case 5:for(int k=0;k<4;k++)news_crate(a,x-39+k*21,3+(k%2)*2,18,a->h-7);break;
 case 6:news_box(a,x-9,y-9,18,18,a->paper);for(int k=0;k<4;k++)news_box(a,x-6,y-6+k*3,k==2?8:12,1,a->ink);news_disc(a,x+6,y+6,2,a->spot);break;
 case 7:news_box(a,x-29,y+6,58,2,a->paper);for(int k=0;k<4;k++){news_box(a,x-22+k*13,y-5,5,11,a->rule);news_box(a,x-21+k*13,y-8,3,4,a->spot);}break;
 case 8:for(int k=0;k<60;k++){float t=k*6.28318f/60;news_dot(a,x+(int)(cosf(t)*30),y+(int)(sinf(t)*7),a->paper);news_dot(a,x+(int)(cosf(t)*24),y+(int)(sinf(t)*4),a->rule);}news_hull(a,x+3,y-3,8);break;
 case 9:news_stroke(a,x-26,y-4,x,y+4,a->rule);news_stroke(a,x,y+4,x+26,y-4,a->rule);for(int k=0;k<3;k++)news_disc(a,x-26+k*26,y+(k==1?4:-4),3,a->paper);break;
 case 10:news_stroke(a,x-9,y+7,x+7,y-7,a->paper);news_stroke(a,x-8,y+8,x+8,y-6,a->paper);news_box(a,x+4,y-9,3,5,a->paper);news_box(a,x+9,y-5,4,3,a->paper);news_disc(a,x-9,y+7,2,a->spot);break;
 default:news_disc(a,x-12,y-4,4,a->paper);news_box(a,x-15,y+1,7,7,a->spot);news_box(a,x+8,y-7,11,9,a->rule);news_dot(a,x+10,y-3,a->paper);news_dot(a,x+16,y-3,a->paper);news_box(a,x+9,y+3,3,5,a->spot);news_box(a,x+15,y+3,3,5,a->spot);break;
 }
 (void)system;
}
static void newspaper_art(const NewspaperBlock *b,int system,const unsigned *c){
 NewsCanvas a={b->x+4,b->y+4,b->w-8,b->h-32,c[0],c[1],c[2],c[4],c[3]};int cx=a.w/2,cy=a.h/2;
 news_box(&a,0,0,a.w,a.h,a.ink);int art=b->art%12;
 if(a.h<32){news_miniature(&a,art,system);return;}
 switch(art){
 case 0:{ /* Canonical body types/colours/seeds, printed with controlled halftone. */
  news_starfield(&a,body_art_seed(system,0));int slots=system_body_slots(system),body=slots>1?1:0;unsigned col,acc;int type;body_tint(system,body,&col,&acc,&type);unsigned seed=body_art_seed(system,body);int r=(a.w<a.h?a.w:a.h)/3;if(r>43)r=43;
  for(int yy=-r;yy<=r;yy++)for(int xx=-r;xx<=r;xx++){int rr=xx*xx+yy*yy;if(rr>r*r)continue;unsigned h=news_hash(seed+(xx+64)*197u+(yy+64)*383u);unsigned ink=type==SUN?col:type==GAS?((yy/4+(int)(seed%7))%3?col:acc):((news_hash(seed+(xx+64)/5*197u+(yy+64)/4*383u)%11)<5?col:acc);if(xx+yy>r/2)ink=((xx+yy)&1)?a.shade:a.ink;else if((h&31)==0)ink=a.paper;news_dot(&a,cx+xx,cy+yy,ink);}
  if(type==GAS){for(int k=0;k<90;k++){float t=k*6.28318f/90;int xx=(int)(cosf(t)*r*1.35f),yy=(int)(sinf(t)*r*.31f)-xx/5;if(yy>0||xx*xx+yy*yy>r*r)news_dot(&a,cx+xx,cy+yy,a.rule);}}
  news_stroke(&a,6,6,18,6,a.paper);news_stroke(&a,6,6,6,18,a.paper);news_stroke(&a,a.w-7,a.h-7,a.w-19,a.h-7,a.paper);break;}
 case 1:news_starfield(&a,system+919);news_station(&a,system);break;
 case 2:{news_starfield(&a,system+449);int r=a.w/3;if(r>a.h)r=a.h;if(r>62)r=62;news_hull(&a,cx,cy-r/5,r);news_stroke(&a,cx-r-3,cy+r/2+8,cx+r+3,cy+r/2+8,a.rule);break;}
 case 3: /* Portrait photo with a practical workshop background, native face. */
  for(int k=0;k<5;k++){news_box(&a,4+k*13,4,6,a.h-8,a.shade);news_box(&a,5+k*13,7,2,7,a.rule);}if(a.w>=48){int size=a.h>=50?40:32;draw_portrait(a.x+cx-size/2,a.y+cy-size/2,size,size,system*17+newspaper_identity[system].publication_family,TRADERS);}break;
 case 4:{/* Raised public seal, radial sun and engraved register. */
  int r=(a.h<a.w?a.h:a.w)/3;news_disc(&a,cx,cy+2,r,a.shade);news_disc(&a,cx,cy,r,a.paper);news_disc(&a,cx,cy,r-3,a.ink);news_disc(&a,cx,cy,5,a.spot);for(int k=0;k<12;k++){float t=k*.523599f;news_stroke(&a,cx+(int)(cosf(t)*9),cy+(int)(sinf(t)*9),cx+(int)(cosf(t)*(r-7)),cy+(int)(sinf(t)*(r-7)),a.paper);}news_box(&a,cx-r,cy+r+5,r*2,2,a.rule);break;}
 case 5: /* A freight photograph: floor perspective, crates and a dock worker. */
  news_box(&a,0,a.h*2/3,a.w,a.h/3,a.shade);for(int k=-2;k<=2;k++)news_stroke(&a,cx+k*13,a.h*2/3,cx+k*34,a.h-1,a.rule);
  news_crate(&a,cx-33,cy-3,29,25);news_crate(&a,cx-11,cy-16,25,24);news_crate(&a,cx+8,cy+1,25,22);if(a.h>70)news_person(&a,cx+35,cy-1,a.spot,0);break;
 case 6: /* Crime desk: pinned evidence, a missing spoon and an exhibit tag. */
  news_box(&a,5,5,a.w-10,a.h-10,a.shade);news_box(&a,cx-27,cy-22,51,44,a.rule);news_box(&a,cx-23,cy-24,47,42,a.paper);news_box(&a,cx-19,cy-18,29,2,a.ink);for(int k=0;k<4;k++)news_box(&a,cx-19,cy-11+k*4,20+(k%2)*12,1,a.shade);news_box(&a,cx-19,cy-6,27,3,a.ink);news_disc(&a,cx+12,cy+7,4,a.ink);news_stroke(&a,cx+9,cy+10,cx-1,cy+21,a.ink);news_disc(&a,cx,cy-24,2,a.spot);break;
 case 7: /* Bar photo: counter, bottle silhouettes, lamp and late-shift visitor. */
  news_box(&a,0,0,a.w,a.h,a.shade);
  if(a.h>65){int wx=a.w*3/5,wy=9,ww=a.w-wx-7,wh=a.h/2;news_box(&a,wx-2,wy-2,ww+4,wh+4,a.rule);news_box(&a,wx,wy,ww,wh,a.ink);for(int k=0;k<10;k++)news_dot(&a,wx+3+k*11%(ww-6),wy+4+k*7%(wh-8),a.paper);news_disc(&a,wx+ww*2/3,wy+wh/2,wh/3,a.rule);news_box(&a,wx+ww/2,wy,2,wh,a.shade);news_box(&a,wx,wy+wh-8,ww,2,a.shade);}
  int shelf_w=a.h>65?a.w*3/5-12:a.w-12;news_box(&a,6,6,shelf_w,26,a.ink);for(int k=0;k<6;k++){int xx=10+k*(shelf_w-8)/6;news_box(&a,xx,14,5,13,a.rule);news_box(&a,xx+1,10,3,5,a.spot);news_box(&a,xx,21,5,3,a.paper);}news_box(&a,5,30,shelf_w+2,2,a.rule);
  int counter=a.h>65?a.h*2/3:a.h-18,lamps=a.h>65?3:1;for(int k=0;k<lamps;k++){int lx=lamps==1?cx:24+k*(a.w-45)/2;news_stroke(&a,lx,0,lx,12,a.paper);for(int yy=0;yy<5;yy++)news_box(&a,lx-yy*2,12+yy,yy*4+1,1,a.spot);}
  if(a.h>65){news_person(&a,22,counter-29,a.spot,0);news_person(&a,a.w*3/4,counter-19,a.rule,1);for(int k=0;k<4;k++){int xx=24+k*(a.w-38)/4;news_box(&a,xx,counter+9,15,4,a.ink);news_box(&a,xx+6,counter+13,3,a.h-counter-15,a.ink);}news_box(&a,4,a.h-3,a.w-8,1,a.rule);}
  news_box(&a,0,counter,a.w,6,a.paper);news_box(&a,0,counter+6,a.w,5,a.rule);news_box(&a,cx-14,counter-9,5,9,a.paper);news_box(&a,cx-12,counter-11,2,2,a.spot);news_box(&a,cx+7,counter-6,7,6,a.spot);news_box(&a,cx+8,counter-6,5,1,a.paper);break;
 case 8: /* Racing circuit: double rail, start line and two small racing hulls. */
  for(int k=0;k<100;k++){float t=k*6.28318f/100;int xx=(int)(cosf(t)*(a.w/2-12)),yy=(int)(sinf(t)*(a.h/2-10));news_dot(&a,cx+xx,cy+yy,a.paper);news_dot(&a,cx+(int)(xx*.8f),cy+(int)(yy*.67f),a.rule);}news_box(&a,cx-2,6,4,9,a.spot);news_hull(&a,cx+14,cy-4,12);news_hull(&a,cx-21,cy+7,8);break;
 case 9: /* Travel diagram, carefully labelled with compact native markers. */
  for(int k=0;k<4;k++){int xx=12+k*(a.w-24)/3,yy=cy+(k%2?9:-9);if(k)news_stroke(&a,12+(k-1)*(a.w-24)/3,cy+((k-1)%2?9:-9),xx,yy,a.rule);news_disc(&a,xx,yy,5,a.paper);news_disc(&a,xx,yy,2,a.spot);news_stroke(&a,xx-4,yy+9,xx+4,yy+9,a.shade);}break;
 case 10: /* Repair bench: tools, open machine and one very necessary fastener. */
  news_box(&a,4,4,a.w-8,a.h-8,a.shade);news_box(&a,7,a.h-18,a.w-14,5,a.paper);news_box(&a,cx-20,cy-13,40,26,a.ink);news_box(&a,cx-16,cy-9,18,18,a.rule);news_disc(&a,cx-7,cy,6,a.paper);news_disc(&a,cx-7,cy,2,a.ink);for(int k=0;k<3;k++)news_box(&a,cx+6,cy-6+k*5,10,2,a.spot);news_stroke(&a,cx+24,cy-15,cx+30,cy-9,a.paper);news_stroke(&a,cx+25,cy-10,cx+33,cy-18,a.paper);news_box(&a,cx-26,cy-17,2,9,a.spot);news_box(&a,cx-28,cy-18,6,2,a.paper);break;
 default: /* Nip is the suited courier; Tug is the square-bodied cargo robot. */
  news_box(&a,3,3,a.w-6,a.h-6,a.paper);news_box(&a,5,5,a.w-10,a.h-10,a.ink);news_stroke(&a,9,a.h-9,a.w-10,a.h-9,a.rule);news_person(&a,cx-24,cy-7,a.spot,0);
  news_box(&a,cx+7,cy-5,18,17,a.rule);news_box(&a,cx+9,cy-3,14,8,a.paper);news_box(&a,cx+11,cy,3,2,a.ink);news_box(&a,cx+18,cy,3,2,a.ink);news_box(&a,cx+10,cy+14,4,7,a.spot);news_box(&a,cx+19,cy+14,4,7,a.spot);news_box(&a,cx+4,cy+3,3,9,a.spot);news_disc(&a,cx+16,cy-8,2,a.spot);break;
 }
}
