# Modular pulp portraits

ArcElite uses one deterministic portrait renderer for the commander, ship
computer, conversations, station contacts, mission boards, wanted posters,
GalacticNet and Spacebook.  Its visual language translates painted 1950s and
1960s science-fiction paperbacks into dense authored pixel art: saturated alien
skies, coarse print-like dithering, strong ink shadows, warm painted highlights
and worn practical clothing.

Ninety-six authored busts are baked from the seven `pulp-*.png` production
sheets in `assets/source/portraits/` into native PSP ARGB1555 pixels: 16 human;
8 each Orrith, amphibian, insectoid, Sauran, avian and masked; 12 robot; 12
android; and 4 each crystal and fungal. Every portrait is assembled on a
canonical 64×64 canvas and
nearest-neighbour sampled into the caller's square, so dialogue portraits,
commander cards and tiny Spacebook avatars retain the same clusters rather than
being separately redrawn or stretched. The atlas is compiled into the EBOOT;
there is no runtime PNG decoding or filtering cost.

The renderer applies the creator fields to the authored pixels themselves:
eight hair/crest palette treatments, four eye treatments, four mouth/expression
treatments and four lower-uniform treatments. Alien morphologies reinterpret
the same fields as crest, shell, optic, mandible, casing or crystal variation.
These changes remain anchored to each bust and never place broad rectangles
across a face. Accessory DNA is retained for a future authored accessory sheet.

## Four-byte identity

The existing `commander_portrait` save field remains four bytes.  Its modular
format stores species, head, skin or shell, hair or crest, eyes, outfit,
accessory, pulp colour treatment and expression. Old values 0–7 are converted
to equivalent human DNA when displayed or edited, so existing saves remain
readable without a save-version change.

The DNA also reserves an embedded portrait-schema version and expansion bits.
Current modular portraits use schema zero and the original eight cards remain
valid. A future art pack can increment that schema and add a migration branch
instead of silently changing an old commander's appearance.

The ten species families are human, elongated Orrith, amphibian, insectoid,
reptilian Sauran, avian, spherical robot, humanoid android, masked/unknown and
crystal or fungal. Robots are full commander choices, not decorative accounts.

Seeded NPC DNA is stable and independent from employment: the same character
seed always rebuilds the same species, face and personal details. Faction is a
separate presentation layer, so changing allegiance never replaces the person.
Each role uses a small authored-looking chest pin and a faction-coloured lower
frame: cyan shield for Law, gold trade bars, red pirate slash or teal survey
compass. These marks deliberately stay below the face; accessories are not
drawn as geometric overlays over the authored art. This avoids storing full
bitmaps and gives the PSP millions of combinations from a small identity value.

## Adding future portrait content

Identity and presentation remain separate. New species geometry belongs in the
species section of `portrait_draw`; personal variants belong in the existing
head, surface, hair/crest, eye, outfit, accessory, colour or expression fields;
employment markings belong only in the faction-kit section. Increase the
matching count and label together, then extend the automated all-species test.

Schema zero has room for up to 16 species, four head shapes, eight surfaces,
eight hair/crest forms, four eye sets, four base outfits, eight accessories,
eight pulp palettes and four expressions. If a category ever needs to exceed
its allocated range, increment the embedded portrait schema and migrate the
older DNA in `portrait_normalize` rather than changing what an existing value
means. NPCs should continue to use a stable world/character seed: the same NPC
then keeps their face in conversations, mission boards, Spacebook and stations,
even if their role or clothing later changes.

## Commander creator

`COMMANDER > SAVE / STATUS > CREATE PORTRAIT` opens the editor. Up and Down
choose a part, Left and Right change it, Square randomizes the whole identity,
and Circle accepts it. The portrait is saved with the commander profile and is
used on save cards and the player's Spacebook profile.

The broad art-direction sheet is
`assets/source/portraits/modular-pulp-portrait-reference.png`. The production
sources are the seven `assets/source/portraits/pulp-*.png` sheets; run
`tools/bake-portrait-atlas.py` after changing them to regenerate
`src/portrait-atlas.h`.
