# Spacebook history (2.5.189)

One global newest-first archive per commander, capped at 4,096 records. Each record holds a packed event/ship/seed/reaction code, timestamp and origin system. Origin bit 8 marks imported V22 records, retaining their original three-variant wording; new records use eight variants. Event bits 0..4, ship bits 5..8, seed bits 9..29 and reaction bits 30..31 remain unchanged.

V23 stores clock, serial, count, tutorial evidence reaction, 256 last-seen stamps, then 4,096 post/stamp/origin triples: 50,192 bytes. V22 loading streams and merges all local records by descending timestamp, with undated fallback records after calendar records. Ties retain deterministic system/within-system order. Existing dated records, author seeds and reactions survive. Obsolete community-wire reactions are discarded except tutorial evidence. Earlier saves start with empty history. Back up saves; older executables cannot read V23.

RTC stamps are UTC Unix seconds for years 2000–2037. High bit marks a gameplay-clock fallback; unavailable dates are labelled honestly. Changed/backward clocks cannot underflow cooldowns. Same event/system cooldown is 90 seconds. Ambient posts occur every 240 active seconds, with at most three among the newest 16 records. Return gossip requires 86,400 seconds and is generated at actual return/load, never backdated.

Twenty event types each have eight distinct templates. Private deterministic seeds do not consume combat RNG. Consecutive same-type events avoid repeating their wording. Existing event hooks report successful actions. All player-event templates take the current title-cased commander name, not a ship model. Renaming updates displayed names in old posts too; names are not historical snapshots.

Two full-width cards per page, actual-font word wrapping, scrollbar, Up/Down selection, Left/Right ten-record jumps. L/R changes GalacticNet tabs. X toggles Like, Square toggles Dislike, Circle exits. No reply action. Tutorial evidence remains pinned when relevant; Inbox is separate. Reactions move with records and persist in manual commander checkpoints. Oldest records roll off at capacity.

To extend: append event IDs below 32, never renumber saved values; add eight bounded templates and call social_emit only after success. Ship IDs must remain below 16. Test 24-character names, numeric names, wrapping, save migration, full archive rollover, controls and RTC changes.

Validation this turn: PSP build and tools/check-spacebook.mjs (160 unique templates, 800 font/layout cases). C regression fixtures updated for V21/V22 migrations, V23 roundtrip, global visibility, rollover, reactions and input, but NOT executed. No emulator launched per user preference. Next: authorised runtime regression suite and physical PSP review of memory, migration, timestamps and controls.

