#ifndef ISQLSERVICE_H
#define ISQLSERVICE_H

class QSqlQueryModel;

#include "Structs/userstruct.h"
#include "Structs/programstruct.h"


class ISqlDatabaseService
{
public:

    //check Tables
    virtual bool CheckUsersTable() = 0;

    virtual bool CheckStartupTables() = 0;

    virtual bool CheckStartupTables(int roleId) = 0;

    virtual bool CheckDesktopTables() = 0;

    virtual bool CheckDesktopTables(int roleId) = 0;

    //clear tables

    virtual void ClearUsersTable() = 0;

    virtual void ClearStartupsTable(int roleId) = 0;

    virtual void ClearDesktopTable(int roleId) = 0;

    //create Tables    
    virtual void CreateUsersTableIfNotExists() = 0;

    virtual void CreateStartupsTableIfNotExists(int roleId) = 0;

    virtual void CreateDesktopRolesIfNotExists(int roleId) = 0;

    //users
    virtual void AppendUserIntoTable(const User &user) = 0;

    virtual void RemoveUserIntoTable(int roleId,const User &user) = 0;

    virtual QList<User> GetAllUsers() = 0;

    //execs
    virtual void AppendStartupIntoRole(int roleId, const QString &startupPath) = 0;

    virtual void RemoveStartupIntoRole(int roleId, const QString &startupPath) = 0;

    virtual QStringList GetAllRoleStartups(int roleId) = 0;

    virtual void GetAllRoleStartupsIntoModel(int roleId) = 0;

    virtual QSqlQueryModel* GetRoleStartupsModel() = 0;

    //desktops
    virtual void AppendDesktopIntoRole(int roleId, const DesktopEntity &entity) = 0;

    virtual void RemoveDesktopIntoRole(int roleId, const QString &entityName) = 0;

    virtual QList<DesktopEntity> GetAllRoleDesktops(int roleId) = 0;

    virtual QStringList GetAllUsersWithRoleId(int roleId) = 0;

    //admin

    virtual QStringList GetAdminsRoleUserName() = 0;

    virtual QString GetUserFCS(const QString &userName) = 0;

    virtual int GetUserRole(const QString &userName) = 0;

    virtual QString GetUserFCS(QStringView userName) = 0;

    virtual int GetUserRole(QStringView userName) = 0;

};

#endif // ISQLSERVICE_H
