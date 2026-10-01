/* Six categories, four banks; first bank retains V13 save compatibility.
 * Passive bits combine; only the selected weapon contributes mining mode. */
enum { FIT_SLOTS = 24, FIT_EMPTY = 0xff };
enum { FIT_WPN = 0, FIT_DEF = 1, FIT_NAV = 2, FIT_HOLD = 3, FIT_FUEL = 4, FIT_UTIL = 5 };

static int equip_slot_for(int i){
 if(i>=26&&i<=31)return FIT_WPN;if(i>=57&&i<=68)return FIT_WPN;if(i>=32&&i<=37)return FIT_DEF;
 if(i>=38&&i<=42)return FIT_NAV;if(i>=43&&i<=45)return FIT_HOLD;
 if(i>=46&&i<=49)return FIT_FUEL;if(i>=50&&i<=56)return FIT_UTIL;
 if(i==1||i==2||i==20||i==25)return FIT_WPN;
 if(i==6||i==7||i==16)return FIT_DEF;
 if(i==4||i==5||i==12||i==13)return FIT_NAV;
 if(i==10||i==11||i==22||i==23)return FIT_HOLD;
 if(i==14||i==15)return FIT_FUEL;
 if(i==8||i==9||i==17||i==18||i==19||i==21||i==24)return FIT_UTIL;
 return -1; /* refuel / missiles are services, not slots */
}

