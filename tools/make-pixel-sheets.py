#!/usr/bin/env python3
"""Author native colored sprite sheets and bake them to an ARGB1555 header.

The in-game 8x8 masks stay in src/space-animation-kit.h. This script renders
those CURRENT masks for review, then writes a PROPOSED colored sheet:
plume, beacon, lamp, crate (16x16 x 4) and courier (24x16 x 4).
"""
from __future__ import annotations

import re
from pathlib import Path

from PIL import Image, ImageDraw

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "assets" / "preview" / "pixel-sheets"
HEADER = ROOT / "src" / "pixel-sprite-sheet.h"
MASKS = ROOT / "src" / "space-animation-kit.h"

PAL = {
    ".": None,
    "n": (8, 13, 24),
    "c": (21, 28, 39),
    "s": (41, 54, 70),
    "o": (90, 96, 76),
    "r": (139, 75, 55),
    "h": (193, 139, 77),
    "e": (229, 210, 163),
    "v": (155, 154, 165),
    "y": (85, 212, 212),
    "a": (240, 180, 91),
    "d": (200, 90, 75),
    "w": (255, 244, 216),
}


def parse_masks() -> list[list[list[int]]]:
    text = MASKS.read_text(encoding="utf-8")
    rows = [int(n) for n in re.findall(r"\d", text.split("static const", 1)[1].split("};", 1)[0]) if n in "01"]
    # The regex above is too greedy across comments. Parse brace groups instead.
    body = text.split("static const unsigned char space_anim_masks", 1)[1]
    body = body.split("};", 1)[0]
    families = []
    for fam in re.findall(r"\{([^{}]*)\}", body):
        bits = [int(x) for x in re.findall(r"[01]", fam)]
        if len(bits) != 64:
            continue
        families.append([bits[i : i + 8] for i in range(0, 64, 8)])
    grouped = [families[i : i + 4] for i in range(0, len(families), 4)]
    return grouped


def paint(frame: list[str], width: int, height: int = 16) -> Image.Image:
    im = Image.new("RGBA", (width, height), (0, 0, 0, 0))
    px = im.load()
    for y, row in enumerate(frame[:height]):
        row = row.ljust(width, ".")[:width]
        for x, ch in enumerate(row):
            rgb = PAL[ch]
            if rgb:
                px[x, y] = (*rgb, 255)
    return im


def sheet(cells: list[tuple[str, list[Image.Image]]], scale: int, label: str) -> Image.Image:
    pad = 8 * scale
    label_h = 14 * scale
    gap = 6 * scale
    width = pad
    height = pad + label_h
    rows = []
    for name, frames in cells:
        fw, fh = frames[0].size
        row_w = pad + len(frames) * (fw * scale + gap)
        width = max(width, row_w + pad)
        rows.append((name, frames, fw, fh))
        height += label_h + fh * scale + gap
    canvas = Image.new("RGB", (width, height), (8, 13, 24))
    draw = ImageDraw.Draw(canvas)
    draw.text((pad, 4), label, fill=(240, 180, 91))
    y = pad + 8
    for name, frames, fw, fh in rows:
        draw.text((pad, y), name, fill=(85, 212, 212))
        y += label_h
        x = pad
        for fr in frames:
            big = fr.resize((fw * scale, fh * scale), Image.NEAREST)
            canvas.paste(big, (x, y), big)
            x += fw * scale + gap
        y += fh * scale + gap
    return canvas


def argb1555(im: Image.Image) -> list[int]:
    vals = []
    px = im.load()
    for y in range(im.height):
        for x in range(im.width):
            r, g, b, a = px[x, y]
            vals.append(0 if a < 128 else 0x8000 | ((r >> 3) << 10) | ((g >> 3) << 5) | (b >> 3))
    return vals


