#include "tray.h"

Tray::Tray(QObject *parent)
    : QObject(parent)
{
    InitActions();
    CreateUI();
    InitUIAndInsertWidgetIntoLayouts();
    ConnectObjects();
}

Tray::~Tray()
{
    delete m_trayMenuIconMovie;
    delete m_trayMenu;

    delete m_quitAction;
    delete m_restoreAction;
    delete m_minimizeAction;

    delete m_trayIcon;
}

void Tray::InitActions()
{
    m_minimizeAction=new QAction(QStringLiteral("Скрыть программу"), this);
    m_restoreAction=new QAction(QStringLiteral("Показать программу"), this);

    m_pauseRunnableServiceButton=new QAction();
    m_pauseRunnableServiceButton->setObjectName(QStringLiteral(":/images/tray/pause_black_notchecked.png"));
    m_slopRunnableServiceButton=new QAction();
    m_slopRunnableServiceButton->setObjectName(QStringLiteral(":/images/tray/stop_black_notchecked.png"));
    m_slopRunnableServiceButton->setToolTip(QStringLiteral("Остановить контроль"));
    m_restartAllAppsRunnableServiceButton=new QAction();
    m_restartAllAppsRunnableServiceButton->setObjectName(QStringLiteral(":/images/tray/refresh_black_notchecked.png"));
    m_restartAllAppsRunnableServiceButton->setToolTip(QStringLiteral("Перезапустить все программы"));
    m_resumeRunnableServiceButton=new QAction();
    m_resumeRunnableServiceButton->setObjectName(QStringLiteral(":/images/tray/play_black_checked.png"));
    m_resumeRunnableServiceButton->setToolTip(QStringLiteral("Запустить контроль"));

    m_activateLogs=new QAction();
    m_activateLogs->setObjectName(QStringLiteral(":/images/tray/ok_black_checked.png"));
    m_activateLogs->setToolTip(QStringLiteral("Включить информационные сообщения"));
    m_disactivateLogs=new QAction();
    m_disactivateLogs->setObjectName(QStringLiteral(":/images/tray/close_black_notchecked.png"));
    m_disactivateLogs->setToolTip(QStringLiteral("Выключить информационные сообщения"));

    m_weatherIcon=new QAction(QStringLiteral("Данные о погоде не получены"));
    m_weatherIcon->setObjectName(QStringLiteral(":/images/tray/weatherNotBlack.png"));

    m_dropAllDbAndClose=new QAction(QStringLiteral("Удалить базу данных и закрыть"));
    m_quitAction=new QAction(QStringLiteral("Выход из программы"), this);
}

void Tray::CreateUI()
{
    m_trayIcon=new QSystemTrayIcon(QIcon(), this);
    m_trayMenu=new TrayMenu(Q_NULLPTR);
}

void Tray::InitUIAndInsertWidgetIntoLayouts()
{
    m_trayMenu->AddButtonToMenu(m_minimizeAction);
    m_trayMenu->AddButtonToMenu(m_restoreAction);

    m_trayMenu->AddSpacing(m_spacing);
    m_trayMenu->AddSeparatorLineHorizontal();
    m_trayMenu->AddTextToMenu(QStringLiteral("Действия над контролем перезапуска:"));
    m_trayMenu->BeginInsertInRow();
    m_trayMenu->AddButtonToMenu(m_pauseRunnableServiceButton);
    m_trayMenu->AddButtonToMenu(m_slopRunnableServiceButton);
    m_trayMenu->AddButtonToMenu(m_restartAllAppsRunnableServiceButton);
    m_trayMenu->AddButtonToMenu(m_resumeRunnableServiceButton);
    m_trayMenu->EndInsertInRow();

    m_trayMenu->AddSpacing(m_spacing);
    m_trayMenu->AddSeparatorLineHorizontal();
    m_trayMenu->AddTextToMenu(QStringLiteral("Действия над логами:"));
    m_trayMenu->BeginInsertInRow();
    m_trayMenu->AddButtonToMenu(m_activateLogs);
    m_trayMenu->AddButtonToMenu(m_disactivateLogs);
    m_trayMenu->EndInsertInRow();
    m_trayMenu->AddSeparatorLineHorizontal();
    m_trayMenu->AddButtonToMenu(m_weatherIcon);
    m_trayMenu->AddSeparatorLineHorizontal();
    m_trayMenu->AddButtonToMenu(m_dropAllDbAndClose);
    m_trayMenu->AddSeparatorLineHorizontal();
    m_trayMenu->AddButtonToMenu(m_quitAction);

    m_trayMenuIconMovie=new QMovie(this);
    m_trayMenuIconMovie->setFileName(QStringLiteral(":/images/myapptray.gif"));
    m_trayMenuIconMovie->start();
    m_trayIcon->setContextMenu(m_trayMenu);
    m_trayIcon->setToolTip(QStringLiteral("Программа синхронизации пользователей"));
    m_trayIcon->show();
}

void Tray::ConnectObjects()
{
    connect(this, &Tray::ToUpdateViewColors, m_trayMenu, &TrayMenu::OnUpdateViewColors);
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &Tray::OnActivated);

    connect(m_restoreAction, &QAction::triggered, this, &Tray::ToShowApp);
    connect(m_minimizeAction, &QAction::triggered, this, &Tray::ToHideApp);
    connect(m_quitAction, &QAction::triggered, this, &Tray::ToCloseApp);

    connect(m_trayMenuIconMovie, &QMovie::frameChanged, this, &Tray::OnSetTrayIcon);
    if (m_trayMenuIconMovie->loopCount() != -1)
    {
        connect(m_trayMenuIconMovie,&QMovie::finished,m_trayMenuIconMovie,&QMovie::start);
    }

    connect(m_pauseRunnableServiceButton, &QAction::triggered, this, &Tray::ToPauseUserControl);
    connect(m_slopRunnableServiceButton, &QAction::triggered, this, &Tray::ToStopUserControl);
    connect(m_restartAllAppsRunnableServiceButton, &QAction::triggered, this, &Tray::ToRestartUserControl);
    connect(m_resumeRunnableServiceButton, &QAction::triggered, this, &Tray::ToResumeUserControl);
}

void Tray::OnShowMessage(const QString &message)
{
    m_trayIcon->showMessage(QStringLiteral("Сообщение"), message);
}

void Tray::OnActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Unknown:
        qWarning("%s", QString(QString(Q_FUNC_INFO)+QStringLiteral(" Причина акцивации трея не определена")).toUtf8().constData());
        break;
    case QSystemTrayIcon::Context://пкм
        break;
    case QSystemTrayIcon::DoubleClick:
        break;
    case QSystemTrayIcon::Trigger://лкм
        Q_EMIT ToShowApp();
        break;
    case QSystemTrayIcon::MiddleClick://колесико
        Q_EMIT ToHideApp();
        break;

    }
}

void Tray::OnSetTrayIcon(int frame)
{
    Q_UNUSED(frame)
    m_trayIcon->setIcon(QIcon(m_trayMenuIconMovie->currentPixmap()));
}

void Tray::SetUserNameAndRole(const QString &userName, const QString &role)
{
    if(!userName.isEmpty() && !m_userRole.isEmpty())
    {
        m_userName=userName;
        m_userRole=role;
        m_trayIcon->setToolTip(QStringLiteral("Программа синхронизации пользователей\nТоварищ: ") + m_userName + " ваша роль:" + m_userRole);
    }
}
