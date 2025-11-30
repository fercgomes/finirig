#!/bin/bash

# Parse version string into components
# Input: "1.0.0-alpha.1" or "1.0.0" or "v1.0.0"
# Output: MAJOR MINOR PATCH PRERELEASE BUILD

VERSION="$1"

if [ -z "$VERSION" ]; then
    echo "Usage: $0 <version>" >&2
    exit 1
fi

# Remove 'v' prefix if present
VERSION="${VERSION#v}"

# Split version into parts
IFS='-' read -r BASE PRERELEASE_BUILD <<< "$VERSION"
IFS='.' read -r MAJOR MINOR PATCH <<< "$BASE"

# Handle prerelease and build metadata
PRERELEASE=""
BUILD=""

if [ -n "$PRERELEASE_BUILD" ]; then
    IFS='+' read -r PRERELEASE BUILD <<< "$PRERELEASE_BUILD"
fi

# Default values
MAJOR=${MAJOR:-0}
MINOR=${MINOR:-0}
PATCH=${PATCH:-0}
PRERELEASE=${PRERELEASE:-""}
BUILD=${BUILD:-""}

echo "$MAJOR $MINOR $PATCH $PRERELEASE $BUILD"

