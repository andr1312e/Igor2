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
    OnRunStartupRunnableManager();
}

void StartupRunnableManager::OnPauseStartupRunnableManager()
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Пользователь поставил на паузу перезапуск программ "));
    for (QProcess* const process :qAsConst(m_runnableProcess))
    {
        disconnect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &StartupRunnableManager::OnRestartProcess);
    }
    if(m_currentTimerId!=-1)
    {
        QObject::killTimer(m_currentTimerId);
        m_currentTimerId=-1;
    }
}

void StartupRunnableManager::OnStopStartupRunnableManager()
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Пользователь остановил перезапуск программ "));
    for (QProcess* const process :qAsConst(m_runnableProcess))
    {
        disconnect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &StartupRunnableManager::OnRestartProcess);
        process->terminate();
    }
    m_runnableProcess.clear();
    m_listAlreadyRunningsApps.clear();
    if(m_currentTimerId!=-1)
    {
        QObject::killTimer(m_currentTimerId);
        m_currentTimerId=-1;
    }
}

void StartupRunnableManager::OnRestartStartupRunnableManager()
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Пользователь перезапустил перезапуск программ "));
    OnStopStartupRunnableManager();
    OnRunStartupRunnableManager();
}

bool StartupRunnableManager::OnRunStartupRunnableManager()
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" начинаем запуск приложений для роли "));
    const QStringList starups = ReadUserStartupFile();

    if (IsAllStartupValid(starups)) {
        InitStartupProcessList(starups);
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Запуск приложений для роли успешен."));
        return true;
    } else {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Запуск приложений для роли не успешен."));
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
        QStringList listOfAlreadyRunningProcessName=m_terminal->GetAllProcessList(Q_FUNC_INFO);
        for (const QString &alreadyRunningApp:qAsConst(m_listAlreadyRunningsApps))
        {
            if(!listOfAlreadyRunningProcessName.contains(alreadyRunningApp))
            {
                Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Программа на подписке упала."));
                m_runnableProcess.append(CreateReRestartApp(alreadyRunningApp));
            }
        }
    }
}

QProcess *StartupRunnableManager::CreateReRestartApp(const QString &startup)
{
    QProcess *process=new QProcess();
    process->setObjectName(m_rlsTiFolderPath+startup);
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Создаем процесс. Путь для исполняемого файла: " )+ process->objectName());
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &StartupRunnableManager::OnRestartProcess);
    process->start(m_rlsTiFolderPath+startup);
    return process;
}

QStringList StartupRunnableManager::ReadUserStartupFile()
{
    QStringList startupsList;
    const int userRole=m_sqlService->GetUserRole(m_currentUserName);
    if (!(-1==userRole))
    {
        startupsList=m_sqlService->GetAllRoleStartups(userRole);
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Получили список перезапускаемых приложений. Роль пользователя: ")+ QString::number(userRole));
    }
    return startupsList;
}

bool StartupRunnableManager::IsAllStartupValid(const QStringList &startupsList)
{
    for (const QString &startup:startupsList) {
        if (!QFile::exists(m_rlsTiFolderPath+startup)) {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Отсутсвует файл:")+ startup);
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
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Список программ для перезапуска пуст "));
    }
    else
    {
        const QStringList listOfAlreadyRunningProcessName=m_terminal->GetAllProcessList(Q_FUNC_INFO);
        QStringList notRunnedApps;
        GetRunnedProcessesAndProcecessesForListen(listOfAlreadyRunningProcessName, startupsList, m_listAlreadyRunningsApps, notRunnedApps);
        for (const QString &startup : qAsConst(notRunnedApps))
        {
            m_runnableProcess.push_back(CreateReRestartApp(startup));
        }
        if(!m_listAlreadyRunningsApps.isEmpty())
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Есть программы с таким же именем, что были запущены до этой, контроль над ними будет осуществляться с помощью таймера. Их колличество : ")+ QString::number(m_listAlreadyRunningsApps.count()));
            m_currentTimerId=QObject::startTimer(1000, Qt::VeryCoarseTimer);
        }
    }
}

void StartupRunnableManager::GetRunnedProcessesAndProcecessesForListen(const QStringList &listOfAllRunningProcessesName, const QStringList &currentUserStartupsList, QStringList &listAlreadyRunningsApps, QStringList &notRunnedAppsList)
{
    for (const QString & startup :currentUserStartupsList)
    {
        if(listOfAllRunningProcessesName.contains(startup))
        {
            listAlreadyRunningsApps.push_back(startup);
        }
        else
        {
            notRunnedAppsList.push_back(startup);
        }
    }
}

