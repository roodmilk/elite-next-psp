# Comms console — build 2.5.271

One native 480x272 console replaces the scrolling list. Dark metal framing,
recessed tuner, bevelled keys, native-size pixel glyphs, teal rockers and orange
focus indicators share one fixed layout. Radio waveform and press feedback use
UI time while simulation is paused. Text never needs a scrolling viewport.

Controls: station tuner, music, chatter, captions, Contact Target / Answer
Incoming, Request Docking, Dismiss, Clear Target, HUD, camera, Quiet Mode,
Mission Log, salvage EVA and Controls help. Station walking and contrast were
removed at the user's request. No volume controls, history or Identify added.

Triangle tap retains quick flight interaction. Hold opens the console even
with an incoming transmission. X operates; Circle returns to flight or the
originating command deck. D-pad uses geometric neighbours. On the tuner, X
advances station and shoulder L/R tune backwards/forwards.

Incoming and outgoing contact use the existing hail/encounter actions.
Docking uses the existing guided docking action. Availability derives from
actual target, message, planet, docking and station-range state. Disabled keys
remain focusable, display dim/crossed glyphs and explain refusal in the bottom
strip. Explicit primary-station selection is distinguished from its ID-zero
fallback so Clear Target and Contact remain meaningful.

Quiet Mode snapshots prior music/chatter flags, mutes radio, battle music and
station jukebox, and restores the snapshot on the second toggle. Mission,
police and danger speech remains available. Captions affect optional speech
and radio ticker subtitles, not essential warnings. Existing radio.cfg stays
compatible; comms-console.cfg stores display/quiet preferences with version,
checksum, temporary write and backup recovery. Commander save layout unchanged.

Exact final EBOOT opening-check: RESULT 0 failures / 148 reported PASS checks.
Tests cover control reachability and nonoverlap, hold/tap, disabled actions,
pause/resume, outgoing/shared incoming conversations, radio tuning, every
setting, all Quiet Mode restoration combinations, persistence, Mission Log,
salvage EVA, help, docking and command-deck return, plus existing opening/ring
checks. Native station-name, disabled, incoming and selector screenshots were
inspected at 480x272. Physical PSP audio/input verification and broad legacy
regression certification remain outstanding.

Report: ../../../work/console-review-final-271/opening-check.txt
