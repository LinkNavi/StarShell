#ifndef QUICKPANELBACKEND_H
#define QUICKPANELBACKEND_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariantList>
#include <QTimer>
#include <QProcess>
#include <qdbusconnection.h>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusMessage>
#include <QDBusVariant>
#include <QDBusMetaType>
#include <QDebug>

class QuickPanelBackend : public QObject {
    Q_OBJECT

    // ── Volume ────────────────────────────────────────────────────────────────
    Q_PROPERTY(int     volume      READ volume      NOTIFY volumeChanged)
    Q_PROPERTY(bool    muted       READ muted       NOTIFY volumeChanged)

    // ── Brightness ────────────────────────────────────────────────────────────
    Q_PROPERTY(int     brightness  READ brightness  NOTIFY brightnessChanged)
    Q_PROPERTY(int     maxBrightness READ maxBrightness NOTIFY brightnessChanged)

    // ── Network ───────────────────────────────────────────────────────────────
    Q_PROPERTY(bool    wifiEnabled READ wifiEnabled NOTIFY networkChanged)
    Q_PROPERTY(QString ssid        READ ssid        NOTIFY networkChanged)
    Q_PROPERTY(int     wifiStrength READ wifiStrength NOTIFY networkChanged)

    // ── Bluetooth ─────────────────────────────────────────────────────────────
    Q_PROPERTY(bool    bluetoothEnabled READ bluetoothEnabled NOTIFY bluetoothChanged)
    Q_PROPERTY(QString bluetoothDevice  READ bluetoothDevice  NOTIFY bluetoothChanged)
    Q_PROPERTY(QVariantList bluetoothDevices READ bluetoothDevices NOTIFY bluetoothChanged)

    // ── Media (MPRIS) ─────────────────────────────────────────────────────────
    Q_PROPERTY(QString mediaTitle   READ mediaTitle   NOTIFY mediaChanged)
    Q_PROPERTY(QString mediaArtist  READ mediaArtist  NOTIFY mediaChanged)
    Q_PROPERTY(QString mediaPlayer  READ mediaPlayer  NOTIFY mediaChanged)
    Q_PROPERTY(bool    mediaPlaying READ mediaPlaying NOTIFY mediaChanged)
    Q_PROPERTY(bool    mediaAvailable READ mediaAvailable NOTIFY mediaChanged)

    // ── Do Not Disturb ────────────────────────────────────────────────────────
    Q_PROPERTY(bool    doNotDisturb READ doNotDisturb WRITE setDoNotDisturb NOTIFY doNotDisturbChanged)

public:
    explicit QuickPanelBackend(QObject *parent = nullptr);
    ~QuickPanelBackend();

    // Getters
    int     volume()          const { return m_volume; }
    bool    muted()           const { return m_muted; }
    int     brightness()      const { return m_brightness; }
    int     maxBrightness()   const { return m_maxBrightness; }
    bool    wifiEnabled()     const { return m_wifiEnabled; }
    QString ssid()            const { return m_ssid; }
    int     wifiStrength()    const { return m_wifiStrength; }
    bool    bluetoothEnabled() const { return m_bluetoothEnabled; }
    QString bluetoothDevice() const { return m_bluetoothDevice; }
    QVariantList bluetoothDevices() const { return m_bluetoothDevices; }
    QString mediaTitle()      const { return m_mediaTitle; }
    QString mediaArtist()     const { return m_mediaArtist; }
    QString mediaPlayer()     const { return m_mediaPlayer; }
    bool    mediaPlaying()    const { return m_mediaPlaying; }
    bool    mediaAvailable()  const { return m_mediaAvailable; }
    bool    doNotDisturb()    const { return m_doNotDisturb; }
    void    setDoNotDisturb(bool v);

    // Volume
    Q_INVOKABLE void setVolume(int percent);
    Q_INVOKABLE void toggleMute();

    // Brightness
    Q_INVOKABLE void setBrightness(int percent);

    // WiFi
    Q_INVOKABLE void setWifiEnabled(bool enabled);
    Q_INVOKABLE void connectWifi(const QString &ssid, const QString &password);
    Q_INVOKABLE void scanWifi();
    Q_INVOKABLE QVariantList wifiNetworks() const { return m_wifiNetworks; }

    // Bluetooth
    Q_INVOKABLE void setBluetoothEnabled(bool enabled);
    Q_INVOKABLE void connectBluetooth(const QString &address);
    Q_INVOKABLE void disconnectBluetooth(const QString &address);
    Q_INVOKABLE void scanBluetooth();

    // Media
    Q_INVOKABLE void mediaPlayPause();
    Q_INVOKABLE void mediaNext();
    Q_INVOKABLE void mediaPrev();
    Q_INVOKABLE void mediaStop();

    // Refresh all
    Q_INVOKABLE void refresh();

signals:
    void volumeChanged();
    void brightnessChanged();
    void networkChanged();
    void wifiNetworksChanged();
    void bluetoothChanged();
    void mediaChanged();
    void doNotDisturbChanged();

private slots:
    void pollStatus();
    void onMprisPropertiesChanged(const QString &interface,
                                   const QVariantMap &changed,
                                   const QStringList &invalidated);

private:
    void refreshVolume();
    void refreshBrightness();
    void refreshNetwork();
    void refreshBluetooth();
    void refreshMedia();
    void findMprisPlayer();

    QString runCommand(const QString &cmd);
    void    runAsync(const QString &cmd);

    // State
    int     m_volume       = 50;
    bool    m_muted        = false;
    int     m_brightness   = 100;
    int     m_maxBrightness = 100;
    bool    m_wifiEnabled  = false;
    QString m_ssid;
    int     m_wifiStrength = 0;
    QVariantList m_wifiNetworks;
    bool    m_bluetoothEnabled = false;
    QString m_bluetoothDevice;
    QVariantList m_bluetoothDevices;
    QString m_mediaTitle;
    QString m_mediaArtist;
    QString m_mediaPlayer;
    bool    m_mediaPlaying   = false;
    bool    m_mediaAvailable = false;
    bool    m_doNotDisturb   = false;

    // MPRIS
    QString        m_mprisService;
    QDBusInterface *m_mprisPlayer = nullptr;

    QTimer *m_pollTimer = nullptr;
};

#endif // QUICKPANELBACKEND_H
