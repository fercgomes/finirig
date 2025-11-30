# CI/CD Documentation

## Overview

Finirig uses GitHub Actions for continuous integration and automated releases.

## Workflows

### 1. CI Workflow (`ci.yml`)

**Triggers:**
- Push to `main` or `develop` branches
- Pull requests to `main` or `develop`

**What it does:**
- Builds and tests on macOS, Linux, and Windows
- Ensures code compiles on all platforms
- Runs test suite

**Status:** Must pass before merging PRs

### 2. Release Workflow (`release.yml`)

**Triggers:**
- Push a tag matching `v*` or `*.*.*` (e.g., `v1.0.0-alpha.1`)
- Manual workflow dispatch

**What it does:**
1. Determines version from tag or input
2. Builds production releases for all platforms
3. Creates distribution packages:
   - macOS: `.dmg` disk image
   - Windows: `.zip` archive
   - Linux: `.tar.gz` with install script
4. Uploads to GitHub Releases
5. Marks as draft/prerelease if alpha/beta/rc

## Quick Start: Creating a Release

### Step 1: Bump Version

```bash
# For next alpha release
./scripts/bump_version.sh alpha

# Or for patch/minor/major
./scripts/bump_version.sh patch
```

### Step 2: Update Changelog

Edit `CHANGELOG.md` with changes for this version.

### Step 3: Commit and Tag

```bash
VERSION=$(cat VERSION)
git add VERSION CHANGELOG.md
git commit -m "Bump version to ${VERSION}"
git tag -a "v${VERSION}" -m "Release ${VERSION}"
git push origin main
git push origin "v${VERSION}"
```

### Step 4: Wait for Build

GitHub Actions will automatically:
- Build for all platforms
- Create packages
- Upload to GitHub Releases
- Mark as prerelease if alpha/beta/rc

## Version Format

Current: `1.0.0-alpha.1`

Format: `MAJOR.MINOR.PATCH[-PRERELEASE[.BUILD]]`

Examples:
- `1.0.0-alpha.1` - Alpha release
- `1.0.0-beta.2` - Beta release
- `1.0.0-rc.1` - Release candidate
- `1.0.0` - Stable release

## Version Bumping

```bash
# Increment alpha build number
./scripts/bump_version.sh alpha
# 1.0.0-alpha.1 → 1.0.0-alpha.2

# Move to beta
./scripts/bump_version.sh beta
# 1.0.0-alpha.5 → 1.0.0-beta.1

# Increment patch version
./scripts/bump_version.sh patch
# 1.0.0 → 1.0.1

# Increment minor version
./scripts/bump_version.sh minor
# 1.0.0 → 1.1.0

# Increment major version
./scripts/bump_version.sh major
# 1.0.0 → 2.0.0
```

## Release Artifacts

Each release includes platform-specific packages:

### macOS
- **File**: `Finirig-{VERSION}-macOS.dmg`
- **Contains**: `Finirig.app` bundle
- **Install**: Drag to Applications folder

### Windows
- **File**: `Finirig-{VERSION}-Windows-x64.zip`
- **Contains**: `finirig_standalone.exe`
- **Install**: Extract and run

### Linux
- **File**: `Finirig-{VERSION}-Linux-x64.tar.gz`
- **Contains**: Binary, icon, desktop file, install script
- **Install**: Extract and run `./install.sh`

## Manual Release

If you need to create a release manually:

1. Go to **Actions** → **Release Build**
2. Click **Run workflow**
3. Enter version (e.g., `1.0.0-alpha.2`)
4. Click **Run workflow**

## Troubleshooting

### Build Fails

- Check GitHub Actions logs
- Verify all dependencies are available
- Ensure CMake configuration is correct

### Version Not Updating

- Check `VERSION` file exists and is committed
- Verify tag format matches workflow triggers
- Check version parsing in workflow

### Release Not Created

- Verify `GITHUB_TOKEN` has release permissions
- Check if release already exists (tags must be unique)
- Review workflow logs for errors

## Best Practices

1. **Always bump version before tagging**
   ```bash
   ./scripts/bump_version.sh alpha
   git add VERSION CHANGELOG.md
   git commit -m "Bump version"
   ```

2. **Update CHANGELOG.md** with each release

3. **Test locally** before creating a release tag

4. **Use semantic versioning** consistently

5. **Tag releases** with `v` prefix: `v1.0.0-alpha.1`

## Current Status

- **Version**: 1.0.0-alpha.1
- **Stage**: Alpha
- **CI**: ✅ Configured
- **Releases**: ✅ Automated

