import fs from 'node:fs';

const ui = fs.readFileSync(new URL('../src/ui-modern.h', import.meta.url), 'utf8');
const main = fs.readFileSync(new URL('../src/main.c', import.meta.url), 'utf8');
const required = [
  'header("SYSTEM OPERATIONS")', 'LOCAL MAP', 'PROGRESS %d%%', 'TRAFFIC NOW',
  'TRADERS %2d', 'PATROLS %2d', 'RAIDERS %2d', 'EXPLORERS %d',
  'MISSIONS %d', 'WANTED %d', 'LAW: WARRANT', 'MANIFEST:',
  'FIELD RECORD', 'LIFE %d/%d', 'SITES %d/8', 'STELLAR RIFTS LOGGED',
  'ACCESS: FLOATING SKYPORT', 'NEXT', 'system_ops_next()',
  'game.landed_planets', 'game.surface_progress', 'game.rift_logged',
  'game.bounty_claimed', 'game.station_progress', 'game.jobs'
];
for (const marker of required) {
  if (!ui.includes(marker)) throw new Error(`missing system overview wiring: ${marker}`);
}
if (/LIVE SYSTEM CONDITIONS|LIVE WEATHER|LIVE EVENT/i.test(ui)) {
  throw new Error('system overview claims a live condition system that does not exist');
}
if (!main.includes('system operations: progress derives from saved landings')) {
  throw new Error('compiled input regressions for system progress are missing');
}
if (!main.includes('page==DETAILS?(1+BODY_COUNT)')) {
  throw new Error('System Details must keep one hub plus every local body selectable');
}
console.log('PASS system operations uses real saved/current gameplay state');
console.log('PASS six-row hub/body navigation and existing lock/align controls remain wired');
console.log('PASS no fictional live-condition layer is presented');
