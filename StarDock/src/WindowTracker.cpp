#include "WindowTracker.h"
#include "Config.h"
#include <QFile>

WindowTracker::WindowTracker(DesktopFileParser *parser, Config *config, QObject *parent) 
    : QObject(parent), m_parser(parser), m_config(config) {
    
    m_ipc = starview_ipc_connect();
    if (!m_ipc) {
        qWarning() << "Failed to connect to StarView IPC";
    }
    
    // Poll for window changes
    m_pollTimer = new QTimer(this);
    connect(m_pollTimer, &QTimer::timeout, this, &WindowTracker::updateRunningApps);
    m_pollTimer->start(m_config ? m_config->updateInterval() : 1000);
    
    // Initial update
    updateRunningApps();
}

WindowTracker::~WindowTracker() {
    if (m_ipc) {
        starview_ipc_disconnect(m_ipc);
    }
}

QVariantList WindowTracker::getRunningApps() const {
    QVariantList list;
    for (const RunningApp &app : m_runningApps) {
        list.append(app.toVariantMap());
    }
    return list;
}

void WindowTracker::updateRunningApps() {
    if (!m_parser) return;
    
    QList<RunningApp> newApps;
    QSet<QString> seenCommands;
    
    // Get list of processes from /proc
    QDir procDir("/proc");
    QStringList procDirs = procDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    
    for (const QString &pidStr : procDirs) {
        bool ok;
        int pid = pidStr.toInt(&ok);
        if (!ok) continue;
        
        // Read command line
        QString cmdlinePath = QString("/proc/%1/cmdline").arg(pid);
        QFile cmdFile(cmdlinePath);
        if (!cmdFile.open(QIODevice::ReadOnly)) continue;
        
        QByteArray cmdData = cmdFile.readAll();
        cmdFile.close();
        
        // Replace null bytes with spaces
        QString command = QString::fromUtf8(cmdData).replace('\0', ' ').trimmed();
        if (command.isEmpty()) continue;
        
        // Extract binary name
        QString binName = command.split(' ').first().split('/').last();
        
        // Skip system processes and common daemons
        QStringList skipList = m_config ? m_config->skipProcesses() : QStringList{
            "systemd", "dbus", "gdm", "kwin", "plasmashell", 
            "bash", "sh", "zsh", "fish", "starview-dock", "starview",
            "pipewire", "pulseaudio", "Xwayland", "at-spi", "gvfs",
            "dbus-daemon", "gnome-shell", "gsd-", "upowerd"
        };
        
        bool skip = false;
        for (const QString &skipPattern : skipList) {
            if (binName.contains(skipPattern, Qt::CaseInsensitive)) {
                skip = true;
                break;
            }
        }
        
        if (skip) continue;
        if (seenCommands.contains(binName)) continue;
        
        // Try to find matching desktop file
        DesktopApp desktopApp = m_parser->findByCommand(binName);
        
        if (!desktopApp.name.isEmpty()) {
            RunningApp app;
            app.name = desktopApp.name;
            app.command = desktopApp.exec;
            app.icon = desktopApp.icon;
            app.desktopFile = desktopApp.desktopFile;
            app.focused = false;
            
            newApps.append(app);
            seenCommands.insert(binName);
        }
    }
    
    // Check if the list has changed
    if (newApps.size() != m_runningApps.size()) {
        m_runningApps = newApps;
        emit runningAppsChanged();
    } else {
        bool changed = false;
        for (int i = 0; i < newApps.size(); ++i) {
            if (newApps[i].name != m_runningApps[i].name) {
                changed = true;
                break;
            }
        }
        if (changed) {
            m_runningApps = newApps;
            emit runningAppsChanged();
        }
    }
}
