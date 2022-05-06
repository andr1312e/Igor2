#ifndef SERVICES_KIOSKSERVICE_H
#define SERVICES_KIOSKSERVICE_H

#include "Services/Terminals/terminal.h"

class KioskService
{
public:
    explicit KioskService();
    ~KioskService();

public:
    void LockUser(const QString &userName);
    void UnLockUser(const QString &userName);
    bool IsUserInKiosk(const QString &userName);
private:
    void SetPrivacyLevelZero(const QString &userName);// нужен режим приватности для киоска 0

private:
    Terminal * const m_terminal;
};

#endif // SERVICES_KIOSKSERVICE_H
