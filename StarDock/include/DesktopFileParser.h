#ifndef DESKTOPFILEPARSER_H
#define DESKTOPFILEPARSER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QVariantMap>
#include <QVariantList>
#include <QDebug>

struct DesktopApp {
    QString name;
    QString exec;
    QString icon;
    QString desktopFile;
    bool noDisplay;
    
    QVariantMap toVariantMap() const {
        return QVariantMap{
            {"name", name},
            {"exec", exec},
            {"icon", icon},
            {"desktopFile", desktopFile}
        };
    }
};

class DesktopFileParser : public QObject {
    Q_OBJECT
    
public:
    explicit DesktopFileParser(QObject *parent = nullptr);
    
    void loadDesktopFiles();
    DesktopApp parseDesktopFile(const QString &filePath);
    DesktopApp findByCommand(const QString &command);
    DesktopApp findByName(const QString &name);
    
    Q_INVOKABLE QVariantList getAllApps() const;
    Q_INVOKABLE QString getIconPath(const QString &iconName);
    
    const QList<DesktopApp>& apps() const { return m_apps; }
    
private:
    QList<DesktopApp> m_apps;
};

#endif // DESKTOPFILEPARSER_H
