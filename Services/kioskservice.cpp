#include "kioskservice.h"
#include <QDebug>

KioskService::KioskService(Terminal *terminal)
    : m_terminal(terminal)
{
}

KioskService::~KioskService()
{

}


void KioskService::lockUser(QString &userName)
{
    setPrivacyLevelZero(userName);
    const QString activateKioskMode ="sudo fly-kiosk -a lock -u "+userName;
    m_terminal->runConsoleCommand(activateKioskMode, "KioskLockUnLockService::LockUser");
}

void KioskService::unLockUser(QString &userName)
{
    const QString deactivateKioskMode="sudo fly-kiosk -a unlock -u "+userName;
    m_terminal->runConsoleCommand(deactivateKioskMode, "KioskLockUnLockService::UnLockUser");
}

bool KioskService::isUserInKiosk(QString &userName)
{
    const QString checkKioskMode="sudo fly-kiosk -s -u "+userName;
    QString userState=m_terminal->runConsoleCommand(checkKioskMode, "KioskLockUnLockService::isUserInKiosk");
    if (userState.contains("not locked"))
        return false;
    else
        return true;
}

void KioskService::setPrivacyLevelZero(QString &userName)
{
    const QString setPrivacyLevelToZero="sudo pdpl-user -l 0:0 "+userName;
    m_terminal->runConsoleCommand(setPrivacyLevelToZero, "KioskLockUnLockService::setPrivacyLevelZero");
}
