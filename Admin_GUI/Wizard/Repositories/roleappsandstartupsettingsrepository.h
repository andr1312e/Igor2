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

    explicit RolesAndStartupsWizardRepository(Terminal * const terminal);

    ~RolesAndStartupsWizardRepository();

    bool HasData(quint8 roleId);

    void GetRoleDesktopsAndStartupsFromLocalRepository(const int roleIndex, QList<DesktopEntity> &roleDesktops, QStringList &startups) const;

    void GetRoleDesktopsAndStartupsFromDb(ISqlDatabaseService *iSqlDatabaseService);

    void SetRoleDesktopsAndStartupsFromBackup(const int &roleIndex, const QDomElement &backupNode, const QString &backupFolder);

    int GetRoleDesktopsAppCount(const int roleIndex) const;

    void SaveRoleDesktopsToDb(ISqlDatabaseService *iSqlDatabaseService, const int roleIndex);

    void SaveRoleExecsToDb(ISqlDatabaseService *iSqlDatabaseService, const int roleIndex);

    QStringList GetAllUniqueDesktopExecsAndStarups(const int roleIndex) const;

private:

    void SetRoleDesktopFromXml(const int roleIndex, const QDomElement &desktops, const QString &backupFolder);

    void SetRoleStartupsFromXml(const int roleIndex, const QDomElement &startups, const QString &backupFolder);

    void AppendRoleStartups(const int roleIndex, const QStringList &startupList);

    void AppendEnittyToRoleDesktops(const int roleIndex, DesktopEntity &desktopEntity);

private:

    const QList<DesktopEntity> &GetDesktopsByIndex(const int roleIndex) const;

    const QStringList &GetStatupsByIndex(const int roleIndex) const;

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
