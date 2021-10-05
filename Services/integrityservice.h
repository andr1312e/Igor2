#ifndef SERVICES_INTEGRITYSERVICE_H
#define SERVICES_INTEGRITYSERVICE_H

#include <QString>

#include "Services/Terminals/terminal.h"

class IntegrityService
{
public:

    IntegrityService(Terminal *terminal);

    ~IntegrityService();

    QString getUserIngerityLevel(QString &userName);

    QString getFileIntegrityLevel(QString &filePath);

    Terminal* getTerminal() {return m_terminal;}

private:

    Terminal *m_terminal;

private:

    QString getUserIngerityLevelFromString(QString &astraUserInfo);

    QString getFileIntegrityLevelFromString(QString &astraFileInfo);

};

#endif // SERVICES_INTEGRITYSERVICE_H
