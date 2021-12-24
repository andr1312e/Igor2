#ifndef TRAY_H
#define TRAY_H

#include <QObject>
#include <QAction>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QMovie>

class Tray : public QObject
{
    Q_OBJECT
public:
    explicit Tray( QObject *parent);
    ~Tray();
private:
    void InitActions();
    void CreateUI();
    void InitUI();
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

   QAction *m_minimizeAction;
   QAction *m_restoreAction;
   QAction *m_quitAction;

   QMenu *m_trayIconMenu;
   QSystemTrayIcon *m_trayIcon;

   QMovie *m_movie;

};

#endif // TRAY_H
