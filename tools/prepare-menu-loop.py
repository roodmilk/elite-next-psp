from pathlib import Path
p=Path('assets/source/intro-film/intro.html');s=p.read_text();s=s.replace('const start=36.4+j*3.3,alpha=span(start,start+.8,t);','const start=36.4+j*3.3,alpha=span(start,start+.8,t)*(1-span(51,56,t));')
s=s.replace('const velocity=.018+(j%5)*.004,phase=(t*velocity+j*.173)%1;','const velocity=(1+j%3)/12,phase=(((t-56.5)*velocity+j*.173)%1+1)%1;')
s=s.replace('const passage=((t-44)/27)%1;','const passage=(((t-56.5)/12)%1+1)%1;')
s=s.replace("if(t>=44)submit('ship',world(-.48+passage*.9,-.065+passage*.025,.15+passage*.12),.027,span(44,45,t),-.08,21);","if(t>=44)submit('ship',world(-.95+passage*1.9,-.065+passage*.025,.22),.022,span(44,45,t),-.08,21);\n      if(t>=56){for(let j=0;j<2;j++){const q=(((t-56.5)/12+j*.5)%1+1)%1;submit('freighter',world(-1.2+q*2.4,.04+j*.035,.65+j*.2),.09,span(56,56.5,t),0,30+j);}}")
s=s.replace('gl.uniform1f(loc.time,elapsed+idle);','gl.uniform1f(loc.time,elapsed+idle<56.5?elapsed+idle:56.5+Math.sin((elapsed+idle-56.5)*Math.PI/6)*.15);')
Path('assets/source/intro-film/intro-loop.html').write_text(s)
p=Path('tools/bake-cinematic-intro.cjs');s=p.read_text().replace('const frame=page.frames().find(f=>f.parentFrame());','const frame=page.frames().find(f=>f.parentFrame())||page;');Path('tools/bake-menu-loop.cjs').write_text(s)
