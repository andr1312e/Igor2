#ifndef PROGRAM_H
#define PROGRAM_H

#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <QApplication>
#include <QInputDialog>
#include <QSharedMemory>

#include "Services/singleinstancemaker.h"
#include "Services/Sql/sqldatabaseserivce.h"
#include "Services/Sql/sqladjuster.h"

#include "Services/Terminals/terminal.h"
#include "Services/startuprunnableservice.h"

#include "Server/sockettorarm.h"

#include "Tray/tray.h"
#include "Admin_GUI/Views/admingui.h"
#include "Admin_GUI/Wizard/Views/startupwizard.h"

#include "User_GUI/User_GUI.h"

#include "Styles/Frameless/framelesswindow.h"
#include "Styles/Themes/stylechanger.h"

class Program : public QApplication
{
   Q_OBJECT
public:
   explicit Program(int &argc, char **argv);
   ~Program();
   bool HasNoRunningInstance();
   bool CreateAndRunApp();

private:
   bool ConnectToDatabase();
   void GetCurrentUserNameIdAndAdminPriviliges();

   LoadingState GetProgramState();

   void ProcessDataLoading(const LoadingState &state);
   ThemesNames GetThemeNameFromSettings() const;
   void InitStyleChanger(ThemesNames themeName);
   void InitFramelessWindow(ThemesNames themeName);
   void StartSettingsWizard(const LoadingState &state);

private:
   void UserLoading();

private Q_SLOTS:
   void OnFullLoading();

public:
   bool AllAppsRunnedWell();
   void GetAllUsersWithIdInSystem();
   void InitRarmSocket();
   void InitAdminUI();
   void ConnectUserObjects();
   void ConnectAdminObjects();

private:
   bool CanGetAdminAccess();
private:
   const QString m_rlstiFolder;
   SingleInstanceMaker* m_singleInstance;
   Terminal* const m_terminal;
   LinuxUserService* const m_linuxUserService;
   const QString m_currentUserName;
   const QString m_currentUserId;

   SqlDatabaseSerivce* const m_sqlDatabaseService;
   SqlAdjuster* m_sqlAdjuster;
   DbState m_oldDbState;

   StartupRunnableManager *m_startupRunnableService;

   Tray* const m_tray;

   StartupWizard *m_startupWizard;
   SocketToRarm *m_socketToRarm;
   Admin_GUI *m_AdminGui;
   FramelessWindow *m_framelessWindow;

   StyleChanger *m_styleChanger;
};
#endif // PROGRAM_H
