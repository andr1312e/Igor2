/****************************************************************************
** Meta object code from reading C++ file 'runnubleapplicationpanel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Admin_GUI/Views/runnubleapplicationpanel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'runnubleapplicationpanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RunnubleApplicationPanel_t {
    QByteArrayData data[11];
    char stringdata0[119];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RunnubleApplicationPanel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RunnubleApplicationPanel_t qt_meta_stringdata_RunnubleApplicationPanel = {
    {
QT_MOC_LITERAL(0, 0, 24), // "RunnubleApplicationPanel"
QT_MOC_LITERAL(1, 25, 14), // "setDefaultApps"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 8), // "userName"
QT_MOC_LITERAL(4, 50, 4), // "role"
QT_MOC_LITERAL(5, 55, 13), // "deleteProgram"
QT_MOC_LITERAL(6, 69, 10), // "addProgram"
QT_MOC_LITERAL(7, 80, 4), // "exec"
QT_MOC_LITERAL(8, 85, 15), // "onProgramSelect"
QT_MOC_LITERAL(9, 101, 11), // "QModelIndex"
QT_MOC_LITERAL(10, 113, 5) // "index"

    },
    "RunnubleApplicationPanel\0setDefaultApps\0"
    "\0userName\0role\0deleteProgram\0addProgram\0"
    "exec\0onProgramSelect\0QModelIndex\0index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RunnubleApplicationPanel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   34,    2, 0x0a /* Public */,
       5,    0,   39,    2, 0x08 /* Private */,
       6,    1,   40,    2, 0x08 /* Private */,
       8,    1,   43,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, 0x80000000 | 9,   10,

       0        // eod
};

void RunnubleApplicationPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RunnubleApplicationPanel *_t = static_cast<RunnubleApplicationPanel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setDefaultApps((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->deleteProgram(); break;
        case 2: _t->addProgram((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->onProgramSelect((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject RunnubleApplicationPanel::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_RunnubleApplicationPanel.data,
      qt_meta_data_RunnubleApplicationPanel,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *RunnubleApplicationPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RunnubleApplicationPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RunnubleApplicationPanel.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int RunnubleApplicationPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
