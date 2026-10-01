/* Included inside the opt-in chart review; fixtures never touch save files. */
{
#define FILTER_CHECK(c,n) do{int pass=(c);fprintf(report,"%s %s\n",pass?"PASS":"FAIL",n);failures+=!pass;}while(0)
 Game *backup=malloc(sizeof(Game));
 FILTER_CHECK(backup!=0,"filter fixture allocated");
 if(backup){
  *backup=game;int saved_cursor=chart_cursor,saved_filter=chart_filter;
  int complement=1,classes=1;
  for(int i=0;i<256;i++){
   chart_filter=CHART_VISITED;int visited=chart_filter_matches(i);
   chart_filter=CHART_UNVISITED;if(visited+chart_filter_matches(i)!=1)complement=0;
   int count=0;for(int f=CHART_RICH;f<=CHART_MEGA;f++){chart_filter=f;count+=chart_filter_matches(i);}
   if(count!=1)classes=0;
  }
  FILTER_CHECK(complement,"visited and unvisited partition all 256 systems");
  FILTER_CHECK(classes,"Rich Poor and Mega match the real station classes");
  chart_filter=CHART_RANGE;int nearest=-1;float cost=1e9f;
  for(int i=0;i<256;i++){float d=distance_ly(&game,game.system,i)*10;if(i!=game.system&&d>0&&d<cost){nearest=i;cost=d;}}
  FILTER_CHECK(nearest>=0&&cost<=player_ships[game.ship].range,"range fixture has a reachable neighbouring system");
  game.fuel=cost;FILTER_CHECK(chart_filter_matches(nearest),"In Range includes exact fuel boundary");
  game.fuel=cost-.02f;FILTER_CHECK(!chart_filter_matches(nearest),"In Range reacts to insufficient current fuel");
  game=*backup;chart_filter=CHART_JOBS;game.job_n=0;game.saga_step=0;game.contract=game.passenger_dest=-1;
  int matches=0;for(int i=0;i<256;i++)matches+=chart_filter_matches(i);
  FILTER_CHECK(matches==0,"Jobs excludes unaccepted offers and manually plotted routes");
  game.job_n=1;game.jobs[0].dest=goal;game.jobs[0].origin=game.system;
  FILTER_CHECK(chart_filter_matches(goal)&&chart_filter_matches(game.system),"Jobs includes accepted destination and origin");
  game.job_n=0;game.passenger_dest=goal;
  FILTER_CHECK(chart_filter_matches(goal),"Jobs includes an accepted passenger destination");
  game=*backup;chart_filter=CHART_ROUTE;chart_routes_prepare();
  int route_ok=1,route_count=0;unsigned char expected[256]={0};
  for(int at=goal,n=0;at>=0&&n<256;n++,at=chart_parents[at]){expected[at]=1;if(at==game.system)break;}
  for(int i=0;i<256;i++){int match=chart_filter_matches(i);route_count+=match;if(match!=expected[i])route_ok=0;}
  FILTER_CHECK(route_ok&&route_count==chart_hops[goal]+1,"Route contains every plotted stop and no unrelated systems");
  game.route_goal=-1;matches=0;for(int i=0;i<256;i++)matches+=chart_filter_matches(i);
  FILTER_CHECK(matches==0,"Route safely handles no plotted journey");
  game=*backup;int windows=1,pins=1;chart_filter=CHART_ALL;chart_cursor=goal;
  for(int f=0;f<CHART_FILTERS;f++){
   if(chart_filter!=f)windows=0;
   int first=chart_filter_first();if(first<0||first+5>CHART_FILTERS||f<first||f>=first+5)windows=0;
   if(!chart_system_visible(game.system)||!chart_system_visible(goal))pins=0;
   input(PSP_CTRL_SQUARE,0,.016f,0,0);
  }
  FILTER_CHECK(windows&&chart_filter==CHART_ALL,"Square cycles all nine filters with selected row in the five-row window");
  FILTER_CHECK(pins,"current system and plotted destination remain visible under every filter");
  game=*backup;chart_cursor=saved_cursor;chart_filter=saved_filter;free(backup);
 }
#undef FILTER_CHECK
}
