/* Living galaxy texture — radio hails, freight colour, rare Voss sightings.
 * Narrative only. No YOU: chrome. Short enough for canopy speech. */
#ifndef LIVING_TEXTURE_H
#define LIVING_TEXTURE_H

/* Rare Voss GalNet sightings after Open Channel (Ch.22 / saga 20+) — Seed E6. */
static const char *living_voss_sighting(const Game *g){
 if(!g||g->campaign_stage<6||g->saga_chapter<21)return 0;
 /* Sparse: only some systems show the rumour so it stays rare. */
 if(((g->system*23)&7)>2)return 0;
 if(g->saga_trust[SAGA_TRUST_LAW]>=2)
  return "Sighting: Voss entered a hearing. Receipts still breathing.";
 if(g->saga_trust[SAGA_TRUST_PUBLIC]>=2)
  return "Sighting: Crown emptied. Ground unsalted. Argument unfinished.";
 if(g->saga_trust[SAGA_TRUST_INDEPENDENT]>=1)
  return "Rumour: he'll surface. No revenge fetch required.";
 return "Unconfirmed Voss ping. Lane open. Do not invent a hunt.";
}

/* Trader hail (non-freighter) — Open Channel echoes when flags are set. */
static const char *living_trader_hail(const Game *g){
 static const char *plain[]={
  "Market's open at the hub. Don't scrape the paint.",
  "Margins thin today. Dinner still ships.",
  "Hub berth gossip tax is unpaid. As usual.",
  "If you need crates, take the board. If you need jokes, take Dockhand."
 };
 if(g&&g->campaign_stage>=6){
  if(g->saga_flags&2)return "Settlements stocking early. Buy food, not safe-route ads.";
  if(g->saga_flags&1)return "Chart dump on GalNet. Buying nothing labelled safe.";
  if(g->saga_flags&4)return "Coalition chatter on the wire. Allies, not owners.";
  if(g->saga_chapter>=SAGA_COUNT)return "Berth six stayed warm. Free flight still pays dinner.";
 }
 return plain[g?(g->system*5)&3:0];
}

/* Freighter mood — cargo line preferred; mood replaces when campaign colours the sky. */
static const char *living_freight_hail(const Game *g){
 if(!g||g->campaign_stage<6)return 0;
 if(g->saga_flags&4&&g->saga_trust[SAGA_TRUST_PUBLIC]>=2)
  return "Lower Light wing: tenders on your vector. No speeches. Fuel.";
 if(g->saga_flags&2)
  return "Slow convoy. Migration bulletin revised. Give us room.";
 if(g->saga_flags&1)
  return "Hauling sealed crates. Boring lie on the label. Prefer it.";
 if(g->saga_chapter>=18)
  return "Public ribbon traffic. Prefer living arguments to wreckage.";
 return 0;
}

static const char *living_law_hail(const Game *g){
 if(g&&g->legal){
  if(g->campaign_stage>=6&&(g->saga_flags&8))
   return "Stop. Warrant live. Inspection calendar is not decorative.";
  return "Stop. Pay the fine or take custody.";
 }
 if(g&&g->campaign_stage>=6){
  if(g->saga_flags&4)return "Clear. Suspend language ready if paint crosses the lane.";
  if(g->saga_flags&1)return "Clear. Evidence cage sealed. Keep the lane honest.";
  if(g->saga_chapter>=21)return "Clear. Unidentified echo — do not invent gods.";
 }
 return "Clear. Keep the lane clean.";
}

static const char *living_pirate_hail(const Game *g){
 /* Most pirates stay silent (paint). Rare taunt when Independent trust or Silence flag. */
 if(!g||g->campaign_stage<6)return 0;
 if(g->saga_trust[SAGA_TRUST_INDEPENDENT]>=1&&((g->system)&1)==0)
  return "Toll. Or the corridor Sable left. Choose.";
 if(g->saga_flags&1&&((g->system*3)&3)==0)
  return "Chart dump made clinics loud. Prices updated. Pay.";
 return 0;
}

static const char *living_explorer_hail(const Game *g){
 if(g&&g->campaign_stage>=6){
  if(g->saga_flags&4)return "Survey channel. Markers mid-cut — measurement as courage.";
  if(g->saga_chapter>=16)return "Survey channel. Curiosity without munitions.";
  if(g->saga_flags&2)return "Survey channel. Observe migrations. Do not herd.";
 }
 static const char *plain[]={
  "Survey channel. We are mapping this sky.",
  "Survey channel. Listen twice before you name it.",
  "Survey channel. Peer review later. Scan now.",
  "Survey channel. Leave the sky where you found it."
 };
 return plain[g?(g->system*7)&3:0];
}

static const char *living_tower_hail(const Game *g){
 if(g&&g->campaign_stage>=6&&g->saga_chapter>=SAGA_COUNT)
  return "Tower. Berth six is yours. Try not to look surprised.";
 if(g&&g->campaign_stage>=6&&(g->saga_flags&2))
  return "Tower. Quiet warnings kept traffic breathing. Slot?";
 return "Tower. Need a docking slot?";
}

static const char *living_anomaly_hail(const Game *g){
 if(g&&g->campaign_stage>=6&&g->saga_chapter>=16)
  return "Echo on the wire. Close in. Scan. Do not fire.";
 return "That's an echo. Close in and scan.";
}

/* Spacebook Dockhand / Spotters — extra rotating jokes + lore. */
static const char *living_dockhand_post(int system){
 static const char *bank[]={
  "Stop boosting near the collar, you maniacs.",
  "Boost pedal is not a personality. Tell your friends.",
  "If your approach looks like a dare, abort. Prefer boring.",
  "Paint is expensive. Your thrusters are not a brush.",
  "Cats unionised the warm pipes. Respect the picket.",
  "I married a boost pedal once. Divorce was quieter."
 };
 return bank[(system*11)&5];
}

static const char *living_spotters_post(const Game *g){
 const char *voss=living_voss_sighting(g);
 if(voss)return voss;
 static const char *bank[]={
  "Saw Law chasing raiders beyond the trade lane.",
  "Whale-song on the edge of scanner. Leave them the road.",
  "Meridian brochure in the canteen. Timestamp looked nervous.",
  "Guild named another rock. Peer review still arguing.",
  "Berth six lights looked warmer tonight. Gossip tax pending.",
  "Unidentified echo — do not approach. Do not invent gods."
 };
 return bank[g?(g->system*9)&5:0];
}

#endif
