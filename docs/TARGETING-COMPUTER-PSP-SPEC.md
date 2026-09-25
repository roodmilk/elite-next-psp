# Targeting Computer — PSP native specification

The targeting computer is a physical ship monitor, not a second cockpit view. It is rendered at the PSP's native **480×272** framebuffer with an 8 px safe grid.

## Layout

- Header: y 0–23, title `TARGET COMPUTER`.
- Scan list: x 8–279, y 32–187. Six rows are visible at once; additional contacts page by six without shrinking text.
- Readout: x 288–471, y 32–187. It shows the selected contact's name, class/status, range, faction and condition bars where applicable.
- Footer: y 248–271. Controls are always explicit: Up/Down select, L/R change scan band, X lock, Triangle details, Circle back.
- The bezel keeps the existing monitor frame and small crew stickers. The scope stamp sits inside the readout only.

## Information hierarchy

The list answers “what can I target?”; the right panel answers “what is it and why does it matter?”. Unknown contacts remain readable as `UNKNOWN CONTACT` until identified. Mission targets use the mission color, factions retain their established colors, and distance is right-aligned in metres. Hull/shield bars are shown only for identified ships and use compact five-segment bars.

Triangle reveals a short details line without opening another page. X locks the selected contact and returns to flight, preserving the existing auto-align rules. L/R changes the current scan band; selection never silently changes when the band is changed.

## Rendering rules

Use the project palette and integer pixel coordinates only. Never draw beyond x 471 or y 247, never place labels over the footer, and keep names clipped to their field width. The monitor's scanline/fleck treatment is drawn before glyphs so all text remains sharp. The screen must remain legible with high-contrast mode and at native 1:1 resolution.

## Validation

The smoke build must open the page with zero contacts, one contact, six contacts and more than six contacts. Verify long station/freighter names, unknown contacts, mission targets, NPC hull/shield bars and Triangle details at 480×272. The page must compile with the existing PSP toolchain and preserve the current held-Square flight targeting controls.
