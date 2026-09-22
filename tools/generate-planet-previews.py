#!/usr/bin/env python3
"""Generate deterministic planet-profile QA data and a tiny dependency-free atlas.

The arithmetic intentionally mirrors src/planet-profile.h and src/sectors.h.
The PSP does not load this output; it is an offline inspection artifact.
"""
from __future__ import annotations

import argparse
import json
from pathlib import Path

MASK = 0xFFFFFFFF
OCEAN, ROCKY, GAS = 2, 1, 3
FAMILY_NAMES = ["ocean", "desert", "ice", "volcanic", "forest"]
FAMILY_COLORS = [(65, 145, 205), (201, 157, 83), (177, 211, 230), (173, 76, 53), (78, 145, 83)]
WORLD_PERM = (
    (OCEAN, ROCKY, GAS, ROCKY),
    (ROCKY, GAS, OCEAN, ROCKY),
    (GAS, OCEAN, ROCKY, ROCKY),
    (ROCKY, OCEAN, ROCKY, GAS),
    (OCEAN, GAS, ROCKY, ROCKY),
    (ROCKY, ROCKY, OCEAN, GAS),
)


def u32(value: int) -> int:
    return value & MASK


def profile_hash(value: int) -> int:
    value = u32(value)
    value = u32(value ^ (value >> 16))
    value = u32(value * 0x7FEB352D)
    value = u32(value ^ (value >> 15))
    value = u32(value * 0x846CA68B)
    return u32(value ^ (value >> 16))


def sector_hash(value: int) -> int:
    return profile_hash(value)


def planet_profile(seed: int, body_type: int) -> dict[str, int | str]:
    value = profile_hash(seed ^ u32(body_type * 0x9E3779B9))
    if body_type == OCEAN:
        family = 0
    else:
        family = 1 + (seed % 4)
    return {
        "seed": seed,
        "family": family,
        "family_name": FAMILY_NAMES[family],
        "terrain_style": (value >> 8) % 4,
        "palette": (value >> 12) % 8,
        "sea_level": 42 if body_type == OCEAN else 0,
        "prop_density": 35 + value % 46,
        "activity_density": 20 + ((value >> 16) % 61),
    }


def make_manifest() -> dict:
    systems = []
    for system in range(256):
        system_value = sector_hash((system + 1) * 0x9E3779B9)
        permutation = (system_value >> 6) % 6
        planets = []
        for body in range(1, 5):
            seed = sector_hash((system + 1) * 911 + body * 65537)
            body_type = WORLD_PERM[permutation][(body - 1) & 3]
            if system == 7 and body == 1:
                body_type = OCEAN
            if body_type == GAS:
                continue
            profile = planet_profile(seed, body_type)
            profile.update({"body": body, "type": "ocean" if body_type == OCEAN else "rocky"})
            planets.append(profile)
        systems.append({"system": system, "planets": planets})
    return {"generator": "tools/generate-planet-previews.py", "profile_version": 1, "systems": systems}


def write_atlas(manifest: dict, path: Path) -> None:
    width = height = 256
    pixels = [[(18, 22, 31) for _ in range(width)] for _ in range(height)]
    for entry in manifest["systems"]:
        system = int(entry["system"])
        ox, oy = (system % 16) * 16, (system // 16) * 16
        for index, planet in enumerate(entry["planets"][:4]):
            color = FAMILY_COLORS[int(planet["family"])]
            px, py = ox + 2 + (index % 2) * 7, oy + 2 + (index // 2) * 7
            for y in range(py, min(py + 5, height)):
                for x in range(px, min(px + 5, width)):
                    pixels[y][x] = color
        # A single bright pixel marks Lave/system 7 for visual orientation.
        if system == 7:
            pixels[oy][ox] = (255, 225, 120)
    with path.open("wb") as output:
        output.write(f"P6\n{width} {height}\n255\n".encode("ascii"))
        for row in pixels:
            output.write(bytes(channel for pixel in row for channel in pixel))


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--out", type=Path, required=True, help="output directory")
    args = parser.parse_args()
    args.out.mkdir(parents=True, exist_ok=True)
    manifest = make_manifest()
    (args.out / "planet-profiles.json").write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    write_atlas(manifest, args.out / "planet-profiles.ppm")
    total = sum(len(system["planets"]) for system in manifest["systems"])
    print(f"Generated {total} landable/ocean planet profiles across 256 systems")
    print(f"Wrote {args.out / 'planet-profiles.json'}")
    print(f"Wrote {args.out / 'planet-profiles.ppm'}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
