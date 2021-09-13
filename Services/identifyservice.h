#ifndef SERVICES_IDENTIFYSERVICE_H
#define SERVICES_IDENTIFYSERVICE_H
#include <QProcess>
#include <QString>

#include "Services/terminal.h"
#include "Services/databaseservice.h"
#include "Services/linuxuserservice.h"

class IdentifyService
{
public:

    IdentifyService(Terminal *terminal, DatabaseService *databaseService, LinuxUserService *userService);

    ~IdentifyService();

    Terminal* getTerminal() {return m_terminal;}

    bool canGetAccess();

private:

    Terminal *m_terminal;

    DatabaseService *m_databaseService;

    LinuxUserService *m_linuxUserService;

    QStringList *m_adminsUserName;
};

#endif // SERVICES_IDENTIFYSERVICE_H
