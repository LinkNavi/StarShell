#ifndef WINDOWTRACKER_H
#define WINDOWTRACKER_H

#include <QObject>
#include <QTimer>
#include <QVariantList>
#include <QVariantMap>
#include <QDebug>
#include <QSet>
#include <QDir>
#include "DesktopFileParser.h"

// Forward declarations
class Config;

extern "C" {
#include "starview_ipc.h"
}

struct RunningApp {
    QString name;
    QString command;
    QString icon;
    QString desktopFile;
    bool focused;
    
    QVariantMap toVariantMap() const {
        return QVariantMap{
            {"name", name},
            {"command", command},
            {"icon", icon},
            {"desktopFile", desktopFile},
            {"focused", focused}
        };
    }
};

class WindowTracker : public QObject {
    Q_OBJECT
    
public:
    explicit WindowTracker(DesktopFileParser *parser, Config *config, QObject *parent = nullptr);
    ~WindowTracker();
    
    Q_INVOKABLE QVariantList getRunningApps() const;
    
signals:
    void runningAppsChanged();
    
private slots:
    void updateRunningApps();
    
private:
    StarViewIPC *m_ipc = nullptr;
    DesktopFileParser *m_parser;
    Config *m_config;
    QTimer *m_pollTimer;
    QList<RunningApp> m_runningApps;
};

#endif // WINDOWTRACKER_H
