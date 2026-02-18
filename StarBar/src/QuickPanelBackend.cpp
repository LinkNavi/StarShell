#include "QuickPanelBackend.h"
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QRegularExpression>

// ─────────────────────────────────────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────────────────────────────────────

QString QuickPanelBackend::runCommand(const QString &cmd)
{
    QProcess p;
    p.start("/bin/sh", {"-c", cmd});
    p.waitForFinished(3000);
    return QString::fromUtf8(p.readAllStandardOutput()).trimmed();
}

void QuickPanelBackend::runAsync(const QString &cmd)
{
    QProcess *p = new QProcess(this);
    connect(p, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            p, &QProcess::deleteLater);
    p->start("/bin/sh", {"-c", cmd});
}

// ─────────────────────────────────────────────────────────────────────────────
// Constructor / Destructor
// ─────────────────────────────────────────────────────────────────────────────

QuickPanelBackend::QuickPanelBackend(QObject *parent)
    : QObject(parent)
{
    m_pollTimer = new QTimer(this);
    m_pollTimer->setInterval(3000);
    connect(m_pollTimer, &QTimer::timeout, this, &QuickPanelBackend::pollStatus);
    m_pollTimer->start();

    // Initial load
    QTimer::singleShot(0, this, &QuickPanelBackend::pollStatus);
}

QuickPanelBackend::~QuickPanelBackend()
{
    delete m_mprisPlayer;
}

// ─────────────────────────────────────────────────────────────────────────────
// Poll
// ─────────────────────────────────────────────────────────────────────────────

void QuickPanelBackend::pollStatus()
{
    refreshVolume();
    refreshBrightness();
    refreshNetwork();
    refreshBluetooth();
    refreshMedia();
}

void QuickPanelBackend::refresh()
{
    pollStatus();
}

// ─────────────────────────────────────────────────────────────────────────────
// Volume  (PipeWire / PulseAudio via pactl/wpctl)
// ─────────────────────────────────────────────────────────────────────────────

void QuickPanelBackend::refreshVolume()
{
    // Try wpctl first (PipeWire), fall back to pactl
    QString out = runCommand("wpctl get-volume @DEFAULT_AUDIO_SINK@ 2>/dev/null");
    if (!out.isEmpty()) {
        // "Volume: 0.50" or "Volume: 0.50 [MUTED]"
        bool muted = out.contains("[MUTED]");
        QRegularExpression re(R"(Volume:\s*([\d.]+))");
        auto m = re.match(out);
        if (m.hasMatch()) {
            int vol = qRound(m.captured(1).toDouble() * 100.0);
            if (vol != m_volume || muted != m_muted) {
                m_volume = vol;
                m_muted  = muted;
                emit volumeChanged();
            }
            return;
        }
    }

    // pactl fallback
    out = runCommand("pactl get-sink-volume @DEFAULT_SINK@ 2>/dev/null");
    QRegularExpression re(R"((\d+)%)");
    auto match = re.match(out);
    if (match.hasMatch()) {
        int vol = match.captured(1).toInt();
        bool muted = runCommand("pactl get-sink-mute @DEFAULT_SINK@").contains("yes");
        if (vol != m_volume || muted != m_muted) {
            m_volume = vol;
            m_muted  = muted;
            emit volumeChanged();
        }
    }
}

void QuickPanelBackend::setVolume(int percent)
{
    percent = qBound(0, percent, 150);
    // Try wpctl first
    QString test = runCommand("which wpctl 2>/dev/null");
    if (!test.isEmpty()) {
        runAsync(QString("wpctl set-volume @DEFAULT_AUDIO_SINK@ %1%").arg(percent));
    } else {
        runAsync(QString("pactl set-sink-volume @DEFAULT_SINK@ %1%").arg(percent));
    }
    m_volume = percent;
    emit volumeChanged();
}

