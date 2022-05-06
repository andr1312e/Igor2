#include "appsettingsservice.h"



AppSettingsService::AppSettingsService(QString &currentUserName, QString &currentUserId, bool hasAdminPrivileges, QWidget *fakeWidget, Terminal *terminal)
    : m_currentUserId(currentUserId)
    , m_currentUserName(currentUserName)
    , m_hasAdminPrivileges(hasAdminPrivileges)
    , m_fakeWidget(fakeWidget)
    , m_terminal(terminal)
    , m_settingsFileName("/sync_settings.ini")
    , m_pathToSettingsFile(QDir::currentPath()+m_settingsFileName)
{
    setCurrentUserNameToDefaultPaths();
}

AppSettingsService::~AppSettingsService()
{
    delete m_setting;
}

bool AppSettingsService::isAllDataLoaded()//main function
{

    if (QFile(m_pathToSettingsFile).exists())
    {
        m_setting=new QSettings(m_pathToSettingsFile, QSettings::IniFormat);
        QString userDbPath=getPathUserDbFromSettings();
        m_viewMode=getViewModeFromSettings();
        m_theme=getThemeNameFromSettings();
        m_folderPathToExecs=getPathToExecFromSettings();
        m_folderPathToRoleFolders=getPathToUserRolesFromSettings();
        if (userDBPathCorrect(userDbPath))
        {
            m_userDBPath=userDbPath;
            return true;
        }
        else
        {
            if (m_hasAdminPrivileges)
            {
                if (wantsUserToFix(userDbPath))
                {
                    m_userDBPath=userDbPath;
                    m_setting->sync();
                    return true;
                }
            }
            else
            {
                QMessageBox::critical(m_fakeWidget,"Строка содержит недопустимые символы", "Файл настроек "+ m_pathToSettingsFile + " имеет неверную структуру, обратитесь к Администратору для решения проблемы",QMessageBox::Ok);
            }
        }
    }
    else
    {
        if (m_hasAdminPrivileges)
        {
            createSettingsFile();
            initWizardUIAndShow(true, true);
        }
        else
        {
            QMessageBox::critical(m_fakeWidget,"Файла настроек и права администратора для его создание отстутвуют", "Файл настроек "+ m_pathToSettingsFile + " обратитесь к Администратору для решения проблемы",QMessageBox::Ok);
        }
    }
    return false;
}

QString AppSettingsService::getUserDBPathValue()
{
    return m_userDBPath;
}

QString AppSettingsService::getViewModeValue()
{
    return m_viewMode;
}

bool AppSettingsService::getThemeValue()
{
    return m_theme;
}

QString AppSettingsService::getFolderPathToExecsValue()
{
    return m_folderPathToExecs;
}

QString AppSettingsService::getFolderPathToRoleFoldersValue()
{
    return m_folderPathToRoleFolders;
}

void AppSettingsService::setCurrentUserNameToDefaultPaths()
{
    m_pathToExecsKeyDefaultValue="/home/"+m_currentUserName+"/RLS_TI/";
    m_pathToRoleFoldersKeyDefaultValue="/home/"+m_currentUserName+"/RLS_TI/";
}

QString AppSettingsService::getPathUserDbFromSettings()
{
    return m_setting->value(m_pathUserDBKey, "").toString();
}

QString AppSettingsService::getViewModeFromSettings()
{
    return m_setting->value(m_viewModeKey, m_viewModeDefaultValue).toString();
}

bool AppSettingsService::getThemeNameFromSettings()
{
    return m_setting->value(m_themeKey, m_themeKeyDefaultValue).toBool();
}

QString AppSettingsService::getPathToExecFromSettings()
{
    return m_setting->value(m_pathToExecsKey, m_pathToExecsKeyDefaultValue).toString();
}

QString AppSettingsService::getPathToUserRolesFromSettings()
{
    return m_setting->value(m_pathToRoleFoldersKey, m_pathToRoleFoldersKeyDefaultValue).toString();
}


void AppSettingsService::createSettingsFile()
{
    m_terminal->checkAndCreateFile(m_pathToSettingsFile, "AppSettingsService::settingsFileCreated");
    m_setting=new QSettings(m_pathToSettingsFile, QSettings::IniFormat);
}

void AppSettingsService::initWizardUIAndShow(bool noAccountsDb, bool noRolesData)
{
    if (noAccountsDb&&noRolesData)
    {
        m_startupWizard=new StartupWizard(0, m_currentUserName, m_currentUserId, QStringList(), nullptr);
    }
    else
    {
        if (noAccountsDb)
        {

        }
        else
        {

        }
    }
    connect(m_startupWizard, &StartupWizard::finished, this, &AppSettingsService::wizardFinished);
    m_startupWizard->show();
}


bool AppSettingsService::userDBPathCorrect(const QString &userDbPath)
{
    if (userDbPath=="" || !QFile::exists(userDbPath))
    {
        return false;
    }
    return true;
}

bool AppSettingsService::wantsUserToFix(QString &userDbPath)
{
    bool fileNotCreated=true;
    while (fileNotCreated)
    {
        bool userPushOkButton;
        if (userDbPath!="")
        {
            userDbPath=" по адресу: " +userDbPath;
        }
        QString fileName= QInputDialog::getText(m_fakeWidget, QString("Не найден файл базы данных !"), QString("Файл настроек "+ m_pathToSettingsFile + " не содержит информацию о базе данных пользователей " + userDbPath + ". Если хотите создать базу, ведите имя файла:") ,QLineEdit::Normal,"",&userPushOkButton);
        if(fileName.endsWith(".xml"))
        {
            fileName.chop(4);
        }
        if(userPushOkButton)
        {
            if (fileName.isEmpty())
            {
                QMessageBox::critical(m_fakeWidget,"Строка пустая","Введите только имя файла без формата",QMessageBox::Ok);
            }
            else
            {
                if (GlobalFunctions::stringContainsBadCharecters(fileName))
                {
                    QMessageBox::critical(m_fakeWidget,"Строка содержит недопустимые символы", "Текст в поле \"имя файла\" не может содержать символы \ / ` : & * ? \" \' < > | () : ;",QMessageBox::Ok);
                }
                else
                {
                    m_userDBPath=QDir::currentPath()+"/"+fileName+".xml";
                    m_terminal->checkAndCreateFile(m_userDBPath, "AppSettingsService::userDbPathCorrect");
                    m_setting->setValue(m_pathUserDBKey, m_userDBPath);
                    fileNotCreated=false;
                }
            }
        }
        else
        {
            int userAnswer = QMessageBox::question(m_fakeWidget,"База данных необходима!","Без базы пользователей работа программы невозможна. Будем создавать файл?",QMessageBox::Ok,QMessageBox::No);
            if(userAnswer != QMessageBox::Ok)
            {
                return false;
            }
        }
    }
    return true;
}

void AppSettingsService::wizardFinished(int result)
{
    Q_UNUSED(result);
    createDefaultParameters();
    Q_EMIT allDataLoaded();
}

void AppSettingsService::createDefaultParameters()
{
    m_setting->setValue(m_viewModeKey, m_viewModeDefaultValue);
    m_setting->setValue(m_themeKey, m_themeKeyDefaultValue);
    m_setting->setValue(m_pathToExecsKey, m_pathToExecsKeyDefaultValue);
    m_setting->setValue(m_folderPathToRoleFolders, m_pathToRoleFoldersKeyDefaultValue);
}