#include "fileexplorer.h"
#include <QDebug>

DesktopPanelPresenter::DesktopPanelPresenter(Terminal *terminal, ISqlDatabaseService *sqlDatabaseService)
    : m_terminal(terminal)
    , m_sqlDatabaseService(sqlDatabaseService)
    , m_filesList(new QList<DesktopEntity>())
    , m_fileModel(new QStandardItemModel())
{

}

DesktopPanelPresenter::~DesktopPanelPresenter()
{
    delete m_filesList;
    delete m_fileModel;
}

QStandardItemModel *DesktopPanelPresenter::GetModel()
{
    return m_fileModel;
}

Terminal *DesktopPanelPresenter::GetTerminal()
{
    return m_terminal;
}
void DesktopPanelPresenter::CheckRoleDesktopTable(const quint8 &roleId)
{
    m_sqlDatabaseService->CreateDesktopRolesIfNotExists(roleId);
}

//ROLES
void DesktopPanelPresenter::GetAllRoleDesktops(const quint8 &roleId)
{
    QList<DesktopEntity> listOfDesktops= m_sqlDatabaseService->GetAllRoleDesktops(roleId);
    UpdateModel(listOfDesktops);
}

void DesktopPanelPresenter::GetAllUserDesktops(const QString &userName)
{
    const QString userDesktopPath = GetUserDesktopPath(userName);
    CheckUserPath(userDesktopPath);
    UpdateIconListDataAndModel(userName);
}

void DesktopPanelPresenter::AddIconToUser(const DesktopEntity &entity, const QString &userName)
{
    QString fileName(entity.name);
    fileName.replace(' ', '_');
    CreateIconFile(userName, fileName);
    const QString iconText = CreateIconProperties(entity);
    WriteIconPropertiesToFile(iconText, fileName, userName);
    UpdateIconListDataAndModel(userName);
}

void DesktopPanelPresenter::DeleteIconToUser(const QString &userName, const QString &iconName)
{
    const QString userDesktopPath = GetUserDesktopPath(userName);
    if(m_terminal->IsDirExists(userDesktopPath, "DesktopPanelPresenter::DeleteIconToUser", true))
    {
        if (m_terminal->IsFileExists(userDesktopPath+iconName, "DesktopPanelPresenter::DeleteIconToUser", true))
        {
            m_terminal->DeleteFileSudo(userDesktopPath + iconName, "DesktopPanelPresenter::DeleteIconToUser");
        }
    }
    UpdateIconListDataAndModel(userName);
}

void DesktopPanelPresenter::DeleteRoleIconFromUser(const QString &userName, const quint8 &roleId)
{

}

void DesktopPanelPresenter::CheckUserPath(const QString &pathToDesktop)
{
    if(m_terminal->IsDirNotExists(pathToDesktop, "DesktopPanelPresenter::CheckUserDesktopPath", true))
    {
        m_terminal->CheckAndCreatePathToElement(pathToDesktop, "DesktopPanelPresenter::CheckUserDesktopPath", true);
        m_terminal->CreateFolder(pathToDesktop, "DesktopPanelPresenter::CheckUserDesktopPath", true);
    }
}

void DesktopPanelPresenter::SetDefaultIconsToUser(const quint8 &oldRoleId, const quint8 &newRoleId,
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
                if(m_terminal->IsFileExists(desktopPath+programOnDesktop, "DesktopPanelPresenter::SetDefaultIconsToUser", true))
                {
                    m_terminal->DeleteFileSudo(desktopPath+programOnDesktop, "DesktopPanelPresenter::SetDefaultIconsToUser");
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
                if(m_terminal->IsFileExists(desktopPath+programOnDesktop, "DesktopPanelPresenter::SetDefaultIconsToUser", true))
                {
                    m_terminal->DeleteFileSudo(desktopPath+programOnDesktop, "DesktopPanelPresenter::SetDefaultIconsToUser");
                }
                AddIconToUser(entity, userName);
            }
        }
    }
    UpdateIconListDataAndModel(userName);
}

void DesktopPanelPresenter::AddIconToRole(const DesktopEntity &entity, const quint8 &roleId)
{
    DesktopEntity entityinRootFolder;
    entityinRootFolder.name=entity.name;
    entityinRootFolder.type="Application";
    if(!entity.icon.isEmpty())
    {
        QString entityIconName=entity.icon.mid(entity.icon.lastIndexOf('/')+1);
        m_terminal->CheckAndCreatePathToElement(m_destinationFolder, "DesktopPanelPresenter::AddIconToRole", true);
        if (m_terminal->IsDirNotExists(m_destinationFolder, "DesktopPanelPresenter::AddIconToRole", true))
        {
            m_terminal->CreateFolder(m_destinationFolder, "DesktopPanelPresenter::AddIconToRole", true);
        }
        else
        {
            if(m_terminal->IsFileExists(m_destinationFolder+entityIconName, "DesktopPanelPresenter::AddIconToRole", true))
            {
                m_terminal->DeleteFileSudo(m_destinationFolder+entityIconName, "DesktopPanelPresenter::AddIconToRole");
            }
            m_terminal->CopyFileSudo(entity.icon, m_destinationFolder, "DesktopPanelPresenter::AddIconToRole");
        }
        entityinRootFolder.icon=m_destinationFolder+entityIconName;
    }

    QString entityIconName=entity.exec.mid(entity.exec.lastIndexOf('/')+1);
    if(m_terminal->IsFileExists(m_destinationFolder+entityIconName, "DesktopPanelPresenter::AddIconToRole", true))
    {
        m_terminal->DeleteFileSudo(m_destinationFolder+entityIconName, "DesktopPanelPresenter::AddIconToRole");
    }
    m_terminal->CopyFileSudo(entity.exec, m_destinationFolder, "DesktopPanelPresenter::AddIconToRole");
    entityinRootFolder.exec=m_destinationFolder+entityIconName;
    m_sqlDatabaseService->AppendDesktopIntoRole(roleId, entityinRootFolder);
    QStandardItem *item = new QStandardItem();
    item->setData(QVariant::fromValue(entityinRootFolder), Qt::UserRole + 1);
    m_fileModel->appendRow(item);
}

