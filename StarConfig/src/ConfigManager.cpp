#include "ConfigManager.h"
#include <QFile>
#include <QDir>
#include <QDebug>

#define SET_PROP(field, val) do { if (field != val) { field = val; emit configChanged(); } } while(0)

ConfigManager::ConfigManager(QObject *parent) : QObject(parent) {
    m_ipc = starview_ipc_connect();
    if (!m_ipc) qWarning() << "Failed to connect to StarView IPC";

    m_reconnectTimer = new QTimer(this);
    m_reconnectTimer->setInterval(5000);
    connect(m_reconnectTimer, &QTimer::timeout, this, &ConfigManager::reconnect);
    if (!m_ipc) m_reconnectTimer->start();

    loadFromFile();

    m_watcher = new QFileSystemWatcher(this);
    QString dir = configDir();
    if (QDir(dir).exists()) {
        m_watcher->addPath(dir);
        QDir d(dir);
        for (const auto &f : d.entryList({"*.toml"}, QDir::Files))
            m_watcher->addPath(dir + "/" + f);
    }

    connect(m_watcher, &QFileSystemWatcher::fileChanged, this, [this](const QString &path) {
        if (!m_externalChange) {
            qDebug() << "Config changed externally:" << path;
            loadFromFile();
            if (!m_watcher->files().contains(path))
                m_watcher->addPath(path);
        }
    });
}

ConfigManager::~ConfigManager() {
    if (m_ipc) starview_ipc_disconnect(m_ipc);
}

QString ConfigManager::configDir() const {
    return QDir::homePath() + "/.config/starview";
}

QString ConfigManager::mainConfigPath() const {
    return configDir() + "/starview.toml";
}

void ConfigManager::ensureConnection() {
    if (m_ipc) return;
    m_ipc = starview_ipc_connect();
    if (m_ipc) {
        m_reconnectTimer->stop();
        emit connectionChanged();
    }
}

void ConfigManager::reconnect() {
    if (m_ipc) return;
    m_ipc = starview_ipc_connect();
    if (m_ipc) {
        m_reconnectTimer->stop();
        emit connectionChanged();
        qDebug() << "Reconnected to compositor IPC";
    }
}

