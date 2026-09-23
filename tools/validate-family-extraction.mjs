#!/usr/bin/env node
/* Audit authored Prospector/Research extraction targets without certifying sparse art. */
import fs from 'node:fs';
import path from 'node:path';

const arg=(name,fallback)=>{const i=process.argv.indexOf(name);return i>=0?process.argv[i+1]:fallback;};
const manifestPath=arg('--manifest');const out=arg('--out','work/planet-preview/family-extraction-audit.json');
if(!manifestPath){console.error('Usage: node tools/validate-family-extraction.mjs --manifest <ELEMENT-MANIFEST.json> [--out <audit.json>]');process.exit(2);}
const manifest=JSON.parse(fs.readFileSync(manifestPath,'utf8'));
const entries=Array.isArray(manifest)?manifest:(manifest.elements||manifest.entries||[]);
const elements=new Map();
for(const entry of entries){
  const size=Array.isArray(entry.size)?entry.size:[entry.width,entry.height];
  if(!entry.id||!Number.isFinite(size[0])||!Number.isFinite(size[1])) continue;
  const current=elements.get(entry.id);
  if(!current) elements.set(entry.id,{...entry,size});
}
const targets=[
 ['PROSPECTOR','ore/sample wall with claim tags',['prospector-ore-wall'],[92,72]],['PROSPECTOR','assay bench and task lamp',['prospector-assay-bench','assay-bench'],[74,42]],['PROSPECTOR','cargo rack / packed crates',['prospector-cargo-rack','cargo-rack'],[52,64]],['PROSPECTOR','lander/desert window',['prospector-lander-window'],[82,52]],['PROSPECTOR','standing prospector with sample case',['prospector-standing-sample-case'],[18,48]],['PROSPECTOR','seated miner silhouette',['prospector-seated-miner'],[18,38]],
 ['RESEARCH','specimen cabinet',['research-specimen-cabinet'],[64,86]],['RESEARCH','survey console',['research-survey-console','survey-console'],[70,44]],['RESEARCH','telescope / orbit instrument',['research-telescope'],[58,58]],['RESEARCH','ringed-planet orbit window',['research-orbit-window'],[104,64]],['RESEARCH','standing researcher',['research-standing-researcher'],[18,48]],['RESEARCH','seated researcher / lounge chair',['research-seated-researcher'],[22,40]]
];
const findings=[];for(const [family,anchor,ids,size] of targets){const id=ids.find(candidate=>elements.has(candidate));const e=id?elements.get(id):null;const found=!!e;const dimensions=found&&e.size[0]===size[0]&&e.size[1]===size[1];findings.push({family,anchor,expected_ids:ids,expected_size:size,found_id:found?id:null,found_size:found?e.size:null,status:found&&dimensions?'candidate':'missing_or_mismatched'});}
const missing=findings.filter(f=>f.status!=='candidate');const report={audit_version:1,manifest_path:manifestPath,native_resolution:manifest.screen?.width===480&&manifest.screen?.height===272?[480,272]:Array.isArray(manifest.screen)&&manifest.screen.length===2?manifest.screen:[480,272],packed4bit_basis:manifest.static_cost_basis||'packed 4-bit indices; variant bytes are manifest-provided',shared_palette_bytes:manifest.shared_palette_bytes||null,targets:findings,summary:{total:targets.length,candidates:targets.length-missing.length,missing_or_mismatched:missing.length,certified:false},gates:{normal_capture:'pending',contrast_capture:'pending',deterministic_repeat:'pending',hotspot_truth:'pending',runtime_allocations:0,draw_budget:'pending'},note:'Dimension matches are candidate evidence only; runtime_ready remains owner-controlled.'};
fs.mkdirSync(path.dirname(out),{recursive:true});fs.writeFileSync(out,JSON.stringify(report,null,2)+'\n');
if(missing.length){console.error(`Family extraction audit: ${missing.length} of ${targets.length} targets missing or mismatched; certification refused`);for(const f of missing)console.error(`- ${f.family}: ${f.anchor} expected ${f.expected_size.join('x')}`);process.exit(1);}
console.log(`Family extraction audit passed: ${targets.length} targets match native sizes`);
