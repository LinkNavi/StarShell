#ifndef COLORPROVIDER_H
#define COLORPROVIDER_H

#include <QObject>
#include <QVariantMap>
#include <QFileSystemWatcher>
#include <QColor>

class ColorProvider : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString primary READ primary NOTIFY colorsChanged)
    Q_PROPERTY(QString onPrimary READ onPrimary NOTIFY colorsChanged)
    Q_PROPERTY(QString primaryContainer READ primaryContainer NOTIFY colorsChanged)
    Q_PROPERTY(QString onPrimaryContainer READ onPrimaryContainer NOTIFY colorsChanged)
    Q_PROPERTY(QString secondary READ secondary NOTIFY colorsChanged)
    Q_PROPERTY(QString onSecondary READ onSecondary NOTIFY colorsChanged)
    Q_PROPERTY(QString secondaryContainer READ secondaryContainer NOTIFY colorsChanged)
    Q_PROPERTY(QString onSecondaryContainer READ onSecondaryContainer NOTIFY colorsChanged)
    Q_PROPERTY(QString tertiary READ tertiary NOTIFY colorsChanged)
    Q_PROPERTY(QString onTertiary READ onTertiary NOTIFY colorsChanged)
    Q_PROPERTY(QString tertiaryContainer READ tertiaryContainer NOTIFY colorsChanged)
    Q_PROPERTY(QString onTertiaryContainer READ onTertiaryContainer NOTIFY colorsChanged)
    Q_PROPERTY(QString background READ background NOTIFY colorsChanged)
    Q_PROPERTY(QString onBackground READ onBackground NOTIFY colorsChanged)
    Q_PROPERTY(QString surface READ surface NOTIFY colorsChanged)
    Q_PROPERTY(QString onSurface READ onSurface NOTIFY colorsChanged)
    Q_PROPERTY(QString surfaceVariant READ surfaceVariant NOTIFY colorsChanged)
    Q_PROPERTY(QString onSurfaceVariant READ onSurfaceVariant NOTIFY colorsChanged)
    Q_PROPERTY(QString outline READ outline NOTIFY colorsChanged)
    Q_PROPERTY(QString outlineVariant READ outlineVariant NOTIFY colorsChanged)
    Q_PROPERTY(QString error READ error NOTIFY colorsChanged)
    Q_PROPERTY(QString onError READ onError NOTIFY colorsChanged)
    Q_PROPERTY(QString inverseSurface READ inverseSurface NOTIFY colorsChanged)
    Q_PROPERTY(QString inverseOnSurface READ inverseOnSurface NOTIFY colorsChanged)
    Q_PROPERTY(QString inversePrimary READ inversePrimary NOTIFY colorsChanged)
    Q_PROPERTY(bool loaded READ loaded NOTIFY colorsChanged)

public:
    explicit ColorProvider(QObject *parent = nullptr);

    QString primary() const { return m_colors.value("primary", "#89b4fa").toString(); }
    QString onPrimary() const { return m_colors.value("on_primary", "#1e1e2e").toString(); }
    QString primaryContainer() const { return m_colors.value("primary_container", "#313244").toString(); }
    QString onPrimaryContainer() const { return m_colors.value("on_primary_container", "#cdd6f4").toString(); }
    QString secondary() const { return m_colors.value("secondary", "#a6e3a1").toString(); }
    QString onSecondary() const { return m_colors.value("on_secondary", "#1e1e2e").toString(); }
    QString secondaryContainer() const { return m_colors.value("secondary_container", "#313244").toString(); }
    QString onSecondaryContainer() const { return m_colors.value("on_secondary_container", "#cdd6f4").toString(); }
    QString tertiary() const { return m_colors.value("tertiary", "#f38ba8").toString(); }
    QString onTertiary() const { return m_colors.value("on_tertiary", "#1e1e2e").toString(); }
    QString tertiaryContainer() const { return m_colors.value("tertiary_container", "#313244").toString(); }
    QString onTertiaryContainer() const { return m_colors.value("on_tertiary_container", "#cdd6f4").toString(); }
    QString background() const { return m_colors.value("background", "#1e1e2e").toString(); }
    QString onBackground() const { return m_colors.value("on_background", "#cdd6f4").toString(); }
    QString surface() const { return m_colors.value("surface", "#1e1e2e").toString(); }
    QString onSurface() const { return m_colors.value("on_surface", "#cdd6f4").toString(); }
    QString surfaceVariant() const { return m_colors.value("surface_variant", "#313244").toString(); }
    QString onSurfaceVariant() const { return m_colors.value("on_surface_variant", "#a6adc8").toString(); }
    QString outline() const { return m_colors.value("outline", "#6c7086").toString(); }
    QString outlineVariant() const { return m_colors.value("outline_variant", "#45475a").toString(); }
    QString error() const { return m_colors.value("error", "#f38ba8").toString(); }
    QString onError() const { return m_colors.value("on_error", "#1e1e2e").toString(); }
    QString inverseSurface() const { return m_colors.value("inverse_surface", "#cdd6f4").toString(); }
    QString inverseOnSurface() const { return m_colors.value("inverse_on_surface", "#1e1e2e").toString(); }
    QString inversePrimary() const { return m_colors.value("inverse_primary", "#45475a").toString(); }
    bool loaded() const { return m_loaded; }

    Q_INVOKABLE void reload();
    Q_INVOKABLE void generateFromWallpaper(const QString &wallpaperPath);
    Q_INVOKABLE QString color(const QString &name) const;

signals:
    void colorsChanged();
    void generateStarted();
    void generateFinished(bool success);

private:
    void loadColors();
    void parseMatugenJson(const QString &path);
    void parseCssColors(const QString &path);
    QString findColorFile() const;

    QVariantMap m_colors;
    QFileSystemWatcher *m_watcher;
    bool m_loaded = false;
};

#endif
