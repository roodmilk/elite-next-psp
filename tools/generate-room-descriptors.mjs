#!/usr/bin/env node
/* Build-time room descriptors; no runtime activation, save writes, or capability creation. */
import fs from 'node:fs';
import path from 'node:path';

const arg=(name,fallback)=>{const i=process.argv.indexOf(name);return i>=0?process.argv[i+1]:fallback;};
const registerPath=arg('--register');
const out=arg('--out','work/planet-preview/room-descriptors.json');
if(!registerPath){console.error('Usage: node tools/generate-room-descriptors.mjs --register <ROOM-COVERAGE-REGISTER.json> [--out <descriptors.json>]');process.exit(2);}
const source=JSON.parse(fs.readFileSync(registerPath,'utf8'));const fail=[];const check=(ok,msg)=>{if(!ok)fail.push(msg);};
const fnv1a=text=>{let h=2166136261>>>0;for(const byte of Buffer.from(text,'utf8')){h^=byte;h=Math.imul(h,16777619)>>>0;}return h.toString(16).padStart(8,'0');};
const roomCodes=source.room_codes||[];const familyNames=(source.family_proposals||[]).map(f=>f.name);const ids=new Set(),fingerprints=new Set();const records=[];
for(const r of source.records||[]){
  const key=`g0:s${r.system_id}:h${r.hub_index}:r${r.room_code}`;
  const family_id=familyNames.indexOf(r.family_proposal);const room_id=roomCodes.indexOf(r.room_code);const layout_id=parseInt(fnv1a(`layout|${r.layout_variant}`),16)%256;const landmark_id=parseInt(fnv1a(`landmark|${r.hero_landmark}`),16)%256;const material_id=parseInt(fnv1a(`material|${r.family_proposal}|${r.layout_variant}`),16)%256;const exception_id=r.system_id===39&&r.hub_index===0&&r.room_code==='ARRIVALS'?1:r.system_id===39&&r.hub_index===0&&r.room_code==='CANTEEN'?2:0;
  check(family_id>=0&&family_id<256,`unknown family ${r.family_proposal}`);check(room_id>=0&&room_id<256,`unknown room ${r.room_code}`);
  const fingerprint=fnv1a(`room-selector-v1|${key}|${family_id}|${layout_id}|${landmark_id}|${material_id}|${exception_id}`);
  check(!ids.has(r.room_id),`duplicate room ${r.room_id}`);check(!fingerprints.has(fingerprint),`duplicate selector fingerprint ${fingerprint}`);ids.add(r.room_id);fingerprints.add(fingerprint);
  records.push({galaxy_id:0,system_id:r.system_id,hub_index:r.hub_index,room_id,room_code:r.room_code,family_id,arrangement_id:layout_id,landmark_id,material_id,art_version:1,exception_id,hub_role:r.hub_role,selector_version:1,selector_key:key,selector_fingerprint:fingerprint,family:r.family_proposal,layout:r.layout_variant,landmark:r.hero_landmark,visual_target:r.visual_plan?.target||'procedural_variant',fallback:r.visual_plan?.current_fallback||'generic renderer',current_actions:[...(r.current_actions||[])],exits:[...(r.exits||[])],capability_source:r.capability_manifest?.source||'authoritative Station/Gameplay record',art_writes:[],runtime_activation:false});
}
check(source.schema_version===1,'unsupported source register schema');check(records.length===5376,'descriptor count must be 5376');check(ids.size===records.length&&fingerprints.size===records.length,'descriptor identity/fingerprint collision');
if(fail.length){console.error(`Room descriptor generation failed (${fail.length} issue${fail.length===1?'':'s'})`);for(const msg of fail.slice(0,20))console.error(`- ${msg}`);process.exit(1);}
const output={descriptor_version:1,source_register_sha:source.source_sha,selector:{version:1,algorithm:'FNV-1a over UTF-8 canonical room-selector-v1 tuple',inputs:['galaxy_id','system_id','hub_index','room_id','family_id','arrangement_id','landmark_id','material_id','art_version','exception_id'],runtime_activation:false},counts:{systems:256,hubs:768,rooms:records.length},records};
fs.mkdirSync(path.dirname(out),{recursive:true});fs.writeFileSync(out,JSON.stringify(output,null,2)+'\n');
console.log(`Generated ${records.length} build-time room descriptors with stable selector fingerprints`);console.log(`Wrote ${out}`);
