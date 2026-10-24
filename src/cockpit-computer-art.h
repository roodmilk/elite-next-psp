/* Native font8 glyphs at their original eight-pixel advance. */
#define HC_BG RGB(5,14,21)
#define HC_CYAN RGB(106,225,216)
#define HC_GOLD RGB(249,193,103)
#define HC_DIM RGB(139,161,169)
#define HC_EDGE RGB(76,112,119)
static int hc_layout_errors=0;
static void hc_fast_rect(int x,int y,int w,int h,unsigned ink){
 int x0=x<0?0:x,y0=y<0?0:y,x1=x+w>W?W:x+w,y1=y+h>H?H:y+h;if(x1<=x0||y1<=y0)return;
 for(int yy=y0;yy<y1;yy++){unsigned*out=fb+yy*STRIDE+x0;int n=x1-x0;for(;n>=8;n-=8,out+=8){out[0]=ink;out[1]=ink;out[2]=ink;out[3]=ink;out[4]=ink;out[5]=ink;out[6]=ink;out[7]=ink;}while(n--)*out++=ink;}
}
typedef struct{signed char x,y;} HcRingPoint;
static HcRingPoint hc_rings[25][256];static unsigned short hc_ring_count[25];
static void hc_circle_fast(int cx,int cy,int r,unsigned ink){
 if(r<1||r>24){circle(cx,cy,r,ink);return;}
 if(!hc_ring_count[r]){unsigned char bitmap[49*49]={0};for(int i=0;i<64;i++){float a=i*6.2831853f/64,b=(i+1)*6.2831853f/64;int x=(int)(cosf(a)*r),y=(int)(sinf(a)*r),xx=(int)(cosf(b)*r),yy=(int)(sinf(b)*r);int dx=abs(xx-x),sx=x<xx?1:-1,dy=-abs(yy-y),sy=y<yy?1:-1,e=dx+dy;for(;;){bitmap[(y+24)*49+x+24]=1;if(x==xx&&y==yy)break;int e2=e*2;if(e2>=dy){e+=dy;x+=sx;}if(e2<=dx){e+=dx;y+=sy;}}}
  int n=0;for(int y=-r;y<=r;y++)for(int x=-r;x<=r;x++)if(bitmap[(y+24)*49+x+24]){if(n==256){hc_ring_count[r]=65535;break;}hc_rings[r][n++]=(HcRingPoint){x,y};}if(hc_ring_count[r]!=65535)hc_ring_count[r]=n;
 }
 if(hc_ring_count[r]==65535){circle(cx,cy,r,ink);return;}for(int k=0;k<hc_ring_count[r];k++)pixel(cx+hc_rings[r][k].x,cy+hc_rings[r][k].y,ink);
}

