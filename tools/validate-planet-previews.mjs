#!/usr/bin/env node
/* Validate the deterministic offline planet-profile manifest. */
import fs from 'node:fs';

const file=process.argv[process.argv.indexOf('--manifest')+1];
if(!file){console.error('Usage: node tools/validate-planet-previews.mjs --manifest <planet-profiles.json>');process.exit(2);}
const data=JSON.parse(fs.readFileSync(file,'utf8'));
const fail=[];const check=(ok,msg)=>{if(!ok)fail.push(msg);};
check(data.profile_version===1,'unsupported profile version');
check(Array.isArray(data.systems)&&data.systems.length===256,'manifest must contain 256 systems');
let total=0;
for(let s=0;s<(data.systems||[]).length;s++){
 const system=data.systems[s];check(system.system===s,`system index mismatch at ${s}`);check(Array.isArray(system.planets),`system ${s} has no planet list`);
 for(const p of system.planets||[]){total++;check(p.body>=1&&p.body<=4,`system ${s} has invalid body index`);check(p.type==='ocean'||p.type==='rocky',`system ${s} body ${p.body} has invalid type`);check(p.family>=0&&p.family<=4,`system ${s} body ${p.body} has invalid family`);check(p.type==='ocean'?p.family===0:p.family===1+(p.seed%4),`system ${s} body ${p.body} breaks orbit-to-ground family parity`);check(p.terrain_style>=0&&p.terrain_style<4,`system ${s} body ${p.body} has invalid terrain style`);check(p.palette>=0&&p.palette<8,`system ${s} body ${p.body} has invalid palette`);check(p.prop_density>=35&&p.prop_density<=80,`system ${s} body ${p.body} has invalid prop density`);check(p.activity_density>=20&&p.activity_density<=80,`system ${s} body ${p.body} has invalid activity density`);}
}
check(total===768,`expected 768 landable/ocean bodies, found ${total}`);
if(fail.length){console.error(`Planet preview validation failed (${fail.length} issue${fail.length===1?'':'s'})`);for(const msg of fail.slice(0,20))console.error(`- ${msg}`);process.exit(1);}
console.log(`Planet preview validation passed: 256 systems, ${total} bodies, bounded profiles, stable art-family parity`);
