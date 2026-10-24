import re
from pathlib import Path
from PIL import Image, ImageDraw

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT.parent / "erl-implementation-candidate" / "outputs" / "magnetic-slice" / "regression-2471-combined-4-diagnostic-erl-quick-race-review" / "quick-title-restored.bmp"
OUT = ROOT / "assets" / "concepts" / "main-menu" / "native"
OUT.mkdir(parents=True, exist_ok=True)

raw = (ROOT / "src" / "font8.h").read_text()
rows = re.findall(r"\{([^{}]+)\}", raw.split("font8[95][8]=", 1)[1])[:95]
FONT = [[int(v.rstrip("uU"), 16) for v in re.findall(r"0x[0-9A-Fa-f]+u?", row)] for row in rows]

NAVY = (4, 13, 25)
PANEL = (5, 19, 31)
PANEL2 = (8, 27, 39)
CYAN = (23, 228, 232)
CYAN_DARK = (18, 99, 112)
GOLD = (255, 195, 43)
WHITE = (218, 235, 232)
MUTED = (111, 139, 143)

def rect(d, box, fill, outline=None):
    d.rectangle(box, fill=fill, outline=outline)

def line(d, xy, fill):
    d.line(xy, fill=fill, width=1)

def text(im, x, y, value, colour=WHITE, scale=1):
    px = im.load()
    for ch in value.upper():
        glyph = FONT[ord(ch) - 32] if 32 <= ord(ch) <= 126 else FONT[31]
        for yy, bits in enumerate(glyph):
            for xx in range(8):
                if bits & (1 << (7 - xx)):
                    for sy in range(scale):
                        for sx in range(scale):
                            tx, ty = x + xx * scale + sx, y + yy * scale + sy
                            if 0 <= tx < 480 and 0 <= ty < 272:
                                px[tx, ty] = colour
        x += 8 * scale

def clear_menu(im):
    # The background remains pixel-for-pixel identical above the current menu.
    d = ImageDraw.Draw(im)
    rect(d, (0, 143, 479, 271), NAVY)
    # Preserve the existing footer scale and give the menu its own quiet deck.
    rect(d, (0, 250, 479, 271), (3, 12, 23))
    line(d, (0, 249, 479, 249), CYAN_DARK)
    return d

def footer(im, hint):
    text(im, 8, 258, "D-PAD")
    text(im, 56, 258, "X OPEN", CYAN)
    text(im, 128, 258, "SQUARE MULTIPLAYER", WHITE)
    text(im, 296, 258, hint, MUTED)

def portrait_from(source):
    return source.crop((272, 162, 321, 211)).resize((48, 48), Image.Resampling.NEAREST)

def concept_a(source):
    im = source.copy(); d = clear_menu(im)
    # Refined version of the current split layout: stronger hierarchy and less empty framing.
    rect(d, (16, 149, 257, 241), PANEL, CYAN)
    rect(d, (17, 150, 256, 171), PANEL2)
    rect(d, (18, 151, 21, 170), GOLD)
    line(d, (22, 171, 255, 171), CYAN_DARK)
    items = ["NEW COMMANDER", "LOAD COMMANDER", "EXTRAS"]
    for i, item in enumerate(items):
        y = 157 + i * 22
        text(im, 29, y, ("> " if i == 0 else "  ") + item, GOLD if i == 0 else WHITE)
    rect(d, (265, 149, 463, 241), PANEL, CYAN)
    im.paste(portrait_from(source), (275, 159))
    text(im, 333, 157, "NEW PILOT", GOLD)
    text(im, 333, 174, "EARTH / 2050", CYAN)
    text(im, 333, 191, "FIRST FLIGHT")
    line(d, (275, 215, 453, 215), CYAN_DARK)
    text(im, 275, 224, "NO COMMANDER SAVE", MUTED)
    footer(im, "TRIANGLE REPLAY INTRO")
    return im

def concept_b(source):
    im = source.copy(); d = clear_menu(im)
    # One unified command console; selection and profile read as one object.
    rect(d, (24, 151, 455, 241), PANEL, CYAN)
    rect(d, (25, 152, 454, 171), PANEL2)
    text(im, 34, 158, "COMMAND DECK", GOLD)
    text(im, 334, 158, "EARTH / 2050", CYAN)
    line(d, (251, 172, 251, 239), CYAN_DARK)
    items = ["NEW COMMANDER", "LOAD COMMANDER", "EXTRAS"]
    for i, item in enumerate(items):
        y = 181 + i * 18
        if i == 0:
            rect(d, (31, y - 4, 243, y + 11), PANEL2)
            rect(d, (31, y - 4, 34, y + 11), GOLD)
        text(im, 42, y, ("> " if i == 0 else "  ") + item, GOLD if i == 0 else WHITE)
    im.paste(portrait_from(source), (265, 181))
    text(im, 322, 181, "NEW PILOT", GOLD)
    text(im, 322, 198, "CHOOSE A SHIP")
    text(im, 322, 215, "FIRST FLIGHT", CYAN)
    footer(im, "SELECT PROFILE")
    return im

def concept_c(source):
    im = source.copy(); d = clear_menu(im)
    # Compact floating cards reveal more of the existing background at the sides.
    rect(d, (12, 154, 222, 240), PANEL, CYAN)
    text(im, 22, 160, "START", CYAN)
    line(d, (22, 172, 211, 172), CYAN_DARK)
    items = ["NEW COMMANDER", "LOAD COMMANDER", "EXTRAS"]
    for i, item in enumerate(items):
        y = 181 + i * 17
        if i == 0:
            rect(d, (18, y - 3, 215, y + 11), PANEL2)
            text(im, 23, y, ">", GOLD)
        text(im, 39, y, item, GOLD if i == 0 else WHITE)
    rect(d, (271, 154, 467, 240), PANEL, CYAN)
    text(im, 281, 160, "COMMANDER PREVIEW", CYAN)
    line(d, (281, 172, 456, 172), CYAN_DARK)
    im.paste(portrait_from(source), (281, 180))
    text(im, 339, 181, "NEW PILOT", GOLD)
    text(im, 339, 198, "NO SAVE DATA", MUTED)
    text(im, 339, 215, "FIRST FLIGHT")
    footer(im, "EXTRAS: TUTORIAL / ERL")
    return im

source = Image.open(SOURCE).convert("RGB")
for name, fn in (("a-refined-split", concept_a), ("b-command-console", concept_b), ("c-floating-cards", concept_c)):
    out = fn(source)
    assert out.size == (480, 272)
    out.save(OUT / f"{name}-480x272.png")
