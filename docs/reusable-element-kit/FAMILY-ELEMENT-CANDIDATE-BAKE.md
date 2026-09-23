# Prospector / Research native candidate bake

Commit: `8ce19e4`

The candidate bake contains twelve approved family anchors, each exported at its exact native target size with `normal` and `contrast` variants. The elements are reduced to the shared 11-role palette for review and composed into native `480x272` proof frames.

## Asset ledger

| Element | Native size | Packed 4-bit bytes |
|---|---:|---:|
| Prospector ore/sample wall | 92x72 | 3,312 |
| Prospector assay bench | 74x42 | 1,554 |
| Prospector cargo rack | 52x64 | 1,664 |
| Prospector lander window | 82x52 | 2,132 |
| Prospector standing prospector | 18x48 | 432 |
| Prospector seated miner | 18x38 | 342 |
| Research specimen cabinet | 64x86 | 2,752 |
| Research survey console | 70x44 | 1,540 |
| Research telescope | 58x58 | 1,682 |
| Research orbit window | 104x64 | 3,328 |
| Research standing researcher | 18x48 | 432 |
| Research seated researcher | 22x40 | 440 |

Totals: `19,610` packed bytes for one complete variant set; `39,220` for normal plus contrast. Shared palette, compiler alignment, metadata, and runtime draw overhead are excluded.

## Proof frames

- `assets/preview/reusable-element-kit/family-prospector-elements-normal-native.png`
- `assets/preview/reusable-element-kit/family-prospector-elements-contrast-native.png`
- `assets/preview/reusable-element-kit/family-research-elements-normal-native.png`
- `assets/preview/reusable-element-kit/family-research-elements-contrast-native.png`

## Certification state

These are art candidates, not runtime-certified assets. Systems must still validate byte equality on deterministic repeat, final packed representation, family assembly budget, zero runtime allocation, and normal/high-contrast output. Station must validate that every visible person/prop/feature maps to an existing hotspot and that art adds no capability. Gameplay and Story must validate the available action/result states and readable person roles. Runtime promotion is blocked until those checks pass.
