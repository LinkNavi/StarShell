#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QColor>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QStandardPaths>

// You'll need to include your TOML library here
// #include "toml.h"

class Config : public QObject {
    Q_OBJECT
    
    // Dock properties
    Q_PROPERTY(QString position READ position NOTIFY configChanged)
    Q_PROPERTY(int dockHeight READ dockHeight NOTIFY configChanged)
    Q_PROPERTY(int iconSize READ iconSize NOTIFY configChanged)
    Q_PROPERTY(int iconSizeZoomed READ iconSizeZoomed NOTIFY configChanged)
    Q_PROPERTY(int spacing READ spacing NOTIFY configChanged)
    Q_PROPERTY(int padding READ padding NOTIFY configChanged)
    
    // Appearance properties
    Q_PROPERTY(QString backgroundColor READ backgroundColor NOTIFY configChanged)
    Q_PROPERTY(double backgroundOpacity READ backgroundOpacity NOTIFY configChanged)
    Q_PROPERTY(QString borderColor READ borderColor NOTIFY configChanged)
    Q_PROPERTY(int borderWidth READ borderWidth NOTIFY configChanged)
    Q_PROPERTY(int cornerRadius READ cornerRadius NOTIFY configChanged)
    Q_PROPERTY(QString accentColor READ accentColor NOTIFY configChanged)
    Q_PROPERTY(QString accentColorFocused READ accentColorFocused NOTIFY configChanged)
    Q_PROPERTY(QString textColor READ textColor NOTIFY configChanged)
    Q_PROPERTY(int textSize READ textSize NOTIFY configChanged)
    
    // Animation properties
    Q_PROPERTY(bool animationsEnabled READ animationsEnabled NOTIFY configChanged)
    Q_PROPERTY(int zoomDuration READ zoomDuration NOTIFY configChanged)
    Q_PROPERTY(int fadeDuration READ fadeDuration NOTIFY configChanged)
    Q_PROPERTY(QString zoomEasing READ zoomEasing NOTIFY configChanged)
    
    // Launcher properties
    Q_PROPERTY(int launcherWidth READ launcherWidth NOTIFY configChanged)
    Q_PROPERTY(int launcherHeight READ launcherHeight NOTIFY configChanged)
    Q_PROPERTY(int launcherCellWidth READ launcherCellWidth NOTIFY configChanged)
    Q_PROPERTY(int launcherCellHeight READ launcherCellHeight NOTIFY configChanged)
    Q_PROPERTY(int launcherIconSize READ launcherIconSize NOTIFY configChanged)
    Q_PROPERTY(QString searchPlaceholder READ searchPlaceholder NOTIFY configChanged)
    
    // Tracking properties
    Q_PROPERTY(int updateInterval READ updateInterval NOTIFY configChanged)
    Q_PROPERTY(QStringList skipProcesses READ skipProcesses NOTIFY configChanged)
    
    // Indicator properties
    Q_PROPERTY(bool indicatorsEnabled READ indicatorsEnabled NOTIFY configChanged)
    Q_PROPERTY(QString indicatorColor READ indicatorColor NOTIFY configChanged)
    Q_PROPERTY(int indicatorSize READ indicatorSize NOTIFY configChanged)
    Q_PROPERTY(double indicatorOpacity READ indicatorOpacity NOTIFY configChanged)
    
    // Separator properties
    Q_PROPERTY(bool separatorEnabled READ separatorEnabled NOTIFY configChanged)
    Q_PROPERTY(int separatorWidth READ separatorWidth NOTIFY configChanged)
    Q_PROPERTY(int separatorHeight READ separatorHeight NOTIFY configChanged)
    Q_PROPERTY(QString separatorColor READ separatorColor NOTIFY configChanged)
    
public:
    explicit Config(QObject *parent = nullptr) : QObject(parent) {
        loadConfig();
    }
    
    // Dock getters
    QString position() const { return m_position; }
    int dockHeight() const { return m_dockHeight; }
    int iconSize() const { return m_iconSize; }
    int iconSizeZoomed() const { return m_iconSizeZoomed; }
    int spacing() const { return m_spacing; }
    int padding() const { return m_padding; }
    int exclusiveZone() const { return m_exclusiveZone; }
    
    // Appearance getters
    QString backgroundColor() const { return m_backgroundColor; }
    double backgroundOpacity() const { return m_backgroundOpacity; }
    QString borderColor() const { return m_borderColor; }
    int borderWidth() const { return m_borderWidth; }
    int cornerRadius() const { return m_cornerRadius; }
    QString accentColor() const { return m_accentColor; }
    QString accentColorFocused() const { return m_accentColorFocused; }
    QString textColor() const { return m_textColor; }
    int textSize() const { return m_textSize; }
    
