#include "integrityservice.h"

IntegrityService::IntegrityService(Terminal *terminal)
    : m_terminal(terminal)
{
}

IntegrityService::~IntegrityService()
{

}

QString IntegrityService::getUserIngerityLevel(QString &userName)
{
    const QString getUserInterityLevelCommand="sudo pdpl-user "+ userName;
    QString astraUserInfo=m_terminal->RunConsoleCommand(getUserInterityLevelCommand, "IntegrityService::getUserIngerityLevel");
    return getUserIngerityLevelFromString(astraUserInfo);
}

QString IntegrityService::getFileIntegrityLevel(QString &filePath)
{
    const QString getFileIntegrityLevelCommand="sudo pdp-ls -Md "+filePath;
    QString astraFileInfo=m_terminal->RunConsoleCommand(getFileIntegrityLevelCommand, "IntegrityService::getFileIntegrityLevel");
    return getFileIntegrityLevelFromString(astraFileInfo);
}

QString IntegrityService::getUserIngerityLevelFromString(QString &astraUserInfo)
{
    int indexOf2DotsFirst=astraUserInfo.lastIndexOf(":");
    if (indexOf2DotsFirst!=-1)
    {
        indexOf2DotsFirst=indexOf2DotsFirst-1;
        int indexOf2DotsSecond=astraUserInfo.lastIndexOf(":", indexOf2DotsFirst);
        int indexOf2DotsThird=astraUserInfo.lastIndexOf(":", indexOf2DotsSecond-1);
        QString integrityLevel=astraUserInfo.mid(indexOf2DotsThird+1, indexOf2DotsSecond-1-indexOf2DotsThird);
        return integrityLevel;
    }
    else
    {
        qFatal("IntegrityService::getUserIngerityLevelFromString не может получить уровень целостности пользователя из комманды sudo pdpl-user userName");
    }
}

QString IntegrityService::getFileIntegrityLevelFromString(QString &astraFileInfo)
{
    int indexOf2DotsFirst=astraFileInfo.lastIndexOf(":");
    if (indexOf2DotsFirst!=-1)
    {
        indexOf2DotsFirst=indexOf2DotsFirst-1;
        int indexOf2DotsSecond=astraFileInfo.lastIndexOf(":", indexOf2DotsFirst);
        int indexOf2DotsThird=astraFileInfo.lastIndexOf(":", indexOf2DotsSecond-1);
        QString integrityLevel=astraFileInfo.mid(indexOf2DotsThird+1, indexOf2DotsSecond-1-indexOf2DotsThird);
        if (integrityLevel=="Высокий")
        {
            integrityLevel="63";
        }
        else
        {
            if (integrityLevel=="Графический_сервер")
            {
                integrityLevel="8";
            }
            else
            {
                if (integrityLevel=="СПО")
                {
                    integrityLevel="4";
                }
                else
                {
                    if (integrityLevel=="Виртуализация")
                    {
                        integrityLevel="2";
                    }
                    else
                    {
                        if (integrityLevel=="Сетевые_сервисы")
                        {
                            integrityLevel="1";
                        }
                        else
                        {
                            if (integrityLevel=="Низкий")
                            {
                                integrityLevel="0";
                            }
                        }
                    }
                }
            }
        }
        return integrityLevel;
    }
    else
    {
        qFatal("IntegrityService::getUserIngerityLevelFromString не может получить уровень целостности пользователя из комманды sudo pdpl-user userName");
    }
}
