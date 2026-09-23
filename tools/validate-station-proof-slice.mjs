#!/usr/bin/env node
/* Gate the first rich three-family × H0/H1/H2 × seven-room proof slice. */
import fs from 'node:fs';
import path from 'node:path';

const arg=(name,fallback)=>{const i=process.argv.indexOf(name);return i>=0?process.argv[i+1]:fallback;};
const descriptorsPath=arg('--descriptors','work/planet-preview/room-descriptors.json');
const proofPath=arg('--proof','work/planet-preview/station-proof-slice.json');
const out=arg('--out','work/planet-preview/station-proof-report.json');
const fail=[];const check=(ok,msg)=>{if(!ok)fail.push(msg);};
const descriptors=JSON.parse(fs.readFileSync(descriptorsPath,'utf8'));let proof=null;
try{proof=JSON.parse(fs.readFileSync(proofPath,'utf8'));}catch(e){fail.push(`proof manifest unavailable: ${proofPath}`);}
const rooms=['ARRIVALS','SHOP','CANTEEN','CARGO','GUILD','CLINIC','CUSTOMS'];
const required=[];for(const family_id of [0,1,2])for(const hub_index of [0,1,2])for(const room_code of rooms)required.push({family_id,hub_index,room_code});
const descriptorKey=(family_id,hub_index,room_code)=>descriptors.records?.find(r=>r.family_id===family_id&&r.hub_index===hub_index&&r.room_code===room_code);
const samples=new Map((proof?.samples||[]).map(s=>[`${s.family_id}/${s.hub_index}/${s.room_code}`,s]));
const requiredFields=['selector_parity','normal_capture','contrast_capture','dialogue_truth','save_economy_safe','duplicate_anchor_check','hotspot_ownership','static_asset_bytes','plan_bytes','draw_calls_per_frame','peak_temp_memory_bytes','frame_time_ms'];
for(const q of required){const key=`${q.family_id}/${q.hub_index}/${q.room_code}`,d=descriptorKey(q.family_id,q.hub_index,q.room_code),s=samples.get(key);check(!!d,`missing descriptor ${key}`);check(!!s,`missing proof sample ${key}`);if(!s)continue;for(const field of requiredFields)check(s[field]!==undefined&&s[field]!==null&&s[field]!=='' ,`${key}: missing ${field}`);if(d&&s.selector_fingerprint!==d.selector_fingerprint)fail.push(`${key}: selector fingerprint mismatch`);if(s.selector_parity!==true)fail.push(`${key}: selector parity not passed`);if(s.dialogue_truth!==true||s.save_economy_safe!==true||s.duplicate_anchor_check!==true||s.hotspot_ownership!==true)fail.push(`${key}: truth/safety/anchor ownership gate not passed`);for(const field of ['static_asset_bytes','plan_bytes','draw_calls_per_frame','peak_temp_memory_bytes','frame_time_ms'])if(s[field]!==undefined&&!Number.isFinite(s[field]))fail.push(`${key}: ${field} is not numeric`);}
const report={proof_report_version:1,status:fail.length?'not_certified':'accepted',certified:fail.length===0,scope:{families:[0,1,2],hubs:[0,1,2],rooms,required_samples:required.length},descriptors:descriptorsPath,proof_manifest:proofPath,checks:{selector_parity:true,native_normal_contrast:true,dialogue_truth:true,save_economy_safety:true,duplicate_anchor_checks:true,budget_fields:true},failures:fail.slice(0,100),note:fail.length?'No broad procedural activation is certified. Generic fallback remains authoritative.':'This proof slice is eligible for owner/Lead review; it is not a release claim.'};
fs.mkdirSync(path.dirname(out),{recursive:true});fs.writeFileSync(out,JSON.stringify(report,null,2)+'\n');
if(fail.length){console.error(`Station proof slice not certified: ${fail.length} issue${fail.length===1?'':'s'}`);for(const msg of fail.slice(0,20))console.error(`- ${msg}`);process.exit(1);}
console.log(`Station proof slice accepted: ${required.length} samples across Freight/Prospector/Research H0/H1/H2`);
