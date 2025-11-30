#!/bin/bash

# Generate platform-specific icons from icon.png
# Requires: sips (macOS), ImageMagick (Linux/Windows), or iconutil (macOS)

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
ICON_PNG="$PROJECT_ROOT/resources/icon.png"
RESOURCES_DIR="$PROJECT_ROOT/resources"

if [ ! -f "$ICON_PNG" ]; then
    echo "Error: icon.png not found at $ICON_PNG"
    exit 1
fi

echo "Generating platform-specific icons from icon.png..."

# macOS: Generate .icns file
if [[ "$OSTYPE" == "darwin"* ]]; then
    echo "Generating macOS .icns file..."
    
    ICONSET_DIR="$RESOURCES_DIR/Finirig.iconset"
    rm -rf "$ICONSET_DIR"
    mkdir -p "$ICONSET_DIR"
    
    # Generate all required sizes for .icns
    sips -z 16 16 "$ICON_PNG" --out "$ICONSET_DIR/icon_16x16.png" > /dev/null 2>&1
    sips -z 32 32 "$ICON_PNG" --out "$ICONSET_DIR/icon_16x16@2x.png" > /dev/null 2>&1
    sips -z 32 32 "$ICON_PNG" --out "$ICONSET_DIR/icon_32x32.png" > /dev/null 2>&1
    sips -z 64 64 "$ICON_PNG" --out "$ICONSET_DIR/icon_32x32@2x.png" > /dev/null 2>&1
    sips -z 128 128 "$ICON_PNG" --out "$ICONSET_DIR/icon_128x128.png" > /dev/null 2>&1
    sips -z 256 256 "$ICON_PNG" --out "$ICONSET_DIR/icon_128x128@2x.png" > /dev/null 2>&1
    sips -z 256 256 "$ICON_PNG" --out "$ICONSET_DIR/icon_256x256.png" > /dev/null 2>&1
    sips -z 512 512 "$ICON_PNG" --out "$ICONSET_DIR/icon_256x256@2x.png" > /dev/null 2>&1
    sips -z 512 512 "$ICON_PNG" --out "$ICONSET_DIR/icon_512x512.png" > /dev/null 2>&1
    sips -z 1024 1024 "$ICON_PNG" --out "$ICONSET_DIR/icon_512x512@2x.png" > /dev/null 2>&1
    
    # Create .icns from iconset
    iconutil -c icns "$ICONSET_DIR" -o "$RESOURCES_DIR/Finirig.icns" 2>/dev/null || {
        echo "Warning: iconutil failed, trying alternative method..."
        # Alternative: use sips to create a simple .icns
        cp "$ICON_PNG" "$RESOURCES_DIR/Finirig.icns"
    }
    
    rm -rf "$ICONSET_DIR"
    echo "✓ Generated Finirig.icns"
fi

# Windows: Generate .ico file (requires ImageMagick or use online converter)
if command -v convert &> /dev/null; then
    echo "Generating Windows .ico file..."
    convert "$ICON_PNG" -define icon:auto-resize=256,128,64,48,32,16 "$RESOURCES_DIR/Finirig.ico" 2>/dev/null && {
        echo "✓ Generated Finirig.ico"
    } || {
        echo "Warning: Could not generate .ico file (ImageMagick convert failed)"
        echo "  You can create Finirig.ico manually using an online converter"
    }
else
    echo "Note: ImageMagick not found. Skipping Windows .ico generation."
    echo "  Install ImageMagick or create Finirig.ico manually from icon.png"
fi

# Linux: Copy PNG (desktop files use PNG directly)
if [ -f "$ICON_PNG" ]; then
    cp "$ICON_PNG" "$RESOURCES_DIR/Finirig.png" 2>/dev/null || true
    echo "✓ Prepared Finirig.png for Linux"
fi

echo ""
echo "Icon generation complete!"
if [[ "$OSTYPE" != "darwin"* ]]; then
    echo ""
    echo "For macOS: Run this script on macOS to generate .icns"
fi
if ! command -v convert &> /dev/null; then
    echo ""
    echo "For Windows: Install ImageMagick or use an online converter to create Finirig.ico"
fi

