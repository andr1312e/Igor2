#include "sqlproblemsfixer.h"

SqlProblemsFixer::SqlProblemsFixer()
    : m_terminal(Terminal::GetTerminal())
    , m_mountChecker(new MountChecker())
    , m_error(DbErrorState::UnknownError)
    , m_astraPackageManagerName(QLatin1Literal("synaptic"))
    , m_astraPackageManagerNameService(QLatin1Literal("synaptic-pkexec"))
    , m_securityAstraFileName(QLatin1Literal("/etc/parsec/mswitch.conf"))
    , m_postgreSqlDriverName(QLatin1Literal("libqt5sql5-psql"))
    , m_postgreSqlPackageName(QLatin1Literal("postgresql-9.6"))
    , m_postgreSqlSerivceStartCommand(QLatin1Literal("service postgresql start"))
{

}

SqlProblemsFixer::~SqlProblemsFixer()
{
    delete m_mountChecker;
}

bool SqlProblemsFixer::InstallPostgreSqlAndDriver()
{
    CheckAndMountRepository();
    CloseSynapticIfItRunned();
    QString outputInfo, errorInfo;
    m_terminal->InstallPackageSudo(m_postgreSqlPackageName, outputInfo, errorInfo);
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Установка постгре Вывод: ") + outputInfo + QStringLiteral(" Ошибки:") + errorInfo);
    if (!errorInfo.isEmpty())
    {
        if (ErrorIsCritical(errorInfo))
        {
            return false;
        }
    }
    if (InstallSqlDriverForQt5())
    {
        return StartPostgreSqlService();
    }
}

bool SqlProblemsFixer::InstallSqlDriverForQt5()
{
    CheckAndMountRepository();
    CloseSynapticIfItRunned();
    QString outputInfo, errorInfo;
    m_terminal->InstallPackageSudo(m_postgreSqlDriverName, outputInfo, errorInfo);
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Установка постгре Вывод: ") + outputInfo + QStringLiteral(" Ошибки:") + errorInfo);
    if (!errorInfo.isEmpty())
    {
        if (ErrorIsCritical(errorInfo))
        {
            return false;
        }
    }
    return true;
}

bool SqlProblemsFixer::StartPostgreSqlService()
{
    QString outputInfo, errorInfo;
    m_terminal->RunConsoleCommandSync(m_postgreSqlSerivceStartCommand, outputInfo, errorInfo);
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Запуск сервиса постгре Вывод: ") + outputInfo + QStringLiteral(" Ошибки:") + errorInfo);
    return true;
}

bool SqlProblemsFixer::ResetPostgreUserPassword(const QString &postgresUser, const QString &postgresPassword)
{
    Terminal *const terminal = Terminal::GetTerminal();
    //Попытка создать пользователя
    const QString createUserCommand = "sudo -i -u postgres psql -c \"CREATE USER " + postgresUser + " WITH SUPERUSER CREATEDB CREATEROLE REPLICATION BYPASSRLS INHERIT LOGIN PASSWORD '" + postgresPassword + "';\"";
    QString commandOutput, commandError;
    terminal->RunConsoleCommandSync(createUserCommand, commandOutput, commandError);
    if (commandOutput.isEmpty())
    {
        //Пытаемся поменять у пользователя пароль
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Ошибка смены логина:") + commandError);
        const QString replaceUserPassword = "sudo -i -u postgres psql -c \"ALTER USER " + postgresUser + " WITH SUPERUSER CREATEDB CREATEROLE REPLICATION BYPASSRLS INHERIT LOGIN PASSWORD '" + postgresPassword + "';\"";
        terminal->RunConsoleCommandSync(replaceUserPassword, commandOutput, commandError);
        if (commandOutput.isEmpty())
        {
            //Пользователь должен удалить пользователя бд
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Не можем поменять пароль ошибка:") + commandError);
            return  false;
        }
    }
    RemoveAstraLinuxPostgreSecurityControl();
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" пароль поменян"));
    return  true;
}

DbErrorState SqlProblemsFixer::GetLastError() const noexcept
{
    return m_error;
}

void SqlProblemsFixer::SetLastError(DbErrorState newError) noexcept
{
    m_error = newError;
}

void SqlProblemsFixer::RemoveAstraLinuxPostgreSecurityControl()
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Убираем защитный костыль от астры"));
    const QString fileData = m_terminal->GetFileText(m_securityAstraFileName, Q_FUNC_INFO, true);
    QStringList rowsList = fileData.split('\n');
    rowsList.removeAll("");
    QVector<int> commendsRow;
    for (int i = 0; i < rowsList.count(); ++i)
    {
        const QString currentRow = rowsList.at(i);
        if (currentRow.startsWith('#')) //Удаляем мусор из файла
        {
            commendsRow.append(i);
        }
    }
    for (int i = commendsRow.count() - 1; i >= 0; i--)
    {
        rowsList.removeAt(commendsRow.at(i));
    }
    const int index = rowsList.indexOf(QLatin1Literal("zero_if_notfound: no"));
    if (index != -1)
    {
        rowsList.replace(index, QLatin1Literal("zero_if_notfound: yes"));
        m_terminal->ClearFileSudo(m_securityAstraFileName, Q_FUNC_INFO);
        for (const QString &row : qAsConst(rowsList))
        {
            m_terminal->AppendTextToFileSudo(row, m_securityAstraFileName, Q_FUNC_INFO);
        }
    }
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Костыль убран"));
}

void SqlProblemsFixer::CloseSynapticIfItRunned()
{
    const QStringList allProcessesList = m_terminal->GetAllProcessList(Q_FUNC_INFO);
    if (allProcessesList.contains(m_astraPackageManagerName) || allProcessesList.contains(m_astraPackageManagerNameService))
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Закрываем синаптик"));
        m_terminal->KillProcess(m_astraPackageManagerName, Q_FUNC_INFO);
    }
}

void SqlProblemsFixer::CheckAndMountRepository()
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Проверка наличие исо образов"));
    if (m_mountChecker->HasMountFiles())
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral("Образы в наличии. Проверка монтировки репозиториев"));
        if (!m_mountChecker->IsReposiotoryMounted(Q_FUNC_INFO))
        {
            m_mountChecker->MountRepository();
        }
    }
    else
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral("Образы отсутвуют. Необходимо поместить их в папку. Имена /opt/devel-smolensk-1.6.iso и /opt/smolensk-1.6.iso или /opt/boot/boot.iso и /opt/dev/dev.iso"));
        QMessageBox::critical(Q_NULLPTR, QStringLiteral("Ошибка поиска образов"), QStringLiteral("Образы отсутвуют. Необходимо поместить их в папку. Имена /opt/devel-smolensk-1.6.iso и /opt/smolensk-1.6.iso или /opt/boot/boot.iso и /opt/dev/dev.iso"));
        QApplication::exit();
    }
}

bool SqlProblemsFixer::ErrorIsCritical(const QString &errorInstall)
{
    if (errorInstall.contains("интерфейс: Dialog"))
    {
        return false;
    }
    QMessageBox msgWarning;
    msgWarning.setText(errorInstall);
    msgWarning.setIcon(QMessageBox::Critical);
    msgWarning.setWindowTitle(QStringLiteral("Ошибка при подключении к бд."));
    msgWarning.exec();
    return  true;
}
