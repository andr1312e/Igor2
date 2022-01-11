#ifndef USERSPROGRAMICONMAKINGSERVICE_H
#define USERSPROGRAMICONMAKINGSERVICE_H

#include <QObject>
#include <QDir>

#include "Services/linuxuserservice.h"

class IconMaker : public QObject
{
    Q_OBJECT
public:
    explicit IconMaker(const QString &applicationName, const QString &destinationFolder , LinuxUserService *service, QObject *parent);
    ~IconMaker();

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
    void AddIcon(const QString &desktopPath);

private:
    const QString m_iconName="users.desktop";

private:
    bool m_addIconToDesktopToCurrentUser;
    bool m_addIconToDesktopToAllUsers;
    bool m_addIconToStartMenu;

private:
    const QStringView m_applicationName;
    const QString m_destinationFolder;
    LinuxUserService* const m_linuxUserService;
    Terminal* const m_terminal;
};

#endif // USERSPROGRAMICONMAKINGSERVICE_H
