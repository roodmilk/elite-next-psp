# GalacticNet newspapers — 2.5.350 technical candidate

**The complete handoff is not certified or finished.** This build implements the
runtime and a fixed draft collection. Do not describe bank-derived draft writing
as 256 individually authored and approved final newspapers. Do not describe an
automated screenshot capture as a recorded manual inspection.

## Implemented

- One stable edition for each of the 256 canonical galaxy systems. Lave has 16
  pages; the collection contains 3,082 pages with configured lengths of 4–24.
- The user approved a separate four-page Earth tutorial paper. Sol is a tutorial
  override of runtime slot 7, not a 257th generated galaxy destination. Its paper
  must not overwrite Lave's identity or copy. Total rendered pages: 3,086.
- Sixteen bounded native layout arrangements, sixteen family identities, 24
  readable print palettes and 256 distinct 24×24 heraldic masthead registers.
  Mastheads have normal, enlarged and reversed treatments. Cultural papers can
  include untranslated native pixel glyphs in a reserved header strip.
- Real game 8×8 body type, integer positions, safe gutters, bounded complete
  sentences, separate art/caption rectangles, reserved margin wear, a left page
  rail and no visible page counts or dates.
- An original complete 16-page Lave issue, with recurring Night Shift Circle
  reporting, adverts, letters, corrections and Nip and Tug dialogue credited to
  Iona Reed. Original front-page writing also exists for Diso, Reorte, Leesti,
  Quator, Riedquat and Tionisla, plus the four-page Earth issue. The remaining
  editions use fixed bank-derived drafts and need bespoke editorial work.
- NEWS uses compiled fixed copy and canonical background facts. It does not read
  live prices, NPC counts, mission results, wanted status or commander activity.
  Canonical station illustrations use background economy, not event-modified
  economy. Paper colours do not use ship paint.
- Up/Down loops through pages; Left/Right and the existing shoulder shortcuts
  change sections while reading NEWS. Circle returns. Reopening starts at the
  front page. A 140 ms page-edge turn and a 120 ms paper-brush sound accompany
  page changes. Repeated input does not wait for the animation to finish.
- Twelve clipped native illustration subjects include printed orbital views,
  Cobra sketches, workshop portraits, public seals, cargo photographs, pinned
  evidence, bar scenes, toy circuits, routes, repair benches and Nip/Tug. These
  improve the common illustration kit; they are not 256 bespoke final art sets.
- X opens marked puzzle rectangles with a short geometric zoom and Circle
  smoothly zooms back before restoring paper navigation. Sudoku has
  cell movement, remembered digit choice, conflict rejection, erase, readable
  3×5 pencil numerals and completion acknowledgement. Word Dock uses a 7×7
  crossword, intersecting answers, an alphabet grid, slot selection, deletion
  and Across/Down switching. Neither puzzle awards anything in the game.
- Puzzle answers automatically save in small checksummed files alongside the
  EBOOT, independently of the main save: `news-puzzle-SLOT-SYSTEM.dat`, with
  temporary and backup files. Each active record is 308 bytes; the text cache
  is 10,560 bytes. No hundreds-of-page image allocation occurs at runtime.
- Spacebook, Inbox, Market, Wanted and Jobs retain their existing implementation
  and controls. The removed live Gazette path no longer drives NEWS.

## Puzzle controls

While zoomed, newspaper paging and section switching are captured. Circle closes
an open entry strip first, then returns to the paper. Sudoku: D-pad moves cells;
X opens/accepts a digit; Left/Right chooses; Square erases; Triangle toggles notes.
Crossword: D-pad moves the grid; X opens a clue; Triangle changes Across/Down.
In letter entry, D-pad selects letters, X enters, Square deletes and L/R chooses
the answer slot. The short alphabet row clamps to its nearest valid column.

## Source and review workflow

`content/newspapers/collection.json` is the fixed editable galaxy corpus.
`major-editions.json`, `lave-issue.json` and `earth.json` hold original writing.
`--issue lave` explicitly imports the complete authored Lave issue into the
collection; it validates complete copy without trimming sentences.
`tools/compile-newspapers.py` compiles the JSON into `src/newspaper-data.h`.
Normal compilation preserves edited records. **`--draft` replaces the collection
with bank-derived drafts; do not run it over subsequent editorial work.**

`tools/audit-newspapers.py` checks bounds, overlap, token/font safety, complete
line fit, unique page signatures/headlines/lead paragraphs and 4.5:1 print-ink
contrast, including the two-row caption limit. It deliberately reports editorial
certification as false. Printed lead paragraphs are counted separately from
metadata: 2,084 duplicate occurrences remain in the other draft editions. Do not
mask those duplicates by adding superficial identifiers.

`tools/review-newspapers.ps1 -CaptureAll` runs the PSP renderer for every galaxy
page and all four Earth pages, exercises navigation and puzzles, checks canonical
star/world counts, and tests visible ink pixels for every cached text line.
This last check was added after native screenshots exposed a paper/ink mapping
bug that geometric text-fit checks alone did not catch.

