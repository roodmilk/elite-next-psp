# ELITE: NEXT shared-development rules

This repository is the canonical project state for every human and AI contributor.

## Before changing anything

1. Run `git pull --rebase --autostash` and `git submodule update --init --recursive`.
2. Read `README.md`, `CLAUDE-HANDOFF.md`, `VERSION`, and the newest `CHANGELOG.md` entry.
3. Check `git status`. Do not overwrite another contributor's unfinished changes.
4. Treat implemented features and design goals separately. Verify code before claiming a feature exists.

## While working

- Keep the PSP limits in mind: 480x272 output, small readable text, bounded memory, and real-hardware input/audio behavior.
- Keep player music out of Git. Only the station folders and placeholder files belong in the repository.
- Update documentation whenever controls, behavior, build steps, missions, or save data change.
- Record a clear commit message that states the player-visible result. Git records the author and time.
- Never commit build objects, saves, temporary test output, credentials, or personal files.

## Before handing work to another AI

1. Build with `./build.ps1` and run `./smoke-test.ps1`.
2. Update `VERSION`, the version in `build.ps1`, `CHANGELOG.md`, and `CLAUDE-HANDOFF.md` together.
3. Copy the tested `EBOOT.PBP` to `dist/ELITE-NEXT-PSP/EBOOT.PBP`.
4. Commit and push the source and documentation. Never leave the newest work only in a chat or local folder.
5. For a public downloadable build, tag the tested commit as `v<version>` and push the tag. The GitHub workflow creates the Release page and ZIP automatically.

## Multi-AI safety

- Pull before starting and before pushing. If the remote changed, rebase and resolve deliberately.
- Prefer one branch per contributor for simultaneous work, then merge only tested changes into `main`.
- GitHub is the source of truth. Chat history is useful context but is not project memory.
- Never say work is synced until `git status` is clean and the remote commit has been verified.
