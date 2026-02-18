#include "NetworkManager.h"
#include <QProcess>
#include <QDebug>

NetworkManager::NetworkManager(QObject *parent) : QObject(parent) {
    updateWifiState();
}

void NetworkManager::updateWifiState() {
    QProcess process;
    process.start("nmcli", QStringList() << "radio" << "wifi");
    process.waitForFinished();
    
    QString output = process.readAllStandardOutput().trimmed();
    bool enabled = (output == "enabled");
    
    if (m_wifiEnabled != enabled) {
        m_wifiEnabled = enabled;
        emit wifiStateChanged();
    }
}

QVariantList NetworkManager::scanNetworks() {
    QVariantList networks;
    
    // Rescan
    QProcess::execute("nmcli", QStringList() << "device" << "wifi" << "rescan");
    
    // Get list
    QProcess process;
    process.start("nmcli", QStringList() << "-t" << "-f" << "SSID,SIGNAL,SECURITY" << "device" << "wifi" << "list");
    process.waitForFinished();
    
    if (process.exitCode() != 0) {
        emit error("Failed to scan networks");
        return networks;
    }
    
    QString output = process.readAllStandardOutput();
    QStringList lines = output.split('\n');
    
    for (const QString &line : lines) {
        if (line.isEmpty()) continue;
        
        QStringList parts = line.split(':');
        if (parts.size() < 3) continue;
        
        QVariantMap network;
        network["ssid"] = parts[0];
        network["signal"] = parts[1].toInt();
        network["security"] = parts[2];
        
        networks.append(network);
    }
    
    emit networksScanned(networks);
    return networks;
}

void NetworkManager::connectToNetwork(const QString &ssid, const QString &password) {
    QProcess *process = new QProcess(this);
    
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [this, process, ssid](int exitCode, QProcess::ExitStatus status) {
            if (exitCode == 0 && status == QProcess::NormalExit) {
                qDebug() << "Connected to network:" << ssid;
                emit connectionSuccess();
            } else {
                QString error = process->readAllStandardError();
                qWarning() << "Failed to connect:" << error;
                emit connectionFailed(error);
            }
            process->deleteLater();
        });
    
    QStringList args;
    args << "device" << "wifi" << "connect" << ssid;
    if (!password.isEmpty()) {
        args << "password" << password;
    }
    
    process->start("nmcli", args);
}

void NetworkManager::disconnectNetwork() {
    QProcess::execute("nmcli", QStringList() << "device" << "disconnect" << "wlan0");
}

void NetworkManager::toggleWifi() {
    QString state = m_wifiEnabled ? "off" : "on";
    QProcess::execute("nmcli", QStringList() << "radio" << "wifi" << state);
    updateWifiState();
}

QVariantList NetworkManager::getSavedNetworks() {
    QVariantList networks;
    
    QProcess process;
    process.start("nmcli", QStringList() << "-t" << "-f" << "NAME,TYPE" << "connection" << "show");
    process.waitForFinished();
    
    if (process.exitCode() != 0) {
        emit error("Failed to get saved networks");
        return networks;
    }
    
    QString output = process.readAllStandardOutput();
    QStringList lines = output.split('\n');
    
    for (const QString &line : lines) {
        if (line.isEmpty()) continue;
        
        QStringList parts = line.split(':');
        if (parts.size() < 2) continue;
        
        // Only wifi connections
        if (parts[1] == "802-11-wireless") {
            QVariantMap network;
            network["name"] = parts[0];
            networks.append(network);
        }
    }
    
    return networks;
}

void NetworkManager::forgetNetwork(const QString &ssid) {
    QProcess::execute("nmcli", QStringList() << "connection" << "delete" << ssid);
}
