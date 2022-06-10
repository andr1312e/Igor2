#include "userdesktopservice.h"
#include <QDebug>

UserDesktopService::UserDesktopService(ISqlDatabaseService *sqlDatabaseService)
    : DesktopService(sqlDatabaseService)
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
/**
 * Добавляем иконку к пользователю из вью
 */
void UserDesktopService::AddIconToUser(const QString &userName, const DesktopEntity &entityData)
{
    QString fileName(entityData.GetName());
    fileName.replace(' ', '_');
    const QString userDesktopPath(GetUserDesktopPath(userName));
    CheckPath(userDesktopPath);
    CreateIconWithData(userDesktopPath, entityData);
    UpdateIconListDataAndModelFromUserDesktop(userDesktopPath);
}

void UserDesktopService::DeleteIconToUser(const QString &userName, const QString &iconName)
{
    const QString userDesktopPath = GetUserDesktopPath(userName);
    CheckPath(userDesktopPath);
    if (m_terminal->IsFileExists(userDesktopPath + iconName, Q_FUNC_INFO, true))
    {
        m_terminal->DeleteFileSudo(userDesktopPath + iconName, Q_FUNC_INFO);
    }
    UpdateIconListDataAndModelFromUserDesktop(userDesktopPath);
}

void UserDesktopService::DeleteAllIconsToUser(int roleId, const QString &userName)
{
    const QString userDesktopPath = GetUserDesktopPath(userName);
    CheckPath(userDesktopPath);
    const QList<DesktopEntity> entities = m_sqlDatabaseService->GetAllRoleDesktops(roleId);
    for (const DesktopEntity &entity : entities)
    {
        DeleteIcon(userDesktopPath, entity.GetName());
    }
}
/**
 * Обновляем модель, собирая все иконки по указанному пути
 */
void UserDesktopService::UpdateIconListDataAndModelFromUserDesktop(const QString &userDesktopPath)
{
    const QList<DesktopEntity> listOfDesktopIcons = UpdateIconsListFromUserDesktop(userDesktopPath);
    UpdateModel(listOfDesktopIcons);
}

QList<DesktopEntity> UserDesktopService::UpdateIconsListFromUserDesktop(const QString &userDesktopPath)
{
    QList<DesktopEntity> iconsList;
    const QStringList allUserDesktopIconsAndFiles = GetAllDesktopEntities(userDesktopPath);
    for ( const QString &entity : allUserDesktopIconsAndFiles)
    {
        if (IsIcon(entity))
        {
            const QString entityInfo = m_terminal->GetFileText(userDesktopPath + entity, Q_FUNC_INFO);
            iconsList.append(ParseAndAppendIconInfoToList(entity, entityInfo));
        }
    }
    return iconsList;
}

QStringList UserDesktopService::GetAllDesktopEntities(const QString &userDesktopPath)
{
    if (m_terminal->IsDirExists(userDesktopPath, Q_FUNC_INFO, true))
    {
        return m_terminal->GetFileList(userDesktopPath, Q_FUNC_INFO, true);
    }
    return QStringList();
}
/**
 * Парсер текста иконки в структуру
 */
DesktopEntity UserDesktopService::ParseAndAppendIconInfoToList(const QString &programName,
        const QString &iconInfo) const
{
    DesktopEntity program;
    program.SetName(programName);
    QStringList iconDataList = iconInfo.split('\n');
    iconDataList.removeLast();

    for (const QString parametr : iconDataList)
    {

        if (parametr.startsWith(QStringLiteral("Exec")))
        {
            const int index = parametr.indexOf('=');
            Q_ASSERT(index > 0);
            program.SetExec(parametr.mid(index + 1));
        }
        else
        {
            if (parametr.startsWith(QStringLiteral("Icon")))
            {
                const int index = parametr.indexOf('=');
                Q_ASSERT(index > 0);
                QString iconPath = parametr.mid(index + 1);
                if (!iconPath.contains('/'))
                {
                    iconPath = GetFullAstraDefaultIconImagePath(iconPath);
                }
                program.SetIcon(iconPath);
            }
        }
    }

    return  program;
}
/**
 * Проверяем иконка ли по концу имени файла
 * @return true- икона, иначе не иконка
 */
bool UserDesktopService::IsIcon(const QString &entityName) const noexcept
{
    return entityName.endsWith(QStringLiteral(".desktop"));
}
