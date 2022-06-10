#ifndef SERVICES_STARTUPMANAGERSERVICE_H
#define SERVICES_STARTUPMANAGERSERVICE_H
#include <QSqlQueryModel>
#include <QStringList>
#include "Services/Sql/isqlservice.h"
#include "Services/Terminals/terminal.h"

class StartupPanelPresenter
{
public:
    explicit StartupPanelPresenter(ISqlDatabaseService *sqlDatabaseService);
    ~StartupPanelPresenter();

public:
    void CheckStartupTable(int roleId);
    QSqlQueryModel *GetRoleStartupsModel();
    int GetMaxStartupCount();
    bool HasDuplicateStartup(QStringView startupName);
    void GetAllStartupsIntoModel(int roleId);
    QString DeleteStartup(int roleId, const QModelIndex &selectedItem);
    void AppendStartup(int roleId, const QString &startupPath);

private:
    QString TryToCopyFile(const QString &startupPath);

private:
    Terminal *m_terminal;
    ISqlDatabaseService *m_sqlDatabaseService;
    const QString m_destinationFolder="/usr/RLS_TI/";

};

#endif // SERVICES_STARTUPMANAGERSERVICE_H
