#include "roleappsandstartupsettingsrepository.h"

RolesAndStartupsWizardRepository::RolesAndStartupsWizardRepository(Terminal * terminal)
    : m_terminal(terminal)
{

}

RolesAndStartupsWizardRepository::~RolesAndStartupsWizardRepository()
{

}

bool RolesAndStartupsWizardRepository::HasData(int roleId) const
{
    const bool isNotEmptyStartups = !GetStatupsByIndex(roleId).isEmpty();
    const bool isNotEmptyDesktops= !GetDesktopsByIndex(roleId).isEmpty();
    return (isNotEmptyStartups || isNotEmptyDesktops);
}

void RolesAndStartupsWizardRepository::GetRoleDesktopsAndStartupsFromLocalRepository(int roleIndex, QList<DesktopEntity> &roleDesktops, QStringList &startups) const
{
    roleDesktops=GetDesktopsByIndex(roleIndex);
    startups=GetStatupsByIndex(roleIndex);
}

void RolesAndStartupsWizardRepository::GetRoleDesktopsAndStartupsFromDb(ISqlDatabaseService *iSqlDatabaseService)
{
    for (int i=0; i<Roles.count(); ++i)
    {
        if(iSqlDatabaseService->CheckStartupTables(i))
        {
            AppendRoleStartups(i, iSqlDatabaseService->GetAllRoleStartups(i));
        }
        if(iSqlDatabaseService->CheckDesktopTables(i))
        {
            const QList<DesktopEntity> desktopEntityList=iSqlDatabaseService->GetAllRoleDesktops(i);
            for (const DesktopEntity & entity: qAsConst(desktopEntityList))
            {
                AppendEnittyToRoleDesktops(i, entity);
            }
        }
    }
}

void RolesAndStartupsWizardRepository::SetRoleDesktopsAndStartupsFromBackup(int roleIndex,const QDomElement &backupNode, const QString &backupFolder)
{
    if(2==backupNode.childNodes().count())
    {
        const QDomElement desktops(backupNode.firstChildElement());
        const QDomElement startups(backupNode.lastChildElement());
        SetRoleDesktopFromXml(roleIndex, desktops, backupFolder);
        SetRoleStartupsFromXml(roleIndex, startups, backupFolder);
    }
}

int RolesAndStartupsWizardRepository::GetRoleDesktopsAppCount( int roleIndex) const
{
    return GetDesktopsByIndex(roleIndex).count();
}

void RolesAndStartupsWizardRepository::SaveRoleDesktopsToDb(ISqlDatabaseService *iSqlDatabaseService, int roleIndex)
{
    iSqlDatabaseService->CreateDesktopRolesIfNotExists(roleIndex);
    iSqlDatabaseService->ClearDesktopTable(roleIndex);

    const QList<DesktopEntity> currentRoleDesktops(GetDesktopsByIndex(roleIndex));

    for (const DesktopEntity &enitty : currentRoleDesktops)
    {
        iSqlDatabaseService->AppendDesktopIntoRole(roleIndex, enitty);
    }
}

void RolesAndStartupsWizardRepository::SaveRoleExecsToDb(ISqlDatabaseService *iSqlDatabaseService, int roleIndex)
{
    iSqlDatabaseService->CreateStartupsTableIfNotExists(roleIndex);
    iSqlDatabaseService->ClearStartupsTable(roleIndex);
    const QStringList currentRoleStartups(GetStatupsByIndex(roleIndex));
    for (const QString &exec :currentRoleStartups)
    {
        iSqlDatabaseService->AppendStartupIntoRole(roleIndex, exec);
    }
}

QStringList RolesAndStartupsWizardRepository::GetAllUniqueDesktopExecsAndStarups(int roleIndex) const
{
    QStringList startupsList=GetStatupsByIndex(roleIndex);
    const QList<DesktopEntity> desktopsList=GetDesktopsByIndex(roleIndex);
    for (const DesktopEntity &entity: desktopsList)
    {
        startupsList.append(entity.exec);
    }
    startupsList.removeDuplicates();
    return  startupsList;
}

void RolesAndStartupsWizardRepository::Clear()
{
    m_firstRoleStartup.clear();
    m_secondRoleStartup.clear();
    m_thirdRoleStartup.clear();
    m_fourthRoleStartup.clear();

    m_firstRoleDesktopsIcons.clear();
    m_secondRoleDesktopsIcons.clear();
    m_thirdRoleDesktopsIcons.clear();
    m_fourthRoleDesktopsIcons.clear();
}

