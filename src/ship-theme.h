/* Cosmetic palette roles. World/faction colours and warning red stay fixed. */
typedef struct {unsigned panel,raised,edge,accent,signal,text,muted;} ShipTheme;
static const ShipTheme ship_themes[DECORATOR_COUNT]={
 {RGB(21,28,39),RGB(41,54,70),RGB(193,139,77),RGB(240,180,91),RGB(85,212,212),RGB(229,210,163),RGB(155,154,165)},
 {RGB(13,29,38),RGB(25,58,69),RGB(61,147,162),RGB(114,232,239),RGB(185,214,255),RGB(223,243,245),RGB(156,191,202)},
 {RGB(35,21,26),RGB(66,37,43),RGB(175,104,86),RGB(255,167,133),RGB(247,210,151),RGB(249,232,220),RGB(200,169,166)},
 {RGB(26,21,40),RGB(49,39,74),RGB(139,111,185),RGB(205,177,255),RGB(118,221,229),RGB(236,229,253),RGB(184,172,207)},
 {RGB(17,31,28),RGB(32,61,49),RGB(81,156,115),RGB(157,235,176),RGB(223,219,153),RGB(228,244,226),RGB(161,193,176)},
 {RGB(33,27,18),RGB(64,49,28),RGB(184,141,64),RGB(255,219,133),RGB(151,226,209),RGB(250,239,211),RGB(201,182,147)},
 {RGB(23,28,35),RGB(45,55,67),RGB(139,160,181),RGB(220,234,250),RGB(113,221,233),RGB(240,244,250),RGB(174,186,200)},
 {RGB(33,20,35),RGB(64,35,64),RGB(176,108,164),RGB(251,177,232),RGB(161,213,255),RGB(250,226,243),RGB(201,171,196)},
 {RGB(22,28,34),RGB(43,52,64),RGB(150,175,200),RGB(245,250,255),RGB(140,220,250),RGB(245,248,255),RGB(180,195,212)},
 {RGB(12,22,43),RGB(25,43,76),RGB(75,125,205),RGB(145,185,255),RGB(140,235,245),RGB(230,242,255),RGB(163,188,220)},
 {RGB(12,31,31),RGB(24,60,60),RGB(65,160,150),RGB(110,240,210),RGB(225,215,150),RGB(223,250,240),RGB(158,200,189)},
 {RGB(32,23,18),RGB(64,43,30),RGB(177,121,73),RGB(245,188,128),RGB(146,223,214),RGB(250,230,211),RGB(204,182,158)},
 {RGB(24,29,15),RGB(47,58,27),RGB(141,164,55),RGB(209,248,126),RGB(133,228,228),RGB(240,247,214),RGB(191,204,151)},
 {RGB(34,16,24),RGB(65,29,43),RGB(176,78,104),RGB(255,153,181),RGB(249,220,170),RGB(255,226,235),RGB(208,162,180)},
 {RGB(34,24,16),RGB(69,43,22),RGB(196,121,50),RGB(255,193,101),RGB(147,223,247),RGB(255,236,208),RGB(212,184,149)},
 {RGB(21,16,39),RGB(44,31,72),RGB(118,91,190),RGB(185,161,255),RGB(147,235,220),RGB(236,229,255),RGB(181,171,212)}
};
static int ship_theme_index(void){
 static int last_ship=-1,index=0;static unsigned last_paint;
 int s=game.ship;if(s<0||s>=player_ship_count)return 0;
 unsigned paint=ship_paint[s];if(s==last_ship&&paint==last_paint)return index;
 last_ship=s;last_paint=paint;index=0;
 for(int i=0;i<DECORATOR_COUNT;i++)if(paint==decorator_finishes[i]){index=i;break;}return index;
}
#define UI_PANEL (ship_themes[ship_theme_index()].panel)
#define UI_RAISED (ship_themes[ship_theme_index()].raised)
#define UI_EDGE (ship_themes[ship_theme_index()].edge)
#define UI_ACCENT (ship_themes[ship_theme_index()].accent)
#define UI_SIGNAL (ship_themes[ship_theme_index()].signal)
#define UI_TEXT (ship_themes[ship_theme_index()].text)
#define UI_MUTED (ship_themes[ship_theme_index()].muted)
#define UI_GOLD (ship_theme_index()?UI_ACCENT:GOLD)
#define UI_CYAN (ship_theme_index()?UI_SIGNAL:CYAN)
/* Repaint console metal while retaining the original luminance/shape.
 * Original Gold retains the authored default. Warnings, artwork, portraits,
 * faction colours and item-category signals are not passed through here. */
