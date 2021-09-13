//#include "toolbardisableservice.h"
//#include <QDebug>

//ToolBarDisableService::ToolBarDisableService(Terminal *terminal, LinuxUserService *userService)
//    : m_terminal(terminal)
//    , m_userService(userService)
//{

//}

//void ToolBarDisableService::disableToolBar()
//{
//    bool restartNeeded=false;
//    QList<User> *users=m_userService->getUsersList();
//    for (QList<User>::const_iterator user=users->constBegin(); user!=users->constEnd(); ++user)
//    {
//        QString userName=(*user).name;
//        if (checkUserToolBar(userName))
//        {
//            restartNeeded=true;
//            disableUserToolbarOnTop(userName);
//        }
//    }
//    if (restartNeeded)
//        resetPC();
//}

//bool ToolBarDisableService::checkUserToolBar(const QString &userName)
//{
//    QString currentThemePath="/home/" + userName +"/.fly/theme/current.themerc";
//    if(QFile::exists(currentThemePath))
//    {
//        QString currentThemeText=m_terminal->getFileText(currentThemePath, "ToolBarDisableService::checkUserToolBar");
//        if (currentThemeText.contains("OnTopTaskbar=true"))
//        {
//            return true;
//        }
//        else
//        {
//            return false;
//        }
//    }
//    return true;
//}

//void ToolBarDisableService::disableUserToolbarOnTop(const QString &userName)
//{
//    QString currentThemePath="/home/" + userName +"/.fly/theme/current.themerc";
//    m_terminal->deleteFile(currentThemePath, "ToolBarDisableService::disableUserToolbarOnTop");
//    QString defaultThemePath="/home/" + userName +"/.fly/theme/default.themerc";
//    QString defaultThemeText=m_terminal->getFileText(defaultThemePath, "ToolBarDisableService::disableUserToolbarOnTop");
//    defaultThemeText.replace("OnTopTaskbar=true", "OnTopTaskbar=false");
//    defaultThemeText.replace("StandbyCmd", " StandbyCmd");
//    qDebug()<< defaultThemeText;
//    m_terminal->writeTextToFile(defaultThemeText, defaultThemePath, "ToolBarDisableService::disableUserToolbarOnTop");
//}

//void ToolBarDisableService::resetPC()
//{
//    QString resetPCCommand="sudo service fly-dm restart";
//    m_terminal->runConsoleCommand(resetPCCommand, "ToolBarDisableService::resetPC");
//}
