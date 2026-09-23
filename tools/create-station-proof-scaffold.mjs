#!/usr/bin/env node
/* Create a deterministic, non-certifying proof manifest for owner handoff. */
import fs from 'node:fs';
import path from 'node:path';
import crypto from 'node:crypto';

const arg=(name,fallback)=>{const i=process.argv.indexOf(name);return i>=0?process.argv[i+1]:fallback;};
const descriptorsPath=arg('--descriptors','work/planet-preview/room-descriptors.json');
const out=arg('--out','work/planet-preview/station-proof-slice.json');
const descriptors=JSON.parse(fs.readFileSync(descriptorsPath,'utf8'));
const rooms=['ARRIVALS','SHOP','CANTEEN','CARGO','GUILD','CLINIC','CUSTOMS'];
const records=descriptors.records||[];
const byKey=new Map();
for(const record of records){
  const key=`${record.family_id}/${record.hub_index}/${record.room_code}`;
  if(!byKey.has(key)) byKey.set(key,record);
}
const samples=[];
for(const family_id of [0,1,2]) for(const hub_index of [0,1,2]) for(const room_code of rooms){
  const key=`${family_id}/${hub_index}/${room_code}`;
  const descriptor=byKey.get(key);
  if(!descriptor) throw new Error(`missing descriptor ${key}`);
  samples.push({
    family_id,hub_index,room_code,
    selector_fingerprint:descriptor.selector_fingerprint,
    selector_parity:null,
    normal_capture:null,
    contrast_capture:null,
    dialogue_truth:null,
    save_economy_safe:null,
    duplicate_anchor_check:null,
    static_asset_bytes:null,
    plan_bytes:null,
    draw_calls_per_frame:null,
    peak_temp_memory_bytes:null,
    frame_time_ms:null,
    evidence:[],
    status:'pending'
  });
}
const descriptorBytes=fs.readFileSync(descriptorsPath);
const manifest={
  proof_version:1,
  status:'pending',
  certified:false,
  source_descriptors:descriptorsPath,
  source_descriptors_sha256:crypto.createHash('sha256').update(descriptorBytes).digest('hex'),
  scope:{families:[0,1,2],hubs:[0,1,2],rooms,required_samples:samples.length},
  samples,
  note:'Scaffold only. Owner evidence must replace every null field; this file must never be treated as runtime certification.'
};
fs.mkdirSync(path.dirname(out),{recursive:true});
fs.writeFileSync(out,JSON.stringify(manifest,null,2)+'\n');
console.log(`Wrote pending station proof scaffold: ${samples.length} samples`);
