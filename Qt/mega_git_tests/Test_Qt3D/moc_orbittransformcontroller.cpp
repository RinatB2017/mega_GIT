/****************************************************************************
** Meta object code from reading C++ file 'orbittransformcontroller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "src/orbittransformcontroller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'orbittransformcontroller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_OrbitTransformController_t {
    QByteArrayData data[9];
    char stringdata0[109];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OrbitTransformController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OrbitTransformController_t qt_meta_stringdata_OrbitTransformController = {
    {
QT_MOC_LITERAL(0, 0, 24), // "OrbitTransformController"
QT_MOC_LITERAL(1, 25, 13), // "targetChanged"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 13), // "radiusChanged"
QT_MOC_LITERAL(4, 54, 12), // "angleChanged"
QT_MOC_LITERAL(5, 67, 6), // "target"
QT_MOC_LITERAL(6, 74, 21), // "Qt3DCore::QTransform*"
QT_MOC_LITERAL(7, 96, 6), // "radius"
QT_MOC_LITERAL(8, 103, 5) // "angle"

    },
    "OrbitTransformController\0targetChanged\0"
    "\0radiusChanged\0angleChanged\0target\0"
    "Qt3DCore::QTransform*\0radius\0angle"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OrbitTransformController[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       3,   32, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,
       3,    0,   30,    2, 0x06 /* Public */,
       4,    0,   31,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       5, 0x80000000 | 6, 0x0049510b,
       7, QMetaType::Float, 0x00495103,
       8, QMetaType::Float, 0x00495103,

 // properties: notify_signal_id
       0,
       1,
       2,

       0        // eod
};

void OrbitTransformController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OrbitTransformController *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->targetChanged(); break;
        case 1: _t->radiusChanged(); break;
        case 2: _t->angleChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (OrbitTransformController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrbitTransformController::targetChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (OrbitTransformController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrbitTransformController::radiusChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (OrbitTransformController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrbitTransformController::angleChanged)) {
                *result = 2;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<OrbitTransformController *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< Qt3DCore::QTransform**>(_v) = _t->target(); break;
        case 1: *reinterpret_cast< float*>(_v) = _t->radius(); break;
        case 2: *reinterpret_cast< float*>(_v) = _t->angle(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<OrbitTransformController *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setTarget(*reinterpret_cast< Qt3DCore::QTransform**>(_v)); break;
        case 1: _t->setRadius(*reinterpret_cast< float*>(_v)); break;
        case 2: _t->setAngle(*reinterpret_cast< float*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject OrbitTransformController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_OrbitTransformController.data,
    qt_meta_data_OrbitTransformController,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *OrbitTransformController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OrbitTransformController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OrbitTransformController.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int OrbitTransformController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void OrbitTransformController::targetChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void OrbitTransformController::radiusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void OrbitTransformController::angleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
