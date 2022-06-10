#ifndef USERSPROGRAMICONMAKINGSERVICE_H
#define USERSPROGRAMICONMAKINGSERVICE_H

#include <QObject>
#include <QApplication>

#include "Services/linuxuserservice.h"

class IconMaker : public QObject
{
    Q_OBJECT
public:
    explicit IconMaker(const QString &applicationPath, const QString &applicationName, const QString &rlsTiFolder, LinuxUserService *service, QObject *parent);
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
    const QString m_iconName;

private:
    bool m_addIconToDesktopToCurrentUser;
    bool m_addIconToDesktopToAllUsers;
    bool m_addIconToStartMenu;

private:
    const QString m_applicationPath;
    const QStringView m_applicationName;
    const QString m_rlsTiFolder;
    LinuxUserService *const m_linuxUserService;
    Terminal *const m_terminal;
};

#endif // USERSPROGRAMICONMAKINGSERVICE_H