void ConfigManager::loadFromFile() {
    QString path = mainConfigPath();
    if (!QFile::exists(path)) {
        // Try loading from split files
        QDir dir(configDir());
        for (const auto &f : dir.entryList({"*.toml"}, QDir::Files)) {
            path = configDir() + "/" + f;
            break;
        }
    }

    FILE *fp = fopen(path.toUtf8().constData(), "r");
    if (!fp) { qWarning() << "Cannot open config:" << path; return; }

    char errbuf[256];
    toml_table_t *root = toml_parse_file(fp, errbuf, sizeof(errbuf));
    fclose(fp);
    if (!root) { qWarning() << "Parse error:" << errbuf; return; }

    toml_datum_t v;

    // [general]
    toml_table_t *general = toml_table_in(root, "general");
    if (general) {
        v = toml_int_in(general, "gaps_inner"); if (v.ok) m_gapsInner = v.u.i;
        v = toml_int_in(general, "gaps_outer"); if (v.ok) m_gapsOuter = v.u.i;
        v = toml_int_in(general, "border_width"); if (v.ok) m_borderWidth = v.u.i;
        v = toml_string_in(general, "border_color_active");
        if (v.ok) { m_borderColorActive = QString("#%1").arg(v.u.s); free(v.u.s); }
        v = toml_string_in(general, "border_color_inactive");
        if (v.ok) { m_borderColorInactive = QString("#%1").arg(v.u.s); free(v.u.s); }
        v = toml_bool_in(general, "focus_follows_mouse"); if (v.ok) m_focusFollowsMouse = v.u.b;
        v = toml_string_in(general, "default_mode");
        if (v.ok) { m_defaultMode = QString(v.u.s); free(v.u.s); }
        v = toml_int_in(general, "resize_step"); if (v.ok) m_resizeStep = v.u.i;
        v = toml_int_in(general, "move_step"); if (v.ok) m_moveStep = v.u.i;
    }

    // [decoration]
    toml_table_t *decor = toml_table_in(root, "decoration");
    if (decor) {
        v = toml_bool_in(decor, "enabled"); if (v.ok) m_decorEnabled = v.u.b;
        v = toml_int_in(decor, "height"); if (v.ok) m_decorHeight = v.u.i;
        v = toml_int_in(decor, "button_size"); if (v.ok) m_decorButtonSize = v.u.i;
        v = toml_int_in(decor, "button_spacing"); if (v.ok) m_decorButtonSpacing = v.u.i;
        v = toml_int_in(decor, "corner_radius"); if (v.ok) m_decorCornerRadius = v.u.i;
        v = toml_string_in(decor, "bg_color");
        if (v.ok) { m_decorBgColor = QString("#%1").arg(v.u.s); free(v.u.s); }
        v = toml_string_in(decor, "bg_color_inactive");
        if (v.ok) { m_decorBgColorInactive = QString("#%1").arg(v.u.s); free(v.u.s); }
        v = toml_string_in(decor, "title_color");
        if (v.ok) { m_decorTitleColor = QString("#%1").arg(v.u.s); free(v.u.s); }
        v = toml_string_in(decor, "title_color_inactive");
        if (v.ok) { m_decorTitleColorInactive = QString("#%1").arg(v.u.s); free(v.u.s); }
        v = toml_string_in(decor, "close_color");
        if (v.ok) { m_decorCloseColor = QString("#%1").arg(v.u.s); free(v.u.s); }
        v = toml_string_in(decor, "maximize_color");
        if (v.ok) { m_decorMaxColor = QString("#%1").arg(v.u.s); free(v.u.s); }
        v = toml_string_in(decor, "minimize_color");
        if (v.ok) { m_decorMinColor = QString("#%1").arg(v.u.s); free(v.u.s); }
        v = toml_string_in(decor, "font");
        if (v.ok) { m_decorFont = QString(v.u.s); free(v.u.s); }
        v = toml_int_in(decor, "font_size"); if (v.ok) m_decorFontSize = v.u.i;
        v = toml_bool_in(decor, "buttons_left"); if (v.ok) m_decorButtonsLeft = v.u.b;
    }

    // [animation]
    toml_table_t *anim = toml_table_in(root, "animation");
    if (anim) {
        v = toml_bool_in(anim, "enabled"); if (v.ok) m_animEnabled = v.u.b;
        v = toml_int_in(anim, "duration"); if (v.ok) m_animDuration = v.u.i;
        v = toml_string_in(anim, "window_open");
        if (v.ok) { m_animWindowOpen = QString(v.u.s); free(v.u.s); }
        v = toml_string_in(anim, "window_close");
        if (v.ok) { m_animWindowClose = QString(v.u.s); free(v.u.s); }
        v = toml_string_in(anim, "curve");
        if (v.ok) { m_animCurve = QString(v.u.s); free(v.u.s); }
    }

    // [tiling]
    toml_table_t *tiling = toml_table_in(root, "tiling");
    if (tiling) {
        v = toml_double_in(tiling, "master_ratio"); if (v.ok) m_masterRatio = v.u.d;
        v = toml_int_in(tiling, "master_count"); if (v.ok) m_masterCount = v.u.i;
    }

    // [background]
    toml_table_t *bg = toml_table_in(root, "background");
    if (bg) {
        v = toml_bool_in(bg, "enabled"); if (v.ok) m_bgEnabled = v.u.b;
        v = toml_string_in(bg, "color");
        if (v.ok) { m_bgColor = QString("#%1").arg(v.u.s); free(v.u.s); }
        v = toml_string_in(bg, "image");
        if (v.ok) { m_bgImage = QString(v.u.s); free(v.u.s); }
        v = toml_string_in(bg, "mode");
        if (v.ok) { m_bgMode = QString(v.u.s); free(v.u.s); }
    }

    // [keybinds]
    m_keybinds.clear();
    toml_table_t *kb = toml_table_in(root, "keybinds");
    if (kb) {
        int i = 0;
        const char *key;
        while ((key = toml_key_in(kb, i++)) != NULL) {
            toml_datum_t val = toml_string_in(kb, key);
            if (val.ok) {
                QVariantMap bind;
                bind["key"] = QString(key);
                bind["action"] = QString(val.u.s);
                m_keybinds.append(bind);
                free(val.u.s);
            }
        }
    }

    // [[rules]]
    m_rules.clear();
    toml_array_t *rulesArr = toml_array_in(root, "rules");
    if (rulesArr) {
        int len = toml_array_nelem(rulesArr);
        for (int i = 0; i < len; i++) {
            toml_table_t *r = toml_table_at(rulesArr, i);
            if (!r) continue;
            QVariantMap rule;
            v = toml_string_in(r, "app_id"); if (v.ok) { rule["app_id"] = QString(v.u.s); free(v.u.s); }
            v = toml_string_in(r, "title"); if (v.ok) { rule["title"] = QString(v.u.s); free(v.u.s); }
            v = toml_bool_in(r, "floating"); if (v.ok) rule["floating"] = (bool)v.u.b;
            v = toml_bool_in(r, "fullscreen"); if (v.ok) rule["fullscreen"] = (bool)v.u.b;
            v = toml_int_in(r, "workspace"); if (v.ok) rule["workspace"] = (int)v.u.i;
            m_rules.append(rule);
        }
    }

    // autostart
    m_autostart.clear();
    toml_array_t *as = toml_array_in(root, "autostart");
    if (as) {
        int len = toml_array_nelem(as);
        for (int i = 0; i < len; i++) {
            toml_datum_t val = toml_string_at(as, i);
            if (val.ok) { m_autostart.append(QString(val.u.s)); free(val.u.s); }
        }
    }

    toml_free(root);
    emit configChanged();
    qDebug() << "Config loaded:" << m_keybinds.size() << "keybinds," << m_rules.size() << "rules";
}

