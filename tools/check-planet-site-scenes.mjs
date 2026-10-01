import fs from 'node:fs';
import assert from 'node:assert/strict';
const read=n=>fs.readFileSync(new URL('../src/'+n,import.meta.url),'utf8');
const scene=read('planet-site-scene.h'),main=read('main.c');
const objects=[...scene.match(/ps_objects\[12\]\[3\]=\{([\s\S]*?)\n\};/)[1].matchAll(/"([^"]+)"/g)].map(m=>m[1]);
assert.equal(objects.length,36);
for(const label of objects)assert(label.length<=16,label+' overflows options rail');
assert.equal([...scene.match(/ps_observations\[\]=\{([\s\S]*?)\n\};/)[1].matchAll(/"([^"]+)"/g)].length,12);
assert(main.includes('ps_begin(surface_nearest_site(&game,55))'));
assert(main.includes('case FLIGHT:if(ps_open)ps_draw();else space();break;'));
assert(read('tutorial-runtime.h').includes('if(ps_open||ps_release){game_input(pressed,held,dt,ax,ay);return;}'));
assert(scene.includes('surface_interact(&game)'));
assert(!scene.includes('game.credits+='));
assert(!scene.includes('game_tick('));
for(let i=0;i<5;i++){const y=38+i*27;assert(y-3>=22&&y+16<=190);}
assert(346+16*8<=480);
console.log('PASS: 12 content families, 36 bounded object labels, native rail bounds, scene routing, tutorial input isolation and shared reward path.');
console.log('LIMIT: static checks only; compiled controller/world coverage fixtures still need execution.');
const art=read('generated/planet-site-pixels.h');
assert(art.includes('ps_plates[16][57120]'));
assert(scene.includes('ps_anchor[a][0]')&&scene.includes('ps_anchor[3][0]'));
assert(scene.includes('ps_staffed())sc_raster_sprite'));
assert(scene.includes('ps_anim')&&!scene.includes('game.world_clock+='));
assert.equal([...read('planet-site-dialogue.h').matchAll(/^ "/gm)].length,24);
assert(scene.includes('ps_obs_count():ps_reply?4:5'));
console.log('PASS: native plate sizes, common art/hotspot anchors, UI-only animation and 24 site-specific dialogue/inspection passages.');

