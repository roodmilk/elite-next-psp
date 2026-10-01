# 2.5.177 — Weapon selection, menu-release safety and explicit docking

## Gesture contract
- A fresh Circle press waits to distinguish a tap from a hold (0.20s).
- Holding opens the Weapon Computer only while the button is held.
- Circle + one new direction equips that tool and closes the selector immediately, even if Circle remains down.
- Releasing a selection or any long hold never activates it. A separate short Circle tap activates the selected tool on release.
- Ambiguous directional chords cannot activate tools. Selection preserves ammo/charges and does not steer.
- Removed both explanatory banners. Bottom Circle hint displays MISSILES, FLARES, ECM or HEAT SINK.
- Selected tool remains selected across menus. No save-format change.

## Menu Back
Every transition back into FLIGHT clears the gesture and requires Circle release. A held Back key (including repeat events) cannot open the computer or discharge a tool. Context exits, approach prompts, cutscenes and police retain their ownership of controls.

## Stations and menus
Triangle hailing a selected station opens the existing canonical COMMS PANEL with REQUEST AUTO-DOCK highlighted. X on that explicit option starts guidance; Triangle alone and Circle near a station do not request it. Circle still cancels menus/exterior docking guidance. Existing deliberate manual flight through the station aperture is preserved; this is separate from requesting auto-dock.
FLY Disembark is visible only while docked. Stable service ID remains20 so tutorial hooks are preserved. Existing safety guard also rejects direct out-of-state access.
Controls, tutorial, outfitting, mission/Guild/campaign guidance now describe selection-then-tap and the auto-dock request.

## Checks
Input regressions cover tap/hold thresholds, selection without resource use, immediate hide, held/repeated keys, release without firing, fresh activation, all four selections, menu Back from multiple screens, Circle near station, explicit docking request, planetary confirmation and dock-only visibility.
Earlier full run smoke-20260927-135249-833 passed all five groups. Final build additionally includes updated help/mission copy and multi-menu Back regression; all five groups pass in smoke-20260927-135749-688 (55.98FPS average). A long Outfitting description was shortened to fit; the GalacticNet Back test follows its real two-step return through HOME.
No hardware validation claimed. Prior planetary shutdown report remains open pending physical PSP test.
