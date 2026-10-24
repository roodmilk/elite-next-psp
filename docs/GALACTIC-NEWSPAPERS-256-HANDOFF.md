# ELITE: NEXT — 256-System Newspaper Expansion

Status: approved implementation handoff 1.0  
Game baseline: ELITE: NEXT 2.5.344  
Target hardware: Sony PSP, native 480×272  
Purpose: implementation-ready specification for 256 distinctive newspapers in GalacticNet's NEWS section, each with its own readable and entertaining page count.

## 1. Product goal

Every inhabited or discoverable system receives a recognisable local publication. Opening the Gazette in Lave should feel different from opening it in Diso, Reorte or a remote frontier system. The difference must extend beyond replacing the system name: masthead, editorial voice, page furniture, colour, recurring columns, adverts, local institutions, humour and article subjects should all reflect the system.

The complete feature comprises:

- Exactly 256 deterministic newspapers: one assigned to every solar system.
- A deliberately chosen page count for each newspaper. Lave currently has 16 pages, but other papers may be shorter or longer.
- Most newspapers contain 8–16 pages. Exceptional formats may be shorter or longer when their concept justifies it.
- Hard limits are 4 pages minimum and 24 pages maximum.
- No visible `PAGE X OF Y`, `X / Y` or total-page-count label. Navigation should make it clear when the reader reaches either end.
- Multiple newspaper families, from reputable broadsheets to sensational gossip tabloids.
- Fixed local reporting and flavour that establish the system's culture, history and character.
- Native 480×272 layouts using the real 8×8 game font and integer pixel coordinates.
- No external bitmap page captures and no save-file bloat.

“Distinct” means each newspaper has its own name, pixel-art logo or masthead mark, editorial personality, recurring contributors, layouts, palette, local proper nouns, articles, adverts and illustrations. Important hubs receive additional handcrafted material.

This feature applies only to **GalacticNet → NEWS**. It does not replace Spacebook, Inbox, Markets, Warrants, Missions or any other GalacticNet section.

NEWS displays only the newspaper assigned to the player's current solar system. It is not a galaxy-wide newspaper browser.

## 2. Current implementation to replace and preserve

The present Gazette is implemented mainly in `src/ui-modern.h` and `src/gazette-lore.h`.

It currently provides:

- A 16-page Lave-style local edition.
- System-sensitive danger, economy, traffic and faction facts.
- Twenty-four tabloid lines.
- Eight longer feature stories.
- Eight adverts and eight small notices.
- A four-sector puzzle.
- One largely shared paper layout.

Preserve its working navigation and native-resolution renderer. Replace the narrow content banks, fixed page count and single visual identity with the system described below.

## 3. Deterministic identity model

Each system owns a fully authored `NewspaperIdentity`. A stable world seed may assist internal indexing, local fact selection or reproducible rendering, but it must not replace writing and design. The newspaper must never change between sessions, saves or world events.

Required identity fields:

```c
typedef struct {
    unsigned seed;
    unsigned publication_family;
    unsigned layout_family;
    unsigned masthead_style;
    unsigned palette;
    unsigned ink_style;
    unsigned border_style;
    unsigned headline_style;
    unsigned illustration_style;
    unsigned editorial_voice;
    unsigned political_lean;
    unsigned sensationalism;
    unsigned humour_level;
    unsigned advert_density;
    unsigned wear_level;
    unsigned page_order_variant;
    unsigned page_count;
    char masthead[32];
    char motto[48];
} NewspaperIdentity;
```

Identity selection must incorporate world facts rather than being purely random:

- Economy influences business coverage, advert density and paper quality.
- Government influences formality, censorship, official notices and political voice.
- Danger influences crime coverage, emergency strips and sensationalism.
- Technology influences page furniture, printing style and technical reporting.
- Faction influence changes column voices and the subjects treated sympathetically.
- Population, stations, planets and local landmarks inform the publication name.
- A writer and designer deliberately choose the final identity, using canonical system facts and stable identifiers.
- Every publication receives a readable bespoke pixel-art logo or masthead mark designed at its actual display size.