void QuickPanelBackend::toggleMute()
{
    QString test = runCommand("which wpctl 2>/dev/null");
    if (!test.isEmpty()) {
        runAsync("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle");
    } else {
        runAsync("pactl set-sink-mute @DEFAULT_SINK@ toggle");
    }
    m_muted = !m_muted;
    emit volumeChanged();
}

// ─────────────────────────────────────────────────────────────────────────────
// Brightness  (brightnessctl)
// ─────────────────────────────────────────────────────────────────────────────

void QuickPanelBackend::refreshBrightness()
{
    QString cur = runCommand("brightnessctl get 2>/dev/null");
    QString max = runCommand("brightnessctl max 2>/dev/null");
    if (cur.isEmpty() || max.isEmpty()) return;

    int c = cur.toInt();
    int m = max.toInt();
    if (m <= 0) return;

    int pct = qRound(c * 100.0 / m);
    if (pct != m_brightness || m != m_maxBrightness) {
        m_brightness    = pct;
        m_maxBrightness = m;
        emit brightnessChanged();
    }
}

void QuickPanelBackend::setBrightness(int percent)
{
    percent = qBound(1, percent, 100);
    runAsync(QString("brightnessctl set %1%").arg(percent));
    m_brightness = percent;
    emit brightnessChanged();
}

// ─────────────────────────────────────────────────────────────────────────────
// Network  (nmcli)
// ─────────────────────────────────────────────────────────────────────────────

void QuickPanelBackend::refreshNetwork()
{
    QString state = runCommand("nmcli radio wifi 2>/dev/null");
    bool enabled = state.trimmed() == "enabled";

    QString ssid, strength;
    int sig = 0;

    if (enabled) {
        // Active connection
        QString conn = runCommand(
            "nmcli -t -f ACTIVE,SSID,SIGNAL dev wifi 2>/dev/null | grep '^yes'");
        if (!conn.isEmpty()) {
            QStringList parts = conn.split(':');
            if (parts.size() >= 3) {
                ssid = parts[1];
                sig  = parts[2].toInt();
            }
        }
    }

    if (enabled != m_wifiEnabled || ssid != m_ssid || sig != m_wifiStrength) {
        m_wifiEnabled  = enabled;
        m_ssid         = ssid;
        m_wifiStrength = sig;
        emit networkChanged();
    }
}

void QuickPanelBackend::setWifiEnabled(bool enabled)
{
    runAsync(QString("nmcli radio wifi %1").arg(enabled ? "on" : "off"));
    m_wifiEnabled = enabled;
    emit networkChanged();
}

void QuickPanelBackend::connectWifi(const QString &ssid, const QString &password)
{
    if (password.isEmpty())
        runAsync(QString("nmcli dev wifi connect \"%1\"").arg(ssid));
    else
        runAsync(QString("nmcli dev wifi connect \"%1\" password \"%2\"").arg(ssid, password));
    QTimer::singleShot(3000, this, &QuickPanelBackend::refreshNetwork);
}

void QuickPanelBackend::scanWifi()
{
    runAsync("nmcli dev wifi rescan");

    QTimer::singleShot(2000, this, [this]() {
        QString out = runCommand(
            "nmcli -t -f SSID,SIGNAL,SECURITY dev wifi list 2>/dev/null");
        m_wifiNetworks.clear();
        for (const QString &line : out.split('\n')) {
            if (line.trimmed().isEmpty()) continue;
            QStringList p = line.split(':');
            if (p.size() < 3 || p[0].isEmpty()) continue;
            QVariantMap net;
            net["ssid"]     = p[0];
            net["signal"]   = p[1].toInt();
            net["security"] = p[2];
            m_wifiNetworks.append(net);
        }
        emit wifiNetworksChanged();
    });
}

// ─────────────────────────────────────────────────────────────────────────────
// Bluetooth  (bluetoothctl)
// ─────────────────────────────────────────────────────────────────────────────

