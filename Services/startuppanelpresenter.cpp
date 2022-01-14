#include "startuppanelpresenter.h"

StartupPanelPresenter::StartupPanelPresenter(ISqlDatabaseService *sqlDatabaseService)
    : m_terminal(Terminal::GetTerminal())
    , m_sqlDatabaseService(sqlDatabaseService)
{

}

StartupPanelPresenter::~StartupPanelPresenter()
{

}

void StartupPanelPresenter::CheckStartupTable(const quint8 &roleId)
{
    m_sqlDatabaseService->CreateStartupsTableIfNotExists(roleId);
}

QSqlQueryModel *StartupPanelPresenter::GetRoleStartupsModel()
{
    return m_sqlDatabaseService->GetRoleStartupsModel();
}

void StartupPanelPresenter::GetAllStartupsIntoModel(const quint8 &roleId)
{
    m_sqlDatabaseService->GetAllRoleStartupsIntoModel(roleId);
}

QString StartupPanelPresenter::DeleteStartup(const int &roleId, const int &selectedItemIndex)
{
    QSqlQueryModel* model=GetRoleStartupsModel();
    Q_ASSERT(roleId>=0 && roleId<Roles.count());
    Q_ASSERT(selectedItemIndex>= 0&& selectedItemIndex<model->rowCount());
    const QModelIndex currentIndex=model->index(selectedItemIndex, 0);
    const QString currentStartupName=model->data(currentIndex, Qt::DisplayRole).toString();
    m_sqlDatabaseService->RemoveStartupIntoRole(roleId, currentStartupName);
    return currentStartupName;
}

void StartupPanelPresenter::AppendStartup(const quint8 &roleId, const QString &startupPath)
{
    const QString startupFileName=TryToCopyFile(startupPath);
    m_sqlDatabaseService->AppendStartupIntoRole(roleId, startupFileName);
}

QString StartupPanelPresenter::TryToCopyFile(const QString &startupPath)
{
    const int indexOfFileNameBegin=startupPath.lastIndexOf('/');
    const QString startupFileName=startupPath.mid(indexOfFileNameBegin+1);
    const QStringRef startupFolderName=startupPath.leftRef(indexOfFileNameBegin+1);
    if (m_destinationFolder!=startupFolderName)
    {
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
    return startupFileName;
}

int StartupPanelPresenter::GetMaxStartupCount()
{
    return GetRoleStartupsModel()->rowCount();
}

bool StartupPanelPresenter::HasDuplicateStartup(QStringView startupName)
{
    QSqlQueryModel* model=GetRoleStartupsModel();
    for (int i=0; i<model->rowCount(); ++i)
    {
        const QModelIndex currentIndex=model->index(i, 0);
        const QString currentStartupName=model->data(currentIndex, Qt::DisplayRole).toString();
        if(currentStartupName==startupName)
        {
            return  true;
        }
    }
    return false;
}