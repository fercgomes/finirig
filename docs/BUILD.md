# Build Instructions

## Quick Start

```bash
# 1. Run setup script to clone JUCE
./setup.sh

# 2. Create build directory
mkdir build && cd build

# 3. Configure CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# 4. Build
cmake --build . -j$(nproc)

# 5. Run
./bin/finirig_standalone
```

## Detailed Setup

### macOS

1. **Install Qt6:**

   ```bash
   brew install qt6
   ```

2. **Set Qt6 path (if CMake can't find it):**

   ```bash
   export Qt6_DIR=$(brew --prefix qt6)/lib/cmake/Qt6
   ```

3. **Install Xcode Command Line Tools:**

   ```bash
   xcode-select --install
   ```

4. **Build:**
   ```bash
   ./setup.sh
   mkdir build && cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   cmake --build . -j$(sysctl -n hw.ncpu)
   ```

### Linux (Ubuntu/Debian)

1. **Install dependencies:**

   ```bash
   sudo apt-get update
   sudo apt-get install -y \
       build-essential \
       cmake \
       qt6-base-dev \
       libasound2-dev \
       libjack-jackd2-dev \
       libfreetype6-dev \
       libx11-dev \
       libxext-dev \
       libxinerama-dev \
       libxrandr-dev \
       libxcursor-dev \
       libxcomposite-dev \
       libxrender-dev
   ```

2. **Build:**
   ```bash
   ./setup.sh
   mkdir build && cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   cmake --build . -j$(nproc)
   ```

### Windows (MSVC)

1. **Install Qt6:**

   - Download from https://www.qt.io/download
   - Or use vcpkg: `vcpkg install qt6-base`

2. **Install Visual Studio 2019 or later** with C++ development tools

3. **Build:**
   ```cmd
   setup.bat  (if created, or manually clone JUCE)
   mkdir build && cd build
   cmake .. -G "Visual Studio 17 2022" -A x64
   cmake --build . --config Release
   ```

## CMake Options

- `BUILD_TESTS=ON/OFF`: Build test suite (default: ON)
- `BUILD_STANDALONE=ON/OFF`: Build standalone application (default: ON)
- `CMAKE_BUILD_TYPE`: Debug, Release, RelWithDebInfo, MinSizeRel
- `CMAKE_PREFIX_PATH`: Path to Qt6 installation (if not in standard location)

Example:

```bash
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_TESTS=ON \
    -DCMAKE_PREFIX_PATH=/path/to/qt6
```

## Troubleshooting

### CMake can't find Qt6

**macOS:**

```bash
export Qt6_DIR=$(brew --prefix qt6)/lib/cmake/Qt6
cmake .. -DCMAKE_PREFIX_PATH=$(brew --prefix qt6)
```

**Linux:**

```bash
cmake .. -DCMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/cmake/Qt6
```

**Windows:**

```bash
cmake .. -DCMAKE_PREFIX_PATH=C:/Qt/6.x.x/msvc2019_64
```

### JUCE not found

Make sure JUCE is cloned:

```bash
./setup.sh
# OR
git clone https://github.com/juce-framework/JUCE.git third_party/JUCE
```

### Audio device errors

- Make sure your audio interface is connected and recognized by the OS
- On Linux, you may need to install JACK or ALSA development libraries
- On macOS, check System Preferences > Security & Privacy for microphone access

### Build errors

- Make sure you have a C++20 compatible compiler
- Check that all dependencies are installed
- Try a clean build: `rm -rf build && mkdir build && cd build && cmake ..`

## Running Tests

```bash
cd build
ctest
# OR
./bin/finirig_tests
```

For verbose test output:

```bash
./bin/finirig_tests -s
```
