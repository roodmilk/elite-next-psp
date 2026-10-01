# Discovery Star Atlas (2.5.186)

## Player journey

Visited galaxy -> system directory -> landed planet -> category -> record.
Charted stars/stations have their own reference pages. Galactic Lore stays
on its existing independent route. Circle returns one level and restores the
cursor; shoulder buttons advance seven list positions. Planet tiles also
accept Left/Right. Empty collections remain readable but cannot open records.

## Sources of truth

- Visited systems: visited bitset plus current system.
- Landed worlds: landed_planets[system], not merely visible planets.
- Flora/fauna/minerals: surface_progress[system][body] species bits, filtered
  through field_species_kind. Stable identity is system/body/slot, not name.
- Site records: completed field_site_bit flags. These do not claim that every
  known site was visited; only completed activities are listed.
- Portraits, names and traits: same field sprite/profile providers used on foot.
- Space signals: V21 stores four rift identity bits per system at scan time.
  Listed records use the shared rift taxonomy, portrait and report providers.
  Older saves contain only a total; these remain unlocated historical records.

## Scaling and extension contract

The current engine has 1,024 landable worlds and 8,192 field identity slots.
The browser holds an eight-frame navigation stack and enumerates one branch
at a time. Each list renders at most seven rows; it does not allocate an
all-species catalogue or all-world image cache.

To add another collection, supply its count, stable ID lookup, label, portrait
and detail provider. Route its child through the same push/back mechanism.
Do not use display names as keys: seeded names can legitimately repeat.
Additional nesting is possible within the bounded stack; refuse deeper
navigation safely rather than overflowing it. Existing deepest route uses
four frames.

Adding more systems, more planets per system, or more than eight species per
world requires engine and save-schema work, not just a UI constant change.
Rift identities now persist at scan time in V21. Existing pre-V21 totals
cannot reconstruct old locations. Other signal collections need their own
identity provider and persistence before being listed as discoveries.
Future filters/search and additional site imagery can extend these providers.

## Verification

Regression coverage enumerates all 1,024 worlds and 8,192 records, validates
category membership, checks empty collections, last-page clamping, record
identity and cursor restoration. Native-resolution captures cover galaxy,
system, planet, every collection and each record type.

V21 adds 256 bytes of rift identity masks; no new external assets.
Back up saves before upgrading: older builds cannot read V21 saves.
Physical PSP readability and controller feel still require hardware review.
