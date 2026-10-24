# Hair material correction — 2.5.363

The 2.5.362 hair polygon crossed the forehead and missed the top and back of the scalp. Its isolation checks trusted that same polygon, so passing checks proved internal consistency without proving the region was really hair. The player correctly rejected the native captures.

Human and Android hair now use manually traced 64×64 raster contours against the exact compiled atlas tiles 0 and 76. The contours follow individual rows around the curls, forehead openings, temples, sideburns and ear cutouts. Source alpha clips the silhouette, and the original dark outline and strand shading remain. The same material lookup is used when an ear part samples adjacent hair. Other species' differently named crest/feather/hood/receiver controls are unchanged.

Source of truth: `tools/bake-portrait-hair-masks.py`; generated runtime data: `src/portrait-hair-masks.h`. These masks are not inferred from a bounding rectangle, broad polygon or runtime RGB threshold. Inspect the compiled source pixels again if the two authored atlas faces change. Diagnostic enlarged source previews are in `outputs/hair363`; these are not the game captures.

The portrait review adds independently reviewed source-art landmarks at the scalp top, curls and temples, plus forehead, ear and cheek exclusions. Blonde and blue must change the hair samples while leaving the face samples pixel-identical. These checks complement all-pairs colour contrast, feature isolation, 20/32/96px captures, navigation and save/load checks. This prevents the old self-referential mask test from certifying the same bug again.

Existing saved DNA and selected options are not migrated. Legacy v0/v1 portrait rendering remains unchanged; v2 hair colours now follow the corrected hair material. New-look controls, scar, menus and planetary work remain intact.

Verified on the packaged executable: 37 portrait checks, 343 opening checks, two boot checks and 36 Command Deck checks. 1542 foreground render cases at 20/32/96px. Native captures were inspected and shown before packaging; copies are in outputs/releases/2.5.363/comparisons. The legacy portrait crop matches 2.5.361 pixel for pixel. Broad smoke retains the same 15 inherited game failures; radio/steering pass. Input smoke remains incomplete, with only baseline failures observed. Physical PSP testing remains pending. Exact hash and runs: BUILD-2.5.363.txt.
