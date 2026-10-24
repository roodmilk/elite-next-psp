# ERL headquarters reopening - 2.5.412

Cause: the accepted RACE_GRID event handler returned before the HOME / service 31 command and before headquarters page navigation. The League button fell through to the generic deck handler, which has no legacy page mapping for service 31, producing no visible response.

Pending events now handle only beacon contact and their own brief, then allow the normal headquarters dispatcher to run. The League button validates HQ availability. Season, Driver, Rivals, Garage and Back remain navigable, with ordinary ERL selection motion. Active countdown/race/result ownership is unchanged.

A docked pending brief points to the existing start beacon without starting the countdown. READY requires being undocked and within the existing 5000-unit contact radius. Accepted brief rendering uses its saved layout/type and exact existing rules. Repeated reopening does not replace the contract/course or advance progression.

Regression adds full input-pipeline League-button reopening, repeated X/Circle, all four headquarters pages, animation selection, docked brief safety and exact pending serial/seed preservation to the 20 pre-start/sleep checks. Native 480x272 pending HQ capture and exact retail HQ startup/music are checked. Physical PSP confirmation remains pending.

Preserve 2.5.411 intro logo, 2.5.410 sleep recovery, Shockwave victory and Picross. No save-format change. Local release with frozen source; GitHub remains unchanged on the shared unfinished tree. Broader unfinished arcade/newspaper expansion remains paused.
