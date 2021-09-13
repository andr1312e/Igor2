/****************************************************************************
** Meta object code from reading C++ file 'desktoppanel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Admin_GUI/Views/desktoppanel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'desktoppanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DesktopPanel_t {
    QByteArrayData data[12];
    char stringdata0[120];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DesktopPanel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DesktopPanel_t qt_meta_stringdata_DesktopPanel = {
    {
QT_MOC_LITERAL(0, 0, 12), // "DesktopPanel"
QT_MOC_LITERAL(1, 13, 18), // "setDefaultRoleApps"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 4), // "role"
QT_MOC_LITERAL(4, 38, 10), // "addProgram"
QT_MOC_LITERAL(5, 49, 4), // "exec"
QT_MOC_LITERAL(6, 54, 8), // "iconPath"
QT_MOC_LITERAL(7, 63, 8), // "iconName"
QT_MOC_LITERAL(8, 72, 13), // "deleteProgram"
QT_MOC_LITERAL(9, 86, 15), // "onProgramSelect"
QT_MOC_LITERAL(10, 102, 11), // "QModelIndex"
QT_MOC_LITERAL(11, 114, 5) // "index"

    },
    "DesktopPanel\0setDefaultRoleApps\0\0role\0"
    "addProgram\0exec\0iconPath\0iconName\0"
    "deleteProgram\0onProgramSelect\0QModelIndex\0"
    "index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DesktopPanel[] = {

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
       1,    1,   34,    2, 0x0a /* Public */,
       4,    3,   37,    2, 0x08 /* Private */,
       8,    0,   44,    2, 0x08 /* Private */,
       9,    1,   45,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    5,    6,    7,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,

       0        // eod
};

void DesktopPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DesktopPanel *_t = static_cast<DesktopPanel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setDefaultRoleApps((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->addProgram((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 2: _t->deleteProgram(); break;
        case 3: _t->onProgramSelect((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DesktopPanel::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DesktopPanel.data,
      qt_meta_data_DesktopPanel,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DesktopPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DesktopPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DesktopPanel.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int DesktopPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
