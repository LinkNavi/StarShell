#ifndef PANEL_CONFIG_H
#define PANEL_CONFIG_H

#include <QObject>
#include <QColor>
#include <QFileSystemWatcher>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QDebug>

class PanelConfig : public QObject {
    Q_OBJECT

    Q_PROPERTY(int height READ height NOTIFY configChanged)
    Q_PROPERTY(int position READ position NOTIFY configChanged)
    Q_PROPERTY(bool showWorkspaces READ showWorkspaces NOTIFY configChanged)
    Q_PROPERTY(bool showTitle READ showTitle NOTIFY configChanged)
    Q_PROPERTY(bool showClock READ showClock NOTIFY configChanged)
    Q_PROPERTY(bool showSystray READ showSystray NOTIFY configChanged)
    Q_PROPERTY(int workspaceCount READ workspaceCount NOTIFY configChanged)
    Q_PROPERTY(QString clockFormat READ clockFormat NOTIFY configChanged)
    Q_PROPERTY(bool showDate READ showDate NOTIFY configChanged)
    Q_PROPERTY(double opacity READ opacity NOTIFY configChanged)
    Q_PROPERTY(int cornerRadius READ cornerRadius NOTIFY configChanged)
    Q_PROPERTY(int margins READ margins NOTIFY configChanged)
    Q_PROPERTY(bool floating READ floating NOTIFY configChanged)
    Q_PROPERTY(QString bgColor READ bgColor NOTIFY configChanged)
    Q_PROPERTY(QString fgColor READ fgColor NOTIFY configChanged)
    Q_PROPERTY(QString accentColor READ accentColor NOTIFY configChanged)
    Q_PROPERTY(QString workspaceActiveColor READ workspaceActiveColor NOTIFY configChanged)
    Q_PROPERTY(QString workspaceInactiveColor READ workspaceInactiveColor NOTIFY configChanged)

public:
    explicit PanelConfig(QObject *parent = nullptr) : QObject(parent) {
        m_watcher = new QFileSystemWatcher(this);

        // Debounce: coalesce rapid file events into a single reload
        m_debounce = new QTimer(this);
        m_debounce->setSingleShot(true);
        m_debounce->setInterval(150);
        QObject::connect(m_debounce, &QTimer::timeout, this, &PanelConfig::doLoad);

        // Poll: if panel.json doesn't exist yet, check every 2s
        m_pollTimer = new QTimer(this);
        m_pollTimer->setInterval(2000);
        QObject::connect(m_pollTimer, &QTimer::timeout, this, [this]() {
            QString jp = jsonPath();
            if (QFile::exists(jp)) {
                qDebug() << "PanelConfig: panel.json appeared, loading";
                m_pollTimer->stop();
                addWatch(jp);
                doLoad();
            }
        });

        // Watch the config directory - catches file creation/deletion
        QString dir = configDir();
        QDir().mkpath(dir);
        m_watcher->addPath(dir);

        QObject::connect(m_watcher, &QFileSystemWatcher::directoryChanged, this, [this](const QString &) {
            // A file was created or deleted in the directory
            QString jp = jsonPath();
            if (QFile::exists(jp)) {
                addWatch(jp);
                m_debounce->start();
            }
        });

        QObject::connect(m_watcher, &QFileSystemWatcher::fileChanged, this, [this](const QString &path) {
            qDebug() << "PanelConfig: File changed:" << path;
            // Schedule re-watch and reload after a small delay
            // (file may be mid-write or temporarily deleted during atomic save)
            QTimer::singleShot(50, this, [this, path]() {
                addWatch(path);
            });
            m_debounce->start();
        });

        // Initial load
        QString jp = jsonPath();
        QString tp = tomlPath();
        if (QFile::exists(jp)) {
            addWatch(jp);
            doLoad();
        } else if (QFile::exists(tp)) {
            addWatch(tp);
            doLoad();
        } else {
            qDebug() << "PanelConfig: No config file yet, polling...";
            m_pollTimer->start();
        }
    }

    int height() const { return m_height; }
    int position() const { return m_position; }
    bool showWorkspaces() const { return m_showWorkspaces; }
    bool showTitle() const { return m_showTitle; }
    bool showClock() const { return m_showClock; }
    bool showSystray() const { return m_showSystray; }
    int workspaceCount() const { return m_workspaceCount; }
    QString clockFormat() const { return m_clockFormat; }
    bool showDate() const { return m_showDate; }
    double opacity() const { return m_opacity; }
    int cornerRadius() const { return m_cornerRadius; }
    int margins() const { return m_margins; }
    bool floating() const { return m_floating; }
    QString bgColor() const { return m_bgColor; }
    QString fgColor() const { return m_fgColor; }
    QString accentColor() const { return m_accentColor; }
    QString workspaceActiveColor() const { return m_workspaceActiveColor; }
    QString workspaceInactiveColor() const { return m_workspaceInactiveColor; }

    Q_INVOKABLE void reload() { doLoad(); }

signals:
    void configChanged();

private:
    void addWatch(const QString &path) {
        if (!m_watcher->files().contains(path) && QFile::exists(path)) {
            m_watcher->addPath(path);
        }
    }

    QString configDir() const { return QDir::homePath() + "/.config/starview"; }
    QString jsonPath() const { return configDir() + "/panel.json"; }
    QString tomlPath() const { return configDir() + "/starview.toml"; }

