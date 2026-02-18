/****************************************************************************
** Meta object code from reading C++ file 'QuickPanelBackend.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/QuickPanelBackend.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QuickPanelBackend.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN17QuickPanelBackendE_t {};
} // unnamed namespace

template <> constexpr inline auto QuickPanelBackend::qt_create_metaobjectdata<qt_meta_tag_ZN17QuickPanelBackendE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QuickPanelBackend",
        "volumeChanged",
        "",
        "brightnessChanged",
        "networkChanged",
        "wifiNetworksChanged",
        "bluetoothChanged",
        "mediaChanged",
        "doNotDisturbChanged",
        "pollStatus",
        "onMprisPropertiesChanged",
        "interface",
        "QVariantMap",
        "changed",
        "invalidated",
        "setVolume",
        "percent",
        "toggleMute",
        "setBrightness",
        "setWifiEnabled",
        "enabled",
        "connectWifi",
        "ssid",
        "password",
        "scanWifi",
        "wifiNetworks",
        "QVariantList",
        "setBluetoothEnabled",
        "connectBluetooth",
        "address",
        "disconnectBluetooth",
        "scanBluetooth",
        "mediaPlayPause",
        "mediaNext",
        "mediaPrev",
        "mediaStop",
        "refresh",
        "volume",
        "muted",
        "brightness",
        "maxBrightness",
        "wifiEnabled",
        "wifiStrength",
        "bluetoothEnabled",
        "bluetoothDevice",
        "bluetoothDevices",
        "mediaTitle",
        "mediaArtist",
        "mediaPlayer",
        "mediaPlaying",
        "mediaAvailable",
        "doNotDisturb"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'volumeChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'brightnessChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'networkChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'wifiNetworksChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'bluetoothChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'mediaChanged'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'doNotDisturbChanged'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'pollStatus'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onMprisPropertiesChanged'
        QtMocHelpers::SlotData<void(const QString &, const QVariantMap &, const QStringList &)>(10, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 11 }, { 0x80000000 | 12, 13 }, { QMetaType::QStringList, 14 },
        }}),
        // Method 'setVolume'
        QtMocHelpers::MethodData<void(int)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 16 },
        }}),
        // Method 'toggleMute'
        QtMocHelpers::MethodData<void()>(17, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'setBrightness'
        QtMocHelpers::MethodData<void(int)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 16 },
        }}),
        // Method 'setWifiEnabled'
        QtMocHelpers::MethodData<void(bool)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 20 },
        }}),
        // Method 'connectWifi'
        QtMocHelpers::MethodData<void(const QString &, const QString &)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 22 }, { QMetaType::QString, 23 },
        }}),
        // Method 'scanWifi'
        QtMocHelpers::MethodData<void()>(24, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'wifiNetworks'
        QtMocHelpers::MethodData<QVariantList() const>(25, 2, QMC::AccessPublic, 0x80000000 | 26),
        // Method 'setBluetoothEnabled'
        QtMocHelpers::MethodData<void(bool)>(27, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 20 },
        }}),
        // Method 'connectBluetooth'
        QtMocHelpers::MethodData<void(const QString &)>(28, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 29 },
        }}),
        // Method 'disconnectBluetooth'
        QtMocHelpers::MethodData<void(const QString &)>(30, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 29 },
        }}),
        // Method 'scanBluetooth'
        QtMocHelpers::MethodData<void()>(31, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'mediaPlayPause'
        QtMocHelpers::MethodData<void()>(32, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'mediaNext'
        QtMocHelpers::MethodData<void()>(33, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'mediaPrev'
        QtMocHelpers::MethodData<void()>(34, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'mediaStop'
        QtMocHelpers::MethodData<void()>(35, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'refresh'
        QtMocHelpers::MethodData<void()>(36, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'volume'
        QtMocHelpers::PropertyData<int>(37, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'muted'
        QtMocHelpers::PropertyData<bool>(38, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'brightness'
        QtMocHelpers::PropertyData<int>(39, QMetaType::Int, QMC::DefaultPropertyFlags, 1),
        // property 'maxBrightness'
        QtMocHelpers::PropertyData<int>(40, QMetaType::Int, QMC::DefaultPropertyFlags, 1),
        // property 'wifiEnabled'
        QtMocHelpers::PropertyData<bool>(41, QMetaType::Bool, QMC::DefaultPropertyFlags, 2),
        // property 'ssid'
        QtMocHelpers::PropertyData<QString>(22, QMetaType::QString, QMC::DefaultPropertyFlags, 2),
        // property 'wifiStrength'
        QtMocHelpers::PropertyData<int>(42, QMetaType::Int, QMC::DefaultPropertyFlags, 2),
        // property 'bluetoothEnabled'
        QtMocHelpers::PropertyData<bool>(43, QMetaType::Bool, QMC::DefaultPropertyFlags, 4),
        // property 'bluetoothDevice'
        QtMocHelpers::PropertyData<QString>(44, QMetaType::QString, QMC::DefaultPropertyFlags, 4),
        // property 'bluetoothDevices'
        QtMocHelpers::PropertyData<QVariantList>(45, 0x80000000 | 26, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 4),
        // property 'mediaTitle'
        QtMocHelpers::PropertyData<QString>(46, QMetaType::QString, QMC::DefaultPropertyFlags, 5),
        // property 'mediaArtist'
        QtMocHelpers::PropertyData<QString>(47, QMetaType::QString, QMC::DefaultPropertyFlags, 5),
        // property 'mediaPlayer'
        QtMocHelpers::PropertyData<QString>(48, QMetaType::QString, QMC::DefaultPropertyFlags, 5),
        // property 'mediaPlaying'
        QtMocHelpers::PropertyData<bool>(49, QMetaType::Bool, QMC::DefaultPropertyFlags, 5),
        // property 'mediaAvailable'
        QtMocHelpers::PropertyData<bool>(50, QMetaType::Bool, QMC::DefaultPropertyFlags, 5),
        // property 'doNotDisturb'
        QtMocHelpers::PropertyData<bool>(51, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 6),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QuickPanelBackend, qt_meta_tag_ZN17QuickPanelBackendE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QuickPanelBackend::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17QuickPanelBackendE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17QuickPanelBackendE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN17QuickPanelBackendE_t>.metaTypes,
    nullptr
} };

void QuickPanelBackend::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QuickPanelBackend *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->volumeChanged(); break;
        case 1: _t->brightnessChanged(); break;
        case 2: _t->networkChanged(); break;
        case 3: _t->wifiNetworksChanged(); break;
        case 4: _t->bluetoothChanged(); break;
        case 5: _t->mediaChanged(); break;
        case 6: _t->doNotDisturbChanged(); break;
        case 7: _t->pollStatus(); break;
        case 8: _t->onMprisPropertiesChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QVariantMap>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QStringList>>(_a[3]))); break;
        case 9: _t->setVolume((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 10: _t->toggleMute(); break;
        case 11: _t->setBrightness((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 12: _t->setWifiEnabled((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 13: _t->connectWifi((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 14: _t->scanWifi(); break;
        case 15: { QVariantList _r = _t->wifiNetworks();
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 16: _t->setBluetoothEnabled((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 17: _t->connectBluetooth((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 18: _t->disconnectBluetooth((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 19: _t->scanBluetooth(); break;
        case 20: _t->mediaPlayPause(); break;
        case 21: _t->mediaNext(); break;
        case 22: _t->mediaPrev(); break;
        case 23: _t->mediaStop(); break;
        case 24: _t->refresh(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (QuickPanelBackend::*)()>(_a, &QuickPanelBackend::volumeChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (QuickPanelBackend::*)()>(_a, &QuickPanelBackend::brightnessChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (QuickPanelBackend::*)()>(_a, &QuickPanelBackend::networkChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (QuickPanelBackend::*)()>(_a, &QuickPanelBackend::wifiNetworksChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (QuickPanelBackend::*)()>(_a, &QuickPanelBackend::bluetoothChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (QuickPanelBackend::*)()>(_a, &QuickPanelBackend::mediaChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (QuickPanelBackend::*)()>(_a, &QuickPanelBackend::doNotDisturbChanged, 6))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->volume(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->muted(); break;
        case 2: *reinterpret_cast<int*>(_v) = _t->brightness(); break;
        case 3: *reinterpret_cast<int*>(_v) = _t->maxBrightness(); break;
        case 4: *reinterpret_cast<bool*>(_v) = _t->wifiEnabled(); break;
        case 5: *reinterpret_cast<QString*>(_v) = _t->ssid(); break;
        case 6: *reinterpret_cast<int*>(_v) = _t->wifiStrength(); break;
        case 7: *reinterpret_cast<bool*>(_v) = _t->bluetoothEnabled(); break;
        case 8: *reinterpret_cast<QString*>(_v) = _t->bluetoothDevice(); break;
        case 9: *reinterpret_cast<QVariantList*>(_v) = _t->bluetoothDevices(); break;
        case 10: *reinterpret_cast<QString*>(_v) = _t->mediaTitle(); break;
        case 11: *reinterpret_cast<QString*>(_v) = _t->mediaArtist(); break;
        case 12: *reinterpret_cast<QString*>(_v) = _t->mediaPlayer(); break;
        case 13: *reinterpret_cast<bool*>(_v) = _t->mediaPlaying(); break;
        case 14: *reinterpret_cast<bool*>(_v) = _t->mediaAvailable(); break;
        case 15: *reinterpret_cast<bool*>(_v) = _t->doNotDisturb(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 15: _t->setDoNotDisturb(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QuickPanelBackend::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QuickPanelBackend::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17QuickPanelBackendE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QuickPanelBackend::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 25)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 25;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void QuickPanelBackend::volumeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void QuickPanelBackend::brightnessChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void QuickPanelBackend::networkChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void QuickPanelBackend::wifiNetworksChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void QuickPanelBackend::bluetoothChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void QuickPanelBackend::mediaChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void QuickPanelBackend::doNotDisturbChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP
