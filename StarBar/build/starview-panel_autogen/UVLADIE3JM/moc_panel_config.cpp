/****************************************************************************
** Meta object code from reading C++ file 'panel_config.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/panel_config.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'panel_config.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN11PanelConfigE_t {};
} // unnamed namespace

template <> constexpr inline auto PanelConfig::qt_create_metaobjectdata<qt_meta_tag_ZN11PanelConfigE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "PanelConfig",
        "configChanged",
        "",
        "reload",
        "height",
        "position",
        "showWorkspaces",
        "showTitle",
        "showClock",
        "showSystray",
        "workspaceCount",
        "clockFormat",
        "showDate",
        "opacity",
        "cornerRadius",
        "margins",
        "floating",
        "bgColor",
        "fgColor",
        "accentColor",
        "workspaceActiveColor",
        "workspaceInactiveColor"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'configChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'reload'
        QtMocHelpers::MethodData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'height'
        QtMocHelpers::PropertyData<int>(4, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'position'
        QtMocHelpers::PropertyData<int>(5, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'showWorkspaces'
        QtMocHelpers::PropertyData<bool>(6, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'showTitle'
        QtMocHelpers::PropertyData<bool>(7, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'showClock'
        QtMocHelpers::PropertyData<bool>(8, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'showSystray'
        QtMocHelpers::PropertyData<bool>(9, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'workspaceCount'
        QtMocHelpers::PropertyData<int>(10, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'clockFormat'
        QtMocHelpers::PropertyData<QString>(11, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'showDate'
        QtMocHelpers::PropertyData<bool>(12, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'opacity'
        QtMocHelpers::PropertyData<double>(13, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'cornerRadius'
        QtMocHelpers::PropertyData<int>(14, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'margins'
        QtMocHelpers::PropertyData<int>(15, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'floating'
        QtMocHelpers::PropertyData<bool>(16, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'bgColor'
        QtMocHelpers::PropertyData<QString>(17, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'fgColor'
        QtMocHelpers::PropertyData<QString>(18, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'accentColor'
        QtMocHelpers::PropertyData<QString>(19, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'workspaceActiveColor'
        QtMocHelpers::PropertyData<QString>(20, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'workspaceInactiveColor'
        QtMocHelpers::PropertyData<QString>(21, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<PanelConfig, qt_meta_tag_ZN11PanelConfigE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject PanelConfig::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11PanelConfigE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11PanelConfigE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11PanelConfigE_t>.metaTypes,
    nullptr
} };

void PanelConfig::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<PanelConfig *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->configChanged(); break;
        case 1: _t->reload(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (PanelConfig::*)()>(_a, &PanelConfig::configChanged, 0))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->height(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->position(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->showWorkspaces(); break;
        case 3: *reinterpret_cast<bool*>(_v) = _t->showTitle(); break;
        case 4: *reinterpret_cast<bool*>(_v) = _t->showClock(); break;
        case 5: *reinterpret_cast<bool*>(_v) = _t->showSystray(); break;
        case 6: *reinterpret_cast<int*>(_v) = _t->workspaceCount(); break;
        case 7: *reinterpret_cast<QString*>(_v) = _t->clockFormat(); break;
        case 8: *reinterpret_cast<bool*>(_v) = _t->showDate(); break;
        case 9: *reinterpret_cast<double*>(_v) = _t->opacity(); break;
        case 10: *reinterpret_cast<int*>(_v) = _t->cornerRadius(); break;
        case 11: *reinterpret_cast<int*>(_v) = _t->margins(); break;
        case 12: *reinterpret_cast<bool*>(_v) = _t->floating(); break;
        case 13: *reinterpret_cast<QString*>(_v) = _t->bgColor(); break;
        case 14: *reinterpret_cast<QString*>(_v) = _t->fgColor(); break;
        case 15: *reinterpret_cast<QString*>(_v) = _t->accentColor(); break;
        case 16: *reinterpret_cast<QString*>(_v) = _t->workspaceActiveColor(); break;
        case 17: *reinterpret_cast<QString*>(_v) = _t->workspaceInactiveColor(); break;
        default: break;
        }
    }
}

const QMetaObject *PanelConfig::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PanelConfig::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11PanelConfigE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int PanelConfig::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void PanelConfig::configChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
