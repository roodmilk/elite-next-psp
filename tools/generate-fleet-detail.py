"""Original panel-fit fleet geometry. Gameplay/collision meshes stay untouched.

Keep the individual Elite silhouettes and Needlefish asymmetry; build physical
recessed armour, raised glazing and engine collars on those actual surfaces.
One small shared functional atlas plus hull-specific insignia/armour tiles,
instead of 44 full 26KiB Adder texture banks or render-time asset decoding.
"""
from pathlib import Path
import json, re, math
from PIL import Image, ImageDraw

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT/'outputs/fleet465'
OUT.mkdir(exist_ok=True)
source = (ROOT/'src/ships.c').read_text()
needle = (ROOT/'src/needlefish-mesh.h').read_text()
names = re.findall(r'\{"([^"]+)",\s*\d+,\s*\d+,\s*(?:v\d+|needlefish_v),', source)
atlas, atlas_names = [], []
pal = [(12,20,28),(35,46,58),(156,174,185),(104,128,147),
       (210,222,219),(201,146,67),(28,40,53),(12,40,55),
       (29,83,105),(178,233,242),(66,216,225),(240,150,58),
       (73,51,39),(104,123,142),(144,161,171),(67,89,110)]

def tile(kind, variant=0):
    im=Image.new('P',(32,32),2); d=ImageDraw.Draw(im)
    d.rectangle((0,0,31,31),outline=1); d.line((2,2,29,2),fill=4)
    if kind=='frame':
        d.rectangle((0,0,31,31),fill=15);d.line((0,0,31,0),fill=4)
    elif kind in ('armour','ceramic','alien'):
        if kind=='ceramic': d.rectangle((2,2,29,29),fill=4)
        # Large clean fitted plates, a deliberately placed paint stripe and
        # distinct hull-address crest. Avoid surface-wide random freckles.
        x=5+variant%4*3
        d.polygon([(x,3),(x+3,3),(x+11,28),(x+8,28)],fill=3 if kind!='alien' else 8)
        d.line((3,24,22,24),fill=1);d.line((3,25,22,25),fill=4)
        for k in range(3):
            if (variant+1)&(1<<k):d.rectangle((22+k*2,7,22+k*2,11+k),fill=5 if kind!='alien' else 10)
        d.line((5,6,9,6),fill=13);d.point((5,7),fill=4)
        if kind=='alien':
            for y in (7,15,23):d.line((2,y,15,y+5,29,y),fill=10)
    elif kind=='glass':
        d.rectangle((0,0,31,31),fill=6);d.polygon([(3,3),(28,3),(27,28),(4,28)],fill=7)
        d.polygon([(5,5),(25,5),(24,25),(7,25)],fill=8)
        d.polygon([(7,6),(11,6),(24,21),(21,23)],fill=9)
        d.line((16,3,16,28),fill=6);d.line((17,3,17,28),fill=3)
    elif kind in ('radiator','engine'):
        d.rectangle((0,0,31,31),fill=6)
        if kind=='engine':
            for rad,col in ((15,3),(13,5),(11,6),(8,12),(5,11),(2,9)):
                d.regular_polygon((15.5,15.5,rad),8,fill=col)
        else:
            for y in range(4,29,4):
                d.line((4,y,27,y),fill=15);d.line((4,y+1,27,y+1),fill=1)
            d.line((3,3,3,28),fill=5)
    elif kind in ('cargo','service','windows','garden','solar','hangar','antenna','tank'):
        d.rectangle((3,4,28,27),fill=3);d.line((4,5,27,5),fill=4)
        if kind=='cargo':
            for x in range(6,28,5):d.line((x,7,x,24),fill=1);d.line((x+1,7,x+1,24),fill=4)
            d.rectangle((12,20,20,23),fill=5)
        elif kind=='windows':
            for y in (8,15,22):
                d.rectangle((5,y,26,y+3),fill=6)
                for x in range(7,26,5):d.rectangle((x,y+1,x+2,y+2),fill=11 if variant&1 else 10)
        elif kind=='garden':
            d.rectangle((5,6,26,25),fill=7)
            for y in (8,13,18,23):d.line((7,y,24,y),fill=8);d.line((9,y,20,y),fill=10)
            d.line((15,6,15,25),fill=4)
        elif kind=='solar':
            d.rectangle((4,5,27,26),fill=7)
            for x in range(5,28,5):d.line((x,5,x,26),fill=15)
            for y in range(6,27,5):d.line((4,y,27,y),fill=8)
        elif kind=='hangar':
            d.rectangle((6,7,25,25),fill=6);d.line((7,8,24,8),fill=10)
            for x in range(8,26,5):d.line((x,26,x+2,24),fill=5)
        elif kind=='antenna':
            for rad in (10,7,4):d.ellipse((16-rad,16-rad,16+rad,16+rad),outline=4)
            d.line((5,16,27,16),fill=15);d.line((16,5,16,27),fill=15);d.point((16,16),fill=10)
        elif kind=='tank':
            for y in (8,22):d.line((5,y,26,y),fill=1);d.line((5,y+1,26,y+1),fill=4)
            d.rectangle((13,11,19,18),fill=6);d.line((14,12,18,12),fill=10)
        else:
            d.rectangle((6,8,14,23),fill=6)
            for y in (10,14,18):d.line((7,y,13,y),fill=15)
            d.rectangle((18,9,25,18),fill=1);d.line((19,10,24,10),fill=4)
            d.point((20,15),fill=10);d.line((18,23,25,23),fill=5)
        for x,y in ((4,5),(27,5),(4,26),(27,26)):d.point((x,y),fill=14)
    atlas_names.append(f'{kind}-{variant}')
    data=[]
    # Lower mip levels use dominant clusters, retaining authored pixel shapes.
    for size in (32,16,8):
        small=im.resize((size,size),Image.Resampling.NEAREST)
        pix=list(small.get_flattened_data())
        data.extend(pix[i]|pix[i+1]<<4 for i in range(0,len(pix),2))
    atlas.append(data)
    return len(atlas)-1

