#include "usersprogramiconmakingservice.h"

UsersProgramIconMakingService::UsersProgramIconMakingService(const QString &destinationFolder, const QString &applicationName, LinuxUserService *service, QObject *parent)
    : QObject(parent)
    , m_addIconToDesktopToCurrentUser(false)
    , m_addIconToDesktopToAllUsers(false)
    , m_addIconToStartMenu(false)
    , m_applicationName(applicationName)
    , m_destinationFolder(destinationFolder)
    , m_linuxUserService(service)
    , m_terminal(service->GetTerminal())
{

}

UsersProgramIconMakingService::~UsersProgramIconMakingService()
{

}

void UsersProgramIconMakingService::OnAddIconToDesktopToCurrentUser(bool state)
{
    m_addIconToDesktopToCurrentUser=state;
}

void UsersProgramIconMakingService::OnAddIconToDesktopToAllUsers(bool state)
{
    m_addIconToDesktopToAllUsers=state;
}

void UsersProgramIconMakingService::OnAddIconToStartMenu(bool state)
{
    m_addIconToStartMenu=state;
}

void UsersProgramIconMakingService::ApplyWizardActions()
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

void UsersProgramIconMakingService::CopyMyAppToRLSTIFolder()
{
    if(m_terminal->IsDirNotExists(m_destinationFolder, "UsersProgramIconMakingService::CopyMyAppToRLSTIFolder", true))
    {
        m_terminal->CreateFolder(m_destinationFolder, "UsersProgramIconMakingService::CopyMyAppToRLSTIFolder", true);
    }
    QDir dir;
    const QString sourceFolder=dir.absolutePath();
    if(m_terminal->IsFileExists(m_destinationFolder+m_applicationName, "UsersProgramIconMakingService::CopyMyAppToRLSTIFolder", true))
    {
        if(sourceFolder==m_destinationFolder)
        {
            return;
        }
        else
        {
            m_terminal->DeleteFileSudo(m_destinationFolder+m_applicationName, "UsersProgramIconMakingService::CopyMyAppToRLSTIFolder");
        }
    }
    m_terminal->CopyFileSudo(sourceFolder+m_applicationName, m_destinationFolder, "UsersProgramIconMakingService::CopyMyAppToRLSTIFolder");
}

void UsersProgramIconMakingService::AddIconToStartMenu()
{
    const QString startPath=QDir::homePath()+"/.fly/startmenu/";
    const QStringList foldersList=m_terminal->GetFolderList(startPath, "UsersProgramIconMakingService::AddIconToStartMenu", true);
    const QString rlsTiStartupFolderPath=startPath+"RLS_TI/";
    if(foldersList.contains("RLS_TI/"))
    {
        m_terminal->ClearFolderSudo(rlsTiStartupFolderPath, "UsersProgramIconMakingService::AddIconToStartMenu");
    }
    else
    {
        m_terminal->CreateFolder(rlsTiStartupFolderPath, "UsersProgramIconMakingService::AddIconToStartMenu", true);
    }
    const QString directoryIconFilePath=rlsTiStartupFolderPath+".directory";
    m_terminal->CreateFile(directoryIconFilePath, "UsersProgramIconMakingService::AddIconToStartMenu", true);

    QString iconString="[Desktop Entry]\nName=Group\nName[ru]=РЛС ТИ Программы\nType=Directory\nNoDisplay=false\nIcon=preferences-desktop-locale\nIcon[ru]=preferences-desktop-locale\nHidden=false";
    m_terminal->WriteTextToFileSudo(iconString.toUtf8(), directoryIconFilePath, "UsersProgramIconMakingService::AddIconToStartMenu");

    const QString syncProgramIconFilePath=rlsTiStartupFolderPath+m_iconName;
    m_terminal->CreateFile(syncProgramIconFilePath, "UsersProgramIconMakingService::AddIconToStartMenu", true);

    iconString="[Desktop Entry]\nName=Панель управления РЛС ТИ\nName[ru]=Панель управления РЛС ТИ\nType=Application\nExec=/usr/RLS_TI/"+m_applicationName+"\nIcon=groups-users\nCategories=Utility\nOnlyShowIn=Fly\nEncoding=UTF-8\nX-Fly-OriginFile=/usr/share/applications/fly-admin-iso.desktop";
    m_terminal->WriteTextToFileSudo(iconString.toUtf8(), syncProgramIconFilePath, "UsersProgramIconMakingService::AddIconToStartMenu");
}

void UsersProgramIconMakingService::AddIconToCurrentUserDesktop()
{
    const QString syncProgramIconDirPath=QDir::homePath()+"/Desktop";
    if(m_terminal->IsDirNotExists(syncProgramIconDirPath, "UsersProgramIconMakingService::AddIconToCurrentUserDesktop", true))
    {
        m_terminal->CreateFolder(syncProgramIconDirPath, "UsersProgramIconMakingService::AddIconToCurrentUserDesktop", true);
    }
    const QString syncProgramIconFilePath=syncProgramIconDirPath+"/"+m_iconName;
    if(m_terminal->IsFileExists(syncProgramIconFilePath, "UsersProgramIconMakingService::AddIconToCurrentUserDesktop", true))
    {
        m_terminal->ClearFileSudo(syncProgramIconFilePath, "UsersProgramIconMakingService::AddIconToCurrentUserDesktop");
    }
    else
    {
        m_terminal->CreateFile(syncProgramIconFilePath, "UsersProgramIconMakingService::AddIconToCurrentUserDesktop", true);
    }
    const QString iconString="[Desktop Entry]\nName=Панель управления РЛС ТИ\nName[ru]=Панель управления РЛС ТИ\nType=Application\nExec=/usr/RLS_TI/"+m_applicationName + "\nIcon=groups-users\nCategories=Utility\nOnlyShowIn=Fly\nEncoding=UTF-8\nX-Fly-OriginFile=/usr/share/applications/fly-admin-iso.desktop";
    m_terminal->WriteTextToFileSudo(iconString.toUtf8(), syncProgramIconFilePath, "WizardService::AddIconToStartMenu");
}

void UsersProgramIconMakingService::AddIconToAllUserDesktops()
{

}

