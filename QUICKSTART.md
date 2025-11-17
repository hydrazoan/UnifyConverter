# Quick Start Guide

Get up and running with PresetConverter in 5 minutes!

## 🚀 Fast Track

### 1. Download JUCE (2 minutes)
```bash
# Visit https://juce.com/get-juce
# Download JUCE 7.x
# Extract to ~/JUCE (macOS/Linux) or C:\JUCE (Windows)
```

### 2. Open Project (1 minute)
```bash
# Open JUCE/extras/Projucer/Builds/[YourPlatform]
# Build Projucer if needed
# Open PresetConverter.jucer in Projucer
```

### 3. Configure Paths (30 seconds)
```
Projucer → File → Global Paths → Set "Path to JUCE"
```

### 4. Build (2 minutes)
```
Projucer → Click "Save and Open in IDE" → Build
```

### 5. Run & Test (30 seconds)
```
Launch app → Drag a .fxb file → Check output folder!
```

## 📋 Platform-Specific Quick Start

### macOS
```bash
# Install Xcode from App Store
open PresetConverter.jucer
# In Projucer: Save and Open in IDE
# In Xcode: Cmd+B to build
```

### Windows
```cmd
REM Install Visual Studio 2022 with C++ tools
PresetConverter.jucer
REM In Projucer: Save and Open in IDE
REM In VS: Ctrl+Shift+B to build
```

### Linux
```bash
# Install dependencies
sudo apt-get install build-essential libasound2-dev \
    libjack-jackd2-dev libfreetype6-dev libx11-dev

cd Builds/LinuxMakefile
make
```

## 🎯 First Conversion

1. **Launch** the app
2. **Select** "Zampler RX" from the dropdown
3. **Drag** a Zampler .fxb file onto the window
4. **Done!** Check the "Converted" folder next to your file

## 📁 Test Files

Don't have test files? Try these:
- Use Zampler RX to save a bank as .fxb
- Download sample VST presets from plugin websites
- Use any VST2 plugin's preset files

## 🐛 Quick Troubleshooting

| Problem | Solution |
|---------|----------|
| "JUCE not found" | Set Global Paths in Projucer |
| "Build failed" | Check C++17 is enabled |
| "No profiles" | Copy Profiles/ folder next to .exe |
| "Parse error" | Make sure it's a VST2 .fxb/.fxp file |

## 📚 Learn More

- Full README: [README.md](README.md)
- Build Details: [docs/BUILD.md](docs/BUILD.md)
- Project Status: [PROJECT_STATUS.md](PROJECT_STATUS.md)

## 💡 Pro Tips

1. **Batch Convert**: Drop multiple files at once
2. **Custom Output**: Click "Output Directory..." to choose location
3. **Custom Profiles**: Add .json files to Profiles/ folder
4. **Check Logs**: Scroll down to see conversion details

## ⚡ Common Use Cases

### Convert Entire Library
```
1. Select all .fxb files
2. Drag onto PresetConverter
3. Wait for batch completion
4. Import .unify files into Unify
```

### Create Custom Profile
```
1. Copy zampler.json from Profiles/
2. Rename and edit with your plugin details
3. Restart app to load new profile
```

### Organize by Category
```
1. Use folder structure for output
2. Convert different banks separately
3. Rename in Unify for better organization
```

## 🎉 Success!

You should now have:
- ✅ Built executable
- ✅ Converted first preset
- ✅ .unify and .guru files created
- ✅ Ready to import into Unify

## 🆘 Need Help?

1. Check [BUILD.md](docs/BUILD.md) for detailed instructions
2. Review error messages in the log window
3. Verify file formats (must be VST2 .fxb or .fxp)
4. Check that plugin profile matches your files

---

**That's it! Happy converting! 🎵**
