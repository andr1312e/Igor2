#ifndef SERVICES_LINUXUSERSERVICE_H
#define SERVICES_LINUXUSERSERVICE_H

#include "Logging/logger.h"

#include "Services/Terminals/terminal.h"
#include "Structs/userstruct.h"

class LinuxUserService
{
public:
    explicit LinuxUserService();
    ~LinuxUserService();
    void GetAllUsersWithIdInSystem();
    const QList<QPair<QString, QString>>& GetSystemUsersNamesWithIds(){ return m_users;}

public:
    const QString GetCurrentUserName();
    const QString GetCurrentUserId();
    bool HasCurrentUserAdminPrivileges();

private:
    QStringList GetUserGroups(const QString &userName);
    void PushUserToNameIdList(const QString &name, const QString &userId);
    void RemoveSystemUsersFromAllUsersList(const QStringList &allUsers);
    bool IsUserSystem(const int &userIdNumber) const;

private:
    Terminal * const m_terminal;
    QString m_currentUserName;
    QString m_currentUserId;
    QList<QPair<QString, QString>> m_users;
};



#endif // SERVICES_LINUXUSERSERVICE_H
