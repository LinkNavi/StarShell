#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QVariantList>
#include <QFileSystemWatcher>
#include <QTextStream>
#include <QTimer>

extern "C" {
#include "toml.h"
#include "starview_ipc.h"
}

class ConfigManager : public QObject {
    Q_OBJECT

    // General
    Q_PROPERTY(int gapsInner READ gapsInner WRITE setGapsInner NOTIFY configChanged)
    Q_PROPERTY(int gapsOuter READ gapsOuter WRITE setGapsOuter NOTIFY configChanged)
    Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth NOTIFY configChanged)
    Q_PROPERTY(QString borderColorActive READ borderColorActive WRITE setBorderColorActive NOTIFY configChanged)
    Q_PROPERTY(QString borderColorInactive READ borderColorInactive WRITE setBorderColorInactive NOTIFY configChanged)
    Q_PROPERTY(bool focusFollowsMouse READ focusFollowsMouse WRITE setFocusFollowsMouse NOTIFY configChanged)
    Q_PROPERTY(QString defaultMode READ defaultMode WRITE setDefaultMode NOTIFY configChanged)
    Q_PROPERTY(int resizeStep READ resizeStep WRITE setResizeStep NOTIFY configChanged)
    Q_PROPERTY(int moveStep READ moveStep WRITE setMoveStep NOTIFY configChanged)

    // Decoration
    Q_PROPERTY(bool decorEnabled READ decorEnabled WRITE setDecorEnabled NOTIFY configChanged)
    Q_PROPERTY(int decorHeight READ decorHeight WRITE setDecorHeight NOTIFY configChanged)
    Q_PROPERTY(int decorButtonSize READ decorButtonSize WRITE setDecorButtonSize NOTIFY configChanged)
    Q_PROPERTY(int decorButtonSpacing READ decorButtonSpacing WRITE setDecorButtonSpacing NOTIFY configChanged)
    Q_PROPERTY(int decorCornerRadius READ decorCornerRadius WRITE setDecorCornerRadius NOTIFY configChanged)
    Q_PROPERTY(QString decorBgColor READ decorBgColor WRITE setDecorBgColor NOTIFY configChanged)
    Q_PROPERTY(QString decorBgColorInactive READ decorBgColorInactive WRITE setDecorBgColorInactive NOTIFY configChanged)
    Q_PROPERTY(QString decorTitleColor READ decorTitleColor WRITE setDecorTitleColor NOTIFY configChanged)
    Q_PROPERTY(QString decorTitleColorInactive READ decorTitleColorInactive WRITE setDecorTitleColorInactive NOTIFY configChanged)
    Q_PROPERTY(QString decorCloseColor READ decorCloseColor WRITE setDecorCloseColor NOTIFY configChanged)
    Q_PROPERTY(QString decorMaxColor READ decorMaxColor WRITE setDecorMaxColor NOTIFY configChanged)
    Q_PROPERTY(QString decorMinColor READ decorMinColor WRITE setDecorMinColor NOTIFY configChanged)
    Q_PROPERTY(QString decorFont READ decorFont WRITE setDecorFont NOTIFY configChanged)
    Q_PROPERTY(int decorFontSize READ decorFontSize WRITE setDecorFontSize NOTIFY configChanged)
    Q_PROPERTY(bool decorButtonsLeft READ decorButtonsLeft WRITE setDecorButtonsLeft NOTIFY configChanged)

    // Animation (full)
    Q_PROPERTY(bool animEnabled READ animEnabled WRITE setAnimEnabled NOTIFY configChanged)
    Q_PROPERTY(int animDuration READ animDuration WRITE setAnimDuration NOTIFY configChanged)
    Q_PROPERTY(QString animWindowOpen READ animWindowOpen WRITE setAnimWindowOpen NOTIFY configChanged)
    Q_PROPERTY(QString animWindowClose READ animWindowClose WRITE setAnimWindowClose NOTIFY configChanged)
    Q_PROPERTY(QString animWindowMove READ animWindowMove WRITE setAnimWindowMove NOTIFY configChanged)
    Q_PROPERTY(QString animWindowResize READ animWindowResize WRITE setAnimWindowResize NOTIFY configChanged)
    Q_PROPERTY(QString animWorkspaceSwitch READ animWorkspaceSwitch WRITE setAnimWorkspaceSwitch NOTIFY configChanged)
    Q_PROPERTY(QString animCurve READ animCurve WRITE setAnimCurve NOTIFY configChanged)
    Q_PROPERTY(double animFadeMin READ animFadeMin WRITE setAnimFadeMin NOTIFY configChanged)
    Q_PROPERTY(double animZoomMin READ animZoomMin WRITE setAnimZoomMin NOTIFY configChanged)

    // Tiling
    Q_PROPERTY(double masterRatio READ masterRatio WRITE setMasterRatio NOTIFY configChanged)
    Q_PROPERTY(int masterCount READ masterCount WRITE setMasterCount NOTIFY configChanged)

    // Background
    Q_PROPERTY(bool bgEnabled READ bgEnabled WRITE setBgEnabled NOTIFY configChanged)
    Q_PROPERTY(QString bgColor READ bgColor WRITE setBgColor NOTIFY configChanged)
    Q_PROPERTY(QString bgImage READ bgImage WRITE setBgImage NOTIFY configChanged)
    Q_PROPERTY(QString bgMode READ bgMode WRITE setBgMode NOTIFY configChanged)

    // Keybinds
    Q_PROPERTY(QVariantList keybinds READ keybinds NOTIFY configChanged)

    // Window rules
    Q_PROPERTY(QVariantList rules READ rules NOTIFY configChanged)

    // Autostart
    Q_PROPERTY(QStringList autostart READ autostart WRITE setAutostart NOTIFY configChanged)

    // Gestures
    Q_PROPERTY(QVariantList touchpadGestures READ touchpadGestures NOTIFY configChanged)
    Q_PROPERTY(QVariantList mouseGestures READ mouseGestures NOTIFY configChanged)
    Q_PROPERTY(double gestureSwipeThreshold READ gestureSwipeThreshold WRITE setGestureSwipeThreshold NOTIFY configChanged)
    Q_PROPERTY(double gesturePinchThreshold READ gesturePinchThreshold WRITE setGesturePinchThreshold NOTIFY configChanged)
    Q_PROPERTY(double gestureMouseThreshold READ gestureMouseThreshold WRITE setGestureMouseThreshold NOTIFY configChanged)

    // Connection state
    Q_PROPERTY(bool connected READ connected NOTIFY connectionChanged)

