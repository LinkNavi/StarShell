#include <DesktopFileParser.h>
#include <QRegularExpression>

DesktopFileParser::DesktopFileParser(QObject *parent) : QObject(parent) {
    loadDesktopFiles();
}

void DesktopFileParser::loadDesktopFiles() {
    m_apps.clear();
    
    QStringList searchPaths = {
        "/usr/share/applications",
        "/usr/local/share/applications",
        QDir::homePath() + "/.local/share/applications"
    };
    
    for (const QString &path : searchPaths) {
        QDir dir(path);
        if (!dir.exists()) continue;
        
        QStringList desktopFiles = dir.entryList(QStringList() << "*.desktop", QDir::Files);
        for (const QString &file : desktopFiles) {
            QString fullPath = dir.absoluteFilePath(file);
            DesktopApp app = parseDesktopFile(fullPath);
            if (!app.name.isEmpty() && !app.noDisplay) {
                m_apps.append(app);
            }
        }
    }
    
    qDebug() << "Loaded" << m_apps.size() << "desktop applications";
}

DesktopApp DesktopFileParser::parseDesktopFile(const QString &filePath) {
    DesktopApp app;
    app.desktopFile = filePath;
    app.noDisplay = false;
    
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return app;
    }
    
    QTextStream in(&file);
    bool inDesktopEntry = false;
    
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        
        if (line == "[Desktop Entry]") {
            inDesktopEntry = true;
            continue;
        } else if (line.startsWith("[") && line.endsWith("]")) {
            inDesktopEntry = false;
            continue;
        }
        
        if (!inDesktopEntry || line.isEmpty() || line.startsWith("#")) {
            continue;
        }
        
        int equalPos = line.indexOf('=');
        if (equalPos == -1) continue;
        
        QString key = line.left(equalPos).trimmed();
        QString value = line.mid(equalPos + 1).trimmed();
        
        if (key == "Name") {
            app.name = value;
        } else if (key == "Exec") {
            // Remove field codes like %u, %f, etc.
            app.exec = value.remove(QRegularExpression(" %[fFuUdDnNickvm]"));
        } else if (key == "Icon") {
            app.icon = value;
        } else if (key == "NoDisplay" && value.toLower() == "true") {
            app.noDisplay = true;
        } else if (key == "Type" && value != "Application") {
            app.noDisplay = true;
        }
    }
    
    file.close();
    return app;
}

DesktopApp DesktopFileParser::findByCommand(const QString &command) {
    // Extract just the binary name from the command
    QString binName = command.split(' ').first();
    binName = binName.split('/').last();
    
    for (const DesktopApp &app : m_apps) {
        QString appBinName = app.exec.split(' ').first();
        appBinName = appBinName.split('/').last();
        
        if (appBinName == binName || app.exec.contains(binName)) {
            return app;
        }
    }
    
    return DesktopApp();
}

DesktopApp DesktopFileParser::findByName(const QString &name) {
    for (const DesktopApp &app : m_apps) {
        if (app.name.compare(name, Qt::CaseInsensitive) == 0) {
            return app;
        }
    }
    return DesktopApp();
}

QVariantList DesktopFileParser::getAllApps() const {
    QVariantList list;
    for (const DesktopApp &app : m_apps) {
        list.append(app.toVariantMap());
    }
    return list;
}

QString DesktopFileParser::getIconPath(const QString &iconName) {
    // Try to find icon in standard locations
    if (iconName.isEmpty()) {
        return "";
    }
    
    // If it's already a path, return it
    if (iconName.startsWith("/")) {
        return "file://" + iconName;
    }
    
    // Try standard icon directories
    QStringList iconPaths = {
        "/usr/share/pixmaps",
        "/usr/share/icons/hicolor/48x48/apps",
        "/usr/share/icons/hicolor/64x64/apps",
        "/usr/share/icons/hicolor/scalable/apps",
        "/usr/share/icons/hicolor/128x128/apps",
        QDir::homePath() + "/.local/share/icons/hicolor/48x48/apps"
    };
    
    QStringList extensions = {"", ".png", ".svg", ".xpm"};
    
    for (const QString &basePath : iconPaths) {
        for (const QString &ext : extensions) {
            QString fullPath = basePath + "/" + iconName + ext;
            if (QFile::exists(fullPath)) {
                return "file://" + fullPath;
            }
        }
    }
    
    return "";
}
