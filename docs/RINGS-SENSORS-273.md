# Rings and sensor identification - 2.5.273

## Rings
Inner radius 1.4-1.55 times globe radius; outer 2.3-2.6 times. Existing seeded tilt retained. A bounded 2-pixel ray/plane annulus raster replaces the nine wire circles. Material opacity varies around 50%, with two clear gaps, soft inner/outer edges, warm seeded colour, globe occlusion and a planet-cast shadow. Close material has granular variation; existing solid mineable ice rocks use the exact same reduced annulus. No ring collision/landing change. Distant and fly-through screenshots are captured by opening-check. This is procedural material, not a new sprite asset. No full-screen postprocess or per-frame allocations.

## Sensors
`module_scan_range * 4` supplies base detection: standard 10 km, existing scanner 20 km, advanced 30 km. Stations register at 2.8 times base; freighters at twice base. The targeting bands, reticle selection and radar use the same detection check. Published celestial landmarks and intersystem route markers are always available. Required primary-hub docking cues and mission-marked stations stay available as navigation waypoints.

Selected contacts leaving range show SIGNAL LOST and stop auto-alignment. Their last observed position freezes until reacquired. Live NPC existence is deliberately separate from sensor detection, avoiding despawning off-screen traffic. The target computer displays base SENSOR range near its footer.

Detected NPCs/discoveries/debris/anomalies begin UNKNOWN. Target locking and ordinary proximity do not reveal names. NPC hails need the existing interaction reach (2.5/5/7.5 km); discovery hails need 12 km; debris inspection 1.8 km. Existing salvage still needs 500 m and remains available through the same Triangle action. Anomaly analysis must succeed before identity is recorded. Station and planet names stay known. Bounty board descriptions name the suspect but do not pre-identify or auto-follow the physical ship. Once hailed, police retain encrypted callsigns.

## Persistence and limits
Discovery/anomaly identity bits are stored per commander in contact-identities-N.cfg, with a format magic and checksum; corrupt files revert to unknown. Live NPC `name_known` resets on a new spawn. Debris recognition is transient and resets on system changes or slot lifetime replacement; it is not an independent saved object catalogue. Sidecar write failure can lose new discovery identification on restart, but does not alter the commander save. Shared commander/world save formats are unchanged.

## Verification
Focused tests cover unknown locking, successful close hail, frozen lost signal, list filtering, equipment-dependent detection, celestial exemptions, identity sidecar reload, failed distant hail, reduced ring dimensions and cargo tutorial recovery. Native screenshots cover distant/inside rings and unknown contacts. Exact final result and binary hash belong in BUILD-2.5.273.txt. Physical PSP fill-rate/FPS, real-hardware visual appearance and the pre-existing broad smoke issues are not certified.
