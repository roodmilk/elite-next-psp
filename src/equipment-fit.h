/* Elite Dangerous-style fitted slots. Six hardpoints mirror the loadout UI.
 * Bits in game.upgrades remain the sim authority; fit[] holds the fitted catalog
 * index (or FIT_EMPTY) so Outfitting / Loadout / credits agree. Save V13. */
enum { FIT_SLOTS = 6, FIT_EMPTY = 0xff };
enum { FIT_WPN = 0, FIT_DEF = 1, FIT_NAV = 2, FIT_HOLD = 3, FIT_FUEL = 4, FIT_UTIL = 5 };

static int equip_slot_for(int i){
 if(i==1||i==2||i==20)return FIT_WPN;
 if(i==6||i==7||i==16)return FIT_DEF;
 if(i==4||i==5||i==12||i==13)return FIT_NAV;
 if(i==10||i==11||i==22||i==23)return FIT_HOLD;
 if(i==14||i==15)return FIT_FUEL;
 if(i==8||i==9||i==17||i==18||i==19||i==21||i==24)return FIT_UTIL;
 return -1; /* refuel / missiles are services, not slots */
}

static int fit_value_valid(int slot,int i){
 return slot>=0&&slot<FIT_SLOTS&&(i==FIT_EMPTY||(i>0&&i<25&&equip_slot_for(i)==slot));
}

static int equip_mask_for(int i){
 if(i==4)return 1;
 if(i==5)return 1024;
 if(i==6)return 2;
 if(i==7)return 2|128;
 if(i==8)return 4;
 if(i==9)return 2048;
 if(i==10||i==23)return 8;
 if(i==11)return 8|64;
 if(i==12)return 16;
 if(i==13)return 4096;
 if(i==14)return 32;
 if(i==15)return 32|8192;
 if(i==16||i==17)return 256;
 if(i==18)return 16384;
 if(i==19)return 32768;
 if(i==20)return 65536;
 if(i==21)return 131072;
 if(i==24)return 262144;
 if(i==22)return 512;
 return 0;
}

static void fit_clear_all(Game *g){for(int s=0;s<FIT_SLOTS;s++)g->fit[s]=(uint8_t)FIT_EMPTY;}

/* Rebuild upgrades + laser from fitted slots so bits never drift. */
static void fit_rebuild(Game *g){
 int bits=0,has_laser=0;
 for(int s=0;s<FIT_SLOTS;s++){
  int i=g->fit[s];
  if(i==FIT_EMPTY||i<=0||i>=25){g->fit[s]=(uint8_t)FIT_EMPTY;continue;}
  bits|=equip_mask_for(i);
  if(i==1||i==2||i==20)has_laser=1;
 }
 g->upgrades=bits;
 g->laser=has_laser?1:0;
}

static void fit_synthesize(Game *g){
 fit_clear_all(g);
 /* Priority matches the old loadout display when migrating V12→V13. */
 if(g->laser){
  if(g->upgrades&65536)g->fit[FIT_WPN]=20;
  else g->fit[FIT_WPN]=2; /* legacy laser bit was always beam-class damage */
 }
 if(g->upgrades&128)g->fit[FIT_DEF]=7;
 else if(g->upgrades&2)g->fit[FIT_DEF]=6;
 else if(g->upgrades&256)g->fit[FIT_DEF]=16;
 if(g->upgrades&16)g->fit[FIT_NAV]=12;
 else if(g->upgrades&1)g->fit[FIT_NAV]=4;
 else if(g->upgrades&4096)g->fit[FIT_NAV]=13;
 else if(g->upgrades&1024)g->fit[FIT_NAV]=5;
 if(g->upgrades&64)g->fit[FIT_HOLD]=11;
 else if(g->upgrades&512)g->fit[FIT_HOLD]=22;
 else if(g->upgrades&8)g->fit[FIT_HOLD]=10;
 if(g->upgrades&8192)g->fit[FIT_FUEL]=15;
 else if(g->upgrades&32)g->fit[FIT_FUEL]=14;
 if(g->upgrades&2048)g->fit[FIT_UTIL]=9;
 else if(g->upgrades&4)g->fit[FIT_UTIL]=8;
 else if(g->upgrades&32768)g->fit[FIT_UTIL]=19;
 else if(g->upgrades&131072)g->fit[FIT_UTIL]=21;
 else if(g->upgrades&16384)g->fit[FIT_UTIL]=18;
 else if(g->upgrades&262144)g->fit[FIT_UTIL]=24;
 else if((g->upgrades&256)&&g->fit[FIT_DEF]!=16)g->fit[FIT_UTIL]=17;
 fit_rebuild(g);
}

static float shield_regen_rate(const Game *g){
 if(g->upgrades&128)return 4.5f;
 if(g->upgrades&2)return 3.0f;
 return 1.5f;
}

static float laser_shot_damage(const Game *g){
 int w=g->fit[FIT_WPN];
 if(w==2)return 36.f;      /* beam */
 if(w==1)return 24.f;      /* pulse */
 if(w==20)return 18.f;     /* mining laser — weak vs ships */
 if(g->laser)return 36.f;  /* legacy */
 return 18.f;              /* stock guns */
}

static float weapon_output_multiplier(const Game *g){
 int p=g->pip_wep;
 if(p<0)p=0;
 if(p>4)p=4;
 return 0.50f+0.25f*p;
}

static float mine_shot_damage(const Game *g){
 float d=laser_shot_damage(g);
 if(g->upgrades&65536)d*=1.5f;
 return d;
}

static float fuel_scoop_rate(const Game *g){
 if(!(g->upgrades&32))return 0.f;
 return (g->upgrades&8192)?0.75f:0.5f;
}