## 4. Publication families

Implement at least twelve clearly recognisable families. Every family needs its own tonal rules, headline vocabulary, preferred page modules and layout weighting.

1. **Civic broadsheet** — serious, balanced, dense columns, restrained colours.
2. **Dockworker daily** — practical traffic news, union notices, canteen gossip and blunt humour.
3. **Financial paper** — markets, freight, contracts, graphs and corporate adverts.
4. **Sensational tabloid** — giant headlines, dramatic borders, rumours, scandals and ridiculous certainty.
5. **Celebrity/gossip sheet** — society parties, famous pilots, romances, fashion and anonymous sources.
6. **Frontier circular** — rough print, survival advice, missing ships, handwritten notices and salvage news.
7. **Government record** — official language, proclamations, public works, census and law notices.
8. **Independent investigative paper** — evidence boxes, redactions, corruption stories and source protection.
9. **Scientific journal-newspaper** — discoveries, planet surveys, diagrams, field reports and cautious language.
10. **Pirate underground rag** — aliases, coded adverts, wanted stories, black humour and unreliable claims.
11. **Luxury society review** — elegant spacing, rich colours, travel, art, dining and ship customization.
12. **Religious/cultural chronicle** — festivals, memorials, traditions, philosophy and local history.

Rare variants may include a school paper, military dispatch, alien-interest magazine, sports paper, satirical paper, propaganda sheet or one-person newsletter.

Every newspaper has a named editor and a persistent group of local reporters, columnists, illustrators or correspondents. Their names, personalities and beats recur across that paper, giving it a recognisable editorial staff.

Publications may openly support, oppose or mock local governments and factions. Political bias must be legible as the newspaper's editorial position rather than presented as neutral game narration.

## 5. Variable-length newspaper structure

These are sixteen available editorial page roles, not a mandatory sixteen-page template. Each newspaper selects an appropriate subset and may repeat a role with completely different local material. Lave may retain sixteen pages; a recovered frontier paper may have only a handful of surviving pages; a major capital paper may be longer. Do not print the total number of pages anywhere in the interface.

The normal range is 8–16 pages. Do not pad a concise newspaper with weak filler, and do not cut a strong special publication merely to fit the normal range.

No newspaper may contain fewer than 4 or more than 24 pages.

1. **Front page** — lead story, secondary headline, teaser and hero illustration.
2. **Local system news** — stations, services, construction, traffic and civic events.
3. **Politics and factions** — government, faction influence, disputes and public statements.
4. **Economy and trade** — commodities, shortages, surpluses, freight and local businesses.
5. **Crime, law and safety** — warrants, piracy, patrols, hazards and safety advice.
6. **Science and exploration** — planets, stars, anomalies, species, rings and discoveries.
7. **People and society** — profiles, relationships, workplaces, celebrations and neighbourhood life.
8. **Gossip, rumours and scandal** — sensational or playful claims, anonymous tips and corrections.
9. **Travel and traffic** — routes, jump conditions, docking, tourism and neighbouring systems.
10. **Culture and entertainment** — music, bars, art, food, books, films and local events.
11. **Ships and technology** — ship models, engineering, outfitting, repairs and technical columns.
12. **Sport and competition** — races, zero-gravity sport, tournaments and eccentric local contests.
13. **Letters and opinion** — reader letters, editorials, arguments, corrections and advice.
14. **Classifieds and adverts** — jobs, sales, personals, missing items and unusual services.
15. **Puzzle, comic or diversion** — logic puzzle, cartoon, horoscope, quiz or serialized fiction.
16. **Back page** — a strong recurring local feature, large advert, sports result or human-interest story.

Pages should contain two to six content modules depending on layout. A page may have one large feature, several briefs, or a mixture. Never force every family into the same editorial balance: a gossip sheet may turn politics into scandal, while a scientific publication may replace sport with a field challenge.

