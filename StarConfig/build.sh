#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "=== StarConfig Build Script ==="
echo

# Check for required dependencies
echo "Checking dependencies..."
missing_deps=()

if ! command -v qmake6 &> /dev/null && ! command -v qmake &> /dev/null; then
    missing_deps+=("qt6-base")
fi

if ! command -v wlr-randr &> /dev/null; then
    missing_deps+=("wlr-randr")
fi

if ! command -v nmcli &> /dev/null; then
    missing_deps+=("networkmanager")
fi

if [ ${#missing_deps[@]} -ne 0 ]; then
    echo "Missing dependencies: ${missing_deps[*]}"
    echo "Install with: sudo pacman -S ${missing_deps[*]}"
    exit 1
fi

echo "✓ All dependencies found"
echo

# Copy shared headers if not already present
echo "Copying shared headers..."

if [ ! -f "include/toml.h" ]; then
    if [ -f "../StarDock/include/toml.h" ]; then
        echo "  Copying toml.h from StarDock..."
        cp ../StarDock/include/toml.h include/
    else
        echo "  ERROR: toml.h not found in ../StarDock/include/"
        exit 1
    fi
else
    echo "  ✓ toml.h already present"
fi

if [ ! -f "src/toml.c" ]; then
    if [ -f "../StarDock/src/toml.c" ]; then
        echo "  Copying toml.c from StarDock..."
        cp ../StarDock/src/toml.c src/
    else
        echo "  ERROR: toml.c not found in ../StarDock/src/"
        exit 1
    fi
else
    echo "  ✓ toml.c already present"
fi

if [ ! -f "include/starview_ipc.h" ]; then
    if [ -f "../StarviewIPC/starview_ipc.h" ]; then
        echo "  Copying starview_ipc.h from StarviewIPC..."
        cp ../StarviewIPC/starview_ipc.h include/
    elif [ -f "../../StarviewIPC/starview_ipc.h" ]; then
        echo "  Copying starview_ipc.h from ../../StarviewIPC..."
        cp ../../StarviewIPC/starview_ipc.h include/
    else
        echo "  ERROR: starview_ipc.h not found"
        exit 1
    fi
else
    echo "  ✓ starview_ipc.h already present"
fi

echo

# Create placeholder QML files if they don't exist
echo "Creating placeholder pages..."

mkdir -p src/pages

if [ ! -f "src/pages/DecorationPage.qml" ]; then
    cat > src/pages/DecorationPage.qml << 'EOF'
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

ScrollView {
    objectName: "decoration"
    
    ColumnLayout {
        width: parent.width - 40
        spacing: 20
        
        Text {
            text: "Decoration Settings"
            font.pixelSize: 28
            font.bold: true
            color: "#cdd6f4"
            Layout.topMargin: 30
            Layout.leftMargin: 30
        }
        
        SettingsGroup {
            title: "Titlebar"
            Layout.fillWidth: true
            Layout.margins: 30
            
            SettingsRow {
                label: "Enable Decorations"
                description: "Show window titlebars"
                
                Switch {
                    checked: configManager.decorationsEnabled
                    onToggled: configManager.decorationsEnabled = checked
                }
            }
            
            SettingsRow {
                label: "Titlebar Height"
                description: "Height of window titlebars"
                
                SpinBox {
                    from: 20
                    to: 50
                    value: configManager.titlebarHeight
                    onValueModified: configManager.titlebarHeight = value
                }
            }
        }
        
        Item { Layout.fillHeight: true }
    }
}
EOF
fi

if [ ! -f "src/pages/MonitorsPage.qml" ]; then
    cat > src/pages/MonitorsPage.qml << 'EOF'
import QtQuick
import QtQuick.Controls
import "../components"

ScrollView {
    objectName: "monitors"
    
    Text {
        text: "Monitor Settings"
        font.pixelSize: 28
        font.bold: true
        color: "#cdd6f4"
        anchors.top: parent.top
        anchors.topMargin: 30
        anchors.left: parent.left
        anchors.leftMargin: 30
    }
    
    Text {
        text: "Monitor configuration coming soon..."
        color: "#6c7086"
        anchors.centerIn: parent
    }
}
EOF
fi

if [ ! -f "src/pages/KeybindingsPage.qml" ]; then
    cat > src/pages/KeybindingsPage.qml << 'EOF'
import QtQuick
import QtQuick.Controls
import "../components"

ScrollView {
    objectName: "keybindings"
    
    Text {
        text: "Keybindings"
        font.pixelSize: 28
        font.bold: true
        color: "#cdd6f4"
        anchors.top: parent.top
        anchors.topMargin: 30
        anchors.left: parent.left
        anchors.leftMargin: 30
    }
    
    Text {
        text: "Keybinding configuration coming soon..."
        color: "#6c7086"
        anchors.centerIn: parent
    }
}
EOF
fi

echo "✓ Placeholder pages created"
echo

# Build
echo "Building StarConfig..."
mkdir -p build
cd build

echo "  Running cmake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

echo "  Running make..."
make -j$(nproc)

echo
echo "=== Build complete! ==="
echo
echo "Run with: ./build/starview-config"
echo "Or install with: cd build && sudo make install"
