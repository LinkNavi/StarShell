#include "ipc_handler.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <cstring>

// i3/sway IPC header
#pragma pack(push, 1)
struct IpcHeader {
    char magic[6];  // "i3-ipc"
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

void IpcHandler::connect() {
    QString socketPath = qgetenv("SWAYSOCK");
    if (socketPath.isEmpty()) {
        socketPath = "/tmp/starview-ipc.sock";
    }
    
    qDebug() << "Connecting to IPC socket:" << socketPath;
    m_socket->connectToServer(socketPath);
}

void IpcHandler::onConnected() {
    qDebug() << "IPC connected!";
    m_reconnectTimer->stop();
    emit connected();
    
    // Subscribe to workspace events
    subscribe();
    // Get current workspace
    getWorkspaces();
}

void IpcHandler::onDisconnected() {
    qDebug() << "IPC disconnected";
    emit disconnected();
    m_reconnectTimer->start();
}

void IpcHandler::onError(QLocalSocket::LocalSocketError error) {
    qDebug() << "IPC error:" << error << m_socket->errorString();
    if (!m_reconnectTimer->isActive()) {
        m_reconnectTimer->start();
    }
}

void IpcHandler::tryReconnect() {
    if (m_socket->state() == QLocalSocket::UnconnectedState) {
        connect();
    }
}

void IpcHandler::sendMessage(uint32_t type, const QByteArray &payload) {
    IpcHeader header;
    memcpy(header.magic, "i3-ipc", 6);
    header.size = payload.size();
    header.type = type;
    
    m_socket->write(reinterpret_cast<char*>(&header), sizeof(header));
    if (!payload.isEmpty()) {
        m_socket->write(payload);
    }
    m_socket->flush();
}

void IpcHandler::subscribe() {
    QJsonArray events;
    events.append("workspace");
    QJsonDocument doc(events);
    sendMessage(IPC_SUBSCRIBE, doc.toJson(QJsonDocument::Compact));
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
            qDebug() << "Invalid IPC magic";
            m_readBuffer.clear();
            return;
        }
        
        size_t totalLen = sizeof(IpcHeader) + header.size;
        if (m_readBuffer.size() < (int)totalLen) {
            return; // Need more data
        }
        
        QByteArray payload = m_readBuffer.mid(sizeof(IpcHeader), header.size);
        m_readBuffer.remove(0, totalLen);
        
        // Parse response
        QJsonDocument doc = QJsonDocument::fromJson(payload);
        
        uint32_t type = header.type;
        bool isEvent = (type & IPC_EVENT_MASK) != 0;
        
        if (isEvent) {
            uint32_t eventType = type & ~IPC_EVENT_MASK;
            if (eventType == IPC_EVENT_WORKSPACE) {
                // Workspace event
                QJsonObject obj = doc.object();
                if (obj.contains("current")) {
                    int ws = obj["current"].toObject()["num"].toInt();
                    if (ws != m_currentWorkspace) {
                        m_currentWorkspace = ws;
                        emit workspaceChanged(ws);
                    }
                }
            }
        } else if (type == IPC_GET_WORKSPACES) {
            // Workspaces response
            QJsonArray arr = doc.array();
            for (const auto &item : arr) {
                QJsonObject ws = item.toObject();
                if (ws["focused"].toBool()) {
                    int num = ws["num"].toInt();
                    if (num != m_currentWorkspace) {
                        m_currentWorkspace = num;
                        emit workspaceChanged(num);
                    }
                    break;
                }
            }
        }
    }
}
