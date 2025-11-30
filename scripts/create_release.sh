#!/bin/bash

# Automated release script for Finirig
# Usage: ./scripts/create_release.sh [bump_type] [--skip-bump] [--skip-changelog] [--dry-run]

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
VERSION_FILE="$PROJECT_ROOT/VERSION"
CHANGELOG_FILE="$PROJECT_ROOT/CHANGELOG.md"
BUMP_SCRIPT="$SCRIPT_DIR/bump_version.sh"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Parse arguments
BUMP_TYPE=""
SKIP_BUMP=false
SKIP_CHANGELOG=false
DRY_RUN=false

while [[ $# -gt 0 ]]; do
    case $1 in
        --skip-bump)
            SKIP_BUMP=true
            shift
            ;;
        --skip-changelog)
            SKIP_CHANGELOG=true
            shift
            ;;
        --dry-run)
            DRY_RUN=true
            shift
            ;;
        major|minor|patch|alpha|beta|rc)
            BUMP_TYPE="$1"
            shift
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            echo "Usage: $0 [bump_type] [--skip-bump] [--skip-changelog] [--dry-run]"
            echo "  bump_type: major, minor, patch, alpha, beta, rc"
            exit 1
            ;;
    esac
done

# Helper functions
log_info() {
    echo -e "${BLUE}ℹ${NC} $1"
}

log_success() {
    echo -e "${GREEN}✓${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}⚠${NC} $1"
}

log_error() {
    echo -e "${RED}✗${NC} $1"
}

# Check prerequisites
check_prerequisites() {
    log_info "Checking prerequisites..."
    
    if [ ! -f "$VERSION_FILE" ]; then
        log_error "VERSION file not found at $VERSION_FILE"
        exit 1
    fi
    
    if [ ! -f "$BUMP_SCRIPT" ]; then
        log_error "bump_version.sh not found at $BUMP_SCRIPT"
        exit 1
    fi
    
    if ! command -v git &> /dev/null; then
        log_error "git is not installed"
        exit 1
    fi
    
    # Check if we're in a git repository
    if ! git rev-parse --git-dir > /dev/null 2>&1; then
        log_error "Not in a git repository"
        exit 1
    fi
    
    # Check if there are uncommitted changes
    if [ -n "$(git status --porcelain)" ] && [ "$SKIP_BUMP" = false ]; then
        log_warning "You have uncommitted changes. They will be included in the release commit."
        read -p "Continue? (y/N) " -n 1 -r
        echo
        if [[ ! $REPLY =~ ^[Yy]$ ]]; then
            log_info "Release cancelled"
            exit 0
        fi
    fi
    
    # Check if we're on main/master branch
    CURRENT_BRANCH=$(git branch --show-current)
    if [[ "$CURRENT_BRANCH" != "main" && "$CURRENT_BRANCH" != "master" ]]; then
        log_warning "You're not on main/master branch (currently on: $CURRENT_BRANCH)"
        read -p "Continue anyway? (y/N) " -n 1 -r
        echo
        if [[ ! $REPLY =~ ^[Yy]$ ]]; then
            log_info "Release cancelled"
            exit 0
        fi
    fi
    
    log_success "Prerequisites check passed"
}

# Get current version
get_current_version() {
    cat "$VERSION_FILE" | tr -d '[:space:]'
}

