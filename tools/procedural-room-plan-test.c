#include "../src/procedural-room-plan.h"
int main(void){
 ProcRoomIdentity id={0,1,39,PROC_FREIGHT,0,2,1,0,1,1,0x12345678u};
 ProcRoomPlan a,b;
 if(!proc_room_plan_make(&id,0,&a)||!proc_room_plan_make(&id,0,&b))return 1;
 if(a.selector_hash!=b.selector_hash||a.count!=b.count||a.element[0].x!=b.element[0].x)return 2;
 if(proc_room_plan_make(&id,1,&b)!=1||b.contrast!=1)return 3;
 id.selector_version=2;if(proc_room_plan_make(&id,0,&b)!=0)return 4;
 return 0;
}
