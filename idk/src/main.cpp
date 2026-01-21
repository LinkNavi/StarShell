// main.cpp - Use Wayland protocol directly
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QWindow>
#include <QDebug>
#include <QtWaylandClient/private/qwaylandwindow_p.h>
#include <QtWaylandClient/private/qwaylandshellsurface_p.h>
#include <wayland-client.h>

// We'll need to load the layer-shell protocol
extern "C" {
    #include "wlr-layer-shell-unstable-v1-client-protocol.h"
}

// Global registry for layer shell
static struct zwlr_layer_shell_v1 *layer_shell = nullptr;

static void registry_global(void *data, struct wl_registry *registry,
                            uint32_t name, const char *interface, uint32_t version) {
    if (strcmp(interface, zwlr_layer_shell_v1_interface.name) == 0) {
        layer_shell = static_cast<struct zwlr_layer_shell_v1*>(
            wl_registry_bind(registry, name, &zwlr_layer_shell_v1_interface, 1));
        qDebug() << "Found layer-shell protocol!";
    }
}

static void registry_global_remove(void *data, struct wl_registry *registry, uint32_t name) {
    // Not needed
}

static const struct wl_registry_listener registry_listener = {
    registry_global,
    registry_global_remove
};

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORM", "wayland");
    qputenv("QT_WAYLAND_DISABLE_WINDOWDECORATION", "1");
    
    QGuiApplication app(argc, argv);
    
    qDebug() << "=== Direct Wayland Layer-Shell Panel ===";
    
    // Get Wayland display
    auto waylandApp = dynamic_cast<QPlatformNativeInterface*>(
        QGuiApplication::platformNativeInterface());
    
    if (!waylandApp) {
        qCritical() << "Not running on Wayland!";
        return -1;
    }
    
    struct wl_display *display = static_cast<struct wl_display*>(
        waylandApp->nativeResourceForIntegration("display"));
    
    if (!display) {
        qCritical() << "Failed to get Wayland display!";
        return -1;
    }
    
    qDebug() << "Got Wayland display";
    
    // Get registry and bind layer-shell
    struct wl_registry *registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, nullptr);
    wl_display_roundtrip(display);
    
    if (!layer_shell) {
        qCritical() << "Layer-shell protocol not available!";
        qCritical() << "Your compositor doesn't support wlr-layer-shell";
        return -1;
    }
    
    qDebug() << "Layer-shell protocol bound successfully";
    
    // Create window
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/StarViewPanel/src/Main.qml")));
    
    if (engine.rootObjects().isEmpty()) {
        qWarning() << "Failed to load QML";
        return -1;
    }
    
    QQuickWindow *window = qobject_cast<QQuickWindow*>(engine.rootObjects().first());
    if (!window) {
        qCritical() << "Root object is not a window";
        return -1;
    }
    
    // Get the Wayland surface
    QWindow *qwindow = qobject_cast<QWindow*>(window);
    if (!qwindow->handle()) {
        qwindow->create();
    }
    
    struct wl_surface *surface = static_cast<struct wl_surface*>(
        waylandApp->nativeResourceForWindow("surface", qwindow));
    
    if (!surface) {
        qCritical() << "Failed to get wl_surface";
        return -1;
    }
    
    qDebug() << "Got wl_surface, creating layer surface...";
    
    // Create layer surface with correct parameters
    struct zwlr_layer_surface_v1 *layer_surface = 
        zwlr_layer_shell_v1_get_layer_surface(
            layer_shell,
            surface,
            nullptr,  // output (nullptr = let compositor choose)
            ZWLR_LAYER_SHELL_V1_LAYER_TOP,
            "starview-panel"
        );
    
    if (!layer_surface) {
        qCritical() << "Failed to create layer surface";
        return -1;
    }
    
    qDebug() << "Layer surface created, configuring...";
    
    // Set anchors: TOP | LEFT | RIGHT = 1 | 4 | 8 = 13
    uint32_t anchors = ZWLR_LAYER_SURFACE_V1_ANCHOR_TOP |
                       ZWLR_LAYER_SURFACE_V1_ANCHOR_LEFT |
                       ZWLR_LAYER_SURFACE_V1_ANCHOR_RIGHT;
    
    zwlr_layer_surface_v1_set_anchor(layer_surface, anchors);
    qDebug() << "Anchors set:" << anchors;
    
    zwlr_layer_surface_v1_set_exclusive_zone(layer_surface, 40);
    qDebug() << "Exclusive zone: 40";
    
    zwlr_layer_surface_v1_set_margin(layer_surface, 0, 0, 0, 0);
    qDebug() << "Margins: 0,0,0,0";
    
    // Commit the surface
    wl_surface_commit(surface);
    wl_display_flush(display);
    
    qDebug() << "Configuration committed, showing window...";
    
    window->show();
    
    return app.exec();
}
