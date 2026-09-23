#!/usr/bin/env node
/* Validate the shared station-room planning register without activating runtime art. */
import fs from 'node:fs';
import crypto from 'node:crypto';
import path from 'node:path';

const arg=(name,fallback)=>{const i=process.argv.indexOf(name);return i>=0?process.argv[i+1]:fallback;};
const registerPath=arg('--register');
const stationPath=arg('--station','work/planet-preview/station-activity.json');
const out=arg('--out','work/planet-preview/room-coverage-report.json');
if(!registerPath){console.error('Usage: node tools/validate-room-coverage.mjs --register <ROOM-COVERAGE-REGISTER.json> [--station <station-activity.json>] [--out <report.json>]');process.exit(2);}
const data=JSON.parse(fs.readFileSync(registerPath,'utf8'));
const station=JSON.parse(fs.readFileSync(stationPath,'utf8'));
const fail=[];const check=(ok,msg)=>{if(!ok)fail.push(msg);};
const codes=data.room_codes||[];
check(data.schema_version===1,'unsupported register schema');
check(data.counts?.systems===256&&data.counts?.hubs===768&&data.counts?.logical_rooms_per_hub===7&&data.counts?.room_records===5376,'register counts are not 256x3x7');
check(codes.length===7&&new Set(codes).size===7,'room code set is incomplete or duplicated');
check(Array.isArray(data.records)&&data.records.length===5376,'register record count mismatch');
check(Array.isArray(station.records)&&station.records.length===768,'station selector manifest must cover 768 hubs');
const ids=new Set(),hubIds=new Set(),roomCounts=new Map();let pending=0,fallback=0,authored=0,truthfulDialogue=0;
for(const r of data.records||[]){
  check(!ids.has(r.room_id),`duplicate room_id ${r.room_id}`);ids.add(r.room_id);
  check(r.system_id>=0&&r.system_id<256&&r.hub_index>=0&&r.hub_index<3,`invalid hub identity ${r.room_id}`);
  check(codes.includes(r.room_code),`invalid room code ${r.room_id}`);
  const hub=`${r.system_id}/${r.hub_index}`;hubIds.add(hub);roomCounts.set(hub,(roomCounts.get(hub)||0)+1);
  check(r.family_selector?.status==='Systems-owned selector pending','register unexpectedly freezes selector');
  check(r.capability_manifest?.proposed_actions_ignored_by_art===true,'capability manifest is not read-only for art');
  check(Array.isArray(r.current_actions)&&Array.isArray(r.planned_actions),'dialogue/action state arrays missing');
  check(Array.isArray(r.capability_manifest?.actions)&&r.current_actions.every(action=>r.capability_manifest.actions.includes(action)),'capability manifest omits a current action');
  check(r.planned_actions.every(action=>!r.capability_manifest.actions.includes(action)),'planned action leaked into authoritative capability actions');
  check(r.asset?.asset_bytes===null||Number.isInteger(r.asset?.asset_bytes)||Array.isArray(r.asset?.asset_bytes),'asset byte field must remain null, integer, or array');
  if(r.family_selector?.selector_version==null||r.family_selector?.selector_hash==null)pending++;
  if(r.visual_plan?.current_fallback==='generic renderer')fallback++;
  if(/Reorte H0 (Arrivals|Second Shift)/.test(`${r.art_status} ${r.no_bespoke_reason}`))authored++;
  if(r.current_actions.every(action=>r.capability_manifest.actions.includes(action))&&r.planned_actions.every(action=>!r.capability_manifest.actions.includes(action)))truthfulDialogue++;
}
for(const [hub,count] of roomCounts)check(count===7,`hub ${hub} has ${count} room records`);
check(hubIds.size===768,'register does not cover all hubs');
check(data.production_split?.authored_exceptions?.length===2,'authored exception count must remain two');
const sourceHash=crypto.createHash('sha256').update(fs.readFileSync(registerPath)).digest('hex');
const report={report_version:1,register_schema:data.schema_version,register_source_sha:data.source_sha,register_file_sha256:sourceHash,coverage:{systems:256,hubs:hubIds.size,rooms:ids.size,rooms_per_hub:7,selector_pending:pending,generic_fallback:fallback,authored_exception_mentions:authored,truthful_dialogue_records:truthfulDialogue},dialogue_qa:{status:'fixture_pending',native_resolution:[480,272],required_checks:['long-name speech wrapping','choice wrapping and clipping','normal/high-contrast readability','repeated conversation state','unavailable and completed outcome text','pause/exit/save/reload stability','no text for unavailable capabilities'],source:'Story & Dialogue fixtures plus authoritative Gameplay/Station capability manifest'},activation:{runtime_art_activation:false,capability_manifest_read_only:true,save_schema_change:false,selector_version:'pending',selector_hash:'pending',generator_version:'pending',generator_hash:'pending',native_capture:'pending',draw_cost:'pending'},notes:['This is a build-time planning validation report.','Do not treat planned procedural variants as shipped art or gameplay capability.','Generic station behavior remains the truthful fallback until freeze gates pass.']};
if(fail.length){console.error(`Room coverage validation failed (${fail.length} issue${fail.length===1?'':'s'})`);for(const msg of fail.slice(0,30))console.error(`- ${msg}`);process.exit(1);}
fs.mkdirSync(path.dirname(out),{recursive:true});fs.writeFileSync(out,JSON.stringify(report,null,2)+'\n');
console.log(`Room coverage validation passed: ${ids.size} records, ${hubIds.size} hubs, ${pending} selectors pending, generic fallback preserved`);
console.log(`Wrote ${out}`);
