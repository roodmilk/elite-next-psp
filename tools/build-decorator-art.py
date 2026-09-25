"""Turn the approved concept into a fixed 480x272 PSP palette/RLE plate.

The generated image supplies decoration only. Gameplay labels and the rotating
ship are drawn by the game in reserved areas so they remain sharp and live.
"""
from pathlib import Path
from PIL import Image

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "assets/source/decorator-monitor-source.png"
NATIVE = ROOT / "assets/generated/decorator-monitor-native.png"
HEADER = ROOT / "src/decorator-monitor-art.h"

image = Image.open(SOURCE).convert("RGB").resize((240, 136), Image.Resampling.NEAREST)
image = image.resize((480, 272), Image.Resampling.NEAREST)
image = image.quantize(colors=48, method=Image.Quantize.FASTOCTREE, dither=Image.Dither.NONE)
image.save(NATIVE)
palette = image.getpalette()[: 48 * 3]
data = list(image.tobytes())
runs = []
for y in range(272):
    row = data[y * 480 : (y + 1) * 480]
    x = 0
    while x < 480:
        color = row[x]
        length = 1
        while x + length < 480 and row[x + length] == color and length < 255:
            length += 1
        runs.extend((color, length))
        x += length

colors = [
    f"RGB({palette[i * 3]},{palette[i * 3 + 1]},{palette[i * 3 + 2]})"
    for i in range(48)
]
lines = [
    "/* Generated from assets/source/decorator-monitor-source.png.",
    " * Regenerate with tools/build-decorator-art.py. Native 480x272 palette/RLE. */",
    "static const unsigned decorator_monitor_palette[48]={",
]
lines += [" " + ",".join(colors[i : i + 6]) + ("," if i + 6 < 48 else "") for i in range(0, 48, 6)]
lines += ["};", f"static const unsigned char decorator_monitor_runs[{len(runs)}]={{"]
lines += [" " + ",".join(map(str, runs[i : i + 24])) + ("," if i + 24 < len(runs) else "") for i in range(0, len(runs), 24)]
lines += ["};", "static void decorator_monitor_draw(void){", " int x=0,y=0;", " for(unsigned i=0;i<sizeof(decorator_monitor_runs);i+=2){", "  int color=decorator_monitor_runs[i],count=decorator_monitor_runs[i+1];", "  rect(x,y,count,1,decorator_monitor_palette[color]);", "  x+=count;if(x>=480){x=0;y++;}", " }", "}"]
HEADER.write_text("\n".join(lines) + "\n", encoding="ascii")
print(f"{NATIVE}: 480x272, {len(runs)//2} runs, {len(runs)} bytes")
