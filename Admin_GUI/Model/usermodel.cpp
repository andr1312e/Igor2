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
            SetImageToUser(user.role, user.m_image);
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
        if (user.userId == userId)
        {
            m_databaseService->RemoveUserIntoTable(user.role, user);
            user.FCS.clear();
            user.rank.clear();
            user.role=-1;
            SetImageToUser(user.role, user.m_image);
            FillModelByList();
            return;
        }
    }
    qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Неудачная попытка удалить пользователя. Ид пользователя: ") + userId + QStringLiteral(" , его и не было ")).toUtf8().constData());
}

QStandardItemModel *UserModel::GetModel() const
{
    return m_model;
}

void UserModel::DataChanged()
{
    if(Log4Qt::Logger::rootLogger()->HasAppenders())
    {
        Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Заполняем юзеров инфой из бд в соответствии с ид и именем в системе"));
    }
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
                SetImageToUser(realUser.role, realUser.m_image);
            }
        }
    }
    FillModelByList();
}

QList<User> UserModel::FillListByUserService(const QList< QPair< QString, QString>> &namesAndIdsList) const
{
    if(Log4Qt::Logger::rootLogger()->HasAppenders())
    {
        Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Заполняем юзеров системной инфой "));
    }
    QList<User> usersInSystem;
    for (const QPair< QString, QString> &userNameAndId : namesAndIdsList)
    {
        User user;
        user.name=userNameAndId.first;
        user.userId=userNameAndId.second;
        user.m_image=QStringLiteral(":/images/0.jpg");
        user.role=-1;
        usersInSystem.append(user);
    }
    return  usersInSystem;
}

QList<User> UserModel::FillListByDatabaseService()
{
    if(Log4Qt::Logger::rootLogger()->HasAppenders())
    {
        Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Запрашиваем юзеров из базы данных "));
    }
    return m_databaseService->GetAllUsers();
}

void UserModel::FillModelByList()
{
    m_model->clear();
    if(Log4Qt::Logger::rootLogger()->HasAppenders())
    {
        Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Заполняем модель из листа "));
    }
    for (const User & user :qAsConst(m_users)) {
        QStandardItem *item = new QStandardItem();
        item->setData(QVariant::fromValue(user), Qt::UserRole + 1);
        m_model->appendRow(item);
    }
}

void UserModel::SetImageToUser(const int &userRole, QString &userImage)
{
    if (0 == userRole) {
        userImage = QStringLiteral(":/images/0.jpg");
    } else {
        if (1==userRole) {
            userImage = QStringLiteral(":/images/1.jpg");
        } else {
            if (2==userRole) {
                userImage = QStringLiteral(":/images/2.jpg");
            } else {
                userImage = QStringLiteral(":/images/3.jpg");
            }
        }
    }
}


