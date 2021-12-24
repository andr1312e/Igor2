#ifndef LINUXSYSTEMEDITSERVICE_H
#define LINUXSYSTEMEDITSERVICE_H

#include <QDir>
#include <QDateTime>

#include "Services/Terminals/terminal.h"

class LinuxSystemEditService
{
public:
    LinuxSystemEditService(Terminal *terminal);
    ~LinuxSystemEditService();

public:
    void SetProgramToStartup();
    void SetDateTime();

private:
    void CreateStartupAppIcon();
    const QString StartupIconText(const QString &myExecName);
    void CopyFileToStartupDirectory(const QString &currentPath, const QString &fileName);

private:
    Terminal *m_terminal;
    const QString m_startupDirectory;
    const QString m_rlstiFolder;
    const QString m_myIconName;
};

#endif // LINUXSYSTEMEDITSERVICE_H
