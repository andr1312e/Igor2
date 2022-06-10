#include "sqlproblemsfixer.h"

SqlProblemsFixer::SqlProblemsFixer()
    : m_terminal(Terminal::GetTerminal())
    , m_error(DbErrorState::UnknownError)
    , m_astraPackageManagerName(QLatin1Literal("synaptic"))
    , m_astraPackageManagerNameService(QLatin1Literal("synaptic-pkexec"))
    , m_securityAstraFileName(QLatin1Literal("/etc/parsec/mswitch.conf"))
    , m_postgreSqlDriverName(QLatin1Literal("libqt5sql5-psql"))
    , m_postgreSqlPackageName(QLatin1Literal("libqt5sql5-psql"))
{
}

SqlProblemsFixer::~SqlProblemsFixer()
{

}

bool SqlProblemsFixer::InstallPostgreSqlAndDriver()
{
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
    return InstallSqlDriverForQt5();
}

bool SqlProblemsFixer::InstallSqlDriverForQt5()
{
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

bool SqlProblemsFixer::ResetPostgreUserPassword(const QString &postgresUser, const QString &postgresPassword)
{
    Terminal *const terminal = Terminal::GetTerminal();
    //Попытка создать пользователя
    const QString createUserCommand = "sudo -u postgres bash -c \"psql -c \\\"CREATE USER '"
                                      + postgresUser + "' WITH  SUPERUSER CREATEDB CREATEROLE REPLICATION BYPASSRLS INHERIT LOGIN PASSWORD '" + postgresPassword + "';\\\"\"";
    QString commandOutput, commandError;
    terminal->RunConsoleCommandSync(createUserCommand, commandOutput, commandError);
    if (commandOutput.isEmpty())
    {
        //Пытаемся поменять у пользователя пароль
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Ошибка смены логина:") + commandError);
        const QString replaceUserPassword = "sudo -u postgres bash -c \"psql -c \\\"ALTER USER '"
                                            + postgresUser + "' WITH LOGIN SUPERUSER CREATEDB CREATEROLE REPLICATION IBYPASSRLS NHERIT LOGIN PASSWORD '" + postgresPassword + "';\\\"\"";
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
    //Убираем защитный костыль от астры
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

bool SqlProblemsFixer::ErrorIsCritical(const QString &errorInstall)
{
    if (errorInstall.contains("Невозможно получить некоторые архивы"))
    {
        QMessageBox msgWarning;
        msgWarning.setText(QStringLiteral("Для монтирования диска разработчика обратитесь к 1 части руководства:"
                                          "\nАдминистратора Операционной системы специального назначения «Astra Linux»\n"
                                          "РУСБ.10015-01 95 01-1, Глава 9, стр. 228-231"));
        msgWarning.setIcon(QMessageBox::Critical);
        msgWarning.setWindowTitle(QStringLiteral("Не примонтирован диск разработчика."));
        msgWarning.exec();
        return  true;
    }
    return false;
}