PLUME = [
    [
        "................",
        ".......ww.......",
        "......waaw......",
        "......ahhha.....",
        ".....ahhhha.....",
        ".....whhhhw.....",
        "......rrrra.....",
        "......drrrd.....",
        ".......dd.......",
        "................",
        "................",
        "................",
        "................",
        "................",
        "................",
        "................",
    ],
    [
        "................",
        ".......ww.......",
        "......waaw......",
        ".....aahhaa.....",
        ".....ahhhha.....",
        "....awhhhwwa....",
        ".....arrrra.....",
        "......drrd......",
        "......dd.dd.....",
        ".......d.d......",
        "................",
        "................",
        "................",
        "................",
        "................",
        "................",
    ],
    [
        "................",
        ".......ee.......",
        "......waaw......",
        ".....aahhha.....",
        "....aahhhhaa....",
        "....awhhhhwa....",
        ".....ahrrha.....",
        ".....ddrddd.....",
        "......d.d.d.....",
        ".......d.d......",
        "................",
        "................",
        "................",
        "................",
        "................",
        "................",
    ],
    [
        "................",
        ".......ww.......",
        "......eaee......",
        "......ahhha.....",
        ".....aahhha.....",
        ".....whhhhw.....",
        "......rrrra.....",
        ".....ddrrdd.....",
        "......d...d.....",
        ".......d.d......",
        "................",
        "................",
        "................",
        "................",
        "................",
        "................",
    ],
]

BEACON = [
    [
        "..ssssssssssss..",
        ".scyyyyyyyyyycs.",
        ".sy..........ys.",
        ".sy....ww....ys.",
        ".sy...waaw...ys.",
        ".sy...ahhha..ys.",
        ".sy...waaw...ys.",
        ".sy....ww....ys.",
        ".sy..........ys.",
        ".syyyyyyyyyyyys.",
        ".ssssssssssssss.",
        "......cccc......",
        ".....cssssc.....",
        "................",
        "................",
        "................",
    ],
    [
        "..ssssssssssss..",
        ".scyyyyyyyyyycs.",
        ".sy....ww....ys.",
        ".sy...waaw...ys.",
        ".sy..aahhaa..ys.",
        ".sy..aahhaa..ys.",
        ".sy..waaawa..ys.",
        ".sy...waaw...ys.",
        ".sy....ee....ys.",
        ".syyyyyyyyyyyys.",
        ".ssssssssssssss.",
        "......cccc......",
        ".....cssssc.....",
        "................",
        "................",
        "................",
    ],
    [
        "..ssssssssssss..",
        ".scceeeeeeeeccs.",
        ".sy..wwwwww..ys.",
        ".sy.wwaaaaww.ys.",
        ".sy.waahhhaawys.",
        ".sy.waahhhaawys.",
        ".sy.wwaaaaww.ys.",
        ".sy..wwwwww..ys.",
        ".sy....ww....ys.",
        ".seeeeeeeeeeees.",
        ".ssssssssssssss.",
        "......yyyy......",
        ".....cssssc.....",
        "................",
        "................",
        "................",
    ],
    [
        "..ssssssssssss..",
        ".scyyyyyyyyyycs.",
        ".sy...eeee...ys.",
        ".sy..wwaaww..ys.",
        ".sy..waahha..ys.",
        ".sy..waahha..ys.",
        ".sy..wwaaww..ys.",
        ".sy...eeee...ys.",
        ".sy..........ys.",
        ".syyyyyyyyyyyys.",
        ".ssssssssssssss.",
        "......cccc......",
        ".....cyyyyc.....",
        "................",
        "................",
        "................",
    ],
]

LAMP = [
    [
        "......ssss......",
        ".....seeees.....",
        ".....saaaas.....",
        ".....shhhhs.....",
        ".....ssssss.....",
        ".......cc.......",
        "......sccs......",
        ".....sc..cs.....",
        ".....s....s.....",
        ".....s.aa.s.....",
        ".....s.hh.s.....",
        ".....ssssss.....",
        "....ssssssss....",
        "...ssssssssss...",
        "................",
        "................",
    ],
    [
        "......ssss......",
        ".....seeees.....",
        ".....swaaws.....",
        ".....sahhhs.....",
        ".....ssssss.....",
        ".......cc.......",
        "......sccs......",
        ".....sc..cs.....",
        ".....s.ww.s.....",
        ".....s.aa.s.....",
        ".....s.hh.s.....",
        ".....ssssss.....",
        "....ssssssss....",
        "...ssssssssss...",
        "................",
        "................",
    ],
    [
        "......eeee......",
        ".....ewwwwe.....",
        ".....eaaaee.....",
        ".....eahhhe.....",
        ".....ssssss.....",
        ".......yy.......",
        "......sccs......",
        ".....scwwcs.....",
        ".....swaaws.....",
        ".....s.aa.s.....",
        ".....s.hh.s.....",
        ".....ssssss.....",
        "....ssssssss....",
        "...ssssssssss...",
        "................",
        "................",
    ],
    [
        "......ssss......",
        ".....seaaes.....",
        ".....sahhhs.....",
        ".....shhhhs.....",
        ".....ssssss.....",
        ".......cc.......",
        "......sccs......",
        ".....sc..cs.....",
        ".....s....s.....",
        ".....s.ha.s.....",
        ".....s.hh.s.....",
        ".....ssssss.....",
        "....ssssssss....",
        "...ssssssssss...",
        "................",
        "................",
    ],
]

