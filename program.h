#ifndef PROGRAM_H
#define PROGRAM_H
#pragma once

#include <QDebug>
#include <QSettings>
#include <QApplication>
#include <QMessageBox>
#include <QInputDialog>
#include <QSharedMemory>

#include "Services/Settings/appsettingsservice.h"

#include "Services/identifyservice.h"
#include "Services/Terminals/terminal.h"
#include "Services/startuprunnableservice.h"

#include "Server/dataHandler.h"

#include "Admin_GUI/Views/admingui.h"
#include "User_GUI/User_GUI.h"

#include <Styles/Frameless/framelesswindow.h>
#include <Styles/Themes/stylechanger.h>

class Program : public QApplication
{
    Q_OBJECT
public:
    explicit Program(int &argc, char **argv);
    virtual ~Program();
    bool hasNoRunningInscance();
    void createApp();

private:

    void initTerminal();
    void initUserService();
    void getUserPrivilegesAndName();

    void initSettingsService();
    bool settingsLoaded();
    void getSettings();

    void initRunnableService();
    bool allAppsRunned();

    void initAdminServices();
    void initRarmSocket();

    void initAdminUI();

    void startAdminServices();

    void createConnections();

    void initStyle();

private:

    bool m_hasAdminPrivileges;
    QString m_currentUserName;

    QSharedMemory *m_sharedMemory;

    QString m_userDBPath;

    Terminal *m_terminal;
    LinuxUserService *m_linuxUserService;
    AppSettingsService *m_settingFileService;

    DatabaseService *m_dataBaseService;
    IdentifyService *m_indentifyService;
    StartupRunnableService *m_startupRunnableService;
    SocketToRarm *m_socketToRarm;
    Admin_GUI *m_AdminGui;
    FakeUI *m_fakeUI;
    FramelessWindow *m_framelessWindow;

    StyleChanger *m_proxyStyle;

};
#endif // PROGRAM_H
