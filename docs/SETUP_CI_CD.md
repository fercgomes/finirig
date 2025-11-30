# CI/CD Setup Guide

## Prerequisites Checklist

### 1. GitHub Repository

- [ ] Repository exists on GitHub
- [ ] Repository is initialized with your code
- [ ] All files are committed and pushed

### 2. GitHub Actions Permissions

GitHub Actions should work automatically, but verify:

- [ ] Go to **Settings** → **Actions** → **General**
- [ ] Under "Workflow permissions", select:
  - ✅ **Read and write permissions** (needed for releases)
  - ✅ **Allow GitHub Actions to create and approve pull requests** (optional)

### 3. Required Files (Already Created)

These files are already in your repository:

- ✅ `.github/workflows/ci.yml` - CI workflow
- ✅ `.github/workflows/release.yml` - Release workflow
- ✅ `VERSION` - Version file (currently `1.0.0-alpha.1`)
- ✅ `CHANGELOG.md` - Release notes
- ✅ `scripts/bump_version.sh` - Version bumping script

### 4. Initial Setup Steps

#### Step 1: Push Everything to GitHub

```bash
# Make sure all files are committed
git add .
git commit -m "Add CI/CD workflows and versioning system"
git push origin main
```

#### Step 2: Verify Workflows Are Active

1. Go to your GitHub repository
2. Click on **Actions** tab
3. You should see "CI" and "Release Build" workflows
4. They should be ready to run (no errors shown)

#### Step 3: Test CI Workflow

The CI workflow runs automatically on push. To test:

```bash
# Make a small change and push
echo "# Test" >> README.md
git add README.md
git commit -m "Test CI workflow"
git push origin main
```

Check the **Actions** tab - you should see the CI workflow running.

### 5. Creating Your First Release

#### Option A: Automatic Release (Recommended)

```bash
# 1. Bump version (if needed)
./scripts/bump_version.sh alpha

# 2. Update CHANGELOG.md with changes

# 3. Commit and push
VERSION=$(cat VERSION)
git add VERSION CHANGELOG.md
git commit -m "Bump version to ${VERSION}"

# 4. Create and push tag
git tag -a "v${VERSION}" -m "Release ${VERSION}"
git push origin main
git push origin "v${VERSION}"
```

GitHub Actions will automatically:

- Build for all platforms
- Create packages
- Upload to GitHub Releases

#### Option B: Manual Release

1. Go to **Actions** → **Release Build**
2. Click **Run workflow**
3. Enter version: `1.0.0-alpha.1` (or your desired version)
4. Click **Run workflow**

### 6. Verify Release

After the workflow completes:

1. Go to **Releases** in your GitHub repository
2. You should see a new release with:
   - Version tag
   - Release notes
   - Downloadable artifacts for macOS, Windows, Linux

### 7. Optional: Branch Protection

To ensure releases only come from main:

1. Go to **Settings** → **Branches**
2. Add rule for `main` branch:
   - ✅ Require pull request reviews
   - ✅ Require status checks to pass (select CI workflow)
   - ✅ Require branches to be up to date

## Troubleshooting

### Workflows Not Showing Up

- **Issue**: Workflows don't appear in Actions tab
- **Fix**:
  - Ensure `.github/workflows/` directory is committed
  - Check that files are in the correct location
  - Verify YAML syntax is valid

### Build Fails

- **Issue**: Build fails in GitHub Actions
- **Fix**:
  - Check workflow logs for specific errors
  - Verify all dependencies are available
  - Ensure CMake configuration is correct
  - Test build locally first

### Release Not Created

- **Issue**: Release workflow runs but no release is created
- **Fix**:
  - Check that `GITHUB_TOKEN` has write permissions
  - Verify tag doesn't already exist
  - Check workflow logs for errors
  - Ensure version format is correct

### Version Not Parsing

- **Issue**: Version parsing fails
- **Fix**:
  - Ensure `VERSION` file exists and is committed
  - Check version format matches expected pattern
  - Verify tag format (should start with `v`)

## Quick Test

To verify everything works:

```bash
# 1. Check version
cat VERSION
# Should show: 1.0.0-alpha.1

# 2. Test version bump
./scripts/bump_version.sh alpha
cat VERSION
# Should show: 1.0.0-alpha.2


# 3. Reset to original
echo "1.0.0-alpha.1" > VERSION

# 4. Verify workflows exist
ls -la .github/workflows/
# Should show: ci.yml, release.yml

# 5. Push to trigger CI
git add .
git commit -m "Test CI"
git push origin main
```

## Next Steps

Once setup is complete:

1. ✅ CI will run on every push/PR
2. ✅ Create releases by pushing tags
3. ✅ Artifacts automatically uploaded to GitHub Releases
4. ✅ Alpha/beta releases marked as prereleases

## Current Status

- **Version**: `1.0.0-alpha.1`
- **Workflows**: ✅ Created
- **Scripts**: ✅ Ready
- **Next**: Push to GitHub and create your first release!
