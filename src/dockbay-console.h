/* Approved DockBay v3. Native glyphs and primitives; only visible cards draw. */
#include "dockbay-icons.h"
static int db_detail=-1,db_filter,db_cursor,db_sheet,db_choice=1,db_quantity=1,db_release,db_replace;
static unsigned db_revision;static float db_clock,db_motion,db_pulse;static char db_notice[96];
static const char *db_filters[]={"ALL SYSTEMS","NEARBY","EQUIPMENT","CARGO","MY DEALS"};
static const char *db_sites[]={"STATION PERIMETER","OUTER TRADE LANES","ASTEROID SHELF","SALVAGE FIELD","RING APPROACH","QUIET ANCHORAGE"};
static const char *db_headlines[20]={"HEAT BUFFER. MOSTLY LEGAL.","SPARES. MOVING OUT.","LASER. DIVORCE SALE.","COLLECTOR'S PRIVATE SALE.","MY FIRST SALE. BE NICE.","FOOD. PLEASE HURRY.","MINERALS BY THE TONNE.","MECHANIC'S SPARE PART.","BUYING CARGO. CASH READY.","MODDED. EXCESS ENTHUSIASM.","TRAVELLING PARTS SHOP.","CREW'S CLEARANCE SALE.","ANYONE AWAKE OUT THERE?","WORN. STILL TRIES HARD.","COURIER'S PRIVATE STOCK.","ONE CAREFUL OWNER.","I'VE GOT JUST THE THING.","FOUND BESIDE A WRECK.","MERCHANT CONVOY SALE.","YOUR REGULAR SUPPLIER."};
static const char *db_pitches[20]={
 "Kept my engine cool. Marriage still failed. Come and meet me outside the hub.",
 "The landlord counts my hold as a bedroom. Please help me clear the floor.",
 "Apparently the laser was the problem. Collection only. Don't mention my ex.",
 "An unusual piece from my collection. Exact figures supplied; no surprises.",
 "Never sold anything before. I even polished the box. Please be nice.",
 "The fridge broke. Everything smells brave. Bring a hauler. No expiry date!",
 "You bring the hold space. I'll provide the suspicious rattling noise.",
 "A proper permanent part, tested on my own ship. Greasy fingerprints included.",
 "Crew has started naming the emergency biscuits. We need to end this.",
 "Goes better than I can comfortably explain. The numbers do the talking.",
 "My shop has engines instead of a front door. No parking validation needed.",
 "Crew vote says the cupboard must go. Goods, not the actual cupboard.",
 "Yes, I really have something to sell. Also, how has your day been?",
 "Not as quick as it once was. Neither am I. Worn means worn for good.",
 "Special delivery, no deadline. I waited years for this one. Take a look.",
 "Three previous owners weren't careful. At least the asking price is fixed.",
 "Still available, Commander. Nothing changes hands until you say yes.",
 "Recovered it myself. The wreck is optional scenery, not part of the price.",
 "Hail my named ship, please. The others only want to talk about lunch.",
 "New stock, same pilot. I remembered your ship. Come and have a look."};
#include "dockbay-pages.h"
