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

    DatabaseService(const QString &filePath, Terminal *terminal);

    ~DatabaseService();

    void getRLSTIAdminsUserName (QStringList &adminsUserNames);

    void fillUserFromList (User &currentUser);

    QString getUserAttributeByLinuxUserNameToList(const QString &userName, const QString &attribute);

    QString getFilePath(){ return m_filePath;}

    Terminal* getTerminal() {return m_terminal;}

    void writeToFile(QList<User> *users);

private:

    QString m_filePath;

    Terminal *m_terminal;

    QFile *m_file;

    QByteArray m_array;

    QDomDocument m_accountsData;

private:

    QDomElement createUserElement(QDomDocument &users, const User &user);

};

#endif // SERVICES_DATABASESERVICE_H
