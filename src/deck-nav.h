/* Stable service IDs keep existing tutorial hooks and shortcuts compatible. */
#define DECK_ITEMS 22
#define DECK_GROUPS 5
static const int deck_rows[DECK_GROUPS][6]={{0,8,11,2,20,-1},{1,3,4,21,-1,-1},{17,12,13,7,14,-1},{15,-1,-1,-1,-1,-1},{5,6,16,19,9,-1}};
static const int deck_sizes[]={5,4,5,1,5};
static const char *deck_groups[]={"FLY","SHIP","WORK","DISCOVER","COMMANDER"};
static int deck_focus[DECK_GROUPS]={0,1,17,15,5},deck_last=0,help_tab=0;
static int nav_pages[8],nav_rows[8],nav_depth=0,nav_back=0;
static int deck_group(int id){for(int g=0;g<DECK_GROUPS;g++)for(int i=0;i<deck_sizes[g];i++)if(deck_rows[g][i]==id)return g;return 0;}
/* Shipyard (3), Outfitting (4), Mission board (12) only while docked — removed from the list in flight. */
static int deck_service_visible(int id){if(id==3||id==4||id==12)return game.docked;return 1;}
static int deck_fill(int g,int out[6]){int n=0;for(int i=0;i<deck_sizes[g];i++){int id=deck_rows[g][i];if(id>=0&&deck_service_visible(id))out[n++]=id;}return n;}
static void deck_clamp_row(void){if(deck_service_visible(row))return;int g=deck_group(row),vis[6],n=deck_fill(g,vis);if(n>0){row=vis[0];deck_focus[g]=row;}else {row=0;deck_focus[0]=0;}}
static void deck_reset(void){const int defaults[]={0,1,17,15,5};for(int i=0;i<5;i++)deck_focus[i]=defaults[i];deck_last=0;help_tab=0;nav_depth=0;nav_back=0;}
static void deck_step(int direction){int g=deck_group(row),vis[6],n=deck_fill(g,vis),index=0;if(n<=0)return;for(int i=0;i<n;i++)if(vis[i]==row)index=i;row=vis[(index+n+direction)%n];deck_focus[g]=row;}
static void deck_tab(int direction){int g=deck_group(row);deck_focus[g]=row;g=(g+DECK_GROUPS+direction)%DECK_GROUPS;row=deck_focus[g];deck_clamp_row();deck_focus[g]=row;}
