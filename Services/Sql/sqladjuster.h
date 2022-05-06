#ifndef SQLADJUSTER_H
#define SQLADJUSTER_H
#include <QString>
#include <QDebug>
#include <QObject>
#include <QProcess>
#include <QSettings>
#include <QStringList>
#include <QMessageBox>
#include <QApplication>

#include "Services/Terminals/terminal.h"

//Настрока sql при первом запуске

class SqlAdjuster
{
public:
    explicit SqlAdjuster();
    ~SqlAdjuster();
public:
    void InstallPostgreSqlAndDriver();
    void InstallSqlDriverForQt5();
    void ResetPostgreUserPassword(const QString &postgresUser, const QString &postgresPassword);
private:
    void RemoveAstraLinuxPostgreSecurityControl();
    void CloseSynapticIfItRunned();
private:
    Terminal * const m_terminal;
    const QString m_astraPackageManagerName;
    const QString m_astraPackageManagerNameService;
    const QString m_securityAstraFileName;
    const QString m_postgreSqlDriverName;
    const QString m_postgreSqlPackageName;
};

#endif // SQLADJUSTER_H
