#include "WallpaperManager.h"
#include <QStandardPaths>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>

WallpaperManager::WallpaperManager(QObject *parent) : QObject(parent) {
}

void WallpaperManager::setWallpaper(const QString &path) {
    if (path.isEmpty()) {
        emit error("Empty wallpaper path");
        return;
    }
    
    // Search all TOML files in ~/.config/starview
    QString configDir = QDir::homePath() + "/.config/starview";
    QDir dir(configDir);
    
    if (!dir.exists()) {
        emit error("StarView config directory not found");
        return;
    }
    
    // Get all .toml files
    QStringList tomlFiles = dir.entryList(QStringList() << "*.toml", QDir::Files);
    
    bool updated = false;
    
    // Search through each TOML file for [background] section
    for (const QString &filename : tomlFiles) {
        QString filePath = configDir + "/" + filename;
        QFile file(filePath);
        
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            continue;
        }
        
        QString content = file.readAll();
        file.close();
        
        // Check if this file has a [background] section
        if (!content.contains("[background]")) {
            continue;
        }
        
        // Parse and update
        QStringList lines = content.split('\n');
        bool inBackgroundSection = false;
        bool foundImage = false;
        
        for (int i = 0; i < lines.size(); i++) {
            QString line = lines[i].trimmed();
            
            // Check if we're in [background] section
            if (line == "[background]") {
                inBackgroundSection = true;
                continue;
            }
            
            // Check if we left the section
            if (inBackgroundSection && line.startsWith("[") && line.endsWith("]")) {
                // Before leaving section, if we didn't find image, add it
                if (!foundImage) {
                    lines.insert(i, QString("image = \"%1\"").arg(path));
                    foundImage = true;
                    updated = true;
                }
                inBackgroundSection = false;
            }
            
            // Update image line
            if (inBackgroundSection && (line.startsWith("image") || line.startsWith("#image"))) {
                lines[i] = QString("image = \"%1\"").arg(path);
                foundImage = true;
                updated = true;
                qDebug() << "Updated background image in" << filename;
            }
        }
        
        // If we found [background] section but no image, add it at end of section
        if (inBackgroundSection && !foundImage) {
            lines.append(QString("image = \"%1\"").arg(path));
            updated = true;
        }
        
        // Write back if modified
        if (updated || foundImage) {
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
                qWarning() << "Failed to write" << filePath;
                continue;
            }
            
            QTextStream out(&file);
            out << lines.join('\n');
            file.close();
            
            qDebug() << "Updated" << filePath;
            break; // Stop after updating first file with [background]
        }
    }
    
    // If no file had background config, create in background.toml
    if (!updated) {
        QString targetFile = configDir + "/background.toml";
        
        QFile file(targetFile);
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            out << "\n[background]\n";
            out << "enabled = true\n";
            out << QString("image = \"%1\"\n").arg(path);
            out << "mode = \"fill\"\n";
            file.close();
            qDebug() << "Added background section to" << targetFile;
            updated = true;
        }
    }
    
    if (updated) {
        m_currentWallpaper = path;
        emit wallpaperChanged();
        qDebug() << "Wallpaper updated in config:" << path;
        
        // Generate colors with matugen
        generateColors(path);
    } else {
        emit error("Failed to update wallpaper in any config file");
    }
}

void WallpaperManager::generateColors(const QString &wallpaperPath) {
    QProcess *process = new QProcess(this);
    
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [this, process](int exitCode, QProcess::ExitStatus status) {
            if (exitCode == 0 && status == QProcess::NormalExit) {
                qDebug() << "Matugen colors generated successfully";
                emit colorsGenerated();
            } else {
                qWarning() << "Matugen failed:" << process->errorString();
                emit error("Failed to generate colors with matugen");
            }
            process->deleteLater();
        });
    
    // Run matugen
    QStringList args = {"image", wallpaperPath, "-j", "json"};
    process->start("matugen", args);
    
    qDebug() << "Running matugen on:" << wallpaperPath;
}

QString WallpaperManager::selectWallpaper() {
    // This is called from QML FileDialog, just return empty
    // The actual file selection is done in QML
    return QString();
}
