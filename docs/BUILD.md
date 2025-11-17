# Build Instructions

## Prerequisites

### All Platforms

1. **JUCE Framework**
   - Download from https://juce.com/get-juce
   - Version 7.x or later recommended
   - Extract to a location like `~/JUCE` or `C:\JUCE`

2. **Projucer**
   - Comes with JUCE
   - Located in `JUCE/extras/Projucer`
   - Build Projucer first if not already built

### macOS Specific

- Xcode 13.0 or later
- macOS 10.13 or later

### Windows Specific

- Visual Studio 2019 or 2022
- Windows SDK
- Desktop development with C++

### Linux Specific

```bash
sudo apt-get install build-essential
sudo apt-get install libasound2-dev libjack-jackd2-dev \
    libcurl4-openssl-dev libfreetype6-dev \
    libx11-dev libxcomposite-dev libxcursor-dev \
    libxext-dev libxinerama-dev libxrandr-dev \
    libxrender-dev libwebkit2gtk-4.0-dev \
    libglu1-mesa-dev mesa-common-dev
```

## Building Step-by-Step

### Method 1: Using Projucer (Recommended)

1. **Configure Module Paths** (First Time Only)
   ```
   - Open Projucer
   - Go to File → Global Paths
   - Set "Path to JUCE" to your JUCE installation directory
   ```

2. **Open Project**
   ```
   - Open PresetConverter.jucer
   - Projucer will load the project
   ```

3. **Save and Open in IDE**
   
   **macOS:**
   ```
   - Click "Save and Open in IDE" (Xcode icon)
   - Xcode will open
   - Select scheme: PresetConverter - My Mac
   - Click Product → Build (Cmd+B)
   ```
   
   **Windows:**
   ```
   - Click "Save and Open in IDE" (Visual Studio icon)
   - Visual Studio will open
   - Select configuration: Debug or Release
   - Click Build → Build Solution (Ctrl+Shift+B)
   ```
   
   **Linux:**
   ```
   - Click "Save Project"
   - Open terminal in project directory
   - cd Builds/LinuxMakefile
   - make CONFIG=Release
   ```

### Method 2: Direct IDE Build

After Projucer has generated the build files:

**macOS:**
```bash
cd Builds/MacOSX
xcodebuild -configuration Release
```

**Windows:**
```cmd
cd Builds\VisualStudio2022
msbuild PresetConverter.sln /p:Configuration=Release
```

**Linux:**
```bash
cd Builds/LinuxMakefile
make CONFIG=Release
```

## Build Output Locations

### macOS
```
Builds/MacOSX/build/Release/PresetConverter.app
```

### Windows
```
Builds/VisualStudio2022/x64/Release/App/PresetConverter.exe
```

### Linux
```
Builds/LinuxMakefile/build/PresetConverter
```

## Troubleshooting

### "JUCE modules not found"

**Solution:** Update module paths in Projucer:
1. Open PresetConverter.jucer
2. Click on each exporter (Xcode, VS2022, Linux)
3. Update "Path to JUCE" in module paths
4. Save project

### Compiler Errors

**C++ Standard Issues:**
Ensure C++17 is enabled:
- Xcode: Set "C++ Language Dialect" to C++17
- VS: Set "C++ Language Standard" to ISO C++17
- Linux: Add `-std=c++17` to CONFIG

**Missing Headers:**
Verify JUCE modules are correctly linked:
```
juce_core
juce_data_structures
juce_events
juce_graphics
juce_gui_basics
juce_gui_extra
```

### Runtime Issues

**"Profile not found":**
- Ensure `Profiles/` directory exists next to the executable
- Copy profile JSON files to the correct location

**"Failed to load library":**
- On macOS: Check that the app is in /Applications or give permissions
- On Linux: Install required libraries (see Linux Prerequisites)

## Creating a Distributable Package

### macOS

```bash
cd Builds/MacOSX/build/Release
# Create DMG
hdiutil create -volname "PresetConverter" -srcfolder PresetConverter.app -ov -format UDZO PresetConverter.dmg
```

### Windows

1. Build in Release mode
2. Copy executable and all DLLs
3. Include Profiles/ directory
4. Use Inno Setup or NSIS to create installer

### Linux

```bash
# Create tarball
tar -czf PresetConverter.tar.gz PresetConverter Profiles/
```

## Development Tips

1. **Enable All Warnings**: Catch issues early
2. **Use Debug Build**: Better error messages during development
3. **Test Profiles**: Create test profiles before distribution
4. **Memory Leaks**: Use JUCE's leak detector (enabled in Debug)

## Next Steps

After building:
1. Copy sample profiles to `Profiles/` directory
2. Test with sample FXB/FXP files
3. Create custom profiles for your plugins
4. Report issues on GitHub

## Additional Resources

- JUCE Documentation: https://docs.juce.com
- JUCE Forum: https://forum.juce.com
- JUCE Tutorials: https://juce.com/learn/tutorials
