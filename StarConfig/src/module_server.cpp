#include "module_server.h"

// ═══════════════════════════════════════════════════════════════════════════════
// ModuleModel
// ═══════════════════════════════════════════════════════════════════════════════

ModuleModel::ModuleModel(QObject *parent)
    : QAbstractListModel(parent)
{}

int ModuleModel::rowCount(const QModelIndex &) const
{
    return m_modules.size();
}

QVariant ModuleModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_modules.size())
        return {};

    const ModuleData &m = m_modules[index.row()];
    switch (role) {
        case NameRole:      return m.name;
        case TextRole:      return m.text;
        case IconRole:      return m.icon;
        case TooltipRole:   return m.tooltip;
        case ColorRole:     return m.color;
        case TypeRole:      return m.type;
        case QmlPathRole:   return m.qmlPath;
        case SlotRole:      return m.slot;
        case OrderRole:     return m.order;
        case ConnectedRole: return m.connected;
        default:            return {};
    }
}

QHash<int, QByteArray> ModuleModel::roleNames() const
{
    return {
        { NameRole,      "name"      },
        { TextRole,      "text"      },
        { IconRole,      "icon"      },
        { TooltipRole,   "tooltip"   },
        { ColorRole,     "color"     },
        { TypeRole,      "type"      },
        { QmlPathRole,   "qmlPath"   },
        { SlotRole,      "slot"      },
        { OrderRole,     "order"     },
        { ConnectedRole, "connected" },
    };
}

int ModuleModel::indexOf(const QString &name) const
{
    for (int i = 0; i < m_modules.size(); i++)
        if (m_modules[i].name == name) return i;
    return -1;
}

void ModuleModel::upsert(const ModuleData &mod)
{
    int i = indexOf(mod.name);
    if (i >= 0) {
        m_modules[i] = mod;
        emit dataChanged(index(i), index(i));
    } else {
        beginInsertRows({}, m_modules.size(), m_modules.size());
        m_modules.append(mod);
        endInsertRows();
    }
}

void ModuleModel::remove(const QString &name)
{
    int i = indexOf(name);
    if (i < 0) return;
    beginRemoveRows({}, i, i);
    m_modules.removeAt(i);
    endRemoveRows();
}

void ModuleModel::updateField(const QString &name, std::function<void(ModuleData &)> fn)
{
    int i = indexOf(name);
    if (i < 0) return;
    fn(m_modules[i]);
    emit dataChanged(index(i), index(i));
}

const QList<ModuleData> &ModuleModel::modules() const
{
    return m_modules;
}

// ═══════════════════════════════════════════════════════════════════════════════
// ModuleServer
// ═══════════════════════════════════════════════════════════════════════════════

ModuleServer::ModuleServer(QObject *parent)
    : QObject(parent)
    , m_model(new ModuleModel(this))
    , m_server(new QLocalServer(this))
    , m_watcher(new QFileSystemWatcher(this))
{
    QLocalServer::removeServer(SOCKET_PATH);

    m_server->setSocketOptions(QLocalServer::WorldAccessOption);
    if (m_server->listen(SOCKET_PATH)) {
        qDebug() << "ModuleServer: listening on" << SOCKET_PATH;
        emit listeningChanged();
    } else {
        qWarning() << "ModuleServer: failed to listen:" << m_server->errorString();
    }

    connect(m_server,  &QLocalServer::newConnection,          this, &ModuleServer::onNewConnection);
    connect(m_watcher, &QFileSystemWatcher::directoryChanged, this, &ModuleServer::scanModules);
    connect(m_watcher, &QFileSystemWatcher::fileChanged,      this, &ModuleServer::scanModules);

    QString dir = modulesDir();
    QDir().mkpath(dir);
    m_watcher->addPath(dir);

    QTimer::singleShot(0, this, &ModuleServer::scanModules);
}

ModuleServer::~ModuleServer()
{
    for (auto *p : m_processes) {
        p->kill();
        p->waitForFinished(500);
    }
    QLocalServer::removeServer(SOCKET_PATH);
}

ModuleModel *ModuleServer::model()     const { return m_model; }
bool         ModuleServer::listening() const { return m_server->isListening(); }

QString ModuleServer::modulesDir() const
{
    return QDir::homePath() + MODULES_DIR;
}

