/****************************************************************************
** Meta object code from reading C++ file 'cgewnd.h'
**
** Created: Fri Jul 28 15:57:16 2017
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
      12,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x08,
      16,    7,    7,    7, 0x08,
      31,    7,    7,    7, 0x08,
      48,    7,    7,    7, 0x08,
      66,    7,    7,    7, 0x08,
      78,    7,    7,    7, 0x08,
      96,    7,    7,    7, 0x08,
     113,    7,    7,    7, 0x08,
     127,    7,    7,    7, 0x08,
     139,    7,    7,    7, 0x08,
     156,    7,    7,    7, 0x08,
     179,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CGEwnd[] = {
    "CGEwnd\0\0about()\0slot_loadKML()\0"
    "slot_inputFile()\0slot_loadAllKml()\0"
    "slot_topo()\0slot_traceRoute()\0"
    "slot_toporoute()\0slot_asTopo()\0"
    "slot_exit()\0slot_gotoUESTC()\0"
    "slot_gotoDefaultView()\0setStsBarVisibleOrNot()\0"
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
        case 1: slot_loadKML(); break;
        case 2: slot_inputFile(); break;
        case 3: slot_loadAllKml(); break;
        case 4: slot_topo(); break;
        case 5: slot_traceRoute(); break;
        case 6: slot_toporoute(); break;
        case 7: slot_asTopo(); break;
        case 8: slot_exit(); break;
        case 9: slot_gotoUESTC(); break;
        case 10: slot_gotoDefaultView(); break;
        case 11: setStsBarVisibleOrNot(); break;
        }
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
