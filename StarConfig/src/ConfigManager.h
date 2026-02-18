#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QFileSystemWatcher>
#include <QTextStream>

extern "C" {
#include "toml.h"
#include "starview_ipc.h"
}

class ConfigManager : public QObject {
    Q_OBJECT
    
    // General
    Q_PROPERTY(int gapSize READ gapSize WRITE setGapSize NOTIFY configChanged)
    Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth NOTIFY configChanged)
    Q_PROPERTY(QString borderColor READ borderColor WRITE setBorderColor NOTIFY configChanged)
    Q_PROPERTY(QString focusedBorderColor READ focusedBorderColor WRITE setFocusedBorderColor NOTIFY configChanged)
    
    // Decoration
    Q_PROPERTY(bool decorationsEnabled READ decorationsEnabled WRITE setDecorationsEnabled NOTIFY configChanged)
    Q_PROPERTY(int titlebarHeight READ titlebarHeight WRITE setTitlebarHeight NOTIFY configChanged)
    Q_PROPERTY(QString decorationBgColor READ decorationBgColor WRITE setDecorationBgColor NOTIFY configChanged)
    Q_PROPERTY(QString decorationTextColor READ decorationTextColor WRITE setDecorationTextColor NOTIFY configChanged)
    
    // Wallpaper
    Q_PROPERTY(QString wallpaperPath READ wallpaperPath WRITE setWallpaperPath NOTIFY configChanged)
    Q_PROPERTY(QString wallpaperMode READ wallpaperMode WRITE setWallpaperMode NOTIFY configChanged)
    
public:
    explicit ConfigManager(QObject *parent = nullptr);
    ~ConfigManager();
    
    // Getters
    int gapSize() const { return m_gapSize; }
    int borderWidth() const { return m_borderWidth; }
    QString borderColor() const { return m_borderColor; }
    QString focusedBorderColor() const { return m_focusedBorderColor; }
    
    bool decorationsEnabled() const { return m_decorationsEnabled; }
    int titlebarHeight() const { return m_titlebarHeight; }
    QString decorationBgColor() const { return m_decorationBgColor; }
    QString decorationTextColor() const { return m_decorationTextColor; }
    
    QString wallpaperPath() const { return m_wallpaperPath; }
    QString wallpaperMode() const { return m_wallpaperMode; }
    
    // Setters
    void setGapSize(int size);
    void setBorderWidth(int width);
    void setBorderColor(const QString &color);
    void setFocusedBorderColor(const QString &color);
    
    void setDecorationsEnabled(bool enabled);
    void setTitlebarHeight(int height);
    void setDecorationBgColor(const QString &color);
    void setDecorationTextColor(const QString &color);
    
    void setWallpaperPath(const QString &path);
    void setWallpaperMode(const QString &mode);
    
    Q_INVOKABLE void load();
    Q_INVOKABLE void save();
    Q_INVOKABLE void reload();
    Q_INVOKABLE void reloadCompositor();
    
signals:
    void configChanged();
    void saved();
    void reloadFailed(QString error);
    
private:
    void loadFromFile();
    void saveToFile();
    void writeSectionContent(QTextStream &out, const QString &section);
    QString configPath() const;
    
    StarViewIPC *m_ipc = nullptr;
    QFileSystemWatcher *m_watcher = nullptr;
    
    // General
    int m_gapSize = 10;
    int m_borderWidth = 2;
    QString m_borderColor = "#45475a";
    QString m_focusedBorderColor = "#89b4fa";
    
    // Decoration
    bool m_decorationsEnabled = true;
    int m_titlebarHeight = 30;
    QString m_decorationBgColor = "#1e1e2e";
    QString m_decorationTextColor = "#cdd6f4";
    
    // Wallpaper
    QString m_wallpaperPath;
    QString m_wallpaperMode = "fill";
};

#endif // CONFIGMANAGER_H
