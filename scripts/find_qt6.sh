#!/bin/bash

# Script to find Qt6 installation path for VSCode configuration

echo "Searching for Qt6 installation..."

# Common macOS paths
QT_PATHS=(
    "/opt/homebrew/opt/qt@6"
    "/opt/homebrew/opt/qt6"
    "/usr/local/opt/qt@6"
    "/usr/local/opt/qt6"
    "$(brew --prefix qt@6 2>/dev/null)"
    "$(brew --prefix qt6 2>/dev/null)"
)

# Common Linux paths
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    QT_PATHS+=(
        "/usr/lib/x86_64-linux-gnu/cmake/Qt6"
        "/usr/lib/cmake/Qt6"
        "/usr/local/lib/cmake/Qt6"
    )
fi

for path in "${QT_PATHS[@]}"; do
    if [ -d "$path" ] && [ -d "$path/include" ]; then
        echo "Found Qt6 at: $path"
        echo ""
        echo "To use this in VSCode, update .vscode/c_cpp_properties.json:"
        echo "Add to includePath:"
        echo "  \"$path/include\","
        echo "  \"$path/include/QtCore\","
        echo "  \"$path/include/QtWidgets\","
        echo "  \"$path/include/QtGui\","
        echo ""
        echo "Or set CMAKE_PREFIX_PATH:"
        echo "  cmake .. -DCMAKE_PREFIX_PATH=$path"
        exit 0
    fi
done

echo "Qt6 not found in common locations."
echo ""
echo "Please install Qt6:"
echo "  macOS: brew install qt6"
echo "  Linux: sudo apt-get install qt6-base-dev"
echo ""
echo "Or manually update .vscode/c_cpp_properties.json with your Qt6 path."

