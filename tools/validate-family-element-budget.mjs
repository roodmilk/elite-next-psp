#!/usr/bin/env node
/* Measure native family candidates without promoting them to runtime assets. */
import fs from 'node:fs';
import path from 'node:path';

const arg=(name,fallback)=>{const i=process.argv.indexOf(name);return i>=0?process.argv[i+1]:fallback;};
const manifestPath=arg('--manifest');
const out=arg('--out','work/planet-preview/family-element-budget.json');
const ceiling=Number(arg('--ceiling','28560'));
if(!manifestPath){console.error('Usage: node tools/validate-family-element-budget.mjs --manifest <family-elements/MANIFEST.json> [--out <report.json>]');process.exit(2);}
const manifest=JSON.parse(fs.readFileSync(manifestPath,'utf8'));
const entries=Array.isArray(manifest)?manifest:(manifest.elements||manifest.entries||[]);
const expected={
  PROSPECTOR:[['prospector-ore-wall',[92,72]],['prospector-assay-bench',[74,42]],['prospector-cargo-rack',[52,64]],['prospector-lander-window',[82,52]],['prospector-standing-sample-case',[18,48]],['prospector-seated-miner',[18,38]]],
  RESEARCH:[['research-specimen-cabinet',[64,86]],['research-survey-console',[70,44]],['research-telescope',[58,58]],['research-orbit-window',[104,64]],['research-standing-researcher',[18,48]],['research-seated-researcher',[22,40]]]
};
const fail=[];const check=(ok,msg)=>{if(!ok)fail.push(msg);};
const byId=new Map();
for(const entry of entries){
  const size=Array.isArray(entry.size)?entry.size:[entry.width,entry.height];
  const key=`${entry.id}/${entry.variant||'unspecified'}`;
  if(!entry.id||!Number.isInteger(size[0])||!Number.isInteger(size[1])){fail.push(`invalid candidate entry ${key}`);continue;}
  check(Number.isInteger(entry.packed_4bit_bytes)&&entry.packed_4bit_bytes>0,`invalid packed cost ${key}`);
  byId.set(key,{...entry,size});
}
const families={};
for(const [family,targets] of Object.entries(expected)){
  let normal=0;let contrast=0;let count=0;
  for(const [id,size] of targets){
    for(const variant of ['normal','contrast']){
      const e=byId.get(`${id}/${variant}`);check(!!e,`missing ${variant} candidate ${id}`);if(!e)continue;
      check(e.size[0]===size[0]&&e.size[1]===size[1],`${id}/${variant} size ${e.size.join('x')} != ${size.join('x')}`);
      check(e.runtime_ready===false,`${id}/${variant} must remain runtime_ready:false until owner gates pass`);
      if(variant==='normal')normal+=e.packed_4bit_bytes;else contrast+=e.packed_4bit_bytes;
      count++;
    }
  }
  families[family]={candidate_count:count,normal_bytes:normal,contrast_bytes:contrast,both_variant_bytes:normal+contrast,normal_fits_room_ceiling:normal<=ceiling,contrast_fits_room_ceiling:contrast<=ceiling,both_variants_fit_room_ceiling:normal+contrast<=ceiling};
  check(normal<=ceiling,`${family} normal candidate set exceeds ${ceiling}B room ceiling`);
  check(contrast<=ceiling,`${family} contrast candidate set exceeds ${ceiling}B room ceiling`);
}
const report={report_version:1,manifest_path:manifestPath,native_resolution:[480,272],cost_basis:'packed 4-bit indices; shared palette and compiler alignment excluded',room_candidate_ceiling_bytes:ceiling,families,selector_placement:'pending Composer review against room-selector-v1',hotspot_owner:'pending Station/Gameplay/Planetary/Story review',runtime_ready:false,certified:false,failures:fail};
fs.mkdirSync(path.dirname(out),{recursive:true});fs.writeFileSync(out,JSON.stringify(report,null,2)+'\n');
if(fail.length){console.error(`Family element budget failed: ${fail.length} issue${fail.length===1?'':'s'}`);for(const msg of fail)console.error(`- ${msg}`);process.exit(1);}
console.log(`Family element budget passed: ${Object.keys(families).length} candidate families fit ${ceiling}B per-variant room ceiling; runtime promotion remains pending`);
