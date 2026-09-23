/* Deterministic, read-only station-room composition plan.
 * Composer lane only: no Game, save, RNG, audio or input dependencies. */
#ifndef ELITE_PROCEDURAL_ROOM_PLAN_H
#define ELITE_PROCEDURAL_ROOM_PLAN_H

#include <stdint.h>

#define PROC_ROOM_PLAN_VERSION 1u
#define PROC_ROOM_PLAN_MAX_ELEMENTS 12u
#define PROC_ROOM_SCREEN_W 480
#define PROC_ROOM_SCREEN_H 272
#define PROC_ROOM_X 6
#define PROC_ROOM_Y 20
#define PROC_ROOM_W 340
#define PROC_ROOM_H 168
#define PROC_ROOM_OPTIONS_X 354
#define PROC_ROOM_OPTIONS_Y 20
#define PROC_ROOM_OPTIONS_W 120
#define PROC_ROOM_OPTIONS_H 168
#define PROC_ROOM_FEEDBACK_Y 190
#define PROC_ROOM_SHIP_X 274
#define PROC_ROOM_SHIP_Y 158
#define PROC_ROOM_SHIP_W 68
#define PROC_ROOM_SHIP_H 26

enum {
 PROC_ROOM_ARRIVALS=0, PROC_ROOM_SHOP, PROC_ROOM_CANTEEN,
 PROC_ROOM_CARGO, PROC_ROOM_GUILD, PROC_ROOM_CLINIC, PROC_ROOM_CUSTOMS,
 PROC_ROOM_COUNT
};
enum {
 PROC_FREIGHT=0, PROC_PROSPECTOR, PROC_RESEARCH,
 PROC_MERCHANT, PROC_FRONTIER, PROC_OUTLAW, PROC_FAMILY_COUNT
};
enum {
 PROC_E_PERSON=0, PROC_E_COUNTER, PROC_E_LAMP, PROC_E_WINDOW,
 PROC_E_RACK, PROC_E_ASSAY, PROC_E_CONSOLE, PROC_E_PANEL,
 PROC_E_HATCH, PROC_E_DICE, PROC_E_COUNT
};
enum { PROC_LAYER_BACK=0, PROC_LAYER_MID, PROC_LAYER_FRONT, PROC_LAYER_UI };
enum { PROC_PLACE_AUTHORED=1u, PROC_PLACE_PROCEDURAL=2u, PROC_PLACE_CONTRAST=4u };

typedef struct {
 uint8_t galaxy,hub_index,room_id,family_id;
 uint16_t system_id;
 uint8_t arrangement_id,landmark_id,material_id;
 uint16_t selector_version,exception_id;
 uint32_t stable_seed;
} ProcRoomIdentity;

typedef struct {
 uint8_t element_id,variant,layer,flags;
 int16_t x,y;
} ProcRoomPlacement;

typedef struct {
 uint16_t version;
 uint8_t family_id,room_id,contrast,valid;
 uint32_t selector_hash;
 uint8_t count;
 ProcRoomPlacement element[PROC_ROOM_PLAN_MAX_ELEMENTS];
} ProcRoomPlan;

static uint32_t proc_room_mix(uint32_t h,uint32_t v){
 h^=v+0x9e3779b9u+(h<<6)+(h>>2);h*=16777619u;return h;
}
static uint32_t proc_room_hash(const ProcRoomIdentity *id){
 uint32_t h=2166136261u;
 h=proc_room_mix(h,id->galaxy);h=proc_room_mix(h,id->system_id);
 h=proc_room_mix(h,id->hub_index);h=proc_room_mix(h,id->room_id);
 h=proc_room_mix(h,id->family_id);h=proc_room_mix(h,id->arrangement_id);
 h=proc_room_mix(h,id->landmark_id);h=proc_room_mix(h,id->material_id);
 h=proc_room_mix(h,id->selector_version);h=proc_room_mix(h,id->exception_id);
 h=proc_room_mix(h,id->stable_seed);return h;
}
static int proc_room_push(ProcRoomPlan *p,uint8_t e,uint8_t v,uint8_t layer,
                           uint8_t flags,int x,int y){
 if(p->count>=PROC_ROOM_PLAN_MAX_ELEMENTS)return 0;
 p->element[p->count++]=(ProcRoomPlacement){e,v,layer,flags,(int16_t)x,(int16_t)y};return 1;
}
static uint8_t proc_room_family(uint8_t family){
 return family<PROC_FAMILY_COUNT?family:PROC_FREIGHT;
}

