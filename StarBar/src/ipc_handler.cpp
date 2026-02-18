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
    
    // IMPORTANT: Subscribe to workspace events FIRST
    subscribe();
    
    // Then get initial workspace state
    QTimer::singleShot(100, this, &IpcHandler::getWorkspaces);
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
        qDebug() << "Attempting reconnect...";
        connect();
    }
}

void IpcHandler::sendMessage(uint32_t type, const QByteArray &payload) {
    if (m_socket->state() != QLocalSocket::ConnectedState) {
        qDebug() << "Cannot send message: socket not connected";
        return;
    }
    
    IpcHeader header;
    memcpy(header.magic, "i3-ipc", 6);
    header.size = payload.size();
    header.type = type;
    
    m_socket->write(reinterpret_cast<char*>(&header), sizeof(header));
    if (!payload.isEmpty()) {
        m_socket->write(payload);
    }
    m_socket->flush();
    
    qDebug() << "Sent IPC message: type=" << type << "payload size=" << payload.size();
}

void IpcHandler::subscribe() {
    QJsonArray events;
    events.append("workspace");
    events.append("window");
    events.append("mode");
    
    QJsonDocument doc(events);
    QByteArray payload = doc.toJson(QJsonDocument::Compact);
    
    qDebug() << "Subscribing to events:" << payload;
    sendMessage(IPC_SUBSCRIBE, payload);
}

void IpcHandler::getWorkspaces() {
    qDebug() << "Requesting workspace list...";
    sendMessage(IPC_GET_WORKSPACES, QByteArray());
}

void IpcHandler::switchWorkspace(int ws) {
    QString cmd = QString("workspace %1").arg(ws);
    qDebug() << "Switching workspace:" << cmd;
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
        uint32_t type = header.type;
        bool isEvent = (type & IPC_EVENT_MASK) != 0;
        
        qDebug() << "Received IPC message: type=" << type << "isEvent=" << isEvent << "payload:" << payload;
        
        if (isEvent) {
            uint32_t eventType = type & ~IPC_EVENT_MASK;
            qDebug() << "Event type:" << eventType;
            
            if (eventType == IPC_EVENT_WORKSPACE) {
                // Parse workspace event
                QJsonDocument doc = QJsonDocument::fromJson(payload);
                QJsonObject obj = doc.object();
                
                qDebug() << "Workspace event received:" << doc.toJson(QJsonDocument::Compact);
                
                // Check for workspace change
                if (obj.contains("change")) {
                    QString change = obj["change"].toString();
                    qDebug() << "Workspace change type:" << change;
                    
                    if (change == "focus" || change == "init") {
                        // Get current workspace from the event
                        if (obj.contains("current")) {
                            QJsonObject current = obj["current"].toObject();
                            int ws = current["num"].toInt();
                            qDebug() << "Workspace changed to:" << ws;
                            
                            if (ws != m_currentWorkspace) {
                                m_currentWorkspace = ws;
                                emit workspaceChanged(ws);
                            }
                        }
                    }
                }
            } else if (eventType == IPC_EVENT_WINDOW) {
                // Parse window event
                QJsonDocument doc = QJsonDocument::fromJson(payload);
                QJsonObject obj = doc.object();
                
                qDebug() << "Window event received:" << doc.toJson(QJsonDocument::Compact);
                
                QString change = obj["change"].toString();
                if (change == "focus") {
                    QJsonObject container = obj["container"].toObject();
                    QString title = container["name"].toString();
                    
                    if (title != m_focusedWindowTitle) {
                        m_focusedWindowTitle = title;
                        emit focusedWindowChanged(title);
                        qDebug() << "Focused window changed to:" << title;
                    }
                }
            }
        } else if (type == IPC_GET_WORKSPACES) {
            // Workspaces response
            QJsonDocument doc = QJsonDocument::fromJson(payload);
            QJsonArray arr = doc.array();
            
            qDebug() << "Workspace list received:" << doc.toJson(QJsonDocument::Compact);
            
            for (const auto &item : arr) {
                QJsonObject ws = item.toObject();
                if (ws["focused"].toBool()) {
                    int num = ws["num"].toInt();
                    qDebug() << "Current workspace from list:" << num;
                    
                    if (num != m_currentWorkspace) {
                        m_currentWorkspace = num;
                        emit workspaceChanged(num);
                    }
                    break;
                }
            }
        } else if (type == IPC_SUBSCRIBE) {
            // Subscribe response
            QJsonDocument doc = QJsonDocument::fromJson(payload);
            qDebug() << "Subscribe response:" << doc.toJson(QJsonDocument::Compact);
        } else if (type == IPC_COMMAND) {
            // Command response
            QJsonDocument doc = QJsonDocument::fromJson(payload);
            qDebug() << "Command response:" << doc.toJson(QJsonDocument::Compact);
            
            // After switching workspace, refresh the workspace list
            QTimer::singleShot(50, this, &IpcHandler::getWorkspaces);
        }
    }
}
