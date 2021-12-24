#include "usermodel.h"

UserModel::UserModel(ISqlDatabaseService *databaseService, LinuxUserService *userService, QObject *parent)
    : QObject(parent)
    , m_model(new QStandardItemModel())
    , m_databaseService(databaseService)
    , m_linuxUserService(userService)
    , m_currentRoleUsers(new QStringList())
{
    OnDataChanged();
}

UserModel::~UserModel()
{
    delete m_model;
    delete m_currentRoleUsers;
}

void UserModel::AddUserToModel(const QString &userId, const QString &FCS, const QString &rank, const int &role)
{
    for (User &user  : m_users)
    {
        if (user.userId == userId)
        {
            user.FCS = FCS;
            user.rank = rank;
            user.role = role;
            user.hasData = true;
            m_databaseService->AppendUserIntoTable(user);
            SetImageToUser(user);
            FillModelByList();
            return;
        }
    }

    qFatal("нет юзера");
}

void UserModel::DeleteUser(const QString &userId)
{
    for (User &user  : m_users)
    {
        if (user.userId == userId)
        {
            user.FCS.clear();
            user.rank.clear();
            user.role=-1;
            user.hasData = false;
//            m_databaseService->RemoveUserIntoTable(user);
            SetImageToUser(user);
            FillModelByList();
            return;
        }
    }

    qFatal("нет юзера");
}

QStandardItemModel *UserModel::GetModel()
{
    return m_model;
}

void UserModel::OnDataChanged()
{
    QList<QPair<QString, QString>> namesAndIdsList=m_linuxUserService->GetSystemUsersNamesWithList();
    m_users=FillListByUserService(namesAndIdsList);
    QList<User> databaseUsers=FillListByDatabaseService();
    for (User &systemUser : m_users)
    {
        for (const User &databaseUser: databaseUsers)
        {
            if(systemUser.userId==databaseUser.userId && systemUser.name==databaseUser.name)
            {
                systemUser.FCS=databaseUser.FCS;
                systemUser.rank=databaseUser.rank;
                systemUser.role=databaseUser.role;
                systemUser.hasData=true;
                SetImageToUser(systemUser);
            }
        }
    }
    FillModelByList();
}

void UserModel::ClearList()
{
    m_users.clear();
}

QList<User> UserModel::FillListByUserService(const QList<QPair<QString, QString>> &namesAndIdsList) const
{
    QList<User> usersInSystem;
    for (const QPair<QString, QString> &userNameAndId : namesAndIdsList)
    {
        User user;
        user.name=userNameAndId.first;
        user.userId=userNameAndId.second;
        user.m_image=":/images/0.jpg";
        user.hasData=false;
        usersInSystem.append(user);
    }
    return  usersInSystem;
}

QStringList *UserModel::GetUsersSystemNamesByRole(const QString &role)
{
    m_currentRoleUsers->clear();

    for (User &user:m_users) {
        if (user.role == role) {
            m_currentRoleUsers->append(user.name);
        }
    }

    return m_currentRoleUsers;
}

QList<User> UserModel::FillListByDatabaseService()
{
    return m_databaseService->GetAllUsers();
}

void UserModel::FillModelByList()
{
    m_model->clear();

    for (const User & user :m_users) {
        QStandardItem *item = new QStandardItem();
        item->setData(QVariant::fromValue(user), Qt::UserRole + 1);
        m_model->appendRow(item);
    }
}

void UserModel::SetImageToUser(User &user)
{
    if (user.role == 0) {
        user.m_image = ":/images/0.jpg";
    } else {
        if (user.role == 1) {
            user.m_image = ":/images/1.jpg";
        } else {
            if (user.role == 2) {
                user.m_image = ":/images/2.jpg";
            } else {
                user.m_image = ":/images/3.jpg";
            }
        }
    }
}


