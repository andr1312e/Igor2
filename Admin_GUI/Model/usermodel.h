#ifndef ADMIN_GUI_MODEL_USERMODEL_H
#define ADMIN_GUI_MODEL_USERMODEL_H
#include <QList>

#include "Structs/userstruct.h"

#include "Services/isqlservice.h"
#include "Services/linuxuserservice.h"


class UserModel : public QObject
{
   Q_OBJECT
public:

   UserModel(ISqlDatabaseService *databaseService, LinuxUserService *userService, QObject *parent);

   ~UserModel();

   void AddUserToModel(const QString &userId, const QString &FCS, const QString &rank, const int &role);

   QStandardItemModel *GetModel();

   QStringList *GetUsersSystemNamesByRole(const QString &role);

public Q_SLOTS:

   void DeleteUser(const QString &userId);

   void OnDataChanged();

private:

   QStandardItemModel *m_model;

   ISqlDatabaseService *m_databaseService;

   LinuxUserService *m_linuxUserService;

   QList<User> m_users;

   QStringList *m_currentRoleUsers;

private:

   void ClearList();

   QList<User> FillListByUserService(const QList<QPair<QString, QString> > &namesAndIdsList) const;

   QList<User> FillListByDatabaseService();

   void FillModelByList();

   void SetImageToUser(User &user);

};

#endif // ADMIN_GUI_MODEL_USERMODEL_H
