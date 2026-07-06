/****************************************************************************
** Meta object code from reading C++ file 'scrcpycontroller.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../core/scrcpy/scrcpycontroller.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scrcpycontroller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.3. It"
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
struct qt_meta_tag_ZN4core6scrcpy16ScrcpyControllerE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN4core6scrcpy16ScrcpyControllerE = QtMocHelpers::stringData(
    "core::scrcpy::ScrcpyController",
    "started",
    "",
    "embedded",
    "stopped",
    "error",
    "message",
    "logOutput",
    "text",
    "onProcessFinished",
    "exitCode",
    "QProcess::ExitStatus",
    "status",
    "onProcessError",
    "QProcess::ProcessError",
    "procError",
    "onReadyReadStandardOutput",
    "onReadyReadStandardError",
    "tryEmbedWindow"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN4core6scrcpy16ScrcpyControllerE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   74,    2, 0x06,    1 /* Public */,
       3,    0,   75,    2, 0x06,    2 /* Public */,
       4,    0,   76,    2, 0x06,    3 /* Public */,
       5,    1,   77,    2, 0x06,    4 /* Public */,
       7,    1,   80,    2, 0x06,    6 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       9,    2,   83,    2, 0x08,    8 /* Private */,
      13,    1,   88,    2, 0x08,   11 /* Private */,
      16,    0,   91,    2, 0x08,   13 /* Private */,
      17,    0,   92,    2, 0x08,   14 /* Private */,
      18,    0,   93,    2, 0x08,   15 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    8,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 11,   10,   12,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject core::scrcpy::ScrcpyController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ZN4core6scrcpy16ScrcpyControllerE.offsetsAndSizes,
    qt_meta_data_ZN4core6scrcpy16ScrcpyControllerE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN4core6scrcpy16ScrcpyControllerE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ScrcpyController, std::true_type>,
        // method 'started'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'embedded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'stopped'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'error'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'logOutput'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onProcessFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<QProcess::ExitStatus, std::false_type>,
        // method 'onProcessError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QProcess::ProcessError, std::false_type>,
        // method 'onReadyReadStandardOutput'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onReadyReadStandardError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'tryEmbedWindow'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void core::scrcpy::ScrcpyController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ScrcpyController *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->started(); break;
        case 1: _t->embedded(); break;
        case 2: _t->stopped(); break;
        case 3: _t->error((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->logOutput((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->onProcessFinished((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QProcess::ExitStatus>>(_a[2]))); break;
        case 6: _t->onProcessError((*reinterpret_cast< std::add_pointer_t<QProcess::ProcessError>>(_a[1]))); break;
        case 7: _t->onReadyReadStandardOutput(); break;
        case 8: _t->onReadyReadStandardError(); break;
        case 9: _t->tryEmbedWindow(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (ScrcpyController::*)();
            if (_q_method_type _q_method = &ScrcpyController::started; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (ScrcpyController::*)();
            if (_q_method_type _q_method = &ScrcpyController::embedded; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _q_method_type = void (ScrcpyController::*)();
            if (_q_method_type _q_method = &ScrcpyController::stopped; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _q_method_type = void (ScrcpyController::*)(const QString & );
            if (_q_method_type _q_method = &ScrcpyController::error; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _q_method_type = void (ScrcpyController::*)(const QString & );
            if (_q_method_type _q_method = &ScrcpyController::logOutput; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject *core::scrcpy::ScrcpyController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *core::scrcpy::ScrcpyController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN4core6scrcpy16ScrcpyControllerE.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int core::scrcpy::ScrcpyController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void core::scrcpy::ScrcpyController::started()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void core::scrcpy::ScrcpyController::embedded()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void core::scrcpy::ScrcpyController::stopped()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void core::scrcpy::ScrcpyController::error(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void core::scrcpy::ScrcpyController::logOutput(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
