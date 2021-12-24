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

#include "Services/Sql/tablecolumnsnames.h"

#include "Services/isqlservice.h"

class SqlDatabaseSerivce : public QObject, public ISqlDatabaseService
{
    Q_OBJECT
public:
    explicit SqlDatabaseSerivce(QObject *parent);
    ~SqlDatabaseSerivce();
public:
    bool ConnectToDataBase(const QString &hostName,
                           const quint16 &port,
                           const QString &databaseName,
                           const QString &userName,
                           const QString &dbPassword);//никогда так не делай
public:

    virtual bool CheckUserTable() Q_DECL_OVERRIDE;
    virtual bool CheckExecTables() Q_DECL_OVERRIDE ;
    virtual bool ChekcDesktopTables() Q_DECL_OVERRIDE;



    virtual void ClearUserTable() Q_DECL_OVERRIDE;
    virtual void ClearExecsTable(quint8 roleId) Q_DECL_OVERRIDE;
    virtual void ClearDesktopTable(quint8 roleId) Q_DECL_OVERRIDE;

    virtual void AppendUserIntoTable(User &user) Q_DECL_OVERRIDE;
    virtual void RemoveUserIntoTable(quint8 roleId, User &user) Q_DECL_OVERRIDE ;
    virtual QList<User> GetAllUsers() Q_DECL_OVERRIDE;

    virtual void AppendExecIntoRole(quint8 roleId, const QString &exec) Q_DECL_OVERRIDE;
    virtual void RemoveExecIntoRole(quint8 roleId, const QString &startupPath) Q_DECL_OVERRIDE;
    virtual QStringList GetAllRoleExecs(quint8 roleId) Q_DECL_OVERRIDE;

    virtual void AppendDesktopIntoRole(quint8 roleId, DesktopEntity &entity) Q_DECL_OVERRIDE;
    virtual void RemoveDesktopIntoRole(quint8 roleId, DesktopEntity &entity) Q_DECL_OVERRIDE;
    virtual QList<DesktopEntity> GetAllRoleDesktops(quint8 roleId) Q_DECL_OVERRIDE;

public:
    virtual void CreateUsersTableIfNotExists() Q_DECL_OVERRIDE;
    virtual void CreateExecsTableInNotExists(quint8 roleId) Q_DECL_OVERRIDE;
    virtual void CreateDesktopRolesIfNotExists(quint8 roleId) Q_DECL_OVERRIDE;

public:
    virtual QStringList GetAdminsRoleUserName() Q_DECL_OVERRIDE;
    virtual QString GetUserFCS(QString &currentUserName) Q_DECL_OVERRIDE;
    virtual QString GetUserRank(QString &currentUserName) Q_DECL_OVERRIDE;
    virtual QString GetUserRole(const QString &currentUserName) Q_DECL_OVERRIDE;

private:
    void ClearTable(QString tableName);
    bool GetBoolFromMessage(QSqlQuery &query);
    void GetStringFromMessage(QString &inputString, const QSqlRecord &record, int rowPos);
private:
    QString postgeSqlDatabaseDriverStringKey="QPSQL";
    QSqlDatabase *db;

    const QString accountsTableName="rlstiaccounts";
    const QString execTablePrefix="rlstiexecs";
    const QString desktopTablePrefix="rlstidesktops";
};

#endif // SQLDATABASESERIVCE_H
