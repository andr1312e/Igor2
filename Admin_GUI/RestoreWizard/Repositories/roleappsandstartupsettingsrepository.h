#ifndef SERVICES_SETTINGS_ROLESANDSTARTUPSWIZARDREPOSITORY_H
#define SERVICES_SETTINGS_ROLESANDSTARTUPSWIZARDREPOSITORY_H
#include <QStringList>
#include <QDomDocument>

#include "Logging/logger.h"
#include "Services/Sql/isqlservice.h"

#include "Services/Terminals/terminal.h"
#include "Structs/programstruct.h"
#include "Structs/user.h"

class RolesAndStartupsWizardRepository
{
public:

    explicit RolesAndStartupsWizardRepository(Terminal *terminal);
    ~RolesAndStartupsWizardRepository();

public:
    bool HasData(int roleIndex) const;

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

    void SetRoleStartupsFromXml(int roleId, const QDomElement &startups, const QString &backupFolder);

    void AppendRoleStartups(int roleIndex, const QStringList &startupList);

    void AppendEnittyToRoleDesktops(int roleIndex, const DesktopEntity &desktopEntity);

private:

    const QList<DesktopEntity> &GetDesktopsByIndex(int roleIndex) const;

    const QStringList &GetStatupsByIndex(int roleIndex) const;

private:

    Terminal *const m_terminal;

    QVector<QStringList> m_rolesStartup;
    QVector<QList<DesktopEntity>> m_roleDesktopsIcons;
};

#endif // SERVICES_SETTINGS_ROLESANDSTARTUPSWIZARDREPOSITORY_H
