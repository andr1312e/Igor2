#ifndef ADMIN_GUI_MODEL_USERMODEL_H
#define ADMIN_GUI_MODEL_USERMODEL_H
#include <QList>

#include "Structs/userstruct.h"

#include "Services/Sql/isqlservice.h"
#include "Services/linuxuserservice.h"


class UserModel
{

public:

   UserModel(ISqlDatabaseService *databaseService, LinuxUserService *userService);
   ~UserModel();
   int GetRoleIdByUserId(const QString &userId) const;
   void AddUserToModel(const QString &userId, const QString &userName, const QString &FCS, const QString &rank, const int &role);
   void DeleteUser(const QString &userId);

   QStandardItemModel *GetModel() const;

private:
   void DataChanged();
   QList<User> FillListByUserService(const QList<QPair<QString, QString> > &namesAndIdsList) const;
   QList<User> FillListByDatabaseService();
   void FillModelByList();
   void SetImageToUser(User &user);

private:
   QStandardItemModel *m_model;
   ISqlDatabaseService *m_databaseService;
   LinuxUserService *m_linuxUserService;
   QList<User> m_users;

};

#endif // ADMIN_GUI_MODEL_USERMODEL_H
