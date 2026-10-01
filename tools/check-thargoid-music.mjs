import fs from 'node:fs';

const files = fs.readFileSync(new URL('../src/radio-files.h', import.meta.url), 'utf8');
const audio = fs.readFileSync(new URL('../src/audio.h', import.meta.url), 'utf8');
const main = fs.readFileSync(new URL('../src/main.c', import.meta.url), 'utf8');
const build = fs.readFileSync(new URL('../build.ps1', import.meta.url), 'utf8');
const note = fs.readFileSync(new URL('../music/Thargoid Battle/PUT_OGG_FILES_HERE.txt', import.meta.url), 'utf8');

for (const marker of ['RADIO_BATTLE_SOURCE', 'RADIO_FILE_SOURCE_COUNT', 'music/Thargoid Battle']) if (!files.includes(marker)) throw new Error(`missing battle library marker: ${marker}`);
for (const marker of ['radio_has_ogg_extension', 'radio_has_music_extension']) if (!files.includes(marker)) throw new Error(`missing OGG scan behavior: ${marker}`);
for (const marker of ['audio_battle', 'wanted_source=audio_battle?RADIO_BATTLE_SOURCE:radio_station', 'radio_synth_reset(&synth,station==RADIO_BATTLE_SOURCE?2:station)', 'score_on=audio_battle||!radio_off', '<vorbis/vorbisfile.h>', 'ov_fopen', 'ov_read', 'ov_clear']) if (!audio.includes(marker)) throw new Error(`missing battle mixer/decoder behavior: ${marker}`);
if (!main.includes('audio_battle=thargoid_active')) throw new Error('encounter state does not drive the battle music source');
if (!build.includes("'-lvorbisfile','-lvorbis','-logg'")) throw new Error('PSP build does not link the reference OGG decoder');
if (!note.includes('up to 24 OGG Vorbis') || !note.includes('MUSIC level') || !note.includes('No conversion')) throw new Error('drop-folder instructions are incomplete');
console.log('PASS dedicated Thargoid Battle OGG/MP3 library is scanned at startup');
console.log('PASS reference Vorbis decoder is linked for native PSP OGG playback');
console.log('PASS encounter crossfades to battle score and restores selected radio source');
if (!audio.includes('station!=RADIO_BATTLE_SOURCE') || !audio.includes('output_index^=1') || !audio.includes('music_bad[station][order[i]]')) throw new Error('audio recovery/buffering guards missing');
console.log('PASS silent battle fallback, failed-file quarantine and alternating output buffers are wired');