Page composition should vary widely. Some pages contain a substantial properly written feature; others contain several short reports, cartoons, adverts, lists, notices or visual diagrams. Variation must exist both between newspapers and between pages of the same newspaper.

Several pages within a newspaper may cover the same local scandal, disaster, celebration, discovery or political dispute from different viewpoints. Reports must advance or reinterpret the subject rather than repeat the same paragraph.

Long major features may continue onto the following page. Continuations must use a clear `CONTINUED` treatment, retain the writer's byline and provide a clean reading break rather than splitting a sentence arbitrarily.

Uninhabited, dead or rogue systems may contain an old recovered newspaper, expedition journal, archived colony paper or automated exploration bulletin. Its age, damage, missing sheets and historical voice become part of its identity. It still counts as that system's single newspaper.

Recovered papers may contain torn corners, missing columns, burned areas, chemical damage, incomplete headlines and visibly absent pages. Damage must remain clear of essential controls, and enough surviving material must remain readable for the newspaper to feel rewarding rather than broken.

Do not stamp recovered papers with interface labels such as `ARCHIVED COPY`, `PARTIAL RECORD` or similar explanations. Their age and condition should be communicated through authored content and visual design.

## 6. Layout system

Build pages from bounded native-resolution layout recipes. Use exact integer coordinates. All text must be tested with worst-case strings.

### Shared screen bounds

- Screen: `0,0,480,272`.
- Existing game header/navigation region: preserve current safe area.
- Newspaper sheet recommended bounds: `6,43,468,205`.
- Minimum readable body text: real 8×8 font at 1× scale.
- Minimum gap between unrelated text blocks: 4 px vertically and 4 px horizontally.
- No body copy beneath the footer.
- Scroll rail must remain visible and must not cover article text.

### Required layout recipes

Implement at least sixteen genuinely different recipes:

1. Classic five-column broadsheet.
2. Banner headline with two lower columns.
3. Left hero illustration with right article stack.
4. Right hero illustration with left article stack.
5. Tabloid splash headline and inset gossip boxes.
6. Financial dashboard with ticker, graph and briefs.
7. Government notice with seal, proclamation and side annotations.
8. Investigative evidence board with document fragments and redaction bars.
9. Scientific plate with diagram labels and field notes.
10. Dock bulletin with arrivals board and pinned notices.
11. Society magazine with portrait frame and short columns.
12. Classified grid with many compact adverts.
13. Letters page with editor reply boxes.
14. Comic/puzzle page with one large visual and optional playable puzzle focus.
15. Sports results board with standings and feature story.
16. Minimal luxury layout with generous space and decorative rules.

Each recipe defines fixed rectangles for headline, deck, body, image, sidebar, statistic, caption, page furniture and advert. Content must be selected to fit the recipe; do not simply truncate arbitrary articles.

The system may mirror, reorder or slightly resize modules, but it must use curated recipes rather than uncontrolled procedural coordinates.

Do not impose one universal front page. Some papers use a large headline and hero illustration; others use several equal stories, a government proclamation, a financial table, a handwritten notice wall, an image-heavy society cover, a scientific plate or another family-appropriate opening design.

Graphic density is part of the identity. Some restrained papers may be mostly typography and rules, while playful publications may contain many readable native-size cartoons, icons, stamps, diagrams, adverts and decorative illustrations. Graphics must still respect the text masks and PSP readability limits.

## 7. Visual variation

### Palettes

Create at least 24 tested palettes. Each palette defines paper, primary ink, secondary ink, rule, highlight, warning and illustration colours. Examples:

- Warm cream with charcoal and rust.
- Cold white with navy and cyan.
- Cheap grey paper with black and faded red.
- Pale green administrative stock with dark green ink.
- Luxury midnight paper with cream and muted gold.
- Industrial tan with oil-blue accents.
- Tabloid white with aggressive red and yellow.
- Pirate black with bone ink and toxic green marks.

Colours must maintain readability on the PSP LCD. Do not use noisy textures beneath 8×8 text.

Colour usage varies substantially. Some publications may be vivid full-colour papers or magazines, while others use black-and-white, one spot colour, faded archival ink or restrained two-tone printing.

