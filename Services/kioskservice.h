#ifndef SERVICES_KIOSKSERVICE_H
#define SERVICES_KIOSKSERVICE_H

#include "Services/terminal.h"

class KioskService
{
public:

    KioskService(Terminal *terminal);

    ~KioskService();

    void lockUser(QString &userName);

    void unLockUser(QString &userName);

    bool isUserInKiosk(QString &userName);

    Terminal* getTerminal() {return m_terminal;}

private:

    Terminal *m_terminal;

private:

    void setPrivacyLevelZero(QString &userName);// нужен режим приватности для киоска 0

};

#endif // SERVICES_KIOSKSERVICE_H