CRATE = [
    [
        "................",
        "..hhhhhhhhhh....",
        "..heeeeeeeee....",
        "..ehrrrrrrhe....",
        "..ehreeeeehe....",
        "..ehrheeehhe....",
        "..ehrhhhhhhe....",
        "..ehreeeeehe....",
        "..ehrrrrrrhe....",
        "..eeeeeeeeee....",
        "..ssssssssss....",
        "................",
        "................",
        "................",
        "................",
        "................",
    ],
    [
        "................",
        "...hhhhhhhhhh...",
        "...heeeeeeeee...",
        "...ehrrrrrrhe...",
        "...ehreeeeehe...",
        "...ehrheeehhe...",
        "...ehrhhhhhhe...",
        "...ehreeeeehe...",
        "...ehrrrrrrhe...",
        "...eeeeeeeeee...",
        "...ssssssssss...",
        "................",
        "................",
        "................",
        "................",
        "................",
    ],
    [
        "................",
        "....hhhhhhhhhh..",
        "....heeeeeeeee..",
        "....ehrrrrrrhe..",
        "....ehreeeeehe..",
        "....ehrheeehhe..",
        "....ehrhhhhhhe..",
        "....ehreeeeehe..",
        "....ehrrrrrrhe..",
        "....eeeeeeeeee..",
        "....ssssssssss..",
        "................",
        "................",
        "................",
        "................",
        "................",
    ],
    [
        "................",
        "...hhhhhhhhhh...",
        "...heeeeeeeee...",
        "...ehrrrrrrhe...",
        "...ehreeeeehe...",
        "...ehrheeehhe...",
        "...ehrhhhhhhe...",
        "...ehreeeeehe...",
        "...ehrrrrrrhe...",
        "...eeeeeeeeee...",
        "...ssssssssss...",
        ".....aaaaaa.....",
        "................",
        "................",
        "................",
        "................",
    ],
]

COURIER = [
    [
        "........................",
        ".........ssss..........",
        ".......sseeeess........",
        ".....sseeeeeeeess......",
        "...ssseeeehhhheeess....",
        "..sseeehhhaaaahhhees...",
        ".sseeehhhawwwhhhhees...",
        ".sshhhhhhhhhhhhhhss....",
        "..ssssssssssssssss.....",
        "......yyy.....aaa.....",
        "......................",
        "......................",
        "......................",
        "......................",
        "......................",
        "......................",
    ],
    [
        "........................",
        ".........ssss..........",
        ".......sseeeess........",
        ".....sseeeeeeeess......",
        "...ssseeeehhhheeess....",
        "..sseeehhhaaaahhhees...",
        ".sseeehhhawwwhhhhees...",
        ".sshhhhhhhhhhhhhhss....",
        "..ssssssssssssssss.....",
        "......yyy....waaw.....",
        "...............dd......",
        "......................",
        "......................",
        "......................",
        "......................",
        "......................",
    ],
    [
        "........................",
        ".........eeee..........",
        ".......sseeeess........",
        ".....sseeeeeeeess......",
        "...ssseeeehhhheeess....",
        "..sseeehhhawwaahhhees..",
        ".sseeehhhawwwhhhhees...",
        ".sshhhhhhhhhhhhhhss....",
        "..ssssssssssssssss.....",
        "......yyy...ewwwe.....",
        "..............ddd.....",
        "......................",
        "......................",
        "......................",
        "......................",
        "......................",
    ],
    [
        "........................",
        ".........ssss..........",
        ".......sseeeess........",
        ".....sseeeeeeeess......",
        "...ssseeeehhhheeess....",
        "..sseeehhhaaaahhhees...",
        ".sseeehhhaewwhhhhees...",
        ".sshhhhhhhhhhhhhhss....",
        "..ssssssssssssssss.....",
        "......eee.....aaa.....",
        "......................",
        "......................",
        "......................",
        "......................",
        "......................",
        "......................",
    ],
]


