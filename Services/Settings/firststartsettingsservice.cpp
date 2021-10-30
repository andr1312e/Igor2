#include "firststartsettingsservice.h"

FirstStartSettingsService::FirstStartSettingsService(QString &currentUserName, QString &currentUserId, bool hasAdminPrivileges, QWidget *fakeWidget, Terminal *terminal)
   : m_currentUserId(currentUserId)
   , m_currentUserName(currentUserName)
   , m_hasAdminPrivileges(hasAdminPrivileges)
   , m_fakeWidget(fakeWidget)
   , m_terminal(terminal)
   , m_pathToSettingsFile(QDir::currentPath() + "/sync_settings.ini")
   , m_defaultSettingsPaths(
{"/home/" + m_currentUserName + "/sync_db.userdb", "/home/" + m_currentUserName + "/RLS_TI/", "/home/" + m_currentUserName + "/RLS_TI/"
})
{

}

FirstStartSettingsService::~FirstStartSettingsService()
{
   delete m_setting;
}

ProgramState FirstStartSettingsService::IsAllDataLoaded()//main function
{
   if (m_terminal->IsFileNotExists(m_pathToSettingsFile, "AppFirstLoadlingSettingsService::isAllDataLoaded", m_hasAdminPrivileges)) {
      if (m_hasAdminPrivileges) {
         СreateSettingsFile();
         OpenSettingsFile();
         return ProgramState::NoFiles;
      } else {
         return ProgramState::CantRun;
      }
   } else {
      OpenSettingsFile();
      GetValuesFromSettingsFile();
      return CheckIsPathsInSettingsFileCorrect();
   }
}

QString &FirstStartSettingsService::GetUserDBPathValue()
{
   return m_userDBPath;
}

QString &FirstStartSettingsService::GetViewModeValue()
{
   return m_viewMode;
}

bool FirstStartSettingsService::GetThemeValue() const
{
   return m_theme;
}

QString &FirstStartSettingsService::GetFolderPathToExecsValue()
{
   return m_folderPathToExecs;
}

QString &FirstStartSettingsService::GetFolderPathToRoleFoldersValue()
{
   return m_folderPathToRoleFolders;
}

const QString &FirstStartSettingsService::GetUserId() const
{
   return m_currentUserId;
}

Terminal *FirstStartSettingsService::GetTerminal() const
{
   return m_terminal;
}

QStringList &FirstStartSettingsService::GetValidSettingsPaths()
{
   return m_validSettingsPaths;
}

const QStringList &FirstStartSettingsService::GetDefaultSettingsPaths() const
{
   return m_defaultSettingsPaths;
}

const QString &FirstStartSettingsService::GetUserName() const
{
   return m_currentUserName;
}

QString FirstStartSettingsService::GetPathUserDbFromSettings()
{
   return m_setting->value(m_pathUserDBKey, "").toString();
}

QString FirstStartSettingsService::GetViewModeFromSettings()
{
   return m_setting->value(m_viewModeKey, m_viewModeDefaultValue).toString();
}

bool FirstStartSettingsService::GetThemeNameFromSettings()
{
   return m_setting->value(m_themeKey, m_themeKeyDefaultValue).toBool();
}

QString FirstStartSettingsService::GetPathToExecFromSettings()
{
   return m_setting->value(m_pathToExecsKey, "").toString();
}

QString FirstStartSettingsService::GetPathToUserRolesFromSettings()
{
   return m_setting->value(m_pathToRoleFoldersKey, "").toString();
}

void FirstStartSettingsService::OpenSettingsFile()
{
   m_setting = new QSettings(m_pathToSettingsFile, QSettings::IniFormat);
}

void FirstStartSettingsService::СreateSettingsFile()
{
   if (m_terminal->IsFileNotExists(m_pathToSettingsFile, "AppFirstLoadlingSettingsService::СreateSettingsFile", m_hasAdminPrivileges)) {
      m_terminal->CreateFile(m_pathToSettingsFile, "AppFirstLoadlingSettingsService::СreateSettingsFile", m_hasAdminPrivileges);
   }
}

void FirstStartSettingsService::GetValuesFromSettingsFile()
{
   m_userDBPath = GetPathUserDbFromSettings();
   m_viewMode = GetViewModeFromSettings();
   m_theme = GetThemeNameFromSettings();
   m_folderPathToExecs = GetPathToExecFromSettings();
   m_folderPathToRoleFolders = GetPathToUserRolesFromSettings();
}

ProgramState FirstStartSettingsService::CheckIsPathsInSettingsFileCorrect()
{
   if (FodlerPathIsExists(m_folderPathToExecs) || FodlerPathIsExists(m_folderPathToRoleFolders)) {
      if (FilePathIsCorrect(m_userDBPath)) {
         return ProgramState::Fine;
      } else {
         if (m_hasAdminPrivileges) {
            m_validSettingsPaths.append(m_folderPathToRoleFolders);
            m_validSettingsPaths.append(m_folderPathToExecs);
            return ProgramState::NoUserDb;
         } else {
            return ProgramState::CantRun;
         }
      }
   } else {
      if (m_hasAdminPrivileges) {
         if (FilePathIsCorrect(m_userDBPath)) {
            m_validSettingsPaths.append(m_userDBPath);
            return ProgramState::NoRoleData;
         } else {
            return ProgramState::NoFiles;
         }
      } else {
         return ProgramState::CantRun;
      }
   }

}


bool FirstStartSettingsService::FilePathIsCorrect(const QString &filePath)
{
   if (filePath == "" || m_terminal->IsFileNotExists(filePath, "AppFirstLoadlingSettingsService::FilePathIsCorrect", m_hasAdminPrivileges)) {
      return false;
   }

   return true;
}

bool FirstStartSettingsService::FodlerPathIsExists(const QString &folderPath)
{
   if (folderPath == "" || m_terminal->IsDirNotExists(folderPath, "AppFirstLoadlingSettingsService::FodlerPathIsExists", m_hasAdminPrivileges)) {
      return false;
   }

   return true;
}

void FirstStartSettingsService::wizardFinished(int result)
{
   Q_UNUSED(result);
   SetSettingsValueToDefault();
   Q_EMIT AllDataLoaded();
}

void FirstStartSettingsService::SetSettingsValueToDefault()
{
   m_setting->setValue(m_viewModeKey, m_viewModeDefaultValue);
   m_setting->setValue(m_themeKey, m_themeKeyDefaultValue);
   m_setting->setValue(m_pathToExecsKey, m_pathToExecsDefaultValue);
   m_setting->setValue(m_folderPathToRoleFolders, m_pathToRoleFoldersDefaultValue);
}
