/* Stable service IDs keep existing tutorial hooks and shortcuts compatible. */
#define DECK_ITEMS 20
#define DECK_GROUPS 5
static const int deck_rows[DECK_GROUPS][6]={{0,8,11,2,-1,-1},{1,3,4,-1,-1,-1},{17,12,13,7,14,-1},{15,-1,-1,-1,-1,-1},{5,6,16,19,9,-1}};
static const int deck_sizes[]={4,3,5,1,5};
static const char *deck_groups[]={"FLY","SHIP","WORK","DISCOVER","COMMANDER"};
static int deck_focus[DECK_GROUPS]={0,1,17,15,5},deck_last=0,help_tab=0;
static int nav_pages[8],nav_rows[8],nav_depth=0,nav_back=0;
static int deck_group(int id){for(int g=0;g<DECK_GROUPS;g++)for(int i=0;i<deck_sizes[g];i++)if(deck_rows[g][i]==id)return g;return 0;}
static void deck_reset(void){const int defaults[]={0,1,17,15,5};for(int i=0;i<5;i++)deck_focus[i]=defaults[i];deck_last=0;help_tab=0;nav_depth=0;nav_back=0;}
static void deck_step(int direction){int g=deck_group(row),index=0;for(int i=0;i<deck_sizes[g];i++)if(deck_rows[g][i]==row)index=i;row=deck_rows[g][(index+deck_sizes[g]+direction)%deck_sizes[g]];deck_focus[g]=row;}
static void deck_tab(int direction){int g=deck_group(row);deck_focus[g]=row;g=(g+DECK_GROUPS+direction)%DECK_GROUPS;row=deck_focus[g];}
