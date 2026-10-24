from pathlib import Path
p=Path('tools/generate-encounter-upgrades.py');s=p.read_text();s=s.replace('base=len(re.findall',"font_text=(root/'src/font8.h').read_text();glyphs=[[int(x.rstrip('uU'),16) for x in re.findall(r'0x[0-9a-fA-F]+u?',row)] for row in re.findall(r'\\{([^{}]+)\\}',font_text.split('font8[95][8]=',1)[1])[:95]]\nbase=len(re.findall")
s=s.replace('fill=2,outline=4','fill=2,outline=1')
s=s.replace('  arr=list(im.getdata());data=[]','''  # Functional microdetail is designed around the main panel, not random noise.
  # Recessed gasket, offset bolts, protected copper bus and inspection fasteners.
  d.line((2,3,2,28),fill=0);d.line((29,3,29,28),fill=0)
  for x,y in ((3,3),(27,3),(3,27),(27,27)):
   d.rectangle((x,y,x+1,y+1),fill=3);d.point((x,y),fill=5)
  for y in (12,20):
   d.rectangle((3,y,4,y+3),fill=0);d.point((4,y),fill=10)
  if role==0:
   label=['RLY','AIR','BIO','HAB','NOW','OFF','SRV'][family]
   d.rectangle((4,2,27,9),fill=0)
   for n,ch in enumerate(label):
    for yy,bits in enumerate(glyphs[ord(ch)-32]):
     for xx in range(8):
      if bits&(1<<(7-xx)):d.point((4+n*8+xx,2+yy),fill=5 if family in (1,5,6) else 10)
  elif role==1:
   d.line((6,26,25,26),fill=5);d.line((6,27,25,27),fill=0)
   for x in (7,13,19,25):d.point((x,25),fill=4)
  elif role==2:
   for x in (8,16,24):d.rectangle((x,13,x+2,19),fill=0);d.line((x+1,14,x+1,17),fill=10)
  else:
   d.rectangle((20,3,26,9),fill=2);d.rectangle((22,4,25,7),fill=8);d.point((23,5),fill=10)
   d.line((5,25,17,25),fill=5);d.line((5,26,17,26),fill=0)
  arr=list(im.getdata());data=[]''')
p.write_text(s)
