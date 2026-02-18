/****************************************************************************
** Meta object code from reading C++ file 'ColorProvider.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/ColorProvider.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ColorProvider.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13ColorProviderE_t {};
} // unnamed namespace

template <> constexpr inline auto ColorProvider::qt_create_metaobjectdata<qt_meta_tag_ZN13ColorProviderE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ColorProvider",
        "colorsChanged",
        "",
        "generateStarted",
        "generateFinished",
        "success",
        "reload",
        "generateFromWallpaper",
        "wallpaperPath",
        "color",
        "name",
        "primary",
        "onPrimary",
        "primaryContainer",
        "onPrimaryContainer",
        "secondary",
        "onSecondary",
        "secondaryContainer",
        "onSecondaryContainer",
        "tertiary",
        "onTertiary",
        "tertiaryContainer",
        "onTertiaryContainer",
        "background",
        "onBackground",
        "surface",
        "onSurface",
        "surfaceVariant",
        "onSurfaceVariant",
        "outline",
        "outlineVariant",
        "error",
        "onError",
        "inverseSurface",
        "inverseOnSurface",
        "inversePrimary",
        "loaded"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'colorsChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'generateStarted'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'generateFinished'
        QtMocHelpers::SignalData<void(bool)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 5 },
        }}),
        // Method 'reload'
        QtMocHelpers::MethodData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'generateFromWallpaper'
        QtMocHelpers::MethodData<void(const QString &)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 8 },
        }}),
        // Method 'color'
        QtMocHelpers::MethodData<QString(const QString &) const>(9, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::QString, 10 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'primary'
        QtMocHelpers::PropertyData<QString>(11, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'onPrimary'
        QtMocHelpers::PropertyData<QString>(12, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'primaryContainer'
        QtMocHelpers::PropertyData<QString>(13, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'onPrimaryContainer'
        QtMocHelpers::PropertyData<QString>(14, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'secondary'
        QtMocHelpers::PropertyData<QString>(15, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'onSecondary'
        QtMocHelpers::PropertyData<QString>(16, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'secondaryContainer'
        QtMocHelpers::PropertyData<QString>(17, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'onSecondaryContainer'
        QtMocHelpers::PropertyData<QString>(18, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'tertiary'
        QtMocHelpers::PropertyData<QString>(19, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'onTertiary'
        QtMocHelpers::PropertyData<QString>(20, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'tertiaryContainer'
        QtMocHelpers::PropertyData<QString>(21, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'onTertiaryContainer'
        QtMocHelpers::PropertyData<QString>(22, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'background'
        QtMocHelpers::PropertyData<QString>(23, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'onBackground'
        QtMocHelpers::PropertyData<QString>(24, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'surface'
        QtMocHelpers::PropertyData<QString>(25, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'onSurface'
        QtMocHelpers::PropertyData<QString>(26, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'surfaceVariant'
        QtMocHelpers::PropertyData<QString>(27, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'onSurfaceVariant'
        QtMocHelpers::PropertyData<QString>(28, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'outline'
        QtMocHelpers::PropertyData<QString>(29, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'outlineVariant'
        QtMocHelpers::PropertyData<QString>(30, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'error'
        QtMocHelpers::PropertyData<QString>(31, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'onError'
        QtMocHelpers::PropertyData<QString>(32, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'inverseSurface'
        QtMocHelpers::PropertyData<QString>(33, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'inverseOnSurface'
        QtMocHelpers::PropertyData<QString>(34, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'inversePrimary'
        QtMocHelpers::PropertyData<QString>(35, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'loaded'
        QtMocHelpers::PropertyData<bool>(36, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ColorProvider, qt_meta_tag_ZN13ColorProviderE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ColorProvider::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ColorProviderE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ColorProviderE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13ColorProviderE_t>.metaTypes,
    nullptr
} };

void ColorProvider::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ColorProvider *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->colorsChanged(); break;
        case 1: _t->generateStarted(); break;
        case 2: _t->generateFinished((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->reload(); break;
        case 4: _t->generateFromWallpaper((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: { QString _r = _t->color((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ColorProvider::*)()>(_a, &ColorProvider::colorsChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ColorProvider::*)()>(_a, &ColorProvider::generateStarted, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ColorProvider::*)(bool )>(_a, &ColorProvider::generateFinished, 2))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->primary(); break;
        case 1: *reinterpret_cast<QString*>(_v) = _t->onPrimary(); break;
        case 2: *reinterpret_cast<QString*>(_v) = _t->primaryContainer(); break;
        case 3: *reinterpret_cast<QString*>(_v) = _t->onPrimaryContainer(); break;
        case 4: *reinterpret_cast<QString*>(_v) = _t->secondary(); break;
        case 5: *reinterpret_cast<QString*>(_v) = _t->onSecondary(); break;
        case 6: *reinterpret_cast<QString*>(_v) = _t->secondaryContainer(); break;
        case 7: *reinterpret_cast<QString*>(_v) = _t->onSecondaryContainer(); break;
        case 8: *reinterpret_cast<QString*>(_v) = _t->tertiary(); break;
        case 9: *reinterpret_cast<QString*>(_v) = _t->onTertiary(); break;
        case 10: *reinterpret_cast<QString*>(_v) = _t->tertiaryContainer(); break;
        case 11: *reinterpret_cast<QString*>(_v) = _t->onTertiaryContainer(); break;
        case 12: *reinterpret_cast<QString*>(_v) = _t->background(); break;
        case 13: *reinterpret_cast<QString*>(_v) = _t->onBackground(); break;
        case 14: *reinterpret_cast<QString*>(_v) = _t->surface(); break;
        case 15: *reinterpret_cast<QString*>(_v) = _t->onSurface(); break;
        case 16: *reinterpret_cast<QString*>(_v) = _t->surfaceVariant(); break;
        case 17: *reinterpret_cast<QString*>(_v) = _t->onSurfaceVariant(); break;
        case 18: *reinterpret_cast<QString*>(_v) = _t->outline(); break;
        case 19: *reinterpret_cast<QString*>(_v) = _t->outlineVariant(); break;
        case 20: *reinterpret_cast<QString*>(_v) = _t->error(); break;
        case 21: *reinterpret_cast<QString*>(_v) = _t->onError(); break;
        case 22: *reinterpret_cast<QString*>(_v) = _t->inverseSurface(); break;
        case 23: *reinterpret_cast<QString*>(_v) = _t->inverseOnSurface(); break;
        case 24: *reinterpret_cast<QString*>(_v) = _t->inversePrimary(); break;
        case 25: *reinterpret_cast<bool*>(_v) = _t->loaded(); break;
        default: break;
        }
    }
}

const QMetaObject *ColorProvider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ColorProvider::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ColorProviderE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ColorProvider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    }
    return _id;
}

// SIGNAL 0
void ColorProvider::colorsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ColorProvider::generateStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ColorProvider::generateFinished(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}
QT_WARNING_POP
