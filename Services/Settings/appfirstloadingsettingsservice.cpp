#include "appfirstloadingsettingsservice.h"

AppFirstLoadlingSettingsService::AppFirstLoadlingSettingsService(QString &currentUserName, QString &currentUserId, bool hasAdminPrivileges, QWidget *fakeWidget, Terminal *terminal)
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

AppFirstLoadlingSettingsService::~AppFirstLoadlingSettingsService()
{
   delete m_setting;
}

ProgramFilesState AppFirstLoadlingSettingsService::isAllDataLoaded()//main function
{
   if (m_terminal->IsFileNotExists(m_pathToSettingsFile, "AppFirstLoadlingSettingsService::isAllDataLoaded", m_hasAdminPrivileges)) {
      if (m_hasAdminPrivileges) {
         СreateSettingsFile();
         OpenSettingsFile();
         return ProgramFilesState::NoFiles;
      } else {
         return ProgramFilesState::CantRun;
      }
   } else {
      OpenSettingsFile();
      GetValuesFromSettingsFile();
      return CheckIsPathsInSettingsFileCorrect();
   }
}

QString AppFirstLoadlingSettingsService::GetPathUserDbFromSettings()
{
   return m_setting->value(m_pathUserDBKey, "").toString();
}

QString AppFirstLoadlingSettingsService::GetViewModeFromSettings()
{
   return m_setting->value(m_viewModeKey, m_viewModeDefaultValue).toString();
}

bool AppFirstLoadlingSettingsService::GetThemeNameFromSettings()
{
   return m_setting->value(m_themeKey, m_themeKeyDefaultValue).toBool();
}

QString AppFirstLoadlingSettingsService::GetPathToExecFromSettings()
{
   return m_setting->value(m_pathToExecsKey, "").toString();
}

QString AppFirstLoadlingSettingsService::GetPathToUserRolesFromSettings()
{
   return m_setting->value(m_pathToRoleFoldersKey, "").toString();
}

void AppFirstLoadlingSettingsService::OpenSettingsFile()
{
   m_setting = new QSettings(m_pathToSettingsFile, QSettings::IniFormat);
}

void AppFirstLoadlingSettingsService::СreateSettingsFile()
{
   if (m_terminal->IsFileNotExists(m_pathToSettingsFile, "AppFirstLoadlingSettingsService::СreateSettingsFile", m_hasAdminPrivileges)) {
      m_terminal->CreateFile(m_pathToSettingsFile, "AppFirstLoadlingSettingsService::СreateSettingsFile", m_hasAdminPrivileges);
   }
}

void AppFirstLoadlingSettingsService::GetValuesFromSettingsFile()
{
   m_userDBPath = GetPathUserDbFromSettings();
   m_viewMode = GetViewModeFromSettings();
   m_theme = GetThemeNameFromSettings();
   m_folderPathToExecs = GetPathToExecFromSettings();
   m_folderPathToRoleFolders = GetPathToUserRolesFromSettings();
}

ProgramFilesState AppFirstLoadlingSettingsService::CheckIsPathsInSettingsFileCorrect()
{
   if (FodlerPathIsExists(m_folderPathToExecs) || FodlerPathIsExists(m_folderPathToRoleFolders)) {
      if (FilePathIsCorrect(m_userDBPath)) {
         return ProgramFilesState::Fine;
      } else {
         if (m_hasAdminPrivileges) {
            m_validSettingsPaths.append(m_folderPathToRoleFolders);
            m_validSettingsPaths.append(m_folderPathToExecs);
            return ProgramFilesState::NoUserDb;
         } else {
            return ProgramFilesState::CantRun;
         }
      }
   } else {
      if (m_hasAdminPrivileges) {
         if (FilePathIsCorrect(m_userDBPath)) {
            m_validSettingsPaths.append(m_userDBPath);
            return ProgramFilesState::NoRoleData;
         } else {
            return ProgramFilesState::NoFiles;
         }
      } else {
         return ProgramFilesState::CantRun;
      }
   }

}


bool AppFirstLoadlingSettingsService::FilePathIsCorrect(const QString &filePath)
{
   if (filePath == "" || m_terminal->IsFileNotExists(filePath, "AppFirstLoadlingSettingsService::FilePathIsCorrect", m_hasAdminPrivileges)) {
      return false;
   }

   return true;
}

bool AppFirstLoadlingSettingsService::FodlerPathIsExists(const QString &folderPath)
{
   if (folderPath == "" || m_terminal->IsDirNotExists(folderPath, "AppFirstLoadlingSettingsService::FodlerPathIsExists", m_hasAdminPrivileges)) {
      return false;
   }

   return true;
}

void AppFirstLoadlingSettingsService::wizardFinished(int result)
{
   Q_UNUSED(result);
   SetSettingsValueToDefault();
   Q_EMIT allDataLoaded();
}

void AppFirstLoadlingSettingsService::SetSettingsValueToDefault()
{
   m_setting->setValue(m_viewModeKey, m_viewModeDefaultValue);
   m_setting->setValue(m_themeKey, m_themeKeyDefaultValue);
   m_setting->setValue(m_pathToExecsKey, m_pathToExecsDefaultValue);
   m_setting->setValue(m_folderPathToRoleFolders, m_pathToRoleFoldersDefaultValue);
}
