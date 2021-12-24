#ifndef SERVICES_LINUXUSERSERVICE_H
#define SERVICES_LINUXUSERSERVICE_H

#include <QProcess>
#include <QStandardItemModel>

#include "Services/Terminals/terminal.h"
#include "Structs/userstruct.h"

class LinuxUserService
{
public:

    LinuxUserService(Terminal *terminal);

    ~LinuxUserService();

    void GetAllUsersWithIdInSystem();

    QList<QPair<QString, QString>>& GetSystemUsersNamesWithList(){ return m_users;}

    Terminal* GetTerminal() {return m_terminal;}

public:

    const QString GetCurrentUserName();

    const QString GetCurrentUserId();

    bool HasCurrentUserAdminPrivileges();

private:

    QStringList GetUserGroups(const QString &userName);

    void PushUserToNameIdList(const QString &name, const QString &userId);

    void RemoveSystemUsersFromAllUsersList(const QStringList &allUsers);

    static bool IsUserSystem(int &userIdNumber);

private:

    Terminal *m_terminal;

    QList<QPair<QString, QString>> m_users;

};



#endif // SERVICES_LINUXUSERSERVICE_H
