# 2.5.390 — Portrait Broadcast (approved style 15)

Built from the current local 2.5.389 physical-PSP boot hotfix, not an older
GitHub checkout. The starting retail EBOOT matches the archived 2.5.389 release.
The staged missions, V33 saves, music fixes and separate retail/review memory
budgets remain. No save format, economy or mission outcome changes.

## Native layout

All coordinates are for the physical 480x272 framebuffer and the real 8x8 font.
The existing resident native space illustration supplies the background: no new
large image allocation or runtime image decoding is needed.

| Element | Native coordinates |
|---|---|
| Header | 0,0 — 480x24 |
| Monitor surround | 5,28 — 470x217 |
| Caller name / animated waveform | 16,36 — 334x46 |
| Actual caller portrait | 368,34 — 96x96 |
| Portrait frame | 362,31 — 108x102 |
| Speech | 12,94 — 338x52 (four-choice mode: y88) |
| Speech tail | Right-facing, ending at 360,87 beside the caller |
| Objective / cargo exchange | 12,152 — 456x36 |
| Two objective action cards | x12/244, y194 — 224x48 |
| Three ordinary reply cards | x12/166/320, y154 — 146x88 |
| Four police replies | x12/244, y146/198 — 224x44 |
| Footer / actual PSP button symbols | 0,248 — 480x24 |

Three replies accompanied by context use full-width, two-line compact keys in
the lower area; the context moves to y148 to keep a clear gap. Four-choice
contexts and unusually long objectives join the paginated speech instead of
being hidden behind controls. Long caller names and headings move within their
allocated display. Oversize selected reply text cycles through its remaining
lines, restarting at the beginning when selected. Speech uses L/R paging.

Left/right selects neighbouring horizontal cards; up/down remains usable for
ordinary choices. Police's four choices form a real 2x2 directional grid. X and
Circle retain their existing actions and restrictions. Contact-record browsing
keeps left/right stage selection and additionally supports L/R speech pages.

## Covered screens

Shared renderer: story briefings, tracked missions, Guild dialogue, incoming
ship conversations, trader offers and sign-off, private Quiet Watch channels,
ship-computer analysis, law interception/custody/release, and all 155 staged
mission calls/contact records. Actual speaker identity and repaired portrait
renderers are retained, including Kei/Ryn and the ship computer. The Comms
Panel console/settings and mission completion celebration remain unchanged.

Police dialogue renders after suppressing the underlying flight view so the
cockpit cannot overwrite its cards. Custody animation occupies a separate lower
window. Cargo exchange text is passed through the shared context layout.

## Verification

Review-only harness: `tools/review-dialogue-broadcast.ps1`, built using
`./build.ps1 -EnableReview`. It exercises real inputs/actions, caller ownership,
pause/resume, pagination, trade refusal, all four police choices, mission-log
navigation and framebuffer bounds, and captures every staged call/page.

Retail EBOOT must pass `tools/check-psp-memory.ps1` and exact-binary startup
checks using `tools/review-psp-boot.ps1`. Physical PSP confirmation is separate
from emulator validation. Test counts and evidence paths are recorded with the
release once validation completes.

Replace only EBOOT.PBP. Keep all saves, settings and music. This is a local
release on the existing shared working tree; GitHub is not claimed updated.

### Release validation results

494 focused checks pass: 43 broadcast/input/identity checks, 85 Comms checks,
334 staged-contract backend checks, 10 mission UI/action checks and 22 exact
retail startup/render/audio checks. All 155 staged transmission screens were
captured at 480x272. Native captures inspected include incoming replies, trades,
private chats, four police choices/custody, tracked missions, long speech and
ship themes. Native conversation rendering averaged 13.008 ms over 30 emulator
frames; this is not a physical-PSP frame-rate measurement.

Retail mapped image: 11,693,976 bytes. Reserved firmware/runtime headroom:
5,083,240 bytes (the 2.5.389 memory safeguards are unchanged). Exact retail
startup with the supplied intro MP3 retains at least 4,931,584 free kernel bytes.
The emulator's PSP-1000 and PSP-2000 models, home menu, flight and conversation
all pass. The retail SHA-256 is:
511CEB5633CCD0CD55CA9B5B8844F397AFCD50EDA9F89555BCDEE00F2DD765D2

The broad smoke still reports the 15 pre-existing core failures. That run reached
its time budget during the full input group; it is not reported as a broad pass.
Radio and steering reports pass. Broader failures are outside this UI change.

Release downloads, native galleries and machine-readable evidence:
outputs/releases/2.5.390/. Physical PSP/ARK retesting remains pending.
### Broader input follow-up

The completed input-only run reports 9,652 passes and 164 failures. Comparing
its failure messages with the recorded 2.5.388 baseline found only two new
instances, both an obsolete footer-coordinate assertion. The assertion now
checks the actual blue X icon at (117,260); both native normal/contrast captures
pass that check. The other failure messages are inherited. The broad suite is
not reported green, and unrelated gameplay failures are not claimed fixed.

Reports: outputs/expedition-review-20261011-012243-879/profile-law-review/.
The release evidence includes the unedited broad reports and native footer
check, separate from the 494 successful focused checks.