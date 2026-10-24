enum { DASHBOARD_FIRST=119, DASHBOARD_LAST=134 };
static int dashboard_item(int i){return i>=DASHBOARD_FIRST&&i<=DASHBOARD_LAST;}
static int cosmetic_family(int i){return i>=115&&i<=118?1:dashboard_item(i)?2:0;}
static int cosmetic_item(int i){return cosmetic_family(i)!=0;}
static const char*cosmetic_label(int i){return dashboard_item(i)?"DASHBOARD COSMETIC MODULE":"COSMETIC MODULE";}
enum { CHARM_UFO=115, CHARM_ALIEN, CHARM_BURGER, CHARM_DUCK };
static int charm_item(int i){return i>=CHARM_UFO&&i<=CHARM_DUCK;}
/* Six categories, eight banks. The first 24 indices retain V24 compatibility. */
enum { FIT_SLOTS = 48, FIT_EMPTY = 0xff };
enum { FIT_WPN = 0, FIT_DEF = 1, FIT_NAV = 2, FIT_HOLD = 3, FIT_FUEL = 4, FIT_UTIL = 5 };

enum {
 TECH_TWIN_PULSE=70,TECH_BEAM_REGULATOR,TECH_ARMOUR_CAPACITOR,TECH_MINING_RESONATOR,TECH_POINT_DEFENCE,
 TECH_ABLATIVE_PLATING,TECH_KINETIC_SCREEN,TECH_OVERCHARGED_GRID,TECH_FAST_SHIELD_RELAY,TECH_THERMAL_BARRIER,TECH_EMERGENCY_BULKHEADS,
 TECH_VECTORING_JETS,TECH_PRECISION_JETS,TECH_INERTIAL_STABILISER,TECH_FLIGHT_ASSIST_BYPASS,TECH_DRIFT_CONTROLLER,
 TECH_CHARGE_ACCELERATOR,TECH_MIL_CHARGE_ACCELERATOR,TECH_JUMP_STABILISER,TECH_MASS_LOCK_COMPENSATOR,TECH_APPROACH_GOVERNOR,
 TECH_MISSION_LOCKER,TECH_SECURE_VAULT,TECH_REFRIGERATED_HOLD,TECH_ORE_BINS,TECH_LUXURY_SUITE,TECH_HEAVY_TRACTOR,
 TECH_CRUISE_THRUSTERS,TECH_COMBAT_THRUSTERS,TECH_BOOST_INJECTORS,TECH_EFFICIENT_BOOSTER,TECH_EMERGENCY_BOOSTER,
 TECH_DEEP_SPACE_DRIVE,TECH_EFFICIENT_HYPERDRIVE,TECH_MILITARY_HYPERDRIVE,
 TECH_THERMAL_SHUNT,TECH_ADVANCED_HEAT_BUFFER,TECH_REPAIR_GANTRY,TECH_SALVAGE_DRONES,TECH_SIGNAL_DECRYPTER,
 TECH_SIGNATURE_BAFFLER,TECH_ENERGY_ROUTER,TECH_SURVEY_DATA_CORE,TECH_ERL_VECTOR_GOVERNOR,TECH_ERL_COOLANT_MATRIX
};
typedef struct {const char *name;uint8_t category,tech;uint16_t price_u;float primary,secondary;} TechModule;
static const TechModule tech_catalogue[45]={
 {"Twin Pulse Coupler",FIT_WPN,7,1450,1.15f,1.12f},{"Sustained Beam Regulator",FIT_WPN,8,1600,.82f,.90f},
 {"Armour-Piercing Capacitor",FIT_WPN,9,1900,.20f,1.15f},{"Mining Resonator",FIT_WPN,1,1250,1.45f,.70f},
 {"Point-Defence Turret",FIT_WPN,8,1750,1,0},{"Ablative Hull Plating",FIT_DEF,1,1850,.72f,.92f},
 {"Kinetic Screen",FIT_DEF,6,1450,.55f,.90f},{"Overcharged Shield Grid",FIT_DEF,10,2400,1.35f,1.25f},
 {"Fast-Cycle Shield Relay",FIT_DEF,8,1700,2,.85f},{"Thermal Barrier",FIT_DEF,7,1550,.60f,0},
 {"Emergency Bulkheads",FIT_DEF,11,2750,.10f,.96f},{"Vectoring Jets",FIT_NAV,6,1250,1.18f,1.08f},
 {"Precision Manoeuvring Jets",FIT_NAV,6,1350,1.30f,.92f},{"Inertial Stabiliser",FIT_NAV,1,1100,.75f,.80f},
 {"Flight-Assist Bypass",FIT_NAV,7,1500,.72f,.70f},{"Drift Vector Controller",FIT_NAV,10,2250,.60f,.85f},
 {"Hyperdrive Charge Accelerator",FIT_NAV,9,2100,.75f,1.20f},{"Military Charge Accelerator",FIT_NAV,12,3400,.58f,1.35f},
 {"Jump Stabiliser",FIT_NAV,8,1700,1.50f,1.08f},{"Mass-Lock Compensator",FIT_NAV,11,2500,.55f,0},
 {"Approach Governor",FIT_NAV,1,1150,.96f,0},{"Mission Cargo Locker",FIT_HOLD,1,1000,8,0},
 {"Secure Cargo Vault",FIT_HOLD,8,1900,12,0},{"Refrigerated Hold",FIT_HOLD,1,1400,12,.65f},
 {"Ore Bins",FIT_HOLD,1,1650,20,0},{"Luxury Passenger Suite",FIT_HOLD,1,2600,2,0},
 {"Heavy Tractor Array",FIT_HOLD,8,1750,1.65f,.70f},{"Cruise Thrusters",FIT_FUEL,6,1500,1.12f,1.10f},
 {"Combat Thrusters",FIT_FUEL,8,1850,1.22f,1.18f},{"Boost Injectors",FIT_FUEL,8,2100,1.30f,1.12f},
 {"Efficient Booster",FIT_FUEL,7,1700,.68f,.82f},{"Emergency Booster",FIT_FUEL,7,1550,1.45f,0},
 {"Deep-Space Drive",FIT_FUEL,10,2650,1.22f,1.15f},{"Efficient Hyperdrive",FIT_FUEL,9,2000,.78f,1.18f},
 {"Military Hyperdrive",FIT_FUEL,12,3600,1.15f,.72f},{"Thermal Cruise Shunt",FIT_UTIL,7,1450,.65f,1.08f},
 {"Advanced Heat Buffer",FIT_UTIL,11,2600,.18f,0},{"Field Repair Gantry",FIT_UTIL,1,2250,2.5f,.96f},
 {"Salvage Drone Rack",FIT_UTIL,1,2000,1.30f,1.25f},{"Signal Decrypter",FIT_UTIL,8,1650,1.50f,.65f},
 {"Signature Baffler",FIT_UTIL,10,2200,.80f,.85f},{"Emergency Energy Router",FIT_UTIL,10,2450,15,45},
 {"Survey Data Core",FIT_UTIL,1,1750,1.20f,0},
 {"ERL Vector Governor",FIT_NAV,1,0,1.12f,1.08f},{"ERL Coolant Matrix",FIT_UTIL,1,0,.85f,2}
};
static const TechModule *tech_module(int item){return item>=TECH_TWIN_PULSE&&item<CHARM_UFO?&tech_catalogue[item-TECH_TWIN_PULSE]:0;}
static int fit_is_active_weapon(int item){return item==1||item==2||item==20||item==25||item==69||(item>=26&&item<=31)||(item>=57&&item<=68);}

