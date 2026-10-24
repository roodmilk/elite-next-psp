"""Extract individual source subjects, including those crossing nominal cells."""
import json
from PIL import Image


def load_props(root):
    layout = json.loads((root / 'assets/source/biome-props/prop-frames.json').read_text())
    source = Image.open(root / 'assets/source/biome-props/kit-alpha.png').convert('RGBA')
    assert source.size == (layout['width'], layout['height'])
    assert len(layout['frames']) == 24
    owned = set()
    cells = []
    for frame in layout['frames']:
        left, top, right, bottom = frame['bounds']
        assert 0 <= left < right <= source.width and 0 <= top < bottom <= source.height
        crop = source.crop((left, top, right, bottom))
        alpha = crop.getchannel('A')
        remaining = {(x, y) for y in range(crop.height) for x in range(crop.width)
                     if alpha.getpixel((x, y)) > 160}
        islands = []
        while remaining:
            seed = remaining.pop()
            todo, component = [seed], {seed}
            while todo:
                x, y = todo.pop()
                for dx, dy in ((-1,0),(1,0),(0,-1),(0,1),(-1,-1),(1,-1),(-1,1),(1,1)):
                    p = x+dx, y+dy
                    if p in remaining:
                        remaining.remove(p)
                        todo.append(p)
                        component.add(p)
            islands.append(component)
        subject = max(islands, key=len)
        assert len(subject) > 10000, f'Missing subject: {frame["name"]}'
        world = {(x+left, y+top) for x, y in subject}
        assert not world & owned, f'Shared source pixels: {frame["name"]}'
        owned.update(world)
        # Adjacent canopies overlap in bounding-box space. Keep only this
        # subject's connected silhouette, never pieces of its neighbour.
        prop = Image.new('RGBA', crop.size)
        for x, y in subject:
            prop.putpixel((x, y), (*crop.getpixel((x, y))[:3], 255))
        prop = prop.crop(prop.getbbox())
        prop.thumbnail((46, 62), Image.Resampling.NEAREST)
        cell = Image.new('RGBA', (48, 64))
        cell.paste(prop, ((48-prop.width)//2, 64-prop.height))
        cells.append(cell)
    return cells