types={k:tile(k) for k in ('frame','armour','ceramic','alien','glass','radiator','engine','cargo','service','windows','garden','solar','hangar','antenna','tank')}
def sub(a,b):return [x-y for x,y in zip(a,b)]
def cross(a,b):return [a[1]*b[2]-a[2]*b[1],a[2]*b[0]-a[0]*b[2],a[0]*b[1]-a[1]*b[0]]
def unit(v):
    l=math.sqrt(sum(x*x for x in v));return [x/max(l,1e-9) for x in v]
def ff(x):return f'{x:.7f}f'
def vec(v):return '{'+','.join(ff(x) for x in v)+'}'
allfaces=[]; models=[]; report=[]; h=['/* Generated by tools/generate-fleet-detail.py. Visuals only; immutable compact atlas. */']
for idx,name in enumerate(names):
    if idx in (0,1,2,10,11,25):continue
    if idx==4:continue # Exact approved Adder and its physical artwork retained.
    text=needle if idx==34 else source
    vn='needlefish_v' if idx==34 else f'v{idx}'
    tn='needlefish_t' if idx==34 else f't{idx}'
    raw=re.search(rf'{vn}\[\]\s*=\s*\{{(.*?)\}};',text).group(1)
    vertices=[[float(x.rstrip('f')) for x in g.split(',')] for g in re.findall(r'\{([^{}]+)\}',raw)]
    raw=re.search(rf'{tn}\[\]\s*=\s*\{{(.*?)\}};',text).group(1)
    old=[([int(a),int(b),int(c)],[float(x.rstrip('f')) for x in n.split(',')]) for a,b,c,n in re.findall(r'\{(\d+),(\d+),(\d+),\{([^}]+)\}\}',raw)]
    radius=max(math.sqrt(sum(x*x for x in p)) for p in vertices)
    armour=tile('alien' if idx==16 else 'ceramic' if idx==32 else 'armour',idx)
    tris=[]; face=[]; medium=[]
    def emit(t,mat,normal=None):
        n=normal or unit(cross(sub(vertices[t[1]],vertices[t[0]]),sub(vertices[t[2]],vertices[t[0]])))
        if sum(x*x for x in n)<.1:return
        tris.append((t,n));face.append(mat)
    for number,(t,n) in enumerate(old):
        points=[vertices[v] for v in t];centre=[sum(p[k] for p in points)/3 for k in range(3)]
        inset=[]
        for p in points:
            inset.append(len(vertices));vertices.append([centre[k]+(p[k]-centre[k])*.83+n[k]*radius*.006 for k in range(3)])
        # Six narrow bevel faces around one continuous physical armour plate.
        for e in range(3):
            a,b=t[e],t[(e+1)%3];c,d=inset[(e+1)%3],inset[e]
            emit([a,b,c],types['frame']);emit([a,c,d],types['frame'])
        mat=armour if n[1]>.2 else types['radiator'] if n[1]<-.45 else types['cargo'] if idx in (19,20,21,29) else types['service']
        emit(inset,mat,n)
        medium.append((list(t),list(n),len(tris)-1))
    hull_end=len(tris)
    # The forward upper pressure panel carries a physically raised canopy.
    # Its footprint is fitted to that hull's own triangle, not a shared plane.
    if idx!=16:
        candidates=[(sum(vertices[v][2] for v in t)/3,t,n) for t,n in old if n[1]>.35]
        if candidates:
            _,t,n=max(candidates,key=lambda x:x[0])
            centre=[sum(vertices[v][k] for v in t)/3 for k in range(3)]
            lower=[];upper=[]
            for v in t:
                p=vertices[v];q=[centre[k]+(p[k]-centre[k])*.45+n[k]*radius*.012 for k in range(3)]
                lower.append(len(vertices));vertices.append(q)
                upper.append(len(vertices));vertices.append([centre[k]+(p[k]-centre[k])*.30+n[k]*radius*.075 for k in range(3)])
            emit(upper,types['glass'],n)
            for e in range(3):
                a,b=lower[e],lower[(e+1)%3];c,d=upper[(e+1)%3],upper[e]
                emit([a,b,c],types['glass']);emit([a,c,d],types['glass'])
    # Stern collars are mapped onto real hull-ray intersections. Runtime player
    # and NPC wakes read precisely these same outlet positions.
    engines=[]; engine_medium=[]; engine_start=len(tris)
    width=max(abs(v[0]) for v in vertices);ymid=(min(v[1] for v in vertices)+max(v[1] for v in vertices))*.5
    for side in ((1,) if idx==34 else (-1,1)):
        if idx==34:x,y,z=40.5,0,-66
        else:
            hits=[];x=side*width*.32;y=ymid
            for t,n in old:
                a,b,c=[vertices[v] for v in t];den=(b[1]-c[1])*(a[0]-c[0])+(c[0]-b[0])*(a[1]-c[1])
                if abs(den)<1e-6:continue
                u=((b[1]-c[1])*(x-c[0])+(c[0]-b[0])*(y-c[1]))/den
                v=((c[1]-a[1])*(x-c[0])+(a[0]-c[0])*(y-c[1]))/den
                if min(u,v,1-u-v)>=-.0001:hits.append(u*a[2]+v*b[2]+(1-u-v)*c[2])
            z=min(hits) if hits else min(v[2] for v in vertices)
        rad=min(radius*.058,max(1.5,width*.12));rings=[]
        for depth,r in ((z+rad*.12,rad),(z-rad*.65,rad*.85)):
            rings.append(list(range(len(vertices),len(vertices)+8)))
            vertices.extend([[x+math.cos(i*math.tau/8)*r,y+math.sin(i*math.tau/8)*r,depth] for i in range(8)])
        engine_face_start=len(tris)
        for k in range(8):
            a,b=rings[0][k],rings[0][(k+1)%8];c,d=rings[1][(k+1)%8],rings[1][k]
            emit([a,c,b],types['radiator']);emit([a,d,c],types['radiator'])
        centre=len(vertices);vertices.append([x,y,z-rad*.65])
        for k in range(8):emit([centre,rings[1][(k+1)%8],rings[1][k]],types['engine'],[0,0,-1])
        shaft=len(vertices);vertices.append([x,y,z+rad*.12])
        corners=[rings[1][k] for k in (0,2,4,6)]
        for k in range(4):
            t=[shaft,corners[k],corners[(k+1)%4]];n=unit(cross(sub(vertices[t[1]],vertices[t[0]]),sub(vertices[t[2]],vertices[t[0]])))
            engine_medium.append((t,n,engine_face_start))
        for t in ([corners[0],corners[3],corners[2]],[corners[0],corners[2],corners[1]]):
            coords=[max(0,min(31,round(15.5+(vertices[v][k]-(x if k==0 else y))*15.5/(rad*.85)))) for v in t for k in (0,1)]
            engine_medium.append((t,[0,0,-1],coords))
        engines.append([x,y,z-rad*.65-.1])
    offset=len(allfaces)
    for (t,n),mat in zip(tris,face):
        points=[vertices[v] for v in t];dom=max(range(3),key=lambda k:abs(n[k]));axes=[k for k in range(3) if k!=dom]
        lo=[min(v[k] for v in points) for k in axes];hi=[max(v[k] for v in points) for k in axes]
        uv=[max(0,min(31,round((vertices[v][k]-lo[j])*31/max(.0001,hi[j]-lo[j])))) for v in t for j,k in enumerate(axes)]
        if mat==types['engine']:
            centre=min(engines,key=lambda e:abs(e[0]-sum(v[0] for v in points)/3))
            rad=min(radius*.058,max(1.5,width*.12))*.85
            uv=[max(0,min(31,round(15.5+(vertices[v][k]-centre[k])*15.5/rad))) for v in t for k in (0,1)]
        allfaces.append((mat,uv))
    for k,(t,n,f) in enumerate(engine_medium):
        if isinstance(f,list):
            engine_medium[k]=(t,n,len(allfaces)-offset);allfaces.append((types['engine'],f))
    assert len(vertices)<=256 and len(tris)<=400
    symbol=f'fleet_{idx}'
    h.append(f'static const Vec3 {symbol}_v[]={{'+','.join(vec(v) for v in vertices)+'};')
    h.append(f'static const MeshTri {symbol}_t[]={{'+','.join('{'+','.join(map(str,t))+','+vec(n)+'}' for t,n in tris)+'};')
    h.append(f'static const Mesh {symbol}={{"{name} PANEL HULL",{len(vertices)},{len(tris)},{symbol}_v,{symbol}_t}};')
    # At modest screen sizes physical armour seams become subpixel geometry.
    # Keep the full original closed hull, fitted art, raised glass and actual
    # engine collars; compact away only the tiny armour bevel strips.
    medium.extend((list(t),list(n),i) for i,(t,n) in enumerate(tris) if hull_end<=i<engine_start)
    medium.extend(engine_medium)
    used=sorted({v for t,n,f in medium for v in t});remap={v:i for i,v in enumerate(used)}
    h.append(f'static const Vec3 {symbol}_mid_v[]={{'+','.join(vec(vertices[v]) for v in used)+'};')
    h.append(f'static const MeshTri {symbol}_mid_t[]={{'+','.join('{'+','.join(str(remap[v]) for v in t)+','+vec(n)+'}' for t,n,f in medium)+'};')
    h.append(f'static const unsigned short {symbol}_mid_face[]={{'+','.join(str(f) for t,n,f in medium)+'};')
    h.append(f'static const Mesh {symbol}_mid={{"{name} FITTED HULL",{len(used)},{len(medium)},{symbol}_mid_v,{symbol}_mid_t}};')
    actual=max(math.sqrt(sum(x*x for x in p)) for p in vertices)
    models.append((idx,symbol,offset,actual,engines))
    report.append(dict(name=name,mesh_id=idx,vertices=len(vertices),triangles=len(tris),medium_vertices=len(used),medium_triangles=len(medium),radius=actual,engines=engines,face_offset=offset))
