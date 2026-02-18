#include "ColorProvider.h"
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QProcess>
#include <QDebug>
#include <QRegularExpression>

ColorProvider::ColorProvider(QObject *parent) : QObject(parent) {
    m_watcher = new QFileSystemWatcher(this);

    // Watch common matugen output locations
    QStringList watchPaths;
    QString home = QDir::homePath();
    watchPaths << home + "/.cache/matugen"
               << home + "/.config/matugen";

    for (const auto &p : watchPaths) {
        if (QDir(p).exists()) {
            m_watcher->addPath(p);
        }
    }

    connect(m_watcher, &QFileSystemWatcher::directoryChanged, this, [this]() {
        loadColors();
    });
    connect(m_watcher, &QFileSystemWatcher::fileChanged, this, [this]() {
        loadColors();
    });

    loadColors();
}

void ColorProvider::reload() {
    loadColors();
}

QString ColorProvider::color(const QString &name) const {
    return m_colors.value(name, "#ff00ff").toString();
}

QString ColorProvider::findColorFile() const {
    QString home = QDir::homePath();

    // Check common matugen output paths
    QStringList candidates = {
        home + "/.cache/matugen/colors.json",
        home + "/.config/matugen/colors.json",
        home + "/.cache/wal/colors.json",
        home + "/.cache/matugen/scheme-dark.json",
        home + "/.cache/matugen/scheme.json",
        // CSS fallback
        home + "/.config/matugen/colors.css",
        home + "/.cache/matugen/colors.css",
    };

    for (const auto &path : candidates) {
        if (QFile::exists(path)) return path;
    }
    return {};
}

void ColorProvider::loadColors() {
    QString path = findColorFile();
    if (path.isEmpty()) {
        qDebug() << "[ColorProvider] No matugen color file found, using defaults";
        return;
    }

    // Watch the file
    if (!m_watcher->files().contains(path)) {
        m_watcher->addPath(path);
    }

    if (path.endsWith(".json")) {
        parseMatugenJson(path);
    } else if (path.endsWith(".css")) {
        parseCssColors(path);
    }
}

void ColorProvider::parseMatugenJson(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (doc.isNull()) return;

    QJsonObject root = doc.object();

    // matugen outputs in various formats, handle the common ones
    // Format 1: { "colors": { "dark": { "primary": "#xxx", ... } } }
    // Format 2: { "schemes": { "dark": { "primary": "#xxx", ... } } }
    // Format 3: flat { "primary": "#xxx", ... }

    QJsonObject colors;

    if (root.contains("colors")) {
        QJsonObject c = root["colors"].toObject();
        if (c.contains("dark")) colors = c["dark"].toObject();
        else if (c.contains("light")) colors = c["light"].toObject();
        else colors = c;
    } else if (root.contains("schemes")) {
        QJsonObject s = root["schemes"].toObject();
        if (s.contains("dark")) colors = s["dark"].toObject();
        else if (s.contains("light")) colors = s["light"].toObject();
        else colors = s;
    } else {
        colors = root;
    }

    if (colors.isEmpty()) return;

    m_colors.clear();
    for (auto it = colors.begin(); it != colors.end(); ++it) {
        QString key = it.key();
        QString val = it.value().toString();

        // Normalize key: camelCase -> snake_case
        key.replace(QRegularExpression("([a-z])([A-Z])"), "\\1_\\2");
        key = key.toLower();
        key.replace('-', '_');

        if (!val.startsWith('#') && val.length() == 6) {
            val = "#" + val;
        }

        m_colors[key] = val;
    }

    m_loaded = true;
    qDebug() << "[ColorProvider] Loaded" << m_colors.size() << "colors from" << path;
    emit colorsChanged();
}

void ColorProvider::parseCssColors(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QString content = file.readAll();
    file.close();

    // Parse CSS custom properties: --color-name: #hex;
    QRegularExpression re("--([\\w-]+):\\s*(#[0-9a-fA-F]{3,8})");
    auto it = re.globalMatch(content);

    m_colors.clear();
    while (it.hasNext()) {
        auto match = it.next();
        QString key = match.captured(1);
        QString val = match.captured(2);
        key.replace('-', '_');
        m_colors[key] = val;
    }

    if (!m_colors.isEmpty()) {
        m_loaded = true;
        qDebug() << "[ColorProvider] Loaded" << m_colors.size() << "colors from CSS";
        emit colorsChanged();
    }
}

void ColorProvider::generateFromWallpaper(const QString &wallpaperPath) {
    if (wallpaperPath.isEmpty()) return;

    emit generateStarted();

    QProcess *proc = new QProcess(this);
    connect(proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [this, proc](int exitCode, QProcess::ExitStatus) {
            bool ok = exitCode == 0;
            if (ok) {
                loadColors();
            } else {
                qWarning() << "[ColorProvider] matugen failed:" << proc->readAllStandardError();
            }
            emit generateFinished(ok);
            proc->deleteLater();
        });

    // Try matugen with JSON output
    proc->start("matugen", {"image", wallpaperPath, "-j", "json"});

    if (!proc->waitForStarted(3000)) {
        qWarning() << "[ColorProvider] Failed to start matugen";
        emit generateFinished(false);
        proc->deleteLater();
    }
}
