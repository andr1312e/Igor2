#include "usersettingsrepository.h"

UsersDataWizardRepository::UsersDataWizardRepository(LinuxUserService *service)
    : m_service(service)
{

}

UsersDataWizardRepository::~UsersDataWizardRepository()
{

}

const QString &UsersDataWizardRepository::GetCurrentUserFCS() const
{
    return m_currentUserFCS;
}

const QList<User> &UsersDataWizardRepository::GetUsersList() const
{
    return m_usersList;
}

void UsersDataWizardRepository::GetUsersFromDb(ISqlDatabaseService *m_iSqlDatabaseService)
{
    m_usersList=m_iSqlDatabaseService->GetAllUsers();
    JuxtaposeRepUserIdWithSystemUserId();
    for (const User &user: qAsConst(m_usersList))
    {
        if(user.name==m_service->GetCurrentUserName() && user.userId==m_service->GetCurrentUserId())
        {
            m_currentUserFCS = user.FCS;
        }
    }
}

int UsersDataWizardRepository::GetUserCount() const
{
    return m_usersList.count();
}

bool UsersDataWizardRepository::HasData() const
{
    return !m_usersList.isEmpty();
}

void UsersDataWizardRepository::GetUsersFromXmL(const QDomElement &usersNode)
{
    m_usersList.clear();

    if (QStringLiteral("USERS") == usersNode.tagName()) {
        const QDomNodeList usersTags = usersNode.childNodes();
        int usersCount = usersTags.count();

        for (int i = 0; i < usersCount; ++i) {
            const QDomElement userElement = usersTags.at(i).toElement();

            if (userElement.attribute(QStringLiteral("name")) == m_service->GetCurrentUserName()) {
                m_currentUserFCS = userElement.attribute(QStringLiteral("FCS"));
            }

            User user;
            user.FCS = userElement.attribute(QStringLiteral("FCS"));
            user.name = userElement.attribute(QStringLiteral("name"));
            user.role = GetKorrektUserRole(userElement);
            user.userId = userElement.attribute(QStringLiteral("userId"));

            m_usersList.push_back(user);
        }
        JuxtaposeRepUserIdWithSystemUserId();
    }
}

void UsersDataWizardRepository::SaveUsers(ISqlDatabaseService *m_iSqlDatabaseService, bool needToWriteOnlyAdmin)
{
    if(!m_iSqlDatabaseService->CheckUsersTable())
    {
        m_iSqlDatabaseService->CreateUsersTableIfNotExists();
    }
    else
    {
        m_iSqlDatabaseService->ClearUsersTable();
    }

    if (needToWriteOnlyAdmin) {
        WriteAdminToDatabase(m_iSqlDatabaseService);
    } else {
        if (!FindAndUpdateAdminData()) {
            AppendAdminToCacheList();
        }
        WriteAllUsersToDatabase(m_iSqlDatabaseService);
    }
}

void UsersDataWizardRepository::Clear()
{
    m_usersList.clear();
    m_currentUserFCS.clear();
}

int UsersDataWizardRepository::GetKorrektUserRole(const QDomElement &userDomElement) const
{
    int userRoleId=userDomElement.attribute(QStringLiteral("role")).toInt();
    if(userRoleId<=0 || userRoleId>=Roles.count())
    {
        userRoleId=Roles.count()-1;
    }
    return userRoleId;
}

void UsersDataWizardRepository::JuxtaposeRepUserIdWithSystemUserId()
{

    const QList<QPair<QString, QString>> nameIdList=m_service->GetSystemUsersNamesWithIds();
    for (int i=m_usersList.count()-1; i>=0; i--)
    {
        const QString userName=m_usersList.at(i).name;
        const QString userId=m_usersList.at(i).userId;
        bool finden=false;
        for(const QPair<QString, QString> &item:nameIdList)
        {
            if(userName==item.first && userId==item.second)
            {
                finden=true;
                break;
            }
        }
        if(!finden)
        {
            m_usersList.removeAt(i);
        }
    }
}

bool UsersDataWizardRepository::FindAndUpdateAdminData()
{
    for (User &user:m_usersList) {
        if (m_service->GetCurrentUserName() == user.name) {
            CheckCurrentUserFcsAndRank();
            user.FCS = m_currentUserFCS;
            user.role = Roles.count() - 1;
            user.userId = m_service->GetCurrentUserId();
            return true;
        }
    }

    return false;
}

void UsersDataWizardRepository::AppendAdminToCacheList()
{
    CheckCurrentUserFcsAndRank();
    User user;
    user.FCS = m_currentUserFCS;
    user.role = Roles.count() - 1;
    user.userId = m_service->GetCurrentUserId();
    user.name = m_service->GetCurrentUserName();
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
    CheckCurrentUserFcsAndRank();
    User admin;
    admin.userId=m_service->GetCurrentUserId();
    admin.name=m_service->GetCurrentUserName();
    admin.FCS=m_currentUserFCS;
    admin.role=Roles.count()-1;
    m_iSqlDatabaseService->AppendUserIntoTable(admin);
}

void UsersDataWizardRepository::CheckCurrentUserFcsAndRank()
{
    if(m_currentUserFCS.simplified().isEmpty())
    {
        m_currentUserFCS=QStringLiteral("Администратор");
    }
}
