# Combat feedback and close-space effects

- Actual NPC/freighter weapon fire records a transient 2.5-second bearing per
  attacker. Thin chevrons track current camera yaw/pitch/roll; two chevrons mark
  a rear source. Incoming missile sources also qualify. Dead contacts, expired
  fire and system transitions clear hints. Solar/collision damage does not
  invent a ship attacker. HUD modes retain the small indicators.
- Player missile starts ahead/right of the ship facing forward. It accelerates
  out for 0.18 seconds, then turns at a bounded rate toward the locked ship.
  Twelve world-space trail samples show exhaust. Segment collision avoids
  stepping through a target. Existing ammo, range, lifespan and legal rules
  remain; launch speed keeps ahead of the ship even when boosting.
- Distant point-rendered ships have tapered faction-tinted rear engine streaks.
  Endpoints are view-clipped and roots remain attached to hull rear positions.
- Sun lens streak grows with apparent radius and forward alignment; larger
  soft optical ghosts remain bounded, and high contrast retains its FX opt-out.
- Solar exposure grows quadratically within 5,200m of the photosphere. At high
  heat it drains shields then hull, using thermal rather than attack damage.
- Planet sprite enlargement precomputes horizontal samples and tints each
  source row once, removing divisions/tint conversion from the per-pixel loop.
  Pixel-reference tests compare large and clipped planet renders exactly.

No external assets or save-format changes. New state is transient.
Tests: forward/side/rear missile paths at 60/20Hz, muzzle clearance, direction
after player turning, expired-source removal, solar damage and pixel parity.
Native captures: launch stages, incoming bearings, distant trails, close sun.

Verified ../../work/smoke-20260927-142958-646: all five groups RESULT 0 failures;55.45 FPS average,worst150.15ms,25 frames>25ms,planetary scenes >=24 FPS. Forward/side/rear missile paths, camera-relative/expired fire hints, solar shield/hull damage and exact enlarged-planet pixel parity pass. Native missile stages, bearing arrows, distant trails and close sun inspected. SHA256 167367168E4707BB1444E0DE1CCAC259B0D949EF20A6227D0811ED20EE15A541. Local-only; no commit/push/release. Highest priority: physical PSP close-planet performance and effect review, plus earlier unresolved landing shutdown. Physical PSP performance and gameplay review recommended;
the earlier planetary landing shutdown remains a separate unresolved report.
