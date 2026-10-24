"""Authored asymmetric Needlefish solid mesh; no borrowed hull or runtime generation."""
from pathlib import Path
from math import sqrt

vertices=[]
triangles=[]
def body(points,faces):
    offset=len(vertices)
    vertices.extend(points)
    centre=tuple(sum(p[k] for p in points)/len(points) for k in range(3))
    for face in faces:
        for n in range(1,len(face)-1):
            ids=[face[0],face[n],face[n+1]]
            a,b,c=[points[i] for i in ids]
            u=[b[k]-a[k] for k in range(3)]
            v=[c[k]-a[k] for k in range(3)]
            normal=[u[1]*v[2]-u[2]*v[1],u[2]*v[0]-u[0]*v[2],u[0]*v[1]-u[1]*v[0]]
            fc=tuple((a[k]+b[k]+c[k])/3 for k in range(3))
            if sum(normal[k]*(fc[k]-centre[k]) for k in range(3))<0:
                ids[1],ids[2]=ids[2],ids[1]
                normal=[-x for x in normal]
            length=sqrt(sum(x*x for x in normal))
            assert length>0
            triangles.append((tuple(i+offset for i in ids),tuple(x/length for x in normal)))

# Main long blade, port tip extends forward. Central pilot pod anchors flight centre.
body([(-12,0,120),(6,0,92),(18,0,-48),(-20,0,-63),(-30,0,12),(-7,12,25),(-7,-7,25)],
     [(0,1,5),(1,2,5),(2,3,5),(3,4,5),(4,0,5),(1,0,6),(2,1,6),(3,2,6),(4,3,6),(0,4,6)])
# Starboard offset engine nacelle, mounted with a swept pylon.
body([(10,2,14),(42,2,-26),(44,2,-42),(12,2,-25),(11,-4,14),(42,-4,-26),(44,-4,-42),(12,-4,-25)],
     [(0,1,2,3),(4,7,6,5),(0,4,5,1),(1,5,6,2),(2,6,7,3),(3,7,4,0)])
body([(30,8,-22),(51,8,-30),(51,8,-66),(30,8,-66),(30,-8,-22),(51,-8,-30),(51,-8,-66),(30,-8,-66)],
     [(0,1,2,3),(4,7,6,5),(0,4,5,1),(1,5,6,2),(2,6,7,3),(3,7,4,0)])
# Small cockpit ridge and dorsal fin keep a deliberate silhouette at 20 pixels.
body([(-10,10,48),(2,10,35),(3,10,9),(-12,10,9),(-7,19,25)],
     [(0,1,4),(1,2,4),(2,3,4),(3,0,4),(3,2,1,0)])
body([(-16,2,-34),(-13,24,-52),(-9,2,-60),(-12,2,-34),(-9,24,-52),(-5,2,-60)],
     [(0,1,2),(3,5,4),(0,3,4,1),(1,4,5,2),(2,5,3,0)])

def vec(v):return '{'+','.join(f'{x:.6f}f' for x in v)+'}'
text='/* Authored by tools/generate-needlefish.py. Offset engine ends at z=-66. */\n'
text+='static const Vec3 needlefish_v[]={'+','.join(vec(v) for v in vertices)+'};\n'
text+='static const MeshTri needlefish_t[]={'+','.join('{'+','.join(str(i) for i in ids)+','+vec(n)+'}' for ids,n in triangles)+'};\n'
Path('src/needlefish-mesh.h').write_text(text,encoding='utf-8')
print(f'Needlefish: {len(vertices)} vertices, {len(triangles)} triangles')
