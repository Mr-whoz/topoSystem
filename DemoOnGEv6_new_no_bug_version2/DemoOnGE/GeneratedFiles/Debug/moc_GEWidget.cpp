/****************************************************************************
** Meta object code from reading C++ file 'GEWidget.h'
**
** Created: Mon Jul 9 09:43:28 2018
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../GEWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GEWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GeWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_GeWidget[] = {
    "GeWidget\0"
};

const QMetaObject GeWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_GeWidget,
      qt_meta_data_GeWidget, 0 }
};

const QMetaObject *GeWidget::metaObject() const
{
    return &staticMetaObject;
}

void *GeWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GeWidget))
	return static_cast<void*>(const_cast< GeWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int GeWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
