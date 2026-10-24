/* Resolve the closest phase surface once on the opaque renderer's fixed grid.
 * Uses the idle draw scratch, not another framebuffer. No per-frame truncation. */
static void __attribute__((noinline,optimize("O2"))) neon_phase_raster(const DrawTri *t){
 const Point *p=t->p;float area=edge(p[0],p[1],p[2].x,p[2].y);if(fabsf(area)<.05f)return;
 float iz[3]={1/(p[0].z*area),1/(p[1].z*area),1/(p[2].z*area)},slope[3];
 float dz=(p[2].y-p[1].y)*iz[0]+(p[0].y-p[2].y)*iz[1]+(p[1].y-p[0].y)*iz[2];
 for(int e=0;e<3;e++){Point a=p[e],b=p[(e+1)%3];slope[e]=b.y==a.y?0:(b.x-a.x)/(b.y-a.y);}
 int first=(surface_floor(fmaxf(0,fminf(p[0].y,fminf(p[1].y,p[2].y))))+1)&~1,last=surface_ceil(fminf(neon_render_height-1,fmaxf(p[0].y,fmaxf(p[1].y,p[2].y))));
 for(int y=first;y<=last;y+=2){float yy=y+1,left=1e9f,right=-1e9f;
  for(int e=0;e<3;e++){Point a=p[e],b=p[(e+1)%3];if((a.y<=yy&&b.y>yy)||(b.y<=yy&&a.y>yy)){float x=a.x+(yy-a.y)*slope[e];left=fminf(left,x);right=fmaxf(right,x);}}
  int x0=(surface_ceil(fmaxf(0,left-1))+1)&~1,x1=surface_floor(fminf(neon_render_width-1,right-1));if(x0>x1)continue;
  float inv=edge(p[1],p[2],x0+1,yy)*iz[0]+edge(p[2],p[0],x0+1,yy)*iz[1]+edge(p[0],p[1],x0+1,yy)*iz[2];
  for(int x=x0;x<=x1;x+=2,inv+=dz*2){if(inv<=0)continue;unsigned z=neon_encode_depth(inv),*cell=&render_scratch.deck[(y/2)*(W/2)+x/2];if(z<=surface_depth[y*W+x]&&z<(*cell>>8))*cell=(z<<8)|(t->color&255);}
 }
}
static void neon_phase_begin(void){
 neon_flush();memset(render_scratch.deck,255,(W/2)*(H/2)*sizeof(unsigned));race_phase_collect=1;
}
static void neon_phase_triangle(Vec3 a,Vec3 b,Vec3 c,int alpha){
 /* The queue and coverage map alias: rasterize stack-local clipped faces. */
 Vec3 in[3]={neon_camera(a),neon_camera(b),neon_camera(c)},v[4];int n=0;
 for(int i=0;i<3;i++){Vec3 p=in[i],q=in[(i+1)%3];if(p.z>=64)v[n++]=p;if((p.z>=64)!=(q.z>=64))v[n++]=add(p,mul(sub(q,p),(64-p.z)/(q.z-p.z)));}
 for(int i=1;i+1<n;i++){DrawTri t={.p={project(v[0]),project(v[i]),project(v[i+1])},.color=RGB(alpha,0,0)};neon_phase_raster(&t);}
}
static void __attribute__((noinline,optimize("O3"))) neon_phase_end(void){
 race_phase_collect=0;
 for(int y=0;y<neon_render_height;y+=2)for(int x=0;x<neon_render_width;x+=2){unsigned packed=render_scratch.deck[(y/2)*(W/2)+x/2];if(packed==~0u)continue;unsigned depth=packed>>8;int alpha=packed&255;
  int at=y*W+x,out=y*STRIDE+x;unsigned old=fb[out],color=0;int uniform=old==fb[out+1]&&old==fb[out+STRIDE]&&old==fb[out+STRIDE+1];
  if(uniform){unsigned rb=(((old&0xff00ff)*(256-alpha)+(race_glass_ink&0xff00ff)*alpha)>>8)&0xff00ff;unsigned green=(((old&0xff00)*(256-alpha)+(race_glass_ink&0xff00)*alpha)>>8)&0xff00;color=0xff000000|rb|green;}
  for(int dy=0;dy<2;dy++)for(int dx=0;dx<2;dx++)if(depth<=surface_depth[at+dy*W+dx]){unsigned *pixel=&fb[out+dy*STRIDE+dx];if(uniform)*pixel=color;else{unsigned c=*pixel;
   unsigned rb=(((c&0xff00ff)*(256-alpha)+(race_glass_ink&0xff00ff)*alpha)>>8)&0xff00ff;unsigned green=(((c&0xff00)*(256-alpha)+(race_glass_ink&0xff00)*alpha)>>8)&0xff00;*pixel=0xff000000|rb|green;}}

 }
}
