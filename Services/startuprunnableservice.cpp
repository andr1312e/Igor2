#include "startuprunnableservice.h"
#include <QDebug>
#include <QFile>

StartupRunnableManager::StartupRunnableManager(const QString &currentUserName, QStringView rlstiFolderPath, ISqlDatabaseService *sqlService, QObject *parent)
    : QObject(parent)
    , m_currentUserName(currentUserName)
    , m_rlsTiFolderPath(rlstiFolderPath)
    , m_sqlService(sqlService)
    , m_terminal(Terminal::GetTerminal())
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

void StartupRunnableManager::OnCurrentUserRoleChanged()
{
    m_listAlreadyRunningsApps.clear();
    for (QProcess *process:m_runnableProcess)
    {
        disconnect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &StartupRunnableManager::OnRestartProcess);
        process->kill();
    }
    m_runnableProcess.clear();
    RunStartups();
}

bool StartupRunnableManager::RunStartups()
{
    const QStringList starups = ReadUserStartupFile();

    if (IsAllStartupValid(starups)) {
        InitStartupProcessList(starups);
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

QProcess *StartupRunnableManager::CreateReRestartApp(const QString &startup)
{
    QProcess *process=new QProcess();
    process->setObjectName(m_rlsTiFolderPath+startup);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &StartupRunnableManager::OnRestartProcess);
    process->start(m_rlsTiFolderPath+startup);
    return process;
}

QStringList StartupRunnableManager::ReadUserStartupFile()
{
    QStringList startupsList;
    const int userRole=m_sqlService->GetUserRole(m_currentUserName);
    if (!(userRole==-1))
    {
        startupsList=m_sqlService->GetAllRoleStartups(userRole);
    }
    return startupsList;
}

bool StartupRunnableManager::IsAllStartupValid(const QStringList &startupsList)
{
    for (const QString &startup:startupsList) {
        if (!QFile::exists(m_rlsTiFolderPath+startup)) {
            Q_EMIT ToStartupApplicationNotExsists(m_rlsTiFolderPath+startup);
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
        for (const QString &startup : notRunnedApps)
        {
            m_runnableProcess.append(CreateReRestartApp(startup));
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

