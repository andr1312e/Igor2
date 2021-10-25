#include "startuprepositoryservice.h"
#include <QDebug>

StartupRepositoryService::StartupRepositoryService(Terminal *terminal)
   : m_terminal(terminal)
{}

void StartupRepositoryService::checkStartupFile(const QString &folderPath, const QString &filePath)
{
   if (m_terminal->IsDirNotExists(folderPath, "StartupManagerService::checkStartupFile")) {

   }

   if (m_terminal->IsFileNotExists(filePath, "StartupManagerService::checkStartupFile")) {

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
   m_terminal->CopyFileSudo("/home/user/RLS_TI/" + role + ".start", filePath, "StartupManagerService::setDefaultApps");
}

QStringList StartupRepositoryService::getAllEcexFromStartupFile(const QString &filePath)
{
   QString ecexs = m_terminal->GetFileText(filePath, "StartupManagerService::getAllEcexFromStartupFile");
   QStringList execsList = ecexs.split('\n');
   execsList.removeAll("");
   return execsList;

}
