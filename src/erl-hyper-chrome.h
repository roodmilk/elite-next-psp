/* Hyper Circuit: native integer geometry, no textures or framebuffer effects. */
static void erl_backdrop(void){
 rect(0,0,W,H,EB);for(int i=0;i<220;i++)pixel((i*167+13)%480,(i*83+17)%272,i%13?RGB(36,50,78):RGB(92,119,153));
 static short arcs[5][65][2];static int ready;if(!ready){for(int a=0;a<5;a++)for(int i=0;i<65;i++){float t=-1.5707963f+i*.0490874f;arcs[a][i][0]=105+3*a+cosf(t)*(265-a*15);arcs[a][i][1]=125+3*a+sinf(t)*(225-a*15);}ready=1;}
 static const unsigned inks[]={RGB(40,37,80),RGB(55,41,100),RGB(89,48,120),RGB(153,74,160),RGB(241,85,185)};
 for(int a=0;a<5;a++)for(int i=1;i<65;i++)line(arcs[a][i-1][0],arcs[a][i-1][1],arcs[a][i][0],arcs[a][i][1],inks[a]);
}
static void erl_chamfer(int x,int y,int w,int h,unsigned fill,unsigned edge){
 int cut=6;rect(x+cut,y,w-cut*2,h,fill);rect(x,y+cut,w,h-cut*2,fill);
 for(int i=1;i<cut;i++){rect(x+cut-i,y+i,w-cut*2+i*2,1,fill);rect(x+cut-i,y+h-1-i,w-cut*2+i*2,1,fill);}
 line(x+cut,y,x+w-1-cut,y,edge);line(x+cut,y+h-1,x+w-1-cut,y+h-1,edge);
 line(x,y+cut,x,y+h-1-cut,edge);line(x+w-1,y+cut,x+w-1,y+h-1-cut,edge);
 line(x+cut,y,x,y+cut,edge);line(x+w-1-cut,y,x+w-1,y+cut,edge);
 line(x,y+h-1-cut,x+cut,y+h-1,edge);line(x+w-1,y+h-1-cut,x+w-1-cut,y+h-1,edge);
}
/* Exact 47px round markers from the approved championship concept. */
static void erl_round_node(int cx,int cy,unsigned fill,unsigned edge){
 static unsigned char outer[47],inner[47];static int ready;
 if(!ready){for(int y=-23;y<=23;y++){int x=23;while(x*x+y*y>23*23)x--;outer[y+23]=x;int inset=21;if(y<-21||y>21)inner[y+23]=255;else{while(inset*inset+y*y>21*21)inset--;inner[y+23]=inset;}}ready=1;}
 for(int row=0;row<47;row++){int x=outer[row];rect(cx-x,cy+row-23,x*2+1,1,edge);if(inner[row]!=255){x=inner[row];rect(cx-x,cy+row-23,x*2+1,1,fill);}}
}
static int erl_xp_goal(void){return erl_thresholds[game.erl.licence<1?1:game.erl.licence>3?3:game.erl.licence];}
static int erl_xp_remaining(void){return game.erl.licence>=4?0:(int)fmaxf(0,erl_xp_goal()-game.erl.xp);}
static int erl_xp_fill(int width){return game.erl.licence>=4?width:(int)(width*fminf(1,fmaxf(0,(float)game.erl.xp/fmaxf(1,erl_xp_goal()))));}
static void erl_xp_bar(int x,int y,int w){
 erl_box(x,y,w,9,EB,planet_rgb_mix(ER,EB,120));int span=w-6,fill=erl_xp_fill(span);
 float t=fmaxf(0,fminf(1,erl_motion_entry_age/.45f)),a=1-t;fill=(int)(fill*(1-a*a*a));
 for(int i=0;i<span;i+=8){int n=span-i<6?span-i:6;rect(x+3+i,y+2,n,5,planet_rgb_mix(EP,EC,35));int lit=fill-i;if(lit>n)lit=n;if(lit>0)rect(x+3+i,y+2,lit,5,ET);}
}
