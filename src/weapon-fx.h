/* Primary weapon signatures. These are deliberately screen-space and use the
 * existing soft framebuffer primitives: the PSP gets animated silhouettes,
 * muzzle light and impact colour without particles, textures or allocations. */
static void wfx_ray(int x0,int y0,int x1,int y1,unsigned ink,int core){
 unsigned haze=dim_rgb(ink,1,3);line(x0-1,y0,x1-1,y1,haze);line(x0+1,y0,x1+1,y1,haze);line(x0,y0,x1,y1,ink);if(core)line(x0,y0-1,x1,y1-1,WHITE);
}
static void wfx_muzzle(int x,int y,unsigned ink,int size){
 circle(x,y,size,dim_rgb(ink,1,2));circle(x,y,size>2?size-2:1,ink);pixel(x,y,WHITE);sfx_add(x,y,ink,view_top(),view_bot());
}
static void wfx_segment(int x0,int y0,int x1,int y1,float t,float span,unsigned ink){
 float a=fmaxf(0,t-span),b=fminf(1,t);int ax=x0+(int)((x1-x0)*a),ay=y0+(int)((y1-y0)*a),bx=x0+(int)((x1-x0)*b),by=y0+(int)((y1-y0)*b);wfx_ray(ax,ay,bx,by,ink,1);
}
static void wfx_pulses(int x0,int y0,int x1,int y1,float age,unsigned ink,int count,float span){
 for(int i=0;i<count;i++){float t=fmodf(age*1.45f+i/(float)count,1.f);wfx_segment(x0,y0,x1,y1,t,span,ink);}
}
static void wfx_zigzag(int x0,int y0,int x1,int y1,float age,unsigned ink,int bends,int amp){
 int px=x0,py=y0;for(int i=1;i<=bends;i++){float t=i/(float)bends;int x=x0+(int)((x1-x0)*t),y=y0+(int)((y1-y0)*t);if(i<bends){float wave=sinf(age*18+i*2.31f);x+=(int)(wave*amp);y+=(i&1)?amp:-amp;}line(px,py,x,y,dim_rgb(ink,1,2));line(px+1,py,x+1,y,ink);px=x;py=y;}
}
static void wfx_impact(int x,int y,unsigned ink,int radius){
 circle(x,y,radius,ink);circle(x,y,radius+3,dim_rgb(ink,1,2));line(x-radius-3,y,x-2,y,WHITE);line(x+2,y,x+radius+3,y,WHITE);sfx_add(x,y,ink,view_top(),view_bot());
}
static void weapon_fx_draw(int w,float age,int shot,int bot){
 int lx=50,rx=430,tx=240,ty=110;age=fmaxf(0,fminf(1,age));
 unsigned red=RGB(255,68,58),green=RGB(92,255,112),blue=RGB(76,160,255),violet=RGB(194,92,255),pink=RGB(255,92,205),amber=RGB(255,164,52),cyan=RGB(65,235,235),lime=RGB(176,255,72);
 switch(w){
  case 1: /* Pulse laser: compact red packets, not a continuous beam. */
   wfx_pulses(lx,bot,tx-3,ty,age,red,3,.10f);wfx_pulses(rx,bot,tx+3,ty,age+.12f,red,3,.10f);wfx_muzzle(lx,bot,red,3);wfx_muzzle(rx,bot,red,3);wfx_impact(tx,ty,red,3);break;
  case 2: /* Beam laser: stable ice-blue twin traces with a white core. */
   wfx_ray(lx,bot,tx-3,ty,cyan,1);wfx_ray(rx,bot,tx+3,ty,cyan,1);wfx_muzzle(lx,bot,cyan,4);wfx_muzzle(rx,bot,cyan,4);wfx_impact(tx,ty,cyan,4);break;
  case 20: /* Mining laser: amber cutting forks and a rotating drill crown. */
   wfx_ray(lx,bot,tx-5,ty,amber,0);wfx_ray(rx,bot,tx+5,ty,amber,0);for(int i=0;i<5;i++){int d=8+i*6,s=(i+(int)(age*8))&1;line(tx+(s?d:-d),ty-10-i*3,tx+(s?-d:d),ty-7-i*3,RGB(255,205,82));}wfx_impact(tx,ty,amber,7+(shot&1)*2);break;
  case 25: /* Heavy laser: thick crimson recoil with a white-hot spine. */
   wfx_ray(lx,bot,tx-7,ty,RGB(255,58,45),1);wfx_ray(lx+3,bot,tx-3,ty,red,0);wfx_ray(rx,bot,tx+7,ty,RGB(255,58,45),1);wfx_ray(rx-3,bot,tx+3,ty,red,0);wfx_muzzle(lx,bot,red,7-(int)(age*3));wfx_muzzle(rx,bot,red,7-(int)(age*3));wfx_impact(tx,ty,red,8);break;
  case 26: /* Rapid pulse: a dense stream of short gold bolts. */
   wfx_pulses(lx,bot,tx-2,ty,age,RGB(255,220,95),6,.055f);wfx_pulses(rx,bot,tx+2,ty,age+.08f,RGB(255,220,95),6,.055f);wfx_muzzle(lx,bot,RGB(255,220,95),2);wfx_muzzle(rx,bot,RGB(255,220,95),2);break;
  case 27: /* Lance: one long ivory needle with converging range rails. */
   line(lx,bot,tx,ty,RGB(110,105,80));line(rx,bot,tx,ty,RGB(110,105,80));wfx_ray(tx,bot,tx,ty,RGB(255,244,205),1);line(tx-8,ty,tx+8,ty,WHITE);line(tx,ty-8,tx,ty+8,WHITE);wfx_impact(tx,ty,RGB(255,225,160),5);break;
  case 28: /* Scatter: a short, hot cone whose geometry advertises its range. */
   for(int i=-3;i<=3;i++){int ex=tx+i*19,ey=ty+abs(i)*7;line(lx,bot,ex,ey,i?amber:WHITE);line(rx,bot,ex,ey,i?RGB(255,118,42):WHITE);}wfx_muzzle(lx,bot,amber,6);wfx_muzzle(rx,bot,amber,6);break;
  case 29: /* Ion projector: blue travelling globes with shield-drain rings. */
   wfx_zigzag(lx,bot,tx,ty,age,blue,6,4);wfx_zigzag(rx,bot,tx,ty,age+.3f,blue,6,4);for(int i=0;i<3;i++){float t=fmodf(age+i*.31f,1);int x=lx+(int)((tx-lx)*t),y=bot+(int)((ty-bot)*t);circle(x,y,3+i%2,blue);}wfx_impact(tx,ty,blue,8+(int)(age*5));break;
  case 30: /* Plasma cutter: paired magenta plasma knots, hot enough to bleed hull. */
   wfx_ray(lx,bot,tx-4,ty,pink,0);wfx_ray(rx,bot,tx+4,ty,pink,0);for(int side=0;side<2;side++){float t=fmodf(age*1.2f+side*.18f,1);int sx=side?rx:lx,x=sx+(int)((tx-sx)*t),y=bot+(int)((ty-bot)*t);circle(x,y,7,pink);circle(x,y,3,WHITE);}wfx_impact(tx,ty,pink,9);break;
  case 31: /* Disruptor: green interference arcs and a broken target waveform. */
   wfx_zigzag(lx,bot,tx,ty,age,lime,8,5);wfx_zigzag(rx,bot,tx,ty,age+.5f,lime,8,5);for(int i=-2;i<=2;i++)line(tx-14,ty+i*4,tx+14-(abs(i)*4),ty+i*4,i?dim_rgb(lime,1,2):WHITE);break;
  case 57: /* Red Beam */
   wfx_ray(lx,bot,tx-3,ty,red,1);wfx_ray(rx,bot,tx+3,ty,red,1);wfx_muzzle(lx,bot,red,4);wfx_muzzle(rx,bot,red,4);wfx_impact(tx,ty,red,5);break;
  case 58: /* Green Pulse */
   wfx_pulses(lx,bot,tx-2,ty,age,green,5,.075f);wfx_pulses(rx,bot,tx+2,ty,age+.1f,green,5,.075f);wfx_muzzle(lx,bot,green,3);wfx_muzzle(rx,bot,green,3);break;
  case 59: /* Blue Ion Lance */
   wfx_ray(tx,bot,tx,ty,blue,1);line(tx-2,bot,tx-1,ty,RGB(155,215,255));line(tx+2,bot,tx+1,ty,RGB(155,215,255));for(int r=4;r<=12;r+=4)circle(tx,ty,r+(int)(age*3),r==4?WHITE:blue);break;
  case 60: /* Violet Arc */
   wfx_zigzag(lx,bot,tx,ty,age,violet,10,7);wfx_zigzag(rx,bot,tx,ty,age+.7f,violet,10,7);wfx_muzzle(lx,bot,violet,4);wfx_muzzle(rx,bot,violet,4);wfx_impact(tx,ty,violet,6);break;
  case 61:{ /* Rainbow Prism: colour order rotates on every actual shot. */
   static const unsigned prism[3]={RGB(255,214,76),RGB(255,92,206),RGB(70,230,255)};int phase=shot%3;
   for(int i=0;i<3;i++){unsigned c=prism[(i+phase)%3];wfx_pulses(lx+i*2,bot,tx-6+i*6,ty,age+i*.12f,c,2,.16f);wfx_pulses(rx-i*2,bot,tx+6-i*6,ty,age+i*.12f,c,2,.16f);}wfx_impact(tx,ty,prism[phase],7);break;}
  case 62: /* Amber Burst: two visibly separated heavy packets. */
   for(int burst=0;burst<2;burst++){float t=fmodf(age*1.25f+burst*.42f,1);wfx_segment(lx,bot,tx-5,ty,t,.18f,amber);wfx_segment(rx,bot,tx+5,ty,t,.18f,RGB(255,205,94));}wfx_muzzle(lx,bot,amber,6);wfx_muzzle(rx,bot,amber,6);wfx_impact(tx,ty,amber,7);break;
  case 63: /* Cyan Ripple: coherent waves, not generic jagged lightning. */
   for(int side=0;side<2;side++){int sx=side?rx:lx,px=sx,py=bot;for(int i=1;i<=18;i++){float t=i/18.f;int x=sx+(int)((tx-sx)*t),y=bot+(int)((ty-bot)*t+sinf(t*25-age*16+(side?3.14f:0))*7);line(px,py,x,y,cyan);px=x;py=y;}}wfx_impact(tx,ty,cyan,6);break;
  case 64: /* White Rail: brilliant central needle and expanding shock rings. */
   line(lx,bot,tx,ty,RGB(100,125,145));line(rx,bot,tx,ty,RGB(100,125,145));wfx_ray(tx,bot,tx,ty,WHITE,1);for(int r=4;r<17;r+=6)circle(tx,ty,r+(int)(age*7),r==4?WHITE:RGB(145,205,255));line(tx-15,ty,tx+15,ty,WHITE);break;
  case 65: /* Orange Flare Array */
   for(int i=-4;i<=4;i++){int ex=tx+i*18,ey=ty+abs(i)*6;wfx_segment(i&1?rx:lx,bot,ex,ey,fmodf(age+i*.07f+1,1),.24f,i?amber:WHITE);}wfx_muzzle(lx,bot,amber,7);wfx_muzzle(rx,bot,amber,7);break;
  case 66: /* Pink Phase: packets blink between two offset trajectories. */
   for(int i=0;i<5;i++){float t=fmodf(age+i*.2f,1);int x=lx+(int)((tx-lx)*t),y=bot+(int)((ty-bot)*t);if(((i+shot)&1)==0){circle(x,y,5,pink);pixel(x,y,WHITE);}x=rx+(int)((tx-rx)*t);if(((i+shot)&1)!=0){circle(x,y,5,RGB(205,105,255));pixel(x,y,WHITE);}}wfx_impact(tx,ty,pink,10+(shot&1)*3);break;
  case 67: /* Lime Shard: a broad fan of discrete, fast anti-fighter darts. */
   for(int i=-5;i<=5;i++){int ex=tx+i*16,ey=ty+abs(i)*5;float t=fmodf(age*1.7f+(i+5)*.073f,1);wfx_segment(i<0?lx:rx,bot,ex,ey,t,.08f,i?lime:WHITE);}break;
  case 68: /* Blackstar: dark core surrounded by an unstable violet corona. */
   wfx_zigzag(lx,bot,tx,ty,age,RGB(112,62,190),9,3);wfx_zigzag(rx,bot,tx,ty,age+.4f,RGB(112,62,190),9,3);circle(tx,ty,13+(int)(age*4),violet);circle(tx,ty,8,RGB(50,24,78));circle(tx,ty,4,RGB(12,8,22));line(tx-18,ty,tx-7,ty,violet);line(tx+7,ty,tx+18,ty,violet);line(tx,ty-18,tx,ty-7,violet);line(tx,ty+7,tx,ty+18,violet);break;
  default:wfx_ray(lx,bot,tx-3,ty,red,0);wfx_ray(rx,bot,tx+3,ty,red,0);break;
 }
}
static void weapon_fx_fire(void){
 if(game.shot<=0)return;
 int w=fit_weapon_item(&game);float cycle=weapon_cycle(&game),window=fminf(.18f,fmaxf(.07f,cycle*.65f));
 if(game.shot<=fmaxf(0,cycle-window))return;
 float age=(cycle-game.shot)/window;weapon_fx_draw(w,age,game.shots,view_bot());
}
