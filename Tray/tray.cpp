#include "tray.h"

Tray::Tray(QObject *parent)
    : QObject(parent)
{
    InitActions();
    CreateUI();
    InitUIAndInsertWidgetIntoLayouts();
    StartTimer();
    ConnectObjects();
}

Tray::~Tray()
{
    delete m_trayMenu;

    delete m_quitAction;
    delete m_restoreAction;
    delete m_minimizeAction;

    delete m_programIcon;
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

    m_dropAllDbAndClose=new QAction(QStringLiteral("Удалить базу данных и закрыть"));
    m_quitAction=new QAction(QStringLiteral("Выход из программы"), this);
}

void Tray::CreateUI()
{
    m_programIcon=new QSystemTrayIcon(this);
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
    m_trayMenu->AddButtonToMenu(m_dropAllDbAndClose);
    m_trayMenu->AddSeparatorLineHorizontal();
    m_trayMenu->AddButtonToMenu(m_quitAction);

    m_programIcon->setContextMenu(m_trayMenu);
    m_programIcon->setToolTip(QStringLiteral("Программа синхронизации пользователей"));
    m_programIcon->show();
}

void Tray::StartTimer()
{
    startTimer(1500, Qt::VeryCoarseTimer);
}

void Tray::ConnectObjects()
{
    connect(this, &Tray::ToUpdateViewColors, m_trayMenu, &TrayMenu::OnUpdateViewColors);
    connect(m_programIcon, &QSystemTrayIcon::activated, this, &Tray::OnActivated);

    connect(m_restoreAction, &QAction::triggered, this, &Tray::ToShowApp);
    connect(m_minimizeAction, &QAction::triggered, this, &Tray::ToHideApp);
    connect(m_quitAction, &QAction::triggered, this, &Tray::ToCloseApp);

    connect(m_pauseRunnableServiceButton, &QAction::triggered, this, &Tray::ToPauseUserControl);
    connect(m_slopRunnableServiceButton, &QAction::triggered, this, &Tray::ToStopUserControl);
    connect(m_restartAllAppsRunnableServiceButton, &QAction::triggered, this, &Tray::ToRestartUserControl);
    connect(m_resumeRunnableServiceButton, &QAction::triggered, this, &Tray::ToResumeUserControl);
}

void Tray::ShowMessage(const QString &message)
{
    m_programIcon->showMessage(QStringLiteral("Сообщение: "), message);
}

void Tray::OnSendWeather(const DevicesMeteoKitGetMessage &message)
{
    if(4==message.state)
    {
        if(qAbs(m_lastWeatherTemperature-message.temperature)>2.0)
        {
            m_lastWeatherTemperature=message.temperature;
            const QString iconPath(GetImageBasedOnData(message.temperature, message.wet));
            QPixmap icon(iconPath);
            QRect rectToDraw=icon.rect();
            rectToDraw.setTop(10);
            QString showedText(QStringLiteral("   "));
            if(message.temperature<0)
            {
                showedText.append(QString::number((int)message.temperature));
            }
            else
            {
                showedText.append(' ');
                showedText.append(QString::number((int)message.temperature));
            }
            const QPen m_iconTextPen(Qt::white);
            const QFont m_textFont(QStringLiteral("Carlito"), 22, QFont::Bold);
            QPainter painter(&icon);
            painter.setPen(m_iconTextPen);
            painter.setFont(m_textFont);
            painter.drawText(rectToDraw, Qt::AlignLeft| Qt::AlignVCenter, showedText);
            m_trayWeatherIcon->setIcon(icon);
        }
        if(!m_trayWeatherIcon->isVisible())
        {
            m_trayWeatherIcon->show();
        }
    }
    else
    {
        if(m_trayWeatherIcon->isVisible())
        {
            m_trayWeatherIcon->hide();
        }
    }
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

void Tray::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    m_programIcon->setIcon(GetNextProgramIcon());
}

QIcon Tray::GetNextProgramIcon()
{
    switch (m_programPixMapIndex) {
    case 0:
        m_programPixMapIndex++;
        return QIcon(QStringLiteral(":/images/tray/trayIcon0.png"));
    case 1:
        m_programPixMapIndex++;
        return QIcon(QStringLiteral(":/images/tray/trayIcon1.png"));
    default:
        m_programPixMapIndex=0;
        return QIcon(QStringLiteral(":/images/tray/trayIcon2.png"));
    }
}

QString Tray::GetImageBasedOnData(int temperature, int wet)
{
    if(temperature<-10)
    {
        return QStringLiteral(":/veryCold.png");
    }
    else
    {
        if(temperature<0)
        {
            return QStringLiteral(":/cold.png");
        }
        else
        {
            if(temperature<10)
            {
                if(wet>60)
                {
                    return QStringLiteral(":/warmlyRain.png");
                }
                else
                {
                    return QStringLiteral(":/warmlyDry.png");
                }
            }
            else
            {
                if(wet>60)
                {
                    return QStringLiteral(":/hotRain.png");
                }
                else
                {
                    return QStringLiteral(":/hotDry.png");
                }
            }
        }
    }
}
