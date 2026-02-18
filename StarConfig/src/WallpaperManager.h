#ifndef WALLPAPERMANAGER_H
#define WALLPAPERMANAGER_H

#include <QObject>
#include <QString>
#include <QProcess>

class WallpaperManager : public QObject {
    Q_OBJECT
    
    Q_PROPERTY(QString currentWallpaper READ currentWallpaper NOTIFY wallpaperChanged)
    
public:
    explicit WallpaperManager(QObject *parent = nullptr);
    
    QString currentWallpaper() const { return m_currentWallpaper; }
    
    Q_INVOKABLE void setWallpaper(const QString &path);
    Q_INVOKABLE void generateColors(const QString &wallpaperPath);
    Q_INVOKABLE QString selectWallpaper();
    
signals:
    void wallpaperChanged();
    void colorsGenerated();
    void error(QString message);
    
private:
    QString m_currentWallpaper;
};

#endif // WALLPAPERMANAGER_H
