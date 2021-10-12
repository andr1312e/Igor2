#include "usermodel.h"

UserModel::UserModel(LinuxUserService *linuxUserService, DatabaseService *databaseService, QObject *parent)
    : QObject(parent)
    , m_model(new QStandardItemModel())
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

void UserModel::addUserToModel(const QString &userId, const QString &FCS, const QString &rank, const QString &role)
{
    for(QList<User>::iterator it=m_users->begin(); it!=m_users->end(); ++it)
    {
        if (it->userId==userId)
        {
            it->FCS=FCS;
            it->rank=rank;
            it->role=role;
            it->hasData=true;
            setUserImage(*it);
            FillModelByList();
            return;
        }
    }
    qFatal("нет юзера");
}

void UserModel::deleteUserFromModel(const QString &userId)
{
    for(QList<User>::iterator it=m_users->begin(); it!=m_users->end(); ++it)
    {
        if (it->userId==userId)
        {
            it->FCS.clear();
            it->rank.clear();
            it->role.clear();
            it->hasData=false;
            setUserImage(*it);
            FillModelByList();
            return;
        }
    }
    qFatal("нет юзера");
}

void UserModel::dataChanged()
{
    FillListByLinuxUserService();
    FillListByDatabaseService();
    FillModelByList();
}

QStringList* UserModel::getUsersNamesByRole(const QString &role)
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
        setUserImage(*it);
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

void UserModel::setUserImage(User &user)
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


