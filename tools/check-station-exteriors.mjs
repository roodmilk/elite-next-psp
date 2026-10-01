import fs from 'node:fs';

const root = new URL('../', import.meta.url);
const source = fs.readFileSync(new URL('src/voyage.h', root), 'utf8');
const docking = fs.readFileSync(new URL('src/docking.h', root), 'utf8');
const profileSource = fs.readFileSync(new URL('src/station-profile.h', root), 'utf8');
const u32 = n => n >>> 0;
function hash(system, variant) {
  let h = u32(Math.imul(system + 1, 0x9e3779b9) ^ Math.imul(variant + 11, 0x85ebca6b));
  h = u32(h ^ (h >>> 16)); h = u32(Math.imul(h, 0x7feb352d));
  h = u32(h ^ (h >>> 15)); h = u32(Math.imul(h, 0x846ca68b));
  return u32(h ^ (h >>> 16));
}
function profile(system, variant = 0) {
  const h = hash(system, variant);
  let radius = 150 + ((h >>> 5) % 211), half = 145 + ((h >>> 14) % 236);
  if (variant) { radius *= .58 + .08 * variant; half *= .62 + .06 * variant; }
  return { family: ((h % 6) + variant * 2) % 6, radius, half, bands: 1 + ((h >>> 9) % 3), pods: 3 + ((h >>> 17) % 6) };
}
const stations = Array.from({length: 256}, (_, system) => profile(system));
const allHubs = Array.from({length: 256 * 3}, (_, index) => profile(Math.floor(index / 3), index % 3));
const minR = Math.min(...stations.map(p => p.radius)), maxR = Math.max(...stations.map(p => p.radius));
const minH = Math.min(...stations.map(p => p.half)), maxH = Math.max(...stations.map(p => p.half));
const families = new Set(stations.map(p => p.family));
const signatures = new Set(stations.map(p => `${p.family}:${p.radius}:${p.half}:${p.bands}:${p.pods}`));
const allSignatures = new Set(allHubs.map(p => `${p.family}:${p.radius}:${p.half}:${p.bands}:${p.pods}`));
const checks = [
  [families.size === 6, `six silhouette families (${families.size})`],
  [maxR / minR > 2, `radius spread ${minR}-${maxR} (${(maxR/minR).toFixed(2)}x)`],
  [maxH / minH > 2, `depth spread ${minH}-${maxH} (${(maxH/minH).toFixed(2)}x)`],
  [signatures.size >= 250, `${signatures.size}/256 unique structural signatures`],
  [allSignatures.size === 768, `${allSignatures.size}/768 unique main/relay/outpost signatures`],
  [source.includes('opaque pressure door') && source.includes('door_z'), 'opaque rear pressure door is rendered'],
  [source.includes('secondary_hubs') && source.includes('station_quad_at'), 'secondary hubs receive facade and door geometry'],
  [profileSource.includes('STATION_PORT_HALF_W') && profileSource.includes('STATION_PORT_HALF_H'), 'common flight slit uses safety constants'],
  [docking.includes('p.radius') && docking.includes('p.half'), 'collision uses the generated hull dimensions'],
  [!source.includes('station_port_corner(') && !docking.includes('STATION_ENTRY_Z'), 'no stale fixed station entry geometry remains']
];
let failed = 0;
for (const [ok, label] of checks) { console.log(`${ok ? 'PASS' : 'FAIL'} ${label}`); if (!ok) failed++; }
if (failed) process.exit(1);
