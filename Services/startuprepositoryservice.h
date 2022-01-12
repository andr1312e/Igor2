#ifndef SERVICES_STARTUPMANAGERSERVICE_H
#define SERVICES_STARTUPMANAGERSERVICE_H

#include <QStringList>
#include "Services/Sql/isqlservice.h"
#include "Services/Terminals/terminal.h"

class StartupRepositoryPresenter
{
public:

    StartupRepositoryPresenter(ISqlDatabaseService *sqlDatabaseService);

    ~StartupRepositoryPresenter();

    void CheckStartupTable(const quint8 &roleId);

    QStringList GetAllStartups(const quint8 &roleId);

    void DeleteStartup(const quint8 &roleId, const QString &startupName);

    void AppendStartup(const quint8 &roleId, const QString &startupPath);

private:
    void TryDeleteFile(const QString &startupPath);
    QString TryToCopyFile(const QString &startupPath);

private:
    Terminal *m_terminal;
    ISqlDatabaseService *m_sqlDatabaseService;
    const QString m_destinationFolder="/usr/RLS_TI/";

};

#endif // SERVICES_STARTUPMANAGERSERVICE_H
