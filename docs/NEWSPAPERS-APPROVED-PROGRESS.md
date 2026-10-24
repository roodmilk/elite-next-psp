# ELITE: NEXT 2.5.399 — first five approved newspaper issues

Built on the exact local 2.5.398 source snapshot, retaining DockBay, ERL, planets and unrelated work. Only NEWS rendering/content, its native inspection entry point, tests and release metadata changed. V35 saves, existing puzzle progress and 1,000-unit puzzle prizes remain compatible. Replace only EBOOT.PBP; keep saves, settings and music.

Completed and individually inspected: Lave 16 pages, Diso 12, Quator 12, Riedquat 10, Leesti 8. The five approved fronts use native illustrations and the real font8 glyphs. Twenty print recipes cover continuations, interviews, scientific diagrams, letters, adverts, classifieds, sports and puzzles. Roughly 4,000 words of original local copy follow planned recurring people and institutions. The four-page Earth tutorial paper remains playable and unchanged editorially.

The completion ledger counts **58 accepted pages and five complete editions**. The other **251 galaxy editions / 3,024 pages**, plus the retained four-page Earth paper, are not certified against the new visual/editorial benchmark. Existing draft repetitions are not presented as completed writing. This is the first-batch checkpoint, not completion of the whole 256-edition request.

## Verification

- 59 newspaper assertions pass, zero fail. All 3,082 galaxy pages render; the separate Earth paper, actual D-pad/shoulder navigation, reopening, Sudoku/crossword entry, notes, deletion, persistence, reward payment and migration checks pass.
- All 58 new pages pass bounds, text/art overlap, ASCII font safety, minimum contrast and exact-paragraph duplication checks. Sentence-level scan finds no repeated long sentences. Main body ink meets 4.5:1; smaller approved accent text meets 3:1.
- Every new page was viewed at native 480×272 within unscaled comparison sheets. Per-page notes and hashes are in content/newspapers/native-page-reviews.json and completion-ledger.json. Fixed an invisible garden label, two mismatched puzzle instructions and phantom puzzle actions on two back pages. Puzzle zooms now originate in their actual printed apertures.
- Every new paper stays pixel-identical over time and when the ship is repainted red. No scrolling text, animated print noise, dates or visible page totals were added.
- Worst warm new-page software draw: 3,324 us in the emulator. Worst cold draw including legacy pages: 24,497 us. These are renderer measurements, not physical PSP FPS claims.
- Exact retail EBOOT: five original-PSP-model newspaper fronts pass nonblank native rendering, startup and memory checks. All five retail framebuffer images are pixel-identical to their QA counterparts.
- Exact retail startup with the user's actual MP3 passes on the original PSP model (intro) and PSP-2000 model (home). Minimum observed kernel headroom: 3,913,216 bytes. Retail image budget: 12,712,424 bytes; heap 7,340,032; main stack 1,048,576; remaining budget 4,064,792.
- Retail excludes embedded QA. Illustrations are compact native RGB565 primitives, not entire-page bitmaps. No new save fields or external runtime assets are needed for this batch.
- Physical PSP-2000/ARK playtest remains pending. Inherited broader game/input/performance failures from 2.5.398 remain documented and are not reclassified as passes; unchanged broad suites were not rerun for these newspaper-only changes.

SHA256: `cb73b7c4d0cb5289f268d22195e19a789515bd4b4ab11acd94611f1cb70c1ada`

## Remaining work / estimate

The galaxy-wide foundation exists, but 251 publications still require individual plans, original prose, suitable native artwork and all page gates. The next architectural step is a bounded per-edition print pack/cache so thousands of additional native illustrations are not all resident in PSP memory. Expand in complete-issue batches, retain cross-paper continuity and inspect every final page. Provisional remaining estimate: **30–50 active hours**, subject to measured content/art throughput and feedback; no calendar completion deadline is claimed.

Local release. GitHub remains unchanged: automatic approval previously rejected pull/rebase/autostash on the shared tree containing unfinished work. The shared checkout was not reset, stashed or committed wholesale.
