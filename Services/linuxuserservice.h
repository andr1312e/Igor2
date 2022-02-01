#ifndef SERVICES_LINUXUSERSERVICE_H
#define SERVICES_LINUXUSERSERVICE_H

#include "Logging/logger.h"

#include "Services/Terminals/terminal.h"
#include "Structs/userstruct.h"

class LinuxUserService
{
public:
    LinuxUserService();
    ~LinuxUserService();
    void GetAllUsersWithIdInSystem();
    const QList<QPair<QString, QString>>& GetSystemUsersNamesWithIdsList(){ return m_users;}

public:
    const QString GetCurrentUserName();
    const QString GetCurrentUserId();
    bool HasUserAdminPrivileges(const QString &userName);

private:

    QStringList GetUserGroups(const QString &userName);

    void PushUserToNameIdList(const QString &name, const QString &userId);

    void RemoveSystemUsersFromAllUsersList(const QStringList &allUsers);

    bool IsUserSystem(const int &userIdNumber) const;

private:

    Terminal * const m_terminal;

    QList<QPair<QString,QString>> m_users;

};



#endif // SERVICES_LINUXUSERSERVICE_H
