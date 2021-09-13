/****************************************************************************
** Meta object code from reading C++ file 'admingui.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Admin_GUI/Views/admingui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'admingui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Admin_GUI_t {
    QByteArrayData data[10];
    char stringdata0[106];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Admin_GUI_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Admin_GUI_t qt_meta_stringdata_Admin_GUI = {
    {
QT_MOC_LITERAL(0, 0, 9), // "Admin_GUI"
QT_MOC_LITERAL(1, 10, 14), // "setStyleSheets"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 22), // "hideAdditionalSettings"
QT_MOC_LITERAL(4, 49, 5), // "state"
QT_MOC_LITERAL(5, 55, 17), // "roleToViewChanged"
QT_MOC_LITERAL(6, 73, 4), // "role"
QT_MOC_LITERAL(7, 78, 16), // "onLinuxUserClick"
QT_MOC_LITERAL(8, 95, 5), // "User&"
QT_MOC_LITERAL(9, 101, 4) // "user"

    },
    "Admin_GUI\0setStyleSheets\0\0"
    "hideAdditionalSettings\0state\0"
    "roleToViewChanged\0role\0onLinuxUserClick\0"
    "User&\0user"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Admin_GUI[] = {

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
       1,    0,   34,    2, 0x08 /* Private */,
       3,    1,   35,    2, 0x08 /* Private */,
       5,    1,   38,    2, 0x08 /* Private */,
       7,    1,   41,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, 0x80000000 | 8,    9,

       0        // eod
};

void Admin_GUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Admin_GUI *_t = static_cast<Admin_GUI *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setStyleSheets(); break;
        case 1: _t->hideAdditionalSettings((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->roleToViewChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->onLinuxUserClick((*reinterpret_cast< User(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Admin_GUI::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Admin_GUI.data,
      qt_meta_data_Admin_GUI,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Admin_GUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Admin_GUI::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Admin_GUI.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Admin_GUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
