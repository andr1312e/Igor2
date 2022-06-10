#include "Services/linuxuserservice.h"

#include <QDebug>

LinuxUserService::LinuxUserService()
    : m_terminal(Terminal::GetTerminal())
{

}

LinuxUserService::~LinuxUserService()
{

}

void LinuxUserService::GetAllUsersWithIdInSystem()
{
    const QStringList allUsersList = m_terminal->GetAllUsersList(Q_FUNC_INFO);
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Получаем список всех пользователей: ") + allUsersList.join(' '));
    RemoveSystemUsersFromAllUsersList(allUsersList);
}

const QList<QPair<QString, QString> > &LinuxUserService::GetSystemUsersNamesWithIds()
{
    return m_users;
}

const QString LinuxUserService::GetCurrentUserName()
{
    if (m_currentUserName.isEmpty())
    {
        const QString getCurrentUserNameCommand(QLatin1Literal("id -u -n"));
        m_currentUserName = m_terminal->RunConsoleCommandSync(getCurrentUserNameCommand, Q_FUNC_INFO).remove('\n');
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Получаем имя пользователя: ") + m_currentUserName);
    }
    return m_currentUserName;
}

const QString LinuxUserService::GetCurrentUserId()
{
    if (m_currentUserId.isEmpty())
    {
        const QString getCurrentUserIdCommand(QLatin1Literal("id -u"));
        m_currentUserId = m_terminal->RunConsoleCommandSync(getCurrentUserIdCommand, Q_FUNC_INFO).remove('\n');
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Получаем ид пользователя: ") + m_currentUserId);
    }
    return m_currentUserId;
}

bool LinuxUserService::HasCurrentUserAdminPrivileges()
{
    const QStringList listOfCurrentUserGroups = GetUserGroups(GetCurrentUserName());
    if (listOfCurrentUserGroups.contains(QLatin1Literal("astra-admin")) || listOfCurrentUserGroups.contains(QLatin1Literal("root")))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void LinuxUserService::OpenFlyAdminSmc()
{
    //там 2 разных процесса, костыль астры
    const QString astraUserControlNameService = QLatin1Literal("fly-admin-smc");
    const QString astraUserControlNameView = QLatin1Literal("fly-admin-gmc");
    const QStringList processList = m_terminal->GetAllProcessList(Q_FUNC_INFO);
    if (processList.contains(astraUserControlNameService))
    {
        m_terminal->KillProcess(astraUserControlNameService, Q_FUNC_INFO);
    }
    if (processList.contains(astraUserControlNameView))
    {
        m_terminal->KillProcess(astraUserControlNameView, Q_FUNC_INFO);
    }
    m_terminal->RunConsoleCommandAsync(QLatin1Literal("sudo ") + astraUserControlNameService);
}

QStringList LinuxUserService::GetUserGroups(const QString &userName)
{
    const QString getAllCurrentUserGroupCommand = QStringLiteral("id -Gn ") + userName;
    QString userGroups = m_terminal->RunConsoleCommandSync(getAllCurrentUserGroupCommand, Q_FUNC_INFO);
    userGroups.remove('\n');
    const QStringList list = userGroups.split(' ');
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Получаем список групп пользователя: ") + userName + QStringLiteral(" список групп:  ") + userGroups);
    return list;
}

void LinuxUserService::RemoveSystemUsersFromAllUsersList(const QStringList &allUsers)
{
    QStringList nonSystemUsers;
    int index = 0;
    for (const QString &user : allUsers)
    {
        const int indexOfFirstDots = user.indexOf(':');
        if (-1 == indexOfFirstDots)
        {
            qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral("RemoveSystemUsers не смог получить позицию вторых : строка имеет неверный параметр")).toUtf8().constData());
        }
        else
        {
            const QString userName = user.left(indexOfFirstDots);
            const QString userId = user.mid(indexOfFirstDots + 1);
            bool convertOk = false;
            const int userIdNumber = userId.toInt(&convertOk, 10);
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
                qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral("RemoveSystemUsers не смог получить userId")).toUtf8().constData());
            }
        }
    }
}

void LinuxUserService::PushUserToNameIdList(const QString &name, const QString &userId)
{
    m_users.append(qMakePair(name, userId));
}

bool LinuxUserService::IsUserSystem(const int &userIdNumber) const
{
    if (userIdNumber > 999 && userIdNumber < 29991)
    {
        return true;
    }
    else
    {
        return false;
    }
}
