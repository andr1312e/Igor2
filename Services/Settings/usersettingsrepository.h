#ifndef SERVICES_SETTINGS_USERSETTINGSREPOSITORY_H
#define SERVICES_SETTINGS_USERSETTINGSREPOSITORY_H

#include <QList>
#include <QString>
#include <QDomElement>

#include "Structs/userstruct.h"

#include "Services/isqlservice.h"
#include "Services/linuxuserservice.h"

class UsersDataWizardRepository
{

public:

   explicit UsersDataWizardRepository(LinuxUserService *service);

   ~UsersDataWizardRepository();

   const QString &GetCurrentUserName() const;

   bool HasData() const;

   QString &GetCurrentUserFCS();

   QString &GetCurrentUserRank();

   QList<User> &GetUsersList();

   void GetExsistsUsersListFromDb(ISqlDatabaseService *m_iSqlDatabaseService);

   int GetUserCount() const;

   void GetFCSAndRolesFromXml(const QDomElement &usersNode);

   void WriteUserRepositoryToDB(ISqlDatabaseService *m_iSqlDatabaseService, bool adminOnly);

private:

   void JuxtaposeUserIdAndUserNameWithSystemsData();

   bool FindAndUpdateAdminData();

   void AppendAdminToCacheList();

   void WriteAllUsersToDatabase(ISqlDatabaseService *m_iSqlDatabaseService);

   void WriteAdminToDatabase(ISqlDatabaseService *m_iSqlDatabaseService);

private:

   LinuxUserService *m_service;

   const QString m_curerntUserName;

   const QString m_curerntUserId;

private:

   QString m_currentUserFCS;

   QString m_currenUserRank;

   QList<User> m_usersList;
};

#endif // SERVICES_SETTINGS_USERSETTINGSREPOSITORY_H