static QString stripHash(const QString &c) {
    return c.startsWith('#') ? c.mid(1) : c;
}

void ConfigManager::writeFullConfig(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "Cannot write config:" << path;
        return;
    }
    QTextStream out(&file);

    out << "[general]\n";
    out << "gaps_inner = " << m_gapsInner << "\n";
    out << "gaps_outer = " << m_gapsOuter << "\n";
    out << "border_width = " << m_borderWidth << "\n";
    out << "border_color_active = \"" << stripHash(m_borderColorActive) << "\"\n";
    out << "border_color_inactive = \"" << stripHash(m_borderColorInactive) << "\"\n";
    out << "focus_follows_mouse = " << (m_focusFollowsMouse ? "true" : "false") << "\n";
    out << "default_mode = \"" << m_defaultMode << "\"\n";
    out << "resize_step = " << m_resizeStep << "\n";
    out << "move_step = " << m_moveStep << "\n";

    out << "\n[decoration]\n";
    out << "enabled = " << (m_decorEnabled ? "true" : "false") << "\n";
    out << "height = " << m_decorHeight << "\n";
    out << "button_size = " << m_decorButtonSize << "\n";
    out << "button_spacing = " << m_decorButtonSpacing << "\n";
    out << "corner_radius = " << m_decorCornerRadius << "\n";
    out << "bg_color = \"" << stripHash(m_decorBgColor) << "\"\n";
    out << "bg_color_inactive = \"" << stripHash(m_decorBgColorInactive) << "\"\n";
    out << "title_color = \"" << stripHash(m_decorTitleColor) << "\"\n";
    out << "title_color_inactive = \"" << stripHash(m_decorTitleColorInactive) << "\"\n";
    out << "close_color = \"" << stripHash(m_decorCloseColor) << "\"\n";
    out << "maximize_color = \"" << stripHash(m_decorMaxColor) << "\"\n";
    out << "minimize_color = \"" << stripHash(m_decorMinColor) << "\"\n";
    out << "font = \"" << m_decorFont << "\"\n";
    out << "font_size = " << m_decorFontSize << "\n";
    out << "buttons_left = " << (m_decorButtonsLeft ? "true" : "false") << "\n";

    out << "\n[animation]\n";
    out << "enabled = " << (m_animEnabled ? "true" : "false") << "\n";
    out << "duration = " << m_animDuration << "\n";
    out << "window_open = \"" << m_animWindowOpen << "\"\n";
    out << "window_close = \"" << m_animWindowClose << "\"\n";
    out << "curve = \"" << m_animCurve << "\"\n";

    out << "\n[tiling]\n";
    out << "master_ratio = " << m_masterRatio << "\n";
    out << "master_count = " << m_masterCount << "\n";

    out << "\n[background]\n";
    out << "enabled = " << (m_bgEnabled ? "true" : "false") << "\n";
    out << "color = \"" << stripHash(m_bgColor) << "\"\n";
    if (!m_bgImage.isEmpty())
        out << "image = \"" << m_bgImage << "\"\n";
    out << "mode = \"" << m_bgMode << "\"\n";

    // Keybinds
    if (!m_keybinds.isEmpty()) {
        out << "\n[keybinds]\n";
        for (const auto &kb : m_keybinds) {
            QVariantMap m = kb.toMap();
            out << "\"" << m["key"].toString() << "\" = \"" << m["action"].toString() << "\"\n";
        }
    }

    // Rules
    for (const auto &r : m_rules) {
        QVariantMap m = r.toMap();
        out << "\n[[rules]]\n";
        if (m.contains("app_id")) out << "app_id = \"" << m["app_id"].toString() << "\"\n";
        if (m.contains("title")) out << "title = \"" << m["title"].toString() << "\"\n";
        if (m.contains("floating")) out << "floating = " << (m["floating"].toBool() ? "true" : "false") << "\n";
        if (m.contains("fullscreen")) out << "fullscreen = " << (m["fullscreen"].toBool() ? "true" : "false") << "\n";
        if (m.contains("workspace")) out << "workspace = " << m["workspace"].toInt() << "\n";
    }

    // Autostart
    if (!m_autostart.isEmpty()) {
        out << "\nautostart = [\n";
        for (int i = 0; i < m_autostart.size(); i++) {
            out << "  \"" << m_autostart[i] << "\"";
            if (i < m_autostart.size() - 1) out << ",";
            out << "\n";
        }
        out << "]\n";
    }

    file.close();
}

