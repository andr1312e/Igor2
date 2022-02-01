#include "kioskservice.h"

KioskService::KioskService()
    : m_terminal(Terminal::GetTerminal())
{
}

KioskService::~KioskService()
{

}

void KioskService::LockUser(const QString &userName)
{
    SetPrivacyLevelZero(userName);
    const QString activateKioskMode =QStringLiteral("sudo fly-kiosk -a lock -u ")+userName;
    m_terminal->RunConsoleCommand(activateKioskMode, Q_FUNC_INFO);
}

void KioskService::UnLockUser(const QString &userName)
{
    const QString deactivateKioskMode=QStringLiteral("sudo fly-kiosk -a unlock -u ")+userName;
    m_terminal->RunConsoleCommand(deactivateKioskMode, Q_FUNC_INFO);
}

bool KioskService::IsUserInKiosk(const QString &userName)
{
    const QString checkKioskMode=QStringLiteral("sudo fly-kiosk -s -u ")+userName;
    const QString userState=m_terminal->RunConsoleCommand(checkKioskMode, Q_FUNC_INFO);
    if (userState.contains(QStringLiteral("not locked")))
        return false;
    else
        return true;
}

void KioskService::SetPrivacyLevelZero(const QString &userName)
{
    const QString setPrivacyLevelToZero=QStringLiteral("sudo pdpl-user -l 0:0 ")+userName;
    m_terminal->RunConsoleCommand(setPrivacyLevelToZero, Q_FUNC_INFO);
}
