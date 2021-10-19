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

    void loadFromFile(const QString &userDbPath);

    ~DatabaseService();

    void getRLSTIAdminsUserName (QStringList &adminsUserNames);

    void readUserFromFile (User &currentUser);

    QString getUserAttributeByLinuxUserNameToList(const QString &userName, const QString &attribute);

    QString getFilePath(){ return m_userDbPath;}

    Terminal* getTerminal() {return m_terminal;}

    void writeToFile(QList<User> *users);

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
