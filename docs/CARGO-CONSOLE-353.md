# Cargo and exchange console — 2.5.353

Implements the commander's two supplied native 480×272 reference layouts.
Both modes retain the existing Cargo menu entry and live inventory/trading data.
The in-flight page is **CARGO / HOLD OVERVIEW**; docking changes the same page to
**CARGO / EXCHANGE OVERVIEW**. No new screen or tab is required.

The selected commodity's existing 32×32 item art is displayed at its native size
inside the 40×44 icon frame. Its full description occupies five 22-character
lines at x=68, y=54, with a nine-pixel baseline step. Every commodity has an
original 9×9 table icon drawn for that exact size, rather than shrinking its
larger item art. Gold selection, warm cream lettering, charcoal panels and
restrained gold rules follow the approved references.

The upper information panel occupies x=8–471, y=29–106. In flight, its right-hand
side contains a small pixel-built cargo bay, perspective floor, crates and the
highlighted cargo's native item art. While docked, that area instead shows the
actual galactic average and station stock, plus a concise price comparison,
restricted-goods warning or reserved-job quantity. Unsupported local commodities
show **NOT TRADED HERE / SELL AT ANOTHER HUB**, without pretending there is a
local price or shop stock.

The full-width table has headers at y=118 and up to seven rows beginning at
y=139, spaced 13 pixels apart. Commodity names, held quantities and numerical
columns have separate fixed bounds. In flight, the remaining columns show each
tonne commodity's share of occupied hold space and cargo reserved for missions.
Kilogram/gram consignments retain K/G units and have no tonne-share fill because
the existing game rules do not charge them tonne slots. Passenger space remains
included in the aggregate hold bar through cargo_used().

While docked, the table shows held quantity, actual shop stock and **PRICE HERE**.
Poor ports continue to offer their existing restricted commodity catalogue, but
items already carried are added to the list even if that hub cannot trade them.
Such rows remain selectable for inspection; both transaction directions are
blocked before calling trade(). Unsupported goods should be carried to a port
that supports them. Mission-reserved cargo retains its existing sell protection.

Up/Down select; Left sells and Right buys while docked; Circle returns. Footer
prompts use proper directional arrows and the PSP Circle icon. Available trading
labels dim on an unsupported row. Page position sits beside COMMODITIES and a
left-hand scroll indicator stays clear of numerical columns. Capacity and units
keep fixed bottom positions, with the units badge growing leftward for large
balances. Only rendering clamps the hold bar; actual quantities are never altered.

Source ownership: src/cargo-console.h, its include in ui-modern.h, and the MARKET
input availability check in main.c. The review is src/cargo-console-review.h,
run with tools/review-cargo-console.ps1. Newspaper/RUMOURS/GalacticNet Market
rendering and save format V32 are retained. No new save fields or migrations.

Release evidence and known inherited broad-check failures are recorded in
BUILD-2.5.353.txt after verification. Emulator/native review does not replace
physical PSP certification. The mixed shared checkout must not be published
wholesale.
