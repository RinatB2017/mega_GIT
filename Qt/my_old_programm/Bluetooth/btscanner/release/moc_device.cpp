/****************************************************************************
** Meta object code from reading C++ file 'device.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../device.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'device.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DeviceDiscoveryDialog_t {
    QByteArrayData data[21];
    char stringdata0[329];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DeviceDiscoveryDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DeviceDiscoveryDialog_t qt_meta_stringdata_DeviceDiscoveryDialog = {
    {
QT_MOC_LITERAL(0, 0, 21), // "DeviceDiscoveryDialog"
QT_MOC_LITERAL(1, 22, 9), // "addDevice"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 20), // "QBluetoothDeviceInfo"
QT_MOC_LITERAL(4, 54, 16), // "on_power_clicked"
QT_MOC_LITERAL(5, 71, 7), // "clicked"
QT_MOC_LITERAL(6, 79, 23), // "on_discoverable_clicked"
QT_MOC_LITERAL(7, 103, 18), // "displayPairingMenu"
QT_MOC_LITERAL(8, 122, 3), // "pos"
QT_MOC_LITERAL(9, 126, 11), // "pairingDone"
QT_MOC_LITERAL(10, 138, 17), // "QBluetoothAddress"
QT_MOC_LITERAL(11, 156, 30), // "QBluetoothLocalDevice::Pairing"
QT_MOC_LITERAL(12, 187, 9), // "startScan"
QT_MOC_LITERAL(13, 197, 12), // "scanFinished"
QT_MOC_LITERAL(14, 210, 19), // "setGeneralUnlimited"
QT_MOC_LITERAL(15, 230, 9), // "unlimited"
QT_MOC_LITERAL(16, 240, 13), // "itemActivated"
QT_MOC_LITERAL(17, 254, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(18, 271, 4), // "item"
QT_MOC_LITERAL(19, 276, 20), // "hostModeStateChanged"
QT_MOC_LITERAL(20, 297, 31) // "QBluetoothLocalDevice::HostMode"

    },
    "DeviceDiscoveryDialog\0addDevice\0\0"
    "QBluetoothDeviceInfo\0on_power_clicked\0"
    "clicked\0on_discoverable_clicked\0"
    "displayPairingMenu\0pos\0pairingDone\0"
    "QBluetoothAddress\0QBluetoothLocalDevice::Pairing\0"
    "startScan\0scanFinished\0setGeneralUnlimited\0"
    "unlimited\0itemActivated\0QListWidgetItem*\0"
    "item\0hostModeStateChanged\0"
    "QBluetoothLocalDevice::HostMode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DeviceDiscoveryDialog[] = {

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
       6,    1,   70,    2, 0x0a /* Public */,
       7,    1,   73,    2, 0x0a /* Public */,
       9,    2,   76,    2, 0x0a /* Public */,
      12,    0,   81,    2, 0x08 /* Private */,
      13,    0,   82,    2, 0x08 /* Private */,
      14,    1,   83,    2, 0x08 /* Private */,
      16,    1,   86,    2, 0x08 /* Private */,
      19,    1,   89,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::QPoint,    8,
    QMetaType::Void, 0x80000000 | 10, 0x80000000 | 11,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   15,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void, 0x80000000 | 20,    2,

       0        // eod
};

void DeviceDiscoveryDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DeviceDiscoveryDialog *_t = static_cast<DeviceDiscoveryDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addDevice((*reinterpret_cast< const QBluetoothDeviceInfo(*)>(_a[1]))); break;
        case 1: _t->on_power_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->on_discoverable_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->displayPairingMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 4: _t->pairingDone((*reinterpret_cast< const QBluetoothAddress(*)>(_a[1])),(*reinterpret_cast< QBluetoothLocalDevice::Pairing(*)>(_a[2]))); break;
        case 5: _t->startScan(); break;
        case 6: _t->scanFinished(); break;
        case 7: _t->setGeneralUnlimited((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->itemActivated((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 9: _t->hostModeStateChanged((*reinterpret_cast< QBluetoothLocalDevice::HostMode(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QBluetoothAddress >(); break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QBluetoothLocalDevice::Pairing >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QBluetoothLocalDevice::HostMode >(); break;
            }
            break;
        }
    }
}

const QMetaObject DeviceDiscoveryDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DeviceDiscoveryDialog.data,
      qt_meta_data_DeviceDiscoveryDialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *DeviceDiscoveryDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DeviceDiscoveryDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_DeviceDiscoveryDialog.stringdata0))
        return static_cast<void*>(const_cast< DeviceDiscoveryDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int DeviceDiscoveryDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
