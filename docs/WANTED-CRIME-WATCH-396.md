# Crime Watch implementation — 2.5.396

The attached approved concept 6 is the layout source. The updated native PNG
defines the green treatment and footer; the GIF is timing reference only.
Original game space art is read from intro_backdrop. Text, portrait and rewards
are live data. GalacticNet header/tabs stay at y0..41 and footer at y248..271.

Large portrait: x300,y73,169x112; cyan frame x298,y71,173x135; separate status
banner y186..202. Five 76x29 thumbnails begin x62+82*i,y214. Portraits preserve
square source anatomy inside the reserved viewport. Large reward uses integer
3x real font8 glyphs; all actual values fit. The fixed Crime Watch navy/pink/cream
brand stays readable with every ship-colour chrome theme.

Controls: Left/Right wraps suspects, L/R shoulders select GalacticNet sections,
Circle closes. Up/Down/X/Square/Triangle have no warrant action. The board is
informational and never changes target, aim, docked state or units. Gameplay is
paused as before; only UI time advances. Repeated navigation interrupts the
180ms incoming portrait slide immediately. Only the new portrait enters, so
another suspect's face is never paired with newly selected name/reward/risk.
Entrance wipe 260ms; Circle reverse wipe 120ms. Shoulders interrupt immediately.
Tiny signal lamps and distant lights animate slowly; meaningful text is steady.
Claimed banner/check use saved bounty_claimed bits, with a brief frame stamp.

Tint: art_mix with 20/256 of #45c98a, followed on every fourth row by 20/256
of #062114. Row offset advances four pixels per second. The effect is confined
to the large portrait viewport, including during a slide. Source UVs never move;
thumbnails, frame, status and text remain untreated. There is no rolling sweep.

Portrait defect diagnosis: the old equal-height source-sheet grid captured
shoulders from the previous nonuniform row. The 2.5.379 repair already isolated
all 96 complete faces with reviewed explicit sheet gutters and a two-pixel tile
gutter. The latest 2.5.395 EBOOT renders both reported suspects cleanly; the
design ZIP used older October 9 captures. Keep the corrected legacy atlas,
row-span decoder, anatomical rigs and saved DNA unchanged. Copy five complete
64x64 source tiles into a bounded 40KiB cache once per system; no old mockup
portrait crops, top-row erasing or tint concealment. Validate actual MIPS output
against corrected source rows at 112px and 24px, inspect every source face at
both sizes, and include tint on/off captures for each suspect.

Economy: credits are internal tenths of units. Reward = 6000 +
4000*(danger-1) + 750*warrant_index, hence 600–2500U. Lave pays 600–900U;
danger five pays 2200–2500U. This is roughly 20–28 times the previous board
payouts. One low-risk capture covers even a badly damaged starter hull
(maximum repair about 455U); several captures fund a useful module or the
first 2500U hull upgrade. Expensive 25,000U+ hulls still need sustained progress.
Case priority explains modest variation among equal-danger warrants; NPC stats
and standard 15U incidental pirate bounty are unchanged. All five local warrants
are persistent one-time rewards. Claim bits survive save/load and system respawn;
an explicit already-claimed check protects against stale actor double payment.

Review: tools/review-wanted-crime-watch.ps1 -Eboot <review EBOOT>.
Legacy baseline: the same tool with -Suite wanted-poster-review.
Dedicated review covers real input routing, pause/return, all 1280 payout paths,
duplicate/stale hits, assisted kill, claimed respawn/save, all five tint comparisons,
96 source faces at both sizes, modern species, every ship theme and timing.
Native captures, comparisons, detailed checks and retail boot evidence are in
outputs/releases/2.5.396. Emulator measurements are not physical PSP FPS claims.
