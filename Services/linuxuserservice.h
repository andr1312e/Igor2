#ifndef SERVICES_LINUXUSERSERVICE_H
#define SERVICES_LINUXUSERSERVICE_H

#include <QProcess>
#include <QStandardItemModel>

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
    bool HasCurrentUserAdminPrivileges();

private:

    QStringList GetUserGroups(const QString &userName);

    void PushUserToNameIdList(const QString &name, const QString &userId);

    void RemoveSystemUsersFromAllUsersList(const QStringList &allUsers);

    static bool IsUserSystem(const int &userIdNumber);

private:

    Terminal * const m_terminal;

    QList<QPair<QString, QString>> m_users;

};



#endif // SERVICES_LINUXUSERSERVICE_H
