#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQmlContext>
#include <LayerShellQt/Window>
#include "ipc_handler.h"

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORM", "wayland");
    
    QGuiApplication app(argc, argv);
    
    // Create IPC handler
    IpcHandler ipcHandler;
    
    QQmlApplicationEngine engine;
    
    // Expose IPC handler to QML
    engine.rootContext()->setContextProperty("ipcHandler", &ipcHandler);
    
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [](QObject *obj, const QUrl &) {
            if (!obj) return;
            
            auto window = qobject_cast<QQuickWindow*>(obj);
            if (!window) return;
            
            qDebug() << "Window created, configuring LayerShell...";
            
            auto layer = LayerShellQt::Window::get(window);
            if (!layer) {
                qCritical() << "Failed to get LayerShellQt interface!";
                return;
            }
            
            layer->setLayer(LayerShellQt::Window::LayerTop);
            layer->setAnchors(static_cast<LayerShellQt::Window::Anchors>(
                LayerShellQt::Window::AnchorTop | 
                LayerShellQt::Window::AnchorLeft | 
                LayerShellQt::Window::AnchorRight
            ));
            layer->setExclusiveZone(40);
            layer->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityNone);
            
            window->show();
            qDebug() << "Panel ready!";
        }, Qt::QueuedConnection);
    
    engine.load(QUrl(QStringLiteral("qrc:/StarViewPanel/src/Main.qml")));
    
    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Failed to load QML";
        return -1;
    }
    
    return app.exec();
}
