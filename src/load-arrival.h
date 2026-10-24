/* Main-menu save arrival. Presentation only: no camera/simulation/save writes,
 * no extra framebuffer, no heap allocations. All times use the UI clock. */
#define LOAD_ESTABLISH 3.8f
#define LOAD_EXTERIOR_END 4.9f
#define LOAD_DECK_START 5.05f
#define LOAD_END 6.65f
static unsigned load_arrival_sounds=0;
static unsigned load_arrival_seed=0,load_arrival_serial=0;
static int load_arrival_nav_order[DECK_GROUPS];
static float load_arrival_sat(float t){return fminf(1.f,fmaxf(0.f,t));}
static void load_arrival_begin(void){
 load_arrival_time=0;load_arrival_sounds=0;load_arrival_release=0;
 load_arrival_seed=sun_hash(game.bodies[0].seed^(unsigned)(preview_time*1000.f)^++load_arrival_serial);
 for(int i=0;i<DECK_GROUPS;i++)load_arrival_nav_order[i]=i;
 unsigned shuffle=load_arrival_seed;
 for(int i=DECK_GROUPS-1;i>0;i--){shuffle=sun_hash(shuffle);int j=shuffle%(i+1),v=load_arrival_nav_order[i];load_arrival_nav_order[i]=load_arrival_nav_order[j];load_arrival_nav_order[j]=v;}
 deck_open_cancel();game.voice_time=game.message_time=0;game.cue=0;
}
static int load_arrival_input(unsigned pressed,unsigned held,float dt){
 (void)pressed;
 if(load_arrival_time<0){if(!load_arrival_release)return 0;if(!held)load_arrival_release=0;return 1;}
 if(page!=HOME){load_arrival_time=-1;load_arrival_release=1;return 1;}
 load_arrival_time+=fminf(.05f,fmaxf(0,dt));
 float t=load_arrival_time-LOAD_DECK_START;
 static const float delays[]={0,.48f,.95f};
 static const int sounds[]={SFX_POWER_BUS,SFX_POWER_RELAY,SFX_POWER_READY};
 for(int i=0;i<3;i++)if(t>=delays[i]&&!(load_arrival_sounds&(1u<<i))){load_arrival_sounds|=1u<<i;audio_play(sounds[i]);}
 if(load_arrival_time>=LOAD_END){load_arrival_time=-1;load_arrival_release=held!=0;}
 return 1;
}
static void load_arrival_dim(int x,int y,int w,int h,float amount){
 int a=(int)(256*load_arrival_sat(amount));if(a==256)return;
 for(int yy=y;yy<y+h;yy++)for(int xx=x;xx<x+w;xx++){unsigned c=fb[yy*STRIDE+xx];fb[yy*STRIDE+xx]=RGB(((c&255)*a)>>8,(((c>>8)&255)*a)>>8,(((c>>16)&255)*a)>>8);}
}
static void load_arrival_title_line(const char *s,int y){
 int n=(int)strlen(s),left=(W-n*12)/2;
 for(int i=0;i<n;i++){unsigned ch=(unsigned char)s[i];if(ch<32||ch>126)ch='?';for(int yy=0;yy<12;yy++)for(int xx=0;xx<12;xx++)if(font8[ch-32][yy*2/3]&(1u<<(7-xx*2/3))){pixel(left+i*12+xx+1,y+yy+1,RGB(1,4,9));pixel(left+i*12+xx,y+yy,RGB(238,228,198));}}
}
static void load_arrival_title(void){
 char name[96];snprintf(name,sizeof(name),"%s...",station_name(&game));
 for(int i=0;name[i];i++)if(i&&name[i-1]!=' '&&name[i]>='A'&&name[i]<='Z')name[i]+='a'-'A';
 load_arrival_title_line("Somewhere in",17);
 if(strlen(name)<=38)load_arrival_title_line(name,36);
 else {int cut=38;while(cut>18&&name[cut]!=' ')cut--;char tail[96];snprintf(tail,sizeof(tail),"%s",name+cut+(name[cut]==' '));name[cut]=0;load_arrival_title_line(name,36);load_arrival_title_line(tail,52);}
}
static void load_arrival_exterior(float t){
 if(drawcount)flush_meshes();
 int ox=proj_ox,oy=proj_oy,cx0=clipx0,cx1=clipx1,cy0=clipy0,cy1=clipy1;
 int old_depth=surface_depth_on,old_material=surface_material,old_city=mc_depth_active,old_valid=mega_depth_valid;
 int old_station=station_ge_active_frame,old_fleet=fleet_ge_active_frame,old_limit=render_triangle_limit,old_step=render_raster_step;
 surface_depth_on=mc_depth_active=mega_depth_valid=surface_material=station_ge_active_frame=fleet_ge_active_frame=0;
 render_triangle_limit=4096;render_raster_step=1;
 unsigned sky=RGB(3,8,17);rect(0,0,W,H,sky);preview_clip(W/2,164,0,0,W-1,H-1);
 float rush=load_arrival_sat((t-LOAD_ESTABLISH)/(LOAD_EXTERIOR_END-LOAD_ESTABLISH));rush=rush*rush*rush;
 float phase=.55f+t*.065f;StationProfile p=station_profile_for(&game,game.station_variant);
 for(int i=0;i<85;i++){unsigned s=sun_hash(game.bodies[0].seed+i*6151u);int x=((int)(s%W)+(int)(t*(i%3+1)))%W,y=(s>>9)%H;pixel(x,y,i%5?RGB(55,74,99):RGB(155,182,204));}
 int sx=245+(int)(70*sinf(.2f+t*.4f)),sy=158+(int)(12*sinf(t*.33f));
 unsigned sunlight=mix_rgb(game.bodies[0].color,RGB(255,225,161),.42f);
 /* Light is behind the actual silhouette, so every opaque panel masks it. */
 for(int yy=-58;yy<=58;yy++)for(int xx=-58;xx<=58;xx++){
  int d=xx*xx+yy*yy;if(d>=3364)continue;int weight=(3364-d)*108/3364;weight=weight*weight/108;
  sun_bloom_dot(sx+xx,sy+yy,dim_rgb(sunlight,weight,256),0,65,W,H);
 }
 for(int ray=0;ray<12;ray++){
  float a=ray*.5235988f+t*.035f;int length=22+(ray%3)*12;
  for(int k=7;k<length;k++)sun_bloom_dot(sx+cosf(a)*k,sy+sinf(a)*k,dim_rgb(sunlight,(length-k)*28/length,256),0,65,W,H);
 }
 for(int yy=-3;yy<=3;yy++)for(int xx=-3;xx<=3;xx++)if(xx*xx+yy*yy<=9)pixel(sx+xx,sy+yy,sunlight);
 unsigned unmasked[9];for(int i=0;i<9;i++)unmasked[i]=fb[(sy+i/3-1)*STRIDE+sx+i%3-1];
 menu_station_arrival_camera=1;
 menu_station_arrival=(MenuStationCamera){phase,-.28f+.025f*sinf(t*.45f),(p.station_class==STATION_MEGA?2.1f:game.station_variant?1.4f:1.7f)*(1+7*rush),5*sinf(t*.24f),1.2f*cosf(t*.28f)};
 if(!game.station_variant)menu_station_generated(0,64,W,208,phase);else menu_station_relay(W,208,phase);
 int visible=0;for(int i=0;i<9;i++)visible+=fb[(sy+i/3-1)*STRIDE+sx+i%3-1]==unmasked[i];
 static float glare,last_t=-1;float dt=t-last_t;last_t=t;if(dt<0||dt>.2f){glare=0;dt=0;}
 glare+=(visible/9.f-glare)*fminf(1,dt*9);
 if(glare>.01f){
  for(int k=-62;k<=62;k++)sun_bloom_dot(sx+k,sy,dim_rgb(sunlight,(int)((1-fabsf(k)/63.f)*glare*43),256),0,65,W,H);
  for(int ghost=1;ghost<=2;ghost++){int gx=sx+(240-sx)*ghost/2,gy=sy+(164-sy)*ghost/2;unsigned tint=ghost==1?RGB(91,174,198):RGB(203,139,90);for(int yy=-3;yy<=3;yy++)for(int xx=-3;xx<=3;xx++)if(xx*xx+yy*yy<10)sun_bloom_dot(gx+xx,gy+yy,dim_rgb(tint,(int)(glare*10),256),0,65,W,H);}
 }
 surface_depth_on=old_depth;surface_material=old_material;mc_depth_active=old_city;mega_depth_valid=old_valid;
 station_ge_active_frame=old_station;fleet_ge_active_frame=old_fleet;render_triangle_limit=old_limit;render_raster_step=old_step;
 menu_station_arrival_camera=0;preview_clip(ox,oy,cx0,cy0,cx1,cy1);
 load_arrival_title();
 float light=load_arrival_sat(t/.75f)*(1-load_arrival_sat((t-4.55f)/.35f));load_arrival_dim(0,0,W,H,light);
}
static float load_arrival_nav_light(int group,float t){
 int rank=0;for(int i=0;i<DECK_GROUPS;i++)if(load_arrival_nav_order[i]==group)rank=i;
 float age=t-(.12f+rank*.035f),settle=.85f+rank*.045f;
 if(age<=0)return 0;
 if(t>=settle)return load_arrival_sat((t-settle)/.16f);
 if(age<.07f)return .95f;
 if(age<.13f)return .025f;
 unsigned tick=(unsigned)((age-.13f)/.085f);
 return sun_hash(load_arrival_seed+(group+1)*7919u+tick*104729u)&1u?.95f:.025f;
}
static void load_arrival_monitor(float t){
 enum {X=232,Y=59,WIDTH=248,HEIGHT=135};
 if(t>=.92f)return;
 if(t<.32f){
  rect(X,Y,WIDTH,HEIGHT,0);
  float spread=load_arrival_sat((t-.16f)/.16f);int w=(int)(WIDTH*spread*spread);
  if(w>0){int x=X+(WIDTH-w)/2,y=Y+HEIGHT/2;unsigned glow=ui_paint_surface(RGB(125,206,196));line(x,y,x+w-1,y,WHITE);if(w>4){line(x+2,y-1,x+w-3,y-1,glow);line(x+2,y+1,x+w-3,y+1,glow);}}
  return;
 }
 float p=load_arrival_sat((t-.32f)/.46f),open=1-(1-p)*(1-p)*(1-p);
 int height=1+(int)((HEIGHT-1)*open),top=Y+(HEIGHT-height)/2;
 unsigned column[HEIGHT];
 /* Read each source column before writing it: bounded in-place compression
  * preserves the real sun/station art, with only 540 bytes of stack scratch. */
 for(int x=X;x<X+WIDTH;x++){
  for(int y=0;y<HEIGHT;y++)column[y]=fb[(Y+y)*STRIDE+x];
  for(int y=Y;y<Y+HEIGHT;y++)fb[y*STRIDE+x]=y>=top&&y<top+height?column[(y-top)*HEIGHT/height]:RGB(0,0,0);
 }
 load_arrival_dim(X,top,WIDTH,height,.52f+.48f*p);
 int a=(int)(100*(1-load_arrival_sat((t-.64f)/.28f)));
 if(a>0){int scan=top+(int)(height*load_arrival_sat((t-.32f)/.6f));if(scan>=Y+HEIGHT)scan=Y+HEIGHT-1;for(int x=X;x<X+WIDTH;x++)dc_blend(x,scan,RGB(194,235,220),a);}
}
static void load_arrival_dashboard(float t){
 /* One-time display power-up; ordinary menu rendering is untouched. */
 load_arrival_dim(0,0,W,28,t/.30f);
 load_arrival_dim(0,28,W,31,t/.32f);
 int groups[DECK_GROUPS],n=deck_visible_groups(groups),width=n?470/n:94;
 for(int col=0;col<n;col++){
  int x=8+col*width,w=width-6;float light=load_arrival_nav_light(groups[col],t);
  load_arrival_dim(x,31,w,20,light);
  if(t<1.2f&&light>.8f){line(x,31,x+w-1,31,UI_ACCENT);rect(x+7,46,3,2,WHITE);rect(x+w-10,46,3,2,WHITE);}
 }
 load_arrival_dim(0,59,232,23,(t-.23f)/.22f);
 int group=deck_group(row),ids[9],count=deck_fill(group,ids);
 int last=82;
 for(int i=0;i<count;i++){int y,h;if(!dc_row_geometry(group,ids[i],&y,&h))continue;int bottom=y+h;float delay=.26f+i*.09f;load_arrival_dim(0,last,232,bottom-last,(t-delay)/.22f);float spark=t-delay;if(spark>0&&spark<.16f){line(12,y,219,y,UI_ACCENT);rect(14+(int)(spark*1150),y,4,2,WHITE);}last=bottom;}
 if(last<248)load_arrival_dim(0,last,232,248-last,(t-1.f)/.25f);
 load_arrival_monitor(t);
 load_arrival_dim(232,194,248,54,(t-.72f)/.35f);
 load_arrival_dim(0,248,W,24,(t-1.12f)/.3f);
}
static void load_arrival_draw(void){
 if(load_arrival_time<0||page!=HOME)return;
 if(load_arrival_time<LOAD_EXTERIOR_END)load_arrival_exterior(load_arrival_time);
 else if(load_arrival_time<LOAD_DECK_START)rect(0,0,W,H,0);
 else load_arrival_dashboard(load_arrival_time-LOAD_DECK_START);
}
