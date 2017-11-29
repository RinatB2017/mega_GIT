/****************************************************************************
** Meta object code from reading C++ file 'lightingdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/lightingdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'lightingdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Plot_t {
    QByteArrayData data[1];
    char stringdata0[5];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Plot_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Plot_t qt_meta_stringdata_Plot = {
    {
QT_MOC_LITERAL(0, 0, 4) // "Plot"

    },
    "Plot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Plot[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void Plot::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject Plot::staticMetaObject = {
    { &SPlot::staticMetaObject, qt_meta_stringdata_Plot.data,
      qt_meta_data_Plot,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Plot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Plot::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Plot.stringdata0))
        return static_cast<void*>(this);
    return SPlot::qt_metacast(_clname);
}

int Plot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SPlot::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_LightingDlg_t {
    QByteArrayData data[12];
    char stringdata0[85];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LightingDlg_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LightingDlg_t qt_meta_stringdata_LightingDlg = {
    {
QT_MOC_LITERAL(0, 0, 11), // "LightingDlg"
QT_MOC_LITERAL(1, 12, 11), // "setDistance"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 11), // "setEmission"
QT_MOC_LITERAL(4, 37, 7), // "setDiff"
QT_MOC_LITERAL(5, 45, 7), // "setSpec"
QT_MOC_LITERAL(6, 53, 7), // "setShin"
QT_MOC_LITERAL(7, 61, 5), // "reset"
QT_MOC_LITERAL(8, 67, 11), // "setRotation"
QT_MOC_LITERAL(9, 79, 1), // "x"
QT_MOC_LITERAL(10, 81, 1), // "y"
QT_MOC_LITERAL(11, 83, 1) // "z"

    },
    "LightingDlg\0setDistance\0\0setEmission\0"
    "setDiff\0setSpec\0setShin\0reset\0setRotation\0"
    "x\0y\0z"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LightingDlg[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x0a /* Public */,
       3,    1,   52,    2, 0x0a /* Public */,
       4,    1,   55,    2, 0x0a /* Public */,
       5,    1,   58,    2, 0x0a /* Public */,
       6,    1,   61,    2, 0x0a /* Public */,
       7,    0,   64,    2, 0x0a /* Public */,
       8,    3,   65,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double,    9,   10,   11,

       0        // eod
};

void LightingDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LightingDlg *_t = static_cast<LightingDlg *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setDistance((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setEmission((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setDiff((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->setSpec((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->setShin((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->reset(); break;
        case 6: _t->setRotation((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObject LightingDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_LightingDlg.data,
      qt_meta_data_LightingDlg,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *LightingDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LightingDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LightingDlg.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Ui::Dialog"))
        return static_cast< Ui::Dialog*>(this);
    return QDialog::qt_metacast(_clname);
}

int LightingDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
