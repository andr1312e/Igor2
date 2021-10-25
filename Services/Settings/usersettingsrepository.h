#ifndef USERSETTINGSREPOSITORY_H
#define USERSETTINGSREPOSITORY_H
#include <QString>
#include <QFile>
#include <QDomDocument>
#include <QStringLiteral>
#include <QVector>

#include "Structs/userstruct.h"

#include "Services/Terminals/terminal.h"

class UsersDataWizardRepository
{

public:

   explicit UsersDataWizardRepository(const QString &curerntUserName, const  QString &curerntUserId, Terminal *terminal);

   ~UsersDataWizardRepository();

   const QString &GetCurrentUserName() const;

   bool HasData() const;

   QString &GetCurrentUserFCS();

   QString &GetCurrentUserRank();

   QVector<User> GetUsersList();

   int GetUserCount();

   void SetFCSAndRolesFromFile(QString &pathToUserDb);

   void GetFCSAndRolesFromXml(QDomElement &usersNode);

   void WriteUserRepositoryToFile(const QString &pathToWriteDb, bool adminOnly);

private:

   bool FindAndUpdateAdminData();

   void AppendAdminToList();

   void CreateMainTag();

   void WriteAllUsersToDomDocument();

   void WriteAdminToDomDocument();

   void AppendUserToDomDocument(QDomDocument &document, const User &user);

   void EncryptAndWriteToFile(const QString &pathToWriteDb);

private:

   const QString m_curerntUserName;

   const QString m_curerntUserId;

   Terminal *m_terminal;

private:

   bool m_hasData;

   QString m_userFCS;

   QString m_userRank;

   QVector<User> m_usersList;

   QDomDocument *m_accountsData;

};

#endif // USERSETTINGSREPOSITORY_H