public:
    explicit ConfigManager(QObject *parent = nullptr);
    ~ConfigManager();

    // General
    int gapsInner() const { return m_gapsInner; }
    int gapsOuter() const { return m_gapsOuter; }
    int borderWidth() const { return m_borderWidth; }
    QString borderColorActive() const { return m_borderColorActive; }
    QString borderColorInactive() const { return m_borderColorInactive; }
    bool focusFollowsMouse() const { return m_focusFollowsMouse; }
    QString defaultMode() const { return m_defaultMode; }
    int resizeStep() const { return m_resizeStep; }
    int moveStep() const { return m_moveStep; }

    // Decoration
    bool decorEnabled() const { return m_decorEnabled; }
    int decorHeight() const { return m_decorHeight; }
    int decorButtonSize() const { return m_decorButtonSize; }
    int decorButtonSpacing() const { return m_decorButtonSpacing; }
    int decorCornerRadius() const { return m_decorCornerRadius; }
    QString decorBgColor() const { return m_decorBgColor; }
    QString decorBgColorInactive() const { return m_decorBgColorInactive; }
    QString decorTitleColor() const { return m_decorTitleColor; }
    QString decorTitleColorInactive() const { return m_decorTitleColorInactive; }
    QString decorCloseColor() const { return m_decorCloseColor; }
    QString decorMaxColor() const { return m_decorMaxColor; }
    QString decorMinColor() const { return m_decorMinColor; }
    QString decorFont() const { return m_decorFont; }
    int decorFontSize() const { return m_decorFontSize; }
    bool decorButtonsLeft() const { return m_decorButtonsLeft; }

    // Animation
    bool animEnabled() const { return m_animEnabled; }
    int animDuration() const { return m_animDuration; }
    QString animWindowOpen() const { return m_animWindowOpen; }
    QString animWindowClose() const { return m_animWindowClose; }
    QString animWindowMove() const { return m_animWindowMove; }
    QString animWindowResize() const { return m_animWindowResize; }
    QString animWorkspaceSwitch() const { return m_animWorkspaceSwitch; }
    QString animCurve() const { return m_animCurve; }
    double animFadeMin() const { return m_animFadeMin; }
    double animZoomMin() const { return m_animZoomMin; }

    // Tiling
    double masterRatio() const { return m_masterRatio; }
    int masterCount() const { return m_masterCount; }

    // Background
    bool bgEnabled() const { return m_bgEnabled; }
    QString bgColor() const { return m_bgColor; }
    QString bgImage() const { return m_bgImage; }
    QString bgMode() const { return m_bgMode; }

    // Keybinds, rules, autostart
    QVariantList keybinds() const { return m_keybinds; }
    QVariantList rules() const { return m_rules; }
    QStringList autostart() const { return m_autostart; }

    // Gestures
    QVariantList touchpadGestures() const { return m_touchpadGestures; }
    QVariantList mouseGestures() const { return m_mouseGestures; }
    double gestureSwipeThreshold() const { return m_gestureSwipeThreshold; }
    double gesturePinchThreshold() const { return m_gesturePinchThreshold; }
    double gestureMouseThreshold() const { return m_gestureMouseThreshold; }

    bool connected() const { return m_ipc != nullptr; }

    // Setters
    void setGapsInner(int v);
    void setGapsOuter(int v);
    void setBorderWidth(int v);
    void setBorderColorActive(const QString &v);
    void setBorderColorInactive(const QString &v);
    void setFocusFollowsMouse(bool v);
    void setDefaultMode(const QString &v);
    void setResizeStep(int v);
    void setMoveStep(int v);

    void setDecorEnabled(bool v);
    void setDecorHeight(int v);
    void setDecorButtonSize(int v);
    void setDecorButtonSpacing(int v);
    void setDecorCornerRadius(int v);
    void setDecorBgColor(const QString &v);
    void setDecorBgColorInactive(const QString &v);
    void setDecorTitleColor(const QString &v);
    void setDecorTitleColorInactive(const QString &v);
    void setDecorCloseColor(const QString &v);
    void setDecorMaxColor(const QString &v);
    void setDecorMinColor(const QString &v);
    void setDecorFont(const QString &v);
    void setDecorFontSize(int v);
    void setDecorButtonsLeft(bool v);

    void setAnimEnabled(bool v);
    void setAnimDuration(int v);
    void setAnimWindowOpen(const QString &v);
    void setAnimWindowClose(const QString &v);
    void setAnimWindowMove(const QString &v);
    void setAnimWindowResize(const QString &v);
    void setAnimWorkspaceSwitch(const QString &v);
    void setAnimCurve(const QString &v);
    void setAnimFadeMin(double v);
    void setAnimZoomMin(double v);

    void setMasterRatio(double v);
    void setMasterCount(int v);

    void setBgEnabled(bool v);
    void setBgColor(const QString &v);
    void setBgImage(const QString &v);
    void setBgMode(const QString &v);

    void setAutostart(const QStringList &v);

    void setGestureSwipeThreshold(double v);
    void setGesturePinchThreshold(double v);
    void setGestureMouseThreshold(double v);

    Q_INVOKABLE void load();
    Q_INVOKABLE void save();
    Q_INVOKABLE void saveAndReload();
    Q_INVOKABLE void reloadCompositor();
    Q_INVOKABLE void reconnect();

    Q_INVOKABLE void addKeybind(const QString &key, const QString &action);
    Q_INVOKABLE void removeKeybind(int index);
    Q_INVOKABLE void addRule(const QVariantMap &rule);
    Q_INVOKABLE void removeRule(int index);
    Q_INVOKABLE void addAutostart(const QString &cmd);
    Q_INVOKABLE void removeAutostart(int index);

    Q_INVOKABLE void addTouchpadGesture(int fingers, const QString &direction, const QString &action);
    Q_INVOKABLE void removeTouchpadGesture(int index);
    Q_INVOKABLE void addMouseGesture(const QString &button, const QString &modifiers, const QString &direction, const QString &action);
    Q_INVOKABLE void removeMouseGesture(int index);

    Q_INVOKABLE void applyMatugenColors(const QVariantMap &colors);

