/* Sparse emission bloom. Sample the untouched scene into the flushed queue
 * arena before compositing small radial halos. No allocation or full blur. */
static void __attribute__((noinline,optimize("O2"))) race_neon_bloom(void){
 unsigned *emission=render_scratch.deck;int count=0;
 for(int y=2;y<neon_render_height-2;y+=8)for(int x=2;x<neon_render_width-2;x+=8){
  unsigned best=0;int peak=220,bx=x,by=y;
  for(int dy=0;dy<4;dy+=2)for(int dx=0;dx<4;dx+=2){
   unsigned c=fb[(y+dy)*STRIDE+x+dx];int red=c&255,green=(c>>8)&255,blue=(c>>16)&255,m=red>green?red:green;if(blue>m)m=blue;
   if(m>peak){peak=m;best=c;bx=x+dx;by=y+dy;}
  }
  if(best){emission[count++]=bx|(by<<16);emission[count++]=best;}
 }
 /* Reuse the radial kernel's colour products. The old inner loop repeated
  * them for every pixel, although the 81 taps have only 14 strengths. */
 static const unsigned char distance[14]={0,1,2,4,5,8,9,10,13,16,17,18,20,25};
 static const signed char kernel[11][11]={
  {-1,-1,-1,-1,-1,13,-1,-1,-1,-1,-1},
  {-1,-1,13,12,10,9,10,12,13,-1,-1},
  {-1,13,11,8,7,6,7,8,11,13,-1},
  {-1,12,8,5,4,3,4,5,8,12,-1},
  {-1,10,7,4,2,1,2,4,7,10,-1},
  {13,9,6,3,1,0,1,3,6,9,13},
  {-1,10,7,4,2,1,2,4,7,10,-1},
  {-1,12,8,5,4,3,4,5,8,12,-1},
  {-1,13,11,8,7,6,7,8,11,13,-1},
  {-1,-1,13,12,10,9,10,12,13,-1,-1},
  {-1,-1,-1,-1,-1,13,-1,-1,-1,-1,-1}
 };
 for(int n=0;n<count;n+=2){
  int x=emission[n]&65535,y=emission[n]>>16;unsigned c=emission[n+1];
  unsigned tint[14];for(int i=0;i<14;i++){unsigned strength=26-distance[i];tint[i]=((((c&0xff00ff)*strength)>>8)&0xff00ff)|((((c&0xff00)*strength)>>8)&0xff00);}
  int dx0=x<5?-x:-5,dx1=x+5>=W?W-x-1:5,dy0=y<5?-y:-5,dy1=y+5>=H?H-y-1:5;
  for(int dy=dy0;dy<=dy1;dy++)for(int dx=dx0;dx<=dx1;dx++){
   int k=kernel[dy+5][dx+5];if(k<0)continue;unsigned rb=tint[k]&0xff00ff,g=tint[k]&0xff00;
   unsigned *pixel=&fb[(y+dy)*STRIDE+x+dx],old=*pixel;rb+=old&0xff00ff;g+=old&0xff00;
   unsigned overflow=rb&0x1000100;rb=(rb|(overflow-(overflow>>8)))&0xff00ff;if(g>0xff00)g=0xff00;*pixel=0xff000000|rb|g;
  }
 }
}
