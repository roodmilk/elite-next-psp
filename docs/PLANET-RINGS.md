# Ringed planet rendering

Version 2.5.203 replaces the small single-pass gas-giant ellipse with a depth-aware ring band.

The rear half is drawn before the planet sprite. The brighter foreground half is drawn afterward, so it visibly crosses the near face instead of disappearing behind the globe. Four closely spaced rails make the ring read as a band at PSP resolution. Seeded outer radius ranges from about 1.82 to 2.06 planet radii, with modest seeded thickness and skew variation. The complete ring follows cockpit roll with the planet.

Normal flight, the visible portion of hyperspace and first-person station departure use the same rear/body/front order. Very close planets stop drawing rings once the projected globe exceeds 240 pixels to avoid pathological off-screen line work.

PSP compilation and static wiring checks pass. The emulator remained closed; appearance and performance still need an on-device check.
