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
    void DeleteUser(const QString &userId, const QString &userName);
    QStandardItemModel *GetModel() const noexcept;
    void OpenFlyAdminSmc();
    void ListUpdate();
    bool TwoOrMoreUsers() const noexcept;
private:
    QList<QPair<QString, QString> > RemoveNotExsistsUsers(const QList<QPair<QString, QString> > &namesAndIdsList);
    void AppendNewUsers(const QList<QPair<QString, QString> > &namesAndIdsList);
    QList<User> FillListByDatabaseService();
    void JoinModelAndDbList(const QList<User> &databaseUsers);

    const QString GetUserImageFromRole(int userRole) const noexcept;

private:
    QStandardItemModel *const m_model;
    ISqlDatabaseService *const m_databaseService;
    LinuxUserService *const m_linuxUserService;

};

#endif // ADMIN_GUI_MODEL_USERMODEL_H
