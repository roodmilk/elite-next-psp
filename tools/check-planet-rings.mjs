import fs from 'node:fs';
const root = new URL('../', import.meta.url);
const fx = fs.readFileSync(new URL('src/flight-extras.h', root), 'utf8');
const main = fs.readFileSync(new URL('src/main.c', root), 'utf8');
const departure = fs.readFileSync(new URL('src/station-departure-view.h', root), 'utf8');
const checks = [
 [fx.includes('static void celestial_rings(int foreground)'), 'ring renderer has explicit depth phase'],
 [fx.includes('(sinf(mid)>=0)!=(foreground!=0)'), 'ring segments split into rear and foreground halves'],
 [fx.includes('1.82f') && fx.includes('*.08f'), 'ring radius starts above 1.8 planet radii and varies by seed'],
 [fx.includes('for(int band=0;band<4;band++)'), 'ring is a visible four-line band'],
 [fx.includes('cosf(game.roll)') && fx.includes('sinf(game.roll)'), 'ring follows cockpit roll'],
 [main.includes('celestial_rings(0);celestial_rims();draw_bodies();celestial_rings(1)'), 'normal and warp flight use rear/body/front painter order'],
 [departure.includes('celestial_rings(0);celestial_rims();draw_bodies();celestial_rings(1)'), 'departure view uses the same painter order']
];
let failed=0;for(const [ok,label] of checks){console.log(`${ok?'PASS':'FAIL'} ${label}`);if(!ok)failed++;}
if(failed)process.exit(1);