### Typography using the real game font

The project must continue using `src/font8.h`. Variation comes from treatment:

- 1× body copy.
- 2× or hand-assembled pixel headline glyphs for very short headlines.
- Bold simulation using a one-pixel overdraw only when it remains readable.
- Shadow, underline, reversed label strips and outlined headline boxes.
- Different tracking and capitalization conventions by publication family.
- Decorative masthead lettering may use small bespoke pixel glyph additions, but body copy remains the real font.

### Alien writing

Some alien-influenced newspapers may include headings, adverts, stamps, captions or sidebars in a fictional alien script. Create bespoke pixel-symbol banks drawn at their actual display size; do not rely on unsupported Unicode glyphs. Essential navigation and enough article content must remain readable in English. Alien text should feel intentionally typeset and culturally coherent, not like corrupted characters.

Alien writing remains untranslated. Do not add a translation button, tooltip or decoded-text mode. Its surrounding English layout must provide enough context for the page to remain enjoyable without explaining every symbol.

Human-readable prose uses polished British English. Alien publications may replace selected text modules with their bespoke symbol script, subject to the readability rules above.

### Paper and print character

Allow restrained deterministic wear:

- Fold line, registration offset, faded corner, ink stamp, coffee ring, staple, torn edge or handwritten correction.
- Wear must avoid all text and controls using reserved masks.
- Rich systems tend toward clean printing; frontier and underground papers may be rougher.
- Never animate noise across text.

### Unique mastheads and logos

Every one of the 256 newspapers receives a unique pixel-art logo or masthead mark. Similar publication families may share visual grammar, but never the same finished logo. Marks may include local planets, suns, station silhouettes, animals, faction motifs, tools, heraldry, abstract print symbols or typographic monograms. Design each logo at its actual on-screen size and certify that it remains identifiable at 1:1 PSP resolution.

All 256 systems receive carefully authored publication names and marks. Famous systems such as Lave, Diso, Reorte and Leesti receive additional lore attention, but minor systems are not allowed to remain generated filler.

Naming styles deliberately vary. Some titles use the system or station name, while others use distinctive cultural names based on a local landmark, historical phrase, profession, animal, astronomical feature or editorial attitude.

## 8. Pixel-art illustration system

Create modular native-size illustrations rather than shrinking large art. Required categories:

- Stars, planets, rings and moons.
- Station silhouettes by station type.
- Ship silhouettes by hull.
- Portraits using the game's portrait system.
- Faction emblems and government seals.
- Commodity crates and market icons.
- Crime-scene markers and warrant imagery.
- Bars, food, music and entertainment props.
- Sports symbols, race tracks and score markers.
- Weather, hazard and route diagrams.
- Advert products and small comic characters.

Newspaper photographs are bespoke readable pixel-art scenes, portraits and documentary images drawn for their exact native rectangles. Strong silhouettes, controlled palettes and uncluttered compositions take priority over tiny detail. Every photograph must remain understandable at 1:1 on the PSP screen.

Some publications contain recurring comic strips with persistent named characters, artists and visual identities. Strips may be funny, satirical, dramatic or culturally strange, and may recur across several pages of the same paper.

Illustrations inherit the current paper palette and ink style. They may use stippling, hatching or two-tone shading. Every art module must declare its native display size and safe caption region.

Local documentary art must accurately reflect the system's canonical planets, star colours, station types, ship hulls, species and landmarks. Stylisation is allowed, but the depicted subject must remain recognisable and must not contradict the actual game world.

Advertisements must name persistent fictional local businesses, bars, ship dealers, repair yards, restaurants, products and services. Their art and language should match the publication and system. Recurring local businesses may advertise on several pages without repeating identical copy.

## 9. Content-generation architecture

All newspaper content is written as coherent publication copy. Structured article records and authored banks may help store and render it safely, but the final headlines, decks, bylines, bodies, captions, adverts, comics and letters for all 256 newspapers must be deliberately authored and reviewed rather than emitted as unchecked procedural prose.

