#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QVariantList>
#include <LayerShellQt/Window>

int main(int argc, char *argv[]) {
    qputenv("QT_QPA_PLATFORM", "wayland");
    QGuiApplication app(argc, argv);
    
    // Hardcode apps for now
    QVariantList apps;
    apps << QVariantMap{{"name", "Firefox"}, {"icon", "🦊"}, {"command", "firefox"}};
    apps << QVariantMap{{"name", "Terminal"}, {"icon", "💻"}, {"command", "kitty"}};
    apps << QVariantMap{{"name", "Files"}, {"icon", "📁"}, {"command", "thunar"}};
    apps << QVariantMap{{"name", "Code"}, {"icon", "⌨️"}, {"command", "code"}};
    apps << QVariantMap{{"name", "Music"}, {"icon", "🎵"}, {"command", "spotify"}};
    
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("dockApps", apps);
    
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [](QObject *obj, const QUrl &) {
            auto window = qobject_cast<QQuickWindow*>(obj);
            if (!window) return;
            
            auto layer = LayerShellQt::Window::get(window);
            layer->setLayer(LayerShellQt::Window::LayerTop);
            layer->setAnchors(static_cast<LayerShellQt::Window::Anchors>(
                LayerShellQt::Window::AnchorBottom | 
                LayerShellQt::Window::AnchorLeft | 
                LayerShellQt::Window::AnchorRight
            ));
            layer->setExclusiveZone(70);
            layer->setKeyboardInteractivity(
                LayerShellQt::Window::KeyboardInteractivityNone
            );
            window->show();
        });
    
    engine.load(QUrl(QStringLiteral("qrc:/StarViewDock/src/Main.qml")));
    return app.exec();
}
