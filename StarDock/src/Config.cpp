#include "Config.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

extern "C" {
#include "toml.h"
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
    
    // Load defaults first
    loadDefaults();
    
    // Parse TOML file
    FILE *fp = fopen(configPath.toUtf8().constData(), "r");
    if (!fp) {
        qWarning() << "Failed to open config file:" << configPath;
        return;
    }
    
    char errbuf[256];
    toml_table_t *root = toml_parse_file(fp, errbuf, sizeof(errbuf));
    fclose(fp);
    
    if (!root) {
        qWarning() << "Failed to parse config:" << errbuf;
        return;
    }
    
    // Parse [dock] section
    toml_table_t *dock = toml_table_in(root, "dock");
    if (dock) {
        toml_datum_t val;
        
        val = toml_string_in(dock, "position");
        if (val.ok) {
            m_position = QString(val.u.s);
            free(val.u.s);
        }
        
        val = toml_int_in(dock, "height");
        if (val.ok) m_dockHeight = val.u.i;
        
        val = toml_int_in(dock, "icon_size");
        if (val.ok) m_iconSize = val.u.i;
        
        val = toml_int_in(dock, "icon_size_zoomed");
        if (val.ok) m_iconSizeZoomed = val.u.i;
        
        val = toml_int_in(dock, "spacing");
        if (val.ok) m_spacing = val.u.i;
        
        val = toml_int_in(dock, "padding");
        if (val.ok) m_padding = val.u.i;
        
        val = toml_int_in(dock, "exclusive_zone");
        if (val.ok) m_exclusiveZone = val.u.i;
    }
    
    // Parse [appearance] section
    toml_table_t *appearance = toml_table_in(root, "appearance");
    if (appearance) {
        toml_datum_t val;
        
        val = toml_string_in(appearance, "background_color");
        if (val.ok) {
            m_backgroundColor = QString(val.u.s);
            free(val.u.s);
        }
        
        val = toml_double_in(appearance, "background_opacity");
        if (val.ok) m_backgroundOpacity = val.u.d;
        
        val = toml_string_in(appearance, "border_color");
        if (val.ok) {
            m_borderColor = QString(val.u.s);
            free(val.u.s);
        }
        
        val = toml_int_in(appearance, "border_width");
        if (val.ok) m_borderWidth = val.u.i;
        
        val = toml_int_in(appearance, "corner_radius");
        if (val.ok) m_cornerRadius = val.u.i;
        
        val = toml_string_in(appearance, "accent_color");
        if (val.ok) {
            m_accentColor = QString(val.u.s);
            free(val.u.s);
        }
        
        val = toml_string_in(appearance, "accent_color_focused");
        if (val.ok) {
            m_accentColorFocused = QString(val.u.s);
            free(val.u.s);
        }
        
        val = toml_string_in(appearance, "text_color");
        if (val.ok) {
            m_textColor = QString(val.u.s);
            free(val.u.s);
        }
        
        val = toml_int_in(appearance, "text_size");
        if (val.ok) m_textSize = val.u.i;
        
        val = toml_bool_in(appearance, "shadow_enabled");
        if (val.ok) m_shadowEnabled = val.u.b;
        
        val = toml_string_in(appearance, "shadow_color");
        if (val.ok) {
            m_shadowColor = QString(val.u.s);
            free(val.u.s);
        }
        
        val = toml_double_in(appearance, "shadow_blur");
        if (val.ok) m_shadowBlur = val.u.d;
        
        val = toml_int_in(appearance, "shadow_offset_x");
        if (val.ok) m_shadowOffsetX = val.u.i;
        
        val = toml_int_in(appearance, "shadow_offset_y");
        if (val.ok) m_shadowOffsetY = val.u.i;
    }
    
    // Parse [animations] section
    toml_table_t *animations = toml_table_in(root, "animations");
    if (animations) {
        toml_datum_t val;
        
        val = toml_bool_in(animations, "enabled");
        if (val.ok) m_animationsEnabled = val.u.b;
        
        val = toml_int_in(animations, "zoom_duration");
        if (val.ok) m_zoomDuration = val.u.i;
        
        val = toml_int_in(animations, "fade_duration");
        if (val.ok) m_fadeDuration = val.u.i;
        
        val = toml_int_in(animations, "color_duration");
        if (val.ok) m_colorDuration = val.u.i;
        
        val = toml_string_in(animations, "zoom_easing");
        if (val.ok) {
            m_zoomEasing = QString(val.u.s);
            free(val.u.s);
        }
    }
    
    // Parse [launcher] section
    toml_table_t *launcher = toml_table_in(root, "launcher");
    if (launcher) {
        toml_datum_t val;
        
        val = toml_int_in(launcher, "width");
        if (val.ok) m_launcherWidth = val.u.i;
        
        val = toml_int_in(launcher, "height");
        if (val.ok) m_launcherHeight = val.u.i;
        
        val = toml_int_in(launcher, "cell_width");
        if (val.ok) m_launcherCellWidth = val.u.i;
        
        val = toml_int_in(launcher, "cell_height");
        if (val.ok) m_launcherCellHeight = val.u.i;
        
        val = toml_int_in(launcher, "icon_size");
        if (val.ok) m_launcherIconSize = val.u.i;
        
        val = toml_string_in(launcher, "search_placeholder");
        if (val.ok) {
            m_searchPlaceholder = QString(val.u.s);
            free(val.u.s);
        }
        
        val = toml_bool_in(launcher, "backdrop_enabled");
        if (val.ok) m_backdropEnabled = val.u.b;
        
        val = toml_string_in(launcher, "backdrop_color");
        if (val.ok) {
            m_backdropColor = QString(val.u.s);
            free(val.u.s);
        }
    }
    
    // Parse [tracking] section
    toml_table_t *tracking = toml_table_in(root, "tracking");
    if (tracking) {
        toml_datum_t val;
        
        val = toml_int_in(tracking, "update_interval_ms");
        if (val.ok) m_updateInterval = val.u.i;
        
        // Parse skip_processes array
        toml_array_t *skip_array = toml_array_in(tracking, "skip_processes");
        if (skip_array) {
            m_skipProcesses.clear();
            int n = toml_array_nelem(skip_array);
            for (int i = 0; i < n; i++) {
                toml_datum_t elem = toml_string_at(skip_array, i);
                if (elem.ok) {
                    m_skipProcesses.append(QString(elem.u.s));
                    free(elem.u.s);
                }
            }
        }
    }
    
    // Parse [icons] section
    toml_table_t *icons = toml_table_in(root, "icons");
    if (icons) {
        toml_datum_t val;
        
        // Parse search_paths array
        toml_array_t *paths_array = toml_array_in(icons, "search_paths");
        if (paths_array) {
            m_iconSearchPaths.clear();
            int n = toml_array_nelem(paths_array);
            for (int i = 0; i < n; i++) {
                toml_datum_t elem = toml_string_at(paths_array, i);
                if (elem.ok) {
                    QString path = QString(elem.u.s);
                    // Expand ~ to home directory
                    if (path.startsWith("~")) {
                        path.replace(0, 1, QDir::homePath());
                    }
                    m_iconSearchPaths.append(path);
                    free(elem.u.s);
                }
            }
        }
        
        val = toml_bool_in(icons, "use_text_fallback");
        if (val.ok) m_useTextFallback = val.u.b;
        
        val = toml_int_in(icons, "fallback_text_size");
        if (val.ok) m_fallbackTextSize = val.u.i;
    }
    
    // Parse [desktop_files] section
    toml_table_t *desktop_files = toml_table_in(root, "desktop_files");
    if (desktop_files) {
        // Parse search_paths array
        toml_array_t *paths_array = toml_array_in(desktop_files, "search_paths");
        if (paths_array) {
            m_desktopFileSearchPaths.clear();
            int n = toml_array_nelem(paths_array);
            for (int i = 0; i < n; i++) {
                toml_datum_t elem = toml_string_at(paths_array, i);
                if (elem.ok) {
                    QString path = QString(elem.u.s);
                    // Expand ~ to home directory
                    if (path.startsWith("~")) {
                        path.replace(0, 1, QDir::homePath());
                    }
                    m_desktopFileSearchPaths.append(path);
                    free(elem.u.s);
                }
            }
        }
    }
    
    // Parse [indicators] section
    toml_table_t *indicators = toml_table_in(root, "indicators");
    if (indicators) {
        toml_datum_t val;
        
        val = toml_bool_in(indicators, "enabled");
        if (val.ok) m_indicatorsEnabled = val.u.b;
        
        val = toml_string_in(indicators, "color");
        if (val.ok) {
            m_indicatorColor = QString(val.u.s);
            free(val.u.s);
        }
        
        val = toml_int_in(indicators, "size");
        if (val.ok) m_indicatorSize = val.u.i;
        
        val = toml_double_in(indicators, "opacity");
        if (val.ok) m_indicatorOpacity = val.u.d;
    }
    
    // Parse [separator] section
    toml_table_t *separator = toml_table_in(root, "separator");
    if (separator) {
        toml_datum_t val;
        
        val = toml_bool_in(separator, "enabled");
        if (val.ok) m_separatorEnabled = val.u.b;
        
        val = toml_int_in(separator, "width");
        if (val.ok) m_separatorWidth = val.u.i;
        
        val = toml_int_in(separator, "height");
        if (val.ok) m_separatorHeight = val.u.i;
        
        val = toml_string_in(separator, "color");
        if (val.ok) {
            m_separatorColor = QString(val.u.s);
            free(val.u.s);
        }
        
        val = toml_double_in(separator, "opacity");
        if (val.ok) m_separatorOpacity = val.u.d;
    }
    
    // Parse [behavior] section
    toml_table_t *behavior = toml_table_in(root, "behavior");
    if (behavior) {
        toml_datum_t val;
        
        val = toml_bool_in(behavior, "tooltips_enabled");
        if (val.ok) m_tooltipsEnabled = val.u.b;
        
        val = toml_int_in(behavior, "tooltip_delay_ms");
        if (val.ok) m_tooltipDelay = val.u.i;
    }
    
    toml_free(root);
    qInfo() << "Config loaded successfully";
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
