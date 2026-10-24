/* Authored lightweight discoveries. Fixed seed and no per-frame allocation.
 * Certification slice only: Lave plus systems 0, 1 and 2. No services implied. */
typedef struct {const char *name,*reply;} SpaceDiscovery;
static const SpaceDiscovery discovery_catalogue[75]={
{"Patchwork Fuel Stop","Pump two works. Pump one is decorative now."},
{"Salvage Village","Mind the approach lights. We fitted those ourselves."},
{"Mobile Mechanic","If it rattles, we can probably fix it. If it sings, ask next door."},
{"Community Greenhouse","First harvest this month. We're rather proud of it."},
{"Old Passenger Ferry","Next stop: the inner settlements. Please keep the loading ramp clear."},
{"Scrap Sculpture","A local monument constructed from retired spacecraft."},
{"Miners' Tea Barge","Kitchen's open. Don't ask what happened to the last kettle."},
{"Retired Patrol Ship","She's had three careers. This one's the quietest."},
{"Cooperative Ore Depot","Independent crews, shared equipment. It works better than you'd think."},
{"Parts Exchange","Looking is free. Please stop scanning the proprietor."},
{"Beacon Repair Crew","We'll have the light back shortly. Follow the working ones."},
{"Freighter Housing Block","Residential berth. Deliveries use the other end."},
{"Closed Resort","Your perfect getaway awaits."},
{"Emergency Water Tanks","Reserve levels stable. Thank you for checking."},
{"Local Radio Shack","You're passing through the loudest room in the system."},
{"Shipbreaker's Yard","Stay beyond the yellow markers. Things come loose."},
{"Memorial Lanterns","A memorial to crews lost on this route."},
{"Neighbourhood Cinema","Tonight's feature has sound again. Mostly."},
{"Trading Family's Barge","Third generation aboard. Same engine, apparently."},
{"Makeshift Sports Ground","Practice session. Please don't fly through the goal."},
{"Private Yacht Marina","Visitor berth enquiries are handled by reception."},
{"Orbital Hotel","Welcome to the view. Accommodation enquiries are available at reception."},
{"Casino Station","Fortune favours the visitor. Terms and conditions also apply."},
{"Botanical Habitat","The gardens are enjoying an artificial spring."},
{"Art Gallery","The large blue object is an exhibit. Please stop requesting docking clearance from it."},
{"Showroom Shipyard","Today's display: tomorrow's regrettable financial decision."},
{"Research Campus","Routine observations in progress. Your exhaust is unexpectedly interesting."},
{"Luxury Cruise Liner","You're alongside the Velvet Horizon. Enjoy the view and keep clear of our shuttles."},
{"Solar-Sailing Club","Leisure flight in progress. Kindly reduce your wake."},
{"Private Observatory","We're watching something very distant. You're currently in the foreground."},
{"Perfume Advertisement","NEBULA. Leave an impression across several docking bays."},
{"Fine-Dining Satellite","A table with a planetary view? Naturally."},
{"Medical Retreat","Recovery facilities operating normally. Please respect the quiet zone."},
{"Precision Factory","Production cycle nominal. Public tours remain entirely virtual."},
{"Executive Shuttle Lounge","Your pilot has been notified. If you are the pilot, please disregard."},
{"Collector's Dock","None are for sale. Yes, that includes the broken one."},
{"Orbital Spa","Relax. Your ship can remain anxious outside."},
{"Designer Freight Terminal","Freight movement active. Please follow the illuminated lanes."},
{"Pleasure-Garden Ferry","On your left: the planet. On your right: also the planet."},
{"Charity Habitat","Food, shelter and onward passage. Everyone deserves a safe arrival."},
{"Grand Passenger Interchange","Connecting services depart throughout the cycle. Please check your destination."},
{"Capital Ship Construction Dock","Construction perimeter. The unfinished end is not a docking entrance."},
{"Asteroid Theme Park","Welcome to Orbital Screams. All screaming remains inside the vehicle."},
{"Grand Casino Complex","A whole world of entertainment. Conveniently detached from an actual world."},
{"Diplomatic Station","Diplomatic arrivals only. General enquiries use the public channel."},
{"Fleet Anchorage","Anchorage control. Maintain your current clearance distance."},
{"Broadcasting Centre","You are listening to the system's most confidently informed broadcaster."},
{"Orbital Stadium","Tonight's final is sold out. The commentary is considerably less exclusive."},
{"University Habitat","Admissions are open. Parking remains a matter of academic debate."},
{"Historic First Station","From these modest berths, the city above the world began."},
{"Monument to Navigation","Vast illuminated sculpture visible from far away."},
{"Grand Hotel Spire","Reception is delighted to acknowledge your presence."},
{"Orbital Exhibition Hall","Current exhibition: Engines That Changed Our Lives; and Several Insurance Policies."},
{"Freight Megaport","Cargo operations active. Your designated approach remains clear."},
{"Cruise Assembly Point","The grand tour departs shortly. Please wave responsibly."},
{"Public Garden Ring","Public gardens open throughout the cycle. No purchase required."},
{"Entertainment Billboard Cluster","Large fictional advertisements beside a busy route."},
{"Orbital Aquarium","Please do not tap the exterior. We can hear it."},
{"Civil Transport Depot","Public transport keeps this system moving. Usually in the intended direction."},
{"Festival Platform","Festival week is underway. Local time has become a suggestion."},
{"Listening Post","We listen so you don't have to. Nothing urgent today."},
{"Abandoned Giant Freighter","No active crew signal. The hull shows extensive damage."},
{"Animal Transport Cage","Licensed wildlife transport. Keep your engines clear of the enclosure."},
{"Wildlife Sanctuary Buoy","Observe from a distance. Feeding the wildlife is strongly discouraged."},
{"Space-Fauna Group","Several life signs. Their movement appears coordinated."},
{"Comet Fragment","Volatile ice. Keep your distance from the active vents."},
{"Old Navigation Buoy","An outdated formal recording identifies the route."},
{"Survey Drone","Survey in progress. You have been classified as temporary scenery."},
{"Cargo Container Chain","Oversized load. We turn slowly and apologise in advance."},
{"Empty Fuel Tanker","Empty return leg. Nothing to buy, plenty to admire."},
{"Remote Weather Monitor","Surface forecast: unpleasant. Orbital forecast: unchanged."},
{"Private Memorial Ship","A short dedication names its former captain."},
{"Travelling Advertisement Ship","Your business could be annoying pilots right here."},
{"Maintenance Drone Group","Automated repair zone. Please avoid becoming additional work."},
{"Unidentified Wreck Fragment","Identification inconclusive. Recording sensor data."},
};
static int discovery_count(void){return opening_active(&game)||(!navigation_enabled(&game)&&game.system>2&&game.system!=7)?0:8;}
static unsigned discovery_seed(int slot){return ((unsigned)game.system+1)*2654435761u+(unsigned)(slot+1)*2246822519u;}
static int discovery_capture_entry=-1;
static int discovery_entry(int slot){
 if(discovery_capture_entry>=0&&slot==4)return discovery_capture_entry;
 if(slot<4)return 60+slot; /* Listening post, wreck, enclosure, sanctuary. */
 unsigned h=discovery_seed(slot);int base=game.systems[game.system].economy<=2?20:0;
 if(game.systems[game.system].government>=6&&game.systems[game.system].tech>=10)base=40;
 return base+(h>>8)%20;
}
static Vec3 discovery_compute_position(int slot){return exploration_discovery_position(&game,slot);}
static Vec3 discovery_position(int slot){
 static Vec3 points[8];static int sys=-1,mode=-1;static Vec3 anchor;
 int enabled=navigation_enabled(&game);Vec3 body=game.bodies[1].pos;
 if(sys!=game.system||mode!=enabled||memcmp(&anchor,&body,sizeof(body))){sys=game.system;mode=enabled;anchor=body;for(int k=0;k<8;k++)points[k]=exploration_discovery_position(&game,k);}
 return slot>=0&&slot<8?points[slot]:exploration_discovery_position(&game,slot);
}
static const SpaceDiscovery *discovery_data(int id){return &discovery_catalogue[discovery_entry(id-DISCOVERY_ID_MIN)];}

