# Qt GUI Setup Instructions

## Quick Start Guide for AdvancedHashTable GUI

### Step 1: Install Qt

1. **Download Qt:**
   - Visit: https://www.qt.io/download-open-source
   - Download Qt Online Installer
   - Choose appropriate version for your OS

2. **Install Qt:**
   - Run installer
   - Create Qt account (free)
   - Select components:
     - ✅ Qt 5.15.2 or Qt 6.x
     - ✅ Desktop gcc 64-bit (Linux)
     - ✅ MinGW 8.1.0 64-bit (Windows)
     - ✅ Qt Creator IDE
   - Complete installation (may take 30-60 minutes)

### Step 2: Open Project in Qt Creator

1. Launch Qt Creator
2. Click: **File → Open File or Project**
3. Navigate to project folder
4. Select: `AdvancedHashTableGUI.pro`
5. Click **Open**

### Step 3: Configure Build Kit

1. Qt Creator will prompt to configure project
2. Select a kit:
   - **Windows:** Desktop Qt 5.15.2 MinGW 64-bit
   - **Linux:** Desktop Qt 5.15.2 GCC 64-bit
   - **macOS:** Desktop Qt 5.15.2 clang 64-bit
3. Click **Configure Project**

### Step 4: Build Project

1. Click Build menu → **Build Project** (or press Ctrl+B)
2. Wait for compilation to complete
3. Check "Compile Output" pane for any errors

### Step 5: Run Application

1. Click the green **Run** button (or press Ctrl+R)
2. Application window should appear
3. Test all features!

---

## Alternative: Command Line Build

### Windows (with MinGW)

```cmd
cd "Design and Implementation of Hash Table"
C:\Qt\5.15.2\mingw81_64\bin\qmake.exe AdvancedHashTableGUI.pro
C:\Qt\Tools\mingw810_64\bin\mingw32-make.exe
.\release\AdvancedHashTableGUI.exe
```

### Linux

```bash
cd "Design and Implementation of Hash Table"
qmake AdvancedHashTableGUI.pro
make
./AdvancedHashTableGUI
```

### macOS

```bash
cd "Design and Implementation of Hash Table"
qmake AdvancedHashTableGUI.pro
make
open AdvancedHashTableGUI.app
```

---

## Troubleshooting

### Qt Not Found

**Error:** `qmake: command not found`

**Solution:**
```bash
# Linux
export PATH=/opt/Qt/5.15.2/gcc_64/bin:$PATH

# Windows (PowerShell)
$env:PATH += ";C:\Qt\5.15.2\mingw81_64\bin"
```

### Build Errors

**Error:** `Project ERROR: Unknown module(s) in QT: widgets`

**Solution:** Install Qt Widgets module via Qt Maintenance Tool

### UI File Not Found

**Error:** `ui_MainWindow.h: No such file or directory`

**Solution:** Run `qmake` first, then build again

---

## File Checklist

Before building, ensure these files exist:

```
✅ AdvancedHashTableGUI.pro
✅ src/main_gui.cpp
✅ src/MainWindow.h
✅ src/MainWindow.cpp
✅ ui/MainWindow.ui
✅ include/hashtable.h
✅ src/hashtable.cpp
✅ (all other source files)
```

---

## Expected Result

When running successfully, you should see:
- Window titled "Advanced Hash Table - Phone Directory System"
- Student ID displayed: F23040111
- Input fields for Username, Phone, Address
- 10 operation buttons with emoji icons
- Empty table (or pre-loaded records if data files exist)
- Status bar showing "Ready. Loaded X records."

---

## Next Steps

1. Try inserting a test record
2. Search for it by username
3. Search for it by phone
4. View statistics
5. Save to file
6. Close and reopen to verify persistence

---

**Note:** This GUI version uses the same hash table backend as the console version, just with a modern Qt interface!
