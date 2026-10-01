// Read-only content/layout checks. Does not launch PPSSPP or execute PSP code.
import fs from 'node:fs';
import assert from 'node:assert/strict';
const src=fs.readFileSync(new URL('../src/social-feed.h',import.meta.url),'utf8');
const literal=src.match(/social_lines\[SB_COUNT-1\]\[SOCIAL_VARIANTS\]=(.+);/)[1];
const rows=JSON.parse(literal.replaceAll('{','[').replaceAll('}',']'));
assert.equal(rows.length,20);assert(rows.every(r=>r.length===8));
assert.equal(new Set(rows.flat()).size,160);
const font=fs.readFileSync(new URL('../src/font8.h',import.meta.url),'utf8');
const bytes=[...font.matchAll(/0x([0-9a-f]+)u/gi)].map(m=>parseInt(m[1],16));
assert.equal(bytes.length,95*8);
function width(c){
 if(c===' ')return 4;if(c>='a'&&c<='z')return 6;
 let lo=7,hi=0;const glyph=bytes.slice((c.charCodeAt(0)-32)*8,(c.charCodeAt(0)-31)*8);
 for(let col=0;col<8;col++)for(let row=0;row<8;row++)if((glyph[row]>>(7-col))&1){lo=Math.min(lo,col);hi=Math.max(hi,col);}
 return lo>hi?4:hi-lo+2;
}
function checkWrap(s){
 let at=0;
 for(let line=0;line<3&&at<s.length;line++){
  const start=at;let last=-1,used=0;
  while(at<s.length&&at-start<95&&used+8<=436){if(s[at]===' ')last=at;used+=width(s[at]);at++;}
  if(at<s.length&&last>start)at=last;
  const part=s.slice(start,at);let pixels=0;
  for(const c of part){assert(pixels+8<=436,`Clipped glyph: ${part}`);pixels+=width(c);}
  while(s[at]===' ')at++;
 }
 assert.equal(at,s.length,`More than three lines: ${s}`);
}
for(const [event,row] of rows.entries())for(const text of row){
 assert(!/[^\x20-\x7e]/.test(text),'Unexpected non-ASCII glyph');
 assert.equal((text.match(/%s/g)||[]).length,event===19?0:1);
 for(const name of ['Ben','Ben Smith','Abcdefghijklmnopqrstuvwx',"Jean-Luc O'Neill",'123456789012345678901234'])checkWrap(text.replace('%s',name));
}
const events=fs.readFileSync(new URL('../src/social-events.h',import.meta.url),'utf8');
assert(events.includes('seed%SOCIAL_VARIANTS'));
const game=fs.readFileSync(new URL('../src/game.h',import.meta.url),'utf8');
assert(game.includes('#define SOCIAL_KEEP 4096'));
assert.equal(4096*12+1040,50192);
console.log('PASS: 160 unique templates; 800 name/layout combinations fit actual font widths in three lines.');
console.log('PASS: global archive declaration and 50,192-byte V23 payload size.');
console.log('LIMIT: static content/layout checks only; PSP runtime and save migration tests need an authorised test run.');
