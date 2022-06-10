#include "program.h"
#include <QDebug>

Program::Program(int &argc, char **argv)
    : QApplication(argc, argv)
    , m_rlstiFolder(QStringLiteral("/usr/RLS_TI/"))
    , m_terminal(Terminal::GetTerminal())
    , m_linuxUserService(new LinuxUserService())
    , m_currentUserName(m_linuxUserService->GetCurrentUserName())
    , m_currentUserId(m_linuxUserService->GetCurrentUserId())
    , m_sqlDatabaseService(new SqlDatabaseSerivce(this))
    , m_singleInstance(Q_NULLPTR)
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
    delete m_singleInstance;
    delete m_terminal;
    delete m_linuxUserService;
    delete m_sqlDatabaseService;
    delete m_startupRunnableService;
    delete m_tray;
    if (Q_NULLPTR != m_startupWizard)
    {
        delete m_startupWizard;
    }
    if (Q_NULLPTR != m_AdminGui)
    {
        delete m_AdminGui;
        delete m_styleChanger;
        delete m_framelessWindow;
    }
    if (Q_NULLPTR != m_socketToRarm)
    {
        delete m_socketToRarm;
    }
}

/**
 * log Проверяем на наличие копии программы в зависимости от аргументов командной строки
 * Если последний аргумент restart то запуск программы разрешается, и происходит присоединение к уже живому процессу
 * Если такого аргумента нет то запуск считается единичным и происходит проверка в спец классе
 */

bool Program::HasNoRunningInstance()
{
    Log4QtInfo(QStringLiteral("Аргументы командной строки приложения: ") + arguments().join(','));
    m_singleInstance = new SingleInstanceMaker(applicationName());
    if (QStringLiteral("restart") == arguments().constLast())
    {
        m_singleInstance->ConnectToExsistsApp();
        return true;
    }
    else
    {
        //Копий не было
        if (m_singleInstance->TryToExecute())
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Экземпляр программы НЕ запущен "));
            return true;
        }
        else
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Экземпляр программы запущен "));
            return false;
        }
    }
}

/**
 * log Главная точка программы
 * Получаем список пользователей и подключаемся к бд
 * Если бд не установлена или драйверов нет, или лог пасс не тот - фиксим и возращается что надо перезапуститься
 * Иначе проверям таблицы,и значения готовности state кладем в ProcessDataLoading
 * @return состояние подключения
 */

DbConnectionState Program::CreateAndRunApp()//MAIN
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Начало создания программы"));
    CollectAllUsersWithIdInSystem();
    const DbConnectionState connectionState = m_sqlDatabaseService->ConnectToDataBase();
    if (DbConnectionState::Connected == connectionState)
    {
        const LoadingStates states = GetLoadingStates();
        ContinueDataLoading(states);
    }
    else
    {
        if (DbConnectionState::NeedRestart == connectionState)
        {
            QStringList appArguments = qApp->arguments();
            if (!appArguments.isEmpty() && QLatin1Literal("restart") == appArguments.constLast())
            {
                Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Перезапускаемся еще раз, что то не то с логикой программы "));
                return DbConnectionState::FailedConnection;
            }
            else
            {
                appArguments.append(QLatin1Literal("restart"));
                QProcess::startDetached(qApp->arguments().front(), appArguments);
                Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Перезапускаем программу "));
            }
        }
    }
    return connectionState;
}
/**
 * Получить у UserService всех пользователей в системе, без системных
 */
void Program::CollectAllUsersWithIdInSystem()
{
    m_linuxUserService->GetAllUsersWithIdInSystem();
}