static int equip_slot_for(int i){
 if(dashboard_item(i))return FIT_HOLD;
 if(charm_item(i))return FIT_UTIL;
 if(i>=TECH_TWIN_PULSE&&i<CHARM_UFO)return tech_catalogue[i-TECH_TWIN_PULSE].category;
 if(i==69)return FIT_WPN; /* MiniBolt MiningGun Lite */
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
 static const uint8_t caps[23][6]={
 {1,1,1,1,1,1},{2,1,1,1,1,2},{2,2,2,1,2,2},{2,2,2,2,2,2},
 {3,2,3,2,2,3},{3,3,3,4,2,4},{4,4,4,4,2,4},
 {3,3,3,2,2,3},{4,3,3,3,2,3},{4,3,4,4,2,4},
 {4,5,7,4,6,7},{3,6,3,8,7,6},{6,6,6,4,5,7},{5,7,3,8,6,8},
 {8,8,3,6,5,6},{3,6,6,8,8,8},{4,4,8,5,7,5},{7,8,4,5,6,8},
 {5,8,5,5,8,7},{6,4,8,3,6,8},{5,6,5,6,6,8},{8,8,2,5,5,5},{4,2,4,1,2,3}};
 return ship>=0&&ship<23&&category>=0&&category<6?caps[ship][category]:1;
}
static int fit_slot_open(const Game *g,int slot){return slot>=0&&slot<FIT_SLOTS&&slot/6<fit_capacity(g->ship,slot%6);}
static int fit_find(const Game *g,int item){for(int s=0;s<FIT_SLOTS;s++)if(g->fit[s]==item)return s;return -1;}
static int charm_installed(const Game*g){for(int s=0;s<FIT_SLOTS;s++)if(charm_item(g->fit[s]))return s;return -1;}
static int cosmetic_installed(const Game*g,int item){int family=cosmetic_family(item);for(int s=0;s<FIT_SLOTS;s++)if(family&&cosmetic_family(g->fit[s])==family)return s;return -1;}
#include "equipment-instances.h"
/* Preserve even legacy/debug spare counters without extending the V34 stream. */
static int charm_store_normalise(Game*g){int need=0,free=0;for(int b=CHARM_UFO;b<EQUIPMENT_COUNT;b++)need+=g->module_store[b];for(int i=0;i<DB_ITEMS;i++)free+=!g->dockbay.owned[i].id;if(need>free)return 0;
 for(int b=CHARM_UFO;b<EQUIPMENT_COUNT;b++)while(g->module_store[b]){int i=db_owned_free(g);g->dockbay.owned[i]=(DockRoll){++g->dockbay.serial,b,DB_NORMAL,1,0,1000,1000,0};g->module_store[b]--;}return 1;}

