# Procedural station exteriors

Version 2.5.202 replaces the one-size station exterior with a deterministic profile generated from system and hub identity. The primary hub in each of the 256 systems selects one of six structural families plus independent hull radius, depth, spin rate, colours, light colour, band count and pod count. Outer relays and frontier outposts receive their own profiles.

The inhabited hull ranges from 300 to 720 metres across and 290 to 760 metres deep. The navigable entrance remains a common 140 × 64 metre slit with the existing 104 × 44 metre safe ship-clearance box. Rendering, docking guidance and swept collision all read the same profile dimensions.

The entrance is no longer an open tube. Four dark corridor walls run nearly through the hull and terminate at an opaque, cross-braced, lit pressure door. This blocks stars, planets and traffic behind the station from being visible through its opposite side. Secondary hubs receive the same facade, tunnel and door treatment.

## Verification

- PSP compilation succeeds.
- `tools/check-station-exteriors.mjs` enumerates all 768 main hubs, relays and outposts. It verifies 768 distinct structural signatures, six-family coverage, more-than-twofold primary size/depth variation, fixed safe aperture, pressure-door wiring and shared generated collision dimensions.
- Core smoke fixtures compile checks for determinism, family coverage, aperture safety and dimensional spread.
- The emulator was deliberately kept closed. Runtime appearance, frame rate, manual slit flight and physical PSP behaviour remain to be verified.

No save data or external assets were added. Existing V26 commander files remain compatible.
