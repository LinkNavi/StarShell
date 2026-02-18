#ifndef MONITORMANAGER_H
#define MONITORMANAGER_H

#include <QObject>
#include <QVariantList>

class MonitorManager : public QObject {
    Q_OBJECT
    
public:
    explicit MonitorManager(QObject *parent = nullptr);
    
    Q_INVOKABLE QVariantList getMonitors();
    Q_INVOKABLE void setMonitorMode(const QString &output, int width, int height, int refresh);
    Q_INVOKABLE void setMonitorPosition(const QString &output, int x, int y);
    Q_INVOKABLE void setMonitorScale(const QString &output, double scale);
    Q_INVOKABLE void setMonitorEnabled(const QString &output, bool enabled);
    
signals:
    void monitorsChanged();
    void error(QString message);
};

#endif // MONITORMANAGER_H
