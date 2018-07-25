/****************************************************************************
** Meta object code from reading C++ file 'serverSet.h'
**
** Created: Mon Jul 9 09:43:28 2018
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../serverSet.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serverSet.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ServerSet[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      24,   10,   10,   10, 0x08,
      40,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ServerSet[] = {
    "ServerSet\0\0addClicked()\0deleteClicked()\0"
    "traceClicked()\0"
};

const QMetaObject ServerSet::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ServerSet,
      qt_meta_data_ServerSet, 0 }
};

const QMetaObject *ServerSet::metaObject() const
{
    return &staticMetaObject;
}

void *ServerSet::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ServerSet))
	return static_cast<void*>(const_cast< ServerSet*>(this));
    return QDialog::qt_metacast(_clname);
}

int ServerSet::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: addClicked(); break;
        case 1: deleteClicked(); break;
        case 2: traceClicked(); break;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
