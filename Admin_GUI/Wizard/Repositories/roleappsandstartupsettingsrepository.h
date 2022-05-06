#ifndef SERVICES_SETTINGS_ROLESANDSTARTUPSWIZARDREPOSITORY_H
#define SERVICES_SETTINGS_ROLESANDSTARTUPSWIZARDREPOSITORY_H
#include <QStringList>
#include <QDomDocument>

#include "Services/Sql/isqlservice.h"

#include "Services/Terminals/terminal.h"
#include "Structs/programstruct.h"
#include "Structs/userstruct.h"

class RolesAndStartupsWizardRepository
{
public:

    explicit RolesAndStartupsWizardRepository(Terminal * terminal);

    ~RolesAndStartupsWizardRepository();

    bool HasData(int roleId) const;

    void GetRoleDesktopsAndStartupsFromLocalRepository(int roleIndex, QList<DesktopEntity> &roleDesktops, QStringList &startups) const;

    void GetRoleDesktopsAndStartupsFromDb(ISqlDatabaseService *iSqlDatabaseService);

    void SetRoleDesktopsAndStartupsFromBackup(int roleIndex, const QDomElement &backupNode, const QString &backupFolder);

    int GetRoleDesktopsAppCount(int roleIndex) const;

    void SaveRoleDesktopsToDb(ISqlDatabaseService *iSqlDatabaseService, int roleIndex);

    void SaveRoleExecsToDb(ISqlDatabaseService *iSqlDatabaseService, int roleIndex);

    QStringList GetAllUniqueDesktopExecsAndStarups(int roleIndex) const;

    void Clear();

private:

    void SetRoleDesktopFromXml(int roleIndex, const QDomElement &desktops, const QString &backupFolder);

    void SetRoleStartupsFromXml(const int roleIndex, const QDomElement &startups, const QString &backupFolder);

    void AppendRoleStartups(int roleIndex, const QStringList &startupList);

    void AppendEnittyToRoleDesktops(int roleIndex, const DesktopEntity &desktopEntity);

private:

    const QList<DesktopEntity> &GetDesktopsByIndex(int roleIndex) const;

    const QStringList &GetStatupsByIndex(int roleIndex) const;

private:

    Terminal * const m_terminal;

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
