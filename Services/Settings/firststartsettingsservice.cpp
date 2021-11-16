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
}

LoadingState FirstStartSettingsService::IsAllDataLoaded()//main function
{
    if (m_terminal->IsFileNotExists(m_pathToSettingsFile, "AppFirstLoadlingSettingsService::isAllDataLoaded", m_hasAdminPrivileges)) {
        if (m_hasAdminPrivileges) {
            CreateSettingsFile();
            return LoadingState::NoFiles;
        } else {
            return LoadingState::CantRun;
        }
    } else {
        QSettings settings(m_pathToSettingsFile, QSettings::IniFormat);
        GetValuesFromSettingsFile(settings);
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

QString FirstStartSettingsService::GetPathUserDbFromSettings(QSettings &settings)
{
    return settings.value(m_pathUserDBKey, "").toString();
}

QString FirstStartSettingsService::GetViewModeFromSettings(QSettings &settings)
{
    return settings.value(m_viewModeKey, m_viewModeDefaultValue).toString();
}

bool FirstStartSettingsService::GetThemeNameFromSettings(QSettings &settings)
{
    return settings.value(m_themeKey, m_themeKeyDefaultValue).toBool();
}

QString FirstStartSettingsService::GetPathToExecFromSettings(QSettings &settings)
{
    return settings.value(m_pathToExecsKey, "").toString();
}

QString FirstStartSettingsService::GetPathToUserRolesFromSettings(QSettings &settings)
{
    return settings.value(m_pathToRoleFoldersKey, "").toString();
}

void FirstStartSettingsService::GetValuesFromSettingsFile(QSettings &settings)
{
    m_userDBPath = GetPathUserDbFromSettings(settings);
    m_viewMode = GetViewModeFromSettings(settings);
    m_theme = GetThemeNameFromSettings(settings);
    m_folderPathToExecs = GetPathToExecFromSettings(settings);
    m_folderPathToRoleFolders = GetPathToUserRolesFromSettings(settings);
}

LoadingState FirstStartSettingsService::CheckIsPathsInSettingsFileCorrect()
{
    if (FolderPathIsExists(m_folderPathToExecs) && FolderPathIsExists(m_folderPathToRoleFolders)){
        if (FilePathIsCorrect(m_userDBPath)) {
            return LoadingState::Fine;
        } else {
            if (m_hasAdminPrivileges) {
                m_validSettingsPaths.push_back(m_folderPathToRoleFolders);
                m_validSettingsPaths.push_back(m_folderPathToExecs);
                return LoadingState::NoUserDb;
            } else {
                return LoadingState::CantRun;
            }
        }
    } else {
        if (m_hasAdminPrivileges) {
            if (FilePathIsCorrect(m_userDBPath)) {
                m_validSettingsPaths.push_back(m_userDBPath);
                return LoadingState::NoRoleData;
            } else {
                return LoadingState::NoFiles;
            }
        } else {
            return LoadingState::CantRun;
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

bool FirstStartSettingsService::FolderPathIsExists(const QString &folderPath)
{
    if (folderPath == "" || m_terminal->IsDirNotExists(folderPath, "FirstStartSettingsService::FolderPathIsNotExists", m_hasAdminPrivileges)) {
        return false;
    }

    return true;
}

void FirstStartSettingsService::OnWizardFinished(int result)
{
    Q_UNUSED(result);
    Q_EMIT ToAllDataLoaded();
}

void FirstStartSettingsService::OnSetDbAndIconsPaths(const QStringList &paths)
{
    Q_ASSERT(paths.count()==3);
    m_userDBPath=paths.front();
    m_folderPathToRoleFolders=paths.at(1);
    m_folderPathToExecs=paths.last();
    QSettings settings(m_pathToSettingsFile, QSettings::IniFormat);
    SetSettingsValues(settings);

}

void FirstStartSettingsService::SetSettingsValues(QSettings &settings)
{
    QString settingsString;
    settingsString.append(m_viewModeKey).append("=").append(m_viewModeDefaultValue).append('\n');
    settingsString.append(m_themeKey).append("=").append(m_themeKeyDefaultValue).append('\n');
    settingsString.append(m_pathUserDBKey).append("=").append(m_userDBPath).append('\n');
    settingsString.append(m_pathToExecsKey).append("=").append(m_folderPathToExecs).append('\n');
    settingsString.append(m_pathToRoleFoldersKey).append("=").append(m_folderPathToRoleFolders).append('\n');
    m_terminal->ClearFileSudo(m_pathToSettingsFile, "FirstStartSettingsService::SetSettingsValues");
    m_terminal->WriteTextToFileSudo(settingsString.toLatin1(), m_pathToSettingsFile, "FirstStartSettingsService::SetSettingsValues");
}

void FirstStartSettingsService::CreateSettingsFile()
{
    if (m_terminal->IsFileNotExists(m_pathToSettingsFile, "FirstStartSettingsService::CreateSettingsFile", m_hasAdminPrivileges)) {
       m_terminal->CreateFile(m_pathToSettingsFile, "FirstStartSettingsService::CreateSettingsFile", m_hasAdminPrivileges);
    }
}