void ConfigManager::saveToFile() {
    QDir dir;
    dir.mkpath(configDir());

    m_externalChange = true;
    writeFullConfig(mainConfigPath());
    m_externalChange = false;

    emit saved();
    qDebug() << "Config saved to" << mainConfigPath();
}

void ConfigManager::load() { loadFromFile(); }
void ConfigManager::save() { saveToFile(); }

void ConfigManager::saveAndReload() {
    saveToFile();
    reloadCompositor();
}

void ConfigManager::reloadCompositor() {
    ensureConnection();
    if (!m_ipc) {
        emit reloadFailed("Not connected to compositor");
        return;
    }
    if (starview_ipc_reload_config(m_ipc)) {
        qDebug() << "Compositor config reloaded";
        emit reloadSuccess();
    } else {
        emit reloadFailed("IPC reload command failed");
    }
}

void ConfigManager::addKeybind(const QString &key, const QString &action) {
    QVariantMap kb;
    kb["key"] = key;
    kb["action"] = action;
    m_keybinds.append(kb);
    emit configChanged();
}

void ConfigManager::removeKeybind(int index) {
    if (index >= 0 && index < m_keybinds.size()) {
        m_keybinds.removeAt(index);
        emit configChanged();
    }
}

void ConfigManager::addRule(const QVariantMap &rule) {
    m_rules.append(rule);
    emit configChanged();
}

void ConfigManager::removeRule(int index) {
    if (index >= 0 && index < m_rules.size()) {
        m_rules.removeAt(index);
        emit configChanged();
    }
}

void ConfigManager::addAutostart(const QString &cmd) {
    m_autostart.append(cmd);
    emit configChanged();
}

void ConfigManager::removeAutostart(int index) {
    if (index >= 0 && index < m_autostart.size()) {
        m_autostart.removeAt(index);
        emit configChanged();
    }
}

void ConfigManager::applyMatugenColors(const QVariantMap &colors) {
    if (colors.contains("primary")) m_borderColorActive = colors["primary"].toString();
    if (colors.contains("surface_variant")) m_borderColorInactive = colors["surface_variant"].toString();
    if (colors.contains("surface")) m_decorBgColor = colors["surface"].toString();
    if (colors.contains("surface_variant")) m_decorBgColorInactive = colors["surface_variant"].toString();
    if (colors.contains("on_surface")) m_decorTitleColor = colors["on_surface"].toString();
    if (colors.contains("on_surface_variant")) m_decorTitleColorInactive = colors["on_surface_variant"].toString();
    if (colors.contains("error")) m_decorCloseColor = colors["error"].toString();
    if (colors.contains("secondary")) m_decorMaxColor = colors["secondary"].toString();
    if (colors.contains("tertiary")) m_decorMinColor = colors["tertiary"].toString();
    if (colors.contains("background")) m_bgColor = colors["background"].toString();
    emit configChanged();
}

