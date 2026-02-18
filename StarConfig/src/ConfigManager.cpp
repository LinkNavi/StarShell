#include "ConfigManager.h"
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>

ConfigManager::ConfigManager(QObject *parent) : QObject(parent) {
    // Connect to compositor IPC
    m_ipc = starview_ipc_connect();
    if (!m_ipc) {
        qWarning() << "Failed to connect to StarView IPC";
    }
    
    // Load config
    loadFromFile();
    
    // Watch for external changes
    m_watcher = new QFileSystemWatcher(this);
    m_watcher->addPath(configPath());
    
    connect(m_watcher, &QFileSystemWatcher::fileChanged, this, [this]() {
        qDebug() << "Config file changed externally, reloading...";
        loadFromFile();
        
        // Re-add watch
        if (!m_watcher->files().contains(configPath())) {
            m_watcher->addPath(configPath());
        }
    });
}

ConfigManager::~ConfigManager() {
    if (m_ipc) {
        starview_ipc_disconnect(m_ipc);
    }
}

QString ConfigManager::configPath() const {
    return QDir::homePath() + "/.config/starview";
}

void ConfigManager::loadFromFile() {
    QString configDir = configPath();
    QDir dir(configDir);
    
    if (!dir.exists()) {
        qWarning() << "Config directory not found:" << configDir;
        return;
    }
    
    // Get all .toml files
    QStringList tomlFiles = dir.entryList(QStringList() << "*.toml", QDir::Files);
    
    qDebug() << "Loading config from" << tomlFiles.size() << "TOML files";
    
    // Parse each file
    for (const QString &filename : tomlFiles) {
        QString filePath = configDir + "/" + filename;
        
        FILE *fp = fopen(filePath.toUtf8().constData(), "r");
        if (!fp) {
            qWarning() << "Failed to open:" << filePath;
            continue;
        }
        
        char errbuf[256];
        toml_table_t *root = toml_parse_file(fp, errbuf, sizeof(errbuf));
        fclose(fp);
        
        if (!root) {
            qWarning() << "Failed to parse" << filename << ":" << errbuf;
            continue;
        }
        
        // Parse [general] section
        toml_table_t *general = toml_table_in(root, "general");
        if (general) {
            toml_datum_t val;
            
            val = toml_int_in(general, "gap_size");
            if (val.ok) m_gapSize = val.u.i;
            
            val = toml_int_in(general, "border_width");
            if (val.ok) m_borderWidth = val.u.i;
            
            val = toml_string_in(general, "border_color");
            if (val.ok) {
                m_borderColor = QString(val.u.s);
                free(val.u.s);
            }
            
            val = toml_string_in(general, "focused_border_color");
            if (val.ok) {
                m_focusedBorderColor = QString(val.u.s);
                free(val.u.s);
            }
        }
        
        // Parse [decoration] section
        toml_table_t *decoration = toml_table_in(root, "decoration");
        if (decoration) {
            toml_datum_t val;
            
            val = toml_bool_in(decoration, "enabled");
            if (val.ok) m_decorationsEnabled = val.u.b;
            
            val = toml_int_in(decoration, "titlebar_height");
            if (val.ok) m_titlebarHeight = val.u.i;
            
            val = toml_string_in(decoration, "bg_color");
            if (val.ok) {
                m_decorationBgColor = QString(val.u.s);
                free(val.u.s);
            }
            
            val = toml_string_in(decoration, "text_color");
            if (val.ok) {
                m_decorationTextColor = QString(val.u.s);
                free(val.u.s);
            }
        }
        
        // Parse [background] section
        toml_table_t *background = toml_table_in(root, "background");
        if (background) {
            toml_datum_t val;
            
            val = toml_string_in(background, "image");
            if (val.ok) {
                QString path = QString(val.u.s);
                // Expand ~ to home directory
                if (path.startsWith("~")) {
                    path.replace(0, 1, QDir::homePath());
                }
                m_wallpaperPath = path;
                free(val.u.s);
            }
            
            val = toml_string_in(background, "mode");
            if (val.ok) {
                m_wallpaperMode = QString(val.u.s);
                free(val.u.s);
            }
        }
        
        toml_free(root);
    }
    
    emit configChanged();
    
    qDebug() << "Config loaded successfully";
    qDebug() << "  Gap size:" << m_gapSize;
    qDebug() << "  Border width:" << m_borderWidth;
    qDebug() << "  Decorations enabled:" << m_decorationsEnabled;
    qDebug() << "  Wallpaper:" << m_wallpaperPath;
}

