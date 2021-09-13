/****************************************************************************
** Meta object code from reading C++ file 'usereditpanel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Admin_GUI/Views/usereditpanel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'usereditpanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UserEditPanel_t {
    QByteArrayData data[11];
    char stringdata0[133];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UserEditPanel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UserEditPanel_t qt_meta_stringdata_UserEditPanel = {
    {
QT_MOC_LITERAL(0, 0, 13), // "UserEditPanel"
QT_MOC_LITERAL(1, 14, 18), // "setDefaultRoleApps"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 4), // "role"
QT_MOC_LITERAL(4, 39, 17), // "roleToViewChanged"
QT_MOC_LITERAL(5, 57, 22), // "fillUserAdditionalInfo"
QT_MOC_LITERAL(6, 80, 3), // "FCS"
QT_MOC_LITERAL(7, 84, 4), // "rank"
QT_MOC_LITERAL(8, 89, 23), // "clearUserAdditionalInfo"
QT_MOC_LITERAL(9, 113, 8), // "saveUser"
QT_MOC_LITERAL(10, 122, 10) // "deleteUser"

    },
    "UserEditPanel\0setDefaultRoleApps\0\0"
    "role\0roleToViewChanged\0fillUserAdditionalInfo\0"
    "FCS\0rank\0clearUserAdditionalInfo\0"
    "saveUser\0deleteUser"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UserEditPanel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    1,   47,    2, 0x06 /* Public */,
       5,    3,   50,    2, 0x06 /* Public */,
       8,    0,   57,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   58,    2, 0x08 /* Private */,
      10,    0,   59,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    6,    7,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void UserEditPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        UserEditPanel *_t = static_cast<UserEditPanel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setDefaultRoleApps((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->roleToViewChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->fillUserAdditionalInfo((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 3: _t->clearUserAdditionalInfo(); break;
        case 4: _t->saveUser(); break;
        case 5: _t->deleteUser(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (UserEditPanel::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserEditPanel::setDefaultRoleApps)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (UserEditPanel::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserEditPanel::roleToViewChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (UserEditPanel::*)(const QString & , const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserEditPanel::fillUserAdditionalInfo)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (UserEditPanel::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserEditPanel::clearUserAdditionalInfo)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject UserEditPanel::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_UserEditPanel.data,
      qt_meta_data_UserEditPanel,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *UserEditPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UserEditPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UserEditPanel.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int UserEditPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void UserEditPanel::setDefaultRoleApps(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void UserEditPanel::roleToViewChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void UserEditPanel::fillUserAdditionalInfo(const QString & _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void UserEditPanel::clearUserAdditionalInfo()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
