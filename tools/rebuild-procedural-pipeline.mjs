#!/usr/bin/env node
/* Rebuild the complete offline procedural/content validation chain. */
import {spawnSync} from 'node:child_process';
import crypto from 'node:crypto';
import fs from 'node:fs';
import path from 'node:path';

const arg=(name,fallback)=>{const i=process.argv.indexOf(name);return i>=0?process.argv[i+1]:fallback;};
const out=arg('--out','work/planet-preview');
const contract=arg('--contract','tools/planet-art-contract.json');
const register=arg('--register');
const elementManifest=arg('--element-manifest');
const run=(script,args)=>{const r=spawnSync(process.execPath,[script,...args],{stdio:'inherit'});if(r.status!==0)process.exit(r.status??1);};
fs.mkdirSync(out,{recursive:true});
run('tools/generate-planet-previews.mjs',['--out',out]);
run('tools/generate-station-activity.mjs',['--out',out]);
run('tools/generate-planet-art-atlas.mjs',['--out',out]);
run('tools/export-planet-content-pack.mjs',['--manifest',path.join(out,'planet-profiles.json'),'--out','planet.content']);
run('tools/export-station-content-pack.mjs',['--manifest',path.join(out,'station-activity.json'),'--out','station.content']);
run('tools/measure-psp-art-budget.mjs',['--dir',out,'--contract',contract,'--planet-pack','planet.content','--station-pack','station.content']);
run('tools/validate-planet-previews.mjs',['--manifest',path.join(out,'planet-profiles.json')]);
run('tools/validate-art-pipeline.mjs',['--dir',out,'--contract',contract,'--pack','planet.content']);
if(register){run('tools/generate-room-descriptors.mjs',['--register',register,'--out',path.join(out,'room-descriptors.json')]);run('tools/validate-room-coverage.mjs',['--register',register,'--station',path.join(out,'station-activity.json'),'--out',path.join(out,'room-coverage-report.json')]);}
if(elementManifest)run('tools/validate-element-kit-budget.mjs',['--manifest',elementManifest,'--out',path.join(out,'element-kit-budget.json')]);
const files=['planet-profiles.json','planet-profiles.ppm','station-activity.json','planet-art-atlas.json','planet-art-atlas.ppm','psp-art-budget.json','planet.content','station.content','room-descriptors.json','room-coverage-report.json','element-kit-budget.json'].filter(name=>fs.existsSync(path.join(out,name))||fs.existsSync(name));
const artifacts={};for(const name of files){const file=fs.existsSync(path.join(out,name))?path.join(out,name):name;const bytes=fs.readFileSync(file);artifacts[name]={path:file,bytes:bytes.length,sha256:crypto.createHash('sha256').update(bytes).digest('hex')};}
fs.writeFileSync(path.join(out,'pipeline-index.json'),JSON.stringify({index_version:1,contract_version:JSON.parse(fs.readFileSync(contract,'utf8')).contract_version,profile_version:1,generated_by:'tools/rebuild-procedural-pipeline.mjs',artifacts},null,2)+'\n');
console.log(`Procedural pipeline rebuild passed: ${out}`);