```c
typedef struct {
    char section[24];
    char headline[80];
    char deck[128];
    char byline[40];
    char body[768];
    char caption[96];
    unsigned tone;
    unsigned art_kind;
    int subject_system;
    int subject_body;
    int subject_faction;
} NewsArticle;
```

Writers may use these layers as planning inputs:

1. Publication-family voice.
2. Page-role subject.
3. System facts.
4. Canonical background state for the system.
5. Named local people, businesses and institutions.
6. Sentence variants and connective phrasing.
7. Headline, deck, byline and caption variants.

Each finished article must pass editorial, grammar, duplicate and fit checks. Never expose template tokens, internal IDs or awkward constructions.

Show a named journalist, columnist, wire service or editorial desk byline whenever appropriate. Very small notices, anonymous gossip, adverts and visual-only pieces may omit a personal byline deliberately.

### Uniqueness rule

Calculate a stable content signature for every generated page from masthead, logo, layout, palette, headline, lead subject, secondary subject, local proper nouns, advert set and illustration. Automated certification must confirm that every generated page signature is unique across the complete 256-paper collection.

Unique signatures alone are insufficient. Also detect excessive textual similarity:

- No two pages may share the same headline.
- No two systems may receive the same full lead paragraph.
- Reused sentence frames must substitute at least three meaningful local facts.
- The same long feature must not appear in more than one edition unless explicitly framed as a syndicated article with different local commentary.

## 10. Local names and recurring personalities

Author persistent system-local names for:

- Newspaper and motto.
- Editor, reporters, gossip columnist, illustrator and cartoonist.
- Station districts, bars, restaurants and shops.
- Sports teams and competitions.
- Unions, clubs, schools, charities and religious groups.
- Corporations, repair yards and freight operators.
- Local celebrities, officials and notorious figures.

These names must recur across relevant pages, making the publication feel coherent. Store compact authored records and shared rendering resources rather than full-screen page images.

Selected journalists, celebrities, cartoon characters and other newspaper personalities may occasionally appear elsewhere in the game. Such crossovers should feel deliberate and remain consistent with the character established in print.

Each system normally has its own independent publisher. Occasional syndicated columns, cartoons or wire reports may cross system boundaries, but they must be clearly credited and receive local framing. Syndication must never make two newspapers feel like the same publication.

## 11. Truth, bias and gossip

Every claim receives a truth class:

- Verified fact.
- Official claim.
- Analysis.
- Rumour.
- Satire.
- Advertisement.
- Deliberate propaganda.

The player should be able to understand the framing through labels and tone. Gossip papers may exaggerate stories about their own celebrities, politicians, pilots and local scandals. They must not invent gossip about the player commander; Spacebook owns player-focused social reactions.

Newspaper stories are world-building flavour, not mission or discovery hints. They must not unlock missions, point toward valuable locations or imply that a gameplay reward is waiting.

Sensational headlines should be funny and characterful rather than randomly cruel. Avoid copying real-world newspaper brands, slogans or living public figures.

Mild adult humour is allowed in gossip, nightlife, society and pirate publications. Keep it suggestive or cheeky rather than explicit, and do not use hateful or sexually graphic material.

The occasional words `damn` and `hell` are allowed where they fit a character or publication. Do not use `bastard`, stronger profanity or repeated swearing as decoration.

Unreliable or comic papers may print correction columns that admit earlier mistakes, contradict exaggerated headlines or create running jokes. Corrections must remain internally coherent and must not refer to content that does not exist in that newspaper.

Serious subjects such as disasters, deaths, wars and political corruption are allowed only when appropriate to the system, publication voice and established lore. Handle them with suitable tone rather than inserting grim material merely for variety.

## 12. Fixed publication content

Each system's newspaper is a fixed publication. Its content does not refresh after missions, discoveries, wanted levels, market changes or story events. This makes every newspaper an authored piece of permanent local culture rather than a live news feed.

