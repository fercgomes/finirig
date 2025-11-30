# Project Structure

## Overview

Finirig is organized with clear separation of concerns:

- **Audio Processing**: JUCE-based real-time audio processing
- **UI**: Qt-based user interface
- **Pedals**: Modular pedal effects system
- **Amps**: Amplifier modeling system
- **Tests**: Comprehensive unit tests

## Directory Structure

```
finirig/
├── CMakeLists.txt          # Main build configuration
├── .cursorrules            # Cursor IDE coding standards
├── .gitignore             # Git ignore rules
├── .gitmodules            # Git submodules (JUCE)
├── setup.sh               # Setup script for JUCE
│
├── README.md              # Quick start (links to docs)
├── docs/
│   ├── README.md          # Full project documentation
│   ├── BUILD.md           # Detailed build instructions
│   ├── PROJECT_STRUCTURE.md # This file
│   ├── VSCODE_SETUP.md    # VSCode configuration
│   └── ICON_SETUP.md      # Icon setup guide
│
├── include/               # Public headers
│   └── finirig/
│       ├── audio/         # Audio engine and processing
│       │   ├── AudioEngine.h
│       │   └── AudioProcessor.h
│       ├── pedals/        # Pedal effects
│       │   ├── PedalBase.h
│       │   └── OverdrivePedal.h
│       ├── amps/          # Amplifier models
│       │   └── AmpModel.h
│       └── ui/            # UI components
│           ├── MainWindow.h
│           └── AudioControlsWidget.h
│
├── src/                   # Implementation files
│   ├── main.cpp          # Application entry point
│   ├── audio/
│   ├── pedals/
│   ├── amps/
│   └── ui/
│
├── tests/                 # Test files
│   ├── test_main.cpp
│   ├── audio/
│   ├── pedals/
│   └── amps/
│
├── third_party/           # External dependencies
│   └── JUCE/             # JUCE framework (git submodule)
│
└── resources/             # Resources
    └── Info.plist        # macOS bundle info
```

## Component Architecture

### Audio Layer (`audio/`)

- **AudioEngine**: Manages audio device I/O, implements JUCE's `AudioIODeviceCallback`
- **AudioProcessor**: Base interface for all audio processing units

**Key Design Decisions:**
- Real-time safe: No allocations in audio callbacks
- Sample-accurate processing
- Thread-safe communication with UI

### Pedal Layer (`pedals/`)

- **PedalBase**: Abstract base class for all pedals
- **OverdrivePedal**: Example overdrive implementation

**Key Design Decisions:**
- Template method pattern: `processSample()` calls `processSampleImpl()`
- Enable/disable functionality built-in
- Sample-by-sample processing for maximum flexibility

### Amp Layer (`amps/`)

- **AmpModel**: Abstract base class for amplifier models

**Key Design Decisions:**
- Separate from pedals (different modeling approach)
- Gain and master volume controls standard interface

### UI Layer (`ui/`)

- **MainWindow**: Main application window (Qt)
- **AudioControlsWidget**: Audio device controls

**Key Design Decisions:**
- Qt for all UI (no JUCE GUI)
- Signal/slot for communication
- Separate from audio thread

## Data Flow

```
Guitar Input
    ↓
Audio Interface
    ↓
JUCE AudioDeviceManager
    ↓
AudioEngine::audioDeviceIOCallback (real-time thread)
    ↓
AudioProcessor::processSample
    ↓
Pedal Chain (if enabled)
    ↓
Amp Model
    ↓
Audio Output
    ↓
Audio Interface
    ↓
Speakers/Headphones
```

## Threading Model

### Audio Thread (Real-time)
- JUCE audio callback
- Audio processing only
- No UI access
- No allocations
- No mutex locks

### UI Thread (Qt Event Loop)
- User interactions
- Parameter updates
- Status display
- Communicates with audio via message passing

### Communication
- UI → Audio: Parameter changes via lock-free queues or atomics
- Audio → UI: Status updates via JUCE MessageManager

## Adding New Components

### Adding a New Pedal

1. Create header: `include/finirig/pedals/YourPedal.h`
   ```cpp
   #include "finirig/pedals/PedalBase.h"
   class YourPedal : public PedalBase {
       // Implement processSampleImpl()
   };
   ```

2. Create implementation: `src/pedals/YourPedal.cpp`

3. Add to CMakeLists.txt:
   ```cmake
   src/pedals/YourPedal.cpp
   include/finirig/pedals/YourPedal.h
   ```

4. Write tests: `tests/pedals/test_your_pedal.cpp`

### Adding a New Amp Model

1. Create header: `include/finirig/amps/YourAmp.h`
   ```cpp
   #include "finirig/amps/AmpModel.h"
   class YourAmp : public AmpModel {
       // Implement processSample(), setGain(), etc.
   };
   ```

2. Create implementation: `src/amps/YourAmp.cpp`

3. Add to CMakeLists.txt

4. Write tests: `tests/amps/test_your_amp.cpp`

## Coding Standards

See `.cursorrules` for detailed standards. Key points:

- C++20 standard
- Real-time safety for audio code
- RAII for resource management
- Smart pointers (no raw pointers for ownership)
- Comprehensive unit tests
- Clear separation of concerns

## Build System

- **CMake 3.22+**: Build system
- **C++20**: Language standard
- **Qt6**: UI framework (CMake finds automatically)
- **JUCE**: Audio framework (git submodule)
- **Catch2**: Testing framework (fetched automatically)

## Testing Strategy

- **Unit Tests**: Each component tested in isolation
- **Integration Tests**: Audio pipeline tested end-to-end
- **Real-time Tests**: Verify no allocations in callbacks
- **Coverage Target**: >80% for audio processing code

Run tests:
```bash
cd build
ctest
# OR
./bin/finirig_tests
```

