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
    , m_sqlAdjuster(Q_NULLPTR)
    , m_oldDbState(DbState::DbConnected)
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
    if(Q_NULLPTR!=m_startupWizard)
    {
        delete m_startupWizard;
    }
    if(Q_NULLPTR!=m_AdminGui)
    {
        delete m_AdminGui;
        delete m_styleChanger;
        delete m_framelessWindow;
    }
    if(Q_NULLPTR!=m_socketToRarm)
    {
        delete m_socketToRarm;
    }
}

bool Program::HasNoRunningInstance()
{
    Log4QtInfo(qApp->arguments().join(','));
    m_singleInstance= new SingleInstanceMaker(applicationName());
    if(QStringLiteral("restart")==arguments().last())
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
        } else {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Экземпляр программы запущен "));
            return false;
        }
    }
}

bool Program::CreateAndRunApp()//MAIN
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Начало создания программы"));
    GetAllUsersWithIdInSystem();
    const bool needToRestart=ConnectToDatabase();
    if(needToRestart)
    {
        QStringList appArguments=qApp->arguments();
        appArguments.append(QStringLiteral("restart"));
        QProcess::startDetached(qApp->arguments().front(), appArguments);
        return -1;
    }
    else
    {
        const LoadingState state=GetProgramState();
        ProcessDataLoading(state);
        return 0;
    }
}

void Program::GetAllUsersWithIdInSystem()
{
    m_linuxUserService->GetAllUsersWithIdInSystem();
}

bool Program::ConnectToDatabase()
{
    const bool needToRestart=true;
    const QString iniFilePath(Program::applicationDirPath()+QStringLiteral("/sql.ini"));
    QSettings setting(iniFilePath, QSettings::IniFormat, this);
    const QString host=setting.value(QStringLiteral("host"), QStringLiteral("localhost")).toString();
    const quint16 port=setting.value(QStringLiteral("port"), 5432).toUInt();
    const QString dbName=setting.value(QStringLiteral("dbName"), QStringLiteral("postgres")).toString();
    const QString userName=setting.value(QStringLiteral("userName"), QStringLiteral("user")).toString();
    const QString userPassword=setting.value(QStringLiteral("userPassword"), QStringLiteral("user1234")).toString();
    Q_FOREVER
    {
        const DbState state=m_sqlDatabaseService->ConnectToDataBase(host, port, dbName, userName, userPassword);
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Состояние подключения: ")+ QString::number(state));
        if(DbState::DbConnected==state)
        {
            break;
        }
        else
        {
            if(state==m_oldDbState)
            {
                QString errorReason;
                switch (state) {
                case DbNoPostgre:
                    errorReason=QStringLiteral(" не смогли установить постгрес");
                    break;
                case DbNoDriver:
                    errorReason=QStringLiteral(" не смогли установить драйвер постгре для кути");
                    break;
                case DbWrongPassword:
                    errorReason=QStringLiteral(" не смогли поменять пароль для бд (нет пользователя postgres или смена пароля запрещена)");
                    break;
                case DbUnknownError:
                    errorReason=QStringLiteral(" неизвестная ошибка");
                    break;
                default:
                    errorReason=QStringLiteral(" нереализованное поведение ")+Q_FUNC_INFO;
                    break;
                }
                QMessageBox::warning(Q_NULLPTR, QStringLiteral("Подключите базу данных postgres"), QStringLiteral("Подключение к бд не удалось, причина: ")+errorReason);
                exit(1);
            }
            else
            {
                if(Q_NULLPTR==m_sqlAdjuster)
                {
                    m_sqlAdjuster=new SqlAdjuster();
                }
                switch (state) {
                case DbNoPostgre:
                    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Не можем подключится к бд нет пакета постгре установленого"));
                    m_tray->ShowMessage(QStringLiteral("Устанавливаем базу данных Postgre SQL. Пожалуйста подождите..."));
                    m_sqlAdjuster->InstallPostgreSqlAndDriver();
                    return needToRestart;
                case DbNoDriver:
                    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Не можем подключится к бд нет драйвера установленного"));
                    m_tray->ShowMessage(QStringLiteral("Устанавливаем драйвер базы данных Postgre SQL. Пожалуйста подождите... "));
                    m_sqlAdjuster->InstallSqlDriverForQt5();
                    return needToRestart;
                case DbWrongPassword:
                    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Не можем подключится к бд неверный пароль и/или имя пользователя"));
                    m_tray->ShowMessage(QStringLiteral("Postgre SQL создаем пользователя ")+ userName + ':'+userPassword);
                    m_sqlAdjuster->ResetPostgreUserPassword(userName, userPassword);
                    break;
                case DbUnknownError:
                    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Невозможно запустить приложение, ошибка в процессе подключения к бд. Ошибка не известная. Проверьте пользоватлей, пакет драйвера к бд, и установлен ли пакет постгре. Переустановите их если ошибка не убирается"));
                    QMessageBox::warning(Q_NULLPTR, QStringLiteral("Неизвестная ошибка"), QStringLiteral("смотрите ошибку в файле логов"));
                    exit(1);
                default:
                    qFatal("%s", QString(Q_FUNC_INFO+ QStringLiteral(" Невозможно обработать ошибку так как обработчик не написан")).toUtf8().constData());
                    break;
                }
            }
        }
    }
    return false;
}