void RolesAndStartupsWizardRepository::SetRoleDesktopFromXml(int roleIndex,const QDomElement &desktops, const QString &backupFolder)
{
    if (QStringLiteral("desktops") == desktops.tagName()) {
        const QDomNodeList desktopsList = desktops.childNodes();

        for (int i = 0; i < desktopsList.count(); ++i) {
            const QDomElement elem = desktopsList.at(i).toElement();

            if (QStringLiteral("desktopApp") != elem.tagName()) {
                break;
            } else {
                DesktopEntity desktopEntity;
                desktopEntity.exec = elem.attribute(QStringLiteral("exec"));
                desktopEntity.name = elem.attribute(QStringLiteral("name"));
                desktopEntity.icon = elem.attribute(QStringLiteral("icon"));
                desktopEntity.type = elem.attribute(QStringLiteral("type"));
                if (m_terminal->IsDirExists(backupFolder, Q_FUNC_INFO, true))
                {
                    if (m_terminal->IsFileExists(backupFolder+desktopEntity.exec, Q_FUNC_INFO, true))
                    {
                        AppendEnittyToRoleDesktops(roleIndex, desktopEntity);
                    }
                    else
                    {
                        qDebug()<< "no file" << backupFolder+desktopEntity.exec;
                    }
                }
                else
                {
                    qDebug()<< "no dir in backup" << backupFolder;
                }
            }
        }
    }
}

void RolesAndStartupsWizardRepository::SetRoleStartupsFromXml(const int roleIndex,const QDomElement &startups, const QString &backupFolder)
{
    QStringList startupList;

    if (QStringLiteral("startups") == startups.tagName()) {
        const QDomNodeList startupsList = startups.childNodes();

        for (int i = 0; i < startupsList.count(); ++i) {
            const QDomElement startup = startupsList.at(i).toElement();

            if (QStringLiteral("startupApp") != startup.tagName()) {
                break;
            } else {
                if (m_terminal->IsDirExists(backupFolder, Q_FUNC_INFO, true))
                {
                    if (m_terminal->IsFileExists(backupFolder+startup.attribute(QStringLiteral("exec")), Q_FUNC_INFO, true))
                    {
                        startupList.append(startup.attribute(QStringLiteral("exec")));
                    }
                    else
                    {
                        qDebug()<< "no file" << backupFolder+startup.attribute(QStringLiteral("exec"));
                    }
                }
                else
                {
                    qDebug()<< "no dir in backup" << backupFolder;
                }
            }
        }
        if(!startupList.isEmpty())
        {
            AppendRoleStartups(roleIndex, startupList);
        }
    }
}

void RolesAndStartupsWizardRepository::AppendEnittyToRoleDesktops(int roleIndex,const DesktopEntity &desktopEntity)
{
    switch (roleIndex) {
    case 0: {
        m_firstRoleDesktopsIcons.push_back(desktopEntity);
        break;
    }

    case 1: {
        m_secondRoleDesktopsIcons.push_back(desktopEntity);
        break;
    }

    case 2: {
        m_thirdRoleDesktopsIcons.push_back(desktopEntity);
        break;
    }

    case 3: {
        m_fourthRoleDesktopsIcons.push_back(desktopEntity);
        break;
    }

    default: {
        qFatal("Роли только 4");
    }
    }
}

const QList<DesktopEntity> &RolesAndStartupsWizardRepository::GetDesktopsByIndex(int roleIndex) const
{
    switch (roleIndex) {
    case 0: {
        return m_firstRoleDesktopsIcons;
        break;
    }

    case 1: {
        return m_secondRoleDesktopsIcons;
        break;
    }

    case 2: {
        return m_thirdRoleDesktopsIcons;
        break;
    }

    case 3: {
        return m_fourthRoleDesktopsIcons;
        break;
    }

    default: {
        qFatal("Роли только 4");
    }
    }
}

void RolesAndStartupsWizardRepository::AppendRoleStartups(int roleIndex, const QStringList &startupList)
{
    switch (roleIndex) {
    case 0: {
        m_firstRoleStartup = startupList;
        break;
    }

    case 1: {
        m_secondRoleStartup = startupList;
        break;
    }

    case 2: {
        m_thirdRoleStartup = startupList;
        break;
    }

    case 3: {
        m_fourthRoleStartup = startupList;
        break;
    }

    default: {
        qFatal("Роли только 4");
    }
    }
}

const QStringList &RolesAndStartupsWizardRepository::GetStatupsByIndex(int roleIndex) const
{
    switch (roleIndex) {
    case 0: {
        return m_firstRoleStartup;
        break;
    }

    case 1: {
        return m_secondRoleStartup;
        break;
    }

    case 2: {
        return m_thirdRoleStartup;
        break;
    }

    case 3: {
        return m_fourthRoleStartup;
        break;
    }

    default: {
        qFatal("Роли только 4");
    }
    }
}


