/****************************************************************************
** Meta object code from reading C++ file 'Config.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/Config.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Config.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN6ConfigE_t {};
} // unnamed namespace

template <> constexpr inline auto Config::qt_create_metaobjectdata<qt_meta_tag_ZN6ConfigE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Config",
        "configChanged",
        "",
        "getEasingType",
        "reload",
        "position",
        "dockHeight",
        "iconSize",
        "iconSizeZoomed",
        "spacing",
        "padding",
        "backgroundColor",
        "backgroundOpacity",
        "borderColor",
        "borderWidth",
        "cornerRadius",
        "accentColor",
        "accentColorFocused",
        "textColor",
        "textSize",
        "animationsEnabled",
        "zoomDuration",
        "fadeDuration",
        "zoomEasing",
        "launcherWidth",
        "launcherHeight",
        "launcherCellWidth",
        "launcherCellHeight",
        "launcherIconSize",
        "searchPlaceholder",
        "updateInterval",
        "skipProcesses",
        "indicatorsEnabled",
        "indicatorColor",
        "indicatorSize",
        "indicatorOpacity",
        "separatorEnabled",
        "separatorWidth",
        "separatorHeight",
        "separatorColor"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'configChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'getEasingType'
        QtMocHelpers::MethodData<QString() const>(3, 2, QMC::AccessPublic, QMetaType::QString),
        // Method 'reload'
        QtMocHelpers::MethodData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'position'
        QtMocHelpers::PropertyData<QString>(5, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'dockHeight'
        QtMocHelpers::PropertyData<int>(6, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'iconSize'
        QtMocHelpers::PropertyData<int>(7, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'iconSizeZoomed'
        QtMocHelpers::PropertyData<int>(8, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'spacing'
        QtMocHelpers::PropertyData<int>(9, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'padding'
        QtMocHelpers::PropertyData<int>(10, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'backgroundColor'
        QtMocHelpers::PropertyData<QString>(11, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'backgroundOpacity'
        QtMocHelpers::PropertyData<double>(12, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'borderColor'
        QtMocHelpers::PropertyData<QString>(13, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'borderWidth'
        QtMocHelpers::PropertyData<int>(14, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'cornerRadius'
        QtMocHelpers::PropertyData<int>(15, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'accentColor'
        QtMocHelpers::PropertyData<QString>(16, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'accentColorFocused'
        QtMocHelpers::PropertyData<QString>(17, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'textColor'
        QtMocHelpers::PropertyData<QString>(18, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'textSize'
        QtMocHelpers::PropertyData<int>(19, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'animationsEnabled'
        QtMocHelpers::PropertyData<bool>(20, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'zoomDuration'
        QtMocHelpers::PropertyData<int>(21, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'fadeDuration'
        QtMocHelpers::PropertyData<int>(22, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'zoomEasing'
        QtMocHelpers::PropertyData<QString>(23, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'launcherWidth'
        QtMocHelpers::PropertyData<int>(24, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'launcherHeight'
        QtMocHelpers::PropertyData<int>(25, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'launcherCellWidth'
        QtMocHelpers::PropertyData<int>(26, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'launcherCellHeight'
        QtMocHelpers::PropertyData<int>(27, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'launcherIconSize'
        QtMocHelpers::PropertyData<int>(28, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'searchPlaceholder'
        QtMocHelpers::PropertyData<QString>(29, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'updateInterval'
        QtMocHelpers::PropertyData<int>(30, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'skipProcesses'
        QtMocHelpers::PropertyData<QStringList>(31, QMetaType::QStringList, QMC::DefaultPropertyFlags, 0),
        // property 'indicatorsEnabled'
        QtMocHelpers::PropertyData<bool>(32, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'indicatorColor'
        QtMocHelpers::PropertyData<QString>(33, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'indicatorSize'
        QtMocHelpers::PropertyData<int>(34, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'indicatorOpacity'
        QtMocHelpers::PropertyData<double>(35, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'separatorEnabled'
        QtMocHelpers::PropertyData<bool>(36, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'separatorWidth'
        QtMocHelpers::PropertyData<int>(37, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'separatorHeight'
        QtMocHelpers::PropertyData<int>(38, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'separatorColor'
        QtMocHelpers::PropertyData<QString>(39, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Config, qt_meta_tag_ZN6ConfigE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Config::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6ConfigE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6ConfigE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN6ConfigE_t>.metaTypes,
    nullptr
} };

void Config::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Config *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->configChanged(); break;
        case 1: { QString _r = _t->getEasingType();
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 2: _t->reload(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Config::*)()>(_a, &Config::configChanged, 0))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->position(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->dockHeight(); break;
        case 2: *reinterpret_cast<int*>(_v) = _t->iconSize(); break;
        case 3: *reinterpret_cast<int*>(_v) = _t->iconSizeZoomed(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->spacing(); break;
        case 5: *reinterpret_cast<int*>(_v) = _t->padding(); break;
        case 6: *reinterpret_cast<QString*>(_v) = _t->backgroundColor(); break;
        case 7: *reinterpret_cast<double*>(_v) = _t->backgroundOpacity(); break;
        case 8: *reinterpret_cast<QString*>(_v) = _t->borderColor(); break;
        case 9: *reinterpret_cast<int*>(_v) = _t->borderWidth(); break;
        case 10: *reinterpret_cast<int*>(_v) = _t->cornerRadius(); break;
        case 11: *reinterpret_cast<QString*>(_v) = _t->accentColor(); break;
        case 12: *reinterpret_cast<QString*>(_v) = _t->accentColorFocused(); break;
        case 13: *reinterpret_cast<QString*>(_v) = _t->textColor(); break;
        case 14: *reinterpret_cast<int*>(_v) = _t->textSize(); break;
        case 15: *reinterpret_cast<bool*>(_v) = _t->animationsEnabled(); break;
        case 16: *reinterpret_cast<int*>(_v) = _t->zoomDuration(); break;
        case 17: *reinterpret_cast<int*>(_v) = _t->fadeDuration(); break;
        case 18: *reinterpret_cast<QString*>(_v) = _t->zoomEasing(); break;
        case 19: *reinterpret_cast<int*>(_v) = _t->launcherWidth(); break;
        case 20: *reinterpret_cast<int*>(_v) = _t->launcherHeight(); break;
        case 21: *reinterpret_cast<int*>(_v) = _t->launcherCellWidth(); break;
        case 22: *reinterpret_cast<int*>(_v) = _t->launcherCellHeight(); break;
        case 23: *reinterpret_cast<int*>(_v) = _t->launcherIconSize(); break;
        case 24: *reinterpret_cast<QString*>(_v) = _t->searchPlaceholder(); break;
        case 25: *reinterpret_cast<int*>(_v) = _t->updateInterval(); break;
        case 26: *reinterpret_cast<QStringList*>(_v) = _t->skipProcesses(); break;
        case 27: *reinterpret_cast<bool*>(_v) = _t->indicatorsEnabled(); break;
        case 28: *reinterpret_cast<QString*>(_v) = _t->indicatorColor(); break;
        case 29: *reinterpret_cast<int*>(_v) = _t->indicatorSize(); break;
        case 30: *reinterpret_cast<double*>(_v) = _t->indicatorOpacity(); break;
        case 31: *reinterpret_cast<bool*>(_v) = _t->separatorEnabled(); break;
        case 32: *reinterpret_cast<int*>(_v) = _t->separatorWidth(); break;
        case 33: *reinterpret_cast<int*>(_v) = _t->separatorHeight(); break;
        case 34: *reinterpret_cast<QString*>(_v) = _t->separatorColor(); break;
        default: break;
        }
    }
}

const QMetaObject *Config::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Config::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6ConfigE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Config::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 35;
    }
    return _id;
}

// SIGNAL 0
void Config::configChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
