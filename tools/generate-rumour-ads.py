"""Authored 16x16 advert illustrations. Integer pixels, no scaled stock art."""
from pathlib import Path

def canvas(): return [['.']*16 for _ in range(16)]
g=canvas()
def dot(x,y,c):
    if 0<=x<16 and 0<=y<16:g[y][x]=c
def box(x,y,w,h,c):
    for yy in range(y,y+h):
        for xx in range(x,x+w):dot(xx,yy,c)
def line(x,y,u,v,c):
    n=max(abs(u-x),abs(v-y))
    for i in range(n+1):dot(round(x+(u-x)*i/max(1,n)),round(y+(v-y)*i/max(1,n)),c)
def disc(x,y,r,c):
    for yy in range(y-r,y+r+1):
        for xx in range(x-r,x+r+1):
            if (xx-x)**2+(yy-y)**2<=r*r:dot(xx,yy,c)
def star(x,y,c):
    line(x-1,y,x+1,y,c);line(x,y-1,x,y+1,c)
def eyes(x,y):
    box(x,y,2,2,'W');box(x+4,y,2,2,'W');dot(x+1,y+1,'#');dot(x+4,y+1,'#')
images=[]
def save():
    global g
    images.append([''.join(row) for row in g]);g=canvas()

# 01 Rental moon: shaded craters and a tiny estate-agent flag.
disc(7,8,6,'#');disc(7,8,5,'B');disc(5,6,2,'W');disc(9,10,2,'A');dot(6,11,'A');dot(10,6,'A');line(11,1,11,7,'#');box(12,1,3,3,'A');save()
# 02 Missing sock: ribbed ankle, heel, toe and a separate dangling partner.
box(3,2,5,9,'#');box(4,3,3,7,'B');box(3,10,9,4,'#');box(4,10,7,3,'B');box(4,4,3,1,'W');box(4,6,3,1,'W');box(9,11,2,2,'A');star(12,4,'A');save()
# 03 Warranty: actual ship over a stamped, folded certificate.
box(2,1,11,14,'#');box(3,2,9,12,'W');line(4,10,10,10,'B');line(4,12,8,12,'B');box(10,2,2,2,'A');line(7,3,4,8,'#');line(7,3,10,8,'#');line(4,8,10,8,'#');box(7,5,1,2,'B');disc(11,12,2,'A');dot(11,12,'W');save()
# 04 Water sachet: torn top, drop symbol and comically empty packet.
box(3,2,10,12,'#');box(4,3,8,10,'W');line(3,2,12,2,'A');box(4,11,8,2,'B');dot(8,4,'B');box(7,5,3,2,'B');disc(8,8,2,'B');dot(7,7,'W');save()
# 05 Vacuum cleaner with hose, wheels and a cheerful dust bag.
disc(6,12,2,'#');disc(11,12,2,'#');box(3,6,11,6,'#');box(4,7,9,4,'B');box(6,8,5,2,'W');line(4,6,2,3,'#');line(2,3,5,1,'#');line(5,1,11,2,'#');line(11,2,13,5,'#');box(12,4,3,2,'A');save()
# 06 Cat pilot: ears, whiskers, goggles and a little flight jacket.
box(3,2,3,4,'#');box(10,2,3,4,'#');disc(8,7,6,'#');disc(8,7,5,'B');dot(4,3,'A');dot(11,3,'A');eyes(5,6);dot(8,9,'A');line(4,9,1,8,'#');line(12,9,15,8,'#');box(4,12,9,3,'A');box(7,12,2,3,'W');save()
# 07 Inflatable pilot: big helmet, glossy seam and a valve.
disc(8,5,4,'#');disc(8,5,3,'W');box(6,4,5,3,'B');box(4,8,9,6,'#');box(5,9,7,5,'A');box(1,9,3,3,'W');box(13,9,2,3,'W');box(6,14,2,2,'#');box(10,14,2,2,'#');dot(11,9,'W');save()
# 08 Black-hole parking: luminous swirl in a marked containment bay.
box(1,2,14,12,'#');box(2,3,12,10,'W');disc(8,8,5,'A');disc(8,8,3,'B');disc(8,8,2,'#');dot(5,4,'W');dot(11,10,'W');box(2,12,3,1,'A');box(11,12,3,1,'A');save()
# 09 Alien tooth: three roots, crown and one spectacular gold filling.
disc(5,5,3,'#');disc(10,5,3,'#');box(3,5,10,5,'#');box(4,4,8,5,'W');box(4,9,2,4,'#');box(7,9,2,5,'#');box(10,9,2,4,'#');box(4,8,2,3,'W');box(7,8,2,4,'W');box(10,8,2,3,'W');box(9,4,2,2,'L');star(13,2,'A');save()
# 10 Speed stripes: swept fighter, contrasted racing stripes and exhaust.
for y in range(2,12):
    half=min(6,(y-1)//2);box(8-half,y,half*2+1,1,'#')
line(8,3,3,10,'B');line(9,5,6,10,'W');line(10,7,9,11,'A');box(6,12,2,2,'L');box(10,12,2,2,'L');star(2,3,'A');save()
# 11 Tiny pine air freshener, on a visible hanging string.
line(8,0,8,3,'#');box(7,12,3,3,'#')
for y,w in ((3,1),(4,3),(5,5),(7,5),(8,7),(9,9),(11,9),(12,11)):
    box(8-w//2,y,w,1,'#' if y in (3,5,9,12) else 'B')
dot(7,7,'W');dot(9,10,'A');save()
# 12 Pocket sun: rays, a warm disc, and completely irresponsible sunglasses.
for u,v in ((8,0),(8,15),(0,8),(15,8),(2,2),(13,2),(2,13),(13,13)):line(8,8,u,v,'A')
disc(8,8,5,'#');disc(8,8,4,'L');box(4,6,9,2,'#');dot(5,8,'#');dot(11,8,'#');line(6,11,10,11,'A');save()
# 13 Dating whale: flukes, white underside, a friendly eye and a pink heart.
disc(7,9,5,'#');box(2,6,8,7,'#');box(3,7,7,5,'B');box(3,11,7,1,'W');line(10,9,14,7,'#');line(14,7,15,5,'B');line(14,7,15,9,'B');dot(4,8,'W');dot(4,9,'#');box(10,1,2,2,'A');box(13,1,2,2,'A');box(11,2,3,3,'A');dot(12,5,'A');save()
# 14 Anti-gravity toast: toast levitating over a toaster and rising crumbs.
box(2,9,12,6,'#');box(3,10,10,4,'B');box(4,10,7,1,'#');dot(12,12,'A');box(5,1,6,6,'#');box(6,2,4,4,'L');box(7,3,2,2,'A');dot(3,4,'A');dot(12,6,'A');line(4,7,4,5,'W');save()
# 15 Pet asteroid: craggy rock, googly eyes, collar and leash.
disc(7,9,5,'#');box(4,4,6,8,'#');disc(7,9,4,'B');box(5,5,4,2,'B');eyes(4,7);box(4,11,7,1,'A');dot(8,13,'L');line(11,11,14,9,'#');line(14,9,14,2,'#');line(14,2,12,1,'#');save()
# 16 Hot singles: three stars, one with a flirty face.
star(3,3,'A');star(12,4,'L');star(4,12,'L');disc(9,9,4,'A');disc(9,9,3,'L');dot(8,8,'#');line(10,8,11,8,'#');line(8,11,10,11,'#');save()
# 17 Folded escape pod: origami creases, hatch and distress beacon.
line(8,1,1,11,'#');line(8,1,14,11,'#');box(2,11,12,3,'#')
for y in range(3,11):box(max(2,8-(y-1)//2),y,min(11,2*((y-1)//2)+1),1,'B')
line(8,2,8,12,'W');line(2,11,8,7,'W');line(8,7,13,11,'W');box(7,5,3,3,'A');dot(8,0,'L');save()
# 18 Tea: lidded cup, clear handle, saucer and three escaped steam pixels.
box(2,5,9,7,'#');box(3,6,7,5,'W');box(11,6,3,5,'#');box(11,7,2,3,'.');box(1,4,11,2,'B');box(5,3,3,1,'#');line(1,13,13,13,'#');box(4,9,4,1,'A');dot(4,1,'A');dot(9,0,'A');dot(12,2,'A');save()
# 19 Freighter ghost: scalloped hem, surprised face and a crossed-out sign.
disc(8,6,5,'#');box(3,6,11,7,'#');disc(8,6,4,'W');box(4,6,9,6,'W');box(4,12,2,2,'W');box(8,12,2,2,'W');box(12,12,1,2,'W');eyes(5,5);box(7,9,3,2,'#');line(1,14,14,1,'A');save()
# 20 Potted captain: plant, enormous commander's cap and a flowerpot.
box(4,11,8,4,'#');box(5,12,6,2,'A');line(8,6,8,11,'#');box(4,7,4,2,'B');box(9,5,4,2,'B');disc(8,5,3,'B');box(4,2,9,3,'#');box(5,2,7,2,'B');box(8,2,2,1,'L');line(4,5,12,5,'#');save()
# 21 Premium nothing: open gift box with deliberately empty interior.
box(3,7,10,8,'#');box(4,8,8,6,'B');box(7,8,2,6,'A');line(3,7,1,4,'#');line(1,4,6,3,'#');line(6,3,8,7,'#');line(8,7,11,3,'#');line(11,3,15,5,'#');line(15,5,13,8,'#');box(4,5,2,2,'W');box(10,5,2,2,'W');star(8,1,'A');save()
# 22 Foil hat: angular folds and bright specular glints.
for y in range(2,12):
    half=(y-1)//2;box(8-half,y,half*2+1,1,'#');box(8-half+1,y,max(1,half*2-1),1,'B')
line(8,3,6,10,'W');line(9,5,11,10,'A');box(1,12,14,2,'#');box(2,12,12,1,'W');star(13,3,'A');save()
# 23 Reverse tractor: a horseshoe magnet and arrows pointing away.
box(4,3,3,9,'#');box(10,3,3,9,'#');box(4,10,9,4,'#');box(5,4,1,7,'A');box(11,4,1,7,'B');box(6,11,5,2,'B');box(5,3,1,3,'W');box(11,3,1,3,'W');line(2,2,2,6,'A');line(2,2,0,4,'A');line(2,2,4,4,'A');line(14,2,14,6,'B');line(14,2,12,4,'B');save()
# 24 Nebula jar: glass outline, lid, bright cloud and scattered stars.
box(4,1,8,2,'#');box(5,2,6,1,'A');box(3,4,10,11,'#');box(4,5,8,9,'W');disc(7,10,3,'B');disc(9,8,2,'A');dot(5,6,'B');dot(10,12,'A');dot(5,9,'L');dot(9,6,'L');line(4,5,4,11,'W');save()

assert len(images)==24 and len({tuple(im) for im in images})==24
assert all(len(row)==16 and set(row)<=set('.#BALW') for im in images for row in im)
target=Path(__file__).resolve().parents[1]/'src/rumour-ads-art.h'
out=['/* Authored native 16x16 illustrations; tools/generate-rumour-ads.py. */',
     'static const char rumour_ad_art[24][16][17]={']
for i,im in enumerate(images):
    out.append(' /* Advert %02d */'%(i+1))
    out.append(' {'+','.join('"'+row+'"' for row in im)+'},')
out.append('};')
target.write_text('\n'.join(out)+'\n',encoding='utf-8')
print('Wrote 24 distinct authored pixel illustrations:',target)
