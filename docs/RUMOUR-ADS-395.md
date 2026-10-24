# Rumours pop-up adverts — 2.5.395

24 distinct joke adverts, each with an authored 16×16 illustration displayed
as crisp 4×4 pixel blocks. Native 8×8 lettering sits in a 384×156 window.
The illustration, title, five-line copy and Triangle Close control have
separate reserved areas. The two-line Space Secrets introduction stays intact.

- First appearance: eight seconds of browsing.
- After dismissal: 22–35 seconds of further browsing before another.
- All 24 rotate before repeating while remaining in the same system.
- Opening: 140 ms grow, tiny overshoot, settle to exact native pixels.
- Triangle: 110 ms shrink from the currently visible size, including early
  dismissal. Repeated presses do not restart the animation.
- Distinct synthesized rising whoop and descending airy thwip; they use the
  existing effects mixer and settings. No recorded audio assets were added.
- Ads block X, browsing and tracking commands beneath the window. X must be
  released after dismissal. No units, cargo, navigation data or mission state
  change. These are fictional adverts, with no purchasing action.
- No adverts appear in SpacePay. A real offer selection takes priority over an
  advert due on that frame. Category L/R and Circle Back remain available.
- Leaving or reopening GalacticNet resets the initial clear browsing period.

No extra framebuffer, heap allocation or world simulation runs for this feature.
Only the short transitions scale; settled text and pictures use native drawing.
Illustrations are reproducible with `tools/generate-rumour-ads.py`.
`tools/review-rumour-ads.ps1 -Eboot <review EBOOT>` exercises real input, all
24 native pictures, money isolation, timing, exit routes, framebuffer guards,
render cost and the two synthesized sounds. QA code is excluded from retail.

Release reports, individual native screenshots and sound samples are in
`outputs/releases/2.5.395/test-evidence`. Physical PSP verification is pending.
Existing broader and legacy-save test failures are documented separately;
the release evidence identifies exactly which suites were run.
