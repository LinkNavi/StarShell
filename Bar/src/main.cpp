// main.cpp - Configure BEFORE window is created
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQmlContext>
#include <LayerShellQt/Window>

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORM", "wayland");
    
    QGuiApplication app(argc, argv);
    
    QQmlApplicationEngine engine;
    
    // Set up a callback BEFORE loading QML
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [](QObject *obj, const QUrl &) {
            if (!obj) return;
            
            auto window = qobject_cast<QQuickWindow*>(obj);
            if (!window) return;
            
            qDebug() << "Window created, configuring LayerShell...";
            
            // Get LayerShell interface
            auto layer = LayerShellQt::Window::get(window);
            if (!layer) {
                qCritical() << "Failed to get LayerShellQt interface!";
                return;
            }
            
            // Configure layer shell
            layer->setLayer(LayerShellQt::Window::LayerTop);
            layer->setAnchors(static_cast<LayerShellQt::Window::Anchors>(
                LayerShellQt::Window::AnchorTop | 
                LayerShellQt::Window::AnchorLeft | 
                LayerShellQt::Window::AnchorRight
            ));
            layer->setExclusiveZone(40);
            layer->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityNone);
            
            qDebug() << "LayerShell configured: TOP layer, anchors=" 
                     << (int)layer->anchors() << ", exclusive=" << layer->exclusionZone();
            
            // NOW show the window
            window->show();
            qDebug() << "Window shown!";
        }, Qt::QueuedConnection);
    
    // Load QML - window not shown yet
    engine.load(QUrl(QStringLiteral("qrc:/StarViewPanel/src/Main.qml")));
    
    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Failed to load QML";
        return -1;
    }
    
    return app.exec();
}
