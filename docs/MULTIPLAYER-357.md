# Multiplayer — Retro Terminal A, 2.5.357

Implements the approved `ELITE-NEXT-MULTIPLAYER-DESIGN-HANDOFF-2026-10-10.zip` on local 2.5.356, retaining Orbit Motors/Crown Grand, Controls, Cargo, Loadout and separate Item Storage. The ZIP is a design reference; its fictional pilots, room codes and system names are never release data. `src/font8.h` and the 100×18 native logo match the handoff fingerprints.

## Native screen

`src/multiplayer-console.h` draws the actual interactive screen through native primitives. It does not load screenshot-sized backgrounds, allocate per frame, read files while drawing, or initiate networking during rendering. The sparse starfield is deterministic. All coordinates below are PSP pixels, with an 8px font advance and cell height.

| Region | x,y,w,h |
|---|---|
| Header / logo | 0,0,480,24 / 5,2,100,18 |
| Application window | 8,36,464,204 |
| Title bar | 16,44,448,16 |
| Connection panel | 16,68,256,96 |
| Connection button | 24,78,240,46 |
| Action inset | 232,89,24,24 |
| Pilot/flight panel | 288,68,176,96 |
| Bottom-left / right | 16,180,216,46 / 248,180,216,46 |
| Footer | 0,252,480,20 |
| Confirmation shadow / dialog | 38,80,404,120 / 42,84,396,112 |
| Yes / No | 100,154,128,28 / 252,154,128,28 |

Colours follow the reference: violet background `#211d39`, charcoal-violet window `#40374e`, recessed panels `#292b3d` and `#202b39`, normal buttons `#505166`, selected `#44696c`, cream `#f0e7d4`, teal `#b6e6d5`, error red `#ed777f`. The connected control **always reads CONNECTED**, with a plain two-stroke red X inside its dark inset. The PSP Cross input icon appears separately in the footer. Small signal bars, a working lamp pulse and a nonblocking button flash provide animation.

The right panel shows the selected valid pilot and their actual protocol location. Names wrap into two 19-cell lines; overlong second lines use a confined slow marquee. System/planet/state text uses the same bounded treatment below it. Other systems remain visible, with same-system pilots first. Local results show actual advertised FLIGHT codes, never invented commander identities. Three flight rows fit at a time; longer lists scroll inside the fixed panel. Full online rosters remain bounded. Joining retains the selected flight code and its actual channel.

## Routing and controls

Fly now contains one **Multiplayer** entry (stable service ID 29). The old standalone Host/Join entries are incorporated into this screen; they are no longer separate deck buttons. Title Square opens the same console. Legacy page identifiers remain compatibility aliases; there is only one presentation/input implementation.

Focus regions are connect, list, bottom-left and bottom-right. Right from connect enters a nonempty list; Down reaches Host. List Up/Down traverses real rows, with Down from its last row reaching Join. Bottom-left Right reaches bottom-right and Up reaches connect. Bottom-right Left reaches bottom-left and Up reaches the list, or connect if empty. Outer edges stay stable. Missing rows clamp safely. Cross activates once per press; Circle returns to the actual caller without disconnecting. Square rescans only completed local results. No new Triangle shortcut.

Dialogs capture input, start with No selected, use Left/Right and Cross, and let Circle cancel only the dialog. Switching online → local, local → online and ending local require confirmation. Hosting and joining have distinct confirmation wording. Availability is rechecked after confirmation before tearing down any existing transport. Title callers can manage online state but cannot track a pilot or start an unloaded local commander. Existing tutorial, planetary, docking, jumping and law restrictions on starting local Wi-Fi remain.

Tracking uses `online_apply`, `wifi_contact_route` and existing safe target/surface rules. It leaves the console open with concise feedback so the player can review the route, then return. It does not teleport or launch the commander. Cargo, economy, save progression, session replication and purchase rules are unchanged.

## Transport and lifecycle

`src/online-presence.h` and `src/local-wifi.h` remain the actual transports and protocol authority. Online has separate Wi-Fi association, server handshake and connected stages. `online.enabled` is a preference, never a connected lamp. The private server still has two total pilot slots; client peer capacity and security validation are not increased.

Explicit disconnect closes the socket and network modules, clears peers/remote locks and disables retries. Terminal failures await an explicit retry instead of secretly restarting. Error causes distinguish WLAN off, no profile, initialization failure, server unavailable/full and lost connection. Settings writes retain their existing staged-file/backup sequence. A failed disconnect preference write cannot keep an unwanted socket alive. Retrying a failed write retries persistence; a live connection stays truthfully CONNECTED if a settings write fails. Offline play remains available.

Local Host/Join reuse the existing nonblocking ad-hoc scan, advertisements, connection and session code. Setup failure rollback remains distinct from normal session progression. Circle preserves a valid waiting/session connection. Explicit cancel/end owns teardown. Online and local never initialize simultaneously. Late server acknowledgements after cancellation cannot resurrect a link; peer departure/expiry removes stale targeting and selection.

Online heartbeats continue in the main loop while the console is open; local service continues through its input tick. The commander simulation stays paused in the menu. Sleep uses the shared `multiplayer_suspend` cleanup, clears modal and remote targeting state, preserves commander progress and exposes truthful lost-link/reconnect state. Real physical PSP sleep and two-device ad-hoc testing remain separate hardware gates.

## Verification

- `tools/review-multiplayer-console.ps1`: production-rendered reference states, input/navigation, empty/full lists, long names, dialogs, actual handshake packet validation, save failures, target expiry, caller returns, cancellation, transport exclusivity and simulated suspend cleanup. Its transport hook is installed only by the opt-in fixture; normal execution uses real drivers.
- `tools/review-multiplayer-console.ps1 -Mode Online`: existing online protocol fixtures, updated only for canonical screen routing/track feedback.
- `tools/review-multiplayer-console.ps1 -Mode Local`: existing ad-hoc protocol/progression fixtures, updated for the single deck entry. Three pre-existing planetary allocation assertions remain a separate baseline failure group.
- `tools/review-multiplayer-live.ps1`: isolated portable PPSSPP configuration and loopback server, real console connection/heartbeat/return/disconnect/persistence sequence; does not alter the user's emulator settings or expose a public server.
- `server/test.mjs`: unchanged server security, capacity, presence, cross-system and stale-client checks.

Final evidence, artifact hash, actual test results and remaining limits are in `BUILD-2.5.357.txt`. Do not infer full-game or physical PSP certification from passing menu checks. The mixed shared checkout remains unpublished while Git pull is blocked by existing untracked music placeholders; preserve unrelated changes and player music.
