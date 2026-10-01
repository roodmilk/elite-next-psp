# 2.5.175 — Flight HUD labels and missile indicator

- Hold-Square targeting computer: L shoulder icon + ": NEXT", R shoulder icon + ": LOCK", including an empty contact list.
- Normal flight Triangle caption reads COMMS (input/hold behaviour unchanged).
- Removed bottom-bar MS count; the live missile number now appears beside a small missile silhouette to the right of WEP.
- Power bank spacing reduced from 46 to 36 pixels (SYS336, ENG372, WEP408). The selected bank highlight remains 36 pixels wide, separate from missile art at443..453 and count at456.
- Existing power selection tests moved to new coordinates; coverage remains all16 themes,3 HUD modes and both contrast settings. Added target-hint raster check and native flight/target-label captures.
- No save, ammunition or control behaviour changes. Final test results and hashes in CLAUDE-HANDOFF.md.
