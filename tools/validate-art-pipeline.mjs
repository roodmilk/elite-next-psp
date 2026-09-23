#!/usr/bin/env node
/* End-to-end offline gate for the deterministic planet/art/content pipeline. */
import fs from 'node:fs';

const dir=process.argv[process.argv.indexOf('--dir')+1]||'work/planet-preview';
const contractPath=process.argv[process.argv.indexOf('--contract')+1]||'tools/planet-art-contract.json';
const packPath=process.argv[process.argv.indexOf('--pack')+1]||'planet.content';
const fail=[];const check=(ok,msg)=>{if(!ok)fail.push(msg);};
function readJson(file){try{return JSON.parse(fs.readFileSync(file,'utf8'));}catch(e){fail.push(`${file}: ${e.message}`);return {};}}
const contract=readJson(contractPath),profiles=readJson(`${dir}/planet-profiles.json`),stations=readJson(`${dir}/station-activity.json`),atlas=readJson(`${dir}/planet-art-atlas.json`);
check(contract.contract_version===1&&contract.profile_version===1,'contract version mismatch');
check(contract.rules.gameplay_rng===false&&contract.rules.collision_from_art===false&&contract.rules.runtime_allocations===0,'contract safety rule missing');
check(Array.isArray(profiles.systems)&&profiles.systems.length===256,'profiles do not cover 256 systems');
let profileCount=0;for(const s of profiles.systems||[])for(const p of s.planets||[]){profileCount++;check(p.type==='ocean'?p.family===0:p.family===1+(p.seed%4),`profile family mismatch ${s.system}/${p.body}`);}
check(profileCount===768,`profiles contain ${profileCount} bodies instead of 768`);
check(stations.activity_version===1&&Array.isArray(stations.records)&&stations.records.length===768,'station manifest must contain 768 records');
check(stations.records?.every(r=>r.architecture>=0&&r.architecture<6&&r.arrangement>=0&&r.arrangement<8&&r.landmark>=0&&r.landmark<8&&r.windows>=4&&r.windows<=13&&r.traffic>=2&&r.traffic<=10&&r.security>=1&&r.security<=7&&r.service>=1&&r.service<=5),'station profile/activity bounds invalid');
check(atlas.atlas_version===1&&atlas.variants===160&&atlas.tile?.[0]===32&&atlas.tile?.[1]===32,'planet art atlas metadata invalid');
try{const b=fs.readFileSync(packPath);check(b.length===12304,'planet pack size invalid');check(b.toString('ascii',0,4)==='EPWP'&&b[4]===1,'planet pack header/version invalid');check(b.readUInt16LE(6)===768&&b.readUInt32LE(8)===12288,'planet pack record metadata invalid');let sum=2166136261>>>0;for(let i=16;i<b.length;i++){sum^=b[i];sum=Math.imul(sum,16777619)>>>0;}check(sum===b.readUInt32LE(12),'planet pack checksum invalid');}catch(e){fail.push(`planet pack: ${e.message}`);}
try{const b=fs.readFileSync('station.content');check(b.length===12304,'station pack size invalid');check(b.toString('ascii',0,4)==='EPST'&&b[4]===1,'station pack header/version invalid');check(b.readUInt16LE(6)===768&&b.readUInt32LE(8)===12288,'station pack record metadata invalid');let sum=2166136261>>>0;for(let i=16;i<b.length;i++){sum^=b[i];sum=Math.imul(sum,16777619)>>>0;}check(sum===b.readUInt32LE(12),'station pack checksum invalid');}catch(e){fail.push(`station pack: ${e.message}`);}
if(fail.length){console.error(`Art pipeline validation failed (${fail.length} issue${fail.length===1?'':'s'})`);for(const msg of fail.slice(0,30))console.error(`- ${msg}`);process.exit(1);}
console.log('Art pipeline validation passed: contract, profiles, station activity, atlas, and EPWP sidecar are coherent');
