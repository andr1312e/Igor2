#include "usersettingsrepository.h"

UsersDataWizardRepository::UsersDataWizardRepository(const QString &currentUserName, const QString &currentUserId, LinuxUserService *service)
    : m_service(service)
    , m_curerntUserName(currentUserName)
    , m_curerntUserId(currentUserId)
    , m_currentUserFCS()
    , m_currenUserRank()
{

}

UsersDataWizardRepository::~UsersDataWizardRepository()
{

}

const QString &UsersDataWizardRepository::GetCurrentUserName() const
{
    return m_curerntUserName;
}

QString &UsersDataWizardRepository::GetCurrentUserFCS()
{
    return m_currentUserFCS;
}

QString &UsersDataWizardRepository::GetCurrentUserRank()
{
    return m_currenUserRank;
}

QList<User> &UsersDataWizardRepository::GetUsersList()
{
    return m_usersList;
}

void UsersDataWizardRepository::GetExsistsUsersListFromDb(ISqlDatabaseService *m_iSqlDatabaseService)
{
    QList<User> users=m_iSqlDatabaseService->GetAllUsers();
    m_usersList=users;
}

int UsersDataWizardRepository::GetUserCount() const
{
    return m_usersList.count();
}

bool UsersDataWizardRepository::HasData() const
{
    return !m_usersList.isEmpty();
}

void UsersDataWizardRepository::SetUsersFromBackup(const QDomElement &usersNode)
{
    m_usersList.clear();

    if (usersNode.tagName() == "USERS") {
        const QDomNodeList usersTags = usersNode.childNodes();
        int usersCount = usersTags.count();

        for (int i = 0; i < usersCount; ++i) {
            const QDomElement userElement = usersTags.at(i).toElement();

            if (userElement.attribute("name") == m_curerntUserName) {
                m_currentUserFCS = userElement.attribute("FCS");
                m_currenUserRank = userElement.attribute("rank");
            }

            User user;
            user.FCS = userElement.attribute("FCS");
            user.name = userElement.attribute("name");
            user.rank = userElement.attribute("rank");
            user.role = GetKorrektUserRole(userElement);
            user.userId = userElement.attribute("userId");

            m_usersList.push_back(user);
        }
        JuxtaposeUserIdAndUserNameWithSystemsData();
    }
}

void UsersDataWizardRepository::WriteUserRepositoryToDB(ISqlDatabaseService *m_iSqlDatabaseService, bool needToWriteOnlyAdmin)
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

int UsersDataWizardRepository::GetKorrektUserRole(const QDomElement &userDomElement)
{
    int userRoleId=userDomElement.attribute("role").toInt();
    if(userRoleId<=0 || userRoleId>=Roles.count())
    {
        userRoleId=Roles.count()-1;
    }
    return userRoleId;
}

void UsersDataWizardRepository::JuxtaposeUserIdAndUserNameWithSystemsData()
{
    auto nameIdList(m_service->GetSystemUsersNamesWithIdsList());
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
        if (user.name == m_curerntUserName) {
            CheckCurrentUserFcsAndRank();
            user.FCS = m_currentUserFCS;
            user.rank = m_currenUserRank;
            user.role = Roles.count() - 1;
            user.userId = m_curerntUserId;
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
    user.rank = m_currenUserRank;
    user.role = Roles.count() - 1;
    user.userId = m_curerntUserId;
    user.name = m_curerntUserName;
    m_usersList.append(user);
}

void UsersDataWizardRepository::WriteAllUsersToDatabase(ISqlDatabaseService *m_iSqlDatabaseService)
{
    for (User &user : m_usersList)
    {
        m_iSqlDatabaseService->AppendUserIntoTable(user);
    }
}

void UsersDataWizardRepository::WriteAdminToDatabase(ISqlDatabaseService *m_iSqlDatabaseService)
{
    CheckCurrentUserFcsAndRank();
    User admin;
    admin.userId=m_curerntUserId;
    admin.name=m_curerntUserName;
    admin.FCS=m_currentUserFCS;
    admin.rank=m_currenUserRank;
    admin.role=Roles.count()-1;
    m_iSqlDatabaseService->AppendUserIntoTable(admin);
}

void UsersDataWizardRepository::CheckCurrentUserFcsAndRank()
{
    if(m_currentUserFCS.simplified()=="")
    {
        m_currentUserFCS="Администратор";
    }
    if(m_currenUserRank.isEmpty())
    {
        m_currenUserRank=Ranks.last();
    }
}