    void doLoad() {
        QString jp = jsonPath();
        if (QFile::exists(jp)) {
            loadJson(jp);
            addWatch(jp);
            return;
        }

        QString tp = tomlPath();
        if (QFile::exists(tp)) {
            loadToml(tp);
            addWatch(tp);
            return;
        }

        if (!m_pollTimer->isActive())
            m_pollTimer->start();
    }

    void loadJson(const QString &path) {
        QFile f(path);
        if (!f.open(QIODevice::ReadOnly)) {
            qWarning() << "PanelConfig: Failed to open" << path;
            return;
        }
        QByteArray data = f.readAll();
        f.close();

        if (data.isEmpty()) return;

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(data, &err);
        if (err.error != QJsonParseError::NoError) {
            qWarning() << "PanelConfig: JSON parse error:" << err.errorString();
            return;
        }

        QJsonObject o = doc.object();
        if (o.isEmpty()) return;

        if (o.contains("height")) m_height = o["height"].toInt(40);
        if (o.contains("position")) m_position = o["position"].toInt(0);
        if (o.contains("show_workspaces")) m_showWorkspaces = o["show_workspaces"].toBool(true);
        if (o.contains("show_title")) m_showTitle = o["show_title"].toBool(true);
        if (o.contains("show_clock")) m_showClock = o["show_clock"].toBool(true);
        if (o.contains("show_systray")) m_showSystray = o["show_systray"].toBool(true);
        if (o.contains("workspace_count")) m_workspaceCount = o["workspace_count"].toInt(9);
        if (o.contains("clock_format")) m_clockFormat = o["clock_format"].toString("hh:mm");
        if (o.contains("show_date")) m_showDate = o["show_date"].toBool(false);
        if (o.contains("opacity")) m_opacity = o["opacity"].toDouble(0.95);
        if (o.contains("corner_radius")) m_cornerRadius = o["corner_radius"].toInt(0);
        if (o.contains("margins")) m_margins = o["margins"].toInt(0);
        if (o.contains("floating")) m_floating = o["floating"].toBool(false);
        if (o.contains("bg_color")) m_bgColor = o["bg_color"].toString();
        if (o.contains("fg_color")) m_fgColor = o["fg_color"].toString();
        if (o.contains("accent_color")) m_accentColor = o["accent_color"].toString();
        if (o.contains("workspace_active_color")) m_workspaceActiveColor = o["workspace_active_color"].toString();
        if (o.contains("workspace_inactive_color")) m_workspaceInactiveColor = o["workspace_inactive_color"].toString();

        qDebug() << "PanelConfig: Loaded from" << path << "height=" << m_height;
        emit configChanged();
    }

    void loadToml(const QString &path) {
        QFile f(path);
        if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return;
        QString content = f.readAll();
        f.close();

        bool inPanel = false;
        bool found = false;
        for (const QString &rawLine : content.split('\n')) {
            QString line = rawLine.trimmed();
            if (line.startsWith('[')) {
                inPanel = (line == "[panel]");
                continue;
            }
            if (!inPanel || line.isEmpty() || line.startsWith('#')) continue;
            found = true;

            int eq = line.indexOf('=');
            if (eq < 0) continue;
            QString key = line.left(eq).trimmed();
            QString val = line.mid(eq + 1).trimmed();
            if (val.startsWith('"') && val.endsWith('"'))
                val = val.mid(1, val.length() - 2);

            if (key == "height") m_height = val.toInt();
            else if (key == "position") m_position = (val == "bottom") ? 1 : 0;
            else if (key == "show_workspaces") m_showWorkspaces = (val == "true");
            else if (key == "show_title") m_showTitle = (val == "true");
            else if (key == "show_clock") m_showClock = (val == "true");
            else if (key == "show_systray") m_showSystray = (val == "true");
            else if (key == "workspace_count") m_workspaceCount = val.toInt();
            else if (key == "clock_format") m_clockFormat = val;
            else if (key == "show_date") m_showDate = (val == "true");
            else if (key == "opacity") m_opacity = val.toDouble();
            else if (key == "corner_radius") m_cornerRadius = val.toInt();
            else if (key == "margins") m_margins = val.toInt();
            else if (key == "floating") m_floating = (val == "true");
            else if (key == "bg_color") m_bgColor = val;
            else if (key == "fg_color") m_fgColor = val;
            else if (key == "accent_color") m_accentColor = val;
            else if (key == "workspace_active_color") m_workspaceActiveColor = val;
            else if (key == "workspace_inactive_color") m_workspaceInactiveColor = val;
        }

        if (found) {
            qDebug() << "PanelConfig: Loaded from TOML [panel]";
            emit configChanged();
        }
    }

    QFileSystemWatcher *m_watcher;
    QTimer *m_debounce;
    QTimer *m_pollTimer;

    int m_height = 40;
    int m_position = 0;
    bool m_showWorkspaces = true;
    bool m_showTitle = true;
    bool m_showClock = true;
    bool m_showSystray = true;
    int m_workspaceCount = 9;
    QString m_clockFormat = "hh:mm";
    bool m_showDate = false;
    double m_opacity = 0.95;
    int m_cornerRadius = 0;
    int m_margins = 0;
    bool m_floating = false;
    QString m_bgColor;
    QString m_fgColor;
    QString m_accentColor;
    QString m_workspaceActiveColor;
    QString m_workspaceInactiveColor;
};

#endif // PANEL_CONFIG_H
