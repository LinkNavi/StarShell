#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "ConfigManager.h"
#include "WallpaperManager.h"
#include "MonitorManager.h"
#include "NetworkManager.h"
#include "ColorProvider.h"
#include "PanelConfigWriter.h"

// ModuleServer lives in StarBar's source tree; adjust include path in your
// build system so StarConfig can see it, e.g. add:
//   include_directories(../StarBar/src)
#include "module_server.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QGuiApplication::setApplicationName("StarView Settings");
    QGuiApplication::setOrganizationName("StarView");

    ConfigManager      configManager;
    WallpaperManager   wallpaperManager;
    MonitorManager     monitorManager;
    NetworkManager     networkManager;
    ColorProvider      colorProvider;
    PanelConfigWriter  panelConfigWriter;

    // ModuleServer — connects to the same socket the panel listens on,
    // but here we instantiate it purely for directory scanning + createModule().
    // If StarBar is running, its server owns the socket; StarConfig only reads
    // the model via the shared object (they'd be in the same process if embedded,
    // or you'd use a lightweight client here).
    ModuleServer moduleServer;

    QQmlApplicationEngine engine;
    auto *ctx = engine.rootContext();
    ctx->setContextProperty("configManager",     &configManager);
    ctx->setContextProperty("wallpaperManager",  &wallpaperManager);
    ctx->setContextProperty("monitorManager",    &monitorManager);
    ctx->setContextProperty("networkManager",    &networkManager);
    ctx->setContextProperty("colors",            &colorProvider);
    ctx->setContextProperty("panelConfigWriter", &panelConfigWriter);
    ctx->setContextProperty("moduleServer",      &moduleServer);

    QObject::connect(&colorProvider, &ColorProvider::colorsChanged, [&]() {
        if (!colorProvider.loaded()) return;
        QVariantMap c;
        c["primary"]          = colorProvider.primary();
        c["secondary"]        = colorProvider.secondary();
        c["tertiary"]         = colorProvider.tertiary();
        c["error"]            = colorProvider.error();
        c["background"]       = colorProvider.background();
        c["on_background"]    = colorProvider.onBackground();
        c["surface"]          = colorProvider.surface();
        c["on_surface"]       = colorProvider.onSurface();
        c["surface_variant"]  = colorProvider.surfaceVariant();
        c["on_surface_variant"] = colorProvider.onSurfaceVariant();
        c["outline"]          = colorProvider.outline();
        configManager.applyMatugenColors(c);
    });

    engine.load(QUrl(QStringLiteral("qrc:/StarViewConfig/src/Main.qml")));
    if (engine.rootObjects().isEmpty()) return -1;

    return app.exec();
}
