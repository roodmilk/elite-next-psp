#!/usr/bin/env node
/* Validate the frozen native Reusable Element Kit as an offline budget input. */
import fs from 'node:fs';
import path from 'node:path';

const arg=(name,fallback)=>{const i=process.argv.indexOf(name);return i>=0?process.argv[i+1]:fallback;};
const manifestPath=arg('--manifest');
const out=arg('--out','work/planet-preview/element-kit-budget.json');
if(!manifestPath){console.error('Usage: node tools/validate-element-kit-budget.mjs --manifest <ELEMENT-MANIFEST.json> [--out <report.json>]');process.exit(2);}
const m=JSON.parse(fs.readFileSync(manifestPath,'utf8'));const fail=[];const check=(ok,msg)=>{if(!ok)fail.push(msg);};
check(m.screen?.width===480&&m.screen?.height===272,'native screen is not 480x272');
check(JSON.stringify(m.screen?.room)==='[6,20,340,168]','room bounds do not match Art1 contract');
check(m.static_cost_basis?.startsWith('packed 4-bit indices'),'packed4bit cost basis missing');
check(Array.isArray(m.elements)&&m.elements.length===12,'element manifest must contain 12 frozen elements');
const ids=new Set();let total=0;
for(const e of m.elements||[]){check(!ids.has(e.id),`duplicate element ${e.id}`);ids.add(e.id);check(Array.isArray(e.size)&&e.size.length===2&&e.size.every(Number.isInteger),`invalid native size ${e.id}`);check(Number.isInteger(e.packed_4bit_bytes)&&e.packed_4bit_bytes>0,`invalid packed cost ${e.id}`);check(typeof e.authored==='boolean'&&typeof e.procedural==='boolean',`missing authored/procedural flag ${e.id}`);check(typeof e.contrast==='string'&&e.contrast.length>0,`missing contrast rule ${e.id}`);total+=e.packed_4bit_bytes;}
const assembly=m.assembly_cost_bytes||{};for(const [name,bytes] of Object.entries({freight_core:11055,prospector_core:11108,research_core:10872}))check(assembly[name]===bytes,`${name} assembly cost changed or missing`);
check(m.shared_palette_bytes===44,'shared palette cost must remain 44 bytes');
const report={report_version:1,manifest_path:manifestPath,screen:m.screen,element_count:ids.size,element_index_bytes:total,shared_palette_bytes:m.shared_palette_bytes,assembly_cost_bytes:assembly,room_candidate_ceiling_bytes:28560,assembly_fits_room_candidate:Object.values(assembly).every(v=>Number.isInteger(v)&&v+44<=28560),activation:{runtime_wiring:false,selector_version:'pending',generator_version:'pending',native_capture:'planning evidence only',draw_cost:'pending PSP/emulator capture'},owners:m.owners};
if(fail.length){console.error(`Element-kit budget validation failed (${fail.length} issue${fail.length===1?'':'s'})`);for(const msg of fail)console.error(`- ${msg}`);process.exit(1);}
fs.mkdirSync(path.dirname(out),{recursive:true});fs.writeFileSync(out,JSON.stringify(report,null,2)+'\n');
console.log(`Element-kit budget validation passed: ${ids.size} elements, ${total}B individual indices, three family assemblies fit the room ceiling`);
console.log(`Wrote ${out}`);
