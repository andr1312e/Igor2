#ifndef SERVICES_SETTINGS_ROLESANDSTARTUPSWIZARDREPOSITORY_H
#define SERVICES_SETTINGS_ROLESANDSTARTUPSWIZARDREPOSITORY_H
#include <QStringList>
#include <QDomDocument>

#include "Services/isqlservice.h"

#include "Services/Terminals/terminal.h"
#include "Structs/programstruct.h"
#include "Structs/userstruct.h"

class RolesAndStartupsWizardRepository
{
public:

    explicit RolesAndStartupsWizardRepository(Terminal *terminal);

    ~RolesAndStartupsWizardRepository();

    bool HasData();

    void GetRoleDesktopsAndStartupsFromLocalRepository(const int roleIndex, QList<DesktopEntity> &roleDesktops, QStringList &startups);

    void GetRoleDesktopsAndStartupsFromDb(ISqlDatabaseService *iSqlDatabaseService);

    void SetRoleDesktopsAndStartupsFromBackup(const int &roleIndex, QDomElement &backupNode, const QString &backupFolder);

    int GetRoleDesktopsAppCount(const int roleIndex);

    void SaveRoleDesktopsToDb(ISqlDatabaseService *iSqlDatabaseService, const int roleIndex);

    void SaveRoleExecsToDb(ISqlDatabaseService *iSqlDatabaseService, const int roleIndex);

    QStringList GetAllUniqueDesktopExecsAndStarups(const int roleIndex);

private:

    void SetRoleDesktopFromXml(const int roleIndex, QDomElement &desktops, const QString &backupFolder);

    void SetRoleStartupsFromXml(const int roleIndex, QDomElement &startups, const QString &backupFolder);

    void AppendRoleStartups(const int roleIndex, const QStringList &startupList);

    void AppendEnittyToRoleDesktops(const int roleIndex, DesktopEntity &desktopEntity);

private:

    QList<DesktopEntity> &GetDesktopsByIndex(const int roleIndex);

    QStringList &GetStatupsByIndex(const int roleIndex);

private:

    Terminal *m_terminal;

    QStringList m_firstRoleStartup;
    QStringList m_secondRoleStartup;
    QStringList m_thirdRoleStartup;
    QStringList m_fourthRoleStartup;

    QList<DesktopEntity> m_firstRoleDesktopsIcons;
    QList<DesktopEntity> m_secondRoleDesktopsIcons;
    QList<DesktopEntity> m_thirdRoleDesktopsIcons;
    QList<DesktopEntity> m_fourthRoleDesktopsIcons;
};

#endif // SERVICES_SETTINGS_ROLESANDSTARTUPSWIZARDREPOSITORY_H
