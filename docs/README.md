# Finirig

A professional guitar processing plugin with custom pedal and amplifier modeling.

## Features

- **Standalone Application**: Connect your audio interface and guitar directly
- **Custom Pedal Modeling**: Build and model your own guitar pedals
- **Custom Amp Modeling**: Model your own amplifiers
- **Real-time Audio Processing**: Low-latency audio processing using JUCE
- **Modern UI**: Qt-based user interface

## Requirements

- **CMake** 3.22 or higher
- **C++20** compatible compiler (GCC 10+, Clang 12+, MSVC 2019+)
- **Qt6** (Core, Widgets)
- **JUCE** 7.0+ (cloned into `third_party/JUCE`)

## Building

### Prerequisites

1. Install Qt6:
   ```bash
   # macOS (using Homebrew)
   brew install qt6
   
   # Linux (Ubuntu/Debian)
   sudo apt-get install qt6-base-dev
   
   # Or download from https://www.qt.io/download
   ```

2. Clone JUCE:
   ```bash
   # Use the setup script (recommended)
   ./setup.sh
   
   # OR manually
   git submodule update --init --recursive
   # OR
   git clone https://github.com/juce-framework/JUCE.git third_party/JUCE
   ```

### Build Steps

```bash
# Generate platform-specific icons (optional but recommended)
./scripts/generate_icons.sh

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build . -j$(nproc)

# Run standalone app
# macOS: open bin/Finirig.app
# Linux/Windows: ./bin/finirig_standalone

# Run tests
ctest
# OR
./bin/finirig_tests
```

**Note:** See `docs/ICON_SETUP.md` for detailed icon setup instructions.

### CMake Options

- `BUILD_TESTS=ON/OFF`: Build test suite (default: ON)
- `BUILD_STANDALONE=ON/OFF`: Build standalone application (default: ON)
- `CMAKE_BUILD_TYPE`: Debug, Release, RelWithDebInfo, MinSizeRel

## Project Structure

```
finirig/
├── CMakeLists.txt          # Main CMake configuration
├── .cursorrules            # Cursor IDE rules and standards
├── README.md               # Quick start guide
├── docs/                   # Documentation
│   ├── README.md           # This file (full documentation)
│   ├── BUILD.md            # Build instructions
│   ├── PROJECT_STRUCTURE.md # Architecture
│   ├── VSCODE_SETUP.md     # VSCode setup
│   └── ICON_SETUP.md       # Icon configuration
├── include/                # Public headers
│   └── finirig/
│       ├── audio/         # Audio engine and processing
│       ├── pedals/        # Pedal modeling
│       ├── amps/          # Amplifier modeling
│       └── ui/            # UI components
├── src/                    # Source files
│   ├── main.cpp           # Application entry point
│   ├── audio/             # Audio implementation
│   ├── pedals/            # Pedal implementation
│   ├── amps/              # Amp implementation
│   └── ui/                # UI implementation
├── tests/                  # Test files
│   ├── test_main.cpp
│   ├── audio/
│   ├── pedals/
│   └── amps/
├── third_party/            # External dependencies
│   └── JUCE/              # JUCE framework
└── resources/             # Resources (icons, etc.)
```

## Architecture

### Audio Processing Pipeline

```
Audio Input → AudioEngine → AudioProcessor → Pedal Chain → Amp Model → Audio Output
```

- **AudioEngine**: Manages audio device I/O using JUCE
- **AudioProcessor**: Base processing interface
- **Pedal Chain**: Series of pedal effects (overdrive, distortion, etc.)
- **Amp Model**: Amplifier modeling and cabinet simulation

### Threading Model

- **Audio Thread**: Real-time audio processing (JUCE audio callback)
- **UI Thread**: Qt event loop for user interface
- **Communication**: Message passing via JUCE's MessageManager

### Real-time Safety

All audio processing code must be real-time safe:
- No memory allocation in audio callbacks
- No mutex locks
- Use lock-free data structures
- Pre-allocated buffers

## Development

### Code Style

See `.cursorrules` for detailed coding standards. Key points:
- C++20 standard
- Modern C++ practices (smart pointers, RAII)
- Real-time safe audio processing
- Comprehensive unit testing

### Testing

```bash
# Run all tests
ctest

# Run specific test
./bin/finirig_tests "AudioProcessor"

# Run with verbose output
./bin/finirig_tests -s
```

### Adding a New Pedal

1. Create header in `include/finirig/pedals/YourPedal.h`
2. Inherit from `PedalBase`
3. Implement `processSample(float input)` method
4. Add to source in `src/pedals/YourPedal.cpp`
5. Write unit tests in `tests/pedals/test_your_pedal.cpp`

### Adding a New Amp Model

1. Create header in `include/finirig/amps/YourAmp.h`
2. Inherit from `AmpModel`
3. Implement amplifier modeling algorithm
4. Add to source in `src/amps/YourAmp.cpp`
5. Write unit tests in `tests/amps/test_your_amp.cpp`

## License

[Add your license here]

## Contributing

[Add contribution guidelines if applicable]