static int fit_empty_slot(const Game *g,int cat){for(int b=0;b<fit_capacity(g->ship,cat);b++)if(g->fit[cat+b*6]==FIT_EMPTY)return cat+b*6;return -1;}
static int module_hold_bonus(int i){return i==44?32:i==43?24:i==11?16:i==10||i==23?8:i==45?4:0;}
static int fit_hold_bonus(const Game *g){
 int bonus=0;for(int s=FIT_HOLD;s<FIT_SLOTS;s+=6){int i=g->fit[s];bonus+=(int)floorf(module_hold_bonus(i)*db_slot_power(g,s,0)+.5f);}
 return bonus;
}
static int fit_weapon_item(const Game *g){int item=g->active_weapon<FIT_SLOTS&&g->active_weapon%6==FIT_WPN?g->fit[g->active_weapon]:FIT_EMPTY;return fit_is_active_weapon(item)?item:FIT_EMPTY;}
static void fit_clear_all(Game *g){memset(g->dockbay.fitted,0,sizeof(g->dockbay.fitted));g->active_weapon=0;for(int s=0;s<FIT_SLOTS;s++)g->fit[s]=(uint8_t)FIT_EMPTY;}

/* Rebuild upgrades + laser from fitted slots so bits never drift. */
static void fit_rebuild(Game *g){
 int bits=0,has_laser=0;unsigned char seen[EQUIPMENT_COUNT]={0};
 for(int s=0;s<FIT_SLOTS;s++){
  int i=g->fit[s];
  if(i==FIT_EMPTY)continue;
  if(!fit_value_valid(s,i)||!fit_slot_open(g,s)||(seen[i]&&(!db_slot_roll(g,s)||!db_slot_roll(g,seen[i]-1)||g->dockbay.fitted[s]==g->dockbay.fitted[seen[i]-1]))){g->fit[s]=(uint8_t)FIT_EMPTY;g->dockbay.fitted[s]=0;continue;}
  seen[i]=s+1;
  if(i!=20&&i!=69)bits|=equip_mask_for(i);
  if(fit_is_active_weapon(i))has_laser=1;
 }
 if(fit_weapon_item(g)==FIT_EMPTY||!fit_slot_open(g,g->active_weapon)){
  g->active_weapon=0;for(int s=0;s<FIT_SLOTS;s+=6)if(fit_is_active_weapon(g->fit[s])){g->active_weapon=s;break;}
 }
 if(fit_weapon_item(g)==20||fit_weapon_item(g)==69)bits|=65536;
 g->upgrades=bits;
 g->laser=has_laser?1:0;
}