/* Explicit source identity: never infer a discovery speaker from a faction. */
static const char *discovery_speakers[75]={"FUEL CLERK","VILLAGER","MECHANIC","GROWER","FERRY CREW","COMPUTER","BARGE COOK","WORKSHOP","DEPOT CLERK","PARTS DEALER","REPAIR CREW","RESIDENT","RECORDING","TANK MONITOR","RADIO HOST","YARD FOREMAN","COMPUTER","CINEMA HOST","BARGE CREW","COACH","MARINA DESK","HOTEL DESK","CASINO HOST","BOTANIST","CURATOR","SHIP DEALER","RESEARCHER","CRUISE HOST","SAILING CLUB","ASTRONOMER","ADVERTISING","HEAD CHEF","MEDICAL TEAM","FACTORY DESK","LOUNGE HOST","COLLECTOR","SPA HOST","CARGO CLERK","FERRY GUIDE","RELIEF TEAM","TRANSIT DESK","DOCK FOREMAN","PARK HOST","CASINO HOST","DIPLOMAT","FLEET COMMS","BROADCASTER","COMMENTATOR","ADMISSIONS","RECORDING","COMPUTER","HOTEL DESK","EXHIBIT HOST","PORT CONTROL","CRUISE DESK","GARDENER","ADVERTISING","AQUARIST","TRANSIT DESK","FESTIVAL MC","POST CONTROL","COMPUTER","KEEPER","RANGER","COMPUTER","COMPUTER","RECORDING","SURVEY DRONE","TUG CAPTAIN","TANKER CREW","WEATHER UNIT","COMPUTER","ADVERTISING","REPAIR DRONE","COMPUTER"};
static int discovery_voice_entry(void){int e=-1000-game.voice_seed;return game.voice_who==VOICE_CONTACT&&e>=0?e%75:-1;}
static const char *discovery_voice_label(void){int e=discovery_voice_entry();return e>=0?discovery_speakers[e]:0;}
static int discovery_voice_automated(void){const char *n=discovery_voice_label();return n&&(!strcmp(n,"COMPUTER")||!strcmp(n,"RECORDING")||strstr(n,"DRONE")||!strcmp(n,"TANK MONITOR")||!strcmp(n,"WEATHER UNIT")||!strcmp(n,"ADVERTISING"));}
