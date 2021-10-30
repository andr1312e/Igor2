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

   ProgramState IsAllDataLoaded();

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

Q_SIGNALS:

   void AllDataLoaded();

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

   QString m_pathToSettingsFile;

   QSettings *m_setting;

private:

   const QString m_pathUserDBKey = "userdb";

   const QString m_viewModeKey = "viewmode";

   const QString m_themeKey = "blacked";

   const QString m_pathToExecsKey = "execslist";

   const QString m_pathToRoleFoldersKey = "rolefolders";

private:

   QString m_userDBPathDefaultValue;

   const QString m_viewModeDefaultValue = "full";

   const bool m_themeKeyDefaultValue = true;

   QString m_pathToExecsDefaultValue;

   QString m_pathToRoleFoldersDefaultValue;

private:

   QStringList m_validSettingsPaths;

   const QStringList m_defaultSettingsPaths;

private:

   QString GetPathUserDbFromSettings();

   QString GetViewModeFromSettings();

   bool GetThemeNameFromSettings();

   QString GetPathToExecFromSettings();

   QString GetPathToUserRolesFromSettings();

private:

   void OpenSettingsFile();

   void СreateSettingsFile();

   void GetValuesFromSettingsFile();

   ProgramState CheckIsPathsInSettingsFileCorrect();

   bool FilePathIsCorrect(const QString &filePath);

   bool FodlerPathIsExists(const QString &folderPath);

   void SetSettingsValueToDefault();

private Q_SLOTS:

   void wizardFinished(int result);

};

#endif //SERVICES_SETTINGS_FIRSTSTARTSETTINGSSERVICE_H
