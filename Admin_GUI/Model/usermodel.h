#ifndef ADMIN_GUI_MODEL_USERMODEL_H
#define ADMIN_GUI_MODEL_USERMODEL_H
#include <QList>
#include <QStandardItemModel>

#include "Structs/user.h"
#include "Logging/logger.h"
#include "Services/Sql/isqlservice.h"
#include "Services/linuxuserservice.h"


class UserModel
{

public:
    explicit UserModel(ISqlDatabaseService *databaseService, LinuxUserService *userService);
    ~UserModel();

public:
    int GetRoleIdByUserId(const QString &userId) const noexcept;
    void AddUserToModel(const QString &userId, const QString &userName, const QString &FCS, int role);
    void DeleteUser(const QString &userId);
    QStandardItemModel *GetModel() const noexcept;
    void OpenFlyAdminSmc();
private:
    void DataChanged();
    QList<User> FillListByUserService(const QList<QPair<QString, QString> > &namesAndIdsList) const;
    QList<User> FillListByDatabaseService();
    void FillModelByList() noexcept;
    const QString GetUserImageFromRole(int userRole) const noexcept;

private:
    QStandardItemModel *const m_model;
    ISqlDatabaseService *const m_databaseService;
    LinuxUserService *const m_linuxUserService;
    QList<User> m_users;

};

#endif // ADMIN_GUI_MODEL_USERMODEL_H
