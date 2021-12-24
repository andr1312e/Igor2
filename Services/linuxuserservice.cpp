#include "Services/linuxuserservice.h"
#include "linuxuserservice.h"
#include <QDebug>

LinuxUserService::LinuxUserService(Terminal *terminal)
    : m_terminal(terminal)
{

}

LinuxUserService::~LinuxUserService()
{

}

void LinuxUserService::GetAllUsersWithIdInSystem()
{
    const QStringList allUsersList=m_terminal->GetAllUsersList("LinuxUserService::getAllUsersInSystem");
    RemoveSystemUsersFromAllUsersList(allUsersList);
}

//QList<QPair<QString, QString> >* LinuxUserService::GetAllUsersNamesWithIdInSystem()
//{
//    QList<QPair<QString, QString> > usersNameUserIdDictonary;
//    const QString getAllUsersWithIdCommand= " awk -F: '{print $1 \":\" $3}' /etc/passwd";
//    QString userListOneString=m_terminal->RunConsoleCommand(getAllUsersWithIdCommand, "LinuxUserService::GetAllUsersNamesWithIdInSystem");
//    QStringList allNameIdList=userListOneString.split('\n');
//    allNameIdList.removeLast();
//    QStringList nameIdList=RemoveSystemUsers(allNameIdList);
//    for(const QString &nameId:nameIdList)
//    {
//        int pos=nameId.indexOf(":");
//        QString id=nameId.mid(pos+1);
//        QString name=nameId.left(pos);
//        QPair<QString, QString> nameAndId;
//        nameAndId.first=name;
//        nameAndId.second=id;
//        usersNameUserIdDictonary.push_back(nameAndId);
//    }
//    return usersNameUserIdDictonary;
//}

const QString LinuxUserService::GetCurrentUserName()
{
    const QString getCurrentUserNameCommand="id -u -n";
    QString userName=m_terminal->RunConsoleCommand(getCurrentUserNameCommand, "LinuxUserService::GetCurrentUserName").remove('\n');
    return userName;
}

const QString LinuxUserService::GetCurrentUserId()
{
    const QString getCurrentUserIdCommand="id -u";
    QString userId=m_terminal->RunConsoleCommand(getCurrentUserIdCommand, "LinuxUserService::getCurrentUserId").remove('\n');
    return userId;
}

bool LinuxUserService::HasCurrentUserAdminPrivileges()
{
    QStringList listOfCurrentUserGroups=GetUserGroups(GetCurrentUserName());
    if (listOfCurrentUserGroups.contains("astra-admin") || listOfCurrentUserGroups.contains("root"))
    {
        return true;
    }
    else
    {
        return false;
    }
}

QStringList LinuxUserService::GetUserGroups(const QString &userName)
{
    const QString getAllCurrentUserGroupCommand="id -Gn "+userName;
    QString userGroups=m_terminal->RunConsoleCommand(getAllCurrentUserGroupCommand, "LinuxUserService::getUserGroups");
    userGroups.remove('\n');
    QStringList list=userGroups.split(' ');
    return list;
}

void LinuxUserService::RemoveSystemUsersFromAllUsersList(const QStringList &allUsers)
{
    QStringList nonSystemUsers;
    bool convertOk=false;
    int index=0;
    for (const QString &user :allUsers)
    {
        int indexOfFirstDots=user.indexOf(':');
        if (indexOfFirstDots!=-1)
        {
                    QString userName=user.left(indexOfFirstDots);
                    QString userId=user.mid(indexOfFirstDots+1);
                    int userIdNumber=userId.toInt(&convertOk, 10);
                    if (convertOk)
                    {
                        if (IsUserSystem(userIdNumber))
                        {
                            PushUserToNameIdList(userName, userId);
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
                qFatal("RemoveSystemUsers не смог получить позицию вторых : строка имеет неверный параметр");
            }
        }
}

void LinuxUserService::PushUserToNameIdList(const QString &name, const QString &userId)
{
    m_users.push_back(QPair<QString, QString>(name, userId));
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