void QuickPanelBackend::refreshBluetooth()
{
    QString powered = runCommand(
        "bluetoothctl show 2>/dev/null | grep 'Powered:' | awk '{print $2}'");
    bool enabled = powered.trimmed() == "yes";

    // Connected devices
    QString devOut = runCommand(
        "bluetoothctl devices Connected 2>/dev/null");
    QVariantList devices;
    QString firstDev;
    for (const QString &line : devOut.split('\n')) {
        if (line.trimmed().isEmpty()) continue;
        // "Device AA:BB:CC:DD:EE:FF Name"
        QStringList parts = line.trimmed().split(' ');
        if (parts.size() >= 3) {
            QString addr = parts[1];
            QString name = parts.mid(2).join(' ');
            QVariantMap d;
            d["address"] = addr;
            d["name"]    = name;
            d["connected"] = true;
            devices.append(d);
            if (firstDev.isEmpty()) firstDev = name;
        }
    }

    if (enabled != m_bluetoothEnabled || firstDev != m_bluetoothDevice
        || devices.size() != m_bluetoothDevices.size()) {
        m_bluetoothEnabled = enabled;
        m_bluetoothDevice  = firstDev;
        m_bluetoothDevices = devices;
        emit bluetoothChanged();
    }
}

void QuickPanelBackend::setBluetoothEnabled(bool enabled)
{
    runAsync(QString("bluetoothctl power %1").arg(enabled ? "on" : "off"));
    m_bluetoothEnabled = enabled;
    emit bluetoothChanged();
}

void QuickPanelBackend::connectBluetooth(const QString &address)
{
    runAsync(QString("bluetoothctl connect %1").arg(address));
    QTimer::singleShot(4000, this, &QuickPanelBackend::refreshBluetooth);
}

void QuickPanelBackend::disconnectBluetooth(const QString &address)
{
    runAsync(QString("bluetoothctl disconnect %1").arg(address));
    QTimer::singleShot(2000, this, &QuickPanelBackend::refreshBluetooth);
}

void QuickPanelBackend::scanBluetooth()
{
    runAsync("bluetoothctl scan on &");
    QTimer::singleShot(5000, this, [this]() {
        runAsync("bluetoothctl scan off");
        // Get paired devices
        QString out = runCommand("bluetoothctl devices 2>/dev/null");
        m_bluetoothDevices.clear();
        for (const QString &line : out.split('\n')) {
            if (line.trimmed().isEmpty()) continue;
            QStringList parts = line.trimmed().split(' ');
            if (parts.size() >= 3) {
                QString addr = parts[1];
                QString name = parts.mid(2).join(' ');
                QString connOut = runCommand(
                    QString("bluetoothctl info %1 2>/dev/null | grep 'Connected:' | awk '{print $2}'")
                    .arg(addr));
                QVariantMap d;
                d["address"]   = addr;
                d["name"]      = name;
                d["connected"] = connOut.trimmed() == "yes";
                m_bluetoothDevices.append(d);
            }
        }
        emit bluetoothChanged();
    });
}

// ─────────────────────────────────────────────────────────────────────────────
// Media  (MPRIS via D-Bus)
// ─────────────────────────────────────────────────────────────────────────────

void QuickPanelBackend::findMprisPlayer()
{
    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusInterface names("org.freedesktop.DBus", "/org/freedesktop/DBus",
                         "org.freedesktop.DBus", bus);
    QDBusReply<QStringList> reply = names.call("ListNames");
    if (!reply.isValid()) return;

    QString best;
    for (const QString &name : reply.value()) {
        if (name.startsWith("org.mpris.MediaPlayer2.")) {
            best = name;
            break;
        }
    }

    if (best == m_mprisService) return;

    delete m_mprisPlayer;
    m_mprisPlayer = nullptr;
    m_mprisService = best;

    if (best.isEmpty()) return;

    m_mprisPlayer = new QDBusInterface(
        best,
        "/org/mpris/MediaPlayer2",
        "org.mpris.MediaPlayer2.Player",
        bus, this);

    // Watch property changes
    bus.connect(best,
                "/org/mpris/MediaPlayer2",
                "org.freedesktop.DBus.Properties",
                "PropertiesChanged",
                this,
                SLOT(onMprisPropertiesChanged(QString, QVariantMap, QStringList)));
}

