# Homeward tutorial 2.5.266

The first Earth conversation and its response choices play over the orbital artwork. Early Earth approaches, direct entry and swept high-speed crossings turn the ship out into space; Dad gives a nonblocking reminder. Only the scheduled countryside landing is permitted in Sol, and Earth closes again after departure. Old premature approach state is safely recovered.

The countryside stop now uses original Alpine pixel art: enormous mountain waterfall, distant snowy ranges, turquoise valley and wildflower meadow. The vista appears behind the walkable 3D meadow and fills the illustrated Dad/artifact conversations. The waterfall is background art, not a new traversable 3D waterfall or water simulation.

System Details uses the regular native animated almanac, with Sol text, familiar planets and eight-world catalogue. Physical Sol flight still uses the existing Sun plus Earth, Mars, Jupiter and Neptune subset; the other four planets appear in the almanac. The renderer clamps stale menu selections before indexing planet arrays, fixing a possible invalid access. GalacticNet uses shared native chrome, newspaper, Spacebook cards and market/job styling with Earth-specific data, family threads and the party photograph. Wanted stays absent in Sol.

Flight-control, approach, landing, walking and mining instructions are nonblocking. Steering and throttle work immediately. Their compact portrait/caption leaves the cockpit visible; no Cross acknowledgement is required. Authored story conversations and response choices retain their pacing. Departure progression also detects atmosphere transitions completed during a button-release frame.

Validation: exact packaged O3/LTO PSP binary tested in PPSSPP. Full tutorial passes for all three starter ships and both artifact choices; 268-node dialogue validation and save/checkpoint checks pass. Regression cases cover immediate control use, forbidden Earth entry, swept crossing, legal landing, post-stop re-entry, orbital conversation artwork and invalid almanac row recovery. Ordinary main-loop run passes New Game through Earth atmospheric entry, touchdown and EVA with ROCK audio and both VRAM planes. Core game, radio and steering checks pass. Reports are included in the download.

Limitations: physical PSP verification remains outstanding. The earlier broad legacy smoke failures (191 input assertions, performance threshold and null-framebuffer fixtures) were not resolved by this focused tutorial change. This is a local private build, not a public release or Git sync claim.

Installation: replace EBOOT.PBP in your game folder, keeping your saves and music. Choose NEW GAME and finish character creation to see the revised opening. Existing tutorial checkpoints keep their stage; completed control coaching does not require another acknowledgement.
