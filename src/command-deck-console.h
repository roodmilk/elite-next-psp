/* Approved Observation Lounge 6D. All geometry is native 480x272.
 * Presentation only: service IDs, availability and dispatch remain canonical. */
#define DC_INK (ship_theme_index()?UI_TEXT:RGB(226,231,215))
#define DC_TEAL (ship_theme_index()?UI_ACCENT:RGB(122,196,193))
#define DC_GOLD (ship_theme_index()?UI_ACCENT:RGB(229,175,103))
#define DC_DIM (ship_theme_index()?UI_MUTED:RGB(147,167,172))
#define DC_PANEL (ship_theme_index()?UI_PANEL:RGB(16,31,45))
static float dc_sat(float p){return p<0?0:p>1?1:p;}
static float dc_ease(float p){p=dc_sat(p);return 1-(1-p)*(1-p)*(1-p);}
static unsigned dc_rgb(unsigned p){int r=(p>>10)&31,g=(p>>5)&31,b=p&31;return RGB((r<<3)|(r>>2),(g<<3)|(g>>2),(b<<3)|(b>>2));}
static unsigned dc_mix(unsigned a,unsigned b,int t){return planet_rgb_mix(a,b,t);}
static void dc_blend(int x,int y,unsigned ink,int alpha){if(x<0||x>=W||y<0||y>=H)return;fb[y*STRIDE+x]=dc_mix(fb[y*STRIDE+x],ink,alpha);}
static void dc_box(int x,int y,int w,int h,unsigned fill,unsigned edge){rect(x,y,w,h,fill);line(x,y,x+w-1,y,edge);line(x,y+h-1,x+w-1,y+h-1,edge);line(x,y,x,y+h-1,edge);line(x+w-1,y,x+w-1,y+h-1,edge);}
static void dc_glass(int x,int y,int w,int h,unsigned ink,int opacity){for(int yy=y;yy<y+h;yy++)for(int xx=x;xx<x+w;xx++)dc_blend(xx,yy,ink,opacity);}
static const char *dc_copy[DECK_ITEMS]={
 "Fly at your own pace.","Your hold and local goods.","Choose your next system.","Find your next ship.","Buy modules for your ship.",
 "Your identity and career.","View the flight controls.","Meet the local factions.","Select ships or worlds.","Change cash or world state.",
 "Manage calls and displays.","View this system's worlds.","Find work at this station.","Review your active missions.","News and local Spacebook.",
 "Review your discoveries.","Choose music and sound.","See your mission's next step.","Review Guild assignments.","Choose HUD and chatter.",
 "Step aboard this station.","Manage your fitted modules.","Explore the galaxy's history.","Choose a finish or pattern.","Repair and refuel your ship.",
 "Return to the title screen.","Watch local broadcasts.","Invite one nearby PSP.","Join a nearby flight.","Online and local Wi-Fi.","Review and pay local fines.","Race, qualify and build your fleet."
};
static const char *dc_label(int id){
 static char local[32];
 static const char *names[DECK_ITEMS]={"Launch","Cargo & market","Galaxy.Maps","Shipyard","Outfitting","Commander Profile","Controls","Factions","Targeting computer","Debug tools","Comms panel","System details","Mission board","Mission log","GalacticNet","Discovery Codex","Radio & audio","Tracked mission","Explorers Guild","Display & chatter","Disembark","Ship loadout","Galactic Lore","PIMP-MY-SHIP.NET","Mechanics","Quit to main menu","Local TV","Host local flight","Join local flight","Multiplayer","LAW","ERL LEAGUE"};
 if(id<0||id>=DECK_ITEMS)return "";
 if(id==0&&!game.docked)return "Resume flight";
 if(id==1&&!game.docked)return "Cargo";
 if(id==3&&station_class(&game)==STATION_MEGA)return "Mega Shipyard";
 if(id==24&&opening_exists(&game))return "Engineers";
 if(id==26){snprintf(local,sizeof(local),"%.12s LOCAL TV",game.systems[game.system].name);return local;}
 return names[id];
}
/* Twelve-pixel silhouettes, hand drawn for their actual displayed size. */
enum { DC_ICON_MUSIC=10,DC_ICON_EXIT,DC_ICON_BOARD,DC_ICON_LOG,DC_ICON_MAP,DC_ICON_CODEX,DC_ICON_TV,DC_ICON_SYSTEM,DC_ICON_SHOE,DC_ICON_SHIP,DC_ICON_MEGA_SHIP,DC_ICON_COUNT };
static const char dc_icons[DC_ICON_COUNT][12][13]={
 {"............","..########..",".##########.",".##..##..##.",".##..##..##.",".##########.",".##..##..##.",".##..##..##.",".##########.","..########..","............","............"},
 {".....##.....",".....##.....","....####....","....####....","...######...","..########..",".####..####.","#####..#####","..###..###..","...#....#...","...#....#...","............"},
 {"..#..#..#...","..#..#..#...",".##########.","..#......#..","###.####.###","..#.####.#..","..#.####.#..","###.####.###","..#......#..",".##########.","..#..#..#...","..#..#..#..."},
 {".###.###.###",".#.#.#.#.#.#",".###.###.###","............",".###.###.###",".#.#.#.#.#.#",".###.###.###","............",".###.###.###",".#.#.#.#.#.#",".###.###.###","............"},
 {"..B.B.B.B...","..BBBBBBB...","..BBBBBBB...","..BBBBBBB...","..#######...","..#.....#...","..#######...","....###.....","....###.....","....###.....","....###.....",".....#......"},
 {"........##..",".......##...","......##..##","......##..##","......######",".....######.","....###.....","...###......","..###.......",".###........","###.........",".##........."},
 {".....#......",".....##.....","..#######...","..#######...",".....##.....",".....#......","............","..#######...","..#.....#...","..#######...","............","............"},
 {".##########.",".#........#.",".#.##.###.#.",".#........#.",".#.##.###.#.",".#........#.",".#.##.###.#.",".#........#.",".#.##.###.#.",".#........#.",".##########.","............"},
 {"..########..","..#......#..","###.####.#..","..#......#..","###.####.#..","..#......#..","###.####.#..","..#......#..","###.####.#..","..#......#..","..########..","............"},
 {"..##....##..",".####..####.",".####..####.","..##....##..","............",".####..####.","######.#####","######.#####","............","............","............","............"},
 /* Joined eighth notes: hollow space around the stems keeps them readable. */
 {"............","....#######.","....#######.","....#.....#.","....#.....#.","....#.....#.","....#.....#.","....#...###.","..###..####.",".####..###..",".###........","............"},
 /* Exit doorway and outward arrow. */
 {".######.....",".#....#.....",".#....#.....",".#....#..#..",".#....#..##.",".#..########",".#..########",".#....#..##.",".#....#..#..",".#....#.....",".######.....","............"},
 /* Pinned notices on a physical board. */
 {".##########.",".#........#.",".#..B..B..#.",".#.###.##.#.",".#.#.#.##.#.",".#.###....#.",".#.....B..#.",".#.##.###.#.",".#.##.#.#.#.",".#....###.#.",".#........#.",".##########."},
 /* Mission log: ticks alongside journal lines. */
 {"..#########.","..#.......#.","..#..#.##.#.","..#.#..##.#.","..##......#.","..#.......#.","..#..#.##.#.","..#.#..##.#.","..##......#.","..#.......#.","..#########.","............"},
 /* Spiral galaxy: two swept arms around a bright, recessed core. */
 {"...........#","......###...","...###...##.","..##...#..##",".##..####..#","##..##BB##..","#..##BB##...",".##.####..#.","..##...###..","...####.....",".#..........","............"},
 /* Open codex, central spine and separate lines on both pages. */
 {".###...###..","#...#.#...#.","#.##.#.##.#.","#....#....#.","#.##.#.##.#.","#....#....#.","#.##.#.##.#.","#....#....#.","#....#....#.",".###.#.###..","....###.....","............"},
 /* Antenna, inset screen, side controls and feet. */
 {"...#....#...","....#..#....",".....##.....",".##########.",".#.......##.",".#.#####.##.",".#.#...#.##.",".#.#####.##.",".#.......##.",".##########.","...#....#...","............"},
 /* Nearby sun, orbital arc and a shaded planet. */
 {".#..........","###....##...",".#..###..##.","...##......#","..#...###..#",".#...##B##..",".#...#BBB#..",".#...##B##..","..#...###...","..##....#...","....####....","............"},
 /* Walking shoe: ankle, alternating laces, rounded toe and solid sole. */
 {"..#####.....","..#BBB#.....","..#B#B#.....","..#BBB#.....","..#B#B#.....","..#BBB##....","..#BBBB###..","..#BBBBBB##.",".##BBBBBBB#.",".##########.",".##......##.","............"},
 /* Regular yard: swept wings, clear cockpit and twin engine outlets. */
 {".....##.....","....####....","....#BB#....","...##BB##...","...######...","..########..",".###.##.###.","####.##.####","###..##..###","...##..##...","...BB..BB...","............"},
 /* Mega yard: broad capital hull, raised bridge and four drive ports. */
 {".....##.....","....####....","..###BB###..",".####BB####.","###.####.###","###.####.###","############",".##########.","..###BB###..","..##.##.##..","..BB.BB.BB..","............"}
};
static int dc_icon_for(int id){switch(id){case 16:return DC_ICON_MUSIC;case 25:return DC_ICON_EXIT;case 12:return DC_ICON_BOARD;case 13:return DC_ICON_LOG;case 2:return DC_ICON_MAP;case 15:return DC_ICON_CODEX;case 26:return DC_ICON_TV;case 11:return DC_ICON_SYSTEM;case 20:return DC_ICON_SHOE;case 3:return station_class(&game)==STATION_MEGA?DC_ICON_MEGA_SHIP:DC_ICON_SHIP;case 1:return 0;case 0:return 1;case 4:return 2;case 21:return 3;case 23:return 4;case 24:return 5;case 17:return 6;case 14:return 7;case 22:case 6:return 8;case 7:case 29:case 5:return 9;case 10:return 7;case 30:return 9;default:return 2;}}
static void dc_icon(int x,int y,int id,unsigned ink){int n=dc_icon_for(id);for(int yy=0;yy<12;yy++)for(int xx=0;xx<12;xx++){char p=dc_icons[n][yy][xx];if(p=='#'||p=='B')pixel(x+xx,y+yy,p=='B'?DC_TEAL:ink);}}
static int dc_row_geometry(int group,int id,int *y,int *h){int vis[9],n=deck_fill(group,vis),step=n>6?22:25;for(int i=0;i<n;i++)if(vis[i]==id){*y=85+i*step;*h=n>6?20:22;return 1;}return 0;}
static int dc_copy_lines(const char *s,int draw){int line_n=0;while(*s){int len=(int)strlen(s),cut=len<26?len:26;if(len>26){int k=cut;while(k>0&&s[k]!=' ')k--;if(k>0)cut=k;}if(draw&&line_n<2)text_px(246,222+line_n*10,DC_INK,"%.*s",cut,s);line_n++;s+=cut;while(*s==' ')s++;}return line_n;}
static void dc_clipped_text(int x,int y,unsigned ink,const char *s){for(int i=0;s[i]&&i<21;i++){unsigned ch=(unsigned char)s[i];if(ch<32||ch>126)ch='?';for(int yy=0;yy<8;yy++)for(int xx=0;xx<8;xx++)if(font8[ch-32][yy]&(1<<(7-xx)))pixel(x+i*8+xx,y+yy,ink);}}
static unsigned dc_backdrop_rgb[ALMANAC_BACKDROP_W*ALMANAC_BACKDROP_H];
static int dc_backdrop_ready=0;
static void dc_background(void){
 float t=preview_time,phase=t*(6.283185307f/7.8f);int bright[16];for(int b=0;b<16;b++)bright[b]=(int)(12+10*sinf(phase+b*.7f));
 if(!dc_backdrop_ready){for(int i=0;i<ALMANAC_BACKDROP_W*ALMANAC_BACKDROP_H;i++){unsigned p=almanac_backdrop[i];dc_backdrop_rgb[i]=(p&0x8000)?dc_rgb(p):ui_paint_surface(RGB(4,11,23));}dc_backdrop_ready=1;}
 for(int y=24;y<248;y++){const unsigned *source=dc_backdrop_rgb+(y%ALMANAC_BACKDROP_H)*ALMANAC_BACKDROP_W;memcpy(fb+y*STRIDE,source,ALMANAC_BACKDROP_W*sizeof(unsigned));memcpy(fb+y*STRIDE+ALMANAC_BACKDROP_W,source,(W-ALMANAC_BACKDROP_W)*sizeof(unsigned));}
 /* Illuminate only the visible cosmos, not pixels hidden behind the buttons. */
 for(int y=58;y<199;y++)for(int x=224;x<412;x++){
  unsigned ink=fb[y*STRIDE+x];
  int r=ink&255,g=(ink>>8)&255,b=(ink>>16)&255;
  if(b>r+10&&b<120){int wave=bright[((x>>5)+(y>>5))&15];r+=r*wave/100;g+=g*wave/100;b+=b*wave/100;ink=RGB(r,g,b);}
  fb[y*STRIDE+x]=ink;
 }
 /* Fixed stars: only brightness and small haloes change, never positions. */
 for(int i=0;i<25;i++){unsigned h=sun_hash(3321u+i*7919u);int x=228+h%185,y=62+(h>>10)%132;float q=.5f+.5f*sinf(phase*(1+i%2)+i*1.7f);int a=(int)(65+180*q*q);dc_blend(x,y,RGB(213,234,244),a);if(i%4==0){dc_blend(x-1,y,DC_TEAL,a/4);dc_blend(x+1,y,DC_TEAL,a/4);dc_blend(x,y-1,DC_TEAL,a/4);dc_blend(x,y+1,DC_TEAL,a/4);}}
 /* Native sun art: less than one pixel of plasma drift, stable disc. */
 for(int y=0;y<154;y++){int dx=(int)roundf(.55f*sinf(y*.11f+phase)),glow=(int)(2*sinf(phase*2+y*.03f));for(int x=0;x<68;x++){int sx=x-dx;if(sx<0||sx>=154)continue;unsigned p=almanac_sun[y*154+sx];if(!(p&0x8000))continue;unsigned ink=dc_rgb(p);int r=(ink&255)+glow,g=((ink>>8)&255)+glow,b=((ink>>16)&255)+glow;if(r<0)r=0;if(r>255)r=255;if(g<0)g=0;if(g>255)g=255;if(b<0)b=0;if(b>255)b=255;pixel(412+x,40+y,RGB(r,g,b));}}
 if(game.docked){
  int kind=game.station_variant?STATION_POOR:station_class(&game),w=kind==STATION_MEGA?150:kind==STATION_RICH?118:64,h=kind==STATION_MEGA?96:kind==STATION_RICH?76:58,x=358-w/2,y=140-h/2;
  const uint16_t *art=kind==STATION_MEGA?almanac_station_mega:kind==STATION_RICH?almanac_station_rich:almanac_station_poor;draw_next_art(art,w,h,x,y,w,h);
  static const int ports[7][2]={{28,36},{41,42},{52,35},{68,40},{80,36},{91,35},{60,5}};
  for(int i=0;i<7;i++){int px=x+ports[i][0]*w/118,py=y+ports[i][1]*h/76;float q=.5f+.5f*sinf(phase*(1+i%2)+i*1.3f);unsigned lamp=i==6?RGB(244,194,127):RGB(154,220,205);dc_blend(px,py,lamp,(int)(55+175*q));dc_blend(px-1,py,lamp,(int)(12+35*q));dc_blend(px+1,py,lamp,(int)(12+35*q));dc_blend(px,py-1,lamp,(int)(7+15*q));dc_blend(px,py+1,lamp,(int)(7+15*q));}
 }else menu_space_view(240,72,232,118);
}
static void dc_status_name(char out[24]){snprintf(out,24,"%s",game.docked?station_name_for(&game,game.station_variant):game.systems[game.system].name);char *p=strstr(out," System Hub");if(p)snprintf(p,24-(int)(p-out)," Hub");}
static void dc_chrome(int group){
 rect(0,0,480,24,ui_paint_surface(RGB(16,32,43)));draw_next_art(next_logo_small,100,18,8,3,100,18);
 dc_box(270,2,202,19,game.docked?ui_paint_surface(RGB(24,61,52)):ui_paint_surface(RGB(35,45,57)),game.docked?ui_paint_surface(RGB(61,113,96)):ui_paint_surface(RGB(67,86,105)));line(272,3,469,3,ui_paint_surface(RGB(82,126,101)));
 int a=(int)(140+100*(.5f+.5f*sinf(preview_time*2.4166f)));unsigned lamp=dc_mix(ui_paint_surface(RGB(63,113,121)),RGB(155,224,189),a);int x=276,y=5;
 line(x,y,x,y+11,lamp);line(x,y,x+3,y,lamp);line(x,y+11,x+3,y+11,lamp);line(x+15,y,x+15,y+11,lamp);line(x+12,y,x+15,y,lamp);line(x+12,y+11,x+15,y+11,lamp);
 for(int yy=2;yy<=8;yy++){int half=(yy-2)*3/6;rect(x+7-half,y+yy,2+half*2,1,DC_INK);}rect(x+7,y+4,2,2,ui_paint_surface(RGB(74,135,148)));
 char name[24];dc_status_name(name);char status[48];snprintf(status,sizeof(status),"%s %s",game.docked?"DOCKED AT":"IN FLIGHT /",name);
 if(strlen(status)<=21)text_px(300,8,DC_TEAL,"%s",status);else{ text_px(300,3,DC_TEAL,game.docked?"DOCKED AT":"IN FLIGHT");int n=(int)strlen(name);text_px(300,12,DC_INK,n>21?"%.18s...":"%s",name);}
 int groups[5],n=deck_visible_groups(groups),width=n?470/n:94,oldcol=-1,newcol=0;
 for(int i=0;i<n;i++){if(groups[i]==deck_motion_from)oldcol=i;if(groups[i]==group)newcol=i;}
 float p=dc_ease((preview_time-deck_motion_start)/.18f),pos=newcol;if(oldcol>=0&&p<1)pos=oldcol+(newcol-oldcol)*p;
 for(int col=0;col<n;col++){int g=groups[col],xx=8+col*width,w=width-6;float strength=1-fabsf(col-pos);if(strength<0)strength=0;
  rect(xx+3,34,w,23,ui_paint_surface(RGB(8,17,28)));dc_glass(xx,31,w,20,dc_mix(ui_paint_surface(RGB(14,29,42)),ui_paint_surface(RGB(26,66,73)),(int)(255*strength)),230);
  unsigned edge=dc_mix(ui_paint_surface(RGB(43,76,86)),RGB(119,205,200),(int)(strength*(.72f+.1f*sinf(preview_time*2.6f))*255));line(xx,31,xx+w-1,31,edge);if(strength>.01f){line(xx,31,xx,50,edge);line(xx+w-1,31,xx+w-1,50,ui_paint_surface(RGB(66,110,120)));rect(xx+7,46,3,2,edge);rect(xx+w-10,46,3,2,edge);}
  text_px(xx+(w-(int)strlen(deck_groups[g])*8)/2,37,dc_mix(RGB(181,199,202),RGB(181,239,219),(int)(255*strength)),"%s",deck_groups[g]);
 }
 line(8,55,471,55,ui_paint_surface(RGB(36,66,79)));if(n){int marker=8+(int)(width*pos)+(width-36)/2;rect(marker,53,36,2,RGB(131,216,206));}
}
static void dc_button(int id,int y,int h,int selected,int offset){
 int x=12+offset; /* Explicit local clipping, including letters, for sliding banks. */
 unsigned fill=selected?ui_paint_surface(RGB(39,66,69)):ui_paint_surface(RGB(14,29,42)),edge=selected?DC_GOLD:ui_paint_surface(RGB(70,99,110));
 int x0=x<8?8:x,x1=x+208>224?224:x+208;if(x1<=x0)return;
 rect(x0,y,x1-x0,h,fill);line(x0,y,x1-1,y,edge);line(x0,y+h-1,x1-1,y+h-1,ui_paint_surface(RGB(35,62,72)));
 if(selected&&x>=8&&x<224){rect(x,y,2,h,DC_GOLD);float shine=fmodf(preview_time,3.2f)/.7f;if(shine<1){int sx=x+(int)(shine*184);for(int k=0;k<24;k++)if(sx+k>=8&&sx+k<224)pixel(sx+k,y+1,dc_mix(DC_GOLD,RGB(255,238,184),(int)(sinf(k*3.14159f/23)*255)));}}
 int ox0=clipx0,ox1=clipx1,oy0=clipy0,oy1=clipy1;clipx0=x0;clipx1=x1;clipy0=y;clipy1=y+h;
 dc_icon(x+10,y+(h-12)/2,id,selected?DC_GOLD:DC_DIM);dc_clipped_text(x+31,y+(h-8)/2,DC_INK,dc_label(id));clipx0=ox0;clipx1=ox1;clipy0=oy0;clipy1=oy1;
}
static void dc_bank(int group,int focus,int offset){
 int vis[9],n=deck_fill(group,vis),step=n>6?22:25;for(int i=0;i<n;i++)dc_button(vis[i],85+i*step,n>6?20:22,vis[i]==focus,offset);
}
static void dc_description(int id){
 rect(239,202,236,43,ui_paint_surface(RGB(5,14,24)));dc_box(236,199,236,43,DC_PANEL,ui_paint_surface(RGB(82,119,128)));line(241,200,468,200,ui_paint_surface(RGB(120,154,159)));rect(237,204,2,31,DC_GOLD);
 dc_icon(246,204,id,DC_GOLD);text_px(266,206,DC_GOLD,"%.24s",dc_label(id));line(246,218,462,218,ui_paint_surface(RGB(50,78,93)));dc_copy_lines(game.message_time>0&&game.message[0]?game.message:dc_copy[id],1);rect(461,207,2,2,ui_paint_surface(RGB(102,136,139)));rect(466,207,2,2,ui_paint_surface(RGB(102,136,139)));
}
static void dc_footer(void){
 rect(0,248,480,24,ui_paint_surface(RGB(16,31,43)));line(0,248,479,248,ui_paint_surface(RGB(63,98,102)));button_icon(8,255,'U',DC_INK);button_icon(19,255,'D',DC_INK);text_px(33,257,DC_INK,"SELECT");button_icon(95,255,'l',DC_INK);button_icon(107,255,'r',DC_INK);text_px(122,257,DC_INK,"SECTION");button_icon(192,255,'X',DC_INK);text_px(207,257,DC_INK,"OPEN");button_icon(256,255,'O',DC_INK);text_px(271,257,DC_INK,"BACK");
 char money[24];if(game.credits<100000000)snprintf(money,sizeof(money),"%.1f U",game.credits*.1f);else snprintf(money,sizeof(money),"%.2fM U",game.credits*.0000001f);text_px(472-(int)strlen(money)*8,257,DC_GOLD,"%s",money);
}
static void dc_home(void){
 deck_clamp_row();int group=deck_group(row);dc_background();dc_chrome(group);
 static const char *headings[]={"FLIGHT & NAVIGATION","SHIP SERVICES","WORK & CONTRACTS","DISCOVER","COMMANDER"};dc_glass(16,62,176,15,DC_PANEL,230);text_px(24,66,DC_TEAL,"%s",headings[group]);
 float p=dc_ease((preview_time-deck_motion_start)/.18f);
 if(p<1&&deck_motion_from!=group){dc_bank(deck_motion_from,deck_motion_row,-(int)(deck_motion_direction*216*p));dc_bank(group,row,(int)(deck_motion_direction*216*(1-p)));}else dc_bank(group,row,0);
 dc_description(row);dc_footer();
}
/* One clean source frame, rendered off-screen on activation. The live target
 * page is redrawn normally every frame, then shrunk into the expanding tile.
 * No replay feedback, second destination snapshot, scan bands or bright flash. */
