# Planetary ambience

The on-foot renderer now builds a bounded ambience profile from the same stable identity used by the Field Guide and Codex: body type and palette, biome, settlement culture, weather, geology, seed, local time, fauna catalogue, and nearby site type.

## Shipped layer

- Stable world-space ground cover with biome-specific reeds, grass, scrub, flowers, and ice crystals.
- Up to 24 allocation-free airborne particles: leaves, dust, snow, ash, spores, or mist.
- Zero to two small distant flocks, enabled only where the existing fauna catalogue contains gliding fauna and suppressed at night.
- Weather motion, including stronger electric-haze wind and a restrained distant electrical fork.
- Local effects for thermal vents, gardens, ruins, migration towers, weather arrays, rescue sites, observatories, crystals, wrecks, caches, archives, and fossil beds.
- Procedural surface audio: quiet filtered wind plus biome accents, mixed through the existing SFX volume control without streamed assets.

The renderer intentionally favours varied behaviour over high object counts. Effects use deterministic hashes, stack storage, existing depth clipping, and no runtime allocation. The Square scanner remains separate and can identify or track the same world entities without owning ambience state.

## Verification

- PSP cross-build passes and produces `EBOOT.PBP` for version 2.5.229.
- Native-resolution PPSSPP launch/capture completes without an ambience compile or launch fault.
- The full regression runner currently reports unrelated existing failures in station tunnelling, outfitting, departure, and dialogue tests. Its performance report is not emitted because the current test process exits after the expanded input suite; therefore physical-PSP frame pacing remains to be confirmed.

## Next performance pass

Profile forest, electric-haze, and large-POI scenes on hardware. If a scene misses budget, reduce particle count first, then one flock; retain stable ground cover and landmark effects because they contribute more to planetary identity.
