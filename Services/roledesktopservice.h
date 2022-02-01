#ifndef ROLEDESKTOPSERVICE_H
#define ROLEDESKTOPSERVICE_H
class QStandardItemModel;

#include <Services/idesktopservice.h>

class Terminal;
class ISqlDatabaseService;

class RoleDesktopService : public DesktopService
{
public:
    RoleDesktopService(ISqlDatabaseService *sqlDatabaseService);
    ~RoleDesktopService();

public:
    void CheckRoleDesktopTable(const quint8 &roleId);
    void GetAllRoleDesktops(const quint8 &roleId);
    void AddIconToRole(const quint8 &roleId, const DesktopEntity &entity);
    void DeleteIconToRole(const quint8 &roleId, const QString iconName);
    void SetDefaultIconsToUserOnUserRoleUpdate(const quint8 &oldRoleId, const quint8& newRoleId, const QString &userName);

private:
    void DeleteOldIconsFromUser(const quint8 &roleId, const QString &userDesktopPath);
    void SetIconsToUser(const quint8 &roleId, const QString &userDesktopPath);
    DesktopEntity CopyExecToRlsTIFolderAndReturnNewEntity(const DesktopEntity &entity);
};

#endif // ROLEDESKTOPSERVICE_H
