#include "identifyservice.h"
#include <QDebug>

IdentifyService::IdentifyService(Terminal *terminal, DatabaseService *databaseService, LinuxUserService *userService)
    : m_terminal(terminal)
    , m_databaseService(databaseService)
    , m_linuxUserService(userService)
    , m_adminsUserName(new QStringList())
{
}

IdentifyService::~IdentifyService()
{
    delete m_adminsUserName;
}

bool IdentifyService::canGetAccess()
{
    if (!m_linuxUserService->hasCurrentUserAdminPrivileges())
        return false;
    m_databaseService->GetRLSTIAdminsUserName(*m_adminsUserName);
    QString userName=m_linuxUserService->getCurrentUserName();
    for (QStringList::iterator it=m_adminsUserName->begin(); it!=m_adminsUserName->end(); ++it)
    {
        if (userName==*it)
        {
            return true;
        }
    }
    return true;
}
