#include "kioskservice.h"

KioskService::KioskService(Terminal *terminal)
    : m_terminal(terminal)
{
}

KioskService::~KioskService()
{

}

void KioskService::LockUser(const QString &userName)
{
    SetPrivacyLevelZero(userName);
    const QString activateKioskMode ="sudo fly-kiosk -a lock -u "+userName;
    m_terminal->RunConsoleCommand(activateKioskMode, "KioskLockUnLockService::LockUser");
}

void KioskService::UnLockUser(const QString &userName)
{
    const QString deactivateKioskMode="sudo fly-kiosk -a unlock -u "+userName;
    m_terminal->RunConsoleCommand(deactivateKioskMode, "KioskLockUnLockService::UnLockUser");
}

bool KioskService::IsUserInKiosk(const QString &userName)
{
    const QString checkKioskMode="sudo fly-kiosk -s -u "+userName;
    const QString userState=m_terminal->RunConsoleCommand(checkKioskMode, "KioskLockUnLockService::isUserInKiosk");
    if (userState.contains("not locked"))
        return false;
    else
        return true;
}

void KioskService::SetPrivacyLevelZero(const QString &userName)
{
    const QString setPrivacyLevelToZero="sudo pdpl-user -l 0:0 "+userName;
    m_terminal->RunConsoleCommand(setPrivacyLevelToZero, "KioskLockUnLockService::setPrivacyLevelZero");
}
