# Reusable element visual-quality audit

Audit scope: current native Freight/Prospector/Research candidate elements, station cue kit, ship preview, targeting scope, and normal/high-contrast proof frames. No runtime integration changes were made.

## Weakest elements ranked

1. **Door hatch — failed before this pass.** The previous Freight candidate was a window crop and did not read as an exit at 1x. Replaced with the canonical 52x60 native station-door pair from the cue kit. This preserves the frozen ID, pivot, hitbox, palette, and 1,560 packed-byte cost per variant.
2. **Panel sign — weak but usable.** The current 48x24 candidate has a readable cream label field, but its source crop is visually busier than the door and needs a family-specific sign pass once Station confirms the supported room label.
3. **Task lamp — small but acceptable.** The 21x18 silhouette reads at native size; contrast preserves the fixture edge. It should remain a secondary cue, never the only activity indicator.
4. **People — readable silhouettes, limited identity.** Standing/carrying/seated roles survive native scale and contrast, but Story should approve the pose-to-role mapping before these become named station actors.
5. **Ship preview / targeting scope — structurally strong, evidence incomplete.** Native clipping and bounded geometry are present; integrated normal/contrast captures and performance evidence remain outstanding.

## Upgraded asset set

- `assets/preview/reusable-element-kit/freight-elements/door-hatch-normal.png`
- `assets/preview/reusable-element-kit/freight-elements/door-hatch-contrast.png`
- Native size: `52x60`
- Packed estimate: `1,560` bytes per variant
- Normal SHA256: `0DB271B8E87F511670AD4EA3671B7B4F86C14B404371113CEE7692ADC1D42B6D`
- Contrast SHA256: `1A2CF981B35EC9533141CAAA3E2EE8DC5F46FAB4CADBA84CC5227C33EF016A65`
- Proof regenerated: `assets/preview/reusable-element-kit/family-freight-elements-normal-native.png` and `family-freight-elements-contrast-native.png`

## Outcome

The door-hatch weakness is corrected without changing runtime ownership or adding an interaction. The remaining visual work is panel-sign identity and stronger person-role readability; both are art-quality follow-ups, not permission to broaden station capabilities. Candidate status remains `runtime_ready:false` until the existing Systems/Station/Story gates pass.
