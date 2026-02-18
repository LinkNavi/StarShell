#ifndef MATUGENTHEME_H
#define MATUGENTHEME_H

#include <QObject>
#include <QString>
#include <QColor>
#include <QFileSystemWatcher>
#include <QDebug>

extern "C" {
#include "matugen_colors.h"
}

class MatugenTheme : public QObject {
    Q_OBJECT
    
    // Primary colors
    Q_PROPERTY(QColor primary READ primary NOTIFY colorsChanged)
    Q_PROPERTY(QColor onPrimary READ onPrimary NOTIFY colorsChanged)
    Q_PROPERTY(QColor primaryContainer READ primaryContainer NOTIFY colorsChanged)
    Q_PROPERTY(QColor onPrimaryContainer READ onPrimaryContainer NOTIFY colorsChanged)
    
    // Secondary colors
    Q_PROPERTY(QColor secondary READ secondary NOTIFY colorsChanged)
    Q_PROPERTY(QColor onSecondary READ onSecondary NOTIFY colorsChanged)
    Q_PROPERTY(QColor secondaryContainer READ secondaryContainer NOTIFY colorsChanged)
    Q_PROPERTY(QColor onSecondaryContainer READ onSecondaryContainer NOTIFY colorsChanged)
    
    // Tertiary colors
    Q_PROPERTY(QColor tertiary READ tertiary NOTIFY colorsChanged)
    Q_PROPERTY(QColor onTertiary READ onTertiary NOTIFY colorsChanged)
    Q_PROPERTY(QColor tertiaryContainer READ tertiaryContainer NOTIFY colorsChanged)
    Q_PROPERTY(QColor onTertiaryContainer READ onTertiaryContainer NOTIFY colorsChanged)
    
    // Error colors
    Q_PROPERTY(QColor error READ error NOTIFY colorsChanged)
    Q_PROPERTY(QColor onError READ onError NOTIFY colorsChanged)
    Q_PROPERTY(QColor errorContainer READ errorContainer NOTIFY colorsChanged)
    Q_PROPERTY(QColor onErrorContainer READ onErrorContainer NOTIFY colorsChanged)
    
    // Surface colors
    Q_PROPERTY(QColor surface READ surface NOTIFY colorsChanged)
    Q_PROPERTY(QColor onSurface READ onSurface NOTIFY colorsChanged)
    Q_PROPERTY(QColor surfaceVariant READ surfaceVariant NOTIFY colorsChanged)
    Q_PROPERTY(QColor onSurfaceVariant READ onSurfaceVariant NOTIFY colorsChanged)
    Q_PROPERTY(QColor surfaceDim READ surfaceDim NOTIFY colorsChanged)
    Q_PROPERTY(QColor surfaceBright READ surfaceBright NOTIFY colorsChanged)
    Q_PROPERTY(QColor surfaceContainerLowest READ surfaceContainerLowest NOTIFY colorsChanged)
    Q_PROPERTY(QColor surfaceContainerLow READ surfaceContainerLow NOTIFY colorsChanged)
    Q_PROPERTY(QColor surfaceContainer READ surfaceContainer NOTIFY colorsChanged)
    Q_PROPERTY(QColor surfaceContainerHigh READ surfaceContainerHigh NOTIFY colorsChanged)
    Q_PROPERTY(QColor surfaceContainerHighest READ surfaceContainerHighest NOTIFY colorsChanged)
    
    // Background
    Q_PROPERTY(QColor background READ background NOTIFY colorsChanged)
    Q_PROPERTY(QColor onBackground READ onBackground NOTIFY colorsChanged)
    
    // Outline
    Q_PROPERTY(QColor outline READ outline NOTIFY colorsChanged)
    Q_PROPERTY(QColor outlineVariant READ outlineVariant NOTIFY colorsChanged)
    
