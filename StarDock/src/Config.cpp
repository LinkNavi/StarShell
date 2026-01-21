#include "Config.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

Config::Config(QObject *parent) : QObject(parent) {
    loadConfig();
}

void Config::loadConfig() {
    // Try to load from standard config locations
    QStringList configPaths = {
        QDir::homePath() + "/.config/stardock/config.toml",
        "/etc/stardock/config.toml",
        QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/stardock/config.toml",
        "config.toml"  // Current directory
    };
    
    QString configPath;
    for (const QString &path : configPaths) {
        if (QFile::exists(path)) {
            configPath = path;
            break;
        }
    }
    
    if (configPath.isEmpty()) {
        qInfo() << "No config file found, using defaults";
        loadDefaults();
        return;
    }
    
    qInfo() << "Loading config from:" << configPath;
    
    // Simple TOML parser - we'll just use defaults for now
    // In a full implementation, you'd use toml11 or similar
    // For now, this gives us the structure and you can add parsing later
    loadDefaults();
    
    // TODO: Parse TOML file and override defaults
    // Example with toml11:
    // auto data = toml::parse(configPath.toStdString());
    // m_iconSize = toml::find_or<int>(data, "dock", "icon_size", 48);
}

void Config::loadDefaults() {
    // Dock defaults
    m_position = "bottom";
    m_dockHeight = 70;
    m_iconSize = 48;
    m_iconSizeZoomed = 64;
    m_spacing = 8;
    m_padding = 30;
    m_exclusiveZone = 90;
    
    // Appearance defaults
    m_backgroundColor = "#1a1a2e";
    m_backgroundOpacity = 0.85;
    m_borderColor = "#45475a";
    m_borderWidth = 1;
    m_cornerRadius = 16;
    m_accentColor = "#89b4fa";
    m_accentColorFocused = "#a6e3a1";
    m_textColor = "#cdd6f4";
    m_textSize = 14;
    
    // Shadow defaults
    m_shadowEnabled = true;
    m_shadowColor = "#90000000";
    m_shadowBlur = 1.0;
    m_shadowOffsetX = 0;
    m_shadowOffsetY = 8;
    
    // Animation defaults
    m_animationsEnabled = true;
    m_zoomDuration = 200;
    m_fadeDuration = 150;
    m_colorDuration = 200;
    m_zoomEasing = "OutCubic";
    
    // Launcher defaults
    m_launcherWidth = 600;
    m_launcherHeight = 500;
    m_launcherCellWidth = 120;
    m_launcherCellHeight = 120;
    m_launcherIconSize = 48;
    m_searchPlaceholder = "Search applications...";
    m_backdropEnabled = true;
    m_backdropColor = "#80000000";
    
    // Tracking defaults
    m_updateInterval = 1000;
    m_skipProcesses = {
        "systemd", "dbus", "dbus-daemon", "gdm", "kwin", "plasmashell",
        "bash", "sh", "zsh", "fish", "starview-dock", "starview",
        "pipewire", "pulseaudio", "Xwayland", "at-spi", "gvfs",
        "gnome-shell", "gsd-", "upowerd"
    };
    
    // Icon defaults
    m_iconSearchPaths = {
        "/usr/share/pixmaps",
        "/usr/share/icons/hicolor/48x48/apps",
        "/usr/share/icons/hicolor/64x64/apps",
        "/usr/share/icons/hicolor/scalable/apps",
        "/usr/share/icons/hicolor/128x128/apps",
        QDir::homePath() + "/.local/share/icons/hicolor/48x48/apps"
    };
    m_useTextFallback = true;
    m_fallbackTextSize = 18;
    
    // Desktop files defaults
    m_desktopFileSearchPaths = {
        "/usr/share/applications",
        "/usr/local/share/applications",
        QDir::homePath() + "/.local/share/applications"
    };
    
    // Indicator defaults
    m_indicatorsEnabled = true;
    m_indicatorColor = "#89b4fa";
    m_indicatorSize = 4;
    m_indicatorOpacity = 0.8;
    
    // Separator defaults
    m_separatorEnabled = true;
    m_separatorWidth = 2;
    m_separatorHeight = 40;
    m_separatorColor = "#45475a";
    m_separatorOpacity = 0.5;
    
    // Behavior defaults
    m_tooltipsEnabled = true;
    m_tooltipDelay = 500;
}

QString Config::getEasingType() const {
    // Convert string to QML Easing type
    if (m_zoomEasing == "Linear") return "Easing.Linear";
    if (m_zoomEasing == "InQuad") return "Easing.InQuad";
    if (m_zoomEasing == "OutQuad") return "Easing.OutQuad";
    if (m_zoomEasing == "InOutQuad") return "Easing.InOutQuad";
    if (m_zoomEasing == "OutCubic") return "Easing.OutCubic";
    if (m_zoomEasing == "InOutCubic") return "Easing.InOutCubic";
    return "Easing.OutCubic";  // Default
}
