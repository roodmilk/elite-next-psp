# 2.5.174 — Ship separation and close-range combat

## Confirmed causes
Ordinary NPC ships steered at their enemy's centre and dropped to 40 m/s below 200 m, without light-ship separation. Antiparallel normalized linear steering also could remain pointing the wrong way indefinitely. These behaviours can produce the reported overlapping, slow rotating pairs.

## Changes
- Bounded angular steering with a deterministic perpendicular axis for exact 180-degree turns.
- Close combat flies a breakaway leg inside 450 m and retains 300 m/s combat speed.
- Predictive local traffic avoidance checks relative approach up to 1.2 seconds ahead, with right-side passing.
- Three bounded light-hull separation passes repair existing overlaps without damage, bounty, crime or kill changes. Capital traffic uses its existing berth/hull system.
- Weapons alignment tests the actual target direction, not the avoidance/breakaway direction, so ships cannot fire backwards while escaping.
- No new persistent fields, saves or assets.

## Tests and limitations
New model tests cover exact-opposite turning, coincident and overlapping patrol/pirate pairs, head-on encounters, finite unit directions and continued travel at 60/20 Hz. Separation preserves rewards, legal state, hull health and capital berth positions.
All five suites passed in smoke-20260927-131810-501, including independent NPC combat producing kills, police interceptions, missions, freighters and performance (55.45 FPS average; planetary scenes >=24 FPS). New regression assertions all passed.
Physical PSP playtesting remains necessary. Existing unrelated planetary shutdown investigation is not claimed resolved by this update.
