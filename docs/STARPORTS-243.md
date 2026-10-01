# Planetary starports — 2.5.243
Date: 30 September 2026. Local cumulative candidate, based on 2.5.242.

## Player changes
- Shared planetary airport apron enlarged from 800m square to 1,440m square (3.24 times the area). Central player pad is 440m square; two heavy traffic berths are 320m square.
- Seven structures now include three large barrel-vault hangars, glazed stepped terminal, 300m control tower with wide control crown, services and cargo buildings. Physical signs, illuminated trims, taxi lanes and approach lights.
- Python/Anaconda traffic replaces the smaller decorative traffic models. These are visual airport traffic, not new purchasable capital ships.
- Wider rover garage moved clear of the player pad; shared collision dimensions, local-map outlines and cloud-platform footprint updated together.
- Parked player hull height derives from its lowest mesh vertex. All ten currently purchasable ships fit the pad and support exit/reboarding.
- Nearby seeded activity sites moved outside the larger airport reservation. SPACEPORT interaction now uses the actual terminal forecourt. Discovery IDs, rewards, save schema and controls unchanged.
- Retains 242 animal shadows, 241 debug fuel/jump toggles and planetary map/HUD, 240 first-person transfers, and Lave environments/animal behaviours. Separate expanded-TV branch is not incorporated.

## Implementation
Shared dimensions in planet-profile.h drive rendering, terrain flattening, collision, foliage clearance, traffic berths and map. New starport-render.h supplies the building families. Surface activity positions are cached by full world/profile identity. Covered terrain and invisible architectural faces/signs are culled. Lave uses an opaque airport prepass; other systems retain their existing depth-pass order.
Tests: starport-layout-tests.h and starport-review.h. Existing garage-position fixtures updated; broad terrain/terminal tests now sample the enlarged foundation and actual terminal footprint.

## Verification
Isolated build/proof directory: C:/Users/skarm/Documents/Codex/2026-09-26/referenced-chatgpt-conversation-this-is-an/starports-243
- PSP SDK build succeeds; final EBOOT 7,131,063 bytes.
- port-confirm: layout and native visual checks RESULT 0. All 1,024 world slots checked for dry level foundation and site reservation; all current ship clearances checked. Native views of all four Lave ports, day/night/high-contrast and framebuffer guards pass. Actual 480x272 images inspected.
- worlds-final: Lave II–IV walk/rover routes, scans, activities, port jobs and save checks pass.
- pilot-final: all-four-Lave landing/auto-exit/seat/launch input and camera clearance checks pass; 64-world generated landmark path sample passes.
- integration-final: 47 debug/map/HUD checks pass.
- shadows-final: 29 contact-shadow checks pass.
- These focused regression reports were generated before the final two test-fixture corrections; gameplay/rendering code is identical. Final EBOOT reran starport and broad game checks in port-confirm and smoke-confirm.
- Broad game RESULT 1: existing orbital-station swept-tunnelling failure remains. Earlier broad radio/steering pass. Broad input run timed out before completion; it includes inherited outfitting/dialogue/rover fixture failures. No whole-game green claim.

## Performance and limits
Native emulator port views sampled about 38.8/36.0/47.8/35.7ms average for Lave I/II/III/IV; worst samples 58.9/44.5/92.5/50.5ms, excluding audio/display. This is NOT locked 30 FPS, especially the cloud port. Physical PSP frame pacing and memory are unverified; further profiling remains.
Buildings are collision-backed exterior structures, not new walk-through interiors. Shared layout applies to all systems; visual validation covers Lave, not a complete visit to every planet. Other systems retain their prior environment art. No high-resolution concept or hitch-free guarantee.
Next: physical PSP review of large hangar/terminal close-ups, heavy-traffic clearance and worst-case cloud-port performance; then optimise remaining hot views and repair unrelated broad-suite failures.

## Delivery
C:/Users/skarm/Documents/Codex/2026-09-26/referenced-chatgpt-conversation-this-is-an/ELITE-NEXT-2.5.243/EBOOT.PBP
SHA256 097D3FBABAE480AD7E675AEF81DFB5253A84FA4A5ACBD5C8DD19D2931FE926EA
All new geometry is embedded; no extra art files required. Keep existing saves/music. No review flags or test saves in delivery. Shared root/dist binaries, other contributor work and user saves/music untouched; no Git sync/commit/push/tag/public release.