LoadingStates Program::GetLoadingStates()
{
    LoadingStates states;
    if (m_sqlDatabaseService->CheckUsersTable())
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Таблицы пользователей существует"));
        if (m_sqlDatabaseService->CheckDesktopTables() && m_sqlDatabaseService->CheckStartupTables())
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Таблицы ролей существуют"));
            states = LoadingState::Fine;
        }
        else
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Таблицы ролей отсутсвуют"));
            states = LoadingState::NoRoleData;
        }
    }
    else
    {
        if (m_sqlDatabaseService->CheckStartupTables() && m_sqlDatabaseService->CheckStartupTables())
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Таблицы ролей существуют"));
            states = LoadingState::NoUserDb;
        }
        else
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Таблицы ролей отсутсвуют"));
            states = LoadingState::NoFiles;
        }
    }
    if (QStringLiteral("--reset") == qApp->arguments().constLast())
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Вызвано принудительное восстановление..."));
        states = states  | LoadingState::ForceReset;
    }
    return  states;
}

void Program::ContinueDataLoading(LoadingStates states)
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Продолжаем загрузку..."));
    switch (states)
    {
    case LoadingState::NoUserDb | LoadingState::ForceReset:
    case LoadingState::NoRoleData| LoadingState::ForceReset:
    case LoadingState::NoFiles| LoadingState::ForceReset:
    case LoadingState::Fine| LoadingState::ForceReset:
    case LoadingState::NoUserDb :
    case LoadingState::NoRoleData:
    case LoadingState::NoFiles:
    {
        if (CanGetAdminAccess())
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Права разработчика доступны, но данные не полные, запускаем мастер настройки..."));
            const ThemesNames currentThemeName = GetThemeNameFromSettings();
            InitFramelessWindow(currentThemeName);
            InitStyleChanger(currentThemeName);
            StartSettingsWizard(states);
        }
        else
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Права разработчика недоступны и данные не полные, дальнейшая работа невозможна... "));
            QMessageBox::critical(Q_NULLPTR, QStringLiteral("Приложение не может запуститься"), QStringLiteral("Права разработчика недоступны, запустите программу от имени администратора и повторите попытку..."), QMessageBox::Ok);
            quit();
        }
        break;
    }
    case LoadingState::Fine:
    {
        if (CanGetAdminAccess())
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Права разработчика доступны, имеются все данные, запускаем программу администрирования..."));
            const ThemesNames currentThemeName = GetThemeNameFromSettings();
            InitFramelessWindow(currentThemeName);
            InitStyleChanger(currentThemeName);
            OnFullLoading();
        }
        else
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Права разработчика не доступны, но имеются все данные, запускаем программу контроля программ..."));
            UserLoading();
        }
        break;
    }
    default:
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Невозможно обработать ошибку так как обработчик не написан")).toUtf8().constData());
    }
    }
}

ThemesNames Program::GetThemeNameFromSettings() const
{

    const QSettings settings(organizationName(), applicationName());
    const ThemesNames themeName = qvariant_cast<ThemesNames>(settings.value(QLatin1Literal("theme")));
    return themeName;
}

void Program::InitFramelessWindow(ThemesNames themeName)
{
    m_framelessWindow = new FramelessWindow(themeName, Q_NULLPTR);
    setWindowIcon(QIcon(QLatin1Literal(":/images/programIcon.png")));
    m_framelessWindow->setObjectName(QLatin1Literal("FramelessWindowObject"));//QSS
    m_framelessWindow->OnSetWindowTitle(QStringLiteral("Мастер первоначальной настройки"));
    m_framelessWindow->show();
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Создали безрамочное окно "));
}

void Program::InitStyleChanger(ThemesNames themeName)
{
    m_styleChanger = new StyleChanger();
    m_styleChanger->OnChangeTheme(themeName);
    connect(m_framelessWindow, &FramelessWindow::ToChangeTheme, m_styleChanger, &StyleChanger::OnChangeTheme);
    connect(m_styleChanger, &StyleChanger::ToUpdateViewColors, m_tray, &Tray::ToUpdateViewColors);
    Q_EMIT m_tray->ToUpdateViewColors(themeName);
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Создали обьект изменения тем "));
}

