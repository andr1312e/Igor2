#include "appfirstloadingsettingsservice.h"

AppFirstLoadlingSettingsService::AppFirstLoadlingSettingsService(QString &currentUserName, QString &currentUserId, bool hasAdminPrivileges, QWidget *fakeWidget, Terminal *terminal)
    : m_currentUserId(currentUserId)
    , m_currentUserName(currentUserName)
    , m_hasAdminPrivileges(hasAdminPrivileges)
    , m_fakeWidget(fakeWidget)
    , m_terminal(terminal)
    , m_settingsFileName("/sync_settings.ini")
    , m_pathToSettingsFile(QDir::currentPath()+m_settingsFileName)
{
    SetCurrentUserNameToDefaultPaths();
}

AppFirstLoadlingSettingsService::~AppFirstLoadlingSettingsService()
{
    delete m_setting;
}

ProgramFilesState AppFirstLoadlingSettingsService::isAllDataLoaded()//main function
{
    if (QFile(m_pathToSettingsFile).exists())
    {
        OpenSettingsFile();
        GetValuesFromSettingsFile();
        return CheckIsPathsInSettingsFileCorrect();
    }
    else
    {
        if (m_hasAdminPrivileges)
        {
            СreateSettingsFile();
            return ProgramFilesState::NoFiles;
        }
        else
        {
            return ProgramFilesState::CantRun;
        }
    }
}

void AppFirstLoadlingSettingsService::SetCurrentUserNameToDefaultPaths()
{
    m_userDBPathDefaultValue="/home/"+m_currentUserName+"/sync_db.kuntsevodb";
    m_pathToExecsDefaultValue="/home/"+m_currentUserName+"/RLS_TI/";
    m_pathToRoleFoldersDefaultValue="/home/"+m_currentUserName+"/RLS_TI/";
    m_defaultSettingsPaths.append(m_userDBPath);
    m_defaultSettingsPaths.append(m_pathToExecsDefaultValue);
    m_defaultSettingsPaths.append(m_pathToRoleFoldersDefaultValue);
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
    return m_setting->value(m_pathToExecsKey, m_pathToExecsDefaultValue).toString();
}

QString AppFirstLoadlingSettingsService::GetPathToUserRolesFromSettings()
{
    return m_setting->value(m_pathToRoleFoldersKey, m_pathToRoleFoldersDefaultValue).toString();
}

void AppFirstLoadlingSettingsService::OpenSettingsFile()
{
    m_setting=new QSettings(m_pathToSettingsFile, QSettings::IniFormat);
}

void AppFirstLoadlingSettingsService::СreateSettingsFile()
{
    m_terminal->checkAndCreateFile(m_pathToSettingsFile, "AppSettingsService::settingsFileCreated");
    m_setting=new QSettings(m_pathToSettingsFile, QSettings::IniFormat);
}

void AppFirstLoadlingSettingsService::GetValuesFromSettingsFile()
{
    m_userDBPath=GetPathUserDbFromSettings();
    m_viewMode=GetViewModeFromSettings();
    m_theme=GetThemeNameFromSettings();
    m_folderPathToExecs=GetPathToExecFromSettings();
    m_folderPathToRoleFolders=GetPathToUserRolesFromSettings();
}

ProgramFilesState AppFirstLoadlingSettingsService::CheckIsPathsInSettingsFileCorrect()
{
    if (FodlerPathIsExists(m_folderPathToExecs)||FodlerPathIsExists(m_folderPathToRoleFolders))
    {
        if (FilePathIsCorrect(m_userDBPath))
        {
            return ProgramFilesState::Fine;
        }
        else
        {
            if (m_hasAdminPrivileges)
            {
                m_validSettingsPaths.append(m_folderPathToRoleFolders);
                m_validSettingsPaths.append(m_folderPathToExecs);
                return ProgramFilesState::NoUserDb;
            }
            else
            {
                return ProgramFilesState::CantRun;
            }
        }
    }
    else
    {
        if (m_hasAdminPrivileges)
        {
            if (FilePathIsCorrect(m_userDBPath))
            {

                m_validSettingsPaths.append(m_userDBPath);
                return ProgramFilesState::NoRoleData;
            }
            else
            {
                return ProgramFilesState::NoFiles;
            }
        }
        else
        {
            return ProgramFilesState::CantRun;
        }
    }

}

void AppFirstLoadlingSettingsService::InitWizardServiceAndStartIt(ProgramFilesState state)
{
    //    QStringList defaultValues;
    //    defaultValues<< m_userDBPathDefaultValue << m_pathToExecsKeyDefaultValue << m_pathToRoleFoldersKeyDefaultValue;
    //    m_wizardService=new WizardService(state, m_currentUserName, m_currentUserId, m_validSettingsPaths, defaultValues, m_terminal, nullptr);

    //    //    m_startupWizard=new StartupWizard(state, , nullptr, nullptr);
    //    //    connect(m_startupWizard, &StartupWizard::finished, this, &AppSettingsService::wizardFinished);
    //    //    m_startupWizard->show();
}


bool AppFirstLoadlingSettingsService::FilePathIsCorrect(const QString &filePath)
{
    if (filePath=="" || !QFile::exists(filePath))
    {
        return false;
    }
    return true;
}

bool AppFirstLoadlingSettingsService::FodlerPathIsExists(const QString &folderPath)
{
    QDir Dir;
    if (folderPath=="" || !Dir.exists(folderPath))
    {
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