`tools/newspaper-gallery.py CAPTURE_DIRECTORY` converts every capture to native
PNG and creates a review desk displaying one 480×272 image without scaling.
Review notes are explicit; the tool never automatically approves a manual review.
The editorial manifest separately records writer, prose review and native review
status for every page.

## Remaining handoff requirements

1. Rewrite/review the complete draft corpus as genuinely bespoke editions, with
   stronger local institutions, continuing stories, voices, letters, adverts,
   political framing and long features. Unique signatures alone are insufficient.
2. Replace the common procedural illustration grammar with the requested richer
   bespoke native photographs, credited recurring comics and culturally distinct
   finished publication logos. Some current art is deliberately simple silhouette
   work; it is not the final art collection requested by the handoff.
3. Develop the semantic page treatments further: the sixteen bounded arrangements
   exist, but financial dashboards, sports tables, evidence boards and luxury
   spreads need more individually designed content and furniture.
4. Record an actual individual manual inspection for every final page. Automated
   captures and representative inspections do not satisfy this requirement.
5. Expand the puzzle collection and refine press feedback. The current puzzle
   runtime has smooth entry/return transitions but uses a small answer bank.
6. Measure memory, frame time, audio and input on physical PSP hardware. Emulator
   timings are evidence about that run, not a physical-hardware guarantee.
7. Resolve inherited broad game regression failures separately. Do not label this
   whole-game certification or publish it as the completed handoff.

The original handoff remains authoritative. This document describes actual
implementation progress and prevents the next contributor from confusing the
technical candidate with its much larger editorial/art acceptance gate.

## Lore and provenance

Current numerical facts come from the project's canonical `galaxy()` generator,
system archetype functions and station profile rules. Local people and cultural
stories are original fictional additions, not claims copied from Elite books or
later GalNet events. The original Elite-A source context is documented at
[Elite on the 6502](https://elite.bbcelite.com/elite-a/). Historical claims from
later Elite games must receive a separate source/era check before being added;
this candidate does not silently import their later political history.

## Verification notes for the final local candidate

The whole collection must be recaptured after any copy, art, palette or layout
change. The earlier 22:20 captures precede the complete Lave issue and illustration
kit. The 22:34 run found one real caption overflow on Lave's portrait page; its
caption was rewritten to fit, and the compiler/auditor now reject that case before
building. Do not reuse that failed run as final release proof.

The renderer also tests all twelve illustration subjects in eleven distinct native
apertures, checking every surrounding framebuffer pixel to protect captions and
neighbouring modules. Puzzle tests include zoom-back control capture and return.

Compiled code/data/BSS before the final caption correction: 13,925,488 bytes;
active puzzle record 308 bytes; text cache 10,560 bytes. The game additionally
reserves an 8 MiB heap and 1 MiB main stack. These are measured sizes, not a claim
that physical PSP memory/performance gates have been certified.


## Editorial and native review ownership

`content/newspapers/native-review-350.json` records only screenshots actually
viewed, with image hashes and specific findings. A representative matrix covers
all 16 families, all 24 palettes, all 16 arrangements and all 12 system
archetypes, along with the complete Lave and Earth issues, shortest/longest issues
and zoomed puzzles. This is not a claim that all 3,086 pages received manual
inspection. Use `tools/newspaper-review-matrix.py CAPTURE_DIRECTORY` to recreate
the unscaled contact sheets; it never approves them automatically.

The artwork review caught incorrect generic crates in short strips and a missing
portrait in a shallow aperture. The final renderer gives each subject a native
miniature and draws a smaller portrait when needed. The crossword capture fixture
also now explicitly waits beyond its zoom transition instead of photographing the
first frame. Those findings must be checked against the final recapture.

Outside NEWS, Market captures are compared byte-for-byte against the preceding
packaged EBOOT by `tools/review-news-isolation.ps1`. Spacebook, private Inbox and
Wanted use their existing focused checks. The two timestamp-bearing Spacebook
proofs can differ in their UTC timestamp; attachment and private-letter proofs
remain stable. The general game suite still has 15 failures identical to the
pre-change baseline, while radio and steering pass. This does not establish
whole-game release certification.

## Final exact-build evidence

Verification: final EBOOT newspaper checks RESULT 0 failures, including all 3,086 native pages and puzzle persistence/navigation. Market isolation is byte-identical in five states; Spacebook/Inbox and Wanted checks pass. Fifty-three actual paper pages and two enlarged puzzle views inspected; 3,033 individual paper inspections remain. Broad game failures exactly match the 15 pre-change failures; radio and steering pass. Physical PSP certification and full bespoke editorial/art acceptance remain open.

Final local EBOOT SHA256: 51DF45568F12828DC36097118DA26534896EF497659A836A3491BAB4FBCAF504.
Evidence: outputs/newspapers-20261009-224919-284; content/newspapers/test-evidence-350.json and native-review-350.json.


The final compiled text/data/BSS total is 13,933,812 bytes. Active heap/stack reservations are unchanged. The complete final native gallery is retained as lossless 480x272 PNGs; redundant intermediate BMP captures may be discarded. Printed draft paragraphs still contain 2,084 duplicate occurrences. Source and candidate packaging do not imply the full handoff is complete.
