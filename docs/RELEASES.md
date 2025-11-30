# Release Process

## Versioning System

Finirig uses [Semantic Versioning](https://semver.org/) with pre-release identifiers:

- **Format**: `MAJOR.MINOR.PATCH[-PRERELEASE[.BUILD]]`
- **Current**: `1.0.0-alpha.1` (alpha stage)

### Version Stages

1. **Alpha** (`-alpha.N`): Early development, features may be incomplete
2. **Beta** (`-beta.N`): Feature complete, testing phase
3. **RC** (`-rc.N`): Release candidate, final testing before release
4. **Release** (no suffix): Stable production release

## Creating a Release

### Automatic Release (Recommended)

1. **Bump version:**
   ```bash
   ./scripts/bump_version.sh alpha  # or patch, minor, major, beta, rc
   ```

2. **Update CHANGELOG.md** with changes for this version

3. **Commit and push:**
   ```bash
   git add VERSION CHANGELOG.md
   git commit -m "Bump version to $(cat VERSION)"
   git push
   ```

4. **Create and push tag:**
   ```bash
   VERSION=$(cat VERSION)
   git tag -a "v${VERSION}" -m "Release ${VERSION}"
   git push origin "v${VERSION}"
   ```

5. **GitHub Actions will automatically:**
   - Build for macOS, Windows, and Linux
   - Create distribution packages
   - Upload to GitHub Releases
   - Mark as draft/prerelease if alpha/beta

### Manual Release

1. Go to GitHub Actions → "Release Build"
2. Click "Run workflow"
3. Enter version (e.g., `1.0.0-alpha.2`)
4. Click "Run workflow"

## Release Artifacts

Each release produces:

### macOS
- **DMG**: `Finirig-{VERSION}-macOS.dmg`
  - Contains `Finirig.app` bundle
  - Drag to Applications to install

### Windows
- **ZIP**: `Finirig-{VERSION}-Windows-x64.zip`
  - Contains `finirig_standalone.exe`
  - Extract and run

### Linux
- **TAR.GZ**: `Finirig-{VERSION}-Linux-x64.tar.gz`
  - Contains binary, icon, desktop file, and install script
  - Extract and run `./install.sh` or install manually

## Version Bumping Examples

```bash
# Current: 1.0.0-alpha.1

# Next alpha release
./scripts/bump_version.sh alpha
# Result: 1.0.0-alpha.2

# Move to beta
./scripts/bump_version.sh beta
# Result: 1.0.0-beta.1

# Next beta
./scripts/bump_version.sh beta
# Result: 1.0.0-beta.2

# Release candidate
./scripts/bump_version.sh rc
# Result: 1.0.0-rc.1

# Final release (remove prerelease)
./scripts/bump_version.sh patch
# Result: 1.0.1 (if starting from 1.0.0-rc.1)

# Or for first stable release
./scripts/bump_version.sh patch
# Result: 1.0.0 (if starting from 1.0.0-rc.1, but you'd typically just remove the suffix)
```

## Pre-release vs Release

- **Alpha/Beta/RC**: Marked as "prerelease" on GitHub
- **Release**: Marked as "latest release" on GitHub

Alpha and beta releases are automatically marked as drafts/prereleases.

## Release Checklist

- [ ] Update `VERSION` file
- [ ] Update `CHANGELOG.md` with changes
- [ ] Run tests locally
- [ ] Commit version bump
- [ ] Create and push git tag
- [ ] Verify GitHub Actions build succeeds
- [ ] Verify artifacts are uploaded to GitHub Releases
- [ ] Test downloaded artifacts on each platform
- [ ] Announce release (if applicable)

