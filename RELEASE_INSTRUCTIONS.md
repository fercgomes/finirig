# How to Create a Release

## Quick Start (Automated - Recommended)

```bash
# Run the automated release script
./scripts/create_release.sh

# Or specify the bump type directly:
./scripts/create_release.sh alpha    # Bump to next alpha
./scripts/create_release.sh patch   # Release version
./scripts/create_release.sh minor    # Minor release
./scripts/create_release.sh major   # Major release

# Options:
./scripts/create_release.sh --skip-bump        # Keep current version
./scripts/create_release.sh --skip-changelog   # Skip changelog update
./scripts/create_release.sh --dry-run          # Preview without making changes
```

The script will:
1. ✅ Check prerequisites (git repo, uncommitted changes, etc.)
2. ✅ Bump version (interactive or via argument)
3. ✅ Update CHANGELOG.md
4. ✅ Create git commit
5. ✅ Create and push tag
6. ✅ Trigger GitHub Actions release workflow

## Manual Release (Alternative)

```bash
# Current version: 1.0.0-alpha.1

# Option 1: Bump to next alpha version
./scripts/bump_version.sh alpha
# This will change VERSION to: 1.0.0-alpha.2

# Option 2: Keep current version
# (skip the bump_version step)

# Then create the release:
VERSION=$(cat VERSION)
git add VERSION CHANGELOG.md
git commit -m "Release ${VERSION}"
git tag -a "v${VERSION}" -m "Release ${VERSION}"
git push origin main
git push origin "v${VERSION}"
```

## Version Bump Options

- `./scripts/bump_version.sh alpha` - Increment alpha (1.0.0-alpha.1 → 1.0.0-alpha.2)
- `./scripts/bump_version.sh beta` - Switch to beta (1.0.0-alpha.1 → 1.0.0-beta.1)
- `./scripts/bump_version.sh rc` - Switch to release candidate (1.0.0-alpha.1 → 1.0.0-rc.1)
- `./scripts/bump_version.sh patch` - Release version (1.0.0-alpha.1 → 1.0.0)
- `./scripts/bump_version.sh minor` - Minor release (1.0.0 → 1.1.0)
- `./scripts/bump_version.sh major` - Major release (1.0.0 → 2.0.0)

## What Happens After You Push the Tag

1. ✅ GitHub Actions automatically detects the tag
2. ✅ Builds for macOS, Windows, and Linux
3. ✅ Creates packages (DMG, ZIP, TAR.GZ)
4. ✅ Uploads to GitHub Releases
5. ✅ Marks as prerelease if version contains `-alpha`, `-beta`, or `-rc`

## Check Release Status

1. Go to **Actions** tab → **Release Build**
2. Watch the workflow run (takes ~10-15 minutes)
3. Go to **Releases** tab to see the new release

## Troubleshooting

- **Tag already exists?** Delete it: `git tag -d v1.0.0-alpha.1 && git push origin :refs/tags/v1.0.0-alpha.1`
- **Workflow not running?** Check that the tag format matches `v*` or `*.*.*`
- **Build failing?** Check the Actions logs for specific errors

