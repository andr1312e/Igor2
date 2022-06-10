#include "usersettingsrepository.h"

UsersDataWizardRepository::UsersDataWizardRepository(LinuxUserService *service)
    : m_service(service)
{

}

UsersDataWizardRepository::~UsersDataWizardRepository()
{

}

bool UsersDataWizardRepository::HasData() const noexcept
{
    return !m_usersList.isEmpty();
}

const QString &UsersDataWizardRepository::GetCurrentUserFCS() const
{
    return m_currentUserFCS;
}

const QList<User> &UsersDataWizardRepository::GetUsersList() const
{
    return m_usersList;
}

int UsersDataWizardRepository::GetUserCount() const noexcept
{
    return m_usersList.count();
}

void UsersDataWizardRepository::GetUsersFromDb(ISqlDatabaseService *m_iSqlDatabaseService)
{
    m_usersList = m_iSqlDatabaseService->GetAllUsers();
    JuxtaposeRepUserIdWithSystemUserId();
    RemoveDublicates();
    if (HasAdmin())
    {
        UpdateAdminRole();
    }
    else
    {
        AppendAdminToCacheList();
    }
}

void UsersDataWizardRepository::GetUsersFromXmL(const QDomElement &usersNode)
{
    m_usersList.clear();
    if (QLatin1Literal("USERS") == usersNode.tagName())
    {
        const QDomNodeList usersTags = usersNode.childNodes();
        const int usersCount = usersTags.count();
        for (int i = 0; i < usersCount; ++i)
        {
            const QDomElement userElement = usersTags.at(i).toElement();
            if (UserHasAllAtributes(userElement))
            {
                qDebug() << userElement.tagName();
                if (FindCurrentUserName(userElement))
                {
                    Log4QtInfo(Q_FUNC_INFO + QStringLiteral("Найден текущий пользователь"));
                    ParceCurrentUser(userElement);
                }
                else
                {
                    ParceUser(userElement);
                }
            }
            else
            {
                Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Файл настроек. Пользователь не имеет правильные параметры . По счету (c 0): ") + QString::number(i));
            }
        }
    }
    else
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Файл настроек. Пользователи: неверный тэг элемента пользователи") + QString::number(m_usersList.count()));
    }
    JuxtaposeRepUserIdWithSystemUserId();
    RemoveDublicates();
    GetCurrentUserFCS();
    if (!HasAdmin())
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Админа нет добавляем "));
        AppendAdminToCacheList();
    }
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Файл настроек. Пользователи: распарсены. Колличество: ") + QString::number(m_usersList.count()));
}

void UsersDataWizardRepository::UpdateAdminFCS(const QString &fcs)
{
    m_currentUserFCS = fcs;
}

void UsersDataWizardRepository::UpdateUser(const QString &userId, const QString &userName, const QString &fcs)
{
    for (User &user : m_usersList)
    {
        if (user.GetUserId() == userId && user.GetUserName() == userName)
        {
            user.SetUserData(fcs);
            return;
        }
    }
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Пытались изменить пользователя с данными ") + userId + ':' + userName + QStringLiteral(" но его нет в базе"));
}

void UsersDataWizardRepository::UpdateUser(const QString &userId, const QString &userName, int roleIndex)
{
    for (User &user : m_usersList)
    {
        if (user.GetUserId() == userId && user.GetUserName() == userName)
        {
            user.SetUserData(roleIndex);
            return;
        }
    }
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Пытались изменить пользователя с данными ") + userId + ':' + userName + QStringLiteral(" но его нет в базе"));
}

void UsersDataWizardRepository::SaveUsers(ISqlDatabaseService *m_iSqlDatabaseService, bool needToWriteOnlyAdmin)
{
    if (!m_iSqlDatabaseService->CheckUsersTable())
    {
        m_iSqlDatabaseService->CreateUsersTableIfNotExists();
    }
    else
    {
        m_iSqlDatabaseService->ClearUsersTable();
    }
    if (needToWriteOnlyAdmin)
    {
        WriteAdminToDatabase(m_iSqlDatabaseService);
    }
    else
    {
        WriteAllUsersToDatabase(m_iSqlDatabaseService);
    }
}

void UsersDataWizardRepository::Clear()
{
    m_usersList.clear();
    m_currentUserFCS.clear();
}

bool UsersDataWizardRepository::UserHasAllAtributes(const QDomElement &userDomElement) const noexcept
{
    return (userDomElement.hasAttribute(QLatin1Literal("userId"))
            && userDomElement.hasAttribute(QLatin1Literal("name"))
            && userDomElement.hasAttribute(QLatin1Literal("FCS"))
            && userDomElement.hasAttribute(QLatin1Literal("role")));
}

bool UsersDataWizardRepository::FindCurrentUserName(const QDomElement &userDomElement) const noexcept
{
    return (userDomElement.attribute(QLatin1Literal("name")) == m_service->GetCurrentUserName());//1 проверка
}

