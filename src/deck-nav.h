/* Stable service IDs keep existing tutorial hooks and shortcuts compatible. */
#include "opening-unlocks.h"
#define DECK_ITEMS 32
#define DECK_GROUPS 5
static const int deck_rows[DECK_GROUPS][9]={{0,11,2,10,20,30,29,-1,-1},{1,3,4,21,23,24,-1,-1,-1},{17,12,13,7,31,-1,-1,-1,-1},{15,14,22,26,-1,-1,-1,-1,-1},{5,6,16,9,25,-1,-1,-1,-1}};
static const int deck_sizes[]={7,6,5,4,5};
static const char *deck_groups[]={"FLY","SHIP","WORK","DISCOVER","COMMANDER"};
static int deck_focus[DECK_GROUPS]={0,1,17,15,5},deck_last=0,help_tab=0;
static float deck_motion_start=-10;
static int deck_motion_from=0,deck_motion_row=0,deck_motion_direction=1;
static int nav_pages[8],nav_rows[8],nav_depth=0,nav_back=0;
static int deck_group(int id){for(int g=0;g<DECK_GROUPS;g++)for(int i=0;i<deck_sizes[g];i++)if(deck_rows[g][i]==id)return g;return 0;}
/* Shipyard (3), Outfitting (4), Mission board (12) only while docked — removed from the list in flight. */
static int deck_service_visible(int id){
 if(id==31)return erl_hq(&game);
 if(!opening_menu_available(&game,id))return 0;
 if(id==23&&opening_exists(&game)&&(game.opening.flags&OF_APP))return 1;
 if(id==18||id==19||id==27||id==28)return 0;if(id==25)return 1;if(id==30)return game.docked&&!game.tutorial_step;if(id>=27)return !game.tutorial_step;if(!tutorial_service(&game,id))return 0;
 /* Poor ports have a market and mechanics, but no ship sales, module shop or
    decorator.  Omit unavailable services rather than showing dead entries. */
 if(game.docked&&station_class(&game)==STATION_POOR&&(id==3||id==4||id==23))return 0;
 if(id==3||id==4||id==12||id==20||id==23||id==24)return game.docked;return 1;
}
static int deck_fill(int g,int out[9]){int n=0;if(g<0||g>=DECK_GROUPS)return 0;for(int i=0;i<deck_sizes[g];i++){int id=deck_rows[g][i];if(id>=0&&deck_service_visible(id))out[n++]=id;}return n;}
static int deck_visible_groups(int out[DECK_GROUPS]){int n=0,vis[9];for(int g=0;g<DECK_GROUPS;g++)if(deck_fill(g,vis)>0)out[n++]=g;return n;}
static void deck_clamp_row(void){if(deck_service_visible(row))return;int g=deck_group(row),vis[9],n=deck_fill(g,vis);if(n>0){row=vis[0];deck_focus[g]=row;return;}for(g=0;g<DECK_GROUPS;g++)if(deck_fill(g,vis)>0){row=vis[0];deck_focus[g]=row;return;}row=0;}
static void deck_reset(void){nav_virtual_target=-1;nav_follow=0;const int defaults[]={0,1,17,15,5};for(int i=0;i<5;i++)deck_focus[i]=defaults[i];deck_last=0;help_tab=0;nav_depth=0;nav_back=0;deck_motion_start=-10;deck_motion_from=0;}
static void deck_step(int direction){int g=deck_group(row),vis[9],n=deck_fill(g,vis),index=0;if(n<=0)return;for(int i=0;i<n;i++)if(vis[i]==row)index=i;row=vis[(index+n+direction)%n];deck_focus[g]=row;}
static void deck_tab(int direction){int groups[DECK_GROUPS],n=deck_visible_groups(groups),g=deck_group(row),at=0;if(n<=0)return;deck_motion_from=g;deck_motion_row=row;deck_motion_direction=direction<0?-1:1;deck_motion_start=preview_time;deck_focus[g]=row;for(int i=0;i<n;i++)if(groups[i]==g)at=i;g=groups[(at+n+(direction<0?-1:1))%n];row=deck_focus[g];deck_clamp_row();deck_focus[g]=row;}
