#include "usersettingsrepository.h"

UsersDataWizardRepository::UsersDataWizardRepository(LinuxUserService *service)
    : m_service(service)
    , m_curerntUserName(m_service->GetCurrentUserName())
    , m_curerntUserId(m_service->GetCurrentUserName())
    , m_currentUserFCS("")
    , m_currenUserRank("")
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

void UsersDataWizardRepository::GetFCSAndRolesFromXml(const QDomElement &usersNode)
{
    m_usersList.clear();

    if (usersNode.tagName() == "USERS") {
        QDomNodeList usersTags = usersNode.childNodes();
        int usersCount = usersTags.count();

        for (int i = 0; i < usersCount; ++i) {
            QDomElement userElement = usersTags.at(i).toElement();

            if (userElement.attribute("name") == m_curerntUserName) {
                m_currentUserFCS = userElement.attribute("FCS");
                m_currenUserRank = userElement.attribute("rank");
            }

            User user;
            user.FCS = userElement.attribute("FCS");
            user.name = userElement.attribute("name");
            user.rank = userElement.attribute("rank");
            user.role = userElement.attribute("role").toInt();
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

void UsersDataWizardRepository::JuxtaposeUserIdAndUserNameWithSystemsData()
{

    QList<QPair<QString, QString> > nameIdList=m_service->GetSystemUsersNamesWithList();
    for (int i=m_usersList.count()-1; i>=0; i--)
    {
        QString userName=m_usersList.at(i).name;
        QString userId=m_usersList.at(i).userId;
        bool findet=false;
        for(const QPair<QString, QString> &item:nameIdList)
        {
            if(userName==item.first && userId==item.second)
            {
                findet=true;
                break;
            }
        }
        if(!findet)
        {
            m_usersList.removeAt(i);
        }
    }
}

bool UsersDataWizardRepository::FindAndUpdateAdminData()
{
    for (User &user:m_usersList) {
        if (user.name == m_curerntUserName) {
            user.FCS = m_currentUserFCS;
            user.rank = m_currenUserRank;
            return true;
        }
    }

    return false;
}

void UsersDataWizardRepository::AppendAdminToCacheList()
{
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
    User admin;
    admin.userId=m_curerntUserId;
    admin.name=m_curerntUserName;
    admin.FCS=m_currentUserFCS;
    admin.rank=m_currenUserRank;
    admin.role=Roles.count()-1;
    m_iSqlDatabaseService->AppendUserIntoTable(admin);
}