static void hc_words(int x,int y,int width,unsigned ink,const char*s){
 int cap=width/8,remaining=(int)strlen(s);
 for(int row=0;row<2&&remaining;row++){
  int cut=remaining<cap?remaining:cap;
  if(remaining>cap)for(int k=cut;k>cap/2;k--)if(s[k]==' '){cut=k;break;}
  char line_text[64];int copy=cut<63?cut:63;memcpy(line_text,s,copy);line_text[copy]=0;draw_text_pixels(x,y+row*9,ink,line_text);s+=cut;while(*s==' ')s++;remaining=(int)strlen(s);
 }
 if(remaining)hc_layout_errors++;
}
static void hc_scroll(int x,int y,int width,unsigned ink,const char*s,float phase){
 int n=(int)strlen(s);if(!n)return;int moving=n*8>width,offset=moving?(int)(fmaxf(0,phase)*16)%((n+3)*8):0,start=offset/8,sub=offset%8,count=moving?width/8+2:n;
 for(int k=0;k<count;k++){int px=x+k*8-sub,index=moving?(start+k)%(n+3):k;unsigned ch=index<n?(unsigned char)s[index]:' ';if(ch<32||ch>126)ch='?';
  for(int row=0;row<8;row++)for(int col=0;col<8;col++)if(px+col>=x&&px+col<x+width&&(font8[ch-32][row]&(128u>>col)))pixel(px+col,y+row,ink);
 }
}
static void hc_frame(int x,int y,int w,int h,unsigned edge){
 hc_fast_rect(x+3,y,w-6,3,HC_BG);hc_fast_rect(x+3,y+h-3,w-6,3,HC_BG);hc_fast_rect(x,y+3,w,h-6,HC_BG);
 line(x+4,y,x+w-5,y,edge);line(x+4,y+h-1,x+w-5,y+h-1,HC_EDGE);
 line(x,y+4,x,y+h-5,HC_EDGE);line(x+w-1,y+4,x+w-1,y+h-5,edge);
 line(x,y+4,x+4,y,edge);line(x+w-5,y,x+w-1,y+4,edge);
 line(x,y+h-5,x+4,y+h-1,HC_EDGE);line(x+w-5,y+h-1,x+w-1,y+h-5,HC_EDGE);
 line(x+5,y+2,x+w-6,y+2,RGB(30,50,56));
 pixel(x+3,y+h-4,HC_DIM);pixel(x+w-4,y+h-4,HC_DIM);
 line(x+2,y+6,x+2,y+h-7,RGB(22,43,49));line(x+w-3,y+6,x+w-3,y+h-7,RGB(12,27,34));
 for(int yy=y+11;yy<y+h-8;yy+=19){pixel(x+1,yy,RGB(74,90,94));pixel(x+w-2,yy+1,RGB(32,57,62));}
}
static void hc_key(int x,int y,char key,int pressed){
 y+=pressed?1:0;
 if(key=='l'||key=='r'){
  rect(x+2,y,15,9,RGB(25,35,43));line(x+2,y,x+14,y,HC_DIM);line(x,y+2,x,y+8,HC_DIM);line(x+16,y+2,x+16,y+8,HC_DIM);line(x,y+8,x+16,y+8,HC_DIM);text_px(x+5,y+1,WHITE,key=='l'?"L":"R");return;
 }
 hc_circle_fast(x+6,y+7,6,RGB(0,4,8));hc_circle_fast(x+6,y+6,6,HC_DIM);hc_circle_fast(x+6,y+6,5,RGB(25,35,43));button_icon(x+1,y+1,key,WHITE);
}
static void hc_direction(int x,int y,char dir,int active){
 unsigned ink=active?HC_GOLD:HC_DIM;rect(x-3,y-3,7,7,RGB(27,42,49));
 int dx=dir=='R'?1:dir=='L'?-1:0,dy=dir=='D'?1:dir=='U'?-1:0;static const int shape[5][2]={{-4,-4},{4,-4},{4,1},{0,5},{-4,1}};
 for(int k=0;k<5;k++){int j=(k+1)%5,u=shape[k][0],v=shape[k][1],uu=shape[j][0],vv=shape[j][1];line(x-u*dy+v*dx,y+u*dx+v*dy,x-uu*dy+vv*dx,y+uu*dx+vv*dy,ink);}
 button_icon(x-5,y-5,dir,ink);
}
static void hc_slot_dots(int x,int y,int selected){
 int capacity=fit_capacity(game.ship,FIT_WPN);if(capacity>8)capacity=8;
 for(int bank=0;bank<capacity;bank++){
  unsigned ink=bank*6==selected?HC_GOLD:bank*6==game.active_weapon?HC_CYAN:HC_DIM;int px=x+bank*7;
  rect(px,y,4,4,ink);if(!fit_is_active_weapon(game.fit[bank*6]))rect(px+1,y+1,2,2,HC_BG);
 }
}
static void hc_tool_caption(int x,int y,int action){
 const char *reason=hc_tool_reason(action);int ready=!strcmp(reason,"READY")||!strcmp(reason,"PULLING")||!strcmp(reason,"ALIGNING");
 text_px(x,y,ready?HC_GOLD:RGB(81,101,112),"%s",tool_names[action]);
 text_px(x,y+11,ready?HC_DIM:RGB(125,139,145),"%s",reason);
}
static void tools_panel(void){
 if(!tools_context()||!tools_open)return;
 uint64_t hc_tool_stamp=sceKernelGetSystemTimeWide();
 /* Armoured cross with four directional arms and two short shoulder bays. */
 const char*missile=hc_tool_reason(0),*flare=hc_tool_reason(1);int mr=!strcmp(missile,"READY"),fr=!strcmp(flare,"READY");
 hc_frame(245,41,109,23,HC_EDGE);hc_key(250,47,'l',0);text_px(275,46,mr?HC_GOLD:RGB(81,101,112),"MISSILES");
 if(mr)text_px(275,55,HC_DIM,"%d LEFT",game.missiles);else text_px(275,55,HC_DIM,"%s",missile);
 hc_frame(365,41,109,23,HC_EDGE);hc_key(370,47,'T',0);text_px(395,46,fr?HC_GOLD:RGB(81,101,112),"FLARES");
 if(fr)text_px(395,55,HC_DIM,"%d/%d",game.flare_charges,flare_capacity(&game));else text_px(395,55,HC_DIM,"%s",flare);
 hc_frame(320,68,78,29,HC_EDGE);hc_frame(232,98,89,36,HC_EDGE);
 hc_frame(398,96,76,46,HC_EDGE);hc_frame(320,140,78,24,HC_EDGE);
 rect(321,96,76,44,HC_BG);line(336,97,344,90,HC_EDGE);line(374,90,382,97,HC_EDGE);
 hc_tool_caption(326,72,4);hc_tool_caption(237,104,2);text_px(237,125,HC_DIM,"%d M",(int)tech_tractor_range(&game));
 text_px(324,143,!strcmp(hc_tool_reason(3),"READY")?HC_GOLD:HC_DIM,"HEAT SINK");text_px(324,154,HC_DIM,"%s",hc_tool_reason(3));
 int capacity=fit_capacity(game.ship,FIT_WPN),next=hc_next_weapon_slot(),item=next>=0?game.fit[next]:FIT_EMPTY;
 text_px(402,98,next<0?HC_DIM:HC_GOLD,capacity==1?"ONLY GUN":"NEXT GUN");
 hc_words(402,109,72,item==FIT_EMPTY?HC_DIM:HC_CYAN,fit_weapon_short_name(item));
 int dots=capacity>8?8:capacity;hc_slot_dots(403+(66-(dots*7-3))/2,134,next);
 hc_key(353,106,'O',1);hc_circle_fast(359,112,17,RGB(31,62,70));
 int pulse=(int)(game.time*12)%3;for(int k=0;k<3;k++)rect(350+k*8,129,3,2,k==pulse?HC_GOLD:HC_EDGE);
 hc_direction(359,96,'U',tools_confirm_action==4&&tools_confirm_time>0);
 hc_direction(329,113,'L',tools_confirm_action==2&&tools_confirm_time>0);
 hc_direction(389,113,'R',tools_confirm_action==3&&tools_weapon_result&&tools_confirm_time>0);
 hc_direction(359,137,'D',tools_confirm_action==3&&!tools_weapon_result&&tools_confirm_time>0);
 hc_profile_us[4]+=sceKernelGetSystemTimeWide()-hc_tool_stamp;
}
