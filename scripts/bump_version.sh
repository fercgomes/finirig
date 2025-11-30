#!/bin/bash

# Bump version in VERSION file
# Usage: ./bump_version.sh [major|minor|patch|alpha|beta|rc]

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
VERSION_FILE="$PROJECT_ROOT/VERSION"

if [ ! -f "$VERSION_FILE" ]; then
    echo "Error: VERSION file not found at $VERSION_FILE"
    exit 1
fi

CURRENT_VERSION=$(cat "$VERSION_FILE" | tr -d '[:space:]')
echo "Current version: $CURRENT_VERSION"

# Remove 'v' prefix if present
CURRENT_VERSION="${CURRENT_VERSION#v}"

# Parse version
IFS='-' read -r BASE PRERELEASE_BUILD <<< "$CURRENT_VERSION"
IFS='.' read -r MAJOR MINOR PATCH <<< "$BASE"

# Handle prerelease and build
PRERELEASE=""
BUILD=""

if [ -n "$PRERELEASE_BUILD" ]; then
    # Check if there's a '+' for build metadata (e.g., "alpha.1+abc123")
    if [[ "$PRERELEASE_BUILD" == *"+"* ]]; then
        IFS='+' read -r PRERELEASE_PART BUILD <<< "$PRERELEASE_BUILD"
    else
        PRERELEASE_PART="$PRERELEASE_BUILD"
        BUILD=""
    fi
    
    # Split prerelease part (e.g., "alpha.1" -> "alpha" and "1")
    if [[ "$PRERELEASE_PART" == *"."* ]]; then
        IFS='.' read -r PRERELEASE BUILD_NUM <<< "$PRERELEASE_PART"
        if [ -n "$BUILD_NUM" ]; then
            BUILD="$BUILD_NUM"
        fi
    else
        PRERELEASE="$PRERELEASE_PART"
    fi
fi

# Default values
MAJOR=${MAJOR:-0}
MINOR=${MINOR:-0}
PATCH=${PATCH:-0}

BUMP_TYPE="${1:-patch}"

case "$BUMP_TYPE" in
    major)
        MAJOR=$((MAJOR + 1))
        MINOR=0
        PATCH=0
        PRERELEASE=""
        BUILD=""
        ;;
    minor)
        MINOR=$((MINOR + 1))
        PATCH=0
        PRERELEASE=""
        BUILD=""
        ;;
    patch)
        PATCH=$((PATCH + 1))
        PRERELEASE=""
        BUILD=""
        ;;
    alpha)
        if [ -z "$PRERELEASE" ] || [ "$PRERELEASE" != "alpha" ]; then
            PRERELEASE="alpha"
            BUILD="1"
        else
            # Increment alpha build number
            BUILD_NUM=${BUILD:-1}
            # Handle case where BUILD might be empty or non-numeric
            if ! [[ "$BUILD_NUM" =~ ^[0-9]+$ ]]; then
                BUILD_NUM=1
            fi
            BUILD=$((BUILD_NUM + 1))
        fi
        ;;
    beta)
        if [ -z "$PRERELEASE" ] || [ "$PRERELEASE" != "beta" ]; then
            PRERELEASE="beta"
            BUILD="1"
        else
            # Increment beta build number
            BUILD_NUM=${BUILD:-1}
            if ! [[ "$BUILD_NUM" =~ ^[0-9]+$ ]]; then
                BUILD_NUM=1
            fi
            BUILD=$((BUILD_NUM + 1))
        fi
        ;;
    rc)
        if [ -z "$PRERELEASE" ] || [ "$PRERELEASE" != "rc" ]; then
            PRERELEASE="rc"
            BUILD="1"
        else
            # Increment rc build number
            BUILD_NUM=${BUILD:-1}
            if ! [[ "$BUILD_NUM" =~ ^[0-9]+$ ]]; then
                BUILD_NUM=1
            fi
            BUILD=$((BUILD_NUM + 1))
        fi
        ;;
    *)
        echo "Usage: $0 [major|minor|patch|alpha|beta|rc]"
        exit 1
        ;;
esac

# Build new version string
NEW_VERSION="${MAJOR}.${MINOR}.${PATCH}"
if [ -n "$PRERELEASE" ]; then
    if [ -n "$BUILD" ]; then
        NEW_VERSION="${NEW_VERSION}-${PRERELEASE}.${BUILD}"
    else
        NEW_VERSION="${NEW_VERSION}-${PRERELEASE}"
    fi
fi

echo "New version: $NEW_VERSION"
echo "$NEW_VERSION" > "$VERSION_FILE"
echo "Updated $VERSION_FILE"

