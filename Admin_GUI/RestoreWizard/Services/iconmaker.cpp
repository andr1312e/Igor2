#include "iconmaker.h"

#include <QDir>

IconMaker::IconMaker(const QString &applicationPath, const QString &applicationName, const QString &rlsTiFolder, LinuxUserService *service, QObject *parent)
    : QObject(parent)
    , m_iconName(QStringLiteral("users.desktop"))
    , m_addIconToDesktopToCurrentUser(false)
    , m_addIconToDesktopToAllUsers(false)
    , m_addIconToStartMenu(false)
    , m_applicationPath(QString(applicationPath + '/'))
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
    m_addIconToDesktopToCurrentUser = state;
}

void IconMaker::OnAddIconToDesktopToAllUsers(bool state)
{
    m_addIconToDesktopToAllUsers = state;
}

void IconMaker::OnAddIconToStartMenu(bool state)
{
    m_addIconToStartMenu = state;
}

void IconMaker::ApplyWizardActions()
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Начинаем создавать иконки"));
    CopyMyAppToRLSTIFolder();
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Программа скопирована в папку"));
    if (m_addIconToDesktopToAllUsers)
    {
        AddIconToAllUserDesktops();
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Добавлена иконка всем пользователям"));
    }
    else
    {
        if (m_addIconToDesktopToCurrentUser)
        {
            AddIconToCurrentUserDesktop();
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Добавлена иконка текущему пользователю"));
        }
    }

    if (m_addIconToStartMenu)
    {
        AddIconToStartMenu();
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Добавлена иконка в меню пуск"));
    }
}
/**
 * Копировать мою программу в папку РЛС ТИ
 */
void IconMaker::CopyMyAppToRLSTIFolder()
{
    if (m_terminal->IsDirNotExists(m_rlsTiFolder, Q_FUNC_INFO, true))
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Создаем папку РЛС ТИ. Путь: ") + m_rlsTiFolder);
        m_terminal->CreateFolder(m_rlsTiFolder, Q_FUNC_INFO, true);
    }
    if (m_terminal->IsFileExists(m_rlsTiFolder + m_applicationName, Q_FUNC_INFO, true))
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" В папке уже имеется файл программы"));
        if (m_applicationPath == m_rlsTiFolder)
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Делаем права для запуска"));
            m_terminal->SetPermissionToExecuteSudo(m_rlsTiFolder + m_applicationName, Q_FUNC_INFO);
            return;
        }
        else
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Заменяем другую программу на мою. Удаляем старую программу"));
            m_terminal->DeleteFileSudo(m_rlsTiFolder + m_applicationName, Q_FUNC_INFO);
        }
    }
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Кладем програму из папки: ") + m_applicationPath + m_applicationName + QStringLiteral(" в папку: ") + m_rlsTiFolder);
    m_terminal->CopyFileSudo(m_applicationPath + m_applicationName, m_rlsTiFolder, Q_FUNC_INFO);
    m_terminal->SetPermissionToExecuteSudo(m_rlsTiFolder + m_applicationName, Q_FUNC_INFO);
}
/**
 * Добавить иконку в меню пуск
 */
void IconMaker::AddIconToStartMenu()
{
    const QString startPath(QDir::homePath() + QStringLiteral("/.fly/startmenu/"));
    const QStringList foldersList(m_terminal->GetFolderList(startPath, Q_FUNC_INFO, true));
    const QString rlsTiStartupFolderPath(startPath + QStringLiteral("RLS_TI/"));
    if (foldersList.contains(QStringLiteral("RLS_TI/")))
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Отчищаем папку в меню пуск"));
        m_terminal->ClearFolderSudo(rlsTiStartupFolderPath, Q_FUNC_INFO);
    }
    else
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Создаем папку в меню пуск"));
        m_terminal->CreateFolder(rlsTiStartupFolderPath, Q_FUNC_INFO, true);
    }
    const QString directoryIconFilePath(rlsTiStartupFolderPath + QStringLiteral(".directory"));
    m_terminal->CreateFile(directoryIconFilePath, Q_FUNC_INFO, true);

    QString iconString = QStringLiteral("[Desktop Entry]\nName=Group\nName[ru]=РЛС ТИ Программы\nType=Directory\nNoDisplay=false\nIcon=preferences-desktop-locale\nIcon[ru]=preferences-desktop-locale\nHidden=false");
    m_terminal->WriteTextToFileSudo(iconString, directoryIconFilePath, Q_FUNC_INFO);

    const QString syncProgramIconFilePath = rlsTiStartupFolderPath + m_iconName;
    m_terminal->CreateFile(syncProgramIconFilePath, Q_FUNC_INFO, true);

    iconString = QStringLiteral("[Desktop Entry]\nName=Панель управления РЛС ТИ\nName[ru]=Панель управления РЛС ТИ\nType=Application\nExec=/usr/RLS_TI/") + m_applicationName + QStringLiteral("\nIcon=groups-users\nCategories=Utility\nOnlyShowIn=Fly\nEncoding=UTF-8\nX-Fly-OriginFile=/usr/share/applications/fly-admin-iso.desktop");
    m_terminal->WriteTextToFileSudo(iconString, syncProgramIconFilePath, Q_FUNC_INFO);
}

void IconMaker::AddIconToCurrentUserDesktop()
{
    const QString currentUserDesktopPath = QDir::homePath() + QStringLiteral("/Desktop");
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Добавляем икноку текущему пользователю "));
    AddIcon(currentUserDesktopPath);
}

void IconMaker::AddIconToAllUserDesktops()
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Добавляем икноки пользователю "));
    const QList<QPair<QString, QString>> userNameAndIdList(m_linuxUserService->GetSystemUsersNamesWithIds());
    for (const QPair<QString, QString> &nameAndId : userNameAndIdList)
    {
        const QString userDesktopFolderPath(QStringLiteral("/home/") + nameAndId.first + QStringLiteral("/Desktop/"));
        AddIcon(userDesktopFolderPath);
    }
}

void IconMaker::AddIcon(const QString &desktopPath)
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Cоздаем иконку ") + desktopPath);
    if (m_terminal->IsDirNotExists(desktopPath, Q_FUNC_INFO, true))
    {
        m_terminal->CreateFolder(desktopPath, Q_FUNC_INFO, true);
    }
    const QString userFileName = desktopPath + m_iconName;
    if (m_terminal->IsFileExists(userFileName, Q_FUNC_INFO, true))
    {
        m_terminal->ClearFileSudo(userFileName, Q_FUNC_INFO);
    }
    else
    {
        m_terminal->CreateFile(userFileName, Q_FUNC_INFO, true);
    }
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Пишем текст в иконку "));
    const QString iconString(QStringLiteral("[Desktop Entry]\nName=Панель управления РЛС ТИ\nName[ru]=Панель управления РЛС ТИ\nType=Application\nExec=/usr/RLS_TI/") + m_applicationName + QStringLiteral("\nIcon=groups-users\nCategories=Utility\nOnlyShowIn=Fly\nEncoding=UTF-8\nX-Fly-OriginFile=/usr/share/applications/fly-admin-iso.desktop"));
    m_terminal->WriteTextToFileSudo(iconString, userFileName, Q_FUNC_INFO);
}

