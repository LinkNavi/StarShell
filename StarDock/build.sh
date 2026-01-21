#!/bin/bash
# One-step build script for StarDock

echo "🚀 StarDock One-Step Build"
echo "==========================="
echo

# Make sure we're in the right place
if [ ! -d "include" ] || [ ! -d "src" ]; then
    echo "❌ Error: Must run from StarDock project root"
    echo "   (directory with include/ and src/ folders)"
    exit 1
fi

echo "📋 Step 1: Checking files..."

# Check all required files exist
MISSING=0

if [ ! -f "include/DesktopFileParser.h" ]; then
    echo "❌ Missing: include/DesktopFileParser.h"
    MISSING=1
fi

if [ ! -f "include/WindowTracker.h" ]; then
    echo "❌ Missing: include/WindowTracker.h"
    MISSING=1
fi

if [ ! -f "include/starview_ipc.h" ]; then
    echo "❌ Missing: include/starview_ipc.h"
    MISSING=1
fi

if [ ! -f "src/DesktopFileParser.cpp" ]; then
    echo "❌ Missing: src/DesktopFileParser.cpp"
    MISSING=1
fi

if [ ! -f "src/WindowTracker.cpp" ]; then
    echo "❌ Missing: src/WindowTracker.cpp"
    MISSING=1
fi

if [ ! -f "src/main.cpp" ]; then
    echo "❌ Missing: src/main.cpp"
    MISSING=1
fi

if [ ! -f "src/Main.qml" ]; then
    echo "❌ Missing: src/Main.qml"
    MISSING=1
fi

if [ $MISSING -eq 1 ]; then
    echo
    echo "❌ Some files are missing. Please copy them first:"
    echo "   Headers (.h) go in include/"
    echo "   Source (.cpp, .qml) go in src/"
    exit 1
fi

echo "✅ All files present"
echo

echo "🔧 Step 2: Fixing IPC header..."
if [ -f "fix_ipc_header.sh" ]; then
    chmod +x fix_ipc_header.sh
    ./fix_ipc_header.sh > /dev/null 2>&1
    echo "✅ IPC header fixed"
else
    echo "⚠️  fix_ipc_header.sh not found (might be OK if already fixed)"
fi
echo

echo "🧹 Step 3: Cleaning old build..."
rm -rf build
mkdir build
echo "✅ Build directory ready"
echo

echo "⚙️  Step 4: Configuring with CMake..."
cd build
if ! cmake .. > /tmp/cmake_output.txt 2>&1; then
    echo "❌ CMake failed. Output:"
    cat /tmp/cmake_output.txt
    exit 1
fi
echo "✅ CMake configuration successful"
echo

echo "🔨 Step 5: Compiling..."
if ! make 2>&1 | tee /tmp/make_output.txt; then
    echo
    echo "❌ Compilation failed. Check /tmp/make_output.txt for details"
    echo
    echo "Common issues:"
    echo "  - Headers in wrong location (must be in include/)"
    echo "  - Source files in wrong location (must be in src/)"
    echo "  - IPC header not fixed (run ./fix_ipc_header.sh)"
    exit 1
fi
echo
echo "✅ Compilation successful!"
echo

echo "🎉 Build Complete!"
echo
echo "Your dock is ready to run:"
echo "  cd build && ./starview-dock"
echo
echo "Or from project root:"
echo "  ./run.sh"
echo
