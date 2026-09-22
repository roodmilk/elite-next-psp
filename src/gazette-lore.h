/* Galactic Gazette tabloid + faction lore banks.
 * Narrative texture only — funny/tabloid pieces that still carry lore.
 * No YOU: chrome. Distinct faction voices. Bound by PSP line lengths. */
#ifndef GAZETTE_LORE_H
#define GAZETTE_LORE_H

/* ~3/8 of Gazette desk posts go tabloid/joke; rest stay straight local news. */
static int gazette_wants_tabloid(int system,int slot){
 return ((system*17+slot*11)&7)<3;
}

/* Author mastheads for tabloid / sidebar pieces (no YOU:). */
static const char *gazette_tabloid_author(int system,int slot){
 static const char *a[]={
  "CITY DESK","SIDEBAR","WHALE WATCH","SOCIETY PAGE","RUMOUR MILL",
  "DOCK GOSSIP","LOST + FOUND","OUTBOUND LIES","NIGHT BEAT","SAFE-ROUTE ADS"
 };
 return a[(system*5+slot*3)%10];
}

/* Funny / tabloid body lines that still leak useful world texture. ≤~52 chars. */
static const char *gazette_tabloid_body(int system,int slot){
 static const char *bank[]={
  /* jokes with teeth */
  "Local chef claims pirates tip better than Law. Dispute ongoing.",
  "Dockhand_77 married a boost pedal. Reception was loud.",
  "Pilot mistook a gas giant for a parking buoy. Fine pending.",
  "Anonymous: 'I am not a pirate.' Followed by a wink emoji. Banned.",
  "Station cats declared a union. Demands: warmer pipes, less Law.",
  "VIPER pilots invent 'casual pursuit.' Civilians remain unamused.",
  "Trader sold 'lucky jump fuel.' Luck was other people's misfortune.",
  "Guild surveyor named a rock 'Steve.' Peer review delayed six weeks.",
  /* tabloid lore */
  "EXCLUSIVE: Meridian 'safe route' brochure printed before the survey.",
  "Who keeps the night berths warm? Sources say unpaid favours.",
  "Whale-song chart leak: clinics prepared; pirates updated prices.",
  "Three governments smile the same smile. Map still has doubts.",
  "Iona's evidence cage: ugly, legible, enough. Badge theatre denied.",
  "Sable Rook denies owning the gas giant. 'I lease the interesting bits.'",
  "Kei over-explains again. Readers demand a shorter ping.",
  "Ryn handwriting spotted in whale-song. Meridian calls coincidence.",
  /* more jokes */
  "Wanted poster spelling contest ends in a warrant.",
  "Freighter named Lower Light refuses loftier titles. Pride intact.",
  "Anomaly scanned twice. Second scan still confusing. Guild pleased.",
  "Customs officer stamps DENIED, then DENIED AGAIN for emphasis.",
  "Market tip: hunger is never out of season. Margins disagree.",
  "Pirate hail: 'This is a toll.' Pilot: 'This is a no.' Both correct.",
  "Lave Hub gossip tax remains unpaid. Venn files it under weather.",
  "Explorer Guild seeks boring pilots. Drama applicants redirected."
 };
 return bank[(system*13+slot*7)%24];
}

/* Secondary dek under the headline on the paper layout. */
static const char *gazette_dek(int system,int slot){
 if(gazette_wants_tabloid(system,slot)){
  static const char *tab[]={
   "Sources spoke on condition of remaining slightly wrong.",
   "Readers advised: jokes may contain navigational hazards.",
   "Continued on GalacticNet — bring coffee, not a sermon.",
   "Editorial note: we regret the punctuation. Not the scoop.",
   "If this is accurate, someone's stationery just got richer."
  };
  return tab[(system+slot)%5];
 }
 static const char *straight[]={
  "Pilots are advised to check routes before launch.",
  "Scanner reports update throughout the local day.",
  "Dock control asks commanders to approach at safe speed.",
  "Prices remain available only at a docked terminal.",
  "Guild observers invite verified field reports."
 };
 return straight[slot>=0&&slot<5?slot:0];
}

/* Left-list tag under faction name — short, distinct, no YOU:. */
static const char *faction_lore_tag(int role){
 static const char *t[]={
  "Margins with a pulse.",
  "Receipts over sermons.",
  "Knives with invoices.",
  "Listen twice. Stamp once."
 };
 if(role<0||role>=FACTION_COUNT)role=0;
 return t[role];
}

/* Right-panel lore lines — rotate by system so hubs feel different. */
static const char *faction_lore_line(int role,int which,int rot){
 /* which 0 or 1; rot indexes the bank */
 static const char *traders0[]={
  "Convoy lanes feed the hub — and the gossip tax.",
  "A crate is a promise with thrusters.",
  "Hail them for market talk; tip if the joke lands.",
  "Hungry pads outrank pretty charts. Always."
 };
 static const char *traders1[]={
  "They will sell you dinner and a rumour in one crate.",
  "Freight remembers every unpaid favour.",
  "Slow convoy crossing: give them room, take the story.",
  "Margins thin. Hunger not. That is the whole brief."
 };
 static const char *law0[]={
  "Patrols stop wanted ships. Clocks are witnesses.",
  "Pay a fine or take custody — choose like tomorrow exists.",
  "Suspend language works often enough to matter.",
  "Early badges do not get to become tomorrow's truth."
 };
 static const char *law1[]={
  "Warrants stay local. Drama does not buy a free lane.",
  "Ugly and correct beats a flattering fog.",
  "Inspection days are mandatory, not decorative.",
  "They will not invent gods from unresolved returns."
 };
 static const char *pirates0[]={
  "Raid passing traders. Update prices after refusals.",
  "Missiles lock hostiles only — manners are optional.",
  "A wink on the wire is still a boarding plan.",
  "Toll booths with thrusters. Pay, flee, or argue."
 };
 static const char *pirates1[]={
  "Uneasy friends are still friends — bargains stay explicit.",
  "They tip better than Law when the joke is mutual.",
  "Quiet corridors cost interest. Six minutes of honesty.",
  "Free cargo inspection is a joke until it isn't."
 };
 static const char *guild0[]={
  "Survey nearby worlds. Peer review slows the knife.",
  "They will not fire first — curiosity is their weapon.",
  "Markers tell truth mid-cut. Measurement as courage.",
  "Listen twice. Then leave the sky where you found it."
 };
 static const char *guild1[]={
  "Anomaly work is a prayer with a checklist.",
  "Boring pilots keep the channel open. Drama is optional.",
  "They name rocks carefully. Continuity without perfume.",
  "Verified field reports beat forged safe-route ads."
 };
 const char **a=traders0,**b=traders1;
 if(role==LAW){a=law0;b=law1;}
 else if(role==PIRATES){a=pirates0;b=pirates1;}
 else if(role==EXPLORERS){a=guild0;b=guild1;}
 const char **bank=which?b:a;
 return bank[rot&3];
}

#endif
