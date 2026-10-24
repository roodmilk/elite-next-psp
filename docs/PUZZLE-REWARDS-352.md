# Newspaper puzzle interaction and prizes — 2.5.352

NEWS now receives its own input through the real top-level dispatcher when an
opening-story commander views GalacticNet. Previously, the opening handler
swallowed X even after the story reached free flight, while the direct newspaper
test bypassed that handler. X now smoothly enlarges the printed puzzle. Existing
Sudoku number entry, pencil notes, crossword letters and Circle return remain.
Leaving the Earth newspaper still advances its original onboarding story beat;
closing a puzzle does not leave the paper or advance a story.

Completing a Sudoku or crossword pays exactly **1,000 units** (10,000 internal
credit tenths) and opens a native 190×190 congratulations card. Its short expanding
border, medal, colourful sparks and Continue prompt use the game's actual 8×8
font and native pixel primitives. The final answer cannot dismiss it immediately.
While the card is open, input cannot change the paper or puzzle underneath it.

Each commander can claim each system's puzzle kind once. Reopening, erasing and
completing it again cannot repeat payment. An already completed, previously
unrewarded puzzle receives its prize when opened. Incorrect answers and pencil
notes do not count. A full credit balance leaves the prize unclaimed.

Save format V32 appends 257 bytes to the existing checksummed commander record:
indices 0–255 identify canonical systems; index 256 reserves Earth's separate
paper. Bit 0 means Sudoku paid and bit 1 means crossword paid. Other bits are
invalid. Credits and claims persist together through the normal commander save;
puzzle answers continue using their existing separate checksummed small files.
V1–V31 saves default the new ledger to zero without changing their balance. Older
builds cannot load a newly written V32 save, so retain a backup before upgrading.

Focused tests exercise actual top-level input, full-paper rendering, puzzle
editing, both prizes, animation input capture, repeat-payment protection, V32
roundtrip, V31 migration, invalid claim-mask rejection and Earth onboarding.
Existing legacy-save fixture truncation lengths account for the new tail.

This focused fix preserves the RUMOURS implementation and newspaper draft copy.
It does not certify all newspaper writing/art, clear inherited broad regression
failures or replace physical PSP testing. Exact build evidence is recorded in
BUILD-2.5.352.txt after verification.
