#ifndef SERVICES_SETTINGSFILESERVICE_H
#define SERVICES_SETTINGSFILESERVICE_H
#include <QDir>
#include <QFile>
#include <QSettings>
#include <QInputDialog>
#include <QMessageBox>

#include "globalfunctions.h"

#include "Services/Terminals/terminal.h"

class AppSettingsService
{
public:
    AppSettingsService(bool hasAdminPrivileges, QWidget *fakeWidget, Terminal *terminal);

    ~AppSettingsService();

    bool tryToLoadSettings();

    QString m_pathUserDB;

    QString m_viewMode;

    QString m_theme;

    QString m_pathToExecs;

    QString m_pathToRoleFolders;

private:

    bool m_hasAdminPrivileges;

    QWidget *m_fakeWidget;

    Terminal *m_terminal;

    QSettings *m_setting;

    const QString m_pathToSettingsFile=QDir::currentPath()+"/PROCESS_CONTROLLER.ini";

    const QString m_pathUserDBKey="userdb";

    const QString m_viewModeKey="viewmode";

    const QString m_themeKey="theme";

    const QString m_pathToExecsKey="execslist";

    const QString m_pathToRoleFoldersKey="rolefolders";

    const QString m_viewModeDefaultValue="full";

    const QString m_themeKeyDefaultValue="dark";

    const QString m_pathToExecsKeyDefaultValue="/home/user/RLS_TI/";

    const QString m_pathToRoleFoldersKeyDefaultValue="/home/user/RLS_TI/";

private:

    QString getPathUserDb();

    QString getViewMode();

    QString getThemeName();

    QString getPathToExec();

    QString getPathToUserRoles();

    bool settingsFileCreated();

    void createDefaultParameters();

    bool userDBPathCorrect(const QString &userDbPath);

    bool wantsUserToFix(QString &userDbPath);

};

#endif // SERVICES_SETTINGSFILESERVICE_H
