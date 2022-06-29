#ifndef PROGRAM_H
#define PROGRAM_H

#include "unistd.h"
#include "errno.h"

#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <QApplication>
#include <QInputDialog>
#include <QFontDatabase>

#include "Services/singleinstancemaker.h"
#include "Services/Sql/sqldatabaseserivce.h"
#include "Services/Sql/sqlproblemsfixer.h"

#include "Services/Terminals/terminal.h"
#include "Services/startuprunnableservice.h"

#include "Server/sockettorarm.h"

#include "Tray/tray.h"
#include "Admin_GUI/Views/admingui.h"
#include "Admin_GUI/RestoreWizard/Views/startupwizard.h"

#include "Styles/Frameless/framelesswindow.h"
#include "Styles/Themes/stylechanger.h"
#include "Services/ShortCut/globalshortcut.h"

class Program : public QApplication
{
    Q_OBJECT
public:
    explicit Program(int &argc, char **argv);
    ~Program();
    bool HasNoRunningInstance();
    DbConnectionState CreateAndRunApp();

private:
    void GetCurrentUserNameIdAndAdminPriviliges();

    LoadingStates GetLoadingStates();

    void ContinueDataLoading(LoadingStates states);
    ThemesNames GetThemeNameFromSettings() const;
    void InitStyleChanger(ThemesNames themeName);
    void InitFramelessWindow(ThemesNames themeName);
    void StartSettingsWizard(LoadingStates states);

private:
    void UserLoading();

private Q_SLOTS:
    void OnFullLoading();

public:
    bool AllAppsRunnedWell();
    void CollectAllUsersWithIdInSystem();
    void InitRarmSocket();
    void InitAdminUI();
    void ConnectUserObjects();
    void ConnectAdminObjects();
    void CreateGlobalShortCut();

private:
    bool CanGetAdminAccess();
private:
    const QString m_rlstiFolder;
    Terminal *const m_terminal;
    LinuxUserService *const m_linuxUserService;
public:
    const QString m_currentUserName;
    const QString m_currentUserId;
private:
    SqlDatabaseSerivce *const m_sqlDatabaseService;

    SingleInstanceMaker *m_singleInstance;

    StartupRunnableManager *m_startupRunnableService;

    Tray *m_tray;

    StartupWizard *m_startupWizard;
    SocketToRarm *m_socketToRarm;
    Admin_GUI *m_AdminGui;
    FramelessWindow *m_framelessWindow;

    StyleChanger *m_styleChanger;
private:
    ShortCut::GlobalShortcut *m_shortCut;
};
#endif // PROGRAM_H
