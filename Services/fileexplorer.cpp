#include "fileexplorer.h"
#include <QDebug>

IconMakingService::IconMakingService(Terminal *terminal, ISqlDatabaseService *sqlDatabaseService)
    : m_terminal(terminal)
    , m_sqlDatabaseService(sqlDatabaseService)
    , m_filesList(new QList<DesktopEntity>())
    , m_fileModel(new QStandardItemModel())
{

}

IconMakingService::~IconMakingService()
{
    delete m_filesList;
    delete m_fileModel;
}

QStandardItemModel *IconMakingService::GetModel()
{
    return m_fileModel;
}

Terminal *IconMakingService::GetTerminal()
{
    return m_terminal;
}

void IconMakingService::CheckRoleDesktopTable(const quint8 &roleId)
{
    m_sqlDatabaseService->CreateDesktopRolesIfNotExists(roleId);
}

//ROLES
void IconMakingService::GetAllRoleDesktops(const quint8 &roleId)
{
    const QList<DesktopEntity> listOfDesktops= m_sqlDatabaseService->GetAllRoleDesktops(roleId);
    UpdateModel(listOfDesktops);
}

void IconMakingService::GetAllUserDesktops(const QString &userName)
{
    const QString userDesktopPath = GetUserDesktopPath(userName);
    CheckUserPath(userDesktopPath);
    UpdateIconListDataAndModel(userName);
}

void IconMakingService::AddIconToUser(const DesktopEntity &entity, const QString &userName)
{
    QString fileName(entity.name);
    fileName.replace(' ', '_');
    CreateIconFile(userName, fileName);
    const QString iconText = CreateIconProperties(entity);
    WriteIconPropertiesToFile(iconText, fileName, userName);
    UpdateIconListDataAndModel(userName);
}

void IconMakingService::DeleteIconToUser(const QString &userName, const QString &iconName)
{
    const QString userDesktopPath = GetUserDesktopPath(userName);
    if(m_terminal->IsDirExists(userDesktopPath, Q_FUNC_INFO, true))
    {
        if (m_terminal->IsFileExists(userDesktopPath+iconName, Q_FUNC_INFO, true))
        {
            m_terminal->DeleteFileSudo(userDesktopPath + iconName, Q_FUNC_INFO);
        }
    }
    UpdateIconListDataAndModel(userName);
}

void IconMakingService::DeleteRoleIconFromUser(const QString &userName, const quint8 &roleId)
{

}

void IconMakingService::CheckUserPath(const QString &pathToDesktop)
{
    if(m_terminal->IsDirNotExists(pathToDesktop, Q_FUNC_INFO, true))
    {
        m_terminal->CheckAndCreatePathToElement(pathToDesktop, Q_FUNC_INFO, true);
        m_terminal->CreateFolder(pathToDesktop, Q_FUNC_INFO, true);
    }
}

void IconMakingService::SetDefaultIconsToUser(const quint8 &oldRoleId, const quint8 &newRoleId,
                                                  const QString &userName)
{
    const QString desktopPath=GetUserDesktopPath(userName);
    CheckUserPath(userName);

    const QList<DesktopEntity> oldDesktops = m_sqlDatabaseService->GetAllRoleDesktops(oldRoleId);
    const QStringList userDesktopNames=GetAllDesktopEntities(userName);
    for (const DesktopEntity &entity: oldDesktops)
    {
        for (const QString &programOnDesktop : userDesktopNames)
        {
            if(entity.name==programOnDesktop)
            {
                if(m_terminal->IsFileExists(desktopPath+programOnDesktop, Q_FUNC_INFO, true))
                {
                    m_terminal->DeleteFileSudo(desktopPath+programOnDesktop, Q_FUNC_INFO);
                }
            }
        }
    }

    const QList<DesktopEntity> newDesktops = m_sqlDatabaseService->GetAllRoleDesktops(newRoleId);
    const QStringList newUserDesktopNames=GetAllDesktopEntities(userName);
    for (const DesktopEntity &entity: newDesktops)
    {
        for (const QString &programOnDesktop : newUserDesktopNames)
        {
            if(entity.name==programOnDesktop)
            {
                if(m_terminal->IsFileExists(desktopPath+programOnDesktop, Q_FUNC_INFO, true))
                {
                    m_terminal->DeleteFileSudo(desktopPath+programOnDesktop, Q_FUNC_INFO);
                }
                AddIconToUser(entity, userName);
            }
        }
    }
    UpdateIconListDataAndModel(userName);
}

