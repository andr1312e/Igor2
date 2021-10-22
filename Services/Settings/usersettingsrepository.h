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

   explicit UsersDataWizardRepository(QString &curerntUserName);

   ~UsersDataWizardRepository();

   const QString &GetCurrentUserName() const;

   bool HasData() const;

   QString &GetCurrentUserFCS();

   QString &GetCurrentUserRank();

   QVector<User> &GetUsersList();

   int GetUserCount();

   void SetFCSAndRolesFromDb(QString &pathToUserDb);

   void GetFCSAndRolesFromXml(QDomElement &usersNode);

   void WriteToFile(Terminal *terminal, QString &pathToWritingDb);

private:

   const QString m_curerntUserName;

   bool m_hasData;

   QString m_userFCS;

   QString m_userRank;

   QVector<User> m_usersList;

};

#endif // USERSETTINGSREPOSITORY_H
