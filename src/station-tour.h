/* Station welcome tour: a bounded, session-local mission for the authored bar. */
/* Reorte; its primary hub owns THE SECOND SHIFT. */

static int station_tour_active(void){
 return station_tour_stage>=STATION_TOUR_ROUTE&&station_tour_stage<STATION_TOUR_DONE;
}
static const char *station_tour_title(void){return "STATION WELCOME / SECOND SHIFT";}
static const char *station_tour_objective(void){
 if(station_tour_stage==STATION_TOUR_ROUTE)return "Fly to Reorte, then dock at the primary hub.";
 if(station_tour_stage==STATION_TOUR_DOCK)return "Dock at Reorte Hub to begin the station walk.";
 if(station_tour_stage==STATION_TOUR_WALK)return "Disembark and WALK from Arrivals to CANTEEN.";
 if(station_tour_stage==STATION_TOUR_BAR)return "Enter THE SECOND SHIFT and choose LYSA KEST.";
 if(station_tour_stage==STATION_TOUR_TALK)return "Talk to LYSA KEST, the bar's first contact.";
 return "Station welcome complete.";
}
static void station_tour_begin(void){
 if(station_tour_stage!=STATION_TOUR_OFF)return;
 if(smoke)return; /* automated smoke scenes must remain deterministic */
 if(game.docked&&game.system==7){
  station_tour_stage=STATION_TOUR_ROUTE;
  message(&game,"NEW MISSION: Station Welcome. Reorte Hub has a bar to explore.");
  game.cue=SFX_UI;
 }
}
static void station_tour_tick(void){
 station_tour_begin();
 if(!station_tour_active())return;
 if(station_tour_stage==STATION_TOUR_ROUTE){
  if(game.system==7&&!game.docked){
   route_set_goal(&game,STATION_TOUR_DEST);route_refresh_destination(&game);
   station_tour_stage=STATION_TOUR_DOCK;
   message(&game,"Station Welcome: follow the plotted route to Reorte.");
  }
 }else if(station_tour_stage==STATION_TOUR_DOCK){
  if(game.system==STATION_TOUR_DEST&&game.docked){
   station_tour_stage=STATION_TOUR_WALK;
   route_clear(&game);
   message(&game,"Docked at Reorte Hub. Disembark, then walk to CANTEEN.");
  }
 }else if(station_tour_stage==STATION_TOUR_WALK){
  if(page==WALK&&sc_room==SC_R_CANTEEN&&bar_preview_at()){
   station_tour_stage=STATION_TOUR_BAR;
   message(&game,"THE SECOND SHIFT is open. Choose LYSA KEST to talk.");
   game.cue=SFX_UI;
  }
 }else if(station_tour_stage==STATION_TOUR_BAR){
  if(page==WALK&&sc_room==SC_R_CANTEEN&&bar_preview_at()&&sc_menu==SC_MENU_TALK&&sc_talk_who==0){
   station_tour_stage=STATION_TOUR_TALK;
   message(&game,"Lysa Kest is waiting by the window. Choose HEAR THEM OUT.");
   game.cue=SFX_TALK;
  }
 }else if(station_tour_stage==STATION_TOUR_TALK){
  if(page==WALK&&sc_room==SC_R_CANTEEN&&bar_preview_at()&&sc_menu==SC_MENU_NONE&&game.message_time>0){
   station_tour_stage=STATION_TOUR_DONE;
   message(&game,"Station Welcome complete. The Second Shift is now on your map.");
   game.cue=SFX_UI;
  }
 }
}
static void station_tour_action(void){
 if(!station_tour_active())return;
 if(station_tour_stage==STATION_TOUR_ROUTE||station_tour_stage==STATION_TOUR_DOCK){
  route_set_goal(&game,STATION_TOUR_DEST);route_refresh_destination(&game);change_page(CHART);
  message(&game,"Route plotted: Reorte. Choose the highlighted next jump.");
 }else if(station_tour_stage==STATION_TOUR_WALK||station_tour_stage==STATION_TOUR_BAR||station_tour_stage==STATION_TOUR_TALK){
  if(game.docked){walk_kind=0;walk_x=walk_z=walk_yaw=0;sc_built_for=-1;page=WALK;message(&game,"Station Welcome: walk to THE SECOND SHIFT.");}
  else message(&game,"Dock at Reorte Hub before walking the station.");
 }
}
