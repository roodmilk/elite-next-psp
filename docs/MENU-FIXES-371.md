# System Details, Comms, Cargo and Loadout — 2.5.371

System Details keeps STARS / WORLDS on its own line and shows the actual counts
underneath at (324,217), inside the right panel. This supports zero-star systems,
multiple stars and crowded systems without the old label/number overlap.

Comms navigation prefers controls sharing the current row or column before
diagonal candidates. Contact Target → Right goes directly to Request Docking;
Request Docking → Left returns to Contact Target. Disabled controls remain
selectable and blocked from activation. Global L/R radio tuning is retained.

In the in-flight Cargo screen, Square jettisons one unit of the selected commodity
as a real recoverable canister, behind and outside the ship hull. Quantities keep
their actual commodity units. It does not sell cargo or change credits/stock.
Reserved mission cargo is protected; a full debris pool leaves the cargo intact.
Jettison requires open-space flight (not docked, landed, dead, jumping, approaching,
docking or stopped by police). The list clamps safely after the final item leaves.
The footer shows Square Jettison in flight. Buy/sell still uses Left/Right while
docked, without the Bought/Sold or locked-market-control popup text.

Ship Loadout's detail heading moves from y=205 to y=207 with names/effects lower
inside the adjusted inset. Its footer contains D-pad Slot, Square Store, Triangle
Item Storage and Circle Back; L/R hints are removed. Existing shoulder category
shortcuts remain functional. Square stores the selected fitted bank while docked
and shows confirmation or a refusal reason inside the reserved detail area.
It does not sell the module; capacity, occupied cabins and cargo limits remain
protected. Moving to another slot clears the temporary feedback immediately.

Native tests exercise all 256 System Details count readouts, actual Comms D-pad
input, cargo jettison/recovery, mission and full-pool guards, silent market controls,
selected NAV bank-three storage, bounded feedback and the final footer. Existing
Cargo, Loadout, Comms, Commander Profile, Command Deck and normal boot checks run
against the same EBOOT. Native 480×272 captures are inspected before packaging.
The previous portrait cleanup, centered profile buttons and 21-second music skip
remain intact. Exact results and hashes are in BUILD-2.5.371.txt.

Physical PSP testing and inherited broad gameplay/input failures remain open.
This is a focused local release, not whole-game certification. Git pull was blocked
by existing untracked music placeholders and restored its autostash; unrelated
shared work remains preserved. GitHub has not been updated. Next: PSP playtest,
inherited reliability work, then planetary work.