h.append('typedef struct {const Mesh*mesh;int face;float radius;int engines;Vec3 outlet[2];const Mesh*medium;const unsigned short*medium_face;} FleetHull;')
h.append('static const FleetHull fleet_hulls[35]={')
for idx,symbol,off,rad,engines in models:
    ep=engines if len(engines)==2 else engines*2
    h.append(f'[{idx}]={{&{symbol},{off},{ff(rad)},{len(engines)},{{'+','.join(vec(e) for e in ep)+f'}},&{symbol}_mid,{symbol}_mid_face'+ '},')
h.append('};')
(ROOT/'src/fleet-detail-mesh.h').write_text('\n'.join(h)+'\n')
(ROOT/'src/fleet-detail-faces.h').write_text('/* Immutable fitted panel addresses; material >=8192. */\n'
    +'typedef struct {unsigned char tile,uv[6];} FleetFace;\nstatic const FleetFace fleet_faces[]={'
    +','.join('{'+str(mat)+',{'+','.join(map(str,uv))+'}}' for mat,uv in allfaces)+'};\n')
(ROOT/'src/fleet-detail-atlas.h').write_text('/* Shared functional pixel art and individual hull paint crests; 4bpp + mips. */\n'
    +'static const unsigned char fleet_tiles[][672]={' + ','.join('{'+','.join(map(str,d))+'}' for d in atlas)+'};\n'
    +'enum {'+','.join('FLEET_'+name.upper()+'='+str(i) for name,i in types.items())+'};\n')
(OUT/'fleet-geometry.json').write_text(json.dumps(report,indent=2))
(OUT/'atlas-plan.json').write_text(json.dumps(atlas_names,indent=2))
sheet=Image.new('RGB',(8*96,math.ceil(len(atlas)/8)*110),(10,18,25));sd=ImageDraw.Draw(sheet)
for i,data in enumerate(atlas):
    im=Image.new('RGB',(32,32));im.putdata([pal[(data[k//2]>>((k&1)*4))&15] for k in range(1024)])
    x=i%8*96;y=i//8*110;sheet.paste(im.resize((96,96),Image.Resampling.NEAREST),(x,y+14));sd.text((x+2,y),atlas_names[i],fill=(148,220,231))
sheet.save(OUT/'fitted-panel-atlas.png')
print(f'{len(models)} upgraded standard hulls + preserved Adder; {len(allfaces)} mapped faces; {len(atlas)*672} immutable atlas bytes; no decode/cache allocation.')
