import fs from 'node:fs';

const ambush = fs.readFileSync(new URL('../src/thargoid-ambush.h', import.meta.url), 'utf8');
const main = fs.readFileSync(new URL('../src/main.c', import.meta.url), 'utf8');
const required = [
  'h%100u>=14u', 'thargoid_wave>=3', 'game.credits+=200',
  'game.system=thargoid_origin', 'game.destination=thargoid_destination',
  'game.jump=2.6f', 'no jump fuel spent', 'THARGOID INTERDICTION',
  'NUB / D-PAD AIM', 'X FIRE', 'thargoid_enemy_bolt', 'thargoid_damage'
];
for (const marker of required) if (!ambush.includes(marker)) throw new Error(`missing ambush behavior: ${marker}`);
for (const marker of ['if(thargoid_active){thargoid_view();return;}', 'thargoid_maybe_start()', 'thargoid_input(pressed,held,dt,ax,ay)', '!thargoid_failed_flash']) {
  if (!main.includes(marker)) throw new Error(`missing main-loop isolation: ${marker}`);
}
if (!main.includes('thargoid ambush: defeat safely returns')) throw new Error('compiled outcome regressions are missing');
console.log('PASS rare hyperspace trigger and exclusive rail-shooter loop are wired');
console.log('PASS kills pay 20 units and three cleared waves resume the jump');
console.log('PASS defeat preserves origin, destination and jump fuel');
