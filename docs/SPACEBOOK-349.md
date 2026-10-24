# Spacebook homepage and story Inbox - 2.5.349

The shared NPC event feed now owns Spacebook throughout the opening and free flight. Removed the opening's +4 cached-post offset, its separate card rendering and the tutorial's pinned story post. Existing NPC history and reaction bits remain in their original save fields.

The homepage uses a blue/white Facebook parody at native 480x272, with the real commander portrait, a navigation rail, one complete focused activity card, an event image strip and a bounded local-pilot sidebar. D-pad up/down changes posts; left/right still skips ten. X likes and Square dislikes real NPC events. Triangle opens Inbox; shoulder L/R changes tabs.

Private correspondence is derived from saved progress in src/story-inbox.h. Earth friends/family and the first-ship note remain readable, Scott's photograph is an Inbox attachment, and reached Homeward, First Light and Open Channel briefings are retained without future chapter spoilers. Records are reconstructed briefings and notes, not a newly invented full transcript of past player choices. No save format changes.

Inbox uses a five-row scrolling message rail and pixel-measured, paginated letters. Square turns pages or opens Scott's photo. Circle closes the photo before returning to the menu. X on a story letter opens Mission Log; it never accepts or advances the story. X on cached Earth correspondence retains the existing undelivered reply flags. Triangle returns to the NPC feed. Actual home/story portraits remain consistent with their original scenes.

Run tools/review-spacebook-home.ps1 for feed-source, reaction, migration-to-correspondence, photo navigation, offline Dad reply, spoiler gating, Mission Log navigation, page clamping and save/load checks plus native screenshots. The opening integration check must also pass. Inherited broader regression failures remain outside this focused UI update.

## Final verification

Verification: PSP build succeeds. Spacebook home review RESULT 0 failures across 12 checks: one NPC-only feed in every opening phase, existing reactions, migrated private letters/photo, offline Dad reply, chapter spoiler gating, page clamping, Mission Log navigation and save/load. Opening integration reports RESULT 0 failures for all three starter ships through guided free-play progression. Seven native 480x272 feed, Inbox, attachment, final-archive and long-name captures inspected. Inherited broader regression failures remain open; physical PSP certification is not claimed.

Evidence: outputs/spacebook-home-20261009-214238-189 and outputs/opening-check-20261009-214238-988. Run tools/review-spacebook-home.ps1 and tools/review-eva-performance.ps1 -Review opening-check. See docs/SPACEBOOK-349.md.