void Program::StartSettingsWizard(LoadingStates states)
{
    m_startupWizard = new StartupWizard(applicationDirPath(), applicationName(), m_rlstiFolder, states, m_linuxUserService, m_sqlDatabaseService, m_styleChanger->GetThemeName(), Q_NULLPTR);
    connect(m_startupWizard, &QWizard::accepted, this, &Program::OnFullLoading);
    connect(m_startupWizard, &QWizard::rejected, this, &QApplication::quit);
    connect(m_framelessWindow, &FramelessWindow::ToChangeTheme, m_startupWizard, &StartupWizard::OnChangeTheme);
    m_framelessWindow->SetMainWidget(m_startupWizard);
    m_framelessWindow->repaint();
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Создали мастера первоначальной настройки "));
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
        QMessageBox::critical(Q_NULLPTR, QStringLiteral("Приложение не может запустить подпрограмму"), QStringLiteral("При наличии прав администратора измените парметры роли для пользователя: ") + m_currentUserName, QMessageBox::Ok);
    }
}

void Program::OnFullLoading()
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Запускаем админскую часть "));
    UserLoading();
    InitAdminUI();
    ConnectAdminObjects();
}

bool Program::AllAppsRunnedWell()
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Проверяем все ли программы возможно запустить"));
    return m_startupRunnableService->OnRunStartupRunnableManager();
}

void Program::InitRarmSocket()
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Создаем сокет к рарму "));
    m_socketToRarm = new SocketToRarm(QLatin1Literal("127.0.0.1"), 4242, this);
}

void Program::InitAdminUI()
{
    m_AdminGui = new Admin_GUI(m_sqlDatabaseService, m_linuxUserService, m_currentUserId, m_currentUserName, Q_NULLPTR);
    m_framelessWindow->OnSetWindowTitle(QStringLiteral("Панель управления пользователями и модулями РЛС ТИ"));
    m_framelessWindow->SetMainWidget(m_AdminGui);
    m_framelessWindow->show();

    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Создали админ вью "));
}

void Program::ConnectUserObjects()
{
    connect(m_tray, &Tray::ToCloseApp, this, &QApplication::quit);
    connect(m_tray, &Tray::ToShowApp, m_framelessWindow, &FramelessWindow::show);
    connect(m_tray, &Tray::ToHideApp, m_framelessWindow, &QWidget::hide);
    connect(m_startupRunnableService, &StartupRunnableManager::ToProgramFall, m_socketToRarm, &SocketToRarm::OnProgramFall);
    connect(m_socketToRarm, &SocketToRarm::ToSendWeather, m_tray, &Tray::OnSendWeather);
    connect(m_tray, &Tray::ToPauseUserControl, m_startupRunnableService, &StartupRunnableManager::OnPauseStartupRunnableManager);
    connect(m_tray, &Tray::ToStopUserControl, m_startupRunnableService, &StartupRunnableManager::OnStopStartupRunnableManager);
    connect(m_tray, &Tray::ToRestartUserControl, m_startupRunnableService, &StartupRunnableManager::OnRestartStartupRunnableManager);
    connect(m_tray, &Tray::ToResumeUserControl, m_startupRunnableService, &StartupRunnableManager::OnRunStartupRunnableManager);
}

void Program::ConnectAdminObjects()
{
    connect(m_tray, &Tray::ToDropDatabase, m_sqlDatabaseService, &SqlDatabaseSerivce::OnDropDatabase);
    connect(m_tray, &Tray::ToDropDatabase, this, &QApplication::quit, Qt::QueuedConnection);
    connect(m_AdminGui, &Admin_GUI::ToCurrentUserRoleChanged, m_startupRunnableService, &StartupRunnableManager::OnCurrentUserRoleChanged);
}

bool Program::CanGetAdminAccess()
{
    if (m_linuxUserService->HasCurrentUserAdminPrivileges())
    {
        if (m_sqlDatabaseService->CheckUsersTable())
        {
            const QStringList adminsUserName = m_sqlDatabaseService->GetAdminsRoleUserName();
            return  true;
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
