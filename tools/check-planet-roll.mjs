// Headless mathematical checks only; no emulator or gameplay launch.
import assert from 'node:assert/strict';
import fs from 'node:fs';
const root=new URL('../',import.meta.url);
const art=fs.readFileSync(new URL('src/art-runtime.h',root),'utf8');
const main=fs.readFileSync(new URL('src/main.c',root),'utf8');
assert(main.includes('b->type,game.roll,xt,yt,xb,yb)'));
assert(art.includes('du=c*step,dv=-s*step'));
assert(art.includes('((left-cx)*c+(py-cy)*s)*step'));
assert(art.includes('(-(left-cx)*s+(py-cy)*c)*step'));
let samples=0;
for(const roll of [0,Math.PI/4,Math.PI/2,Math.PI,-Math.PI/2,2*Math.PI,14.2]){
 const c=Math.cos(roll),s=Math.sin(roll);
 // Actual camera basis at yaw=pitch=0, projected with screen Y inverted.
 for(const [wx,wy] of [[12,0],[0,12],[-7,19]]){
  const screenX=wx*c+wy*s,screenY=wx*s-wy*c;
  assert(Math.abs(c*screenX+s*screenY-wx)<1e-10);
  assert(Math.abs(-s*screenX+c*screenY+wy)<1e-10);
 }
 for(const r of [1,17,240,700])for(const cx of [-100,240,530]){
  const cy=110,extent=Math.ceil(r*(Math.abs(c)+Math.abs(s)));
  const left=Math.max(0,cx-extent),right=Math.min(480,cx+extent+1);
  const top=Math.max(24,cy-extent),bottom=Math.min(190,cy+extent+1);
  const step=30/r;
  for(let y=top;y<bottom;y++){
   let u=32+((left-cx)*c+(y-cy)*s)*step;
   let v=32+(-(left-cx)*s+(y-cy)*c)*step;
   for(let x=left;x<right;x++,u+=c*step,v-=s*step){
    // Independent inverse via polar coordinates, including clipped close-ups.
    const a=Math.atan2(y-cy,x-cx)-roll,d=Math.hypot(x-cx,y-cy);
    assert(Math.abs(u-(32+Math.cos(a)*d*step))<1e-8);
    assert(Math.abs(v-(32+Math.sin(a)*d*step))<1e-8);
    if(u>=2&&u<62&&v>=2&&v<62){
     const sx=Math.trunc(u),sy=Math.trunc(v);
     assert(sx>=0&&sx<64&&sy>=0&&sy<64);samples++;
    }
   }
  }
 }
}
console.log(`PASS: camera/sprite orientation, full turns, both directions and clipped sampling (${samples} visible samples).`);
console.log('LIMIT: mathematical/source checks; PSP rendering/performance still need runtime verification.');
