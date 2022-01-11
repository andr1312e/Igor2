#include "program.h"
#include <QDebug>

Program::Program(int &argc, char **argv)
    : QApplication(argc, argv, true)
    , m_rlstiFolder("/usr/RLS_TI/")
    , m_sharedMemory(new QSharedMemory("PROCESS_CONTROLLER", this))
    , m_terminal(nullptr)
    , m_linuxUserService(nullptr)
    , m_startupWizard(nullptr)
    , m_sqlDatabaseService(nullptr)
    , m_startupRunnableService(nullptr)
    , m_socketToRarm(nullptr)
    , m_tray(nullptr)
    , m_AdminGui(nullptr)
    , m_framelessWindow(nullptr)
    , m_styleChanger(nullptr)
{
}

Program::~Program()
{
    m_sharedMemory->detach();
    delete m_sharedMemory;
    delete m_sqlDatabaseService;
    if (m_terminal != nullptr) {
        delete m_tray;
        if (m_AdminGui != nullptr) {
            delete m_AdminGui;
        }

        delete m_startupRunnableService;
        delete m_linuxUserService;
        delete m_socketToRarm;
        delete m_terminal;
    }
}

bool Program::HasNoRunningInscance()
{
    if (m_sharedMemory->attach(QSharedMemory::ReadOnly)) {
        m_sharedMemory->detach();
        return false;
    } else {
        if (m_sharedMemory->create(1, QSharedMemory::ReadWrite)) {
            return true;
        } else {
            qDebug() << m_sharedMemory->errorString();
            return false;
        }
    }
}

void Program::CreateApp()//MAIN
{

    InitTerminal();
    InitUserService();
    GetAllUsersWithIdInSystem();
    InitSqlService();
    GetCurrentUserNameIdAndAdminPriviliges();
    const LoadingState state=GetProgramState();
    InitTrayIcon();
    InitFramelessWindow();
    InitStyle();
    ProcessDataLoading(state);
}

void Program::InitTerminal()
{
    m_terminal = new Terminal();
}

void Program::InitUserService()
{
    m_linuxUserService = new LinuxUserService(m_terminal);
}

void Program::InitSqlService()
{
    m_sqlDatabaseService=new SqlDatabaseSerivce(this);
    if (!m_sqlDatabaseService->ConnectToDataBase("localhost", 5432, "postgres", "postgres", "user1234"))
    {
        QMessageBox::warning(nullptr, "Подключите базу данных postgres", "Подключение к бд не удалось");
        this->exit(-1);
    }
}

void Program::GetCurrentUserNameIdAndAdminPriviliges()
{
    m_currentUserName = m_linuxUserService->GetCurrentUserName();
    m_currentUserId = m_linuxUserService->GetCurrentUserId();

}

LoadingState Program::GetProgramState()
{
    if (m_sqlDatabaseService->CheckUsersTable())
    {
        if(m_sqlDatabaseService->CheckDesktopTables() && m_sqlDatabaseService->CheckStartupTables())
        {
            return LoadingState::Fine;
        }
        else
        {
            return LoadingState::NoRoleData;
        }
    }
    else
    {
        if(m_sqlDatabaseService->CheckStartupTables() && m_sqlDatabaseService->CheckStartupTables())
        {
            return LoadingState::NoUserDb;
        }
        else
        {
            return LoadingState::NoFiles;
        }
    }
    qFatal("Invalid state");
}

void Program::InitTrayIcon()
{
    m_tray=new Tray(this);
}

void Program::InitFramelessWindow()
{
    m_framelessWindow = new FramelessWindow(Q_NULLPTR);
    this->setWindowIcon(QIcon(":/images/0.jpg"));
    m_framelessWindow->setObjectName("FramelessWindowObject");
    m_framelessWindow->OnSetWindowTitle("Мастер первоначальной настройки");
    m_framelessWindow->show();
}

void Program::InitStyle()
{
    this->setStyle(QStringLiteral("Fusion"));
    m_styleChanger = new StyleChanger(this);
    m_styleChanger->OnChangeTheme(1);
    connect(m_framelessWindow, &FramelessWindow::ToChangeTheme, m_styleChanger, &StyleChanger::OnChangeTheme);
}

