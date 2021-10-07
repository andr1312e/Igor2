#include "appsettingsservice.h"



AppSettingsService::AppSettingsService(bool hasAdminPrivileges, QWidget *fakeWidget, Terminal *terminal)
    : m_hasAdminPrivileges(hasAdminPrivileges)
    , m_fakeWidget(fakeWidget)
    , m_terminal(terminal)
{

}

AppSettingsService::~AppSettingsService()
{
    delete m_setting;
}

bool AppSettingsService::tryToLoadSettings()
{
    if (QFile(m_pathToSettingsFile).exists())
    {
        m_setting=new QSettings(QDir::currentPath()+"/PROCESS_CONTROLLER.ini", QSettings::IniFormat);
        QString userDbPath=getPathUserDb();
        m_viewMode=getViewMode();
        m_theme=getThemeName();
        m_pathToExecs=getPathToExec();
        m_pathToRoleFolders=getPathToUserRoles();
        if (userDBPathCorrect(userDbPath))
        {
            m_pathUserDB=userDbPath;
            return true;
        }
        else
        {
            if (m_hasAdminPrivileges)
            {
                if (wantsUserToFix(userDbPath))
                {
                    m_pathUserDB=userDbPath;
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
            if(settingsFileCreated())
            {
                createDefaultParameters();
                QString userDbPath=getPathUserDb();
                if (userDBPathCorrect(userDbPath))
                {
                    m_pathUserDB=userDbPath;
                    m_setting->sync();
                    return true;
                }
                else
                {
                    if (wantsUserToFix(userDbPath))
                    {
                        m_pathUserDB=userDbPath;
                        m_setting->sync();
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool AppSettingsService::settingsFileCreated()
{
    QString desctiption="Файл настроек: "+ m_pathToSettingsFile + " не найден. Создать файл настроек прямо сейчас?";
    int userAnswer = QMessageBox::question(m_fakeWidget,"Не найден файл настроек!",desctiption,QMessageBox::Ok,QMessageBox::No);
    if(userAnswer == QMessageBox::Ok)
    {
        m_terminal->checkAndCreateFile(m_pathToSettingsFile, "AppSettingsService::settingsFileCreated");
        m_setting=new QSettings(QDir::currentPath()+"/PROCESS_CONTROLLER.ini", QSettings::IniFormat);
        return true;
    }
    else
    {
        QMessageBox::critical(m_fakeWidget,"Файла настроек нет","Без файла настроек работа программы невозможна",QMessageBox::Ok);
        return false;
    }
}

void AppSettingsService::createDefaultParameters()
{
    m_setting->setValue(m_viewModeKey, m_viewModeDefaultValue);
    m_setting->setValue(m_themeKey, m_themeKeyDefaultValue);
    m_setting->setValue(m_pathToExecsKey, m_pathToExecsKeyDefaultValue);
    m_setting->setValue(m_pathToRoleFolders, m_pathToRoleFoldersKeyDefaultValue);

}

QString AppSettingsService::getPathUserDb()
{
    return m_setting->value(m_pathUserDBKey, "").toString();
}

QString AppSettingsService::getViewMode()
{
    return m_setting->value(m_viewModeKey, m_viewModeDefaultValue).toString();
}

QString AppSettingsService::getThemeName()
{
    return m_setting->value(m_themeKey, m_themeKeyDefaultValue).toString();
}

QString AppSettingsService::getPathToExec()
{
    return m_setting->value(m_pathToExecsKey, m_pathToExecsKeyDefaultValue).toString();
}

QString AppSettingsService::getPathToUserRoles()
{
    return m_setting->value(m_pathToRoleFoldersKey, m_pathToRoleFoldersKeyDefaultValue).toString();
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
                    m_pathUserDB=QDir::currentPath()+"/"+fileName+".xml";
                    m_terminal->checkAndCreateFile(m_pathUserDB, "AppSettingsService::userDbPathCorrect");
                    m_setting->setValue(m_pathUserDBKey, m_pathUserDB);
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
