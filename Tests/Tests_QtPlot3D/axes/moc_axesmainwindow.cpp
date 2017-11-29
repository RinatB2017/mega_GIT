/****************************************************************************
** Meta object code from reading C++ file 'axesmainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/axesmainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'axesmainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AxesMainWindow_t {
    QByteArrayData data[15];
    char stringdata0[158];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AxesMainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AxesMainWindow_t qt_meta_stringdata_AxesMainWindow = {
    {
QT_MOC_LITERAL(0, 0, 14), // "AxesMainWindow"
QT_MOC_LITERAL(1, 15, 12), // "setNumberGap"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 3), // "gap"
QT_MOC_LITERAL(4, 33, 11), // "setLabelGap"
QT_MOC_LITERAL(5, 45, 14), // "setSmoothLines"
QT_MOC_LITERAL(6, 60, 12), // "setTicLength"
QT_MOC_LITERAL(7, 73, 3), // "val"
QT_MOC_LITERAL(8, 77, 12), // "setTicNumber"
QT_MOC_LITERAL(9, 90, 6), // "degree"
QT_MOC_LITERAL(10, 97, 13), // "standardItems"
QT_MOC_LITERAL(11, 111, 12), // "complexItems"
QT_MOC_LITERAL(12, 124, 11), // "letterItems"
QT_MOC_LITERAL(13, 136, 9), // "timeItems"
QT_MOC_LITERAL(14, 146, 11) // "customScale"

    },
    "AxesMainWindow\0setNumberGap\0\0gap\0"
    "setLabelGap\0setSmoothLines\0setTicLength\0"
    "val\0setTicNumber\0degree\0standardItems\0"
    "complexItems\0letterItems\0timeItems\0"
    "customScale"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AxesMainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x0a /* Public */,
       4,    1,   67,    2, 0x0a /* Public */,
       5,    1,   70,    2, 0x0a /* Public */,
       6,    1,   73,    2, 0x0a /* Public */,
       8,    1,   76,    2, 0x0a /* Public */,
      10,    0,   79,    2, 0x0a /* Public */,
      11,    0,   80,    2, 0x0a /* Public */,
      12,    0,   81,    2, 0x0a /* Public */,
      13,    0,   82,    2, 0x0a /* Public */,
      14,    0,   83,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AxesMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AxesMainWindow *_t = static_cast<AxesMainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setNumberGap((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setLabelGap((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setSmoothLines((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->setTicLength((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->setTicNumber((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->standardItems(); break;
        case 6: _t->complexItems(); break;
        case 7: _t->letterItems(); break;
        case 8: _t->timeItems(); break;
        case 9: _t->customScale(); break;
        default: ;
        }
    }
}

const QMetaObject AxesMainWindow::staticMetaObject = {
    { &DummyBase::staticMetaObject, qt_meta_stringdata_AxesMainWindow.data,
      qt_meta_data_AxesMainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *AxesMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AxesMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AxesMainWindow.stringdata0))
        return static_cast<void*>(this);
    return DummyBase::qt_metacast(_clname);
}

int AxesMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DummyBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