It may be based on the system's canonical economy, government, danger, technology, factions, planets, stations, history and landmarks. Once generated or authored, those pages remain identical on every visit and after save/load.

Do not display a publication date. The paper is a timeless local edition, avoiding a false implication that its fixed stories should refresh.

Each newspaper must read as one coherent issue. Articles, letters, editorials, adverts and comics may refer to people, organizations or events established on earlier pages, provided every reference still makes sense when encountered on its own.

Newspapers may accurately reference established Elite lore, historical events, organizations and cultures where appropriate. Any such material must be checked against the project's lore sources and must not contradict the game's canonical world state.

Writers must paraphrase and create original newspaper prose. Do not copy sentences or substantial passages from Elite books, manuals, games, websites or other copyrighted sources.

## 13. Newspaper navigation and feedback

- Opening GalacticNet NEWS always starts on that system newspaper's front page.
- Up/Down turns newspaper pages.
- Left/Right remains reserved for changing GalacticNet sections and must never turn newspaper pages.
- Page navigation loops: Down on the final page returns to the front page, and Up on the front page moves to the final page.
- Circle returns.
- Do not display total pages or labels such as `PAGE 01 / 16`. Small Up/Down page indicators, tabs or a physical page stack may communicate navigation. Because navigation loops, both page-turn directions remain available on every page.
- Changing page receives a short paper-slide or page-turn animation of roughly 100–180 ms and a restrained paper-flip sound.
- Selected page indicator moves clearly.
- Avoid long blocking animations during repeated browsing.

All pages are immediately readable on arrival in the system. Do not lock articles behind missions, discoveries, reputation, story progress or other conditions.

Leaving NEWS discards the current page position. Reopening NEWS always returns to the local newspaper's front page.

Every one of the 256 systems has a functioning GalacticNet connection. Do not create offline, disconnected or unavailable NEWS states.

Optional interactions such as clipping an article or following a rumour remain outside this implementation.

### Puzzle invitation and zoom

Sudoku and crossword sections may be playable. When the player views a page containing one, animate a loose hand-drawn highlight circle around the puzzle area and place the proper PSP **X button icon** beside it. The stroke should draw on quickly like editorial pen or grease pencil, then settle into a restrained pulse. It must not cover clues, grid cells or surrounding article text. This visual alone should make `press X here` obvious at a glance.

Pressing X smoothly zooms the puzzle rectangle until it fills most of the 480×272 screen. Keep a faint portion of the newspaper visible around the edge so the player still feels that they are examining a printed page. While zoomed in, suspend newspaper page-turning and GalacticNet section navigation. Circle zooms back to the complete newspaper page.

### Crossword input

Crosswords use compact 7×7 or 9×9 grids and short answers suitable for PSP entry.

- D-pad on the grid chooses a clue.
- X opens that clue as a large row of answer slots with a compact alphabet grid beneath it.
- D-pad moves around the alphabet grid.
- X enters the selected letter.
- Square deletes the previous letter.
- L/R jumps between answer slots.
- Triangle switches between available Across and Down clues.
- Circle first closes letter entry, then exits Puzzle Mode when pressed again.
- Completing an answer fills all corresponding crossword cells automatically.

### Sudoku input

- D-pad moves between cells.
- X opens a large horizontal `1 2 3 4 5 6 7 8 9` strip.
- Left/Right chooses a number; the strip remembers the last chosen number.
- X places the number.
- Square erases the selected cell.
- Triangle toggles pencil-note mode.
- Circle closes the number strip, then exits Puzzle Mode when pressed again.

Selections require clear cell highlights, short press feedback and subtle correct/invalid cues. Invalid input may flash red briefly but must not interrupt play. Automatically preserve puzzle progress so leaving GalacticNet does not erase entered answers. Puzzles provide entertainment and completion acknowledgement, but no units, missions, discoveries or progression rewards.

Do not use two-page spreads. Every displayed PSP screen is one complete newspaper page with its own safe layout, even when neighbouring pages cover the same continuing story.

## 14. Performance and memory budget

