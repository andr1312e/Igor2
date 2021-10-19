#include "program.h"
#include <QDebug>


Program::Program(int &argc, char **argv)
    : QApplication(argc, argv, true)
    , m_loadedDbAdnRolesState(ProgramFilesState::CantRun)
    , m_hasAdminPrivileges(false)
    , m_sharedMemory(new QSharedMemory("PROCESS_CONTROLLER", this))
    , m_terminal(nullptr)
    , m_linuxUserService(nullptr)
    , m_settingFileService(nullptr)
    , m_startupWizard(nullptr)
    , m_dataBaseService(nullptr)
    , m_indentifyService(nullptr)
    , m_startupRunnableService(nullptr)
    , m_socketToRarm(nullptr)
    , m_AdminGui(nullptr)
    , m_fakeUI(new FakeUI)
    , m_framelessWindow(nullptr)
    , m_proxyStyle(nullptr)
{
    m_fakeUI->hide();
}

Program::~Program()
{
    m_sharedMemory->detach();
    delete m_sharedMemory;
    delete m_fakeUI;
    if (m_terminal!=nullptr)
    {
        delete m_settingFileService;
        if (m_AdminGui!=nullptr)
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
    getCurrentUserSystemData();
    initSettingsService();
    LoadDbAndRoles();
    ProcessDataLoading();
}

void Program::ProcessDataLoading()
{
    switch (m_loadedDbAdnRolesState) {
    case NoFiles:
    case NoUserDb:
    case NoRoleData:
    {
        StartSettingsWizard();
        break;
    }
    case Fine:
    {
        ContinueLoading();
        break;
    }
    case CantRun:
    {
        QMessageBox::critical(nullptr,"Приложение не может запуститься", "Файл настроек  имеет неверную структуру, обратитесь к Администратору для решения проблемы",QMessageBox::Ok);
        break;
    }
    }
}

void Program::StartSettingsWizard()
{
    m_startupWizard=new StartupWizard(m_loadedDbAdnRolesState, m_settingFileService, nullptr);
    connect(m_startupWizard, &StartupWizard::finish, this, &Program::ContinueLoading);
    m_startupWizard->show();
}

void Program::ContinueLoading()
{
    getSettings();
    initRunnableService();
    if(allAppsRunned())
    {
        initRarmSocket();
        createConnections();
        if (m_hasAdminPrivileges)
        {
            initAdminServices();
            initAdminUI();
            initStyle();
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

void Program::getCurrentUserSystemData()
{
    m_hasAdminPrivileges=m_linuxUserService->hasCurrentUserAdminPrivileges();
    m_currentUserName=m_linuxUserService->getCurrentUserName();
}

void Program::initSettingsService()
{
    m_settingFileService=new AppFirstLoadlingSettingsService(m_currentUserName,m_currentUserId, m_hasAdminPrivileges, m_fakeUI, m_terminal);
}

void Program::LoadDbAndRoles()
{
    m_loadedDbAdnRolesState= m_settingFileService->isAllDataLoaded();
}

void Program::getSettings()
{
    m_userDBPath=m_settingFileService->getUserDBPathValue();
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
    m_dataBaseService=new DatabaseService(m_terminal);
    m_dataBaseService->loadFromFile(m_userDBPath);
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

void Program::initStyle()
{
    this->setStyle(QStringLiteral("Fusion"));
    m_proxyStyle=new StyleChanger(this);
    m_proxyStyle->changeTheme(m_settingFileService->getThemeValue());
    connect(m_AdminGui, &Admin_GUI::setTheme, m_proxyStyle, &StyleChanger::changeTheme);
}

void Program::startAdminServices()
{
    m_linuxUserService->getAllUsersInSystem();
}