void IconMakingService::AddIconToRole(const DesktopEntity &entity, const quint8 &roleId)
{
    DesktopEntity entityinRootFolder;
    entityinRootFolder.name=entity.name;
    entityinRootFolder.type="Application";
    if(!entity.icon.isEmpty())
    {
        QString entityIconName=entity.icon.mid(entity.icon.lastIndexOf('/')+1);
        m_terminal->CheckAndCreatePathToElement(m_destinationFolder, Q_FUNC_INFO, true);
        if (m_terminal->IsDirNotExists(m_destinationFolder, Q_FUNC_INFO, true))
        {
            m_terminal->CreateFolder(m_destinationFolder, Q_FUNC_INFO, true);
        }
        else
        {
            if(m_terminal->IsFileExists(m_destinationFolder+entityIconName, Q_FUNC_INFO, true))
            {
                m_terminal->DeleteFileSudo(m_destinationFolder+entityIconName, Q_FUNC_INFO);
            }
            m_terminal->CopyFileSudo(entity.icon, m_destinationFolder, Q_FUNC_INFO);
        }
        entityinRootFolder.icon=m_destinationFolder+entityIconName;
    }

    QString entityIconName=entity.exec.mid(entity.exec.lastIndexOf('/')+1);
    if(m_terminal->IsFileExists(m_destinationFolder+entityIconName, Q_FUNC_INFO, true))
    {
        m_terminal->DeleteFileSudo(m_destinationFolder+entityIconName, Q_FUNC_INFO);
    }
    m_terminal->CopyFileSudo(entity.exec, m_destinationFolder, Q_FUNC_INFO);
    entityinRootFolder.exec=m_destinationFolder+entityIconName;
    m_sqlDatabaseService->AppendDesktopIntoRole(roleId, entityinRootFolder);
    QStandardItem *item = new QStandardItem();
    item->setData(QVariant::fromValue(entityinRootFolder), Qt::UserRole + 1);
    m_fileModel->appendRow(item);
}

void IconMakingService::DeleteIconToRole(const QString iconName, const quint8 &roleId)
{
    if (m_terminal->IsFileExists(m_destinationFolder+iconName, Q_FUNC_INFO, true))
    {
        m_terminal->DeleteFileSudo(m_destinationFolder+iconName, Q_FUNC_INFO);
    }
    DesktopEntity entity;
    entity.name=iconName;
    m_sqlDatabaseService->RemoveDesktopIntoRole(roleId, entity);
}

void IconMakingService::UpdateIconListDataAndModel(const QString &userName)
{
    UpdateIconsList(userName);
    UpdateModel(*m_filesList);
}

void IconMakingService::UpdateIconsList(const QString &userName)
{
    const QString userDesktopPath=GetUserDesktopPath(userName);
    QStringList allUserDesktopIconsAndFiles = GetAllDesktopEntities(userName);
    m_filesList->clear();

    for ( QString &entity : allUserDesktopIconsAndFiles)
    {
        if (entity.at(entity.count() - 1) == '@' ||
                entity.at(entity.count() - 1) == '*') {
            entity.remove(entity.count() - 1, 1);
        }

        if (IsIcon(entity)) {
            QString entityInfo = m_terminal->GetFileText(
                        userDesktopPath + entity, Q_FUNC_INFO);
            appendIconInfoToList(entity, entityInfo);
        } else {
            appendFileInfoToList(entity);
        }
    }
}

void IconMakingService::UpdateModel(const QList<DesktopEntity> &listOfDesktopEntities)
{
    m_fileModel->clear();

    for (const DesktopEntity &icon: listOfDesktopEntities) {
        QStandardItem *item = new QStandardItem();
        item->setData(QVariant::fromValue(icon), Qt::UserRole + 1);
        m_fileModel->appendRow(item);
    }
}

QString IconMakingService::GetFullIconPath(const QString &iconName)
{
    QDir astraIconsDirs(m_astraIconPath);
    const QStringList dirList = astraIconsDirs.entryList(QDir::Dirs| QDir::NoDot| QDir::NoDotDot);
    for (const QString &dir : dirList)
    {
        QDir subIconsDir(m_astraIconPath+dir);
        const QStringList  pngsList = subIconsDir.entryList(QStringList() << "*.png", QDir::Files);
        for (const QString &png : pngsList)
        {
            if (png.left(png.length()-4)==iconName)
            {
                return m_astraIconPath+dir+"/"+png;
            }
        }
    }
    return m_astraIconPath+m_defaulIconPath;
}

