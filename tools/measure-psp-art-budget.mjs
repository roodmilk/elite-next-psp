#!/usr/bin/env node
/* Measure offline art/content outputs without touching PSP runtime state. */
import fs from 'node:fs';
import path from 'node:path';

const arg=(name,fallback)=>{const i=process.argv.indexOf(name);return i>=0?process.argv[i+1]:fallback;};
const dir=arg('--dir','work/planet-preview');
const contractPath=arg('--contract','tools/planet-art-contract.json');
const out=arg('--out',path.join(dir,'psp-art-budget.json'));
const contract=JSON.parse(fs.readFileSync(contractPath,'utf8'));
const stat=file=>({path:file,bytes:fs.statSync(file).size});
const packInfo=(file,magic)=>{const b=fs.readFileSync(file);if(b.toString('ascii',0,4)!==magic||b[4]!==1)throw new Error(`${file}: unsupported ${magic} header`);const records=b.readUInt16LE(6);return {path:file,bytes:b.length,records,payloadBytes:b.readUInt32LE(8),recordBytes:b.readUInt32LE(8)/records};};
const atlas=JSON.parse(fs.readFileSync(path.join(dir,'planet-art-atlas.json'),'utf8'));
const atlasFile=path.join(dir,'planet-art-atlas.ppm');
const planetPack=packInfo(arg('--planet-pack','planet.content'),'EPWP');
const stationPack=packInfo(arg('--station-pack','station.content'),'EPST');
const tilePixels=atlas.tile[0]*atlas.tile[1];
const atlasTiles=atlas.variants;
const report={
  budget_report_version:1,contract_version:contract.contract_version,profile_version:contract.profile_version,
  native_resolution:contract.native_resolution,generated_from:{contract:contractPath,preview_dir:dir},
  measured:{planet_profiles_json:stat(path.join(dir,'planet-profiles.json')),station_activity_json:stat(path.join(dir,'station-activity.json')),planet_pack:planetPack,station_pack:stationPack,planet_art_atlas:{...stat(atlasFile),width:atlas.grid[0]*atlas.tile[0],height:atlas.grid[1]*atlas.tile[1],tiles:atlasTiles,tile:atlas.tile}},
  projections:{atlas_packed4bit_bytes:Math.ceil(atlasTiles*tilePixels/2),atlas_palette_bytes:atlasTiles*4*3,room_candidate_packed4bit_bytes:340*168/2,room_candidate_palette_bytes:44,room_candidate_basis:'Art Manager bounded 340x168 native room candidate; projection only until packed export exists'},
  runtime_guardrails:{runtime_allocations:contract.rules.runtime_allocations,collision_from_art:contract.rules.collision_from_art,gameplay_rng:contract.rules.gameplay_rng,max_surface_props:contract.budgets.max_surface_props,max_scannables:contract.budgets.max_scannables,max_dynamic_activity:contract.budgets.max_dynamic_activity,draw_cost:'not instrumented; requires PSP/emulator capture before runtime composition expands'}
};
fs.mkdirSync(path.dirname(out),{recursive:true});fs.writeFileSync(out,JSON.stringify(report,null,2)+'\n');
console.log(`Measured PSP art budget: ${planetPack.bytes}B planet pack, ${stationPack.bytes}B station pack, ${report.projections.atlas_packed4bit_bytes}B projected atlas indices`);
console.log(`Wrote ${out}`);
