/****************************************************************************
** Meta object code from reading C++ file 'recorderSettings.h'
**
** Created: Thu Apr 24 17:39:37 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../qt_gui_inputs/recorderSettings.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'recorderSettings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RecorderSettings[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      24,   18,   17,   17, 0x08,
      52,   17,   17,   17, 0x08,
      73,   17,   17,   17, 0x08,
      96,   17,   17,   17, 0x08,
     118,   18,   17,   17, 0x08,
     162,  154,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_RecorderSettings[] = {
    "RecorderSettings\0\0index\0"
    "on_camIdTab1_activated(int)\0"
    "on_setTab1_clicked()\0on_startTab1_clicked()\0"
    "on_stopTab1_clicked()\0"
    "on_colorChannelsTab1_activated(int)\0"
    "checked\0on_setTab1_clicked(bool)\0"
};

void RecorderSettings::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        RecorderSettings *_t = static_cast<RecorderSettings *>(_o);
        switch (_id) {
        case 0: _t->on_camIdTab1_activated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_setTab1_clicked(); break;
        case 2: _t->on_startTab1_clicked(); break;
        case 3: _t->on_stopTab1_clicked(); break;
        case 4: _t->on_colorChannelsTab1_activated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->on_setTab1_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData RecorderSettings::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject RecorderSettings::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_RecorderSettings,
      qt_meta_data_RecorderSettings, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RecorderSettings::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RecorderSettings::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RecorderSettings::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RecorderSettings))
        return static_cast<void*>(const_cast< RecorderSettings*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int RecorderSettings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