    // Other
    Q_PROPERTY(QColor shadow READ shadow NOTIFY colorsChanged)
    Q_PROPERTY(QColor inverseSurface READ inverseSurface NOTIFY colorsChanged)
    Q_PROPERTY(QColor inverseOnSurface READ inverseOnSurface NOTIFY colorsChanged)
    Q_PROPERTY(QColor inversePrimary READ inversePrimary NOTIFY colorsChanged)
    
public:
    explicit MatugenTheme(QObject *parent = nullptr) : QObject(parent) {
        loadColors();
        
        // Watch for changes to colors.json
        QString home = qgetenv("HOME");
        QString colorsPath = home + "/.config/matugen/colors.json";
        
        m_watcher = new QFileSystemWatcher(this);
        m_watcher->addPath(colorsPath);
        
        connect(m_watcher, &QFileSystemWatcher::fileChanged, this, [this, colorsPath]() {
            qDebug() << "Matugen colors changed, reloading...";
            loadColors();
            
            // Re-add watch (some editors remove the file on save)
            if (!m_watcher->files().contains(colorsPath)) {
                m_watcher->addPath(colorsPath);
            }
        });
    }
    
    // Getters
    QColor primary() const { return fromUint32(m_colors.primary); }
    QColor onPrimary() const { return fromUint32(m_colors.on_primary); }
    QColor primaryContainer() const { return fromUint32(m_colors.primary_container); }
    QColor onPrimaryContainer() const { return fromUint32(m_colors.on_primary_container); }
    
    QColor secondary() const { return fromUint32(m_colors.secondary); }
    QColor onSecondary() const { return fromUint32(m_colors.on_secondary); }
    QColor secondaryContainer() const { return fromUint32(m_colors.secondary_container); }
    QColor onSecondaryContainer() const { return fromUint32(m_colors.on_secondary_container); }
    
    QColor tertiary() const { return fromUint32(m_colors.tertiary); }
    QColor onTertiary() const { return fromUint32(m_colors.on_tertiary); }
    QColor tertiaryContainer() const { return fromUint32(m_colors.tertiary_container); }
    QColor onTertiaryContainer() const { return fromUint32(m_colors.on_tertiary_container); }
    
    QColor error() const { return fromUint32(m_colors.error); }
    QColor onError() const { return fromUint32(m_colors.on_error); }
    QColor errorContainer() const { return fromUint32(m_colors.error_container); }
    QColor onErrorContainer() const { return fromUint32(m_colors.on_error_container); }
    
    QColor surface() const { return fromUint32(m_colors.surface); }
    QColor onSurface() const { return fromUint32(m_colors.on_surface); }
    QColor surfaceVariant() const { return fromUint32(m_colors.surface_variant); }
    QColor onSurfaceVariant() const { return fromUint32(m_colors.on_surface_variant); }
    QColor surfaceDim() const { return fromUint32(m_colors.surface_dim); }
    QColor surfaceBright() const { return fromUint32(m_colors.surface_bright); }
    QColor surfaceContainerLowest() const { return fromUint32(m_colors.surface_container_lowest); }
    QColor surfaceContainerLow() const { return fromUint32(m_colors.surface_container_low); }
    QColor surfaceContainer() const { return fromUint32(m_colors.surface_container); }
    QColor surfaceContainerHigh() const { return fromUint32(m_colors.surface_container_high); }
    QColor surfaceContainerHighest() const { return fromUint32(m_colors.surface_container_highest); }
    
    QColor background() const { return fromUint32(m_colors.background); }
    QColor onBackground() const { return fromUint32(m_colors.on_background); }
    
    QColor outline() const { return fromUint32(m_colors.outline); }
    QColor outlineVariant() const { return fromUint32(m_colors.outline_variant); }
    
    QColor shadow() const { return fromUint32(m_colors.shadow); }
    QColor inverseSurface() const { return fromUint32(m_colors.inverse_surface); }
    QColor inverseOnSurface() const { return fromUint32(m_colors.inverse_on_surface); }
    QColor inversePrimary() const { return fromUint32(m_colors.inverse_primary); }
    
    Q_INVOKABLE void reload() {
        loadColors();
    }
    
signals:
    void colorsChanged();
    
private:
    void loadColors() {
        if (matugen_load_colors(nullptr, &m_colors)) {
            qDebug() << "Loaded matugen colors";
            matugen_print_colors(&m_colors);
            emit colorsChanged();
        } else {
            qWarning() << "Failed to load matugen colors, using defaults";
        }
    }
    
    QColor fromUint32(uint32_t color) const {
        int r = (color >> 24) & 0xFF;
        int g = (color >> 16) & 0xFF;
        int b = (color >> 8) & 0xFF;
        int a = color & 0xFF;
        return QColor(r, g, b, a);
    }
    
    MatugenColors m_colors;
    QFileSystemWatcher *m_watcher;
};

#endif // MATUGENTHEME_H
