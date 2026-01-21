#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORM", "wayland");
    
    QGuiApplication app(argc, argv);
    app.setApplicationName("StarView Panel");
    
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/StarViewPanel/src/Main.qml")));
    
    if (engine.rootObjects().isEmpty()) {
        qWarning() << "Failed to load QML";
        return -1;
    }
    
    return app.exec();
}
