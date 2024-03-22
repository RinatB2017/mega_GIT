/****************************************************************************
** Meta object code from reading C++ file 'orbittransformcontroller.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/orbittransformcontroller.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'orbittransformcontroller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.2. It"
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

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSOrbitTransformControllerENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSOrbitTransformControllerENDCLASS = QtMocHelpers::stringData(
    "OrbitTransformController",
    "targetChanged",
    "",
    "radiusChanged",
    "angleChanged",
    "target",
    "Qt3DCore::QTransform*",
    "radius",
    "angle"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSOrbitTransformControllerENDCLASS_t {
    uint offsetsAndSizes[18];
    char stringdata0[25];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[14];
    char stringdata4[13];
    char stringdata5[7];
    char stringdata6[22];
    char stringdata7[7];
    char stringdata8[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSOrbitTransformControllerENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSOrbitTransformControllerENDCLASS_t qt_meta_stringdata_CLASSOrbitTransformControllerENDCLASS = {
    {
        QT_MOC_LITERAL(0, 24),  // "OrbitTransformController"
        QT_MOC_LITERAL(25, 13),  // "targetChanged"
        QT_MOC_LITERAL(39, 0),  // ""
        QT_MOC_LITERAL(40, 13),  // "radiusChanged"
        QT_MOC_LITERAL(54, 12),  // "angleChanged"
        QT_MOC_LITERAL(67, 6),  // "target"
        QT_MOC_LITERAL(74, 21),  // "Qt3DCore::QTransform*"
        QT_MOC_LITERAL(96, 6),  // "radius"
        QT_MOC_LITERAL(103, 5)   // "angle"
    },
    "OrbitTransformController",
    "targetChanged",
    "",
    "radiusChanged",
    "angleChanged",
    "target",
    "Qt3DCore::QTransform*",
    "radius",
    "angle"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSOrbitTransformControllerENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       3,   35, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   32,    2, 0x06,    4 /* Public */,
       3,    0,   33,    2, 0x06,    5 /* Public */,
       4,    0,   34,    2, 0x06,    6 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       5, 0x80000000 | 6, 0x0001510b, uint(0), 0,
       7, QMetaType::Float, 0x00015103, uint(1), 0,
       8, QMetaType::Float, 0x00015103, uint(2), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject OrbitTransformController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSOrbitTransformControllerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSOrbitTransformControllerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSOrbitTransformControllerENDCLASS_t,
        // property 'target'
        QtPrivate::TypeAndForceComplete<Qt3DCore::QTransform*, std::true_type>,
        // property 'radius'
        QtPrivate::TypeAndForceComplete<float, std::true_type>,
        // property 'angle'
        QtPrivate::TypeAndForceComplete<float, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<OrbitTransformController, std::true_type>,
        // method 'targetChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'radiusChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'angleChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

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
            if (_t _q_method = &OrbitTransformController::targetChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (OrbitTransformController::*)();
            if (_t _q_method = &OrbitTransformController::radiusChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (OrbitTransformController::*)();
            if (_t _q_method = &OrbitTransformController::angleChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    } else if (_c == QMetaObject::ReadProperty) {
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
    } else if (_c == QMetaObject::BindableProperty) {
    }
    (void)_a;
}

const QMetaObject *OrbitTransformController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OrbitTransformController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSOrbitTransformControllerENDCLASS.stringdata0))
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
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
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
