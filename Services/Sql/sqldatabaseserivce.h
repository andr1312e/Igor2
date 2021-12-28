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

#include "Services/Sql/isqlservice.h"

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

    virtual bool CheckUsersTable() Q_DECL_OVERRIDE;
    virtual bool CheckStartupTables() Q_DECL_OVERRIDE;
    virtual bool CheckStartupTables(quint8 roleId) Q_DECL_OVERRIDE;
    virtual bool CheckDesktopTables() Q_DECL_OVERRIDE;
    virtual bool CheckDesktopTables(quint8 roleId) Q_DECL_OVERRIDE;


    virtual void ClearUsersTable() Q_DECL_OVERRIDE;
    virtual void ClearStartupsTable(quint8 roleId) Q_DECL_OVERRIDE;
    virtual void ClearDesktopTable(quint8 roleId) Q_DECL_OVERRIDE;

    virtual void AppendUserIntoTable(const User &user) Q_DECL_OVERRIDE;
    virtual void RemoveUserIntoTable(quint8 roleId, User &user) Q_DECL_OVERRIDE ;
    virtual QList<User> GetAllUsers() Q_DECL_OVERRIDE;

    virtual void AppendStartupIntoRole(quint8 roleId, const QString &exec) Q_DECL_OVERRIDE;
    virtual void RemoveStartupIntoRole(quint8 roleId, const QString &startupPath) Q_DECL_OVERRIDE;
    virtual QStringList GetAllRoleStartups(quint8 roleId) Q_DECL_OVERRIDE;

    virtual void AppendDesktopIntoRole(quint8 roleId, DesktopEntity &entity) Q_DECL_OVERRIDE;
    virtual void RemoveDesktopIntoRole(quint8 roleId, DesktopEntity &entity) Q_DECL_OVERRIDE;
    virtual QList<DesktopEntity> GetAllRoleDesktops(quint8 roleId) Q_DECL_OVERRIDE;

public:
    virtual void CreateUsersTableIfNotExists() Q_DECL_OVERRIDE;
    virtual void CreateStartupsTableInNotExists(quint8 roleId) Q_DECL_OVERRIDE;
    virtual void CreateDesktopRolesIfNotExists(quint8 roleId) Q_DECL_OVERRIDE;

public:
    virtual QStringList GetAdminsRoleUserName() Q_DECL_OVERRIDE;
    virtual QString GetUserFCS(QString &currentUserName) Q_DECL_OVERRIDE;
    virtual QString GetUserRank(QString &currentUserName) Q_DECL_OVERRIDE;
    virtual int GetUserRole(const QString &currentUserName) Q_DECL_OVERRIDE;

private:
    void ClearTable(QString tableName);
    bool GetBoolFromMessage(QSqlQuery &query);
    void GetStringFromMessage(QString &inputString, const QSqlRecord &record, int rowPos);
private:
    QString postgeSqlDatabaseDriverStringKey="QPSQL";
    QSqlDatabase *m_db;

    const QString usersTableName="rlstiusers";
    const QString startupTablePrefix="rlstistartups";
    const QString desktopTablePrefix="rlstidesktops";
};

#endif // SQLDATABASESERIVCE_H
