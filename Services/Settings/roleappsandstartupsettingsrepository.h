#ifndef WIZARDSERVICE_H
#define WIZARDSERVICE_H
#include <QStringList>

#include "Services/Terminals/terminal.h"
#include "Structs/programstruct.h"

class RoleAndStartupWizardRepository
{
public:
    RoleAndStartupWizardRepository(QString &currentUserName, Terminal *terminal);
    ~RoleAndStartupWizardRepository();

    void getRoleData(int roleNum, QList<DesktopEntity> *m_RoleDesktops, QStringList *m_startups);
private:
    Terminal *m_terminal;
    QString m_currentUserName;

    QStringList *m_firstRoleStartup;
    QStringList *m_secondRoleStartup;
    QStringList *m_thirdRoleStartup;
    QStringList *m_fourthRoleStartup;

    QList<DesktopEntity> *m_firstRoleDesktopsIcons;
    QList<DesktopEntity> *m_secondRoleDesktopsIcons;
    QList<DesktopEntity> *m_thirdRoleDesktopsIcons;
    QList<DesktopEntity> *m_fourthRoleDesktopsIcons;
};

#endif // WIZARDSERVICE_H
