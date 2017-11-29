/****************************************************************************
** Meta object code from reading C++ file 'mainbox.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/mainbox/mainbox.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainbox.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainBox_t {
    QByteArrayData data[17];
    char stringdata0[238];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainBox_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainBox_t qt_meta_stringdata_MainBox = {
    {
QT_MOC_LITERAL(0, 0, 7), // "MainBox"
QT_MOC_LITERAL(1, 8, 4), // "send"
QT_MOC_LITERAL(2, 13, 0), // ""
QT_MOC_LITERAL(3, 14, 9), // "read_data"
QT_MOC_LITERAL(4, 24, 4), // "data"
QT_MOC_LITERAL(5, 29, 9), // "set_color"
QT_MOC_LITERAL(6, 39, 18), // "update_frame_color"
QT_MOC_LITERAL(7, 58, 4), // "test"
QT_MOC_LITERAL(8, 63, 19), // "command_set_color_R"
QT_MOC_LITERAL(9, 83, 19), // "command_set_color_G"
QT_MOC_LITERAL(10, 103, 19), // "command_set_color_B"
QT_MOC_LITERAL(11, 123, 19), // "command_set_color_W"
QT_MOC_LITERAL(12, 143, 21), // "command_set_color_all"
QT_MOC_LITERAL(13, 165, 19), // "command_run_motor_1"
QT_MOC_LITERAL(14, 185, 19), // "command_run_motor_2"
QT_MOC_LITERAL(15, 205, 18), // "command_run_motors"
QT_MOC_LITERAL(16, 224, 13) // "color_changed"

    },
    "MainBox\0send\0\0read_data\0data\0set_color\0"
    "update_frame_color\0test\0command_set_color_R\0"
    "command_set_color_G\0command_set_color_B\0"
    "command_set_color_W\0command_set_color_all\0"
    "command_run_motor_1\0command_run_motor_2\0"
    "command_run_motors\0color_changed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainBox[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   87,    2, 0x08 /* Private */,
       5,    0,   90,    2, 0x08 /* Private */,
       6,    0,   91,    2, 0x08 /* Private */,
       7,    0,   92,    2, 0x08 /* Private */,
       8,    0,   93,    2, 0x08 /* Private */,
       9,    0,   94,    2, 0x08 /* Private */,
      10,    0,   95,    2, 0x08 /* Private */,
      11,    0,   96,    2, 0x08 /* Private */,
      12,    0,   97,    2, 0x08 /* Private */,
      13,    0,   98,    2, 0x08 /* Private */,
      14,    0,   99,    2, 0x08 /* Private */,
      15,    0,  100,    2, 0x08 /* Private */,
      16,    1,  101,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QByteArray,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void MainBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainBox *_t = static_cast<MainBox *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->send((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->read_data((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 2: _t->set_color(); break;
        case 3: _t->update_frame_color(); break;
        case 4: _t->test(); break;
        case 5: _t->command_set_color_R(); break;
        case 6: _t->command_set_color_G(); break;
        case 7: _t->command_set_color_B(); break;
        case 8: _t->command_set_color_W(); break;
        case 9: _t->command_set_color_all(); break;
        case 10: _t->command_run_motor_1(); break;
        case 11: _t->command_run_motor_2(); break;
        case 12: _t->command_run_motors(); break;
        case 13: _t->color_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (MainBox::*_t)(const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainBox::send)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MainBox::staticMetaObject = {
    { &MyWidget::staticMetaObject, qt_meta_stringdata_MainBox.data,
      qt_meta_data_MainBox,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainBox::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainBox.stringdata0))
        return static_cast<void*>(this);
    return MyWidget::qt_metacast(_clname);
}

int MainBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MyWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void MainBox::send(const QByteArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
