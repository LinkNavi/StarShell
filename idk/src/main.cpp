// main.cpp - StarView Panel - Configure BEFORE showing
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QDebug>
#include <LayerShellQt/Window>

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORM", "wayland");
    qputenv("QT_WAYLAND_DISABLE_WINDOWDECORATION", "1");
    
    QGuiApplication app(argc, argv);
    
    qDebug() << "=== StarView Panel with LayerShellQt ===";
    
    // Create QML engine
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/StarViewPanel/src/Main.qml")));
    
    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Failed to load QML";
        return -1;
    }
    
    QQuickWindow *window = qobject_cast<QQuickWindow*>(engine.rootObjects().first());
    if (!window) {
        qCritical() << "Root object is not a window";
        return -1;
    }
    
    qDebug() << "QML loaded, configuring layer shell...";
    
    // Set size BEFORE creating window
    window->setWidth(1920);
    window->setHeight(40);
    
    // Configure LayerShellQt BEFORE creating the window
    auto layerShell = LayerShellQt::Window::get(window);
    if (!layerShell) {
        qCritical() << "Failed to get LayerShellQt::Window";
        return -1;
    }
    
    // Configure as top panel
    layerShell->setLayer(LayerShellQt::Window::LayerTop);
    layerShell->setAnchors(static_cast<LayerShellQt::Window::Anchors>(
        LayerShellQt::Window::AnchorTop | 
        LayerShellQt::Window::AnchorLeft | 
        LayerShellQt::Window::AnchorRight
    ));
    layerShell->setExclusiveZone(40);
    layerShell->setMargins({0, 0, 0, 0});
    layerShell->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityNone);
    
    qDebug() << "Layer shell configured:";
    qDebug() << "  Layer: TOP";
    qDebug() << "  Anchors: TOP | LEFT | RIGHT";
    qDebug() << "  Exclusive zone: 40px";
    qDebug() << "  Size: 1920x40";
    
    // NOW create and show the window
    window->show();
    
    qDebug() << "Panel shown successfully!";
    
    return app.exec();
}
