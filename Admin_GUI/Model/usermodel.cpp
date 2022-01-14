#include "usermodel.h"

UserModel::UserModel(ISqlDatabaseService *databaseService, LinuxUserService *userService)
    : m_model(new QStandardItemModel())
    , m_databaseService(databaseService)
    , m_linuxUserService(userService)
{
    DataChanged();
}

UserModel::~UserModel()
{
    delete m_model;
}

int UserModel::GetRoleIdByUserId(const QString &userId) const
{
    for (const User &user : qAsConst(m_users))
    {
        if (userId==user.userId)
        {
            return user.role;
        }
    }
    return -1;
}

void UserModel::AddUserToModel(const QString &userId, const QString &userName, const QString &FCS, const QString &rank, const int &role)
{
    for (User &user  : m_users)
    {
        if (userId==user.userId && userName==user.name)
        {
            user.FCS = FCS;
            user.rank = rank;
            user.role = role;
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
//            m_databaseService->RemoveUserIntoTable(user);
            SetImageToUser(user);
            FillModelByList();
            return;
        }
    }

    qFatal("нет юзера");
}

QStandardItemModel *UserModel::GetModel() const
{
    return m_model;
}

void UserModel::DataChanged()
{
    const QList<QPair<QString, QString>> namesAndIdsList=m_linuxUserService->GetSystemUsersNamesWithIdsList();
    m_users=FillListByUserService(namesAndIdsList);
    const QList<User> databaseUsers=FillListByDatabaseService();
    for (User &realUser : m_users)
    {
        for (const User &databaseUser: databaseUsers)
        {
            if(realUser.userId==databaseUser.userId && realUser.name==databaseUser.name)
            {
                realUser.FCS=databaseUser.FCS;
                realUser.rank=databaseUser.rank;
                realUser.role=databaseUser.role;
                SetImageToUser(realUser);
            }
        }
    }
    FillModelByList();
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
        user.role=-1;
        usersInSystem.append(user);
    }
    return  usersInSystem;
}

QList<User> UserModel::FillListByDatabaseService()
{
    return m_databaseService->GetAllUsers();
}

void UserModel::FillModelByList()
{
    m_model->clear();

    for (const User & user :qAsConst(m_users)) {
        QStandardItem *item = new QStandardItem();
        item->setData(QVariant::fromValue(user), Qt::UserRole + 1);
        m_model->appendRow(item);
    }
}

void UserModel::SetImageToUser(User &user)
{
    if (0 == user.role) {
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