    // Shadow getters
    bool shadowEnabled() const { return m_shadowEnabled; }
    QString shadowColor() const { return m_shadowColor; }
    double shadowBlur() const { return m_shadowBlur; }
    int shadowOffsetX() const { return m_shadowOffsetX; }
    int shadowOffsetY() const { return m_shadowOffsetY; }
    
    // Animation getters
    bool animationsEnabled() const { return m_animationsEnabled; }
    int zoomDuration() const { return m_zoomDuration; }
    int fadeDuration() const { return m_fadeDuration; }
    int colorDuration() const { return m_colorDuration; }
    QString zoomEasing() const { return m_zoomEasing; }
    
    // Launcher getters
    int launcherWidth() const { return m_launcherWidth; }
    int launcherHeight() const { return m_launcherHeight; }
    int launcherCellWidth() const { return m_launcherCellWidth; }
    int launcherCellHeight() const { return m_launcherCellHeight; }
    int launcherIconSize() const { return m_launcherIconSize; }
    QString searchPlaceholder() const { return m_searchPlaceholder; }
    bool backdropEnabled() const { return m_backdropEnabled; }
    QString backdropColor() const { return m_backdropColor; }
    
    // Helper methods
    Q_INVOKABLE QString getEasingType() const;
    
    // Tracking getters
    int updateInterval() const { return m_updateInterval; }
    QStringList skipProcesses() const { return m_skipProcesses; }
    
    // Icon getters
    QStringList iconSearchPaths() const { return m_iconSearchPaths; }
    bool useTextFallback() const { return m_useTextFallback; }
    int fallbackTextSize() const { return m_fallbackTextSize; }
    
    // Desktop files getters
    QStringList desktopFileSearchPaths() const { return m_desktopFileSearchPaths; }
    
    // Indicator getters
    bool indicatorsEnabled() const { return m_indicatorsEnabled; }
    QString indicatorColor() const { return m_indicatorColor; }
    int indicatorSize() const { return m_indicatorSize; }
    double indicatorOpacity() const { return m_indicatorOpacity; }
    
    // Separator getters
    bool separatorEnabled() const { return m_separatorEnabled; }
    int separatorWidth() const { return m_separatorWidth; }
    int separatorHeight() const { return m_separatorHeight; }
    QString separatorColor() const { return m_separatorColor; }
    double separatorOpacity() const { return m_separatorOpacity; }
    
    // Behavior getters
    bool tooltipsEnabled() const { return m_tooltipsEnabled; }
    int tooltipDelay() const { return m_tooltipDelay; }
    
    Q_INVOKABLE void reload() {
        loadConfig();
        emit configChanged();
    }
    
signals:
    void configChanged();
    
private:
    void loadConfig();    
    void loadDefaults();    
    // Dock members
    QString m_position;
    int m_dockHeight;
    int m_iconSize;
    int m_iconSizeZoomed;
    int m_spacing;
    int m_padding;
    int m_exclusiveZone;
    
    // Appearance members
    QString m_backgroundColor;
    double m_backgroundOpacity;
    QString m_borderColor;
    int m_borderWidth;
    int m_cornerRadius;
    QString m_accentColor;
    QString m_accentColorFocused;
    QString m_textColor;
    int m_textSize;
    
    // Shadow members
    bool m_shadowEnabled;
    QString m_shadowColor;
    double m_shadowBlur;
    int m_shadowOffsetX;
    int m_shadowOffsetY;
    
    // Animation members
    bool m_animationsEnabled;
    int m_zoomDuration;
    int m_fadeDuration;
    int m_colorDuration;
    QString m_zoomEasing;
    
    // Launcher members
    int m_launcherWidth;
    int m_launcherHeight;
    int m_launcherCellWidth;
    int m_launcherCellHeight;
    int m_launcherIconSize;
    QString m_searchPlaceholder;
    bool m_backdropEnabled;
    QString m_backdropColor;
    
    // Tracking members
    int m_updateInterval;
    QStringList m_skipProcesses;
    
    // Icon members
    QStringList m_iconSearchPaths;
    bool m_useTextFallback;
    int m_fallbackTextSize;
    
    // Desktop files members
    QStringList m_desktopFileSearchPaths;
    
    // Indicator members
    bool m_indicatorsEnabled;
    QString m_indicatorColor;
    int m_indicatorSize;
    double m_indicatorOpacity;
    
    // Separator members
    bool m_separatorEnabled;
    int m_separatorWidth;
    int m_separatorHeight;
    QString m_separatorColor;
    double m_separatorOpacity;
    
    // Behavior members
    bool m_tooltipsEnabled;
    int m_tooltipDelay;
};

#endif // CONFIG_H
