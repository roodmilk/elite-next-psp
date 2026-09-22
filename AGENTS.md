# ELITE: NEXT shared-development rules

This repository is the canonical project state for every human and AI contributor.
Chat history is useful context but is **not** project memory — update Git and the handoff docs before finishing a turn.

## Before changing anything

1. Run `git pull --rebase --autostash` and `git submodule update --init --recursive`.
2. Read `CLAUDE-HANDOFF.md` first (priority list + playable vs planned), then `README.md`, `VERSION`, and the newest `CHANGELOG.md` entry. `CLAUDE-START-PROMPT.txt` is the short bootstrap for new AI sessions.
3. Check `git status`. Do not overwrite another contributor's unfinished changes.
4. Treat implemented features and design goals separately. Verify code before claiming a feature exists.
5. Establish a baseline with `./build.ps1` + `./smoke-test.ps1` (Windows) or `./build.sh` + `./smoke-test.sh` (Linux).

## While working

- Keep the PSP limits in mind: 480x272 output, small readable text, bounded memory, and real-hardware input/audio behavior.
- Keep player music out of Git. Only the station folders and placeholder files belong in the repository.
- Update documentation whenever controls, behavior, build steps, missions, or save data change.
- Record a clear commit message that states the player-visible result. Git records the author and time.
- Never commit build objects, saves, temporary test output, credentials, or personal files.
- Prefer one focused branch per change (`cursor/<short-name>-…`), then merge only after smoke is green.

## Before handing work to another AI

1. Build and smoke-test until every group reports `RESULT 0 failures`.
2. Update `VERSION`, the version string in `build.ps1` (and `build.sh` if present), `CHANGELOG.md`, and `CLAUDE-HANDOFF.md` together. Refresh the handoff **Highest-priority remaining work** list so the next bot knows what to do.
3. Copy the tested `EBOOT.PBP` to `dist/ELITE-NEXT-PSP/EBOOT.PBP`.
4. Commit and push the source and documentation. Never leave the newest work only in a chat or local folder.
5. **Always** tag the tested commit as `v<version>` and push the tag (`git tag v…` then `git push origin v…`). The GitHub workflow creates the Release page with `ELITE-NEXT-PSP-v….zip` and `EBOOT.PBP` so the commander can download and play. A branch tip alone is not enough — Releases is the playable download.

## Multi-AI safety

- Pull before starting and before pushing. If the remote changed, rebase and resolve deliberately.
- Prefer one branch per contributor for simultaneous work, then merge only tested changes into `main`.
- GitHub is the source of truth. Chat history is useful context but is not project memory.
- Never say work is synced until `git status` is clean and the remote commit has been verified.
- Do not invent playable features from design-bible prose. If it is not in source + tests, say it is still planned.
