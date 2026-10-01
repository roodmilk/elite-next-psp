# Post-release backlog

## Discovery Codex station presentation

Do not begin during the current release freeze.

Add station-specific sprites to Discovery Codex / Discovery Star Atlas entries, including orbital stations such as Lave System Hub. The selected list item must have a clear focus state, and the same station sprite must appear in its detail page.

Future ownership:

- UI/Art: define the deterministic station-to-sprite mapping, native 480x272 focus treatment, high-contrast treatment, asset dimensions, and bounded asset budget.
- QA: capture native Atlas and station-detail screens in normal and high-contrast modes; verify focus legibility, sprite identity, clipping, and no save-schema change.

Constraints: preserve PSP readability, use bounded assets, reuse existing discovery identity data, and add no save fields.

## Galactic Lore return context

Do not begin during the current release freeze.

When Galactic Lore is opened from Discovery, Circle must return to that Discovery menu and retain the selected Atlas/Lore context. When Lore is opened from flight, preserve the existing return-to-flight behaviour.

Regression coverage must verify Discovery entry, Circle return destination, retained selected context, and the existing flight-origin return path.

## Station departure presentation

Do not begin during the current release freeze.

Simplify the departure view by removing the top "DEPARTURE ACCELERATING / CLEAR OF STATION"-style copy and the bottom "DEPARTURE GUIDANCE" footer. Keep the existing "CONTROLS LOCKED" state visible until control is actually returned to the commander.

QA must capture the native 480x272 departure screen and verify that launch safety, timing, and input locking are unchanged.

## Planetary EVA HUD readability

Do not begin during the current release freeze.

On-foot EVA HUD changes:

- Remove the unclear "SAFETY RAIL" connected-dot indicator.
- Remove Square tap / cycle-hold / target footer prompts from the lower control row.
- Replace lower-left R-tap wording with compact `R BOOST / JUMP` guidance.
- Keep the footer inside the native 480x272 safe band.
- Replace numeric HZ/HP-style suit readouts with compact graphical suit and health indicators: green at full, orange at warning, red when depleted. Icons must remain legible in high-contrast mode.

Do not change the actual controls, boost/jetpack behavior, damage, recovery, or save state. Add native normal/high-contrast visual regressions plus input checks proving those systems are unchanged.

## Planetary EVA local area map

Post-release feature proposal only. Do not begin during the current release freeze.

On-foot planetary EVA: START should open a local area map instead of the field guide. Render the actual bounded local scenery: terrain and shoreline, landing pad and ship, port buildings, rover, authored sites/landmarks, and major traversable boundaries. Areas remain under fog-of-discovery until visited on foot; do not automatically reveal distant scenery.

First implementation constraints:

- Fixed-size map and visited grid; no per-frame allocation.
- Native 480x272 readability and high-contrast support.
- Discovery lasts for the current session/current visit only unless a save-format proposal is explicitly approved.
- Preserve the field guide through a clear documented secondary action.

Required coverage: START and secondary-action input routing, map reveal, map boundaries, no distant auto-reveal, and native normal/high-contrast visual captures.