LoadingState Program::GetProgramState()
{
    if (m_sqlDatabaseService->CheckUsersTable())
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Таблицы пользователей существует"));
        if(m_sqlDatabaseService->CheckDesktopTables() && m_sqlDatabaseService->CheckStartupTables())
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Таблицы ролей существуют"));
            return LoadingState::Fine;
        }
        else
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Таблицы ролей отсутсвуют"));
            return LoadingState::NoRoleData;
        }
    }
    else
    {
        if(m_sqlDatabaseService->CheckStartupTables() && m_sqlDatabaseService->CheckStartupTables())
        {
            Log4QtInfo(Q_FUNC_INFO+ QStringLiteral(" Таблицы ролей существуют"));
            return LoadingState::NoUserDb;
        }
        else
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Таблицы ролей отсутсвуют"));
            return LoadingState::NoFiles;
        }
    }
    qFatal("%s", QString(Q_FUNC_INFO+ QStringLiteral(" Нереализованное поведение")).toUtf8().constData());
}

void Program::ProcessDataLoading(const LoadingState &state)
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Продолжаем загрузку..."));
    switch (state) {
    case NoFiles:
    case NoUserDb:
    case NoRoleData:
    {
        if(CanGetAdminAccess())
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Права разработчика доступны, но данные не полные, запускаем мастер настройки..."));
            const ThemesNames currentThemeName=GetThemeNameFromSettings();
            InitFramelessWindow(currentThemeName);
            InitStyleChanger(currentThemeName);
            StartSettingsWizard(state);
        }
        else
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Права разработчика недоступны и данные не полные, дальнейшая работа невозможна... "));
            QMessageBox::critical(Q_NULLPTR, QStringLiteral("Приложение не может запуститься"), QStringLiteral("Права разработчика недоступны, запустите программу от имени администратора и повторите попытку..."), QMessageBox::Ok);
        }
        break;
    }

    case Fine: {
        if(CanGetAdminAccess())
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Права разработчика доступны, имеются все данные, запускаем программу администрирования..."));
            const ThemesNames currentThemeName=GetThemeNameFromSettings();
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
    default:{
        qFatal("%s", QString(Q_FUNC_INFO+ QStringLiteral(" Невозможно обработать ошибку так как обработчик не написан")).toUtf8().constData());
    }
    }
}

ThemesNames Program::GetThemeNameFromSettings() const
{

    const QSettings settings(organizationName(), applicationName());
    const ThemesNames themeName=qvariant_cast<ThemesNames>(settings.value(QStringLiteral("theme"), ThemesNames::BlackTheme));
    return themeName;
}

void Program::InitFramelessWindow(ThemesNames themeName)
{
    m_framelessWindow = new FramelessWindow(themeName, Q_NULLPTR);
    setWindowIcon(QIcon(QStringLiteral(":/images/0.jpg")));
    m_framelessWindow->setObjectName(QStringLiteral("FramelessWindowObject"));//QSS
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
    Q_EMIT m_tray->ToUpdateViewColors();
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Создали обьект изменения тем "));
}

void Program::StartSettingsWizard(const LoadingState &state)
{
    m_startupWizard = new StartupWizard(applicationName(), m_rlstiFolder, state, m_linuxUserService, m_sqlDatabaseService, m_styleChanger->GetThemeName(), Q_NULLPTR);
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
        QMessageBox::critical(Q_NULLPTR, QStringLiteral("Приложение не может запустить подпрограмму"), QStringLiteral("При наличии прав администратора измените парметры роли для пользователя: ")+ m_currentUserName, QMessageBox::Ok);
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
    m_socketToRarm = new SocketToRarm(QStringLiteral("127.0.0.1"), 4242, this);
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