- Generate one page at a time.
- Keep shared art procedural or in compact sprite banks.
- Do not allocate hundreds of full-screen newspaper images.
- Cache the current newspaper identity and current page article.
- Avoid per-frame string generation; regenerate only when entering the newspaper or changing page.
- Maintain the existing PSP frame budget and avoid heap fragmentation.
- Bound every string write and render operation.

## 15. Fully authored collection

All 256 newspapers are custom written and designed. Major story systems such as Lave, Diso, Reorte, Leesti and central story destinations receive additional lore attention. Authored material includes:

- Unique mastheads.
- Recurring named journalists.
- Handcrafted local front pages.
- Local advertisements and landmarks.
- Continuing serialized stories.
- Additional local history and recurring personalities.

All custom content must still use the common article and layout contracts so it remains safe and testable.

## 16. Required implementation stages

### Stage A — data and identity

- Introduce authored publication identities and local proper nouns for all 256 systems.
- Define families, palettes, page roles and layout recipes.
- Support variable page counts without showing totals.

### Stage B — renderer

- Implement all bounded layout recipes.
- Add modular native-size illustrations and print details.
- Add page-turn feedback.

### Stage C — content

- Write the complete headline, deck, body, byline, advert, letter, gossip, comic and puzzle collection.
- Connect canonical system background data without live player-state reporting.
- Give major lore systems additional editorial and design review.

### Stage D — certification

- Render every page from all 256 newspapers to native screenshots in an offline QA pass.
- Run fit, uniqueness, grammar-token and contrast checks.
- Manually inspect representatives of every family, palette, layout and extreme string case.
- Build and test the PSP EBOOT.

## 17. Automated acceptance tests

The completed implementation must prove:

- Exactly 256 systems produce exactly one newspaper each.
- Every newspaper exposes its configured number of pages and never displays the total.
- Every generated page signature is unique across the collection.
- No unresolved template tokens appear.
- No headline or body text crosses its assigned rectangle.
- No text overlaps art, page furniture, scrollbar or footer.
- Worst-case system, station and character names fit or wrap cleanly.
- Every palette passes the project's minimum readability test.
- System facts match canonical world state.
- Articles contain no mission, discovery or reward hooks.
- Missions and player actions do not alter newspaper copy.
- Save/load reproduces the same publication and content.
- Navigation reaches every page and returns safely.
- Generation remains deterministic across repeated runs.
- An editorial manifest identifies the writer/reviewer status of every newspaper and page; no placeholder or unchecked generated copy remains.
- Puzzle-page circles and X icons point to the correct rectangle without covering content.
- Entering Puzzle Mode zooms the correct puzzle and temporarily captures its controls.
- Crossword and Sudoku entry, deletion, clue/cell movement, pencil notes, exit and resume all work predictably.
- Puzzle progress survives leaving and reopening GalacticNet.
- Memory and frame-time budgets remain within PSP limits.

## 18. Visual review matrix

Capture at least:

- One page from every layout recipe.
- One edition from every publication family.
- Every palette at least once.
- Clean, worn, tabloid, luxury, government and pirate extremes.
- Systems with zero planets, many planets, multiple stars, high danger, low technology and unusual governments.
- Long system names and dense stories.
- Opening, middle and final pages from several differently sized newspapers.

Review every screenshot at actual 480×272 and at 1:1 pixel scale. Do not approve layouts by viewing enlarged screenshots alone.

Newspaper palettes and graphic styles belong to their publications and remain independent of the player's ship paint and UI colour theme.

Every individual page must receive a recorded manual 480×272 inspection before the collection is approved, not merely one representative page from each template.

## 19. Definition of done

The feature is complete when a player can travel through the galaxy, open each system's single newspaper and immediately recognise differences in place, culture and editorial personality; when variable-length papers, names, logos, layouts and articles remain deterministic; when every page passes uniqueness and native-resolution layout checks; and when the PSP build remains responsive and stable.

## 20. Confirmed creative direction

