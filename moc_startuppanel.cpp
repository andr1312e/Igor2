/****************************************************************************
** Meta object code from reading C++ file 'startuppanel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Admin_GUI/Views/startuppanel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'startuppanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_StartupPanel_t {
    QByteArrayData data[11];
    char stringdata0[125];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_StartupPanel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_StartupPanel_t qt_meta_stringdata_StartupPanel = {
    {
QT_MOC_LITERAL(0, 0, 12), // "StartupPanel"
QT_MOC_LITERAL(1, 13, 22), // "roleStartupFileChanged"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 18), // "setDefaultRoleApps"
QT_MOC_LITERAL(4, 56, 4), // "role"
QT_MOC_LITERAL(5, 61, 13), // "deleteProgram"
QT_MOC_LITERAL(6, 75, 10), // "addProgram"
QT_MOC_LITERAL(7, 86, 4), // "exec"
QT_MOC_LITERAL(8, 91, 15), // "onProgramSelect"
QT_MOC_LITERAL(9, 107, 11), // "QModelIndex"
QT_MOC_LITERAL(10, 119, 5) // "index"

    },
    "StartupPanel\0roleStartupFileChanged\0"
    "\0setDefaultRoleApps\0role\0deleteProgram\0"
    "addProgram\0exec\0onProgramSelect\0"
    "QModelIndex\0index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_StartupPanel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   40,    2, 0x0a /* Public */,
       5,    0,   43,    2, 0x08 /* Private */,
       6,    1,   44,    2, 0x08 /* Private */,
       8,    1,   47,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, 0x80000000 | 9,   10,

       0        // eod
};

void StartupPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        StartupPanel *_t = static_cast<StartupPanel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->roleStartupFileChanged(); break;
        case 1: _t->setDefaultRoleApps((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->deleteProgram(); break;
        case 3: _t->addProgram((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->onProgramSelect((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (StartupPanel::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&StartupPanel::roleStartupFileChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject StartupPanel::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_StartupPanel.data,
      qt_meta_data_StartupPanel,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *StartupPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *StartupPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_StartupPanel.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int StartupPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void StartupPanel::roleStartupFileChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
