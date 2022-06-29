#include "usermodel.h"

UserModel::UserModel(ISqlDatabaseService *databaseService, LinuxUserService *userService)
    : m_model(new QStandardItemModel())
    , m_databaseService(databaseService)
    , m_linuxUserService(userService)
{
    ListUpdate();
}

UserModel::~UserModel()
{
    delete m_model;
}

int UserModel::GetRoleIdByUserId(const QString &userId) const noexcept
{

    for (int i = 0; i < m_model->rowCount(); ++i)
    {
        const User user = m_model->item(i)->data(Qt::UserRole + 1).value<User>();
        if (userId == user.GetUserId())
        {
            return user.GetUserRole();
        }
    }
    return -1;
}

void UserModel::AddUserToModel(const QString &userId, const QString &userName, const QString &FCS, int role)
{
    for (int i = 0; i < m_model->rowCount(); ++i)
    {
        const User user = m_model->item(i)->data(Qt::UserRole + 1).value<User>();
        if (userId == user.GetUserId() && userName == user.GetUserName() )
        {
            User newUser(userId, userName, FCS, role);
            m_databaseService->AppendUserIntoTable(newUser);
            newUser.SetUserImage(GetUserImageFromRole(newUser.GetUserRole()));
            QStandardItem *item = m_model->item(i);
            item->setData(QVariant::fromValue(newUser), Qt::UserRole + 1);
            return ;
        }
    }
    qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Неудачная попытка добавить пользователя. Ид пользователя: ") + userId + QStringLiteral(" имя пользователя: ") + userName + QStringLiteral(" не увенчалась успехом ")).toUtf8().constData());
}

void UserModel::DeleteUser(const QString &userId, const QString &userName)
{
    for (int i = 0; i < m_model->rowCount(); ++i)
    {
        User user = m_model->item(i)->data(Qt::UserRole + 1).value<User>();
        if (user.GetUserId() == userId && user.GetUserName() == userName)
        {
            m_databaseService->RemoveUserIntoTable(user.GetUserRole(), user);
            user.ClearUserData();
            user.SetUserImage(GetUserImageFromRole(user.GetUserRole()));
            QStandardItem *item = m_model->item(i);
            item->setData(QVariant::fromValue(user), Qt::UserRole + 1);
            return;
        }
    }
    qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Неудачная попытка удалить пользователя. Ид пользователя: ") + userId + QStringLiteral(" , его и не было ")).toUtf8().constData());
}

QStandardItemModel *UserModel::GetModel() const noexcept
{
    return m_model;
}
/**
 * void UserModel::OpenFlyAdminSmc()
 * Отркрываем панель управления пользователями Астры
 */
void UserModel::OpenFlyAdminSmc()
{
    m_linuxUserService->OpenFlyAdminSmc();
}
/**
 * void UserModel::ListUpdate()
 * Сперва получаем всех пользователей в системе
 * Затем удаляем из модели пользователей (если она не пуста) которых там нет
 * Затем добавляем новых пользвателей
 * Затем запрашиваем пользователей в бд
 * И потом их объеденяем
 */
void UserModel::ListUpdate()
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Заполняем юзеров инфой из бд в соответствии с ид и именем в системе"));
    const QList<QPair<QString, QString>> usersNow = m_linuxUserService->GetAllUsersWithIdInSystem();
    const QList<QPair<QString, QString> > usersThatNotContains = RemoveNotExsistsUsers(usersNow);
    AppendNewUsers(usersThatNotContains);
    const QList<User> databaseUsers = FillListByDatabaseService();
    JoinModelAndDbList(databaseUsers);
}
/**
 * QList<QPair<QString, QString> > UserModel::RemoveNotExsistsUsers(const QList<QPair<QString, QString> > &namesAndIdsList)
 * Удаляем из модели пользователей которых нет в системе
 */
QList<QPair<QString, QString> > UserModel::RemoveNotExsistsUsers(const QList<QPair<QString, QString> > &namesAndIdsList)
{
    QList<QPair<QString, QString> > usersThatNotContains = namesAndIdsList;
    for (int i = m_model->rowCount() - 1; i >= 0; i--)
    {
        const User user = m_model->item(i)->data(Qt::UserRole + 1).value<User>();
        bool finden = false;
        for (int j = namesAndIdsList.count() - 1; j >= 0; j--)
        {
            const QPair<QString, QString> nameAndId = namesAndIdsList.at(j);
            if (nameAndId.first == user.GetUserName() && nameAndId.second == user.GetUserId())
            {
                finden = true;
                usersThatNotContains.removeAt(j);
                break;
            }
        }
        if (!finden)
        {
            m_model->removeRow(i);
        }
    }
    return usersThatNotContains;
}
/**
 * Добавляем новых пользователей в модель если таковые остались
 */
void UserModel::AppendNewUsers(const QList<QPair<QString, QString>> &namesAndIdsList)
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Дозаполняем юзеров которых в базе нет"));
    for (const QPair<QString, QString> userNameAndId : qAsConst(namesAndIdsList))
    {
        QStandardItem *const item = new QStandardItem();
        const User user(userNameAndId.second, userNameAndId.first);
        item->setData(QVariant::fromValue(user), Qt::UserRole + 1);
        m_model->appendRow(item);
    }
}
/**
 * Получаем пользователей из бд
 */
QList<User> UserModel::FillListByDatabaseService()
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Запрашиваем юзеров из базы данных "));
    return m_databaseService->GetAllUsers();
}
/**
 * Объеденяем юзеров из таблицы и юзеров из системы
 * Если у них разные поля, заменяем юзера (на юзера с бд с данными) и добавл ему картинку
 */
void UserModel::JoinModelAndDbList(const QList<User> &databaseUsers)
{
    for (int i = 0; i < m_model->rowCount(); ++i)
    {
        QStandardItem *const item = m_model->item(i);
        User user = item->data(Qt::UserRole + 1).value<User>();
        for (User databaseUser : databaseUsers)
        {
            if (user.GetUserId() == databaseUser.GetUserId() && user.GetUserName() == databaseUser.GetUserName())
            {
                if (user.GetUserFCS() != databaseUser.GetUserFCS()  || user.GetUserRole() != databaseUser.GetUserRole())
                {
                    databaseUser.SetUserImage(GetUserImageFromRole(databaseUser.GetUserRole()));
                    item->setData(QVariant::fromValue(databaseUser), Qt::UserRole + 1);
                    break;
                }
            }
        }
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


