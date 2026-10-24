# 2.5.354 — Circuit Console loadout and Item Storage

Built on the verified local **2.5.353** source, retaining the native Cargo and exchange console. Its predecessor EBOOT SHA256 is `28DE99FF5EC3D4D02EC5F075976E28E5CEC2F0CDFF551FCEF91D17FA814D14EA`. The base was rebuilt independently before integration; existing shared changes were preserved.

## Approved design and implementation

Implements the chosen first concept from `../loadout-concepts-three-20261010`, including the final request to remove the lower-right storage comparison and divider. Navy instrument surfaces, coloured module cartridges, amber cursor and mint active-weapon lamps are drawn at native 480×272. Text uses the unchanged game font and 8px advance. Decorative texture is behind readable surfaces.

All six categories and up to eight banks remain visible. Only a hull's usable banks are drawn; there are no invented extra sockets. Full selected names and complete effects appear below, with two effect lines for longer real modules. The prominent Triangle Item Storage card shows current occupancy and a live bar. A bounded miniature of the current real hull shows its current paint; each preview restores the renderer clip after drawing.

Item Storage is a separate screen. It contains only actual stored copies, with quantity, category, six-row scrolling list and left scrollbar. The left panel automatically shows only the highlighted module's category and its real usable banks. Fitted modules do not become spares merely because they are installed. The selected full name/effect occupies the lower strip, without duplicate instructions, capacity text or comparisons. Current units stay in a bottom-right pocket.

## Controls and underlying rules

- Loadout: D-pad Left/Right selects banks; Up/Down selects category. L/R shoulders also select categories and clamp to the new category's bank capacity. Square moves a fitted module to storage while docked. Triangle opens separate Item Storage. Circle returns.
- Storage: Up/Down selects an item. X selects it, then Up/Down chooses a matching bank and X fits/swaps. Circle first returns from bank selection to the item list, then leaves. The old module returns to storage on a successful swap.
- Loadout and storage cannot sell. The separate Outfitting sell menu and its confirmation/prices remain unchanged.
- Fitting and storing require docking. Cargo capacity, occupied passenger cabins and full storage retain their existing protections. Opening storage in flight is read-only; unavailable mutation hints are omitted.

No save schema or catalogue changes. Normal V32 saves remain authoritative for fitted banks, stored counts and credits. These screens do not grant new items or change weapon stats.

## Exact geometry

Rectangles use `(x, y, width, height)`, exclusive far edge. Game width/height remain 480×272.

| Region | Pixels |
|---|---|
| Native title | 6,4,468,18 |
| Ship stamp | 304,29,168,28 |
| First bank | 76,77,43,17 |
| Bank spacing | 47px horizontal; 20px vertical |
| Selected loadout detail | 8,201,294,45 |
| Storage action | 310,201,162,45 |
| Matching storage banks | 8,64,184,136 |
| Stored list | 200,64,272,136 |
| Bank rows | y85 + bank×14 |
| Stored item rows | y84 + row×19 |
| Storage selected detail | 8,205,464,39 |
| Credits | 341,252,123,13 |
| Footer | 8,249,464,19 |

Long storage-row labels reserve space dynamically for the quantity, including three-digit counts. Full names remain in the lower strip. Every catalogue effect fits two 34-character loadout lines; storage has 55-character lines. Credits align against a fixed right edge.

## Verification

`tools/review-loadout-console.ps1 -Eboot <candidate>` exercises the actual game input dispatcher, empty/large hulls, eighth banks, category clamping, item selection and swaps, storing without selling, flight restrictions, cargo/cabin protection, sell confirmation and normal save persistence. It captures all hulls and longest per-category effects at native resolution.

Run the existing cargo review against the same EBOOT and compare broader core failures with the preserved pre-change build. Final reports, hashes, screenshots and remaining limitations are recorded in `BUILD-2.5.354.txt` after verification. Static concepts alone do not constitute gameplay certification. Physical PSP verification remains pending, and the inherited core failures must not be described as fixed by this menu change.

Source ownership: `src/loadout-console.h` renders both selected layouts; `src/ui-modern.h` dispatches them while preserving Outfitting resale; `src/main.c` adds the shoulder category input and focused review hook; `src/loadout-console-review.h` contains the disposable review.
