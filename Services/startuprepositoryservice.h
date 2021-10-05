#ifndef SERVICES_STARTUPMANAGERSERVICE_H
#define SERVICES_STARTUPMANAGERSERVICE_H

#include <QStringList>

#include "Services/Terminals/terminal.h"

class StartupRepositoryService
{
public:

    StartupRepositoryService(Terminal *terminal);

    void checkStartupFile(const QString &folderPath,const QString &filePath);

    void clearFile(const QString&filePath);

    void writeExecToStartupFile(const QString &filePath,const QStringList &execPaths);

    void setDefaultApps(const QString &role, const QString &filePath);

    QStringList getAllEcexFromStartupFile(const QString &filePath);

    Terminal* getTerminal() {return m_terminal;}

private:

    Terminal *m_terminal;

};

#endif // SERVICES_STARTUPMANAGERSERVICE_H
