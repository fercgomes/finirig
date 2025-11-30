#!/bin/bash

# Get version from VERSION file or git tag
# Priority: VERSION file > latest git tag > default

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
VERSION_FILE="$PROJECT_ROOT/VERSION"

if [ -f "$VERSION_FILE" ]; then
    VERSION=$(cat "$VERSION_FILE" | tr -d '[:space:]')
    echo "$VERSION"
elif [ -n "${GITHUB_REF}" ] && [[ "${GITHUB_REF}" == refs/tags/* ]]; then
    # GitHub Actions: use tag name
    VERSION="${GITHUB_REF#refs/tags/}"
    echo "$VERSION"
elif command -v git &> /dev/null && git rev-parse --git-dir > /dev/null 2>&1; then
    # Try to get latest tag
    TAG=$(git describe --tags --abbrev=0 2>/dev/null || echo "")
    if [ -n "$TAG" ]; then
        echo "$TAG"
    else
        # Fallback: use commit hash
        COMMIT=$(git rev-parse --short HEAD 2>/dev/null || echo "dev")
        echo "0.0.0-dev+${COMMIT}"
    fi
else
    echo "0.0.0-dev"
fi

