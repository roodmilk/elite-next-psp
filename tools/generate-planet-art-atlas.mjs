#!/usr/bin/env node
/* Generate native-scale, dependency-free planet art reference tiles. */
import fs from 'node:fs';
import path from 'node:path';

const names=['ocean','desert','ice','volcanic','forest'];
const palettes=[[[26,38,62],[65,145,205],[188,219,227],[229,239,210]],[[42,29,25],[201,157,83],[230,190,112],[112,70,42]],[[27,38,58],[177,211,230],[225,244,249],[113,159,194]],[[48,23,24],[173,76,53],[224,120,61],[255,187,82]],[[22,40,32],[78,145,83],[133,190,94],[210,190,106]]];
function h(v){v>>>=0;v^=v>>>16;v=Math.imul(v,0x7feb352d)>>>0;v^=v>>>15;v=Math.imul(v,0x846ca68b)>>>0;return (v^(v>>>16))>>>0;}
const tileW=32,tileH=32,cols=10,rows=16,w=cols*tileW,height=rows*tileH,pixels=Buffer.alloc(w*height*3);
function px(x,y,c){if(x<0||y<0||x>=w||y>=height)return;const i=(y*w+x)*3;pixels[i]=c[0];pixels[i+1]=c[1];pixels[i+2]=c[2];}
function rect(x,y,rw,rh,c){for(let yy=y;yy<y+rh;yy++)for(let xx=x;xx<x+rw;xx++)px(xx,yy,c);}
function tile(family,style,palette){const tx=((family*32+style*8+palette)%cols)*tileW,ty=Math.floor((family*32+style*8+palette)/cols)*tileH,cs=palettes[family],seed=h((family+1)*911+style*65537+palette*131071),cx=tx+16,cy=ty+15,r=11+((seed>>>4)%4);rect(tx,ty,tileW,tileH,cs[0]);for(let y=-r;y<=r;y++)for(let x=-r;x<=r;x++)if(x*x+y*y<=r*r){let c=cs[1];if(family===0&&y>2)c=cs[2];else if(family===1&&y<((style&1)?-2:3))c=cs[2];else if(family===2&&(y<-4||y>5))c=cs[2];else if(family===3&&((x+y*3+seed)%11===0))c=cs[3];else if(family===4&&((x*7+y*3+seed)%13===0))c=cs[3];px(cx+x,cy+y,c);}if(family===0)for(let i=0;i<4;i++){const yy=cy-3+i*3;for(let x=-r+2;x<r-1;x++)px(cx+x,yy,cs[(i+style)%3+1]);}if(family===3)for(let i=0;i<3;i++){const x=cx-6+((seed>>>i*4)%13);for(let y=-3;y<6;y++)if(x*x+y*y<r*r)px(x,cy+y,cs[3]);}if(family===4)for(let i=0;i<4;i++){const x=cx-8+((seed>>>i*3)%17),y=cy+3-((seed>>>i*5)%8);px(x,y,cs[3]);}}
for(let family=0;family<5;family++)for(let style=0;style<4;style++)for(let palette=0;palette<8;palette++)tile(family,style,palette);
const out=process.argv[process.argv.indexOf('--out')+1];if(!out){console.error('Usage: node tools/generate-planet-art-atlas.mjs --out <directory>');process.exit(2);}fs.mkdirSync(out,{recursive:true});fs.writeFileSync(path.join(out,'planet-art-atlas.ppm'),Buffer.concat([Buffer.from(`P6\n${w} ${height}\n255\n`),pixels]));fs.writeFileSync(path.join(out,'planet-art-atlas.json'),JSON.stringify({atlas_version:1,tile:[tileW,tileH],grid:[cols,rows],families:names,variants:160},null,2)+'\n');console.log(`Generated 160 native-scale planet art reference tiles (${w}x${height} PPM)`);
