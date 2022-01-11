#include "startuprepositoryservice.h"

StartupRepositoryPresenter::StartupRepositoryPresenter(ISqlDatabaseService *sqlDatabaseService)
    : m_terminal(Terminal::GetTerminal())
    , m_sqlDatabaseService(sqlDatabaseService)
{

}

StartupRepositoryPresenter::~StartupRepositoryPresenter()
{

}

void StartupRepositoryPresenter::CheckStartupTable(const quint8 &roleId)
{
    m_sqlDatabaseService->CreateStartupsTableIfNotExists(roleId);
}

QStringList StartupRepositoryPresenter::GetAllStartups(const quint8 &roleId)
{
    QStringList startupsList = m_sqlDatabaseService->GetAllRoleStartups(roleId);
    return startupsList;
}

void StartupRepositoryPresenter::DeleteStartup(const quint8 &roleId, const QString &startupName)
{
    m_sqlDatabaseService->RemoveStartupIntoRole(roleId, startupName);
//    TryDeleteFile(m_destinationFolder+startupName);
}

void StartupRepositoryPresenter::AppendStartup(const quint8 &roleId, const QString &startupPath)
{
    m_sqlDatabaseService->AppendStartupIntoRole(roleId, startupPath);
    TryToCopyFile(startupPath);
}

void StartupRepositoryPresenter::TryDeleteFile(const QString &startupPath)
{
//    if(m_terminal->IsFileExists(startupPath, Q_FUNC_INFO, true))
//    {
//        m_terminal->DeleteFileSudo(startupPath, Q_FUNC_INFO);
//    }
}

void StartupRepositoryPresenter::TryToCopyFile(const QString &startupPath)
{
    int index=startupPath.lastIndexOf('/');
    QString startupFileName=startupPath.mid(index);
    if(m_terminal->IsDirNotExists(m_destinationFolder, Q_FUNC_INFO, true))
    {
        m_terminal->CreateFolder(m_destinationFolder, Q_FUNC_INFO, true);
    }
    if(m_terminal->IsFileExists(m_destinationFolder+startupFileName, Q_FUNC_INFO, true))
    {
        m_terminal->DeleteFileSudo(m_destinationFolder+startupFileName, Q_FUNC_INFO);
    }
    m_terminal->CopyFileSudo(startupPath, m_destinationFolder, Q_FUNC_INFO);
}


