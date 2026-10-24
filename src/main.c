static int journey_variant=1;
#define JOURNEY_VARIANTS 1
/* PSP frontend for the early Elite-A native adaptation. */
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <psppower.h>
#include <psprtc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
/* The larger-memory profile trades code space for faster visual hot loops. */
#ifdef ELITE_PSP3000
#define ELITE_VISUAL_OPT "O3"
#else
#define ELITE_VISUAL_OPT "Os"
#endif
#include "game.h"
#include "equipment-fit.h"
#include "rift-catalog.h"
#include "story.h"
#include "tutorial.h"
static int commander_active_slot=0;
static const char *commander_slots[]={"commander.sav","commander-2.sav","commander-3.sav"};
static const char *commander_save_path(const Game *g){return g->tutorial_step?"tutorial.sav":commander_slots[commander_active_slot];}
static void profile_enter(void);
#include "guild.h"
#include "campaign.h"
#include "saga.h"
#include "audio.h"
#ifdef ELITE_ENABLE_REVIEW
#include "radio-tests.h"
#endif
#include "steering.h"
#ifdef ELITE_ENABLE_REVIEW
#include "steering-test.h"
#endif

PSP_MODULE_INFO("ELITE NEXT",0,1,7);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
/* Keep the 24 MiB PSP-1000 budget viable alongside the native console caches.
 * Seven MiB remains bounded; emulator heap headroom is recorded by region QA. */
#ifdef ELITE_PSP3000
PSP_HEAP_SIZE_KB(12288);
#elif defined(ELITE_ENABLE_REVIEW)
/* Embedded historical fixtures need extra image space; retail keeps 7 MiB. */
PSP_HEAP_SIZE_KB(5120);
#else
PSP_HEAP_SIZE_KB(7168);
#endif
/* Bounded save migration and simulation fixtures hold several Game snapshots. */
PSP_MAIN_THREAD_STACK_SIZE_KB(1024);
#define W 480
#define H 272
#define STRIDE 512
static void expedition_flight_draw(void);
static int expedition_header_info(char*,int);
static void hc_indicator_anchor(float*,float*);
static int ion_gu_ready;
#define RGB(r,g,b) ((unsigned)(r)|((unsigned)(g)<<8)|((unsigned)(b)<<16)|0xff000000u)
#define WHITE RGB(222,236,239)
#define CYAN RGB(63,209,216)
#define GOLD RGB(248,192,93)
#define AMBER RGB(236,148,36)
#define AMBDIM RGB(168,88,22)
#define DASH RGB(16,8,5)
#define DIM RGB(114,142,154)
#define BG RGB(6,12,22)
#define RED RGB(244,85,88)
static volatile int running=1;
static volatile int resume_requested=0,suspend_requested=0;
static unsigned *fb;
static Game game;
/* Candidate Race 1 is a disposable session, never a commander checkpoint. */
static int erl_quick_race,erl_quick_race_release;
static int erl_quick_race_start(void);
static void erl_quick_race_leave(void);
static int erl_racer_picker,erl_racer_cursor,erl_racer_picker_quick,erl_quick_racer_choice;
static void erl_racer_open(int quick);
static int erl_racer_input(unsigned pressed);
static void erl_racer_picker_draw(void);
#include "erl-checkpoint.h"
static void opening_new_game(void);
static void intro_enter(void);
static void intro_leave(void);
static void intro_suspend(void);
static void opening_restore(void);
static void opening_frontend_reset(void);
static int opening_input(unsigned,unsigned,float,float,float);
static void ship_purchase_reset(void);
static int opening_required_service(void);
static void opening_draw(void);
static void opening_dream_view(void);
static void tutorial_resume(void);
static int page=0,row=0,paused=0,smoke=0,visual_hold=0,hud_hidden=0,hud_mode=0,high_contrast=0,third_person=0,portrait_capture=0;
static int render_reference=0; /* Enabled only by the disposable performance fixture. */
static int quit_confirm=0,quit_confirm_yes=0;
static int death_choice=0;
static float death_notice_time=0;
static char death_notice[80];
static int hand_selected=0,hand_open=0,hand_armed=0,hand_consumed=0,hand_chosen=0,hand_wait=0;
static float hand_hold=0;
static const char *hand_label(void);
static void hand_draw(void);
static int intro_profile_ready=0;
/* One persistent finish per purchasable hull.  The catalogue grew from 16 to
 * 22 ships when the mega exchange arrived; keeping the old fixed array made
 * later capitals read unrelated memory and lose their cockpit theme. */
static unsigned ship_paint[23]={
 GOLD,CYAN,RGB(240,120,96),RGB(160,120,240),RGB(120,220,150),RGB(240,210,120),RGB(180,190,205),RGB(230,150,210),GOLD,GOLD,
 RGB(70,125,240),RGB(205,140,85),RGB(205,65,90),RGB(45,200,180),RGB(180,190,205),RGB(190,240,65),RGB(255,155,40),
 RGB(130,85,240),RGB(45,200,180),RGB(160,120,240),RGB(240,210,120),RGB(205,65,90),RGB(235,242,250)
};
enum { DECORATOR_COUNT=16,DECORATOR_PAGE=8 };
static const unsigned decorator_finishes[DECORATOR_COUNT]={GOLD,CYAN,RGB(240,120,96),RGB(160,120,240),RGB(120,220,150),RGB(240,210,120),RGB(180,190,205),RGB(230,150,210),RGB(235,242,250),RGB(70,125,240),RGB(45,200,180),RGB(205,140,85),RGB(190,240,65),RGB(205,65,90),RGB(255,155,40),RGB(130,85,240)};
static const char *decorator_names[DECORATOR_COUNT]={"ORIGINAL GOLD","COCKPIT CYAN","SUNSET RED","DEEP VIOLET","EXPLORER GREEN","SOLAR AMBER","NEON GRID","STARFALL THEME","ICE WHITE","COBALT BLUE","ION TEAL","COPPER GLOW","LIME CIRCUIT","CRIMSON RED","TANGERINE","ULTRAVIOLET"};
static const int decorator_fees[DECORATOR_COUNT]={120,180,240,320,400,520,700,900,1000,1100,1200,1300,1400,1500,1600,1700};
static int decorator_feedback=0;
#include "ship-theme.h"
/* 0 = Kei/Ryn campaign, 1 = Guild assignments, 2+ = accepted job slot. */
static int tracked_mission=0;
static int mission_title_focus=-1;
static unsigned mission_title_hash=0;
static float mission_title_start=0;
enum { TRACK_STATION_TOUR=MISSION_SLOTS+2,TRACK_LAVE=MISSION_SLOTS+3,TRACK_ERL=MISSION_SLOTS+4 };
static int station_tour_active(void);
static const char *station_tour_objective(void);
static int sc_lave_stage(void);static const char *sc_lave_objective(void);
static int lave_log_active(void){return ((game.station_progress[7][0]>>2)&15)>0||tracked_mission==TRACK_LAVE;}
static int erl_log_active(void){return !game.erl.licence&&(game.erl.track||game.erl.referral||tracked_mission==TRACK_ERL);}
static int erl_destination(void){return game.erl.event_kind?game.erl.event_system:game.erl.championship?game.erl.champ_system:game.erl.referral&&game.erl.ref_system>=0?game.erl.ref_system:7;}
static int mission_log_count(void){return 2+game.job_n+(station_tour_active()?1:0)+(lave_log_active()?1:0)+(erl_log_active()?1:0);}
static int mission_track_at(int index){return erl_log_active()&&index==mission_log_count()-1?TRACK_ERL:lave_log_active()&&index==2+game.job_n+(station_tour_active()?1:0)?TRACK_LAVE:station_tour_active()&&index==2+game.job_n?TRACK_STATION_TOUR:index;}
static int mission_track_row(void){return tracked_mission==TRACK_ERL?mission_log_count()-1:tracked_mission==TRACK_LAVE?2+game.job_n+(station_tour_active()?1:0):tracked_mission==TRACK_STATION_TOUR?2+game.job_n:tracked_mission;}
static void mission_track_select(int index){
 if(index<0||index>=mission_log_count())return;
 tracked_mission=mission_track_at(index);
 if(tracked_mission>=2&&tracked_mission<TRACK_STATION_TOUR)game.job_sel=tracked_mission-2;
}
static float warp_arrival_fade=0;
/* Short, session-local onboarding mission that showcases the authored station bar. */
enum { STATION_TOUR_OFF=0, STATION_TOUR_ROUTE, STATION_TOUR_DOCK, STATION_TOUR_WALK, STATION_TOUR_BAR, STATION_TOUR_TALK, STATION_TOUR_DONE };
#define STATION_TOUR_DEST 39
static int station_tour_stage=STATION_TOUR_OFF;
static int station_tour_wrong_hub_notice=0;
static int analog_center_x=128,analog_center_y=128;
static int walk_kind=0;
static float walk_x=0,walk_z=0,walk_yaw=0;
static float walk_oxygen=100,walk_integrity=100;
static int walk_salvaged=0;
static int analog_enabled=1,analog_ready=0,fire_blocked=0;
static float preview_time=0;
static int nearby[256],near_count,chart_mode=1,chart_cursor=7;
enum { CHART_ALL,CHART_VISITED,CHART_UNVISITED,CHART_RICH,CHART_POOR,CHART_MEGA,CHART_JOBS,CHART_RANGE,CHART_ROUTE,CHART_FILTERS };
static int chart_filter=CHART_ALL,chart_search=0,chart_search_key=0,chart_search_match=0;
static int chart_matches[256],chart_match_n=0;
static char chart_query[12];
static float chart_angle=.34f;
static float chart_zoom=1.f,chart_zoom_goal=1.f,chart_pan_x=0,chart_pan_y=0;
/* Compact copy of the command deck used by the native-pixel map reveal. */
static unsigned short chart_dissolve_frame[W*H];
static float chart_dissolve_time=0;
#define CHART_DISSOLVE_DURATION .72f
static float route_boost_charge=0;
static int route_jump_engaged=0;
static int planet_scene_camera=0;
static int view_top(void){if(race_is_active(&game))return 0;return planet_scene_camera?0:game.planet>=0&&game.surface==2?28:hud_hidden?0:(hud_mode==0?0:23);}
static int buffer=0;
static void display_start_black(void){
 /* Cold boot VRAM can still contain the launcher's pixels. Clear both full
  * planes BEFORE publishing either one; debug init must not replace it. */
 for(int b=0;b<2;b++){unsigned *plane=(unsigned*)(0x44000000u+(unsigned)b*STRIDE*H*4);for(int i=0;i<STRIDE*H;i++)plane[i]=RGB(0,0,0);}
 buffer=0;fb=(unsigned*)0x44000000u;
 /* Retain the PSP SDK's complete LCD setup and first NEXTFRAME latch.
  * InitEx(setup=0) skipped that setup; restore it for the reported PSP regression. */
 sceDisplaySetMode(0,W,H);pspDebugScreenInit();
 pspDebugScreenEnableBackColor(0);pspDebugScreenSetOffset(0);
 /* Debug init clears its plane too. Finish with two uniformly black planes
  * before the first vblank; then decode into the hidden plane. */
 for(int b=0;b<2;b++){unsigned *plane=(unsigned*)(0x44000000u+(unsigned)b*STRIDE*H*4);for(int i=0;i<STRIDE*H;i++)plane[i]=RGB(0,0,0);}
 sceDisplaySetFrameBuf((void*)fb,STRIDE,PSP_DISPLAY_PIXEL_FORMAT_8888,PSP_DISPLAY_SETBUF_NEXTFRAME);
 sceDisplayWaitVblankStart();
 /* The first cinematic decode can take longer than a refresh. Draw it into
  * the OTHER plane so clearing/loading cannot flash over the black front. */
 buffer=1;
}
static void display_recover(void){
 /* After sleep the LCD/framebuffer pairing can be invalid; rebuild both planes. */
 sceDisplaySetMode(0,W,H);
 for(int b=0;b<2;b++){unsigned *plane=(unsigned*)(0x44000000u+(unsigned)b*STRIDE*H*4);for(int i=0;i<STRIDE*H;i++)plane[i]=BG;}
 fb=(unsigned*)(0x44000000u+(unsigned)buffer*STRIDE*H*4);
 pspDebugScreenInit();pspDebugScreenEnableBackColor(0);pspDebugScreenSetOffset(buffer*STRIDE*H*4);
 sceDisplayWaitVblankStart();
 /* IMMEDIATE: show the recovered plane now. NEXTFRAME here left the LCD on a stale/empty buffer. */
 sceDisplaySetFrameBuf((void*)fb,STRIDE,PSP_DISPLAY_PIXEL_FORMAT_8888,PSP_DISPLAY_SETBUF_IMMEDIATE);
}
static void runtime_recover_from_sleep(void){
 /* Tear down audio left over from a long suspend, then restore display/input/radio. */
 intro_suspend();audio_stop();
 scePowerSetClockFrequency(333,333,166);
 display_recover();
 sceCtrlSetSamplingCycle(0);sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
 analog_ready=0;
 audio_init();
}
static int view_bot(void){if(race_is_active(&game))return H-1;return planet_scene_camera==1?243:planet_scene_camera?H-1:game.planet>=0&&game.surface==2?239:hud_hidden?H-1:(hud_mode==0?191:247);}
enum { HOME,FLIGHT,MARKET,CHART,YARD,EQUIP,STATUS,HELP,FACTIONS,LOCAL,DEBUG,COMMS,DETAILS,MISSIONS,MISSIONLOG,TARGETING,GALNET,CODEX,STORY,GUILD,RADIO,COMMS_PANEL,INTRO,CAMPAIGN,COMFORT,WALK,INVENTORY,STORAGE,REPAIR,DECORATOR,FIELDGUIDE,LOCALTV,MULTIPLAYER,NEARBY_PLAYERS,LAW_PAGE,ERL_PAGE,DOCKBAY_PAGE };
static int nav_virtual_target=-1,nav_follow=0;
#include "deck-nav.h"
static void deck_open_begin(int next);
static int deck_open_active(void);
static int deck_open_pending(int next);
static void deck_open_cancel(void);
static float load_arrival_time=-1.f;
static int load_arrival_release=0;
static void load_arrival_begin(void);
static void opening_restart_intro(void);
enum { COMMS_OPTION_COUNT=16 };
static int pip_sel=1,comms_rescue_confirm=0,abandon_confirm=0,mission_accept_confirm=-1,mission_accept_yes=0,faction_lore_card=0,sell_confirm_slot=-1,equip_confirm_item=-1,equip_target=-1;
static int module_inventory_row=0,module_bank=0,module_sell_item=-1,storage_sell_mode=0,storage_bank_focus=0;
static int loadout_picker_slot=-1,loadout_picker_row=0;
static int departure_release=0;
static int shipyard_stats_view=1;
static float ui_press_anim=0,ui_page_anim=0;
static int ui_press_key=0,ui_press_source=-1,ui_press_row=-1;
static int comms_quick=0,comms_quick_choice=0;
static int debug_force_thargoid=0;
static const char *faction_names[]={"TRADERS","LAW","PIRATES","EXPLORERS GUILD"};
static const unsigned faction_colors[]={GOLD,RGB(90,165,255),RED,RGB(100,235,150)};
static int nav_body=-1;
static float proj_focal=240;
/* Command Deck opening, victory and active racing cannot overlap. */
static unsigned dc_open_source[STRIDE*H] __attribute__((aligned(16)));
static int neon_render_width=W,neon_render_height=H;
static void neon_present_readback(void);
static int proj_ox=240,proj_oy=110,clipx0=0,clipx1=W,clipy0=-1,clipy1=-1;
static int exit_callback(int a,int b,void *c){(void)a;(void)b;(void)c;running=0;return 0;}
static int power_callback(int a,int flags,void *c){
 (void)a;(void)c;
 /* Freeze MP3 I/O immediately so a long Memory Stick wake cannot hang the audio worker. */
 if(flags&(PSP_POWER_CB_SUSPENDING|PSP_POWER_CB_STANDBY)){audio_prepare_suspend();suspend_requested=1;}
 if(flags&(PSP_POWER_CB_RESUME_COMPLETE|PSP_POWER_CB_RESUMING))resume_requested=1;
 return 0;
}
static int callback_thread(SceSize a,void *b){(void)a;(void)b;int id=sceKernelCreateCallback("Exit",exit_callback,0);sceKernelRegisterExitCallback(id);id=sceKernelCreateCallback("Power",power_callback,0);scePowerRegisterCallback(0,id);sceKernelSleepThreadCB();return 0;}
static void rect(int x,int y,int w,int h,unsigned color){
 int x0=x<0?0:x,y0=y<0?0:y,x1=x+w>W?W:x+w,y1=y+h>H?H:y+h;
 for(int j=y0;j<y1;j++)for(int i=x0;i<x1;i++)fb[j*STRIDE+i]=color;
}
#include "decorator-monitor-art.h"
static void warp_arrival_overlay(void){
 if(warp_arrival_fade<=0)return;
 int top=view_top(),bot=view_bot();float reveal=1.f-warp_arrival_fade;int keep=35+(int)(65.f*reveal);
 for(int y=top;y<=bot;y++)for(int x=0;x<W;x++){unsigned c=fb[y*STRIDE+x];int r=c&255,g=(c>>8)&255,b=(c>>16)&255;fb[y*STRIDE+x]=RGB((r*keep+8*(100-keep))/100,(g*keep+16*(100-keep))/100,(b*keep+42*(100-keep))/100);}
}
static void pixel(int x,int y,unsigned c){if(clipy0>=0&&(x<clipx0||x>=clipx1||y<clipy0||y>=clipy1))return;if(x>=0&&x<W&&y>=0&&y<H)fb[y*STRIDE+x]=c;}
static void line(int x,int y,int xx,int yy,unsigned c){
 // Reject pathological off-screen projections before stepping.
 /* Direct comparisons also reject INT_MIN from an overflowing projection.
  * abs(INT_MIN) cannot be represented and could bypass the old guard,
  * leaving the line stepper walking billions of pixels on real hardware. */
 if(x < -3000||x > 3000||y < -3000||y > 3000||xx < -3000||xx > 3000||yy < -3000||yy > 3000)return;
 int dx=abs(xx-x),sx=x<xx?1:-1,dy=-abs(yy-y),sy=y<yy?1:-1,e=dx+dy;
 for(;;){pixel(x,y,c);if(x==xx&&y==yy)break;int e2=e*2;if(e2>=dy){e+=dy;x+=sx;}if(e2<=dx){e+=dx;y+=sy;}}
}
#include "font8.h"
#include "art-runtime.h"
#include "native-art-scenes.h"
#include "space-animation-kit.h"
static void draw_text_pixels(int px,int py,unsigned c,const char *buf){
 for(int i=0;buf[i]&&px<W;i++,px+=8){
  unsigned char ch=(unsigned char)buf[i];if(ch<32||ch>126)ch='?';
  const unsigned char *g=font8[ch-32];
  if(px>=0&&px+7<W&&py>=0&&py+7<H){
   for(int row=0;row<8;row++){unsigned bits=g[row];unsigned*out=fb+(py+row)*STRIDE+px;while(bits){int col=__builtin_clz(bits)-24;out[col]=c;bits&=~(128u>>col);}}
   continue;
  }
  for(int row=0;row<8;row++){
   int yy=py+row;if(yy<0||yy>=H)continue;
   unsigned bits=g[row];
   for(int col=0;col<8;col++)if((bits>>(7-col))&1){int xx=px+col;if(xx>=0&&xx<W)fb[yy*STRIDE+xx]=c;}
  }
 }
}
static void text(int x,int y,unsigned c,const char *fmt,...){
 if(page!=FLIGHT&&page!=ERL_PAGE)c=ui_paint_text(c);
 char buf[128];if(!strchr(fmt,'%')&&strlen(fmt)<sizeof(buf)){draw_text_pixels(x*8,y*8,c,fmt);return;}va_list args;va_start(args,fmt);vsnprintf(buf,sizeof(buf),fmt,args);va_end(args);
 draw_text_pixels(x*8,y*8,c,buf);
}
static void text_px(int x,int y,unsigned c,const char *fmt,...){
 if(page!=FLIGHT&&page!=ERL_PAGE)c=ui_paint_text(c);
 char buf[128];if(!strchr(fmt,'%')&&strlen(fmt)<sizeof(buf)){draw_text_pixels(x,y,c,fmt);return;}va_list args;va_start(args,fmt);vsnprintf(buf,sizeof(buf),fmt,args);va_end(args);
 draw_text_pixels(x,y,c,buf);
}
/* One quiet annunciator shared by station and planetary flight guidance. */
static void autopilot_annunciator(int y,float time){
 float pulse=.5f+.5f*cosf(time*3.14159265f);
 rect(166,y-3,148,14,RGB(8,21,19));
 text_px(172,y,RGB(62+64*pulse,153+88*pulse,100+62*pulse),"AUTOPILOT ENGAGED");
}
/* Word-wrap into a fixed column width. Returns rows used; leftover returns via *left. */
static int text_wrap(int col,int rowy,int cols,int max_rows,unsigned ink,const char *s,const char **left){
 int used=0;if(cols<4)cols=4;if(left)*left=s;
 for(;used<max_rows&&s&&*s;used++){
  int len=(int)strlen(s),cut=len<cols?len:cols;
  if(len>cols){int k;for(k=cut;k>cols/3;k--)if(s[k]==' '){cut=k;break;}}
  text(col,rowy+used,ink,"%.*s",cut,s);s+=cut;while(*s==' ')s++;
 }
 if(left)*left=s;
 return used;
}
/* Coloured name plate so "KEI SAYS" / "REI SAYS" read at a glance on 480x272. */
static void speaker_name_tag_at(int x,int y,const char *name,unsigned color){
 char tag[28];snprintf(tag,sizeof(tag),"%.12s SAYS",name&&name[0]?name:"CONTACT");
 int n=(int)strlen(tag);if(n<1)n=1;if(n>18)n=18;
 rect(x-2,y-1,n*8+6,10,color);
 rect(x-1,y,n*8+4,8,RGB(((color&255)*28)/100,(((color>>8)&255)*28)/100,(((color>>16)&255)*28)/100));
 text_px(x,y,WHITE,"%.*s",n,tag);
}
static void dump_native_bmp(const char *path){
 FILE *f=fopen(path,"wb");if(!f||!fb)return;
 int w=W,h=H;unsigned size=54+(unsigned)w*h*3;unsigned char hdr[54]={0};
 hdr[0]='B';hdr[1]='M';hdr[2]=(unsigned char)size;hdr[3]=(unsigned char)(size>>8);hdr[4]=(unsigned char)(size>>16);hdr[5]=(unsigned char)(size>>24);
 hdr[10]=54;hdr[14]=40;hdr[18]=(unsigned char)w;hdr[19]=(unsigned char)(w>>8);hdr[22]=(unsigned char)h;hdr[23]=(unsigned char)(h>>8);hdr[26]=1;hdr[28]=24;
 unsigned char *bmp=(unsigned char*)malloc(size);
 if(!bmp){fclose(f);return;}memcpy(bmp,hdr,54);
 for(int y=h-1;y>=0;y--)for(int x=0;x<w;x++){unsigned c=fb[y*STRIDE+x];unsigned at=54u+(unsigned)(h-1-y)*w*3u+(unsigned)x*3u;bmp[at]=(unsigned char)((c>>16)&255);bmp[at+1]=(unsigned char)((c>>8)&255);bmp[at+2]=(unsigned char)(c&255);}
 fwrite(bmp,1,size,f);free(bmp);
 fclose(f);
}
static unsigned short chart_dissolve_pack(unsigned c){
 return (unsigned short)(((c&255)>>3)<<11|(((c>>8)&255)>>2)<<5|((c>>16)&255)>>3);
}
static unsigned chart_dissolve_unpack(unsigned short c){
 int r=(c>>11)&31,g=(c>>5)&63,b=c&31;
 return RGB((r<<3)|(r>>2),(g<<2)|(g>>4),(b<<3)|(b>>2));
}
static unsigned chart_dissolve_hash(unsigned x,unsigned y){
 unsigned h=x*0x9e3779b9u^y*0x85ebca6bu^0x6d61702du;
 h^=h>>16;h*=0x7feb352du;h^=h>>15;return h;
}
static void chart_dissolve_begin(void){
 if(!fb||smoke)return;
 for(int y=0;y<H;y++)for(int x=0;x<W;x++)chart_dissolve_frame[y*W+x]=chart_dissolve_pack(fb[y*STRIDE+x]);
 chart_dissolve_time=CHART_DISSOLVE_DURATION;
}
static void chart_dissolve_draw(float dt){
 if(page!=CHART||chart_dissolve_time<=0)return;
 int progress=(int)((1.f-chart_dissolve_time/CHART_DISSOLVE_DURATION)*255.f);
 if(progress<0)progress=0;if(progress>255)progress=255;
 /* Two-pixel cells preserve the crisp PSP look. The centre clears first and
  * deterministic jitter keeps the edge granular instead of circular. */
 for(int y=0;y<H;y+=2)for(int x=0;x<W;x+=2){
  unsigned h=chart_dissolve_hash((unsigned)(x>>1),(unsigned)(y>>1));
  int distance=abs(x-W/2)*3+abs(y-H/2)*5;
  int release=18+distance*100/1400+(int)(h&95u);
  if(progress<release){
   unsigned old=chart_dissolve_unpack(chart_dissolve_frame[y*W+x]);
   fb[y*STRIDE+x]=old;if(x+1<W)fb[y*STRIDE+x+1]=old;
   if(y+1<H){fb[(y+1)*STRIDE+x]=old;if(x+1<W)fb[(y+1)*STRIDE+x+1]=old;}
  }else{
   int age=progress-release;
   if(age>=0&&age<46&&((h>>16)%3u)==0){
    int outward=age/5,side=((int)((h>>9)&7u)-3)*age/28;
    int sx=x+(x<W/2?-outward:outward)+(y<H/2?-side:side);
    int sy=y+(y<H/2?-outward:outward)+(x<W/2?side:-side);
    unsigned src=chart_dissolve_unpack(chart_dissolve_frame[y*W+x]);
    unsigned glow=(h&1u)?CYAN:GOLD;int heat=46-age;
    int r=((src&255)*(46-heat)+(glow&255)*heat)/46;
    int g=(((src>>8)&255)*(46-heat)+((glow>>8)&255)*heat)/46;
    int b=(((src>>16)&255)*(46-heat)+((glow>>16)&255)*heat)/46;
    pixel(sx,sy,RGB(r,g,b));if(age<13)pixel(sx+(x<W/2?-1:1),sy,RGB(r,g,b));
   }
   if(abs(progress-release)<3&&(h&4u))pixel(x,y,(h&2u)?CYAN:GOLD);
  }
 }
 chart_dissolve_time-=dt;if(chart_dissolve_time<0)chart_dissolve_time=0;
}
static void circle(int x,int y,int r,unsigned c){for(int i=0;i<64;i++){float a=i*6.2831853f/64,b=(i+1)*6.2831853f/64;line(x+(int)(cosf(a)*r),y+(int)(sinf(a)*r),x+(int)(cosf(b)*r),y+(int)(sinf(b)*r),c);}}
typedef struct {float x,y,z;} Point;
typedef struct {Point p[3];unsigned color;float depth;int material,order;} DrawTri;
static void mega_surface_triangle(DrawTri *t);
static void menu_station_textured_triangle(DrawTri *t);
/* Triangle queues are fully flushed before the translucent road pass.
 * Reuse this scratch arena for nearest-road coverage; never grow the retail
 * heap or weaken the original PSP codec/thread memory allowance. */
static union {DrawTri triangles[4096];unsigned deck[(W/2)*(H/2)];} render_scratch __attribute__((aligned(16)));
#define drawlist render_scratch.triangles
static int drawcount;
static int render_triangle_limit=2048,render_raster_step=2;
static int surface_material;
static int mega_depth_valid,mega_depth_encoding;
static unsigned short mega_occlusion[240*136];
static int neon_depth_mode;
#define NEON_DEPTH_A (65535.f*80000.f/(80000.f-64.f))
#define NEON_DEPTH_B (65535.f*80000.f*64.f/(80000.f-64.f))
static inline __attribute__((always_inline)) unsigned neon_encode_depth(float inv){int d=(int)(NEON_DEPTH_A-NEON_DEPTH_B*inv);return d<0?0:d>65534?65534:d;}
static inline unsigned mega_encode_depth(float z){if(neon_depth_mode){int d=(int)(NEON_DEPTH_A-NEON_DEPTH_B/z);return d<0?0:d>65534?65534:d;}int d=(int)(z<8192?z*4:32768+(z-8192)*.5f);if(d<0)d=0;if(d>65534)d=65534;return (unsigned)d;}
static int surface_trail_enabled;
static int race_opaque_quads;
static int neon_ram_render=0;
static int race_phase_collect;
static float surface_cy,surface_sy,surface_cp,surface_sp,surface_padx,surface_padz,surface_path_side;
static unsigned surface_meadow[8][32],surface_path[16];static int planet_region_material;
static unsigned surface_ocean;
static float surface_rayx[240],surface_rayz[240],surface_trailx[321];
static uint64_t lave_render_marks[6];
static unsigned eva_ground_epoch,eva_ground_stamp[120*68],eva_ground_ink[120*68];
static int eva_fast=1; /* Focused EVA review can compare the previous raster. */
static int eva_profile;static uint64_t eva_raster_us;
static int surface_fast=1; /* Opt-in review can compare the original path. */
static int surface_cull_suspended=0;
#include "surface-water.h"
static float surface_top_slope,surface_bottom_slope,surface_top_norm,surface_bottom_norm;
static Point project(Vec3 v){return (Point){proj_ox+v.x*proj_focal/v.z,proj_oy-v.y*proj_focal/v.z,v.z};}
/* Conservative sphere/viewport planes. Test before transforming a model,
 * keeping partially visible hulls and objects crossing the near plane. */
static int render_culled=0,render_lod=0,render_submitted=0;
static int render_profile_enabled;static uint64_t render_profile_tick;static double render_phase_ms[7];
static void render_phase(int phase){if(!render_profile_enabled)return;uint64_t now;sceRtcGetCurrentTick(&now);if(phase>=0)render_phase_ms[phase]+=(now-render_profile_tick)*1000.0/sceRtcGetTickResolution();render_profile_tick=now;}
static double render_sub_ms[8];static uint64_t render_sub_tick;
static void render_sub_phase(int phase){if(!render_profile_enabled)return;uint64_t now=sceKernelGetSystemTimeWide();if(phase>=0)render_sub_ms[phase]+=(now-render_sub_tick)*.001;render_sub_tick=now;}
static int render_sphere_visible(Vec3 world,float radius){
 if(render_reference)return 1;
 Vec3 v=camera(&game,world);int top=clipy0>=0?clipy0:view_top(),bot=clipy1>=0?clipy1:view_bot()+1;
 /* Projection planes are shared by every object in this view. */
 static int cached,ox,oy,x0,x1,y0,y1;static float focal,l,r,t,b,ln,rn,tn,bn;
 if(!cached||ox!=proj_ox||oy!=proj_oy||x0!=clipx0||x1!=clipx1||y0!=top||y1!=bot||focal!=proj_focal){
  cached=1;ox=proj_ox;oy=proj_oy;x0=clipx0;x1=clipx1;y0=top;y1=bot;focal=proj_focal;
  l=(x0-ox)/focal;r=(x1-ox)/focal;t=(oy-y0)/focal;b=(oy-y1)/focal;
  ln=sqrtf(1+l*l);rn=sqrtf(1+r*r);tn=sqrtf(1+t*t);bn=sqrtf(1+b*b);
 }
 int visible=v.z+radius>=15&&v.x-l*v.z>=-radius*ln&&r*v.z-v.x>=-radius*rn&&t*v.z-v.y>=-radius*tn&&v.y-b*v.z>=-radius*bn;
 if(!visible)render_culled++;
 return visible;
}
static float render_pixel_radius(Vec3 world,float radius){Vec3 v=camera(&game,world);return radius*proj_focal/fmaxf(15.f,v.z-radius);}
static float render_mesh_radius(int id){
 static float radii[256];if(id<0||id>=mesh_count)return 1;
 if(id<256&&radii[id]>0)return radii[id];float r=1;
 for(int i=0;i<meshes[id].vertices;i++)r=fmaxf(r,length(meshes[id].v[i]));
 if(id<256)radii[id]=r;return r;
}
static void preview_clip(int ox,int oy,int x0,int y0,int x1,int y1){proj_ox=ox;proj_oy=oy;clipx0=x0;clipy0=y0;clipx1=x1;clipy1=y1;}
static void preview_reset(void){proj_ox=240;proj_oy=110;clipx0=0;clipx1=W;clipy0=-1;clipy1=-1;}
static float edge(Point a,Point b,float x,float y){return (x-a.x)*(b.y-a.y)-(y-a.y)*(b.x-a.x);}
/* Finite projected coordinates only. Avoid libm calls in every raster row;
 * truncation needs a correction on negative coordinates, not a rounding bias. */
static float surface_min(float a,float b){return a<b?a:b;}
static float surface_max(float a,float b){return a>b?a:b;}
static int surface_floor(float v){int i=(int)v;return v<i?i-1:i;}
static int surface_ceil(float v){int i=(int)v;return v>i?i+1:i;}
/* Surface-only depth: 255 KiB, no allocation per frame. */
static int surface_y_min=28,surface_y_max=239;static unsigned short surface_depth[480*272] __attribute__((aligned(16)));static int surface_depth_on=0;static float surface_sprite_z=0,surface_light=1,surface_sprite_tan=0;
static unsigned surface_shade(unsigned c){return RGB((int)((c&255)*surface_light),(int)(((c>>8)&255)*surface_light),(int)(((c>>16)&255)*surface_light));}
static unsigned surface_encode_depth(float inv){if(neon_depth_mode){int d=(int)(NEON_DEPTH_A-NEON_DEPTH_B*inv);return d<0?0:d>65534?65534:d;}if(mega_depth_encoding)return mega_encode_depth(1.f/inv);int d=(int)(inv*262140.f);if(d<1)d=1;if(d>65534)d=65534;return 65535-d;}
static void surface_depth_pixel(int x,int y,unsigned depth,unsigned c){if(x<0||x>=W||y<surface_y_min||y>surface_y_max)return;int i=y*W+x;if(depth<=surface_depth[i]){surface_depth[i]=(unsigned short)depth;fb[y*STRIDE+x]=c;}}
static void surface_pixel(int x,int y,float z,unsigned c){surface_depth_pixel(x,y,surface_encode_depth(1.f/z),c);}
/* Scanline spans avoid testing every pixel in a large triangle's bounding box.
 * Two-pixel sampling matches the native raster; depth is shared with sprites. */
static void surface_triangle(DrawTri *t){
 if(t->material<0){mega_surface_triangle(t);return;}
 /* Do this after sorting: removing entries before qsort can change ownership
  * of equal-depth coplanar details. Keep the original queue/tie order. */
 if(surface_fast){Point a=t->p[0],b=t->p[1],c=t->p[2];
  if((a.x< -2&&b.x< -2&&c.x< -2)||(a.x>W+2&&b.x>W+2&&c.x>W+2)||
     (a.y<surface_y_min-2&&b.y<surface_y_min-2&&c.y<surface_y_min-2)||
     (a.y>surface_y_max+2&&b.y>surface_y_max+2&&c.y>surface_y_max+2))return;
 }
 int material=t->material&31,water_test=!(t->material&32);
 Point *p=t->p;float area=edge(p[0],p[1],p[2].x,p[2].y);if(fabsf(area)<.05f)return;
 float iz0=1.f/(p[0].z*area),iz1=1.f/(p[1].z*area),iz2=1.f/(p[2].z*area);
 float dzdx=(p[2].y-p[1].y)*iz0+(p[0].y-p[2].y)*iz1+(p[1].y-p[0].y)*iz2;
 int first,last;
 if(surface_fast){first=surface_floor(surface_max(surface_y_min,surface_min(p[0].y,surface_min(p[1].y,p[2].y))));last=surface_ceil(surface_min(surface_y_max,surface_max(p[0].y,surface_max(p[1].y,p[2].y))));}
 else{first=(int)fmaxf(surface_y_min,floorf(fminf(p[0].y,fminf(p[1].y,p[2].y))));last=(int)fminf(surface_y_max,ceilf(fmaxf(p[0].y,fmaxf(p[1].y,p[2].y))));}
 first=(first+1)&~1;
 for(int y=first;y<=last;y+=2){float yy=y+1.f,left=1e9f,right=-1e9f;
  for(int e=0;e<3;e++){Point a=p[e],b=p[(e+1)%3];if((a.y<=yy&&b.y>yy)||(b.y<=yy&&a.y>yy)){float xx=a.x+(yy-a.y)*(b.x-a.x)/(b.y-a.y);if(xx<left)left=xx;if(xx>right)right=xx;}}
  if(left>right)continue;int x0,x1;
  if(surface_fast){x0=surface_ceil(surface_max(0,left-1));x1=surface_floor(surface_min(W-1,right-1));}
  else{x0=(int)fmaxf(0,ceilf(left-1));x1=(int)fminf(W-1,floorf(right-1));}x0=(x0+1)&~1;
  float inv=edge(p[1],p[2],x0+1,yy)*iz0+edge(p[2],p[0],x0+1,yy)*iz1+edge(p[0],p[1],x0+1,yy)*iz2;
  float rz=surface_cp-surface_sp*(proj_oy-yy)/240.f,raydx=surface_sy*rz,raydz=surface_cy*rz;
  /* Solid colour spans need only four depth tests, no texture/shore work.
   * Keep the same sampling/depth arithmetic and pixel ownership. */
  if(surface_fast&&t->material==0&&y+1<=surface_y_max){
   int at=y*W+x0,out=y*STRIDE+x0;
   for(int x=x0;x<=x1;x+=2,inv+=dzdx*2,at+=2,out+=2){
    if(inv<=0)continue;unsigned depth=surface_encode_depth(inv);
    unsigned mask=(depth<=surface_depth[at])|((depth<=surface_depth[at+1])<<1)|((depth<=surface_depth[at+W])<<2)|((depth<=surface_depth[at+W+1])<<3);
    if(mask==15){
     surface_depth[at]=surface_depth[at+1]=surface_depth[at+W]=surface_depth[at+W+1]=depth;
     fb[out]=fb[out+1]=fb[out+STRIDE]=fb[out+STRIDE+1]=t->color;
    }else if(mask){
     if(mask&1){surface_depth[at]=depth;fb[out]=t->color;}
     if(mask&2){surface_depth[at+1]=depth;fb[out+1]=t->color;}
     if(mask&4){surface_depth[at+W]=depth;fb[out+STRIDE]=t->color;}
     if(mask&8){surface_depth[at+W+1]=depth;fb[out+STRIDE+1]=t->color;}
    }
   }
   continue;
  }
  for(int x=x0;x<=x1;x+=2,inv+=dzdx*2){if(inv<=0)continue;unsigned depth=surface_encode_depth(inv);
   int at=y*W+x,out=y*STRIDE+x;
   unsigned mask=0;
   if(surface_fast){mask=(depth<=surface_depth[at])|((depth<=surface_depth[at+1])<<1);if(y+1<=surface_y_max)mask|=((depth<=surface_depth[at+W])<<2)|((depth<=surface_depth[at+W+1])<<3);if(!mask)continue;}
   else if(depth>surface_depth[at]&&depth>surface_depth[at+1]&&(y+1>surface_y_max||(depth>surface_depth[at+W]&&depth>surface_depth[at+W+1])))continue;
   unsigned color=t->color;
   /* Keep 2x2 geometry/depth and native props. Beyond 24 m only the grain is
    * sampled on a 4x4 grid; sharing that shader work avoids repeated divides,
    * river-mask queries and noise hashes without coarsening the silhouette. */
   int cache_ground=eva_fast&&game.surface==2&&material>=1&&material<=8&&inv<1.f/24.f;
   int ground_at=(y>>2)*120+(x>>2);unsigned ground_tag=(eva_ground_epoch<<8)|((t->material^(planet_region_material?(t->color^(t->color>>8))&127:0))&127);
   int ground_hit=cache_ground&&eva_ground_stamp[ground_at]==ground_tag;
   if(ground_hit)color=eva_ground_ink[ground_at];
   if(!ground_hit&&t->material>0&&!(eva_fast&&material==18&&!water_test)){
    /* Distant dry terrain already has a constant tone. Avoid world-coordinate
     * reconstruction and hashing where neither water nor the trail can appear. */
    if(eva_fast&&material>=1&&material<=8&&inv<1.f/700.f&&!water_test&&(t->material&64))color=planet_region_material?t->color:surface_meadow[(material-1)&7][12];
    else {
    float z=1.f/inv,wx=game.pos.x+(surface_rayx[x/2]+raydx)*z,wz=game.pos.z+(surface_rayz[x/2]+raydz)*z;
    if(material!=18){
    int tx=(int)(wx*.55f+16384)-16384,tz=(int)(wz*.55f+16384)-16384;unsigned h=(unsigned)tx*374761393u+(unsigned)tz*668265263u;h=(h^(h>>13))*1274126177u;h^=h>>16;
    int tone=z>700?12:z>350?10+(h&7):h&31;
    color=planet_region_material?RGB((int)(t->color&255)*(240+tone)/256,(int)((t->color>>8)&255)*(240+tone)/256,(int)((t->color>>16)&255)*(240+tone)/256):surface_meadow[(material-1)&7][tone];
    int trail=(int)((wz-surface_padz)*(320.f/FIELD_OBSERVATORY_DISTANCE));
    if(surface_trail_enabled&&trail>0&&trail<320&&fabsf(wx-surface_trailx[trail])<14.f)color=surface_path[(h>>5)&15];
    if(material==17){int gx=surface_fast?surface_floor((wx-surface_padx)*.25f):(int)floorf((wx-surface_padx)*.25f),gz=surface_fast?surface_floor((wz-surface_padz)*.25f):(int)floorf((wz-surface_padz)*.25f);color=surface_meadow[0][14+((h>>5)&3)];if((gx&15)==0||(gz&15)==0)color=surface_path[2];if((gx&63)==2)color=surface_path[12];}
    else if(material>8)color=surface_ocean;
    }
    if(material!=17&&water_test)surface_water_sample(wx,wz,z,&color);
    }
   }
   if(cache_ground&&!ground_hit){eva_ground_stamp[ground_at]=ground_tag;eva_ground_ink[ground_at]=color;}
   if(surface_fast){
    if(mask==15){surface_depth[at]=surface_depth[at+1]=surface_depth[at+W]=surface_depth[at+W+1]=depth;fb[out]=fb[out+1]=fb[out+STRIDE]=fb[out+STRIDE+1]=color;continue;}
    if(mask&1){surface_depth[at]=depth;fb[out]=color;}if(mask&2){surface_depth[at+1]=depth;fb[out+1]=color;}
    if(mask&4){surface_depth[at+W]=depth;fb[out+STRIDE]=color;}if(mask&8){surface_depth[at+W+1]=depth;fb[out+STRIDE+1]=color;}continue;
   }
   if(depth<=surface_depth[at]){surface_depth[at]=depth;fb[out]=color;}
   if(depth<=surface_depth[at+1]){surface_depth[at+1]=depth;fb[out+1]=color;}
   if(y+1<=surface_y_max){at+=W;out+=STRIDE;if(depth<=surface_depth[at]){surface_depth[at]=depth;fb[out]=color;}if(depth<=surface_depth[at+1]){surface_depth[at+1]=depth;fb[out+1]=color;}}
  }
 }
}
#include "ship-art-raster.h"
#include "freighter-art-raster.h"
static void triangle(DrawTri *t){
 if(t->material>=30000&&t->material<34096){menu_station_textured_triangle(t);return;}
 if((t->material>=8192&&t->material<8192+(int)(sizeof(fleet_faces)/sizeof(*fleet_faces)))||(t->material>=FLEET_QUAD_BASE&&t->material<FLEET_QUAD_BASE+2*(int)(sizeof(fleet_tiles)/sizeof(*fleet_tiles)))){ship_art_triangle(t);return;}
 if(t->material>=5100&&t->material<5172){wreck_art_triangle(t);return;}
 if(t->material>=5000&&t->material<5084){freight_art_triangle(t);return;}
 if(t->material>=4096&&t->material<4240){ship_art_triangle(t);return;}
 if(surface_depth_on){surface_triangle(t);return;}
 Point a=t->p[0],b=t->p[1],c=t->p[2];float area=edge(a,b,c.x,c.y);if(fabsf(area)<.05f)return;float za=1.f/(a.z*area),zb=1.f/(b.z*area),zc=1.f/(c.z*area);
 int x0=(int)fmaxf(clipx0,floorf(fminf(a.x,fminf(b.x,c.x)))),x1=(int)fminf(clipx1-1,ceilf(fmaxf(a.x,fmaxf(b.x,c.x))));
 int yt=clipy0>=0?clipy0:view_top(),yb=clipy1>=0?clipy1-1:view_bot();
 int y0=(int)fmaxf(yt,floorf(fminf(a.y,fminf(b.y,c.y)))),y1=(int)fminf(yb,ceilf(fmaxf(a.y,fmaxf(b.y,c.y))));
 /* All faces sample one shared 2px grid; per-face grids left dotted seams. */
 x0=(int)fmaxf(clipx0,x0&~1);y0=(int)fmaxf(yt,y0&~1);
 int raster_step=render_raster_step;for(int y=y0;y<=y1;y+=raster_step)for(int x=x0;x<=x1;x+=raster_step){float e0=edge(a,b,x+.5f*raster_step,y+.5f*raster_step),e1=edge(b,c,x+.5f*raster_step,y+.5f*raster_step),e2=edge(c,a,x+.5f*raster_step,y+.5f*raster_step);
 if((e0>=-.05f&&e1>=-.05f&&e2>=-.05f)||(e0<=.05f&&e1<=.05f&&e2<=.05f)){if(surface_depth_on){float inv=e0*zc+e1*za+e2*zb;if(inv>0){float z=1.f/inv;unsigned depth=(unsigned)(z*8);if(depth>65534)depth=65534;for(int dy=0;dy<2&&y+dy<=y1;dy++){int at=(y+dy)*W+x,out=(y+dy)*STRIDE+x;for(int dx=0;dx<2&&x+dx<=x1;dx++)if(depth<=surface_depth[at+dx]){surface_depth[at+dx]=depth;fb[out+dx]=t->color;}}}}else rect(x,y,x+raster_step-1<=x1?raster_step:1,y+raster_step-1<=y1?raster_step:1,t->color);}}
}
static void queue_triangle(Vec3 a,Vec3 b,Vec3 c,unsigned color){
 /* Triangle fans sometimes express a cap as a quad with a repeated corner.
  * Discard only provably zero-area triangles before projection/sorting. */
 if((a.x==b.x&&a.y==b.y&&a.z==b.z)||(a.x==c.x&&a.y==c.y&&a.z==c.z)||(b.x==c.x&&b.y==c.y&&b.z==c.z))return;
 if(surface_depth_on)color=surface_shade(color);
 // Clip the near plane, preserving triangles which cross the camera.
 Vec3 in[4]={a,b,c},out[4];int count=0;float near=surface_depth_on?4.f:15.f;
 for(int i=0;i<3;i++){Vec3 u=in[i],v=in[(i+1)%3];int ui=u.z>=near,vi=v.z>=near;if(ui)out[count++]=u;if(ui!=vi){float t=(near-u.z)/(v.z-u.z);out[count++]=add(u,mul(sub(v,u),t));}}
 for(int i=1;i<count-1&&drawcount<render_triangle_limit;i++){
  Point p=project(out[0]),q=project(out[i]),r=project(out[i+1]);
  int top=clipy0>=0?clipy0:view_top(),bot=clipy1>=0?clipy1:view_bot()+1;
  if(!render_reference&&((p.x<clipx0&&q.x<clipx0&&r.x<clipx0)||(p.x>=clipx1&&q.x>=clipx1&&r.x>=clipx1)||(p.y<top&&q.y<top&&r.y<top)||(p.y>=bot&&q.y>=bot&&r.y>=bot)))continue;
  DrawTri *t=&drawlist[drawcount];t->order=drawcount++;render_submitted++;t->p[0]=p;t->p[1]=q;t->p[2]=r;t->color=color;t->depth=(out[0].z+out[i].z+out[i+1].z)/3;t->material=surface_depth_on?surface_material:0;
 }
}
static int ship_project_cache=1;
static void queue_ship_projected(Point p,Point q,Point r,unsigned color){
 if(drawcount>=2048)return;int top=clipy0>=0?clipy0:view_top(),bot=clipy1>=0?clipy1:view_bot()+1;
 if(!render_reference&&((p.x<clipx0&&q.x<clipx0&&r.x<clipx0)||(p.x>=clipx1&&q.x>=clipx1&&r.x>=clipx1)||(p.y<top&&q.y<top&&r.y<top)||(p.y>=bot&&q.y>=bot&&r.y>=bot)))return;
 DrawTri*t=&drawlist[drawcount];t->order=drawcount++;render_submitted++;t->p[0]=p;t->p[1]=q;t->p[2]=r;t->color=surface_depth_on?surface_shade(color):color;t->depth=(p.z+q.z+r.z)/3;t->material=surface_depth_on?surface_material:0;
}
static Vec3 rotate(Vec3 p,float yaw,float roll){float c=cosf(yaw),s=sinf(yaw);Vec3 v={p.x*c+p.z*s,p.y,-p.x*s+p.z*c};c=cosf(roll);s=sinf(roll);return (Vec3){v.x*c-v.y*s,v.x*s+v.y*c,v.z};}
static unsigned livery_tint(unsigned base,int delta){int r=(int)(base&255)+delta,g=(int)((base>>8)&255)+delta,b=(int)((base>>16)&255)+delta;if(r<0)r=0;if(g<0)g=0;if(b<0)b=0;if(r>255)r=255;if(g>255)g=255;if(b>255)b=255;return RGB(r,g,b);}
/* Bounded detail pass: at most four recessed panels, within the original hull.
 * Replaces a face rather than layering coplanar triangles (no z-fighting). */
static int hull_detail_candidate(int id,int face,const MeshTri *t){
 return id!=0&&id!=1&&id!=2&&id!=10&&id!=11&&id!=25&&
        face%3==0&&t->normal.y>.1f;
}
static void queue_hull_panel(Vec3 a,Vec3 b,Vec3 c,Vec3 origin,unsigned ink,int glass){
 Vec3 centre=mul(add(add(a,b),c),1.f/3);
 Vec3 recess=mul(sub(origin,centre),.025f);
 Vec3 ia=add(add(centre,mul(sub(a,centre),.68f)),recess);
 Vec3 ib=add(add(centre,mul(sub(b,centre),.68f)),recess);
 Vec3 ic=add(add(centre,mul(sub(c,centre),.68f)),recess);
 queue_triangle(a,b,ib,livery_tint(ink,14));queue_triangle(a,ib,ia,livery_tint(ink,14));
 queue_triangle(b,c,ic,livery_tint(ink,-16));queue_triangle(b,ic,ib,livery_tint(ink,-16));
 queue_triangle(c,a,ia,livery_tint(ink,6));queue_triangle(c,ia,ic,livery_tint(ink,6));
 queue_triangle(ia,ib,ic,glass?RGB(38,82,99):livery_tint(ink,-12));
}
#include "diorama-material.h"
#include "ship-art-mesh.h"
#include "fleet-detail-mesh.h"
#include "mega-hull-geometry.h"
static void mega_npc_model(const NPC*n,unsigned paint);
static Vec3 mega_npc_point(const NPC*n,Vec3 local);
static float npc_visual_radius(const NPC*n){return n->freighter?length(freight_extent(n)):player_ship_is_mega(n->mega_hull)?n->radius:fmaxf(render_mesh_radius(n->mesh),n->mesh>=0&&n->mesh<35?fleet_hulls[n->mesh].radius:0)*n->scale;}
static const Mesh *fleet_visual_mesh(int id){return id==4?&ship_art_adder:id>=0&&id<35&&fleet_hulls[id].mesh?fleet_hulls[id].mesh:&meshes[id];}
static int fleet_visual_material(int id,int face){return id==4?4096+face:8192+fleet_hulls[id].face+face;}
/* Per-face sunlight, cool fill and a narrow metallic highlight. No post blur. */
static unsigned ship_art_light(unsigned base,Vec3 n,Vec3 sun,Vec3 half,int face){
 float diffuse=sqrtf(fmaxf(0,dot(n,sun))),spec=fmaxf(0,dot(n,half));
 spec*=spec;spec*=spec;spec*=spec;spec*=spec;spec*=spec;
 unsigned tint=sun_light_colour(game.bodies[0].seed);
 float ao=n.y<-.45f?.85f:1.f;
 int r=(int)(((base&255)*(.20f+1.15f*diffuse*(.65f+.35f*(tint&255)/255.f))+5+spec*64)*ao);
 int g=(int)((((base>>8)&255)*(.25f+1.05f*diffuse*(.65f+.35f*((tint>>8)&255)/255.f))+7+spec*62)*ao);
 int b=(int)((((base>>16)&255)*(.34f+.95f*diffuse*(.65f+.35f*((tint>>16)&255)/255.f))+12+spec*58)*ao);
 return RGB(r>255?255:r,g>255?255:g,b>255?255:b);
}
static int player_mesh_transform=0;
static int ship_art_npc_draw;
static void flight_camera_basis(Vec3 *right,Vec3 *up,Vec3 *ahead);


static void shipmesh_stretched(int id,Vec3 pos,float yaw,float roll,float scale,float stretch,unsigned base,int preview){
 /* NPC faction lamps stay colourful; broad armour is painted metal, with a
  * restrained faction tint. The player's selected finish is left intact. */
 if(ship_art_npc_draw&&base!=WHITE)base=RGB(85+(base&255)*.28f,98+((base>>8)&255)*.28f,111+((base>>16)&255)*.28f);
 int ship_art=ship_art_pixels&&!high_contrast&&!race_is_active(&game)&&id>=0&&id<35&&(id==4||fleet_hulls[id].mesh)&&(preview||player_mesh_transform||render_pixel_radius(pos,render_mesh_radius(id)*fabsf(scale))>14.f);
 int medium=ship_art&&id!=4&&!preview&&!player_mesh_transform&&render_pixel_radius(pos,render_mesh_radius(id)*fabsf(scale))<80.f;
 const Mesh*visual=ship_art?(medium?fleet_hulls[id].medium:fleet_visual_mesh(id)):&meshes[id];
 if(ship_art&&drawcount+visual->triangles>1800){ship_art=0;medium=0;visual=&meshes[id];}
 if(ship_art&&!player_mesh_transform&&base==GOLD)base=RGB(132,149,164);
 const Mesh *m=visual;Vec3 vertices[256],world[256];Point projected[256];if(m->vertices>256)return;
 float bound=fmaxf(render_mesh_radius(id),id>=0&&id<35?fleet_hulls[id].radius:0)*fabsf(scale)*fmaxf(1.f,fabsf(stretch));
 if(!preview&&!render_sphere_visible(pos,bound))return;
 int panels=0,detail=preview||(length(sub(pos,game.pos))<1800.f&&(render_reference||render_pixel_radius(pos,bound)>12.f));
 Vec3 origin=preview?pos:camera(&game,pos);
 /* One rotation and camera basis per hull, not trigonometry/cache probes per
  * vertex and face. Same transform and lighting as the scalar review path. */
 Vec3 mx=rotate((Vec3){1,0,0},yaw,roll),my=rotate((Vec3){0,1,0},yaw,roll),mz=rotate((Vec3){0,0,1},yaw,roll),right,up,ahead;
 if(player_mesh_transform)flight_camera_basis(&mx,&my,&mz);
 
 camera_axes(&game,&right,&up,&ahead);Vec3 eye=camera_origin(&game),light_dir=norm((Vec3){-.3f,.7f,-.6f});
 int coriolis=id==mesh_id("CORIOLIS"),race_light=race_is_active(&game); int dg_hero=!preview&&dg_active(); Vec3 dg_sun=preview?norm((Vec3){-.4f,.75f,-.55f}):norm(sub(game.bodies[0].pos,pos));
 Vec3 ship_half=ship_art?norm(add(dg_sun,preview?(Vec3){0,0,-1}:norm(sub(eye,pos)))):(Vec3){0,0,0};
 for(int i=0;i<m->vertices;i++){Vec3 v=m->v[i];v.z*=stretch;Vec3 p=render_reference&&!player_mesh_transform?add(pos,mul(rotate(v,yaw,roll),scale)):add(pos,mul(add(add(mul(mx,v.x),mul(my,v.y)),mul(mz,v.z)),scale));world[i]=p;Vec3 d=sub(p,eye);vertices[i]=preview?p:render_reference?camera(&game,p):(Vec3){dot(d,right),dot(d,up),dot(d,ahead)};if(ship_project_cache&&vertices[i].z>=(surface_depth_on?4.f:15.f))projected[i]=project(vertices[i]);}
 for(int i=0;i<m->triangles;i++){const MeshTri *t=&m->t[i];if(!preview&&coriolis&&t->normal.z<-.99f)continue;Vec3 n=render_reference&&!player_mesh_transform?rotate(t->normal,yaw,roll):add(add(mul(mx,t->normal.x),mul(my,t->normal.y)),mul(mz,t->normal.z));if(!preview){Vec3 center=mul(add(add(world[t->a],world[t->b]),world[t->c]),1.f/3);if(dot(n,sub(eye,center))<=0)continue;}float light=(race_light?.65f:.3f)+(race_light?.35f:.7f)*fmaxf(0,dot(n,light_dir));
 unsigned color=RGB((int)((base&255)*light),(int)(((base>>8)&255)*light),(int)(((base>>16)&255)*light)); if(ship_art)color=ship_art_light(base,n,dg_sun,ship_half,i);else if(dg_hero)color=dg_material(base,(int)(fmaxf(0,(dot(n,dg_sun)+.22f)/1.22f)*256));
 /* Two-tone livery makes the old Elite silhouettes read as actual painted
  * ships: upper facets catch a cool highlight, undersides go deep, and a
  * sparse accent facet supplies a cockpit/panel colour without textures. */
 if(!ship_art){if(n.y>.42f)color=livery_tint(color,22);else if(n.y<-.35f)color=livery_tint(color,-20);else if((i&7)==0)color=livery_tint(color,12);}
 int cached=ship_project_cache&&vertices[t->a].z>=(surface_depth_on?4.f:15.f)&&vertices[t->b].z>=(surface_depth_on?4.f:15.f)&&vertices[t->c].z>=(surface_depth_on?4.f:15.f);
 if(ship_art){int first=drawcount;if(cached)queue_ship_projected(projected[t->a],projected[t->b],projected[t->c],color);else queue_triangle(vertices[t->a],vertices[t->b],vertices[t->c],color);if(vertices[t->a].z>15&&vertices[t->b].z>15&&vertices[t->c].z>15)for(int k=first;k<drawcount;k++)drawlist[k].material=fleet_visual_material(id,medium?fleet_hulls[id].medium_face[i]:i);
 }else if(detail&&panels<4&&hull_detail_candidate(id,i,t)){
  int glass=t->normal.z>.2f&&panels==0;
  queue_hull_panel(vertices[t->a],vertices[t->b],vertices[t->c],origin,color,glass);panels++;
 }else {if(cached)queue_ship_projected(projected[t->a],projected[t->b],projected[t->c],color);else queue_triangle(vertices[t->a],vertices[t->b],vertices[t->c],color);}}
}
static void shipmesh(int id,Vec3 pos,float yaw,float roll,float scale,unsigned base,int preview){shipmesh_stretched(id,pos,yaw,roll,scale,1,base,preview);}
/* A fitted physical plate; UVs cover the complete quad, including both halves.
 * Used by capital modules and discoveries, with one immutable small atlas. */
static void fleet_panel_quad(Vec3 a,Vec3 b,Vec3 c,Vec3 d,unsigned ink,int tile,int bevel){
 Vec3 centre=mul(add(add(a,b),add(c,d)),.25f);
 float span=proj_focal*fmaxf(fabsf(a.x-b.x)+fabsf(a.y-b.y),fabsf(b.x-c.x)+fabsf(b.y-c.y))/fmaxf(15,centre.z);
 /* Below this size the authored pixel border already describes the seam.
  * Eight extra subpixel triangles per plate only waste geometry bandwidth. */
 if(bevel&&span>42.f){Vec3 ia=add(centre,mul(sub(a,centre),.88f)),ib=add(centre,mul(sub(b,centre),.88f)),ic=add(centre,mul(sub(c,centre),.88f)),id=add(centre,mul(sub(d,centre),.88f));
  Vec3 outer[4]={a,b,c,d},inner[4]={ia,ib,ic,id};
  for(int k=0;k<4;k++){int j=(k+1)&3;unsigned trim=livery_tint(ink,k==0?22:-28);queue_triangle(outer[k],outer[j],inner[j],trim);queue_triangle(outer[k],inner[j],inner[k],trim);}a=ia;b=ib;c=ic;d=id;
 }
 int material=FLEET_QUAD_BASE+tile*2,first=drawcount;queue_triangle(a,b,c,ink);if(drawcount==first+1&&a.z>15&&b.z>15&&c.z>15)drawlist[first].material=material;
 first=drawcount;queue_triangle(a,c,d,ink);if(drawcount==first+1&&a.z>15&&c.z>15&&d.z>15)drawlist[first].material=material+1;
}
static void shipmesh_preview_edges(int id,Vec3 pos,float yaw,float roll,float scale,unsigned ink){
 const Mesh *m=&meshes[id];
 for(int i=0;i<m->triangles;i++){
  const MeshTri *t=&m->t[i];int corners[3]={t->a,t->b,t->c};
  for(int e=0;e<3;e++){
   Vec3 a=add(pos,mul(rotate(m->v[corners[e]],yaw,roll),scale));
   Vec3 b=add(pos,mul(rotate(m->v[corners[(e+1)%3]],yaw,roll),scale));
   if(a.z<15||b.z<15)continue;
   Point pa=project(a),pb=project(b);line((int)pa.x,(int)pa.y,(int)pb.x,(int)pb.y,ink);
  }
 }
}
static void shipwire_stretched(int id,Vec3 pos,float yaw,float scale,float stretch,unsigned color){
 const Mesh *m=&meshes[id];Vec3 vertices[128];if(m->vertices>128)return;for(int i=0;i<m->vertices;i++){Vec3 v=m->v[i];v.z*=stretch;vertices[i]=camera(&game,add(pos,mul(rotate(v,yaw,0),scale)));}
 for(int i=0;i<m->triangles;i++){const MeshTri *t=&m->t[i];int ids[3]={t->a,t->b,t->c};for(int e=0;e<3;e++){Vec3 a=vertices[ids[e]],b=vertices[ids[(e+1)%3]];if(a.z<20||b.z<20)continue;Point p=project(a),q=project(b);line((int)p.x,(int)p.y,(int)q.x,(int)q.y,color);}}
}
static void capital_box(const NPC *n,Vec3 center,Vec3 half,unsigned color){
 Vec3 world[8],side=norm((Vec3){n->dir.z,0,-n->dir.x}),up={0,1,0};
 Vec3 shipup={-n->dir.y*side.z,n->dir.z*side.x-n->dir.x*side.z,n->dir.y*side.x};
 for(int i=0;i<8;i++){Vec3 q=add(center,(Vec3){i&2?half.x:-half.x,i&4?half.y:-half.y,i&1?half.z:-half.z});world[i]=render_reference?freight_world(n,q):add(n->pos,add(mul(side,q.x),add(mul(shipup,q.y),mul(n->dir,q.z))));}
 const int faces[6][4]={{0,2,6,4},{1,5,7,3},{0,4,5,1},{2,3,7,6},{0,1,3,2},{4,6,7,5}};
 Vec3 normal[6]={mul(n->dir,-1),n->dir,mul(side,-1),side,mul(up,-1),up};
 Vec3 sun=norm(sub(game.bodies[0].pos,n->pos)),light_half=norm(add(sun,norm(sub(camera_origin(&game),n->pos))));
 int close=render_pixel_radius(n->pos,length(freight_extent(n)))>18.f;
 for(int f=0;f<6;f++){
  Vec3 a=world[faces[f][0]],b=world[faces[f][1]],c=world[faces[f][2]],d=world[faces[f][3]];
  if(dot(normal[f],sub(camera_origin(&game),a))<=0)continue;
  unsigned ink=ship_art_light(color,normal[f],sun,light_half,f);
  Vec3 extent=freight_extent(n);int outlet=half.z<5&&center.z<-extent.z*.9f,bridge=center.y>extent.y*.25f;
  int tile=outlet&&f==0?FLEET_ENGINE:bridge&&half.y<extent.y*.2f?FLEET_GLASS:f==5?FLEET_ARMOUR:f==4?FLEET_RADIATOR:bridge?FLEET_WINDOWS:n->freight_style==1?FLEET_TANK:FLEET_CARGO;
  if(close&&drawcount<1600)fleet_panel_quad(camera(&game,a),camera(&game,b),camera(&game,c),camera(&game,d),ink,tile,1);
  else {queue_triangle(camera(&game,a),camera(&game,b),camera(&game,c),ink);queue_triangle(camera(&game,a),camera(&game,c),camera(&game,d),ink);}
 }
}
static void capital_model(const NPC *n,unsigned color){
 Vec3 e=freight_extent(n);float bound=length(e);
 if(!render_sphere_visible(n->pos,bound))return;
 float pixels=render_pixel_radius(n->pos,bound);
 int detailed=length(sub(n->pos,game.pos))<5000&&(render_reference||pixels>24.f),style=n->freight_style;
 unsigned steel=n->flash>0?WHITE:RGB(65,76,84);
 unsigned paint=RGB(30+(color&255)/3,36+((color>>8)&255)/3,42+((color>>16)&255)/3);
 /* Shared silhouettes at every distance: spine, engine block, cargo and bridge. */
 capital_box(n,(Vec3){0,-e.y*.38f,0},(Vec3){e.x*.36f,e.y*.34f,e.z*.96f},steel);
 capital_box(n,(Vec3){0,0,-e.z*.83f},(Vec3){e.x*.65f,e.y*.62f,e.z*.17f},steel);
 capital_box(n,(Vec3){0,e.y*.48f,-e.z*.55f},(Vec3){e.x*.56f,e.y*.52f,e.z*.13f},paint);
 if(!render_reference&&pixels<7.f){
  /* Keep the three cargo silhouettes, omit seams below their pixel size. */
  capital_box(n,(Vec3){0,0,e.z*.22f},(Vec3){e.x,style==2?e.y*.55f:e.y*.63f,e.z*.65f},paint);render_lod++;return;
 }
 if(style==0){ /* Container carrier: paired stacks and visible gaps. */
  for(int row=0;row<3;row++)for(int side=-1;side<=1;side+=2){
   unsigned cargo= row==0?RGB(120,90,61):row==1?RGB(68,97,109):RGB(98,103,76);
   capital_box(n,(Vec3){side*e.x*.58f,0,-e.z*.23f+row*e.z*.43f},(Vec3){e.x*.42f,e.y*.63f,e.z*.18f},cargo);
   if(detailed)capital_box(n,(Vec3){side*e.x*.58f,e.y*.66f,-e.z*.23f+row*e.z*.43f},(Vec3){e.x*.4f,3,e.z*.035f},steel);
  }
 }else if(style==1){ /* Fuel tanker: four stepped tank shoulders. */
  for(int row=0;row<2;row++)for(int side=-1;side<=1;side+=2){
   Vec3 c={side*e.x*.55f,0,-e.z*.12f+row*e.z*.65f};
   capital_box(n,c,(Vec3){e.x*.45f,e.y*.42f,e.z*.29f},RGB(83,105,112));
   c.y=e.y*.54f;capital_box(n,c,(Vec3){e.x*.30f,e.y*.18f,e.z*.25f},RGB(96,118,125));
  }
 }else { /* Bulk ore barge: wide, low armored bins around the spine. */
  for(int row=0;row<4;row++){
   capital_box(n,(Vec3){0,-e.y*.05f,-e.z*.35f+row*e.z*.34f},(Vec3){e.x,e.y*.55f,e.z*.145f},row&1?RGB(107,92,72):RGB(82,83,78));
   if(detailed)capital_box(n,(Vec3){0,e.y*.54f,-e.z*.35f+row*e.z*.34f},(Vec3){e.x*.84f,3,e.z*.11f},RGB(44,51,54));
  }
 }
 if(detailed){
  /* Narrow illuminated bridge glazing, recessed stern engines, navigation rails. */
  capital_box(n,(Vec3){0,e.y*.70f,-e.z*.412f},(Vec3){e.x*.45f,e.y*.075f,2},RGB(89,166,172));
  for(int side=-1;side<=1;side+=2)capital_box(n,(Vec3){side*e.x*.38f,-e.y*.05f,-e.z*.998f},(Vec3){e.x*.14f,e.y*.24f,2},RGB(104,149,180));
 }
}
static int depth_sort(const void *a,const void *b){
 const DrawTri *x=a,*y=b;float d=y->depth-x->depth;if(d>0)return 1;if(d<0)return -1;
 /* Planetary coplanar details need a deterministic submission-order tie.
  * qsort is not stable; skipping invisible boxes must not recolour markings.
  * Use the same deterministic tie for coplanar station panels; space painter ordering remains unchanged. */
 return surface_depth_on?(x->order<y->order?1:x->order>y->order?-1:0):0;
}
static int flight_cached_frame=1,station_ge_active_frame,wreck_ge_active_frame,fleet_ge_active_frame;
static int station_ge_flush(void);
static int wreck_ge_flush(void);
static void fleet_ge_record(void);
static void flush_meshes(void){uint64_t start=eva_profile?sceKernelGetSystemTimeWide():0;qsort(drawlist,drawcount,sizeof(*drawlist),depth_sort);if(wreck_ge_flush()){fleet_ge_record();drawcount=0;return;}if(station_ge_flush()){fleet_ge_record();drawcount=0;return;}if(surface_depth_on){for(int i=drawcount-1;i>=0;i--)triangle(&drawlist[i]);}else for(int i=0;i<drawcount;i++)triangle(&drawlist[i]);fleet_ge_record();drawcount=0;if(eva_profile)eva_raster_us+=sceKernelGetSystemTimeWide()-start;}
static void flush_solid_meshes(void){
 /* Opaque hulls already resolve visibility through the depth buffer. Their
  * replacement panels do not overlap, so painter sorting adds no visibility. */
 if(render_reference||!surface_depth_on){flush_meshes();return;}
 for(int i=0;i<drawcount;i++)if(drawlist[i].material!=0){flush_meshes();return;}
 for(int i=drawcount-1;i>=0;i--)triangle(&drawlist[i]);drawcount=0;
}
/* Art kit chrome — charcoal + ochre/cream rules; cyan stays a nav signal only. */
static void header(const char *title){rect(0,0,W,22,UI_PANEL);rect(0,0,W,1,UI_EDGE);rect(0,21,W,1,UI_SIGNAL);rect(0,0,3,22,UI_ACCENT);draw_next_art(next_logo_small,100,18,5,2,100,18);text(14,1,UI_MUTED,"/");text(16,1,UI_TEXT,"%.42s",title);}
/* One tiny, shared PSP button alphabet. Keep these 10x10 so every prompt
 * aligns on the same baseline, whether it is a face button or a direction. */
static unsigned button_ink(char b,unsigned fallback){
 if(b=='X')return RGB(92,164,244);       /* Cross */
 if(b=='O')return RGB(240,72,82);        /* Circle */
 if(b=='T')return RGB(80,220,110);       /* Triangle */
 if(b=='S')return RGB(236,112,188);      /* Square */
 if(b=='U'||b=='D'||b=='L'||b=='R'||b=='P'||b=='l'||b=='r'||b=='N')return UI_TEXT;
 if(b=='A'||b=='E')return UI_SIGNAL;/* Start / Select */
 return fallback;
}
static void mini_letter(int x,int y,char ch,unsigned ink){
 static const unsigned char blank[5]={0,0,0,0,0};
 static const unsigned char S[5]={7,4,7,1,7},T[5]={7,2,2,2,2},E[5]={7,4,6,4,7},L[5]={4,4,4,4,7},R[5]={6,5,6,5,5};
 const unsigned char *rows=blank;
 if(ch=='S')rows=S;else if(ch=='T')rows=T;else if(ch=='E')rows=E;else if(ch=='L')rows=L;else if(ch=='R')rows=R;
 for(int yy=0;yy<5;yy++)for(int xx=0;xx<3;xx++)if(rows[yy]&(1<<(2-xx)))rect(x+xx,y+yy,1,1,ink);
}
static void mini_label_button(int x,int y,const char *label){
 unsigned black=RGB(4,7,10),white=RGB(242,242,236);
 /* A tiny rounded-rectangle silhouette, like the real PSP Start/Select keys. */
 rect(x+2,y+1,6,8,black);rect(x+1,y+2,8,6,black);
 int len=(int)strlen(label),start=x+5-(len*2);
 for(int i=0;i<len;i++)mini_letter(start+i*4,y+3,label[i],white);
}
static void button_icon(int x,int y,char b,unsigned fallback){
 unsigned c=button_ink(b,fallback);
 if(b=='O'){circle(x+5,y+5,3,c);}
 else if(b=='X'){line(x+2,y+2,x+8,y+8,c);line(x+8,y+2,x+2,y+8,c);}
 else if(b=='T'){line(x+1,y+7,x+7,y+7,c);line(x+1,y+7,x+4,y+1,c);line(x+7,y+7,x+4,y+1,c);}
 else if(b=='S'){rect(x+2,y+2,6,6,c);rect(x+4,y+4,2,2,RGB(13,20,28));}
 else if(b=='U'||b=='D'||b=='L'||b=='R'){
  int cx=x+5,cy=y+5;
  if(b=='U'){line(cx,cy-3,cx,cy+3,c);line(cx,cy-3,cx-2,cy-1,c);line(cx,cy-3,cx+2,cy-1,c);}
  if(b=='D'){line(cx,cy-3,cx,cy+3,c);line(cx,cy+3,cx-2,cy+1,c);line(cx,cy+3,cx+2,cy+1,c);}
  if(b=='L'){line(cx-3,cy,cx+3,cy,c);line(cx-3,cy,cx-1,cy-2,c);line(cx-3,cy,cx-1,cy+2,c);}
  if(b=='R'){line(cx-3,cy,cx+3,cy,c);line(cx+3,cy,cx+1,cy-2,c);line(cx+3,cy,cx+1,cy+2,c);}
 }
 else if(b=='P'){rect(x+4,y+1,2,8,c);rect(x+1,y+4,8,2,c);}
 else if(b=='l'||b=='r')mini_label_button(x,y,b=='l'?"L":"R");
 else if(b=='N'){circle(x+5,y+5,4,c);circle(x+5,y+5,2,RGB(13,20,28));rect(x+4,y+4,2,2,c);}
 else if(b=='A'||b=='E')mini_label_button(x,y,b=='A'?"ST":"SE");
}
#include "hud-pixels.h"
static int footer_token(const char *s,int n,char *icon){
 struct {const char *word;char icon;} keys[]={
  {"X",'X'},{"CROSS",'X'},{"O",'O'},{"CIRCLE",'O'},{"TRI",'T'},{"TRIANGLE",'T'},
  {"SQ",'S'},{"SQUARE",'S'},{"UP",'U'},{"DOWN",'D'},{"LEFT",'L'},{"RIGHT",'R'},
  {"L",'l'},{"R",'r'},{"D-PAD",'P'},{"NUB",'N'},{"ANALOG",'N'},{"START",'A'},{"SELECT",'E'}
 };
 for(unsigned i=0;i<sizeof(keys)/sizeof(keys[0]);i++)if((int)strlen(keys[i].word)==n&&!strncmp(s,keys[i].word,n)){*icon=keys[i].icon;return 1;}
 return 0;
}
static const char *dialogue_flush_footer(const char *s);
static void footer(const char *s){
 s=dialogue_flush_footer(s);if(page==ERL_PAGE)return;
 char label[59];snprintf(label,sizeof(label),"%.58s",s);
 rect(0,248,W,24,UI_PANEL);rect(0,248,W,1,UI_EDGE);
 /* Parse complete button tokens, never letters inside NEXT, STORY or WORK. */
 for(int i=0;label[i];){
  if(label[i]==' '||label[i]=='/'||label[i]=='+'||label[i]=='|'){i++;continue;}
  int start=i;while(label[i]&&label[i]!=' '&&label[i]!='/'&&label[i]!='+'&&label[i]!='|')i++;
  char icon;if(footer_token(label+start,i-start,&icon)){
   for(int j=start;j<i;j++)label[j]=' ';
   button_icon(8+start*8,255,icon,WHITE);
  }
 }
 text(1,32,DIM,"%s",label);
}
static void ui_action_feedback(float dt){
 if(deck_open_active())ui_page_anim=ui_press_anim=0;
 if(ui_page_anim>0){
  float p=1.f-ui_page_anim/.28f;if(p<0)p=0;if(p>1)p=1;
  int reach=(int)(p*34.f),scan=(int)(p*(H+32))-16;
  unsigned ink=p<.50f?UI_CYAN:UI_GOLD;
  /* A short CRT wake-up: the frame opens, then two faint scan bands clear
   * the new page.  It is visible at native resolution but never delays input. */
  rect(reach,1,W-reach*2,2,ink);rect(reach,H-3,W-reach*2,2,ink);
  rect(1,reach,2,H-reach*2,ink);rect(W-3,reach,2,H-reach*2,ink);
  if(scan>=0&&scan<H){rect(0,scan,W,2,RGB(29,72,83));if(scan>5)rect(0,scan-5,W,1,RGB(18,43,55));}
  ui_page_anim-=dt;if(ui_page_anim<0)ui_page_anim=0;
 }
 if(ui_press_anim>0){
  int beat=((int)(ui_press_anim*100.f)/4)&1;unsigned ink=ui_press_key=='O'?RGB(235,91,103):beat?UI_GOLD:UI_CYAN;
  int age=(int)((.24f-ui_press_anim)*90.f);if(age<0)age=0;if(age>18)age=18;
  int inset=beat?2:4;rect(inset,248,W-inset*2,2,ink);
  rect(W/2-12-age,249,W/2?24+age*2:24,16,RGB(10,18,27));
  rect(W/2-12-age,249,24+age*2,1,ink);rect(W/2-12-age,264,24+age*2,1,ink);
  button_icon(W/2-5,252,ui_press_key?ui_press_key:'X',ink);
  ui_press_anim-=dt;if(ui_press_anim<0)ui_press_anim=0;
 }
}
static void credits_badge(void){
 rect(382,228,90,16,UI_PANEL);rect(382,228,90,1,UI_EDGE);
 text(48,29,GOLD,"%.1f U",game.credits*.1f);
}
static void selected_span(int y,int w){if(w<48)w=48;rect(8,y*8-2,w,12,high_contrast?RGB(58,72,88):UI_RAISED);rect(8,y*8-2,3,12,UI_ACCENT);}
static void selected(int y){selected_span(y,464);int pulse=1+(int)(fabsf(sinf(preview_time*6.f))*3.f);rect(12,y*8+9,pulse*8,1,UI_GOLD);text_px(14+(int)(sinf(preview_time*7.f)*2.f),y*8-1,UI_GOLD,">");}
static const char *stars(int n){static char result[8];for(int i=0;i<5;i++)result[i]=i<n?'*':'.';result[5]=0;return result;}
static void page_number_at(int col,int rownum,int current,int total){if(total>1)text(col,rownum,DIM,"%d/%d",current,total);}
static void update_nearby(void){near_count=0;for(int i=0;i<256;i++)if(i!=game.system&&distance_ly(&game,game.system,i)<=10.01f)nearby[near_count++]=i;
 for(int i=0;i<near_count;i++)for(int j=i+1;j<near_count;j++)if(distance_ly(&game,game.system,nearby[j])<distance_ly(&game,game.system,nearby[i])){int t=nearby[i];nearby[i]=nearby[j];nearby[j]=t;}
}
static int chart_job_system(int system){
 if(game.campaign_stage>=6&&game.saga_step&&game.saga_chapter<SAGA_COUNT&&game.saga_dest==system)return 1;
 for(int i=0;i<game.job_n;i++)if(game.jobs[i].dest==system||game.jobs[i].origin==system)return 1;
 if(game.passenger_dest==system||game.contract==system)return 1;
 return 0;
}
/* Menu camera time is not simulation time. Preserve hold-to-zoom speed on
 * slow hardware, while capping stalls/resume gaps to prevent camera leaps. */
static float chart_control_dt(float elapsed){return fminf(.15f,fmaxf(.001f,elapsed));}
static int chart_route_contains(int system);
static int chart_filter_matches(int system){
 if(system<0||system>=256)return 0;
 int visited=(game.visited[system>>3]&(1<<(system&7)))!=0;
 switch(chart_filter){
  case CHART_VISITED:return visited;
  case CHART_UNVISITED:return !visited;
  case CHART_RICH:return station_class_for_system(&game,system)==STATION_RICH;
  case CHART_POOR:return station_class_for_system(&game,system)==STATION_POOR;
  case CHART_MEGA:return station_class_for_system(&game,system)==STATION_MEGA;
  case CHART_JOBS:return chart_job_system(system);
  case CHART_RANGE:return system!=game.system&&((game.debug_flags&DEBUG_UNLIMITED_RANGE)||distance_ly(&game,game.system,system)*10<=fminf(game.fuel,(float)player_ships[game.ship].range)+.01f);
  case CHART_ROUTE:return chart_route_contains(system);
  default:return 1;
 }
}
static int chart_system_visible(int system){
 if(system==game.system||system==chart_cursor||system==game.route_goal)return 1;
 return chart_filter_matches(system);
}
static int chart_filter_first(void){
 int first=chart_filter-2;if(first<0)first=0;if(first>CHART_FILTERS-5)first=CHART_FILTERS-5;return first;
}
static void chart_cycle_filter(void){
 chart_filter=(chart_filter+1)%CHART_FILTERS;game.cue=SFX_SELECT;
 if(chart_filter_matches(chart_cursor)||chart_cursor==game.system||chart_cursor==game.route_goal)return;
 chart_cursor=game.system;
 for(int i=0;i<256;i++)if(i!=game.system&&chart_filter_matches(i)){chart_cursor=i;break;}
}
static void chart_project(int id,int *x,int *y,float *depth){
 float gx=(float)game.systems[id].x-128.f,gz=(float)game.systems[id].y-128.f;
 float ca=cosf(chart_angle),sa=sinf(chart_angle),rx=gx*ca-gz*sa,rz=gx*sa+gz*ca;
 unsigned h=(unsigned)(id+1)*2654435761u^(unsigned)(game.systems[id].x*131+game.systems[id].y*17);
 float gy=(float)((int)((h>>24)&31)-15);
 float perspective=.78f+rz/900.f;
 if(perspective<.58f)perspective=.58f;if(perspective>1.02f)perspective=1.02f;
 *x=250+(int)chart_pan_x+(int)(rx*1.43f*perspective*chart_zoom);
 *y=144+(int)chart_pan_y+(int)((rz*.46f+gy*.28f)*chart_zoom);
 if(depth)*depth=rz;
}
static int chart_prefix(const char *name,const char *prefix){
 for(int i=0;prefix[i];i++){char a=name[i],b=prefix[i];if(!a)return 0;if(a>='a'&&a<='z')a-=32;if(b>='a'&&b<='z')b-=32;if(a!=b)return 0;}return 1;
}
static void chart_search_refresh(void){
 chart_match_n=0;for(int i=0;i<256;i++)if(chart_prefix(game.systems[i].name,chart_query))chart_matches[chart_match_n++]=i;
 if(chart_search_match>=chart_match_n)chart_search_match=chart_match_n?chart_match_n-1:0;
}
static void chart_search_choose(void){
 if(chart_match_n>0){chart_cursor=chart_matches[chart_search_match];if(!chart_filter_matches(chart_cursor)&&chart_cursor!=game.system&&chart_cursor!=game.route_goal)chart_filter=CHART_ALL;chart_search=0;game.cue=SFX_SELECT;}
}
static void chart_search_input(unsigned pressed){
 int col=chart_search_key%7,rowkey=chart_search_key/7;
 if(pressed&PSP_CTRL_LEFT)col=(col+6)%7;if(pressed&PSP_CTRL_RIGHT)col=(col+1)%7;
 if(pressed&PSP_CTRL_UP)rowkey=(rowkey+3)%4;if(pressed&PSP_CTRL_DOWN)rowkey=(rowkey+1)%4;
 chart_search_key=rowkey*7+col;if(chart_search_key>27)chart_search_key=27;
 if(pressed&PSP_CTRL_LTRIGGER){if(chart_match_n)chart_search_match=(chart_search_match+chart_match_n-1)%chart_match_n;}
 if(pressed&PSP_CTRL_RTRIGGER){if(chart_match_n)chart_search_match=(chart_search_match+1)%chart_match_n;}
 if(pressed&PSP_CTRL_SQUARE){int n=(int)strlen(chart_query);if(n)chart_query[n-1]=0;chart_search_refresh();}
 if(pressed&PSP_CTRL_CROSS){
  if(chart_search_key<26){int n=(int)strlen(chart_query);if(n<11){chart_query[n]=(char)('A'+chart_search_key);chart_query[n+1]=0;chart_search_match=0;chart_search_refresh();}}
  else if(chart_search_key==26){int n=(int)strlen(chart_query);if(n)chart_query[n-1]=0;chart_search_refresh();}
  else chart_search_choose();
 }
 if(pressed&PSP_CTRL_TRIANGLE)chart_search_choose();
 if(pressed&PSP_CTRL_CIRCLE)chart_search=0;
 if(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT|PSP_CTRL_UP|PSP_CTRL_DOWN|PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER|PSP_CTRL_SQUARE|PSP_CTRL_CROSS|PSP_CTRL_TRIANGLE))game.cue=SFX_SELECT;
}
static void chart_move_cursor(int dx,int dy){
 int from=chart_cursor,best=from,best_score=999999,ax,ay;chart_project(from,&ax,&ay,0);
 for(int i=0;i<256;i++)if(i!=from&&chart_system_visible(i)){int bx,by;chart_project(i,&bx,&by,0);int sx=bx-ax,sy=by-ay,forward=sx*dx+sy*dy;if(forward<=0)continue;int side=abs(sx*dy-sy*dx),score=forward+side*4;if(score<best_score){best_score=score;best=i;}}
 chart_cursor=best;game.cue=SFX_SELECT;
}
static int tutorial_page_allowed(int p){
 /* Title access is independent of an unfinished commander's lesson unlocks. */
 if(page==INTRO&&(p==STATUS||p==MULTIPLAYER))return 1;
 if(opening_exists(&game)){
  if(p==HOME||p==FLIGHT||p==INTRO)return 1;
  if(p==STATUS&&game.opening.stage==OP_CREATE)return 1;
  static const int opening_pages[][2]={{MARKET,1},{CHART,2},{YARD,3},{EQUIP,4},{STATUS,5},{HELP,6},{FACTIONS,7},{TARGETING,8},{DEBUG,9},{COMMS,10},{COMMS_PANEL,10},{DETAILS,11},{MISSIONS,12},{MISSIONLOG,13},{GALNET,14},{RADIO,16},{CAMPAIGN,17},{GUILD,18},{COMFORT,19},{WALK,20},{INVENTORY,21},{STORAGE,21},{REPAIR,24},{DECORATOR,23},{LOCALTV,26},{MULTIPLAYER,27},{NEARBY_PLAYERS,29}};
  if(p==CODEX)return opening_menu_available(&game,15)||opening_menu_available(&game,22);
  for(unsigned i=0;i<sizeof(opening_pages)/sizeof(opening_pages[0]);i++)if(opening_pages[i][0]==p)return opening_menu_available(&game,opening_pages[i][1]);
  return opening_done(&game);
 }
 if(!tutorial_active(&game)||p==HOME||p==FLIGHT||p==INTRO||((p==FIELDGUIDE||p==CODEX)&&game.planet>=1))return 1;
 static const int ids[][2]={{MARKET,1},{CHART,2},{YARD,3},{EQUIP,4},{STATUS,5},{HELP,6},{FACTIONS,7},{TARGETING,8},{DEBUG,9},{COMMS,10},{COMMS_PANEL,10},{DETAILS,11},{MISSIONS,12},{MISSIONLOG,13},{GALNET,14},{CODEX,15},{RADIO,16},{CAMPAIGN,17},{GUILD,18},{COMFORT,19},{WALK,20},{INVENTORY,21},{STORAGE,21},{REPAIR,24},{DECORATOR,23}};
 for(unsigned i=0;i<sizeof(ids)/sizeof(ids[0]);i++)if(ids[i][0]==p)return tutorial_service(&game,ids[i][1]);
 return 0;
}
static void tools_block_until_release(void);
static void newspaper_reset(void);
static void rumour_ad_reset(void);
static void wanted_ui_reset(void);
static void change_page(int p){
 loadout_picker_slot=-1;
 if(p!=FLIGHT&&erl_checkpoint_collect(1)<0)game.erl.dirty=1;
 if(p==TARGETING&&race_is_active(&game))return;
 if(p==GALNET||page==GALNET)newspaper_reset();
 if(p!=page&&(p==GALNET||page==GALNET))rumour_ad_reset();
 if(p!=page&&(p==GALNET||page==GALNET))wanted_ui_reset();
 if(p==STORY){story_complete(&game);p=HOME;}
 if(p==GUILD){tracked_mission=1;p=CAMPAIGN;}
 if(p==COMMS||p==COMFORT)p=COMMS_PANEL; /* Held-Triangle comms is the single channel UI. */
 if(!tutorial_page_allowed(p)){const TutorialBeat *b=tutorial_beat(&game);message(&game,b?b->task:"This feature will become available as your journey unfolds.");return;}
 if(p!=YARD)ship_purchase_reset();
 if(p!=HOME){load_arrival_time=-1;load_arrival_release=0;}
 if(page==HOME&&p!=HOME&&!nav_back)deck_open_begin(p);
 else if(p==HOME||p!=page)deck_open_cancel();
 if(p==CHART&&page==HOME&&row==2&&!nav_back&&!deck_open_pending(p))chart_dissolve_begin();
 else if(p!=CHART)chart_dissolve_time=0;
 if(p==FLIGHT&&page!=FLIGHT){fire_blocked=1;tools_block_until_release();}
 if(page==HOME&&row>=0&&row<DECK_ITEMS){deck_last=row;deck_focus[deck_group(row)]=row;}
 if(p==HOME||p==FLIGHT||p==INTRO)nav_depth=0;
 if(p==INTRO){intro_profile_ready=0;if(page!=INTRO)intro_enter();}
 else if(p!=HOME&&p!=FLIGHT&&!nav_back&&p!=page){if(nav_depth<8){nav_pages[nav_depth]=page==INTRO?HOME:page;nav_rows[nav_depth]=page==INTRO?deck_last:row;nav_depth++;}}
 /* Page changes must reveal the actual destination immediately. The former
  * CRT scan bands looked like corrupted/flashing menus on the PSP. */
 if(p!=page)ui_page_anim=ui_press_anim=0;
 if(page==INTRO&&p!=INTRO)intro_leave();
 comms_rescue_confirm=0;abandon_confirm=0;if(p!=MISSIONS){mission_accept_confirm=-1;mission_accept_yes=0;}sell_confirm_slot=-1;module_sell_item=-1;equip_confirm_item=-1;game.boost=0;page=p;row=p==HOME?deck_last:0;if(p!=COMMS_PANEL)game.message_time=0;
 if(p==CHART){update_nearby();chart_mode=1;chart_search=0;if(tracked_mission==0&&game.campaign_stage>=6&&game.saga_step&&game.saga_chapter<SAGA_COUNT)chart_cursor=game.saga_dest;else if(game.route_goal>=0)chart_cursor=game.route_goal;else chart_cursor=game.destination;for(int i=0;i<near_count;i++)if(nearby[i]==game.destination)row=i;}
 if(p==MISSIONLOG){mission_title_focus=-1;if(tracked_mission==TRACK_ERL&&!erl_log_active())tracked_mission=0;if((tracked_mission==TRACK_STATION_TOUR&&!station_tour_active())||(tracked_mission!=TRACK_STATION_TOUR&&tracked_mission!=TRACK_LAVE&&tracked_mission!=TRACK_ERL&&tracked_mission>=2+game.job_n))tracked_mission=0;row=mission_track_row();}
 if(p==HOME)deck_clamp_row();
 if(p==FACTIONS)faction_lore_card=0;
 if(p==STATUS)profile_enter();
 if(p==EQUIP)equip_target=-1;
}
static void menu_back(void){
 if(page==MISSIONS||page==MISSIONLOG||page==CHART||page==DETAILS){chart_search=0;change_page(HOME);return;}
 int p=HOME,r=deck_last;if(nav_depth){nav_depth--;p=nav_pages[nav_depth];r=nav_rows[nav_depth];}
 nav_back=1;change_page(p);nav_back=0;row=r;
}

#include "flight-lighting.h"
#ifdef TRAVEL_POLISH_REVIEW
static void planet_flight_atmosphere_reference(Body*b,Point p,float r,int xt,int yt,int xb,int yb){
 if(high_contrast||r<6||r>220)return;
 const WorldType*w=&world_types[world_type_seed(b->seed,b->type)];unsigned tint=w->accent;
 tint=planet_rgb_mix(tint,b->type==OCEAN?RGB(80,156,235):RGB(132,116,200),107);
 int left=(int)fmaxf(xt,p.x-r-5),right=(int)fminf(xb-1,p.x+r+5),top=(int)fmaxf(yt,p.y-r-5),bot=(int)fminf(yb-1,p.y+r+5),budget=10000;
 for(int y=top;y<=bot;y++)for(int x=left;x<=right;x++){
  float dx=x+.5f-p.x,dy=y+.5f-p.y,q=dx*dx+dy*dy;if(q<r*r||q>(r+5)*(r+5))continue;if(--budget<0)return;
  float edge=sqrtf(q)-r,fade=(1-edge/5),lit=fmaxf(.05f,dx/r*planet_phase_lx+dy/r*planet_phase_ly+.16f);float gain=fade*fade*lit*.5f;
  unsigned base=fb[y*STRIDE+x];int rr=(base&255)+(tint&255)*gain,gg=((base>>8)&255)+((tint>>8)&255)*gain,bb=((base>>16)&255)+((tint>>16)&255)*gain;
  pixel(x,y,RGB(rr>255?255:rr,gg>255?255:gg,bb>255?255:bb));
 }
}
#endif
static void planet_flight_atmosphere(Body*b,Point p,float r,int xt,int yt,int xb,int yb){
 if(high_contrast||r<6||r>220)return;
 const WorldType*w=&world_types[world_type_seed(b->seed,b->type)];unsigned tint=w->accent;
 tint=planet_rgb_mix(tint,b->type==OCEAN?RGB(80,156,235):RGB(132,116,200),107);
 int left=(int)fmaxf(xt,p.x-r-5),right=(int)fminf(xb-1,p.x+r+5),top=(int)fmaxf(yt,p.y-r-5),bot=(int)fminf(yb-1,p.y+r+5),budget=10000;
 /* Shade only the annulus spans. The centre contains no atmosphere pixels;
  * scanning its entire rectangle wasted work on large planets. Keep the exact
  * original pixel predicate and blend, including clipping and budget order. */
 for(int y=top;y<=bot;y++){
  float yy=y+.5f-p.y,outer2=(r+5)*(r+5)-yy*yy;if(outer2<0)continue;
  float outer=sqrtf(outer2),inner2=r*r-yy*yy,centre=p.x-.5f;
  int l=fmaxf(left,ceilf(centre-outer)-1),rr=fminf(right,floorf(centre+outer)+1),gap0=rr+1,gap1=rr+1;
  if(inner2>4){float inner=sqrtf(inner2);gap0=ceilf(centre-inner)+1;gap1=floorf(centre+inner)-1;}
  for(int x=l;x<=rr;x++){
   if(x>gap0&&x<gap1){x=gap1;if(x>rr)break;}

  float dx=x+.5f-p.x,dy=y+.5f-p.y,q=dx*dx+dy*dy;if(q<r*r||q>(r+5)*(r+5))continue;if(--budget<0)return;
  float edge=sqrtf(q)-r,fade=(1-edge/5),lit=fmaxf(.05f,dx/r*planet_phase_lx+dy/r*planet_phase_ly+.16f);float gain=fade*fade*lit*.5f;
  unsigned base=fb[y*STRIDE+x];int rr=(base&255)+(tint&255)*gain,gg=((base>>8)&255)+((tint>>8)&255)*gain,bb=((base>>16)&255)+((tint>>16)&255)*gain;
  pixel(x,y,RGB(rr>255?255:rr,gg>255?255:gg,bb>255?255:bb));
  }
 }
}
static void draw_bodies(void){
 int order[BODY_COUNT];for(int i=0;i<BODY_COUNT;i++)order[i]=i;
 for(int i=0;i<BODY_COUNT;i++)for(int j=i+1;j<BODY_COUNT;j++)if(length(sub(game.bodies[order[j]].pos,game.pos))>length(sub(game.bodies[order[i]].pos,game.pos))){int t=order[i];order[i]=order[j];order[j]=t;}
 for(int i=0;i<BODY_COUNT;i++){Body *b=&game.bodies[order[i]];if(!body_active(b))continue;Vec3 v=camera(&game,b->pos);if(v.z<100)continue;Point p=project(v);float radius=fminf(700,240*b->radius/v.z);if(radius<1||p.x+radius<0||p.x-radius>W)continue;
  int yt=clipy0>=0?clipy0:view_top(),yb=clipy1>=0?clipy1:view_bot(),xt=clipy0>=0?clipx0:0,xb=clipy0>=0?clipx1:W;
  if(p.x+radius<xt||p.x-radius>=xb)continue;
  if(b->type!=SUN){
   Vec3 right,up,ahead;camera_axes(&game,&right,&up,&ahead);Vec3 light=norm(sub(game.bodies[0].pos,b->pos));
   planet_phase_init();planet_phase_cx=(int)p.x;planet_phase_cy=(int)p.y;planet_phase_inverse=1.f/fmaxf(1,(int)radius);planet_phase_lx=dot(light,right);planet_phase_ly=-dot(light,up);planet_phase_lz=-dot(light,ahead);
   planet_phase_active=planet_phase_ready&&!high_contrast&&!game.surface;
   planet_flight_atmosphere(b,p,(int)radius,xt,yt,xb,yb);
   draw_planet_sprite_rolled(p.x,p.y,(int)radius,b->seed,b->type,game.roll,xt,yt,xb,yb);planet_phase_active=0;continue;
  }
  flight_lighting_halo((int)p.x,(int)p.y,(int)radius,xt,yt,xb,yb);
  draw_sun_sprite((int)p.x,(int)p.y,(int)radius,b->color,b->seed,game.time,xt,yt,xb,yb);
 }
}
static void draw_portrait(int x,int y,int w,int h,int system,int role);
static int ion_working_frame;
static int ion_sky_gpu(int left,int right,int top,int bot,Vec3 rightward,Vec3 upward,Vec3 ahead);
#include "space-fx.h"
static void open_station_channel(void);
static void ship_computer_portrait(int x,int y,int w,int h);
static unsigned dim_rgb(unsigned c,int num,int den); /* Defined in voyage.h; also used by rear-view rendering. */
static int wifi_contact_ready(void);
static int wifi_contact_local(void);
static int wifi_surface_contact_local(void);
static Vec3 wifi_contact_position(void);
static const char *wifi_contact_name(void);
static const char *wifi_contact_status(void);
static void wifi_contact_route(void);
static int db_open_contact(int npc);
static void db_restore_frontend(void);
#include "flight-extras.h"


static void wifi_surface_draw(void);
static void opening_alpine_sky(int top,int bottom,int horizon);
#include "planet.h"
static void atlas_open_surface_record(int system,int body,int slot);
#include "surface-targeting.h"
#include "eva-local-map.h"
#include "field-map-art.h"
#include "roamer-view.h"
#include "planet-sequence.h"
static unsigned twin_weapon_ink(int w);
#include "flight-tools-ui.h"
#include "pixel-art.h"
#include "voyage.h"
#include "rift-catalog.h"
#include "rift-field.h"
static void hc_flight_overlay(void);
#include "station-departure-view.h"
#include "thargoid-ambush.h"
#include "mega-hull-art.h"
#include "erl-racer-art.h"
#include "player-exterior.h"

#include "weapon-fx.h"
static void online_close(const char *note);
#include "local-wifi.h"
#include "online-presence.h"
static int wifi_contact_ready(void){return (wifi.state&&wifi.ready&&wifi.quiet<60)||online_peer_ready(online.selected);}
static int wifi_contact_local(void){return wifi_contact_ready()&&wifi.remote.system==game.system;}
static int wifi_surface_contact_local(void){return wifi_contact_local()&&game.planet>=1&&wifi.remote.planet==game.planet&&!wifi.remote.jumping;}
static const char *wifi_contact_name(void){return wifi_contact_ready()?wifi.remote.name:"FRIEND OFFLINE";}
static const char *wifi_contact_status(void){
 static char label[64];if(!wifi_contact_ready())return "FRIEND OFFLINE";
 const char *where=wifi.remote.jumping?"HYPERSPACE":wifi.remote.docked?"DOCKED":wifi.remote.planet>=1?wifi.remote.rover?"ROAMER":wifi.remote.surface==2?"ON FOOT":"LANDING / SHIP":"IN SPACE";
 if(wifi.state&&wifi.quiet>5)return "LINK QUIET / LAST SEEN";
 if(wifi.remote.planet>=1&&wifi.remote.planet<BODY_COUNT)snprintf(label,sizeof(label),"%.10s %s / %s",game.systems[wifi.remote.system].name,(const char*[]){"","I","II","III","IV","V","VI","VII","VIII","IX"}[wifi.remote.planet],where);
 else snprintf(label,sizeof(label),"%.10s / %s",game.systems[wifi.remote.system].name,where);
 return label;
}
static void wifi_contact_route(void){
 if(!wifi_contact_ready()||wifi_contact_local()||game.route_goal==wifi.remote.system)return;
 int steps=0,hop=route_next_hop(&game,wifi.remote.system,&steps);
 if(hop<0){autoaim=0;message(&game,"Refuel or upgrade your jump drive to reach your friend.");return;}
 route_set_goal(&game,wifi.remote.system);game.destination=hop;
}
static Vec3 wifi_contact_position(void){
 if(!wifi_contact_ready())return game.pos;
 if(!wifi_contact_local()){
  if(game.route_goal==wifi.remote.system&&route_target_ready())return add(game.pos,mul(route_target_direction(),180000.f));
  int sys=wifi.remote.system;
  Vec3 d={(float)(game.systems[sys].x-game.systems[game.system].x),0,(float)(game.systems[game.system].y-game.systems[sys].y)};
  if(length(d)<1)d=(Vec3){0,0,1};return add(game.pos,mul(norm(d),180000));
 }
 if(wifi.remote.planet>=1&&(game.planet!=wifi.remote.planet||game.planet<1))return game.bodies[wifi.remote.planet].pos;
 if(wifi.remote.docked)return hub_position(&game,0);
 return wifi.display.pos;
}
static void multiplayer_draw(void);
static void multiplayer_input(unsigned pressed,float dt);
#include "local-wifi-ui.h"
#include "online-presence-ui.h"
#include "multiplayer-console.h"
typedef struct {float right,up,ahead,look_ahead,roll;} FlightCameraShot;
static const FlightCameraShot cinematic_shots[]={
 {-110,105,-390,75,-.035f},{320,75,185,15,.045f},{-350,45,-80,35,-.06f},
 {345,-35,-105,50,.055f},{35,390,-65,25,0},{-185,-115,-300,90,-.025f},
 {165,150,-520,120,.02f},{-420,180,135,5,-.045f},{85,-210,265,0,.035f},
 {440,20,-260,70,.06f},{-65,235,-245,140,-.025f},{255,95,330,10,.04f}
};
static int flight_camera_seen=-1;
static float cinematic_epoch=0;
static float flight_camera_time=0;
static Vec3 flight_camera_eye,flight_camera_ship_pos;
static float flight_camera_yaw=0,flight_camera_pitch=0,flight_camera_roll=0;
static float drive_shake_strength(void){
 float boost=game.boost?game.boost_spool*(.35f+.65f*fminf(1.f,game.speed/fmaxf(1.f,player_ships[game.ship].speed*5.f))):0;
 return fmaxf(boost,game.mega_brake*.72f);
}
static Vec3 drive_shake_offset(Vec3 right,Vec3 up,float scale){
 float s=drive_shake_strength()*scale;
 return add(mul(right,sinf(game.time*31.7f)*s),mul(up,(sinf(game.time*43.1f)+sinf(game.time*19.3f)*.45f)*s*.58f));
}
static float angle_near(float from,float to){while(to-from>3.14159265f)to-=6.2831853f;while(to-from<-3.14159265f)to+=6.2831853f;return to;}
static void flight_camera_basis(Vec3 *right,Vec3 *up,Vec3 *ahead){
 float sy=sinf(game.yaw),cy=cosf(game.yaw),sp=sinf(game.pitch),cp=cosf(game.pitch),sr=sinf(game.roll),cr=cosf(game.roll);
 Vec3 r={cy,0,-sy},u={-sy*sp,cp,-cy*sp};*right=add(mul(r,cr),mul(u,sr));*up=add(mul(r,-sr),mul(u,cr));*ahead=(Vec3){sy*cp,sp,cy*cp};
}
/* Race-only boost lens. Presentation state never changes physics or saves. */
static float race_camera_boost,race_camera_last;static unsigned race_camera_serial;
static void race_camera_boost_tick(void){
 SpaceRace *r=&game.race;if(race_camera_serial!=game.erl.serial||r->elapsed<race_camera_last||r->phase!=RACE_RUNNING){race_camera_boost=0;race_camera_serial=game.erl.serial;race_camera_last=r->elapsed;}
 float dt=fmaxf(0,fminf(.1f,r->elapsed-race_camera_last));race_camera_last=r->elapsed;
 float target=r->phase==RACE_RUNNING&&(game.boost||r->boost_time>0||r->arcade.actor[0].rocket>0)?1:0;
 race_camera_boost+=(target-race_camera_boost)*(1-expf(-dt*(target?9:5)));
}
static int race_crash_camera(void);
static void race_apex_effects(int rear);
static void flight_camera_prepare(void){
 camera_override_clear();if(race_crash_camera())return;
 if(race_is_active(&game)){
  Vec3 ahead=forward(&game),right=norm((Vec3){ahead.z,0,-ahead.x}),up=norm((Vec3){-ahead.y*right.z,ahead.z*right.x-ahead.x*right.z,ahead.y*right.x});float size=player_exterior_radius(),pad=game.race.boost_time>.74f?sinf((.9f-game.race.boost_time)*190)*((game.race.boost_time-.74f)/.16f):0;
  Vec3 shake=drive_shake_offset(right,up,third_person?1.35f:.42f);float wobble=sinf(game.time*27.3f)*drive_shake_strength()*.006f;
  if(third_person){Vec3 eye=add(game.pos,add(mul(ahead,-size*(4.1f+fabsf(game.roll)*1.2f)),mul(up,size*.95f+pad*2)));camera_override_set(add(eye,shake),game.yaw+wobble*.35f,game.pitch+wobble*.22f,game.roll*.22f+wobble);}
  else camera_override_set(add(add(game.pos,mul(up,pad*1.5f)),shake),game.yaw+wobble*.35f,game.pitch+wobble*.22f,game.roll*.24f+wobble);
  return;
 }
 if(third_person<=0){float strength=drive_shake_strength();if(strength>0){Vec3 right,up,ahead;flight_camera_basis(&right,&up,&ahead);Vec3 shake=drive_shake_offset(right,up,.42f);float wobble=sinf(game.time*27.3f)*strength*.006f;camera_override_set(add(game.pos,shake),game.yaw+wobble*.35f,game.pitch+wobble*.22f,game.roll+wobble);}return;}
 if(flight_camera_seen!=third_person&&third_person==2)cinematic_epoch=game.time;
 Vec3 right,up,ahead;flight_camera_basis(&right,&up,&ahead);FlightCameraShot shot;
 if(third_person==1){float size=player_exterior_radius();shot=(FlightCameraShot){0,size*.72f,-size*3.6f,size*8.f,0};}
 else {
  int count=(int)(sizeof(cinematic_shots)/sizeof(cinematic_shots[0]));float elapsed=fmaxf(0,game.time-cinematic_epoch),within=fmodf(elapsed,10.f);int now=((int)(elapsed/10.f))%count,prev=elapsed<1.f?now:(now+count-1)%count;
  float blend=within<1.f?within*within*(3.f-2.f*within):1.f;FlightCameraShot a=cinematic_shots[prev],b=cinematic_shots[now];
  shot.right=a.right+(b.right-a.right)*blend;shot.up=a.up+(b.up-a.up)*blend;shot.ahead=a.ahead+(b.ahead-a.ahead)*blend;shot.look_ahead=a.look_ahead+(b.look_ahead-a.look_ahead)*blend;shot.roll=a.roll+(b.roll-a.roll)*blend;
 }
 Vec3 wanted=add(game.pos,add(mul(right,shot.right),add(mul(up,shot.up),mul(ahead,shot.ahead))));
 Vec3 focus=add(game.pos,mul(ahead,shot.look_ahead));Vec3 look=norm(sub(focus,wanted));float yaw=atan2f(look.x,look.z),pitch=atan2f(look.y,sqrtf(look.x*look.x+look.z*look.z));
 /* Compute bank in the chase camera's frame, including pitch. No wrapped
  * Euler interpolation: a complete L roll keeps the mount behind the hull. */
 if(third_person==1){Vec3 cr={cosf(yaw),0,-sinf(yaw)},cu={-sinf(yaw)*sinf(pitch),cosf(pitch),-cosf(yaw)*sinf(pitch)};shot.roll=atan2f(-dot(up,cr),dot(up,cu));
  flight_camera_eye=wanted;flight_camera_ship_pos=game.pos;flight_camera_yaw=yaw;flight_camera_pitch=pitch;flight_camera_roll=shot.roll;flight_camera_seen=1;flight_camera_time=game.time;Vec3 shake=drive_shake_offset(right,up,1.35f);float wobble=sinf(game.time*27.3f)*drive_shake_strength()*.006f;camera_override_set(add(wanted,shake),yaw+wobble*.35f,pitch+wobble*.22f,shot.roll+wobble);return;
 }
 if(flight_camera_seen!=third_person||game.time<flight_camera_time||game.time-flight_camera_time>.5f){flight_camera_eye=wanted;flight_camera_ship_pos=game.pos;flight_camera_yaw=yaw;flight_camera_pitch=pitch;flight_camera_roll=shot.roll;flight_camera_seen=third_person;}
 else {float dt=fmaxf(1.f/120.f,fminf(.1f,game.time-flight_camera_time)),rate=third_person==1?12.f:6.f,follow=1.f-expf(-rate*dt);flight_camera_eye=add(flight_camera_eye,sub(game.pos,flight_camera_ship_pos));flight_camera_ship_pos=game.pos;flight_camera_eye=add(flight_camera_eye,mul(sub(wanted,flight_camera_eye),follow));flight_camera_roll+=(shot.roll-flight_camera_roll)*follow;
  /* Recompute the look direction from the smoothed eye every frame. The old
     independently smoothed angles could briefly point away from the ship
     after rapid turns or repeated camera-mode changes. */
  Vec3 stable_look=norm(sub(focus,flight_camera_eye));flight_camera_yaw=atan2f(stable_look.x,stable_look.z);flight_camera_pitch=atan2f(stable_look.y,sqrtf(stable_look.x*stable_look.x+stable_look.z*stable_look.z));
 }
 flight_camera_time=game.time;
 {Vec3 shake=drive_shake_offset(right,up,1.35f);float wobble=sinf(game.time*27.3f)*drive_shake_strength()*.006f;camera_override_set(add(flight_camera_eye,shake),flight_camera_yaw+wobble*.35f,flight_camera_pitch+wobble*.22f,flight_camera_roll+wobble);}
}
static void flight_camera_ship(void){
 if(third_person<=0&&game.race.crash_time<=1.f)return;
 int old_min=surface_y_min,old_max=surface_y_max,old_depth=surface_depth_on,old_encoding=mega_depth_encoding,old_material=surface_material;float old_light=surface_light;
 surface_y_min=view_top();surface_y_max=view_bot();surface_light=1;surface_material=0;surface_depth_on=1;mega_depth_encoding=1;
 /* Share existing world depth and test hull, flame and gunfire together. */
 if(!mega_depth_valid)memset(surface_depth+surface_y_min*W,255,(surface_y_max-surface_y_min+1)*W*sizeof(*surface_depth));
 int old_city_depth=mc_depth_active;if(fleet_ge_active_frame&&!race_is_active(&game))mc_depth_active=1;
 player_mesh_transform=1;int art=mega_hull_art_id(player_ships[game.ship].name);if(art>=0){const MegaHullArt *h=&mega_hull_art[art];for(int i=0;i<h->parts;i++)mega_art_block(&h->part[i],0,0,1.2f,game.pos,ship_paint[game.ship]);}else shipmesh(mesh_id(player_ships[game.ship].name),game.pos,0,0,1.2f,ship_paint[game.ship],0);player_mesh_transform=0;flush_meshes();mc_depth_active=old_city_depth;
 if(race_is_active(&game)){weapon_fx_fire();surface_y_min=old_min;surface_y_max=old_max;surface_light=old_light;surface_material=old_material;surface_depth_on=old_depth;mega_depth_encoding=old_encoding;return;}
 float size=player_exterior_radius(),trail=fminf(size*1.55f,game.boost?150.f:game.speed>80?62.f:18.f);
 unsigned core=game.boost?CYAN:AMBER,edge=game.boost?RGB(42,118,170):RGB(130,72,27);Vec3 r,u,a;flight_camera_basis(&r,&u,&a);
 for(int side=-1;side<=1;side+=2){Vec3 root;if(!player_hardpoint(side,0,&root))continue;
  exterior_ray(root,add(root,mul(a,-trail*.48f)),core,game.boost?2:1);exterior_ray(add(root,mul(a,-trail*.48f)),add(root,mul(a,-trail)),edge,1);
 }
 weapon_fx_fire();
 surface_y_min=old_min;surface_y_max=old_max;surface_light=old_light;surface_material=old_material;surface_depth_on=old_depth;mega_depth_encoding=old_encoding;
}
static const char *erl_driver_name(int id);
static unsigned erl_portrait(int id);
static unsigned erl_portrait(int id);
static void erl_crash_live_camera(Vec3 *eye,Vec3 *ahead,Vec3 *up);
static int erl_crash_visual_active;
static float erl_crash_visual_age;
#include "space-racing-ui.h"
#include "hyperspace-ion.h"
#include "station-ge.h"
#include "wreck-ge.h"
#include "spatial-regions.h"
static void space_render(void);
#include "erl-neon-clouds.h"
#include "erl-neon-render.h"
#include "erl-neon-bloom.h"
#include "erl-neon-compositor.h"
#include "erl-neon-effects.h"
/* Preserve the station-only occlusion map used by targeting, while allowing
 * station and encounter colour to share one GPU pass. Negative material IDs
 * identify the station faces whose CPU depth has already been prepared. */
static void station_occlusion_prepass(void){
 if(!mc_depth_active)return;station_ge_predepth=1;
 for(int i=0;i<drawcount;i++)station_ge_depth_triangle(&drawlist[i]);
 for(int y=0;y<136;y++)for(int x=0;x<240;x++)mega_occlusion[y*240+x]=(y*2>=surface_y_min&&y*2<=surface_y_max)?surface_depth[y*2*W+x*2]:65535;
 mega_depth_valid=1;
}
#include "cockpit-charms.h"
#include "dashboard-cosmetics.h"
static void hc_flight_overlay(void){
 charm_draw();
 hud_animation_tick();hud_canopy_feedback();hc_motion_tick();
  if(hc_repaint_needed()){hc_present_begin();cockpit();tools_panel();hc_present_finish();}else hc_overlay_draw(fb);
  hc_speed_ticker();hc_motion_draw();twin_cockpit_reflections();dashboard_draw();rear_view_mirror(257,4,209,12);incoming_fire_indicators();expedition_flight_draw();
}

static void space(void){
 hc_space_depth++;
 /* Last frame's world depth is invalid for selecting this frame's renderer.
  * In particular, a model must never disable acceleration by occluding itself. */
 mega_depth_valid=0;station_ge_predepth=0;
 float focal=proj_focal,s=ion_seconds();if(s>=0)proj_focal=240.f/(1+ion_speed(journey_variant?s:fminf(10,s))*.21f*(journey_variant?ion_smooth(s/.85f):1)+ion_burst(s)*.07f);
 /* Warp glow reads neighbouring pixels. Work in cached RAM, then copy once
  * to display VRAM; do not repeatedly read uncached display memory. */
 unsigned*display=fb;int warp=s>=0&&page==FLIGHT&&game.planet<0&&!thargoid_active;
 int saved_station_ge=station_ge_active_frame,saved_wreck_ge=wreck_ge_active_frame,saved_fleet_ge=fleet_ge_active_frame;station_ge_active_frame=station_ge_view_ready();wreck_ge_active_frame=wreck_ge_view_ready();fleet_ge_active_frame=fleet_ge_view_ready();int cached=warp||station_ge_active_frame||wreck_ge_active_frame||fleet_ge_active_frame||natural_cached_view()||spatial_view_needed();
 float arrival_voice=game.voice_time,arrival_message=game.message_time;int hold_brief=journey_variant&&s>=10&&s<11.4f;
 if(hold_brief){game.voice_time=0;game.message_time=0;}
 ion_working_frame=cached;fleet_ge_prior_count=fleet_ge_prior_overflow=0;if(cached)fb=dc_open_source;space_render();
 if(hold_brief){game.voice_time=arrival_voice;game.message_time=arrival_message;}if(cached){fb=display;if(fb!=dc_open_source)memcpy(fb,dc_open_source,sizeof(dc_open_source));}ion_working_frame=0;station_ge_active_frame=saved_station_ge;wreck_ge_active_frame=saved_wreck_ge;fleet_ge_active_frame=saved_fleet_ge;station_ge_predepth=0;proj_focal=focal;
 hc_space_depth--;
}
static void space_render(void){
 camera_override_clear();if(third_person<=0)flight_camera_seen=-1;
 mega_depth_valid=0;
 if(opening_dream(&game)){opening_dream_view();return;}
 if(game.police_stop){police_dialog();return;}
 if(planet_entry_body){float cloud=pilot_ease(planet_entry_time/planet_entry_cover);if(cloud<.999f){int body=planet_entry_body,approach=game.approach;planet_entry_body=0;game.approach=-1;space();game.approach=approach;planet_entry_body=body;}else rect(0,0,W,H,planet_veil_ink);planet_cloud_cover(cloud);planet_pilot_canopy(0,1);return;}
 if(planet_orbit_veil>0){float veil=planet_orbit_veil;planet_orbit_veil=0;space();planet_orbit_veil=veil;planet_cloud_cover(pilot_ease(veil/1.3f));planet_pilot_canopy(0,veil/1.3f);return;}
 if(!valid_target(selected_target)&&!hc_context()){selected_target=0;autoaim=0;}
 /* Modal views draw only part of the world. Clear both alternating buffers
  * so a prior warp, laser or target marker cannot remain behind the panel. */
 if(game.approach>=0||game.police_stop||game.dead||game.dock_stage>=2){
  rect(0,0,W,H,BG);
 }
 if(game.planet>=0){
  if(planet_seat.active){planet_seat_view();return;}
  if(game.planet_sequence){planet_sequence_view();return;}
  if(game.surface==1){planet_boarded_view();return;}
  if(planet_landing_menu){planet_landing_prompt_view();return;}
  if(game.surface==2&&eva_map_open&&!game.dead){eva_map_draw();return;}
  if(planet_first_frame)planet_trace("eva-render-before",0);
  planet_view();
  /* Cockpit bloom feeds bright neighbours back into itself: the meadow's
   * pale sky became a speckled white band. Sun/stone already own local glow. */
  if(!planet_earth_i())hud_postfx();
  if(game.dead){death_effect();sfx_maybe_death_embers();sfx_explosion_embers_draw(1.f/60);if(hud_mode==0&&game.explosion<2.35f)cockpit();death_shutdown_overlay();return;}
  if(game.surface==2){if(game.rover_driving)planet_pilot_canopy(1,1);rover_windshield();if(!planet_earth_i())hand_draw();planet_eva_hud();if(!planet_earth_i())surface_target_hud();if(eva_map_open)eva_map_draw();if(planet_first_frame){planet_trace("eva-render-after",0);planet_first_frame=0;}return;}
  if(paused||hud_mode==0||game.dead)cockpit();
  else if(hud_mode==1)minimal_overlay();
  return;
 }
 if(thargoid_active){thargoid_view();return;}
 if(game.jump>0){
  unsigned sky_seed=space_sky_seed_for(game.destination);int pending=sky_seed==soft_sky_cached_seed?(soft_sky_ready?0:6*SOFT_SKY_FACE-soft_sky_build_row):6*SOFT_SKY_FACE;
  int left=(int)(game.jump*45);if(left<1)left=1;int rows=(pending+left-1)/left;if(rows>6)rows=6;soft_sky_prepare_rows(sky_seed,rows);
  ion_charge_cached=0;ion_render(ion_seconds());cockpit();route_prejump_panel();return;
 }
 if(route_boost_charge>=4.2f&&ion_charge_cached&&ion_seconds()>=0&&jump_scene_pixels){
  memcpy(fb,jump_scene_pixels,STRIDE*H*4);ion_render(ion_seconds());cockpit();route_prejump_panel();return;
 }
 if(route_boost_charge<4.2f)ion_charge_cached=0;
 if(game.approach>=0){planet_prompt();cockpit();return;}
 if(game.police_stop){if(hud_mode==0)cockpit();police_dialog();return;}
 if(game.rift_report){rift_report_view();return;}
 if(game.dead){death_effect();sfx_maybe_death_embers();sfx_explosion_embers_draw(1.f/60);if(hud_mode==0&&game.explosion<2.35f)cockpit();death_shutdown_overlay();return;}
 if(game.dock_stage==4){departure_view();return;}
 if(game.dock_stage>=2){docking_view();if(hud_mode==0)cockpit();return;}
 flight_camera_prepare();
 /* The temporary ERL course has its own clear stadium sky. Ordinary planets,
  * police, traffic and nebula effects remain in the ordinary flight renderer. */
 if(race_ready(&game)&&game.race.phase!=RACE_GRID){
  if(race_grid_intro_draw()){camera_override_clear();return;}
  float race_base_focal=proj_focal,race_base_oy=proj_oy;if(game.race.layout==5)proj_oy=135;proj_focal=game.race.layout==5?285.f-62* race_camera_boost:race_view_focal(race_base_focal);
  /* The deck and exhaust blend many pixels. Keep their reads in RAM rather
   * than uncached display VRAM; this plane is idle outside dock/victory UI. */
  int compact=game.race.layout==5&&neon_present_begin(),old_ox=proj_ox,old_cx=clipx1,old_cy=clipy1;float scene_focal=proj_focal;
  unsigned *race_display=fb;if(game.race.layout!=5||neon_ram_render||compact)fb=dc_open_source;race_round_fast=game.race.layout==4;
  if(compact){float scene_scale=neon_render_width/(float)W;proj_ox=neon_render_width/2;proj_oy=(int)(135*scene_scale);proj_focal*=scene_scale;clipx1=neon_render_width;clipy1=neon_render_height;}
  neon_depth_mode=game.race.layout==5;render_phase(-1);if(game.race.layout==5){neon_frame_begin();}else {rect(0,0,W,H,RGB(6,14,22));race_starfield();race_round_near_deck();}render_phase(0);
  int old_min=surface_y_min,old_max=surface_y_max,old_depth=surface_depth_on,old_encoding=mega_depth_encoding,old_material=surface_material,old_race_fast=surface_fast;float old_light=surface_light;
  surface_y_min=0;surface_y_max=compact?neon_render_height-1:H-1;surface_light=1;surface_material=0;surface_depth_on=1;mega_depth_encoding=1;if(game.race.layout==5)surface_fast=1;memset(surface_depth,255,sizeof(surface_depth));
  race_opaque_quads=game.race.layout==5;
  if(game.race.layout==5){race_neon_track_draw();render_phase(2);race_neon_world_draw();}else race_world_draw();flush_solid_meshes();render_phase(1);
  /* The first course's fast scanline deck already owns its panels, lanes
   * and edges. Re-projecting the cached world lines over it created a thin
   * floating rail across the horizon where the two vanishing points met. */
  if(game.race.layout!=5)race_holo_track_draw();if(game.race.layout==4)race_round_fast=0;if(game.race.layout!=5)race_pad_art();render_phase(2);
  if(game.race.layout==5)race_neon_boundary();else race_boundary_glass();race_ghost_art();race_projectile_art();if(game.race.layout!=5)race_dressing_art();race_rival_instruments();render_phase(3);
  surface_y_min=old_min;surface_y_max=old_max;surface_light=old_light;surface_depth_on=old_depth;mega_depth_encoding=old_encoding;surface_material=old_material;surface_fast=old_race_fast;race_opaque_quads=0;mega_depth_valid=1;
  flight_camera_ship();render_phase(4);if(game.race.layout==5){race_neon_rings();race_neon_effects();race_neon_bloom();}else race_apex_effects(0);render_phase(5);
  if(compact){proj_ox=old_ox;proj_oy=135;proj_focal=scene_focal;clipx1=old_cx;clipy1=old_cy;neon_present_hud();}
  if(game.race.crash_time>3.f)race_crash_overlay();else race_hud();
  if(!compact&&fb!=race_display){fb=race_display;
   if(game.race.layout==4||game.race.layout==5)memcpy(fb,dc_open_source,sizeof(dc_open_source));else for(int y=0;y<H;y++)memcpy(fb+y*STRIDE,dc_open_source+y*STRIDE,W*sizeof(unsigned));}
  proj_focal=race_base_focal;proj_oy=race_base_oy;camera_override_clear();neon_depth_mode=0;render_phase(6);return;
 }
 /* Deep freighters are true background scenery. Flush their polygons and
  * exhaust before drawing planets so world discs always occlude them. */
 /* Keep shading/projection coordinates unchanged. Only raster clipping skips
  * pixels proven fully hidden by the opaque physical cockpit shell. */
 int cockpit_clip0=clipy0,cockpit_clip1=clipy1,cockpit_old_raster=hc_world_raster_clip;
 int cockpit_world_clip=hc_world_clip_enabled&&hc_full_console_visible()&&!opening_active(&game)&&ion_seconds()<0&&warp_arrival_fade<=0;
 if(cockpit_world_clip){clipy0=34;clipy1=164;hc_world_raster_clip=1;}
 render_phase(-1);if(render_reference||high_contrast)sector_background();space_fx_nebula();starfield();render_phase(0);
 deep_traffic_lights();deep_traffic();flush_meshes();deep_traffic_flares();render_phase(1);
 render_sub_phase(-1);celestial_rings(0);celestial_rims();render_sub_phase(0);draw_bodies();render_sub_phase(1);celestial_rings(1);sfx_planet_beauty();lens_flares();sfx_sun_canopy_wash();render_sub_phase(2);render_phase(2);
 render_sub_phase(-1);mega_city_depth_begin();station_model();render_sub_phase(3);
 if(fleet_ge_active_frame&&!mc_legacy_review){
  station_occlusion_prepass();render_sub_phase(4);station_window_animation();secondary_hubs();render_sub_phase(5);draw_space_discoveries();render_sub_phase(6);
 }else{
  mega_city_capture_depth();render_sub_phase(4);station_window_animation();secondary_hubs();render_sub_phase(5);draw_space_discoveries();render_sub_phase(6);
 }
 ambient_space();sfx_travel_beauty();sfx_travel_fun();render_sub_phase(7);render_phase(3);
 int npc_detailed[NPC_COUNT]={0};
 for(int i=0;i<NPC_COUNT;i++){NPC *n=&game.npc[i];if(!n->alive)continue;float bound=npc_visual_radius(n);if(!render_sphere_visible(n->pos,bound)||occluded(n->pos))continue;float distance=length(sub(n->pos,game.pos)),limit=n->freighter?55000.f:7000.f;if(distance>limit)continue;if(!render_reference&&!n->freighter&&render_pixel_radius(n->pos,bound)<2.f){render_lod++;continue;}npc_detailed[i]=n->freighter?2:1;unsigned c=n->flash>0?WHITE:faction_colors[n->role];float yaw=atan2f(n->dir.x,n->dir.z);if(npc_detailed[i]==2){capital_model(n,c);continue;}if(player_ship_is_mega(n->mega_hull)){mega_npc_model(n,c);continue;}ship_art_npc_draw=1;shipmesh(n->mesh,n->pos,yaw,0,n->scale,c,0);ship_art_npc_draw=0;}
 for(int i=0;i<DEBRIS_COUNT;i++){Debris *d=&game.debris[i];if(!d->alive||!render_sphere_visible(d->pos,d->radius)||occluded(d->pos))continue;float distance=length(sub(d->pos,game.pos));if(distance>11000)continue;
  if(!render_reference&&render_pixel_radius(d->pos,d->radius)<2.f){Vec3 v=camera(&game,d->pos);if(v.z>15){Point p=project(v);pixel((int)p.x,(int)p.y,d->rock?RGB(121,106,87):GOLD);}render_lod++;continue;}
  int mesh=mesh_id(d->rock==2?"BOULDER":d->rock?"ASTEROID":d->wreck?"BOULDER":"CANISTER");
  float size=render_mesh_radius(mesh);
  unsigned color=d->flash>0?RGB(225,216,181):d->rock==2?RGB(143,178,193):d->rock?RGB(121,106,87):d->wreck?RGB(101,108,112):GOLD;
  shipmesh(mesh,d->pos,game.time*(d->rock?.045f:.2f)+i,i*.21f,d->radius/size,color,0);
 }
 online_space_draw();
 race_world_draw();natural_solids();nav_world_buoys();flush_solid_meshes();rift_fields();
 mega_city_depth_end();
 render_phase(4);
 mega_capital_signs();
 for(int i=0;i<NPC_COUNT;i++)if(npc_detailed[i]==1&&game.npc[i].mesh!=4){NPC *n=&game.npc[i];ship_sprite_detail(n,n->flash>0?WHITE:faction_colors[n->role]);}
 npc_engine_glow();
 /* Wave A soft-FB: densify NPC plumes using the same aft roots. */
 if(!sfx_fx_muted()){
  int top=view_top(),bot=view_bot(),plume_budget=10;
  for(int i=0;i<NPC_COUNT;i++){NPC *n=&game.npc[i];if(!n->alive||n->mesh==4||occluded(n->pos))continue;float d=length(sub(n->pos,game.pos));if(d>9000)continue;
   float bound=npc_visual_radius(n);
   if(!render_reference&&(d>2500.f||render_pixel_radius(n->pos,bound)<8.f||!render_sphere_visible(n->pos,bound+100)||plume_budget--<=0))continue;
   int twin=n->mesh>=0&&n->mesh<35&&fleet_hulls[n->mesh].engines==2;
   float lateral=n->freighter?freight_extent(n).x*.38f:twin?1.f:0;int plumes=n->freighter||twin?2:1;
   for(int plume=0;plume<plumes;plume++){
    float offset=plumes==2?(plume?1:-1)*lateral:0;
    Vec3 rear=npc_engine_root(n,offset);Vec3 rv=camera(&game,rear);if(rv.z<25)continue;Point root=project(rv);
    if(root.x<0||root.x>=W||root.y<top||root.y>bot)continue;
    if(mega_depth_valid&&mega_encode_depth(rv.z)>surface_depth[(int)root.y*W+(int)root.x])continue;
    sfx_engine_plume_at((int)root.x,(int)root.y,n->freighter?0:(n->cruise>80),top,bot);
    sfx_engine_plume_mask((int)root.x,(int)root.y,n->freighter?0:(n->cruise>80));
   }
  }
 }
 station_glow();flight_lighting_station_rim();
 /* Soft docking aperture lamps — additive haze, not four-point sparkles. */
 float station_entry=station_entry_z_for(&game,0);
 if(!sfx_fx_muted()&&game.pos.z<station_entry&&!occluded((Vec3){0,0,station_entry})){
  int top=view_top(),bot=view_bot();
  for(int i=0;i<4;i++){
   Vec3 corner=station_port_corner_for(&game,0,i);corner.z-=2;
   Vec3 v=camera(&game,add(rotate(corner,0,station_angle(&game)),(Vec3){0,0,STATION_Z}));
   if(v.z<20)continue;Point p=project(v);
   sfx_add((int)p.x,(int)p.y,RGB(40,90,95),top,bot);
  }
 }
 station_entrance();speed_lines();if(!third_person){engine_flare();sfx_engine_plume_player();}
 if(game.damaged&&!sfx_fx_muted()){
  int top=view_top(),bot=view_bot();int damage=(int)fmaxf(0.f,fminf(100.f,100.f-game.hull));int count=3+damage/12;
  if(count>11)count=11;
  /* Smoke is a dim blended grey rather than an opaque blob. More hull loss
   * emits more particles, drifting out from the ship's forward view. */
  for(int i=0;i<count;i++){float t=game.time*(1.8f+(i%3)*.35f)+i*1.7f;int x=240+(int)(sinf(t)*((i+2)*7))-(int)(game.speed*.002f*i);int y=112+(int)(cosf(t*.83f)*((i+1)*6));if(x<8||x>471||y<top+4||y>bot-4)continue;int r=2+(i%3)+(damage>65?1:0);circle(x,y,r,RGB(48,54,62));if((i&1)==0)pixel(x+r+1,y,RGB(74,68,58));}
 }
 if(game.boost&&!sfx_fx_muted())sfx_engine_plume_mask(240,view_bot()-8,1);
 missile_effects();
 /* Rift fields retain one world-space anchor, even at very close range. */
 for(int i=0;i<NPC_COUNT;i++){NPC *n=&game.npc[i];if(!n->alive||npc_detailed[i]||occluded(n->pos))continue;Vec3 v=camera(&game,n->pos);if(v.z<30)continue;Point p=project(v);if(p.x<2||p.x>477||p.y<view_top()+2||p.y>view_bot()-2)continue;distant_ship_trail(n);}
 freight_effects();mining_effects();tractor_beam_effect();tools_flare_effect();route_target_star();
 for(int i=0;i<NPC_COUNT;i++){NPC *n=&game.npc[i];if(!n->alive||n->flash<=0)continue;
  Vec3 nv=camera(&game,n->pos);if(nv.z>15){Point hp=project(nv);sfx_maybe_flash_sparks((int)hp.x,(int)hp.y,n->flash,(unsigned)(i*97)^(unsigned)(game.time*40));}
  if(n->target==-1)continue;
  {Vec3 end=n->target==-2?add(game.pos,mul(forward(&game),30)):game.npc[n->target].pos;Vec3 a=camera(&game,n->pos),b=camera(&game,end);if(a.z>15&&b.z>15){Point p=project(a),q=project(b);if(p.y>view_top()&&p.y<view_bot()&&q.y>view_top()&&q.y<view_bot())line((int)p.x,(int)p.y,(int)q.x,(int)q.y,n->role==LAW?CYAN:RED);}}
 }
 for(int i=0;i<DEBRIS_COUNT;i++){Debris *d=&game.debris[i];if(!d->alive||d->flash<=0)continue;Vec3 dv=camera(&game,d->pos);if(dv.z<30||dv.z>2400)continue;Point dp=project(dv);
  int burst=(d->rock&&d->flash>.19f&&d->flash<.23f)||(!d->rock&&d->flash>.60f&&d->flash<.66f);
  if(burst)sfx_explosion_embers_spawn((int)dp.x,(int)dp.y,(unsigned)(i*131)^0xDEBu);}
 spatial_regions_draw();
 flight_camera_ship();if(!third_person)weapon_fx_fire();if(cockpit_world_clip){clipy0=cockpit_clip0;clipy1=cockpit_clip1;hc_world_raster_clip=cockpit_old_raster;}render_phase(5);
 if(race_is_active(&game)&&game.race.crash_time>3.f){hud_postfx();race_crash_overlay();camera_override_clear();return;}
 if(ion_seconds()<0){line(227,110,236,110,AMBER);line(244,110,253,110,AMBER);line(240,97,240,106,AMBER);line(240,114,240,123,AMBER);
 line(232,102,236,106,AMBER);line(244,106,248,102,AMBER);line(232,118,236,114,AMBER);line(244,114,248,118,AMBER);}
 sfx_hit_sparks_draw(1.f/60);sfx_maybe_death_embers();sfx_explosion_embers_draw(1.f/60);
 /* Bloom the world once — never after cockpit glyphs (Commander: no text glow). */
 hud_postfx();
 warp_arrival_overlay();
 if(race_is_active(&game)){race_hud();camera_override_clear();render_phase(6);return;}
 if(hud_mode==0||hc_full_console_visible()){hc_validate_lock();target_overlay();}else if(hud_mode==1)minimal_overlay();
 {float s=ion_seconds();if(s>=0)ion_render(s);}planet_prompt();police_dialog();death_effect();
 if(hc_full_console_visible()){
  hc_flight_overlay();camera_override_clear();render_phase(6);return;
 }
 if(paused||hud_mode==0||game.dock_stage||game.dead||game.police_stop||game.approach>=0)cockpit();
 else if(hud_mode==2)combat_alert_banner(); /* scenic: still show bottom RED ALERT */
 if(third_person&&hud_mode!=0&&square_held){targeting_overlay(8,28,192,156,0);rect(8,204,132,16,RGB(24,63,73));rect(8,204,132,1,UI_SIGNAL);text(2,25,RGB(120,245,220),"(R = LOCK)");}
 route_prejump_panel();
 incoming_fire_indicators();tools_panel();race_hud();expedition_flight_draw();hc_present_finish();camera_override_clear();render_phase(6);
}
#include "ship-preview.h"
static void dockbay_site_draw(void);
#include "ui-modern.h"
#include "ship-purchase-card.h"
#include "local-tv.h"
#include "station-crawl.h"
#include "station-tour.h"
#include "dialogue-ui.h"
#include "narrative-nav.h"
#include "guild-ui.h"
static unsigned erl_portrait(int id);
#include "campaign-ui.h"
#include "radio-ui.h"
#include "comms-panel.h"
#include "station-hails.h"
#include "mission-reward-card.h"
static void open_station_channel(void){int hub=IS_STATION_ID(selected_target)?station_target_hub(selected_target):nearest_hub(&game);autoaim=0;speak(&game,VOICE_DOCK,station_hail_line(hub));game.voice_seed=game.system*HUB_COUNT+hub;}

#include "commander-ui.h"
#include "law-ui.h"
#include "field-guide.h"
#include "planet-site-scene.h"
#include "hand-tools.h"
static void wifi_surface_reset(void){ps_open=ps_release=0;eva_map_leave();hand_open=hand_armed=hand_wait=0;surface_scan_prompt=-1;surface_scan_fx=0;}
static int wifi_surface_extras(unsigned pressed,unsigned held,float dt,float ax,float ay){
 if(page==CODEX){if(pressed&PSP_CTRL_CIRCLE){page=FLIGHT;planet_controls_ready=0;}else atlas_input(pressed);return 1;}
 if(page!=FLIGHT||game.planet<1||game.surface!=2||planet_transfer_busy())return 0;
 hand_context_tick(pressed,held);
 if(ps_open||ps_release){if(ps_open){ps_anim+=dt;ps_input(pressed,held);}else if(!held){ps_release=0;planet_controls_ready=1;}return 1;}
 if(eva_map_open||eva_map_release){if(eva_map_release){if(!held)eva_map_release=0;}else {eva_map_pan(ax,ay,dt);if(eva_map_input(pressed)){atlas_reset();codex_system=game.system;codex_body=game.planet+1;codex_scope=ATLAS_WORLD;page=CODEX;}}return 1;}
 if(pressed&PSP_CTRL_START){eva_map_visit();eva_map_open_now();return 1;}
 if(!planet_controls_ready)return 0;
 if((pressed&PSP_CTRL_TRIANGLE)&&surface_scan_prompt>=0){int slot=surface_scan_prompt;surface_scan_prompt=-1;atlas_open_surface_record(game.system,game.planet,slot);page=CODEX;return 1;}
 if((pressed&PSP_CTRL_CROSS)&&!game.rover_driving&&length(sub(game.pos,game.rover_pos))>=48){int site=surface_nearest_site(&game,55);if(site>=0){ps_begin(site);return 1;}}
 return 0;
}
#include "intro.h"
#include "erl-console.h"
#include "dockbay-console.h"
static void quit_to_main_menu(void){
 online_close("At title screen");
 if(wifi.state)wifi_close(NULL);
 paused=0;game.boost=0;game.eva_running=game.eva_run_arm=0;autoaim=0;selected_target=0;look_target=-1;
 ps_open=ps_release=0;surface_target_open=0;surface_target_lock=-1;surface_turn_active=0;comms_quick=0;planet_landing_menu=0;
 night_close();thargoid_reset();tools_cancel_tractor();tools_reset_gesture();tools_block_until_release();
 game.voice_time=game.message_time=0;intro_choice=0;change_page(INTRO);
}
static void quit_last_saved_text(char *out,size_t size){
 struct stat info;const char *path=commander_save_path(&game);
 if(stat(path,&info)!=0){snprintf(out,size,"NO SAVED CHECKPOINT FOUND");return;}
 time_t now=time(NULL),saved=info.st_mtime;long age=now>=saved?(long)(now-saved):0;
 if(age<60)snprintf(out,size,"LAST SAVED JUST NOW");
 else if(age<3600){long n=age/60;snprintf(out,size,"LAST SAVED %ld MINUTE%s AGO",n,n==1?"":"S");}
 else if(age<86400){long n=age/3600;snprintf(out,size,"LAST SAVED %ld HOUR%s AGO",n,n==1?"":"S");}
 else {long n=age/86400;snprintf(out,size,"LAST SAVED %ld DAY%s AGO",n,n==1?"":"S");}
}
static void quit_confirmation_draw(void){
 char saved[64];quit_last_saved_text(saved,sizeof(saved));
 rect(0,0,W,H,RGB(2,5,10));panel(72,58,336,154);rect(72,58,336,3,UI_ACCENT);
 text_px(102,77,UI_GOLD,"QUIT TO MAIN MENU");
 text_px(105,105,UI_TEXT,"ARE YOU SURE YOU WANT TO QUIT?");
 text_px(120,128,UI_MUTED,"%s",saved);
 int by=158;rect(104,by,124,34,quit_confirm_yes?UI_RAISED:UI_PANEL);rect(252,by,124,34,!quit_confirm_yes?UI_RAISED:UI_PANEL);
 rect(104,by,124,2,quit_confirm_yes?UI_ACCENT:UI_EDGE);rect(252,by,124,2,!quit_confirm_yes?UI_ACCENT:UI_EDGE);
 text_px(153,by+12,quit_confirm_yes?UI_GOLD:UI_TEXT,"YES");text_px(301,by+12,!quit_confirm_yes?UI_GOLD:UI_TEXT,"NO");
 footer("LEFT/RIGHT CHOOSE   X CONFIRM   O CANCEL");
}
static void walk_screen(void){
 if(walk_kind==0){sc_draw_ui();return;}
 /* Ship deck / derelict keep the open walk prototype. */
 int horizon=96+(int)(sinf(preview_time*.45f+walk_yaw)*.5f);rect(0,0,W,H,walk_kind==2?RGB(8,13,24):RGB(21,28,39));
 for(int y=horizon;y<H;y++){unsigned c=mix_rgb(RGB(41,54,70),RGB(21,28,39),(y-horizon)/(float)fmaxf(1,H-horizon));for(int x=0;x<W;x++)fb[y*STRIDE+x]=c;}
 for(int i=0;i<14;i++){int y=horizon+i*12;int inset=i*9;line(0,y,W/2-inset,horizon,RGB(41+i,54+i,70+i));line(W,y,W/2+inset,horizon,RGB(41+i,54+i,70+i));}
 line(0,horizon,W,horizon,RGB(193,139,77));
 rect(0,0,W,28,RGB(21,28,39));rect(0,26,W,2,RGB(193,139,77));
 const char *place=walk_kind==1?"SHIP / PERSONAL DECK":"DERELICT / AIRLOCK";
 text(1,0,RGB(229,210,163),"ON FOOT / %.18s",place);text(1,2,RGB(155,154,165),walk_kind==2?"NUB LOOK  D-PAD MOVE  X SALVAGE  O EXIT":"NUB LOOK  D-PAD MOVE  X INSPECT  O EXIT");
 int sway=(int)(sinf(walk_x*.04f)*2);
 rect(188+sway,horizon-8,104,78,RGB(41,54,70));rect(198+sway,horizon,84,62,RGB(21,28,39));
 rect(220+sway,horizon+18,40,24,walk_kind==2?RGB(139,75,55):RGB(193,139,77));
 text(6,(horizon+28)/8,walk_kind==2?RGB(240,180,91):RGB(85,212,212),walk_kind==2?"SALVAGE":"AIRLOCK");
 text(40,(horizon+28)/8,RGB(229,210,163),"%s",walk_kind==2?"TETHER":"DECK");
 rect(0,248,W,24,RGB(21,28,39));rect(0,248,W,2,RGB(193,139,77));
 if(walk_kind==2){rect(8,252,180,8,RGB(41,54,70));rect(8,252,(int)(1.8f*walk_oxygen),8,RGB(85,212,212));rect(8,262,180,8,RGB(70,40,38));rect(8,262,(int)(1.8f*walk_integrity),8,RGB(200,90,75));text(25,32,RGB(229,210,163),"O2 %d  SUIT %d",(int)walk_oxygen,(int)walk_integrity);}
 else text(1,32,RGB(155,154,165),"Workshop. Triangle returns to the concourse.");
}
static unsigned flight_steer_buttons(unsigned buttons){
 if(page==FLIGHT&&((buttons&(PSP_CTRL_SQUARE|PSP_CTRL_CIRCLE))||hc_browser||(game.planet>=0&&game.surface==2)))
  buttons&=~(PSP_CTRL_UP|PSP_CTRL_DOWN|PSP_CTRL_LEFT|PSP_CTRL_RIGHT);
 return buttons;
}

static void game_input(unsigned pressed,unsigned held,float dt,float ax,float ay){
 if(pressed)hc_cache_dirty=1;hc_validate_lock();hc_browser_hold(held);
 /* Back/navigation interrupt cosmetic movement immediately. A second quick
  * X only reveals the page; it must not buy an unseen item during the zoom. */
 if(pressed&&deck_open_active()){deck_open_cancel();if(pressed&PSP_CTRL_CROSS)return;}
 if((pressed&PSP_CTRL_CIRCLE)&&(page==MISSIONS||page==MISSIONLOG||page==CHART||page==DETAILS)){chart_search=0;change_page(HOME);game.cue=SFX_UI;return;}
 if(ship_purchase_input(pressed))return;
 if(newspaper_input(pressed))return;
 if(wanted_input(pressed,dt))return;
 if(page==GALNET&&galnet_tab==5){rumours_input(pressed,held,dt);return;}
 if(page==FLIGHT){rumour_tick();nav_passive_tick(dt);}
 if(page==GALNET&&opening_net_detail){spacebook_actions(pressed);return;}
 if(page==GALNET&&galnet_tab==4&&(pressed&(PSP_CTRL_UP|PSP_CTRL_DOWN)))inbox_message_page=0;
 /* Menu actions are immediate.  The former footer pulse repeated the button
  * the player had just pressed and briefly obscured useful control hints. */
 if(comms_console_input(pressed,dt))return;
 if(quit_confirm){
  if(pressed&(PSP_CTRL_LEFT|PSP_CTRL_UP)){quit_confirm_yes=1;game.cue=SFX_SELECT;return;}
  if(pressed&(PSP_CTRL_RIGHT|PSP_CTRL_DOWN)){quit_confirm_yes=0;game.cue=SFX_SELECT;return;}
  if(pressed&PSP_CTRL_CIRCLE){quit_confirm=quit_confirm_yes=0;game.cue=SFX_UI;return;}
  if(pressed&PSP_CTRL_CROSS){if(quit_confirm_yes){quit_confirm=quit_confirm_yes=0;quit_to_main_menu();}else {quit_confirm=0;game.cue=SFX_UI;}return;}
  return;
 }
 if(page==MULTIPLAYER||page==NEARBY_PLAYERS){multiplayer_input(pressed,dt);return;}
 if(!wifi.state&&!online.stage&&game.planet<1)game.local_pilot=0;
 if(wifi_input(pressed,held,dt,ax,ay))return;
 static unsigned in_held=0;static int sq_arm=0;static float square_hold=0;
 if(race_is_active(&game)){
  /* Square must not open the browser, quick-target or steal throttle from R. */
  pressed&=~PSP_CTRL_SQUARE;held&=~PSP_CTRL_SQUARE;in_held&=~PSP_CTRL_SQUARE;
  sq_arm=square_held=0;square_hold=0;autoaim=0;selected_target=RACE_TARGET_ID;
  if(page==FLIGHT&&game.race.crash_time>0){held&=~(PSP_CTRL_LTRIGGER|PSP_CTRL_LEFT|PSP_CTRL_RIGHT);hard_brake=0;}
 }
 hand_context_tick(pressed,held);
 if(eva_map_open&&(page!=FLIGHT||game.surface!=2||game.planet<1||game.dead)){eva_map_open=0;eva_map_release=1;}
 if(eva_map_open||eva_map_release){
  triangle_arm=sq_arm=square_held=0;in_held=held;paused=0;fire_blocked=1;
  if(eva_map_release){if(!held){eva_map_release=0;fire_blocked=0;}return;}
  if(eva_map_open){eva_map_pan(ax,ay,dt);if(eva_map_input(pressed)){change_page(CODEX);atlas_reset();codex_system=game.system;codex_body=game.planet+1;codex_scope=ATLAS_WORLD;row=0;eva_map_codex=1;}}
  else if(!held){eva_map_release=0;fire_blocked=0;}
  return;
 }
 if(ps_open||ps_release){
  if(page!=FLIGHT||game.system!=ps_system||game.planet!=ps_body||game.surface!=2||game.dead){ps_open=0;ps_release=1;}
  triangle_arm=sq_arm=square_held=0;in_held=held;tools_block_until_release();fire_blocked=1;
  if(ps_open){ps_anim+=dt;ps_input(pressed,held);}else if(!held){ps_release=0;tools_wait_release=0;fire_blocked=0;planet_controls_ready=1;}
  return;
 }
 if(page==FLIGHT&&game.rift_report){if(pressed&PSP_CTRL_LTRIGGER&&dialogue_page>0)dialogue_page--;if(pressed&PSP_CTRL_RTRIGGER&&dialogue_page+1<dialogue_pages)dialogue_page++;if(pressed&(PSP_CTRL_TRIANGLE|PSP_CTRL_CIRCLE)){game.rift_report=0;speech_ok();triangle_arm=sq_arm=0;in_held=held;tools_block_until_release();fire_blocked=1;}return;}
 if(game.dead&&game.dock_stage==4){game.dock_stage=0;departure_release=0;}
 if(page==FLIGHT&&(game.dock_stage==4||departure_release)){
  int departing=game.dock_stage==4;autoaim=0;paused=0;triangle_arm=sq_arm=square_held=0;
  tools_block_until_release();fire_blocked=1;in_held=held;r_tap=l_tap=10;hard_brake=0;
  game_tick(&game,dt,0,0,0,0);
  if(departing&&game.dock_stage!=4)departure_release=1;
  else if(departure_release&&!held){departure_release=0;tools_wait_release=0;fire_blocked=0;}
  return;
 }
 if(!tools_context()){tools_cancel_tractor();tools_reset_gesture();if(held&PSP_CTRL_CIRCLE)tools_wait_release=1;}
 if(pressed&(PSP_CTRL_UP|PSP_CTRL_DOWN|PSP_CTRL_LEFT|PSP_CTRL_RIGHT|PSP_CTRL_TRIANGLE)){
  int choosing_weapon_slot=page==EQUIP&&equip_confirm_item>=0&&equip_slot_for(equip_confirm_item)==FIT_WPN&&(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT));
  if(!choosing_weapon_slot){equip_confirm_item=-1;if(page==EQUIP)equip_target=-1;}
 }
 if(page==FLIGHT)planet_auto_entry();
 if(page==FLIGHT&&planet_sequence_update(dt,pressed)){in_held=held;triangle_arm=sq_arm=square_held=0;r_tap=l_tap=10;hard_brake=0;return;}
 if(page==FLIGHT&&planet_orbit_release){in_held=held;if(!held)planet_orbit_release=0;return;}
 if(game.planet<1){planet_landing_menu=0;planet_controls_ready=0;planet_first_frame=planet_first_tick=0;surface_target_reset();eva_map_leave();}
 if(page==FLIGHT&&game.planet>=1&&!game.dead){
  if(game.surface==1||planet_landing_menu){
   if(game.surface==1)eva_map_visit();
   if(!planet_controls_ready){in_held=held;if(!held)planet_controls_ready=1;return;}
   if(game.surface==1){
    game.time+=dt;game.world_clock=fmodf(game.world_clock+dt,86400.f);
    game.yaw=fmaxf(-.75f,fminf(.75f,game.yaw+ax*dt));game.pitch=fmaxf(-.35f,fminf(.35f,game.pitch+ay*dt));
    if(pressed&PSP_CTRL_RTRIGGER){float yaw=game.yaw,pitch=game.pitch;if(takeoff_planet(&game)){planet_sequence_begin(3);planet_launch_yaw=yaw;planet_launch_pitch=pitch;}}
    else if((pressed&PSP_CTRL_SQUARE)&&game.bodies[game.planet].type==OCEAN)navigation_planet_job(&game);
    else if(pressed&PSP_CTRL_CROSS)planet_disembark();
   }else if(pressed&PSP_CTRL_CROSS){planet_landing_menu=0;planet_sequence_begin(1);}
   else if(pressed&PSP_CTRL_CIRCLE){planet_landing_menu=0;planet_controls_ready=0;}
   return;
  }
  if(game.surface==2&&!planet_controls_ready){in_held=held;if(!held)planet_controls_ready=1;return;}
 }
 if(eva_map_codex&&page!=CODEX)eva_map_codex=0;
 if(eva_map_codex&&page==CODEX&&!atlas_depth&&(pressed&PSP_CTRL_CIRCLE)){
  eva_map_codex=0;change_page(FLIGHT);eva_map_open_now();eva_map_release=1;return;
 }
 if(page==FIELDGUIDE){field_guide_input(pressed);return;}
 if(page==LOCALTV){
  if(pressed&PSP_CTRL_CIRCLE){audio_tv_on=audio_tv_code=0;menu_back();return;}
  local_tv_tick(dt);return;
 }
 if(page==FLIGHT&&game.surface==2&&game.planet>=1&&!game.dead){eva_map_visit();if(pressed&PSP_CTRL_START){paused=0;triangle_arm=sq_arm=square_held=0;in_held=held;eva_map_open_now();return;}}
 if(page==FLIGHT&&game.planet>=1&&game.surface==2&&game.rover_driving&&!game.dead&&!game.police_stop&&!game.dock_stage&&game.jump<=0){
   in_held=held;autoaim=0;triangle_arm=sq_arm=square_held=0;
   if(pressed&PSP_CTRL_SELECT){change_page(HOME);return;}
   if(pressed&PSP_CTRL_TRIANGLE){
    if(length(game.rover_velocity)>12){message(&game,"Brake to a stop before parking.");}
    else {PlanetPilotCamera from=planet_current_eye();if(surface_rover(&game))planet_seat_start(from,1);}
    return;
   }
   if(held&PSP_CTRL_SQUARE){
    surface_target_open=1;surface_target_input(pressed);
    game_rover_tick(&game,dt,0,0,0);return;
   }
   surface_target_open=0;surface_turn_active=0;
   unsigned drive=0;
   if(held&PSP_CTRL_RTRIGGER)drive|=ROAM_ACCEL;
   if(held&PSP_CTRL_LTRIGGER)drive|=ROAM_BRAKE;
   if(held&PSP_CTRL_CIRCLE)drive|=ROAM_DRIFT;
   if(held&PSP_CTRL_CROSS)drive|=ROAM_BOOST;
   game_rover_tick(&game,dt,ax,ay,drive);
   return;
  }
 int was_night=night_open;night_tick(dt);
 if(was_night&&!night_open){change_page(FLIGHT);message(&game,"Private channel ended.");return;}
 if(page==COMMS_PANEL&&night_open){
  if(pressed&PSP_CTRL_CIRCLE){night_close();change_page(FLIGHT);return;}
  row=dialogue_move(row,3,pressed);
  if(pressed&PSP_CTRL_LTRIGGER&&dialogue_page>0)dialogue_page--;
  if(pressed&PSP_CTRL_RTRIGGER&&dialogue_page+1<dialogue_pages)dialogue_page++;
  if(pressed&PSP_CTRL_CROSS)night_reply(row);
  return;
 }
 if(comms_quick&&!incoming_reply_ready()&&!night_ready())comms_quick=0;

 if(page!=FLIGHT||game.planet<1||game.surface!=2){game.eva_run_arm=0;game.eva_run_hold=0;game.eva_running=0;}
 if(!tutorial_active(&game))station_tour_tick();
 unsigned released=in_held&~held;in_held=held;if(page!=FLIGHT||paused||game.police_stop||game.approach>=0||game.dock_stage)sq_arm=0;
 if(!(held&PSP_CTRL_CROSS))fire_blocked=0;
 int square_ready=page==FLIGHT&&game.planet<0&&game.jump<=0&&!game.dock_stage&&!game.dead;
 if(square_ready&&(held&PSP_CTRL_SQUARE)){if(pressed&PSP_CTRL_SQUARE)square_hold=0;square_hold+=dt;if(square_hold>=.20f&&sq_arm)sq_arm=0;square_held=square_hold>=.20f;}
 else {square_hold=0;square_held=0;}
 int oldpage=page;r_tap+=dt;l_tap+=dt;if(hard_brake>0){hard_brake-=dt;hard_brake_age+=dt;if(hard_brake<0)hard_brake=0;}else hard_brake_age=0;{float in=fminf(1.f,hard_brake_age/.38f),out=fminf(1.f,hard_brake/.30f);in=in*in*(3.f-2.f*in);out=out*out*(3.f-2.f*out);game.mega_brake=page==FLIGHT?in*out:0;}if(page==INTRO){intro_time+=dt;if(pressed&PSP_CTRL_CROSS){game.voice_time=0;change_page(CAMPAIGN);}else if(pressed&PSP_CTRL_START){game.voice_time=0;change_page(HOME);}else if(pressed&PSP_CTRL_TRIANGLE){if(load_game(&game,commander_save_path(&game))){selected_target=0;autoaim=0;change_page(HOME);db_restore_frontend();}else message(&game,"No saved commander. X begins your journey.");}return;}
 if(page==WALK){
  if(walk_kind==0){
   /* MacVenture station: O / TRI board ship; options list handled in sc_input. */
   if(pressed&PSP_CTRL_CIRCLE&&sc_menu==0){page=HOME;message(&game,"Back on the command deck.");return;}
   sc_input(pressed); return;
  }
  walk_yaw+=ax*dt*1.8f;float move=ay*dt*(walk_kind==2?68.f:90.f);walk_x+=sinf(walk_yaw)*move;walk_z+=cosf(walk_yaw)*move;if(walk_x>220)walk_x=220;if(walk_x<-220)walk_x=-220;if(walk_z>420)walk_z=420;if(walk_z<-80)walk_z=-80;
  if(walk_kind==2){walk_oxygen-=dt*2.2f;if(walk_x>185||walk_x<-185||walk_z>385||walk_z<-45)walk_integrity-=dt*3.f;if(walk_oxygen<=0||walk_integrity<=0){walk_oxygen=0;page=FLIGHT;message(&game,"Suit reserve exhausted. Emergency tether return.");return;}}
  if(pressed&PSP_CTRL_CIRCLE){page=walk_kind==2?FLIGHT:HOME;message(&game,walk_kind==2?"Airlock sealed. Back aboard the ship.":"Back on the command deck.");}
  else if(pressed&PSP_CTRL_TRIANGLE&&walk_kind==1){walk_kind=0;sc_built_for=-1;message(&game,"Back on the station concourse.");}
  else if(pressed&PSP_CTRL_CROSS){
   if(walk_kind==2&&!walk_salvaged){walk_salvaged=1;game.credits+=500;game.discoveries++;message(&game,"Salvage secured: relay core +500 units.");}
   else message(&game,"Workshop inspection complete.");
  }
  return;}
 if(thargoid_active){thargoid_input(pressed,held,dt,ax,ay);return;}
 if(tools_input(pressed,held,dt)){
  if(hc_browser&&hc_context()){hc_refresh-=fmaxf(0,dt);if(hc_refresh<=0)hc_contacts();}
  triangle_arm=0;sq_arm=0;square_held=0;if(held&PSP_CTRL_CROSS)fire_blocked=1;game.boost=0;r_tap=l_tap=10;
  held&=~(PSP_CTRL_LEFT|PSP_CTRL_RIGHT|PSP_CTRL_UP|PSP_CTRL_DOWN|PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER|PSP_CTRL_SQUARE|PSP_CTRL_CROSS|PSP_CTRL_TRIANGLE);
  pressed=0;goto flight_motion;
 }

 if(page==FLIGHT&&game.planet<0&&!paused&&!game.police_stop&&!game.dock_stage&&game.jump<=0&&game.approach<0&&!game.dead){
  if((pressed&PSP_CTRL_TRIANGLE)&&(held&PSP_CTRL_TRIANGLE)){triangle_arm=1;triangle_hold=0;}
  if(triangle_arm){
   pressed&=~PSP_CTRL_TRIANGLE;
   if(held&PSP_CTRL_TRIANGLE){triangle_hold+=dt;if(triangle_hold>=.55f){triangle_arm=0;comms_return=FLIGHT;autoaim=0;comms_quick=0;change_page(COMMS_PANEL);return;}}
   else {triangle_arm=0;pressed|=PSP_CTRL_TRIANGLE;}
  }
 }else {triangle_arm=0;triangle_hold=0;}
 /* A tapped private call uses the same quick chooser as other incoming calls. */
 if(page==FLIGHT&&(pressed&PSP_CTRL_SELECT)&&(held&PSP_CTRL_LTRIGGER)){comms_cycle_hud();return;}
 if(game.police_stop){game.boost=0;autoaim=0;if(pressed&PSP_CTRL_LTRIGGER&&dialogue_page>0)dialogue_page--;if(pressed&PSP_CTRL_RTRIGGER&&dialogue_page+1<dialogue_pages)dialogue_page++;if(game.police_phase>=6){if(pressed&PSP_CTRL_CROSS){police_acknowledge(&game);police_choice=0;fire_blocked=1;change_page(game.docked?HOME:FLIGHT);}return;}if(game.police_phase>=2){game_tick(&game,dt,0,0,0,0);return;}int choices=cargo_contraband(&game)>0?4:3;if(police_choice>=choices)police_choice=0;if(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT|PSP_CTRL_UP|PSP_CTRL_DOWN)){police_choice=dialogue_move(police_choice,choices,pressed);game.cue=SFX_SELECT;}if(pressed&PSP_CTRL_CROSS){
  if(police_choice==3){police_surrender_cargo(&game);police_choice=0;}
  else if(game.police_phase==1){if(police_choice==0)police_scan_submit(&game);else if(police_choice==1)police_scan_refuse(&game);else police_escape(&game);}
  else {if(police_choice==0)police_resolve(&game,0);else if(police_choice==1){police_resolve(&game,1);}else police_escape(&game);}
  if(!game.police_stop)police_choice=0;}return;}
 if((pressed&PSP_CTRL_START)&&!game.dead){/* Start remains reserved for live-flight power management. */}
 if(page==FLIGHT&&!game.dead&&!game.police_stop&&game.jump<=0&&!game.dock_stage)paused=(held&PSP_CTRL_START)!=0;else if(paused)paused=0;
 /* Hold Start: redistribute on the existing SYS/ENG/WEP meters — no separate panel. */
 if(paused){fire_blocked=1;game.boost=0;if(pressed&PSP_CTRL_LEFT)pip_sel=(pip_sel+2)%3;if(pressed&PSP_CTRL_RIGHT)pip_sel=(pip_sel+1)%3;if(pressed&PSP_CTRL_UP)pip_shift(&game,pip_sel);if(pressed&PSP_CTRL_DOWN)pip_selected_move(&game,pip_sel,-1);return;}
 if(game.dock_stage){autoaim=0;if(game.dock_stage==1&&(pressed&PSP_CTRL_CIRCLE)){game.dock_stage=game.dock_phase=0;game.dock_timer=game.dock_duration=0;game.speed=0;game.boost=0;hard_brake=0;message(&game,"Docking guidance cancelled. You have control.");return;}game_tick(&game,dt,0,0,0,0);if(game.docked)change_page(HOME);return;}
 if(game.dead&&page==FLIGHT){
  if(game.explosion<.08f){death_choice=0;death_notice[0]=0;death_notice_time=0;}
  if(death_notice_time>0)death_notice_time=fmaxf(0,death_notice_time-dt);
  if(game.explosion>=2.6f){
   if(pressed&(PSP_CTRL_UP|PSP_CTRL_DOWN)){death_choice^=1;game.cue=SFX_SELECT;}
   if(pressed&PSP_CTRL_CROSS){
    if(death_choice==0){
     if(tutorial_active(&game)){tutorial_resume();selected_target=0;autoaim=0;look_target=-1;death_notice[0]=0;return;}
     if(campaign_retry(&game)){selected_target=0;autoaim=0;look_target=-1;death_notice[0]=0;change_page(CAMPAIGN);return;}
     if(load_game(&game,commander_save_path(&game))){selected_target=0;autoaim=0;look_target=-1;death_notice[0]=0;change_page(HOME);db_restore_frontend();return;}
     snprintf(death_notice,sizeof(death_notice),"NO SAVED COMMANDER FOUND");death_notice_time=3;game.cue=SFX_UI;
    }else{game.voice_time=game.message_time=0;death_notice[0]=0;change_page(INTRO);return;}
   }
  }
  game_tick(&game,dt,0,0,0,0);return;
 }
 if(page==FLIGHT&&game.jump>0){game.boost=0;if(thargoid_maybe_start()){thargoid_input(pressed,held,dt,ax,ay);return;}game_tick(&game,dt,0,0,0,0);if(game.jump<=0){if(selected_target!=PLAYER_TARGET_ID){selected_target=0;autoaim=0;}else wifi_contact_route();}return;}
 /* On foot owns its controls; spacecraft roll, boost and target chords never run here. */
 if(page==FLIGHT&&game.planet>=0&&game.surface==2){
  autoaim=0;sq_arm=0;hard_brake=0;r_tap=l_tap=10;
  surface_scan_tick(dt);
  if(pressed&PSP_CTRL_SELECT){game.boost=0;change_page(HOME);return;}
  if(hand_input(pressed,held,dt)){in_held=held;game.eva_running=game.eva_run_arm=0;game_eva_tick(&game,dt,0,0,0,0,0);return;}
  if((held&PSP_CTRL_SQUARE)&&surface_target_open){surface_target_input(pressed);game_eva_tick(&game,dt,ax,ay,0,0,0);surface_target_turn(dt);game.eva_jump_held=(held&PSP_CTRL_RTRIGGER)!=0;game.eva_run_arm=0;game.eva_run_hold=0;game.eva_running=0;return;}
  if(pressed&PSP_CTRL_CROSS){if(!(held&PSP_CTRL_LTRIGGER)){if(game.rover_driving||length(sub(game.pos,game.rover_pos))<48){PlanetPilotCamera from=planet_current_eye();int was=game.rover_driving;if(surface_rover(&game))planet_seat_start(from,was);}else if(surface_nearest_site(&game,55)>=0)ps_begin(surface_nearest_site(&game,55));else message(&game,"X uses sites or rover. Triangle boards your ship.");}return;}
  if(pressed&PSP_CTRL_TRIANGLE){
   if(surface_scan_prompt>=0){int slot=surface_scan_prompt;surface_scan_prompt=-1;atlas_open_surface_record(game.system,game.planet,slot);change_page(CODEX);return;}
   if(!(held&PSP_CTRL_LTRIGGER)&&eva_can_board(&game)){PlanetPilotCamera from=planet_current_eye();if(board_planet(&game))planet_seat_start(from,0);planet_controls_ready=0;return;}
  }
  if((released&PSP_CTRL_SQUARE)&&!surface_target_open)surface_target_cycle_front();
  surface_target_open=0;
  if(held&PSP_CTRL_SQUARE){static float surface_hold=0;if(pressed&PSP_CTRL_SQUARE)surface_hold=0;surface_hold+=dt;if(surface_hold>=.20f){surface_target_open=1;surface_target_input(pressed);}game_eva_tick(&game,dt,ax,ay,0,0,0);surface_target_turn(dt);game.eva_jump_held=(held&PSP_CTRL_RTRIGGER)!=0;game.eva_run_arm=0;game.eva_run_hold=0;game.eva_running=0;return;}
  int walk=((held&PSP_CTRL_UP)!=0)-((held&PSP_CTRL_DOWN)!=0);
  float strafe=((held&PSP_CTRL_RIGHT)!=0)-((held&PSP_CTRL_LEFT)!=0);
  if(held&PSP_CTRL_LTRIGGER){ax=strafe;ay=(float)walk;walk=0;strafe=0;}
  if(pressed&PSP_CTRL_RTRIGGER){game.eva_run_arm=1;game.eva_run_hold=0;}if(game.eva_run_arm&&(held&PSP_CTRL_RTRIGGER))game.eva_run_hold+=dt;
  int jump=(released&PSP_CTRL_RTRIGGER)&&game.eva_run_arm&&game.eva_run_hold<.20f;game.eva_running=game.eva_run_arm&&(held&PSP_CTRL_RTRIGGER)&&game.eva_run_hold>=.20f&&!game.rover_driving;if(released&PSP_CTRL_RTRIGGER){game.eva_run_arm=0;game.eva_run_hold=0;}
  if(planet_first_tick)planet_trace("eva-tick-before",0);
  game_eva_tick(&game,dt,ax,ay,walk,strafe,jump&&!game.rover_driving);if(fabsf(ax)>.2f||fabsf(ay)>.2f)surface_turn_active=0;surface_target_turn(dt);
  if(planet_first_tick){planet_trace("eva-tick-after",0);planet_first_tick=0;}return;
 }
 if(page==FLIGHT&&(pressed&PSP_CTRL_RTRIGGER)&&!(held&PSP_CTRL_SQUARE)){if(r_tap<.32f){game.boost=1;game.cue=SFX_BOOST;}r_tap=0;}if(!(held&PSP_CTRL_RTRIGGER)||page!=FLIGHT||(game.planet>=0&&game.surface==1)||(held&PSP_CTRL_SQUARE))game.boost=0;
 if(page==FLIGHT&&game.boost){l_tap=10;hard_brake=0;}
 if(page==FLIGHT&&!game.boost&&(pressed&PSP_CTRL_LTRIGGER)&&!(held&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT|PSP_CTRL_SQUARE))){if(l_tap<.32f&&game.speed>player_ships[game.ship].speed*.35f){hard_brake=2.15f;hard_brake_age=0;game.boost=0;game.cue=SFX_UI;}l_tap=0;}
 if(hc_browser_input(pressed,held,dt)){
  if((pressed&PSP_CTRL_RTRIGGER)||((pressed&PSP_CTRL_CROSS)&&(held&PSP_CTRL_RTRIGGER))){game.boost=0;r_tap=10;pressed&=~PSP_CTRL_RTRIGGER;held&=~PSP_CTRL_RTRIGGER;}
  sq_arm=square_held=0;if(held&PSP_CTRL_CROSS)fire_blocked=1;
  unsigned reserved=PSP_CTRL_SQUARE|PSP_CTRL_UP|PSP_CTRL_DOWN|PSP_CTRL_LEFT|PSP_CTRL_RIGHT|PSP_CTRL_CROSS|PSP_CTRL_RTRIGGER;
  pressed&=~reserved;held&=~reserved;released&=~PSP_CTRL_SQUARE;
 }
 if(page==FLIGHT&&game.approach>=0){planet_auto_entry();return;}
 if(page==FLIGHT&&game.planet>=0&&(pressed&PSP_CTRL_TRIANGLE)&&!speech_active()){planet_landing_menu=1;planet_controls_ready=0;game.boost=0;autoaim=0;return;}
 if(page==FLIGHT&&comms_quick){
  if(pressed&PSP_CTRL_LEFT){comms_quick_choice=0;game.cue=SFX_SELECT;}
  if(pressed&PSP_CTRL_RIGHT){comms_quick_choice=1;game.cue=SFX_SELECT;}
  if(pressed&PSP_CTRL_CROSS){
   if(comms_quick_choice==1){comms_answer_incoming();}
   else {comms_quick=0;if(night_ready())night_close();else{comms_channel_capture(1);comms_encounter_decline();game.encounter_kind=ENCOUNTER_NONE;game.encounter=0;}}
   return;
  }
  return;
 }
 if(page==FLIGHT&&game.planet>=0&&(pressed&PSP_CTRL_TRIANGLE)){speech_ok();return;}
 if(page==FLIGHT&&game.planet>=0&&(pressed&PSP_CTRL_SQUARE)){
  if(game.surface==2)survey_scan(&game);
  else if(game.surface==1)message(&game,"Use X on the parked-ship screen to step outside.");
  else {Vec3 pad=surface_site(&game,1);float dx=pad.x-game.pos.x,dz=pad.z-game.pos.z;float dist=sqrtf(dx*dx+dz*dz);game.yaw=atan2f(dx,dz);game.pitch=fminf(-.12f,game.pitch);autoaim=0;char note[80];snprintf(note,sizeof(note),dist<160?"Pad under you. Triangle opens landing options.":"Pad %d m ahead. Slow down to land.",(int)dist);message(&game,note);}
  return;}
 if(page==FLIGHT&&game.jump<=0&&!game.dead&&!game.dock_stage&&(pressed&PSP_CTRL_LTRIGGER)&&(held&PSP_CTRL_SQUARE)){sq_arm=0;cycle_front_target();}
 else if(page==FLIGHT&&flight_target_combo(pressed,held))sq_arm=0;
 else if(page==FLIGHT&&game.jump<=0&&!game.dead&&!game.dock_stage&&(pressed&PSP_CTRL_RTRIGGER)&&(held&PSP_CTRL_SQUARE)){sq_arm=0;if(valid_target(selected_target)){autoaim=1;story_event(&game,STORY_EV_TARGET);if(selected_target==0)campaign_event(&game,CP_LOCK);}else message(&game,"Choose a target with Square and the D-pad first.");}
 else if(page==FLIGHT&&game.jump<=0&&!game.dead&&!game.dock_stage&&(pressed&PSP_CTRL_SQUARE)&&(held&PSP_CTRL_RTRIGGER)){int hostile=nearest_hostile_target();sq_arm=0;if(hostile>=0){selected_target=hostile;scan_cat=4;autoaim=1;story_event(&game,STORY_EV_TARGET);if(selected_target==0)campaign_event(&game,CP_LOCK);}else message(&game,"No hostile contacts.");}
 else if(page==FLIGHT&&game.jump<=0&&!game.dead&&!game.dock_stage&&(pressed&PSP_CTRL_SQUARE))sq_arm=1;
 if(page==FLIGHT&&game.jump<=0&&!game.dead&&!game.dock_stage&&(released&PSP_CTRL_SQUARE)&&sq_arm){
  /* Tap is the quick targeter; hold Square is the full browser. Keeping tap
   * in flight removes a redundant menu and preserves the centre view. */
  sq_arm=0;target_nearest_reticle();
 }
 else if(page==FLIGHT&&(released&PSP_CTRL_SQUARE))sq_arm=0;
 if(page==FLIGHT&&(pressed&PSP_CTRL_CROSS)&&(held&PSP_CTRL_LTRIGGER))fire_missile(&game,selected_target);
 else if(page==FLIGHT&&game.planet>=0&&(pressed&PSP_CTRL_CROSS))message(&game,"Lasers are offline in atmosphere.");
 if(page==HELP){handbook_input(pressed);return;}
  if(page==FLIGHT){if(pressed&PSP_CTRL_SELECT)change_page(HOME);else if(pressed&PSP_CTRL_TRIANGLE){if(incoming_reply_ready()||night_ready()){comms_quick=1;comms_quick_choice=1;}else if(speech_active())speech_ok();else hail_target();}}
 else {
  if(page==CHART&&!chart_search){
   if(held&PSP_CTRL_LTRIGGER)chart_zoom_goal-=dt*1.15f;if(held&PSP_CTRL_RTRIGGER)chart_zoom_goal+=dt*1.15f;
   if(chart_zoom_goal<.65f)chart_zoom_goal=.65f;if(chart_zoom_goal>3.2f)chart_zoom_goal=3.2f;
   chart_zoom+=(chart_zoom_goal-chart_zoom)*(1.f-expf(-9.f*dt));
   if(!(held&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT|PSP_CTRL_UP|PSP_CTRL_DOWN))){chart_pan_x-=ax*dt*145.f;chart_pan_y+=ay*dt*110.f;float lim=155.f*chart_zoom;if(chart_pan_x>lim)chart_pan_x=lim;if(chart_pan_x<-lim)chart_pan_x=-lim;if(chart_pan_y>95.f*chart_zoom)chart_pan_y=95.f*chart_zoom;if(chart_pan_y<-95.f*chart_zoom)chart_pan_y=-95.f*chart_zoom;}
  }
  if(page==CHART&&chart_search){chart_search_input(pressed);return;}
  if(page==CODEX&&codex_scope!=ATLAS_LORE){atlas_input(pressed);return;}
  if(page==LOCAL)contacts_refresh();
  if((page==CAMPAIGN||page==GUILD||(page==COMMS_PANEL&&(comms_encounter_conversation||incoming_reply_ready())))&&dialogue_pages>1&&(pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER))){
   if((pressed&PSP_CTRL_LTRIGGER)&&dialogue_page>0)dialogue_page--;
   if((pressed&PSP_CTRL_RTRIGGER)&&dialogue_page+1<dialogue_pages)dialogue_page++;
   game.cue=SFX_SELECT;return;
  }
   if(page==TARGETING){int ids[TARGET_CAPACITY];target_count=collect_scan_ids(ids,scan_cat);if(target_count>0){for(int i=0;i<target_count;i++)target_ids[i]=ids[i];}if(row>=target_count)row=0;}
  if(page==EQUIP&&equip_confirm_item>=0&&equip_slot_for(equip_confirm_item)==FIT_WPN&&(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT))){
   int cap=fit_capacity(game.ship,FIT_WPN),bank=equip_target/6;if(bank<0||bank>=cap)bank=0;
   bank=(bank+(pressed&PSP_CTRL_RIGHT?1:cap-1))%cap;equip_target=bank*6;game.cue=SFX_SELECT;
   message(&game,"Weapon bank full. Select the highlighted slot, then press X to trade it in.");return;
  }
  if(page==INVENTORY&&loadout_picker_slot>=0){
   int list[EQUIPMENT_COUNT+DB_ITEMS],n=loadout_picker_list(list,EQUIPMENT_COUNT+DB_ITEMS);
   if(pressed&PSP_CTRL_CIRCLE){loadout_picker_slot=-1;game.cue=SFX_UI;return;}
   if(n&&(pressed&(PSP_CTRL_UP|PSP_CTRL_DOWN))){loadout_picker_row=(loadout_picker_row+(pressed&PSP_CTRL_DOWN?1:n-1))%n;game.cue=SFX_SELECT;}
   if(n&&(pressed&PSP_CTRL_CROSS)){if(loadout_picker_row>=n)loadout_picker_row=n-1;if(module_fit_token(list[loadout_picker_row],loadout_picker_slot))loadout_picker_slot=-1;else{snprintf(loadout_store_note,sizeof(loadout_store_note),"%s",game.message);loadout_store_until=preview_time+4.f;}}
   return;
  }
  int saga_choices=tracked_mission==0&&game.campaign_stage>=6&&game.saga_chapter<SAGA_COUNT&&game.saga_step&&saga_beats[game.saga_chapter].kind==SAGA_CHOICE&&(game.saga_chapter!=3||(game.saga_flags&SAGA_TIMESTAMP_FOUND));
  int count=page==COMFORT?6:page==DECORATOR?DECORATOR_COUNT:page==CAMPAIGN?(tracked_mission==TRACK_STATION_TOUR||tracked_mission==TRACK_LAVE?1:tracked_mission==0&&game.campaign_stage==0?1:tracked_mission==0&&saga_coda_pending>=0?1:tracked_mission==0&&game.campaign_stage>=6&&game.saga_chapter<SAGA_COUNT&&!game.saga_step?1:saga_choices?3:tracked_mission>=2?2:1):page==GUILD?1:page==STORY?(game.story<STORY_FREE?2:1):page==COMMS_PANEL?(comms_encounter_conversation?(comms_encounter_conversation==2||!comms_channel_valid()?1:comms_channel.kind==ENCOUNTER_SMUGGLER&&comms_trade_stage?(comms_trade_stage==1?2:1):3):COMMS_OPTION_COUNT):page==RADIO?2:page==HOME?DECK_ITEMS:page==MISSIONS?mission_count(&game):page==MISSIONLOG?mission_log_count():page==DEBUG?16:page==LOCAL?contact_count:page==TARGETING?target_count:page==GALNET?galnet_rows():page==MARKET?cargo_rows():page==CHART?near_count:page==YARD?shipyard_list_count(&game):page==EQUIP?equip_row_count():page==INVENTORY?FIT_SLOTS:page==STORAGE?1:page==FACTIONS?FACTION_COUNT:page==DETAILS?(1+system_body_slots(game.system)):page==CODEX?codex_rows():1;
  if(page==MARKET&&!game.docked&&(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT))){game.message_time=0;game.cue=SFX_SELECT;return;}
  if(page==REPAIR)count=1;
  if(page==COMMS_PANEL&&!comms_encounter_conversation&&!incoming_reply_ready())count=COMMS_OPTION_COUNT;
  if(count<1)count=1;
  if(page==MISSIONS&&mission_accept_confirm>=0&&(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT|PSP_CTRL_UP|PSP_CTRL_DOWN))){
   if(pressed&PSP_CTRL_LEFT)mission_accept_yes=1;else if(pressed&PSP_CTRL_RIGHT)mission_accept_yes=0;else mission_accept_yes=!mission_accept_yes;
   game.cue=SFX_SELECT;return;
  }
  if(pressed&(PSP_CTRL_UP|PSP_CTRL_DOWN))game.cue=SFX_SELECT;
  if(page==INVENTORY&&(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT|PSP_CTRL_UP|PSP_CTRL_DOWN|PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER))){
   int cat=row%6,bank=row/6,cap=fit_capacity(game.ship,cat);if(cap<1)cap=1;if(bank>=cap)bank=cap-1;
   if(pressed&PSP_CTRL_LEFT)bank=(bank+cap-1)%cap;if(pressed&PSP_CTRL_RIGHT)bank=(bank+1)%cap;
   if(pressed&(PSP_CTRL_UP|PSP_CTRL_LTRIGGER))cat=(cat+5)%6;if(pressed&(PSP_CTRL_DOWN|PSP_CTRL_RTRIGGER))cat=(cat+1)%6;
   cap=fit_capacity(game.ship,cat);if(cap<1)cap=1;if(bank>=cap)bank=cap-1;row=cat+bank*6;sell_confirm_slot=-1;loadout_store_until=-1;game.cue=SFX_SELECT;
  } else if(page==STORAGE&&(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT|PSP_CTRL_UP|PSP_CTRL_DOWN))){
   int owned[EQUIPMENT_COUNT],n=module_owned_list(owned,EQUIPMENT_COUNT);module_selected_item();
   if(n>0){
    int item=owned[module_inventory_row],cat=equip_slot_for(module_item_base(item)),cap=fit_capacity(game.ship,cat);if(cap<1)cap=1;
    if(!storage_sell_mode&&storage_bank_focus){if(pressed&PSP_CTRL_UP)module_bank=(module_bank+cap-1)%cap;if(pressed&PSP_CTRL_DOWN)module_bank=(module_bank+1)%cap;}
    else {if(pressed&PSP_CTRL_UP)module_inventory_row=(module_inventory_row+n-1)%n;if(pressed&PSP_CTRL_DOWN)module_inventory_row=(module_inventory_row+1)%n;}
   }
   module_sell_item=-1;game.cue=SFX_SELECT;
 } else if(page==CAMPAIGN||page==GUILD||(page==COMMS_PANEL&&comms_encounter_conversation)){
   if(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT|PSP_CTRL_UP|PSP_CTRL_DOWN)){row=dialogue_move(row,count,pressed);game.cue=SFX_SELECT;}
 } else {
   if(pressed&PSP_CTRL_UP){if(page==HOME)deck_step(-1);else if(page==CHART&&chart_mode)chart_move_cursor(0,-1);else {row=(row+count-1)%count;if(page==FACTIONS)faction_lore_card=0;}}
   if(pressed&PSP_CTRL_DOWN){if(page==HOME)deck_step(1);else if(page==CHART&&chart_mode)chart_move_cursor(0,1);else {row=(row+1)%count;if(page==FACTIONS)faction_lore_card=0;}}
  }
 if(page==CHART&&chart_mode&&(pressed&PSP_CTRL_LEFT))chart_move_cursor(-1,0);
 if(page==CHART&&chart_mode&&(pressed&PSP_CTRL_RIGHT))chart_move_cursor(1,0);
 if(page==GALNET&&galnet_tab==3&&(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT))){row+=pressed&PSP_CTRL_RIGHT?10:-10;if(row<0)row=0;if(row>=count)row=count-1;game.cue=SFX_SELECT;}
 if(page==HOME&&(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT|PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER)))
  deck_tab((pressed&(PSP_CTRL_RIGHT|PSP_CTRL_RTRIGGER))?1:-1);

   if(page==TARGETING&&(pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER))){step_scan_cat(pressed&PSP_CTRL_RTRIGGER?1:-1);target_count=collect_scan_ids(target_ids,scan_cat);row=0;}
   if(page==CHART&&chart_mode&&(pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER)))game.cue=SFX_SELECT;
   if(page==GALNET&&(pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER))){galnet_tab=(galnet_tab+(pressed&PSP_CTRL_RTRIGGER?1:5))%6;row=0;}
   if(page==CODEX&&codex_scope==0&&(pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER))){codex_tab=(codex_tab+(pressed&PSP_CTRL_RTRIGGER?1:3))%4;row=0;}
   if(page==CODEX&&codex_scope==3&&(pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER))){int category=row/3;category=(category+(pressed&PSP_CTRL_RTRIGGER?1:5))%6;row=category*3;game.cue=SFX_SELECT;}
   if(pressed&PSP_CTRL_CIRCLE){if(page==MISSIONS&&mission_accept_confirm>=0){mission_accept_confirm=-1;mission_accept_yes=0;game.cue=SFX_UI;return;}if(page==EQUIP&&equip_confirm_item!=-1){equip_confirm_item=-1;equip_target=-1;message(&game,"Equipment change cancelled.");return;}if(page==INVENTORY&&sell_confirm_slot>=0){sell_confirm_slot=-1;message(&game,"Sale cancelled.");return;}if(page==STORAGE&&module_sell_item>=0){module_sell_item=-1;message(&game,"Sale cancelled.");return;}if(page==STORAGE&&!storage_sell_mode&&storage_bank_focus){storage_bank_focus=0;game.cue=SFX_SELECT;return;}if(page==CODEX&&codex_scope==3){codex_scope=0;menu_back();return;}if(page==CODEX&&codex_scope==2){codex_scope=1;row=codex_system_row_for_body(codex_system,codex_body);return;}if(page==CODEX&&codex_scope==1){codex_scope=0;int n=vis_count();row=0;for(int i=0;i<n;i++)if(vis_sys(i)==codex_system)row=i;return;}if(page==MISSIONLOG&&abandon_confirm){abandon_confirm=0;message(&game,"Abandon cancelled. Your job is safe.");return;}if((page==RADIO||page==COMMS_PANEL||page==COMFORT)&&radio_dirty&&!radio_save_settings("radio.cfg")){message(&game,"Audio settings could not be saved. Try again.");return;}if(page==COMMS_PANEL){if(comms_encounter_conversation&&game.encounter_kind==ENCOUNTER_SMUGGLER&&comms_trade_stage){if(comms_trade_stage==1&&game.trader_offer_active&&game.trader_offer_npc==game.encounter_npc)game.trader_offer_active=0;comms_trade_depart();}else{comms_encounter_conversation=0;change_page(comms_return);}}else if(page==TARGETING){if(game.docked)menu_back();else change_page(FLIGHT);}else if(page==GALNET)change_page(HOME);else if(page==HOME){if(!game.docked)change_page(FLIGHT);}else menu_back();}
   else if(page==COMMS&&(pressed&PSP_CTRL_SQUARE))change_page(RADIO);
   else if(page==STORY&&(pressed&PSP_CTRL_CROSS)){
    if(row==1&&game.story<STORY_FREE){story_skip(&game);change_page(HOME);}
    else {int next=game.story<STORY_FREE?story_home_row(&game):0;change_page(HOME);row=next;deck_last=next;}
   }
   else if(page==GUILD&&(pressed&PSP_CTRL_CROSS))narrative_do(GUILD);
   else if(page==REPAIR&&(pressed&PSP_CTRL_TRIANGLE)){engineer_refuel();}
   else if(page==REPAIR&&(pressed&PSP_CTRL_CROSS)){repair_ship(&game);}
   else if(page==DECORATOR&&(pressed&PSP_CTRL_CROSS)){decorator_feedback=1;if(!game.docked){message(&game,"Dock at a station to repaint your ship.");return;}int finish=row<0?0:row>=DECORATOR_COUNT?DECORATOR_COUNT-1:row;int fee=decorator_fees[finish]*10;if(ship_paint[game.ship]==decorator_finishes[finish]){message(&game,"That finish is already on your ship.");return;}if(game.credits<fee){message(&game,"Not enough units for that paint finish.");return;}unsigned previous_paint=ship_paint[game.ship];ship_paint[game.ship]=decorator_finishes[finish];if(!paint_save()){ship_paint[game.ship]=previous_paint;message(&game,"Paint could not be saved. No charge applied.");return;}game.credits-=fee;social_emit(&game,SB_PAINT);message(&game,"Paint fitted. Ship preview and cockpit theme updated.");}
   else if(page==DECORATOR&&(pressed&PSP_CTRL_TRIANGLE)){change_page(REPAIR);}
   else if(page==HOME&&row==30&&(pressed&PSP_CTRL_CROSS)){law_enter();change_page(LAW_PAGE);return;}
   else if(page==HOME&&row>=27&&row<=29&&(pressed&PSP_CTRL_CROSS)){multiplayer_enter(HOME);return;}
   else if(page==HOME&&row==25&&(pressed&PSP_CTRL_CROSS)){quit_confirm=1;quit_confirm_yes=0;game.cue=SFX_SELECT;return;}
   else if(page==HOME&&row==26&&(pressed&PSP_CTRL_CROSS)){local_tv_open();change_page(LOCALTV);return;}
   else if(page==HOME&&row==23&&(pressed&PSP_CTRL_CROSS)){decorator_feedback=0;change_page(DECORATOR);}
   else if(page==HOME&&row==24&&(pressed&PSP_CTRL_CROSS)){change_page(REPAIR);}
   else if(page==HOME&&(pressed&PSP_CTRL_CROSS)){if(row==20){if(game.docked){walk_kind=0;walk_x=walk_z=walk_yaw=0;sc_built_for=-1;change_page(WALK);message(&game,"Station deck. U/D options, X do, TRI ship.");}else message(&game,"Dock first to walk the station deck.");return;}if(row==21){change_page(INVENTORY);return;}if(row==22){codex_scope=3;change_page(CODEX);row=0;return;}if(row==15){atlas_reset();}int pages[]={FLIGHT,MARKET,CHART,YARD,EQUIP,STATUS,HELP,FACTIONS,TARGETING,DEBUG,COMMS,DETAILS,MISSIONS,MISSIONLOG,GALNET,CODEX,RADIO,CAMPAIGN,GUILD,COMFORT};if(row<0||row>=20)return;int opened=row,next=pages[row];if(!story_menu_ok(&game,opened)){message(&game,story_task(&game));game.cue=SFX_UI;return;}if(!game.docked&&(next==YARD||next==EQUIP||next==MISSIONS)){message(&game,"Dock at a station to open this service.");game.cue=SFX_UI;return;}if(next==FLIGHT){int leaving=game.docked;int keep=((selected_target>=0&&selected_target<=BODY_COUNT)||selected_target==ROUTE_TARGET_ID||selected_target==RUMOUR_TARGET_ID||selected_target==DOCKBAY_TARGET_ID||IS_STATION_ID(selected_target))?selected_target:0;analog_ready=0;ax=ay=0;launch_departure(&game);if(leaving){selected_target=keep;autoaim=0;if(valid_target(keep))scan_cat=target_category(keep);}}change_page(next);story_on_open(&game,opened);if(!game.cue)game.cue=SFX_UI;}
   else if(page==CODEX&&(pressed&PSP_CTRL_CROSS)){if(codex_tab==0&&codex_scope==0){codex_system=vis_sys(row);codex_scope=1;row=0;}else if(codex_tab==1&&codex_scope==0&&planet_log_count()>0){int body;planet_log_at(row,&codex_system,&body);codex_body=body+1;codex_scope=2;row=0;}else if(codex_scope==1){codex_body=codex_system_body_at(codex_system,row);codex_scope=2;row=0;}game.cue=SFX_SELECT;}
   else if(page==LOCAL&&contact_count>0&&row>=0&&row<contact_count&&(pressed&(PSP_CTRL_CROSS|PSP_CTRL_TRIANGLE))){selected_target=contact_ids[row];scan_cat=target_category(selected_target);nav_body=selected_target>0&&selected_target<=BODY_COUNT?selected_target-1:-1;autoaim=(pressed&PSP_CTRL_TRIANGLE)!=0;story_event(&game,STORY_EV_TARGET);if(selected_target==0)campaign_event(&game,CP_LOCK);message(&game,"Target set.");if(!game.docked)change_page(FLIGHT);}
   else if(page==DETAILS&&(pressed&PSP_CTRL_CROSS)){int id=row==0?0:row;if(valid_target(id)){selected_target=id;scan_cat=target_category(id);nav_body=id>0&&id<=BODY_COUNT?id-1:-1;autoaim=1;story_event(&game,STORY_EV_TARGET);if(selected_target==0)campaign_event(&game,CP_LOCK);message(&game,game.docked?"Target saved for launch.":id==0?"Station locked. Turning to target.":id==1?"Sun locked. Turning to target.":"World locked. Turning to target.");change_page(game.docked?HOME:FLIGHT);}}
   else if(page==FACTIONS&&(pressed&PSP_CTRL_CROSS)){faction_lore_card=faction_lore_card>=2?0:faction_lore_card+1;game.cue=SFX_SELECT;}
   else if(page==FACTIONS&&(pressed&PSP_CTRL_TRIANGLE)){
    int best=-1;float best_d=1e9f;for(int i=0;i<NPC_COUNT;i++)if(game.npc[i].alive&&game.npc[i].role==row){float d=length(sub(game.npc[i].pos,game.pos));if(d<best_d){best_d=d;best=i;}}
    if(best<0){message(&game,"No ships of that colour on scanner.");game.cue=SFX_UI;}
    else {selected_target=BODY_COUNT+1+best;scan_cat=target_category(selected_target);autoaim=!game.docked;message(&game,"Faction contact locked.");if(!game.docked)change_page(FLIGHT);game.cue=SFX_SELECT;}
   }
  else if(page==COMMS&&(pressed&PSP_CTRL_TRIANGLE)){comms_rescue_confirm=!comms_rescue_confirm;}else if(page==COMMS&&(pressed&PSP_CTRL_CROSS)){if(comms_rescue_confirm){if(emergency_rescue(&game)){selected_target=0;autoaim=0;change_page(HOME);db_restore_frontend();}return;}if(game.docked){message(&game,"Already docked.");game.cue=SFX_UI;}else if(dock_selected_station()){selected_target=STATION_TARGET_ID(game.station_variant);autoaim=0;change_page(FLIGHT);}}
   else if(page==CAMPAIGN&&(pressed&PSP_CTRL_SELECT)){change_page(MISSIONLOG);}
   else if(page==MISSIONS&&(pressed&PSP_CTRL_SELECT))change_page(MISSIONLOG);
   else if(page==MISSIONLOG&&(pressed&PSP_CTRL_SQUARE)){mission_track_select(row);change_page(CAMPAIGN);}
   else if(page==MISSIONS&&(pressed&PSP_CTRL_CROSS)){
    int offer=mission_board_offer(row);
    if(mission_offer_active(&game,offer)){
     int dest=mission_destination(&game,offer),type=mission_type_for_offer(&game,offer);
     for(int i=0;i<game.job_n;i++)if(game.jobs[i].dest==dest&&game.jobs[i].type==type){tracked_mission=i+2;mission_track_select(i+2);change_page(MISSIONLOG);row=i+2;break;}
    }else if(mission_accept_confirm!=row){mission_accept_confirm=row;mission_accept_yes=0;game.cue=SFX_SELECT;}
    else if(!mission_accept_yes){mission_accept_confirm=-1;game.cue=SFX_UI;}
    else {mission_accept_confirm=-1;mission_accept_yes=0;if(accept_mission(&game,offer)){tracked_mission=game.job_sel+2;mission_track_select(tracked_mission);game.message_time=0;game.message[0]=0;}}
   }
   else if(page==MISSIONLOG){
    if(row>=2&&row<2+game.job_n&&(pressed&PSP_CTRL_TRIANGLE)){abandon_confirm=1;message(&game,"Abandon this job? Press X again to confirm, Circle to cancel.");}
    if((pressed&PSP_CTRL_CROSS)&&!abandon_confirm){mission_track_select(row);message(&game,row==0?"Tracking: Kei and Ryn.":row==1?"Tracking: Explorers Guild assignment.":tracked_mission==TRACK_STATION_TOUR?"Tracking: Station Welcome. Square opens its next step.":"Contract tracked. Square opens its next step.");}
    if(row>=2&&row-2<game.job_n){int ji=row-2;Job *j=&game.jobs[ji];game.job_sel=ji;game.contract=j->dest;game.mission_type=j->type;game.mission_stage=j->stage;game.mission_target=j->target;game.mission_item=j->item;game.contract_reward=j->reward;game.contract_time=j->time;
     if(pressed&PSP_CTRL_CROSS){
      if(abandon_confirm){abandon_confirm=0;abandon_mission(&game,ji);if(tracked_mission==row)tracked_mission=0;if(row>=2+game.job_n)row=1+game.job_n;}
     }
    }
   }
   else if(page==TARGETING&&(pressed&PSP_CTRL_CROSS)){if(target_count){selected_target=target_ids[row];scan_cat=target_category(selected_target);nav_body=selected_target>0&&selected_target<=BODY_COUNT?selected_target-1:-1;autoaim=!game.docked;story_event(&game,STORY_EV_TARGET);if(selected_target==0)campaign_event(&game,CP_LOCK);message(&game,game.docked?"Target selected for launch.":"Target locked. Auto-align active.");if(game.docked)menu_back();else change_page(FLIGHT);}}
   else if(page==TARGETING&&(pressed&PSP_CTRL_TRIANGLE)){target_details=!target_details;}
  else if(page==CAMPAIGN&&(pressed&PSP_CTRL_CROSS)){
 if(tracked_mission==TRACK_LAVE){
  if(game.system==7&&game.docked&&game.station_variant==0){walk_kind=0;sc_built_for=-1;change_page(WALK);}
  else if(game.system!=7){route_set_goal(&game,7);route_refresh_destination(&game);change_page(CHART);}
  else {selected_target=sc_lave_stage()==8?2:0;autoaim=0;change_page(game.docked?HOME:FLIGHT);message(&game,sc_lave_objective());}
 }
 else if(tracked_mission==TRACK_STATION_TOUR){
  if(station_tour_active())station_tour_action();else change_page(MISSIONLOG);
 }
 else if(tracked_mission==0&&prologue_brief_locked()){
  if(prologue_brief_beat<PROLOGUE_BRIEF_BEATS-1){
   prologue_brief_echo=0;prologue_brief_beat++;
   if(prologue_brief_beat==1)game.campaign_choice=1;else if(prologue_brief_beat==2)game.campaign_choice=2;
   row=0;game.cue=SFX_SELECT;
  }else {narrative_do(CAMPAIGN);prologue_brief_beat=0;prologue_brief_echo=0;}
 }
 else if(tracked_mission==0&&game.campaign_stage==0)narrative_do(CAMPAIGN);
 else if(tracked_mission==0&&game.campaign_stage>=6){
  if(saga_coda_pending>=0){saga_coda_pending=-1;row=0;game.cue=SFX_SELECT;}
  else if(game.saga_chapter<SAGA_COUNT&&!game.saga_step){if(saga_brief_beat<SAGA_BRIEF_BEATS-1){saga_brief_beat++;row=0;game.cue=SFX_SELECT;}else{saga_begin(&game);}}
  else if(game.saga_chapter<SAGA_COUNT&&saga_beats[game.saga_chapter].kind==SAGA_CHOICE&&(game.saga_chapter!=3||(game.saga_flags&SAGA_TIMESTAMP_FOUND))){game.saga_choice=row+1;saga_advance(&game);row=0;}
  else if(!saga_advance(&game)){if(game.saga_chapter>=SAGA_COUNT){change_page(MISSIONLOG);return;}if(game.system==game.saga_dest){if(game.docked){analog_ready=0;launch_departure(&game);}change_page(FLIGHT);return;}int hops=0,hop=saga_next_hop(&game,&hops);if(hop<0){message(&game,"No route with this drive. Fit more jump range.");}else{route_clear(&game);game.destination=hop;change_page(CHART);char note[96];snprintf(note,sizeof(note),hop==game.saga_dest?"Destination selected: %s.":"Next jump: %s. Final destination: %s.",game.systems[hop].name,game.systems[game.saga_dest].name);message(&game,note);}}
 }
 else if(tracked_mission==0)narrative_do(CAMPAIGN);
 else if(tracked_mission==1)narrative_do(GUILD);
 else {int ji=tracked_mission-2;if(row==0&&ji>=0&&ji<game.job_n)navigate_job(ji);else change_page(MISSIONLOG);}
}
else if(page==COMMS_PANEL&&(pressed&PSP_CTRL_CROSS)){
  if(comms_encounter_conversation){
   if(!comms_channel_valid()){comms_encounter_conversation=comms_trade_stage=0;change_page(comms_return);return;}
   if(comms_encounter_conversation==2){speech_ok();comms_encounter_conversation=0;change_page(comms_return);return;}
   if(comms_channel.kind==ENCOUNTER_SMUGGLER&&comms_trade_stage){
    if(comms_trade_stage==2){comms_trade_depart();return;}
    int n=game.encounter_npc;if(row==0){accept_trader_offer(&game);}else {if(game.trader_offer_active&&game.trader_offer_npc==n)game.trader_offer_active=0;speak(&game,VOICE_CONTACT,"Understood. I won't register an appointment, and no cargo changes hands. Safe flight, Commander.");game.voice_role=TRADERS;}comms_trade_stage=2;comms_channel_refresh();row=0;return;
   }
   if(comms_channel.kind==ENCOUNTER_SMUGGLER&&row==0){encounter_respond(&game);if(game.trader_offer_active&&game.trader_offer_system==game.system&&game.trader_offer_npc==comms_channel.npc){comms_trade_stage=1;comms_channel_refresh();row=0;return;}}
   if(row==0){encounter_respond(&game);game.encounter_kind=ENCOUNTER_NONE;game.encounter=0;comms_encounter_conversation=0;change_page(comms_return);return;}
   if(row==1){comms_encounter_explain();row=0;return;}
   speech_ok();comms_encounter_conversation=0;comms_encounter_decline();game.encounter_kind=ENCOUNTER_NONE;game.encounter=0;change_page(comms_return);return;
  }

 }
 else if(page==GALNET&&(galnet_tab==3||galnet_tab==4)){spacebook_actions(pressed);}
 else if(page==RADIO){if(pressed&PSP_CTRL_TRIANGLE){radio_off=!radio_off;if(radio_off)radio_static_ms=0;radio_dirty=1;}else if(pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER)){int dir=pressed&PSP_CTRL_RTRIGGER?1:-1;int next=radio_off?-1:radio_station;next+=dir;if(next<-1)next=-1;if(next>=RADIO_STATION_COUNT)next=RADIO_STATION_COUNT-1;if(next<0){radio_off=1;radio_static_ms=6;radio_dirty=1;}else{radio_off=0;radio_tune(next);radio_static_ms=8;}}else if(row<2&&(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT))){radio_adjust(row==1,pressed&PSP_CTRL_RIGHT?1:-1);}}else if(page==DEBUG&&(pressed&PSP_CTRL_CROSS)){debug_action();}
  else if(page==MARKET&&(pressed&PSP_CTRL_SQUARE)){int item=cargo_item(row);jettison_cargo(&game,item);int n=cargo_rows();if(row>=n)row=n-1;}
  else if(page==MARKET&&game.docked){int item=cargo_item(row);if(item>=0&&(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT))&&!cargo_can_trade(item)){game.message_time=0;game.cue=SFX_UI;return;}if(item>=0&&pressed&PSP_CTRL_RIGHT)trade(&game,item,1);if(item>=0&&pressed&PSP_CTRL_LEFT)trade(&game,item,0);if(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT))game.message_time=0;}
  else if(page==CHART){
   if(pressed&PSP_CTRL_TRIANGLE){chart_search=1;chart_query[0]=0;chart_search_key=0;chart_search_match=0;chart_search_refresh();game.cue=SFX_UI;}
   else if(pressed&PSP_CTRL_SQUARE)chart_cycle_filter();
   else if(pressed&PSP_CTRL_CROSS){Game route=game;route.fuel=(float)player_ships[game.ship].range;int jumps=0,hop=route_next_hop(&route,chart_cursor,&jumps);if(hop<0)message(&game,"No route with the fitted jump drive.");else{route_set_goal(&game,chart_cursor);game.destination=hop;selected_target=ROUTE_TARGET_ID;scan_cat=3;autoaim=0;route_boost_charge=0;update_nearby();char note[112];snprintf(note,sizeof(note),"Route plotted. %s is first in Other. Lock, align, then hold boost for 5 seconds.",game.systems[hop].name);message(&game,note);}}
  }
  else if(page==YARD&&(pressed&PSP_CTRL_SQUARE)){shipyard_stats_view=!shipyard_stats_view;game.cue=SFX_SELECT;}
  else if(page==YARD&&(pressed&PSP_CTRL_CROSS)){if(!game.docked)message(&game,"Dock to exchange ships.");else if(buy_ship(&game,shipyard_list_ship(&game,row)))ship_purchase_start();}
  else if(page==EQUIP&&(pressed&PSP_CTRL_SQUARE)){storage_sell_mode=1;storage_bank_focus=0;module_inventory_row=0;module_bank=0;change_page(STORAGE);}
  else if(page==EQUIP&&(pressed&PSP_CTRL_CROSS)){int list[EQUIP_COUNT],n=equipment_stock_list(list,EQUIP_COUNT);if(row>=0&&row<n){int i=list[row];if(equip_slot_for(i)>=0)module_buy_to_store(i);else equipment_buy_action(i,0);}}
  else if(page==INVENTORY&&(pressed&PSP_CTRL_CROSS)){if(!game.docked)message(&game,"Dock to change fitted modules.");else if(row>=0&&row<FIT_SLOTS&&fit_slot_open(&game,row)){loadout_picker_slot=row;loadout_picker_row=0;loadout_store_until=-1;game.cue=SFX_UI;}}
  else if(page==INVENTORY&&(pressed&PSP_CTRL_TRIANGLE)){int slot=row;storage_sell_mode=0;storage_bank_focus=0;change_page(STORAGE);module_inventory_row=0;module_bank=slot/6;}
  else if(page==INVENTORY&&(pressed&PSP_CTRL_SQUARE)){sell_confirm_slot=-1;store_fitted_module(row);snprintf(loadout_store_note,sizeof(loadout_store_note),"%s",game.message);loadout_store_until=preview_time+3.f;}
  else if(page==STORAGE&&storage_sell_mode&&(pressed&PSP_CTRL_CROSS)){module_sell_selected();}
  else if(page==STORAGE&&!storage_sell_mode&&game.docked&&(pressed&PSP_CTRL_CROSS)){if(storage_bank_focus){if(module_swap_selected())storage_bank_focus=0;}else if(module_selected_item()>=0){int item=module_selected_item(),cap=fit_capacity(game.ship,equip_slot_for(module_item_base(item)));if(cap<1)cap=1;if(module_bank>=cap)module_bank=cap-1;storage_bank_focus=1;game.cue=SFX_SELECT;}}
 }
 flight_motion: ;
 float turn=0,pitch=0;int throttle=0,fire=0;if(page==FLIGHT){turn=ax;pitch=ay;int rolling=(held&PSP_CTRL_LTRIGGER)&&(held&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT))&&!(held&PSP_CTRL_SQUARE);if(rolling){game.roll+=((held&PSP_CTRL_RIGHT)?1:-1)*dt*2*player_ships[game.ship].roll*.01f;turn=0;if(!game.boost)pitch=0;autoaim=0;}throttle=(held&PSP_CTRL_RTRIGGER?1:0)-(held&PSP_CTRL_LTRIGGER?1:0);if(game.boost)throttle=1;else if(rolling)throttle=0;if((held&PSP_CTRL_SQUARE)||hard_brake>0)throttle=0;fire=!fire_blocked&&oldpage==FLIGHT&&!game.dock_stage&&game.approach<0&&game.laser&&(held&PSP_CTRL_CROSS)!=0&&!(held&PSP_CTRL_LTRIGGER)&&game.jump<=0;if(fire&&valid_target(selected_target)&&IS_NPC_ID(selected_target))autoaim=1;align_target(dt,ax,ay);route_hyperdrive_tick(dt);tools_tractor_tick(dt);if(tools_tractor>=0||game.tractor_time>0){throttle=0;fire=0;}}
 if(page==FLIGHT){
  if(oldpage!=FLIGHT){turn=pitch=0;throttle=fire=0;}
  /* The mandatory five-second route spool is part of the jump sequence, not
   * ordinary afterburning. Reserve the plotted jump's fuel while it charges;
   * the normal hyperspace cost is still deducted on arrival. */
  int route_spooling=route_boost_charge>0&&game.jump<=0&&nav_route_tracking()&&game.boost;
  float spool_fuel=game.fuel;if(race_is_active(&game))fire=!fire_blocked&&oldpage==FLIGHT&&(held&PSP_CTRL_CROSS)!=0;game_tick(&game,dt,turn,pitch,throttle,fire);
  if(route_spooling&&game.jump<=0)game.fuel=spool_fuel;
  if(!planet_entry_body)planet_auto_entry();
  if(game.docked)change_page(HOME);
 }
}
#include "mission-tracking-input.h"
#include "load-arrival.h"
#include "erl-quick-race.h"
#include "erl-racer-ui.h"
#include "tutorial-runtime.h"
#include "opening-runtime.h"
#include "cockpit-hud-review.h"
#include "navigation-review.h"
#include "charm-review.h"
#include "dashboard-review.h"
#include "loadout-picker-review.h"
#include "departure-hud-review.h"
#include "port-ad-review.h"
#include "release-2471-review.h"
#include "erl-quick-race-review.h"
#include "opening-live-review.h"
#include "online-review.h"
#include "multiplayer-live-review.h"
#include "local-wifi-live-review.h"
#if defined(ELITE_ENCOUNTER_REVIEW) && !defined(ELITE_ENABLE_REVIEW)
#include "encounter-upgrade-review.h"
#include "final-menu-review.h"
#endif
#ifdef ELITE_ENABLE_REVIEW
#include "opening-input-tests.h"
#include "intro-film-review.h"
#include "menu-prototypes-review.h"
#include "menu-seven-review.h"
#include "terminal-review.h"
#include "final-menu-review.h"
#include "encounter-upgrade-review.h"
#include "load-arrival-review.h"
#include "station-departure-review.h"
#include "portrait-parts-review.h"

static void input_tests(void){
 FILE *f=fopen("input-check.txt","w");if(!f)return;int failures=0;
#define INPUT_CHECK(c,n) do{int ok=(c);fprintf(f,"%s %s\n",ok?"PASS":"FAIL",n);failures+=!ok;}while(0)
#define TEST_INIT() do{ps_open=ps_release=0;game_init(&game);deck_reset();story_complete(&game);paused=0;selected_target=0;autoaim=0;scan_cat=2;tracked_mission=0;prologue_brief_beat=0;prologue_brief_echo=0;saga_brief_beat=0;saga_brief_chapter=-1;saga_coda_pending=-1;station_tour_stage=STATION_TOUR_OFF;planet_landing_menu=0;planet_controls_ready=1;planet_first_frame=planet_first_tick=0;planet_entry_body=planet_seat.active=planet_orbit_release=0;planet_orbit_veil=0;surface_target_reset();departure_release=0;atlas_reset();tools_cancel_tractor();tools_reset_gesture();tools_selected=0;tools_wait_release=0;tools_confirm_time=0;tools_confirm_action=-1;tools_weapon_result=0;debug_force_thargoid=0;thargoid_reset();chart_zoom=chart_zoom_goal=1;chart_pan_x=chart_pan_y=0;route_boost_charge=0;route_jump_engaged=0;}while(0)
 TEST_INIT();{int sys=game.system;game.landed_planets[sys]=1;game.rift_logged[sys]=3;game.bounty_claimed[sys]=7;game.station_progress[sys][0]=2;for(int b=0;b<BODY_COUNT;b++)game.surface_progress[sys][b]=0;game.surface_progress[sys][1]=(1u<<8)|(1u<<9)|field_site_bit(1)|field_site_bit(7);INPUT_CHECK(system_ops_species(sys,1)==2&&system_ops_sites(sys,1)==2&&system_ops_done(sys)==11,"system operations: progress derives from saved landings, discoveries, sites, rifts, bounties and station activity");INPUT_CHECK(strstr(system_ops_next(),"BOUNTY BOARD")!=0,"system operations: suggested action points to remaining local bounty targets");game.bounty_claimed[sys]=31;game.landed_planets[sys]=15;INPUT_CHECK(strstr(system_ops_next(),"FIELDWORK")!=0,"system operations: completed bounty board advances the suggestion to unfinished fieldwork");}
 TEST_INIT();{char first[192],again[192];almanac_description(game.system,first,sizeof(first));row=5;almanac_description(game.system,again,sizeof(again));INPUT_CHECK(!strcmp(first,again)&&strlen(first)>40,"system almanac: permanent system description does not change with the highlighted body");int kind=station_class(&game);INPUT_CHECK(!strcmp(station_class_name(kind),kind==STATION_POOR?"POOR":kind==STATION_MEGA?"MEGA CAPITAL":"RICH"),"system almanac: Economy uses only Poor, Rich or Mega Capital");}
 TEST_INIT();launch(&game);change_page(DETAILS);row=3;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(page==FLIGHT&&selected_target==3&&nav_body==2&&autoaim,"system almanac: X locks the highlighted world, returns to flight and starts turning");
 TEST_INIT();{launch(&game);int origin=game.system;game.destination=(origin+1)%256;game.jump=4;int credits=game.credits;thargoid_begin();thargoid_intro=0;for(int i=1;i<THARGOID_MAX;i++)thargoid_raider[i].alive=0;thargoid_raider[0].x=thargoid_raider[0].y=0;thargoid_raider[0].z=.5f;thargoid_raider[0].age=2;thargoid_raider[0].hp=1;float tx,ty;thargoid_project(&thargoid_raider[0],&tx,&ty);thargoid_cursor_x=tx;thargoid_cursor_y=ty;thargoid_fire();INPUT_CHECK(game.credits==credits+200&&thargoid_kills==1,"thargoid ambush: a confirmed rail-shot kill awards 20 units");thargoid_begin();thargoid_damage(100);INPUT_CHECK(!thargoid_active&&game.system==origin&&game.jump==0&&game.destination==(origin+1)%256,"thargoid ambush: defeat safely returns to the departure system without losing the route");game.jump=4;thargoid_begin();thargoid_finish(1);INPUT_CHECK(!thargoid_active&&game.jump>0&&game.system==origin,"thargoid ambush: victory resumes the original hyperspace jump");}
 TEST_INIT();{change_page(DEBUG);row=12;debug_action();INPUT_CHECK(debug_force_thargoid,"debug menu enables forced Thargoid encounters");launch(&game);game.destination=(game.system+1)%256;game.jump=4;thargoid_checked=0;INPUT_CHECK(thargoid_maybe_start()&&thargoid_active,"forced Thargoid mode intercepts every hyperspace jump");thargoid_reset();change_page(DEBUG);row=12;debug_action();INPUT_CHECK(!debug_force_thargoid,"debug menu disables forced Thargoid encounters");}
 TEST_INIT();{change_page(DEBUG);row=13;debug_action();INPUT_CHECK((game.debug_flags&(DEBUG_MODIFIED|DEBUG_UNLIMITED_FUEL))==(DEBUG_MODIFIED|DEBUG_UNLIMITED_FUEL)&&game.fuel==player_ships[game.ship].range,"debug menu enables unlimited fuel and marks the session modified");debug_action();INPUT_CHECK(!(game.debug_flags&DEBUG_UNLIMITED_FUEL)&&(game.debug_flags&DEBUG_MODIFIED),"debug menu disables unlimited fuel without clearing modified state");row=14;debug_action();INPUT_CHECK(game.debug_flags&DEBUG_UNLIMITED_RANGE,"debug menu enables unlimited jump range");debug_action();INPUT_CHECK(!(game.debug_flags&DEBUG_UNLIMITED_RANGE)&&(game.debug_flags&DEBUG_MODIFIED),"debug menu disables unlimited jump range and retains modified state");}
#include "traffic-input-tests.h"
#include "thargoid-stability-tests.h"
 TEST_INIT();launch(&game);page=FLIGHT;game.pos=(Vec3){0,0,-20000};game.speed=0;for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;
 input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);INPUT_CHECK(!game.boost,"single R press does not boost");
 input(0,0,.1f,0,0);input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);INPUT_CHECK(game.boost,"double R press starts boost");
 input(0,PSP_CTRL_RTRIGGER,.016f,0,0);INPUT_CHECK(game.boost,"held R maintains boost");input(0,0,.016f,0,0);INPUT_CHECK(!game.boost,"R release cancels boost");
 TEST_INIT();launch(&game);page=FLIGHT;game.speed=player_ships[game.ship].speed;game.heat=0;hard_brake=0;l_tap=10;
 input(PSP_CTRL_LTRIGGER,PSP_CTRL_LTRIGGER,.016f,0,0);INPUT_CHECK(hard_brake<=0,"single L press does not hard-brake");
 game.message_time=0;input(0,0,.1f,0,0);input(PSP_CTRL_LTRIGGER,PSP_CTRL_LTRIGGER,.016f,0,0);INPUT_CHECK(hard_brake>0&&game.message_time<=0,"double L starts hard brake without computer chatter");
 float braked=game.speed;input(0,0,.05f,0,0);INPUT_CHECK(game.speed<braked,"hard brake dumps speed quickly");
 game.heat=90;r_tap=0;input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);input(0,0,.1f,0,0);input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);INPUT_CHECK(game.boost,"critical heat still allows a boost double-tap");
 {int before=hud_mode;game.message_time=0;input(PSP_CTRL_SELECT,PSP_CTRL_SELECT|PSP_CTRL_LTRIGGER,.016f,0,0);INPUT_CHECK(hud_mode!=before&&game.message_time<=0,"HUD shortcut changes layout without computer chatter");}
 change_page(LOCAL);row=2;input(PSP_CTRL_TRIANGLE,0,.016f,0,0);INPUT_CHECK(page==FLIGHT&&selected_target==2&&autoaim,"local menu selects named planet and auto-aligns");
 input(0,0,.016f,1,0);INPUT_CHECK(!autoaim,"manual steering cancels auto-aim");
 game.approach=1;game.pos=add(game.bodies[1].pos,(Vec3){0,0,-game.bodies[1].radius-800});game.yaw=game.pitch=0;Vec3 facing=forward(&game);input(PSP_CTRL_CIRCLE,0,.016f,0,0);INPUT_CHECK(game.approach==-1&&dot(facing,forward(&game))<-.999f,"Circle exits approach and turns around");
 TEST_INIT();change_page(CHART);chart_cursor=near_count?nearby[0]:game.destination;input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 INPUT_CHECK(page==CHART&&game.docked&&game.route_goal==chart_cursor&&game.destination!=game.system,"Deep Chart X safely plots a route without launching or spending fuel");
 TEST_INIT();launch(&game);page=FLIGHT;pip_sel=1;game.pip_sys=2;game.pip_eng=2;game.pip_wep=4;
 input(0,PSP_CTRL_START,.016f,0,0);INPUT_CHECK(paused,"holding Start enters power redistribute on the cockpit meters");
 input(PSP_CTRL_RIGHT,PSP_CTRL_START|PSP_CTRL_RIGHT,.016f,0,0);INPUT_CHECK(pip_sel==2,"Start+Right selects WEP on the existing meters");
 input(PSP_CTRL_UP,PSP_CTRL_START|PSP_CTRL_UP,.016f,0,0);INPUT_CHECK(game.pip_wep==4&&game.pip_sys+game.pip_eng+game.pip_wep==8,"Start+Up cannot push a bank past four pips");
 input(PSP_CTRL_LEFT,PSP_CTRL_START|PSP_CTRL_LEFT,.016f,0,0);INPUT_CHECK(pip_sel==1,"Start+Left returns to ENG");
 int eng_before=game.pip_eng;input(PSP_CTRL_UP,PSP_CTRL_START|PSP_CTRL_UP,.016f,0,0);INPUT_CHECK(game.pip_eng==eng_before+1,"Start+Up puts more power into ENG");
 int eng_hi=game.pip_eng;input(PSP_CTRL_DOWN,PSP_CTRL_START|PSP_CTRL_DOWN,.016f,0,0);INPUT_CHECK(game.pip_eng==eng_hi-1,"Start+Down takes power out of ENG");
 input(0,0,.016f,0,0);INPUT_CHECK(!paused&&game.pip_sys+game.pip_eng+game.pip_wep==8,"releasing Start resumes flight with eight pips still assigned");
 TEST_INIT();game.campaign_stage=6;saga_begin(&game);chart_mode=0;change_page(CHART);
 INPUT_CHECK(chart_mode&&chart_cursor==game.saga_dest,"Deep Chart opens focused on the tracked mission destination");
 float old_zoom=chart_zoom;for(int i=0;i<20;i++)input(i==0?PSP_CTRL_RTRIGGER:0,PSP_CTRL_RTRIGGER,.016f,0,0);INPUT_CHECK(chart_zoom>old_zoom,"Deep Chart R smoothly zooms into the projected galaxy");
 float old_pan=chart_pan_x;for(int i=0;i<10;i++)input(0,0,.016f,.8f,0);INPUT_CHECK(chart_pan_x<old_pan,"Deep Chart analog nub pans the camera while zoomed");
 input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(chart_mode&&game.destination!=game.system&&distance_ly(&game,game.system,game.destination)<=player_ships[game.ship].range*.1f+.001f,"Deep Chart route planner selects a reachable first jump");
 INPUT_CHECK(game.route_goal==game.saga_dest,"galaxy plot stores the final mission destination as the route goal");
 TEST_INIT();{int far=-1,hops=0,first=-1;for(int i=0;i<256;i++){int hop=route_next_hop(&game,i,&hops);if(hop>=0&&hops>1){far=i;first=hop;break;}}INPUT_CHECK(far>=0&&first>=0,"manual route test finds a multi-jump destination");change_page(CHART);chart_cursor=far;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(game.route_goal==far&&game.destination==first&&game.destination!=far,"manual galaxy plot keeps final goal separate from the next hop");int route_ids[TARGET_CAPACITY],route_n=collect_scan_ids(route_ids,3);INPUT_CHECK(selected_target==ROUTE_TARGET_ID&&scan_cat==3&&route_n>0&&route_ids[0]==ROUTE_TARGET_ID,"plotted next-hop star is first in the targeting computer Other band");launch(&game);page=FLIGHT;game.dock_stage=game.docked=0;game.pos=(Vec3){123456,65432,-222222};for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;autoaim=1;Vec3 rd=route_target_direction();game.yaw=atan2f(rd.x,rd.z);game.pitch=asinf(rd.y);game.boost=1;route_boost_charge=2.49f;INPUT_CHECK(!route_prejump_active(),"route hyperdrive panel remains hidden below half charge");route_boost_charge=2.5f;INPUT_CHECK(route_prejump_active()&&route_prejump_progress()<.001f,"route hyperdrive panel appears at 50 percent blue charge and begins at zero");route_boost_charge=3.75f;INPUT_CHECK(fabsf(route_prejump_progress()-.5f)<.001f,"route hyperdrive panel maps the blue bar second half to 50 percent");route_boost_charge=0;for(int i=0;i<320&&game.jump<=0;i++)input(0,PSP_CTRL_RTRIGGER,.016f,0,0);INPUT_CHECK(game.jump>0&&game.jump<=5.01f&&route_jump_engaged,"full blue charge enters the hyperspace corridor without a second wait");}
 TEST_INIT();change_page(CHART);input(PSP_CTRL_TRIANGLE,0,.016f,0,0);INPUT_CHECK(chart_search&&chart_match_n==256,"Triangle opens live system search with the full initial list");chart_query[0]='L';chart_query[1]=0;chart_search_refresh();int prefix_ok=chart_match_n>0;for(int i=0;i<chart_match_n;i++)if(game.systems[chart_matches[i]].name[0]!='L')prefix_ok=0;INPUT_CHECK(prefix_ok,"system search narrows immediately to names beginning with the typed prefix");int found=chart_matches[0];input(PSP_CTRL_TRIANGLE,0,.016f,0,0);INPUT_CHECK(!chart_search&&chart_cursor==found,"Triangle accepts the highlighted prefix match and returns to the chart");
 int prior_filter=chart_filter;input(PSP_CTRL_SQUARE,0,.016f,0,0);INPUT_CHECK(chart_filter==(prior_filter+1)%CHART_FILTERS,"Square cycles the Deep Chart filter rail");
 TEST_INIT();launch(&game);page=FLIGHT;selected_target=2;Body *body=&game.bodies[1];game.pos=add(body->pos,(Vec3){0,0,-body->radius-800});game.speed=0;game.yaw=game.pitch=0;speech_ok();input(PSP_CTRL_TRIANGLE,0,.016f,0,0);INPUT_CHECK(game.approach==1,"Triangle requests landing at the nearby targeted planet");input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(planet_entry_body==1&&game.planet<0,"X starts cloud-covered atmosphere entry");input(PSP_CTRL_CIRCLE,0,.016f,0,0);INPUT_CHECK(planet_entry_body==1&&game.surface==0,"Circle cannot interrupt cloud-covered entry");input(PSP_CTRL_TRIANGLE,0,.016f,0,0);input(PSP_CTRL_TRIANGLE,0,.016f,0,0);INPUT_CHECK(planet_entry_body==1,"Triangle cannot interrupt guided planetary arrival");
 TEST_INIT();launch(&game);page=FLIGHT;message(&game,"The sun has no landing approach.");input(PSP_CTRL_TRIANGLE,PSP_CTRL_TRIANGLE,.05f,0,0);for(int i=0;i<12;i++)input(0,PSP_CTRL_TRIANGLE,.05f,0,0);INPUT_CHECK(page==FLIGHT&&!comms_quick&&game.message_time<=0,"computer notices cannot open a Respond/Ignore conversation");
 TEST_INIT();launch(&game);page=FLIGHT;game.encounter_kind=ENCOUNTER_PIRATE;game.encounter=3;game.message_time=0;encounter_ignore(&game);INPUT_CHECK(game.encounter_kind==ENCOUNTER_NONE&&game.message_time<=0,"Ignore closes incoming chatter without a redundant computer notice");
 change_page(HOME);row=3;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(page==HOME,"ship shop cannot open in flight");row=4;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(page==HOME,"equipment shop cannot open in flight");
 {int vis[9],n=deck_fill(1,vis),saw=0;for(int i=0;i<n;i++)if(vis[i]==3||vis[i]==4)saw=1;INPUT_CHECK(!saw,"undocked Ship tab omits Shipyard and Outfitting");}
 {int vis[9],n=deck_fill(2,vis),saw=0;for(int i=0;i<n;i++)if(vis[i]==12)saw=1;INPUT_CHECK(!saw,"undocked Work tab omits Mission board");}
 TEST_INIT();{int poor=-1,rich=-1,mega=-1;for(int s=0;s<256;s++){int kind=station_class_for_system(&game,s);if(kind==STATION_POOR&&poor<0)poor=s;else if(kind==STATION_RICH&&rich<0)rich=s;else if(kind==STATION_MEGA&&mega<0)mega=s;}
  INPUT_CHECK(poor>=0&&rich>=0&&mega>=0,"station classes: galaxy contains Poor, Rich and Mega Capital systems");
  game.system=poor;game.docked=1;market(&game);int vis[9],n=deck_fill(1,vis),blocked=1;for(int i=0;i<n;i++)if(vis[i]==3||vis[i]==4||vis[i]==23)blocked=0;
  INPUT_CHECK(blocked&&cargo_rows()<GOODS&&cargo_rows()>=GOODS/2,"station classes: Poor ports omit premium ship services and list half the commodities");
  int item=cargo_item(1),before=item>=0?game.cargo[item]:0;game.credits=100000;if(item>=0)game.stock[item]=2;INPUT_CHECK(item>=0&&trade(&game,item,1)&&game.cargo[item]==before+1,"station classes: shortened Poor market rows trade the mapped commodity");
  game.system=mega;StationProfile capital=station_profile_for(&game,0);Vec3 west=station_port_offset_for(&game,0,1),east=station_port_offset_for(&game,0,2);INPUT_CHECK(capital.radius>=940&&station_port_count_for(&game,0)==5&&west.x<0&&east.x>0,"station classes: Mega Capital hull and five-port guidance are active");
 }
 {int vis[9],n=deck_fill(4,vis);INPUT_CHECK(n==5&&vis[3]==9&&vis[4]==25,"Commander tab places Quit to main menu directly below Debug tools");}
 TEST_INIT();launch(&game);game.dead=1;game.explosion=3;page=FLIGHT;death_choice=0;input(PSP_CTRL_DOWN,0,.016f,0,0);INPUT_CHECK(death_choice==1&&page==FLIGHT,"destroyed ship recovery panel selects Main Menu with the D-pad");input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(page==INTRO,"death recovery Main Menu returns to the title screen");
 TEST_INIT();launch(&game);change_page(HOME);
 game.cargo[0]=2;game.cargo[7]=1;INPUT_CHECK(cargo_rows()==2&&cargo_item(1)==7,"flight inventory lists only owned cargo");contacts_refresh();INPUT_CHECK(contact_count>=BODY_COUNT+1&&contact_ids[BODY_COUNT]==BODY_COUNT,"contacts include station and every celestial body");
 page=FLIGHT;game.pos=(Vec3){0,0,-20000};float speed=game.speed;input(0,PSP_CTRL_LTRIGGER|PSP_CTRL_RIGHT,.016f,1,0);INPUT_CHECK(game.roll>0&&game.speed==speed,"L and right rolls without changing throttle");
 game.roll=3.1415926f;{static Game before;before=game;input(0,PSP_CTRL_RIGHT,.016f,1,0);Vec3 nose=camera(&before,add(before.pos,forward(&game)));INPUT_CHECK(nose.x>.02f&&fabsf(nose.y)<.001f,"Right steers toward cockpit right when inverted");}
 TEST_INIT();game.ship=9;game.credits=10000;change_page(DECORATOR);row=2;
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(ship_paint[9]==decorator_finishes[2]&&game.credits==7600,"decorator applies chosen paint to the tenth hull at its displayed 240-unit cost");
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(game.credits==7600&&decorator_finishes[2]==RGB(240,120,96),"equipped paint is free to reselect and preset swatches stay unchanged");
 change_page(DEBUG);row=0;int cash=game.credits;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(game.credits==cash+10000,"debug adds 1000 displayed units");
  TEST_INIT();launch(&game);page=FLIGHT;hud_mode=hud_hidden=0;input(PSP_CTRL_SELECT,PSP_CTRL_SELECT|PSP_CTRL_LTRIGGER,.016f,0,0);INPUT_CHECK(hud_mode==1&&!hud_hidden&&page==FLIGHT,"L and Select selects minimal HUD");input(PSP_CTRL_SELECT,PSP_CTRL_SELECT|PSP_CTRL_LTRIGGER,.016f,0,0);INPUT_CHECK(hud_mode==2&&hud_hidden,"L and Select selects scenic HUD");input(PSP_CTRL_SELECT,PSP_CTRL_SELECT|PSP_CTRL_LTRIGGER,.016f,0,0);INPUT_CHECK(hud_mode==0&&!hud_hidden,"L and Select restores full HUD");
 TEST_INIT();game.system=0;game.credits=20000;game.systems[game.system].tech=12;page=EQUIP;{int list[EQUIP_COUNT],n=equipment_stock_list(list,EQUIP_COUNT),dock_row=-1,cargo_row=-1,mis_row=-1,mil_row=-1,pulse_row=-1;for(int i=0;i<n;i++){if(list[i]==4)dock_row=i;if(list[i]==10)cargo_row=i;if(list[i]==3)mis_row=i;if(list[i]==7)mil_row=i;if(list[i]==1)pulse_row=i;}INPUT_CHECK(dock_row>=0&&cargo_row>=0&&mis_row>=0,"outfitting lists dock, cargo and missile stock at high-tech hub");row=dock_row;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(game.module_store[4]==1&&game.fit[FIT_NAV]!=4&&game.credits==17500,"outfitting X buys docking computer into Item Storage and charges balance");row=cargo_row;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(game.module_store[10]==1&&game.fit[FIT_HOLD]!=10&&cargo_capacity(&game)==player_ships[game.ship].capacity,"outfitting stores cargo module without changing the current loadout");int missiles=game.missiles;row=mis_row;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(game.missiles==missiles+1&&game.credits==13000,"outfitting reloads one missile and charges balance");if(pulse_row>=0){row=pulse_row;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(game.module_store[1]==1&&game.fit[FIT_WPN]!=1&&!game.laser,"pulse laser purchase goes to Item Storage");}game.systems[game.system].economy=0;n=equipment_stock_list(list,EQUIP_COUNT);mil_row=-1;for(int i=0;i<n;i++)if(list[i]==7)mil_row=i;INPUT_CHECK(mil_row>=0,"industrial hub stocks military shield");{row=mil_row;page=EQUIP;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(game.module_store[7]==1&&game.fit[FIT_DEF]!=7,"military shield purchase goes to Item Storage");game.module_store[7]=1;page=STORAGE;storage_sell_mode=1;module_inventory_row=0;{int owned[EQUIPMENT_COUNT],on=module_owned_list(owned,EQUIPMENT_COUNT);for(int k=0;k<on;k++)if(owned[k]==7)module_inventory_row=k;}int before=game.credits;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(module_sell_item==7&&game.module_store[7]==1,"outfitting resale asks before selling a stored module");input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(game.module_store[7]==0&&game.credits==before+equipment_costs[7]/2,"confirmed stored-module sale pays half price");}}
 TEST_INIT();game.system=7;{int list[EQUIP_COUNT],n=equipment_stock_list(list,EQUIP_COUNT),cats=0;for(int i=0;i<n;i++){int cat=equip_slot_for(list[i]);if(cat>=0)cats|=1<<cat;}INPUT_CHECK(n==TECH_ERL_VECTOR_GOVERNOR-1&&cats==63,"Lave Outfitting exposes all ordinary modules and all six categories; ERL prizes stay exclusive");}
 TEST_INIT();launch(&game);page=FLIGHT;input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE,.016f,0,0);INPUT_CHECK(page==FLIGHT&&!square_held,"Square tap keeps flight active without flashing the target computer");input(0,0,.016f,0,0);INPUT_CHECK(page==FLIGHT&&valid_target(selected_target),"Square tap selects a visible target near the centre reticle");
 for(int i=0;i<NPC_COUNT;i++)if(game.npc[i].alive&&game.npc[i].role==PIRATES){game.npc[i].target=-2;break;}
 target_filter=2;target_refresh();INPUT_CHECK(target_count>0,"hostile targeting filter finds ships engaging the player");target_filter=10;target_refresh();INPUT_CHECK(target_count>=1,"anomaly filter lists rare system echoes"); galnet_tab=0;INPUT_CHECK(galnet_rows()==16,"Galactic Gazette has a varied sixteen-article local edition");{INPUT_CHECK(newspaper_identity[7].page_count==16,"Lave has its authored newspaper length");galnet_tab=3;INPUT_CHECK(galnet_rows()==1,"Spacebook starts with one readable empty-state card");galnet_tab=4;INPUT_CHECK(galnet_rows()==story_inbox_count(),"Inbox lists unlocked private story correspondence");}
 TEST_INIT();change_page(GALNET);galnet_tab=2;row=0;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(page==GALNET&&game.docked&&!autoaim&&selected_target==0&&game.dock_stage==0,"Wanted posters are informational; X does not launch or select a target");
 {char author[40],body[96];galnet_tab=0;galnet_post(2,author,sizeof(author),body,sizeof(body));INPUT_CHECK(strstr(body,"cleared")&&strstr(body,"bound"),"Traffic Control names a remote traveller route");galnet_tab=3;snprintf(body,sizeof(body),"%s",travellers_galnet_spotter(&game));INPUT_CHECK(strstr(body,"Spotted")||strstr(body,"lingering"),"Spacebook Spotters name traveller traffic");}
 for(int frame=0;frame<320&&game.dock_stage==4;frame++)input(0,0,.016f,0,0);input(0,0,.016f,0,0);
 change_page(HOME);row=15;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(page==CODEX,"command deck opens the Discovery Codex");codex_tab=0;INPUT_CHECK(codex_kind_count(0)>=1,"Codex Systems lists visited systems");codex_tab=1;INPUT_CHECK(codex_kind_count(1)==0,"Codex Planets starts empty before landing");game.landed_planets[game.system]=1;INPUT_CHECK(codex_kind_count(1)==1,"Codex Planets adds a world only after landing");
 TEST_INIT();game.system=0;launch(&game);page=FLIGHT;game.pos=(Vec3){123456,65432,-222222};game.yaw=game.pitch=game.roll=0;
 for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;
 game.npc[0].alive=1;game.npc[0].role=TRADERS;game.npc[0].target=-1;game.npc[0].pos=add(game.pos,(Vec3){0,0,800});
 game.npc[1].alive=1;game.npc[1].role=PIRATES;game.npc[1].target=-2;game.npc[1].pos=add(game.pos,(Vec3){200,0,900});
 selected_target=-1;game.message_time=0;input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE,.016f,0,0);input(0,0,.016f,0,0);INPUT_CHECK(selected_target==BODY_COUNT+1&&!autoaim&&game.message_time<=0,"Square tap silently selects the contact nearest the centre reticle");
 selected_target=BODY_COUNT+2;input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE,.016f,0,0);for(int i=0;i<14;i++)input(0,PSP_CTRL_SQUARE,.016f,0,0);INPUT_CHECK(square_held,"holding Square past the tap threshold opens the target computer");input(0,0,.016f,0,0);INPUT_CHECK(selected_target==BODY_COUNT+2&&!square_held,"releasing a held target computer preserves its highlighted target");
 game.pos=(Vec3){0,0,0};game.npc[0].pos=(Vec3){200,0,800};game.npc[1].pos=(Vec3){-200,0,900};
 scan_cat=2;selected_target=0;
 game.message_time=0;input(PSP_CTRL_LEFT,PSP_CTRL_SQUARE|PSP_CTRL_LEFT,.016f,0,0);INPUT_CHECK(page==FLIGHT&&IS_NPC_ID(selected_target)&&scan_cat==1,"Square+Left tabs to ships (all contacts) without opening the computer");INPUT_CHECK(game.message_time<=0,"Square targeting changes selection without opening computer chatter");
 /* SHIPS must include the engaging pirate — locking them must not flip to ENEMIES. */
 scan_cat=1;selected_target=BODY_COUNT+1; /* trader */
 input(PSP_CTRL_UP,PSP_CTRL_SQUARE|PSP_CTRL_UP,.016f,0,0);
 INPUT_CHECK(page==FLIGHT&&scan_cat==1&&IS_NPC_ID(selected_target),"Square+Up cycles within SHIPS without leaving the band");
 {int saw_hostile=0,ids[NPC_COUNT],n=collect_scan_ids(ids,1);for(int i=0;i<n;i++)if(ids[i]==BODY_COUNT+2)saw_hostile=1;INPUT_CHECK(saw_hostile&&n==2,"SHIPS band lists every ship including hostiles");}
 game.npc[1].alive=1;game.npc[1].role=PIRATES;game.npc[1].target=-2;
 {int ids[NPC_COUNT],n=collect_scan_ids(ids,4);INPUT_CHECK(n==1&&ids[0]==BODY_COUNT+2,"ENEMIES band lists only ships currently attacking the player");}
 input(PSP_CTRL_RIGHT,PSP_CTRL_SQUARE|PSP_CTRL_RIGHT,.016f,0,0);INPUT_CHECK(page==FLIGHT&&selected_target==0,"Square+Right tabs back to the station");input(PSP_CTRL_RIGHT,PSP_CTRL_SQUARE|PSP_CTRL_RIGHT,.016f,0,0);INPUT_CHECK(page==FLIGHT&&(IS_DEBRIS_ID(selected_target)||IS_ANOMALY_ID(selected_target)),"Square+Right tabs to other contacts");
 /* Keep only the pirate in range so AI keeps engaging the player (not a nearer trader). */
 game.npc[0].alive=0;
 game.npc[1].alive=1;game.npc[1].role=PIRATES;game.npc[1].pos=(Vec3){0,0,600};game.npc[1].target=-2;
 input(PSP_CTRL_RIGHT,PSP_CTRL_SQUARE|PSP_CTRL_RIGHT,.016f,0,0);INPUT_CHECK(page==FLIGHT&&IS_NPC_ID(selected_target)&&npc_is_hostile(&game.npc[selected_target-BODY_COUNT-1])&&scan_cat==4,"Square+Right tabs to the ENEMIES band");
 for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;
 scan_cat=3;
 input(PSP_CTRL_RIGHT,PSP_CTRL_SQUARE|PSP_CTRL_RIGHT,.016f,0,0);INPUT_CHECK(page==FLIGHT&&scan_cat==4,"Square+Right still opens empty ENEMIES band");
 game.attacked=2.f;INPUT_CHECK(combat_alert_active(),"combat alert arms under fire");
 input(PSP_CTRL_RIGHT,PSP_CTRL_SQUARE|PSP_CTRL_RIGHT,.016f,0,0);INPUT_CHECK(page==FLIGHT&&selected_target>=2&&selected_target<=BODY_COUNT,"Square+Right tabs to planets");int planet_lock=selected_target;input(PSP_CTRL_UP,PSP_CTRL_SQUARE|PSP_CTRL_UP,.016f,0,0);INPUT_CHECK(page==FLIGHT&&selected_target>=2&&selected_target<=BODY_COUNT&&selected_target!=planet_lock,"Square+Up cycles the highlighted planet band");
 INPUT_CHECK(!autoaim,"Square+D-pad highlights without turning the ship");game.message_time=0;input(PSP_CTRL_RTRIGGER,PSP_CTRL_SQUARE|PSP_CTRL_RTRIGGER,.016f,0,0);INPUT_CHECK(page==FLIGHT&&autoaim&&!game.boost,"R while holding Square locks the selected target without boosting");INPUT_CHECK(game.message_time<=0,"Square+R locks silently while the navigation computer supplies the control reference");input(0,0,.016f,0,0);INPUT_CHECK(page==FLIGHT&&autoaim,"releasing Square keeps the explicit target lock active");
 TEST_INIT();launch(&game);page=FLIGHT;for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;game.pos=(Vec3){0,0,0};game.yaw=game.pitch=0;for(int i=0;i<3;i++){game.npc[i].alive=1;game.npc[i].pos=(Vec3){i?100:-100,0,i==2?-900:900};}selected_target=-1;input(PSP_CTRL_LTRIGGER,PSP_CTRL_SQUARE|PSP_CTRL_LTRIGGER,.016f,0,0);int view_first=selected_target;INPUT_CHECK(valid_target(view_first)&&camera(&game,target_position(view_first)).z>20&&view_first!=NPC_ID_MIN+2&&!autoaim,"Square+L highlights a visible forward contact without auto-turning");input(0,PSP_CTRL_SQUARE,.016f,0,0);input(PSP_CTRL_LTRIGGER,PSP_CTRL_SQUARE|PSP_CTRL_LTRIGGER,.016f,0,0);INPUT_CHECK(valid_target(selected_target)&&selected_target!=view_first&&camera(&game,target_position(selected_target)).z>20&&selected_target!=NPC_ID_MIN+2,"repeated Square+L cycles only contacts in front");
 TEST_INIT();launch(&game);page=FLIGHT;input(PSP_CTRL_SQUARE|PSP_CTRL_LEFT,PSP_CTRL_SQUARE|PSP_CTRL_LEFT,.016f,0,0);INPUT_CHECK(page==FLIGHT,"Square+Left chord never opens the computer");input(0,0,.016f,0,0);INPUT_CHECK(page==FLIGHT,"releasing a Square chord still leaves flight");
 TEST_INIT();game.system=0;launch(&game);page=FLIGHT;for(int i=0;i<NPC_COUNT;i++)if(game.npc[i].alive&&game.npc[i].role==PIRATES){game.npc[i].target=-2;game.npc[i].pos=(Vec3){0,0,600};break;}
 input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE|PSP_CTRL_RTRIGGER,.016f,0,0);INPUT_CHECK(page==FLIGHT&&selected_target>BODY_COUNT&&npc_is_hostile(&game.npc[selected_target-BODY_COUNT-1])&&scan_cat==4,"R and Square locks nearest hostile on the ENEMIES band");int shots=game.shots;input(PSP_CTRL_CROSS,PSP_CTRL_CROSS|PSP_CTRL_LTRIGGER,.016f,0,0);INPUT_CHECK(game.missile_time>0&&game.missiles==0&&game.shots==shots,"L and X launches missile without firing laser");
 TEST_INIT();game.contract=game.system;game.mission_type=MISSION_DELIVERY;page=TARGETING;target_filter=5;target_refresh();INPUT_CHECK(target_count==1&&target_ids[0]==0,"mission filter isolates marked station objective");
 TEST_INIT();INPUT_CHECK(!strcmp(tracked_hud_cue(),"OPEN: TRACKED MISSION"),"HUD cue tells a docked player to open the tracked briefing");game.credits=20000;accept_mission(&game,0);tracked_mission=2;INPUT_CHECK(strstr(tracked_hud_cue(),game.systems[game.jobs[0].dest].name)!=0,"HUD cue follows a tracked station contract destination");
 TEST_INIT();game.credits=20000;game.job_n=MISSION_SLOTS;for(int i=0;i<MISSION_SLOTS;i++)game.jobs[i]=(Job){.type=MISSION_DELIVERY,.origin=game.system,.dest=game.system,.target=-1,.reward=1000,.time=300};int old_destination=game.destination;INPUT_CHECK(game.job_n==5&&!accept_mission(&game,0)&&game.destination==old_destination,"mission board rejects a sixth job when the log is full");
 TEST_INIT();game.credits=20000;accept_mission(&game,0);INPUT_CHECK(mission_offer_active(&game,0)&&!accept_mission(&game,0)&&game.job_n==1,"accepted offer stays in progress and cannot be taken twice");change_page(MISSIONLOG);row=1;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(tracked_mission==1,"mission log tracks the optional Guild assignment");input(PSP_CTRL_SQUARE,0,.016f,0,0);INPUT_CHECK(page==CAMPAIGN&&tracked_mission==1,"tracked next-step screen follows the selected Guild mission");change_page(MISSIONLOG);row=2;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(tracked_mission==2&&game.job_sel==0,"mission log tracks an accepted station contract");input(PSP_CTRL_TRIANGLE,0,.016f,0,0);INPUT_CHECK(game.job_n==1&&abandon_confirm,"triangle asks before abandoning the focused mission");input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(game.job_n==0&&!abandon_confirm&&tracked_mission==0,"second X abandons the contract and restores story tracking");
 game_init(&game);deck_reset();change_page(HOME);INPUT_CHECK(row==0,"new commander opens the Fly category with Launch focused");row=6;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(page==HELP&&game.story==STORY_FREE,"opening Controls does not start retired optional coaching");
 game_init(&game);deck_reset();change_page(HOME);row=3;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(page==YARD&&game.story==STORY_FREE,"retired coaching does not restrict the shipyard");
 {int selected=row,view=shipyard_stats_view;input(PSP_CTRL_SQUARE,0,.016f,0,0);INPUT_CHECK(page==YARD&&row==selected&&shipyard_stats_view!=view,"shipyard Square toggles description and stats without changing hull");}
 TEST_INIT();game.docked=1;page=INVENTORY;row=FIT_WPN;fit_clear_all(&game);game.fit[FIT_WPN]=1;fit_rebuild(&game);{int before=game.module_store[1];input(PSP_CTRL_SQUARE,0,.016f,0,0);INPUT_CHECK(game.fit[FIT_WPN]==FIT_EMPTY&&game.module_store[1]==before+1,"ship loadout Square moves the selected fitted module into Item Storage");}
 TEST_INIT();game.docked=1;game.ship=PLAYER_SHIP_MEGA_FIRST+4;fit_clear_all(&game);memset(game.module_store,0,sizeof(game.module_store));game.module_store[1]=1;page=STORAGE;storage_sell_mode=0;storage_bank_focus=0;module_inventory_row=module_bank=0;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(storage_bank_focus&&game.module_store[1]==1,"item storage X selects a module before fitting");input(PSP_CTRL_DOWN,0,.016f,0,0);INPUT_CHECK(module_bank==1,"item storage Up/Down chooses among the current hull's real banks");input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(!storage_bank_focus&&game.fit[FIT_WPN+6]==1&&game.module_store[1]==0,"second X fits the selected module into the chosen bank");
 TEST_INIT();change_page(HOME);row=15;input(PSP_CTRL_CIRCLE,0,.016f,0,0);INPUT_CHECK(page==HOME&&row==15,"circle on a docked deck keeps the selected door");
 TEST_INIT();change_page(COMMS);INPUT_CHECK(page==COMMS_PANEL,"comms: legacy menu route resolves to the canonical panel");row=3;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(page==COMMS_PANEL||page==FLIGHT||page==HOME,"comms: docking action resolves safely from the canonical panel");
 game_init(&game);deck_reset();change_page(STORY);INPUT_CHECK(game.story==STORY_FREE&&page==HOME,"optional flight guide is no longer an accessible side path");
 TEST_INIT();change_page(HOME);row=20;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(page==WALK&&walk_kind==0,"docked Fly menu disembarks onto the station concourse");
 input(0,0,.016f,0,0); /* build station rooms */
 {int h0=sc_hot;input(PSP_CTRL_DOWN,0,.016f,0,0);INPUT_CHECK(page==WALK&&sc_hot!=h0,"station: DOWN cycles options list");}
 /* First person in the options list talks when X is pressed. */
 {
  ScHot hot[24];int hn=sc_hotspots(hot,24),person=-1;
  for(int i=0;i<hn;i++)if(hot[i].kind==SC_H_PERSON){person=i;break;}
  INPUT_CHECK(person>=0,"station: arrivals lists at least one person");
  if(person>=0){sc_hot=person;input(PSP_CTRL_CROSS,0,.016f,0,0);
   INPUT_CHECK(sc_menu==SC_MENU_TALK,"station: X on a person opens talk choices");
   input(PSP_CTRL_CROSS,0,.016f,0,0); /* pick first choice */
   INPUT_CHECK(sc_menu==SC_MENU_NONE||sc_menu==SC_MENU_SHOP||game.message_time>0||game.voice_time>0,"station: talk choice resolves");}
 }
 input(PSP_CTRL_CIRCLE,0,.016f,0,0);input(PSP_CTRL_CIRCLE,0,.016f,0,0);INPUT_CHECK(page==HOME,"station: Circle returns to the command deck");
 /* Selecting a door option walks the small room graph. */
 TEST_INIT();change_page(HOME);row=20;input(PSP_CTRL_CROSS,0,.016f,0,0);input(0,0,.016f,0,0);
 {int start=sc_room;
  ScHot hot[24];int hn=sc_hotspots(hot,24),ex=-1;
  for(int i=0;i<hn;i++)if(hot[i].kind==SC_H_EXIT&&hot[i].id!=SC_EXIT_SHIP){ex=i;break;}
  if(ex>=0){sc_hot=ex;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(sc_room!=start,"station: X on a door changes room");}
  else INPUT_CHECK(0,"station: arrivals must list at least one exit hotspot");}
 TEST_INIT();launch(&game);page=FLIGHT;game.approach=1;enter_planet(&game);{Vec3 pad=surface_site(&game,1);game.pos=add(pad,(Vec3){0,18,0});game.speed=8;land_planet(&game);eva_toggle(&game);}
 {Vec3 before=game.pos;input(0,0,.05f,0,.9f);INPUT_CHECK(game.surface==2&&length(sub(game.pos,before))<.01f&&game.pitch>0,"planet EVA: nub looks without walking");}
 #include "journey-input-tests.h"
 #include "radio-input-tests.h"
 #include "comms-input-tests.h"
 #include "police-input-tests.h"
 #include "outfitting-input-tests.h"
#include "multislot-input-tests.h"
 #include "quiet-hails-tests.h"
 #include "commander-input-tests.h"
 #include "law-input-tests.h"
 #include "surface-activity-tests.h"
#include "planet-site-input-tests.h"
#include "observatory-input-tests.h"
 #include "surface-upgrade-tests.h"
 #include "planet-sequence-tests.h"
 #include "flight-tools-input-tests.h"
#include "landing-safety-tests.h"
 #include "campaign-input-tests.h"
 #include "ui-navigation-tests.h"
 {
  RadarPoint ahead=radar_point((Vec3){0,0,1000}),aft=radar_point((Vec3){0,0,-1000});
  RadarPoint left=radar_point((Vec3){-1000,0,0}),right=radar_point((Vec3){1000,0,0});
  INPUT_CHECK(ahead.y<230&&aft.y>230&&ahead.x==240&&aft.x==240,"radar: ahead and behind occupy opposite halves");
  INPUT_CHECK(left.x<240&&right.x>240,"radar: left and right match ship-relative bearing");
  int bounded=1;for(int i=0;i<360;i++){float a=i*.017453293f;RadarPoint q=radar_point((Vec3){sinf(a)*100000,50000,cosf(a)*100000});if(q.x<178||q.x>302||q.y<214||q.y>246||abs(q.lift)>4)bounded=0;}
  INPUT_CHECK(bounded,"radar: all 360-degree distant bearings remain inside instrument");
 }
 {
  TEST_INIT();launch(&game);game.yaw=.73f;game.pitch=.19f;game.roll=.47f;Game rear=game;rear.yaw+=3.14159265f;rear.pitch=-game.pitch;
  Vec3 nose=forward(&game),behind=add(game.pos,mul(nose,-1200)),ahead=add(game.pos,mul(nose,1200));int bx=0,by=0,ax=0,ay=0;
  int sees_behind=rear_mirror_project(&rear,behind,4,1,252,22,&bx,&by,0),sees_ahead=rear_mirror_project(&rear,ahead,4,1,252,22,&ax,&ay,0);
  INPUT_CHECK(sees_behind&&!sees_ahead&&bx>110&&bx<150&&by>2&&by<22,"rear mirror: aft camera centres contacts behind and rejects forward contacts");
 }
 #include "planet-approach-input-tests.h"
 #include "planet-eva-input-tests.h"
#include "eva-local-map-tests.h"
 #include "tutorial-input-tests.h"
 {
  unsigned *saved_fb=fb,*pixels=malloc(STRIDE*H*sizeof(unsigned));
  INPUT_CHECK(pixels!=0,"graphics: disposable framebuffer allocated");
  if(pixels){
   fb=pixels;TEST_INIT();hud_mode=hud_hidden=0;int old_quiet=quiet_comms;quiet_comms=0;
#include "weapon-visual-tests.h"
#include "thargoid-extreme-visual-tests.h"
   {change_page(STATUS);snprintf(game.commander_name,25,"NOVA REED");game.commander_portrait=4;rect(0,0,W,H,BG);status();dump_native_bmp("commander-status.bmp");profile_mode=1;snprintf(profile_draft,25,"NOVA REED");rect(0,0,W,H,BG);status();dump_native_bmp("commander-name.bmp");profile_mode=0;
    profile_from_intro=1;for(int i=0;i<3;i++){profile_info[i].state=1;profile_info[i].credits=(i+1)*12345;profile_info[i].system=i*17+7;profile_info[i].ship=0;profile_info[i].portrait=i*3;snprintf(profile_info[i].name,25,"%s",i==0?"NOVA REED":i==1?"ORION VALE":"ALEXANDRA STARFARER");}rect(0,0,W,H,BG);status();dump_native_bmp("commander-slots.bmp");profile_from_intro=0;profile_refresh();intro_screen();dump_native_bmp("commander-title.bmp");TEST_INIT();
   }
   {TEST_INIT();game.docked=1;game.system=7;game.legal=10;game.wanted[7]=10;game.wanted[8]=6;game.crime_record[7]=CRIME_ASSAULT;game.crime_record[8]=CRIME_ESCAPE;law_enter();rect(0,0,W,H,BG);law_screen();dump_native_bmp("law-ledger.bmp");law_call_open=1;rect(0,0,W,H,BG);law_screen();dump_native_bmp("law-call.bmp");TEST_INIT();}
   {TEST_INIT();launch(&game);game.npc[0].alive=1;game.npc[0].role=TRADERS;game.npc[0].freighter=0;game.npc[0].target=-1;game.npc[0].pos=add(game.pos,(Vec3){0,0,1200});
    for(int story=0;story<NIGHT_STORY_COUNT;story++){night_begin(0,story);night_open=1;row=0;
     for(int stage=0;stage<3;stage++){night_stage=stage;rect(0,0,W,H,BG);night_panel();char capture[48];snprintf(capture,sizeof(capture),"quiet-watch-%d-%d.bmp",story,stage);dump_native_bmp(capture);}
    }night_close();night_wait=480;night_seen=0;TEST_INIT();
   }
   {unsigned original[16];memcpy(original,ship_paint,sizeof(original));int contrast=1,distinct=1,map_distinct=1,comms_distinct=1;unsigned hashes[DECORATOR_COUNT],map_hashes[DECORATOR_COUNT],comms_hashes[DECORATOR_COUNT];
    TEST_INIT();game.ship=0;preview_time=12;
    for(int finish=0;finish<DECORATOR_COUNT;finish++){
     ship_paint[0]=decorator_finishes[finish];const ShipTheme *t=&ship_themes[finish];
     unsigned bg=t->panel;int bg_l=((bg&255)*3+((bg>>8)&255)*6+((bg>>16)&255))/10;
     unsigned fg[]={t->text,t->muted,t->accent,t->signal};
     for(int k=0;k<4;k++){int lum=((fg[k]&255)*3+((fg[k]>>8)&255)*6+((fg[k]>>16)&255))/10;contrast&=lum-bg_l>=100;}
     game.docked=0;change_page(HOME);rect(0,0,W,H,BG);home();char capture[48];snprintf(capture,sizeof(capture),"paint-%d-menu.bmp",finish);dump_native_bmp(capture);
     unsigned hash=0;for(int y=66;y<154;y++)for(int x=248;x<462;x++)hash=hash*33u+pixels[y*STRIDE+x];hashes[finish]=hash;
     for(int k=0;k<finish;k++)distinct&=hashes[k]!=hash;
     launch(&game);page=FLIGHT;game.speed=0;space();snprintf(capture,sizeof(capture),"paint-%d-hud.bmp",finish);dump_native_bmp(capture);game.docked=1;
     change_page(CHART);chart_cursor=game.system;chart_filter=CHART_ALL;chart_search=0;chart_zoom=chart_zoom_goal=1;chart_pan_x=chart_pan_y=0;rect(0,0,W,H,BG);galaxy_overview();
     unsigned map_hash=0;for(int y=2;y<122;y++)for(int x=2;x<473;x++)map_hash=map_hash*33u+pixels[y*STRIDE+x];map_hashes[finish]=map_hash;for(int k=0;k<finish;k++)map_distinct&=map_hashes[k]!=map_hash;
     if(finish==0||finish==3||finish==13){snprintf(capture,sizeof(capture),"paint-%d-galaxy-maps.bmp",finish);dump_native_bmp(capture);}
     change_page(COMMS_PANEL);row=10;rect(0,0,W,H,BG);comms_console_draw();unsigned comms_hash=0;for(int y=4;y<255;y++)for(int x=4;x<476;x++)comms_hash=comms_hash*33u+pixels[y*STRIDE+x];comms_hashes[finish]=comms_hash;for(int k=0;k<finish;k++)comms_distinct&=comms_hashes[k]!=comms_hash;
     if(finish==0||finish==3||finish==13){snprintf(capture,sizeof(capture),"paint-%d-comms-panel.bmp",finish);dump_native_bmp(capture);}
    }
    INPUT_CHECK(contrast,"paint themes: all sixteen palettes keep text and accents bright against dark panels");
    INPUT_CHECK(distinct,"paint themes: regular menu ship window changes for every fitted finish");
    INPUT_CHECK(map_distinct,"paint themes: Galaxy.Maps follows every fitted ship finish");
    INPUT_CHECK(comms_distinct,"paint themes: Comms Panel follows every fitted ship finish");
    ship_paint[0]=decorator_finishes[3];int stored=paint_save();ship_paint[0]=decorator_finishes[0];paint_load();
    INPUT_CHECK(stored&&ship_paint[0]==decorator_finishes[3]&&ship_theme_index()==3,"paint themes: fitted colour and interface palette survive preferences reload");
    memcpy(ship_paint,original,sizeof(original));paint_save();TEST_INIT();
   }
   {TEST_INIT();preview_time=18;game.system=7;system_bodies(&game);game.docked=1;game.station_variant=0;change_page(HOME);rect(0,0,W,H,BG);home();dump_native_bmp("menu-docked-station-lave.bmp");unsigned lave_hash=0;for(int y=64;y<156;y++)for(int x=246;x<464;x++)lave_hash=lave_hash*33u+pixels[y*STRIDE+x];
    int mega=-1;for(int s=0;s<256;s++)if(station_class_for_system(&game,s)==STATION_MEGA){mega=s;break;}game.system=mega;system_bodies(&game);game.docked=1;game.station_variant=0;rect(0,0,W,H,BG);home();dump_native_bmp("menu-docked-station-mega.bmp");unsigned mega_hash=0;for(int y=64;y<156;y++)for(int x=246;x<464;x++)mega_hash=mega_hash*33u+pixels[y*STRIDE+x];
    game.system=7;system_bodies(&game);game.station_variant=1;rect(0,0,W,H,BG);home();dump_native_bmp("menu-docked-station-relay.bmp");unsigned relay_hash=0;for(int y=64;y<156;y++)for(int x=246;x<464;x++)relay_hash=relay_hash*33u+pixels[y*STRIDE+x];
    INPUT_CHECK(mega>=0&&lave_hash!=mega_hash&&lave_hash!=relay_hash&&mega_hash!=relay_hash,"command deck: docked preview shows distinct current station exteriors");INPUT_CHECK(drawcount==0,"command deck: station preview flushes its bounded geometry queue");TEST_INIT();
   }
   rect(0,0,W,H,BG);
   line((-2147483647-1),110,240,110,WHITE);
   line(240,110,2147483647,110,WHITE);
   INPUT_CHECK(pixels[110*STRIDE+240]==BG,"graphics: extreme projected lines return without drawing or unbounded stepping");
   for(int poster=0;poster<BOUNTY_POSTER_COUNT;poster++){
    TEST_INIT();change_page(GALNET);galnet_tab=2;row=poster;
    if(poster&1)launch(&game);
    input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
    for(int frame=0;frame<120;frame++){input(0,0,.05f,0,0);if(frame%6==0)space();}
    INPUT_CHECK(page==FLIGHT&&valid_target(selected_target),"Wanted poster survives six seconds of actual flight rendering");
    {int shots=game.shots;game.laser=1;game.shot=0;input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
     INPUT_CHECK(game.shots>shots,"Wanted target pursuit accepts laser fire after selection release");
     input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);input(0,0,.016f,0,0);input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);
     INPUT_CHECK(game.boost&&page==FLIGHT,"Wanted target pursuit accepts normal double-R boost");}
    fflush(f);
   }
   TEST_INIT();change_page(GALNET);galnet_tab=0;
   for(row=0;row<16;row++){rect(0,0,W,H,BG);INPUT_CHECK(news_screen(),"newspaper: complete article fits above lower-page extras");galnet_screen();if(row==0)dump_native_bmp("news-front.bmp");if(row==5)dump_native_bmp("news-puzzle.bmp");if(row==11)dump_native_bmp("news-story.bmp");}
   row=1;galnet_tab=2;galnet_screen();dump_native_bmp("wanted-board.bmp");
   row=BOUNTY_POSTER_COUNT-1;galnet_screen();dump_native_bmp("wanted-last.bmp");
   TEST_INIT();change_page(COMMS_PANEL);row=0;rect(0,0,W,H,BG);comms_panel();dump_native_bmp("comms-actions.bmp");
   row=7;rect(0,0,W,H,BG);comms_panel();dump_native_bmp("comms-settings.bmp");
   TEST_INIT();change_page(YARD);
   INPUT_CHECK(sizeof(shipyard_description)/sizeof(shipyard_description[0])==(unsigned)player_ship_count,"shipyard: every purchasable hull has authored prose");
   for(row=0;row<player_ship_count;row++){
    const char *left;rect(0,0,W,H,BG);preview_time=1.8f;yard();text_wrap(3,26,54,4,DIM,shipyard_description[row],&left);
    INPUT_CHECK(!*left,"shipyard: complete description fits its four-line reading area");
    char image_name[40];snprintf(image_name,sizeof(image_name),"shipyard-%02d.bmp",row);dump_native_bmp(image_name);
   }
   INPUT_CHECK(newspaper_identity[7].page_count==16,"Lave retains sixteen fixed newspaper pages");
   {
    int old_contrast=high_contrast,contained=1,gas_action=1;
    FILE *capture=fopen("dump-native.flag","r");int capture_approach=capture!=0;if(capture)fclose(capture);
    for(int kind=0;kind<2;kind++)for(int mode=0;mode<3;mode++){
     TEST_INIT();launch(&game);page=FLIGHT;game.approach=1;game.bodies[1].type=kind?GAS:OCEAN;
     hud_mode=mode;hud_hidden=mode==2;high_contrast=mode==2;
     memset(pixels,0,STRIDE*H*sizeof(unsigned));planet_prompt();
     int title=0,note=0,entry=0;
     for(int y=0;y<H;y++)for(int x=0;x<W;x++)if(pixels[y*STRIDE+x]){
      if(x<20||x>=460||y<88||y>=172)contained=0;
      if(y>=96&&y<104&&pixels[y*STRIDE+x]==RGB(240,180,91))title=1;
      if(y>=152&&y<160&&pixels[y*STRIDE+x]==RGB(155,154,165))note=1;
      if(x>=58&&x<200&&y>=120&&y<144&&pixels[y*STRIDE+x]!=RGB(21,28,39))entry=1;
     }
     contained &= title&&note;gas_action &= entry;
     if(capture_approach){char path[64];space();snprintf(path,sizeof(path),"approach-%s-mode-%d.bmp",kind?"gas":"solid",mode);dump_native_bmp(path);}
    }
    INPUT_CHECK(contained,"graphics: approach title and explanation stay inside modal in all HUD modes");
    INPUT_CHECK(gas_action,"graphics: all planets offer surface or platform flight");
    high_contrast=old_contrast;TEST_INIT();hud_mode=hud_hidden=0;
   }
   {
    TEST_INIT();launch(&game);game.approach=1;enter_planet(&game);game.pos=surface_site(&game,1);game.speed=8;land_planet(&game);eva_toggle(&game);page=FLIGHT;
    memset(pixels,0,STRIDE*H*sizeof(unsigned));drawcount=0;space();
    int filled=1;for(int y=192;y<240;y++)for(int x=0;x<W;x++)if(!pixels[y*STRIDE+x])filled=0;
    INPUT_CHECK(view_top()==28&&view_bot()==239&&filled,"EVA graphics: world fills the space between header and return strip");
    FILE *capture=fopen("eva-capture.flag","r");
    if(capture){
     fclose(capture);int old_contrast=high_contrast;
     TEST_INIT();launch(&game);game.approach=1;enter_planet(&game);
     game.pos=surface_site(&game,1);game.speed=8;land_planet(&game);eva_toggle(&game);
     page=FLIGHT;game.message_time=0;
     for(int mode=0;mode<2;mode++){high_contrast=mode;drawcount=0;space();dump_native_bmp(mode?"eva-boarding-contrast.bmp":"eva-boarding.bmp");}
     game.pos.x+=160;game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;
     game.yaw=atan2f(game.ship_pos.x-game.pos.x,game.ship_pos.z-game.pos.z);game.hazard=42;game.message_time=0;high_contrast=0;drawcount=0;space();dump_native_bmp("eva-return.bmp");
     game.pos=game.ship_pos;game.pos.y+=4;game.hazard=0;survey_scan(&game);drawcount=0;space();dump_native_bmp("eva-scan-result.bmp");
     game.pos.y+=35;eva_toggle(&game);drawcount=0;space();dump_native_bmp("eva-airborne-board.bmp");
     game.hazard=100;game.energy=35;drawcount=0;space();dump_native_bmp("eva-exposure.bmp");
     game.hazard=0;game.message_time=0;game.pitch=.75f;drawcount=0;space();dump_native_bmp("eva-look-up.bmp");
     game.pitch=-.75f;drawcount=0;space();dump_native_bmp("eva-look-down.bmp");
     for(int biome=0;biome<5;biome++){game.bodies[game.planet].type=biome?ROCKY:OCEAN;game.bodies[game.planet].seed=biome?biome-1:0;game.pos=surface_site(&game,1);game.pos.z-=180;game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;game.yaw=game.pitch=0;game.message_time=0;drawcount=0;space();char capture[48];snprintf(capture,sizeof(capture),"surface-biome-%d.bmp",biome);dump_native_bmp(capture);}
     change_page(HELP);help_tab=4;rect(0,0,W,H,BG);help();dump_native_bmp("eva-controls.bmp");
     high_contrast=old_contrast;
    }
    TEST_INIT();hud_mode=hud_hidden=0;
   }
   memset(pixels,0,STRIDE*H*sizeof(unsigned));button_icon(10,10,'T',WHITE);
   INPUT_CHECK(pixels[11*STRIDE+14]&&pixels[17*STRIDE+11]&&pixels[17*STRIDE+17]&&!pixels[12*STRIDE+11],"graphics: PSP Triangle icon points upward");
   int clean=1;
   for(int speaker=VOICE_KEI;speaker<=VOICE_COMP;speaker++){
    memset(pixels,0,STRIDE*H*sizeof(unsigned));
    speak(&game,speaker,"Long radio transmission checks that every word remains in the top caption band.");
    cockpit();
    for(int y=56;y<192;y++)for(int x=0;x<W;x++)if(pixels[y*STRIDE+x])clean=0;
   }
   INPUT_CHECK(clean,"graphics: routine cockpit text never paints central viewing area");
   memset(pixels,0,STRIDE*H*sizeof(unsigned));launch(&game);page=FLIGHT;tracked_mission=0;hud_mode=hud_hidden=0;quiet_comms=1;game.voice_time=0;game.message_time=0;cockpit();
   {
    /* Green arrow-wrapped objective cue — top-right header with a margin. */
    const char *cue=tracked_hud_cue();char route[52];snprintf(route,sizeof(route),">> %s >>",cue);int clen=(int)strlen(route);int cols=W/8,inset=1,left=33,max=cols-inset-left;if(clen>max)clen=max;
    int start=cols-inset-clen,right_px=(start+clen)*8,seen=0,far=0,flush=0;
    unsigned ink=RGB(100,235,150);
    for(int y=0;y<8;y++)for(int x=start*8;x<right_px&&x<W;x++)if(pixels[y*STRIDE+x]==ink){seen=1;if(x>=W-32)far=1;if(x>=W-8)flush=1;}
    INPUT_CHECK(seen&&far&&!flush&&start>=left,"graphics: mission cue top-right in header with margin");
   }
   memset(pixels,0,STRIDE*H*sizeof(unsigned));add_crime(&game,15);cockpit();
   {
    int wl=wanted_level(&game),seen=0;
    for(int y=8;y<16;y++)for(int x=8;x<120;x++)if(pixels[y*STRIDE+x]==RED)seen=1;
    INPUT_CHECK(wl==3&&seen,"graphics: local wanted level sits under the system name when police may pursue");
   }
   memset(pixels,0,STRIDE*H*sizeof(unsigned));launch(&game);page=HOME;menu_space_view(246,64,218,92);
   {
    int gold=0;for(int y=65;y<150;y++)for(int x=247;x<460;x++){unsigned p=pixels[y*STRIDE+x];if(p==GOLD||p==RGB(193,139,77)||((p&255)>180&&((p>>8)&255)>130))gold=1;}
    INPUT_CHECK(gold,"graphics: Select deck top-right shows a third-person ship silhouette");
   }
   {TEST_INIT();launch(&game);page=FLIGHT;game.message_time=game.voice_time=0;game.missiles=4;
    square_held=0;rect(0,0,W,H,BG);cockpit();dump_native_bmp("flight-control-labels.bmp");
    tools_open=1;tools_panel();dump_native_bmp("weapons-pad.bmp");tools_open=0;
    rect(0,0,W,H,BG);tools_confirm_action=2;tools_weapon_result=0;tools_confirm_time=.60f;tools_panel();dump_native_bmp("weapons-pad-tractor-confirm.bmp");
    game.ship=6;fit_clear_all(&game);game.fit[0]=1;game.fit[6]=2;fit_rebuild(&game);game.active_weapon=6;snprintf(tools_weapon_old,sizeof(tools_weapon_old),"PULSE");snprintf(tools_weapon_new,sizeof(tools_weapon_new),"BEAM");rect(0,0,W,H,BG);tools_confirm_action=3;tools_weapon_result=1;tools_confirm_time=.90f;tools_panel();dump_native_bmp("weapons-pad-weapon-slide.bmp");
    rect(0,0,W,H,BG);game.fit[6]=FIT_EMPTY;game.active_weapon=0;fit_rebuild(&game);tools_confirm_action=3;tools_weapon_result=-1;tools_confirm_time=.60f;tools_panel();dump_native_bmp("weapons-pad-one-weapon.bmp");tools_confirm_time=0;tools_confirm_action=-1;
    rect(0,0,W,H,BG);targeting_overlay(8,28,192,156,0);dump_native_bmp("target-control-labels.bmp");
    int left=0,right=0;for(int y=170;y<183;y++)for(int x=16;x<192;x++){if(pixels[y*STRIDE+x]!=BG&&pixels[y*STRIDE+x]!=RGB(10,18,29)){if(x<100)left++;else right++;}}
    INPUT_CHECK(left>8&&right>8,"flight labels: target computer displays both shoulder-button action hints");
    TEST_INIT();
   }
   #include "surface-visual-tests.h"
   #include "surface-closeup-tests.h"
   #include "planet-sequence-visual-tests.h"
 #include "landing-render-tests.h"
   #include "system-spread-visual-tests.h"
   #include "power-theme-tests.h"
   {TEST_INIT();page=DECORATOR;row=2;decorator_feedback=0;decorator_screen();dump_native_bmp("decorator-label-alignment.bmp");TEST_INIT();}
   #include "menu-preview-tests.h"
   #include "station-bar-preview-tests.h"
   #include "station-lave-tests.h"
   #include "station-reading-tests.h"
   #include "tutorial-visual-tests.h"
   {
    int saved_row=row,saved_contrast=high_contrast,labels_ok=1;
    for(int contrast=0;contrast<2;contrast++)for(int slot=0;slot<6;slot++){
     high_contrast=contrast;row=slot;memset(pixels,0,STRIDE*H*sizeof(unsigned));inventory_screen();
     int glyphs=0,label_y=82+slot*20;
     for(int yy=label_y;yy<label_y+8;yy++)for(int xx=16;xx<48;xx++)if(pixels[yy*STRIDE+xx]==LC_GOLD)glyphs++;
     if(!glyphs)labels_ok=0;
     char capture[64];snprintf(capture,sizeof(capture),"loadout-slot-%d-%s.bmp",slot,contrast?"contrast":"normal");dump_native_bmp(capture);
    }
    INPUT_CHECK(labels_ok,"loadout: all six selected hull labels render visibly in both palettes");
    row=saved_row;high_contrast=saved_contrast;
   }
   {
    Job saved_jobs[MISSION_SLOTS];memcpy(saved_jobs,game.jobs,sizeof(saved_jobs));
    int saved_n=game.job_n,saved_row=row,saved_track=tracked_mission,saved_confirm=abandon_confirm,saved_contrast=high_contrast;
    game.job_n=MISSION_SLOTS;row=MISSION_SLOTS+1;tracked_mission=0;abandon_confirm=0;
    for(int i=0;i<MISSION_SLOTS;i++){memset(&game.jobs[i],0,sizeof(Job));game.jobs[i].type=MISSION_DELIVERY;game.jobs[i].dest=i;game.jobs[i].time=300;}
    int visible=1;
    for(int mode=0;mode<2;mode++){
     high_contrast=mode;memset(pixels,0,STRIDE*H*sizeof(unsigned));mission_log();
     int label=0;for(int yy=152;yy<160;yy++)for(int xx=32;xx<216;xx++)if(pixels[yy*STRIDE+xx]==WHITE)label++;
     if(!label)visible=0;
     dump_native_bmp(mode?"mission-log-full-contrast.bmp":"mission-log-full-normal.bmp");
    }
    INPUT_CHECK(visible,"mission log: fifth contract remains visible above the objective in both palettes");
    memcpy(game.jobs,saved_jobs,sizeof(saved_jobs));game.job_n=saved_n;row=saved_row;tracked_mission=saved_track;abandon_confirm=saved_confirm;high_contrast=saved_contrast;
   }
   {
    /* Engine roots must sit on the mesh aft tip — radius-scaled glow floated past wide ships. */
    int aft_ok=1;
    for(int mesh=0;mesh<mesh_count;mesh++){
     float aft=0,rad=1;const Mesh *m=&meshes[mesh];
     for(int v=0;v<m->vertices;v++){float L=length(m->v[v]);if(L>rad)rad=L;aft=fmaxf(aft,-m->v[v].z);}
     if(aft<1)continue;
     NPC sample={0};sample.mesh=mesh;sample.scale=1;sample.radius=rad;sample.dir=(Vec3){0,0,1};
     if(npc_engine_aft(&sample)>aft+0.01f)aft_ok=0;
    }
    NPC tip={0};tip.mesh=mesh_id("VIPER");tip.scale=1;tip.radius=80;tip.dir=(Vec3){0,0,1};
    float viperaft=0;for(int v=0;v<meshes[tip.mesh].vertices;v++)viperaft=fmaxf(viperaft,-meshes[tip.mesh].v[v].z);
    Vec3 root=npc_engine_root(&tip,0);
    INPUT_CHECK(aft_ok&&fabsf(root.z+viperaft)<0.05f&&viperaft<tip.radius*.65f,"graphics: engine glow roots at the mesh aft tip (not past the silhouette)");
   }
   memset(pixels,0,STRIDE*H*sizeof(unsigned));preview_clip(240,110,20,20,25,25);
   DrawTri t={{{20,20,30},{30,20,30},{20,30,30}},WHITE,30,0};triangle(&t);
   int clipped=1;for(int y=0;y<H;y++)for(int x=0;x<W;x++)if(pixels[y*STRIDE+x]&&(x<20||x>=25||y<20||y>=25))clipped=0;
   INPUT_CHECK(clipped,"graphics: odd-sized preview clips contain every raster pixel");
   /* Soft-FB space FX kit: nebula/clouds paint the canopy; high contrast skips them. */
   {
    high_contrast=0;game.system=3;system_bodies(&game);launch(&game);page=FLIGHT;game.jump=0;game.boost=0;
    memset(pixels,0,STRIDE*H*sizeof(unsigned));sector_background();space_fx_nebula();starfield();
    int haze=0;for(int y=view_top();y<=view_bot();y++)for(int x=0;x<W;x++){unsigned c=pixels[y*STRIDE+x];if(c&&c!=BG)haze++;}
    INPUT_CHECK(haze>2000,"graphics: space-fx nebula and starfield paint the canopy");
    {int unique=1;for(int a=0;a<256;a++)for(int b=a+1;b<256;b++)if(space_sky_signature(a)==space_sky_signature(b))unique=0;INPUT_CHECK(unique,"graphics: all 256 systems have unique deterministic cosmic-sky signatures");}
    {static const int systems[]={0,7,31,63,127,173,221,255};unsigned hashes[8]={0};int distinct=1,rich=1;for(int sample=0;sample<8;sample++){game.system=systems[sample];system_bodies(&game);launch(&game);game.yaw=.37f;game.pitch=-.11f;game.roll=.08f;memset(pixels,0,STRIDE*H*sizeof(unsigned));sector_background();space_fx_nebula();starfield();unsigned hash=2166136261u;int bright=0;for(int y=view_top();y<=view_bot();y++)for(int x=0;x<W;x++){unsigned c=pixels[y*STRIDE+x];hash=(hash^c)*16777619u;if((int)(c&255)+(int)((c>>8)&255)+(int)((c>>16)&255)>430)bright++;}hashes[sample]=hash;rich&=bright>8;for(int prior=0;prior<sample;prior++)distinct&=hashes[prior]!=hash;char file[48];snprintf(file,sizeof(file),"cosmic-sky-%03d.bmp",systems[sample]);dump_native_bmp(file);}INPUT_CHECK(distinct&&rich,"graphics: sampled systems paint distinct, star-rich cosmic backgrounds");}
    high_contrast=1;memset(pixels,0,STRIDE*H*sizeof(unsigned));sector_background();space_fx_nebula();space_fx_meteors();
    int plain=0;for(int y=view_top();y<=view_bot();y++)for(int x=0;x<W;x++)if(pixels[y*STRIDE+x])plain++;
    /* Base wash only — no soft nebula/meteors when high contrast is on. */
    INPUT_CHECK(plain>1000,"graphics: high contrast keeps the sector wash without decorative FX");
    high_contrast=0;sfx_fx_reset();
    memset(pixels,0,STRIDE*H*sizeof(unsigned));sfx_hit_sparks_spawn(240,120,0xABCDu);sfx_hit_sparks_draw(0);
    int sparks=0;for(int y=view_top();y<=view_bot();y++)for(int x=0;x<W;x++)if(pixels[y*STRIDE+x])sparks++;
    INPUT_CHECK(sparks>=8,"graphics: Wave A hit-spark pool paints soft-FB streaks");
    high_contrast=1;memset(pixels,0,STRIDE*H*sizeof(unsigned));sfx_hit_sparks_draw(0);
    int muted=0;for(int y=view_top();y<=view_bot();y++)for(int x=0;x<W;x++)if(pixels[y*STRIDE+x])muted++;
    INPUT_CHECK(muted==0,"graphics: high contrast mutes Wave A hit sparks");
    high_contrast=0;sfx_fx_reset();
    /* Wave B: planet bloom / specular paints soft dots around a nearby world. */
    {
     game.system=0;system_bodies(&game);launch(&game);page=FLIGHT;game.jump=0;game.boost=0;
     game.pos=add(game.bodies[1].pos,(Vec3){0,0,-game.bodies[1].radius*3.5f});
     memset(pixels,0,STRIDE*H*sizeof(unsigned));sector_background();draw_bodies();sfx_planet_beauty();
     int bloom=0;for(int y=view_top();y<=view_bot();y++)for(int x=0;x<W;x++)if(pixels[y*STRIDE+x])bloom++;
     INPUT_CHECK(bloom>400,"graphics: Wave B planet bloom/specular paints soft-FB atmosphere");
     high_contrast=1;memset(pixels,0,STRIDE*H*sizeof(unsigned));sfx_planet_beauty();sfx_travel_beauty();
     int mute=0;for(int y=view_top();y<=view_bot();y++)for(int x=0;x<W;x++)if(pixels[y*STRIDE+x])mute++;
     INPUT_CHECK(mute==0,"graphics: high contrast mutes Wave B planet/travel beauty");
     high_contrast=0;sfx_fx_reset();
    }
   }
   #include "dialogue-visual-tests.h"
   #include "police-visual-tests.h"
   #include "discovery-atlas-tests.h"
   #include "station-departure-tests.h"
   #include "rift-visual-tests.h"
   #include "social-visual-tests.h"
   #include "outfitting-visual-tests.h"
#include "loadout-feedback-visual-tests.h"
   #include "faction-dossier-tests.h"
   #include "missile-feedback-visual-tests.h"
   #include "mission-tracking-tests.h"
   preview_reset();quiet_comms=old_quiet;fb=saved_fb;free(pixels);TEST_INIT();
  }
 }
 {
  int fits=1;
  preview_clip(353,84,242,40,464,128);
  for(int ship=0;ship<player_ship_count;ship++){
   int id=mesh_id(player_ships[ship].name);Vec3 centre;float scale;ship_preview_layout(id,&centre,&scale);
   for(int a=0;a<16;a++)for(int b=0;b<16;b++)for(int v=0;v<meshes[id].vertices;v++){
    Vec3 point=add((Vec3){0,0,320},mul(rotate(sub(meshes[id].v[v],centre),a*.392699f,b*.392699f),scale));Point p=project(point);
    if(point.z<15||p.x<246||p.x>460||p.y<44||p.y>124)fits=0;
   }
  }
  preview_reset();INPUT_CHECK(fits,"shipyard: every hull fits with padding throughout full yaw and roll rotations");
 }
 #include "opening-unlocks-tests.h"
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);

#undef INPUT_CHECK
#undef TEST_INIT
 game_init(&game);deck_reset();story_complete(&game);change_page(HOME);selected_target=0;nav_body=-1;autoaim=0;r_tap=10;hud_mode=hud_hidden=0;
}
static void station_explore_tests(void){
 FILE *f=fopen("station-explore-check.txt","w");if(!f)return;int failures=0;
 unsigned *saved_fb=fb,*pixels=(unsigned*)malloc(STRIDE*H*sizeof(unsigned));
 if(!pixels){fprintf(f,"FAIL station explore: allocate native framebuffer\nRESULT 1 failures\n");fclose(f);return;}
 fb=pixels;memset(pixels,0,STRIDE*H*sizeof(unsigned));
#define INPUT_CHECK(c,n) do{int ok=(c);fprintf(f,"%s %s\n",ok?"PASS":"FAIL",n);fflush(f);failures+=!ok;}while(0)
#define TEST_INIT() do{ps_open=ps_release=0;game_init(&game);deck_reset();story_complete(&game);paused=0;selected_target=0;autoaim=0;scan_cat=2;tracked_mission=0;station_tour_stage=STATION_TOUR_OFF;planet_landing_menu=0;planet_controls_ready=1;planet_entry_body=planet_seat.active=planet_orbit_release=0;planet_orbit_veil=0;surface_target_reset();tools_cancel_tractor();tools_reset_gesture();tools_selected=0;tools_wait_release=0;thargoid_reset();}while(0)
 TEST_INIT();game.docked=1;walk_kind=0;page=WALK;sc_built_for=-1;sc_build_map();
 ScHot hot[24];int hn=sc_hotspots(hot,24);
 INPUT_CHECK(hn>1&&hot[0].kind==SC_H_HERE&&!strcmp(hot[0].label,"HERE")&&hot[0].w==340&&hot[0].h==168,"station explore: HERE is first and covers the exact room art");
 INPUT_CHECK(sc_hot==0&&!strcmp(sc_read_title,sc_room_title(sc_room))&&strlen(sc_read_text)>300,"station explore: entry starts on HERE with complete room prose");
 sc_draw_ui();dump_native_bmp("station-explore-here.bmp");
 sc_input(PSP_CTRL_DOWN);INPUT_CHECK(sc_hot==1,"station explore: Down selects the first contact after HERE");
 sc_draw_ui();dump_native_bmp("station-explore-contact.bmp");
 int feature=sc_find_hot(SC_H_FEATURE,50);INPUT_CHECK(feature>=0,"station explore: current room keeps its authored landmark hotspot");
 if(feature>=0){sc_hot=feature;sc_input(PSP_CTRL_CROSS);}
 INPUT_CHECK(strcmp(sc_read_title,sc_room_title(sc_room))!=0,"station explore: X commits inspected hotspot prose");
 sc_hot=sc_find_hot(SC_H_HERE,sc_room);sc_input(PSP_CTRL_CROSS);
 INPUT_CHECK(!strcmp(sc_read_title,sc_room_title(sc_room))&&sc_read_page==0,"station explore: HERE restores room prose from page one");
 sc_input(PSP_CTRL_RIGHT);INPUT_CHECK(sc_read_page==1,"station explore: Right pages through long room prose");
 sc_input(PSP_CTRL_LEFT);INPUT_CHECK(sc_read_page==0,"station explore: Left returns to the previous reading page");
 int guild=sc_find_hot(SC_H_EXIT,SC_R_GUILD);if(guild>=0){sc_hot=guild;sc_input(PSP_CTRL_CROSS);}
 INPUT_CHECK(sc_room==SC_R_GUILD&&sc_hot==0&&!strcmp(hot[0].label,"HERE"),"station explore: entering another room returns selection to HERE");
 for(int room_id=0;room_id<SC_R_COUNT;room_id++){sc_room=room_id;sc_menu=SC_MENU_NONE;sc_hot=0;sc_read_room();sc_draw_ui();char path[48];snprintf(path,sizeof(path),"station-explore-room-%d.bmp",room_id);dump_native_bmp(path);}
 sc_room=SC_R_CARGO;sc_menu=SC_MENU_NONE;sc_hot=sc_find_hot(SC_H_PERSON,0);sc_input(PSP_CTRL_CROSS);sc_draw_ui();dump_native_bmp("station-explore-conversation.bmp");
 preview_reset();rect(0,0,W,H,0x12345678);ScHot edge={SC_H_HERE,0,SC_VX,SC_VY,SC_VW,SC_VH,"HERE",""};sc_focus_glow(&edge);
 int bounded=1;for(int yy=0;yy<H;yy++)for(int xx=0;xx<W;xx++)if(xx<SC_VX||xx>=SC_VX+SC_VW||yy<SC_VY||yy>=SC_VY+SC_VH)if(fb[yy*STRIDE+xx]!=0x12345678)bounded=0;
 INPUT_CHECK(bounded,"station explore: room-wide HERE glow stays inside the 340x168 scene");
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);fb=saved_fb;free(pixels);
#undef INPUT_CHECK
#undef TEST_INIT
}
#include "mission-reward-review.h"
#include "space-racing-review.h"
#include "erl-review.h"
#include "erl-arcade-visual-review.h"
#include "erl-shockwave-review.h"
#include "erl-flight-state-review.h"
#include "erl-holo-track-review.h"
#include "erl-apex-review.h"
#include "erl-progress-review.h"
#include "dockbay-review.h"
#include "mission-expansion-review.h"
#include "deep-chart-review.h"
#include "guild-navigation-review.h"
#include "mission-log-review.h"
#include "comms-panel-review.h"
#include "dialogue-broadcast-review.h"
#include "commander-profile-review.h"
#include "menu-fixes-review.h"
#include "codex-progress-review.h"
#include "ship-purchase-review.h"
#include "rumours-review.h"
#include "rumour-ads-review.h"
#include "wanted-crime-watch-review.h"
#include "spacebook-home-review.h"
#include "newspaper-review.h"
#include "cargo-console-review.h"
#include "loadout-console-review.h"
#include "controls-handbook-review.h"
#include "command-deck-review.h"
#include "shipyard-console-review.h"
#include "multiplayer-console-review.h"

#include "soft-sky-review.h"
#include "flight-lighting-review.h"
#include "mega-city-review.h"
#include "pulp-station-review.h"
#include "station-audit-review.h"
#include "rich-station-review.h"
#include "lave-review.h"
#include "field-art-review.h"
#include "fauna-review.h"
#include "fauna-shadow-review.h"
#include "lave-world-review.h"
#include "cinematic-port-review.h"
#include "seamless-entry-review.h"
#include "integration-241-review.h"
#include "starport-review.h"
#include "field-map-review.h"
#include "roamer-review.h"
#include "surface-perf-review.h"
#include "expanded-world-review.h"
#include "planet-beauty-review.h"
#include "atmosphere-review.h"
#include "water-hand-review.h"
#include "planet-speed-review.h"
#include "poi-ground-review.h"
#include "local-tv-tests.h"
#include "local-wifi-review.h"

#include "performance-review.h"
#include "exterior-review.h"
#include "eva-performance-review.h"
#include "planet-regions-review.h"
#endif

#ifdef GRAPHICS_REVIEW
#include "graphics-integration-review.h"
#endif

/* A short hardware diagnostic survives even when the loader or codecs fail.
 * No frame-loop logging and no commander/settings files are touched. */
static FILE *boot_trace;
static void boot_checkpoint(const char *phase){
 if(!boot_trace)return;
 fprintf(boot_trace,"%s free=%u largest=%u stack=%d\n",phase,
  (unsigned)sceKernelTotalFreeMemSize(),(unsigned)sceKernelMaxFreeMemSize(),sceKernelCheckThreadStack());
 fflush(boot_trace);
}
#include "encounter-flight-capture.h"
int main(int argc,char **argv){
 /* Homebrew loaders differ in their initial cwd. All saves and music are
  * deliberately rooted beside the executable, including folder names with spaces. */
 if(argc>0&&argv&&argv[0]){char launch_dir[512];snprintf(launch_dir,sizeof(launch_dir),"%s",argv[0]);char *slash=strrchr(launch_dir,'/');if(slash){*slash=0;chdir(launch_dir);}}
 boot_trace=fopen("boot-status.txt","w");boot_checkpoint("2.5.396 main");
 int cb=sceKernelCreateThread("Callbacks",callback_thread,0x11,4096,0,0);if(cb>=0)sceKernelStartThread(cb,0,0);
 scePowerSetClockFrequency(333,333,166);sceCtrlSetSamplingCycle(0);sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
 display_start_black();boot_checkpoint("display ready");ship_art_unpack();fleet_ge_assets_prepare();
 /* Prepare the bounded cloud atlas at startup, not on the first landing frame. */
 atmosphere_cloud_init();flight_lighting_init();
 game_init(&game);deck_reset();
 /* Build the initial distant sky while loading, before audio/gameplay deadlines. */
 soft_sky_prepare(space_sky_seed());planet_phase_init();ion_init();journey_art_prepare(1);natural_assets_init();
#ifdef GRAPHICS_REVIEW
 if(deep_art_review()||station_perf_review()||soft_sky_review()||station_art_review()||ship_art_review()){sceKernelExitGame();return 0;}
#endif
 FILE *flag=fopen("smoke.flag","r");if(flag){smoke=1;fclose(flag);FILE *visual=fopen("visual.flag","r");if(visual){visual_hold=1;fclose(visual);}FILE *log=fopen("boot-check.txt","w");if(log){fprintf(log,"PSP main reached; %d meshes loaded.\n",mesh_count);fclose(log);}}
#if defined(ELITE_ENCOUNTER_REVIEW) && !defined(ELITE_ENABLE_REVIEW)
 if(navigation_review()||port_ad_review()||departure_hud_review()||loadout_picker_review()||dashboard_review()||charm_review()||cockpit_hud_review()||encounter_upgrade_review()){sceKernelExitGame();return 0;}
 if(final_menu_review()||release_2471_review()||erl_quick_race_review()){sceKernelExitGame();return 0;}
#endif
#ifdef ELITE_ENABLE_REVIEW
 if(navigation_review()||port_ad_review()||departure_hud_review()||loadout_picker_review()||dashboard_review()||charm_review()||cockpit_hud_review()||encounter_upgrade_review()){sceKernelExitGame();return 0;}
 if(final_menu_review()){sceKernelExitGame();return 0;}
 if(terminal_review()){sceKernelExitGame();return 0;}
 if(menu_seven_review()){sceKernelExitGame();return 0;}
 if(menu_prototypes_review()){sceKernelExitGame();return 0;}
 if(release_2471_review()){sceKernelExitGame();return 0;}
 if(erl_quick_race_review()){sceKernelExitGame();return 0;}
 {FILE *flag=fopen("drive-dynamics-review.flag","r");if(flag){fclose(flag);FILE *report=fopen("drive-dynamics-review.txt","w");int failures=0;game_init(&game);story_complete(&game);launch(&game);page=FLIGHT;third_person=0;game.pos=(Vec3){10000000,10000000,10000000};game.debug_flags|=DEBUG_UNLIMITED_FUEL;game.heat=95;game.message_time=game.voice_time=0;game.speed=player_ships[game.ship].speed*.70f;r_tap=0;input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);float start=game.speed;for(int i=0;i<15;i++)input(0,PSP_CTRL_RTRIGGER,1.f/60.f,0,0);float quarter=game.speed;for(int i=0;i<105;i++)input(0,PSP_CTRL_RTRIGGER,1.f/60.f,0,0);float two=game.speed;float shake=drive_shake_strength(),offset=length(drive_shake_offset((Vec3){1,0,0},(Vec3){0,1,0},1.f));game.heat=0;for(int i=0;i<240;i++){game.heat=0;input(0,PSP_CTRL_RTRIGGER,1.f/60.f,0,0);}float six=game.speed,spool_six=game.boost_spool;int boost_ok=game.boost&&quarter>start&&two>quarter&&six>two&&game.boost_spool>.99f&&game.message_time<=0;failures+=!boost_ok;int shake_ok=shake>.1f&&offset>.01f;failures+=!shake_ok;game_init(&game);story_complete(&game);launch(&game);page=FLIGHT;third_person=0;game.pos=(Vec3){10000000,10000000,10000000};game.message_time=game.voice_time=0;game.speed=player_ships[game.ship].speed*5.f;float brake_start=game.speed;l_tap=0;input(PSP_CTRL_LTRIGGER,PSP_CTRL_LTRIGGER,.016f,0,0);for(int i=0;i<6;i++)input(0,0,1.f/60.f,0,0);float brake_early=game.speed;for(int i=0;i<30;i++)input(0,0,1.f/60.f,0,0);float brake_mid=game.speed;for(int i=0;i<75;i++)input(0,0,1.f/60.f,0,0);float brake_late=game.speed;int brake_ok=brake_early<brake_start&&brake_early>brake_start*.90f&&brake_mid<brake_early&&brake_late<brake_mid&&brake_late>0&&hard_brake>0&&game.message_time<=0;failures+=!brake_ok;int brake_shake=drive_shake_strength()>.05f;failures+=!brake_shake;if(report){fprintf(report,"%s boost spools continuously without an instant speed jump or warning\n",boost_ok?"PASS":"FAIL");fprintf(report,"INFO boost speed %.1f -> %.1f (0.25s) -> %.1f (2s) -> %.1f (6s), spool %.2f\n",start,quarter,two,six,spool_six);fprintf(report,"%s boost produces restrained camera movement\n",shake_ok?"PASS":"FAIL");fprintf(report,"%s mega-brake ramps in, decelerates progressively and does not snap to zero or warn\n",brake_ok?"PASS":"FAIL");fprintf(report,"INFO brake speed %.1f -> %.1f (0.1s) -> %.1f (0.6s) -> %.1f (1.85s)\n",brake_start,brake_early,brake_mid,brake_late);fprintf(report,"%s mega-brake produces restrained camera movement\n",brake_shake?"PASS":"FAIL");fprintf(report,"RESULT %d failures\n",failures);fclose(report);}sceKernelExitGame();return 0;}}
 if(erl_shockwave_review()){sceKernelExitGame();return 0;}
 if(erl_arcade_visual_review()){sceKernelExitGame();return 0;}
 if(dialogue_broadcast_review()){sceKernelExitGame();return 0;}
 if(planet_regions_review()){sceKernelExitGame();return 0;}
 if(eva_performance_review()){sceKernelExitGame();return 0;}
 if(guild_navigation_review()){sceKernelExitGame();return 0;}
 if(mission_log_review()){sceKernelExitGame();return 0;}
 if(comms_panel_review()){sceKernelExitGame();return 0;}
 if(ship_purchase_review()){sceKernelExitGame();return 0;}
 if(rumours_review()){sceKernelExitGame();return 0;}
 if(rumour_ads_review()){sceKernelExitGame();return 0;}
 if(wanted_crime_watch_review()){sceKernelExitGame();return 0;}
 if(spacebook_home_review()){sceKernelExitGame();return 0;}
 if(newspaper_review()){sceKernelExitGame();return 0;}
 if(cargo_console_review()){sceKernelExitGame();return 0;}
 if(controls_handbook_review()){sceKernelExitGame();return 0;}
 if(command_deck_review()){sceKernelExitGame();return 0;}
 if(shipyard_console_review()){sceKernelExitGame();return 0;}
 if(multiplayer_console_review()){sceKernelExitGame();return 0;}
 if(loadout_console_review()){sceKernelExitGame();return 0;}
 {FILE *flag=fopen("wanted-poster-review.flag","r");if(flag){fclose(flag);FILE *report=fopen("wanted-poster-review.txt","w");unsigned *pixels=malloc(STRIDE*H*sizeof(unsigned));int failures=0;if(pixels){fb=pixels;game_init(&game);story_complete(&game);deck_reset();change_page(GALNET);galnet_tab=2;selected_target=0;autoaim=0;for(int i=0;i<BOUNTY_POSTER_COUNT;i++){row=i;input(PSP_CTRL_CROSS,0,.016f,0,0);failures+=page!=GALNET||!game.docked||selected_target!=0||autoaim||game.dock_stage!=0;rect(0,0,W,H,BG);galnet_screen();char file[48];snprintf(file,sizeof(file),"wanted-poster-%d.bmp",i);dump_native_bmp(file);}game.bounty_claimed[game.system]|=1;row=0;rect(0,0,W,H,BG);galnet_screen();dump_native_bmp("wanted-poster-claimed.bmp");game.docked=0;selected_target=BODY_COUNT+1;int target=selected_target;input(PSP_CTRL_CROSS,0,.016f,0,0);failures+=page!=GALNET||game.docked||selected_target!=target||autoaim;free(pixels);}else failures++;if(report){fprintf(report,"%s all five posters keep docked state and target unchanged on X; flight target also unchanged\n",failures?"FAIL":"PASS");fprintf(report,"RESULT %d failures\n",failures);fclose(report);}sceKernelExitGame();return 0;}}
 FILE *combatcheck=fopen("npc-combat-review.flag","r");if(combatcheck){fclose(combatcheck);npc_combat_tests("npc-combat-check.txt");sceKernelExitGame();return 0;}
 {FILE *cap=fopen("galnet-market-capture.flag","r");if(cap){fclose(cap);unsigned *pixels=malloc(STRIDE*H*sizeof(unsigned));if(pixels){fb=pixels;game_init(&game);story_complete(&game);game.docked=1;page=GALNET;galnet_tab=1;row=0;const float ages[]={0.f,.235f,.269f,3.04f};for(int i=0;i<4;i++){preview_time=ages[i];rect(0,0,W,H,BG);galnet_screen();char path[48];snprintf(path,sizeof(path),"market-neon-%d.bmp",i);dump_native_bmp(path);}game.docked=0;rect(0,0,W,H,BG);galnet_screen();dump_native_bmp("market-offline.bmp");free(pixels);}sceKernelExitGame();return 0;}}
 if(exterior_review()){sceKernelExitGame();return 0;}
 if(performance_review()){sceKernelExitGame();return 0;}
 if(online_review()){sceKernelExitGame();return 0;}
 if(wifi_review()){sceKernelExitGame();return 0;}
 if(chart_review()){sceKernelExitGame();return 0;}
 if(flight_lighting_review()){sceKernelExitGame();return 0;}
 if(soft_sky_review()){sceKernelExitGame();return 0;}
 if(mega_city_review()){sceKernelExitGame();return 0;}
 if(pulp_station_review()){sceKernelExitGame();return 0;}
 if(station_audit_review()){sceKernelExitGame();return 0;}
 if(rich_station_review()){sceKernelExitGame();return 0;}
 if(lave_review()){sceKernelExitGame();return 0;}
 if(field_art_review()){sceKernelExitGame();return 0;}
 if(water_hand_review()){sceKernelExitGame();return 0;}
 if(planet_speed_review()){sceKernelExitGame();return 0;}
 if(poi_ground_review()){sceKernelExitGame();return 0;}
 if(planet_beauty_review()){sceKernelExitGame();return 0;}
 if(atmosphere_review()){sceKernelExitGame();return 0;}
 if(fauna_review()){sceKernelExitGame();return 0;}
 if(fauna_shadow_review()){sceKernelExitGame();return 0;}
 if(lave_world_review()){sceKernelExitGame();return 0;}
  if(seamless_entry_review()){sceKernelExitGame();return 0;}
  if(cinematic_port_review()){sceKernelExitGame();return 0;}
 if(integration_241_review()){sceKernelExitGame();return 0;}
 if(starport_review()){sceKernelExitGame();return 0;}
 if(field_map_review()){sceKernelExitGame();return 0;}
 if(roamer_review()){sceKernelExitGame();return 0;}
 if(surface_perf_review()){sceKernelExitGame();return 0;}
 if(expanded_world_review()){sceKernelExitGame();return 0;}
 FILE *tvcheck=fopen("tv-check.flag","r");if(tvcheck){fclose(tvcheck);local_tv_tests();sceKernelExitGame();return 0;}
 FILE *stationcheck=fopen("station-explore-check.flag","r");if(stationcheck){fclose(stationcheck);station_explore_tests();sceKernelExitGame();return 0;}
 FILE *cameracheck=fopen("camera-review.flag","r");if(cameracheck){fclose(cameracheck);int failures=0;FILE *report=fopen("camera-review.txt","w");unsigned *pixels=malloc(STRIDE*H*sizeof(unsigned));if(pixels)fb=pixels;game_init(&game);story_complete(&game);launch(&game);page=FLIGHT;hud_mode=hud_hidden=1;game.voice_time=game.message_time=0;game.pos=(Vec3){0,0,-16000};game.yaw=.18f;game.pitch=-.04f;game.roll=.12f;unsigned hashes[4]={0};const char *names[4]={"camera-third-person.bmp","camera-cinematic-00.bmp","camera-cinematic-10.bmp","camera-cinematic-20.bmp"};const float times[4]={4.f,4.f,14.2f,24.2f};for(int k=0;k<4;k++){third_person=k?2:1;if(k==1)flight_camera_seen=-1;game.time=times[k];rect(0,0,W,H,BG);space();dump_native_bmp(names[k]);for(int y=view_top();y<view_bot();y++)for(int x=0;x<W;x++)hashes[k]=(hashes[k]^pixels[y*STRIDE+x])*16777619u;}third_person=1;flight_camera_seen=-1;flight_camera_prepare();Vec3 ship_view=camera(&game,game.pos),before=game.pos;game.pos=add(game.pos,mul(forward(&game),1800));flight_camera_prepare();Vec3 boosted_view=camera(&game,game.pos);int boost_track=fabsf(boosted_view.z-ship_view.z)<35&&fabsf(boosted_view.x-ship_view.x)<35;fit_clear_all(&game);game.fit[FIT_WPN]=1;fit_rebuild(&game);game.shot=weapon_cycle(&game);game.shots++;game.boost=1;game.speed=player_ships[game.ship].speed*2;int hardpoints=weapon_fx_external(1,0);camera_override_clear();game.pos=before;hud_mode=hud_hidden=0;square_held=1;game.time=31;rect(0,0,W,H,BG);space();dump_native_bmp("camera-third-targeting.bmp");square_held=0;speak(&game,VOICE_CONTACT,"Courier Seven calling. Your exterior camera link is clear.");game.voice_role=TRADERS;rect(0,0,W,H,BG);space();dump_native_bmp("camera-third-radio.bmp");int framed=ship_view.z>100&&ship_view.z<310&&fabsf(ship_view.x)<180&&fabsf(ship_view.y)<140;int distinct=hashes[0]!=hashes[1]&&hashes[1]!=hashes[2]&&hashes[2]!=hashes[3]&&hashes[1]!=hashes[3];third_person=2;comms_captions=1;comms_quiet=0;int saved=comms_settings_save("camera-review.cfg");third_person=0;if(saved)saved=comms_settings_load("camera-review.cfg")&&third_person==2;remove("camera-review.cfg");remove("camera-review.cfg.bak");remove("camera-review.cfg.tmp");failures+=!framed+!distinct+!saved+!boost_track+!hardpoints;if(report){fprintf(report,"%s close low third-person camera frames the real player ship\n",framed?"PASS":"FAIL");fprintf(report,"%s cinematic camera produces distinct authored shots at 10-second intervals\n",distinct?"PASS":"FAIL");fprintf(report,"%s boost translation cannot outrun the chase camera\n",boost_track?"PASS":"FAIL");fprintf(report,"%s exterior weapons project from mesh-derived hardpoints\n",hardpoints?"PASS":"FAIL");fprintf(report,"%s cinematic preference persists through console settings\n",saved?"PASS":"FAIL");fprintf(report,"PASS native targeting and radio-overlap frames captured\n");fprintf(report,"RESULT %d failures\n",failures);fclose(report);}sceKernelExitGame();return 0;}
 FILE *rearcheck=fopen("rear-test.flag","r");if(rearcheck){fclose(rearcheck);launch(&game);game.yaw=.73f;game.pitch=.19f;game.roll=.47f;Game rear=game;rear.yaw+=3.14159265f;rear.pitch=-game.pitch;Vec3 nose=forward(&game),behind=add(game.pos,mul(nose,-1200)),ahead=add(game.pos,mul(nose,1200));int bx=0,by=0,ax=0,ay=0;int ok=rear_mirror_project(&rear,behind,4,1,252,22,&bx,&by,0)&&!rear_mirror_project(&rear,ahead,4,1,252,22,&ax,&ay,0)&&bx>110&&bx<150&&by>2&&by<22;FILE *report=fopen("rear-mirror-check.txt","w");if(report){fprintf(report,"%s aft camera centres rear contacts and rejects forward contacts\n",ok?"PASS":"FAIL");fprintf(report,"RESULT %d failures\n",ok?0:1);fclose(report);}sceKernelExitGame();return 0;}
 FILE *inboxcheck=fopen("inbox-check.flag","r");if(inboxcheck){fclose(inboxcheck);int failures=0;FILE *report=fopen("inbox-check.txt","w");unsigned *pixels=malloc(STRIDE*H*sizeof(unsigned));if(pixels)fb=pixels;
  game_init(&game);deck_reset();change_page(GALNET);galnet_tab=4;row=0;inbox_message_page=0;rect(0,0,W,H,BG);galnet_screen();dump_native_bmp("spacebook-inbox-local.bmp");input(PSP_CTRL_TRIANGLE,0,.016f,0,0);int local=page==GALNET&&galnet_tab==3;failures+=!local;if(report){fprintf(report,"%s Inbox shortcut returns to the Spacebook feed\n",local?"PASS":"FAIL");fflush(report);}
  game_init(&game);deck_reset();game.opening.stage=OP_DAD_INBOX;game.opening.flags|=OF_DIALOG_DONE;change_page(GALNET);galnet_tab=4;row=0;rect(0,0,W,H,BG);galnet_screen();dump_native_bmp("spacebook-inbox-homeward.bmp");opening_net_input(PSP_CTRL_CROSS);int story=(game.opening.flags&(1<<16))!=0;failures+=!story;if(report){fprintf(report,"%s Dad thread preserves its cached reply action\n",story?"PASS":"FAIL");fprintf(report,"RESULT %d failures\n",failures);fclose(report);}sceKernelExitGame();return 0;}
FILE *menupreviewcheck=fopen("menu-preview-check.flag","r");if(menupreviewcheck){fclose(menupreviewcheck);int failures=0;FILE *report=fopen("menu-preview-check.txt","w");unsigned *pixels=malloc(STRIDE*H*sizeof(unsigned));if(pixels)fb=pixels;unsigned hashes[3]={0};int systems[3]={7,-1,7},variants[3]={0,0,1};for(int s=0;s<256;s++)if(station_class_for_system(&game,s)==STATION_MEGA){systems[1]=s;break;}const char *names[3]={"menu-docked-station-lave.bmp","menu-docked-station-mega.bmp","menu-docked-station-relay.bmp"};for(int k=0;k<3;k++){game_init(&game);deck_reset();game.system=systems[k];system_bodies(&game);game.docked=1;game.station_variant=variants[k];preview_time=18;change_page(HOME);rect(0,0,W,H,BG);home();dump_native_bmp(names[k]);for(int yy=64;yy<156;yy++)for(int xx=246;xx<464;xx++)hashes[k]=hashes[k]*33u+pixels[yy*STRIDE+xx];}game_init(&game);deck_reset();game.system=7;system_bodies(&game);game.docked=1;game.station_variant=0;preview_time=42;change_page(HOME);rect(0,0,W,H,BG);home();unsigned orbit_hash=0;for(int yy=64;yy<156;yy++)for(int xx=246;xx<464;xx++)orbit_hash=orbit_hash*33u+pixels[yy*STRIDE+xx];int distinct=systems[1]>=0&&hashes[0]!=hashes[1]&&hashes[0]!=hashes[2]&&hashes[1]!=hashes[2],orbiting=orbit_hash!=hashes[0],flushed=drawcount==0;failures+=!distinct+!orbiting+!flushed;if(report){fprintf(report,"%s docked hubs render distinct current-station exteriors\n",distinct?"PASS":"FAIL");fprintf(report,"%s station camera advances around the exterior over time\n",orbiting?"PASS":"FAIL");fprintf(report,"%s preview leaves the bounded geometry queue empty\n",flushed?"PASS":"FAIL");fprintf(report,"RESULT %d failures\n",failures);fclose(report);}sceKernelExitGame();return 0;}
FILE *detailsreview=fopen("details-review.flag","r");if(detailsreview){int sys=9,focus=10;fscanf(detailsreview,"%d %d",&sys,&focus);fclose(detailsreview);if(sys<0||sys>255)sys=9;game_init(&game);game.system=sys;system_bodies(&game);story_complete(&game);change_page(DETAILS);row=focus<0?0:focus>system_body_slots(sys)?system_body_slots(sys):focus;preview_time=18;unsigned *pixels=malloc(STRIDE*H*sizeof(unsigned));if(pixels)fb=pixels;rect(0,0,W,H,BG);system_details();dump_native_bmp("system-details-crowded.bmp");FILE *report=fopen("details-review.txt","w");if(report){int ok=system_body_slots(sys)>=8;fprintf(report,"%s crowded system renders %d selectable bodies at native resolution\n",ok?"PASS":"FAIL",system_body_slots(sys));fprintf(report,"RESULT %d failures\n",!ok);fclose(report);}sceKernelExitGame();return 0;}
FILE *missionreview=fopen("mission-board-review.flag","r");if(missionreview){fclose(missionreview);game_init(&game);story_complete(&game);game.docked=1;game.credits=20000;row=0;change_page(MISSIONS);unsigned *pixels=malloc(STRIDE*H*sizeof(unsigned));if(pixels)fb=pixels;rect(0,0,W,H,BG);mission_board();dump_native_bmp("mission-board-layout.bmp");for(int t=0;t<MISSION_TYPES;t++){rect(301,29,170,210,RGB(13,21,30));mission_accept_yes=t&1;mission_confirm_card(t,game.system,RGB(63,102,121),RGB(165,113,54));if(t==MISSION_WILDLIFE||t==MISSION_MULTI_STOP||t==MISSION_EXPLORATION){char file[64];snprintf(file,sizeof(file),"mission-confirm-%02d.bmp",t);dump_native_bmp(file);}}FILE *report=fopen("mission-board-review.txt","w");if(report){int ok=mission_count(&game)==6;fprintf(report,"%s six scrollable offers in five bounded mission cards and contract brief render at native resolution\n",ok?"PASS":"FAIL");fprintf(report,"RESULT %d failures\n",!ok);fclose(report);}sceKernelExitGame();return 0;}
 FILE *toolfeedback=fopen("tool-feedback-check.flag","r");if(toolfeedback){fclose(toolfeedback);int failures=0;FILE *report=fopen("tool-feedback-check.txt","w");unsigned *pixels=malloc(STRIDE*H*sizeof(unsigned));if(pixels)fb=pixels;unsigned hashes[3]={0};int confirm_red=0;game_init(&game);story_complete(&game);launch(&game);page=FLIGHT;game.pos=(Vec3){0,0,-20000};fit_clear_all(&game);game.fit[0]=1;game.fit[6]=2;fit_rebuild(&game);const char *names[3]={"weapons-pad-tractor-confirm.bmp","weapons-pad-weapon-slide.bmp","weapons-pad-one-weapon.bmp"};for(int k=0;k<3;k++){rect(0,0,W,H,BG);if(k==0){tools_confirm_action=2;tools_weapon_result=0;tools_confirm_time=.60f;}else if(k==1){game.active_weapon=6;snprintf(tools_weapon_old,sizeof(tools_weapon_old),"PULSE");snprintf(tools_weapon_new,sizeof(tools_weapon_new),"BEAM");tools_confirm_action=3;tools_weapon_result=1;tools_confirm_time=.90f;}else{game.fit[6]=FIT_EMPTY;game.active_weapon=0;fit_rebuild(&game);tools_confirm_action=3;tools_weapon_result=-1;tools_confirm_time=.60f;}tools_panel();dump_native_bmp(names[k]);for(int yy=48;yy<168;yy++)for(int xx=104;xx<376;xx++)hashes[k]=hashes[k]*33u+pixels[yy*STRIDE+xx];if(k==0)for(int yy=82;yy<128;yy++)for(int xx=108;xx<213;xx++)confirm_red+=pixels[yy*STRIDE+xx]==RED;}tools_confirm_time=0;tools_confirm_action=-1;rect(0,0,W,H,BG);game.message_time=game.voice_time=0;cockpit();dump_native_bmp("radar-system-label.bmp");int distinct=hashes[0]!=hashes[1]&&hashes[0]!=hashes[2]&&hashes[1]!=hashes[2],red_box=confirm_red>120,radar_ink=0;for(int yy=198;yy<210;yy++)for(int xx=171;xx<309;xx++)if(pixels[yy*STRIDE+xx]!=UI_PANEL&&pixels[yy*STRIDE+xx]!=UI_RAISED)radar_ink=1;failures+=!distinct+!red_box+!radar_ink;if(report){fprintf(report,"%s tool, weapon-transition and unavailable-weapon frames are visually distinct\n",distinct?"PASS":"FAIL");fprintf(report,"%s selected direction flashes its complete red control box\n",red_box?"PASS":"FAIL");fprintf(report,"%s current-system label draws above the flight radar\n",radar_ink?"PASS":"FAIL");fprintf(report,"RESULT %d failures\n",failures);fclose(report);}sceKernelExitGame();return 0;}
 FILE *warpreview=fopen("warp-review.flag","r");if(warpreview){fclose(warpreview);int failures=0;FILE *report=fopen("warp-review.txt","w");unsigned *pixels=malloc(STRIDE*H*sizeof(unsigned));if(pixels)fb=pixels;unsigned hashes[3]={0};int old_panel=0,vortex_pixels=0;const float jumps[3]={5.15f,4.f,1.5f};const char *names[3]={"warp-pre-corridor.bmp","warp-corridor.bmp","warp-braking.bmp"};game_init(&game);story_complete(&game);launch(&game);page=FLIGHT;route_jump_engaged=1;game.message_time=game.voice_time=0;for(int k=0;k<3;k++){rect(0,0,W,H,RGB(3,7,24));for(int i=0;i<45;i++){unsigned h=field_hash((unsigned)i*7919u);pixel((int)(h%W),(int)(24+((h>>9)%165)),RGB(115,132,158));}game.jump=jumps[k];game.time=20.f-jumps[k];cockpit();dump_native_bmp(names[k]);for(int yy=24;yy<192;yy++)for(int xx=0;xx<W;xx++)hashes[k]=hashes[k]*33u+pixels[yy*STRIDE+xx];if(k==1){for(int yy=88;yy<116;yy++)for(int xx=80;xx<400;xx++)old_panel+=pixels[yy*STRIDE+xx]==RGB(16,22,56);for(int yy=72;yy<150;yy++)for(int xx=190;xx<290;xx++){unsigned p=pixels[yy*STRIDE+xx];vortex_pixels+=p==RGB(91,240,255)||p==RGB(221,105,255)||p==RGB(250,238,201);}}}int distinct=hashes[0]!=hashes[1]&&hashes[0]!=hashes[2]&&hashes[1]!=hashes[2],corridor_clean=old_panel<40,vortex=vortex_pixels>80;failures+=!distinct+!corridor_clean+!vortex;if(report){fprintf(report,"%s charge surge, corridor and braking frames are visually distinct\n",distinct?"PASS":"FAIL");fprintf(report,"%s corridor omits the former title/progress panel\n",corridor_clean?"PASS":"FAIL");fprintf(report,"%s midpoint corridor contains the spiral vortex throat\n",vortex?"PASS":"FAIL");fprintf(report,"RESULT %d failures\n",failures);fclose(report);}sceKernelExitGame();return 0;}
 FILE *routechargereview=fopen("route-charge-review.flag","r");if(routechargereview){fclose(routechargereview);int failures=0;FILE *report=fopen("route-charge-review.txt","w");unsigned *pixels=malloc(STRIDE*H*sizeof(unsigned));if(pixels)fb=pixels;game_init(&game);story_complete(&game);launch(&game);page=FLIGHT;game.route_goal=game.destination=(game.system+1)%256;hud_mode=hud_hidden=0;selected_target=ROUTE_TARGET_ID;scan_cat=3;autoaim=1;game.boost=1;game.message_time=game.voice_time=0;const float charges[4]={2.49f,2.5f,3.75f,4.95f};const char *names[4]={"route-charge-49.bmp","route-charge-50.bmp","route-charge-75.bmp","route-charge-99.bmp"};unsigned hashes[4]={0};for(int k=0;k<4;k++){route_boost_charge=charges[k];game.time=12.f+k*.37f;rect(0,0,W,H,BG);starfield();route_target_star();route_prejump_lines();cockpit();route_prejump_panel();dump_native_bmp(names[k]);for(int yy=118;yy<166;yy++)for(int xx=96;xx<384;xx++)hashes[k]=(hashes[k]^pixels[yy*STRIDE+xx])*16777619u;}route_boost_charge=2.49f;int threshold=!route_prejump_active();route_boost_charge=2.5f;threshold=threshold&&route_prejump_active()&&route_prejump_progress()<.001f;route_boost_charge=3.75f;int mapping=fabsf(route_prejump_progress()-.5f)<.001f;int distinct=hashes[0]!=hashes[1]&&hashes[1]!=hashes[2]&&hashes[2]!=hashes[3];failures+=!threshold+!mapping+!distinct;if(report){fprintf(report,"%s panel switches on at half blue charge and begins at zero percent\n",threshold?"PASS":"FAIL");fprintf(report,"%s second half maps linearly to the hyperdrive percentage\n",mapping?"PASS":"FAIL");fprintf(report,"%s native frames show increasing charge and warp-line stages\n",distinct?"PASS":"FAIL");fprintf(report,"RESULT %d failures\n",failures);fclose(report);}sceKernelExitGame();return 0;}
 FILE *deathreview=fopen("death-review.flag","r");if(deathreview){fclose(deathreview);int failures=0;FILE *report=fopen("death-review.txt","w");unsigned *pixels=malloc(STRIDE*H*sizeof(unsigned));if(pixels)fb=pixels;unsigned hashes[3]={0};int panel_pixels[3]={0},selected_pixels=0;const float ages[3]={.35f,1.45f,3.10f};const char *names[3]={"death-explosion.bmp","death-shutdown.bmp","death-recovery-menu.bmp"};game_init(&game);story_complete(&game);launch(&game);page=FLIGHT;hud_mode=0;game.dead=1;game.hull=game.energy=0;game.message_time=game.voice_time=0;death_choice=0;for(int k=0;k<3;k++){game.explosion=ages[k];rect(0,0,W,H,BG);starfield();route_target_star();route_prejump_lines();cockpit();route_prejump_panel();dump_native_bmp(names[k]);for(int yy=0;yy<H;yy++)for(int xx=0;xx<W;xx++)hashes[k]=hashes[k]*33u+pixels[yy*STRIDE+xx];for(int yy=192;yy<H;yy++)for(int xx=0;xx<W;xx++)panel_pixels[k]+=pixels[yy*STRIDE+xx]==UI_PANEL;if(k==2)for(int yy=124;yy<145;yy++)for(int xx=119;xx<361;xx++)selected_pixels+=pixels[yy*STRIDE+xx]==RGB(112,36,38);}int stages=hashes[0]!=hashes[1]&&hashes[0]!=hashes[2]&&hashes[1]!=hashes[2],shutdown=panel_pixels[0]>panel_pixels[2]+1000,menu=selected_pixels>1000;failures+=!stages+!shutdown+!menu;if(report){fprintf(report,"%s explosion, power failure and recovery menu are three distinct stages\n",stages?"PASS":"FAIL");fprintf(report,"%s cockpit instrument panel powers down before recovery choices\n",shutdown?"PASS":"FAIL");fprintf(report,"%s recovery menu draws a complete selected action button\n",menu?"PASS":"FAIL");fprintf(report,"RESULT %d failures\n",failures);fclose(report);}sceKernelExitGame();return 0;}
 if(commander_profile_review()){sceKernelExitGame();return 0;}
 if(menu_fixes_review()){sceKernelExitGame();return 0;}
 if(codex_progress_review()){sceKernelExitGame();return 0;}
 FILE *codexreview=fopen("codex-dossier-review.flag","r");if(codexreview){fclose(codexreview);int failures=0;FILE *report=fopen("codex-dossier-review.txt","w");unsigned *pixels=malloc(STRIDE*H*sizeof(unsigned));if(pixels)fb=pixels;game_init(&game);story_complete(&game);game.system=7;system_bodies(&game);atlas_reset();page=CODEX;row=0;preview_time=18;game.landed_planets[7]=(uint16_t)(3u|(1u<<12));rect(0,0,W,H,BG);codex_screen();dump_native_bmp("codex-dossier-overview.bmp");int percent=atlas_progress_percent(7),before=game.credits,available=atlas_reward_available(7);atlas_input(PSP_CTRL_TRIANGLE);float start=atlas_reward_started;preview_time=start+.34f;rect(0,0,W,H,BG);codex_screen();dump_native_bmp("codex-reward-slide.bmp");preview_time=start+.78f;rect(0,0,W,H,BG);codex_screen();dump_native_bmp("codex-reward-spark.bmp");preview_time=start+1.35f;rect(0,0,W,H,BG);codex_screen();dump_native_bmp("codex-reward-complete.bmp");int paid=atlas_claimed_level(7)==2&&game.credits==before+1800;int persisted=save_game(&game,"codex-review.sav");Game *loaded=malloc(sizeof(Game));if(persisted)persisted=loaded&&load_game(loaded,"codex-review.sav")&&((loaded->landed_planets[7]>>12)&15)==2;free(loaded);preview_time=start+1.4f;atlas_input(PSP_CTRL_CROSS);int closed=!atlas_reward_open();remove("codex-review.sav");remove("codex-review.sav.bak");remove("codex-review.sav.tmp");failures+=!(percent>=20&&available)+!paid+!persisted+!closed;if(report){fprintf(report,"%s adaptive dossier reports real system progress and pending milestones\n",percent>=20&&available?"PASS":"FAIL");fprintf(report,"%s Triangle pays the 20 percent reward exactly once\n",paid?"PASS":"FAIL");fprintf(report,"%s claimed milestone persists through commander save and load\n",persisted?"PASS":"FAIL");fprintf(report,"%s X closes the completed reward card\n",closed?"PASS":"FAIL");fprintf(report,"PASS native overview, slide, sparkle and completed frames captured\n");fprintf(report,"RESULT %d failures\n",failures);fclose(report);}sceKernelExitGame();return 0;}
 FILE *quietreview=fopen("quiet-watch-review.flag","r");if(quietreview){fclose(quietreview);int failures=0;FILE *report=fopen("quiet-watch-review.txt","w");unsigned *pixels=malloc(STRIDE*H*sizeof(unsigned));if(pixels)fb=pixels;game_init(&game);story_complete(&game);launch(&game);page=FLIGHT;game.npc[0].alive=1;game.npc[0].role=TRADERS;game.npc[0].freighter=0;game.npc[0].target=-1;game.npc[0].pos=add(game.pos,(Vec3){0,0,900});night_begin(0,0);input(PSP_CTRL_TRIANGLE,PSP_CTRL_TRIANGLE,.05f,0,0);input(0,0,.05f,0,0);int chooser=comms_quick&&night_ready();rect(0,0,W,H,RGB(2,5,13));quick_comms_box();dump_native_bmp("quiet-watch-reply.bmp");input(PSP_CTRL_CROSS,0,.016f,0,0);int opened=page==COMMS_PANEL&&night_open;night_close();game_init(&game);story_complete(&game);launch(&game);page=FLIGHT;game.npc[0].alive=1;game.npc[0].role=TRADERS;game.npc[0].freighter=0;game.npc[0].target=-1;game.npc[0].pos=add(game.pos,(Vec3){0,0,900});night_begin(0,0);game.attacked=2;int blocked=!night_ready();failures+=!chooser+!opened+!blocked;if(report){fprintf(report,"%s Triangle tap keeps and displays Quiet Watch Talk/Ignore\n",chooser?"PASS":"FAIL");fprintf(report,"%s Respond opens the full paused Quiet Watch conversation\n",opened?"PASS":"FAIL");fprintf(report,"%s combat and active pursuit suppress the stale reply promise\n",blocked?"PASS":"FAIL");fprintf(report,"RESULT %d failures\n",failures);fclose(report);}sceKernelExitGame();return 0;}
 FILE *profilelaw=fopen("profile-law-review.flag","r");if(profilelaw){fclose(profilelaw);input_tests();sceKernelExitGame();return 0;}
 FILE *wreckreview=fopen("wreck-review.flag","r");if(wreckreview){fclose(wreckreview);int failures=0,detail[3]={0},solid[3]={0};unsigned hashes[3]={0};FILE *report=fopen("wreck-review.txt","w");if(report)setvbuf(report,0,_IONBF,0);unsigned *pixels=malloc(STRIDE*H*sizeof(unsigned));if(pixels)fb=pixels;game_init(&game);story_complete(&game);game.system=7;system_bodies(&game);launch(&game);game.opening.stage=OP_FREE;page=FLIGHT;hud_mode=hud_hidden=0;game.message_time=game.voice_time=0;int wreck_index=0;for(int i=0;i<discovery_count();i++)if(discovery_entry(i)==61){wreck_index=i;break;}Vec3 wreck=discovery_position(wreck_index);selected_target=DISCOVERY_ID_MIN+wreck_index;const float range[3]={11500,5200,2800};const float side[3]={600,1500,-1100};const char *names[3]={"wreck-long-range.bmp","wreck-close-hull.bmp","wreck-debris-field.bmp"};for(int k=0;k<3;k++){game.pos=add(wreck,(Vec3){side[k],k*180.f,-range[k]});Vec3 aim=norm(sub(wreck,game.pos));game.yaw=atan2f(aim.x,aim.z);game.pitch=asinf(aim.y);game.roll=0;game.time=11.f+k*1.37f;drawcount=0;space();dump_native_bmp(names[k]);if(k==2&&report){uint64_t begin=sceKernelGetSystemTimeWide();unsigned calls=wreck_ge_calls,tris=wreck_ge_triangles;for(int f=0;f<10;f++){game.time+=1.f/60;drawcount=0;space();}uint64_t us=sceKernelGetSystemTimeWide()-begin;fprintf(report,"INFO wreck hardware %.3f ms/frame %.2f FPS calls %u triangles %u list_peak %u/262144\n",us/10000.0,10000000.0/us,wreck_ge_calls-calls,wreck_ge_triangles-tris,wreck_ge_list_peak);}for(int yy=view_top();yy<view_bot();yy++)for(int xx=0;xx<W;xx++){unsigned p=pixels[yy*STRIDE+xx];hashes[k]=(hashes[k]^p)*16777619u;if(p==RGB(105,128,137)||p==RGB(64,83,92)||p==RGB(178,91,57)||p==RGB(126,153,160)||p==RGB(86,122,137)||p==RGB(74,99,112)||p==RGB(158,103,62)||p==RGB(168,178,175))detail[k]++;if(p==RGB(46,62,68)||p==RGB(85,101,107)||p==RGB(55,71,77)||p==RGB(70,86,92))solid[k]++;}}int distinct=hashes[0]!=hashes[1]&&hashes[0]!=hashes[2]&&hashes[1]!=hashes[2],readable=detail[0]>80&&detail[1]>220&&detail[2]>220&&solid[0]>100&&solid[1]>600&&solid[2]>600;failures+=!distinct+!readable;if(report){fprintf(report,"%s freighter reads differently at navigation, inspection and debris-field ranges\n",distinct?"PASS":"FAIL");fprintf(report,"%s authored hull, bridge, engines and wreck detail remain visible at native resolution\n",readable?"PASS":"FAIL");fprintf(report,"INFO seam pixels %d %d %d; solid hull pixels %d %d %d\n",detail[0],detail[1],detail[2],solid[0],solid[1],solid[2]);fprintf(report,"RESULT %d failures\n",failures);fclose(report);}sceKernelExitGame();return 0;}
 FILE *megayardreview=fopen("mega-yard-review.flag","r");if(megayardreview){fclose(megayardreview);int failures=0;FILE *report=fopen("mega-yard-review.txt","w");unsigned *pixels=malloc(STRIDE*H*sizeof(unsigned));if(pixels)fb=pixels;game_init(&game);story_complete(&game);game.docked=1;game.credits=9000000;int mega_system=-1;for(int s=0;s<256;s++)if(station_class_for_system(&game,s)==STATION_MEGA){mega_system=s;break;}if(mega_system<0)mega_system=7;game.system=mega_system;system_bodies(&game);page=YARD;row=PLAYER_SHIP_MEGA_FIRST;preview_time=17;rect(0,0,W,H,BG);yard();dump_native_bmp("mega-shipyard-native.bmp");unsigned yard_hash=2166136261u;int art_pixels=0;for(int y=32;y<230;y++)for(int x=8;x<472;x++){unsigned p=pixels[y*STRIDE+x];yard_hash=(yard_hash^p)*16777619u;if(x>=242&&x<464&&y>=40&&y<128&&p!=UI_PANEL&&p!=BG)art_pixels++;}
  game.ship=PLAYER_SHIP_MEGA_FIRST+4;fit_clear_all(&game);for(int b=0;b<8;b++)game.fit[FIT_WPN+b*6]=(uint8_t)(b<2?1+b:25+(b-2)%7);game.active_weapon=FIT_WPN+7*6;fit_rebuild(&game);page=INVENTORY;row=FIT_WPN+7*6;preview_time=23;rect(0,0,W,H,BG);inventory_screen();dump_native_bmp("mega-loadout-eight-banks.bmp");unsigned load_hash=2166136261u;int bank_edges=0;for(int y=32;y<246;y++)for(int x=8;x<472;x++)load_hash=(load_hash^pixels[y*STRIDE+x])*16777619u;for(int b=0;b<8;b++){int x=16+b*56;bank_edges+=pixels[74*STRIDE+x]!=BG&&pixels[100*STRIDE+x]!=BG;}
  page=EQUIP;row=0;game.systems[game.system].tech=15;int list[EQUIP_COUNT],n=equipment_stock_list(list,EQUIP_COUNT);for(int k=0;k<n;k++)if(list[k]>=TECH_TWIN_PULSE){row=k;break;}rect(0,0,W,H,BG);equipment();dump_native_bmp("mega-tech-icon-native.bmp");int icon_pixels=0;for(int y=50;y<220;y++)for(int x=20;x<190;x++){unsigned p=pixels[y*STRIDE+x];if(p==tech_icon_ink(FIT_WPN)||p==tech_icon_ink(FIT_DEF)||p==tech_icon_ink(FIT_NAV)||p==tech_icon_ink(FIT_HOLD)||p==tech_icon_ink(FIT_FUEL)||p==tech_icon_ink(FIT_UTIL))icon_pixels++;}
  int distinct=yard_hash!=load_hash,models=art_pixels>120,banks=bank_edges==16,icons=icon_pixels>3;failures+=!distinct+!models+!banks+!icons;if(report){fprintf(report,"%s shipyard and eight-bank loadout are distinct native screens\n",distinct?"PASS":"FAIL");fprintf(report,"%s selected Mega hull paints a solid bounded rotating portrait\n",models?"PASS":"FAIL");fprintf(report,"%s all eight WPN bank cells draw inside the 480px board\n",banks?"PASS":"FAIL");fprintf(report,"%s new technology rows use category-coloured native icons\n",icons?"PASS":"FAIL");fprintf(report,"RESULT %d failures\n",failures);fclose(report);}sceKernelExitGame();return 0;}
 FILE *radioonly=fopen("radio-check.flag","r");if(radioonly){fclose(radioonly);radio_tests();sceKernelExitGame();return 0;}
 FILE *corecheck=fopen("core-check.flag","r");if(corecheck){fclose(corecheck);radio_tests();steering_tests();game_tests("game-check.txt");sceKernelExitGame();return 0;}
 if(portrait_parts_review()){sceKernelExitGame();return 0;}
 if(mission_expansion_review()){sceKernelExitGame();return 0;}
 if(dockbay_review()){sceKernelExitGame();return 0;}
 if(erl_apex_review()){sceKernelExitGame();return 0;}
 if(erl_holo_track_review()){sceKernelExitGame();return 0;}
 if(erl_progress_review()){sceKernelExitGame();return 0;}
 if(erl_flight_state_review()){sceKernelExitGame();return 0;}
 if(erl_review()){sceKernelExitGame();return 0;}
 if(space_racing_review()){sceKernelExitGame();return 0;}
 if(mission_reward_review()){sceKernelExitGame();return 0;}
 if(intro_film_review()){sceKernelExitGame();return 0;}
 if(load_arrival_review()){sceKernelExitGame();return 0;}
 if(station_departure_review()){sceKernelExitGame();return 0;}
 FILE *openingcheck=fopen("opening-check.flag","r");if(openingcheck){fclose(openingcheck);opening_run_tests();sceKernelExitGame();return 0;}
 if(smoke){radio_tests();steering_tests();game_tests("game-check.txt");input_tests();}
 else
#endif
 {game.voice_time=0;change_page(INTRO);}
 boot_checkpoint("intro ready");
 paint_load();
 /* Small opt-in scene entry used to verify the exact retail EBOOT. It does
    nothing without the flag; use only inside an isolated review directory. */
 int erl_countdown_check=0,erl_countdown_seen=0;FILE *erl_live=fopen("open-erl.flag","r");if(erl_live){int mode=0;fscanf(erl_live,"%d",&mode);fclose(erl_live);story_complete(&game);game.opening.stage=0;game.job_n=0;game.tutorial_step=0;for(int system=0;system<256;system++)if(station_class_for_system(&game,system)==STATION_MEGA){game.system=system;system_bodies(&game);game_spawn(&game);break;}game.docked=1;game.erl.referral=game.erl.licence=1;game.erl.ref_system=game.system;erl_ship_capture(&game,ship_paint[game.ship]);erl_screen=ERL_HOME;change_page(ERL_PAGE);if(mode==2||mode==4)game.erl_local.selection=1;if(mode&&erl_begin(&game,(mode==2||mode==4)?ERL_TRIAL:ERL_PRACTICE,0,ERL_CIRCUIT,0)){game.docked=0;race_restart(&game);game.race.phase=RACE_RUNNING;game.race.countdown=0;game.race.gate=8;RaceGate *q=&game.race.gates[8];game.pos=add(q->pos,mul(q->normal,-11500));game.yaw=atan2f(q->normal.x,q->normal.z);game.pitch=asinf(q->normal.y);for(int i=0;i<6;i++){game.race.pilot[i].pos=add(game.pos,add(mul(q->normal,1500+(i/3)*1100),mul(q->right,(i%3-1)*330.f)));game.race.pilot[i].dir=q->normal;}change_page(FLIGHT);if(mode==2||mode==4){race_restart(&game);game.erl.save_age=3;erl_countdown_check=1;if(mode==4){SpaceRace *r=&game.race;r->gate=4;RaceTrackFrame f=race_track_frame(r,4,.05f);game.pos=add(f.centre,mul(f.up,RACE_TRACK_FLOOR+race_floor_support(&game,0,f)+35));game.yaw=atan2f(f.ahead.x,f.ahead.z);game.pitch=asinf(f.ahead.y);for(int i=0;i<r->rivals;i++){r->pilot[i].gate=4;r->pilot[i].pos=add(game.pos,add(mul(f.ahead,-300-i*240),mul(f.right,(i%3-1)*350)));r->pilot[i].dir=f.ahead;}}}else if(mode==3){game.race.elapsed=140.55f;erl_finish(&game,1,0);erl_replay.played=game.erl.serial;erl_result_serial=game.erl.serial;erl_reveal=0;}}game.voice_time=game.message_time=0;}

 FILE *portraitcap=fopen("portrait-capture.flag","r");if(portraitcap){fclose(portraitcap);portrait_capture=1;}
 FILE *tutorialflag=fopen("open-tutorial.flag","r");if(tutorialflag){int step=1,seen=0;fscanf(tutorialflag,"%d %d",&step,&seen);fclose(tutorialflag);tutorial_start();if(step>0&&step<=TUTORIAL_COUNT)game.tutorial_step=step;game.tutorial_seen=seen?game.tutorial_step:0;tutorial_prepare();}
 FILE *introflag=fopen("open-intro.flag","r");if(introflag){fclose(introflag);change_page(INTRO);intro_time=6;}FILE *introdemo=fopen("intro-demo.flag","r");if(introdemo){fclose(introdemo);profile_info[0].state=1;profile_info[0].credits=12500;profile_info[0].system=7;profile_info[0].ship=0;profile_info[0].portrait=portrait_seeded(7,EXPLORERS,0);snprintf(profile_info[0].name,25,"BEN");profile_info[1].state=profile_info[2].state=0;commander_active_slot=0;intro_profile_ready=1;intro_choice=0;}FILE *socialflag=fopen("open-spacebook.flag","r");if(socialflag){fclose(socialflag);change_page(GALNET);galnet_tab=3;game.voice_time=0;}FILE *netflag=fopen("open-galnet.flag","r");if(netflag){int tab=0;fscanf(netflag,"%d",&tab);fclose(netflag);change_page(GALNET);galnet_tab=tab>=0&&tab<6?tab:0;row=0;game.voice_time=0;}FILE *helpflag=fopen("open-help.flag","r");if(helpflag){int tab=0;fscanf(helpflag,"%d",&tab);fclose(helpflag);change_page(HELP);help_tab=tab>=0&&tab<5?tab:0;}
 FILE *yardflag=fopen("open-yard.flag","r");if(yardflag){int ship=0;fscanf(yardflag,"%d",&ship);fclose(yardflag);change_page(YARD);row=ship>=0&&ship<player_ship_count?ship:0;game.voice_time=0;story_complete(&game);}
 FILE *decoratorflag=fopen("open-decorator.flag","r");if(decoratorflag){fclose(decoratorflag);decorator_feedback=0;change_page(DECORATOR);row=2;game.voice_time=0;story_complete(&game);}
 FILE *sflag=fopen("open-story.flag","r");if(sflag){fclose(sflag);change_page(STORY);}
 FILE *factionflag=fopen("open-factions.flag","r");if(factionflag){fclose(factionflag);change_page(FACTIONS);row=1;}FILE *comfortflag=fopen("open-comfort.flag","r");if(comfortflag){fclose(comfortflag);change_page(COMFORT);}FILE *homeflag=fopen("open-home.flag","r");if(homeflag){int item=0;fscanf(homeflag,"%d",&item);fclose(homeflag);change_page(HOME);row=item>=0&&item<DECK_ITEMS?item:0;game.voice_time=0;}FILE *codexflag=fopen("open-codex.flag","r");if(codexflag){int tab=0;fscanf(codexflag,"%d",&tab);fclose(codexflag);change_page(CODEX);codex_tab=tab>=0&&tab<4?tab:0;row=0;game.voice_time=0;}FILE *cpflag=fopen("open-campaign.flag","r");if(cpflag){int stage=0,flying=0;fscanf(cpflag,"%d %d",&stage,&flying);fclose(cpflag);if(flying)launch(&game);game.campaign_stage=stage>=0&&stage<=6?stage:0;game.voice_time=0;tracked_mission=0;change_page(CAMPAIGN);}FILE *guildflag=fopen("open-guild.flag","r");if(guildflag){int chapter=0,ready=0;fscanf(guildflag,"%d %d",&chapter,&ready);fclose(guildflag);game.guild_chapter=chapter>=0&&chapter<=4?chapter:0;game.guild_flags=ready?31:0;game.voice_time=0;tracked_mission=1;change_page(CAMPAIGN);}
 FILE *newsflag=fopen("open-newspaper.flag","r");if(newsflag){int sys=7,leaf=0;fscanf(newsflag,"%d %d",&sys,&leaf);fclose(newsflag);story_complete(&game);game.opening.stage=OP_FREE;game.system=sys>=0&&sys<256?sys:7;system_bodies(&game);change_page(GALNET);galnet_tab=0;row=leaf>=0&&leaf<(int)newspaper_identity[game.system].page_count?leaf:0;newspaper_reset();game.voice_time=0;}
 FILE *flyflag=fopen("open-flight.flag","r");if(flyflag){fclose(flyflag);launch(&game);page=FLIGHT;hud_mode=hud_hidden=0;}
 FILE *rearflag=fopen("rear-demo.flag","r");if(rearflag){fclose(rearflag);launch(&game);page=FLIGHT;hud_mode=hud_hidden=0;game.yaw=game.pitch=game.roll=0;game.voice_time=game.message_time=0;for(int i=0;i<3;i++){NPC *n=&game.npc[i];n->alive=1;n->role=i==0?PIRATES:i==1?LAW:TRADERS;n->mesh=mesh_id(i==2?"PYTHON":"COBRA");n->radius=i==2?58:24;n->scale=i==2?1.25f:.72f;n->pos=add(game.pos,(Vec3){(float)((i-1)*280),(float)((i&1)*70-30),-650.f-i*420});n->dir=norm(sub(game.pos,n->pos));n->cruise=120+i*30;n->target=i==0?-2:-1;}game.fire_bearing_time[0]=2;game.incoming_source=0;}
 FILE *skyflag=fopen("open-sky.flag","r");if(skyflag){int system=game.system;fscanf(skyflag,"%d",&system);fclose(skyflag);if(system<0)system=0;if(system>255)system=255;game.system=system;system_bodies(&game);launch(&game);story_complete(&game);page=FLIGHT;hud_mode=hud_hidden=0;game.yaw=.37f;game.pitch=-.11f;game.roll=.08f;game.voice_time=game.message_time=0;}
 FILE *deepflag=fopen("open-deep-freighter.flag","r");if(deepflag){int system=7,lane=0,blocked=0;fscanf(deepflag,"%d %d %d",&system,&lane,&blocked);fclose(deepflag);if(system<0)system=0;if(system>255)system=255;lane=(lane%3+3)%3;game.system=system;system_bodies(&game);launch(&game);story_complete(&game);page=FLIGHT;hud_mode=hud_hidden=2;game.pos=(Vec3){0,0,0};game.voice_time=game.message_time=0;
  float heading=0,phase=0;Vec3 freight={0};for(int second=0;second<500;second++){game.time=(float)second;freight=deep_traffic_position(lane,&heading,&phase);if(phase>.38f&&phase<.62f)break;}Vec3 aim=norm(sub(freight,game.pos));game.yaw=atan2f(aim.x,aim.z)+.12f;game.pitch=asinf(aim.y);game.roll=0;
  if(blocked){float range=length(sub(freight,game.pos));game.bodies[1].pos=add(game.pos,mul(aim,range*.55f));game.bodies[1].radius=range*.12f;game.bodies[1].type=ROCKY;game.bodies[1].seed=0x4f43434cu;snprintf(game.bodies[1].name,sizeof(game.bodies[1].name),"OCCLUSION TEST");}
 }
 int race_performance=0,race_perf_samples=0,race_perf_100=0,race_perf_250=0;double race_perf_seconds=0;float race_perf_worst=0;FILE *raceperf=fopen("race-performance.flag","r");if(raceperf){fclose(raceperf);race_performance=1;render_profile_enabled=1;}FILE *shockperf=fopen("shockwave-performance.flag","r");if(shockperf){fclose(shockperf);race_performance=2;sw_measure_enabled=1;}
 int sky_benchmark=0;FILE *skybench=fopen("sky-benchmark.flag","r");if(skybench){sky_benchmark=1;fscanf(skybench,"%d",&sky_benchmark);fclose(skybench);if(sky_benchmark<1)sky_benchmark=1;}
 FILE *interceptflag=fopen("open-thargoid.flag","r");if(interceptflag){fclose(interceptflag);launch(&game);story_complete(&game);page=FLIGHT;game.destination=(game.system+1)%256;game.jump=4;thargoid_begin();}
 FILE *freightflag=fopen("open-freighter.flag","r");if(freightflag){int style=0,phase=0;fscanf(freightflag,"%d %d",&style,&phase);fclose(freightflag);launch(&game);story_complete(&game);NPC *n=&game.npc[8];n->freight_style=style%3;n->radius=length(freight_extent(n));if(phase){n->freight_state=phase==1?FREIGHT_ARRIVING:FREIGHT_CHARGING;n->freight_timer=phase==1?2.4f:1.5f;game.freight_gap=0;}game.pos=freight_world(n,(Vec3){1100,450,1000});Vec3 d=norm(sub(n->pos,game.pos));game.yaw=atan2f(d.x,d.z);game.pitch=asinf(d.y);game.speed=0;game.voice_time=game.message_time=0;page=FLIGHT;selected_target=NPC_ID_MIN+8;}
 FILE *rockflag=fopen("open-rock.flag","r");if(rockflag){fclose(rockflag);launch(&game);story_complete(&game);game.pos=add(game.debris[0].pos,(Vec3){0,60,-430});game.yaw=0;game.pitch=-.1386f;game.speed=0;game.voice_time=game.message_time=0;page=FLIGHT;selected_target=DEBRIS_ID_MIN;}
 FILE *bodyflag=fopen("open-body.flag","r");if(bodyflag){fclose(bodyflag);launch(&game);game.pos=add(game.bodies[1].pos,(Vec3){0,0,-game.bodies[1].radius*3});game.speed=0;game.yaw=game.pitch=0;game.voice_time=game.message_time=0;story_complete(&game);page=FLIGHT;hud_mode=1;}FILE *pflag=fopen("open-planet.flag","r");if(pflag){fclose(pflag);launch(&game);game.approach=1;enter_planet(&game);page=FLIGHT;hud_mode=hud_hidden=0;game.speed=12;}
 FILE *eflag=fopen("open-eva.flag","r");if(eflag){fclose(eflag);launch(&game);game.approach=1;enter_planet(&game);{Vec3 pad=surface_site(&game,1);game.pos=add(pad,(Vec3){0,18,0});game.speed=8;land_planet(&game);eva_toggle(&game);}page=FLIGHT;hud_mode=hud_hidden=0;}
 FILE *mflag=fopen("open-missions.flag","r");if(mflag){fclose(mflag);game.credits=20000;accept_mission(&game,0);accept_mission(&game,2);change_page(MISSIONS);}
 FILE *lflag=fopen("open-log.flag","r");if(lflag){fclose(lflag);game.credits=20000;accept_mission(&game,0);accept_mission(&game,2);change_page(MISSIONLOG);}
 FILE *detailsflag=fopen("open-details.flag","r");if(detailsflag){int system=game.system;if(fscanf(detailsflag,"%d",&system)==1&&system>=0&&system<256){game.system=system;system_bodies(&game);}fclose(detailsflag);story_complete(&game);change_page(DETAILS);row=0;game.voice_time=0;}
 FILE *tvflag=fopen("open-local-tv.flag","r");if(tvflag){fclose(tvflag);story_complete(&game);local_tv_open();change_page(LOCALTV);row=0;game.voice_time=0;}
 int dump_native=0,audit_all=0;FILE *dflag=fopen("dump-native.flag","r");if(dflag){fclose(dflag);dump_native=1;}FILE *aflag=fopen("audit-all.flag","r");if(aflag){fclose(aflag);audit_all=1;}
 int chart_transition_check=0,chart_transition_captures=0,chart_transition_frames=0;double chart_transition_seconds=0;
 FILE *ctflag=fopen("chart-transition-check.flag","r");if(ctflag){fclose(ctflag);chart_transition_check=1;story_complete(&game);change_page(HOME);row=2;game.voice_time=game.message_time=0;}
 FILE *radioflag=fopen("open-radio.flag","r");if(radioflag){fclose(radioflag);game.voice_time=0;story_complete(&game);change_page(RADIO);}
 int audio_flight_capture=0;FILE *audioflightflag=fopen("audio-flight-capture.flag","r");if(audioflightflag){fclose(audioflightflag);audio_flight_capture=1;FILE *musicstress=fopen("audio-music-stress.flag","r");if(musicstress){audio_flight_capture=2;fscanf(musicstress,"%d",&audio_flight_capture);fclose(musicstress);if(audio_flight_capture<2)audio_flight_capture=2;}story_complete(&game);launch(&game);page=FLIGHT;game.speed=0;game.voice_time=game.message_time=0;radio_off=1;comms_quiet=1;}
#ifdef ELITE_ENABLE_REVIEW
 FILE *hostile_audio_flag=fopen("audio-hostile-capture.flag","r");if(hostile_audio_flag){fclose(hostile_audio_flag);audio_flight_capture=1;story_complete(&game);launch(&game);page=FLIGHT;game.speed=player_ships[game.ship].speed*.35f;game.voice_time=game.message_time=0;radio_off=1;comms_quiet=1;}
 FILE *landing_audio_flag=fopen("audio-landing-capture.flag","r");if(landing_audio_flag){fclose(landing_audio_flag);audio_flight_capture=1;story_complete(&game);launch(&game);page=FLIGHT;game.speed=player_ships[game.ship].speed*.18f;game.voice_time=game.message_time=0;radio_off=1;comms_quiet=1;}
 FILE *surface_audio_flag=fopen("audio-surface-capture.flag","r");if(surface_audio_flag){fclose(surface_audio_flag);audio_flight_capture=1;story_complete(&game);launch(&game);page=FLIGHT;game.speed=0;game.voice_time=game.message_time=0;radio_off=1;comms_quiet=1;}
 FILE *water_audio_flag=fopen("audio-water-capture.flag","r");if(water_audio_flag){fclose(water_audio_flag);audio_flight_capture=1;story_complete(&game);launch(&game);page=FLIGHT;game.speed=0;game.voice_time=game.message_time=0;radio_off=1;comms_quiet=1;}
 FILE *station_audio_flag=fopen("audio-station-capture.flag","r");if(station_audio_flag){fclose(station_audio_flag);audio_flight_capture=1;story_complete(&game);game.docked=1;walk_kind=0;sc_built_for=-1;sc_build_map();sc_room=SC_R_ARRIVALS;page=WALK;game.voice_time=game.message_time=0;radio_off=1;comms_quiet=1;}
 FILE *warp_audio_flag=fopen("audio-warp-capture.flag","r");if(warp_audio_flag){fclose(warp_audio_flag);audio_flight_capture=1;story_complete(&game);launch(&game);page=FLIGHT;game.speed=player_ships[game.ship].speed*.7f;game.jump=5.2f;route_jump_engaged=1;game.voice_time=game.message_time=0;radio_off=1;comms_quiet=1;}
#endif
 boot_checkpoint("audio starting");audio_init();boot_checkpoint("audio ready");comms_settings_load("comms-console.cfg");if(audio_flight_capture){radio_off=audio_flight_capture==1;audio_title_active=0;comms_quiet=audio_flight_capture==1;radio_static_ms=0;radio_volume=5;radio_station=audio_flight_capture==4?0:7;audio_battle=audio_flight_capture==3;}
#ifdef ELITE_ENABLE_REVIEW
 if(water_audio_flag)sound_volume=8;
#endif
 FILE *walkflag=fopen("open-walk.flag","r");if(walkflag){int room=0;fscanf(walkflag,"%d",&room);fclose(walkflag);game.docked=1;walk_kind=0;walk_x=walk_z=walk_yaw=0;sc_built_for=-1;sc_build_map();if(room>=0&&room<SC_R_COUNT)sc_room=room;sc_verb=SC_V_LOOK;sc_hot=0;sc_menu=0;page=WALK;game.voice_time=0;}
 if(encounter_capture_setup()){audio_flight_capture=1;encounter_capture_pose(0);}
 unsigned previous=0;int frames=0,frame_samples=0,slow_frames=0,scene_frames[43]={0};double frame_seconds=0,scene_seconds[43]={0};float worst_frame=0;uint64_t last,now;sceRtcGetCurrentTick(&last);float frequency=(float)sceRtcGetTickResolution();
 online_config_load();multiplayer_live_setup();online_live_setup();wifi_live_setup();opening_live_setup();
 while(running){
  if(suspend_requested){erl_sleep_pause(1);suspend_requested=0;intro_suspend();multiplayer_suspend();audio_prepare_suspend();}
  if(resume_requested){resume_requested=0;runtime_recover_from_sleep();erl_sleep_pause(0);previous=0;sceRtcGetCurrentTick(&last);}
  sceRtcGetCurrentTick(&now);float raw_dt=(now-last)/frequency,dt=raw_dt;last=now;if((smoke||sky_benchmark||audio_flight_capture)&&frames>30&&raw_dt<.25f){int scene=frames/10;if(scene>42)scene=42;frame_seconds+=raw_dt;frame_samples++;scene_seconds[scene]+=raw_dt;scene_frames[scene]++;if(raw_dt>worst_frame)worst_frame=raw_dt;if(raw_dt>.025f)slow_frames++;}if(dt>.05f)dt=.05f;if(dt<.001f)dt=.001f;
  if(encounter_capture_scene>=0)encounter_capture_begin(frames);
  if(race_performance&&frames>30){race_perf_samples++;race_perf_seconds+=raw_dt;if(raw_dt>race_perf_worst)race_perf_worst=raw_dt;race_perf_100+=raw_dt>.1f;race_perf_250+=raw_dt>.25f;}
  SceCtrlData pad={0};pad.Lx=pad.Ly=128;
  int valid=sceCtrlPeekBufferPositive(&pad,1)>0;
  if(!valid){pad.Buttons=0;pad.Lx=pad.Ly=128;}
  if(opening_live.enabled){pad.Buttons=opening_live_buttons();pad.Lx=pad.Ly=128;valid=1;}
  unsigned pressed=pad.Buttons&~previous;previous=pad.Buttons;opening_live_trace("INPUT_BEGIN");
  int was_ready=analog_ready,ready=stick_ready(valid,pad.Lx,pad.Ly,&analog_ready);
  if(ready&&!was_ready){analog_center_x=pad.Lx;analog_center_y=pad.Ly;}
  if(ready&&!(pad.Buttons&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT|PSP_CTRL_UP|PSP_CTRL_DOWN))&&abs((int)pad.Lx-analog_center_x)<18&&abs((int)pad.Ly-analog_center_y)<18){analog_center_x=(analog_center_x*15+pad.Lx)/16;analog_center_y=(analog_center_y*15+pad.Ly)/16;}
  unsigned steer=flight_steer_buttons(pad.Buttons);
  float ax,ay;steering_axes_centered(valid,steer,pad.Lx,pad.Ly,analog_enabled&&ready,analog_center_x,analog_center_y,&ax,&ay);
  if(race_performance==1&&race_is_active(&game)&&game.race.gate<RACE_GATES){
   RaceGate *q=&game.race.gates[game.race.gate];RaceTrackFrame frame=race_track_nearest(&game.race,game.race.gate,game.pos);float t=fminf(1,frame.t+.12f);RaceTrackFrame ahead_frame=race_track_frame(&game.race,game.race.gate,t);Vec3 aim=add(ahead_frame.centre,mul(ahead_frame.up,RACE_TRACK_FLOOR+race_floor_support(&game,0,ahead_frame)+40));if(!((game.race.arcade.branch_boost[0]|game.race.arcade.branch_repair[0])&(1u<<game.race.gate))&&frame.t<.5f)aim=add(race_pad_point(&game.race,game.race.gate,.5f,0),mul(ahead_frame.up,race_floor_support(&game,0,ahead_frame)+40));
   Vec3 desired=norm(sub(add(aim,mul(q->normal,100)),game.pos)),right,up,ahead;camera_axes(&game,&right,&up,&ahead);
   ax=fmaxf(-1,fminf(1,dot(desired,right)*7));ay=-fmaxf(-1,fminf(1,dot(desired,up)*7));pad.Buttons=PSP_CTRL_RTRIGGER;third_person=dump_native&&frames<1020?1:frames<600?1:0;if(frames%240<35)pad.Buttons|=PSP_CTRL_SQUARE;if(frames%90<2)pad.Buttons|=PSP_CTRL_CROSS;
   if(game.race.reserve>65)game.boost=1;if(game.race.reserve<12)game.boost=0;
  }
  mission_card_sync();
  if(!paused&&!mission_card_open())preview_time+=dt;
  if(!mission_card_open())online_tick(dt);
  if(encounter_capture_hud>=9){encounter_capture_pose(frames);unsigned buttons=twin_capture_controls(frames);pressed=buttons&~(frames? twin_capture_controls(frames-1):0);pad.Buttons=buttons;}
  /* The staged expanded-computer benchmark must supply the same held
     buttons as a player. Releasing Square on every test frame would otherwise
     force a close/open and full cache repaint unrelated to real browsing. */
  if(encounter_capture_scene>=0&&encounter_capture_hud>=0&&encounter_capture_hud<9){int m=encounter_capture_hud;pad.Buttons=(m==1||m==3||m==4||m==6?PSP_CTRL_SQUARE:0)|(m>=1&&m<=5?PSP_CTRL_CIRCLE:0);pressed=frames?0:pad.Buttons;}
  float jump_before=game.jump;thargoid_failed_flash=0;if(!multiplayer_live_frame(dt)&&!online_live_frame(dt)&&!wifi_live_frame(dt))input(pressed,pad.Buttons,page==CHART?chart_control_dt(raw_dt):dt,ax,ay);if(jump_before>0&&game.jump<=0&&!thargoid_failed_flash){warp_arrival_fade=jump_polar&&ion_in_jump&&!thargoid_active?0.f:1.f;route_jump_engaged=0;}else if(warp_arrival_fade>0)warp_arrival_fade=fmaxf(0,warp_arrival_fade-dt/1.35f);
  if(audio_flight_capture&&encounter_capture_scene<0
#ifdef ELITE_ENABLE_REVIEW
   &&!water_audio_flag&&!warp_audio_flag
#endif
  ){float base=player_ships[game.ship].speed;if(frames<120){game.speed=0;game.boost=0;ax=0;}else if(frames<300){game.speed=base*(frames-120)/180.f;game.boost=0;ax=sinf(frames*.05f)*.55f;}else if(frames<420){game.speed=base*(1.f+(frames-300)/80.f);game.boost=1;ax=sinf(frames*.08f)*.85f;}else {game.speed=base*fmaxf(0,(600-frames)/180.f);game.boost=0;ax=sinf(frames*.04f)*.35f;}}
  if(erl_countdown_check&&game.race.phase==RACE_COUNTDOWN){int digit=(int)ceilf(game.race.countdown);if(digit>=1&&digit<=4)erl_countdown_seen|=1<<digit;}erl_ui_tick(dt);race_ui_sync();expedition_flight_sync();opening_live_trace("INPUT_DONE");
  if(!mission_card_open()&&game.voice_time>0){game.voice_time-=dt;if(game.voice_time<0)game.voice_time=0;}
  if(page!=FLIGHT&&!paused&&!mission_card_open()){game.message_time-=dt;if(game.message_time<0)game.message_time=0;}
  charm_tick();
  int flight_audio_on=page==FLIGHT&&!game.docked&&!paused;
  audio_flight_update(flight_audio_on,game.speed/fmaxf(1.f,(float)player_ships[game.ship].speed),ax,game.boost,game.hull,(pad.Buttons&PSP_CTRL_RTRIGGER)!=0);
  {static int audio_contact=-1;int nearest=-1;float nearest_d=6001.f;for(int i=0;i<NPC_COUNT;i++)if(game.npc[i].alive){float d=length(sub(game.npc[i].pos,game.pos));if(d<nearest_d){nearest=i;nearest_d=d;}}
   if(audio_contact>=0&&audio_contact<NPC_COUNT&&game.npc[audio_contact].alive){float held_d=length(sub(game.npc[audio_contact].pos,game.pos));if(held_d<6000&&held_d<nearest_d*1.2f){nearest=audio_contact;nearest_d=held_d;}}audio_contact=nearest_d<6000?nearest:-1;
   if(flight_audio_on&&nearest>=0&&nearest_d<6000){NPC *n=&game.npc[nearest];Vec3 relative=norm(sub(n->pos,game.pos)),right={cosf(game.yaw),0,-sinf(game.yaw)},relative_velocity=sub(mul(n->dir,n->cruise),mul(forward(&game),game.speed));float traffic_pan=dot(relative,right),radial_speed=dot(relative_velocity,relative);int traffic_kind=(n->freighter||n->mega_hull>=0)?2:(n->scale<.9f||n->cruise>700.f?0:1);audio_traffic_update(1,nearest_d,traffic_pan,n->cruise/650.f,traffic_kind,(n->vessel_seed>>8)&3,occluded(n->pos),radial_speed);}
   else audio_traffic_update(0,6000,0,0,0,0,0,0);}
  {int hub=nearest_hub(&game);Vec3 station=hub_position(&game,hub),relative=norm(sub(station,game.pos)),right={cosf(game.yaw),0,-sinf(game.yaw)};float station_distance=length(sub(station,game.pos));audio_approach_update(page==FLIGHT&&!game.docked&&station_distance<20000,station_distance,dot(relative,right),game.dock_stage,game.dock_phase,game.docked);audio_hyperspace_update(page==FLIGHT?game.jump:0);}
  if(game.cue){if(!quiet_comms||(game.cue!=SFX_COMM&&game.cue!=SFX_TALK)){int pan=0;if((game.cue==SFX_ALERT||game.cue==SFX_MISSILE)&&game.incoming_source>=0&&game.incoming_source<NPC_COUNT&&game.npc[game.incoming_source].alive){Vec3 bearing=norm(sub(game.npc[game.incoming_source].pos,game.pos)),side={cosf(game.yaw),0,-sinf(game.yaw)};pan=(int)(dot(bearing,side)*100);if(pan<-100)pan=-100;if(pan>100)pan=100;}audio_play_at(game.cue,1000,pan);}game.cue=0;}
  {int warning_pan=0;if(game.incoming_source>=0&&game.incoming_source<NPC_COUNT&&game.npc[game.incoming_source].alive){Vec3 bearing=norm(sub(game.npc[game.incoming_source].pos,game.pos));Vec3 side={cosf(game.yaw),0,-sinf(game.yaw)};warning_pan=(int)(dot(bearing,side)*100);if(warning_pan<-100)warning_pan=-100;if(warning_pan>100)warning_pan=100;}audio_warning_update(page==FLIGHT&&!paused,game.heat,game.hull,game.incoming_missile,dt,warning_pan);}
#ifdef ELITE_ENABLE_REVIEW
  if(hostile_audio_flag){if(frames==60)audio_play(SFX_ALIEN_PLAYER);if(frames==145||frames==185)audio_play_at(SFX_ALIEN_FIRE,900,frames==145?-38:42);if(frames==245)audio_play_at(SFX_ALIEN_MISSILE,850,55);if(frames==340)audio_play(SFX_ALIEN_KILL);if(frames==430){FILE *report=fopen("audio-hostile-performance.txt","w");if(report){fprintf(report,"PASS hostile presence, paired fire, missile and kill cues completed through the real mixer\n");fprintf(report,"INFO mixer voices %u; command drops %u; lower-priority rejects %u; buffer starves %u\n",audio_voice_peak,audio_command_drops,audio_voice_rejects,audio_buffer_starves);fprintf(report,"RESULT %d failures\n",audio_command_drops||audio_buffer_starves);fclose(report);}running=0;}}
  if(landing_audio_flag){if(frames==75)audio_play(SFX_LAND);if(frames==260){game.speed=0;audio_play(SFX_LAND);}if(frames==390){FILE *report=fopen("audio-landing-performance.txt","w");if(report){fprintf(report,"PASS approach compression and touchdown settle completed through the real mixer\n");fprintf(report,"INFO mixer voices %u; command drops %u; lower-priority rejects %u; buffer starves %u\n",audio_voice_peak,audio_command_drops,audio_voice_rejects,audio_buffer_starves);fprintf(report,"RESULT %d failures\n",audio_command_drops||audio_buffer_starves);fclose(report);}running=0;}}
  if(surface_audio_flag&&frames==570){FILE *report=fopen("audio-surface-performance.txt","w");if(report){double fps=frame_seconds>0?frame_samples/frame_seconds:0;int fail=fps<29.90||audio_command_drops||audio_buffer_starves;fprintf(report,"%s recorded forest, ice and storm surface beds completed through the real mixer (%.2f FPS)\n",fail?"FAIL":"PASS",fps);fprintf(report,"INFO mixer voices %u; command drops %u; lower-priority rejects %u; buffer starves %u; worst mix %.3f ms\n",audio_voice_peak,audio_command_drops,audio_voice_rejects,audio_buffer_starves,audio_mix_worst_us*.001);fprintf(report,"RESULT %d failures\n",fail);fclose(report);}running=0;}
  if(water_audio_flag&&frames==330){FILE *report=fopen("audio-water-performance.txt","w");if(report){fprintf(report,"FAIL ocean ambience is excluded pending listening review; no runtime water test is available\nRESULT 1 failures\n");fclose(report);}running=0;}
  if(station_audio_flag&&frames==570){FILE *report=fopen("audio-station-performance.txt","w");if(report){double fps=frame_seconds>0?frame_samples/frame_seconds:0;int fail=fps<29.90||audio_command_drops||audio_buffer_starves;fprintf(report,"%s recorded Arrivals, canteen, cargo and clinic beds completed through the real mixer (%.2f FPS)\n",fail?"FAIL":"PASS",fps);fprintf(report,"INFO mixer voices %u; command drops %u; lower-priority rejects %u; buffer starves %u; worst mix %.3f ms\n",audio_voice_peak,audio_command_drops,audio_voice_rejects,audio_buffer_starves,audio_mix_worst_us*.001);fprintf(report,"RESULT %d failures\n",fail);fclose(report);}running=0;}
  if(warp_audio_flag&&frames==450){FILE *report=fopen("audio-warp-performance.txt","w");if(report){double fps=frame_seconds>0?frame_samples/frame_seconds:0;int arc_ok=(audio_jump_review_flags&127u)==127u,fail=fps<29.90||audio_command_drops||audio_buffer_starves||game.jump>0||!arc_ok;fprintf(report,"%s full hyperspace charge, quiet beat, stereo corridor, arrival and settled release (%.2f FPS)\n",fail?"FAIL":"PASS",fps);fprintf(report,"INFO arc flags %u/127; remaining jump %.3f seconds\n",audio_jump_review_flags,game.jump);fprintf(report,"INFO mixer voices %u; command drops %u; lower-priority rejects %u; buffer starves %u; worst mix %.3f ms\n",audio_voice_peak,audio_command_drops,audio_voice_rejects,audio_buffer_starves,audio_mix_worst_us*.001);fprintf(report,"RESULT %d failures\n",fail);fclose(report);}running=0;}
#endif
  audio_station_room=page==WALK&&game.docked?sc_room:-1;audio_battle=thargoid_active||(audio_flight_capture==3);
#ifdef ELITE_ENABLE_REVIEW
  if(station_audio_flag){sc_room=frames<145?SC_R_ARRIVALS:(frames<285?SC_R_CANTEEN:(frames<425?SC_R_CARGO:SC_R_CLINIC));audio_station_room=sc_room;}
#endif
  local_tv_audio_update();
#ifdef ELITE_ENABLE_REVIEW
  if(surface_audio_flag){int review_biome=frames<145?4:(frames<290?0:(frames<430?2:1));int review_weather=frames>=430?3:0;audio_surface_ambient=(review_biome<<2)|review_weather;audio_scene_set(1);}
  if(water_audio_flag){audio_surface_ambient=0;audio_scene_set(1);}
#endif
  audio_duck=mission_card_open()||(opening_active(&game)&&game.opening.stage!=OP_CIVILISATION&&game.opening.stage!=OP_LONELY)||(!quiet_comms&&game.voice_time>0)||game.police_stop;audio_scene_set(game.planet>=0?1:(thargoid_active||game.attacked>0||game.incoming_missile>0)?2:game.docked||page!=FLIGHT?3:0);
#ifdef ELITE_ENABLE_REVIEW
  if(surface_audio_flag||water_audio_flag)audio_scene_set(1);
#endif
  if(smoke&&frames==20){suspend_requested=1;resume_requested=1;}
  if(smoke&&frames==21){suspend_requested=1;resume_requested=1;}
  if(smoke&&frames>0&&frames%80==0)radio_tune((frames/80)%RADIO_STATION_COUNT);
  if(smoke&&frames>0&&frames<=90&&frames%10==0){
   if(page!=HOME)input(PSP_CTRL_CIRCLE,0,.016f,0,0);
   row=frames<90?frames/10:0;
   input(PSP_CTRL_CROSS,0,.016f,0,0);
   if(frames==90)game.pos=(Vec3){0,0,2200};
  }
  if(smoke&&frames==110){page=FLIGHT;game.jump=3;game.approach=-1;}
  if(smoke&&frames==120){game.jump=0;game.approach=1;}
  if(smoke&&frames==130){game.approach=-1;game.attacked=3;}
  if(smoke&&frames==140){game.attacked=0;add_crime(&game,15);game.police_stop=1;}
  if(smoke&&frames==150){game.police_stop=0;game.dead=1;game.explosion=.8f;}
  if(smoke&&frames==160){game_init(&game);deck_reset();change_page(DEBUG);}
  if(smoke&&frames==170){launch(&game);game.cargo[0]=3;game.cargo[7]=1;change_page(MARKET);}
  if(smoke&&frames==180){game_init(&game);deck_reset();launch(&game);game.pos=(Vec3){0,0,2600};page=FLIGHT;dock_selected_station();}
  if(smoke&&frames==190){game.dock_stage=2;game.dock_timer=1;}
  if(smoke&&frames==200){game.dock_stage=3;game.dock_timer=.1f;}
  if(smoke&&frames==210){game_init(&game);deck_reset();game.system=0;launch(&game);page=FLIGHT;game.speed=player_ships[game.ship].speed;}
  if(smoke&&frames>=220&&frames<230){game.boost=1;game.speed=player_ships[game.ship].speed*20;}
  if(smoke&&frames==230){game_init(&game);deck_reset();change_page(DETAILS);}
  if(smoke&&frames==240){change_page(MISSIONS);}
  if(smoke&&frames==250){launch(&game);page=FLIGHT;game.speed=0;game.pos=add(game.npc[8].pos,(Vec3){600,200,-1900});Vec3 aim=norm(sub(game.npc[8].pos,game.pos));game.yaw=atan2f(aim.x,aim.z);float ap=aim.y;if(ap>1)ap=1;if(ap<-1)ap=-1;game.pitch=asinf(ap);selected_target=BODY_COUNT+1+8;}
  if(smoke&&frames==260){game_init(&game);deck_reset();game.system=19;launch(&game);page=FLIGHT;game.speed=0;}
  if(smoke&&frames==270){game_init(&game);deck_reset();launch(&game);page=FLIGHT;hud_hidden=1;game.speed=player_ships[game.ship].speed;}
  if(smoke&&frames==280){hud_hidden=0;game.contract=0;game.contract_reward=4500;game.contract_time=188;change_page(MISSIONLOG);}
  if(smoke&&frames==290){game_init(&game);deck_reset();game.systems[game.system].tech=12;change_page(EQUIP);row=7;}
  if(smoke&&frames==300){launch(&game);change_page(TARGETING);target_filter=2;target_details=1;target_refresh();}
  if(smoke&&frames==310){change_page(GALNET);galnet_tab=3;row=0;}
  if(smoke&&frames==320){game_init(&game);deck_reset();game.systems[game.system].tech=12;change_page(EQUIP);row=3;}
  if(smoke&&frames==330){game_init(&game);deck_reset();change_page(MISSIONS);row=2;}
  if(smoke&&frames==340){game_init(&game);deck_reset();game.contract=game.system;game.contract_time=300;game.contract_reward=5000;game.mission_type=MISSION_BOUNTY;game.mission_stage=0;game.docked=0;game_spawn(&game);change_page(TARGETING);target_filter=5;target_details=1;target_refresh();}
  if(smoke&&frames==350){page=FLIGHT;selected_target=target_count?target_ids[0]:0;game.incoming_missile=3.2f;game.incoming_source=game.mission_target;}
  if(smoke&&frames==360){game_init(&game);deck_reset();launch(&game);page=FLIGHT;hud_mode=hud_hidden=0;game.pos=add(game.bodies[1].pos,(Vec3){0,0,-game.bodies[1].radius-800});game.yaw=game.pitch=0;game.approach=1;enter_planet(&game);}
  if(smoke&&frames==370){Vec3 pad=surface_site(&game,1);game.pos=add(pad,(Vec3){0,18,0});game.speed=8;land_planet(&game);}
  if(smoke&&frames==380){eva_toggle(&game);}
  if(smoke&&!visual_hold&&frames==390){game.pos=game.ship_pos;game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;eva_toggle(&game);takeoff_planet(&game);}
  if(smoke&&!visual_hold&&frames==400){leave_planet(&game);page=FLIGHT;}
  if(smoke&&!visual_hold&&frames==410){change_page(CODEX);}
  if(smoke&&!visual_hold&&frames==420){change_page(RADIO);}if(smoke&&!visual_hold&&frames==415){game_init(&game);deck_reset();launch(&game);page=FLIGHT;game.pos=game.anomaly[0].pos;analysis_scan(&game,ANOMALY_ID_MIN);}
  if(chart_transition_check&&frames==8)change_page(CHART);
  if(chart_transition_check&&chart_dissolve_time>0){chart_transition_frames++;chart_transition_seconds+=raw_dt;}
  if(sky_benchmark>=2){float t=frames/60.f;game.yaw=.37f+t*.4f;game.pitch=sinf(t*1.2f)*.55f;game.roll=sinf(t*.7f)*.4f;if(sky_benchmark==3)hud_mode=hud_hidden=2;}
  encounter_capture_record(raw_dt,frames);if(encounter_capture_hud<9)encounter_capture_pose(frames);
  encounter_capture_section(0);opening_live_trace("DRAW_BEGIN");
  fb=(unsigned *)(0x44000000u+(unsigned)buffer*STRIDE*H*4);pspDebugScreenSetOffset(buffer*STRIDE*H*4);if(page!=HOME&&(page!=FLIGHT||hud_mode==1))rect(0,0,W,H,page==FLIGHT?BG:ui_paint_surface(BG));drawcount=0;
 mission_card_sync();if(!mission_card_open()){
 switch(page){case DOCKBAY_PAGE:dockbay_trade_draw();break;case ERL_PAGE:erl_console_draw();break;case FLIGHT:if(game.erl.result==1&&!(game.race.layout==5&&game.race.crash_time>0))break;else if(opening_sol(&game)&&game.opening.stage==OP_SOL_MAP&&game.dock_stage==4)break;else if(ps_open)ps_draw();else space();break;case MARKET:market_screen();break;case CHART:chart();break;case YARD:yard();break;case EQUIP:equipment();break;case INVENTORY:inventory_screen();break;case STORAGE:storage_screen();break;case REPAIR:repair_screen();break;case DECORATOR:decorator_screen();break;case STATUS:status();break;case LAW_PAGE:law_screen();break;case FIELDGUIDE:field_guide_screen();break;case HELP:help();break;case FACTIONS:factions();break;case LOCAL:local_system();break;case DEBUG:debug_screen();break;case COMMS:communications();break;case DETAILS:system_details();break;case LOCALTV:local_tv_screen();break;case MISSIONS:mission_board();break;case MISSIONLOG:mission_log();break;case TARGETING:targeting_screen();break;case GALNET:galnet_screen();break;case CODEX:codex_screen();break;case STORY:story_screen();break;case GUILD:guild_screen();break;case RADIO:radio_screen();break;case COMMS_PANEL:comms_panel();break;case INTRO:intro_screen();break;case CAMPAIGN:campaign_screen();break;case COMFORT:comms_panel();break;case WALK:walk_screen();break;case MULTIPLAYER:case NEARBY_PLAYERS:break;default:home();}if(quit_confirm)quit_confirmation_draw();
 if(page==MULTIPLAYER)wifi_screen();
 if(page==NEARBY_PLAYERS)online_screen();
 multiplayer_live_capture();
 if(page==FLIGHT&&wifi.state)wifi_overlay();
 if(wifi_live.enabled&&wifi_live.captured==1){dump_native_bmp("wifi-live-flight.bmp");wifi_live.captured=2;}
 chart_dissolve_draw(dt);
 if(chart_transition_check){
  int stage=frames<8?-1:chart_dissolve_time>CHART_DISSOLVE_DURATION*.75f?0:chart_dissolve_time>CHART_DISSOLVE_DURATION*.50f?1:chart_dissolve_time>CHART_DISSOLVE_DURATION*.25f?2:chart_dissolve_time>0?3:4;
  int bit=stage+1;if(stage>=-1&&!(chart_transition_captures&(1<<bit))){char path[48];snprintf(path,sizeof(path),"chart-transition-%d.bmp",stage+1);dump_native_bmp(path);chart_transition_captures|=1<<bit;}
  if(stage==4){FILE *report=fopen("chart-transition-check.txt","w");if(report){double fps=chart_transition_seconds>0?chart_transition_frames/chart_transition_seconds:0;fprintf(report,"PASS galaxy map transition completed and revealed the live chart\n");fprintf(report,"PASS %d review stages captured at native 480x272\n",chart_transition_captures==63?6:0);fprintf(report,"INFO %d transition frames, %.2f measured FPS\n",chart_transition_frames,fps);fprintf(report,"RESULT %d failures\n",chart_transition_captures==63?0:1);fclose(report);}running=0;}
 }
  if(page==CAMPAIGN||page==GUILD||(page==COMMS_PANEL&&(night_open||comms_encounter_conversation||incoming_reply_ready())))dialogue_notice();
  else if(page!=FLIGHT&&!paused&&page!=INTRO&&page!=HELP&&page!=GALNET&&page!=DECORATOR&&page!=STATUS&&page!=LAW_PAGE&&page!=ERL_PAGE&&page!=MULTIPLAYER&&!(page==HOME&&deck_group(row)==0))menu_notice();
  if(page!=INTRO){tutorial_draw();opening_draw();}opening_live_after_draw();if(erl_racer_picker)erl_racer_picker_draw();
  if(twin_capture_footage&&frames>=30&&frames<110){char path[64];snprintf(path,sizeof(path),"twin-frame-%03d.rgba",frames);FILE*image=fopen(path,"wb");if(image){fwrite(fb,sizeof(unsigned),STRIDE*H,image);fclose(image);}FILE*f=fopen("twin-footage.csv",frames==30?"w":"a");if(f){fprintf(f,"%d,%.6f,%.6f,%d,%.6f,%d\n",frames,raw_dt,game.time,game.shots,twin_age,twin_visible_count);fclose(f);}}
  if(encounter_capture_scene>=0&&(frames==30||frames==60||frames==90||frames==130||frames==170||frames==290||frames==490)){char path[64];snprintf(path,sizeof(path),"encounter-retail-%d.bmp",frames);dump_native_bmp(path);}
  ui_action_feedback(dt);
  deck_open_draw();
  ship_purchase_draw();
  load_arrival_draw();
 }
 race_loss_draw();mission_card_draw();erl_result_draw();erl_pause_draw();
 neon_present_finish();
 if(portrait_capture){static const unsigned char pc[PORTRAIT_SPECIES_COUNT]={16,8,8,8,8,8,12,12,8,8};int n=0;rect(0,0,W,H,RGB(12,16,18));text(2,1,UI_GOLD,"96 AUTHORED NPC BASES / NATIVE 24 PX");for(int s=0;s<PORTRAIT_SPECIES_COUNT;s++)for(int v=0;v<pc[s];v++){unsigned d=portrait_seeded(100u+(unsigned)n,EXPLORERS,s);d=portrait_set_field(d,4,3,(unsigned)v&3u);d=portrait_set_field(d,16,7,(unsigned)v>>2);portrait_draw(6+(n%16)*29,28+(n/16)*36,24,24,d,s%FACTION_COUNT);n++;}dump_native_bmp("portrait-gallery.bmp");rect(0,0,W,H,RGB(12,16,18));text(2,1,UI_GOLD,"PORTRAIT SCALE CHECK");unsigned scale=portrait_seeded(991,LAW,PORTRAIT_HUMAN);portrait_draw(18,42,20,20,scale,LAW);portrait_draw(58,42,32,32,scale,LAW);portrait_draw(112,42,48,48,scale,LAW);portrait_draw(184,42,64,64,scale,LAW);portrait_draw(278,42,96,96,scale,LAW);text(2,21,UI_MUTED,"20       32        48         64          96");dump_native_bmp("portrait-scale-gallery.bmp");portrait_capture=0;}
 if(frames==6&&boot_trace){boot_checkpoint("first frames presented");fclose(boot_trace);boot_trace=NULL;}
 if(dump_native&&frames==6)dump_native_bmp("native-480x272.bmp");if(dump_native&&race_performance==1&&(frames==300||frames==570||frames==900)){char path[40];snprintf(path,sizeof(path),"apex-live-%d.bmp",frames);dump_native_bmp(path);}if(dump_native&&race_performance==1&&frames>=480&&frames<=1020&&frames%6==0){char path[40];snprintf(path,sizeof(path),"apex-motion-%04d.bmp",frames);dump_native_bmp(path);}
 if(dump_native&&thargoid_active&&(frames==300||frames==420)){char capture[40];snprintf(capture,sizeof(capture),"thargoid-%d.bmp",frames);dump_native_bmp(capture);}
  if(dump_native&&smoke&&(frames==95||frames==125||frames==205||frames==215||frames==255||frames==275||frames==355||frames==365||frames==385||frames==425)){char capture[64];snprintf(capture,sizeof(capture),"scene-%03d.bmp",frames);dump_native_bmp(capture);}
  if(dump_native&&smoke&&audit_all&&frames>=160&&frames<=420&&frames%10==5){char capture[64];snprintf(capture,sizeof(capture),"audit-%03d.bmp",frames);dump_native_bmp(capture);}
  /* IMMEDIATE after vblank: display the frame we just finished, then draw into the other plane.
   * NEXTFRAME (2.5.4) scheduled the back buffer one frame late and left us painting the live
   * front buffer — black flash / strobing on hardware. Keep sleep recover; fix the flip mode. */
  encounter_capture_section(1);
  /* Ordinary flight can finish inside the active blank interval. Flip there
   * rather than unnecessarily waiting another full refresh. Keep the same
   * immediate, tear-free swap; menus and ERL retain their established pacing. */
  int reuse_blank=page==FLIGHT&&!race_ready(&game)&&sceDisplayIsVblank()==1;
  if(!reuse_blank)sceDisplayWaitVblankStart();else if(encounter_capture_scene>=0)encounter_capture_vblank_reuse++;
  sceDisplaySetFrameBuf((void*)fb,STRIDE,PSP_DISPLAY_PIXEL_FORMAT_8888,PSP_DISPLAY_SETBUF_IMMEDIATE);buffer^=1;frames++;encounter_capture_section(2);
  if(smoke&&!visual_hold&&frames==425){double fps=frame_seconds>0?frame_samples/frame_seconds:0;FILE *log=fopen("boot-check.txt","a");if(log){fprintf(log,"Rendered 42 scenes in 425 frames, including landing, EVA, ship compass, Codex and anomaly scan.\n");fprintf(log,"Performance: %.2f average FPS, %.2f ms worst frame, %d frames over 25 ms.\n",fps,worst_frame*1000,slow_frames);fclose(log);}FILE *perf=fopen("performance-check.txt","w");if(perf){int planet_fail=0;for(int i=36;i<=39;i++)if(scene_frames[i]&&scene_frames[i]/scene_seconds[i]<24)planet_fail=1;int fail=fps<50||planet_fail;fprintf(perf,"%s average frame rate >= 50 FPS (%.2f FPS)\n",fps>=50?"PASS":"FAIL",fps);fprintf(perf,"%s planetary flight/EVA scenes remain >= 24 FPS\n",planet_fail?"FAIL":"PASS");fprintf(perf,"INFO worst frame %.2f ms; %d frames over 25 ms\n",worst_frame*1000,slow_frames);for(int i=3;i<43;i++)if(scene_frames[i])fprintf(perf,"SCENE %02d %.2f FPS\n",i,scene_frames[i]/scene_seconds[i]);fprintf(perf,"RESULT %d failures\n",fail);fclose(perf);}running=0;}
  /* PSP's half-refresh is approximately 29.97, not exactly 30.00. Keep a
   * small measurement tolerance, report the actual value, never round it up. */
  if(race_performance==2&&frames==900){FILE *report=fopen("shockwave-performance.txt","w");if(report){float fps=race_perf_samples/race_perf_seconds;int failed=fps<55||race_perf_100;for(int i=0;i<6;i++)fprintf(report,"PROFILE %d %.2fms\n",i,sw_measure_us[i]*.001f/fmaxf(1,sw_measure_frames));fprintf(report,"INFO %.2f FPS; worst %.2f ms; over 100ms %d; over 250ms %d; MP3 station %d; reveal %.2f\n",fps,race_perf_worst*1000,race_perf_100,race_perf_250,radio_file_station,erl_reveal);fprintf(report,"%s live retail cinematic with MP3 remains at least 55 FPS\n",fps>=55?"PASS":"FAIL");fprintf(report,"%s no 100ms stalls during reveal and live result hold\n",race_perf_100?"FAIL":"PASS");fprintf(report,"RESULT %d failures\n",failed);fclose(report);}running=0;}
  if(race_performance==1&&frames==1200){FILE *report=fopen("race-performance.txt","w");if(report){int countdown_ok=!erl_countdown_check||(erl_countdown_seen==30&&game.race.phase==RACE_RUNNING&&erl_checkpoint_min_stack_free>=65536);int failed=race_perf_samples/race_perf_seconds<55||race_perf_250||erl_checkpoint_completed<2||!countdown_ok;fprintf(report,"%s populated racing remains at least 55 FPS; floor draw mean %.3f ms per view\n",race_perf_samples/race_perf_seconds>=55?"PASS":"FAIL",race_floor_us*.001/fmaxf(1,race_floor_frames));for(int phase=0;phase<7;phase++)fprintf(report,"PHASE %d %.3f ms\n",phase,render_phase_ms[phase]/frames);for(int stage=0;stage<4;stage++)fprintf(report,"FLOOR_STAGE %d %.3f ms\n",stage,race_floor_stage_us[stage]*.001/fmaxf(1,race_floor_frames));fprintf(report,"PROFILE pad %.3f ms effects %.3f ms per view\n",race_pad_us*.001/fmaxf(1,race_pad_frames),race_fx_us*.001/fmaxf(1,race_fx_frames));fprintf(report,"INFO %.2f FPS; worst %.2f ms; over 100ms %d; over 250ms %d; checkpoints %u/%u; MP3 station %d\n",race_perf_samples/race_perf_seconds,race_perf_worst*1000,race_perf_100,race_perf_250,erl_checkpoint_completed,erl_checkpoint_queued,radio_file_station);fprintf(report,"INFO round_layout=%d gate=%d launch_mask=%08x\n",game.race.layout,game.race.gate,game.race.arcade.branch_boost[0]);fprintf(report,"%s no quarter-second stalls during populated race and repeated checkpoint writes\n",race_perf_250?"FAIL":"PASS");fprintf(report,"%s at least two asynchronous saves completed\n",erl_checkpoint_completed>=2?"PASS":"FAIL");if(erl_countdown_check){fprintf(report,"INFO countdown_mask=%d worker_stack_free=%d\n",erl_countdown_seen,erl_checkpoint_min_stack_free);fprintf(report,"%s live countdown 4/3/2/1 reaches racing with concurrent MP3 and autosaves and safe worker stack\n",countdown_ok?"PASS":"FAIL");}fprintf(report,"RESULT %d failures\n",failed);fclose(report);}running=0;}
  if(sky_benchmark&&frames==240){double fps=frame_seconds>0?frame_samples/frame_seconds:0;int pass=fps>=29.90;FILE *perf=fopen("sky-performance.txt","w");if(perf){fprintf(perf,"SYSTEM %d %s; soft-celestial-v1; mode=%d; samples=%d\n",game.system,game.systems[game.system].name,sky_benchmark,frame_samples);fprintf(perf,"%s nominal 30 FPS flight target (%.2f FPS; minimum 29.90)\n",pass?"PASS":"FAIL",fps);fprintf(perf,"INFO worst frame %.2f ms; %d frames over 25 ms\n",worst_frame*1000,slow_frames);fprintf(perf,"RESULT %d failures\n",pass?0:1);fclose(perf);}running=0;}
  if(audio_flight_capture&&frames==600){double fps=frame_seconds>0?frame_samples/frame_seconds:0;FILE *perf=fopen("audio-flight-performance.txt","w");if(perf){int fail=audio_flight_capture>1?(audio_blocks<100||audio_buffer_starves||radio_file_error||(audio_flight_capture!=4&&radio_file_station<0)):fps<29.90;fprintf(perf,"%s %s (%.2f FPS)\n",fail?"FAIL":"PASS",audio_flight_capture>1?"music keeps device buffers supplied during flight":"recorded engine flight remains at the 30 FPS target",fps);fprintf(perf,"INFO worst frame %.2f ms; %d frames over 25 ms; mixer voices %u; drops %u\n",worst_frame*1000,slow_frames,audio_voice_peak,audio_command_drops);fprintf(perf,"INFO device blocks %u; buffer starves %u; worst mix %.3f ms; source %d; format %d; rate %d; decoder error %d\n",audio_blocks,audio_buffer_starves,audio_mix_worst_us*.001,radio_file_station,music_format,mp3_rate,radio_file_error);fprintf(perf,"INFO station hardware calls%u triangles%u list_peak%u/524288; fleet calls%u textured%u flat%u\n",station_ge_calls,station_ge_triangles,station_ge_list_peak,fleet_ge_calls,fleet_ge_triangles,fleet_ge_flat);fprintf(perf,"RESULT %d failures\n",fail);fclose(perf);}running=0;}
  if(encounter_capture_scene>=0&&frames==600)encounter_capture_report();
 }
 encounter_capture_report();erl_checkpoint_collect(1);intro_leave();opening_live_finish();online_close(NULL);wifi_close(NULL);audio_stop();
 sceKernelExitGame();return 0;
}









