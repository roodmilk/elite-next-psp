/* Opaque layout 5 only. The pass starts with uniform depth and owns aligned
 * 2x2 quads; decorative lines only read depth. Coverage, reciprocal depth and
 * equal-depth ownership match the established flat-colour raster exactly. */
typedef unsigned NeonDepthPair __attribute__((may_alias));
static inline __attribute__((always_inline)) void race_neon_opaque_impl(DrawTri *t,int fog,int lit){
 Point *p=t->p;
 if((p[0].x< -2&&p[1].x< -2&&p[2].x< -2)||
    (p[0].x>W+2&&p[1].x>W+2&&p[2].x>W+2)||
    (p[0].y<surface_y_min-2&&p[1].y<surface_y_min-2&&p[2].y<surface_y_min-2)||
    (p[0].y>surface_y_max+2&&p[1].y>surface_y_max+2&&p[2].y>surface_y_max+2))return;
 float area=edge(p[0],p[1],p[2].x,p[2].y);if(fabsf(area)<.05f)return;
 float iz0=1.f/(p[0].z*area),iz1=1.f/(p[1].z*area),iz2=1.f/(p[2].z*area);
 float dzdx=(p[2].y-p[1].y)*iz0+(p[0].y-p[2].y)*iz1+(p[1].y-p[0].y)*iz2;
 float slope[3];for(int e=0;e<3;e++){Point a=p[e],b=p[(e+1)%3];slope[e]=b.y==a.y?0:(b.x-a.x)/(b.y-a.y);}
 int first=surface_floor(surface_max(surface_y_min,surface_min(p[0].y,surface_min(p[1].y,p[2].y))));
 int last=surface_ceil(surface_min(surface_y_max,surface_max(p[0].y,surface_max(p[1].y,p[2].y))));
 first=(first+1)&~1;unsigned ink=t->color;
 /* ERL-only smooth lighting reuses the unused material/order slots. No
  * additional triangle buffer or changes to the space/planet raster. */
 float base[3],cx[3],cy[3];int step_rgb[3];
 if(lit)for(int ch=0;ch<3;ch++){int shift=ch*8;float a=(t->color>>shift)&255,b=(t->material>>shift)&255,c=(t->order>>shift)&255;
  cx[ch]=((p[2].y-p[1].y)*a+(p[0].y-p[2].y)*b+(p[1].y-p[0].y)*c)/area;
  cy[ch]=((p[1].x-p[2].x)*a+(p[2].x-p[0].x)*b+(p[0].x-p[1].x)*c)/area;base[ch]=a-cx[ch]*p[0].x-cy[ch]*p[0].y;step_rgb[ch]=(int)(cx[ch]*512);
 }
 /* Height-fog faces dissolve into the already rendered cloud background.
  * Only these city triangles pay for alpha interpolation; road stays opaque. */
 float ax=0,ay=0,az=255;
 if(fog){float a=t->material&255,b=(t->material>>8)&255,c=(t->material>>16)&255;
  ax=((p[2].y-p[1].y)*a+(p[0].y-p[2].y)*b+(p[1].y-p[0].y)*c)/area;
  ay=((p[1].x-p[2].x)*a+(p[2].x-p[0].x)*b+(p[0].x-p[1].x)*c)/area;az=a-ax*p[0].x-ay*p[0].y;}
 for(int y=first;y<=last;y+=2){float yy=y+1.f,left=1e9f,right=-1e9f;
  for(int e=0;e<3;e++){Point a=p[e],b=p[(e+1)%3];
   if((a.y<=yy&&b.y>yy)||(b.y<=yy&&a.y>yy)){float xx=a.x+(yy-a.y)*slope[e];if(xx<left)left=xx;if(xx>right)right=xx;}
  }
  if(left>right)continue;
  int x0=surface_ceil(surface_max(0,left-1)),x1=surface_floor(surface_min(clipx1-1,right-1));x0=(x0+1)&~1;
  /* Keep row starts and additions in the same order as the established path;
   * no interpolated/shortened depth spans or altered silhouette sampling. */
  float inv=edge(p[1],p[2],x0+1,yy)*iz0+edge(p[2],p[0],x0+1,yy)*iz1+edge(p[0],p[1],x0+1,yy)*iz2;
  int at=y*W+x0,out=y*STRIDE+x0,second=y+1<=surface_y_max;
  int shade[3];if(lit)for(int ch=0;ch<3;ch++)shade[ch]=(int)((base[ch]+cx[ch]*(x0-1)+cy[ch]*yy)*256);
  for(int x=x0;x<=x1;x+=2,inv+=dzdx*2,at+=2,out+=2){
   if(lit){shade[0]+=step_rgb[0];shade[1]+=step_rgb[1];shade[2]+=step_rgb[2];}
   if(inv<=0)continue;
   unsigned depth=neon_encode_depth(inv);if(depth>surface_depth[at])continue;
   if(lit){int red=shade[0]>>8,green=shade[1]>>8,blue=shade[2]>>8;
    red=red<0?0:red>255?255:red;green=green<0?0:green>255?255:green;blue=blue<0?0:blue>255?255:blue;ink=RGB(red,green,blue);}
   unsigned colour=ink;if(fog){int alpha=(int)(az+ax*(x+1)+ay*yy);if(alpha<=0)continue;if(alpha<255)colour=planet_rgb_mix(fb[out],ink,alpha);}
   unsigned packed=depth|(depth<<16);*(NeonDepthPair *)(surface_depth+at)=packed;
   fb[out]=fb[out+1]=colour;
   if(second){*(NeonDepthPair *)(surface_depth+at+W)=packed;fb[out+STRIDE]=fb[out+STRIDE+1]=colour;}
  }
 }
}

static void __attribute__((noinline,optimize("O3","fast-math"))) race_neon_opaque_triangle(DrawTri *t){if(t->material>=0&&(t->material&0x20000000))race_neon_opaque_impl(t,1,0);else if(t->material>=0&&(t->material&0x40000000))race_neon_opaque_impl(t,0,1);else race_neon_opaque_impl(t,0,0);}
