#include "program.h"
#include <QDebug>
#include <forward_list>
Program::Program(int &argc, char **argv)
    : QApplication(argc, argv)
    , m_rlstiFolder(QStringLiteral("/usr/RLS_TI/"))
    , m_sharedMemory(new QSharedMemory(QStringLiteral("PROCESS_CONTROLLER"), this))
    , m_terminal(Terminal::GetTerminal())
    , m_linuxUserService(Q_NULLPTR)
    , m_sqlDatabaseService(Q_NULLPTR)
    , m_startupRunnableService(Q_NULLPTR)
    , m_tray(Q_NULLPTR)
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
    if(Q_NULLPTR!=m_startupWizard)
    {
        delete m_startupWizard;
    }
    if(Q_NULLPTR!=m_socketToRarm)
    {
        delete m_socketToRarm;
    }
    if(Q_NULLPTR!=m_AdminGui)
    {
        delete m_AdminGui;
        delete m_framelessWindow;
        delete m_styleChanger;
    }
    ShutDownRootLogger();
}

bool Program::HasNoRunningInscance()
{
    if (m_sharedMemory->attach(QSharedMemory::ReadOnly)) {
        m_sharedMemory->detach();
        if(Log4Qt::Logger::rootLogger()->HasAppenders())
        {
            Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Экземпляр программы запущен "));
        }
        return false;
    } else {
        if (m_sharedMemory->create(1, QSharedMemory::ReadWrite)) {
            if(Log4Qt::Logger::rootLogger()->HasAppenders())
            {
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Экземпляров программы не запущено "));
            }
            return true;
        } else {
            if(Log4Qt::Logger::rootLogger()->HasAppenders())
            {
                Log4Qt::Logger::rootLogger()->warn(Q_FUNC_INFO + QStringLiteral(" Экземпляр программы запущен ") + m_sharedMemory->errorString());
            }
            return false;
        }
    }
}

void Program::CreateAndRunApp()//MAIN
{
    Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Начало создания программы"));
    CreateBasicServicesAndTray();
    GetAllUsersWithIdInSystem();
    ConnectToDatabase();
    const LoadingState state=GetProgramState();
    ProcessDataLoading(state);
}

void Program::CreateBasicServicesAndTray()
{
    m_linuxUserService=new LinuxUserService();
    m_currentUserName=m_linuxUserService->GetCurrentUserName();
    m_currentUserId=m_linuxUserService->GetCurrentUserId();
    m_sqlDatabaseService=new SqlDatabaseSerivce(this);
    m_startupRunnableService=new StartupRunnableManager(m_currentUserName, m_rlstiFolder, m_sqlDatabaseService, this);
    m_tray=new Tray(this);
    if(Log4Qt::Logger::rootLogger()->HasAppenders())
    {
        Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Создали базовые сервисы и иконку трея. "));
    }
}

void Program::GetAllUsersWithIdInSystem()
{
    m_linuxUserService->GetAllUsersWithIdInSystem();
    if(Log4Qt::Logger::rootLogger()->HasAppenders())
    {
        Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Получили список пользователей в системе "));
    }
}

void Program::ConnectToDatabase()
{
    if (!m_sqlDatabaseService->ConnectToDataBase(QStringLiteral("localhost"), 5432, QStringLiteral("postgres"), QStringLiteral("postgres"), QStringLiteral("user1234")))
    {
        if(Log4Qt::Logger::rootLogger()->HasAppenders())
        {
            Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Не можем подключится к бд: "));
        }
        QMessageBox::warning(Q_NULLPTR, QStringLiteral("Подключите базу данных postgres"), QStringLiteral("Подключение к бд не удалось"));
        this->exit(-1);
    }
}

LoadingState Program::GetProgramState()
{
    if (m_sqlDatabaseService->CheckUsersTable())
    {
        if(Log4Qt::Logger::rootLogger()->HasAppenders())
        {
            Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Таблицы пользователей существует"));
        }
        if(m_sqlDatabaseService->CheckDesktopTables() && m_sqlDatabaseService->CheckStartupTables())
        {
            if(Log4Qt::Logger::rootLogger()->HasAppenders())
            {
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Таблицы ролей существуют"));
            }
            return LoadingState::Fine;
        }
        else
        {
            if(Log4Qt::Logger::rootLogger()->HasAppenders())
            {
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Таблицы ролей отсутсвуют"));
            }
            return LoadingState::NoRoleData;
        }
    }
    else
    {
        if(Log4Qt::Logger::rootLogger()->HasAppenders())
        {
            Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Таблицы пользователей отсутсвуют"));
        }
        if(m_sqlDatabaseService->CheckStartupTables() && m_sqlDatabaseService->CheckStartupTables())
        {
            if(Log4Qt::Logger::rootLogger()->HasAppenders())
            {
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO+ QStringLiteral(" Таблицы ролей существуют"));
            }
            return LoadingState::NoUserDb;
        }
        else
        {
            if(Log4Qt::Logger::rootLogger()->HasAppenders())
            {
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Таблицы ролей отсутсвуют"));
            }
            return LoadingState::NoFiles;
        }
    }
    qFatal("%s", QString("%1 Нереализованное поведение ").arg(Q_FUNC_INFO).toUtf8().constData());
}

