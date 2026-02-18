#ifndef MODULE_SERVER_H
#define MODULE_SERVER_H

#include <QObject>
#include <QAbstractListModel>
#include <QLocalServer>
#include <QLocalSocket>
#include <QFileSystemWatcher>
#include <QTimer>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QProcess>
#include <functional>

// ─── ModuleData ────────────────────────────────────────────────────────────────
struct ModuleData {
    QString name;
    QString text;
    QString icon;
    QString tooltip;
    QString color;
    QString type;       // "ipc" | "qml" | "script"
    QString qmlPath;
    QString slot;       // "left" | "center" | "right"
    int     order     = 0;
    bool    connected = false;
};

// ─── ModuleModel ───────────────────────────────────────────────────────────────
class ModuleModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        TextRole,
        IconRole,
        TooltipRole,
        ColorRole,
        TypeRole,
        QmlPathRole,
        SlotRole,
        OrderRole,
        ConnectedRole,
    };

    explicit ModuleModel(QObject *parent = nullptr);

    int     rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    int  indexOf(const QString &name) const;
    void upsert(const ModuleData &mod);
    void remove(const QString &name);
    void updateField(const QString &name, std::function<void(ModuleData &)> fn);

    const QList<ModuleData> &modules() const;

private:
    QList<ModuleData> m_modules;
};

// ─── ModuleServer ──────────────────────────────────────────────────────────────
class ModuleServer : public QObject {
    Q_OBJECT
    Q_PROPERTY(ModuleModel* model     READ model     CONSTANT)
    Q_PROPERTY(bool         listening READ listening NOTIFY listeningChanged)

public:
    static constexpr const char *SOCKET_PATH = "/tmp/starview-modules.sock";
    static constexpr const char *MODULES_DIR = "/.config/starview/panel/modules/";

    explicit ModuleServer(QObject *parent = nullptr);
    ~ModuleServer();

    ModuleModel *model()     const;
    bool         listening() const;
    QString      modulesDir() const;

    Q_INVOKABLE void createModule(const QString &name,
                                  const QString &manifest,
                                  const QString &type);

    Q_INVOKABLE void applySlots(const QVariantList &left,
                                const QVariantList &center,
                                const QVariantList &right);

signals:
    void listeningChanged();
    void slotsChanged();
    void moduleUpdated(QString name);

private slots:
    void scanModules();
    void onNewConnection();
    void onSocketData(QLocalSocket *sock);
    void onSocketDisconnected(QLocalSocket *sock);

private:
    void startScriptModule(const QString &name, const QString &exec, int intervalMs);

    ModuleModel        *m_model;
    QLocalServer       *m_server;
    QFileSystemWatcher *m_watcher;

    QList<QLocalSocket *>             m_sockets;
    QHash<QLocalSocket *, QByteArray> m_socketBuffers;
    QHash<QLocalSocket *, QString>    m_socketToModule;

    QHash<QString, QProcess *> m_processes;
    QHash<QString, QTimer *>   m_scriptTimers;
};

#endif // MODULE_SERVER_H
