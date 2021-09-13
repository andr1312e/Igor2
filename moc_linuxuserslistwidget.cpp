/****************************************************************************
** Meta object code from reading C++ file 'linuxuserslistwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Admin_GUI/Views/linuxuserslistwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'linuxuserslistwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LinuxUsersListWidget_t {
    QByteArrayData data[13];
    char stringdata0[131];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LinuxUsersListWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LinuxUsersListWidget_t qt_meta_stringdata_LinuxUsersListWidget = {
    {
QT_MOC_LITERAL(0, 0, 20), // "LinuxUsersListWidget"
QT_MOC_LITERAL(1, 21, 11), // "onUserClick"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 5), // "User&"
QT_MOC_LITERAL(4, 40, 4), // "user"
QT_MOC_LITERAL(5, 45, 6), // "search"
QT_MOC_LITERAL(6, 52, 4), // "text"
QT_MOC_LITERAL(7, 57, 9), // "attribute"
QT_MOC_LITERAL(8, 67, 16), // "onLineEditChange"
QT_MOC_LITERAL(9, 84, 16), // "onComboBoxChange"
QT_MOC_LITERAL(10, 101, 11), // "getUserData"
QT_MOC_LITERAL(11, 113, 11), // "QModelIndex"
QT_MOC_LITERAL(12, 125, 5) // "index"

    },
    "LinuxUsersListWidget\0onUserClick\0\0"
    "User&\0user\0search\0text\0attribute\0"
    "onLineEditChange\0onComboBoxChange\0"
    "getUserData\0QModelIndex\0index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LinuxUsersListWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       5,    2,   42,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   47,    2, 0x08 /* Private */,
       9,    1,   50,    2, 0x08 /* Private */,
      10,    1,   53,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    6,    7,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, 0x80000000 | 11,   12,

       0        // eod
};

void LinuxUsersListWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LinuxUsersListWidget *_t = static_cast<LinuxUsersListWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onUserClick((*reinterpret_cast< User(*)>(_a[1]))); break;
        case 1: _t->search((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->onLineEditChange((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->onComboBoxChange((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->getUserData((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (LinuxUsersListWidget::*)(User & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LinuxUsersListWidget::onUserClick)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (LinuxUsersListWidget::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LinuxUsersListWidget::search)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LinuxUsersListWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_LinuxUsersListWidget.data,
      qt_meta_data_LinuxUsersListWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *LinuxUsersListWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LinuxUsersListWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LinuxUsersListWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int LinuxUsersListWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void LinuxUsersListWidget::onUserClick(User & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void LinuxUsersListWidget::search(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
