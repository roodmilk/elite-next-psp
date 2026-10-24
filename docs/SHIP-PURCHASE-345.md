# Ship purchase celebration — 2.5.345

Successful shipyard exchanges open one transient congratulations card. The purchase commits through the existing buy_ship function before the card appears; the card never performs another transaction. Credits, cargo, fitted modules and Item Storage retain the existing exchange rules. Unaffordable, already-owned and incompatible hulls do not celebrate.

The 376×204 card is authored at the PSP's 480×272 resolution. An approximately 0.4-second arrival leads into two lines of handwritten pen strokes over 2.65 seconds, then an underline flourish. The new hull name, shipyard seal and crew signature identify the delivery. Four bounded particle bursts animate in the margins, outside the writing and labels. No textures, downloads, GPU shaders or game random-number calls are required.

Six signed messages vary without immediate repeats: Enjoy your new ride; Make it yours; Here's to new horizons; Go find your stars; Fly something special; Your next chapter awaits. The initial purchase press is guarded for 0.25 seconds. X completes unfinished writing; a subsequent X continues. Circle closes directly after that guard. These inputs belong to the card, not the shop beneath it, and closing never refunds or purchases another hull. The inactive shop footer is hidden. Leaving the shipyard clears the transient overlay. Save format is unchanged.

Run tools/review-ship-purchase.ps1 against an EBOOT to check real menu purchases, exact balances and retained items, double-input guards, failure states, all six variants, native captures, footer ownership and exit cleanup. Focused verification does not certify the inherited broader game regressions or physical PSP performance.

Verification: PSP build succeeds. Ship-purchase review RESULT 0 failures across real menu transactions, exact credits/cargo/modules, repeated input, failed purchases, all six notes, animated pen stages, footer ownership and exit cleanup. Native 480x272 arriving, writing and completed captures inspected. Guild navigation/save-load regression also RESULT 0 failures. Broader inherited failures remain open; physical PSP performance and full-game certification are not claimed.

Evidence: outputs/ship-purchase-20261009-210659-847.
EBOOT SHA256: 034F0BF0A0767BC7BE840444F5D0D83C9A21C734913C2A487A2D221EAAFA04E5
