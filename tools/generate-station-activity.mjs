#!/usr/bin/env node
/* Generate a deterministic offline station/activity authoring manifest. */
import fs from 'node:fs';
import path from 'node:path';

const MASK=0xffffffff>>>0;
function h(v){v>>>=0;v^=v>>>16;v=Math.imul(v,0x7feb352d)>>>0;v^=v>>>15;v=Math.imul(v,0x846ca68b)>>>0;return (v^(v>>>16))>>>0;}
function galaxy(){let s0=0x5a4a,s1=0x0248,s2=0xb753;const systems=[];const pairs='..LEXEGEZACEBISOUSESARMAINDIREA.ERATENBERALAVETIEDORQUANTEISRION';for(let i=0;i<256;i++){const x=s1>>>8,y=s0>>>8,government=(s1>>>3)&7;let economy=(s0>>>8)&7;if(government<2)economy|=2;const tech=((s1>>>8)&3)+(economy^7)+(government>>1)+(government&1);systems.push({x,y,government,economy,tech});for(let k=0;k<4;k++){const j=2*((s2>>>8)&31);s0=s0^0;s0=s0>>>0;const t=(s0+s1+s2)&0xffff;s0=s1;s1=s2;s2=t;}}
 return systems;}
const systemData=galaxy();
const names=['SYSTEM HUB','OUTER RELAY','FRONTIER OUTPOST'];
const moods=['quiet','working','busy','crowded'];
const records=[];
for(let system=0;system<256;system++)for(let hub=0;hub<3;hub++){
 const s=systemData[system],packed=(s.x|s.y<<8|s.government<<16|s.economy<<19|s.tech<<22)>>>0,v=h((packed^Math.imul(hub+1,0x9e3779b9))>>>0);
 const windows=4+(v%7)+(s.economy>=4?3:0);
 const traffic=2+((v>>>8)%7)+(s.economy>=4?2:0);
 const security=1+((v>>>16)%5)+(s.government>=5?2:0);
 const service=1+((v>>>24)%4)+(hub>0?1:0);
 records.push({system,hub,name:names[hub],mood:moods[Math.min(3,Math.floor((traffic-2)/3))],architecture:v%6,arrangement:(v>>>3)%8,landmark:(v>>>7)%8,windows,traffic,security,service,seed:v});
}
const out=process.argv[process.argv.indexOf('--out')+1];
if(!out){console.error('Usage: node tools/generate-station-activity.mjs --out <directory>');process.exit(2);}
fs.mkdirSync(out,{recursive:true});
const manifest={generator:'tools/generate-station-activity.mjs',activity_version:1,records};
fs.writeFileSync(path.join(out,'station-activity.json'),JSON.stringify(manifest,null,2)+'\n');
console.log(`Generated ${records.length} deterministic station activity profiles across 256 systems`);
console.log(`Wrote ${path.join(out,'station-activity.json')}`);
