## Winding arcade races — 2.5.387

32 ordered physical gates replace the almost straight twelve-gate sprint.
Seeded courses cover roughly 900 km with repeated S bends, climbing turns
and descending chicanes. A starter Adder finishes in about two minutes using
normal steering, throttle and double-tap-R boost. Bring your own ship: its
actual speed, handling, acceleration, installed modules and fuel still apply.
Gold boost/cooling rings on every leg support sustained arcade racing.
Four scenery sections: asteroid slalom, split freighter wrecks, floodlit
port grandstands/pylons and ice fields, with open racing lanes and trail lamps.
5-10 physical rivals include clean racers and opportunistic shoulder-checkers.
Swept relative-motion collisions catch high-speed rams between frames.
Gentle contact separates the ships and costs speed; hard/boosted impacts
break the rival's actual hull mesh into spinning debris, sparks and a shockwave.
Destroyed racers leave the leaderboard and cannot finish or take the prize.
A bounded 1.2-second slow-motion orbit frames the takedown, then a 0.6-second
swing returns to the actual first/chase/cinematic camera. Safe autopilot runs
through the shot and for one further second afterward, using real gates.
Every racer shares the slow simulation; menus pause the camera and actors.
Held boost resumes on return if still held; there is a cooldown between shots.
Targeting browser, quick-target and target-lock combos are disabled while
racing, including the menu route. Square never steals R throttle during races.
Race HUD shows section, next gate distance, place, time and takedowns.
First place alone earns the existing actual mission prize once. Free retries
reset every rival, effect, gate and camera. Five-minute safety timeout.
Race music uses the MP3 library in music/Sol Rock beside EBOOT.PBP (Rock FM).
Playback works even when ordinary radio music is off, respects Quiet Mode,
does not apply the intro-song skip and restores previous radio preferences.
No MP3s, saves or private settings are included in this package.
Existing V32 save compatibility, mission roster and law enforcement remain.
The 21-second intro music start/fade and SDK display-startup repair remain.

Exact release validation:
INFO production flight: phase 5 gate 32 place 1 time 119.85 heat 0.0 fuel 47.1 hull 100.0
INFO dense race draw: 730 max submitted triangles, 55.89 emulator render FPS (no hardware guarantee)


ELITE: NEXT 2.5.387 - Winding arcade races and takedowns
EBOOT SHA256: 7A4D203DC55FD2A16CFB906314BCBBEE30B3293898C3C7068685C2C35D688472
Embedded cinematic SHA256: 0112B4C740CD880B8F0AF4BBF28D0BD4F17D91B1519EE4971EA5CA56395A499C (unchanged)
Built on local 2.5.386; unrelated production source hashes preserved.

32 ordered physical gates replace the almost straight twelve-gate sprint.
Seeded courses cover roughly 900 km with repeated S bends, climbing turns
and descending chicanes. A starter Adder finishes in about two minutes using
normal steering, throttle and double-tap-R boost. Bring your own ship: its
actual speed, handling, acceleration, installed modules and fuel still apply.
Gold boost/cooling rings on every leg support sustained arcade racing.
Four scenery sections: asteroid slalom, split freighter wrecks, floodlit
port grandstands/pylons and ice fields, with open racing lanes and trail lamps.
5-10 physical rivals include clean racers and opportunistic shoulder-checkers.
Swept relative-motion collisions catch high-speed rams between frames.
Gentle contact separates the ships and costs speed; hard/boosted impacts
break the rival's actual hull mesh into spinning debris, sparks and a shockwave.
Destroyed racers leave the leaderboard and cannot finish or take the prize.
A bounded 1.2-second slow-motion orbit frames the takedown, then a 0.6-second
swing returns to the actual first/chase/cinematic camera. Safe autopilot runs
through the shot and for one further second afterward, using real gates.
Every racer shares the slow simulation; menus pause the camera and actors.
Held boost resumes on return if still held; there is a cooldown between shots.
Targeting browser, quick-target and target-lock combos are disabled while
racing, including the menu route. Square never steals R throttle during races.
Race HUD shows section, next gate distance, place, time and takedowns.
First place alone earns the existing actual mission prize once. Free retries
reset every rival, effect, gate and camera. Five-minute safety timeout.
Race music uses the MP3 library in music/Sol Rock beside EBOOT.PBP (Rock FM).
Playback works even when ordinary radio music is off, respects Quiet Mode,
does not apply the intro-song skip and restores previous radio preferences.
No MP3s, saves or private settings are included in this package.
Existing V32 save compatibility, mission roster and law enforcement remain.
The 21-second intro music start/fade and SDK display-startup repair remain.

Fresh validation: 572 focused emulator checks pass, including
71 racing checks, real MP3 decoding from music/Sol Rock,
all 768 deterministic clear courses, unmodified production flight, physical
contacts, shared slow motion, safe camera return in all three camera modes,
pause, retry, prize once, save/load, targeting lockout and framebuffer guards.
INFO production flight: phase 5 gate 32 place 1 time 119.85 heat 0.0 fuel 47.1 hull 100.0
INFO dense race draw: 730 max submitted triangles, 55.89 emulator render FPS (no hardware guarantee)
Native 480x272 scenery, route, pack, collision and camera captures inspected.
Core: 664 pass; same 15 inherited failures remain.
Review: C:\Users\skarm\Documents\Codex\2026-09-14\let\outputs\arcelite-psp\outputs\race-release-review-20261010-232159-176
Normal boot: C:\Users\skarm\Documents\Codex\2026-09-14\let\outputs\arcelite-psp\outputs\normal-boot-20261010-232431-178
Physical PSP boot, race controls, sound and FPS still need hardware playtesting.

Local release only. GitHub remains unchanged following the earlier automatic
approval rejection of pull/rebase on the mixed shared working tree.
Replace ONLY EBOOT.PBP; keep commander saves, settings and music.