static int fit_value_valid(int slot,int i){
 return slot>=0&&slot<FIT_SLOTS&&(i==FIT_EMPTY||(i>0&&i<EQUIPMENT_COUNT&&equip_slot_for(i)==slot%6));
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

/* WPN DEF NAV HOLD FUEL UTIL: deliberate price progression and hull roles. */
static int fit_capacity(int ship,int category){
 static const uint8_t caps[10][6]={
 {1,1,1,1,1,1},{2,1,1,1,1,2},{2,2,2,1,2,2},{2,2,2,2,2,2},
 {3,2,3,2,2,3},{3,3,3,4,2,4},{4,4,4,4,2,4},
 {3,3,3,2,2,3},{4,3,3,3,2,3},{4,3,4,4,2,4}};
 return ship>=0&&ship<10&&category>=0&&category<6?caps[ship][category]:1;
}
static int fit_slot_open(const Game *g,int slot){return slot>=0&&slot<FIT_SLOTS&&slot/6<fit_capacity(g->ship,slot%6);}
static int fit_find(const Game *g,int item){for(int s=0;s<FIT_SLOTS;s++)if(g->fit[s]==item)return s;return -1;}
static int fit_empty_slot(const Game *g,int cat){for(int b=0;b<fit_capacity(g->ship,cat);b++)if(g->fit[cat+b*6]==FIT_EMPTY)return cat+b*6;return -1;}
static int module_hold_bonus(int i){return i==44?32:i==43?24:i==11?16:i==10||i==23?8:i==45?4:0;}
static int fit_hold_bonus(const Game *g){
 int bonus=0;for(int s=FIT_HOLD;s<FIT_SLOTS;s+=6){int i=g->fit[s];bonus+=module_hold_bonus(i);}
 return bonus;
}
static int fit_weapon_item(const Game *g){return g->active_weapon<FIT_SLOTS&&g->active_weapon%6==FIT_WPN?g->fit[g->active_weapon]:FIT_EMPTY;}
static void fit_clear_all(Game *g){g->active_weapon=0;for(int s=0;s<FIT_SLOTS;s++)g->fit[s]=(uint8_t)FIT_EMPTY;}

/* Rebuild upgrades + laser from fitted slots so bits never drift. */
static void fit_rebuild(Game *g){
 int bits=0,has_laser=0;
 for(int s=0;s<FIT_SLOTS;s++){
  int i=g->fit[s];
  if(i==FIT_EMPTY||!fit_value_valid(s,i)||!fit_slot_open(g,s)){g->fit[s]=(uint8_t)FIT_EMPTY;continue;}
  if(i!=20)bits|=equip_mask_for(i);
  if(equip_slot_for(i)==FIT_WPN)has_laser=1;
 }
 if(fit_weapon_item(g)==FIT_EMPTY||!fit_slot_open(g,g->active_weapon)){
  g->active_weapon=0;for(int s=0;s<FIT_SLOTS;s+=6)if(g->fit[s]!=FIT_EMPTY){g->active_weapon=s;break;}
 }
 if(fit_weapon_item(g)==20)bits|=65536;
 g->upgrades=bits;
 g->laser=has_laser?1:0;
}

/* Primary weapon cycling is shared by flight controls and the Ship Tech
 * Board's active_weapon state. Empty banks are skipped, so Circle+Right can
 * always advance to the next weapon that can actually fire. */
static int fit_weapon_count(const Game *g){
 int count=0;for(int slot=FIT_WPN;slot<FIT_SLOTS;slot+=6)if(fit_slot_open(g,slot)&&g->fit[slot]!=FIT_EMPTY)count++;
 return count;
}
static int fit_cycle_weapon(Game *g){
 int capacity=fit_capacity(g->ship,FIT_WPN),count=fit_weapon_count(g);
 if(count<2)return count==1?-2:-1;
 int bank=g->active_weapon<FIT_SLOTS&&g->active_weapon%6==FIT_WPN?g->active_weapon/6:-1;
 for(int step=1;step<=capacity;step++){
  int slot=((bank+step+capacity)%capacity)*6;
  if(g->fit[slot]!=FIT_EMPTY){g->active_weapon=(uint8_t)slot;fit_rebuild(g);return slot;}
 }
 return -1;
}
static const char *fit_weapon_short_name(int item){
 switch(item){
  case 1:return "PULSE";case 2:return "BEAM";case 20:return "MINING";case 25:return "HEAVY";
  case 26:return "RAPID";case 27:return "LANCE";case 28:return "SCATTER";case 29:return "ION";
  case 30:return "PLASMA";case 31:return "DISRUPTOR";case 57:return "RED BEAM";case 58:return "GREEN PULSE";
  case 59:return "BLUE ION";case 60:return "VIOLET ARC";case 61:return "RAINBOW";case 62:return "AMBER";
  case 63:return "CYAN RIPPLE";case 64:return "WHITE RAIL";case 65:return "ORANGE";case 66:return "PINK PHASE";
  case 67:return "LIME SHARD";case 68:return "BLACKSTAR";default:return "NO WEAPON";
 }
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
 float rate=fit_find(g,34)>=0?6.f:(g->upgrades&128)?4.5f:(g->upgrades&2)?3.f:1.5f;
 return rate+(fit_find(g,33)>=0?1.f:0.f);
}

static float laser_shot_damage(const Game *g){
 int w=fit_weapon_item(g);
 if(w>=26&&w<=31){static const float damage[]={14,90,48,12,70,22};return damage[w-26];}
 if(w>=57&&w<=68){static const float damage[]={42,20,58,30,34,52,26,110,46,38,32,76};return damage[w-57];}
 if(w==25)return 60.f;     /* heavy laser: slower, hotter */
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
 return (0.50f+0.25f*p)*(1.f+(fit_find(g,50)>=0?.10f:0.f)+(fit_find(g,51)>=0?.20f:0.f));
}

static float mine_shot_damage(const Game *g){
 float d=laser_shot_damage(g);
 if(g->upgrades&65536)d=54.f; /* specialised cutter beats the beam against rocks */
 return d;
}

static float fuel_scoop_rate(const Game *g){
 if(fit_find(g,47)>=0)return 1.25f;
 if(fit_find(g,46)>=0)return 1.f;
 if(!(g->upgrades&32))return 0.f;
 return (g->upgrades&8192)?0.75f:0.5f;
}
static float weapon_cycle(const Game *g){
 int w=fit_weapon_item(g);float t=w==25?.36f:.18f;
 if(w>=26&&w<=31){static const float times[]={.08f,.70f,.45f,.25f,.50f,.35f};t=times[w-26];}
 if(w>=57&&w<=68){static const float times[]={.24f,.11f,.42f,.20f,.16f,.38f,.28f,.82f,.32f,.22f,.14f,.58f};t=times[w-57];}
 return t*(fit_find(g,52)>=0?.85f:1.f);
}
static float weapon_heat(const Game *g){
 int w=fit_weapon_item(g);float h=w==25?22.f:12.f;
 if(w>=26&&w<=31){static const float heat[]={5,28,18,9,26,14};h=heat[w-26];}
 if(w>=57&&w<=68){static const float heat[]={14,7,18,13,16,19,11,34,20,15,10,29};h=heat[w-57];}
 return h*(1.f+(fit_find(g,51)>=0?.15f:0.f)+(fit_find(g,52)>=0?.10f:0.f))*(fit_find(g,53)>=0?.80f:1.f);
}
static float weapon_range(const Game *g){
 int w=fit_weapon_item(g);float r=2200;
 if(w>=26&&w<=31){static const float ranges[]={1800,4200,900,2200,1600,2000};r=ranges[w-26];}
 if(w>=57&&w<=68){static const float ranges[]={2800,1600,3200,1900,2500,2600,2100,4800,1200,2400,1500,3000};r=ranges[w-57];}
 return r*(fit_find(g,40)>=0?1.2f:1.f);
}
static float weapon_cone(const Game *g){
 int w=fit_weapon_item(g);float cone=w==27?.012f:w==28?.11f:.035f;
 if(w>=57&&w<=68){static const float cones[]={.025f,.055f,.018f,.065f,.045f,.030f,.075f,.009f,.095f,.050f,.12f,.020f};cone=cones[w-57];}
 return cone*(fit_find(g,41)>=0?1.2f:1.f);
}
static float module_scan_range(const Game *g){return fit_find(g,38)>=0?7500.f:(g->upgrades&16)?5000.f:2500.f;}
static float module_survey_range(const Game *g){return fit_find(g,39)>=0?760.f:(g->upgrades&4096)?560.f:380.f;}
static float module_repair_rate(const Game *g){return fit_find(g,54)>=0?3.5f:(g->upgrades&32768)?2.f:0.f;}
static float module_cooling(const Game *g){return ((g->upgrades&4)?38.f:22.f)+(fit_find(g,55)>=0?8.f:0.f);}



