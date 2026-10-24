# Elite Exploration startup — 2.5.366

Elite Exploration is the default radio station for absent/new preferences.
Valid saved station, radio-off and volume settings continue to take priority.
The tutorial's default-station prompts use the same station name.

On the first audio start in an application session, if Elite Exploration is
selected and audible, its first playable track starts ten seconds in. The audio
worker decodes and discards exactly ten seconds of source PCM, retaining MP3
bit-reservoir state and supporting variable bitrate and Vorbis. It never waits
ten seconds of wall time or guesses an encoded-byte offset. Very short files
fall through the ordinary playlist/repeat path without skipping repeatedly.

The audible output fades from zero to the configured music level over 44,100
output samples (one second), independently of source sample rate. Sound effects
and mission alerts are not faded. Station changes, later playlist tracks and
audio restarts/resume do not repeat this startup skip. Off/Quiet or a different
source cancels the startup treatment. Existing files and save formats are
unchanged; player music is never bundled or modified.

Verification uses the actual EBOOT: real MP3 alongside the intro JPEG decoder,
an exact source-sample discard count, the one-second gain envelope, continuous
audio through the film and release, and an audio restart without another skip.
The standalone radio suite checks default identity, saved settings, off-marker
migration, backup recovery, shuffle bounds, synthesis and suspend/restart. The
opening sequence and normal startup are checked on the same executable.

Exact results and artifact hash are recorded in BUILD-2.5.366.txt. Physical PSP
testing remains pending. Unrelated broader game failures and incomplete input
coverage recorded in BUILD-2.5.365.txt remain unresolved; this is not whole-game
certification.
