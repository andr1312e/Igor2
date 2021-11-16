#ifndef SERVICES_DATABASESERVICE_H
#define SERVICES_DATABASESERVICE_H

#include <QFile>
#include <QString>
#include <QProcess>
#include <QDomDocument>
#include <QDomNodeList>

#include "Structs/userstruct.h"

#include "Services/Terminals/terminal.h"

class DatabaseService
{

public:

   DatabaseService(Terminal *terminal);

   void LoadFromFile(const QString &userDbPath);

   ~DatabaseService();

   void GetRLSTIAdminsUserName (QStringList &adminsUserNames);

   void ReadUserFromFile (User &currentUser);

   QString GetUserAttributeByLinuxUserNameToList(const QString &userName, const QString &attribute);

   QString GetFilePath()
   {
      return m_userDbPath;
   }

   Terminal *GetTerminal()
   {
      return m_terminal;
   }

   void WriteToFile(QList<User> *users);

private:

   QString m_userDbPath;

   Terminal *m_terminal;

   QFile *m_file;

   QDomDocument *m_accountsData;

   QByteArray m_array;

private:

   QDomElement createUserElement(QDomDocument &users, const User &user);

};

#endif // SERVICES_DATABASESERVICE_H
