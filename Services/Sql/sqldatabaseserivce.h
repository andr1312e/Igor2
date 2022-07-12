#ifndef SQLDATABASESERIVCE_H
#define SQLDATABASESERIVCE_H
#include <QDebug>
#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QStringList>
#include <QListView>

#include "Logging/logger.h"

#include "Services/Sql/tablecolumnsnames.h"

#include "Services/Sql/isqlservice.h"
#include "Services/Sql/sqlsettingsrepository.h"
#include "Services/Sql/sqlproblemsfixer.h"

enum class DbConnectionState
{
    Connected = 0,
    NeedRestartAfterDriverInstall = 1,
    NeedRestartAfterPostgreInstall = 2,
    FailedConnection = 3
};


class SqlDatabaseSerivce : public QObject, public ISqlDatabaseService
{
    Q_OBJECT
public:
    explicit SqlDatabaseSerivce(QObject *parent);
    ~SqlDatabaseSerivce();
Q_SIGNALS:
    void ToTrayMessage(const QString &message);
public Q_SLOTS:
    void OnDropDatabase();
public:
    DbConnectionState ConnectToDataBase() noexcept;//никогда так не делай

private:
    DbErrorState AnalyzeError(const QString &errorString, const QString userName) const noexcept;
    DbConnectionState FixError(DbErrorState newError, const QString &errorString, const QString &userName, const QString &password);

public:
    virtual bool CheckUsersTable() Q_DECL_OVERRIDE;
    virtual bool CheckStartupTables() Q_DECL_OVERRIDE;
    virtual bool CheckStartupTables(int roleIndex) Q_DECL_OVERRIDE;
    virtual bool CheckDesktopTables() Q_DECL_OVERRIDE;
    virtual bool CheckDesktopTables(int roleIndex) Q_DECL_OVERRIDE;


    virtual void ClearUsersTable() Q_DECL_OVERRIDE;
    virtual void ClearStartupsTable(int roleIndex) Q_DECL_OVERRIDE;
    virtual void ClearDesktopTable(int roleIndex) Q_DECL_OVERRIDE;

    virtual void AppendUserIntoTable(const User &user) Q_DECL_OVERRIDE;
    virtual void RemoveUserIntoTable(const User &user) Q_DECL_OVERRIDE ;
    virtual QList<User> GetAllUsers() Q_DECL_OVERRIDE;

    virtual void AppendStartupIntoRole(int roleIndex, const QString &exec) Q_DECL_OVERRIDE;
    virtual void RemoveStartupIntoRole(int roleIndex, const QString &startupPath) Q_DECL_OVERRIDE;
    virtual QStringList GetAllRoleStartups(int roleIndex) Q_DECL_OVERRIDE;
    virtual void GetAllRoleStartupsIntoModel(int roleIndex) Q_DECL_OVERRIDE;
    virtual QSqlQueryModel *GetRoleStartupsModel() Q_DECL_OVERRIDE;

    virtual void AppendDesktopIntoRole(int roleIndex, const DesktopEntity &entity) Q_DECL_OVERRIDE;
    virtual void RemoveDesktopIntoRole(int roleIndex, const QString &entityName) Q_DECL_OVERRIDE;
    virtual QList<DesktopEntity> GetAllRoleDesktops(int roleIndex) Q_DECL_OVERRIDE;
    virtual QStringList GetAllUsersWithRoleIndex(int roleIndex) Q_DECL_OVERRIDE;

public:
    virtual void CreateUsersTableIfNotExists() Q_DECL_OVERRIDE;
    virtual void CreateStartupsTableIfNotExists(int roleIndex) Q_DECL_OVERRIDE;
    virtual void CreateDesktopRolesIfNotExists(int roleIndex) Q_DECL_OVERRIDE;

public:
    virtual void DropUsersTable() Q_DECL_OVERRIDE;
    virtual void DropStartupTable(int roleIndex) Q_DECL_OVERRIDE;
    virtual void DropDesktopsTable(int roleIndex) Q_DECL_OVERRIDE;

public:
    virtual QStringList GetAdminsRoleUserName() Q_DECL_OVERRIDE;
    virtual QString GetUserFCS(const QString &currentUserName) Q_DECL_OVERRIDE;
    virtual int GetUserRole(const QString &currentUserName) Q_DECL_OVERRIDE;

    virtual QString GetUserFCS(QStringView currentUserName) Q_DECL_OVERRIDE;
    virtual int GetUserRole(QStringView currentUserName) Q_DECL_OVERRIDE;

private:
    void ClearTable(const QString &tableName);
    bool GetBoolFromMessage(QSqlQuery &query) noexcept;
    QString GetStringFromMessage(const QSqlRecord &record, int rowPos);
private:
    const QString m_postgeSqlDatabaseDriverStringKey;
    QSqlDatabase m_db;
    QSqlQueryModel *const m_currentRoleModel;
    const QString m_usersTablePrefix;
    const QString m_startupTablePrefix;
    const QString m_desktopTablePrefix;
};

#endif // SQLDATABASESERIVCE_H
