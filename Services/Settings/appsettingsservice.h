#ifndef SERVICES_SETTINGSFILESERVICE_H
#define SERVICES_SETTINGSFILESERVICE_H
#include <QDir>
#include <QFile>
#include <QSettings>
#include <QInputDialog>
#include <QMessageBox>

#include "globalfunctions.h"

#include "Services/Terminals/terminal.h"
#include "Services/Settings/usersettingsrepository.h"
#include "Services/Settings/roleappsandstartupsettingsrepository.h"

#include "Styles/Frameless/framelesswindow.h"
#include "Admin_GUI/Views/Wizards/startupwizard.h"

class AppSettingsService:public QObject
{
    Q_OBJECT

public:

    AppSettingsService(QString &currentUserName, bool hasAdminPrivileges, QWidget *fakeWidget, Terminal *terminal);

    ~AppSettingsService();

    bool isAllSettingsLoaded();

    QString getUserDBPathValue();

    QString getViewModeValue();

    bool getThemeValue();

    QString getFolderPathToExecsValue();

    QString getFolderPathToRoleFoldersValue();

Q_SIGNALS:

    void allDataLoaded();

private:

    QString m_userDBPath;

    QString m_viewMode;

    bool m_theme;

    QString m_folderPathToExecs;

    QString m_folderPathToRoleFolders;

private:

    QString m_currentUserName;

    bool m_hasAdminPrivileges;

    QWidget *m_fakeWidget;

    Terminal *m_terminal;

private:

    QSettings *m_setting;

    const QString m_settingsFileName;

    const QString m_pathToSettingsFile;

    const QString m_pathUserDBKey="userdb";

    const QString m_viewModeKey="viewmode";

    const QString m_themeKey="whitetheme";

    const QString m_pathToExecsKey="execslist";

    const QString m_pathToRoleFoldersKey="rolefolders";

    const QString m_viewModeDefaultValue="full";

    const bool m_themeKeyDefaultValue=true;

    QString m_pathToExecsKeyDefaultValue="/home/user/RLS_TI/";

    QString m_pathToRoleFoldersKeyDefaultValue="/home/user/RLS_TI/";

private:

    CurrentUserWizardRepository *m_userWizardRepositry;

    RoleAndStartupWizardRepository *m_roleAndStartupWizardRepository;

    StartupWizard *m_startupWizard;

private:

    void setCurrentUserNameToDefaultPaths();

    QString getPathUserDbFromSettings();

    QString getViewModeFromSettings();

    bool getThemeNameFromSettings();

    QString getPathToExecFromSettings();

    QString getPathToUserRolesFromSettings();

private:

    void createSettingsFile();

    void initWizardServices();

    void initWizardUIAndShow();

    void createDefaultParameters();

    bool userDBPathCorrect(const QString &userDbPath);

    bool wantsUserToFix(QString &userDbPath);

private Q_SLOTS:

    void wizardFinished(int result);

};

#endif // SERVICES_SETTINGSFILESERVICE_H