#!/usr/bin/env node
/* Rebuild the complete offline procedural/content validation chain. */
import {spawnSync} from 'node:child_process';
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
if(register)run('tools/validate-room-coverage.mjs',['--register',register,'--station',path.join(out,'station-activity.json'),'--out',path.join(out,'room-coverage-report.json')]);
if(elementManifest)run('tools/validate-element-kit-budget.mjs',['--manifest',elementManifest,'--out',path.join(out,'element-kit-budget.json')]);
console.log(`Procedural pipeline rebuild passed: ${out}`);
