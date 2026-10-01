/* Explicit disposable capture mode. Runs the actual PSP renderer with a valid
 * off-screen buffer before audio startup; never touches commander saves. */
static int chart_review(void){
 FILE *flag=fopen("chart-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *report=fopen("chart-review.txt","w");
 unsigned *saved=fb,*pixels=calloc(STRIDE*H+32,sizeof(unsigned));
 if(!report||!pixels){if(report)fclose(report);free(pixels);return 1;}
 for(int i=0;i<16;i++)pixels[i]=pixels[STRIDE*H+16+i]=0xa55ac33cu;
 fb=pixels+16;story_complete(&game);change_page(CHART);chart_zoom=chart_zoom_goal=1;chart_pan_x=chart_pan_y=0;
 chart_routes_prepare();int goal=0;
 for(int i=0;i<256;i++)if(station_class_for_system(&game,i)==STATION_MEGA&&chart_hops[i]>=3&&chart_hops[i]<=6){goal=i;break;}
 chart_cursor=goal;route_set_goal(&game,goal);chart_filter=CHART_ALL;
 galaxy_overview();dump_native_bmp("chart-all.bmp");
 chart_filter=CHART_MEGA;galaxy_overview();dump_native_bmp("chart-mega.bmp");
 chart_filter=CHART_ALL;chart_zoom=chart_zoom_goal=2.2f;chart_pan_x=chart_pan_y=0;
 {int x,y;chart_project(goal,&x,&y,0);chart_pan_x=248-x;chart_pan_y=144-y;}
 galaxy_overview();dump_native_bmp("chart-zoom.bmp");
 chart_search=1;snprintf(chart_query,sizeof(chart_query),"L");chart_search_refresh();galaxy_overview();dump_native_bmp("chart-search.bmp");chart_search=0;
 int failures=0,prefix_ok=chart_match_n>0;
 for(int i=0;i<chart_match_n;i++)if(game.systems[chart_matches[i]].name[0]!='L')prefix_ok=0;
 fprintf(report,"%s search lists only the typed prefix\n",prefix_ok?"PASS":"FAIL");failures+=!prefix_ok;
 snprintf(chart_query,sizeof(chart_query),"ZZZZZZZZZZZ");chart_search_refresh();
 fprintf(report,"%s empty search is handled\n",chart_match_n==0?"PASS":"FAIL");failures+=chart_match_n!=0;
 chart_query[0]=0;chart_search_refresh();
 fprintf(report,"%s cleared search restores 256 systems\n",chart_match_n==256?"PASS":"FAIL");failures+=chart_match_n!=256;
 Game *probe=malloc(sizeof(Game));
 if(probe){*probe=game;probe->fuel=(float)player_ships[game.ship].range;
  int consistent=1;for(int i=0;i<256;i+=17){if(i==game.system)continue;int jumps=0,hop=route_next_hop(probe,i,&jumps);if(chart_hops[i]!=(hop<0?-1:jumps))consistent=0;}
  fprintf(report,"%s cached chart routes match the gameplay planner\n",consistent?"PASS":"FAIL");failures+=!consistent;free(probe);
 }else{fprintf(report,"FAIL planner fixture allocation\n");failures++;}
 float zoom=chart_zoom;input(PSP_CTRL_LTRIGGER,PSP_CTRL_LTRIGGER,.05f,0,0);int ok=chart_zoom<zoom;
 fprintf(report,"%s L zooms out\n",ok?"PASS":"FAIL");failures+=!ok;
 zoom=chart_zoom;for(int i=0;i<8;i++)input(0,PSP_CTRL_RTRIGGER,.05f,0,0);ok=chart_zoom>zoom;
 fprintf(report,"%s R zooms in\n",ok?"PASS":"FAIL");failures+=!ok;
 float pan=chart_pan_x;input(0,0,.05f,.8f,0);ok=chart_pan_x<pan;
 fprintf(report,"%s analog nub pans\n",ok?"PASS":"FAIL");failures+=!ok;
 pan=chart_pan_y;input(0,0,.05f,0,.8f);ok=chart_pan_y>pan;
 pan=chart_pan_y;input(0,0,.05f,0,-.8f);ok=ok&&chart_pan_y<pan;
 fprintf(report,"%s chart vertical nub direction is inverted both ways\n",ok?"PASS":"FAIL");failures+=!ok;
 float goals[2];for(int test=0;test<2;test++){
  int hz=test?10:60;chart_zoom=chart_zoom_goal=1;
  for(int frame=0;frame<hz;frame++)input(0,PSP_CTRL_RTRIGGER,chart_control_dt(1.f/hz),0,0);
  goals[test]=chart_zoom_goal;
 }
 ok=fabsf(goals[0]-goals[1])<.001f&&fabsf(goals[0]-2.15f)<.001f&&chart_control_dt(20)==.15f;
 fprintf(report,"%s zoom hold speed is wall-clock consistent at 10 and 60 FPS with stall protection\n",ok?"PASS":"FAIL");failures+=!ok;
 preview_reset();rect(0,0,16,16,RGB(0,0,0));chart_text(0,0,WHITE,"A");ok=1;
 for(int yy=0;yy<10;yy++)for(int xx=0;xx<10;xx++){
  unsigned wanted=yy<7&&xx<5&&(chart_letters[0][yy]&(16>>xx))?WHITE:RGB(0,0,0);
  if(fb[yy*STRIDE+xx]!=wanted)ok=0;
 }
 fprintf(report,"%s small chart glyph uses exact 5x7 pixels with no stretching\n",ok?"PASS":"FAIL");failures+=!ok;
 ok=1;for(unsigned i=0;i<512;i++){
  unsigned a=i*2654435761u,b=i*2246822519u;int t=i%257,s=256-t;
  unsigned expected=RGB((((a&255)*s)+((b&255)*t))>>8,((((a>>8)&255)*s)+(((b>>8)&255)*t))>>8,((((a>>16)&255)*s)+(((b>>16)&255)*t))>>8);
  if(chart_lerp(a,b,t)!=expected)ok=0;
 }
 fprintf(report,"%s faster nebula blending is pixel-identical to reference math\n",ok?"PASS":"FAIL");failures+=!ok;
 chart_cursor=goal;input(PSP_CTRL_CROSS,0,.016f,0,0);ok=game.route_goal==goal&&selected_target==ROUTE_TARGET_ID;
 fprintf(report,"%s plot keeps the route-star targeting integration\n",ok?"PASS":"FAIL");failures+=!ok;
#include "deep-chart-filter-tests.h"
 chart_zoom=chart_zoom_goal=1;chart_pan_x=chart_pan_y=0;chart_filter=CHART_ALL;
 uint64_t begin,end;sceRtcGetCurrentTick(&begin);for(int i=0;i<20;i++)galaxy_overview();sceRtcGetCurrentTick(&end);
 fprintf(report,"Warm all-systems chart draw: %.2f ms\n",(end-begin)*1000.f/sceRtcGetTickResolution()/20.f);
 /* Same camera/route for every filter; exercise real input during zoom. */
 for(int f=0;f<CHART_FILTERS;f++){
  chart_filter=f;chart_zoom=chart_zoom_goal=1;chart_pan_x=chart_pan_y=0;
  galaxy_overview();sceRtcGetCurrentTick(&begin);
  for(int frame=0;frame<16;frame++){input(0,PSP_CTRL_RTRIGGER,1.f/30,0,0);galaxy_overview();}
  sceRtcGetCurrentTick(&end);
  fprintf(report,"Filter %d zoom draw: %.2f ms; zoom %.3f\n",f,(end-begin)*1000.f/sceRtcGetTickResolution()/16.f,chart_zoom);
 }
 for(int i=0;i<CHART_FILTERS;i++){
  char capture[40];snprintf(capture,sizeof(capture),"chart-filter-%d.bmp",i);
  chart_filter=i;chart_zoom=1;chart_pan_x=chart_pan_y=0;galaxy_overview();dump_native_bmp(capture);
  chart_filter=i;chart_zoom=i&1?3.2f:.65f;chart_pan_x=i&2?400.f:-400.f;chart_pan_y=i&1?250.f:-250.f;
  galaxy_overview();
 }
 chart_cursor=game.system;chart_zoom=1;chart_pan_x=chart_pan_y=0;galaxy_overview();
 int intact=1;for(int i=0;i<16;i++)if(pixels[i]!=0xa55ac33cu||pixels[STRIDE*H+16+i]!=0xa55ac33cu)intact=0;
 fprintf(report,"%s framebuffer guards survive zoom, pan, filters and current-system selection\n",intact?"PASS":"FAIL");failures+=!intact;
 fprintf(report,"RESULT %d failures\n",failures);fclose(report);fb=saved;free(pixels);return 1;
}
