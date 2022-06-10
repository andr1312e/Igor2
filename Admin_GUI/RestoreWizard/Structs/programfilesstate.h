#ifndef SERVICES_SETTINGS_PROGRAMFILESSTATE_H
#define SERVICES_SETTINGS_PROGRAMFILESSTATE_H
#include <QtCore/qglobal.h>

enum LoadingState//здесь класс не ставь иначе код не красивый
{
    Fine       = 0x00000000,
    NoUserDb   = 0x00000001,
    NoRoleData = 0x00000002,
    NoFiles    = 0x00000004,
    ForceReset = 0x00000008,
};

Q_DECLARE_FLAGS(LoadingStates, LoadingState)
Q_DECLARE_OPERATORS_FOR_FLAGS (LoadingStates)

//нужно для свойств или как в кути
#endif // SERVICES_SETTINGS_PROGRAMFILESSTATE_H
