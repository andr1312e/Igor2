#include "startuprepositoryservice.h"

StartupRepositoryPresenter::StartupRepositoryPresenter(Terminal *terminal, ISqlDatabaseService *sqlDatabaseService)
    : m_terminal(terminal)
    , m_sqlDatabaseService(sqlDatabaseService)
{

}

StartupRepositoryPresenter::~StartupRepositoryPresenter()
{

}

void StartupRepositoryPresenter::CheckStartupTable(const quint8 &roleId)
{
    m_sqlDatabaseService->CreateExecsTableInNotExists(roleId);
}

QStringList StartupRepositoryPresenter::GetAllStartups(const quint8 &roleId)
{
    QStringList startupsList = m_sqlDatabaseService->GetAllRoleExecs(roleId);
    return startupsList;
}

void StartupRepositoryPresenter::DeleteStartup(const quint8 &roleId, const QString &startupPath)
{
    m_sqlDatabaseService->RemoveExecIntoRole(roleId, startupPath);
    TryDeleteFile(startupPath);
}

void StartupRepositoryPresenter::AppendStartup(const quint8 &roleId, const QString &startupPath)
{
    m_sqlDatabaseService->AppendExecIntoRole(roleId, startupPath);
    TryToCopyFile(startupPath);
}

void StartupRepositoryPresenter::TryDeleteFile(const QString &startupPath)
{
    if(m_terminal->IsFileExists(startupPath, "StartupRepositoryPresenter::DeleteStartup", true))
    {
        m_terminal->DeleteFileSudo(startupPath, "StartupRepositoryPresenter::DeleteStartup");
    }
}

void StartupRepositoryPresenter::TryToCopyFile(const QString &startupPath)
{
    int index=startupPath.lastIndexOf('/');
    QString startupFileName=startupPath.mid(index);
    if(m_terminal->IsDirNotExists(m_destinationFolder, "StartupRepositoryPresenter::AppendStartup", true))
    {
        m_terminal->CreateFolder(m_destinationFolder, "StartupRepositoryPresenter::AppendStartup", true);
    }
    if(m_terminal->IsFileExists(m_destinationFolder+startupFileName, "StartupRepositoryPresenter::AppendStartup", true))
    {
        m_terminal->DeleteFileSudo(m_destinationFolder+startupFileName, "StartupRepositoryPresenter::AppendStartup");
    }
    m_terminal->CopyFileSudo(startupPath, m_destinationFolder, "StartupRepositoryPresenter::AppendStartup");
}


