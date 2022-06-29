#ifndef SERVICES_LINUXUSERSERVICE_H
#define SERVICES_LINUXUSERSERVICE_H

#include "Logging/logger.h"

#include "Services/Terminals/terminal.h"
#include "Structs/user.h"

class LinuxUserService
{
public:
    explicit LinuxUserService();
    ~LinuxUserService();
    QList<QPair<QString, QString> > GetAllUsersWithIdInSystem();

public:
    const QString GetCurrentUserName();
    const QString GetCurrentUserId();
    bool HasCurrentUserAdminPrivileges();
    void OpenFlyAdminSmc();
private:
    QStringList GetUserGroups(const QString &userName);
    QList<QPair<QString, QString> > RemoveSystemUsersFromAllUsersList(const QStringList &allUsers);
    bool IsUserSystem(const int &userIdNumber) const;

private:
    Terminal *const m_terminal;
    QString m_currentUserName;
    QString m_currentUserId;
};



#endif // SERVICES_LINUXUSERSERVICE_H