void Program::ProcessDataLoading(const LoadingState &state)
{
    switch (state) {
    case NoFiles:
    case NoUserDb:
    case NoRoleData: {
        StartSettingsWizard(state);
        break;
    }

    case Fine: {
        OnContinueLoading();
        break;
    }

    case CantRun: {
        QMessageBox::critical(nullptr, "Приложение не может запуститься", "Права разработчика недоступны, запустите программу от имени администратора и повторите попытку...", QMessageBox::Ok);
        this->exit(0);
    }
    }
}

void Program::StartSettingsWizard(const LoadingState &state)
{
    m_startupWizard = new StartupWizard(this->applicationName(), m_rlstiFolder, state, m_linuxUserService, m_sqlDatabaseService, nullptr);
    connect(m_startupWizard, &StartupWizard::ToChangeTheme, m_styleChanger, &StyleChanger::OnChangeTheme);
    connect(m_startupWizard, &QWizard::accepted, this, &Program::OnContinueLoading);
    connect(m_startupWizard, &QWizard::rejected, this, &QApplication::quit);
    m_framelessWindow->SetMainWidget(m_startupWizard);
    m_framelessWindow->repaint();
}

void Program::OnContinueLoading()
{
    InitRunnableService();

    if (AllAppsRunned()) {
        InitRarmSocket();


        //        if (m_linuxUserService->HasCurrentUserAdminPrivileges()) {

        InitAdminUI();
        ConnectObjects();
        //        }
    }
}

void Program::InitRunnableService()
{
    m_startupRunnableService = new StartupRunnableManager(m_linuxUserService->GetCurrentUserName(), m_rlstiFolder, m_sqlDatabaseService,m_terminal, this);
}

bool Program::AllAppsRunned()
{
    return m_startupRunnableService->RunStartups();
}

void Program::InitRarmSocket()
{
    m_socketToRarm = new SocketToRarm(QStringLiteral("127.0.0.1:4242"), this);
}

void Program::GetAllUsersWithIdInSystem()
{
    m_linuxUserService->GetAllUsersWithIdInSystem();
}

void Program::InitAdminUI()
{
    //    if (CanGetAdminAccess())
    {
        m_AdminGui = new Admin_GUI(m_terminal, m_sqlDatabaseService, m_linuxUserService, m_currentUserId, Q_NULLPTR);
        m_framelessWindow->OnSetWindowTitle("Панель управления пользователями и модулями РЛС ТИ");
        m_framelessWindow->SetMainWidget(m_AdminGui);
        m_framelessWindow->show();
    }
}

void Program::ConnectObjects()
{
    connect(m_framelessWindow, &FramelessWindow::ToSetDelegateView, m_AdminGui, &Admin_GUI::ToSetDelegateView);
    connect(m_framelessWindow, &FramelessWindow::ToHideAdditionalSettings, m_AdminGui, &Admin_GUI::OnHideAdditionalSettings);
    connect(m_AdminGui, &Admin_GUI::ToCurrentUserRoleChanged, m_startupRunnableService, &StartupRunnableManager::OnCurrentUserRoleChanged);
    connect(m_startupRunnableService, &StartupRunnableManager::ToProgramFall, m_socketToRarm, &SocketToRarm::OnProgramFall);
    connect(m_tray, &Tray::ToCloseApp, this, &QApplication::quit);
    connect(m_tray, &Tray::ToShowApp, m_framelessWindow, &FramelessWindow::show);
    connect(m_tray, &Tray::ToHideApp, m_framelessWindow, &QWidget::hide);
    //    connect()
}

bool Program::CanGetAdminAccess()
{
    if (m_linuxUserService->HasCurrentUserAdminPrivileges())
    {
        if (m_sqlDatabaseService->CheckUsersTable())
        {
            QStringList adminsUserName=m_sqlDatabaseService->GetAdminsRoleUserName();
            QString userName=m_linuxUserService->GetCurrentUserName();
            for(QString &adminName: adminsUserName)
            {
                if(adminName==userName)
                {
                    return true;
                }
            }
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}
