#ifndef ADMIN_GUI_MODEL_USERMODEL_H
#define ADMIN_GUI_MODEL_USERMODEL_H
#include <QList>
#include <QStandardItemModel>

#include "Structs/userstruct.h"
#include "Logging/logger.h"
#include "Services/Sql/isqlservice.h"
#include "Services/linuxuserservice.h"


class UserModel
{

public:

   UserModel(ISqlDatabaseService *databaseService, LinuxUserService *userService);
   ~UserModel();

public:
   int GetRoleIdByUserId(const QString &userId) const;
   void AddUserToModel(const QString &userId, const QString &userName, const QString &FCS, int role);
   void DeleteUser(const QString &userId);
   QStandardItemModel *GetModel() const;

private:
   void DataChanged();
   QList<User> FillListByUserService(const QList<QPair<QString, QString> > &namesAndIdsList) const;
   QList<User> FillListByDatabaseService();
   void FillModelByList();
   void SetImageToUser(const int &userRole, QString &userImage);

private:
   QStandardItemModel * const m_model;
   ISqlDatabaseService * const m_databaseService;
   LinuxUserService * const m_linuxUserService;
   QList<User> m_users;

};

#endif // ADMIN_GUI_MODEL_USERMODEL_H
