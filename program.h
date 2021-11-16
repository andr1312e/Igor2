#ifndef PROGRAM_H
#define PROGRAM_H

#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <QApplication>
#include <QInputDialog>
#include <QSharedMemory>

#include "Services/Settings/firststartsettingsservice.h"

#include "Services/identifyservice.h"
#include "Services/Terminals/terminal.h"
#include "Services/startuprunnableservice.h"

#include "Server/dataHandler.h"

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
   void GetCurrentUserNameIdAndAdminPriviliges();

   void InitSettingsService();
   void GetProgramState();

   void ProcessDataLoading();
   void InitStyle();
   void InitFramelessWindow();
   void StartSettingsWizard();

private Q_SLOTS:
   void OnContinueLoading();

public:
   void GetSettings();

   void InitRunnableService();
   bool AllAppsRunned();

   void InitAdminServices();
   void InitRarmSocket();

   void InitAdminUI();

   void StartAdminServices();

   void ConnectObjects();

private:
   LoadingState m_loadedDbAdnRolesState;

   bool m_hasAdminPrivileges;
   QString m_currentUserId;
   QString m_currentUserName;

   QSharedMemory *m_sharedMemory;

   QString m_userDBPath;

   Terminal *m_terminal;
   LinuxUserService *m_linuxUserService;
   FirstStartSettingsService *m_firstStartSettingsService;

   StartupWizard *m_startupWizard;

   DatabaseService *m_dataBaseService;
   IdentifyService *m_indentifyService;
   StartupRunnableService *m_startupRunnableService;
   SocketToRarm *m_socketToRarm;
   Admin_GUI *m_AdminGui;
   FakeUI *m_fakeUI;
   FramelessWindow *m_framelessWindow;

   StyleChanger *m_styleChanger;

};
#endif // PROGRAM_H