def write_header(groups: dict[str, list[Image.Image]]) -> None:
    lines = [
        "/* Native colored sprite sheet. Generated by tools/make-pixel-sheets.py.",
        " * PROPOSED period-illustration pixel art. ARGB1555, zero is transparent.",
        " * Does not replace the 8x8 masks in space-animation-kit.h. */",
        "#ifndef ELITE_PIXEL_SPRITE_SHEET_H",
        "#define ELITE_PIXEL_SPRITE_SHEET_H",
        "#include <stdint.h>",
        "",
    ]
    for name, frames in groups.items():
        w, h = frames[0].size
        lines.append(f"#define PIXEL_{name.upper()}_W {w}")
        lines.append(f"#define PIXEL_{name.upper()}_H {h}")
        lines.append(f"static const uint16_t pixel_{name}[4][{w * h}]={{")
        for fr in frames:
            vals = argb1555(fr)
            lines.append(" {")
            for i in range(0, len(vals), 16):
                chunk = ",".join(f"0x{v:04X}" for v in vals[i : i + 16])
                lines.append(f"  {chunk},")
            lines.append(" },")
        lines.append("};")
        lines.append("")
    lines += [
        "static void pixel_sheet_draw(const uint16_t *frame,int sw,int sh,int x,int y){",
        " for(int j=0;j<sh;j++){int yy=y+j;if(yy<0||yy>=H)continue;",
        "  for(int i=0;i<sw;i++){int xx=x+i;if(xx<0||xx>=W)continue;",
        "   unsigned p=frame[j*sw+i];if(!(p&0x8000))continue;",
        "   int r=(p>>10)&31,g=(p>>5)&31,b=p&31;",
        "   pixel(xx,yy,RGB((r<<3)|(r>>2),(g<<3)|(g>>2),(b<<3)|(b>>2)));",
        "  }",
        " }",
        "}",
        "#endif",
        "",
    ]
    HEADER.write_text("\n".join(lines), encoding="utf-8")


def current_sheet() -> None:
    families = parse_masks()
    names = ["PLUME", "BEACON", "SPARK"]
    cells = []
    for name, frames in zip(names, families):
        images = []
        for bits in frames:
            im = Image.new("RGBA", (8, 8), (0, 0, 0, 0))
            px = im.load()
            for y, row in enumerate(bits):
                for x, on in enumerate(row):
                    if on:
                        px[x, y] = (240, 180, 91, 255)
            images.append(im)
        cells.append((f"CURRENT {name} 8x8", images))
    sheet(cells, 8, "CURRENT IN-GAME MASKS  /  src/space-animation-kit.h").save(OUT / "current-8x8-masks.png")


def main() -> None:
    OUT.mkdir(parents=True, exist_ok=True)
    current_sheet()
    groups = {
        "plume": [paint(fr, 16) for fr in PLUME],
        "beacon": [paint(fr, 16) for fr in BEACON],
        "lamp": [paint(fr, 16) for fr in LAMP],
        "crate": [paint(fr, 16) for fr in CRATE],
        "courier": [paint(fr, 24) for fr in COURIER],
    }
    native = Image.new("RGBA", (128, 80), (0, 0, 0, 0))
    slots = {
        "plume": (0, 0),
        "beacon": (64, 0),
        "lamp": (0, 16),
        "crate": (64, 16),
        "courier": (0, 32),
    }
    for name, origin in slots.items():
        ox, oy = origin
        for i, fr in enumerate(groups[name]):
            native.alpha_composite(fr, (ox + i * fr.width, oy))
    native.save(OUT / "proposed-sprite-sheet-native.png")
    labeled = sheet([(name.upper(), frames) for name, frames in groups.items()], 4, "PROPOSED PIXEL SHEET  /  4x nearest  /  not a photo")
    labeled.save(OUT / "proposed-sprite-sheet.png")
    frames = []
    for i in range(4):
        snap = []
        for name, group in groups.items():
            snap.append((name.upper(), [group[i]]))
        frames.append(sheet(snap, 4, f"PROPOSED ANIM FRAME {i + 1}/4"))
    frames[0].save(OUT / "proposed-sprite-anim.gif", save_all=True, append_images=frames[1:], duration=160, loop=0, disposal=2)
    write_header(groups)
    print(f"wrote {OUT}")
    print(f"wrote {HEADER}")


if __name__ == "__main__":
    main()
