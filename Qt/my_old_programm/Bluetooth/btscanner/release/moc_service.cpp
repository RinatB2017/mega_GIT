/****************************************************************************
** Meta object code from reading C++ file 'service.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../service.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'service.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ServiceDiscoveryDialog_t {
    QByteArrayData data[4];
    char stringdata0[57];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ServiceDiscoveryDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ServiceDiscoveryDialog_t qt_meta_stringdata_ServiceDiscoveryDialog = {
    {
QT_MOC_LITERAL(0, 0, 22), // "ServiceDiscoveryDialog"
QT_MOC_LITERAL(1, 23, 10), // "addService"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 21) // "QBluetoothServiceInfo"

    },
    "ServiceDiscoveryDialog\0addService\0\0"
    "QBluetoothServiceInfo"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ServiceDiscoveryDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    2,

       0        // eod
};

void ServiceDiscoveryDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ServiceDiscoveryDialog *_t = static_cast<ServiceDiscoveryDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addService((*reinterpret_cast< const QBluetoothServiceInfo(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject ServiceDiscoveryDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ServiceDiscoveryDialog.data,
      qt_meta_data_ServiceDiscoveryDialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ServiceDiscoveryDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ServiceDiscoveryDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ServiceDiscoveryDialog.stringdata0))
        return static_cast<void*>(const_cast< ServiceDiscoveryDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int ServiceDiscoveryDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