# Bump version
bump_version() {
    if [ "$SKIP_BUMP" = true ]; then
        log_info "Skipping version bump (--skip-bump)"
        return
    fi
    
    if [ -z "$BUMP_TYPE" ]; then
        CURRENT_VERSION=$(get_current_version)
        echo ""
        log_info "Current version: $CURRENT_VERSION"
        echo ""
        echo "Select bump type:"
        echo "  1) alpha   - Increment alpha version (1.0.0-alpha.1 → 1.0.0-alpha.2)"
        echo "  2) beta    - Switch to beta (1.0.0-alpha.1 → 1.0.0-beta.1)"
        echo "  3) rc      - Switch to release candidate (1.0.0-alpha.1 → 1.0.0-rc.1)"
        echo "  4) patch   - Release version (1.0.0-alpha.1 → 1.0.0)"
        echo "  5) minor   - Minor release (1.0.0 → 1.1.0)"
        echo "  6) major   - Major release (1.0.0 → 2.0.0)"
        echo "  7) skip    - Keep current version"
        echo ""
        read -p "Enter choice [1-7]: " choice
        
        case $choice in
            1) BUMP_TYPE="alpha" ;;
            2) BUMP_TYPE="beta" ;;
            3) BUMP_TYPE="rc" ;;
            4) BUMP_TYPE="patch" ;;
            5) BUMP_TYPE="minor" ;;
            6) BUMP_TYPE="major" ;;
            7) SKIP_BUMP=true ;;
            *)
                log_error "Invalid choice"
                exit 1
                ;;
        esac
    fi
    
    if [ "$SKIP_BUMP" = true ]; then
        log_info "Keeping current version"
        return
    fi
    
    log_info "Bumping version: $BUMP_TYPE"
    bash "$BUMP_SCRIPT" "$BUMP_TYPE"
    NEW_VERSION=$(get_current_version)
    log_success "Version bumped to: $NEW_VERSION"
}

# Update changelog
update_changelog() {
    if [ "$SKIP_CHANGELOG" = true ]; then
        log_info "Skipping changelog update (--skip-changelog)"
        return
    fi
    
    VERSION=$(get_current_version)
    
    if [ ! -f "$CHANGELOG_FILE" ]; then
        log_warning "CHANGELOG.md not found, creating it..."
        cat > "$CHANGELOG_FILE" << EOF
# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [$VERSION] - $(date +%Y-%m-%d)

### Added
- Initial release

EOF
        log_success "Created CHANGELOG.md"
        return
    fi
    
    # Check if version already exists in changelog
    if grep -q "## \[$VERSION\]" "$CHANGELOG_FILE"; then
        log_warning "Version $VERSION already exists in CHANGELOG.md"
        read -p "Update it? (y/N) " -n 1 -r
        echo
        if [[ ! $REPLY =~ ^[Yy]$ ]]; then
            log_info "Skipping changelog update"
            return
        fi
    else
        # Insert new version section after [Unreleased]
        if grep -q "## \[Unreleased\]" "$CHANGELOG_FILE"; then
            # Use sed to insert after [Unreleased]
            if [[ "$OSTYPE" == "darwin"* ]]; then
                # macOS sed
                sed -i '' "/## \[Unreleased\]/a\\
\\
## [$VERSION] - $(date +%Y-%m-%d)\\
\\
### Added\\
- \\
\\
### Changed\\
- \\
\\
### Fixed\\
- \\
\\
" "$CHANGELOG_FILE"
            else
                # Linux sed
                sed -i "/## \[Unreleased\]/a\\\n## [$VERSION] - $(date +%Y-%m-%d)\n\n### Added\n- \n\n### Changed\n- \n\n### Fixed\n- \n" "$CHANGELOG_FILE"
            fi
            log_success "Added version $VERSION to CHANGELOG.md"
        else
            # Prepend to file
            {
                echo "# Changelog"
                echo ""
                echo "All notable changes to this project will be documented in this file."
                echo ""
                echo "## [$VERSION] - $(date +%Y-%m-%d)"
                echo ""
                echo "### Added"
                echo "- "
                echo ""
                echo "### Changed"
                echo "- "
                echo ""
                echo "### Fixed"
                echo "- "
                echo ""
                echo "---"
                echo ""
                cat "$CHANGELOG_FILE"
            } > "$CHANGELOG_FILE.tmp" && mv "$CHANGELOG_FILE.tmp" "$CHANGELOG_FILE"
            log_success "Added version $VERSION to CHANGELOG.md"
        fi
    fi
    
    # Open editor for changelog (optional)
    if command -v ${EDITOR:-nano} &> /dev/null; then
        log_info "Opening CHANGELOG.md for editing..."
        read -p "Edit changelog now? (Y/n) " -n 1 -r
        echo
        if [[ ! $REPLY =~ ^[Nn]$ ]]; then
            ${EDITOR:-nano} "$CHANGELOG_FILE"
        fi
    fi
}

