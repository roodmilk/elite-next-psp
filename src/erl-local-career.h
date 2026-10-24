static void erl_local_store(Game *g){
 ErlCareer *e=&g->erl;ErlLocal *all=&g->erl_local;if(!all->ready||all->current>255)return;
 ErlLocalSeason *s=&all->season[all->current];if(!e->season&&!e->championship&&!s->started)return;
 s->started=1;s->number=e->season;s->active=e->championship;s->hub=e->champ_hub;s->round=e->round;s->division=e->division;s->place=e->season_place;
 for(int i=0;i<7;i++){s->points[i]=e->points[i];s->wins[i]=e->round_wins[i];}for(int i=0;i<4;i++)s->results[i][0]=e->places[i];
}
void erl_local_sync(Game *g){
 ErlCareer *e=&g->erl;ErlLocal *all=&g->erl_local;
 if(!all->ready){all->ready=1;all->selection=1;all->current=e->season||e->championship?(unsigned)e->champ_system:(unsigned)g->system;erl_local_store(g);}
 if(all->current==(unsigned)g->system)return;
 if(e->event_kind)return; /* Accepted/running attempt owns its exact context. */
 erl_local_store(g);all->current=g->system;ErlLocalSeason *s=&all->season[g->system];
 e->season=s->number;e->championship=s->active;e->champ_system=g->system;e->champ_hub=s->hub;e->round=s->round;e->division=s->started?s->division:e->licence?e->licence-1:0;e->season_place=s->place;
 for(int i=0;i<7;i++){e->points[i]=s->points[i];e->round_wins[i]=s->wins[i];}for(int i=0;i<4;i++)e->places[i]=s->results[i][0];
}
int erl_standing_better(const Game *g,int left,int right){
 const ErlCareer *e=&g->erl;const ErlLocalSeason *s=&g->erl_local.season[e->champ_system];
 if(e->points[left]!=e->points[right])return e->points[left]>e->points[right];
 /* V35 saves retain wins but not rival round places. Do not invent old races. */
 int complete=1;for(int round=0;round<e->round&&round<4;round++)for(int id=0;id<7;id++)complete&=s->results[round][id]!=0;
 if(!complete&&e->round_wins[left]!=e->round_wins[right])return e->round_wins[left]>e->round_wins[right];
 for(int place=1;place<=7;place++){int l=0,r=0;for(int round=0;round<e->round&&round<4;round++){l+=s->results[round][left]==place;r+=s->results[round][right]==place;}if(l!=r)return l>r;}
 int round=e->round>0?e->round-1:0;if(round>3)round=3;int l=s->results[round][left],r=s->results[round][right];return l&&r&&l<r;
}
