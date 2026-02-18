#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QVariantList>

class NetworkManager : public QObject {
    Q_OBJECT
    
    Q_PROPERTY(bool wifiEnabled READ wifiEnabled NOTIFY wifiStateChanged)
    
public:
    explicit NetworkManager(QObject *parent = nullptr);
    
    bool wifiEnabled() const { return m_wifiEnabled; }
    
    Q_INVOKABLE QVariantList scanNetworks();
    Q_INVOKABLE void connectToNetwork(const QString &ssid, const QString &password);
    Q_INVOKABLE void disconnectNetwork();
    Q_INVOKABLE void toggleWifi();
    Q_INVOKABLE QVariantList getSavedNetworks();
    Q_INVOKABLE void forgetNetwork(const QString &ssid);
    
signals:
    void wifiStateChanged();
    void networksScanned(QVariantList networks);
    void connectionSuccess();
    void connectionFailed(QString error);
    void error(QString message);
    
private:
    void updateWifiState();
    
    bool m_wifiEnabled = false;
};

#endif // NETWORKMANAGER_H