void UsersDataWizardRepository::ParceCurrentUser(const QDomElement &currentUserDomElement) noexcept
{
    QString userId = currentUserDomElement.attribute(QLatin1Literal("userId"));
    if (userId != m_service->GetCurrentUserId())//2 проверка
    {
        userId = m_service->GetCurrentUserId();
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral("Пользователь с именем был найден, но ид пользователя не совпадает, меняем ид"));
    }
    QString fcs = currentUserDomElement.attribute(QLatin1Literal("FCS"));
    if (fcs.isEmpty())
    {
        fcs = QStringLiteral("Пользователь ") + m_service->GetCurrentUserName();
    }
    const User currentUserAdmin(m_service->GetCurrentUserId(), m_service->GetCurrentUserName(), fcs, Roles.count() - 1);
    m_usersList.push_back(currentUserAdmin);
}

void UsersDataWizardRepository::ParceUser(const QDomElement &userDomElement) noexcept
{
    QString fcs = userDomElement.attribute(QLatin1Literal("FCS"));
    if (fcs.isEmpty())
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral("Имя пользователя пусто. Пользователь: ") + userDomElement.attribute(QLatin1Literal("name")));
        fcs = QStringLiteral("Пользователь ") + userDomElement.attribute(QLatin1Literal("name"));
    }
    const User user(userDomElement.attribute(QLatin1Literal("userId")), userDomElement.attribute(QLatin1Literal("name")), fcs, GetKorrektUserRole(userDomElement));
    m_usersList.push_back(user);
}

bool UsersDataWizardRepository::HasAdmin() noexcept
{
    for (const User &user : qAsConst(m_usersList))
    {
        if (user.GetUserName() == m_service->GetCurrentUserName() && user.GetUserId() == m_service->GetCurrentUserId())
        {
            return true;
        }
    }
    return false;
}

void UsersDataWizardRepository::UpdateAdminRole() noexcept
{
    for (User &user : m_usersList)
    {
        if (user.GetUserName() == m_service->GetCurrentUserName() && user.GetUserId() == m_service->GetCurrentUserId())
        {
            user.SetUserData(Roles.count() - 1);
            m_currentUserFCS = user.GetUserFCS();
        }
    }
}

void UsersDataWizardRepository::GetCurrentUserFCS()
{
    for (const User &user : qAsConst(m_usersList))
    {
        if (user.GetUserName() == m_service->GetCurrentUserName() && user.GetUserId() == m_service->GetCurrentUserId())
        {
            m_currentUserFCS = user.GetUserFCS();
            return;
        }
    }
    Q_UNREACHABLE();
}

int UsersDataWizardRepository::GetKorrektUserRole(const QDomElement &userDomElement) const
{
    bool canConvert;
    int userRoleId = userDomElement.attribute(QLatin1Literal("role")).toInt(&canConvert);
    if (!canConvert)
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Ид у пользователя записано не число. Проверьте символы "));
        userRoleId = 0;
    }
    else
    {
        if (!User::RoleIsValid(userRoleId))
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Ид у пользователя не верен. Он либо меньше 0, или больше максимального ") + QString::number(Roles.count() - 1));
            userRoleId = 0;
        }
    }
    return userRoleId;
}

void UsersDataWizardRepository::JuxtaposeRepUserIdWithSystemUserId()
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Совмещаем ид и имена пользователей в базе, и с файла "));
    const QList<QPair<QString, QString>> nameIdList = m_service->GetSystemUsersNamesWithIds();
    for (int i = m_usersList.count() - 1; i >= 0; i--)
    {
        const QString userName = m_usersList.at(i).GetUserName();
        const QString userId = m_usersList.at(i).GetUserId();
        bool finden = false;
        for (const QPair<QString, QString> &item : nameIdList)
        {
            if (userName == item.first && userId == item.second)
            {
                finden = true;
                break;
            }
        }
        if (!finden)
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Пользователь с ид ") + m_usersList.at(i).GetUserId() + QStringLiteral(" и с имененм ") + m_usersList.at(i).GetUserName() + QStringLiteral(" в системе не содержится"));
            m_usersList.removeAt(i);
        }
    }
}

void UsersDataWizardRepository::RemoveDublicates()
{
    std::sort(m_usersList.begin(), m_usersList.end());
    QList<User>::iterator indexToStartDelete = std::unique(m_usersList.begin(), m_usersList.end());
    m_usersList.erase(indexToStartDelete, m_usersList.end());
}

void UsersDataWizardRepository::AppendAdminToCacheList()
{
    m_currentUserFCS = QStringLiteral("Пользователь ") + m_service->GetCurrentUserName();
    const User user(m_service->GetCurrentUserId(), m_service->GetCurrentUserName(), m_currentUserFCS, Roles.count() - 1);
    m_usersList.append(user);
}

void UsersDataWizardRepository::WriteAllUsersToDatabase(ISqlDatabaseService *m_iSqlDatabaseService)
{
    for (const User &user : qAsConst(m_usersList))
    {
        m_iSqlDatabaseService->AppendUserIntoTable(user);
    }
}

void UsersDataWizardRepository::WriteAdminToDatabase(ISqlDatabaseService *m_iSqlDatabaseService)
{
    const User admin(m_service->GetCurrentUserId(), m_service->GetCurrentUserName(), m_currentUserFCS, Roles.count() - 1);
    m_iSqlDatabaseService->AppendUserIntoTable(admin);
}
