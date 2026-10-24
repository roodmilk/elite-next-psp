# RUMOURS / Space Secrets — 2.5.351

Implemented on the local 2.5.350 newspaper candidate. This is a local PSP test build, not a GitHub release or certification of the whole game. Preserve the existing newspaper work and its remaining editorial gates.

## Player experience

Discovery → GalacticNet → RUMOURS replaces the old GalacticNet JOBS feed. Mission Board, Mission Log and mission progression remain separate.

Space Secrets is a purple, vertically scrolling fictional website. Its illustrated classifieds, travel reviews, small adverts and signal diaries sell directions to genuine discoveries in the current system. Only the document body scrolls; GalacticNet branding, tabs, URL and controls stay fixed. Quiet systems can have no offers. Earth has no invented Lave listings. There is no cross-system archive, scam or free introductory offer.

Descriptions invite exploration rather than promising ownership of the object. The whale invitation reads: “Come fly alongside these amazing space whales! We'll send you the search area. You bring the camera!” Every displayed whale experience corresponds to the real local pod. Its movement now stays within a persistent compact region, making those directions useful.

SpacePay uses fictional in-game units. Review shows cost, current balance and balance after purchase. Cancel starts selected; a fresh X press is required to confirm. Payment debits once and records ownership before its short animation. Skipping or closing the animation cannot charge again. The receipt offers TRACK SEARCH AREA and KEEP BROWSING, with browsing selected initially. Buying alone never changes the player's mission or flight target. Existing ownership and naturally identified discoveries provide directions without another charge.

Tracking supplies an offset search centre under the targeting computer's OTHER band. Select that marker through the normal targeting controls to fly there. It is deliberately not the precise object position and does not reveal the object's identity or override the ship's normal sensor range. On arrival, a short computer hint asks the player to watch the scanner. Search locally, select the real nearby signal, then use Triangle to inspect or hail it. Whales can be observed within 2,400 m; anomalies retain their existing close-range interaction rules.

One rumour search area can be active at a time, independently of the tracked mission. Square on RUMOURS stops its waypoint without deleting the purchase. Leaving the system clears the active waypoint; local ownership remains available when returning and must be explicitly tracked again. A permanently unavailable paid target is refunded once. Purchases and discoveries use the normal commander save system: this feature does not introduce automatic saving.

## Controls and layout

- D-pad Up/Down: browse the tall document; Left/Right: switch between the two adjacent top adverts.
- X: review the highlighted listing, activate the selected checkout button, or choose tracking after receipt.
- Circle: close checkout or return from GalacticNet.
- Square: stop active rumour tracking.
- Shoulder L/R: change GalacticNet sections outside checkout. Shoulder switching is suppressed during payment.

All layout and artwork are authored for 480×272. Text uses the existing real font8.h 8×8 glyphs, with explicit clipping and word wrapping. Content occupies x=0..463, y=60..247; URL occupies y=42..59; the right scrollbar occupies x=468..470. The page draws visible modules rather than allocating a tall framebuffer. Full illustrations are native 164×100 indexed images. Small advert diagrams are separately drawn at their displayed pixel size, rather than shrinking detailed illustrations.

Animations include a blinking logo eye, freighter running lights, whale wake/fin motion, signal waveforms, eased document movement, and a short SpacePay units-chip/data-strip transfer followed by receipt sparkles. Text stays stationary within each module. Payment transitions consume their input so a carried-over press cannot purchase or track unexpectedly.

## Source map

- src/rumours.h: canonical local eligibility, prices, safe offset search areas, ownership, tracking, arrival and one-time refunds.
- src/rumours-ui.h: native website, varied modules, animation, checkout and navigation.
- src/rumour-state.h and src/rumour-save.h: explicit saved state and validation.
- src/flight-extras.h: search-area and whale targets, sensor gating and observation.
- src/game.c and src/voyage.h: persistent whale home and actual visible pod positions.
- src/rumours-review.h and tools/review-rumours.ps1: disposable emulator checks and native screenshots.
- assets/source/rumours and tools/bake-rumours-art.py: approved original deterministic pixel art and reproducible indexed bake.

GalacticNet NEWS implementation and newspaper content are not edited by this feature. Existing save migration test fixtures account for the new extension when synthesizing older saves.

## Persistence

Commander save format is V31. The extension writes a revision word, one active search key, then 256 system records. Each record stores owned/found/refunded bit masks and 13 fixed-point prices. Slots 0–7 refer to existing persistent discovery slots, 8–11 to the canonical anomalies, and 12 to the whale pod. Keys never refer to transient NPC/debris indices. The explicit little-endian extension is 16,392 bytes and participates in the existing commander checksum. Invalid masks, invalid active ownership, inconsistent refunds and corrupt payloads are rejected. Existing V30 saves load with empty rumour state; never overwrite the only copy of an old save during hardware testing.

## Verification and limits

The focused suite covers local validity, absent contacts, exact cost, duplicate and insufficient-funds handling, mission independence, normal sensor limits, safe offset regions, one-time arrival hints, real identification, V31 round trips, V30 migration, corruption rejection, naturally discovered free directions, paused browsing, fresh-input checkout, animation skipping, explicit tracking, stop, navigation, all 256 systems, moving whales and exceptional one-time refunds. Native screenshots cover the home page, distinct content modules, whale review, checkout, transfer, receipt and quiet Earth state. It also measures page drawing and samples a complete beacon animation cycle.

The current canonical content supplies 203 eligible offers across 256 systems, including 63 whale systems; 134 systems have no eligible offers. This honestly reflects existing world content, not a claim that every system has unique authored secrets. No artificial discoveries are spawned to fill a webpage.

Baseline 2.5.350 has 15 broader game-check failures. A matching candidate baseline comparison must remain explicit; RUMOURS must not be presented as repairing unrelated migration, armour, traffic or opening failures. Physical PSP controls, memory, performance and long-session save/audio behaviour still require hardware playtesting. Newspaper writing/art acceptance remains governed by docs/NEWSPAPERS-350.md.

Final build evidence, hash and download location are recorded in BUILD-2.5.351.txt after verification. Keep this focused implementation separate from unrelated unfinished work when preparing a future Git commit/release.


Final verification (2026-10-09): 37 RUMOURS checks pass; all newspaper pages/puzzles and 12 Spacebook checks pass. Five Market captures differ only inside the exact renamed-tab glyph rectangle. The complete core report has 663 passes and the same 15 baseline failures; radio, steering and NPC combat have zero failures. Native captures inspected. EBOOT SHA256: 7D81C4E9F5E2443112E989E47798A6865753413F4ED3359F65E14D52BC77B893. Local packaged build: outputs/releases/2.5.351/ELITE-NEXT-PSP/EBOOT.PBP. No GitHub publication. Full evidence: BUILD-2.5.351.txt and outputs/rumours-test-evidence-351.json.
