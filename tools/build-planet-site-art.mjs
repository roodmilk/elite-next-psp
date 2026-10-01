// Native authored pixel kit. All drawing coordinates are on a 340x168 grid.
// No high-resolution source, resampling or external image libraries.
import fs from 'node:fs';
import zlib from 'node:zlib';
import assert from 'node:assert/strict';
const root=new URL('../',import.meta.url),W=340,H=168;
const palette=['101722','192635','263849','3b5261','647d87','94a5a5','d3ccb0','fff0c0','453532','6d4b39','9f7148','d6a45f','263e3c','3c6356','64896a','9ab582','123e51','196478','38a2b0','8cdbd5','542f3d','8a4955','bd7475','e3a693'];
const names=['cache','ruins','observatory','rescue','garden','fossil','thermal','drone','migration','crystal','archive','weather','spaceport','relay','biology','uplink'];
// Main feature, secondary equipment, records, occupant. Shared with runtime.
const anchors=[[18,40,128,99],[157,96,103,47],[224,145,48,17],[286,80,32,64]];
const plates=[];let pixels,clip=null,windowMask,backdrop=false,observatoryMask;
function dot(x,y,c){x=Math.round(x);y=Math.round(y);if(x>=0&&x<W&&y>=0&&y<H&&(!clip||(x>=clip[0]&&y>=clip[1]&&x<clip[2]&&y<clip[3]))){pixels[y*W+x]=c;if(windowMask)windowMask[y*W+x]=backdrop?1:0;}}
function rect(x,y,w,h,c){for(let yy=y;yy<y+h;yy++)for(let xx=x;xx<x+w;xx++)dot(xx,yy,c);}
function line(x0,y0,x1,y1,c){let n=Math.max(Math.abs(x1-x0),Math.abs(y1-y0));for(let i=0;i<=n;i++)dot(x0+(x1-x0)*i/(n||1),y0+(y1-y0)*i/(n||1),c);}
function poly(points,c){let ymin=Math.ceil(Math.min(...points.map(p=>p[1]))),ymax=Math.floor(Math.max(...points.map(p=>p[1])));for(let y=ymin;y<=ymax;y++){const xs=[];for(let i=0;i<points.length;i++){const a=points[i],b=points[(i+1)%points.length];if((a[1]<=y&&b[1]>y)||(b[1]<=y&&a[1]>y))xs.push(a[0]+(y-a[1])*(b[0]-a[0])/(b[1]-a[1]));}xs.sort((a,b)=>a-b);for(let i=0;i+1<xs.length;i+=2)for(let x=Math.ceil(xs[i]);x<=xs[i+1];x++)dot(x,y,c);}}
function ellipse(x,y,rx,ry,c){for(let j=-ry;j<=ry;j++)for(let i=-rx;i<=rx;i++)if(i*i/(rx*rx)+j*j/(ry*ry)<=1)dot(x+i,y+j,c);}
function box(x,y,w,h,c){rect(x,y,w,h,c);line(x,y,x+w-1,y,c+1);line(x,y,x,y+h-1,c+1);line(x+w-1,y,x+w-1,y+h-1,0);line(x,y+h-1,x+w-1,y+h-1,0);}
function monitor(x,y,w=26,h=17){box(x,y,w,h,2);rect(x+3,y+3,w-6,h-6,16);line(x+5,y+h-7,x+w-6,y+6,18);dot(x+w-5,y+4,19);}
function crate(x,y,w,h){box(x,y,w,h,9);rect(x+4,y+3,3,h-5,8);rect(x+w-8,y+3,3,h-5,8);rect(x+w/2-8,y+7,16,8,6);line(x+6,y+h-6,x+w-7,y+h-6,11);}
function plant(x,y,h){line(x,y,x,y-h,14);for(let j=4;j<h;j+=6){line(x,y-j,x-6,y-j-4,13);line(x,y-j-2,x+7,y-j-6,15);}}
function drawConsole(){poly([[160,101],[253,101],[258,118],[157,118]],3);box(158,118,101,22,2);monitor(166,102,36,16);monitor(207,102,23,16);for(let i=0;i<4;i++)rect(236+i*4,108,2,3,11);rect(166,128,48,3,0);rect(164,140,5,3,0);rect(249,140,5,3,0);}
function sceneBase(kind){const outdoor=[1,4,5,7,9].includes(kind);pixels=new Uint8Array(W*H);windowMask=new Uint8Array(W*H);backdrop=false;rect(0,0,W,H,1);
 if(!outdoor){rect(0,0,W,15,2);rect(0,15,12,118,3);rect(328,15,12,118,3);for(let x=18;x<330;x+=52){rect(x,17,2,112,2);rect(x+2,17,1,112,3);}rect(16,19,264,72,0);}
 let x0=outdoor?0:20,y0=outdoor?0:23,sw=outdoor?340:256,sh=outdoor?103:64;
 clip=[x0,y0,x0+sw,y0+sh];backdrop=true;
 rect(x0,y0,sw,sh,16);rect(x0,y0+sh/2,sw,sh/2,17);
 for(let x=x0;x<x0+sw;x+=38){let top=y0+20+((x*7+kind*11)%25);poly([[x, y0+sh],[x+19,top],[x+45,y0+sh]],2);line(x+19,top,x+31,top+18,4);}
 rect(x0,y0+sh-13,sw,13,12);for(let x=x0;x<x0+sw;x+=13)line(x,y0+sh-5,x+7,y0+sh-6,13);
 clip=null;backdrop=false;
 if(!outdoor){box(17,88,262,5,3);rect(137,20,4,68,2);rect(271,24,3,64,3);}
 rect(0,133,W,35,outdoor?8:1);for(let y=137;y<168;y+=9)line(0,y,W-1,y,outdoor?9:2);
 if(!outdoor)for(let x=-80;x<420;x+=56)line(170,100,x,167,2);
 // Occupant's chair remains visible when this site is unstaffed.
 box(291,112,23,25,2);rect(292,137,3,13,3);rect(311,137,3,13,3);box(291,101,23,16,8);
 // Separate readable field-record object; not painted into the desk.
 poly([[226,148],[249,145],[269,148],[269,161],[248,158],[226,161]],6);line(248,148,248,158,9);
 for(let y=151;y<158;y+=3){line(230,y,243,y-1,9);line(253,y-1,264,y,9);}
 drawConsole();
}
for(let k=0;k<names.length;k++){
 sceneBase(k);
 switch(k){
 case 0:crate(24,85,68,50);crate(94,105,43,30);crate(40,55,42,29);line(23,83,91,83,11);break;
 case 1:poly([[25,134],[30,55],[49,43],[71,50],[72,134]],3);poly([[78,134],[87,64],[114,58],[139,72],[139,134]],2);for(let y=67;y<121;y+=9){line(39,y,56,y,11);line(40,y,44,y+4,10);line(97,y+3,124,y+3,4);}break;
 case 2: // Native observatory architecture and mechanical telescope.
  for(let x=12;x<332;x+=42){poly([[x,0],[x+7,0],[x+22,18],[x+15,18]],3);dot(x+3,4,5);dot(x+14,14,5);}
  for(let x=22;x<276;x+=24){dot(x,19,5);dot(x,91,4);}
  box(282,22,39,45,8);rect(285,25,33,39,1);
  for(let j=0;j<11;j++){let x=288+(j*13)%26,y=28+(j*17)%31;dot(x,y,6);if(j%3==0)line(x,y,299,46,3);}
  rect(279,68,44,3,9);ellipse(299,19,10,3,11);rect(294,12,11,4,10);
  rect(9,47,5,22,8);rect(9,51,5,13,11);rect(10,54,3,7,7);
  poly([[24,139],[122,139],[139,149],[34,149]],3);line(34,150,139,150,0);
  for(let j=0;j<28;j++){let x=8+(j*47)%210,y=151+(j*11)%15;line(x,y,x+3,y,2);}
  line(83,103,45,136,4);line(83,103,121,136,3);rect(80,93,7,42,3);ellipse(83,100,15,9,2);
  poly([[32,94],[49,111],[133,61],[117,43]],4);poly([[32,94],[38,101],[126,51],[117,43]],5);line(57,81,73,97,2);line(94,59,111,77,2);
  ellipse(124,51,10,13,1);ellipse(125,50,6,9,18);line(122,45,126,42,19);rect(30,97,12,8,2);
  line(50,110,47,117,0);line(47,117,57,129,0);line(57,129,66,132,3);
  ellipse(84,101,8,6,4);ellipse(84,101,4,3,9);dot(83,99,11);
  line(41,90,52,100,6);line(73,71,86,82,3);line(76,70,89,80,5);
  for(let j=0;j<6;j++)dot(52+j*11,90-j*6,6);
  for(let j=0;j<7;j++){rect(167+j*9,121,4,3,j%2?10:4);dot(168+j*9,121,6);}
  rect(169,129,34,6,6);for(let j=0;j<5;j++)rect(171+j*6,131,3,1,8);
  ellipse(247,128,5,5,4);dot(249,125,6);rect(209,128,20,7,0);
  box(267,132,10,10,9);line(277,134,280,134,10);line(280,134,280,139,10);line(277,140,280,140,10);
  rect(318,89,3,50,3);line(319,89,308,79,4);ellipse(307,78,9,3,11);rect(302,79,10,2,7);
  break;
 case 3:box(36,81,59,52,9);rect(54,96,25,7,6);rect(63,87,7,26,6);line(102,130,102,52,4);ellipse(102,56,18,7,3);line(102,56,119,42,5);crate(111,113,29,23);break;
 case 4:for(let y=89;y<=121;y+=32){box(23,y,117,17,9);rect(27,y+3,109,7,8);for(let x=34;x<134;x+=18)plant(x,y+5,18+(x%9));}line(24,63,137,63,4);line(24,63,24,86,3);break;
 case 5:poly([[20,123],[38,70],[108,63],[144,108],[128,135],[38,138]],9);ellipse(79,102,39,20,10);line(47,100,118,105,6);for(let x=54;x<112;x+=9){line(x,102,x-3,90,6);line(x,102,x-4,116,6);}ellipse(119,101,9,7,6);dot(123,99,8);break;
 case 6:box(43,55,71,81,3);rect(52,63,54,58,1);for(let x=60;x<102;x+=13){rect(x,66,7,65,10);line(x,68,x,128,11);}ellipse(80,91,18,18,2);ellipse(80,91,13,13,6);line(80,91,86,81,20);rect(27,115,18,15,4);rect(112,78,27,11,4);break;
 case 7:ellipse(83,125,62,10,0);poly([[23,92],[60,102],[102,86],[143,112],[104,117],[62,129]],3);ellipse(80,103,27,19,4);ellipse(80,103,16,10,1);rect(70,99,19,5,18);line(33,91,24,68,5);line(128,109,140,129,3);line(68,110,91,91,0);break;
 case 8:box(25,68,113,59,8);rect(31,75,101,32,16);poly([[31,106],[53,87],[76,98],[105,80],[132,103]],13);rect(75,74,4,34,9);rect(22,113,118,8,10);line(96,120,84,135,4);ellipse(97,111,14,5,2);break;
 case 9:for(const [x,y,h] of [[30,133,53],[56,139,85],[89,139,64],[116,136,43]]){poly([[x,y],[x-8,y-h+12],[x,y-h],[x+13,y-h+18],[x+8,y]],18);poly([[x,y],[x,y-h],[x+13,y-h+18],[x+8,y]],17);line(x,y-h,x+4,y-h+12,19);}break;
 case 10:box(22,45,120,94,9);for(let y=51;y<134;y+=19){rect(27,y,109,15,0);for(let x=30;x<133;x+=13){rect(x,y+2,9,12,3+(x%3));rect(x+1,y+4,6,2,6);}}break;
 case 11:line(80,136,80,45,5);line(77,135,77,49,3);line(47,71,111,71,4);ellipse(47,66,13,4,3);ellipse(111,66,13,4,3);line(80,45,127,45,5);poly([[126,41],[138,45],[126,49]],11);box(51,100,58,35,2);monitor(57,106,43,22);break;
 case 12:box(23,96,119,42,9);rect(26,92,114,7,11);monitor(49,69,49,26);rect(70,94,8,4,3);crate(106,107,28,24);break;
 case 13:box(26,51,111,86,2);for(let x=36;x<132;x+=23){rect(x,59,14,59,3);rect(x+3,65,8,29,18);rect(x+4,98,6,9,11);}line(33,128,125,128,10);break;
 case 14:box(24,101,115,34,3);for(let x=36;x<128;x+=27){box(x,63,18,43,4);rect(x+3,66,12,36,16);plant(x+9,99,24);rect(x+2,58,14,5,3);}break;
 case 15:box(38,77,82,59,2);for(let y=84;y<130;y+=13){monitor(44,y,37,11);rect(86,y+3,25,3,4);}line(79,77,79,45,5);ellipse(79,50,36,10,3);line(79,50,100,40,19);break;
 }
 if(k==2)observatoryMask=windowMask;
 plates.push(pixels);
}
const output=new URL('assets/generated/planet-sites/',root);fs.mkdirSync(output,{recursive:true});
function crc(buf){let n=0xffffffff;for(const b of buf){n^=b;for(let j=0;j<8;j++)n=(n>>>1)^((n&1)?0xedb88320:0);}return (n^0xffffffff)>>>0;}
function chunk(type,data){const tag=Buffer.from(type),len=Buffer.alloc(4),sum=Buffer.alloc(4);len.writeUInt32BE(data.length);sum.writeUInt32BE(crc(Buffer.concat([tag,data])));return Buffer.concat([len,tag,data,sum]);}
function png(data,w,h,path){const head=Buffer.alloc(13);head.writeUInt32BE(w);head.writeUInt32BE(h,4);head[8]=8;head[9]=2;const raw=Buffer.alloc((w*3+1)*h);for(let y=0;y<h;y++)for(let x=0;x<w;x++){const c=palette[data[y*w+x]],p=y*(w*3+1)+1+x*3;for(let n=0;n<3;n++)raw[p+n]=parseInt(c.slice(n*2,n*2+2),16);}fs.writeFileSync(path,Buffer.concat([Buffer.from([137,80,78,71,13,10,26,10]),chunk('IHDR',head),chunk('IDAT',zlib.deflateSync(raw)),chunk('IEND',Buffer.alloc(0))]));}
let header='/* Generated from native 340x168 authored recipes; do not hand edit. */\nstatic const unsigned ps_palette[24]={'+palette.map(c=>'RGB('+[0,2,4].map(i=>parseInt(c.slice(i,i+2),16)).join(',')+')').join(',')+'};\n';
header+='static const unsigned short ps_anchor[4][4]={'+anchors.map(a=>'{'+a.join(',')+'}').join(',')+'};\n';
header+='static const unsigned char ps_plates[16][57120]={\n';
const atlas=new Uint8Array(W*4*H*4);
for(let i=0;i<plates.length;i++){const data=plates[i];assert.equal(data.length,W*H);assert(data.every(v=>v<palette.length));png(data,W,H,new URL(names[i]+'.png',output));header+='{/* '+names[i]+' */\n';for(let j=0;j<data.length;j+=80)header+=data.slice(j,j+80).join(',')+',\n';header+='},\n';for(let y=0;y<H;y++)atlas.set(data.slice(y*W,(y+1)*W),(Math.floor(i/4)*H+y)*W*4+i%4*W);}
header+='};\n';header+='static const unsigned char ps_observatory_window_mask[57120]={'+Array.from(observatoryMask).join(',')+'};\n';fs.writeFileSync(new URL('src/generated/planet-site-pixels.h',root),header);
png(atlas,W*4,H*4,new URL('contact-sheet.png',output));
for(const [x,y,w,h] of anchors)assert(x>=2&&y>=2&&x+w+2<=W&&y+h+2<=H);
console.log('PASS: 16 native 340x168 plates; shared bounded hotspots; 913920 indexed pixel bytes; no resampling.');
