# 2.5.178 — GalacticNet and Outfitting polish

This build also advances ordinary NPC positions/collisions every frame (AI decisions remain staggered), eases lock-on alignment near its bearing, reduces boost-only speed heat (coefficient 10 to 0.35; normal overspeed, solar heat and ENG cooling unchanged), and tightens the Weapon Computer from 352x150 to 272x120 with dark amber combat styling. Added motion-per-frame, lock easing and boost endurance regressions.

- Spacebook and Inbox: blue scrollbar between the sidebar and cards.
- Jobs: left scrollbar with a slightly inset card edge to preserve spacing.
- Thumbs represent visible records, shrink on partial final pages, and follow existing Up/Down paging and wraparound.
- Galactic Lore: selected categories keep the SECTOR caption; highlighting still indicates focus.
- Outfitting: STATS:, COSTS:, slot-specific replacement text, and no ONE MODULE PER SLOT caption. Existing trade-in returns and service/no-slot labels remain accurate.

Missiles can lock any living ship faction within the existing range, without a hostility restriction. Existing hit/reward/legal logic remains unchanged. The Circle footer shows the selected missile/flare count. Dead/non-ship/out-of-range targets still reject without consuming ammunition.

No changes to prices, equipment effects, save format or assets.
Regression coverage renders every row in all three feeds, checks scrollbar pixels, verifies wraparound, and captures first/last pages. Existing outfitting captures cover the revised detail panel.

Physical PSP review remains recommended. The previous landing shutdown report is unrelated and remains unverified on hardware.

## Verification

Verified ../../work/smoke-20260927-141110-131: all five groups RESULT 0 failures, 55.45 FPS average, worst150.15ms,25 frames>25ms; surface >=24 FPS. Every-row scrollbar pixel tests/wraparound, all-faction missile locks, actual-dt NPC motion, easing/no-overshoot and four-second boost heat checks pass. Native first/last feed pages, outfitting, compact weapons panel and FLARES count inspected. SHA256 EAD60F0E866B8D7F1CF59D81FE9FF222BB6E7B49A6C6753322A9BEE1E6833D27. No assets/save changes. Local-only; no commit/push/release.