/* Returns 1 for a composed plan, 0 for unsupported identity/fallback. */
static int proc_room_plan_make(const ProcRoomIdentity *id,int high_contrast,
                               ProcRoomPlan *out){
 uint32_t h;if(!id||!out||id->selector_version!=PROC_ROOM_PLAN_VERSION||
    id->room_id>=PROC_ROOM_COUNT)return 0;
 out->version=PROC_ROOM_PLAN_VERSION;out->family_id=proc_room_family(id->family_id);
 out->room_id=id->room_id;out->contrast=(uint8_t)(high_contrast?1:0);
 out->selector_hash=proc_room_hash(id);out->count=0;out->valid=1;h=out->selector_hash;
 /* Structural silhouette, then focal, depth, light/material clusters. */
 if(id->room_id==PROC_ROOM_ARRIVALS){
  proc_room_push(out,PROC_E_WINDOW,(uint8_t)(h&1u),PROC_LAYER_BACK,PROC_PLACE_PROCEDURAL,92,34);
  proc_room_push(out,PROC_E_PANEL,id->landmark_id&3u,PROC_LAYER_MID,PROC_PLACE_AUTHORED,202,104);
  proc_room_push(out,PROC_E_HATCH,id->hub_index&1u,PROC_LAYER_FRONT,PROC_PLACE_AUTHORED,10,56);
 }else if(id->room_id==PROC_ROOM_SHOP){
  proc_room_push(out,PROC_E_COUNTER,id->arrangement_id%3u,PROC_LAYER_MID,PROC_PLACE_AUTHORED,92,112);
  proc_room_push(out,PROC_E_PANEL,id->landmark_id&3u,PROC_LAYER_BACK,PROC_PLACE_PROCEDURAL,122,35);
  proc_room_push(out,PROC_E_RACK,(h>>4)&3u,PROC_LAYER_FRONT,PROC_PLACE_PROCEDURAL,238,94);
 }else if(id->room_id==PROC_ROOM_CANTEEN){
  proc_room_push(out,PROC_E_COUNTER,id->arrangement_id%3u,PROC_LAYER_FRONT,PROC_PLACE_AUTHORED,64,100);
  proc_room_push(out,PROC_E_LAMP,(h>>5)&3u,PROC_LAYER_BACK,PROC_PLACE_PROCEDURAL,126,32);
  proc_room_push(out,PROC_E_WINDOW,(h>>7)&3u,PROC_LAYER_BACK,PROC_PLACE_PROCEDURAL,176,28);
  if(id->exception_id)proc_room_push(out,PROC_E_DICE,0,PROC_LAYER_MID,PROC_PLACE_AUTHORED,220,126);
 }else if(id->room_id==PROC_ROOM_CARGO){
  proc_room_push(out,PROC_E_RACK,id->arrangement_id%3u,PROC_LAYER_BACK,PROC_PLACE_AUTHORED,62,45);
  proc_room_push(out,PROC_E_PANEL,id->landmark_id&3u,PROC_LAYER_MID,PROC_PLACE_PROCEDURAL,166,36);
  proc_room_push(out,PROC_E_COUNTER,(h>>3)&3u,PROC_LAYER_FRONT,PROC_PLACE_PROCEDURAL,118,112);
 }else if(id->room_id==PROC_ROOM_GUILD){
  proc_room_push(out,PROC_E_CONSOLE,id->landmark_id&3u,PROC_LAYER_MID,PROC_PLACE_AUTHORED,130,58);
  proc_room_push(out,PROC_E_PANEL,id->arrangement_id&3u,PROC_LAYER_BACK,PROC_PLACE_PROCEDURAL,82,28);
  proc_room_push(out,PROC_E_LAMP,(h>>6)&3u,PROC_LAYER_FRONT,PROC_PLACE_PROCEDURAL,248,40);
 }else if(id->room_id==PROC_ROOM_CLINIC){
  proc_room_push(out,PROC_E_COUNTER,id->arrangement_id%3u,PROC_LAYER_MID,PROC_PLACE_AUTHORED,96,108);
  proc_room_push(out,PROC_E_LAMP,(h>>5)&3u,PROC_LAYER_BACK,PROC_PLACE_PROCEDURAL,162,28);
  proc_room_push(out,PROC_E_PANEL,id->landmark_id&3u,PROC_LAYER_FRONT,PROC_PLACE_PROCEDURAL,238,52);
 }else{
  proc_room_push(out,PROC_E_HATCH,id->arrangement_id&1u,PROC_LAYER_BACK,PROC_PLACE_AUTHORED,132,38);
  proc_room_push(out,PROC_E_PANEL,id->landmark_id&3u,PROC_LAYER_MID,PROC_PLACE_PROCEDURAL,196,32);
  proc_room_push(out,PROC_E_LAMP,(h>>5)&3u,PROC_LAYER_FRONT,PROC_PLACE_PROCEDURAL,92,40);
 }
 if(high_contrast)for(uint8_t i=0;i<out->count;i++)out->element[i].flags|=PROC_PLACE_CONTRAST;
 return 1;
}
#endif