void ModuleServer::applySlots(const QVariantList &left,
                               const QVariantList &center,
                               const QVariantList &right)
{
    auto assign = [&](const QVariantList &list, const QString &slot) {
        for (int i = 0; i < list.size(); i++) {
            QString name = list[i].toString();
            m_model->updateField(name, [&](ModuleData &m) {
                m.slot  = slot;
                m.order = i;
            });
        }
    };
    assign(left,   "left");
    assign(center, "center");
    assign(right,  "right");
    emit slotsChanged();
}

void ModuleServer::createModule(const QString &name,
                                 const QString &manifest,
                                 const QString &type)
{
    QString dir = modulesDir() + name + "/";
    QDir().mkpath(dir);

    QFile mf(dir + "module.json");
    if (mf.open(QIODevice::WriteOnly | QIODevice::Text)) {
        mf.write(manifest.toUtf8());
        mf.close();
    }

    if (type == "qml") {
        QFile qf(dir + "index.qml");
        if (qf.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qf.write(R"(import QtQuick
import QtQuick.Layouts

Item {
    property color fgColor:     "#cdd6f4"
    property color dimColor:    "#6c7086"
    property color accentColor: "#89b4fa"

    implicitWidth: _label.implicitWidth + 10

    Text {
        id: _label
        anchors.centerIn: parent
        text: "hello"
        font.pixelSize: 12
        color: fgColor
    }
}
)");
            qf.close();
        }
    } else if (type == "script") {
        QFile sf(dir + "run.sh");
        if (sf.open(QIODevice::WriteOnly | QIODevice::Text)) {
            sf.write("#!/bin/sh\n"
                     "# Output plain text or a JSON object:\n"
                     "# echo '{\"text\":\"42%\",\"icon\":\"󰻠\",\"tooltip\":\"My module\"}'\n"
                     "echo \"hello\"\n");
            sf.close();
        }
        QFile::setPermissions(dir + "run.sh",
            QFileDevice::ReadOwner  | QFileDevice::WriteOwner | QFileDevice::ExeOwner |
            QFileDevice::ReadGroup  | QFileDevice::ExeGroup);
    } else if (type == "ipc") {
        QFile rf(dir + "README.md");
        if (rf.open(QIODevice::WriteOnly | QIODevice::Text)) {
            rf.write(QString(
                "# %1 — IPC Module\n\n"
                "Connect to `/tmp/starview-modules.sock` and send newline-delimited JSON:\n\n"
                "```json\n"
                "{\"module\":\"%1\", \"text\":\"42%%\", \"icon\":\"󰻠\", \"tooltip\":\"My module\"}\n"
                "```\n"
            ).arg(name).toUtf8());
            rf.close();
        }
    }

    qDebug() << "ModuleServer: created module scaffold:" << dir;
    QTimer::singleShot(100, this, &ModuleServer::scanModules);
}

void ModuleServer::scanModules()
{
    QString base = modulesDir();
    QDir dir(base);
    if (!dir.exists()) return;

    QSet<QString> found;

    for (const QString &entry : dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString manifestPath = base + entry + "/module.json";
        if (!QFile::exists(manifestPath)) continue;

        QFile f(manifestPath);
        if (!f.open(QIODevice::ReadOnly)) continue;
        QJsonObject obj = QJsonDocument::fromJson(f.readAll()).object();
        f.close();

        ModuleData mod;
        mod.name    = obj.value("name").toString(entry);
        mod.type    = obj.value("type").toString("ipc");
        mod.slot    = obj.value("slot").toString("right");
        mod.order   = obj.value("order").toInt(99);
        mod.icon    = obj.value("icon").toString();
        mod.text    = obj.value("default_text").toString();
        mod.color   = obj.value("color").toString();
        mod.tooltip = obj.value("tooltip").toString();

        if (mod.type == "qml") {
            mod.qmlPath = base + entry + "/index.qml";
        } else if (mod.type == "script") {
            QString exec = obj.value("exec").toString();
            if (!exec.isEmpty()) {
                int interval = obj.value("interval").toInt(5000);
                startScriptModule(mod.name, base + entry + "/" + exec, interval);
            }
        }

        m_model->upsert(mod);
        found.insert(mod.name);

        if (!m_watcher->files().contains(manifestPath))
            m_watcher->addPath(manifestPath);
    }

    for (const ModuleData &m : m_model->modules()) {
        if (!found.contains(m.name))
            m_model->remove(m.name);
    }

    emit slotsChanged();
}

