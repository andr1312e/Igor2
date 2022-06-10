#ifndef SERVICES_ROLEDESKTOPSERVICE_H
#define SERVICES_ROLEDESKTOPSERVICE_H

#include <QStandardItemModel>

#include "Services/idesktopservice.h"
#include "Services/Terminals/terminal.h"
#include "Services/Sql/isqlservice.h"

class RoleDesktopService : public DesktopService
{
public:
    explicit RoleDesktopService(ISqlDatabaseService *sqlDatabaseService);
    ~RoleDesktopService();

public:
    void CheckRoleDesktopTable(int roleId);
    void GetAllRoleDesktops(int roleId);
    void AddIconToRole(int roleId, const DesktopEntity &entity);
    void DeleteIconToRole(int roleId, const QString &iconName);
    void SetDefaultIconsToUserOnUserRoleUpdate(int oldRoleId, int newRoleId, const QString &userName);

private:
    void DeleteOldIconsFromUser(int roleId, const QString &userDesktopPath);
    void SetIconsToUser(int roleId, const QString &userDesktopPath);
    DesktopEntity CopyExecToRlsTIFolderAndReturnNewEntity(const DesktopEntity &entity);
};

#endif // SERVICES_ROLEDESKTOPSERVICE_H
