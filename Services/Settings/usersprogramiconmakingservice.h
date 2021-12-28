#ifndef USERSPROGRAMICONMAKINGSERVICE_H
#define USERSPROGRAMICONMAKINGSERVICE_H

#include <QObject>
#include <QDir>
#include "Services/linuxuserservice.h"
class UsersProgramIconMakingService : public QObject
{
    Q_OBJECT
public:
    explicit UsersProgramIconMakingService(const QString &destinationFolder, const QString &applicationName, LinuxUserService *service, QObject *parent);
    ~UsersProgramIconMakingService();

public Q_SLOTS:
    void OnAddIconToDesktopToCurrentUser(bool state);
    void OnAddIconToDesktopToAllUsers(bool state);
    void OnAddIconToStartMenu(bool state);
public:
    void ApplyWizardActions();
private:
    void CopyMyAppToRLSTIFolder();
    void AddIconToStartMenu();
    void AddIconToCurrentUserDesktop();
    void AddIconToAllUserDesktops();
private:
    const QString m_iconName="users.desktop";
private:
    bool m_addIconToDesktopToCurrentUser;
    bool m_addIconToDesktopToAllUsers;
    bool m_addIconToStartMenu;

private:
    const QString m_applicationName;
    const QString m_destinationFolder;
    LinuxUserService* const m_linuxUserService;
    Terminal* const m_terminal;
};

#endif // USERSPROGRAMICONMAKINGSERVICE_H
