#ifndef SQLADJUSTER_H
#define SQLADJUSTER_H
#include <QString>
#include <QDebug>
#include <QObject>
#include <QProcess>
#include <QSettings>
#include <QStringList>
#include <QApplication>
#include <QMessageBox>
#include "Logging/logger.h"
#include "Admin_GUI/Views/qtoast.h"
#include "Services/Terminals/terminal.h"
#include "Services/mountchecker.h"

//Настрока sql при первом запуске

enum class DbErrorState
{
    NoPostgre = 0,
    NoDriver = 1,
    ServiceNotStart = 2,
    WrongPassword = 3,
    UnknownError = 4
};

class SqlProblemsFixer
{
public:
    explicit SqlProblemsFixer();
    ~SqlProblemsFixer();
public:
    bool InstallPostgreSql();
    bool InstallSqlDriverForQt5();
    bool StartPostgreSqlService();
    bool ResetPostgreUserPassword(const QString &postgresUser, const QString &postgresPassword);
    DbErrorState GetLastError() const noexcept;
    void SetLastError(DbErrorState newError) noexcept;
private:
    void RemoveAstraLinuxPostgreSecurityControl();
    void CloseSynapticIfItRunned();
    void CheckAndMountRepository();
    bool ErrorIsCritical(const QString &errorInstall);
    void ShowToast(const QString &message);
private:
    Terminal *const m_terminal;
    MountChecker *const m_mountChecker;
    DbErrorState m_error;
    const QString m_astraPackageManagerName;
    const QString m_astraPackageManagerNameService;
    const QString m_securityAstraFileName;
    const QString m_postgreSqlDriverName;
    const QString m_postgreSqlPackageName;
    const QString m_postgreSqlSerivceStartCommand;
};

#endif // SQLADJUSTER_H