void QuickPanelBackend::refreshMedia()
{
    findMprisPlayer();

    if (!m_mprisPlayer || !m_mprisPlayer->isValid()) {
        if (m_mediaAvailable) {
            m_mediaAvailable = false;
            m_mediaTitle     = "";
            m_mediaArtist    = "";
            m_mediaPlaying   = false;
            emit mediaChanged();
        }
        return;
    }

    // Playback status
    QDBusInterface props(m_mprisService,
                         "/org/mpris/MediaPlayer2",
                         "org.freedesktop.DBus.Properties",
                         QDBusConnection::sessionBus());

    QDBusReply<QVariant> statusReply = props.call(
        "Get", "org.mpris.MediaPlayer2.Player", "PlaybackStatus");
    bool playing = false;
    if (statusReply.isValid())
        playing = statusReply.value().toString() == "Playing";

    // Metadata
    QDBusReply<QVariant> metaReply = props.call(
        "Get", "org.mpris.MediaPlayer2.Player", "Metadata");

    QString title, artist;
    if (metaReply.isValid()) {
        QVariantMap meta = qdbus_cast<QVariantMap>(metaReply.value().value<QDBusArgument>());
        title  = meta.value("xesam:title").toString();
        QStringList artists = meta.value("xesam:artist").toStringList();
        artist = artists.join(", ");
    }

    // Player name
    QDBusInterface identity(m_mprisService,
                            "/org/mpris/MediaPlayer2",
                            "org.mpris.MediaPlayer2",
                            QDBusConnection::sessionBus());
    QString playerName = identity.property("Identity").toString();

    bool changed = (title != m_mediaTitle || artist != m_mediaArtist
                    || playing != m_mediaPlaying || !m_mediaAvailable
                    || playerName != m_mediaPlayer);
    if (changed) {
        m_mediaTitle     = title;
        m_mediaArtist    = artist;
        m_mediaPlaying   = playing;
        m_mediaAvailable = true;
        m_mediaPlayer    = playerName;
        emit mediaChanged();
    }
}

void QuickPanelBackend::onMprisPropertiesChanged(const QString &,
                                                  const QVariantMap &,
                                                  const QStringList &)
{
    refreshMedia();
}

void QuickPanelBackend::mediaPlayPause()
{
    if (m_mprisPlayer && m_mprisPlayer->isValid())
        m_mprisPlayer->call("PlayPause");
}

void QuickPanelBackend::mediaNext()
{
    if (m_mprisPlayer && m_mprisPlayer->isValid())
        m_mprisPlayer->call("Next");
}

void QuickPanelBackend::mediaPrev()
{
    if (m_mprisPlayer && m_mprisPlayer->isValid())
        m_mprisPlayer->call("Previous");
}

void QuickPanelBackend::mediaStop()
{
    if (m_mprisPlayer && m_mprisPlayer->isValid())
        m_mprisPlayer->call("Stop");
}

// ─────────────────────────────────────────────────────────────────────────────
// Do Not Disturb
// ─────────────────────────────────────────────────────────────────────────────

void QuickPanelBackend::setDoNotDisturb(bool v)
{
    if (v == m_doNotDisturb) return;
    m_doNotDisturb = v;
    // Toggle dunst if available
    if (v)
        runAsync("dunstctl set-paused true 2>/dev/null || true");
    else
        runAsync("dunstctl set-paused false 2>/dev/null || true");
    emit doNotDisturbChanged();
}
