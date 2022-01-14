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
    m_minimizeAction=new QAction("Скрыть программу", this);
    m_restoreAction=new QAction("Показать программу", this);


    m_slopRunnableServiceButton=new QAction(QIcon(":/images/tray/stopBlack.png"), "", this);
    m_slopRunnableServiceButton->setToolTip("Остановить контроль");
    m_restartAllAppsRunnableServiceButton=new QAction(QIcon(":/images/tray/refreshBlack.png"), "", this);
    m_restartAllAppsRunnableServiceButton->setToolTip("Перезапустить все программы");
    m_startRunnableServiceButton=new QAction(QIcon(":/images/tray/playBlack.png"), "", this);
    m_startRunnableServiceButton->setToolTip("Запустить контроль");

    m_activateLogs=new QAction(QIcon(":/images/tray/checkBlack.png"), "", this);
    m_activateLogs->setToolTip("Включить информационные сообщения");
    m_disactivateLogs=new QAction(QIcon(":/images/tray/closeBlack.png"), "", this);
    m_disactivateLogs->setToolTip("Выключить информационные сообщения");

    m_weatherIcon=new QAction(QIcon(":/images/tray/weatherNotBlack.png"), "Данные о погоде не получены", this);

    m_dropAllDbAndClose=new QAction("Удалить базу данных и закрыть", this);
    m_quitAction=new QAction("Выход из программы", this);
}

void Tray::CreateUI()
{
    m_trayIcon=new QSystemTrayIcon(this);
    m_trayMenu=new TrayMenu(Q_NULLPTR);
}

void Tray::InitUIAndInsertWidgetIntoLayouts()
{

    m_trayMenu->AddAction(m_minimizeAction);
    m_trayMenu->AddAction(m_restoreAction);

    m_trayMenu->AddSpacing(m_spacing);
    m_trayMenu->AddSeparatorLineHorizontal();
    m_trayMenu->AddTextToMenu(QStringLiteral("Действия над контролем перезапуска:"));
    m_trayMenu->BeginInsertInRow();
    m_trayMenu->AddAction(m_slopRunnableServiceButton);
    m_trayMenu->AddAction(m_restartAllAppsRunnableServiceButton);
    m_trayMenu->AddAction(m_startRunnableServiceButton);
    m_trayMenu->EndInsertInRow();

    m_trayMenu->AddSpacing(m_spacing);
    m_trayMenu->AddSeparatorLineHorizontal();
    m_trayMenu->AddTextToMenu("Действия над логами:");
    m_trayMenu->BeginInsertInRow();
    m_trayMenu->AddAction(m_activateLogs);
    m_trayMenu->AddAction(m_disactivateLogs);
    m_trayMenu->EndInsertInRow();
    m_trayMenu->AddSeparatorLineHorizontal();
    m_trayMenu->AddAction(m_weatherIcon);
    m_trayMenu->AddSeparatorLineHorizontal();
    m_trayMenu->AddAction(m_dropAllDbAndClose);
    m_trayMenu->AddSeparatorLineHorizontal();
    m_trayMenu->AddAction(m_quitAction);

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
}

void Tray::OnShowMessage(const QString &message)
{
    m_trayIcon->showMessage("fdfads", message);
}

void Tray::OnActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Unknown:
        qFatal("Unknown error");
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
        m_trayIcon->setToolTip("Программа синхронизации пользователей\nТоварищъ: " + m_userName + " ваша роль:" + m_userRole);
    }
}
