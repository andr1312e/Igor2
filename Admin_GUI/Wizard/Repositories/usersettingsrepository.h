#ifndef SERVICES_SETTINGS_USERSETTINGSREPOSITORY_H
#define SERVICES_SETTINGS_USERSETTINGSREPOSITORY_H

#include <QList>
#include <QString>
#include <QDomElement>

#include "Structs/userstruct.h"

#include "Services/Sql/isqlservice.h"
#include "Services/linuxuserservice.h"

class UsersDataWizardRepository
{

public:
   explicit UsersDataWizardRepository(LinuxUserService *service);
   ~UsersDataWizardRepository();

public:
   bool HasData() const;
   const QString &GetCurrentUserFCS() const;
   const QList<User> &GetUsersList() const;
   int GetUserCount() const;

   void GetUsersFromDb(ISqlDatabaseService *m_iSqlDatabaseService);
   void GetUsersFromXmL(const QDomElement &usersNode);
   void SaveUsers(ISqlDatabaseService *m_iSqlDatabaseService, bool adminOnly);
   void Clear();

private:

   int GetKorrektUserRole(const QDomElement &userDomElement) const;
   void JuxtaposeRepUserIdWithSystemUserId();
   bool FindAndUpdateAdminData();
   void AppendAdminToCacheList();
   void WriteAllUsersToDatabase(ISqlDatabaseService *m_iSqlDatabaseService);
   void WriteAdminToDatabase(ISqlDatabaseService *m_iSqlDatabaseService);
   void CheckCurrentUserFcsAndRank();

private:
   LinuxUserService* const m_service;

private:
   QString m_currentUserFCS;
   QList<User> m_usersList;
};

#endif // SERVICES_SETTINGS_USERSETTINGSREPOSITORY_H
