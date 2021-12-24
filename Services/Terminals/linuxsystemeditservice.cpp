#include "linuxsystemeditservice.h"

LinuxSystemEditService::LinuxSystemEditService(Terminal *terminal)
    : m_terminal(terminal)
    , m_startupDirectory("/etc/xdg/autostart/")
    , m_rlstiFolder("usr/RLS_TI/")
    , m_myIconName("userControl.desktop")
{

}

LinuxSystemEditService::~LinuxSystemEditService()
{

}

void LinuxSystemEditService::SetProgramToStartup()
{
    const QString currentPath=QDir::currentPath();
    const QString fileName=QFileInfo(currentPath).fileName();
    const QString startupIconText=StartupIconText(fileName);

    const QStringList listOfIcons=m_terminal->GetFileList(m_startupDirectory, "LinuxSystemEditService::SetProgramToStartup", true);

    for(const QString &icon:listOfIcons)
    {
        if (m_myIconName==icon)
        {
            const QString iconText=m_terminal->GetFileText(m_startupDirectory+m_myIconName, "LinuxSystemEditService::SetProgramToStartup", true);
            if(iconText!=startupIconText)
            {
                m_terminal->DeleteFileSudo(m_startupDirectory+m_myIconName, "LinuxSystemEditService::SetProgramToStartup");
                m_terminal->WriteTextToFileSudo(startupIconText.toLatin1(), m_startupDirectory+m_myIconName, "LinuxSystemEditService::SetProgramToStartup");
                return;
            }

            CopyFileToStartupDirectory(currentPath, fileName);
            return;
        }
    }
    CreateStartupAppIcon();
    m_terminal->WriteTextToFileSudo(startupIconText.toLatin1(), m_startupDirectory+m_myIconName, "LinuxSystemEditService::SetProgramToStartup");
    CopyFileToStartupDirectory(currentPath, fileName);
}

void LinuxSystemEditService::SetDateTime()
{
    QString string = QDateTime::currentDateTime().toString("\"yyyy-MM-dd hh:mm\"");
    QString dateTimeString ("date -s ");
    dateTimeString.append(string);
    int systemDateTimeStatus= system(dateTimeString.toStdString().c_str());
    if (systemDateTimeStatus == -1)
    {
        qDebug() << "Failed to change date time";
    }
    int systemHwClockStatus = system("/sbin/hwclock -w");
    if (systemHwClockStatus == -1 )
    {
        qDebug() << "Failed to sync hardware clock";
    }
}

void LinuxSystemEditService::CreateStartupAppIcon()
{
    m_terminal->CreateFile(m_startupDirectory+m_myIconName, "LinuxSystemEditService::SetProgramToStartup", true);
}

void LinuxSystemEditService::CopyFileToStartupDirectory(const QString &currentPath, const QString &fileName)
{
    if(m_terminal->IsFileNotExists(m_rlstiFolder+fileName, "LinuxSystemEditService::SetProgramToStartup", true))
    {
        m_terminal->CopyFileSudo(currentPath+fileName, m_rlstiFolder, "LinuxSystemEditService::SetProgramToStartup");
    }
}

const QString LinuxSystemEditService::StartupIconText(const QString &myExecName)
{
    return  "[Desktop Entry]\n"
            " Name=myApp\n "
            "Exec="+ m_startupDirectory+myExecName;
}
