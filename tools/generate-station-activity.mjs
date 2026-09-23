#!/usr/bin/env node
/* Generate a deterministic offline station/activity authoring manifest. */
import fs from 'node:fs';
import path from 'node:path';

const MASK=0xffffffff>>>0;
function h(v){v>>>=0;v^=v>>>16;v=Math.imul(v,0x7feb352d)>>>0;v^=v>>>15;v=Math.imul(v,0x846ca68b)>>>0;return (v^(v>>>16))>>>0;}
const names=['SYSTEM HUB','OUTER RELAY','FRONTIER OUTPOST'];
const moods=['quiet','working','busy','crowded'];
const records=[];
for(let system=0;system<256;system++)for(let hub=0;hub<3;hub++){
 const v=h(Math.imul(system+1,911)+Math.imul(hub+1,65537));
 const windows=4+(v%7)+(system%5>=3?3:0);
 const traffic=2+((v>>>8)%7)+(system%5>=3?2:0);
 const security=1+((v>>>16)%5)+(system%8>=5?2:0);
 const service=1+((v>>>24)%4)+(hub>0?1:0);
 records.push({system,hub,name:names[hub],mood:moods[Math.min(3,Math.floor((traffic-2)/3))],windows,traffic,security,service,seed:v});
}
const out=process.argv[process.argv.indexOf('--out')+1];
if(!out){console.error('Usage: node tools/generate-station-activity.mjs --out <directory>');process.exit(2);}
fs.mkdirSync(out,{recursive:true});
const manifest={generator:'tools/generate-station-activity.mjs',activity_version:1,records};
fs.writeFileSync(path.join(out,'station-activity.json'),JSON.stringify(manifest,null,2)+'\n');
console.log(`Generated ${records.length} deterministic station activity profiles across 256 systems`);
console.log(`Wrote ${path.join(out,'station-activity.json')}`);
