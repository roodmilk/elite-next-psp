#ifdef ELITE_ENABLE_REVIEW
/* Sparse emission bloom. Sample the untouched scene into the flushed queue
 * arena before compositing small radial halos. No allocation or full blur. */
static void __attribute__((noinline,optimize("O2"))) neon_review_old_bloom(void){
 unsigned *emission=render_scratch.deck;int count=0;
 for(int y=2;y<H-2;y+=8)for(int x=2;x<W-2;x+=8){
  unsigned best=0;int peak=220,bx=x,by=y;
  for(int dy=0;dy<4;dy+=2)for(int dx=0;dx<4;dx+=2){
   unsigned c=fb[(y+dy)*STRIDE+x+dx];int red=c&255,green=(c>>8)&255,blue=(c>>16)&255,m=red>green?red:green;if(blue>m)m=blue;
   if(m>peak){peak=m;best=c;bx=x+dx;by=y+dy;}
  }
  if(best){emission[count++]=bx|(by<<16);emission[count++]=best;}
 }
 for(int n=0;n<count;n+=2){
  int x=emission[n]&65535,y=emission[n]>>16;unsigned c=emission[n+1];
  for(int dy=-5;dy<=5;dy++)for(int dx=-5;dx<=5;dx++){
   int d=dx*dx+dy*dy;if(d>25||x+dx<0||x+dx>=W||y+dy<0||y+dy>=H)continue;
   unsigned strength=26-d,rb=(((c&0xff00ff)*strength)>>8)&0xff00ff,g=(((c&0xff00)*strength)>>8)&0xff00;
   unsigned *pixel=&fb[(y+dy)*STRIDE+x+dx],old=*pixel;rb+=old&0xff00ff;g+=old&0xff00;
   unsigned overflow=rb&0x1000100;rb=(rb|(overflow-(overflow>>8)))&0xff00ff;if(g>0xff00)g=0xff00;*pixel=0xff000000|rb|g;
  }
 }
}

/* Muted native regression: real course contact/input/camera/render and decoder.
 * Boundary cases are deliberately staged; they are not a human lap claim. */
static RaceTrackFrame neon_review_pose(int section,float lane,float height){
 SpaceRace *r=&game.race;r->gate=section;r->phase=RACE_RUNNING;r->countdown=0;r->elapsed=1;r->repair=r->handover=r->crash_time=0;r->rivals=0;r->protection=10;
 game.erl.serial++;game.erl.paid=game.erl.result=game.erl.paused=0;game.dead=game.docked=0;game.speed=game.boost=0;
 RaceTrackFrame f=race_track_frame(r,section,.45f);game.yaw=atan2f(f.ahead.x,f.ahead.z);game.pitch=asinf(f.ahead.y);
 Vec3 br={cosf(game.yaw),0,-sinf(game.yaw)},bu=norm((Vec3){f.ahead.y*br.z,f.ahead.z*br.x-f.ahead.x*br.z,-f.ahead.y*br.x});game.roll=atan2f(-dot(f.up,br),dot(f.up,bu));
 game.pos=add(f.centre,add(mul(f.right,lane),mul(f.up,RACE_TRACK_FLOOR+race_floor_support(&game,0,f)+35+height)));
 r->arcade.actor[0].health=RA_HEALTH;r->arcade.actor[0].protection=10;return f;
}
static void neon_review_capture(const char *name){render_submitted=0;drawcount=0;space();erl_pause_draw();dump_native_bmp(name);}
static void __attribute__((noinline,optimize("O2"))) neon_review_old_cloud_row(const NeonCloudView *view,int y,unsigned *row){
 Vec3 ray=sub(add(view->ahead,mul(view->up,(proj_oy-y)*view->inv_focal)),mul(view->right,proj_ox*view->inv_focal)),step=mul(view->right,16*view->inv_focal);
 for(int x=0;x<=W/16;x++){row[x]=neon_cloud_colour(view,ray,y);ray=add(ray,step);}
}
static void neon_review_old_base_sky(void){
 for(int y=0;y<H;y++)rect(0,y,W,1,RGB(74+y*112/H,44+y*78/H,98+y*48/H));
}
static void __attribute__((noinline,optimize("O2"))) neon_review_old_cloud_sky(void){
 const SpaceRace *r=&game.race;Vec3 right,up,ahead;camera_axes(&game,&right,&up,&ahead);float scale=race_neon_scale(r);Vec3 origin=mul(sub(camera_origin(&game),r->start),1/scale);
 NeonCloudView view={{dot(origin,r->right),dot(origin,r->up),dot(origin,r->dir)-7200},
  {dot(right,r->right),dot(right,r->up),dot(right,r->dir)},
  {dot(up,r->right),dot(up,r->up),dot(up,r->dir)},
  {dot(ahead,r->right),dot(ahead,r->up),dot(ahead,r->dir)},0,1/proj_focal};view.plane=-95-view.eye.y;
 unsigned first[W/16+1],second[W/16+1],*top=first,*bottom=second;neon_review_old_cloud_row(&view,0,top);
 /* Sixteen-pixel perspective samples, smoothly interpolated on the game's
  * native 2x2 geometry grid. Sky-only blocks fill the original per-row gradient.
  * The road has already written uniform quad depth. Background colours never
  * write depth, leaving every uncovered cloud pixel exactly as before. */
 for(int y=0;y<H;y+=16){neon_review_old_cloud_row(&view,y+16,bottom);
  for(int x=0;x<W/16;x++){
   const unsigned short *depth=surface_depth+y*W+x*16;unsigned uncovered=0;
   for(int col=0;col<4;col++)for(int row=0;row<4;row++)if(depth[row*4*W+col*4]==65535||depth[(row*4+2)*W+col*4]==65535||depth[row*4*W+col*4+2]==65535||depth[(row*4+2)*W+col*4+2]==65535)uncovered|=1u<<(col*4+row);
   if(!uncovered)continue;
   unsigned a[4],b[4];a[0]=top[x];a[2]=neon_cloud_average(a[0],top[x+1]);a[1]=neon_cloud_average(a[0],a[2]);a[3]=neon_cloud_average(a[2],top[x+1]);
   b[0]=bottom[x];b[2]=neon_cloud_average(b[0],bottom[x+1]);b[1]=neon_cloud_average(b[0],b[2]);b[3]=neon_cloud_average(b[2],bottom[x+1]);
   unsigned *pixels=fb+y*STRIDE+x*16;for(int col=0;col<4;col++){unsigned mask=(uncovered>>(col*4))&15;if(!mask)continue;unsigned mid=neon_cloud_average(a[col],b[col]),colours[4]={a[col],neon_cloud_average(a[col],mid),mid,neon_cloud_average(mid,b[col])};
    for(int row=0;row<4;row++){if(!(mask&(1u<<row)))continue;for(int sy=0;sy<4;sy+=2)for(int sx=0;sx<4;sx+=2){if(depth[(row*4+sy)*W+col*4+sx]!=65535)continue;unsigned *out=pixels+(row*4+sy)*STRIDE+col*4+sx;out[0]=out[1]=out[STRIDE]=out[STRIDE+1]=colours[row];}}
   }
  }
  unsigned *swap=top;top=bottom;bottom=swap;
 }
}

