# Second Shift playable-preview art

Base1e3fab8. Asset-only delivery: no station routing, save, economy or input changes.

Include src/station-bar-art.h before station-crawl.h helpers. For the exact active REORTE39/H0 canteen, call sc_illust_second_shift(VX,VY,VW,VH) instead of sc_illust_canteen. Use second_shift_person_pos for its three52x64hotspot origins. Skip generic hatch and generic person sprite rendering in this exact room: the authored illustration includes three people and room architecture. Keep canonical options, feature highlighting and SHIP exit overlay. Other canteens remain untouched. Station/Lead owns conditional wiring and gameplay.

Native340x168 opaque room, packed4bit indices (28560bytes),11 RGB palette entries (44bytes); no heap, texture buffer or per-frame mutation. Decoder explicitly bounds framebuffer writes, clips to requested width/height without rescaling. It uses the native export of original generated art; source illustration was oversized then sampled/palette-limited, not hand-authored native pixels. This is the fast approved-style preview; later native simplification remains advisable.

Source assets/second-shift-native.png includes concept UI for provenance; bake reads ONLY(6,20,340,168). Existing planet/window is illustrative artwork, not authoritative orbit/mission data. No world identity or capability is inferred from this decorative view. Generated UI/cast text is outside the baked region. Runtime labels/dialogue belong to Station. Highcontrast retains this scene and relies on opaque native UI overlays; combined contrast captures must be reviewed.

Rebuild: tools/bake-second-shift.ps1 -Source assets/second-shift-native.png. Replacing the source requires palette/dimension checks, asset regeneration and actual native runtime/interaction/performance validation.

This asset commit alone is not a playable bar. Lead must combine Station's safe preview interactions, verify entry, three NPCs, free practice activity, gated navigation and exit, then build and test the exact EBOOT. No persistent jobs or real-credit dice are implemented by this art.