// Setters
void ConfigManager::setGapsInner(int v) { SET_PROP(m_gapsInner, v); }
void ConfigManager::setGapsOuter(int v) { SET_PROP(m_gapsOuter, v); }
void ConfigManager::setBorderWidth(int v) { SET_PROP(m_borderWidth, v); }
void ConfigManager::setBorderColorActive(const QString &v) { SET_PROP(m_borderColorActive, v); }
void ConfigManager::setBorderColorInactive(const QString &v) { SET_PROP(m_borderColorInactive, v); }
void ConfigManager::setFocusFollowsMouse(bool v) { SET_PROP(m_focusFollowsMouse, v); }
void ConfigManager::setDefaultMode(const QString &v) { SET_PROP(m_defaultMode, v); }
void ConfigManager::setResizeStep(int v) { SET_PROP(m_resizeStep, v); }
void ConfigManager::setMoveStep(int v) { SET_PROP(m_moveStep, v); }
void ConfigManager::setDecorEnabled(bool v) { SET_PROP(m_decorEnabled, v); }
void ConfigManager::setDecorHeight(int v) { SET_PROP(m_decorHeight, v); }
void ConfigManager::setDecorButtonSize(int v) { SET_PROP(m_decorButtonSize, v); }
void ConfigManager::setDecorButtonSpacing(int v) { SET_PROP(m_decorButtonSpacing, v); }
void ConfigManager::setDecorCornerRadius(int v) { SET_PROP(m_decorCornerRadius, v); }
void ConfigManager::setDecorBgColor(const QString &v) { SET_PROP(m_decorBgColor, v); }
void ConfigManager::setDecorBgColorInactive(const QString &v) { SET_PROP(m_decorBgColorInactive, v); }
void ConfigManager::setDecorTitleColor(const QString &v) { SET_PROP(m_decorTitleColor, v); }
void ConfigManager::setDecorTitleColorInactive(const QString &v) { SET_PROP(m_decorTitleColorInactive, v); }
void ConfigManager::setDecorCloseColor(const QString &v) { SET_PROP(m_decorCloseColor, v); }
void ConfigManager::setDecorMaxColor(const QString &v) { SET_PROP(m_decorMaxColor, v); }
void ConfigManager::setDecorMinColor(const QString &v) { SET_PROP(m_decorMinColor, v); }
void ConfigManager::setDecorFont(const QString &v) { SET_PROP(m_decorFont, v); }
void ConfigManager::setDecorFontSize(int v) { SET_PROP(m_decorFontSize, v); }
void ConfigManager::setDecorButtonsLeft(bool v) { SET_PROP(m_decorButtonsLeft, v); }
void ConfigManager::setAnimEnabled(bool v) { SET_PROP(m_animEnabled, v); }
void ConfigManager::setAnimDuration(int v) { SET_PROP(m_animDuration, v); }
void ConfigManager::setAnimWindowOpen(const QString &v) { SET_PROP(m_animWindowOpen, v); }
void ConfigManager::setAnimWindowClose(const QString &v) { SET_PROP(m_animWindowClose, v); }
void ConfigManager::setAnimCurve(const QString &v) { SET_PROP(m_animCurve, v); }
void ConfigManager::setMasterRatio(double v) { SET_PROP(m_masterRatio, v); }
void ConfigManager::setMasterCount(int v) { SET_PROP(m_masterCount, v); }
void ConfigManager::setBgEnabled(bool v) { SET_PROP(m_bgEnabled, v); }
void ConfigManager::setBgColor(const QString &v) { SET_PROP(m_bgColor, v); }
void ConfigManager::setBgImage(const QString &v) { SET_PROP(m_bgImage, v); }
void ConfigManager::setBgMode(const QString &v) { SET_PROP(m_bgMode, v); }
void ConfigManager::setAutostart(const QStringList &v) { m_autostart = v; emit configChanged(); }