static void neon_review_old_racer_triangle(DrawTri*t){
 Point a=t->p[0],b=t->p[1],c=t->p[2];float area=edge(a,b,c.x,c.y);if(fabsf(area)<.05f)return;
 float ia=1/(a.z*area),ib=1/(b.z*area),ic=1/(c.z*area);
 int x0=(int)fmaxf(clipx0,floorf(fminf(a.x,fminf(b.x,c.x)))),x1=(int)fminf(clipx1-1,ceilf(fmaxf(a.x,fmaxf(b.x,c.x))));
 int top=clipy0>=0?clipy0:view_top(),bot=clipy1>=0?clipy1-1:view_bot();
 int y0=(int)fmaxf(top,floorf(fminf(a.y,fminf(b.y,c.y)))),y1=(int)fminf(bot,ceilf(fmaxf(a.y,fmaxf(b.y,c.y))));
 int racer=t->material>=7800&&t->material<7804;
 int fleet=t->material>=8192&&t->material<FLEET_QUAD_BASE,quad=t->material>=FLEET_QUAD_BASE||racer;
 int face=t->material-(fleet?8192:4096),tile=quad?(t->material-FLEET_QUAD_BASE)/2:0;
 static const unsigned char quv[2][6]={{0,0,31,0,31,31},{0,0,31,31,0,31}};
 const unsigned char*uv=fleet?fleet_faces[face].uv:quad?quv[(t->material-FLEET_QUAD_BASE)&1]:ship_art_uv[face];
 if(fleet)tile=fleet_faces[face].tile;if(racer)tile=(t->material-7800)/2;
 /* Minify fine panel detail before it becomes flickering subpixel noise. */
 float span=fmaxf(fabsf(a.x-b.x)+fabsf(a.y-b.y),fmaxf(fabsf(b.x-c.x)+fabsf(b.y-c.y),fabsf(c.x-a.x)+fabsf(c.y-a.y)));
 int mip=(fleet||quad)?(span<12?2:span<25?1:0):0;
 int size=32>>mip,offset=mip==0?0:mip==1?512:640;
 static unsigned racer_palette[16],racer_color;unsigned local_palette[16],*palette=racer?racer_palette:local_palette;
 if(!racer||racer_color!=t->color){if(racer)racer_color=t->color;for(int i=0;i<16;i++){
  palette[i]=ship_art_ink(t->color,i);
  /* Painted white reflections preserve each team's lacquer underneath.
   * Restricted to racer materials; fleet/spaceflight art is unchanged. */
  if(racer&&(i==3||i==4||i==13||i==14)){int k=i==3?28:i==4?72:i==13?140:220;unsigned b=t->color;
   palette[i]=RGB(((b&255)*(256-k)+255*k)>>8,(((b>>8)&255)*(256-k)+255*k)>>8,(((b>>16)&255)*(256-k)+255*k)>>8);}
 }}
 for(int y=y0;y<=y1;y++)for(int x=x0;x<=x1;x++){
  float e0=edge(b,c,x+.5f,y+.5f),e1=edge(c,a,x+.5f,y+.5f),e2=edge(a,b,x+.5f,y+.5f);
  if((e0>=-.05f&&e1>=-.05f&&e2>=-.05f)||(e0<=.05f&&e1<=.05f&&e2<=.05f)){
   float z=e0*ia+e1*ib+e2*ic;if(z<=0)continue;
   int u=(int)((e0*ia*uv[0]+e1*ib*uv[2]+e2*ic*uv[4])/z),v=(int)((e0*ia*uv[1]+e1*ib*uv[3]+e2*ic*uv[5])/z);
   if(u<0)u=0;if(u>31)u=31;if(v<0)v=0;if(v>31)v=31;
   if(surface_depth_on){unsigned depth=surface_encode_depth(z);if(depth>surface_depth[y*W+x])continue;surface_depth[y*W+x]=depth;}
   int at=(v>>mip)*size+(u>>mip);
   unsigned char packed=racer?erl_racer_pixels[tile][offset+(at>>1)]:fleet?fleet_tile_pixels[tile][offset+(at>>1)]:quad?fleet_tile_pixels[tile][offset+(at>>1)]:ship_art_pixels[ship_art_panel[face]][at>>1];
   fb[y*STRIDE+x]=palette[(packed>>((at&1)*4))&15];
  }
 }
}

