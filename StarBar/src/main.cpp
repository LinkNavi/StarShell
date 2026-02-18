#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQmlContext>
#include <LayerShellQt/Window>
#include "ipc_handler.h"
#include "panel_config.h"
#include "module_server.h"
#include "QuickPanelBackend.h"

static void configureLayerShell(QQuickWindow *window, PanelConfig *config)
{
    if (!window) return;

    auto layer = LayerShellQt::Window::get(window);
    if (!layer) {
        qCritical() << "Failed to get LayerShellQt interface!";
        return;
    }

    layer->setLayer(LayerShellQt::Window::LayerTop);
    layer->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityNone);

    int h = config->height();
    int m = config->margins();

    if (config->position() == 1) {
        layer->setAnchors(static_cast<LayerShellQt::Window::Anchors>(
            LayerShellQt::Window::AnchorBottom |
            LayerShellQt::Window::AnchorLeft   |
            LayerShellQt::Window::AnchorRight
        ));
    } else {
        layer->setAnchors(static_cast<LayerShellQt::Window::Anchors>(
            LayerShellQt::Window::AnchorTop  |
            LayerShellQt::Window::AnchorLeft |
            LayerShellQt::Window::AnchorRight
        ));
    }

    layer->setExclusiveZone(h + m * 2);
    layer->setMargins(QMargins(m, m, m, m));

    window->setHeight(h + m * 2);
    window->show();

    qDebug() << "Panel configured: h=" << h << "pos=" << config->position()
             << "margins=" << m << "floating=" << config->floating();
}

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORM", "wayland");

    QGuiApplication app(argc, argv);
    QGuiApplication::setApplicationName("StarView Panel");

    IpcHandler        ipcHandler;
    PanelConfig       panelConfig;
    ModuleServer      moduleServer;
    QuickPanelBackend quickPanelBackend;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("ipcHandler",    &ipcHandler);
    engine.rootContext()->setContextProperty("panelConfig",   &panelConfig);
    engine.rootContext()->setContextProperty("moduleServer",  &moduleServer);
    engine.rootContext()->setContextProperty("quickPanel",    &quickPanelBackend);

    QQuickWindow *panelWindow = nullptr;

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [&panelWindow, &panelConfig](QObject *obj, const QUrl &) {
            if (!obj) return;
            auto *w = qobject_cast<QQuickWindow*>(obj);
            if (!w) return;
            panelWindow = w;
            configureLayerShell(panelWindow, &panelConfig);
        }, Qt::QueuedConnection);

    QObject::connect(&panelConfig, &PanelConfig::configChanged,
        &app, [&panelWindow, &panelConfig]() {
            if (panelWindow) {
                qDebug() << "Config changed, reconfiguring panel...";
                configureLayerShell(panelWindow, &panelConfig);
            }
        });

    engine.load(QUrl(QStringLiteral("qrc:/StarViewPanel/src/Main.qml")));

    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Failed to load QML";
        return -1;
    }

    return app.exec();
}