void DesktopPanelPresenter::DeleteIconToRole(const QString iconName, const quint8 &roleId)
{
    if (m_terminal->IsFileExists(m_destinationFolder+iconName, "DesktopPanelPresenter::AddIconToRole", true))
    {
        m_terminal->DeleteFileSudo(m_destinationFolder+iconName, "DesktopPanelPresenter::AddIconToRole");
    }
    DesktopEntity entity;
    entity.name=iconName;
    m_sqlDatabaseService->RemoveDesktopIntoRole(roleId, entity);
}

void DesktopPanelPresenter::UpdateIconListDataAndModel(const QString &userName)
{
    UpdateIconsList(userName);
    UpdateModel(*m_filesList);
}

void DesktopPanelPresenter::UpdateIconsList(const QString &userName)
{
    QString userDesktopPath=GetUserDesktopPath(userName);
    QStringList allUserDesktopIconsAndFiles = GetAllDesktopEntities(userName);
    m_filesList->clear();

    for (QString &entity : allUserDesktopIconsAndFiles)
    {
        if (entity.at(entity.count() - 1) == '@' ||
                entity.at(entity.count() - 1) == '*') {
            entity.remove(entity.count() - 1, 1);
        }

        if (IsIcon(entity)) {
            QString entityInfo = m_terminal->GetFileText(
                        userDesktopPath + entity, "FileExplorer::updateIconsList");
            appendIconInfoToList(entity, entityInfo);
        } else {
            appendFileInfoToList(entity);
        }
    }
}

void DesktopPanelPresenter::UpdateModel(const QList<DesktopEntity> &listOfDesktopEntities)
{
    m_fileModel->clear();

    for (const DesktopEntity &icon: listOfDesktopEntities) {
        QStandardItem *item = new QStandardItem();
        item->setData(QVariant::fromValue(icon), Qt::UserRole + 1);
        m_fileModel->appendRow(item);
    }
}

QString DesktopPanelPresenter::GetFullIconPath(const QString &iconName)
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

const QString DesktopPanelPresenter::GetUserDesktopPath(const QString &userName) const
{
    return m_homeName + userName + m_desktopName;
}

QStringList DesktopPanelPresenter::GetAllDesktopEntities(const QString &userName)
{
    QString desktoPath=GetUserDesktopPath(userName);
    QStringList desktopEntites;
    if (m_terminal->IsDirExists(desktoPath, "DesktopPanelPresenter::GetAllDesktopEntities", true))
    {
        desktopEntites = m_terminal->GetFileList(
                    desktoPath, "DesktopPanelPresenter::GetAllDesktopEntities", true);
        desktopEntites.removeAll("");
    }
    return desktopEntites;
}

void DesktopPanelPresenter::appendIconInfoToList(const QString &programName,
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

void DesktopPanelPresenter::appendFileInfoToList(const QString &fileName)
{
    DesktopEntity file;
    file.name = fileName;
    file.icon = m_fileIconPath;
    file.exec = m_path + "/" + fileName;
    file.type = "Файл";
    m_filesList->append(file);
}

bool DesktopPanelPresenter::IsIcon(const QString &entityName) const
{
    if (entityName.endsWith(".desktop")) {
        return true;
    } else {
        return false;
    }
}

void DesktopPanelPresenter::CreateIconFile(const QString userName, const QString &iconName)
{
    const QString pathToDesktop=GetUserDesktopPath(userName);
    CheckUserPath(pathToDesktop);
    const QString pathToDesktopIcon = pathToDesktop + iconName + ".desktop";

    if (m_terminal->IsFileNotExists(pathToDesktopIcon, "DesktopPanelPresenter::CreateIconFile", true)) {
        m_terminal->CreateFile(pathToDesktopIcon, "DesktopPanelPresenter::CreateIconFile", true);
    } else {
        m_terminal->ClearFileSudo(pathToDesktopIcon, "DesktopPanelPresenter::CreateIconFile");
    }
}

const QString DesktopPanelPresenter::CreateIconProperties(const DesktopEntity &entity) const
{
    QString iconText = "[Desktop Entry]\nType=Application\nExec='" + entity.exec + "'\n";
    iconText += "Name= " + entity.name + "\nName[ru]= " + entity.name +
            "\nIcon= " + entity.icon + "\n";
    return iconText;
}

void DesktopPanelPresenter::WriteIconPropertiesToFile(const QString &iconProperties, const QString &iconName, const QString &userName)
{
    const QString userDesktopPath=GetUserDesktopPath(userName);
    m_terminal->WriteTextToFileSudo(iconProperties.toLatin1(),
                                    userDesktopPath + iconName + m_iconType,
                                    "DesktopPanelPresenter::WriteIconPropertiesToFile");
}

void DesktopPanelPresenter::CheckFolderToFileAndFileExists(const QString &iconName)
{
    m_terminal->CheckAndCreatePathToElement(m_path, "FileExplorer::AddIcon", true);
    if (m_terminal->IsDirNotExists(m_path, "FileExplorer::AddIcon", true))
    {
        m_terminal->CreateFolder(m_path, "FileExplorer::AddIcon", true);
    }
    if (m_terminal->IsFileExists(m_path+iconName, "FileExplorer::AddIcon", true))
    {
        m_terminal->ClearFileSudo(m_path+iconName, "FileExplorer::AddIcon");
    }
}
