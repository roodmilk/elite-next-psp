"""Deterministic 480x272 Ship Loadout inventory concepts.

Text pixels are read directly from src/font8.h.  Nothing is scaled or
anti-aliased, so these captures use the same 8x8 glyphs as the PSP build.
"""
from pathlib import Path
import re
from PIL import Image, ImageDraw

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "design" / "mockups"
OUT.mkdir(parents=True, exist_ok=True)

source = (ROOT / "src" / "font8.h").read_text(encoding="utf-8")
rows = re.findall(r"\{([^{}]+)\}", source.split("font8[95][8]=", 1)[1])[:95]
FONT = [[int(v.rstrip("uU"), 16) for v in re.findall(r"0x[0-9A-Fa-f]+u?", row)] for row in rows]
assert len(FONT) == 95 and all(len(g) == 8 for g in FONT)

PANEL=(21,28,39); RAISED=(41,54,70); EDGE=(193,139,77); GOLD=(240,180,91)
CYAN=(85,212,212); TEXT=(229,210,163); MUTED=(155,154,165); BG=(4,9,18)
SIGNAL=(82,201,147); RED=(235,91,103); VOID=(8,14,24); LOCK=(15,22,31)

def canvas():
    im=Image.new("RGB",(480,272),BG); d=ImageDraw.Draw(im)
    # deterministic sparse star texture behind the instrument surface
    for i in range(52):
        x=(i*83+17)%480; y=23+(i*47)%225
        d.point((x,y),fill=(25,39,56) if i%7 else (72,92,104))
    return im,d

def rect(d,x,y,w,h,c,outline=None):
    d.rectangle((x,y,x+w-1,y+h-1),fill=c)
    if outline is not None:d.rectangle((x,y,x+w-1,y+h-1),outline=outline)

def line(d,x1,y1,x2,y2,c):d.line((x1,y1,x2,y2),fill=c)

def text(d,x,y,s,c=TEXT):
    for ch in str(s).upper():
        code=ord(ch)
        glyph=FONT[code-32] if 32<=code<=126 else FONT[ord('?')-32]
        for gy,bits in enumerate(glyph):
            for gx in range(8):
                if bits & (0x80>>gx): d.point((x+gx,y+gy),fill=c)
        x+=8

def fit(s,n):return str(s)[:n]

def panel(d,x,y,w,h,title=None,active=False):
    rect(d,x,y,w,h,PANEL,EDGE if active else RAISED)
    line(d,x+1,y+1,x+w-2,y+1,GOLD if active else (58,73,88))
    if title:text(d,x+8,y+6,title,GOLD if active else CYAN)

def header(d,subtitle="SHIP TECH BOARD / MODULE INVENTORY"):
    rect(d,0,0,480,22,PANEL);line(d,0,0,479,0,EDGE);line(d,0,21,479,21,CYAN)
    rect(d,0,0,3,22,GOLD);text(d,7,7,"ELITE: NEXT",GOLD);text(d,103,7,"/",MUTED);text(d,119,7,subtitle,TEXT)

def button(d,x,y,symbol,label,color):
    # Native-size symbolic PSP button, intentionally simple at final size.
    d.ellipse((x,y,x+10,y+10),outline=color)
    tx=x+3 if symbol not in ('X','O') else x+2
    text(d,tx,y+2,symbol,color);text(d,x+15,y+2,label,MUTED)

def footer(d,items):
    rect(d,0,248,480,24,PANEL);line(d,0,248,479,248,EDGE)
    x=12
    for symbol,label,color in items:
        button(d,x,255,symbol,label,color);x+=30+len(label)*8

def tabs(d,active=0,y=46):
    names=("WPN","DEF","NAV","HOLD","FUEL","UTIL")
    for i,name in enumerate(names):
        x=16+i*76;rect(d,x,y,68,18,RAISED if i==active else PANEL,GOLD if i==active else (58,73,88))
        text(d,x+22,y+5,name,TEXT if i==active else MUTED)

def banks(d,y=72,selected=1):
    values=("PLS","BEM","+","--","--","--","--","--")
    for i,v in enumerate(values):
        x=16+i*56;enabled=i<3;sel=i==selected
        rect(d,x,y,52,28,RAISED if sel else PANEL,GOLD if sel else ((58,73,88) if enabled else (32,40,49)))
        text(d,x+4,y+4,f"B{i+1}",MUTED if enabled else (65,72,80))
        text(d,x+25-(len(v)*4),y+15,v,TEXT if enabled else (65,72,80))
        if i==0:rect(d,x+4,y+25,44,2,SIGNAL)

def ship_art(d,cx,cy):
    shadow=(43,58,72);hull=(154,166,171)
    rect(d,cx-9,cy-36,18,72,hull);rect(d,cx-54,cy-8,108,16,hull)
    rect(d,cx-67,cy-2,134,5,GOLD);rect(d,cx-24,cy-19,48,38,shadow)
    rect(d,cx-5,cy-4,10,8,CYAN);rect(d,cx-2,cy-1,4,3,(205,231,210))
    line(d,cx-50,cy+19,cx-30,cy+19,shadow);line(d,cx+30,cy+19,cx+50,cy+19,shadow)

MODULES=(
    ("PULSE LASER","WPN","INSTALLED"),("BEAM LASER","WPN","STORED"),
    ("HEAT BUFFER","UTIL","STORED"),("PLANET SCANNER","NAV","STORED"),
    ("MILITARY SHIELD","DEF","STORED"),("CARGO BAY +8T","HOLD","STORED"),
    ("FUEL SCOOP","FUEL","STORED"),("ECM SYSTEM","UTIL","STORED"),
)

