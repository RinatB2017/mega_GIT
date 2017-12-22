/****************************************************************************
** Meta object code from reading C++ file 'webcamera.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/WebCamera/src/webcamera.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'webcamera.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WebCamera_t {
    QByteArrayData data[16];
    char stringdata0[128];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WebCamera_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WebCamera_t qt_meta_stringdata_WebCamera = {
    {
QT_MOC_LITERAL(0, 0, 9), // "WebCamera"
QT_MOC_LITERAL(1, 10, 5), // "start"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 4), // "stop"
QT_MOC_LITERAL(4, 22, 4), // "test"
QT_MOC_LITERAL(5, 27, 5), // "test2"
QT_MOC_LITERAL(6, 33, 5), // "test3"
QT_MOC_LITERAL(7, 39, 5), // "test4"
QT_MOC_LITERAL(8, 45, 5), // "test5"
QT_MOC_LITERAL(9, 51, 14), // "set_brightness"
QT_MOC_LITERAL(10, 66, 5), // "value"
QT_MOC_LITERAL(11, 72, 12), // "set_contrast"
QT_MOC_LITERAL(12, 85, 14), // "set_saturation"
QT_MOC_LITERAL(13, 100, 7), // "set_hue"
QT_MOC_LITERAL(14, 108, 13), // "set_autofocus"
QT_MOC_LITERAL(15, 122, 5) // "state"

    },
    "WebCamera\0start\0\0stop\0test\0test2\0test3\0"
    "test4\0test5\0set_brightness\0value\0"
    "set_contrast\0set_saturation\0set_hue\0"
    "set_autofocus\0state"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WebCamera[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x08 /* Private */,
       3,    0,   75,    2, 0x08 /* Private */,
       4,    0,   76,    2, 0x08 /* Private */,
       5,    0,   77,    2, 0x08 /* Private */,
       6,    0,   78,    2, 0x08 /* Private */,
       7,    0,   79,    2, 0x08 /* Private */,
       8,    0,   80,    2, 0x08 /* Private */,
       9,    1,   81,    2, 0x08 /* Private */,
      11,    1,   84,    2, 0x08 /* Private */,
      12,    1,   87,    2, 0x08 /* Private */,
      13,    1,   90,    2, 0x08 /* Private */,
      14,    1,   93,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Bool,   15,

       0        // eod
};

void WebCamera::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        WebCamera *_t = static_cast<WebCamera *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->start(); break;
        case 1: _t->stop(); break;
        case 2: _t->test(); break;
        case 3: _t->test2(); break;
        case 4: _t->test3(); break;
        case 5: _t->test4(); break;
        case 6: _t->test5(); break;
        case 7: _t->set_brightness((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->set_contrast((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->set_saturation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->set_hue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->set_autofocus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject WebCamera::staticMetaObject = {
    { &MyWidget::staticMetaObject, qt_meta_stringdata_WebCamera.data,
      qt_meta_data_WebCamera,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *WebCamera::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WebCamera::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WebCamera.stringdata0))
        return static_cast<void*>(this);
    return MyWidget::qt_metacast(_clname);
}

int WebCamera::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MyWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