1. There are exactly 256 newspapers, one per system.
2. Page counts vary by newspaper; Lave currently has sixteen pages.
3. The UI does not list a newspaper's total page count.
4. Empty systems may use old recovered papers or expedition material.
5. Newspapers have cool individual names, logos and masthead treatments.
6. Newspapers remain fixed and do not update after player actions.
7. Gossip focuses on local fictional figures rather than the player commander.
8. Content mixes serious Elite lore with funny stories.
9. Articles do not provide mission, discovery or reward hints.
10. Important systems receive extra handcrafted content.
11. Most papers contain 8–16 pages; justified special formats may fall outside that range.
12. Page-turn indicators remain available because navigation loops between the opening and final pages.
13. Front pages vary by publication rather than always using one large-headline template.
14. Every paper has its own recurring named editor and contributors.
15. Adverts feature persistent fictional local businesses and products.
16. Strange formats such as handwritten sheets, pirate papers, propaganda, journals and damaged archives are encouraged.
17. Every newspaper has a unique pixel-art logo.
18. Layouts and graphic density vary substantially, including some publications with many fun native-size illustrations.
19. GalacticNet NEWS shows only the newspaper for the current system.
20. Alien-influenced papers may use bespoke fictional symbol glyphs, while navigation and sufficient content remain readable in English.
21. Newspapers may strongly support or criticize local governments and factions.
22. Mild adult humour is allowed, but explicit material is not.
23. Newspapers display no date.
24. Pages range from long feature writing to dense mixtures of briefs, cartoons, adverts and diagrams.
25. All 256 newspapers are custom written and designed; famous systems receive additional lore attention.
26. Alien script remains untranslated and decorative/contextual.
27. Publications range from vivid colour to black-and-white and limited-ink printing.
28. Newspaper photographs use readable native-size pixel-art scenes and portraits.
29. Some papers contain recurring comic strips with named characters.
30. Recovered papers may be visibly damaged or incomplete while retaining worthwhile readable content.
31. Multi-page local stories may present one event from several distinct viewpoints.
32. Established Elite lore may be used when verified against project lore sources.
33. Publishers are normally system-local and independent; rare syndicated pieces are clearly credited and locally framed.
34. Every newspaper forms one coherent issue with internal references across its pages.
35. Two-page spreads are prohibited; each PSP screen is one complete page.
36. Newspapers contain 4–24 pages, with 8–16 as the normal range.
37. Publication names range from system-based titles to imaginative local brands.
38. Human-readable copy uses polished British English; selected modules may use untranslated alien script.
39. Major features may continue cleanly onto the following page.
40. Articles show named bylines where appropriate.
41. Comic and unreliable papers may publish coherent humorous corrections.
42. Every page receives a recorded manual inspection at actual 480×272.
43. NEWS always opens on the local paper's front page.
44. Up/Down turns pages; Left/Right continues to change GalacticNet sections.
45. Page navigation loops between the first and final pages.
46. Page turns use a brief paper animation and sound.
47. Every page is immediately available without gameplay locks.
48. Serious subjects appear only when appropriate to the setting and publication.
49. Only occasional `damn` and `hell` are permitted; `bastard` and stronger profanity are excluded.
50. Sudoku and mini-crosswords may be playable through an X-labelled zoomed Puzzle Mode; they grant no gameplay rewards.
51. Recovered papers receive no explicit archive or partial-record interface label.
52. Selected newspaper personalities may occasionally appear elsewhere in the game.
53. Local newspaper art matches canonical planets, stations, ships, species and landmarks.
54. Elite lore may inform original prose, but source sentences and passages are never copied.
55. Newspaper colours remain independent of the player's ship and UI theme.
56. Leaving NEWS forgets the page position; reopening starts at the front page.
57. All 256 systems have functioning GalacticNet access.
58. Playable puzzle pages draw a readable hand-marked circle around the puzzle and show the proper PSP X icon.
59. Crossword answers use a focused alphabet grid; Sudoku uses a remembered 1–9 selection strip.
60. Puzzle progress is saved automatically and Puzzle Mode never steals controls outside its zoomed state.
