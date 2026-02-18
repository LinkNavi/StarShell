#include "ipc_handler.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <cstring>

#pragma pack(push, 1)
struct IpcHeader {
    char magic[6];
    uint32_t size;
    uint32_t type;
};
#pragma pack(pop)

enum IpcType {
    IPC_COMMAND = 0,
    IPC_GET_WORKSPACES = 1,
    IPC_SUBSCRIBE = 2,
};

#define IPC_EVENT_MASK 0x80000000
#define IPC_EVENT_WORKSPACE 0
#define IPC_EVENT_WINDOW 3

IpcHandler::IpcHandler(QObject *parent)
    : QObject(parent)
    , m_socket(new QLocalSocket(this))
    , m_reconnectTimer(new QTimer(this))
{
    QObject::connect(m_socket, &QLocalSocket::connected, this, &IpcHandler::onConnected);
    QObject::connect(m_socket, &QLocalSocket::disconnected, this, &IpcHandler::onDisconnected);
    QObject::connect(m_socket, &QLocalSocket::readyRead, this, &IpcHandler::onReadyRead);
    QObject::connect(m_socket, &QLocalSocket::errorOccurred, this, &IpcHandler::onError);

    m_reconnectTimer->setInterval(2000);
    QObject::connect(m_reconnectTimer, &QTimer::timeout, this, &IpcHandler::tryReconnect);
}

IpcHandler::~IpcHandler() {
    m_socket->close();
}

void IpcHandler::connectToCompositor() {
    QString socketPath = qgetenv("SWAYSOCK");
    if (socketPath.isEmpty())
        socketPath = "/tmp/starview-ipc.sock";
    qDebug() << "IPC: Connecting to" << socketPath;
    m_socket->connectToServer(socketPath);
}

void IpcHandler::onConnected() {
    qDebug() << "IPC: Connected!";
    m_reconnectTimer->stop();
    emit ipcConnected();
    subscribe();
    QTimer::singleShot(100, this, &IpcHandler::getWorkspaces);
}

void IpcHandler::onDisconnected() {
    qDebug() << "IPC: Disconnected";
    emit ipcDisconnected();
    m_reconnectTimer->start();
}

void IpcHandler::onError(QLocalSocket::LocalSocketError err) {
    qDebug() << "IPC: Error" << err << m_socket->errorString();
    if (!m_reconnectTimer->isActive())
        m_reconnectTimer->start();
}

void IpcHandler::tryReconnect() {
    if (m_socket->state() == QLocalSocket::UnconnectedState) {
        qDebug() << "IPC: Attempting reconnect...";
        connectToCompositor();
    }
}

void IpcHandler::sendMessage(uint32_t type, const QByteArray &payload) {
    if (m_socket->state() != QLocalSocket::ConnectedState) return;

    IpcHeader header;
    memcpy(header.magic, "i3-ipc", 6);
    header.size = payload.size();
    header.type = type;

    m_socket->write(reinterpret_cast<char*>(&header), sizeof(header));
    if (!payload.isEmpty())
        m_socket->write(payload);
    m_socket->flush();
}

void IpcHandler::subscribe() {
    QJsonArray events;
    events.append("workspace");
    events.append("window");
    events.append("mode");
    QByteArray payload = QJsonDocument(events).toJson(QJsonDocument::Compact);
    qDebug() << "IPC: Subscribing to events:" << payload;
    sendMessage(IPC_SUBSCRIBE, payload);
}

void IpcHandler::getWorkspaces() {
    sendMessage(IPC_GET_WORKSPACES, QByteArray());
}

void IpcHandler::switchWorkspace(int ws) {
    QString cmd = QString("workspace %1").arg(ws);
    sendMessage(IPC_COMMAND, cmd.toUtf8());
}

void IpcHandler::onReadyRead() {
    m_readBuffer.append(m_socket->readAll());

    while (m_readBuffer.size() >= (int)sizeof(IpcHeader)) {
        IpcHeader header;
        memcpy(&header, m_readBuffer.constData(), sizeof(header));

        if (memcmp(header.magic, "i3-ipc", 6) != 0) {
            qWarning() << "IPC: Invalid magic, clearing buffer";
            m_readBuffer.clear();
            return;
        }

        int totalLen = sizeof(IpcHeader) + header.size;
        if (m_readBuffer.size() < totalLen) return;

        QByteArray payload = m_readBuffer.mid(sizeof(IpcHeader), header.size);
        m_readBuffer.remove(0, totalLen);

        uint32_t type = header.type;
        bool isEvent = (type & IPC_EVENT_MASK) != 0;

        if (isEvent) {
            uint32_t eventType = type & ~IPC_EVENT_MASK;

            if (eventType == IPC_EVENT_WORKSPACE) {
                QJsonObject obj = QJsonDocument::fromJson(payload).object();
                QString change = obj["change"].toString();
                qDebug() << "IPC: Workspace event:" << change;

                if (change == "focus" || change == "init") {
                    if (obj.contains("current")) {
                        int ws = obj["current"].toObject()["num"].toInt();
                        qDebug() << "IPC: Workspace now:" << ws;
                        if (ws != m_currentWorkspace) {
                            m_currentWorkspace = ws;
                            emit workspaceChanged(ws);
                        }
                    }
                }
            } else if (eventType == IPC_EVENT_WINDOW) {
                QJsonObject obj = QJsonDocument::fromJson(payload).object();
                if (obj["change"].toString() == "focus" || obj["change"].toString() == "title") {
                    QString title = obj["container"].toObject()["name"].toString();
                    if (title != m_focusedWindowTitle) {
                        m_focusedWindowTitle = title;
                        emit focusedWindowChanged(title);
                    }
                }
            }
        } else if (type == IPC_GET_WORKSPACES) {
            QJsonArray arr = QJsonDocument::fromJson(payload).array();
            for (const auto &item : arr) {
                QJsonObject ws = item.toObject();
                if (ws["focused"].toBool()) {
                    int num = ws["num"].toInt();
                    qDebug() << "IPC: Current workspace from list:" << num;
                    if (num != m_currentWorkspace) {
                        m_currentWorkspace = num;
                        emit workspaceChanged(num);
                    }
                    break;
                }
            }
        } else if (type == IPC_SUBSCRIBE) {
            QJsonDocument doc = QJsonDocument::fromJson(payload);
            qDebug() << "IPC: Subscribe response:" << doc.toJson(QJsonDocument::Compact);
        } else if (type == IPC_COMMAND) {
            // After switching workspace via click, refresh workspace list
            QTimer::singleShot(50, this, &IpcHandler::getWorkspaces);
        }
    }
}
