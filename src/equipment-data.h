/* Compile-time equipment content pack. Catalog IDs are stable for V13 saves. */
enum { EQUIP_COUNT = 24 };
typedef struct { const char *name,*short_name,*details,*effects; int cost,tech; unsigned economy; } EquipmentDef;
static const EquipmentDef equipment_data[EQUIP_COUNT]={
 {"REFUEL TANK","REFUEL","Fill hyperspace tank.","Tank: now -> ship max",0,0,0xff},
 {"PULSE LASER","PULSE LASER","Solid starter pulse.","Laser 18 -> 24",2200,2,0xff},
 {"BEAM LASER","BEAM LASER","Twice laser damage.","Laser 18 -> 36",4000,4,0x0f},
 {"MISSILE RESTOCK","MISSILE +1","Load one missile.","Missiles +1",1000,2,0xff},
 {"DOCKING COMPUTER","DOCK COMP","Dock from 8,000 m.","Dock 2500 -> 8000 m",2500,5,0xff},
 {"NAV BEACON","NAV BEACON","Clearer next-hop marks.","Next-hop mark+",1800,3,0xf0},
 {"SHIELD BOOSTER","SHIELD BOOST","Twice shield recharge.","Shield 1.5 -> 3.0 /s",6000,6,0x1f},
 {"MILITARY SHIELD","MIL SHIELD","Even faster shields.","Shield 3.0 -> 4.5 /s",9000,8,0x07},
 {"LASER COOLING","LASER COOL","Laser cools faster.","Cool 22 -> 38 /s",4500,5,0x0f},
 {"HEAT SINK","HEAT SINK","Dump heat in a hurry.","Dump when lasers overheat",3200,4,0x1f},
 {"CARGO BAY +8T","CARGO +8T","Adds eight tonnes.","Hold +8 tonnes",3500,3,0xff},
 {"FREIGHT RACK +16T","FREIGHT +16T","Adds sixteen tonnes.","Hold +16 tonnes",7000,6,0x0e},
 {"LONG-RANGE SCANNER","LONG SCAN","IDs distant contacts.","IDs beyond 2500 m",3000,4,0xff},
 {"PLANET SCANNER","PLANET SCAN","Surface scan assist.","Survey range +",2800,3,0xf0},
 {"FUEL SCOOP","FUEL SCOOP","Skim fuel near a sun.","Fuel +0.5 /s at sun",7500,7,0x0f},
 {"AGRI SCOOP","AGRI SCOOP","Scoop near agri belts.","Fuel +0.75 /s at sun",5000,4,0xf0},
 {"ECM SUITE","ECM SUITE","Break missile locks.","50% break missile lock",5500,6,0x0f},
 {"CHAFF DISPENSER","CHAFF","Decoy flare burst.","Break missile lock",2000,3,0xff},
 {"ESCAPE POD","ESCAPE POD","One free emergency tow.","Consume on ship loss",4000,4,0xff},
 {"AUTO-REPAIR KIT","AUTO-REPAIR","Slow hull patching.","Hull +2 /s",3600,5,0x1f},
 {"MINING LASER","MINING LASER","Faster rock mining.","Mine rocks 1.5x",4200,4,0x0e},
 {"REFINERY UNIT","REFINERY","Ore->alloys onboard.","Minerals -> alloys",4800,5,0x0e},
 {"PASSENGER CABIN","PAX CABIN","+1 passenger berth.","Taxi berth required",2500,3,0xff},
 {"EXCLUSIVE CLAMP","EXCL CLAMP","Chandler deck clamp.","Cargo clamp+",1500,2,0x00}
};
/* Temporary compatibility view for the existing input regression expression. */
static const int equipment_costs[EQUIP_COUNT]={0,2200,4000,1000,2500,1800,6000,9000,4500,3200,3500,7000,3000,2800,7500,5000,5500,2000,4000,3600,4200,4800,2500,1500};
static int equipment_data_valid(void){
 for(int i=0;i<EQUIP_COUNT;i++)if(!equipment_data[i].name||!equipment_data[i].short_name||equipment_data[i].cost<0||equipment_data[i].tech<0||equipment_data[i].tech>12)return 0;
 return 1;
}
