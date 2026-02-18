#include "MonitorManager.h"
#include <QProcess>
#include <QDebug>
#include <QRegularExpression>

MonitorManager::MonitorManager(QObject *parent) : QObject(parent) {
}

QVariantList MonitorManager::getMonitors() {
    QVariantList monitors;
    
    QProcess process;
    process.start("wlr-randr", QStringList());
    process.waitForFinished();
    
    if (process.exitCode() != 0) {
        emit error("Failed to get monitor info");
        return monitors;
    }
    
    QString output = process.readAllStandardOutput();
    QStringList lines = output.split('\n');
    
    QVariantMap currentMonitor;
    
    for (const QString &line : lines) {
        if (line.isEmpty()) continue;
        
        // New monitor entry
        if (!line.startsWith(' ') && !line.startsWith('\t')) {
            if (!currentMonitor.isEmpty()) {
                monitors.append(currentMonitor);
            }
            currentMonitor.clear();
            
            QString name = line.split(' ').first();
            currentMonitor["name"] = name;
            currentMonitor["enabled"] = !line.contains("(disabled)");
        }
        // Parse monitor properties
        else if (line.contains("current")) {
            // Parse resolution and refresh rate
            // Example: "  1920x1080 px, 60.000000 Hz (current)"
            QRegularExpression re("(\\d+)x(\\d+).*?(\\d+\\.\\d+)");
            QRegularExpressionMatch match = re.match(line);
            if (match.hasMatch()) {
                currentMonitor["width"] = match.captured(1).toInt();
                currentMonitor["height"] = match.captured(2).toInt();
                currentMonitor["refresh"] = match.captured(3).toDouble();
            }
        }
        else if (line.contains("Position:")) {
            QRegularExpression re("Position: (\\d+),(\\d+)");
            QRegularExpressionMatch match = re.match(line);
            if (match.hasMatch()) {
                currentMonitor["x"] = match.captured(1).toInt();
                currentMonitor["y"] = match.captured(2).toInt();
            }
        }
        else if (line.contains("Scale:")) {
            QRegularExpression re("Scale: ([\\d.]+)");
            QRegularExpressionMatch match = re.match(line);
            if (match.hasMatch()) {
                currentMonitor["scale"] = match.captured(1).toDouble();
            }
        }
    }
    
    if (!currentMonitor.isEmpty()) {
        monitors.append(currentMonitor);
    }
    
    return monitors;
}

void MonitorManager::setMonitorMode(const QString &output, int width, int height, int refresh) {
    QStringList args;
    args << "--output" << output;
    args << "--mode" << QString("%1x%2@%3Hz").arg(width).arg(height).arg(refresh);
    
    QProcess::execute("wlr-randr", args);
    emit monitorsChanged();
}

void MonitorManager::setMonitorPosition(const QString &output, int x, int y) {
    QStringList args;
    args << "--output" << output;
    args << "--pos" << QString("%1,%2").arg(x).arg(y);
    
    QProcess::execute("wlr-randr", args);
    emit monitorsChanged();
}

void MonitorManager::setMonitorScale(const QString &output, double scale) {
    QStringList args;
    args << "--output" << output;
    args << "--scale" << QString::number(scale);
    
    QProcess::execute("wlr-randr", args);
    emit monitorsChanged();
}

void MonitorManager::setMonitorEnabled(const QString &output, bool enabled) {
    QStringList args;
    args << "--output" << output;
    
    if (enabled) {
        args << "--on";
    } else {
        args << "--off";
    }
    
    QProcess::execute("wlr-randr", args);
    emit monitorsChanged();
}