# Create git commit and tag
create_release() {
    VERSION=$(get_current_version)
    TAG="v${VERSION}"
    
    # Check if tag already exists
    if git rev-parse "$TAG" >/dev/null 2>&1; then
        log_error "Tag $TAG already exists!"
        read -p "Delete existing tag and continue? (y/N) " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            if [ "$DRY_RUN" = false ]; then
                git tag -d "$TAG" 2>/dev/null || true
                git push origin ":refs/tags/$TAG" 2>/dev/null || true
                log_success "Deleted existing tag $TAG"
            else
                log_info "[DRY RUN] Would delete tag $TAG"
            fi
        else
            log_info "Release cancelled"
            exit 0
        fi
    fi
    
    # Stage files
    log_info "Staging files..."
    if [ "$DRY_RUN" = false ]; then
        git add "$VERSION_FILE"
        if [ -f "$CHANGELOG_FILE" ]; then
            git add "$CHANGELOG_FILE"
        fi
        # Add any other uncommitted changes
        git add -u
    else
        log_info "[DRY RUN] Would stage: $VERSION_FILE"
        if [ -f "$CHANGELOG_FILE" ]; then
            log_info "[DRY RUN] Would stage: $CHANGELOG_FILE"
        fi
    fi
    
    # Check if there are changes to commit
    if [ -z "$(git diff --cached --name-only)" ]; then
        log_warning "No changes to commit"
    else
        # Create commit
        COMMIT_MSG="Release ${VERSION}"
        log_info "Creating commit: $COMMIT_MSG"
        if [ "$DRY_RUN" = false ]; then
            git commit -m "$COMMIT_MSG"
            log_success "Created commit"
        else
            log_info "[DRY RUN] Would commit: $COMMIT_MSG"
        fi
    fi
    
    # Create tag
    log_info "Creating tag: $TAG"
    if [ "$DRY_RUN" = false ]; then
        git tag -a "$TAG" -m "Release ${VERSION}"
        log_success "Created tag $TAG"
    else
        log_info "[DRY RUN] Would create tag: $TAG"
    fi
    
    # Push to remote
    log_info "Pushing to remote..."
    if [ "$DRY_RUN" = false ]; then
        git push origin "$(git branch --show-current)"
        git push origin "$TAG"
        log_success "Pushed to remote"
    else
        log_info "[DRY RUN] Would push branch and tag $TAG"
    fi
}

# Main execution
main() {
    echo ""
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    echo "                    Finirig Release Script"
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    echo ""
    
    if [ "$DRY_RUN" = true ]; then
        log_warning "DRY RUN MODE - No changes will be made"
        echo ""
    fi
    
    check_prerequisites
    echo ""
    
    CURRENT_VERSION=$(get_current_version)
    log_info "Current version: $CURRENT_VERSION"
    echo ""
    
    bump_version
    echo ""
    
    NEW_VERSION=$(get_current_version)
    if [ "$NEW_VERSION" != "$CURRENT_VERSION" ]; then
        log_success "Version updated: $CURRENT_VERSION → $NEW_VERSION"
    fi
    echo ""
    
    update_changelog
    echo ""
    
    log_info "Ready to create release: $NEW_VERSION"
    echo ""
    read -p "Continue with release? (Y/n) " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Nn]$ ]]; then
        log_info "Release cancelled"
        exit 0
    fi
    echo ""
    
    create_release
    echo ""
    
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    log_success "Release $NEW_VERSION created successfully!"
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    echo ""
    log_info "GitHub Actions will now build and publish the release"
    log_info "Check progress at: https://github.com/$(git remote get-url origin | sed 's/.*github.com[:/]\([^.]*\).*/\1/')/actions"
    echo ""
}

# Run main function
main

