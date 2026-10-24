# Picross addition — 11 October 2026

User decision: each newspaper has **one puzzle**, selected from Sudoku,
crossword and Picross. There is no extra-puzzle button or selector.

The existing draft editions whose system index is divisible by three now
use Picross on their puzzle pages. The five accepted authored editions and
Earth tutorial retain their original puzzles and written instructions.
Current distribution: 84 Picross, 86 Sudoku, 86 crossword system papers.
Repeated puzzle-page appearances within an issue share the same puzzle and
progress; they cannot earn multiple prizes.

Picross uses six authored 10×10 space silhouettes, with exact derived row
and column clues. The pool cycles by system index divided by three, so all
six pictures appear among the eligible editions. Each picture has one
solution (verified by exhaustive line candidates and constraint search).
The native board uses 16-pixel cells and the real font8 glyphs, with clear
spacing for the two-digit clue 10.

Controls: X on the paper opens its puzzle. D-pad moves on the board; X
toggles filled; Square toggles an empty mark; Circle returns to the paper.
Empty marks are optional. Missing or extra filled cells prevent completion.
The solved picture's title is revealed only when correct.

Progress uses a separate versioned, checked 116-byte sidecar for each
commander slot and system (`news-puzzle-SLOT-SYSTEM.pic`), with temporary
and backup files. Existing Sudoku/crossword files and the commander save
layout are unchanged. Picross pays the existing 1,000-unit edition prize,
using that edition's original reward bit; an already-paid older puzzle does
not produce a second payment after conversion.

Verification:

- `tools/test-newspaper-picross.py`: all six pictures uniquely solvable;
  all 256 newspapers have exactly one assigned puzzle type; all six images
  appear in the assigned Picross pool.
- `tools/review-newspaper-picross.c`: isolated PSP diagnostics use the actual
  production Picross renderer, controls and sidecar code. Checks cover
  cursor wrapping, fill/empty toggles, system/slot isolation, persistence,
  corruption/backup recovery, clue bounds, correct/incorrect solutions,
  native clipping and return navigation.
- The integrated `src/main.c` initially compiled with the PSP toolchain.
  A final syntax check after concurrent shared-tree changes encountered an
  unrelated implicit declaration of `race_grid_intro_draw` at main.c:1099.
  No ERL code was changed here to resolve it. The final Picross diagnostic
  build has no warnings and all 38 native checks pass. Native captures and
  test logs are in `outputs/picross-side-review`; mean isolated renderer
  draw was 2,623 microseconds in PPSSPP, not a hardware FPS measurement.

This is a narrow source integration in a side conversation. No release
version, current EBOOT, ERL source, shared Git state or player saves were
changed. Diagnostic EBOOTs are **not** game releases. Full-game packaging was outstanding at this side-chat checkpoint; see the release confirmation below. Physical PSP playtesting remains pending.

## Full-game release confirmation - 12 October 2026

The Picross update is included in the tested 2.5.409 retail EBOOT, SHA-256 28a635dcab4dc8f7eabcf120de301b6654fa40a4d63549c7cc3eb2848a1d3951. Production newspaper integration, Picross renderer/data and diagnostic sources are byte-identical to the released source snapshot. The earlier unrelated ERL compile error is resolved; a new rebuild is unnecessary.

Rechecked all six unique solutions and the 84 Picross / 86 Sudoku / 86 crossword distribution. All 38 native PSP diagnostic checks pass again, including saved progress, commander/system isolation, backup recovery and controls. The exact released full-game EBOOT successfully renders system 0 newspaper puzzle page 6 as PIXEL PICTURE at native 480x272. This retail capture verifies the integrated newspaper entry/preview; the separate diagnostic verifies the shared production board/input/save implementation. Physical PSP confirmation remains pending.

Evidence: outputs/releases/2.5.409/evidence/erl409-picross-*.log and screenshots/picross-retail-newspaper.png. No new game binary or save-format change was needed.
