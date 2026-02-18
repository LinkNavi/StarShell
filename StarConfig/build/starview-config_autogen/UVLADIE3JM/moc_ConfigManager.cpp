/****************************************************************************
** Meta object code from reading C++ file 'ConfigManager.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/ConfigManager.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ConfigManager.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13ConfigManagerE_t {};
} // unnamed namespace

template <> constexpr inline auto ConfigManager::qt_create_metaobjectdata<qt_meta_tag_ZN13ConfigManagerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ConfigManager",
        "configChanged",
        "",
        "saved",
        "connectionChanged",
        "reloadSuccess",
        "reloadFailed",
        "error",
        "load",
        "save",
        "saveAndReload",
        "reloadCompositor",
        "reconnect",
        "addKeybind",
        "key",
        "action",
        "removeKeybind",
        "index",
        "addRule",
        "QVariantMap",
        "rule",
        "removeRule",
        "addAutostart",
        "cmd",
        "removeAutostart",
        "addTouchpadGesture",
        "fingers",
        "direction",
        "removeTouchpadGesture",
        "addMouseGesture",
        "button",
        "modifiers",
        "removeMouseGesture",
        "applyMatugenColors",
        "colors",
        "gapsInner",
        "gapsOuter",
        "borderWidth",
        "borderColorActive",
        "borderColorInactive",
        "focusFollowsMouse",
        "defaultMode",
        "resizeStep",
        "moveStep",
        "decorEnabled",
        "decorHeight",
        "decorButtonSize",
        "decorButtonSpacing",
        "decorCornerRadius",
        "decorBgColor",
        "decorBgColorInactive",
        "decorTitleColor",
        "decorTitleColorInactive",
        "decorCloseColor",
        "decorMaxColor",
        "decorMinColor",
        "decorFont",
        "decorFontSize",
        "decorButtonsLeft",
        "animEnabled",
        "animDuration",
        "animWindowOpen",
        "animWindowClose",
        "animWindowMove",
        "animWindowResize",
        "animWorkspaceSwitch",
        "animCurve",
        "animFadeMin",
        "animZoomMin",
        "masterRatio",
        "masterCount",
        "bgEnabled",
        "bgColor",
        "bgImage",
        "bgMode",
        "keybinds",
        "QVariantList",
        "rules",
        "autostart",
        "touchpadGestures",
        "mouseGestures",
        "gestureSwipeThreshold",
        "gesturePinchThreshold",
        "gestureMouseThreshold",
        "connected"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'configChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'saved'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'connectionChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'reloadSuccess'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'reloadFailed'
        QtMocHelpers::SignalData<void(QString)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 7 },
        }}),
        // Method 'load'
        QtMocHelpers::MethodData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'save'
        QtMocHelpers::MethodData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'saveAndReload'
        QtMocHelpers::MethodData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'reloadCompositor'
        QtMocHelpers::MethodData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'reconnect'
        QtMocHelpers::MethodData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'addKeybind'
        QtMocHelpers::MethodData<void(const QString &, const QString &)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 14 }, { QMetaType::QString, 15 },
        }}),
        // Method 'removeKeybind'
        QtMocHelpers::MethodData<void(int)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 17 },
        }}),
        // Method 'addRule'
        QtMocHelpers::MethodData<void(const QVariantMap &)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 19, 20 },
        }}),
        // Method 'removeRule'
        QtMocHelpers::MethodData<void(int)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 17 },
        }}),
        // Method 'addAutostart'
        QtMocHelpers::MethodData<void(const QString &)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 23 },
        }}),
        // Method 'removeAutostart'
        QtMocHelpers::MethodData<void(int)>(24, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 17 },
        }}),
        // Method 'addTouchpadGesture'
        QtMocHelpers::MethodData<void(int, const QString &, const QString &)>(25, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 26 }, { QMetaType::QString, 27 }, { QMetaType::QString, 15 },
        }}),
        // Method 'removeTouchpadGesture'
        QtMocHelpers::MethodData<void(int)>(28, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 17 },
        }}),
        // Method 'addMouseGesture'
        QtMocHelpers::MethodData<void(const QString &, const QString &, const QString &, const QString &)>(29, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 30 }, { QMetaType::QString, 31 }, { QMetaType::QString, 27 }, { QMetaType::QString, 15 },
        }}),
        // Method 'removeMouseGesture'
        QtMocHelpers::MethodData<void(int)>(32, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 17 },
        }}),
        // Method 'applyMatugenColors'
        QtMocHelpers::MethodData<void(const QVariantMap &)>(33, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 19, 34 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'gapsInner'
        QtMocHelpers::PropertyData<int>(35, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'gapsOuter'
        QtMocHelpers::PropertyData<int>(36, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'borderWidth'
        QtMocHelpers::PropertyData<int>(37, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'borderColorActive'
        QtMocHelpers::PropertyData<QString>(38, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'borderColorInactive'
        QtMocHelpers::PropertyData<QString>(39, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'focusFollowsMouse'
        QtMocHelpers::PropertyData<bool>(40, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'defaultMode'
        QtMocHelpers::PropertyData<QString>(41, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'resizeStep'
        QtMocHelpers::PropertyData<int>(42, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'moveStep'
        QtMocHelpers::PropertyData<int>(43, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'decorEnabled'
        QtMocHelpers::PropertyData<bool>(44, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'decorHeight'
        QtMocHelpers::PropertyData<int>(45, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'decorButtonSize'
        QtMocHelpers::PropertyData<int>(46, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'decorButtonSpacing'
        QtMocHelpers::PropertyData<int>(47, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'decorCornerRadius'
        QtMocHelpers::PropertyData<int>(48, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'decorBgColor'
        QtMocHelpers::PropertyData<QString>(49, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'decorBgColorInactive'
        QtMocHelpers::PropertyData<QString>(50, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'decorTitleColor'
        QtMocHelpers::PropertyData<QString>(51, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'decorTitleColorInactive'
        QtMocHelpers::PropertyData<QString>(52, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'decorCloseColor'
        QtMocHelpers::PropertyData<QString>(53, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'decorMaxColor'
        QtMocHelpers::PropertyData<QString>(54, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'decorMinColor'
        QtMocHelpers::PropertyData<QString>(55, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'decorFont'
        QtMocHelpers::PropertyData<QString>(56, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'decorFontSize'
        QtMocHelpers::PropertyData<int>(57, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'decorButtonsLeft'
        QtMocHelpers::PropertyData<bool>(58, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'animEnabled'
        QtMocHelpers::PropertyData<bool>(59, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'animDuration'
        QtMocHelpers::PropertyData<int>(60, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'animWindowOpen'
        QtMocHelpers::PropertyData<QString>(61, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'animWindowClose'
        QtMocHelpers::PropertyData<QString>(62, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'animWindowMove'
        QtMocHelpers::PropertyData<QString>(63, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'animWindowResize'
        QtMocHelpers::PropertyData<QString>(64, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'animWorkspaceSwitch'
        QtMocHelpers::PropertyData<QString>(65, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'animCurve'
        QtMocHelpers::PropertyData<QString>(66, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'animFadeMin'
        QtMocHelpers::PropertyData<double>(67, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'animZoomMin'
        QtMocHelpers::PropertyData<double>(68, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'masterRatio'
        QtMocHelpers::PropertyData<double>(69, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'masterCount'
        QtMocHelpers::PropertyData<int>(70, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'bgEnabled'
        QtMocHelpers::PropertyData<bool>(71, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'bgColor'
        QtMocHelpers::PropertyData<QString>(72, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'bgImage'
        QtMocHelpers::PropertyData<QString>(73, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'bgMode'
        QtMocHelpers::PropertyData<QString>(74, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'keybinds'
        QtMocHelpers::PropertyData<QVariantList>(75, 0x80000000 | 76, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'rules'
        QtMocHelpers::PropertyData<QVariantList>(77, 0x80000000 | 76, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'autostart'
        QtMocHelpers::PropertyData<QStringList>(78, QMetaType::QStringList, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'touchpadGestures'
        QtMocHelpers::PropertyData<QVariantList>(79, 0x80000000 | 76, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'mouseGestures'
        QtMocHelpers::PropertyData<QVariantList>(80, 0x80000000 | 76, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'gestureSwipeThreshold'
        QtMocHelpers::PropertyData<double>(81, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'gesturePinchThreshold'
        QtMocHelpers::PropertyData<double>(82, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'gestureMouseThreshold'
        QtMocHelpers::PropertyData<double>(83, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'connected'
        QtMocHelpers::PropertyData<bool>(84, QMetaType::Bool, QMC::DefaultPropertyFlags, 2),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ConfigManager, qt_meta_tag_ZN13ConfigManagerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ConfigManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ConfigManagerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ConfigManagerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13ConfigManagerE_t>.metaTypes,
    nullptr
} };

void ConfigManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ConfigManager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->configChanged(); break;
        case 1: _t->saved(); break;
        case 2: _t->connectionChanged(); break;
        case 3: _t->reloadSuccess(); break;
        case 4: _t->reloadFailed((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->load(); break;
        case 6: _t->save(); break;
        case 7: _t->saveAndReload(); break;
        case 8: _t->reloadCompositor(); break;
        case 9: _t->reconnect(); break;
        case 10: _t->addKeybind((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 11: _t->removeKeybind((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 12: _t->addRule((*reinterpret_cast<std::add_pointer_t<QVariantMap>>(_a[1]))); break;
        case 13: _t->removeRule((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 14: _t->addAutostart((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 15: _t->removeAutostart((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 16: _t->addTouchpadGesture((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 17: _t->removeTouchpadGesture((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 18: _t->addMouseGesture((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[4]))); break;
        case 19: _t->removeMouseGesture((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 20: _t->applyMatugenColors((*reinterpret_cast<std::add_pointer_t<QVariantMap>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ConfigManager::*)()>(_a, &ConfigManager::configChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ConfigManager::*)()>(_a, &ConfigManager::saved, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ConfigManager::*)()>(_a, &ConfigManager::connectionChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (ConfigManager::*)()>(_a, &ConfigManager::reloadSuccess, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (ConfigManager::*)(QString )>(_a, &ConfigManager::reloadFailed, 4))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->gapsInner(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->gapsOuter(); break;
        case 2: *reinterpret_cast<int*>(_v) = _t->borderWidth(); break;
        case 3: *reinterpret_cast<QString*>(_v) = _t->borderColorActive(); break;
        case 4: *reinterpret_cast<QString*>(_v) = _t->borderColorInactive(); break;
        case 5: *reinterpret_cast<bool*>(_v) = _t->focusFollowsMouse(); break;
        case 6: *reinterpret_cast<QString*>(_v) = _t->defaultMode(); break;
        case 7: *reinterpret_cast<int*>(_v) = _t->resizeStep(); break;
        case 8: *reinterpret_cast<int*>(_v) = _t->moveStep(); break;
        case 9: *reinterpret_cast<bool*>(_v) = _t->decorEnabled(); break;
        case 10: *reinterpret_cast<int*>(_v) = _t->decorHeight(); break;
        case 11: *reinterpret_cast<int*>(_v) = _t->decorButtonSize(); break;
        case 12: *reinterpret_cast<int*>(_v) = _t->decorButtonSpacing(); break;
        case 13: *reinterpret_cast<int*>(_v) = _t->decorCornerRadius(); break;
        case 14: *reinterpret_cast<QString*>(_v) = _t->decorBgColor(); break;
        case 15: *reinterpret_cast<QString*>(_v) = _t->decorBgColorInactive(); break;
        case 16: *reinterpret_cast<QString*>(_v) = _t->decorTitleColor(); break;
        case 17: *reinterpret_cast<QString*>(_v) = _t->decorTitleColorInactive(); break;
        case 18: *reinterpret_cast<QString*>(_v) = _t->decorCloseColor(); break;
        case 19: *reinterpret_cast<QString*>(_v) = _t->decorMaxColor(); break;
        case 20: *reinterpret_cast<QString*>(_v) = _t->decorMinColor(); break;
        case 21: *reinterpret_cast<QString*>(_v) = _t->decorFont(); break;
        case 22: *reinterpret_cast<int*>(_v) = _t->decorFontSize(); break;
        case 23: *reinterpret_cast<bool*>(_v) = _t->decorButtonsLeft(); break;
        case 24: *reinterpret_cast<bool*>(_v) = _t->animEnabled(); break;
        case 25: *reinterpret_cast<int*>(_v) = _t->animDuration(); break;
        case 26: *reinterpret_cast<QString*>(_v) = _t->animWindowOpen(); break;
        case 27: *reinterpret_cast<QString*>(_v) = _t->animWindowClose(); break;
        case 28: *reinterpret_cast<QString*>(_v) = _t->animWindowMove(); break;
        case 29: *reinterpret_cast<QString*>(_v) = _t->animWindowResize(); break;
        case 30: *reinterpret_cast<QString*>(_v) = _t->animWorkspaceSwitch(); break;
        case 31: *reinterpret_cast<QString*>(_v) = _t->animCurve(); break;
        case 32: *reinterpret_cast<double*>(_v) = _t->animFadeMin(); break;
        case 33: *reinterpret_cast<double*>(_v) = _t->animZoomMin(); break;
        case 34: *reinterpret_cast<double*>(_v) = _t->masterRatio(); break;
        case 35: *reinterpret_cast<int*>(_v) = _t->masterCount(); break;
        case 36: *reinterpret_cast<bool*>(_v) = _t->bgEnabled(); break;
        case 37: *reinterpret_cast<QString*>(_v) = _t->bgColor(); break;
        case 38: *reinterpret_cast<QString*>(_v) = _t->bgImage(); break;
        case 39: *reinterpret_cast<QString*>(_v) = _t->bgMode(); break;
        case 40: *reinterpret_cast<QVariantList*>(_v) = _t->keybinds(); break;
        case 41: *reinterpret_cast<QVariantList*>(_v) = _t->rules(); break;
        case 42: *reinterpret_cast<QStringList*>(_v) = _t->autostart(); break;
        case 43: *reinterpret_cast<QVariantList*>(_v) = _t->touchpadGestures(); break;
        case 44: *reinterpret_cast<QVariantList*>(_v) = _t->mouseGestures(); break;
        case 45: *reinterpret_cast<double*>(_v) = _t->gestureSwipeThreshold(); break;
        case 46: *reinterpret_cast<double*>(_v) = _t->gesturePinchThreshold(); break;
        case 47: *reinterpret_cast<double*>(_v) = _t->gestureMouseThreshold(); break;
        case 48: *reinterpret_cast<bool*>(_v) = _t->connected(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setGapsInner(*reinterpret_cast<int*>(_v)); break;
        case 1: _t->setGapsOuter(*reinterpret_cast<int*>(_v)); break;
        case 2: _t->setBorderWidth(*reinterpret_cast<int*>(_v)); break;
        case 3: _t->setBorderColorActive(*reinterpret_cast<QString*>(_v)); break;
        case 4: _t->setBorderColorInactive(*reinterpret_cast<QString*>(_v)); break;
        case 5: _t->setFocusFollowsMouse(*reinterpret_cast<bool*>(_v)); break;
        case 6: _t->setDefaultMode(*reinterpret_cast<QString*>(_v)); break;
        case 7: _t->setResizeStep(*reinterpret_cast<int*>(_v)); break;
        case 8: _t->setMoveStep(*reinterpret_cast<int*>(_v)); break;
        case 9: _t->setDecorEnabled(*reinterpret_cast<bool*>(_v)); break;
        case 10: _t->setDecorHeight(*reinterpret_cast<int*>(_v)); break;
        case 11: _t->setDecorButtonSize(*reinterpret_cast<int*>(_v)); break;
        case 12: _t->setDecorButtonSpacing(*reinterpret_cast<int*>(_v)); break;
        case 13: _t->setDecorCornerRadius(*reinterpret_cast<int*>(_v)); break;
        case 14: _t->setDecorBgColor(*reinterpret_cast<QString*>(_v)); break;
        case 15: _t->setDecorBgColorInactive(*reinterpret_cast<QString*>(_v)); break;
        case 16: _t->setDecorTitleColor(*reinterpret_cast<QString*>(_v)); break;
        case 17: _t->setDecorTitleColorInactive(*reinterpret_cast<QString*>(_v)); break;
        case 18: _t->setDecorCloseColor(*reinterpret_cast<QString*>(_v)); break;
        case 19: _t->setDecorMaxColor(*reinterpret_cast<QString*>(_v)); break;
        case 20: _t->setDecorMinColor(*reinterpret_cast<QString*>(_v)); break;
        case 21: _t->setDecorFont(*reinterpret_cast<QString*>(_v)); break;
        case 22: _t->setDecorFontSize(*reinterpret_cast<int*>(_v)); break;
        case 23: _t->setDecorButtonsLeft(*reinterpret_cast<bool*>(_v)); break;
        case 24: _t->setAnimEnabled(*reinterpret_cast<bool*>(_v)); break;
        case 25: _t->setAnimDuration(*reinterpret_cast<int*>(_v)); break;
        case 26: _t->setAnimWindowOpen(*reinterpret_cast<QString*>(_v)); break;
        case 27: _t->setAnimWindowClose(*reinterpret_cast<QString*>(_v)); break;
        case 28: _t->setAnimWindowMove(*reinterpret_cast<QString*>(_v)); break;
        case 29: _t->setAnimWindowResize(*reinterpret_cast<QString*>(_v)); break;
        case 30: _t->setAnimWorkspaceSwitch(*reinterpret_cast<QString*>(_v)); break;
        case 31: _t->setAnimCurve(*reinterpret_cast<QString*>(_v)); break;
        case 32: _t->setAnimFadeMin(*reinterpret_cast<double*>(_v)); break;
        case 33: _t->setAnimZoomMin(*reinterpret_cast<double*>(_v)); break;
        case 34: _t->setMasterRatio(*reinterpret_cast<double*>(_v)); break;
        case 35: _t->setMasterCount(*reinterpret_cast<int*>(_v)); break;
        case 36: _t->setBgEnabled(*reinterpret_cast<bool*>(_v)); break;
        case 37: _t->setBgColor(*reinterpret_cast<QString*>(_v)); break;
        case 38: _t->setBgImage(*reinterpret_cast<QString*>(_v)); break;
        case 39: _t->setBgMode(*reinterpret_cast<QString*>(_v)); break;
        case 42: _t->setAutostart(*reinterpret_cast<QStringList*>(_v)); break;
        case 45: _t->setGestureSwipeThreshold(*reinterpret_cast<double*>(_v)); break;
        case 46: _t->setGesturePinchThreshold(*reinterpret_cast<double*>(_v)); break;
        case 47: _t->setGestureMouseThreshold(*reinterpret_cast<double*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *ConfigManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConfigManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ConfigManagerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ConfigManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 21;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 49;
    }
    return _id;
}

// SIGNAL 0
void ConfigManager::configChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ConfigManager::saved()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ConfigManager::connectionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void ConfigManager::reloadSuccess()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void ConfigManager::reloadFailed(QString _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}
QT_WARNING_POP
