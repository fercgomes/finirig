# Finirig

A professional guitar processing plugin with custom pedal and amplifier modeling.

## Quick Start

```bash
# Setup
./setup.sh

# Generate icons (optional)
./scripts/generate_icons.sh

# Build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)

# Run
# macOS: open bin/Finirig.app
# Linux/Windows: ./bin/finirig_standalone
```

## Documentation

- **[Full Documentation](docs/README.md)** - Complete project overview, features, and architecture
- **[Build Instructions](docs/BUILD.md)** - Detailed build instructions for all platforms
- **[Project Structure](docs/PROJECT_STRUCTURE.md)** - Architecture and code organization
- **[VSCode Setup](docs/VSCODE_SETUP.md)** - IntelliSense and development environment setup
- **[Icon Setup](docs/ICON_SETUP.md)** - Cross-platform icon configuration
- **[Release Process](docs/RELEASES.md)** - Versioning and release management

## Features

- **Standalone Application**: Connect your audio interface and guitar directly
- **Custom Pedal Modeling**: Build and model your own guitar pedals
- **Custom Amp Modeling**: Model your own amplifiers
- **Real-time Audio Processing**: Low-latency audio processing using JUCE
- **Modern UI**: Qt-based user interface
- **Audio Level Meters**: Real-time input/output level monitoring
- **Device Selection**: Choose and configure audio interfaces

## Requirements

- **CMake** 3.22+
- **C++20** compiler
- **Qt6** (Core, Widgets)
- **JUCE** 7.0+ (cloned via `setup.sh`)

## Version

Current version: **1.0.0-alpha.1**

See [CHANGELOG.md](CHANGELOG.md) for release history.

## License

[Add your license here]

## Contributing

[Add contribution guidelines if applicable]
