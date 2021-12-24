#ifndef SERVICES_STARTUPRUNNABLESERVICE_H
#define SERVICES_STARTUPRUNNABLESERVICE_H

#include <QProcess>
#include <QStringList>
#include <QObject>
#include <QTimerEvent>

#include "Services/Terminals/terminal.h"
#include "Services/isqlservice.h"

class StartupRunnableManager: public QObject
{
    Q_OBJECT

public:
    StartupRunnableManager(const QString &rlstiFolderPath, ISqlDatabaseService *sqlService, Terminal *terminal, QObject *parent);
    ~StartupRunnableManager();

Q_SIGNALS:
    void ToExecApplicationNotExsists(const QString &execPath);
    void ToProgramFall();

private Q_SLOTS:
    void OnRestartProcess();

public:
    bool SetUserNameAndCheckFilesExsists(const QString &userName);

protected:
    virtual void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private:
    QProcess * CreateReRestartApp(const QString &exec);
    QStringList ReadUserStartupFile(const QString &userName);
    bool IsAllStartupValid(const QStringList &startupsList);
    void InitStartupProcessList(const QStringList &startupsList);

private:
    void StringsThatAreContainedAndNot(const QStringList &sourceList, const QStringList &otherList, QStringList &contained, QStringList &notContained);

private:
    const QString m_rlsTiFolderPath;
    ISqlDatabaseService *m_sqlService;
    Terminal *m_terminal;
    QList<QProcess*> m_runnableProcess;
    QStringList m_listAlreadyRunningsApps;
};

#endif // SERVICES_STARTUPRUNNABLESERVICE_H