signals:
    void configChanged();
    void saved();
    void connectionChanged();
    void reloadSuccess();
    void reloadFailed(QString error);

private:
    void loadFromFile();
    void saveToFile();
    void writeFullConfig(const QString &path);
    QString configDir() const;
    QString mainConfigPath() const;
    void ensureConnection();

    StarViewIPC *m_ipc = nullptr;
    QFileSystemWatcher *m_watcher = nullptr;
    QTimer *m_reconnectTimer = nullptr;
    bool m_externalChange = false;
    bool m_dirty = false;

    // General
    int m_gapsInner = 5;
    int m_gapsOuter = 10;
    int m_borderWidth = 2;
    QString m_borderColorActive = "#89b4fa";
    QString m_borderColorInactive = "#45475a";
    bool m_focusFollowsMouse = true;
    QString m_defaultMode = "tiling";
    int m_resizeStep = 50;
    int m_moveStep = 50;

    // Decoration
    bool m_decorEnabled = true;
    int m_decorHeight = 30;
    int m_decorButtonSize = 12;
    int m_decorButtonSpacing = 8;
    int m_decorCornerRadius = 8;
    QString m_decorBgColor = "#1e1e2e";
    QString m_decorBgColorInactive = "#313244";
    QString m_decorTitleColor = "#cdd6f4";
    QString m_decorTitleColorInactive = "#6c7086";
    QString m_decorCloseColor = "#f38ba8";
    QString m_decorMaxColor = "#a6e3a1";
    QString m_decorMinColor = "#f9e2af";
    QString m_decorFont = "sans";
    int m_decorFontSize = 12;
    bool m_decorButtonsLeft = false;

    // Animation (full)
    bool m_animEnabled = true;
    int m_animDuration = 200;
    QString m_animWindowOpen = "zoom";
    QString m_animWindowClose = "fade";
    QString m_animWindowMove = "none";
    QString m_animWindowResize = "none";
    QString m_animWorkspaceSwitch = "slide";
    QString m_animCurve = "ease_out";
    double m_animFadeMin = 0.0;
    double m_animZoomMin = 0.8;

    // Tiling
    double m_masterRatio = 0.55;
    int m_masterCount = 1;

    // Background
    bool m_bgEnabled = true;
    QString m_bgColor = "#1e1e2e";
    QString m_bgImage;
    QString m_bgMode = "fill";

    // Keybinds, rules, autostart
    QVariantList m_keybinds;
    QVariantList m_rules;
    QStringList m_autostart;

    // Gestures
    QVariantList m_touchpadGestures;
    QVariantList m_mouseGestures;
    double m_gestureSwipeThreshold = 0.3;
    double m_gesturePinchThreshold = 0.15;
    double m_gestureMouseThreshold = 50.0;
};

#endif
