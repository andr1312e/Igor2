#ifndef ISQLSERVICE_H
#define ISQLSERVICE_H


#include "Structs/userstruct.h"
#include "Structs/programstruct.h"

class ISqlDatabaseService
{
public:

    //check Tables
    virtual bool CheckUsersTable() = 0;

    virtual bool CheckStartupTables() = 0;

    virtual bool CheckStartupTables(quint8 roleId) = 0;

    virtual bool CheckDesktopTables() = 0;

    virtual bool CheckDesktopTables(quint8 roleId) = 0;

    //clear tables

    virtual void ClearUsersTable() = 0;

    virtual void ClearStartupsTable(quint8 roleId) = 0;

    virtual void ClearDesktopTable(quint8 roleId) = 0;

    //create Tables    
    virtual void CreateUsersTableIfNotExists() = 0;

    virtual void CreateStartupsTableInNotExists(quint8 roleId) = 0;

    virtual void CreateDesktopRolesIfNotExists(quint8 roleId) = 0;

    //users
    virtual void AppendUserIntoTable(const User &user) = 0;

    virtual void RemoveUserIntoTable(quint8 roleId,const User &user) = 0;

    virtual QList<User> GetAllUsers() = 0;

    //execs
    virtual void AppendStartupIntoRole(quint8 roleId, const QString &startupPath) = 0;

    virtual void RemoveStartupIntoRole(quint8 roleId, const QString &startupPath) = 0;

    virtual QStringList GetAllRoleStartups(quint8 roleId) = 0;

    //desktops
    virtual void AppendDesktopIntoRole(quint8 roleId, const DesktopEntity &entity) = 0;

    virtual void RemoveDesktopIntoRole(quint8 roleId, const QString &entityName) = 0;

    virtual QList<DesktopEntity> GetAllRoleDesktops(quint8 roleId) = 0;

    virtual QStringList GetAllUsersWithRoleId(quint8 roleId) = 0;

    //admin

    virtual QStringList GetAdminsRoleUserName() = 0;

    virtual QString GetUserFCS(const QString &userName) = 0;

    virtual QString GetUserRank(const QString &userName) = 0;

    virtual int GetUserRole(const QString &userName) = 0;

};

#endif // ISQLSERVICE_H
