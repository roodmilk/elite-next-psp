#!/usr/bin/env node
/* Bake Systems' offline room descriptors into a compact read-only PSP table. */
import fs from 'node:fs';
import crypto from 'node:crypto';

const arg=(name,fallback)=>{const i=process.argv.indexOf(name);return i>=0?process.argv[i+1]:fallback;};
const input=arg('--in');
const output=arg('--out','src/generated/procedural-room-descriptors.h');
if(!input){console.error('Usage: node tools/bake-room-descriptors.mjs --in room-descriptors.json [--out header]');process.exit(2);}
const bytes=fs.readFileSync(input);const data=JSON.parse(bytes);const records=data.records||[];
if(data.counts?.systems!==256||data.counts?.hubs!==768||data.counts?.rooms!==5376||records.length!==5376)throw new Error('expected 256 systems, 768 hubs and 5376 rooms');
const seen=new Set();
for(const r of records){const key=`${r.system_id}/${r.hub_index}/${r.room_id}`;if(seen.has(key))throw new Error(`duplicate ${key}`);seen.add(key);}
const rows=records.map(r=>`  {${r.family_id},${r.arrangement_id},${r.landmark_id},${r.material_id},${r.exception_id},${r.art_version},0x${r.selector_fingerprint}u},`).join('\n');
const sourceSha=crypto.createHash('sha256').update(bytes).digest('hex');
const out=`/* Generated from Systems room-descriptors.json; do not hand-edit. */
#ifndef ELITE_PROCEDURAL_ROOM_DESCRIPTORS_H
#define ELITE_PROCEDURAL_ROOM_DESCRIPTORS_H
#include <stdint.h>
#define PROC_ROOM_DESCRIPTOR_COUNT 5376u
#define PROC_ROOM_DESCRIPTOR_SOURCE_SHA \"${sourceSha}\"
typedef struct __attribute__((packed)) {
 uint8_t family_id,arrangement_id,landmark_id,material_id,exception_id,art_version;
 uint32_t selector_hash;
} ProcRoomDescriptor;
static const ProcRoomDescriptor proc_room_descriptors[PROC_ROOM_DESCRIPTOR_COUNT]={
${rows}
};
static const ProcRoomDescriptor *proc_room_descriptor_at(int system_id,int hub_index,int room_id){
 unsigned i;if(system_id<0||system_id>=256||hub_index<0||hub_index>=3||room_id<0||room_id>=7)return 0;
 i=(unsigned)system_id*21u+(unsigned)hub_index*7u+(unsigned)room_id;return &proc_room_descriptors[i];
}
#endif
`;
fs.mkdirSync(output.substring(0,output.lastIndexOf('/'))||'.',{recursive:true});fs.writeFileSync(output,out);console.log(`Baked ${records.length} room descriptors to ${output}`);
