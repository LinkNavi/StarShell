#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "ConfigManager.h"
#include "WallpaperManager.h"
#include "MonitorManager.h"
#include "NetworkManager.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    
    QGuiApplication::setApplicationName("StarView Config");
    QGuiApplication::setOrganizationName("StarView");
    
    // Create managers
    ConfigManager configManager;
    WallpaperManager wallpaperManager;
    MonitorManager monitorManager;
    NetworkManager networkManager;
    
    QQmlApplicationEngine engine;
    
    // Expose to QML
    engine.rootContext()->setContextProperty("configManager", &configManager);
    engine.rootContext()->setContextProperty("wallpaperManager", &wallpaperManager);
    engine.rootContext()->setContextProperty("monitorManager", &monitorManager);
    engine.rootContext()->setContextProperty("networkManager", &networkManager);
    
    // Load main window
    engine.load(QUrl(QStringLiteral("qrc:/StarViewConfig/src/Main.qml")));
    
    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Failed to load Main.qml";
        return -1;
    }
    
    return app.exec();
}
