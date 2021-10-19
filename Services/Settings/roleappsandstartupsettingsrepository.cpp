#include "roleappsandstartupsettingsrepository.h"

RoleAndStartupWizardRepository::RoleAndStartupWizardRepository(QString &currentUserName, Terminal *terminal)
    : m_terminal(terminal)
    , m_currentUserName(currentUserName)
    , m_firstRoleStartup(new QStringList())
    , m_secondRoleStartup(new QStringList())
    , m_thirdRoleStartup(new QStringList())
    , m_fourthRoleStartup(new QStringList())
    , m_firstRoleDesktopsIcons(new QList<DesktopEntity>())
    , m_secondRoleDesktopsIcons(new QList<DesktopEntity>())
    , m_thirdRoleDesktopsIcons(new QList<DesktopEntity>())
    , m_fourthRoleDesktopsIcons(new QList<DesktopEntity>())
{

}

RoleAndStartupWizardRepository::~RoleAndStartupWizardRepository()
{
    delete m_firstRoleStartup;
    delete m_secondRoleStartup;
    delete m_thirdRoleStartup;
    delete m_fourthRoleStartup;

    delete m_firstRoleDesktopsIcons;
    delete m_secondRoleDesktopsIcons;
    delete m_thirdRoleDesktopsIcons;
    delete m_fourthRoleDesktopsIcons;
}

void RoleAndStartupWizardRepository::getRoleData(int roleNum, QList<DesktopEntity> *m_RoleDesktops, QStringList *m_startups)
{
    switch (roleNum) {
    case 0:
    {
        m_RoleDesktops=m_firstRoleDesktopsIcons;
        m_startups=m_firstRoleStartup;
        break;
    }
    case 1:
    {
        m_RoleDesktops=m_secondRoleDesktopsIcons;
        m_startups=m_secondRoleStartup;
        break;
    }
    case 2:
    {
        m_RoleDesktops=m_thirdRoleDesktopsIcons;
        m_startups=m_thirdRoleStartup;
        break;
    }
    case 3:
    {
        m_RoleDesktops=m_fourthRoleDesktopsIcons;
        m_startups=m_fourthRoleStartup;
        break;
    }
    default:
    {
        qFatal("Роли только 4");
    }
    }
}
