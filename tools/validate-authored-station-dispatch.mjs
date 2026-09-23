#!/usr/bin/env node
/* Validate the Art branch's authored Reorte H0 dispatch without activating it in Systems. */
import fs from 'node:fs';
import crypto from 'node:crypto';
import path from 'node:path';
import { execFileSync } from 'node:child_process';

const arg=(name,fallback)=>{const i=process.argv.indexOf(name);return i>=0?process.argv[i+1]:fallback;};
const artRoot=arg('--art-root');
const systemsRoot=arg('--systems-root',process.cwd());
const out=arg('--out','work/planet-preview/authored-station-dispatch-report.json');
const expectedPayloadSha=arg('--payload-sha','7DE4C88C65940E25B34F9B620AE597355DD65F8965C1128D3F745A077399FD5A').toLowerCase();
if(!artRoot){
  console.error('Usage: node tools/validate-authored-station-dispatch.mjs --art-root <station-art-rollout> [--systems-root <systems>] [--out <report.json>] [--payload-sha <sha256>]');
  process.exit(2);
}
const fail=[];const check=(ok,msg)=>{if(!ok)fail.push(msg);};
const read=(root,relative)=>{const file=path.join(root,relative);check(fs.existsSync(file),`missing ${relative}`);return fs.existsSync(file)?fs.readFileSync(file,'utf8'):'';};
const artCrawl=read(artRoot,'src/station-crawl.h');
const artBar=read(artRoot,'src/station-bar-art.h');
const systemsCrawl=read(systemsRoot,'src/station-crawl.h');
const predicate='room==SC_R_CANTEEN&&game.system==39&&game.station_variant==0';
const sourceGates={
  authored_helper:artCrawl.includes('static int sc_second_shift_active(int room)'),
  predicate_exact:artCrawl.includes(predicate),
  authored_renderer:artCrawl.includes('if(sc_second_shift_active(sc_room))sc_illust_second_shift(VX,VY,VW,VH);'),
  generic_canteen_fallback:artCrawl.includes('else if(sc_room==SC_R_CANTEEN)sc_illust_canteen('),
  procedural_accents_gated:artCrawl.includes('if(!authored&&sc_proc_plan(&plan))sc_draw_proc_accents'),
  pixel_payload_included:artBar.includes('#include "generated/second-shift-pixels.h"'),
  systems_keeps_generic_authority:!systemsCrawl.includes('sc_second_shift_active')
};
check(sourceGates.authored_helper, 'Art dispatch helper is missing');
check(sourceGates.predicate_exact, 'Art dispatch predicate no longer targets Reorte H0 exactly');
check(sourceGates.authored_renderer, 'authored renderer branch is missing');
check(sourceGates.generic_canteen_fallback, 'generic canteen fallback branch is missing');
check(sourceGates.procedural_accents_gated, 'procedural accents are not gated behind the authored exception');
check(sourceGates.pixel_payload_included, 'authored pixel payload is not included by station art');
check(sourceGates.systems_keeps_generic_authority, 'Systems branch unexpectedly owns the Art-only authored dispatcher');

const payloadPath=path.join(artRoot,'src/generated/second-shift-pixels.h');
check(fs.existsSync(payloadPath),'missing generated Reorte pixel payload');
let payloadSha=null;
if(fs.existsSync(payloadPath))payloadSha=crypto.createHash('sha256').update(fs.readFileSync(payloadPath)).digest('hex');
check(payloadSha===expectedPayloadSha,`authored payload SHA-256 ${payloadSha||'missing'} != expected ${expectedPayloadSha}`);
let artCommit=null;
try{artCommit=execFileSync('git',['-C',artRoot,'rev-parse','HEAD'],{encoding:'utf8'}).trim();}catch{artCommit='unavailable';}
const report={
  report_version:1,
  systems_branch:{root:systemsRoot,authored_dispatch_owned:false,generic_renderer_authority:true},
  art_branch:{root:artRoot,commit:artCommit},
  selector:{predicate,authored_identity:{system_id:39,hub_index:0,room_code:'CANTEEN'},secondary_hubs_generic:{reorte_h1:true,reorte_h2:true}},
  source_gates:sourceGates,
  payload:{path:payloadPath,sha256:payloadSha,expected_sha256:expectedPayloadSha,match:payloadSha===expectedPayloadSha},
  runtime:{native_normal_capture:'pending',native_high_contrast_capture:'pending',measured_draw_budget:'pending',measured_temp_memory:'pending',runtime_certified:false},
  notes:['This is a source and payload contract gate; it does not claim native PSP capture or runtime certification.','Reorte H0 is the authored exception covered by this gate; Reorte secondary hubs remain on the generic fallback. Other family-specific art selectors are owned by their own rollout gates.']
};
fs.mkdirSync(path.dirname(out),{recursive:true});fs.writeFileSync(out,JSON.stringify({...report,status:fail.length?'fail':'pass',failures:fail},null,2)+'\n');
if(fail.length){console.error(`Authored station dispatch validation failed (${fail.length} issue${fail.length===1?'':'s'})`);for(const msg of fail)console.error(`- ${msg}`);process.exit(1);}
console.log(`Authored station dispatch validation passed: Reorte H0 predicate and generic secondary-hub fallback verified`);
console.log(`Wrote ${out}`);
