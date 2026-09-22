#!/usr/bin/env node
/* Deterministic offline planet-profile manifest and 256-system PPM atlas. */
import fs from 'node:fs';
import path from 'node:path';

const MASK=0xffffffff>>>0,OCEAN=2,ROCKY=1,GAS=3;
const names=['ocean','desert','ice','volcanic','forest'];
const colors=[[65,145,205],[201,157,83],[177,211,230],[173,76,53],[78,145,83]];
const perms=[[OCEAN,ROCKY,GAS,ROCKY],[ROCKY,GAS,OCEAN,ROCKY],[GAS,OCEAN,ROCKY,ROCKY],[ROCKY,OCEAN,ROCKY,GAS],[OCEAN,GAS,ROCKY,ROCKY],[ROCKY,ROCKY,OCEAN,GAS]];
function h(v){v>>>=0;v^=v>>>16;v=Math.imul(v,0x7feb352d)>>>0;v^=v>>>15;v=Math.imul(v,0x846ca68b)>>>0;return (v^(v>>>16))>>>0;}
function profile(seed,type){const v=h((seed^Math.imul(type,0x9e3779b9))>>>0);const family=type===OCEAN?0:1+(seed%4);return {seed,family,family_name:names[family],terrain_style:(v>>>8)%4,palette:(v>>>12)%8,sea_level:type===OCEAN?42:0,prop_density:35+v%46,activity_density:20+((v>>>16)%61)};}
const systems=[];for(let system=0;system<256;system++){const sv=h(Math.imul(system+1,0x9e3779b9));const perm=perms[(sv>>>6)%6];const planets=[];for(let body=1;body<5;body++){const seed=h(Math.imul(system+1,911)+body*65537);let type=perm[(body-1)&3];if(system===7&&body===1)type=OCEAN;if(type===GAS)continue;planets.push({...profile(seed,type),body,type:type===OCEAN?'ocean':'rocky'});}systems.push({system,planets});}
const manifest={generator:'tools/generate-planet-previews.mjs',profile_version:1,systems};
const out=process.argv[process.argv.indexOf('--out')+1];if(!out){console.error('Usage: node tools/generate-planet-previews.mjs --out <directory>');process.exit(2);}fs.mkdirSync(out,{recursive:true});fs.writeFileSync(path.join(out,'planet-profiles.json'),JSON.stringify(manifest,null,2)+'\n');
const w=256,hgt=256,pixels=Buffer.alloc(w*hgt*3,0);for(let i=0;i<w*hgt;i++){pixels[i*3]=18;pixels[i*3+1]=22;pixels[i*3+2]=31;}for(const entry of systems){const ox=(entry.system%16)*16,oy=Math.floor(entry.system/16)*16;entry.planets.slice(0,4).forEach((planet,i)=>{const c=colors[planet.family],px=ox+2+(i%2)*7,py=oy+2+Math.floor(i/2)*7;for(let y=py;y<py+5;y++)for(let x=px;x<px+5;x++){const n=(y*w+x)*3;pixels[n]=c[0];pixels[n+1]=c[1];pixels[n+2]=c[2];}});if(entry.system===7){const n=(oy*w+ox)*3;pixels[n]=255;pixels[n+1]=225;pixels[n+2]=120;}}
fs.writeFileSync(path.join(out,'planet-profiles.ppm'),Buffer.concat([Buffer.from(`P6\n${w} ${hgt}\n255\n`),pixels]));console.log(`Generated ${systems.reduce((n,s)=>n+s.planets.length,0)} landable/ocean planet profiles across 256 systems`);console.log(`Wrote ${path.join(out,'planet-profiles.json')}`);console.log(`Wrote ${path.join(out,'planet-profiles.ppm')}`);
