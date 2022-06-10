#ifndef SQLADJUSTER_H
#define SQLADJUSTER_H
#include <QString>
#include <QDebug>
#include <QObject>
#include <QProcess>
#include <QSettings>
#include <QStringList>
#include <QMessageBox>
#include "Logging/logger.h"
#include "Services/Terminals/terminal.h"
//Настрока sql при первом запуске

enum class DbErrorState
{
    NoPostgre = 0,
    NoDriver = 1,
    WrongPassword = 2,
    UnknownError = 3
};

class SqlProblemsFixer
{
public:
    explicit SqlProblemsFixer();
    ~SqlProblemsFixer();
public:
    bool InstallPostgreSqlAndDriver();
    bool InstallSqlDriverForQt5();
    bool ResetPostgreUserPassword(const QString &postgresUser, const QString &postgresPassword);
    DbErrorState GetLastError() const noexcept;
    void SetLastError(DbErrorState newError) noexcept;
private:
    void RemoveAstraLinuxPostgreSecurityControl();
    void CloseSynapticIfItRunned();
    bool ErrorIsCritical(const QString &errorInstall);
private:
    Terminal *const m_terminal;
    DbErrorState m_error;
    const QString m_astraPackageManagerName;
    const QString m_astraPackageManagerNameService;
    const QString m_securityAstraFileName;
    const QString m_postgreSqlDriverName;
    const QString m_postgreSqlPackageName;
};

#endif // SQLADJUSTER_H
