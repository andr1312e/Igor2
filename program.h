#ifndef PROGRAM_H
#define PROGRAM_H

#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <QApplication>
#include <QInputDialog>
#include <QSharedMemory>

#include "Services/sqldatabaseserivce.h"
#include "Services/Terminals/terminal.h"
#include "Services/startuprunnableservice.h"

#include "Server/dataHandler.h"

#include "tray.h"
#include "Admin_GUI/Views/admingui.h"
#include "Admin_GUI/Views/Wizards/startupwizard.h"

#include "User_GUI/User_GUI.h"

#include "Styles/Frameless/framelesswindow.h"
#include "Styles/Themes/stylechanger.h"

class Program : public QApplication
{
   Q_OBJECT
public:
   explicit Program(int &argc, char **argv);
   virtual ~Program();
   bool HasNoRunningInscance();
   void CreateApp();

private:

   void InitTerminal();
   void InitUserService();
   void InitSqlService();
   void GetCurrentUserNameIdAndAdminPriviliges();

   LoadingState GetProgramState();

   void ProcessDataLoading(LoadingState &state);
   void InitStyle();
   void InitTrayIcon();
   void InitFramelessWindow();
   void StartSettingsWizard(LoadingState &state);

private Q_SLOTS:
   void OnContinueLoading();

public:
   void InitRunnableService();
   bool AllAppsRunned();

   void InitAdminServices();
   void InitRarmSocket();

   void InitAdminUI();

   void ConnectObjects();

private:
   bool CanGetAdminAccess();
private:
   const QString m_rlstiFolder;
   QString m_currentUserId;
   QString m_currentUserName;

   QSharedMemory *m_sharedMemory;

   QString m_userDBPath;

   Terminal *m_terminal;
   LinuxUserService *m_linuxUserService;

   StartupWizard *m_startupWizard;

   SqlDatabaseSerivce *m_sqlDatabaseService;
   StartupRunnableManager *m_startupRunnableService;
   SocketToRarm *m_socketToRarm;

   Tray *m_tray;
   Admin_GUI *m_AdminGui;

   FramelessWindow *m_framelessWindow;

   StyleChanger *m_styleChanger;

};
#endif // PROGRAM_H
