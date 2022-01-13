#ifndef TRAY_H
#define TRAY_H

#include <QObject>
#include <QAction>
#include <QSystemTrayIcon>
#include <QMovie>
#include <QLabel>
#include <QPushButton>

#include "Tray/traymenu.h"

class Tray : public QObject
{
    Q_OBJECT
public:
    explicit Tray( QObject *parent);
    ~Tray();
private:
    void InitActions();
    void CreateUI();
    void InitUIAndInsertWidgetIntoLayouts();
    void ConnectObjects();
Q_SIGNALS:
    void ToHideApp();
    void ToShowApp();
    void ToCloseApp();
public Q_SLOTS:
    void OnShowMessage(const QString &message);
private Q_SLOTS:
    void OnActivated(QSystemTrayIcon::ActivationReason reason);
    void OnSetTrayIcon(int frame);
public:
    void SetUserNameAndRole(const QString &userName, const QString &role);
private:
    QString m_userName;
    QString m_userRole;

private:

    QAction *m_minimizeAction;
    QAction *m_restoreAction;

    QAction *m_slopRunnableServiceButton;
    QAction *m_restartAllAppsRunnableServiceButton;
    QAction *m_startRunnableServiceButton;

    QAction *m_activateLogs;
    QAction *m_disactivateLogs;

    QAction *m_weatherIcon;

    QAction *m_dropAllDbAndClose;
    QAction *m_quitAction;

    TrayMenu *m_trayIconMenu;
    QSystemTrayIcon *m_trayIcon;

    QMovie *m_trayMenuIconMovie;
    const int m_spacing=5;

};

#endif // TRAY_H