const QString IconMakingService::GetUserDesktopPath(const QString &userName) const
{
    return m_homeName + userName + m_desktopName;
}

QStringList IconMakingService::GetAllDesktopEntities(const QString &userName)
{
    QString desktoPath=GetUserDesktopPath(userName);
    QStringList desktopEntites;
    if (m_terminal->IsDirExists(desktoPath, Q_FUNC_INFO, true))
    {
        desktopEntites = m_terminal->GetFileList(
                    desktoPath, Q_FUNC_INFO, true);
        desktopEntites.removeAll("");
    }
    return desktopEntites;
}

void IconMakingService::appendIconInfoToList(const QString &programName,
                                                 const QString &iconInfo)
{
    DesktopEntity program;
    program.name = programName;
    program.type = "Application";
    QStringList iconInfoList = iconInfo.split('\n');
    iconInfoList.removeLast();

    for (QList<QString>::const_iterator iconInfoLine = iconInfoList.cbegin();
         iconInfoLine != iconInfoList.cend(); ++iconInfoLine) {
        QString parametr = *iconInfoLine;

        if (parametr.startsWith("Exec")) {
            int index = parametr.indexOf("=");
            program.exec = parametr.mid(index + 1);
        } else {
            if (parametr.startsWith("Icon")) {
                int index = parametr.indexOf("=");
                QString iconPath = parametr.mid(index + 1);
                if (!iconPath.contains("/"))
                {
                    iconPath=GetFullIconPath(iconPath);
                }
                program.icon=iconPath;
            }
        }
    }

    m_filesList->append(program);
}

void IconMakingService::appendFileInfoToList(const QString &fileName)
{
    DesktopEntity file;
    file.name = fileName;
    file.icon = m_fileIconPath;
    file.exec = m_path + "/" + fileName;
    file.type = "Файл";
    m_filesList->append(file);
}

bool IconMakingService::IsIcon(const QString &entityName) const
{
    if (entityName.endsWith(".desktop")) {
        return true;
    } else {
        return false;
    }
}

void IconMakingService::CreateIconFile(const QString &userName, const QString &iconName)
{
    const QString pathToDesktop=GetUserDesktopPath(userName);
    CheckUserPath(pathToDesktop);
    const QString pathToDesktopIcon = pathToDesktop + iconName + ".desktop";

    if (m_terminal->IsFileNotExists(pathToDesktopIcon, Q_FUNC_INFO, true)) {
        m_terminal->CreateFile(pathToDesktopIcon, Q_FUNC_INFO, true);
    } else {
        m_terminal->ClearFileSudo(pathToDesktopIcon, Q_FUNC_INFO);
    }
}

const QString IconMakingService::CreateIconProperties(const DesktopEntity &entity) const
{
    QString iconText = "[Desktop Entry]\nType=Application\nExec='" + entity.exec + "'\n";
    iconText += "Name= " + entity.name + "\nName[ru]= " + entity.name +
            "\nIcon= " + entity.icon + "\n";
    return iconText;
}

void IconMakingService::WriteIconPropertiesToFile(const QString &iconProperties, const QString &iconName, const QString &userName)
{
    const QString userDesktopPath=GetUserDesktopPath(userName);
    m_terminal->WriteTextToFileSudo(iconProperties.toLatin1(),
                                    userDesktopPath + iconName + m_iconType,
                                    Q_FUNC_INFO);
}

void IconMakingService::CheckFolderToFileAndFileExists(const QString &iconName)
{
    m_terminal->CheckAndCreatePathToElement(m_path, Q_FUNC_INFO, true);
    if (m_terminal->IsDirNotExists(m_path, Q_FUNC_INFO, true))
    {
        m_terminal->CreateFolder(m_path, Q_FUNC_INFO, true);
    }
    if (m_terminal->IsFileExists(m_path+iconName, Q_FUNC_INFO, true))
    {
        m_terminal->ClearFileSudo(m_path+iconName, Q_FUNC_INFO);
    }
}
