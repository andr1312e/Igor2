#include "roleappsandstartupsettingsrepository.h"

RolesAndStartupsWizardRepository::RolesAndStartupsWizardRepository(Terminal * const terminal)
    : m_terminal(terminal)
{

}

RolesAndStartupsWizardRepository::~RolesAndStartupsWizardRepository()
{

}

bool RolesAndStartupsWizardRepository::HasData(quint8 roleId)
{
    bool isNotEmptyStartups = !GetStatupsByIndex(roleId).isEmpty();
    bool isNotEmptyDesktops= !GetDesktopsByIndex(roleId).isEmpty();
    return (isNotEmptyStartups || isNotEmptyDesktops);
}

void RolesAndStartupsWizardRepository::GetRoleDesktopsAndStartupsFromLocalRepository(const int roleIndex, QList<DesktopEntity> &roleDesktops, QStringList &startups) const
{
    roleDesktops=GetDesktopsByIndex(roleIndex);
    startups=GetStatupsByIndex(roleIndex);
}

void RolesAndStartupsWizardRepository::GetRoleDesktopsAndStartupsFromDb(ISqlDatabaseService *iSqlDatabaseService)
{
    for (int i=0; i<Roles.count(); ++i)
    {
        if(iSqlDatabaseService->CheckStartupTables((quint8)i))
        {
            AppendRoleStartups(i, iSqlDatabaseService->GetAllRoleStartups(i));
        }
        if(iSqlDatabaseService->CheckDesktopTables((quint8)(i)))
        {
            QList<DesktopEntity> desktopEntityList=iSqlDatabaseService->GetAllRoleDesktops(i);
            for (DesktopEntity & entity: desktopEntityList)
            {
                AppendEnittyToRoleDesktops(i, entity);
            }
        }
    }
}

void RolesAndStartupsWizardRepository::SetRoleDesktopsAndStartupsFromBackup(const int &roleIndex,const QDomElement &backupNode, const QString &backupFolder)
{
    if(backupNode.childNodes().count()==2)
    {
        const QDomElement desktops = backupNode.firstChildElement();
        const QDomElement startups = backupNode.lastChildElement();
        SetRoleDesktopFromXml(roleIndex, desktops, backupFolder);
        SetRoleStartupsFromXml(roleIndex, startups, backupFolder);
    }
}

int RolesAndStartupsWizardRepository::GetRoleDesktopsAppCount(const int roleIndex) const
{
    return GetDesktopsByIndex(roleIndex).count();
}

void RolesAndStartupsWizardRepository::SaveRoleDesktopsToDb(ISqlDatabaseService *iSqlDatabaseService, const int roleIndex)
{
    iSqlDatabaseService->CreateDesktopRolesIfNotExists(roleIndex);
    iSqlDatabaseService->ClearDesktopTable(roleIndex);

    QList<DesktopEntity> currentRoleDesktops(GetDesktopsByIndex(roleIndex));

    for (DesktopEntity &enitty : currentRoleDesktops)
    {
        iSqlDatabaseService->AppendDesktopIntoRole(roleIndex, enitty);
    }
}

void RolesAndStartupsWizardRepository::SaveRoleExecsToDb(ISqlDatabaseService *iSqlDatabaseService, const int roleIndex)
{
    iSqlDatabaseService->CreateStartupsTableInNotExists(roleIndex);
    iSqlDatabaseService->ClearStartupsTable(roleIndex);
    const QStringList currentRoleStartups(GetStatupsByIndex(roleIndex));
    for (const QString &exec :currentRoleStartups)
    {
        iSqlDatabaseService->AppendStartupIntoRole(roleIndex, exec);
    }
}

QStringList RolesAndStartupsWizardRepository::GetAllUniqueDesktopExecsAndStarups(const int roleIndex) const
{
    QStringList startupsList=GetStatupsByIndex(roleIndex);
    QList<DesktopEntity> desktopsList=GetDesktopsByIndex(roleIndex);
    for (DesktopEntity &entity: desktopsList)
    {
        startupsList.append(entity.exec);
    }
    startupsList.removeDuplicates();
    return  startupsList;
}

void RolesAndStartupsWizardRepository::SetRoleDesktopFromXml(const int roleIndex,const QDomElement &desktops, const QString &backupFolder)
{
    if (desktops.tagName() == "desktops") {
        QDomNodeList desktopsList = desktops.childNodes();

        for (int i = 0; i < desktopsList.count(); i++) {
            QDomElement elem = desktopsList.at(i).toElement();

            if (elem.tagName() != "desktopApp") {
                break;
            } else {
                DesktopEntity desktopEntity;
                desktopEntity.exec = elem.attribute("exec");
                desktopEntity.name = elem.attribute("name");
                desktopEntity.icon = elem.attribute("icon");
                desktopEntity.type = elem.attribute("type");
                if (m_terminal->IsDirExists(backupFolder, "RolesAndStartupsWizardRepository::SetRoleDesktopFromXml", true))
                {
                    if (m_terminal->IsFileExists(backupFolder+desktopEntity.exec, "RolesAndStartupsWizardRepository::SetRoleDesktopFromXml", true))
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

    if (startups.tagName() == "startups") {
        QDomNodeList startupsList = startups.childNodes();

        for (int i = 0; i < startupsList.count(); i++) {
            QDomElement startup = startupsList.at(i).toElement();

            if (startup.tagName() != "startupApp") {
                break;
            } else {
                if (m_terminal->IsDirExists(backupFolder, "RolesAndStartupsWizardRepository::SetRoleDesktopFromXml", true))
                {
                    if (m_terminal->IsFileExists(backupFolder+startup.attribute("exec"), "RolesAndStartupsWizardRepository::SetRoleDesktopFromXml", true))
                    {
                        startupList.append(startup.attribute("exec"));
                    }
                    else
                    {
                        qDebug()<< "no file" << backupFolder+startup.attribute("exec");
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

void RolesAndStartupsWizardRepository::AppendEnittyToRoleDesktops(const int roleIndex, DesktopEntity &desktopEntity)
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

const QList<DesktopEntity> &RolesAndStartupsWizardRepository::GetDesktopsByIndex(const int roleIndex) const
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

void RolesAndStartupsWizardRepository::AppendRoleStartups(const int roleIndex, const QStringList &startupList)
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

const QStringList &RolesAndStartupsWizardRepository::GetStatupsByIndex(const int roleIndex) const
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