/* Primary weapon cycling is shared by flight controls and the Ship Tech
 * Board's active_weapon state. Empty banks are skipped, so Circle+Right can
 * always advance to the next weapon that can actually fire. */
static int fit_weapon_count(const Game *g){
 int count=0;for(int slot=FIT_WPN;slot<FIT_SLOTS;slot+=6)if(fit_slot_open(g,slot)&&fit_is_active_weapon(g->fit[slot]))count++;
 return count;
}
static int fit_cycle_weapon(Game *g){
 int capacity=fit_capacity(g->ship,FIT_WPN),count=fit_weapon_count(g);
 if(count<2)return count==1?-2:-1;
 int bank=g->active_weapon<FIT_SLOTS&&g->active_weapon%6==FIT_WPN?g->active_weapon/6:-1;
 for(int step=1;step<=capacity;step++){
  int slot=((bank+step+capacity)%capacity)*6;
  if(fit_is_active_weapon(g->fit[slot])){g->active_weapon=(uint8_t)slot;fit_rebuild(g);return slot;}
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
  case 67:return "LIME SHARD";case 68:return "BLACKSTAR";case 69:return "MINIBOLT LITE";default:return "NO WEAPON";
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
 if(fit_find(g,6)>=0)rate+=1.5f*(db_item_power(g,6,0)-1.f);
 return rate+(fit_find(g,33)>=0?1.f:0.f)+(fit_find(g,TECH_FAST_SHIELD_RELAY)>=0?2.f:0.f);
}

static float laser_item_damage(int w){
 if(w==69)return 2.f; /* A starter rock cutter barely scratches ship shields. */
 if(w>=26&&w<=31){static const float damage[]={14,90,48,12,70,22};return damage[w-26];}
 if(w>=57&&w<=68){static const float damage[]={42,20,58,30,34,52,26,110,46,38,32,76};return damage[w-57];}
 if(w==25)return 60.f;     /* heavy laser: slower, hotter */
 if(w==2)return 36.f;      /* beam */
 if(w==1)return 24.f;      /* pulse */
 if(w==20)return 18.f;     /* mining laser — weak vs ships */
 if(w==FIT_EMPTY)return 18.f;
 return 18.f;              /* stock guns */
}

static float laser_shot_damage(const Game *g){int w=fit_weapon_item(g);return (w==FIT_EMPTY&&g->laser?36.f:laser_item_damage(w))*db_slot_power(g,g->active_weapon,0);}
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
static float weapon_item_cycle(int w){float t=w==25?.36f:.18f;
 if(w==69)t=.34f;
 if(w>=26&&w<=31){static const float times[]={.08f,.70f,.45f,.25f,.50f,.35f};t=times[w-26];}
 if(w>=57&&w<=68){static const float times[]={.24f,.11f,.42f,.20f,.16f,.38f,.28f,.82f,.32f,.22f,.14f,.58f};t=times[w-57];}
 return t;
}
static float weapon_cycle(const Game *g){return weapon_item_cycle(fit_weapon_item(g))*(fit_find(g,52)>=0?.85f:1.f)/db_slot_power(g,g->active_weapon,1);
}
static float weapon_heat(const Game *g){
 int w=fit_weapon_item(g);float h=w==25?22.f:12.f;
 if(w==69)h=5.f;
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
static float module_scan_range(const Game *g){return fit_find(g,38)>=0?7500.f*db_item_power(g,38,0):(g->upgrades&16)?5000.f:2500.f;}
static float module_survey_range(const Game *g){return fit_find(g,39)>=0?760.f:(g->upgrades&4096)?560.f:380.f;}
static float module_repair_rate(const Game *g){return fit_find(g,54)>=0?3.5f:(g->upgrades&32768)?2.f:0.f;}
static float module_cooling(const Game *g){return ((g->upgrades&4)?38.f:22.f)+(fit_find(g,55)>=0?8.f:0.f);}

/* Central combination hooks for the expanded catalogue. Callers pass the bare
 * hull value, so shop previews and runtime code can share the same result. */
static float tech_clampf(float v,float lo,float hi){return v<lo?lo:v>hi?hi:v;}
static float tech_speed_multiplier(const Game *g){
 float m=1.f;if(fit_find(g,TECH_CRUISE_THRUSTERS)>=0)m*=1.12f;if(fit_find(g,TECH_COMBAT_THRUSTERS)>=0)m*=1.05f;
 if(fit_find(g,TECH_APPROACH_GOVERNOR)>=0)m*=.96f;if(fit_find(g,TECH_EMERGENCY_BULKHEADS)>=0)m*=.96f;if(fit_find(g,TECH_REPAIR_GANTRY)>=0)m*=.96f;
 return tech_clampf(m,.50f,1.35f);
}
static float tech_accel_multiplier(const Game *g){
 float m=1.f;if(fit_find(g,TECH_ABLATIVE_PLATING)>=0)m*=.92f;if(fit_find(g,TECH_COMBAT_THRUSTERS)>=0)m*=1.22f;
 if(fit_find(g,TECH_DEEP_SPACE_DRIVE)>=0)m*=.90f;if(fit_find(g,TECH_EMERGENCY_BOOSTER)>=0&&g->hull<30)m*=1.45f;
 return tech_clampf(m,.50f,1.60f);
}
static float tech_brake_multiplier(const Game *g){
 float m=fit_find(g,TECH_COMBAT_THRUSTERS)>=0?1.18f:1.f;
 if(fit_find(g,TECH_FLIGHT_ASSIST_BYPASS)>=0)m*=.70f;if(fit_find(g,TECH_DRIFT_CONTROLLER)>=0)m*=.85f;
 return tech_clampf(m,.45f,1.60f);
}
static float tech_turn_multiplier(const Game *g,float speed_fraction){
 float m=fit_find(g,TECH_VECTORING_JETS)>=0?1.18f:1.f;
 if(fit_find(g,TECH_PRECISION_JETS)>=0)m*=speed_fraction<.35f?1.30f:.92f;
 if(fit_find(g,TECH_ERL_VECTOR_GOVERNOR)>=0)m*=1.12f;return tech_clampf(m,.55f,1.60f);
}
static float tech_drift_retention(const Game *g){
 float d=0;if(fit_find(g,TECH_FLIGHT_ASSIST_BYPASS)>=0)d=.72f;if(fit_find(g,TECH_DRIFT_CONTROLLER)>=0&&d<.60f)d=.60f;
 if(fit_find(g,TECH_INERTIAL_STABILISER)>=0)d*=.80f;return tech_clampf(d,0,.72f);
}
static float tech_jump_range_multiplier(const Game *g){float m=1.f;if(fit_find(g,TECH_DEEP_SPACE_DRIVE)>=0)m*=1.22f;if(fit_find(g,TECH_MILITARY_HYPERDRIVE)>=0)m*=1.15f;return tech_clampf(m,1.f,1.40f);}
static float tech_charge_multiplier(const Game *g){
 float m=1.f;if(fit_find(g,TECH_CHARGE_ACCELERATOR)>=0)m*=.75f;if(fit_find(g,TECH_MIL_CHARGE_ACCELERATOR)>=0)m*=.58f;
 if(fit_find(g,TECH_JUMP_STABILISER)>=0)m*=1.08f;if(fit_find(g,TECH_EFFICIENT_HYPERDRIVE)>=0)m*=1.18f;if(fit_find(g,TECH_MILITARY_HYPERDRIVE)>=0)m*=.72f;
 return tech_clampf(m,.45f,2.f);
}
static float tech_weapon_damage_multiplier(const Game *g){float m=1.f;if(fit_find(g,TECH_TWIN_PULSE)>=0)m*=1.15f;if(fit_find(g,TECH_BEAM_REGULATOR)>=0)m*=.90f;if(fit_find(g,TECH_MINING_RESONATOR)>=0)m*=.70f;return m;}
static float tech_weapon_heat_multiplier(const Game *g){float m=1.f;if(fit_find(g,TECH_TWIN_PULSE)>=0)m*=1.12f;if(fit_find(g,TECH_THERMAL_SHUNT)>=0)m*=1.08f;return m;}
static float tech_weapon_cycle_multiplier(const Game *g){float m=1.f;if(fit_find(g,TECH_BEAM_REGULATOR)>=0)m*=.82f;if(fit_find(g,TECH_ARMOUR_CAPACITOR)>=0)m*=1.15f;return tech_clampf(m,.45f,2.f);}
static int tech_general_hold_bonus(const Game *g){int n=0;if(fit_find(g,TECH_SECURE_VAULT)>=0)n+=12;if(fit_find(g,TECH_REFRIGERATED_HOLD)>=0)n+=12;return n;}
static int tech_mission_hold_bonus(const Game *g){return fit_find(g,TECH_MISSION_LOCKER)>=0?8:0;}
static int tech_ore_hold_bonus(const Game *g){return fit_find(g,TECH_ORE_BINS)>=0?20:0;}
static int tech_passenger_berths(const Game *g){return fit_find(g,TECH_LUXURY_SUITE)>=0?2:0;}
static float tech_scan_multiplier(const Game *g){return fit_find(g,TECH_SIGNATURE_BAFFLER)>=0?.85f:1.f;}
static float tech_identification_multiplier(const Game *g){return fit_find(g,TECH_SIGNAL_DECRYPTER)>=0?1.50f:1.f;}
static float tech_repair_rate(const Game *g){return fit_find(g,TECH_REPAIR_GANTRY)>=0?2.5f:module_repair_rate(g);}
static float tech_shield_capacity(const Game *g){float m=fit_find(g,TECH_OVERCHARGED_GRID)>=0?1.35f:1.f;if(fit_find(g,TECH_FAST_SHIELD_RELAY)>=0)m*=.85f;return 100.f*m;}
static float tech_shield_regen_multiplier(const Game *g,int tractoring){float m=fit_find(g,TECH_KINETIC_SCREEN)>=0?.90f:1.f;if(tractoring&&fit_find(g,TECH_HEAVY_TRACTOR)>=0)m*=.90f;return m;}
static float tech_hull_damage_multiplier(const Game *g){float m=fit_find(g,32)>=0?.80f/db_item_power(g,32,0):1.f;if(fit_find(g,TECH_ABLATIVE_PLATING)>=0)m*=.72f;return m;}
static float tech_collision_damage_multiplier(const Game *g){float m=fit_find(g,36)>=0?.75f:1.f;if(fit_find(g,TECH_KINETIC_SCREEN)>=0)m*=.55f;return m;}
static float tech_thermal_damage_multiplier(const Game *g){float m=fit_find(g,35)>=0?.70f:1.f;if(fit_find(g,TECH_THERMAL_BARRIER)>=0)m*=.60f;return m;}
static float tech_tractor_range(const Game *g){float m=1.f;if(fit_find(g,TECH_HEAVY_TRACTOR)>=0)m*=1.65f;if(fit_find(g,TECH_SALVAGE_DRONES)>=0)m*=1.25f;return 500.f*m;}
static float tech_tractor_time(const Game *g){return fit_find(g,TECH_HEAVY_TRACTOR)>=0?.75f*.70f:.75f;}
static float tech_salvage_cash_multiplier(const Game *g){float m=fit_find(g,42)>=0?1.20f:1.f;if(fit_find(g,TECH_SALVAGE_DRONES)>=0)m*=1.30f;return m;}
static float tech_discovery_pay_multiplier(const Game *g){return fit_find(g,TECH_SURVEY_DATA_CORE)>=0?1.20f:1.f;}
static float tech_fuel_capacity(const Game *g){return player_ships[g->ship].range*(fit_find(g,TECH_DEEP_SPACE_DRIVE)>=0?1.15f:1.f);}
static float tech_jump_fuel_multiplier(const Game *g){float m=1.f;if(fit_find(g,TECH_EFFICIENT_HYPERDRIVE)>=0)m*=.80f/db_item_power(g,TECH_EFFICIENT_HYPERDRIVE,0);if(fit_find(g,TECH_MILITARY_HYPERDRIVE)>=0)m*=1.40f;return m;}



