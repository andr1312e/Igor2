#include "program.h"
#include <QDebug>

Program::Program(int &argc, char **argv)
    : QApplication(argc, argv, true)
    , m_sharedMemory(new QSharedMemory("PROCESS_CONTROLLER", this))
    , m_terminal(Q_NULLPTR)
    , m_linuxUserService(Q_NULLPTR)
    , m_dataBaseService(Q_NULLPTR)
    , m_indentifyService(Q_NULLPTR)
    , m_startupRunnableService(Q_NULLPTR)
    , m_socketToRarm(Q_NULLPTR)
    , m_AdminGui(Q_NULLPTR)
    , m_fakeUI(new FakeUI)
{
    m_fakeUI->hide();
}

Program::~Program()
{
    m_sharedMemory->detach();
    delete m_sharedMemory;
    delete m_fakeUI;
    if (m_terminal!=Q_NULLPTR)
    {
        delete m_settingFileService;
        if (m_AdminGui!=Q_NULLPTR)
        {
            delete m_AdminGui;
        }
        delete m_indentifyService;
        delete m_dataBaseService;
        delete m_startupRunnableService;
        delete m_linuxUserService;
        delete m_socketToRarm;
        delete m_terminal;
    }
}

bool Program::hasNoRunningInscance()
{
    if (m_sharedMemory->attach(QSharedMemory::ReadOnly))
    {
        m_sharedMemory->detach();
        return false;
    }
    else
    {
        if (m_sharedMemory->create(1, QSharedMemory::ReadWrite))
        {
            return true;
        }
        else
        {
            qDebug()<< m_sharedMemory->errorString();
            return false;
        }
    }
}

void Program::createApp()//MAIN
{
    initTerminal();
    initUserService();
    getUserPrivileges();
    initSettingsService();
    if (settingsLoaded())
    {
        getSettings();
        initRunnableService();
        if(allAppsRunned())
        {
            initRarmSocket();
            createConnections();
            if (hasAdminPrivileges)
            {
                initAdminServices();
                initAdminUI();
                applyStyle();
            }
        }
    }
}

void Program::initTerminal()
{
    m_terminal=new Terminal();
}

void Program::initUserService()
{
    m_linuxUserService=new LinuxUserService(m_terminal);
}

void Program::getUserPrivileges()
{
    hasAdminPrivileges=m_linuxUserService->hasCurrentUserAdminPrivileges();
}

void Program::initSettingsService()
{
    m_settingFileService=new AppSettingsService(hasAdminPrivileges, m_fakeUI, m_terminal);
}

bool Program::settingsLoaded()
{
    return m_settingFileService->tryToLoadSettings();
}

void Program::getSettings()
{
    m_userDBPath=m_settingFileService->m_pathUserDB;
}

void Program::initRunnableService()
{
    m_startupRunnableService=new StartupRunnableService(m_terminal, this);
}

bool Program::allAppsRunned()
{
    return m_startupRunnableService->run(m_linuxUserService->getCurrentUserName());
}

void Program::initRarmSocket()
{
    m_socketToRarm=new SocketToRarm("127.0.0.1:4242", this);
}

void Program::initAdminServices()
{
    m_dataBaseService=new DatabaseService(m_userDBPath, m_terminal);
    m_indentifyService=new IdentifyService(m_terminal, m_dataBaseService, m_linuxUserService);
    m_linuxUserService->getAllUsersInSystem();
}

void Program::initAdminUI()
{
    if (m_indentifyService->canGetAccess())
    {
        m_framelessWindow=new FramelessWindow(Q_NULLPTR);
        m_AdminGui=new Admin_GUI(m_dataBaseService, m_linuxUserService);
        m_framelessWindow->setWindowIcon(QIcon(":/images/ico.png"));
        m_framelessWindow->setWindowTitle("Панель управления пользователями и модулями РЛС ТИ");
        m_framelessWindow->setAdminGUI(m_AdminGui);
        m_framelessWindow->show();
    }
}

void Program::createConnections()
{
    connect(m_startupRunnableService, &StartupRunnableService::programFall, m_socketToRarm, &SocketToRarm::programFall);
}

void Program::applyStyle()
{
    m_proxyStyle=new ProxyStyle();
    m_proxyStyle->setTheme(m_settingFileService->m_theme);
    QApplication::setStyle(m_proxyStyle);
    connect(m_AdminGui, &Admin_GUI::setTheme, m_proxyStyle, &ProxyStyle::changeTheme);
}

void Program::startAdminServices()
{
    m_linuxUserService->getAllUsersInSystem();
}
