#ifndef TRAY_TRAY_H
#define TRAY_TRAY_H

#include <QObject>
#include <QAction>
#include <QSystemTrayIcon>
#include <QMovie>
#include <QLabel>
#include <QPushButton>

#include "Styles/Themes/stylechanger.h"
#include "Logging/logger.h"
#include "Logging/logmanager.h"
#include "Server/DataMessage.h"
#include "Tray/traymenu.h"

enum class MenuShowState
{
    ViewState =  1,
    FullControl = 2,
};

class Tray : public QObject
{
    Q_OBJECT
public:
    explicit Tray(QObject *parent);
    ~Tray();
private:
    void InitActions();
    void CreateUI();
    void InitUIAndInsertWidgetIntoLayouts();
    void FillObjects();
    void StartTimer();
    void ConnectObjects();
Q_SIGNALS:
    void ToHideApp();
    void ToShowApp();
    void ToCloseApp();
    void ToUpdateViewColors(ThemesNames themeName);
    void ToPauseUserControl();
    void ToStopUserControl();
    void ToRestartUserControl();
    void ToResumeUserControl();
    void ToDropDatabase();
public Q_SLOTS:
    void OnSendWeather(const DevicesMeteoKitGetMessage &message);
private Q_SLOTS:
    void OnActivated(QSystemTrayIcon::ActivationReason reason);
    void OnDisableLogs();
    void OnActivateLogs();
public:
    void InitFullUI();
    void ShowMessage(const QString &message);
protected:
    virtual void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
private:
    QIcon GetNextProgramIcon();
    QString GetImageBasedOnData(int temperature, int wet);
private:

    QAction *m_minimizeAction;
    QAction *m_restoreAction;

    QAction *m_pauseRunnableServiceButton;
    QAction *m_slopRunnableServiceButton;
    QAction *m_restartAllAppsRunnableServiceButton;
    QAction *m_resumeRunnableServiceButton;

    QAction *m_activateLogs;
    QAction *m_disactivateLogs;

    QAction *m_dropAllDbAndClose;
    QAction *m_quitAction;

    TrayMenu *m_trayMenu;

private:
    const MenuShowState state;
    QSystemTrayIcon *m_programIcon;
    int m_programPixMapIndex = 0;
    QSystemTrayIcon *m_trayWeatherIcon;
    float m_lastWeatherTemperature = 0.0f;
    int index = 1;

    const int m_spacing = 5;
};

#endif // TRAY_TRAY_H
