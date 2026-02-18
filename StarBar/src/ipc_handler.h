#ifndef IPCHANDLER_H
#define IPCHANDLER_H

#include <QObject>
#include <QLocalSocket>
#include <QTimer>

class IpcHandler : public QObject {
    Q_OBJECT
    
public:
    explicit IpcHandler(QObject *parent = nullptr);
    ~IpcHandler();
    
    Q_INVOKABLE void connect();
    Q_INVOKABLE void switchWorkspace(int ws);
    
signals:
    void workspaceChanged(int ws);
    void focusedWindowChanged(QString title);
    void connected();
    void disconnected();
    
private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onError(QLocalSocket::LocalSocketError error);
    void tryReconnect();
    
private:
    void sendMessage(uint32_t type, const QByteArray &payload);
    void subscribe();
    void getWorkspaces();
    
    QLocalSocket *m_socket;
    QTimer *m_reconnectTimer;
    QByteArray m_readBuffer;
    int m_currentWorkspace = 1;
    QString m_focusedWindowTitle;
};

#endif // IPCHANDLER_H