void Program::ProcessDataLoading(const LoadingState &state)
{
    if(Log4Qt::Logger::rootLogger()->HasAppenders())
    {
        Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Продолжаем загрузку..."));
    }
    switch (state) {
    case NoFiles:
    case NoUserDb:
    case NoRoleData: {
        if(CanGetAdminAccess())
        {
            if(Log4Qt::Logger::rootLogger()->HasAppenders())
            {
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Права разработчика доступны, но данные не полные, запускаем мастер настройки..."));
            }
            InitFramelessWindow();
            InitStyleChanger();
            StartSettingsWizard(state);
        }
        else
        {
            if(Log4Qt::Logger::rootLogger()->HasAppenders())
            {
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Права разработчика недоступны и данные не полные, дальнейшая работа невозможна... "));
            }
            QMessageBox::critical(Q_NULLPTR, "Приложение не может запуститься", "Права разработчика недоступны, запустите программу от имени администратора и повторите попытку...", QMessageBox::Ok);
        }
        break;
    }

    case Fine: {
        if(CanGetAdminAccess())
        {
            if(Log4Qt::Logger::rootLogger()->HasAppenders())
            {
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Права разработчика доступны, имеются все данные, запускаем программу администрирования..."));
            }
            InitFramelessWindow();
            InitStyleChanger();
            OnFullLoading();
        }
        else
        {
            if(Log4Qt::Logger::rootLogger()->HasAppenders())
            {
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Права разработчика не доступны, но имеются все данные, запускаем программу контроля программ..."));
            }
            UserLoading();
        }
        break;
    }
    default:{
        qFatal("%s", QString("%1 Нереализованное поведение...").arg(Q_FUNC_INFO).toUtf8().constData());
    }
    }
}

void Program::InitFramelessWindow()
{
    m_framelessWindow = new FramelessWindow(Q_NULLPTR);
    this->setWindowIcon(QIcon(":/images/0.jpg"));
    m_framelessWindow->setObjectName(QStringLiteral("FramelessWindowObject"));
    m_framelessWindow->show();
    if(Log4Qt::Logger::rootLogger()->HasAppenders())
    {
        Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Создали безрамочное окно "));
    }
}

void Program::InitStyleChanger()
{
    this->setStyle(QStringLiteral("Fusion"));
    m_styleChanger = new StyleChanger(true, this);
    m_styleChanger->OnChangeTheme(1);//получать тему из сеттингов
    connect(m_framelessWindow, &FramelessWindow::ToChangeTheme, m_styleChanger, &StyleChanger::OnChangeTheme);
    connect(m_styleChanger, &StyleChanger::ToUpdateViewColors, m_tray, &Tray::ToUpdateViewColors);
    Q_EMIT m_tray->ToUpdateViewColors();
    if(Log4Qt::Logger::rootLogger()->HasAppenders())
    {
        Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Создали обьект изменения тем "));
    }
}

void Program::StartSettingsWizard(const LoadingState &state)
{
    m_startupWizard = new StartupWizard(m_currentUserName, m_currentUserId, QCoreApplication::applicationName(), m_rlstiFolder, state, m_linuxUserService, m_sqlDatabaseService, Q_NULLPTR);
    connect(m_startupWizard, &StartupWizard::ToChangeTheme, m_styleChanger, &StyleChanger::OnChangeTheme);
    connect(m_startupWizard, &QWizard::accepted, this, &Program::OnFullLoading);
    connect(m_startupWizard, &QWizard::rejected, this, &QApplication::quit);
    m_framelessWindow->OnSetWindowTitle(QStringLiteral("Мастер первоначальной настройки"));
    m_framelessWindow->SetMainWidget(m_startupWizard);
    m_framelessWindow->repaint();
    if(Log4Qt::Logger::rootLogger()->HasAppenders())
    {
        Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Создали мастера первоначальной настройки "));
    }
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
        QMessageBox::critical(Q_NULLPTR, "Приложение не может запустить подпрограмму", "При наличии прав администратора измените парметры роли для пользователя: "+ m_currentUserName, QMessageBox::Ok);
    }
}

void Program::ShutDownRootLogger()
{
    Log4Qt::Logger* rootLogger = Log4Qt::Logger::rootLogger();
    rootLogger->info((QStringLiteral("Логи программы управления пользователями. Конец записи из деструктора: ") + QDateTime::currentDateTime().toString(Qt::ISODate)));
    rootLogger->removeAllAppenders();
    rootLogger->loggerRepository()->shutdown();
}

void Program::OnFullLoading()
{
    if(Log4Qt::Logger::rootLogger()->HasAppenders())
    {
        Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Запускаем админскую часть "));
    }
    UserLoading();
    InitAdminUI();
    ConnectAdminObjects();
}

bool Program::AllAppsRunnedWell()
{
    if(Log4Qt::Logger::rootLogger()->HasAppenders())
    {
        Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Проверяем все ли программы возможно запустить"));
    }
    return m_startupRunnableService->OnRunStartupRunnableManager();
}

void Program::InitRarmSocket()
{
    if(Log4Qt::Logger::rootLogger()->HasAppenders())
    {
        Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Создаем сокет к рарму "));
    }
    m_socketToRarm = new SocketToRarm(QStringLiteral("127.0.0.1"), 4242, this);
}

void Program::InitAdminUI()
{
    m_AdminGui = new Admin_GUI(m_sqlDatabaseService, m_linuxUserService, m_currentUserId, m_currentUserName, Q_NULLPTR);
    m_framelessWindow->OnSetWindowTitle("Панель управления пользователями и модулями РЛС ТИ");
    m_framelessWindow->SetMainWidget(m_AdminGui);
    m_framelessWindow->show();
    if(Log4Qt::Logger::rootLogger()->HasAppenders())
    {
        Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Создали админ вью "));
    }
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
    if (m_linuxUserService->HasUserAdminPrivileges(m_currentUserName))
    {
        if (m_sqlDatabaseService->CheckUsersTable())
        {
            const QStringList adminsUserName=m_sqlDatabaseService->GetAdminsRoleUserName();
            if(adminsUserName.empty())
            {
                return  true;
            }
            else
            {
                //            return adminsUserName.contains(m_currentUserName);
                return  true;
            }
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
