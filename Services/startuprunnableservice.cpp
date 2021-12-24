#include "startuprunnableservice.h"
#include <QDebug>
#include <QFile>

StartupRunnableManager::StartupRunnableManager(const QString &rlstiFolderPath, ISqlDatabaseService *sqlService, Terminal *terminal, QObject *parent)
    : QObject(parent)
    , m_rlsTiFolderPath(rlstiFolderPath)
    , m_sqlService(sqlService)
    , m_terminal(terminal)
{

}

StartupRunnableManager::~StartupRunnableManager()
{

}

void StartupRunnableManager::OnRestartProcess()
{
    QProcess *process = qobject_cast<QProcess *>(sender());
    if(process->exitCode()==0)
    {
        process->start();
        Q_EMIT ToProgramFall();
    }
}

bool StartupRunnableManager::SetUserNameAndCheckFilesExsists(const QString &userName)
{
    const QStringList execs = ReadUserStartupFile(userName);

    if (IsAllStartupValid(execs)) {
        InitStartupProcessList(execs);
        return true;
    } else {
        return false;
    }
}

void StartupRunnableManager::timerEvent(QTimerEvent *event)
{
    if(m_listAlreadyRunningsApps.isEmpty())
    {
        killTimer(event->timerId());
    }
    else
    {
        QStringList listOfAlreadyRunningProcessName=m_terminal->GetAllProcessList("StartupRunnableManager::InitStartupProcessList");
        for (const QString &alreadyRunningApp:m_listAlreadyRunningsApps)
        {
            if(!listOfAlreadyRunningProcessName.contains(alreadyRunningApp))
            {
                m_runnableProcess.append(CreateReRestartApp(alreadyRunningApp));
            }
        }
    }
}

QProcess *StartupRunnableManager::CreateReRestartApp(const QString &exec)
{
    QProcess *process=new QProcess();
    process->setObjectName(m_rlsTiFolderPath+exec);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &StartupRunnableManager::OnRestartProcess);
    process->start(m_rlsTiFolderPath+exec);
    return process;
}

QStringList StartupRunnableManager::ReadUserStartupFile(const QString &userName)
{
    QStringList startupsList;
    const QString userRole=m_sqlService->GetUserRole(userName);
    if (!userRole.isEmpty())
    {
        const int roleId=Roles.indexOf(userRole);
        startupsList=m_sqlService->GetAllRoleExecs(roleId);
    }
    return startupsList;
}

bool StartupRunnableManager::IsAllStartupValid(const QStringList &startupsList)
{
    for (const QString &startup:startupsList) {
        if (!QFile::exists(m_rlsTiFolderPath+startup)) {
            Q_EMIT ToExecApplicationNotExsists(m_rlsTiFolderPath+startup);
            return false;
        }
    }
    return true;
}

void StartupRunnableManager::InitStartupProcessList(const QStringList &startupsList)
{
    if (startupsList.empty())
    {
        return;
    }
    else
    {
        QStringList listOfAlreadyRunningProcessName=m_terminal->GetAllProcessList("StartupRunnableManager::InitStartupProcessList");
        QStringList notRunnedApps;
        StringsThatAreContainedAndNot(listOfAlreadyRunningProcessName, startupsList, m_listAlreadyRunningsApps, notRunnedApps);
        for (const QString &exec : notRunnedApps)
        {
            m_runnableProcess.append(CreateReRestartApp(exec));
        }
        if(!m_listAlreadyRunningsApps.isEmpty())
        {
            startTimer(2000, Qt::VeryCoarseTimer);
        }
    }
}

void StartupRunnableManager::StringsThatAreContainedAndNot(const QStringList &sourceList, const QStringList &otherList, QStringList &contained, QStringList &notContained)
{
    for (const QString & string :otherList)
    {
        if(sourceList.contains(string))
        {
            contained.push_back(string);
        }
        else
        {
            notContained.push_back(string);
        }
    }
}

