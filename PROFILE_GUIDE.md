# Plugin Profile Guide - What You Actually Need

## 🎯 **What Are JSON Profiles For?**

JSON profile files tell the converter:
1. **Which plugin** the presets are for (by matching Plugin ID)
2. **How the plugin stores data** (chunk-based vs parameter-based)
3. **How to map parameters** (if parameter-based)

## 🔑 **Key Concept: Chunk-Based vs Parameter-Based**

### **Chunk-Based (Most Modern Synths)**
- Plugin stores **everything as binary data**
- NO parameter mapping needed
- JSON file is **super simple** - just identification
- **Examples**: U-he Diva, Massive, Pigments, TAL U-NO-LX

### **Parameter-Based (Older/Simpler Plugins)**
- Plugin stores **individual parameter values** (0.0 - 1.0)
- NEEDS parameter mappings in JSON
- More complex JSON file
- **Examples**: Some older VST2 plugins, simple samplers

## 📋 **Profiles You Actually Need**

All the plugins we discussed (U-he, Waldorf, Native Instruments, TAL, Arturia, etc.) are **chunk-based**, so they need **minimal JSON** - just this:

```json
{
  "pluginName": "Diva",
  "pluginId": "U9Di",              ← Matches plugin in FXB file
  "manufacturer": "u-he",
  "isChunkBased": true,            ← This means NO mappings needed!
  "isVst2": true,
  "defaultLayerType": "Synth",
  "parameterMappings": []          ← Empty for chunk-based!
}
```

## ✅ **Profiles Created For You**

I've created 8 profiles in `/Profiles/`:

1. **uhe-diva.json** - U-he Diva
2. **uhe-repro5.json** - U-he Repro-5
3. **waldorf-blofeld.json** - Waldorf Blofeld
4. **ni-massive.json** - Native Instruments Massive
5. **arturia-pigments.json** - Arturia Pigments
6. **tal-uno-lx.json** - TAL U-NO-LX (Juno-60)
7. **korg-wavestation.json** - Korg Wavestation
8. **gforce-minimonsta.json** - GForce Minimonsta 2
9. **zampler.json** - Zampler RX (already there)

## 🎛️ **How the App Uses Profiles**

### **Step 1: You drop an FXB file**
```
MyPresets.fxb contains:
- Plugin ID: "U9Di"
- 100 presets
- Binary chunk data
```

### **Step 2: App reads Plugin ID from FXB**
```
Parser finds: "U9Di"
```

### **Step 3: App looks for matching JSON**
```
Searches Profiles/ directory
Finds: uhe-diva.json
Reads: pluginId = "U9Di" ← MATCH!
```

### **Step 4: App uses profile info**
```
- Sees isChunkBased = true
- Knows to use binary chunk data
- Creates Unify patch with chunk
- Names it "Diva" in Unify
```

## 🔍 **Finding Plugin IDs**

If you need to create more profiles, find the Plugin ID:

### **Method 1: From FXB File**
```bash
# Open FXB in hex editor
# Bytes 12-15 contain the 4-character ID
# Example: "U9Di" = Diva
```

### **Method 2: From Plugin Info**
Some plugins show their ID in:
- About dialog
- Plugin scanner output
- Manufacturer documentation

### **Method 3: Trial and Error**
1. Create profile with guessed ID
2. Try conversion
3. Check log for errors
4. Adjust ID if needed

## 📦 **What Files You ACTUALLY Need**

### **To Build the App:**
```
PresetConverter/
├── PresetConverter.jucer         ← JUCE project
├── Source/                       ← All C++ code
└── docs/BUILD.md                 ← Instructions
```

### **To RUN the App After Building:**
```
PresetConverter.app (or .exe)     ← Built application
Profiles/                         ← JSON files (next to app)
  ├── uhe-diva.json
  ├── waldorf-blofeld.json
  ├── ni-massive.json
  └── ... (all other .json files)
```

**CRITICAL**: Copy the `Profiles/` folder next to your built executable!

## 🚀 **Quick Setup**

1. **Build** the app from source
2. **Copy** `Profiles/` folder next to the .app or .exe
3. **Launch** the app
4. **Select** a plugin from dropdown (now shows all 9 plugins!)
5. **Drop** FXB files → Done!

## 🎵 **Usage Example**

```
1. Launch PresetConverter
2. Dropdown shows: "Diva", "Massive", "Pigments", etc.
3. Select "Diva"
4. Drag "My Diva Bank.fxb" onto window
5. App finds 100 presets in bank
6. Creates 100 .unify files
7. Each contains the complete Diva patch
8. Import into Unify!
```

## ❓ **Common Questions**

### **Q: Do I need to edit the JSON files?**
**A:** No! For chunk-based plugins (all modern synths), the provided JSONs work as-is.

### **Q: What if my plugin isn't in the list?**
**A:** Copy an existing JSON, change the `pluginName` and `pluginId`, save with a new filename.

### **Q: How do I find the Plugin ID?**
**A:** Check the FXB file bytes 12-15 in a hex editor, or check plugin documentation.

### **Q: Why is parameterMappings empty?**
**A:** Because these are chunk-based plugins - all data is in the binary chunk, not individual parameters.

### **Q: Can I add more plugins?**
**A:** Yes! Just create more JSON files in Profiles/ directory. The app scans all .json files on startup.

## 🎯 **Bottom Line**

For **U-he, Waldorf, Native Instruments, TAL, Arturia, Korg, GForce** plugins:
- ✅ They're all **chunk-based**
- ✅ JSON is **super simple** (just ID info)
- ✅ **No parameter mappings** needed
- ✅ Profiles are **ready to use**
- ✅ Just **copy Profiles/ folder** next to your app

The hard work is in the C++ code (parsing chunks, generating XML). The JSON is just saying "Hey, when you see Plugin ID 'U9Di', that's Diva!"

---

**All profiles are ready in the `/Profiles/` directory! 🎉**
