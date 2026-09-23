#!/usr/bin/env node
/* Consolidate offline procedural proof gates without certifying native runtime. */
import fs from 'node:fs';
import path from 'node:path';

const arg=(name,fallback)=>{const i=process.argv.indexOf(name);return i>=0?process.argv[i+1]:fallback;};
const dir=arg('--dir','work/planet-preview');
const out=arg('--out',path.join(dir,'procedural-proof-qa-report.json'));
const read=(name)=>{const file=path.join(dir,name);try{return {file,value:JSON.parse(fs.readFileSync(file,'utf8'))};}catch{return {file,value:null};}};
const station=read('station-proof-report.json');
const extraction=read('family-extraction-audit-native.json');
const familyBudget=read('family-element-budget.json');
const frozenBudget=read('element-kit-budget.json');
const room=read('room-coverage-report.json');
const descriptors=read('room-descriptors.json');
const proof=read('station-proof-slice.json');
const statuses={};
const status=(name,value,details)=>{statuses[name]={status:value,details};};

const descriptorRecords=descriptors.value?.records||[];
const samples=proof.value?.samples||[];
const required=[];for(const family_id of [0,1,2])for(const hub_index of [0,1,2])for(const room_code of ['ARRIVALS','SHOP','CANTEEN','CARGO','GUILD','CLINIC','CUSTOMS'])required.push(`${family_id}/${hub_index}/${room_code}`);
const descriptorByKey=new Map();for(const record of descriptorRecords){const key=`${record.family_id}/${record.hub_index}/${record.room_code}`;if(!descriptorByKey.has(key))descriptorByKey.set(key,record);}
const sampleByKey=new Map(samples.map(s=>[`${s.family_id}/${s.hub_index}/${s.room_code}`,s]));
const descriptorKeyMatches=required.filter(key=>descriptorByKey.has(key)).length;
const fingerprintMatches=required.filter(key=>descriptorByKey.has(key)&&sampleByKey.has(key)&&descriptorByKey.get(key).selector_fingerprint===sampleByKey.get(key).selector_fingerprint).length;
const selectorEvidence=required.filter(key=>sampleByKey.get(key)?.selector_parity===true).length;
status('selector_parity',selectorEvidence===required.length?'pass':'pending',{descriptor_records:descriptorRecords.length,required_samples:required.length,descriptor_keys:descriptorKeyMatches,fingerprint_matches:fingerprintMatches,owner_parity_fields_passed:selectorEvidence});

const targetCount=extraction.value?.summary?.total||12;const candidateCount=extraction.value?.summary?.candidates||0;
status('native_states',candidateCount===targetCount?'pending_capture':'blocked',{dimension_candidates:candidateCount,target_count:targetCount,normal_capture:extraction.value?.gates?.normal_capture||'missing',contrast_capture:extraction.value?.gates?.contrast_capture||'missing',deterministic_repeat:extraction.value?.gates?.deterministic_repeat||'missing'});
status('native_runtime_capture','blocked',{normal_capture:'pending',high_contrast_capture:'pending',input_traversal:'pending',emulator_report:'blocked before game-check.txt'});
const familyFits=familyBudget.value?.families&&Object.values(familyBudget.value.families).every(f=>f.normal_fits_room_ceiling&&f.contrast_fits_room_ceiling);
const frozenFits=frozenBudget.value?.assembly_fits_room_candidate===true;
const measuredBudgetSamples=required.filter(key=>['static_asset_bytes','plan_bytes','draw_calls_per_frame','peak_temp_memory_bytes','frame_time_ms'].every(field=>Number.isFinite(sampleByKey.get(key)?.[field]))).length;
status('budget',familyFits&&frozenFits&&measuredBudgetSamples===required.length?'pass':'pending',{family_candidate_budget:familyFits?'pass':'missing',frozen_kit_budget:frozenFits?'pass':'missing',measured_runtime_samples:measuredBudgetSamples,required_samples:required.length});
const boolGate=(field,name)=>{const passed=required.filter(key=>sampleByKey.get(key)?.[field]===true).length;status(name,passed===required.length?'pass':'pending',{passed,required:required.length});};
boolGate('duplicate_anchor_check','duplicate_anchors');
boolGate('hotspot_ownership','hotspot_ownership');
boolGate('dialogue_truth','dialogue_truth');
boolGate('save_economy_safe','save_economy_safety');
const coverage=room.value?.coverage||room.value?.summary||{};
status('room_coverage',coverage.rooms===5376&&coverage.hubs===768?'pass':'pending',{records:coverage.rooms||coverage.records||0,hubs:coverage.hubs||0,selectors_pending:coverage.selector_pending??coverage.selectors_pending??null});

const pending=Object.entries(statuses).filter(([,v])=>v.status!=='pass').map(([name,v])=>`${name}:${v.status}`);
const report={report_version:1,status:pending.length?'not_certified':'accepted',certified:pending.length===0,scope:{families:['Freight','Prospector','Research'],hubs:['H0','H1','H2'],rooms:7,required_samples:required.length},gates:statuses,source_reports:{station_proof:station.file,native_extraction:extraction.file,family_budget:familyBudget.file,frozen_budget:frozenBudget.file,room_coverage:room.file,descriptors:descriptors.file,proof_manifest:proof.file},note:pending.length?`Offline evidence remains pending: ${pending.join(', ')}. Generic station fallback remains authoritative.`:'All offline proof gates passed; native runtime certification still requires integrated PSP evidence.'};
fs.mkdirSync(path.dirname(out),{recursive:true});fs.writeFileSync(out,JSON.stringify(report,null,2)+'\n');
if(pending.length){console.error(`Procedural proof QA not certified: ${pending.join(', ')}`);process.exit(1);}
console.log(`Procedural proof QA accepted: ${required.length} samples`);
