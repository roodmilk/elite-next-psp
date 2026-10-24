from pathlib import Path
root=Path(__file__).resolve().parents[1]
entry='''## 2.5.358 — Observation Lounge Command Deck

Built on local 2.5.357, preserving its Multiplayer console and prior native UI updates. Implements approved 480x272 6D layout: original logo, green docking status with animated ship icon, glass service buttons, distinct teal category focus and amber service focus, native readable paintbrush/spanner icons, bounded description card and fixed footer funds. Left/Right and L/R shoulders slide between remembered categories; service openings flash briefly, slide and reveal the destination within 340ms. A new input interrupts cosmetic transitions without delaying Back. Station art stays still with twinkling port lights; star glints, nebula glow and restrained native sun shimmer animate. Opening highlights now follow the actual new row geometry. Real availability, actions, networking and V32 saves remain authoritative.

Implementation/coordinates: docs/COMMAND-DECK-358.md. Final validation, captures and EBOOT hash: BUILD-2.5.358.txt. Highest-priority remaining work: inherited broad failures and physical PSP input, sleep and two-device tests. This update is not whole-game certification. GitHub DNS is unavailable; preserve the mixed shared checkout and do not publish unrelated work wholesale.

'''
(root/'VERSION').write_text('2.5.358\n')
for name in ('build.ps1','build.sh'):
    p=root/name;s=p.read_text(encoding='utf-8-sig');assert 'ELITE: NEXT 2.5.357' in s or 'ELITE: NEXT 2.5.358' in s
    p.write_text(s.replace('ELITE: NEXT 2.5.357','ELITE: NEXT 2.5.358'),encoding='utf-8')
for name in ('README.md','CHANGELOG.md','CLAUDE-HANDOFF.md','docs/DESIGN-BIBLE.md','docs/DESIGN-BIBLE-2.0.md'):
    p=root/name;s=p.read_text(encoding='utf-8-sig')
    if not s.startswith('## 2.5.358'):p.write_text(entry+s,encoding='utf-8')
print('Updated 2.5.358 version strings and local handoff/design documentation.')
