#include "Services/linuxuserservice.h"
#include "linuxuserservice.h"
#include <QDebug>

LinuxUserService::LinuxUserService(Terminal *terminal)
    : m_terminal(terminal)
    , m_users(nullptr)
{

}

LinuxUserService::~LinuxUserService()
{
    if (m_users!=nullptr)
        delete m_users;
}

void LinuxUserService::getAllUsersInSystem()
{
    //All users -> remove System Users -> PushUserToList
    QString allUsers=m_terminal->getFileText("/etc/passwd", "LinuxUserService::GetUsers");
    QStringList usersList=allUsers.split('\n');
    removeSystemUsersFromAllUsersList(usersList);
}

const QString LinuxUserService::getCurrentUserName()
{
    const QString getCurrentUserNameCommand="id -u -n";
    QString userName=m_terminal->runConsoleCommand(getCurrentUserNameCommand, "LinuxUserService::GetCurrentUserName").remove('\n');
    return userName;
}

const QString LinuxUserService::getCurrentUserId()
{
    const QString getCurrentUserIdCommand="id -u";
    QString userId=m_terminal->runConsoleCommand(getCurrentUserIdCommand, "LinuxUserService::getCurrentUserId").remove('\n');
    return userId;
}

bool LinuxUserService::hasCurrentUserAdminPrivileges()
{
    QStringList listOfCurrentUserGroups=getUserGroups(getCurrentUserName());
    if (listOfCurrentUserGroups.contains("astra-admin"))
    {
        return true;
    }
    else
    {
        return false;
    }
}

QStringList LinuxUserService::getUserGroups(const QString &userName)
{
    const QString getAllCurrentUserGroupCommand="id -Gn "+userName;
    QString userGroups=m_terminal->runConsoleCommand(getAllCurrentUserGroupCommand, "LinuxUserService::getUserGroups");
    userGroups.remove('\n');
    QStringList list=userGroups.split(' ');
    return list;
}

void LinuxUserService::removeSystemUsersFromAllUsersList(QStringList &allUsers)
{
    m_users=new QList<User>;
    QStringList nonSystemUsers;
    bool convertOk=false;
    int index=0;
    for (QStringList::iterator it=allUsers.begin(); it!=allUsers.end(); ++it) {
        QString user=*it;
        int indexOfFirstDots=user.indexOf(':');
        if (indexOfFirstDots!=-1)
        {
            int indexOfSecondDots=user.indexOf(':', indexOfFirstDots+1);
            if (indexOfSecondDots!=-1)
            {
                int indexOfThirdDots=user.indexOf(':', indexOfSecondDots+1);
                if (indexOfThirdDots!=-1)
                {
                    QString userName=user.left(indexOfFirstDots);
                    QString userId=user.mid(indexOfSecondDots+1, indexOfThirdDots-indexOfSecondDots-1);
                    int userIdNumber=userId.toInt(&convertOk, 10);
                    if (convertOk)
                    {
                        if (IsUserSystem(userIdNumber))
                        {
                            pushUserToList(userName, userId);
                            index++;
                        }
                    }
                    else
                    {
                        qFatal("RemoveSystemUsers не смог получить userId");
                    }
                }
                else
                {
                    qFatal("RemoveSystemUsers не смог получить позицию третьих : строка имеет неверный параметр");
                }
            }
            else
            {
                qFatal("RemoveSystemUsers не смог получить позицию вторых : строка имеет неверный параметр");
            }
        }
        else
        {

        }
    }
}

void LinuxUserService::pushUserToList(const QString &name, const QString &userId)
{
    User user;
    user.name=name;
    user.userId=userId;
    m_users->append(user);
}

bool LinuxUserService::IsUserSystem(int &userIdNumber)
{
    if (userIdNumber>999&&userIdNumber<29991)
    {
        return true;
    }
    else
    {
        return false;
    }
}
