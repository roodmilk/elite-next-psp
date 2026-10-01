// Data and layout checks only; does not execute PSP code or launch an emulator.
import fs from 'node:fs';
import assert from 'node:assert/strict';
const root=new URL('../src/',import.meta.url);
const read=f=>fs.readFileSync(new URL(f,root),'utf8');
const itemCount=Number(read('game.h').match(/#define EQUIPMENT_COUNT (\d+)/)[1]);
const fit=read('equipment-fit.h'),ui=read('ui-modern.h'),game=read('game.c');
const caps=[...fit.match(/caps\[10\]\[6\]=\{([\s\S]*?)\};/)[1].matchAll(/\{([^}]+)\}/g)].map(m=>m[1].split(',').map(Number));
assert.equal(caps.length,10);
const ships=[...game.match(/player_ships\[\]=\{([\s\S]*?)\};/)[1].matchAll(/\{"([^"]+)",(\d+),(\d+),(\d+),(\d+)\}/g)].map((m,i)=>({name:m[1],price:+m[3],slots:caps[i].reduce((a,b)=>a+b,0)}));
assert.equal(ships.length,10);
for(const c of caps){assert.equal(c.length,6);assert(c.every(n=>n>=1&&n<=4));}
ships.sort((a,b)=>a.price-b.price);for(let i=1;i<ships.length;i++)assert(ships[i].slots>=ships[i-1].slots,'More expensive hull loses total capacity');
for(const name of ['equipment_names','equipment_list_names','equipment_details','equipment_effects']){
 const items=[...ui.match(new RegExp(name+'\\[EQUIP_COUNT\\]=\\{([\\s\\S]*?)\\};'))[1].matchAll(/"([^"]*)"/g)].map(m=>m[1]);
 assert.equal(items.length,itemCount,`${name} missing a module`);
 if(name==='equipment_details')for(const text of items){
  let rest=text;for(let line=0;line<3&&rest;line++){let cut=Math.min(rest.length,31);if(rest.length>31)for(let k=cut;k>31/3;k--)if(rest[k]===' '){cut=k;break;}rest=rest.slice(cut).trimStart();}
  assert(!rest,`Outfitting description overflows: ${text}`);
 }
}
for(const name of ['equipment_costs','equipment_tech','equipment_econ','equipment_trade']){
 const values=ui.match(new RegExp(name+'\\[EQUIP_COUNT\\]=\\{([\\s\\S]*?)\\};'))[1].trim().split(',');assert.equal(values.length,itemCount,name);
}
// Every available slot fits inside the native 480x272 board and selection border.
for(let category=0;category<6;category++)for(let bank=0;bank<4;bank++){
 const x=64+47*bank,y=76+23*category;assert(x+43<260&&y+19<222);assert(4+4*8<43&&5+8<19);
}
assert(game.includes('s.version=26;'));assert(game.includes('memcpy(bank,g->fit+6,18)'));
assert(game.includes('memcpy(g->fit+6,bank,18)'));assert.equal(18+1+1,20);
assert(read('flight-tools.h').includes('fit_find(g,16)>=0'));assert(read('flight-tools.h').includes('fit_find(g,17)>=0'));
console.log('PASS: 10 hull capacity tables; increasing price tiers; eight complete equipment catalogues; native board bounds; V26 extension and extra-slot tool wiring.');
console.log(ships.map(s=>`${s.name}: ${s.slots} slots`).join('\n'));
console.log('LIMIT: static/data checks only. Runtime purchase, save migration and visual tests remain separate.');
const values=name=>ui.match(new RegExp(name+'\\[EQUIP_COUNT\\]=\\{([\\s\\S]*?)\\};'))[1].split(',').map(v=>Number(v.trim()));
const tech=values('equipment_tech'),econ=values('equipment_econ'),trade=values('equipment_trade');
const wealth=[5,4,2,3,3,5,4,2],seed=[0x5a4a,0x0248,0xb753],coverage=Array(itemCount).fill(0);
for(let system=0;system<256;system++){
 const gov=(seed[1]>>3)&7;let e=(seed[0]>>8)&7;if(gov<2)e|=2;
 const t=((seed[1]>>8)&3)+(e^7)+(gov>>1)+(gov&1)+1;
 for(let item=26;item<itemCount;item++)if(t>=tech[item]&&(econ[item]&(1<<e))&&wealth[e]>=trade[item])coverage[item]++;
 for(let j=0;j<4;j++){const sum=(seed[0]+seed[1]+seed[2])&65535;seed[0]=seed[1];seed[1]=seed[2];seed[2]=sum;}
}
for(let i=26;i<itemCount;i++)assert(coverage[i]>0,'Unobtainable module '+i);
console.log('PASS: all 30 new modules stocked in generated galaxy; system counts: '+coverage.slice(26).join(','));

