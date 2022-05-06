#include "userdesktopservice.h"
#include <QDebug>

UserDesktopService::UserDesktopService(ISqlDatabaseService *sqlDatabaseService)
    : DesktopService(sqlDatabaseService)
    , m_desktopFileType(".desktop")
{

}

UserDesktopService::~UserDesktopService()
{
}

void UserDesktopService::GetAllUserDesktops(const QString &userName)
{
    const QString userDesktopPath = GetUserDesktopPath(userName);
    CheckPath(userDesktopPath);
    UpdateIconListDataAndModelFromUserDesktop(userDesktopPath);
}

void UserDesktopService::AddIconToUser(const QString &userName, const DesktopEntity &entityData)
{
    QString fileName(entityData.name);
    fileName.replace(' ', '_');
    const QString userDesktopPath = GetUserDesktopPath(userName);
    CheckPath(userDesktopPath);
    CreateIconWithData(userDesktopPath, entityData);
    UpdateIconListDataAndModelFromUserDesktop(userDesktopPath);
}

void UserDesktopService::DeleteIconToUser(const QString &userName, const QString &iconName)
{
    const QString userDesktopPath = GetUserDesktopPath(userName);
    CheckPath(userDesktopPath);
    if (m_terminal->IsFileExists(userDesktopPath+iconName, Q_FUNC_INFO, true))
    {
        m_terminal->DeleteFileSudo(userDesktopPath + iconName, Q_FUNC_INFO);
    }
    UpdateIconListDataAndModelFromUserDesktop(userDesktopPath);
}

void UserDesktopService::DeleteAllIconsToUser(const int &roleId, const QString &userName)
{
    const QString userDesktopPath = GetUserDesktopPath(userName);
    CheckPath(userDesktopPath);
    const QList<DesktopEntity> entities=m_sqlDatabaseService->GetAllRoleDesktops(roleId);
    for (const DesktopEntity &entity : entities)
    {
        DeleteIcon(userDesktopPath, entity.name);
    }
}

void UserDesktopService::UpdateIconListDataAndModelFromUserDesktop(const QString &userDesktopPath)
{
    UpdateIconsListFromUserDesktop(userDesktopPath);
    UpdateModel(m_filesList);
}

void UserDesktopService::UpdateIconsListFromUserDesktop(const QString &userDesktopPath)
{
    m_filesList.clear();
    QStringList allUserDesktopIconsAndFiles = GetAllDesktopEntities(userDesktopPath);

    for ( QString &entity : allUserDesktopIconsAndFiles)
    {
        if ('@' == entity.back() ||
                '*' == entity.back()) {
            entity.chop(1);
        }

        if (IsIcon(entity)) {
            const QString entityInfo = m_terminal->GetFileText(
                        userDesktopPath + entity, Q_FUNC_INFO);
            ParseAndAppendIconInfoToList(entity, entityInfo);
        } else {
            ParseAndAppendFileInfoToList(entity);
        }
    }
}

QStringList UserDesktopService::GetAllDesktopEntities(const QString &userDesktopPath)
{
    QStringList desktopEntitesNames;
    if (m_terminal->IsDirExists(userDesktopPath, Q_FUNC_INFO, true))
    {
        desktopEntitesNames = m_terminal->GetFileList(
                    userDesktopPath, Q_FUNC_INFO, true);
    }
    return desktopEntitesNames;
}

void UserDesktopService::ParseAndAppendIconInfoToList(const QString &programName,
                                              const QString &iconInfo)
{
    DesktopEntity program;
    program.name = programName;
    program.type = "Application";
    QStringList iconDataList = iconInfo.split('\n');
    iconDataList.removeLast();

    for (const QString parametr : iconDataList) {

        if (parametr.startsWith(QStringLiteral("Exec"))) {
            int index = parametr.indexOf('=');
            Q_ASSERT(index>0);
            program.exec = parametr.mid(index + 1);
        } else {
            if (parametr.startsWith(QStringLiteral("Icon"))) {
                int index = parametr.indexOf('=');
                Q_ASSERT(index>0);
                QString iconPath = parametr.mid(index + 1);
                if (!iconPath.contains('/'))
                {
                    iconPath=GetFullAstraDefaultIconImagePath(iconPath);
                }
                program.icon=iconPath;
            }
        }
    }

    m_filesList.push_back(program);
}

void UserDesktopService::ParseAndAppendFileInfoToList(const QString &fileName)
{
    DesktopEntity file;
    file.name = fileName;
    file.icon = m_fileIconPath;
    file.exec = m_path + '/' + fileName;
    file.type = QStringLiteral("Файл");
    m_filesList.push_back(file);
}

bool UserDesktopService::IsIcon(const QString &entityName) const
{
    if (entityName.endsWith(QStringLiteral(".desktop"))) {
        return true;
    } else {
        return false;
    }
}
