#include "roleappsandstartupsettingsrepository.h"

#include <Structs/user.h>

RolesAndStartupsWizardRepository::RolesAndStartupsWizardRepository(Terminal *terminal)
    : m_terminal(terminal)
    , m_rolesStartup(Roles.count())
    , m_roleDesktopsIcons(Roles.count())
{

}

RolesAndStartupsWizardRepository::~RolesAndStartupsWizardRepository()
{

}

bool RolesAndStartupsWizardRepository::HasData(int roleIndex) const
{
    const bool isNotEmptyStartups = !GetStatupsByIndex(roleIndex).isEmpty();
    const bool isNotEmptyDesktops = !GetDesktopsByIndex(roleIndex).isEmpty();
    return (isNotEmptyStartups || isNotEmptyDesktops);
}

void RolesAndStartupsWizardRepository::GetRoleDesktopsAndStartupsFromLocalRepository(int roleIndex, QList<DesktopEntity> &roleDesktops, QStringList &startups) const
{
    roleDesktops = GetDesktopsByIndex(roleIndex);
    startups = GetStatupsByIndex(roleIndex);
}

void RolesAndStartupsWizardRepository::GetRoleDesktopsAndStartupsFromDb(ISqlDatabaseService *iSqlDatabaseService)
{
    for (int i = 0; i < Roles.count(); ++i)
    {
        if (iSqlDatabaseService->CheckStartupTables(i))
        {
            AppendRoleStartups(i, iSqlDatabaseService->GetAllRoleStartups(i));
        }
        if (iSqlDatabaseService->CheckDesktopTables(i))
        {
            const QList<DesktopEntity> desktopEntityList = iSqlDatabaseService->GetAllRoleDesktops(i);
            for (const DesktopEntity &entity : qAsConst(desktopEntityList))
            {
                AppendEnittyToRoleDesktops(i, entity);
            }
        }
    }
}

void RolesAndStartupsWizardRepository::SetRoleDesktopsAndStartupsFromBackup(int roleIndex, const QDomElement &backupNode, const QString &backupFolder)
{
    if (2 == backupNode.childNodes().count())
    {
        const QDomElement desktops(backupNode.firstChildElement());
        const QDomElement startups(backupNode.lastChildElement());
        SetRoleDesktopFromXml(roleIndex, desktops, backupFolder);
        SetRoleStartupsFromXml(roleIndex, startups, backupFolder);
    }
    else
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Роль записана не корректно, должно быть два дочерних подтега. А их: ") + QString::number(backupNode.childNodes().count()));
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
    for (const QString &exec : currentRoleStartups)
    {
        iSqlDatabaseService->AppendStartupIntoRole(roleIndex, exec);
    }
}

QStringList RolesAndStartupsWizardRepository::GetAllUniqueDesktopExecsAndStarups(int roleIndex) const
{
    QStringList startupsList = GetStatupsByIndex(roleIndex);
    const QList<DesktopEntity> desktopsList = GetDesktopsByIndex(roleIndex);
    for (const DesktopEntity &entity : desktopsList)
    {
        startupsList.append(entity.GetExec());
    }
    startupsList.removeDuplicates();
    return  startupsList;
}

void RolesAndStartupsWizardRepository::Clear()
{
    for (QStringList &rolesStartupList : m_rolesStartup)
    {
        rolesStartupList.clear();
    }
    for (QList<DesktopEntity> &rolesDesktopList : m_roleDesktopsIcons)
    {
        rolesDesktopList.clear();
    }
}

