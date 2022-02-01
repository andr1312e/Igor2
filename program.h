#ifndef PROGRAM_H
#define PROGRAM_H

#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <QApplication>
#include <QInputDialog>
#include <QSharedMemory>

#include "Logging/logger.h"
#include "Logging/hierarchy.h"

#include "Services/Sql/sqldatabaseserivce.h"
#include "Services/Terminals/terminal.h"
#include "Services/startuprunnableservice.h"

#include "Server/sockettorarm.h"

#include "Tray/tray.h"
#include "Admin_GUI/Views/admingui.h"
#include "Admin_GUI/Wizard/Views/startupwizard.h"

#include "Styles/Frameless/framelesswindow.h"
#include "Styles/Themes/stylechanger.h"

class Program : public QApplication
{
   Q_OBJECT
public:
   explicit Program(int &argc, char **argv);
   ~Program();
   bool HasNoRunningInscance();
   void CreateAndRunApp();

private:
   void CreateBasicServicesAndTray();
   void ConnectToDatabase();
   void GetCurrentUserNameIdAndAdminPriviliges();

   LoadingState GetProgramState();

   void ProcessDataLoading(const LoadingState &state);
   void InitStyleChanger();
   void InitFramelessWindow();
   void StartSettingsWizard(const LoadingState &state);

private:
   void UserLoading();
   void ShutDownRootLogger();

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
   QSharedMemory* const m_sharedMemory;
   Terminal* const m_terminal;
   LinuxUserService* m_linuxUserService;
   QString m_currentUserName;
   QString m_currentUserId;

   SqlDatabaseSerivce* m_sqlDatabaseService;
   StartupRunnableManager* m_startupRunnableService;

   Tray* m_tray;

   StartupWizard *m_startupWizard;
   SocketToRarm *m_socketToRarm;
   Admin_GUI *m_AdminGui;
   FramelessWindow *m_framelessWindow;

   StyleChanger *m_styleChanger;
   QSettings m_settings;
};
#endif // PROGRAM_H
