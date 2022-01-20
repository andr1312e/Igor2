#include "program.h"
#include <QDebug>

Program::Program(int &argc, char **argv)
    : QApplication(argc, argv, true)
    , m_rlstiFolder("/usr/RLS_TI/")
    , m_sharedMemory(new QSharedMemory("PROCESS_CONTROLLER", this))
    , m_terminal(Terminal::GetTerminal())
    , m_linuxUserService(new LinuxUserService())
    , m_currentUserName(m_linuxUserService->GetCurrentUserName())
    , m_currentUserId(m_linuxUserService->GetCurrentUserId())
    , m_sqlDatabaseService(new SqlDatabaseSerivce(this))
    , m_startupRunnableService(new StartupRunnableManager(m_currentUserName, m_rlstiFolder, m_sqlDatabaseService, this))
    , m_tray(new Tray(this))
    , m_startupWizard(Q_NULLPTR)
    , m_socketToRarm(Q_NULLPTR)
    , m_AdminGui(Q_NULLPTR)
    , m_framelessWindow(Q_NULLPTR)
    , m_styleChanger(Q_NULLPTR)
{
}

Program::~Program()
{
    m_sharedMemory->detach();
    delete m_sharedMemory;
    delete m_terminal;
    delete m_linuxUserService;
    delete m_sqlDatabaseService;
    delete m_startupRunnableService;
    delete m_tray;
    if(m_startupWizard!=Q_NULLPTR)
    {
        delete m_startupWizard;
    }
    if(m_AdminGui!=Q_NULLPTR)
    {
        delete m_AdminGui;
        delete m_styleChanger;
        delete m_framelessWindow;
    }
    if(m_socketToRarm!=Q_NULLPTR)
    {
        delete m_socketToRarm;
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

void Program::CreateAndRunApp()//MAIN
{
    GetAllUsersWithIdInSystem();
    ConnectToDatabase();
    const LoadingState state=GetProgramState();
    ProcessDataLoading(state);
}

void Program::GetAllUsersWithIdInSystem()
{
    m_linuxUserService->GetAllUsersWithIdInSystem();
}

void Program::ConnectToDatabase()
{
    if (!m_sqlDatabaseService->ConnectToDataBase("localhost", 5432, "postgres", "postgres", "user1234"))
    {
        QMessageBox::warning(nullptr, QStringLiteral("Подключите базу данных postgres"), "Подключение к бд не удалось");
        this->exit(-1);
    }
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
    qFatal("%s", QStringLiteral("Нереализованное поведение").toUtf8().constData());
}

void Program::ProcessDataLoading(const LoadingState &state)
{
    switch (state) {
    case NoFiles:
    case NoUserDb:
    case NoRoleData: {
        if(CanGetAdminAccess())
        {
            InitFramelessWindow();
            InitStyleChanger();
            StartSettingsWizard(state);
        }
        else
        {
            QMessageBox::critical(Q_NULLPTR, "Приложение не может запуститься", "Права разработчика недоступны, запустите программу от имени администратора и повторите попытку...", QMessageBox::Ok);
        }
        break;
    }

    case Fine: {
        if(CanGetAdminAccess())
        {
            InitFramelessWindow();
            InitStyleChanger();
            OnFullLoading();
        }
        else
        {
            UserLoading();
        }
        break;
    }
    default:{
        qFatal("%s", QStringLiteral("Нереализованное поведение").toUtf8().constData());
    }
    }
}

void Program::InitFramelessWindow()
{
    m_framelessWindow = new FramelessWindow(Q_NULLPTR);
    this->setWindowIcon(QIcon(":/images/0.jpg"));
    m_framelessWindow->setObjectName("FramelessWindowObject");
    m_framelessWindow->OnSetWindowTitle(QStringLiteral("Мастер первоначальной настройки"));
    m_framelessWindow->show();
}

void Program::InitStyleChanger()
{
    this->setStyle(QStringLiteral("Fusion"));
    m_styleChanger = new StyleChanger(this);
    m_styleChanger->OnChangeTheme(1);
    connect(m_framelessWindow, &FramelessWindow::ToChangeTheme, m_styleChanger, &StyleChanger::OnChangeTheme);
    connect(m_styleChanger, &StyleChanger::ToUpdateViewColors, m_tray, &Tray::ToUpdateViewColors);
    Q_EMIT m_tray->ToUpdateViewColors();
}

void Program::StartSettingsWizard(const LoadingState &state)
{
    m_startupWizard = new StartupWizard(this->applicationName(), m_rlstiFolder, state, m_linuxUserService, m_sqlDatabaseService, nullptr);
    connect(m_startupWizard, &StartupWizard::ToChangeTheme, m_styleChanger, &StyleChanger::OnChangeTheme);
    connect(m_startupWizard, &QWizard::accepted, this, &Program::OnFullLoading);
    connect(m_startupWizard, &QWizard::rejected, this, &QApplication::quit);
    m_framelessWindow->SetMainWidget(m_startupWizard);
    m_framelessWindow->repaint();
}

void Program::UserLoading()
{
    if (AllAppsRunnedWell())
    {
        InitRarmSocket();
        ConnectUserObjects();
    }
    else
    {
        QMessageBox::critical(nullptr, "Приложение не может запустить подпрограмму", "При наличии прав администратора измените парметры роли для пользователя: "+ m_currentUserName, QMessageBox::Ok);
    }
}

void Program::OnFullLoading()
{
    UserLoading();
    InitAdminUI();
    ConnectAdminObjects();
}

bool Program::AllAppsRunnedWell()
{
    return m_startupRunnableService->OnRunStartupRunnableManager();
}

void Program::InitRarmSocket()
{
    m_socketToRarm = new SocketToRarm(QStringLiteral("127.0.0.1"), 4242, this);
}

void Program::InitAdminUI()
{
    m_AdminGui = new Admin_GUI(m_sqlDatabaseService, m_linuxUserService, m_currentUserId, m_currentUserName, Q_NULLPTR);
    m_framelessWindow->OnSetWindowTitle("Панель управления пользователями и модулями РЛС ТИ");
    m_framelessWindow->SetMainWidget(m_AdminGui);
    m_framelessWindow->show();
}

void Program::ConnectUserObjects()
{
    connect(m_tray, &Tray::ToCloseApp, this, &QApplication::quit);
    connect(m_tray, &Tray::ToShowApp, m_framelessWindow, &FramelessWindow::show);
    connect(m_tray, &Tray::ToHideApp, m_framelessWindow, &QWidget::hide);
    connect(m_startupRunnableService, &StartupRunnableManager::ToProgramFall, m_socketToRarm, &SocketToRarm::OnProgramFall);
    connect(m_tray, &Tray::ToPauseUserControl, m_startupRunnableService, &StartupRunnableManager::OnPauseStartupRunnableManager);
    connect(m_tray, &Tray::ToStopUserControl, m_startupRunnableService, &StartupRunnableManager::OnStopStartupRunnableManager);
    connect(m_tray, &Tray::ToRestartUserControl, m_startupRunnableService, &StartupRunnableManager::OnRestartStartupRunnableManager);
    connect(m_tray, &Tray::ToResumeUserControl, m_startupRunnableService, &StartupRunnableManager::OnRunStartupRunnableManager);
}

void Program::ConnectAdminObjects()
{
    connect(m_framelessWindow, &FramelessWindow::ToSetDelegateView, m_AdminGui, &Admin_GUI::ToSetDelegateView);
    connect(m_framelessWindow, &FramelessWindow::ToHideAdditionalSettings, m_AdminGui, &Admin_GUI::OnHideAdditionalSettings);
    connect(m_AdminGui, &Admin_GUI::ToCurrentUserRoleChanged, m_startupRunnableService, &StartupRunnableManager::OnCurrentUserRoleChanged);
}

bool Program::CanGetAdminAccess()
{
    if (m_linuxUserService->HasCurrentUserAdminPrivileges())
    {
        if (m_sqlDatabaseService->CheckUsersTable())
        {
            const QStringList adminsUserName=m_sqlDatabaseService->GetAdminsRoleUserName();
            return  true;
//            return adminsUserName.contains(m_currentUserName);
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
