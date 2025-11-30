# Icon Setup Guide

The Finirig app uses `resources/icon.png` as the source for all platform-specific icons.

## Generating Icons

Run the icon generation script:

```bash
./scripts/generate_icons.sh
```

This script will:

- **macOS**: Generate `Finirig.icns` from the PNG (requires macOS with `sips` and `iconutil`)
- **Windows**: Generate `Finirig.ico` (requires ImageMagick's `convert` command)
- **Linux**: Copy PNG as `Finirig.png` (used directly in desktop files)

## Platform-Specific Details

### macOS

The `.icns` file is automatically included in the app bundle. The icon will appear:

- In Finder
- In the Dock
- In the Applications folder
- In Spotlight

**Requirements:**

- Run `generate_icons.sh` on macOS (uses native `sips` and `iconutil` tools)
- Icon is referenced in `Info.plist` via `CFBundleIconFile`

### Windows

The `.ico` file is embedded in the executable as a Windows resource.

**Requirements:**

- Install ImageMagick: `choco install imagemagick` or download from [ImageMagick website](https://imagemagick.org/)
- Or use an online converter to create `Finirig.ico` manually
- The icon will appear in:
  - File Explorer
  - Taskbar
  - Start Menu
  - Alt+Tab switcher

**Manual Creation:**
If ImageMagick is not available, you can create the `.ico` file using:

- [CloudConvert](https://cloudconvert.com/png-to-ico)
- [ConvertICO](https://convertico.com/)
- Any image editor that exports to ICO format

The ICO file should include multiple sizes: 16x16, 32x32, 48x48, 64x64, 128x128, 256x256.

### Linux

The PNG icon is used directly in the desktop file.

**Installation:**
When you install the app (via `make install` or package manager), the icon will be:

- Installed to `/usr/share/pixmaps/finirig.png`
- Referenced in `/usr/share/applications/finirig.desktop`
- Appear in application launchers and menus

**Desktop Integration:**
The desktop file (`finirig.desktop`) is automatically installed and will:

- Show the app in application menus
- Display the icon in launchers
- Allow launching from desktop environments

## Updating the Icon

1. Replace `resources/icon.png` with your new icon (recommended: 1024x1024 PNG)
2. Run `./scripts/generate_icons.sh` to regenerate platform-specific icons
3. Rebuild the application

## Icon Requirements

**Source Icon (icon.png):**

- Format: PNG with transparency
- Recommended size: 1024x1024 pixels
- Should work well at small sizes (16x16) and large sizes (512x512)

**Generated Icons:**

- macOS: `.icns` (contains all required sizes)
- Windows: `.ico` (contains multiple sizes: 16, 32, 48, 64, 128, 256)
- Linux: `.png` (used directly, any size)

## Troubleshooting

### macOS icon not showing

1. Verify `Finirig.icns` exists in `resources/`
2. Check `Info.plist` has `CFBundleIconFile` set to `Finirig.icns`
3. Rebuild the app bundle
4. Clear icon cache: `killall Finder` or `sudo rm -rf /Library/Caches/com.apple.iconservices.store`

### Windows icon not showing

1. Verify `Finirig.ico` exists in `resources/`
2. Rebuild the executable
3. The icon should be embedded automatically via CMake's `RC_ICONS` property

### Linux icon not showing

1. Verify `Finirig.png` exists in `resources/`
2. Check `finirig.desktop` file has `Icon=finirig`
3. Run `sudo make install` to install to system directories
4. Update desktop database: `sudo update-desktop-database`
5. Refresh icon cache: `gtk-update-icon-cache` (if using GTK)

## Build Integration

Icons are automatically handled during the build process:

- **macOS**: Icon is copied into the `.app` bundle
- **Windows**: Icon is embedded in the `.exe` via resource compiler
- **Linux**: Icon and desktop file are installed via CMake install rules

No manual steps required after running `generate_icons.sh`!
