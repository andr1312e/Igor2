#include "tray.h"

Tray::Tray(QObject *parent)
    : QObject(parent)

{
    InitActions();
    CreateUI();
    InitUI();
    ConnectObjects();
}

Tray::~Tray()
{
    delete m_movie;
    delete m_trayIconMenu;

    delete m_quitAction;
    delete m_restoreAction;
    delete m_minimizeAction;

    delete m_trayIcon;
}

void Tray::InitActions()
{
    m_minimizeAction=new QAction("Скрыть программу", this);
    m_restoreAction=new QAction("Показать программу", this);
    m_quitAction=new QAction("Выход", this);
}

void Tray::CreateUI()
{
    m_trayIcon=new QSystemTrayIcon(this);
    m_trayIconMenu=new QMenu();

}

void Tray::InitUI()
{
    m_trayIconMenu->addAction(m_minimizeAction);
    m_trayIconMenu->addAction(m_restoreAction);
    m_trayIconMenu->addAction(m_quitAction);

    m_movie=new QMovie(this);
    m_movie->setFileName(":/images/myapptray.gif");
    m_movie->start();
    m_trayIcon->setContextMenu(m_trayIconMenu);
    m_trayIcon->setToolTip("Программа синхронизации пользователей");
    m_trayIcon->show();
}

void Tray::ConnectObjects()
{
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &Tray::OnActivated);

    connect(m_restoreAction, &QAction::triggered, this, &Tray::ToShowApp);
    connect(m_minimizeAction, &QAction::triggered, this, &Tray::ToHideApp);
    connect(m_quitAction, &QAction::triggered, this, &Tray::ToCloseApp);

    connect(m_movie, &QMovie::frameChanged, this, &Tray::OnSetTrayIcon);
    if (m_movie->loopCount() != -1)
    {
        connect(m_movie,&QMovie::finished,m_movie,&QMovie::start);
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
    m_trayIcon->setIcon(QIcon(m_movie->currentPixmap()));
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
