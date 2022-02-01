#include "startuprunnableservice.h"
#include <QDebug>
#include <QFile>

StartupRunnableManager::StartupRunnableManager(const QString &currentUserName, QStringView rlstiFolderPath, ISqlDatabaseService *sqlService, QObject *parent)
    : QObject(parent)
    , m_currentUserName(currentUserName)
    , m_rlsTiFolderPath(rlstiFolderPath)
    , m_sqlService(sqlService)
    , m_terminal(Terminal::GetTerminal())
    , m_currentTimerId(-1)
{

}

StartupRunnableManager::~StartupRunnableManager()
{

}

void StartupRunnableManager::OnRestartProcess()
{

    QProcess *process = qobject_cast<QProcess *>(sender());
    if(Q_NULLPTR!=process && 0==process->exitCode())
    {
        Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" какая-то программа упала. Код: ") + QString::number(process->exitCode()));
        process->start();
        Q_EMIT ToProgramFall();
    }
}

void StartupRunnableManager::OnCurrentUserRoleChanged()
{
    Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Пользователь именил свою роль "));
    m_listAlreadyRunningsApps.clear();
    OnStopStartupRunnableManager();
    m_runnableProcess.clear();
    OnRunStartupRunnableManager();
}

void StartupRunnableManager::OnPauseStartupRunnableManager()
{
    Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Пользователь поставил на паузу перезапуск программ "));
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
    //Убиваются только те процессы которые были запущены моей программой,
    //Те что были запущены до, и на жизнь которых я подписан таймером не убиваются
    //Я сознательно не стал убивать эти процессы
    Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Пользователь остановил перезапуск программ "));
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
    Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Пользователь перезапустил перезапуск программ "));
    OnStopStartupRunnableManager();
    OnRunStartupRunnableManager();
}

bool StartupRunnableManager::OnRunStartupRunnableManager()
{
    Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" начинаем запуск приложений для роли "));
    const QStringList starups = ReadUserStartupFile();

    if (IsAllStartupValid(starups)) {
        InitStartupProcessList(starups);
        Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Запуск приложений для роли успешен."));
        return true;
    } else {
        Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Запуск приложений для роли не успешен."));
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
        const QStringList listOfAlreadyRunningProcessName=m_terminal->GetAllProcessList(Q_FUNC_INFO);
        for (const QString &alreadyRunningApp:qAsConst(m_listAlreadyRunningsApps))
        {
            if(!listOfAlreadyRunningProcessName.contains(alreadyRunningApp))
            {
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Программа на подписке упала."));
                m_runnableProcess.push_back(CreateReRestartAppProcess(alreadyRunningApp));
            }
        }
    }
}

QProcess *StartupRunnableManager::CreateReRestartAppProcess(QStringView appNameInFolder)
{
    QProcess* const process=new QProcess(this);
    process->setObjectName(m_rlsTiFolderPath+appNameInFolder);
    Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Создаем процесс. Путь для исполняемого файла: " )+ process->objectName());
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &StartupRunnableManager::OnRestartProcess);
    process->start(process->objectName());
    return process;
}

QStringList StartupRunnableManager::ReadUserStartupFile()//main
{
    QStringList startupsList;
    const int userRole=m_sqlService->GetUserRole(m_currentUserName);
    if (!(-1==userRole))
    {
        startupsList=m_sqlService->GetAllRoleStartups(userRole);
        Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Получили список перезапускаемых приложений. Роль пользователя: ")+ QString::number(userRole));
    }
    return startupsList;
}

bool StartupRunnableManager::IsAllStartupValid(const QStringList &startupsList)
{
    for (const QString &startup:startupsList) {
        if (!QFile::exists(m_rlsTiFolderPath+startup)) {
            Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Отсутсвует файл:")+ startup);
            Q_EMIT ToStartupApplicationNotExsists(m_rlsTiFolderPath+startup);
            return false;
        }
    }
    return true;
}

void StartupRunnableManager::InitStartupProcessList(const QStringList &currentUserStartupsList)
{
    if (!currentUserStartupsList.empty())
    {
        QStringList listOfAlreadyRunningProcessName=m_terminal->GetAllProcessList(Q_FUNC_INFO);
        QStringList notRunnedApps;
        GetRunnedProcessesAndProcecessesForListen(listOfAlreadyRunningProcessName, currentUserStartupsList, m_listAlreadyRunningsApps, notRunnedApps);
        for (const QString &startup : qAsConst(notRunnedApps))
        {
            m_runnableProcess.push_back(CreateReRestartAppProcess(startup));
        }
        if(!m_listAlreadyRunningsApps.isEmpty())
        {
            Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Есть программы с таким же именем, что были запущены до этой, контроль над ними будет осуществляться с помощью таймера. Их колличество : ")+ QString::number(m_listAlreadyRunningsApps.count()));
            m_currentTimerId=QObject::startTimer(1000, Qt::VeryCoarseTimer);
        }
    }
    else
    {
        Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Список программ для перезапуска пуст "));
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

