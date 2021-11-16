#include "startuprepositoryservice.h"
#include <QDebug>

StartupRepositoryService::StartupRepositoryService(Terminal *terminal)
   : m_terminal(terminal)
{}

void StartupRepositoryService::checkStartupFile(const QString &filePath)
{
   m_terminal->CheckAndCreatePathToElement(filePath, "StartupRepositoryService::checkStartupFile", true);
   if (m_terminal->IsFileNotExists(filePath, "StartupManagerService::checkStartupFile")) {
        m_terminal->CreateFile(filePath, "StartupManagerService::checkStartupFile", true);
   }
}

void StartupRepositoryService::clearFile(const QString &filePath)
{
   m_terminal->ClearFileSudo(filePath, "StartupManagerService::writeExecToStartupFile");
}

void StartupRepositoryService::writeExecToStartupFile(const QString &filePath, const QStringList &execPaths)
{
   QString paths = execPaths.join('\n');
   m_terminal->WriteTextToFileSudo(paths.toLatin1(), filePath, "StartupManagerService::writeExecToStartupFile");
}

void StartupRepositoryService::setDefaultApps(const QString &role, const QString &filePath)
{
   m_terminal->CopyFileSudo("/home/user/RLS_TI/" + role + ".startup", filePath, "StartupManagerService::setDefaultApps");
}

QStringList StartupRepositoryService::getAllEcexFromStartupFile(const QString &filePath)
{
   QString ecexs = m_terminal->GetFileText(filePath, "StartupManagerService::getAllEcexFromStartupFile");
   QStringList execsList = ecexs.split('\n');
   execsList.removeAll("");
   return execsList;

}
