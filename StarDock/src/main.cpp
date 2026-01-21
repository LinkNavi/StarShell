#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QProcess>
#include <LayerShellQt/Window>
#include <QRegularExpression>
#include <QQmlComponent>

#include "../include/DesktopFileParser.h"
#include "../include/WindowTracker.h"
#include "../include/Config.h"

extern "C" {
#include "../include/starview_ipc.h"
}

class DockController : public QObject {
    Q_OBJECT
    
public:
    DockController(Config *config, QObject *parent = nullptr) : QObject(parent) {
        // Connect to StarView compositor
        m_ipc = starview_ipc_connect();
        if (!m_ipc) {
            qWarning("Failed to connect to StarView IPC");
        }
        
        // Create desktop file parser
        m_parser = new DesktopFileParser(this);
        
        // Create window tracker
        m_tracker = new WindowTracker(m_parser, config, this);
    }
    
    ~DockController() {
        if (m_ipc) {
            starview_ipc_disconnect(m_ipc);
        }
    }
    
    // Launch an application
    Q_INVOKABLE void launchApp(const QString &command) {
        QString cleanCmd = command;
        
        // Remove field codes if present
        cleanCmd.remove(QRegularExpression(" %[fFuUdDnNickvm]"));
        
        if (m_ipc) {
            if (starview_ipc_spawn(m_ipc, cleanCmd.toUtf8().constData())) {
                qDebug() << "Launched via IPC:" << cleanCmd;
                return;
            }
        }
        
        // Fallback to QProcess
        QProcess::startDetached(cleanCmd, QStringList());
        qDebug() << "Launched via QProcess:" << cleanCmd;
    }
    
    // Get all available applications
    Q_INVOKABLE QVariantList getAllApps() const {
        return m_parser->getAllApps();
    }
    
    // Get running applications
    Q_INVOKABLE QVariantList getRunningApps() const {
        return m_tracker->getRunningApps();
    }
    
    // Get icon path for an icon name
    Q_INVOKABLE QString getIconPath(const QString &iconName) {
        return m_parser->getIconPath(iconName);
    }
    
    // Search apps by name
    Q_INVOKABLE QVariantList searchApps(const QString &query) const {
        QVariantList allApps = m_parser->getAllApps();
        if (query.isEmpty()) {
            return allApps;
        }
        
        QVariantList filtered;
        QString lowerQuery = query.toLower();
        
        for (const QVariant &appVar : allApps) {
            QVariantMap app = appVar.toMap();
            QString name = app["name"].toString().toLower();
            if (name.contains(lowerQuery)) {
                filtered.append(appVar);
            }
        }
        
        return filtered;
    }
    
    // Show app launcher
    Q_INVOKABLE void showLauncher() {
        emit launcherRequested();
    }
    
    WindowTracker* tracker() { return m_tracker; }
    
signals:
    void launcherRequested();
    
private:
    StarViewIPC *m_ipc = nullptr;
    DesktopFileParser *m_parser = nullptr;
    WindowTracker *m_tracker = nullptr;
};

int main(int argc, char *argv[]) {
    qputenv("QT_QPA_PLATFORM", "wayland");
    QGuiApplication app(argc, argv);
    
    // Load configuration
    Config config;
    
    // Create dock controller
    DockController controller(&config);
    
    QQmlApplicationEngine engine;
    
    // Expose to QML
    engine.rootContext()->setContextProperty("config", &config);
    engine.rootContext()->setContextProperty("dockController", &controller);
    engine.rootContext()->setContextProperty("windowTracker", controller.tracker());
    
    // Load main dock window
    engine.load(QUrl(QStringLiteral("qrc:/StarViewDock/src/Main.qml")));
    
    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Failed to load Main.qml";
        return -1;
    }
    
    // Get the main dock window
    QQuickWindow *dockWindow = qobject_cast<QQuickWindow*>(engine.rootObjects().first());
    if (dockWindow) {
        // Configure dock window as layer shell
        auto layer = LayerShellQt::Window::get(dockWindow);
        layer->setLayer(LayerShellQt::Window::LayerTop);
        layer->setAnchors(static_cast<LayerShellQt::Window::Anchors>(
            LayerShellQt::Window::AnchorBottom | 
            LayerShellQt::Window::AnchorLeft | 
            LayerShellQt::Window::AnchorRight
        ));
        layer->setExclusiveZone(config.exclusiveZone());
        layer->setKeyboardInteractivity(
            LayerShellQt::Window::KeyboardInteractivityNone
        );
        dockWindow->show();
    }
    
    // Create launcher window component (but don't show yet)
    QQmlComponent launcherComponent(&engine, QUrl(QStringLiteral("qrc:/StarViewDock/src/AppLauncher.qml")));
    
    // Connect launcher request signal
    QObject::connect(&controller, &DockController::launcherRequested, [&]() {
        if (launcherComponent.isReady()) {
            QObject *launcherObj = launcherComponent.create();
            if (launcherObj) {
                QQuickWindow *launcherWindow = qobject_cast<QQuickWindow*>(launcherObj);
                if (launcherWindow) {
                    // Configure as layer shell overlay
                    auto launcherLayer = LayerShellQt::Window::get(launcherWindow);
                    launcherLayer->setLayer(LayerShellQt::Window::LayerOverlay);
                    launcherLayer->setKeyboardInteractivity(
                        LayerShellQt::Window::KeyboardInteractivityExclusive
                    );
                    
                    launcherWindow->show();
                    launcherWindow->requestActivate();
                    
                    // Auto-delete when closed
                    QObject::connect(launcherWindow, &QQuickWindow::visibleChanged, [launcherWindow]() {
                        if (!launcherWindow->isVisible()) {
                            launcherWindow->deleteLater();
                        }
                    });
                }
            }
        } else {
            qWarning() << "Launcher component not ready:" << launcherComponent.errorString();
        }
    });
    
    return app.exec();
}

#include "main.moc"
