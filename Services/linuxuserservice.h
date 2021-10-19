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

    void getAllUsersInSystem();

    QList<User>* getUsersList(){ return m_users;}

    Terminal* getTerminal() {return m_terminal;}

public:

    const QString getCurrentUserName();

    const QString getCurrentUserId();

    bool hasCurrentUserAdminPrivileges();

private:

    QStringList getUserGroups(const QString &userName);

    void pushUserToList(const QString &name, const QString &userId);

    void removeSystemUsersFromAllUsersList(QStringList &allUsers);

    static bool IsUserSystem(int &userIdNumber);

private:

    Terminal *m_terminal;

    QList<User> *m_users;

};



#endif // SERVICES_LINUXUSERSERVICE_H
