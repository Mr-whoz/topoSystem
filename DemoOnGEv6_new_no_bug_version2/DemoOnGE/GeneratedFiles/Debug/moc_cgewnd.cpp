/****************************************************************************
** Meta object code from reading C++ file 'cgewnd.h'
**
** Created: Mon Jul 9 09:43:29 2018
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../cgewnd.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cgewnd.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CGEwnd[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x08,
      16,    7,    7,    7, 0x08,
      31,    7,    7,    7, 0x08,
      46,    7,    7,    7, 0x08,
      61,    7,    7,    7, 0x08,
      78,    7,    7,    7, 0x08,
      96,    7,    7,    7, 0x08,
     108,    7,    7,    7, 0x08,
     131,    7,    7,    7, 0x08,
     155,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CGEwnd[] = {
    "CGEwnd\0\0about()\0threadFinish()\0"
    "myShowfinish()\0slot_loadKML()\0"
    "slot_inputFile()\0slot_traceRoute()\0"
    "slot_exit()\0slot_gotoDefaultView()\0"
    "setStsBarVisibleOrNot()\0slot_serverSetting()\0"
};

const QMetaObject CGEwnd::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_CGEwnd,
      qt_meta_data_CGEwnd, 0 }
};

const QMetaObject *CGEwnd::metaObject() const
{
    return &staticMetaObject;
}

void *CGEwnd::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CGEwnd))
	return static_cast<void*>(const_cast< CGEwnd*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int CGEwnd::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: about(); break;
        case 1: threadFinish(); break;
        case 2: myShowfinish(); break;
        case 3: slot_loadKML(); break;
        case 4: slot_inputFile(); break;
        case 5: slot_traceRoute(); break;
        case 6: slot_exit(); break;
        case 7: slot_gotoDefaultView(); break;
        case 8: setStsBarVisibleOrNot(); break;
        case 9: slot_serverSetting(); break;
        }
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
