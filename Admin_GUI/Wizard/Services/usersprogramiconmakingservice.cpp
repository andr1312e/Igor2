#include "usersprogramiconmakingservice.h"

IconMaker::IconMaker(const QString &applicationName, const QString &rlsTiFolder, LinuxUserService *service, QObject *parent)
    : QObject(parent)
    , m_iconName(QStringLiteral("users.desktop"))
    , m_addIconToDesktopToCurrentUser(false)
    , m_addIconToDesktopToAllUsers(false)
    , m_addIconToStartMenu(false)
    , m_applicationName(applicationName)
    , m_rlsTiFolder(rlsTiFolder)
    , m_linuxUserService(service)
    , m_terminal(Terminal::GetTerminal())
{

}

IconMaker::~IconMaker()
{

}

void IconMaker::OnAddIconToDesktopToCurrentUser(bool state)
{
    m_addIconToDesktopToCurrentUser=state;
}

void IconMaker::OnAddIconToDesktopToAllUsers(bool state)
{
    m_addIconToDesktopToAllUsers=state;
}

void IconMaker::OnAddIconToStartMenu(bool state)
{
    m_addIconToStartMenu=state;
}

void IconMaker::ApplyWizardActions()
{
    CopyMyAppToRLSTIFolder();
    if(m_addIconToDesktopToAllUsers)
    {
        AddIconToAllUserDesktops();
    }
    else
    {
        if (m_addIconToDesktopToCurrentUser)
        {
            AddIconToCurrentUserDesktop();
        }
    }

    if (m_addIconToStartMenu)
    {
        AddIconToStartMenu();
    }
}

void IconMaker::CopyMyAppToRLSTIFolder()
{
    if(m_terminal->IsDirNotExists(m_rlsTiFolder, Q_FUNC_INFO, true))
    {
        m_terminal->CreateFolder(m_rlsTiFolder, Q_FUNC_INFO, true);
    }
    const QDir dir;
    const QString sourceFolder(dir.absolutePath()+'/');
    if(m_terminal->IsFileExists(m_rlsTiFolder+m_applicationName, Q_FUNC_INFO, true))
    {
        if(sourceFolder==m_rlsTiFolder)
        {
            return;
        }
        else
        {
            m_terminal->DeleteFileSudo(m_rlsTiFolder+m_applicationName, Q_FUNC_INFO);
        }
    }
    m_terminal->CopyFileSudo(sourceFolder+m_applicationName, m_rlsTiFolder, Q_FUNC_INFO);
}

void IconMaker::AddIconToStartMenu()
{
    const QString startPath(QDir::homePath()+QStringLiteral("/.fly/startmenu/"));
    const QStringList foldersList(m_terminal->GetFolderList(startPath, Q_FUNC_INFO, true));
    const QString rlsTiStartupFolderPath(startPath+QStringLiteral("RLS_TI/"));
    if(foldersList.contains(QStringLiteral("RLS_TI/")))
    {
        m_terminal->ClearFolderSudo(rlsTiStartupFolderPath, Q_FUNC_INFO);
    }
    else
    {
        m_terminal->CreateFolder(rlsTiStartupFolderPath, Q_FUNC_INFO, true);
    }
    const QString directoryIconFilePath(rlsTiStartupFolderPath+QStringLiteral(".directory"));
    m_terminal->CreateFile(directoryIconFilePath, Q_FUNC_INFO, true);

    QString iconString=QStringLiteral("[Desktop Entry]\nName=Group\nName[ru]=РЛС ТИ Программы\nType=Directory\nNoDisplay=false\nIcon=preferences-desktop-locale\nIcon[ru]=preferences-desktop-locale\nHidden=false");
    m_terminal->WriteTextToFileSudo(iconString, directoryIconFilePath, Q_FUNC_INFO);

    const QString syncProgramIconFilePath=rlsTiStartupFolderPath+m_iconName;
    m_terminal->CreateFile(syncProgramIconFilePath, Q_FUNC_INFO, true);

    iconString=QStringLiteral("[Desktop Entry]\nName=Панель управления РЛС ТИ\nName[ru]=Панель управления РЛС ТИ\nType=Application\nExec=/usr/RLS_TI/")+m_applicationName+QStringLiteral("\nIcon=groups-users\nCategories=Utility\nOnlyShowIn=Fly\nEncoding=UTF-8\nX-Fly-OriginFile=/usr/share/applications/fly-admin-iso.desktop");
    m_terminal->WriteTextToFileSudo(iconString, syncProgramIconFilePath, Q_FUNC_INFO);
}

void IconMaker::AddIconToCurrentUserDesktop()
{
    const QString currentUserDesktopPath=QDir::homePath()+QStringLiteral("/Desktop");
    AddIcon(currentUserDesktopPath);
}

void IconMaker::AddIconToAllUserDesktops()
{
    const QList<QPair<QString, QString>> userNameAndIdList(m_linuxUserService->GetSystemUsersNamesWithIds());
    for (const QPair<QString, QString> &nameAndId : userNameAndIdList)
    {
        const QString userDesktopFolderPath(QStringLiteral("/home/")+nameAndId.first+QStringLiteral("/Desktop/"));
        AddIcon(userDesktopFolderPath);
    }
}

void IconMaker::AddIcon(const QString &desktopPath)
{
    if (m_terminal->IsDirNotExists(desktopPath, Q_FUNC_INFO, true))
    {
        m_terminal->CreateFolder(desktopPath, Q_FUNC_INFO, true);
    }
    const QString userFileName=desktopPath+m_iconName;
    if(m_terminal->IsFileExists(userFileName, Q_FUNC_INFO, true))
    {
        m_terminal->ClearFileSudo(userFileName, Q_FUNC_INFO);
    }
    else
    {
        m_terminal->CreateFile(userFileName, Q_FUNC_INFO, true);
    }
    const QString iconString(QStringLiteral("[Desktop Entry]\nName=Панель управления РЛС ТИ\nName[ru]=Панель управления РЛС ТИ\nType=Application\nExec=/usr/RLS_TI/")+m_applicationName + QStringLiteral("\nIcon=groups-users\nCategories=Utility\nOnlyShowIn=Fly\nEncoding=UTF-8\nX-Fly-OriginFile=/usr/share/applications/fly-admin-iso.desktop"));
    m_terminal->WriteTextToFileSudo(iconString, userFileName, Q_FUNC_INFO);
}

