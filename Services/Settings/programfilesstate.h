#ifndef SERVICES_SETTINGS_PROGRAMFILESSTATE_H
#define SERVICES_SETTINGS_PROGRAMFILESSTATE_H

enum LoadingState {
   NoFiles = 0,
   NoUserDb = 1,
   NoRoleData = 2,
   Fine = 3,
   CantRun = 4,
};

#endif // SERVICES_SETTINGS_PROGRAMFILESSTATE_H
