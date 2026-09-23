#!/usr/bin/env node
/* Audit authored Prospector/Research extraction targets without certifying sparse art. */
import fs from 'node:fs';
import path from 'node:path';

const arg=(name,fallback)=>{const i=process.argv.indexOf(name);return i>=0?process.argv[i+1]:fallback;};
const manifestPath=arg('--manifest');const out=arg('--out','work/planet-preview/family-extraction-audit.json');
if(!manifestPath){console.error('Usage: node tools/validate-family-extraction.mjs --manifest <ELEMENT-MANIFEST.json> [--out <audit.json>]');process.exit(2);}
const manifest=JSON.parse(fs.readFileSync(manifestPath,'utf8'));const elements=new Map((manifest.elements||[]).map(e=>[e.id,e]));
const targets=[
 ['PROSPECTOR','ore/sample wall with claim tags',null,[92,72]],['PROSPECTOR','assay bench and task lamp','assay-bench',[74,42]],['PROSPECTOR','cargo rack / packed crates','cargo-rack',[52,64]],['PROSPECTOR','lander/desert window',null,[82,52]],['PROSPECTOR','standing prospector with sample case',null,[18,48]],['PROSPECTOR','seated miner silhouette',null,[18,38]],
 ['RESEARCH','specimen cabinet',null,[64,86]],['RESEARCH','survey console','survey-console',[70,44]],['RESEARCH','telescope / orbit instrument',null,[58,58]],['RESEARCH','ringed-planet orbit window',null,[104,64]],['RESEARCH','standing researcher',null,[18,48]],['RESEARCH','seated researcher / lounge chair',null,[22,40]]
];
const findings=[];for(const [family,anchor,id,size] of targets){const e=id?elements.get(id):null;const found=!!e;const dimensions=found&&e.size[0]===size[0]&&e.size[1]===size[1];findings.push({family,anchor,expected_id:id,expected_size:size,found_id:found?id:null,found_size:found?e.size:null,status:found&&dimensions?'candidate':'missing_or_mismatched'});}
const missing=findings.filter(f=>f.status!=='candidate');const report={audit_version:1,manifest_path:manifestPath,native_resolution:manifest.screen?.width===480&&manifest.screen?.height===272?[480,272]:manifest.screen?.[0]||null,packed4bit_basis:manifest.static_cost_basis||null,shared_palette_bytes:manifest.shared_palette_bytes||null,targets:findings,summary:{total:targets.length,candidates:targets.length-missing.length,missing_or_mismatched:missing.length,certified:false},gates:{normal_capture:'pending',contrast_capture:'pending',deterministic_repeat:'pending',hotspot_truth:'pending',runtime_allocations:0,draw_budget:'pending'}};
fs.mkdirSync(path.dirname(out),{recursive:true});fs.writeFileSync(out,JSON.stringify(report,null,2)+'\n');
if(missing.length){console.error(`Family extraction audit: ${missing.length} of ${targets.length} targets missing or mismatched; certification refused`);for(const f of missing)console.error(`- ${f.family}: ${f.anchor} expected ${f.expected_size.join('x')}`);process.exit(1);}
console.log(`Family extraction audit passed: ${targets.length} targets match native sizes`);
