#ifndef ISQLSERVICE_H
#define ISQLSERVICE_H

class QSqlQueryModel;

#include "Structs/user.h"
#include "Structs/programstruct.h"


class ISqlDatabaseService
{
public:

    //check Tables
    virtual bool CheckUsersTable() = 0;

    virtual bool CheckStartupTables() = 0;

    virtual bool CheckStartupTables(int roleIndex) = 0;

    virtual bool CheckDesktopTables() = 0;

    virtual bool CheckDesktopTables(int roleIndex) = 0;

    //clear tables

    virtual void ClearUsersTable() = 0;

    virtual void ClearStartupsTable(int roleIndex) = 0;

    virtual void ClearDesktopTable(int roleIndex) = 0;

    //create Tables
    virtual void CreateUsersTableIfNotExists() = 0;

    virtual void CreateStartupsTableIfNotExists(int roleIndex) = 0;

    virtual void CreateDesktopRolesIfNotExists(int roleIndex) = 0;
    //drop Tables
    virtual void DropUsersTable() = 0;

    virtual void DropStartupTable(int roleIndex) = 0;

    virtual void DropDesktopsTable(int roleIndex) = 0;

    //users
    virtual void AppendUserIntoTable(const User &user) = 0;

    virtual void RemoveUserIntoTable(const User &user) = 0;

    virtual QList<User> GetAllUsers() = 0;

    //execs
    virtual void AppendStartupIntoRole(int roleIndex, const QString &startupPath) = 0;

    virtual void RemoveStartupIntoRole(int roleIndex, const QString &startupPath) = 0;

    virtual QStringList GetAllRoleStartups(int roleIndex) = 0;

    virtual void GetAllRoleStartupsIntoModel(int roleIndex) = 0;

    virtual QSqlQueryModel *GetRoleStartupsModel() = 0;

    //desktops
    virtual void AppendDesktopIntoRole(int roleIndex, const DesktopEntity &entity) = 0;

    virtual void RemoveDesktopIntoRole(int roleIndex, const QString &entityName) = 0;

    virtual QList<DesktopEntity> GetAllRoleDesktops(int roleIndex) = 0;

    virtual QStringList GetAllUsersWithRoleIndex(int roleIndex) = 0;

    //admin

    virtual QStringList GetAdminsRoleUserName() = 0;

    virtual QString GetUserFCS(const QString &userName) = 0;

    virtual int GetUserRole(const QString &userName) = 0;

    virtual QString GetUserFCS(QStringView userName) = 0;

    virtual int GetUserRole(QStringView userName) = 0;

};

#endif // ISQLSERVICE_H