void ModuleServer::onNewConnection()
{
    QLocalSocket *sock = m_server->nextPendingConnection();
    if (!sock) return;

    qDebug() << "ModuleServer: new IPC connection";

    connect(sock, &QLocalSocket::readyRead, this, [this, sock]() {
        onSocketData(sock);
    });
    connect(sock, &QLocalSocket::disconnected, this, [this, sock]() {
        onSocketDisconnected(sock);
        sock->deleteLater();
    });

    m_sockets.append(sock);
}

void ModuleServer::onSocketData(QLocalSocket *sock)
{
    m_socketBuffers[sock].append(sock->readAll());

    auto &buf = m_socketBuffers[sock];
    int idx;
    while ((idx = buf.indexOf('\n')) >= 0) {
        QByteArray frame = buf.left(idx).trimmed();
        buf.remove(0, idx + 1);
        if (frame.isEmpty()) continue;

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(frame, &err);
        if (err.error != QJsonParseError::NoError || !doc.isObject()) {
            qWarning() << "ModuleServer: bad JSON frame:" << err.errorString();
            continue;
        }

        QJsonObject obj = doc.object();
        QString name = obj.value("module").toString();
        if (name.isEmpty()) continue;

        m_socketToModule[sock] = name;

        m_model->updateField(name, [&](ModuleData &m) {
            if (obj.contains("text"))    m.text    = obj.value("text").toString();
            if (obj.contains("icon"))    m.icon    = obj.value("icon").toString();
            if (obj.contains("tooltip")) m.tooltip = obj.value("tooltip").toString();
            if (obj.contains("color"))   m.color   = obj.value("color").toString();
            m.connected = true;
        });

        emit moduleUpdated(name);
    }
}

void ModuleServer::onSocketDisconnected(QLocalSocket *sock)
{
    QString name = m_socketToModule.value(sock);
    if (!name.isEmpty())
        m_model->updateField(name, [](ModuleData &m) { m.connected = false; });

    m_socketBuffers.remove(sock);
    m_socketToModule.remove(sock);
    m_sockets.removeOne(sock);
    qDebug() << "ModuleServer: client disconnected:" << name;
}

void ModuleServer::startScriptModule(const QString &name,
                                      const QString &exec,
                                      int intervalMs)
{
    if (m_processes.contains(name)) {
        m_processes[name]->kill();
        m_processes[name]->deleteLater();
        m_processes.remove(name);
    }
    if (m_scriptTimers.contains(name)) {
        m_scriptTimers[name]->stop();
        m_scriptTimers[name]->deleteLater();
        m_scriptTimers.remove(name);
    }

    auto run = [this, name, exec]() {
        QProcess *proc = new QProcess(this);
        connect(proc, &QProcess::finished, this,
            [this, name, proc](int, QProcess::ExitStatus) {
                QString out = proc->readAllStandardOutput().trimmed();
                if (!out.isEmpty()) {
                    QJsonDocument doc = QJsonDocument::fromJson(out.toUtf8());
                    if (doc.isObject()) {
                        QJsonObject obj = doc.object();
                        m_model->updateField(name, [&](ModuleData &m) {
                            if (obj.contains("text"))    m.text    = obj["text"].toString();
                            if (obj.contains("icon"))    m.icon    = obj["icon"].toString();
                            if (obj.contains("tooltip")) m.tooltip = obj["tooltip"].toString();
                            if (obj.contains("color"))   m.color   = obj["color"].toString();
                        });
                    } else {
                        m_model->updateField(name, [&](ModuleData &m) { m.text = out; });
                    }
                    emit moduleUpdated(name);
                }
                proc->deleteLater();
            });
        proc->start("/bin/sh", { "-c", exec });
    };

    run();

    QTimer *timer = new QTimer(this);
    timer->setInterval(intervalMs);
    connect(timer, &QTimer::timeout, this, run);
    timer->start();
    m_scriptTimers[name] = timer;
}