def list_rows(d,x,y,w,count=5,selected=1,show_state=True):
    for i,(name,cat,state) in enumerate(MODULES[:count]):
        yy=y+i*20;sel=i==selected
        if sel:rect(d,x,yy,w,18,RAISED);rect(d,x,yy,3,18,GOLD)
        text(d,x+8,yy+5,fit(name,14),GOLD if sel else TEXT)
        text(d,x+w-40,yy+5,cat,CYAN if sel else MUTED)
        if show_state and w>=210:text(d,x+w-112,yy+5,"FITTED" if state=="INSTALLED" else "STORED",SIGNAL if state=="INSTALLED" else MUTED)

def scrollbar(d,x,y,h,total,visible,pos):
    rect(d,x,y,3,h,(50,64,77));thumb=max(7,h*visible//total);off=(h-thumb)*pos//max(1,total-visible);rect(d,x,y+off,3,thumb,CYAN)

def detail(d,x,y,w,title="BEAM LASER"):
    text(d,x,y,title,GOLD);text(d,x,y+14,"WPN / STORED MODULE",CYAN)
    text(d,x,y+30,"A SUSTAINED ENERGY WEAPON WITH",TEXT)
    text(d,x,y+41,"IMPROVED RANGE AND SHIELD DAMAGE.",TEXT)
    text(d,x,y+57,"SWAP: PULSE LASER > BEAM LASER",SIGNAL)

def top_status(d):
    text(d,16,29,"COBRA MK 3",CYAN);text(d,296,29,"FITTED 2/14",MUTED);text(d,400,29,"1250.0 U",GOLD)

def option_split():
    im,d=canvas();header(d);top_status(d);tabs(d);banks(d)
    panel(d,8,106,166,136,"CURRENT SHIP");ship_art(d,91,170);text(d,24,224,"CARGO 2/20T",MUTED)
    panel(d,180,106,292,136,"MODULE LOCKER",True);text(d,350,112,"8 / 40",MUTED)
    list_rows(d,190,130,268,4,1);scrollbar(d,184,130,77,8,4,1)
    line(d,190,212,458,212,(58,73,88));text(d,190,220,"BEAM LASER",GOLD);text(d,318,220,"X SWAP WITH BANK 2",SIGNAL)
    footer(d,(("L","CATEGORY",TEXT),("X","SWAP",CYAN),("S","DETAILS",(255,114,213)),("O","BACK",RED)))
    return im

def option_workbench():
    im,d=canvas();header(d,"SHIP TECH BOARD / WORKBENCH");top_status(d);tabs(d);banks(d,72,0)
    panel(d,8,106,464,84,"SELECTED BANK",True);ship_art(d,99,151)
    text(d,180,127,"BANK 1 / PULSE LASER",GOLD);text(d,180,142,"ACTIVE WEAPON",CYAN);text(d,180,157,"SELECT A STORED WPN BELOW TO SWAP.",TEXT)
    panel(d,8,196,464,46,"STORED WPN  2/3");
    cards=(("BEAM","20 DMG","BOX"),("RAIL","110 DMG","BOX"),("PULSE","42 DMG","FIT"))
    for i,(name,stat,state) in enumerate(cards):
        x=20+i*148;sel=i==0;rect(d,x,211,138,24,RAISED if sel else LOCK,GOLD if sel else (58,73,88));text(d,x+7,215,name,GOLD if sel else TEXT);text(d,x+55,215,stat,CYAN);text(d,x+105,226,state,SIGNAL if state=="FIT" else MUTED)
    footer(d,(("L","CATEGORY",TEXT),("X","SWAP",CYAN),("T","LOCKER",SIGNAL),("O","BACK",RED)))
    return im

def option_manifest():
    im,d=canvas();header(d,"SHIP TECH BOARD / STORES MANIFEST");top_status(d)
    panel(d,8,46,190,196,"FITTED BANKS");
    cats=("WPN","DEF","NAV","HOLD","FUEL","UTIL")
    values=("PULSE LASER","EMPTY","PLANET SCAN","CARGO +8T","SCOOP","HEAT BUFFER")
    for i,(cat,val) in enumerate(zip(cats,values)):
        y=66+i*27;sel=i==0
        if sel:rect(d,17,y,172,23,RAISED);rect(d,17,y,3,23,GOLD)
        text(d,25,y+4,cat,CYAN if sel else MUTED);text(d,65,y+4,fit(val,15),GOLD if sel else TEXT)
        text(d,65,y+14,"BANK 1" if val!="EMPTY" else "OPEN",SIGNAL if val!="EMPTY" else MUTED)
    panel(d,206,46,266,196,"MODULE INVENTORY",True);text(d,374,52,"8 / 40",MUTED)
    list_rows(d,216,68,242,6,1,True);scrollbar(d,210,68,117,8,6,1)
    line(d,216,190,458,190,(58,73,88));text(d,216,198,"BEAM LASER",GOLD);text(d,216,211,"SWAP INTO WPN BANK 1",TEXT);text(d,216,225,"OLD MODULE RETURNS TO INVENTORY",SIGNAL)
    footer(d,(("U","SELECT",TEXT),("X","SWAP",CYAN),("S","SORT",(255,114,213)),("O","BACK",RED)))
    return im

for name,build in (("loadout-inventory-split.png",option_split),("loadout-inventory-workbench.png",option_workbench),("loadout-inventory-manifest.png",option_manifest)):
    image=build();assert image.size==(480,272);image.save(OUT/name,optimize=False)
print(OUT)
