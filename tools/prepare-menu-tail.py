from pathlib import Path
p=Path('assets/source/intro-film/intro-loop.html');s=p.read_text().replace('const space=span(35,41,t),drift=t+idle;','const space=span(35,41,t),drift=t+idle<56.5?t+idle:56.5+Math.sin((t+idle-56.5)*Math.PI/6)*.4;');p.write_text(s)
p=Path('tools/bake-menu-loop.cjs');s=p.read_text();s=s.replace('for(let i=0;i<COUNT;i++){','''const existing=fs.readFileSync(path.join(root,'assets/intro/cinematic.enm'));
  for(let i=0;i<LOOP;i++){
   const begin=existing.readUInt32LE(40+i*4),end=existing.readUInt32LE(44+i*4),jpeg=existing.subarray(begin,end);
   index.writeUInt32LE(offset,i*4);fs.writeSync(fd,jpeg);offset+=jpeg.length;max=Math.max(max,jpeg.length);
  }
  for(let i=LOOP;i<COUNT;i++){''');Path('tools/bake-menu-tail.cjs').write_text(s)
