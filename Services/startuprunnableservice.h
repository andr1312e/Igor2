#ifndef SERVICES_STARTUPRUNNABLESERVICE_H
#define SERVICES_STARTUPRUNNABLESERVICE_H

#include <QProcess>
#include <QStringList>
#include <QObject>
#include <QTimerEvent>

#include "Services/Terminals/terminal.h"
#include "Services/Sql/isqlservice.h"

#include "Logging/logger.h"

class StartupRunnableManager: public QObject
{
    Q_OBJECT

public:
    StartupRunnableManager(const QString &currentUserName, QStringView rlstiFolderPath, ISqlDatabaseService *sqlService, QObject *parent);
    ~StartupRunnableManager();

Q_SIGNALS:
    void ToStartupApplicationNotExsists(const QString &execPath);
    void ToProgramFall();

private Q_SLOTS:
    void OnRestartProcess();
public Q_SLOTS:
    void OnCurrentUserRoleChanged();
    void OnPauseStartupRunnableManager();
    void OnStopStartupRunnableManager();
    void OnRestartStartupRunnableManager();
    bool OnRunStartupRunnableManager();

protected:
    virtual void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private:
    QProcess * CreateReRestartAppProcess(QStringView appNameInFolder);
    QStringList ReadUserStartupFile();
    bool IsAllStartupValid(const QStringList &startupsList);
    void InitStartupProcessList(const QStringList &currentUserStartupsList);

private:
    void GetRunnedProcessesAndProcecessesForListen(const QStringList &listOfAllRunningProcessesName, const QStringList &currentUserStartupsList, QStringList &listAlreadyRunningsApps, QStringList &notRunnedAppsList);

private:
    const QString m_currentUserName;
    const QStringView m_rlsTiFolderPath;
    ISqlDatabaseService* const m_sqlService;
    Terminal* const m_terminal;
    QList<QProcess*> m_runnableProcess;
    QStringList m_listAlreadyRunningsApps;
    int m_currentTimerId;
};

#endif // SERVICES_STARTUPRUNNABLESERVICE_H
