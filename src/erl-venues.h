/* A championship belongs to its Mega Capital system.  Every round remains
 * there so progress, results and presentation all share the same owner. */
int erl_round_venue(const Game *g,int round){
 (void)round;return g->erl.championship?g->erl.champ_system:g->system;
}
static int erl_prepare_venue(Game *g,int venue){
 if(venue==g->system){race_prepare(g);return g->race.phase!=RACE_NONE;}
 /* Entry-only scratch state. Never relocate the actual commander, consume
  * fuel, spawn foreign traffic, or rebuild the accepted course in transit. */
 Game *preview=malloc(sizeof(*preview));if(!preview)return 0;*preview=*g;
 preview->system=venue;preview->station_variant=0;system_bodies(preview);
 int ok=race_course_build(preview,0,&preview->race);
 if(ok){race_grid_ships(preview);g->race=preview->race;}
 free(preview);return ok;
}
