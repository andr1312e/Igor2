#include "tray.h"

Tray::Tray(QObject *parent)
    : QObject(parent)
    , state(MenuShowState::FullControl)
{
    InitFullUI();
}

Tray::~Tray()
{
//    delete m_shortCut;
    delete m_trayMenu;

    delete m_quitAction;
    delete m_restoreAction;
    delete m_minimizeAction;

    delete m_programIcon;
}


void Tray::InitActions()
{
    m_minimizeAction = new QAction(QStringLiteral("Скрыть программу"), this);
    m_restoreAction = new QAction(QStringLiteral("Показать программу"), this);
    m_pauseRunnableServiceButton = new QAction();
    m_slopRunnableServiceButton = new QAction();
    m_restartAllAppsRunnableServiceButton = new QAction();
    m_resumeRunnableServiceButton = new QAction();
    m_activateLogs = new QAction();
    m_disactivateLogs = new QAction();
    m_dropAllDbAndClose = new QAction(QStringLiteral("Удалить базу данных и закрыть"));
    m_quitAction = new QAction(QStringLiteral("Выход из программы"), this);

}

void Tray::CreateUI()
{
    m_programIcon = new QSystemTrayIcon(this);
    m_trayWeatherIcon = new QSystemTrayIcon(this);
    m_trayMenu = new TrayMenu(Q_NULLPTR);
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

void Tray::FillObjects()
{
    m_minimizeAction->setToolTip(QStringLiteral("Скрыть приложение из панели задач"));
    m_pauseRunnableServiceButton->setObjectName(QLatin1Literal(":/images/tray/pause_black_notchecked.png"));
    m_pauseRunnableServiceButton->setToolTip(QStringLiteral("Приостановить контроль процессов"));
    m_slopRunnableServiceButton->setObjectName(QLatin1Literal(":/images/tray/stop_black_notchecked.png"));
    m_slopRunnableServiceButton->setToolTip(QStringLiteral("Остановить контроль процессов"));
    m_restartAllAppsRunnableServiceButton->setObjectName(QLatin1Literal(":/images/tray/refresh_black_notchecked.png"));
    m_restartAllAppsRunnableServiceButton->setToolTip(QStringLiteral("Перезапустить все программы"));
    m_resumeRunnableServiceButton->setObjectName(QLatin1Literal(":/images/tray/play_black_checked.png"));
    m_resumeRunnableServiceButton->setToolTip(QStringLiteral("Запустить контроль"));
    m_activateLogs->setObjectName(QLatin1Literal(":/images/tray/ok_black_checked.png"));
    m_activateLogs->setToolTip(QStringLiteral("Включить информационные сообщения"));
    m_disactivateLogs->setObjectName(QLatin1Literal(":/images/tray/close_black_notchecked.png"));
    m_disactivateLogs->setToolTip(QStringLiteral("Выключить информационные сообщения"));

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

    connect(m_activateLogs, &QAction::triggered, this, &Tray::OnActivateLogs);
    connect(m_disactivateLogs, &QAction::triggered, this, &Tray::OnDisableLogs);
    connect(m_dropAllDbAndClose, &QAction::triggered, this, &Tray::ToDropDatabase);

}

void Tray::ShowMessage(const QString &message)
{
    m_programIcon->showMessage(QStringLiteral("Сообщение: "), message);
}

void Tray::OnSendWeather(const DevicesMeteoKitGetMessage &message)
{
    if (4 == message.state)
    {
        if (qAbs(m_lastWeatherTemperature - message.temperature) > 2.0)
        {
            m_lastWeatherTemperature = message.temperature;
            const QString iconPath(GetImageBasedOnData(message.temperature, message.wet));
            QPixmap icon(iconPath);
            QRect rectToDraw = icon.rect();
            rectToDraw.setTop(10);
            QString showedText(QStringLiteral("   "));
            if (message.temperature < 0)
            {
                showedText.append(QString::number((int)message.temperature));
            }
            else
            {
                showedText.append(' ');
                showedText.append(QString::number((int)message.temperature));
            }
            const QPen m_iconTextPen(Qt::white);
            const QFont m_textFont(QLatin1Literal("Carlito"), 22, QFont::Bold);
            QPainter painter(&icon);
            painter.setPen(m_iconTextPen);
            painter.setFont(m_textFont);
            painter.drawText(rectToDraw, Qt::AlignLeft | Qt::AlignVCenter, showedText);
            m_trayWeatherIcon->setIcon(icon);
            m_trayWeatherIcon->setToolTip(QString("Погода: \n Температура: %1 ℃\n Влажность: %2 %\n Давление: %3 мм.р.с").arg(message.temperature).arg(message.wet).arg(message.pressure));
        }
        if (!m_trayWeatherIcon->isVisible())
        {
            m_trayWeatherIcon->show();
        }
    }
    else
    {
        if (m_trayWeatherIcon->isVisible())
        {
            m_trayWeatherIcon->hide();
        }
    }
}

void Tray::OnActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Unknown:
        qWarning("%s", QString(QString(Q_FUNC_INFO) + QStringLiteral(" Причина акцивации трея не определена")).toUtf8().constData());
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

void Tray::OnDisableLogs()
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Логи отключены пользователем"));
    Log4Qt::LogManager::setHandleQtMessages(false);
    Log4Qt::Logger  *const rootLogger = Log4Qt::Logger::rootLogger();
    rootLogger->resetLevel();
}

void Tray::OnActivateLogs()
{
    Log4Qt::Logger  *const rootLogger = Log4Qt::Logger::rootLogger();
    Log4Qt::LogManager::setHandleQtMessages(true);
    rootLogger->setLevel(Log4Qt::Level::INFO_INT);
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Логи включены пользователем"));
}

void Tray::OnPressShortCut()
{
    qInfo() << "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee";
}

void Tray::InitFullUI()
{
    InitActions();
    CreateUI();
    FillObjects();
    InitUIAndInsertWidgetIntoLayouts();
    StartTimer();
    ConnectObjects();
}

void Tray::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    m_programIcon->setIcon(GetNextProgramIcon());
    DevicesMeteoKitGetMessage message;
    message.temperature = 20;
    message.wet = 30;
    message.pressure = 569;
    message.state = 4;
    OnSendWeather(message);
}

QIcon Tray::GetNextProgramIcon()
{
    switch (m_programPixMapIndex)
    {
    case 0:
        m_programPixMapIndex++;
        return QIcon(QLatin1Literal(":/images/tray/trayIcon0.png"));
    case 1:
        m_programPixMapIndex++;
        return QIcon(QLatin1Literal(":/images/tray/trayIcon1.png"));
    default:
        m_programPixMapIndex = 0;
        return QIcon(QLatin1Literal(":/images/tray/trayIcon2.png"));
    }
}

QString Tray::GetImageBasedOnData(int temperature, int wet)
{
    if (temperature < -15)
    {
        return QLatin1Literal(":/images/weather/veryCold.png");
    }
    else
    {
        if (temperature < 0)
        {
            return QLatin1Literal(":/images/weather/сold.png");
        }
        else
        {
            if (temperature < 15)
            {
                if (wet > 60)
                {
                    return QLatin1Literal(":/images/weather/warmlyRain.png");
                }
                else
                {
                    return QLatin1Literal(":/images/weather/warmlyDry.png");
                }
            }
            else
            {
                if (wet > 60)
                {
                    return QLatin1Literal(":/images/weather/hotRain.png");
                }
                else
                {
                    return QLatin1Literal(":/images/weather/hotDry.png");
                }
            }
        }
    }
}