static void neon_review_compact_capture(const char *name){
 unsigned *saved=fb;fb=(unsigned *)0x44000000u;space();erl_pause_draw();neon_present_finish();neon_present_readback();dump_native_bmp(name);fb=saved;
}
static int erl_neon_review(void){
 FILE *flag=fopen("erl-neon-review.flag","r");if(!flag)return 0;fclose(flag);FILE *out=fopen("erl-neon-review.txt","w");if(!out)return 1;
 int failures=0,checks=0;
 #define NCHK(c,s) do{int ok=!!(c);checks++;failures+=!ok;fprintf(out,"%s %s\n",ok?"PASS":"FAIL",s);fflush(out);}while(0)
 unsigned *old=fb,*pixels=calloc(STRIDE*H,sizeof(unsigned));if(!pixels){fclose(out);return 1;}fb=pixels;
 /* Check actual GE output: sharp presentation must never invent colours
  * between the source checker pixels, including the native bottom rows. */
 {unsigned *saved_fb=fb;unsigned a=RGB(21,61,127),b=RGB(237,193,43);for(int y=0;y<216;y++)for(int x=0;x<384;x++)dc_open_source[y*STRIDE+x]=((x^y)&1)?a:b;
  fb=(unsigned *)0x44000000u;neon_present_target=fb;neon_present_image=0;neon_render_width=384;neon_render_height=216;neon_present_hud();neon_present_finish();neon_present_readback();int exact=1,first=0,second=0;
  for(int y=0;y<H;y++)for(int x=0;x<W;x++){unsigned c=fb[y*STRIDE+x];exact&=c==a||c==b;first+=c==a;second+=c==b;}
  NCHK(exact&&first>50000&&second>50000,"sharp GE upscale preserves source colours without any bilinear intermediate pixels across480x272");fb=saved_fb;
 }
 /* Full-width reference versus narrow-cache sprites, including all HUD
  * rows and translucent overlay edges. Compare real GE output byte-for-byte. */
 {unsigned *saved_fb=fb,*reference=malloc(W*H*4);int exact=reference!=0;
  for(int size=0;size<2;size++)for(int sliced=0;sliced<2;sliced++){
   int width=size?384:320,height=size?216:180;for(int y=0;y<height;y++)for(int x=0;x<width;x++)dc_open_source[y*STRIDE+x]=RGB((x*37+y*13)&255,(x*17+y*23)&255,(x*7+y*43)&255);
   fb=(unsigned *)0x44000000u;neon_present_target=fb;neon_present_image=0;neon_render_width=width;neon_render_height=height;neon_present_sliced=sliced;neon_present_hud();rect(13,250,99,22,0x80cc7733u);neon_present_finish();neon_present_readback();
   if(reference)for(int y=0;y<H;y++)if(!sliced)memcpy(reference+y*W,fb+y*STRIDE,W*4);else exact&=memcmp(reference+y*W,fb+y*STRIDE,W*4)==0;
  }
  neon_present_sliced=1;fb=saved_fb;NCHK(exact,"narrow GE sprites preserve every480x272 pixel at320/384 scene sizes including translucent HUD and bottom16rows");free(reference);
 }
 /* Pixel-exact regression for the optimized glow kernel: dense mixed
  * emission, clipped edge halos and untouched stride padding. */
 {unsigned *expected=malloc(STRIDE*H*sizeof(unsigned));int equal=expected!=0;
  if(expected)for(int seed=0;seed<3;seed++){
   unsigned state=0x9e3779b9u+seed;for(int i=0;i<STRIDE*H;i++){state=state*1664525u+1013904223u;fb[i]=0xff000000u|(state&0xffffffu);}
   memcpy(expected,fb,STRIDE*H*sizeof(unsigned));neon_review_old_bloom();
   unsigned *actual=fb;fb=expected;race_neon_bloom();equal&=memcmp(actual,expected,STRIDE*H*sizeof(unsigned))==0;fb=actual;
  }
  NCHK(equal,"optimized emission bloom is pixel-exact on three dense scenes including screen edges and padding");free(expected);
 }
 {int saved_mode=neon_depth_mode,equal=1;neon_depth_mode=1;
  for(int i=0;i<=8192;i++){float inv=i*(1.f/131072);equal&=neon_encode_depth(inv)==surface_encode_depth(inv);}
  NCHK(equal,"inlined ERL depth mapping exactly matches shared conversion across near, far and clamped samples");neon_depth_mode=saved_mode;
 }
 {int mode=neon_depth_mode,enabled=surface_depth_on,cx0=clipx0,cx1=clipx1,cy0=clipy0,cy1=clipy1;
  neon_depth_mode=surface_depth_on=1;clipx0=clipy0=0;clipx1=W;clipy1=H;
  unsigned *expected=malloc(STRIDE*H*sizeof(unsigned));unsigned short *depth=malloc(sizeof(surface_depth));int safe=expected&&depth;
  unsigned changed=0,covered=0;
  if(safe)for(int k=0;k<8;k++){
   DrawTri t={.p={{30.25f,40.25f,500},{160.75f,55.5f,700},{70.5f,140.75f,900}},.color=RGB(45,164,190),.material=7800+(k&3)};
   if(k&4){Point swap=t.p[0];t.p[0]=t.p[2];t.p[2]=swap;}
   for(int i=0;i<STRIDE*H;i++)fb[i]=0xff102030u;memset(surface_depth,255,sizeof(surface_depth));neon_review_old_racer_triangle(&t);
   memcpy(expected,fb,STRIDE*H*sizeof(unsigned));memcpy(depth,surface_depth,sizeof(surface_depth));
   for(int i=0;i<STRIDE*H;i++)fb[i]=0xff102030u;memset(surface_depth,255,sizeof(surface_depth));ship_art_triangle(&t);
   for(int y=0;y<H;y++)for(int x=0;x<W;x++){int at=y*W+x;safe&=(depth[at]==65535)==(surface_depth[at]==65535);if(depth[at]!=65535){covered++;changed+=fb[y*STRIDE+x]!=expected[y*STRIDE+x];safe&=abs((int)depth[at]-(int)surface_depth[at])<=1;}}
  }
  NCHK(safe&&changed<=covered/1000+8,"incremental racer paint preserves coverage and depth, with fewer than 0.1 percent changed texture samples");fprintf(out,"RASTER painted=%u changed=%u\n",covered,changed);
  free(expected);free(depth);neon_depth_mode=mode;surface_depth_on=enabled;clipx0=cx0;clipx1=cx1;clipy0=cy0;clipy1=cy1;
 }
 /* New smooth-light path: interpolate independent channels at a known
  * interior sample, and retain nearer opaque depth ownership. */
 {int ymin=surface_y_min,ymax=surface_y_max,mode=neon_depth_mode;surface_y_min=0;surface_y_max=H-1;neon_depth_mode=1;memset(surface_depth,255,sizeof(surface_depth));
  DrawTri light={.p={{16,16,1000},{80,16,1000},{16,80,1000}},.color=RGB(200,0,0),.material=0x40000000|RGB(0,200,0)&0xffffff,.order=RGB(0,0,200)&0xffffff};
  race_neon_opaque_triangle(&light);unsigned c=fb[32*STRIDE+32];NCHK(abs((int)(c&255)-94)<=2&&abs((int)((c>>8)&255)-53)<=2&&abs((int)((c>>16)&255)-53)<=2,"smooth road light interpolates RGB without flat colour bands");
  surface_depth[32*W+32]=0;fb[32*STRIDE+32]=RGB(9,23,47);race_neon_opaque_triangle(&light);NCHK(fb[32*STRIDE+32]==RGB(9,23,47),"smooth road shading cannot overwrite nearer opaque geometry");
  surface_y_min=ymin;surface_y_max=ymax;neon_depth_mode=mode;
 }
 {int mode=neon_depth_mode,lo=surface_y_min,hi=surface_y_max;neon_depth_mode=1;surface_y_min=0;surface_y_max=H-1;
  DrawTri fog={.p={{16,16,1000},{80,16,1000},{16,80,1000}},.color=RGB(220,80,160),.material=0x20000000};
  memset(surface_depth,255,sizeof(surface_depth));rect(0,0,W,H,RGB(60,40,100));race_neon_opaque_triangle(&fog);
  NCHK(fb[32*STRIDE+32]==RGB(60,40,100)&&surface_depth[32*W+32]==65535,"fully submerged building pixels leave cloud colour and depth untouched");
  fog.material=0x20808080;race_neon_opaque_triangle(&fog);unsigned c=fb[32*STRIDE+32];NCHK((c&255)>120&&(c&255)<160,"cloud boundary blends building silhouette continuously instead of clipping it");
  surface_depth[32*W+32]=0;fb[32*STRIDE+32]=RGB(7,13,29);race_neon_opaque_triangle(&fog);NCHK(fb[32*STRIDE+32]==RGB(7,13,29),"building mist cannot cover nearer road or craft depth");
  neon_depth_mode=mode;surface_y_min=lo;surface_y_max=hi;
 }
 {int old_mode=neon_depth_mode,lo=surface_y_min,hi=surface_y_max;neon_depth_mode=1;surface_y_min=0;surface_y_max=H-1;race_glass_ink=RGB(167,47,130);memset(surface_depth,255,sizeof(surface_depth));rect(0,0,W,H,RGB(8,12,24));
  DrawTri field={.p={{16,16,1000},{80,16,1000},{16,80,1000}},.color=RGB(150,0,0)};
  neon_phase_begin();neon_phase_raster(&field);neon_phase_end();unsigned once=fb[32*STRIDE+32];rect(0,0,W,H,RGB(8,12,24));neon_phase_begin();for(int repeat=0;repeat<8;repeat++)neon_phase_raster(&field);neon_phase_end();NCHK(fb[32*STRIDE+32]==once&&(once&255)>80,"phase surface opacity resolves once even when eight faces overlap");
  field.color=RGB(40,0,0);rect(0,0,W,H,RGB(8,12,24));neon_phase_begin();neon_phase_raster(&field);neon_phase_end();NCHK((fb[32*STRIDE+32]&255)<(once&255),"near phase opacity is stronger than distant opacity");neon_depth_mode=old_mode;surface_y_min=lo;surface_y_max=hi;
 }
 /* All twelve real models, entry restrictions, save identity and grids. */
 game_init(&game);story_complete(&game);game.erl.inbox=8;int parked=game.ship,roster_ok=1;
 for(int ship=0;ship<12;ship++){
  roster_ok&=erl_racer_select(&game,ship)&&erl_racer_id(&game,0)==ship&&game.ship==parked&&(game.erl.inbox&8);
  for(int seed=0;seed<24;seed++){game.race.seed=seed;unsigned seen=1u<<ship;for(int actor=1;actor<=10;actor++){int id=erl_racer_id(&game,actor);roster_ok&=id>=0&&id<12&&!(seen&(1u<<id));seen|=1u<<id;}}
  erl_racer_open(0);erl_racer_cursor=ship;erl_racer_picker_draw();char name[40];snprintf(name,sizeof(name),"racer-%02d.bmp",ship+1);dump_native_bmp(name);erl_racer_picker=0;
 }
 NCHK(roster_ok,"all 12 racers select without changing parked ship/notices; 24 seeded grids per racer contain distinct racer-only opponents");
 int saved_flags=game.erl.inbox;NCHK(!erl_racer_select(&game,-1)&&!erl_racer_select(&game,12)&&game.erl.inbox==saved_flags,"invalid racer selections cannot change registration");
 game.erl.championship=1;NCHK(!erl_racer_select(&game,0)&&erl_racer_id(&game,0)==11,"season locks registered racer");game.erl.championship=0;game.erl.event_kind=ERL_PRACTICE;NCHK(!erl_racer_select(&game,0),"accepted event locks registered racer");game.erl.event_kind=0;
 NCHK(save_game(&game,"erl-racer-v53-test.sav")&&load_game(&game,"erl-racer-v53-test.sav")&&erl_racer_id(&game,0)==11&&game.ship==parked&&(game.erl.inbox&8),"racer identity round-trips through existing commander save without replacing spacefaring ship");
 game.erl.inbox=8;NCHK(erl_racer_id(&game,0)==0,"legacy saves without racer bits receive Kestrel");
 erl_racer_open(1);erl_racer_cursor=7;erl_racer_input(PSP_CTRL_CIRCLE);NCHK(!erl_quick_race&&!erl_racer_picker&&game.ship==parked,"canceling racer selection leaves the commander intact");
 erl_quick_racer_choice=0;
 NCHK(erl_quick_race_start(),"real Race 1 practice initializes");if(!erl_quick_race)goto finish;
 {unsigned *expected=malloc(STRIDE*H*sizeof(unsigned));int equal=expected!=0;
  if(expected)for(int mode=0;mode<3;mode++){
   for(int y=0;y<H;y++)for(int x=0;x<W;x++)surface_depth[y*W+x]=(mode==0||((x/16+y/16)%3==mode))?65535:123;
   for(int i=0;i<STRIDE*H;i++)fb[i]=0xff102030u;memcpy(expected,fb,STRIDE*H*sizeof(unsigned));
   neon_review_old_cloud_sky();unsigned *actual=fb;fb=expected;race_neon_cloud_sky();equal&=memcmp(actual,expected,STRIDE*H*sizeof(unsigned))==0;fb=actual;
  }
  NCHK(equal,"visible-node cloud sampling preserves every pixel with full sky and staggered occlusion masks");free(expected);
 }
 {Game *saved=malloc(sizeof(Game));NCHK(saved&&load_game(saved,"legacy-v37.sav"),"existing v37 commander checkpoint imports into expanded racer storage");free(saved);}
 {Game *saved=malloc(sizeof(Game));NCHK(saved&&save_game(&game,"field15.sav")&&load_game(saved,"field15.sav")&&saved->race.rivals==14&&saved->race.arcade.actor[14].health==RA_HEALTH,"all fifteen actors survive v39 save and reload");free(saved);}
 input(0,0,.016f,0,0);SpaceRace *r=&game.race;float scale=race_neon_scale(r),edge=RACE_NEON_EDGE*2.5f*scale,body=race_neon_body_radius(&game);
 {Vec3 axis[3]={{1,0,0},{0,1,0},{0,0,1}},origin=add(game.pos,(Vec3){340,70,920});RacerView view=racer_view(origin,axis[0],axis[1],axis[2],1.75f,0);int exact=1;
  for(int i=0;i<8;i++){Vec3 p={i&1?125:-125,i&2?30:-6,i&4?140:-70};Vec3 expected=camera(&game,add(origin,mul(p,1.75f)));exact&=length(sub(racer_point(&view,p),expected))<1;}
  NCHK(exact,"cached racer axes preserve transformed hull vertices");}
 /* Native grid restart and unique physical pickup placement. */
 {unsigned slots=0;int unique=1,grid=1;for(int attempt=0;attempt<14;attempt++){race_restart(&game);slots|=1u<<(r->place-1);for(int a=0;a<=r->rivals;a++)for(int b=a+1;b<=r->rivals;b++){Vec3 pa=a?r->pilot[a-1].pos:game.pos,pb=b?r->pilot[b-1].pos:game.pos;grid&=length(sub(pa,pb))>480;}}
  for(int a=0;a<RA_PICKUPS;a++)for(int b=a+1;b<RA_PICKUPS;b++)unique&=length(sub(r->arcade.pickup[a].pos,r->arcade.pickup[b].pos))>300;
  NCHK(slots&&(slots&(slots-1))&&grid,"restarts vary player grid slot without overlapping any racers");NCHK(unique,"all 24 boxes occupy distinct physical course locations instead of overlapping between laps");}
 int tested=0,contained=1,roof_ok=1,penalty_free=1,magnetic=-1,ordinary=-1;
 for(int k=0;k<RACE_GATES/r->laps;k++)if(race_track_surface(r,k,.45f)==0){ordinary=k;break;}
 for(int k=0;k<RACE_GATES/r->laps;k++)if(race_track_surface(r,k,.45f)==1){
  if(magnetic<0)magnetic=k;tested++;
  for(int side=-1;side<=1;side+=2){neon_review_pose(k,side*(edge+body),0);Vec3 from=game.pos;race_tick(&game,from,.016f);
   RaceTrackFrame f=race_track_nearest(r,r->gate,game.pos);contained&=fabsf(dot(sub(game.pos,f.centre),f.right))+body<=edge+2;
   penalty_free&=r->repair==0&&!game.dead&&r->arcade.actor[0].health==RA_HEALTH;
  }
  neon_review_pose(k,0,edge+body);race_tick(&game,game.pos,.016f);
  RaceTrackFrame f=race_track_nearest(r,r->gate,game.pos);roof_ok&=dot(sub(game.pos,f.centre),f.up)+body<=RACE_TRACK_FLOOR+edge+2;
  penalty_free&=r->repair==0&&!game.dead;
 }
 {neon_review_pose(ordinary,0,180*scale);game.speed=r->top_speed*.6f;RaceTrackFrame before=race_track_nearest(r,r->gate,game.pos);float altitude=dot(sub(game.pos,before.centre),before.up);for(int tick=0;tick<60;tick++)erl_input(0,PSP_CTRL_CROSS,1.f/60,0,0);RaceTrackFrame after=race_track_nearest(r,r->gate,game.pos);NCHK(fabsf(dot(sub(game.pos,after.centre),after.up)-altitude)<12,"neutral lift holds ordinary-track altitude for one second while racing");}
 NCHK(tested>=4,"test covers multiple banked magnetic sections");NCHK(contained,"whole player hull is contained at both painted road edges");NCHK(roof_ok,"magnetic curved roof stops upward escape");NCHK(penalty_free,"phase contact adds no arbitrary crash/reset or health penalty");
 if(ordinary>=0){float wide=190.f*2.5f*scale;neon_review_pose(ordinary,wide+body,0);race_tick(&game,game.pos,.016f);RaceTrackFrame of=race_track_nearest(r,r->gate,game.pos);int ordinary_side=fabsf(dot(sub(game.pos,of.centre),of.right))+body<=wide+2;
  neon_review_pose(ordinary,0,wide+body);race_tick(&game,game.pos,.016f);of=race_track_nearest(r,r->gate,game.pos);int ordinary_roof=dot(sub(game.pos,of.centre),of.up)+body<=RACE_TRACK_FLOOR+wide+2;
  NCHK(ordinary_side&&ordinary_roof,"normal road has the same hull-safe cylindrical phase enclosure");
  of=neon_review_pose(ordinary,0,wide*.42f);Vec3 heading=forward(&game);float before_height=dot(sub(game.pos,of.centre),of.up);for(int i=0;i<8;i++)erl_input(0,PSP_CTRL_DOWN,1.f/60,0,0);
  of=race_track_nearest(r,r->gate,game.pos);float after_height=dot(sub(game.pos,of.centre),of.up);NCHK(after_height<before_height-4&&dot(forward(&game),of.up)>=dot(heading,of.up)-.001f,"held Down lowers the craft on normal road without pitching its nose into the floor");}
 if(magnetic<0)goto finish;
 /* Both lateral directions stay relative to the helix road normal. */
 {int aligned=1;for(int k=0;k<RACE_GATES/r->laps;k++)if(race_track_surface(r,k,.45f)==1)for(int steer=-1;steer<=1;steer++){
   neon_review_pose(k,0,0);for(int tick=0;tick<8;tick++)game_tick(&game,1.f/60,steer,0,0,0);RaceTrackFrame f=race_track_nearest(r,r->gate,game.pos);Vec3 rr,uu,aa;flight_camera_basis(&rr,&uu,&aa);aligned&=dot(uu,f.up)>.97f;
  }NCHK(aligned,"helix steering keeps hull up aligned to track up in both directions");}
 {neon_review_pose(ordinary,0,0);for(int i=0;i<RA_PICKUPS;i++)r->arcade.pickup[i].cooldown=5;r->arcade.pickup[0].pos=game.pos;r->arcade.pickup[0].cooldown=0;r->arcade.actor[0].protection=0;
  race_tick(&game,game.pos,.01f);NCHK(r->arcade.pickup[0].cooldown>4.99f,"collected power-up starts a five-second respawn");
  Game *loaded=malloc(sizeof(Game));int saved=loaded&&save_game(&game,"neon54-pickup.sav")&&load_game(loaded,"neon54-pickup.sav")&&loaded->race.arcade.pickup[0].cooldown>4.99f;free(loaded);NCHK(saved,"five-second pickup timers survive commander save validation and reload");
  r->arcade.pickup[0].pos=add(game.pos,(Vec3){100000,0,0});for(int i=0;i<19;i++)race_tick(&game,game.pos,.25f);NCHK(r->arcade.pickup[0].cooldown>.20f,"pickup remains unavailable before five seconds");race_tick(&game,game.pos,.25f);NCHK(r->arcade.pickup[0].cooldown==0,"pickup becomes available at five seconds");}
 {neon_review_pose(ordinary,0,0);third_person=1;flight_camera_prepare();float before=length(sub(camera_origin(&game),game.pos));game.boost=1;
  for(int i=0;i<40;i++){r->elapsed+=1.f/60;flight_camera_prepare();}float after=length(sub(camera_origin(&game),game.pos));NCHK(after>before+40&&race_camera_boost>.99f,"boost eases to wider FOV and moves the ship farther ahead of the camera");neon_review_capture("neon54-boost.bmp");game.boost=0;
  for(int i=0;i<70;i++){r->elapsed+=1.f/60;flight_camera_prepare();}NCHK(race_camera_boost<.01f&&fabsf(length(sub(camera_origin(&game),game.pos))-before)<1,"boost camera eases back to its original chase mount");}
 /* Dedicated racing controls must not inherit the space-flight bindings. */
 RaceTrackFrame f=neon_review_pose(magnetic,0,0);float stopped=game.speed;erl_input(0,PSP_CTRL_CROSS,.1f,0,0);float accelerated=game.speed;erl_input(0,0,.1f,0,0);NCHK(accelerated>stopped&&game.speed<accelerated,"held X accelerates and releasing it slows the ship");
 r->reserve=100;r->boost_lock=0;game.boost=0;erl_input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);NCHK(game.boost,"one held R press starts boost without a double tap");erl_input(0,0,.016f,0,0);NCHK(!game.boost,"releasing R ends manual boost");
 game.speed=r->top_speed*.5f;float before_brake=game.speed;erl_input(0,PSP_CTRL_LTRIGGER,.1f,0,0);NCHK(game.speed<before_brake,"held L brakes");
 erl_input(PSP_CTRL_SQUARE,PSP_CTRL_RIGHT,.016f,1,0);int drift_started=r->drift_input>0;erl_input(0,PSP_CTRL_RIGHT,.016f,1,0);int drift_latched=r->drift_input>0;
 for(int i=0;i<60;i++)erl_input(0,0,1.f/60,0,0);NCHK(drift_started&&drift_latched&&!r->drift_input,"one Square tap starts a smooth timed drift that continues after release and then settles");
 f=race_track_nearest(r,r->gate,game.pos);float before_down=dot(forward(&game),f.up);erl_input(0,PSP_CTRL_DOWN,.1f,0,0);f=race_track_nearest(r,r->gate,game.pos);NCHK(dot(forward(&game),f.up)>=before_down-.001f,"holding Down cannot pitch the ship nose-first into the track");
 f=neon_review_pose(ordinary,0,0);Vec3 before_turn=forward(&game);float before_roll=game.roll;game_tick(&game,1.f/60,1,0,0,0);float ordinary_turn=acosf(fmaxf(-1,fminf(1,dot(before_turn,forward(&game))))),ordinary_roll=game.roll-before_roll;
 f=neon_review_pose(magnetic,0,0);before_turn=forward(&game);before_roll=game.roll;game_tick(&game,1.f/60,1,0,0,0);float magnetic_turn=acosf(fmaxf(-1,fminf(1,dot(before_turn,forward(&game))))),magnetic_roll=game.roll-before_roll;
 NCHK(ordinary_turn>.015f&&ordinary_turn<.04f&&magnetic_turn>.015f&&magnetic_turn<.04f&&fabsf(ordinary_roll)<.04f&&fabsf(magnetic_roll)<.04f,"normal and magnetic road stay inside the same steering and bank response band");
 f=neon_review_pose(ordinary,0,0);r->drift_input=1;before_turn=forward(&game);before_roll=game.roll;game_tick(&game,1.f/60,1,0,0,0);ordinary_turn=acosf(fmaxf(-1,fminf(1,dot(before_turn,forward(&game)))));ordinary_roll=game.roll-before_roll;
 f=neon_review_pose(magnetic,0,0);r->drift_input=1;before_turn=forward(&game);before_roll=game.roll;game_tick(&game,1.f/60,1,0,0,0);magnetic_turn=acosf(fmaxf(-1,fminf(1,dot(before_turn,forward(&game)))));magnetic_roll=game.roll-before_roll;r->drift_input=0;
 NCHK(ordinary_turn>.001f&&ordinary_turn<.1f&&magnetic_turn>.001f&&magnetic_turn<.1f&&fabsf(ordinary_roll)<.08f&&fabsf(magnetic_roll)<.08f,"normal and magnetic road both use the bounded tap-drift response");
 f=neon_review_pose(magnetic,0,0);game_tick(&game,1.f/20,0,0,1,0);Vec3 neutral_pos=game.pos,neutral_dir=forward(&game);
 int attached=1;for(int climb=-1;climb<=1;climb+=2){neon_review_pose(magnetic,0,0);game_tick(&game,1.f/20,0,climb,1,0);f=race_track_nearest(r,r->gate,game.pos);attached&=length(sub(game.pos,neutral_pos))<.1f&&dot(forward(&game),neutral_dir)>.9999f&&fabsf(dot(sub(game.pos,f.centre),f.up)-RACE_TRACK_FLOOR-race_floor_support(&game,0,f)-35)<2;}
 NCHK(attached,"magnetic road ignores climb and dive while keeping the craft attached");
 int hold_ok=1;neon_review_pose(magnetic,edge-body-5,0);game.speed=r->top_speed*.7f;
 for(int i=0;i<90;i++){game_tick(&game,1.f/60,1,0,1,0);f=race_track_nearest(r,r->gate,game.pos);if(race_track_surface(r,f.section,f.t)!=1)break;
  hold_ok&=fabsf(dot(sub(game.pos,f.centre),f.right))+body<=edge+3&&r->repair==0;
 }
 NCHK(hold_ok,"sustained outward acceleration stays inside the magnetic wall");
 f=neon_review_pose(magnetic,0,0);third_person=1;flight_camera_prepare();Vec3 cr,cu,ca;camera_axes(&game,&cr,&cu,&ca);float baseline=dot(ca,f.up);
 float pitch=game.pitch;game.pitch=pitch+.6f;flight_camera_prepare();camera_axes(&game,&cr,&cu,&ca);int stable=fabsf(dot(ca,f.up)-baseline)<.001f&&dot(ca,f.ahead)>.8f;
 game.pitch=pitch-.6f;flight_camera_prepare();camera_axes(&game,&cr,&cu,&ca);stable&=fabsf(dot(ca,f.up)-baseline)<.001f&&dot(ca,f.ahead)>.8f;
 NCHK(stable,"climb/dive input does not pitch the chase camera away from course forward");NCHK(dot(sub(camera_origin(&game),game.pos),f.ahead)<0,"camera mount remains behind the craft");
 {neon_review_pose(ordinary,0,100*scale);RaceItemActor *a=&r->arcade.actor[0];a->item=RA_BOOST;a->ammo=0;a->require_release=a->previous_x=0;a->rocket=a->protection=a->shuffle=a->spin=0;
  erl_input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,1.f/60,0,0);NCHK(a->rocket>.5f&&a->rocket<=.65f,"Circle activates a short boost item burst");
  for(int tick=0;tick<60;tick++)erl_input(0,0,1.f/60,0,0);NCHK(a->rocket==0,"boost item expires within one second");}
 /* The renderer and gameplay consume one generated ring definition. Sweep
  * the real hull through it and verify the compact shared boost contract. */
 {float arc=neon_sky_rings[1][0]*(RACE_GATES/r->laps);int section=(int)arc;RaceTrackFrame ring=race_track_frame(r,section,arc-section);Vec3 centre=add(ring.centre,mul(ring.up,RACE_TRACK_FLOOR+neon_sky_rings[1][1]*scale));
  game.erl.serial++;r->gate=section;r->elapsed=2;r->boost_time=0;game.cue=0;
  game.yaw=atan2f(ring.ahead.x,ring.ahead.z);game.pitch=asinf(ring.ahead.y);Vec3 br={cosf(game.yaw),0,-sinf(game.yaw)},bu=norm(race_ui_cross(ring.ahead,br));game.roll=atan2f(-dot(ring.up,br),dot(ring.up,bu));
  game.speed=r->top_speed*.7f;game.pos=add(centre,mul(ring.ahead,80));race_tick(&game,add(centre,mul(ring.ahead,-80)),.016f);
  NCHK(r->boost_time>.60f&&r->boost_time<=.65f&&game.cue==SFX_BOOST,"flying through a blue ring gives the same audible short boost blast as a floor pad");
  const RaceBoostVisual *flash=race_boost_visual(&game);NCHK(flash&&flash->ring&&length(sub(flash->pos,centre))<.01f,"real ring contact creates one localized cyan flash at the ring");
  neon_review_compact_capture("neon62-ring-contact.bmp");
  float age=r->elapsed;r->elapsed+=.31f;NCHK(!race_boost_visual(&game),"contact flash expires in300ms without extending boost duration");r->elapsed=age;
  r->boost_time=0;game.cue=0;race_tick(&game,add(centre,mul(ring.ahead,-80)),.016f);NCHK(r->boost_time==0&&!game.cue,"one ring cannot retrigger repeatedly during the same race");}
 {float q=13.349596f;RaceTrackFrame lip=race_track_frame(r,(int)q,q-(int)q);game.erl.serial++;r->gate=lip.section;r->elapsed=3;game.speed=r->top_speed*1.4f;game.yaw=atan2f(lip.ahead.x,lip.ahead.z);game.pitch=asinf(lip.ahead.y);game.pos=add(lip.centre,add(mul(lip.up,RACE_TRACK_FLOOR+race_floor_support(&game,0,lip)+35),mul(lip.ahead,120)));Vec3 from=add(game.pos,mul(lip.ahead,-620));race_tick(&game,from,1.f/60);
  NCHK(dot(forward(&game),lip.ahead)>.64f&&game.speed<=r->top_speed*1.121f,"fast jump launch remains aligned with the authored lip and bounded in speed");}
 for(int level=0;level<3;level++){neon_review_pose(ordinary,0,level*160*scale);char name[48];snprintf(name,sizeof(name),"neon56-altitude-%d.bmp",level);neon_review_capture(name);NCHK(render_submitted<2048,"elevated city/cloud view stays inside geometry queue");unsigned presented=neon_present_frames;snprintf(name,sizeof(name),"neon58-compact-altitude-%d.bmp",level);neon_review_compact_capture(name);NCHK(neon_present_frames==presented+1,"PSP GE composes the real scene and native HUD at staged altitude");}

 {neon_review_pose(ordinary,0,0);int anchor=3;float t=0,lane=0;while(anchor<RACE_GATES&&!race_neon_pad(r,anchor,&t,&lane))anchor++;
  RaceTrackFrame pad=race_track_frame(r,anchor,t);r->gate=anchor;game.erl.serial++;game.yaw=atan2f(pad.ahead.x,pad.ahead.z);game.pitch=asinf(pad.ahead.y);
  Vec3 centre=add(pad.centre,add(mul(pad.right,lane),mul(pad.up,RACE_TRACK_FLOOR+race_floor_support(&game,0,pad)+35)));
  memset(r->arcade.branch_boost,0,sizeof(r->arcade.branch_boost));memset(r->arcade.branch_repair,0,sizeof(r->arcade.branch_repair));game.pos=add(centre,mul(pad.ahead,20*scale));r->boost_time=0;
  race_tick(&game,sub(centre,mul(pad.ahead,300*scale)),.016f);const RaceBoostVisual *flash=race_boost_visual(&game);
  NCHK(r->boost_time>.60f&&flash&&!flash->ring,"swept floor-pad collection still gives short boost and its own localized glow burst");neon_review_compact_capture("neon62-pad-contact.bmp");
 }
 /* PSP regressions: a long displayed frame must retain real race time;
  * edge-triggered drift must not re-arm on each small physics step. */
 {neon_review_pose(ordinary,0,0);r->phase=RACE_COUNTDOWN;r->countdown=4;erl_quick_race_release=0;
  for(int i=0;i<10;i++)erl_input(0,0,.12f,0,0);
  NCHK(fabsf(r->countdown-2.8f)<.002f,"ten120ms displayed frames advance countdown by1.2 real seconds");
  neon_review_pose(ordinary,0,0);float before=r->elapsed;erl_input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE|PSP_CTRL_CROSS,.12f,.7f,0);
  NCHK(fabsf(r->elapsed-before-.12f)<.002f&&fabsf(erl_drift_time-.70f)<.002f,"long ERL frame advances full elapsed time with one drift tap across small physics steps");
  before=r->elapsed;erl_input(PSP_CTRL_START,PSP_CTRL_START,.12f,0,0);erl_input(0,0,.12f,0,0);
  NCHK(game.erl.paused&&r->elapsed==before,"pausing never runs catch-up simulation or scripted steering");game.erl.paused=0;
 }
 {RaceProjectile a={.pos={123,456,789},.dir={.25f,.5f,.75f},.kind=RA_MINIGUN,.live=1},b={0};ErlReplayShot compact=erl_replay_shot_pack(&a);erl_replay_shot_unpack(&b,&compact);
  NCHK(!memcmp(&a.pos,&b.pos,sizeof(Vec3))&&!memcmp(&a.dir,&b.dir,sizeof(Vec3))&&a.kind==b.kind&&a.live==b.live&&sizeof(compact)==28,"compact replay preserves exact visible shot fields and all eight rounds while freeing12480 bytes");}
 {neon_review_pose(ordinary,0,0);third_person=1;flight_camera_prepare();neon_frame_begin();
  int lo=surface_y_min,hi=surface_y_max,mode=neon_depth_mode,depth=surface_depth_on,cx=clipx1,cy=clipy1;
  surface_y_min=0;surface_y_max=H-1;clipx1=W;clipy1=H;neon_depth_mode=surface_depth_on=1;
  memset(r->arcade.shot,0,sizeof(r->arcade.shot));RaceProjectile *p=&r->arcade.shot[0];p->live=1;p->kind=RA_MINIGUN;p->dir=neon_ahead;p->pos=add(neon_eye,add(mul(neon_ahead,900),mul(neon_right,80)));Vec3 position=p->pos;
  unsigned bg=RGB(9,17,29);rect(0,0,W,H,bg);memset(surface_depth,255,sizeof(surface_depth));race_neon_projectile_art();int pixels=0,owned=0;
  for(int y=0;y<H;y++)for(int x=0;x<W;x++){pixels+=fb[y*STRIDE+x]!=bg;owned+=surface_depth[y*W+x]!=65535;}
  NCHK(pixels>=8&&owned>=8&&!memcmp(&position,&p->pos,sizeof(position)),"minigun round has a solid3D depth-tested silhouette without changing its physics");dump_native_bmp("neon62-3d-bullet.bmp");
  rect(0,0,W,H,bg);memset(surface_depth,0,sizeof(surface_depth));race_neon_projectile_art();int hidden=1;for(int y=0;y<H;y++)for(int x=0;x<W;x++)hidden&=fb[y*STRIDE+x]==bg;
  NCHK(hidden,"nearer opaque road and ships fully hide the3D round");memset(r->arcade.shot,0,sizeof(r->arcade.shot));
  surface_y_min=lo;surface_y_max=hi;clipx1=cx;clipy1=cy;neon_depth_mode=mode;surface_depth_on=depth;camera_override_clear();
 }
 {neon_review_pose(ordinary,0,0);third_person=1;flight_camera_prepare();neon_frame_begin();int sizes[3]={0};float distances[3]={1800,1000,600};
  int lo=surface_y_min,hi=surface_y_max;surface_y_min=0;surface_y_max=H-1;RaceTrackFrame ring={.right=neon_right,.up=neon_up};
  for(int n=0;n<3;n++){rect(0,0,W,H,RGB(9,17,29));memset(surface_depth,255,sizeof(surface_depth));neon_ring_draw(ring,add(neon_eye,mul(neon_ahead,distances[n])),ERL_BOOST_RING_RADIUS);int left=W,right=-1;
   for(int y=0;y<H;y++)for(int x=0;x<W;x++)if(fb[y*STRIDE+x]!=RGB(9,17,29)){if(x<left)left=x;if(x>right)right=x;}sizes[n]=right-left+1;
   char name[40];snprintf(name,sizeof(name),"neon62-ring-distance-%d.bmp",(int)distances[n]);dump_native_bmp(name);
  }
  NCHK(sizes[0]>0&&sizes[1]>sizes[0]&&sizes[2]>sizes[1],"ring opening and glow grow monotonically from1800 to1000 to600 units");surface_y_min=lo;surface_y_max=hi;camera_override_clear();
 }
 /* Cached/uncached CPU aliases must feed identical complete native HUDs. */
 {unsigned *reference=malloc(W*H*4);int exact=reference!=0;neon_review_pose(ordinary,0,0);neon_hud_cached=0;neon_review_compact_capture("neon61-uncached-hud.bmp");
  if(reference)for(int y=0;y<H;y++)memcpy(reference+y*W,(unsigned *)0x44000000u+y*STRIDE,W*4);
  neon_hud_cached=1;neon_review_compact_capture("neon61-cached-hud.bmp");
  if(reference)for(int y=0;y<H;y++)exact&=memcmp(reference+y*W,(unsigned *)0x44000000u+y*STRIDE,W*4)==0;
  NCHK(exact,"cached and uncached HUD aliases produce identical480x272 scene and HUD pixels");free(reference);
 }
 {Game *watch=malloc(sizeof(Game));NCHK(watch!=0,"hardware recorder obtains an isolated real player fixture");
  if(watch){neon_review_pose(ordinary,0,0);game.erl.paused=paused=0;*watch=game;memset(&erl_hw,0,sizeof(erl_hw));
   for(int i=0;i<4;i++){erl_hw_begin();erl_hw_end();sceDisplayWaitVblankStart();}
   FILE *f=fopen("erl-hardware-timing.txt","r");int absent=f==0;if(f)fclose(f);
   NCHK(absent&&memcmp(watch,&game,sizeof(Game))==0,"hardware timing performs no active-race file writes or authoritative state/input changes");
   game.erl.paused=1;erl_hw_end();f=fopen("erl-hardware-timing.txt","r");char log[1400]={0};if(f){fread(log,1,sizeof(log)-1,f);fclose(f);}
   NCHK(strstr(log,"CLOCK cpu_mhz=")&&strstr(log,"DISPLAY vblank_")&&!erl_hw.dirty,"pausing flushes actual CPU clocks frame intervals and display cadence once");
   unsigned intervals=erl_hw.intervals;game.erl.paused=0;erl_hw_begin();NCHK(erl_hw.intervals==intervals,"resuming excludes paused time from the measured race interval");game=*watch;free(watch);memset(&erl_hw,0,sizeof(erl_hw));
  }
 }
 neon_review_pose(magnetic,0,0);neon_review_capture("neon39-magnetic.bmp");
 for(int side=-1;side<=1;side+=2){neon_review_pose(magnetic,side*(edge-body-20),0);neon_review_capture(side<0?"neon54-left-wall.bmp":"neon54-right-wall.bmp");}neon_review_pose(magnetic,0,0);
 for(int i=0;i<RA_PICKUPS;i++)r->arcade.pickup[i].cooldown=10;
 f=race_track_nearest(r,r->gate,game.pos);r->arcade.pickup[0].cooldown=0;r->arcade.pickup[0].section=magnetic;r->arcade.pickup[0].pos=add(game.pos,add(mul(f.ahead,900),mul(f.up,110)));r->elapsed=2.1f;
 neon_review_capture("neon57-blue-box.bmp");NCHK(render_submitted<2048,"blue-box scene remains inside geometry queue capacity");
 #include "erl-polish-review.h"
 radio_station=4;radio_off=1;radio_dirty=0;race_ui_sync();NCHK(radio_race_on==2&&radio_display_station()==RADIO_DANCE_STATION&&radio_station==4&&radio_off,"ERL selects Dance without changing saved tuning/off preference");
 erl_quick_race_leave();race_ui_sync();NCHK(!radio_race_on&&radio_display_station()==4&&radio_off,"leaving ERL restores original radio preference");
 {RadioSettings legacy={0x52414449u,4,9,5,8,0,0};legacy.check=radio_checksum(&legacy);FILE *p=fopen("neon39-radio.cfg","wb");if(p){fwrite(&legacy,1,sizeof(legacy),p);fclose(p);}
  NCHK(radio_load_settings("neon39-radio.cfg")&&radio_off,"v4 OFF marker remains OFF after Dance is added");radio_tune(RADIO_DANCE_STATION);
  NCHK(radio_save_settings("neon39-radio.cfg")&&radio_load_settings("neon39-radio.cfg")&&!radio_off&&radio_station==RADIO_DANCE_STATION,"Dance selection round-trips in v5 independent preferences");
  remove("neon39-radio.cfg");remove("neon39-radio.cfg.bak");remove("neon39-radio.cfg.tmp");radio_dirty=0;
 }
 radio_scan_music();NCHK(radio_track_count[RADIO_DANCE_STATION]==4,"all four converted Dance files are discovered");
 /* No mixer thread runs during decoder validation; this thread is sole owner. */
 sceKernelSetCompiledSdkVersion(0x06000010);if(!mp3_modules_loaded){sceUtilityLoadModule(PSP_MODULE_AV_AVCODEC);sceUtilityLoadModule(PSP_MODULE_AV_MP3);mp3_modules_loaded=1;}
 mp3_resource_ready=sceMp3InitResource()>=0;NCHK(mp3_resource_ready,"PSP MP3 decoder resource initializes");
 for(int i=0;i<radio_track_count[RADIO_DANCE_STATION]&&mp3_resource_ready;i++){
  int ok=mp3_open_track(RADIO_DANCE_STATION,i),blocks=0,nonzero=0;unsigned samples=0;uint64_t worst=0;
  while(ok&&blocks<20000){uint64_t t=sceKernelGetSystemTimeWide();int decoded=mp3_decode_block();uint64_t elapsed=sceKernelGetSystemTimeWide()-t;if(elapsed>worst)worst=elapsed;if(!decoded)break;
   blocks++;samples+=mp3_frames;for(int n=0;n<mp3_frames*mp3_channels;n+=127)nonzero|=mp3_pcm[n]!=0;
  }
  NCHK(ok&&blocks>100&&blocks<20000&&nonzero&&mp3_rate==44100&&mp3_channels==2&&!radio_file_error,"native PSP decoder reads complete Dance track with real nonzero stereo PCM");
  fprintf(out,"TRACK %s blocks=%d samples=%u seconds=%.3f worst_decode_ms=%.3f error=%d\n",radio_track_label(radio_tracks[RADIO_DANCE_STATION][i]),blocks,samples,samples/44100.0,worst*.001,radio_file_error);int previous=radio_file_track;NCHK(mp3_shuffle_track(RADIO_DANCE_STATION)&&radio_file_track!=previous,"Dance playlist advances to a different available track after EOF");mp3_close_track();
 }
 if(mp3_resource_ready){sceMp3TermResource();mp3_resource_ready=0;}
finish:
 if(erl_quick_race)erl_quick_race_leave();fprintf(out,"CHECKS %d\nRESULT %d failures\n",checks,failures);fclose(out);fb=old;free(pixels);return 1;
 #undef NCHK
}
#endif
