#!/usr/bin/env node
/* Export the validated planet manifest as a deterministic, versioned pack. */
import fs from 'node:fs';

const manifestPath=process.argv[process.argv.indexOf('--manifest')+1];
const out=process.argv[process.argv.indexOf('--out')+1];
if(!manifestPath||!out){console.error('Usage: node tools/export-planet-content-pack.mjs --manifest <planet-profiles.json> --out <planet.content>');process.exit(2);}
const manifest=JSON.parse(fs.readFileSync(manifestPath,'utf8'));
if(manifest.profile_version!==1||!Array.isArray(manifest.systems)||manifest.systems.length!==256)throw new Error('unsupported or incomplete planet manifest');
const records=[];
for(const system of manifest.systems)for(const p of system.planets||[]){
 if(p.family<0||p.family>4||p.terrain_style<0||p.terrain_style>=4||p.palette<0||p.palette>=8||p.prop_density<35||p.prop_density>80||p.activity_density<20||p.activity_density>80)throw new Error(`invalid profile ${system.system}/${p.body}`);
 if(p.type==='ocean'&&p.family!==0)throw new Error(`ocean family mismatch ${system.system}/${p.body}`);
 if(p.type==='rocky'&&p.family!==1+(p.seed%4))throw new Error(`rocky family mismatch ${system.system}/${p.body}`);
 const r=Buffer.alloc(16);r.writeUInt8(system.system,0);r.writeUInt8(p.body,1);r.writeUInt8(p.type==='ocean'?0:1,2);r.writeUInt8(p.family,3);r.writeUInt8(p.terrain_style,4);r.writeUInt8(p.palette,5);r.writeUInt8(p.sea_level,6);r.writeUInt8(p.prop_density,7);r.writeUInt8(p.activity_density,8);r.writeUInt32LE(p.seed>>>0,12);records.push(r);
}
if(records.length!==768)throw new Error(`expected 768 records, found ${records.length}`);
const payload=Buffer.concat(records);let checksum=2166136261>>>0;for(const byte of payload){checksum^=byte;checksum=Math.imul(checksum,16777619)>>>0;}
const header=Buffer.alloc(16);header.write('EPWP',0,'ascii');header.writeUInt8(1,4);header.writeUInt8(0,5);header.writeUInt16LE(records.length,6);header.writeUInt32LE(payload.length,8);header.writeUInt32LE(checksum,12);
fs.writeFileSync(out,Buffer.concat([header,payload]));console.log(`Exported EPWP v1: ${records.length} records, ${payload.length} payload bytes`);console.log(`Checksum: ${checksum.toString(16).padStart(8,'0')}`);console.log(`Wrote ${out}`);
