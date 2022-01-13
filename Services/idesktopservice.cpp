#include <QDir>
#include <QStandardItemModel>

#include "Services/idesktopservice.h"

DesktopService::DesktopService(ISqlDatabaseService *sqlDatabaseService)
    : m_terminal (Terminal::GetTerminal())
    , m_sqlDatabaseService(sqlDatabaseService)
    , m_fileModel(new QStandardItemModel())
{

}

DesktopService::~DesktopService()
{
    delete m_fileModel;
}

QStandardItemModel *DesktopService::GetModel()
{
    return m_fileModel;
}

Terminal *DesktopService::GetTerminal()
{
    return m_terminal;
}

DesktopEntity DesktopService::MoveFilesToProgramFolder(const DesktopEntity &entity)
{
    DesktopEntity movedEntity=entity;
    movedEntity.exec=CopyExecFile(entity.exec);
    movedEntity.icon=CopyIconFile(entity.icon);
    return  movedEntity;
}

QString DesktopService::CopyExecFile(const QString &pathToDesktopExec)
{
    const int indexOfFileNameBegin=pathToDesktopExec.lastIndexOf('/');
    const QString execFileName=pathToDesktopExec.mid(indexOfFileNameBegin+1);
    const QStringRef execFolderName=pathToDesktopExec.leftRef(indexOfFileNameBegin+1);
    if(m_destinationFolder==execFolderName)
    {
        return pathToDesktopExec;
    }
    else
    {
        if(m_terminal->IsDirNotExists(m_destinationFolder, Q_FUNC_INFO, true))
        {
            m_terminal->CreateFolder(m_destinationFolder, Q_FUNC_INFO, true);
        }
        if(m_terminal->IsFileExists(m_destinationFolder+execFileName, Q_FUNC_INFO, true))
        {
            m_terminal->DeleteFileSudo(m_destinationFolder+execFileName, Q_FUNC_INFO);
        }
        m_terminal->CopyFileSudo(pathToDesktopExec, m_destinationFolder, Q_FUNC_INFO);
        return m_destinationFolder+execFileName;
    }
}

QString DesktopService::CopyIconFile(const QString &pathToDesktopIcon)
{
    const int indexOfIconNameBegin=pathToDesktopIcon.lastIndexOf('/');
    const QString iconFileName=pathToDesktopIcon.mid(indexOfIconNameBegin+1);
    const QStringRef iconFolderName=pathToDesktopIcon.leftRef(indexOfIconNameBegin+1);
    if(m_destinationFolder==pathToDesktopIcon)
    {
        return pathToDesktopIcon;
    }
    else
    {
        if(m_terminal->IsFileExists(m_destinationFolder+iconFileName, Q_FUNC_INFO, true))
        {
            m_terminal->DeleteFileSudo(m_destinationFolder+iconFileName, Q_FUNC_INFO);
        }
        m_terminal->CopyFileSudo(pathToDesktopIcon, m_destinationFolder, Q_FUNC_INFO);
        return m_destinationFolder+iconFileName;
    }
}

void DesktopService::CheckPath(const QString &pathToDesktop)
{
    if(m_terminal->IsDirNotExists(pathToDesktop, Q_FUNC_INFO, true))
    {
        m_terminal->CheckAndCreatePathToElement(pathToDesktop, Q_FUNC_INFO, true);
        m_terminal->CreateFolder(pathToDesktop, Q_FUNC_INFO, true);
    }
}

void DesktopService::CreateIconWithData(const QString &userDesktopPath, const DesktopEntity &entity)
{
    CreateIcon(userDesktopPath, entity.name);
    const QString iconProperties=CreateIconData(entity);
    WriteIconDataToFile(userDesktopPath, iconProperties, entity.name);
}

void DesktopService::DeleteIcon(const QString &pathToDesktop, const QString &desktopName)
{
    const QString fullPathToIcon=pathToDesktop+desktopName+m_iconType;
    if (m_terminal->IsFileExists(fullPathToIcon, Q_FUNC_INFO, true))
    {
        m_terminal->DeleteFileSudo(fullPathToIcon, Q_FUNC_INFO);
    }
}

void DesktopService::CreateIcon(const QString &userDesktopPath, const QString &iconName)
{
    const QString fullPathToIcon = userDesktopPath + iconName + m_iconType;
    if (m_terminal->IsFileNotExists(fullPathToIcon, Q_FUNC_INFO, true)) {
        m_terminal->CreateFile(fullPathToIcon, Q_FUNC_INFO, true);
    } else {
        m_terminal->ClearFileSudo(fullPathToIcon, Q_FUNC_INFO);
    }
}

const QString DesktopService::CreateIconData(const DesktopEntity &entity) const
{
    QString iconText = "[Desktop Entry]\nType=Application\nExec='" + entity.exec + "'\n";
    iconText += "Name="+ entity.name + "\nName[ru]="+ entity.name +
            "\nIcon="+ entity.icon + "\n";
    return iconText;
}

void DesktopService::WriteIconDataToFile( const QString &userDesktopPath, const QString &iconProperties, const QString &iconName)
{
    m_terminal->WriteTextToFileSudo(iconProperties.toUtf8(),
                                    userDesktopPath + iconName + m_iconType,
                                    Q_FUNC_INFO);
}

const QString DesktopService::GetUserDesktopPath(const QString &userName) const
{
    return m_homeFolderPathName + userName + m_desktopName;
}

void DesktopService::UpdateModel(const QList<DesktopEntity> &listOfDesktopEntities)
{
    m_fileModel->clear();

    for (const DesktopEntity &icon: listOfDesktopEntities) {
        QStandardItem *item = new QStandardItem();
        item->setData(QVariant::fromValue(icon), Qt::UserRole + 1);
        m_fileModel->appendRow(item);
    }
}

QString DesktopService::GetFullAstraDefaultIconImagePath(const QString &iconName)
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
