import fs from 'node:fs';
import assert from 'node:assert/strict';
const read=f=>fs.readFileSync(new URL('../src/'+f,import.meta.url),'utf8');
const hash=v=>{v^=v>>>16;v=Math.imul(v,0x7feb352d);v^=v>>>15;v=Math.imul(v,0x846ca68b);return (v^(v>>>16))>>>0;};
const seed=[0x5a4a,0x248,0xb753];let count=0;const lave=[];
for(let sys=0;sys<256;sys++){
 const gov=(seed[1]>>>3)&7;let econ=(seed[0]>>>8)&7;if(gov<2)econ|=2;
 const tech=((seed[1]>>>8)&3)+(econ^7)+(gov>>>1)+(gov&1),culture=gov<2?3:tech>=10?2:econ>=4?1:0;
 for(let b=1;b<5;b++){
  const h=hash((sys+1)*911+b*65537),sites=[];
  for(let id=2;id<10;id++){if(id===6)continue;const rank=id<6?id-2:id-3;if((h%12+culture*3+rank*5)%12===2)sites.push(id);}
  assert(sites.length<=1);count+=sites.length;if(sys===7)lave.push(sites);
 }
 for(let j=0;j<4;j++){let n=(seed[0]+seed[1]+seed[2])&65535;seed[0]=seed[1];seed[1]=seed[2];seed[2]=n;}
}
assert.deepEqual(lave,[[],[2],[],[4]]);
const scene=read('observatory-scene.h'),state=read('observatory-state.h'),core=read('surface-activities.h'),game=read('game.c');
assert(state.includes('#define OBS_CLUES (7u<<22)'));
assert(core.includes('if(!surface_interact(g))return 0;'));
assert(core.includes('if(choice==1)g->credits+=200;else g->discoveries++;'));
assert(game.includes('s.version=26;')&&game.includes('observatory_flags_valid'));
assert(scene.includes('ps_reply=3')&&scene.includes('CONFIRM OBSERVATORY CHOICE'));
assert(read('discovery-atlas.h').includes('observatory_report(observatory_choice'));
const art=read('generated/planet-site-pixels.h');
const mask=art.match(/ps_observatory_window_mask\[57120\]=\{([^}]+)\}/)[1].split(',').map(Number);
assert.equal(mask.length,57120);let windows=0;
mask.forEach((v,i)=>{assert(v===0||v===1);if(v){windows++;assert(i%340>=20&&i%340<276&&Math.floor(i/340)>=23&&Math.floor(i/340)<87);}});
assert(windows>5000);
console.log(`PASS: ${count} observatories across 1024 worlds, no duplicate per-world state; Lave 2 POI 3 and Lave 4 POI 5; ${windows} bounded window pixels; choice/save/Codex wiring.`);
console.log('LIMIT: generator-model and static integration checks; C runtime fixtures remain unexecuted.');