#define DC_OPEN_SECONDS .27f
static struct {int enabled,next,id,y,h;float start;} dc_open;
static int deck_open_active(void){
 if(dc_open.enabled&&(page!=dc_open.next||preview_time-dc_open.start>=DC_OPEN_SECONDS))dc_open.enabled=0;
 return dc_open.enabled;
}
static int deck_open_pending(int next){return dc_open.enabled&&dc_open.next==next;}
static void deck_open_cancel(void){dc_open.enabled=0;}
static void deck_open_begin(int next){
 dc_open.enabled=0;
 if(next==FLIGHT||next==INTRO||!dc_row_geometry(deck_group(row),row,&dc_open.y,&dc_open.h))return;
 dc_open.id=row;dc_open.next=next;dc_open.start=preview_time;
 unsigned *destination=fb;fb=dc_open_source;dc_home();fb=destination;
 dc_open.enabled=1;
}
/* Compress the live frame in place. Rows above the fixed point go upwards;
 * rows below go downwards, so no destination overwrites a future source row.
 * A native scratch row handles horizontal overlap; X samples divide once. */
static void dc_open_window(int x0,int y0,int w,int h){
 unsigned row_pixels[W];unsigned short sx[W];
 for(int x=0;x<w;x++)sx[x]=x*W/w;
 int pivot=h==H?y0:(y0*H+H-h-1)/(H-h);if(pivot<y0)pivot=y0;if(pivot>y0+h)pivot=y0+h;
 for(int pass=0;pass<2;pass++){
  int first=pass?pivot:pivot-1,last=pass?y0+h:y0-1,step=pass?1:-1;
  for(int y=first;y!=last;y+=step){int sy=(y-y0)*H/h;memcpy(row_pixels,fb+sy*STRIDE,W*sizeof(unsigned));
   unsigned *dest=fb+y*STRIDE+x0;for(int x=0;x<w;x++)dest[x]=row_pixels[sx[x]];
  }
 }
 for(int y=0;y<H;y++){
  unsigned *dest=fb+y*STRIDE,*source=dc_open_source+y*STRIDE;
  if(y<y0||y>=y0+h)memcpy(dest,source,W*sizeof(unsigned));
  else {if(x0)memcpy(dest,source,x0*sizeof(unsigned));if(x0+w<W)memcpy(dest+x0+w,source+x0+w,(W-x0-w)*sizeof(unsigned));}
 }
}
static void deck_open_draw(void){
 if(!deck_open_active())return;
 float elapsed=preview_time-dc_open.start;
 if(elapsed<.07f){
  for(int y=0;y<H;y++)memcpy(fb+y*STRIDE,dc_open_source+y*STRIDE,W*sizeof(unsigned));
  float pulse=sinf(dc_sat(elapsed/.07f)*3.14159265f);int grow=(int)roundf(pulse*2);
  int x=12-grow,y=dc_open.y-grow,w=208+grow*2,h=dc_open.h+grow*2;
  dc_box(x,y,w,h,dc_mix(ui_paint_surface(RGB(39,66,69)),UI_RAISED,(int)(pulse*110)),UI_ACCENT);
  dc_icon(22,dc_open.y+(dc_open.h-12)/2,dc_open.id,UI_ACCENT);
  dc_clipped_text(43,dc_open.y+(dc_open.h-8)/2,DC_INK,dc_label(dc_open.id));
 }else{
  float p=dc_ease((elapsed-.07f)/.20f);
  int x=(int)roundf(12*(1-p)),y=(int)roundf(dc_open.y*(1-p));
  int w=208+(int)roundf((W-208)*p),h=dc_open.h+(int)roundf((H-dc_open.h)*p);
  dc_open_window(x,y,w,h);
  unsigned edge=dc_mix(UI_EDGE,UI_ACCENT,(int)((1-p)*180));
  line(x,y,x+w-1,y,edge);line(x,y+h-1,x+w-1,y+h-1,edge);line(x,y,x,y+h-1,edge);line(x+w-1,y,x+w-1,y+h-1,edge);
 }
}
