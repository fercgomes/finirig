# VSCode IntelliSense Setup Guide

## Quick Setup (3 steps)

1. **Install Extensions:**
   - Open VSCode
   - Press `Cmd+Shift+X` to open Extensions
   - Install the recommended extensions when prompted (or install manually):
     - C/C++ (ms-vscode.cpptools)
     - CMake Tools (ms-vscode.cmake-tools)

2. **Build the project once** (generates `compile_commands.json`):
   ```bash
   ./setup.sh              # Clone JUCE if needed
   mkdir build && cd build
   cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
   cmake --build .
   ```

3. **Reload VSCode:**
   - Press `Cmd+Shift+P`
   - Type "Developer: Reload Window"
   - Press Enter

## Verify IntelliSense is Working

1. Open any `.cpp` or `.h` file (e.g., `src/audio/AudioEngine.cpp`)
2. Hover over a JUCE type like `juce::AudioBuffer<float>` - you should see tooltips
3. Try autocomplete: Type `juce::` and press `Ctrl+Space` - you should see JUCE classes
4. Try "Go to Definition": `Cmd+Click` on any JUCE or Qt type

## Configuration Files

All VSCode configuration is in `.vscode/`:

- **`c_cpp_properties.json`**: IntelliSense configuration (include paths, compiler settings)
- **`settings.json`**: Editor and CMake settings
- **`tasks.json`**: Build tasks (use `Cmd+Shift+B` to build)
- **`launch.json`**: Debug configurations (use `F5` to debug)
- **`extensions.json`**: Recommended extensions

## How It Works

1. **CMake generates `compile_commands.json`** - This file contains all compiler flags and include paths
2. **IntelliSense reads `compile_commands.json`** - This gives accurate code completion
3. **Fallback to `c_cpp_properties.json`** - Used if compile_commands.json isn't available

## Troubleshooting

### IntelliSense shows errors but code compiles

This usually means `compile_commands.json` is missing or outdated:

```bash
cd build
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build .
```

Then reload IntelliSense:
- `Cmd+Shift+P` → "C/C++: Reset IntelliSense Database"
- `Cmd+Shift+P` → "C/C++: Reload IntelliSense"

### Can't find Qt6 headers

Run the helper script:
```bash
./scripts/find_qt6.sh
```

Then update `.vscode/c_cpp_properties.json` with the paths it suggests.

### Can't find JUCE headers

Make sure JUCE is cloned:
```bash
./setup.sh
```

### CMake Tools can't find Qt6

Set the path in `.vscode/settings.json`:
```json
"cmake.configureArgs": [
    "-DCMAKE_PREFIX_PATH=/opt/homebrew/opt/qt@6"
]
```

Or configure manually:
```bash
cd build
cmake .. -DCMAKE_PREFIX_PATH=$(brew --prefix qt@6)
```

## Using Build Tasks

- **Default Build:** `Cmd+Shift+B`
- **All Tasks:** `Cmd+Shift+P` → "Tasks: Run Task"
- **Available Tasks:**
  - CMake: Configure
  - CMake: Build (default)
  - CMake: Build Release
  - CMake: Clean
  - Run Tests
  - Setup Project

## Using Debugger

1. Set breakpoints in your code (click left margin)
2. Press `F5` to start debugging
3. Choose "Debug Standalone App" or "Debug Tests"
4. Use debug controls: Continue (F5), Step Over (F10), Step Into (F11)

## Tips

- **Auto-save:** Files are formatted on save (configured in settings.json)
- **Quick Build:** `Cmd+Shift+B` builds the project
- **Quick Debug:** `F5` builds and runs in debugger
- **Go to Definition:** `Cmd+Click` on any symbol
- **Find References:** `Shift+F12` on any symbol
- **Symbol Search:** `Cmd+T` to search for symbols

## Advanced: Customizing IntelliSense

If you need to customize include paths, edit `.vscode/c_cpp_properties.json`.

The configuration uses `compile_commands.json` by default (most accurate), but you can add fallback paths in the `includePath` array.

## Next Steps

Once IntelliSense is working:
1. Start coding! Autocomplete will work for JUCE, Qt, and your own code
2. Use `Cmd+Click` to navigate through JUCE and Qt source code
3. Use the debugger (`F5`) to step through your audio processing code

