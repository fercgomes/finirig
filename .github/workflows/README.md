# GitHub Actions Workflows

## CI Workflow (`ci.yml`)

Runs on every push and pull request to `main` and `develop` branches.

**Jobs:**

- Build and test on macOS
- Build and test on Linux
- Build and test on Windows

**Purpose:** Ensure code compiles and tests pass on all platforms before merging.

## Release Workflow (`release.yml`)

Triggers when:

- A tag matching `v*` or `*.*.*` is pushed
- Manual workflow dispatch with version input

**Jobs:**

1. **Version**: Determines version from tag, VERSION file, or input
2. **Build**: Creates production builds for all platforms
3. **Package**: Creates distribution packages (DMG, ZIP, TAR.GZ)
4. **Release**: Uploads artifacts to GitHub Releases

**Platforms:**

- **macOS**: Creates `.app` bundle and `.dmg` disk image
- **Windows**: Creates `.exe` and `.zip` archive
- **Linux**: Creates binary and `.tar.gz` with install script

## Versioning

Version format: `MAJOR.MINOR.PATCH[-PRERELEASE[.BUILD]]`

Examples:

- `1.0.0` - Release version
- `1.0.0-alpha.1` - Alpha pre-release
- `1.0.0-beta.2` - Beta pre-release
- `1.0.0-rc.1` - Release candidate

### Bumping Versions

Use the `scripts/bump_version.sh` script:

```bash
# Bump patch version (1.0.0 -> 1.0.1)
./scripts/bump_version.sh patch

# Bump minor version (1.0.0 -> 1.1.0)
./scripts/bump_version.sh minor

# Bump major version (1.0.0 -> 2.0.0)
./scripts/bump_version.sh major

# Bump alpha version (1.0.0-alpha.1 -> 1.0.0-alpha.2)
./scripts/bump_version.sh alpha

# Bump beta version (1.0.0-beta.1 -> 1.0.0-beta.2)
./scripts/bump_version.sh beta

# Create release candidate (1.0.0-beta.5 -> 1.0.0-rc.1)
./scripts/bump_version.sh rc
```

### Creating a Release

1. **Update VERSION file:**

   ```bash
   ./scripts/bump_version.sh alpha  # or patch, minor, major, etc.
   ```

2. **Commit and push:**

   ```bash
   git add VERSION CHANGELOG.md
   git commit -m "Bump version to $(cat VERSION)"
   git push
   ```

3. **Create and push tag:**

   ```bash
   VERSION=$(cat VERSION)
   git tag -a "v${VERSION}" -m "Release ${VERSION}"
   git push origin "v${VERSION}"
   ```

4. **GitHub Actions will automatically:**
   - Build for all platforms
   - Create packages
   - Upload to GitHub Releases
   - Mark as draft/prerelease if alpha/beta

### Manual Release

You can also trigger a release manually:

1. Go to Actions → Release Build → Run workflow
2. Enter version (e.g., `1.0.0-alpha.1`)
3. Click "Run workflow"

## Release Artifacts

Each release includes:

- **macOS**: `Finirig-{VERSION}-macOS.dmg`
- **Windows**: `Finirig-{VERSION}-Windows-x64.zip`
- **Linux**: `Finirig-{VERSION}-Linux-x64.tar.gz`

All artifacts are uploaded to GitHub Releases and available for download.
