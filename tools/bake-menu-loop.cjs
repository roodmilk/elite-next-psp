// Deterministic 480x272 film bake. No player saves, music or game state are read.
const fs=require('fs'),path=require('path'),crypto=require('crypto');
const runtime=process.env.CODEX_NODE_MODULES||'C:/Users/skarm/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/node_modules';
const {chromium}=require(path.join(runtime,'playwright'));
const root=path.resolve(__dirname,'..');
const source=process.argv[2]||path.join(root,'assets/source/intro-film/preview-browser.html');
const output=process.argv[3]||path.join(root,'assets/intro/cinematic.enm');
const executablePath=process.env.CHROMIUM_EXECUTABLE||'C:/Users/skarm/AppData/Local/ms-playwright/chromium_headless_shell-1228/chrome-headless-shell-win64/chrome-headless-shell.exe';
const FPS=30,COUNT=1440,LOOP=1200,HEADER=40,START=HEADER+(COUNT+1)*4;
const hashIndex=bytes=>{let h=2166136261;for(const b of bytes)h=Math.imul(h^b,16777619)>>>0;return h;};
(async()=>{
 const browser=await chromium.launch({headless:true,executablePath,args:['--use-angle=swiftshader','--enable-unsafe-swiftshader']});
 let fd;
 try{
  const page=await browser.newPage({viewport:{width:800,height:650},deviceScaleFactor:1});
  page.on('pageerror',e=>{throw e;});
  await page.goto('file:///'+path.resolve(source).replaceAll('\\','/'));
  const frame=page.frames().find(f=>f.parentFrame())||page;
  await frame.waitForFunction(()=>document.getElementById('elite-next-hd2d-study')?.introPreview?.state().ready);
  fs.mkdirSync(path.dirname(output),{recursive:true});fd=fs.openSync(output+'.part','w');
  fs.writeSync(fd,Buffer.alloc(START));
  let offset=START,max=0;const index=Buffer.alloc((COUNT+1)*4),proofs=[];
  const proofFrames=new Set([0,180,420,540,720,810,900,1110,1290]);
  for(let i=0;i<COUNT;i++){
   const jpeg=Buffer.from(await frame.evaluate(t=>document.getElementById('elite-next-hd2d-study').introPreview.renderFilm(t),i/FPS),'base64');
   if(jpeg.length>131072||jpeg[0]!==255||jpeg[1]!==216||jpeg.at(-2)!==255||jpeg.at(-1)!==217)throw Error('Invalid or oversized frame '+i);
   index.writeUInt32LE(offset,i*4);fs.writeSync(fd,jpeg);offset+=jpeg.length;max=Math.max(max,jpeg.length);
   if(proofFrames.has(i)){const file=path.join(path.dirname(output),`proof-${String(i).padStart(4,'0')}.jpg`);fs.writeFileSync(file,jpeg);proofs.push(path.basename(file));}
   if(i%60===0)console.log(JSON.stringify({frame:i,of:COUNT,bytes:offset,maxFrame:max}));
  }
  index.writeUInt32LE(offset,COUNT*4);
  const header=Buffer.alloc(HEADER);header.write('ENINTRO1',0,'ascii');
  [1,480,272,FPS,COUNT,LOOP,max,hashIndex(index)].forEach((v,i)=>header.writeUInt32LE(v,8+i*4));
  fs.writeSync(fd,header,0,HEADER,0);fs.writeSync(fd,index,0,index.length,HEADER);fs.closeSync(fd);fd=undefined;
  fs.renameSync(output+'.part',output);
  const metadata={format:'ENINTRO1',width:480,height:272,fps:FPS,frames:COUNT,loopStart:LOOP,maxFrameBytes:max,bytes:offset,sha256:crypto.createHash('sha256').update(fs.readFileSync(output)).digest('hex'),proofs};
  fs.writeFileSync(output+'.json',JSON.stringify(metadata,null,2));console.log(JSON.stringify(metadata));
 }finally{if(fd!==undefined)fs.closeSync(fd);await browser.close();}
})().catch(e=>{console.error(e);process.exitCode=1;});
