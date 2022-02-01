#include "linuxuserservice.h"

LinuxUserService::LinuxUserService()
    : m_terminal(Terminal::GetTerminal())
{

}

LinuxUserService::~LinuxUserService()
{
}

void LinuxUserService::GetAllUsersWithIdInSystem()
{
    const QStringList allUsersList=m_terminal->GetAllUsersList(Q_FUNC_INFO);
    Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Получаем список всех пользователей: ")+ allUsersList.join(' '));
    RemoveSystemUsersFromAllUsersList(allUsersList);
}

const QString LinuxUserService::GetCurrentUserName()
{
    const QString getCurrentUserNameCommand="id -u -n";
    const QString userName=m_terminal->RunConsoleCommand(getCurrentUserNameCommand, Q_FUNC_INFO).remove('\n');
    Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Получаем имя пользователя: ")+userName);
    return userName;
}

const QString LinuxUserService::GetCurrentUserId()
{
    const QString getCurrentUserIdCommand="id -u";
    const QString userId=m_terminal->RunConsoleCommand(getCurrentUserIdCommand, Q_FUNC_INFO).remove('\n');
    Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Получаем ид пользователя: ")+ userId);
    return userId;
}

bool LinuxUserService::HasUserAdminPrivileges(const QString &userName)
{
    const QStringList listOfCurrentUserGroups(GetUserGroups(userName));
    if (listOfCurrentUserGroups.contains(QStringLiteral("astra-admin")) || listOfCurrentUserGroups.contains(QStringLiteral("root")))
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
    QString userGroups=m_terminal->RunConsoleCommand(getAllCurrentUserGroupCommand, Q_FUNC_INFO);
    userGroups.remove('\n');
    const QStringList list=userGroups.split(' ');
    Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Получаем список групп пользователя: ")+ userName + QStringLiteral(" список групп:  ")+ userGroups);
    return list;
}

void LinuxUserService::RemoveSystemUsersFromAllUsersList(const QStringList &allUsers)
{
    for (const QString &user :allUsers)
    {
        const int indexOfFirstDots=user.indexOf(':');
        if (-1==indexOfFirstDots)
        {
            qFatal("%s", QString(Q_FUNC_INFO+QStringLiteral("RemoveSystemUsers не смог получить позицию вторых : строка имеет неверный параметр")).toUtf8().constData());
        }
        else
        {
            bool convertOk;
            const QString userName=user.left(indexOfFirstDots);
            const QString userId=user.mid(indexOfFirstDots+1);
            const int userIdNumber=userId.toInt(&convertOk, 10);
            if (convertOk)
            {
                if (IsUserSystem(userIdNumber))
                {
                    PushUserToNameIdList(userName, userId);
                }
            }
            else
            {
                qFatal("%s", QString(Q_FUNC_INFO+QStringLiteral("RemoveSystemUsers не смог получить userId")).toUtf8().constData());
            }
        }
    }
    //logs
    QString userNames;
    for (const QPair<QString, QString> &userData: qAsConst(m_users))
    {
        userNames.append(userData.first+QChar(' '));
    }
    Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Cписок не системных пользователей: ")+ userNames);
}

void LinuxUserService::PushUserToNameIdList(const QString &name, const QString &userId)
{
    m_users.append(qMakePair(name, userId));
}

bool LinuxUserService::IsUserSystem(const int &userIdNumber) const
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