void ConfigManager::saveToFile() {
    QString configDir = configPath();
    
    // Ensure config directory exists
    QDir dir;
    if (!dir.exists(configDir)) {
        dir.mkpath(configDir);
    }
    
    // Determine which files to update based on what sections they contain
    QMap<QString, QStringList> fileSections;
    fileSections["general.toml"] = QStringList() << "general";
    fileSections["decoration.toml"] = QStringList() << "decoration";
    fileSections["background.toml"] = QStringList() << "background";
    
    // If files don't exist, fall back to config.toml
    QStringList sectionsToWrite;
    sectionsToWrite << "general" << "decoration" << "background";
    
    for (auto it = fileSections.begin(); it != fileSections.end(); ++it) {
        QString filename = it.key();
        QStringList sections = it.value();
        QString filePath = configDir + "/" + filename;
        
        // Check if file exists, if not skip (will write to config.toml instead)
        if (!QFile::exists(filePath)) {
            continue;
        }
        
        // Read existing file
        QFile file(filePath);
        QString existingContent;
        QStringList existingLines;
        
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            existingContent = file.readAll();
            existingLines = existingContent.split('\n');
            file.close();
        }
        
        // Update the file
        if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
            QTextStream out(&file);
            
            QString currentSection;
            bool inTargetSection = false;
            bool wroteSection = false;
            
            for (const QString &line : existingLines) {
                QString trimmed = line.trimmed();
                
                // Track current section
                if (trimmed.startsWith("[") && trimmed.endsWith("]")) {
                    currentSection = trimmed.mid(1, trimmed.length() - 2);
                    inTargetSection = sections.contains(currentSection);
                    
                    // Write section header
                    out << line << "\n";
                    
                    // Write our values right after section header
                    if (inTargetSection && !wroteSection) {
                        writeSectionContent(out, currentSection);
                        wroteSection = true;
                        // Skip existing content in this section
                        continue;
                    }
                    continue;
                }
                
                // Skip lines in sections we're updating
                if (inTargetSection) {
                    continue;
                }
                
                // Keep other lines
                out << line << "\n";
            }
            
            // If section wasn't found, append it
            if (!wroteSection && !sections.isEmpty()) {
                for (const QString &section : sections) {
                    out << "\n[" << section << "]\n";
                    writeSectionContent(out, section);
                    sectionsToWrite.removeAll(section);
                }
            }
            
            file.close();
            qDebug() << "Updated" << filePath;
            
            // Remove sections we wrote from the list
            for (const QString &section : sections) {
                sectionsToWrite.removeAll(section);
            }
        }
    }
    
    // If any sections weren't written, write them to config.toml
    if (!sectionsToWrite.isEmpty()) {
        QString configPath = configDir + "/config.toml";
        QFile file(configPath);
        
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            
            for (const QString &section : sectionsToWrite) {
                out << "\n[" << section << "]\n";
                writeSectionContent(out, section);
            }
            
            file.close();
            qDebug() << "Appended sections to config.toml";
        }
    }
    
    qDebug() << "Config saved";
    emit saved();
}

void ConfigManager::writeSectionContent(QTextStream &out, const QString &section) {
    if (section == "general") {
        out << "gap_size = " << m_gapSize << "\n";
        out << "border_width = " << m_borderWidth << "\n";
        out << "border_color = \"" << m_borderColor << "\"\n";
        out << "focused_border_color = \"" << m_focusedBorderColor << "\"\n";
    }
    else if (section == "decoration") {
        out << "enabled = " << (m_decorationsEnabled ? "true" : "false") << "\n";
        out << "titlebar_height = " << m_titlebarHeight << "\n";
        out << "bg_color = \"" << m_decorationBgColor << "\"\n";
        out << "text_color = \"" << m_decorationTextColor << "\"\n";
    }
    else if (section == "background") {
        out << "enabled = true\n";
        if (!m_wallpaperPath.isEmpty()) {
            out << "image = \"" << m_wallpaperPath << "\"\n";
        }
        out << "mode = \"" << m_wallpaperMode << "\"\n";
    }
}

// Setters
void ConfigManager::setGapSize(int size) {
    if (m_gapSize != size) {
        m_gapSize = size;
        emit configChanged();
    }
}

void ConfigManager::setBorderWidth(int width) {
    if (m_borderWidth != width) {
        m_borderWidth = width;
        emit configChanged();
    }
}

void ConfigManager::setBorderColor(const QString &color) {
    if (m_borderColor != color) {
        m_borderColor = color;
        emit configChanged();
    }
}

void ConfigManager::setFocusedBorderColor(const QString &color) {
    if (m_focusedBorderColor != color) {
        m_focusedBorderColor = color;
        emit configChanged();
    }
}

void ConfigManager::setDecorationsEnabled(bool enabled) {
    if (m_decorationsEnabled != enabled) {
        m_decorationsEnabled = enabled;
        emit configChanged();
    }
}

void ConfigManager::setTitlebarHeight(int height) {
    if (m_titlebarHeight != height) {
        m_titlebarHeight = height;
        emit configChanged();
    }
}

void ConfigManager::setDecorationBgColor(const QString &color) {
    if (m_decorationBgColor != color) {
        m_decorationBgColor = color;
        emit configChanged();
    }
}

void ConfigManager::setDecorationTextColor(const QString &color) {
    if (m_decorationTextColor != color) {
        m_decorationTextColor = color;
        emit configChanged();
    }
}

void ConfigManager::setWallpaperPath(const QString &path) {
    if (m_wallpaperPath != path) {
        m_wallpaperPath = path;
        emit configChanged();
    }
}

void ConfigManager::setWallpaperMode(const QString &mode) {
    if (m_wallpaperMode != mode) {
        m_wallpaperMode = mode;
        emit configChanged();
    }
}

void ConfigManager::load() {
    loadFromFile();
}

void ConfigManager::save() {
    saveToFile();
}

void ConfigManager::reload() {
    loadFromFile();
}

void ConfigManager::reloadCompositor() {
    if (!m_ipc) {
        emit reloadFailed("Not connected to compositor");
        return;
    }
    
    if (starview_ipc_reload_config(m_ipc)) {
        qDebug() << "Compositor config reloaded successfully";
    } else {
        emit reloadFailed("Failed to reload compositor config");
    }
}