static unsigned ui_paint_surface(unsigned color){
 int index=ship_theme_index();if(!index)return color;
 unsigned panel=ship_themes[index].panel;
 int lum=((color&255)*3+((color>>8)&255)*6+((color>>16)&255))/10;
 int base=((panel&255)*3+((panel>>8)&255)*6+((panel>>16)&255))/10;
 int r=lum+((int)(panel&255)-base)*3,g=lum+((int)((panel>>8)&255)-base)*3,b=lum+((int)((panel>>16)&255)-base)*3;
 if(r<0)r=0;if(r>255)r=255;if(g<0)g=0;if(g>255)g=255;if(b<0)b=0;if(b>255)b=255;return RGB(r,g,b);
}
static unsigned ui_paint_text(unsigned color){
 if(!ship_theme_index())return color;
 if(color==GOLD)return UI_ACCENT;
 if(color==CYAN)return UI_SIGNAL;if(color==WHITE)return UI_TEXT;if(color==DIM)return UI_MUTED;
 return color;
}

/* Compact cosmetic preferences, independent of commander save versions. */
typedef struct {unsigned magic,paint[23],check;} PaintSettings;
typedef struct {unsigned magic,paint[22],check;} PaintSettingsV2;
typedef struct {unsigned magic,paint[16],check;} PaintSettingsV1;
static unsigned paint_check(const PaintSettings *p){unsigned c=p->magic;for(int i=0;i<23;i++)c=(c*33u)^p->paint[i];return c;}
static int paint_valid(unsigned paint){for(int i=0;i<DECORATOR_COUNT;i++)if(paint==decorator_finishes[i])return 1;return 0;}
static int paint_read(const char *path,PaintSettings *p){
 FILE *f=fopen(path,"rb");if(!f)return 0;fseek(f,0,SEEK_END);long bytes=ftell(f);rewind(f);
 unsigned words[25]={0};int count=bytes==(long)sizeof(PaintSettings)?23:bytes==(long)sizeof(PaintSettingsV2)?22:bytes==(long)sizeof(PaintSettingsV1)?16:0;
 if(!count){fclose(f);return 0;}int ok=fread(words,1,bytes,f)==(size_t)bytes;fclose(f);
 unsigned expected=count==23?0x504e5433u:count==22?0x504e5432u:0x504e5431u,c=words[0];
 if(!ok||words[0]!=expected)return 0;for(int i=0;i<count;i++){if(!paint_valid(words[i+1]))return 0;c=(c*33u)^words[i+1];}if(c!=words[count+1])return 0;
 memset(p,0,sizeof(*p));p->magic=0x504e5433u;for(int i=0;i<23;i++)p->paint[i]=i<count?words[i+1]:ship_paint[i];p->check=paint_check(p);return 1;
}
static void paint_load(void){PaintSettings p;if(paint_read("ship-paint.cfg",&p)||paint_read("ship-paint.cfg.bak",&p))memcpy(ship_paint,p.paint,sizeof(ship_paint));}
static int paint_save(void){
 PaintSettings p={0x504e5433u,{0},0},verify;memcpy(p.paint,ship_paint,sizeof(ship_paint));p.check=paint_check(&p);
 FILE *f=fopen("ship-paint.cfg.tmp","wb");if(!f)return 0;int ok=fwrite(&p,1,sizeof(p),f)==sizeof(p);if(fflush(f))ok=0;if(fclose(f))ok=0;if(!ok||!paint_read("ship-paint.cfg.tmp",&verify))return 0;
 int backed=paint_read("ship-paint.cfg",&verify);
 if(backed){remove("ship-paint.cfg.bak");if(rename("ship-paint.cfg","ship-paint.cfg.bak"))return 0;}else remove("ship-paint.cfg");
 if(rename("ship-paint.cfg.tmp","ship-paint.cfg")){if(backed)rename("ship-paint.cfg.bak","ship-paint.cfg");return 0;}if(game.erl.fleet_n&&!game.erl.event_kind){game.erl.fleet[game.erl.active_ship].paint=ship_paint[game.ship];game.erl.dirty=1;}return 1;
}
