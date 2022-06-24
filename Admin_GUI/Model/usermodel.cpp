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

int UserModel::GetRoleIdByUserId(const QString &userId) const noexcept
{
    for (const User &user : qAsConst(m_users))
    {
        if (userId == user.GetUserId())
        {
            return user.GetUserRole();
        }
    }
    return -1;
}

void UserModel::AddUserToModel(const QString &userId, const QString &userName, const QString &FCS, int role)
{
    for (User &user  : m_users)
    {
        if (userId == user.GetUserId() && userName == user.GetUserName())
        {
            user.SetUserData(FCS, role);
            m_databaseService->AppendUserIntoTable(user);
            user.SetUserImage(GetUserImageFromRole(user.GetUserRole()));
            FillModelByList();
            return;
        }
    }
    qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Неудачная попытка добавить пользователя. Ид пользователя: ") + userId + QStringLiteral(" имя пользователя: ") + userName + QStringLiteral(" не увенчалась успехом ")).toUtf8().constData());
}

void UserModel::DeleteUser(const QString &userId)
{
    for (User &user  : m_users)
    {
        if (user.GetUserId() == userId)
        {
            m_databaseService->RemoveUserIntoTable(user.GetUserRole(), user);
            user.ClearUserData();
            user.SetUserImage(GetUserImageFromRole(user.GetUserRole()));
            FillModelByList();
            return;
        }
    }
    qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Неудачная попытка удалить пользователя. Ид пользователя: ") + userId + QStringLiteral(" , его и не было ")).toUtf8().constData());
}

QStandardItemModel *UserModel::GetModel() const noexcept
{
    return m_model;
}

void UserModel::OpenFlyAdminSmc()
{
    m_linuxUserService->OpenFlyAdminSmc();
}

void UserModel::DataChanged()
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Заполняем юзеров инфой из бд в соответствии с ид и именем в системе"));
    const QList<QPair<QString, QString>> namesAndIdsList = m_linuxUserService->GetSystemUsersNamesWithIds();
    m_users = FillListByUserService(namesAndIdsList);
    const QList<User> databaseUsers = FillListByDatabaseService();
    for (User &user : m_users)
    {
        for (const User &databaseUser : databaseUsers)
        {
            if (user.GetUserId() == databaseUser.GetUserId() && user.GetUserName() == databaseUser.GetUserName())
            {
                user.SetUserData(databaseUser.GetUserFCS(), databaseUser.GetUserRole());
                user.SetUserImage(GetUserImageFromRole(user.GetUserRole()));
            }
        }
    }
    FillModelByList();
}

QList<User> UserModel::FillListByUserService(const QList<QPair<QString, QString>> &namesAndIdsList) const
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Заполняем юзеров системной инфой "));
    QList<User> usersInSystem;
    for (const QPair<QString, QString> &userNameAndId : namesAndIdsList)
    {
        User user(userNameAndId.second, userNameAndId.first);
        user.SetUserImage(GetUserImageFromRole(-1));
        usersInSystem.append(user);
    }
    return  usersInSystem;
}

QList<User> UserModel::FillListByDatabaseService()
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Запрашиваем юзеров из базы данных "));
    return m_databaseService->GetAllUsers();
}

void UserModel::FillModelByList() noexcept
{
    m_model->clear();
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Заполняем модель из листа "));
    for (const User &user : qAsConst(m_users))
    {
        QStandardItem *const item = new QStandardItem();
        item->setData(QVariant::fromValue(user), Qt::UserRole + 1);
        m_model->appendRow(item);
    }
}

const QString UserModel::GetUserImageFromRole(int userRole) const noexcept
{
    switch (userRole)
    {
    case -1:
        return QLatin1Literal(":/images/users/noRole.png");
    case 0:
        return  QLatin1Literal(":/images/users/user.png");
    case 1:
        return QLatin1Literal(":/images/users/operator.png");
    case 2:
        return QLatin1Literal(":/images/users/specialist.png");
    case 3://Админ
        return QLatin1Literal(":/images/users/admin.png");
    default:
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Неудачная попытка найти картинку для пользователя. Роль: ") + QString::number(userRole)).toUtf8().constData());
        break;
    }
}


