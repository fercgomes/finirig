#!/bin/bash

# Finirig Setup Script
# Clones JUCE and sets up the project

set -e

echo "Setting up Finirig project..."

# Create third_party directory if it doesn't exist
mkdir -p third_party

# Clone JUCE if it doesn't exist
if [ ! -d "third_party/JUCE" ]; then
    echo "Cloning JUCE..."
    git clone https://github.com/juce-framework/JUCE.git third_party/JUCE
    echo "JUCE cloned successfully"
else
    echo "JUCE already exists, skipping clone"
fi

echo ""
echo "Setup complete!"
echo ""
echo "Next steps:"
echo "1. Make sure Qt6 is installed (brew install qt6 on macOS)"
echo "2. Create build directory: mkdir build && cd build"
echo "3. Configure: cmake .."
echo "4. Build: cmake --build ."
echo "5. Run: ./bin/finirig_standalone"

