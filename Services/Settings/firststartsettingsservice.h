#ifndef SERVICES_SETTINGS_FIRSTSTARTSETTINGSSERVICE_H
#define SERVICES_SETTINGS_FIRSTSTARTSETTINGSSERVICE_H
#include <QDir>
#include <QFile>
#include <QSettings>
#include <QInputDialog>
#include <QMessageBox>

#include "functionswithstrings.h"
#include "Structs/userstruct.h"
#include "Services/Settings/programfilesstate.h"

#include "Services/Terminals/terminal.h"

class FirstStartSettingsService : public QObject
{
    Q_OBJECT

public:

    explicit FirstStartSettingsService(QString &currentUserName, QString &currentUserId, bool hasAdminPrivileges, QWidget *fakeWidget, Terminal *terminal);

    ~FirstStartSettingsService();

Q_SIGNALS:

    void ToAllDataLoaded();

private Q_SLOTS:

    void OnWizardFinished(int result);

public Q_SLOTS:

    void OnSetDbAndIconsPaths(const QStringList &paths);

public:

    LoadingState IsAllDataLoaded();

    QString &GetUserDBPathValue();

    QString &GetViewModeValue();

    bool GetThemeValue() const;

    QString &GetFolderPathToExecsValue();

    QString &GetFolderPathToRoleFoldersValue();

public:

    const QString &GetUserName() const;

    const QString &GetUserId() const;

    Terminal *GetTerminal() const;

    QStringList &GetValidSettingsPaths();

    const QStringList &GetDefaultSettingsPaths() const;

private:

    QString m_userDBPath;

    QString m_viewMode;

    bool m_theme;

    QString m_folderPathToExecs;

    QString m_folderPathToRoleFolders;

private:

    const QString m_currentUserId;

    const QString m_currentUserName;

    const bool m_hasAdminPrivileges;

    QWidget *m_fakeWidget;

    Terminal *m_terminal;

private:

    const QString m_pathToSettingsFile;

    const QString m_pathUserDBKey = "userdb";

    const QString m_viewModeKey = "viewmode";

    const QString m_themeKey = "blacked";

    const QString m_pathToExecsKey = "execslist1";

    const QString m_pathToRoleFoldersKey = "rolefolders1";

private:

    QString m_userDBPathDefaultValue;

    const QString m_viewModeDefaultValue = "full";

    const bool m_themeKeyDefaultValue = true;

    const QStringList m_defaultSettingsPaths;

    QStringList m_validSettingsPaths;

private:

    QString GetPathUserDbFromSettings(QSettings &settings);

    QString GetViewModeFromSettings(QSettings &settings);

    bool GetThemeNameFromSettings(QSettings &settings);

    QString GetPathToExecFromSettings(QSettings &settings);

    QString GetPathToUserRolesFromSettings(QSettings &settings);

    void SetSettingsValues(QSettings &settings);

private:

    void CreateSettingsFile();

    void GetValuesFromSettingsFile(QSettings &settings);

    LoadingState CheckIsPathsInSettingsFileCorrect();

    bool FilePathIsCorrect(const QString &filePath);

    bool FolderPathIsExists(const QString &folderPath);

};

#endif //SERVICES_SETTINGS_FIRSTSTARTSETTINGSSERVICE_H
