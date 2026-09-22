# GitHub storefront

The root `README.md` is the **player-facing download page**. Keep it download-first: hero, release CTA, install steps, screenshots, short feature list. Do not dump changelogs there — use `CHANGELOG.md` and `docs/DEVELOPMENT-HISTORY.md`.

## Assets

Images live in `assets/github/`:

| File | Role |
| --- | --- |
| `readme-hero.png` | README header composite |
| `social-banner.png` | 1280×640 Open Graph / social preview |
| `logo.png` | Pixel logo |
| `hero-intro.png` | Native title screen (2×) |
| `shot-flight.png` | Cockpit / docking |
| `shot-station*.png` | MacVenture station decks (Arrivals, Shop, Canteen, Guild) |
| `shot-story.png` | Campaign dialogue |
| `shot-galnet.png` | GalNet board |

Prefer **native 480×272** captures when available (see project store `media/macventure/` and `media/screenshots/`).

## Human checklist after merge

Agent tokens cannot edit repo metadata (GitHub API 403). Someone with admin/write on the repo must click:

### 1. Homepage URL

**Settings → General → Website**

```text
https://github.com/roodmilk/elite-next-psp/releases/latest
```

### 2. Topics

**Settings → General → Topics** (or the gear on the repo About panel):

```text
psp
homebrew
game
elite
retro
space-sim
ppsspp
```

### 3. Social preview

**Settings → General → Social preview → Edit**

Upload:

```text
assets/github/social-banner.png
```

### 4. Verify

- [ ] Repo home shows the new README (hero + Download badge)
- [ ] “Download latest” opens the newest Release
- [ ] ZIP and `EBOOT.PBP` are attached to that Release
- [ ] Shared link previews use the social banner (may take a few minutes to refresh)
- [ ] About panel shows homepage + topics

## Release notes

Tagged releases (`v*`) run `.github/workflows/release.yml`, which attaches the zip + EBOOT and opens with play/install instructions before the generated changelog.
