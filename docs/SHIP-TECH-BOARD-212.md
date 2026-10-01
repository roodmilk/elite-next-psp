# Ship Tech Board — v2.5.212

The board renders only installation slots supported by the current hull. It no
longer draws `--- LOCKED` placeholders: those slots had no unlock action and
made a small hull look artificially incomplete. Each category's real one to
four slots expands across the available row width. Empty usable slots remain
visible because they can receive equipment through Outfitting.

Navigation continues to clamp and wrap within the current category's actual
capacity. Installed-module totals now count only usable slots. Law Scanner item
56 has a bounded `LAW` board label instead of indexing beyond the label table.

Triangle has no action on this page and is absent from its controls. Outfitting
still has its own Triangle shortcut to view the board. `SQUARE ARM` appears in
the footer only when the selected category is WPN; other categories show no
arm instruction. Square's underlying safety response remains harmless if it is
pressed on a non-WPN category.

Automated checks cover Triangle inactivity and exact visible capacities for the
starter and largest hulls. Native normal/high-contrast captures cover all six
selected categories. Existing save data and module installation rules are
unchanged.
