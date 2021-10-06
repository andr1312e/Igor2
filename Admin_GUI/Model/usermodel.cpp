#include "usermodel.h"

UserModel::UserModel(LinuxUserService *linuxUserService, DatabaseService *databaseService)
    : m_model(new QStandardItemModel())
    , m_databaseService(databaseService)
    , m_linuxUserService(linuxUserService)
    , m_currentRoleUsers(new QStringList())
{
    dataChanged();
}

UserModel::~UserModel()
{
    m_databaseService->writeToFile(m_users);
    delete m_model;
    delete m_currentRoleUsers;
}

void UserModel::dataChanged()
{
    FillListByLinuxUserService();
    FillListByDatabaseService();
    FillModelByList();
}

QStringList* UserModel::getUserNamesByRole(const QString &role)
{
    m_currentRoleUsers->clear();
    for (QList<User>::iterator it=m_users->begin(); it!=m_users->end(); ++it)
    {
        if (it->role==role)
        {
            m_currentRoleUsers->append(it->name);
        }
    }
    return m_currentRoleUsers;
}

void UserModel::FillListByLinuxUserService()
{
    m_users=m_linuxUserService->getUsersList();
}

void UserModel::FillListByDatabaseService()
{
    for (QList<User>::iterator it=m_users->begin(); it!=m_users->end(); ++it)
    {
        m_databaseService->readUserFromFile(*it);
        getUserImageToList(*it);
    }
}

void UserModel::FillModelByList()
{
    m_model->clear();
    for (QList<User>::iterator it=m_users->begin(); it!=m_users->end(); ++it)
    {
        QStandardItem *item=new QStandardItem();
        User user=*it;
        item->setData(QVariant::fromValue(user), Qt::UserRole+1);
        m_model->appendRow(item);
    }
}

void UserModel::getUserImageToList(User &user)
{
    if (user.role==Roles.at(0))
    {
        user.m_image=":/images/0.jpg";
    }
    else
    {
        if (user.role==Roles.at(1))
        {
            user.m_image=":/images/1.jpg";
        }
        else
        {
            if (user.role==Roles.at(2))
            {
                user.m_image=":/images/2.jpg";
            }
            else
            {
                user.m_image=":/images/3.jpg";
            }
        }
    }
}


