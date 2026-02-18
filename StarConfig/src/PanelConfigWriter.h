#ifndef PANELCONFIGWRITER_H
#define PANELCONFIGWRITER_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QDebug>

class PanelConfigWriter : public QObject {
    Q_OBJECT
public:
    explicit PanelConfigWriter(QObject *parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE void save(const QString &json) {
        QString dir = QDir::homePath() + "/.config/starview";
        QDir().mkpath(dir);
        QString path = dir + "/panel.json";

        QFile f(path);
        if (f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
            f.write(json.toUtf8());
            f.close();
            qDebug() << "Panel config saved to" << path;
            emit saved();
        } else {
            qWarning() << "Failed to write panel config:" << path;
            emit error("Failed to write " + path);
        }
    }

    Q_INVOKABLE QString load() {
        QString path = QDir::homePath() + "/.config/starview/panel.json";
        QFile f(path);
        if (f.open(QIODevice::ReadOnly)) {
            QByteArray data = f.readAll();
            f.close();
            return QString::fromUtf8(data);
        }
        return "{}";
    }

    Q_INVOKABLE bool exists() {
        return QFile::exists(QDir::homePath() + "/.config/starview/panel.json");
    }

signals:
    void saved();
    void error(QString msg);
};

#endif // PANELCONFIGWRITER_H
