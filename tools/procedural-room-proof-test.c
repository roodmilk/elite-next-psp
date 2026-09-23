#include "../src/procedural-room-plan.h"

/* Bounded proof for the first three native room targets.
 * This validates descriptor -> selector -> plan parity only; it does not
 * authorize runtime art activation or claim that the preview sheets are final. */
static int proof_one(int system_id, uint8_t expected_family){
 const ProcRoomDescriptor *d=proc_room_descriptor_at(system_id,0,PROC_ROOM_CANTEEN);
 ProcRoomIdentity id={0};
 ProcRoomPlan a,b,c;
 uint8_t i;
 if(!d||d->family_id!=expected_family)return 1;
 id.galaxy=0;id.system_id=(uint16_t)system_id;id.hub_index=0;
 id.room_id=PROC_ROOM_CANTEEN;id.family_id=d->family_id;
 id.arrangement_id=d->arrangement_id;id.landmark_id=d->landmark_id;
 id.material_id=d->material_id;id.art_version=d->art_version;
 id.exception_id=d->exception_id;id.selector_version=PROC_ROOM_SELECTOR_VERSION;
 id.selector_hash=d->selector_hash;
 if(!proc_room_plan_make(&id,0,&a)||!proc_room_plan_make(&id,0,&b))return 2;
 if(!proc_room_plan_make(&id,1,&c)||a.selector_hash!=d->selector_hash||
    a.selector_hash!=b.selector_hash||a.count!=b.count||a.count!=c.count||
    a.count<3||a.count>PROC_ROOM_PLAN_MAX_ELEMENTS)return 3;
 for(i=0;i<a.count;i++){
  if(a.element[i].element_id==PROC_E_DICE)return 4;
  if(a.element[i].x<0||a.element[i].x>=PROC_ROOM_W||
     a.element[i].y<0||a.element[i].y>=PROC_ROOM_H)return 5;
  if(a.element[i].element_id!=b.element[i].element_id||
     a.element[i].x!=b.element[i].x||a.element[i].y!=b.element[i].y)return 6;
  if(!(c.element[i].flags&PROC_PLACE_CONTRAST))return 7;
 }
 if(system_id==4&&a.element[0].element_id!=PROC_E_CONSOLE)return 8;
 return 0;
}

static int proof_qube_cargo(void){
 const ProcRoomDescriptor *d=proc_room_descriptor_at(1,0,PROC_ROOM_CARGO);
 ProcRoomIdentity id={0};
 ProcRoomPlan a,b;
 if(!d||d->family_id!=PROC_FREIGHT)return 10;
 id.galaxy=0;id.system_id=1;id.hub_index=0;id.room_id=PROC_ROOM_CARGO;
 id.family_id=d->family_id;id.arrangement_id=d->arrangement_id;
 id.landmark_id=d->landmark_id;id.material_id=d->material_id;
 id.art_version=d->art_version;id.exception_id=d->exception_id;
 id.selector_version=PROC_ROOM_SELECTOR_VERSION;id.selector_hash=d->selector_hash;
 if(!proc_room_plan_make(&id,0,&a)||!proc_room_plan_make(&id,1,&b))return 11;
 if(a.selector_hash!=d->selector_hash||a.count!=3||b.count!=a.count)return 12;
 if(a.element[0].element_id!=PROC_E_RACK||
    a.element[1].element_id!=PROC_E_PANEL||
    a.element[2].element_id!=PROC_E_COUNTER)return 13;
 if(a.element[0].x!=62||a.element[0].y!=45||
    a.element[1].x!=166||a.element[1].y!=36||
    a.element[2].x!=118||a.element[2].y!=112)return 14;
 if(!(b.element[0].flags&PROC_PLACE_CONTRAST)||
    !(b.element[1].flags&PROC_PLACE_CONTRAST)||
    !(b.element[2].flags&PROC_PLACE_CONTRAST))return 15;
 return 0;
}

int main(void){
 /* Qube Freight, Xequerin Research, Reorte Frontier — all H0 CANTEEN. */
 if(proof_one(1,PROC_FREIGHT))return 1;
 if(proof_one(4,PROC_RESEARCH))return 2;
 if(proof_one(39,PROC_FRONTIER))return 3;
 /* Next bounded proof room: Qube H0 CARGO / Freight. */
 if(proof_qube_cargo())return 4;
 return 0;
}
