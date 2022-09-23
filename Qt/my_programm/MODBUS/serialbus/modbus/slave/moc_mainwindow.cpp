/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "src/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[20];
    char stringdata0[269];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 24), // "on_connectButton_clicked"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 14), // "onStateChanged"
QT_MOC_LITERAL(4, 52, 5), // "state"
QT_MOC_LITERAL(5, 58, 11), // "coilChanged"
QT_MOC_LITERAL(6, 70, 2), // "id"
QT_MOC_LITERAL(7, 73, 20), // "discreteInputChanged"
QT_MOC_LITERAL(8, 94, 10), // "bitChanged"
QT_MOC_LITERAL(9, 105, 29), // "QModbusDataUnit::RegisterType"
QT_MOC_LITERAL(10, 135, 5), // "table"
QT_MOC_LITERAL(11, 141, 5), // "value"
QT_MOC_LITERAL(12, 147, 11), // "setRegister"
QT_MOC_LITERAL(13, 159, 13), // "updateWidgets"
QT_MOC_LITERAL(14, 173, 7), // "address"
QT_MOC_LITERAL(15, 181, 4), // "size"
QT_MOC_LITERAL(16, 186, 34), // "on_connectType_currentIndexCh..."
QT_MOC_LITERAL(17, 221, 17), // "handleDeviceError"
QT_MOC_LITERAL(18, 239, 20), // "QModbusDevice::Error"
QT_MOC_LITERAL(19, 260, 8) // "newError"

    },
    "MainWindow\0on_connectButton_clicked\0"
    "\0onStateChanged\0state\0coilChanged\0id\0"
    "discreteInputChanged\0bitChanged\0"
    "QModbusDataUnit::RegisterType\0table\0"
    "value\0setRegister\0updateWidgets\0address\0"
    "size\0on_connectType_currentIndexChanged\0"
    "handleDeviceError\0QModbusDevice::Error\0"
    "newError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x08 /* Private */,
       3,    1,   60,    2, 0x08 /* Private */,
       5,    1,   63,    2, 0x08 /* Private */,
       7,    1,   66,    2, 0x08 /* Private */,
       8,    3,   69,    2, 0x08 /* Private */,
      12,    1,   76,    2, 0x08 /* Private */,
      13,    3,   79,    2, 0x08 /* Private */,
      16,    1,   86,    2, 0x08 /* Private */,
      17,    1,   89,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 9, QMetaType::Bool,    6,   10,   11,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void, 0x80000000 | 9, QMetaType::Int, QMetaType::Int,   10,   14,   15,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, 0x80000000 | 18,   19,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_connectButton_clicked(); break;
        case 1: _t->onStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->coilChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->discreteInputChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->bitChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QModbusDataUnit::RegisterType(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 5: _t->setRegister((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->updateWidgets((*reinterpret_cast< QModbusDataUnit::RegisterType(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 7: _t->on_connectType_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->handleDeviceError((*reinterpret_cast< QModbusDevice::Error(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QModbusDataUnit::RegisterType >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QModbusDataUnit::RegisterType >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
