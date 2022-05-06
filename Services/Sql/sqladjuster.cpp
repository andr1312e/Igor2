#include "sqladjuster.h"

SqlAdjuster::SqlAdjuster()
    : m_terminal(Terminal::GetTerminal())
    , m_astraPackageManagerName(QStringLiteral("synaptic"))
    , m_astraPackageManagerNameService(QStringLiteral("synaptic-pkexec"))
    , m_securityAstraFileName(QStringLiteral("/etc/parsec/mswitch.conf"))
    , m_postgreSqlDriverName(QStringLiteral("libqt5sql5-psql"))
    , m_postgreSqlPackageName(QStringLiteral("libqt5sql5-psql"))
{
}

SqlAdjuster::~SqlAdjuster()
{

}

void SqlAdjuster::InstallPostgreSqlAndDriver()
{
    CloseSynapticIfItRunned();
    m_terminal->InstallPackageSudo(m_postgreSqlPackageName, Q_FUNC_INFO);
    InstallSqlDriverForQt5();

}

void SqlAdjuster::InstallSqlDriverForQt5()
{
    CloseSynapticIfItRunned();
    m_terminal->InstallPackageSudo(m_postgreSqlDriverName, Q_FUNC_INFO);
}

void SqlAdjuster::ResetPostgreUserPassword(const QString &postgresUser, const QString &postgresPassword)
{
    Terminal * const terminal=Terminal::GetTerminal();
    //Попытка создать пользователя
    const QString createUserCommand="sudo -u postgres bash -c \"psql -c \\\"CREATE USER "
            +postgresUser+" WITH  SUPERUSER CREATEDB CREATEROLE REPLICATION BYPASSRLS INHERIT LOGIN PASSWORD '"+postgresPassword+"';\\\"\"";
    QString commandOutput, commandError;
    terminal->RunConsoleCommandSync(createUserCommand, commandOutput, commandError, Q_FUNC_INFO);
    if (commandOutput.isEmpty())
    {
        //Пытаемся поменять у пользователя пароль
        const QString replaceUserPassword="sudo -u postgres bash -c \"psql -c \\\"ALTER USER "
                +postgresUser+" WITH LOGIN SUPERUSER CREATEDB CREATEROLE REPLICATION IBYPASSRLS NHERIT LOGIN PASSWORD '"+postgresPassword+"';\\\"\"";
        terminal->RunConsoleCommandSync(replaceUserPassword, commandOutput, commandError, Q_FUNC_INFO);
        if (commandOutput.isEmpty())
        {
            //Пользователь должен удалить пользоваеля
            QMessageBox::critical(Q_NULLPTR, QStringLiteral("Невозможно создать пользователя в таблице"), QStringLiteral("Воспользуйтесь командами(по порядку): \n1.sudo -u postgres psql - консоль постгре\n2.\\du - список всех пользователей \n3.drop user \"")+postgresUser+"\"; - удалить юзера с неймом: "+postgresUser+"(точка с запятой важна!)\n4.\\q - выход. После этого будьте добры включите программу снова. Спасибо");
            QApplication::exit(1);
        }
    }
    RemoveAstraLinuxPostgreSecurityControl();
}

void SqlAdjuster::RemoveAstraLinuxPostgreSecurityControl()
{
    //Убираем защитный костыль от астры
    const QString fileData=m_terminal->GetFileText(m_securityAstraFileName, Q_FUNC_INFO, true);
    QStringList rowsList=fileData.split('\n');
    rowsList.removeAll("");
    QVector<int> commendsRow;
    for (int i=0; i<rowsList.count(); ++i)
    {
        const QString currentRow=rowsList.at(i);
        if(currentRow.startsWith('#'))//Удаляем мусор из файла
        {
            commendsRow.append(i);
        }
    }
    for (int i=commendsRow.count()-1; i>=0; i--)
    {
        rowsList.removeAt(commendsRow.at(i));
    }
    const int index=rowsList.indexOf(QStringLiteral("zero_if_notfound: no"));
    if(index!=-1)
    {
        rowsList.replace(index, QStringLiteral("zero_if_notfound: yes"));
        m_terminal->ClearFileSudo(m_securityAstraFileName, Q_FUNC_INFO);
        for (const QString &row: qAsConst(rowsList))
        {
           m_terminal->AppendTextToFileSudo(row, m_securityAstraFileName, Q_FUNC_INFO);
        }
    }
}

void SqlAdjuster::CloseSynapticIfItRunned()
{
    const QStringList allProcessesList=m_terminal->GetAllProcessList(Q_FUNC_INFO);
    if(allProcessesList.contains(m_astraPackageManagerName) || allProcessesList.contains(m_astraPackageManagerNameService))
    {
        m_terminal->KillProcess(m_astraPackageManagerName, Q_FUNC_INFO);
    }
}