void RolesAndStartupsWizardRepository::SetRoleDesktopFromXml(int roleId, const QDomElement &desktopTag, const QString &backupFolder)
{
    if (QLatin1Literal("desktops") == desktopTag.tagName())
    {
        const QDomNodeList desktopsList = desktopTag.childNodes();
        if (desktopsList.isEmpty())
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Подтэг <desktops/> чайлдфри. Ид роли:" ) + QString::number(roleId));
        }
        else
        {
            for (int i = 0; i < desktopsList.count(); ++i)
            {
                const QDomElement desktopAppTag = desktopsList.at(i).toElement();
                if (QLatin1Literal("desktopApp") == desktopAppTag.tagName())
                {
                    const DesktopEntity desktopEntity(desktopAppTag.attribute(QLatin1Literal("name")), desktopAppTag.attribute(QLatin1Literal("exec")), desktopAppTag.attribute(QLatin1Literal("icon")));
                    if (m_terminal->IsDirExists(backupFolder, Q_FUNC_INFO, true))
                    {
                        if (m_terminal->IsFileExists(backupFolder + desktopEntity.GetExec(), Q_FUNC_INFO, true))
                        {
                            AppendEnittyToRoleDesktops(roleId, desktopEntity);
                        }
                        else
                        {
                            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Файл восстановления есть, а исполняемого файла который в нем написан нет! Файл: ") + backupFolder + desktopEntity.GetExec());
                        }
                    }
                    else
                    {
                        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Папка бэкапа была удалена. Папка: ") + backupFolder);
                    }
                }
                else
                {
                    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Подтэг <desktops/> не <desktopApp/>. А: ") + desktopTag.tagName());
                }
            }
        }
    }
    else
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Первый тэг не <desktops/>. А: ") + desktopTag.tagName());
    }
}

void RolesAndStartupsWizardRepository::SetRoleStartupsFromXml(int roleId, const QDomElement &startupsTag, const QString &backupFolder)
{
    QStringList startupList;
    if (QLatin1Literal("startups") == startupsTag.tagName())
    {
        const QDomNodeList startupsList = startupsTag.childNodes();
        for (int i = 0; i < startupsList.count(); ++i)
        {
            const QDomElement startupAppTag = startupsList.at(i).toElement();
            if (QLatin1Literal("startupApp") == startupAppTag.tagName())
            {
                if (m_terminal->IsDirExists(backupFolder, Q_FUNC_INFO, true))
                {
                    const QString fileName = startupAppTag.text();
                    if (m_terminal->IsFileExists(backupFolder + fileName, Q_FUNC_INFO, true))
                    {
                        startupList.append(fileName);
                    }
                    else
                    {
                        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Файл восстановления есть, а файла перезапуска нет. Файл: ") + backupFolder + fileName);
                    }
                }
                else
                {
                    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Папка бэкапа была удалена. Папка: ") + backupFolder);
                }
            }
            else
            {
                Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Подтэг <startups/> не <startupApp/>. А: ") + startupAppTag.tagName());
            }
        }
        if (!startupList.isEmpty())
        {
            AppendRoleStartups(roleId, startupList);
        }
    }
    else
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Первый тэг не <startups/>. А: ") + startupsTag.tagName());
    }
}

void RolesAndStartupsWizardRepository::AppendEnittyToRoleDesktops(int roleIndex, const DesktopEntity &desktopEntity)
{
    Q_ASSERT(User::RoleIsValid(roleIndex));
    m_roleDesktopsIcons[roleIndex].append(desktopEntity);
}

const QList<DesktopEntity> &RolesAndStartupsWizardRepository::GetDesktopsByIndex(int roleIndex) const
{
    Q_ASSERT(User::RoleIsValid(roleIndex));
    return m_roleDesktopsIcons.at(roleIndex);
}

void RolesAndStartupsWizardRepository::AppendRoleStartups(int roleIndex, const QStringList &startupList)
{
    Q_ASSERT(User::RoleIsValid(roleIndex));
    m_rolesStartup[roleIndex] = startupList;
}

const QStringList &RolesAndStartupsWizardRepository::GetStatupsByIndex(int roleIndex) const
{
    Q_ASSERT(User::RoleIsValid(roleIndex));
    return m_rolesStartup.at(roleIndex);
}


