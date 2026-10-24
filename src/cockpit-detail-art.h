/* Native-resolution material and instrument detail from the approved cockpit.
 * Drawn into the cached cockpit; all moving data remains owned by the game. */
typedef struct{short x,y;} HcPoint;
static void hc_poly(const HcPoint*p,int n,unsigned ink){
 int top=H-1,bottom=0;for(int k=0;k<n;k++){if(p[k].y<top)top=p[k].y;if(p[k].y>bottom)bottom=p[k].y;}
 if(top<0)top=0;if(bottom>=H)bottom=H-1;
 for(int y=top;y<=bottom;y++){int hit[12],count=0;
  for(int k=0;k<n;k++){HcPoint a=p[k],b=p[(k+1)%n];if((a.y<=y&&b.y>y)||(b.y<=y&&a.y>y)){int x=a.x+(y-a.y)*(b.x-a.x)/(b.y-a.y),j=count++;while(j&&hit[j-1]>x){hit[j]=hit[j-1];j--;}hit[j]=x;}}
  for(int k=0;k+1<count;k+=2)rect(hit[k],y,hit[k+1]-hit[k]+1,1,ink);
 }
}
static void hc_edge_poly(const HcPoint*p,int n,unsigned fill,unsigned edge){hc_poly(p,n,fill);for(int k=0;k<n;k++)line(p[k].x,p[k].y,p[(k+1)%n].x,p[(k+1)%n].y,edge);}
static void hc_segment_meter(int x,int y,int width,int value,unsigned ink){
 rect(x,y,width,4,RGB(21,39,47));if(value<0)value=0;if(value>100)value=100;
 int end=width*value/100;for(int k=0;k<end;k+=5){int w=end-k;if(w>3)w=3;rect(x+k,y,w,4,ink);rect(x+k,y,w,1,dim_rgb(ink,4,5));}
}
static void hc_instrument_icon(int x,int y,int kind,unsigned ink){
 unsigned shade=dim_rgb(ink,1,3),light=RGB(228,231,204);
 if(kind==0){HcPoint p[]={{x+6,y},{x+9,y+5},{x+9,y+11},{x+12,y+15},{x+8,y+14},{x+6,y+17},{x+4,y+14},{x,y+15},{x+3,y+11},{x+3,y+5}};hc_edge_poly(p,10,shade,ink);rect(x+5,y+4,2,9,ink);line(x+5,y+5,x+5,y+11,light);line(x+4,y+15,x+8,y+15,RGB(221,92,49));}
 else if(kind==1){for(int k=0;k<3;k++){int xx=x+k*4,yy=y+(k&1?0:4);rect(xx,yy,3,10,shade);line(xx,yy,xx+2,yy,light);line(xx,yy+2,xx+2,yy+2,ink);line(xx,yy+7,xx+2,yy+7,ink);}for(int k=0;k<5;k++)pixel(x+k*3,y+16-(k&1),ink);}
 else if(kind==2){line(x+1,y+2,x+5,y,ink);line(x+5,y,x+6,y+4,ink);line(x+11,y+2,x+7,y,ink);line(x+7,y,x+6,y+4,ink);line(x+3,y+5,x,y+11,ink);line(x+9,y+5,x+12,y+11,ink);rect(x+4,y+10,5,7,shade);line(x+4,y+10,x+8,y+10,ink);line(x+3,y+7,x+9,y+7,ink);}
 else if(kind==3){HcPoint p[]={{x+1,y+3},{x+10,y},{x+12,y+3},{x+12,y+14},{x+3,y+17},{x,y+14}};hc_edge_poly(p,6,shade,ink);for(int k=0;k<5;k++)line(x+2,y+4+k*2,x+10,y+2+k*2,ink);line(x+4,y+15,x+10,y+13,RGB(230,125,74));}
 else {hc_circle_fast(x+6,y+6,6,ink);hc_circle_fast(x+6,y+6,3,shade);line(x+6,y+6,x+11,y+2,light);rect(x+3,y+13,8,4,shade);line(x+3,y+13,x+10,y+13,ink);}
}
static void hc_material_shell(void){
 /* Static shell is repainted only when panel geometry changes. */
 if(hc_output&&!hc_cache_clear)return;
 const unsigned metal=RGB(34,46,50),edge=RGB(103,124,126);
 rect(0,0,W,35,metal);rect(0,164,W,H-164,metal);
 for(int side=0;side<2;side++){
  HcPoint p[]={{0,28},{12,31},{23,111},{13,143},{33,168},{0,182}};for(int k=0;k<6;k++)if(side)p[k].x=479-p[k].x;hc_edge_poly(p,6,metal,edge);
  int xx=side?479:0,sx=side?-1:1;line(xx+sx*5,37,xx+sx*14,110,RGB(49,94,96));line(xx+sx*14,110,xx+sx*6,143,RGB(49,94,96));
  for(int y=49;y<140;y+=27){line(xx+sx*2,y,xx+sx*10,y+1,RGB(15,25,31));pixel(xx+sx*4,y+3,RGB(95,111,110));}
 }
 HcPoint apron[]={{0,162},{28,152},{159,169},{318,169},{452,146},{479,158},{479,271},{0,271}};hc_edge_poly(apron,8,metal,RGB(112,133,134));
 HcPoint hood[]={{178,176},{214,150},{266,150},{302,176}};hc_edge_poly(hood,4,RGB(31,48,51),RGB(91,123,126));
 HcPoint plane[]={{215,151},{239,157},{239,175},{182,175}};hc_poly(plane,4,RGB(51,70,70));line(214,152,232,169,RGB(105,132,128));line(265,152,247,169,RGB(73,109,112));
 for(int x=3;x<480;x+=158)line(x,172,x,270,RGB(61,83,89));
}
