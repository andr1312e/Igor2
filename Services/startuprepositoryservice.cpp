#include "startuprepositoryservice.h"
#include <QDebug>

StartupRepositoryService::StartupRepositoryService(Terminal *terminal)
    : m_terminal(terminal)
{}

void StartupRepositoryService::checkStartupFile(const QString &folderPath, const QString &filePath)
{
    m_terminal->checkAndCreateFolder(folderPath, "StartupManagerService::checkStartupFile");
    m_terminal->checkAndCreateFile(filePath, "StartupManagerService::checkStartupFile");
}

void StartupRepositoryService::clearFile(const QString &filePath)
{
    m_terminal->clearFileSudo(filePath, "StartupManagerService::writeExecToStartupFile");
}

void StartupRepositoryService::writeExecToStartupFile(const QString &filePath, const QStringList &execPaths)
{
    QString paths=execPaths.join('\n');
    m_terminal->writeTextToFileSudo(paths, filePath, "StartupManagerService::writeExecToStartupFile");
}

void StartupRepositoryService::setDefaultApps(const QString &role, const QString &filePath)
{
    m_terminal->copyFileSudo("/home/user/RLS_TI/"+role+".start", filePath, "StartupManagerService::setDefaultApps");
}

QStringList StartupRepositoryService::getAllEcexFromStartupFile(const QString &filePath)
{
    QString ecexs=m_terminal->getFileText(filePath, "StartupManagerService::getAllEcexFromStartupFile");
    QStringList execsList=ecexs.split('\n');
    execsList.removeAll("");
    return execsList;

}
