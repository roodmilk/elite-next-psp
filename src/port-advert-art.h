/* Six native pixel-art advertisements. Authored once, sampled onto actual
 * projected planes; no screen-facing labels and no per-frame allocations. */
static unsigned port_ad_art[6][64*32];static int port_ad_ready;static unsigned port_ad_written[24];
static unsigned*port_art_target;
static void pa_pixel(int x,int y,unsigned c){if(x>=0&&x<64&&y>=0&&y<32)port_art_target[y*64+x]=c;}
static void pa_rect(int x,int y,int w,int h,unsigned c){for(int j=0;j<h;j++)for(int i=0;i<w;i++)pa_pixel(x+i,y+j,c);}
static void pa_line(int x,int y,int xx,int yy,unsigned c){int dx=abs(xx-x),sx=x<xx?1:-1,dy=-abs(yy-y),sy=y<yy?1:-1,e=dx+dy;for(;;){pa_pixel(x,y,c);if(x==xx&&y==yy)break;int e2=e*2;if(e2>=dy){e+=dy;x+=sx;}if(e2<=dx){e+=dx;y+=sy;}}}
static void pa_text(int x,int y,const char*s,unsigned c){for(int k=0;s[k];k++)for(int j=0;j<8;j++)for(int i=0;i<8;i++)if(font8[(unsigned char)s[k]-32][j]&(128>>i))pa_pixel(x+k*8+i,y+j,c);}
static void port_ads_init(void){if(port_ad_ready)return;port_ad_ready=1;
 static const char*names[]={"KOYO","NOVA","VOX","DRIFT","AZURE","ORBIT"};
 static const unsigned colors[]={RGB(255,137,179),RGB(255,209,109),RGB(95,235,224),RGB(170,198,255),RGB(117,225,246),RGB(232,174,255)};
 for(int id=0;id<6;id++){port_art_target=port_ad_art[id];unsigned c=colors[id],shade=dim_rgb(c,1,3),light=RGB(249,237,206);
  for(int y=0;y<32;y++)for(int x=0;x<64;x++)pa_pixel(x,y,RGB(8+x/16,14+y/8,24+x/24));
  pa_rect(0,0,64,1,c);pa_rect(0,31,64,1,shade);pa_rect(0,1,1,30,shade);pa_rect(63,1,1,30,c);pa_text((64-(int)strlen(names[id])*8)/2,3,names[id],c);pa_rect(3,11,58,1,shade);
  for(int x=4;x<62;x+=7){pa_pixel(x,14,shade);pa_pixel(x+2,28,shade);}
  if(id==0){/* steaming ceramic bowl, noodles, lacquer rim and chopsticks */
   for(int y=21;y<29;y++){int w=23-(y-21)*2;pa_rect(32-w/2,y,w,1,y==21?light:y<24?c:RGB(121,47,74));}pa_rect(21,20,23,2,light);pa_rect(24,21,18,1,RGB(230,164,69));
   for(int k=0;k<6;k++)pa_line(25+k*3,20,27+k*2,23,RGB(255,212,112));pa_line(36,19,49,13,light);pa_line(38,20,51,15,c);
   for(int k=0;k<3;k++){pa_line(25+k*7,17,23+k*7,15,c);pa_pixel(24+k*7,14,light);}pa_rect(28,29,8,1,shade);
  }else if(id==1){/* star roulette / orbit: bright centre, numbered lights */
   for(int k=0;k<20;k++){float a=k*.3141593f;pa_pixel(32+(int)(cosf(a)*13),21+(int)(sinf(a)*7),k%3?c:light);}pa_line(18,25,47,16,shade);pa_line(24,21,40,21,c);pa_line(32,14,32,28,c);pa_line(27,17,37,25,light);pa_line(27,25,37,17,light);pa_rect(30,19,5,5,light);pa_rect(31,20,3,3,c);
  }else if(id==2){/* radio receiver with twin cones and cyan spectrum */
   pa_rect(12,16,40,13,shade);pa_rect(13,17,38,10,RGB(14,39,49));pa_rect(15,18,9,8,c);pa_rect(17,19,5,6,RGB(13,34,45));pa_rect(40,18,9,8,c);pa_rect(42,19,5,6,RGB(13,34,45));
   for(int k=0;k<6;k++){int h=2+(k*3)%7;pa_rect(26+k*2,26-h,1,h,k==2?light:c);}pa_line(41,16,48,13,light);pa_rect(29,18,8,1,light);
  }else if(id==3){/* outlined courier and sectional engine glow */
   for(int y=15;y<=27;y++){int w=2+(y-15)*2;pa_rect(33-w/2,y,w,1,y%3==0?light:RGB(77,109,154));}pa_line(33,14,20,27,c);pa_line(33,14,46,27,c);pa_line(20,27,46,27,light);pa_rect(31,18,4,5,RGB(116,230,240));pa_line(25,24,29,25,shade);pa_line(37,25,41,24,shade);
   pa_rect(24,28,6,2,RGB(255,175,89));pa_rect(36,28,6,2,RGB(255,175,89));pa_rect(26,30,2,1,light);pa_rect(38,30,2,1,light);pa_line(11,21,19,21,shade);pa_line(47,21,55,21,shade);
  }else if(id==4){/* oxygen helmet, visor reflections, valves and hose */
   pa_rect(24,15,17,14,shade);pa_rect(22,18,21,8,c);pa_rect(25,16,15,10,c);pa_rect(26,17,13,7,RGB(13,44,64));pa_line(28,18,35,18,light);pa_line(27,19,29,19,light);pa_rect(29,25,7,3,RGB(43,73,86));pa_rect(30,26,5,1,light);pa_rect(23,27,4,3,light);pa_rect(38,27,4,3,light);pa_line(42,26,47,29,c);pa_line(47,29,52,25,c);pa_rect(15,20,4,7,c);pa_rect(15,18,4,1,light);
  }else{/* ring hotel / planet, inhabited floors and garden lights */
   for(int y=15;y<29;y++){int dx=(int)sqrtf(fmaxf(0,49-(y-22)*(y-22)));pa_rect(32-dx,y,dx*2+1,1,y<21?RGB(98,123,178):RGB(48,66,109));}pa_line(15,26,47,17,c);pa_line(15,27,48,18,light);pa_line(18,29,49,20,shade);
   for(int k=0;k<8;k++){pa_pixel(20+k*3,26-k/2,light);pa_pixel(22+k*3,27-k/2,c);}pa_rect(12,21,5,2,shade);pa_rect(47,25,5,2,shade);pa_pixel(46,14,light);
  }
  pa_pixel(2,2,light);pa_pixel(61,29,light);
 }
}
typedef struct{Vec3 p;float u,v;} PortAdVertex;
static int port_ad_triangle(PortAdVertex a,PortAdVertex b,PortAdVertex c,int art,unsigned paper){
 PortAdVertex in[6]={a,b,c},out[6];int n=3,m=0;for(int i=0;i<n;i++){PortAdVertex p=in[i],q=in[(i+1)%n];int pi=p.p.z>=8.5f,qi=q.p.z>=8.5f;if(pi)out[m++]=p;if(pi!=qi){float t=(8.5f-p.p.z)/(q.p.z-p.p.z);out[m++]=(PortAdVertex){add(p.p,mul(sub(q.p,p.p),t)),p.u+(q.u-p.u)*t,p.v+(q.v-p.v)*t};}}
 int written=0;for(int k=1;k+1<m;k++){PortAdVertex v[3]={out[0],out[k],out[k+1]};Point p[3]={project(v[0].p),project(v[1].p),project(v[2].p)};float area=edge(p[1],p[2],p[0].x,p[0].y);if(fabsf(area)<.05f)continue;float ia=1/area;
  int x0=(int)fmaxf(clipx0,floorf(fminf(p[0].x,fminf(p[1].x,p[2].x)))),x1=(int)fminf(clipx1-1,ceilf(fmaxf(p[0].x,fmaxf(p[1].x,p[2].x)))),y0=(int)fmaxf(fmaxf(35,clipy0),floorf(fminf(p[0].y,fminf(p[1].y,p[2].y)))),y1=(int)fminf(fminf(163,clipy1-1),ceilf(fmaxf(p[0].y,fmaxf(p[1].y,p[2].y))));
  float iz[3]={1/v[0].p.z,1/v[1].p.z,1/v[2].p.z};for(int y=y0;y<=y1;y++)for(int x=x0;x<=x1;x++){unsigned*dst=&fb[y*STRIDE+x];if(*dst!=paper)continue;float w0=edge(p[1],p[2],x+.5f,y+.5f)*ia,w1=edge(p[2],p[0],x+.5f,y+.5f)*ia,w2=1-w0-w1;if(w0<-.001f||w1<-.001f||w2<-.001f)continue;float q0=w0*iz[0],q1=w1*iz[1],q2=w2*iz[2],r=1/(q0+q1+q2);int u=(int)((q0*v[0].u+q1*v[1].u+q2*v[2].u)*r*63+.5f),vv=(int)((q0*v[0].v+q1*v[1].v+q2*v[2].v)*r*31+.5f);if(u<0)u=0;if(u>63)u=63;if(vv<0)vv=0;if(vv>31)vv=31;*dst=port_ad_art[art][vv*64+u];written++;}
 }return written;
}
static unsigned port_ad_paper(int id,int brand){return RGB(10+id,17,26+brand);}
static void port_ad_map(Vec3 a,Vec3 b,Vec3 c,Vec3 d,int brand,int id){port_ads_init();unsigned paper=port_ad_paper(id,brand);PortAdVertex v[4]={{departure_view_point(a),0,0},{departure_view_point(b),1,0},{departure_view_point(c),1,1},{departure_view_point(d),0,1}};int n=port_ad_triangle(v[0],v[1],v[2],brand,paper)+port_ad_triangle(v[0],v[2],v[3],brand,paper);if(id>=0&&id<24)port_ad_written[id]+=n;}
