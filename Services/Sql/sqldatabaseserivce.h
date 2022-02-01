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
    virtual bool CheckStartupTables(int roleId) Q_DECL_OVERRIDE;
    virtual bool CheckDesktopTables() Q_DECL_OVERRIDE;
    virtual bool CheckDesktopTables(int roleId) Q_DECL_OVERRIDE;


    virtual void ClearUsersTable() Q_DECL_OVERRIDE;
    virtual void ClearStartupsTable(int roleId) Q_DECL_OVERRIDE;
    virtual void ClearDesktopTable(int roleId) Q_DECL_OVERRIDE;

    virtual void AppendUserIntoTable(const User &user) Q_DECL_OVERRIDE;
    virtual void RemoveUserIntoTable(int roleId,const User &user) Q_DECL_OVERRIDE ;
    virtual QList<User> GetAllUsers() Q_DECL_OVERRIDE;

    virtual void AppendStartupIntoRole(int roleId, const QString &exec) Q_DECL_OVERRIDE;
    virtual void RemoveStartupIntoRole(int roleId, const QString &startupPath) Q_DECL_OVERRIDE;
    virtual QStringList GetAllRoleStartups(int roleId) Q_DECL_OVERRIDE;
    virtual void GetAllRoleStartupsIntoModel(int roleId) Q_DECL_OVERRIDE;
    virtual QSqlQueryModel * GetRoleStartupsModel() Q_DECL_OVERRIDE;

    virtual void AppendDesktopIntoRole(int roleId, const DesktopEntity &entity) Q_DECL_OVERRIDE;
    virtual void RemoveDesktopIntoRole(int roleId, const QString &entityName) Q_DECL_OVERRIDE;
    virtual QList<DesktopEntity> GetAllRoleDesktops(int roleId) Q_DECL_OVERRIDE;
    virtual QStringList GetAllUsersWithRoleId(int roleId) Q_DECL_OVERRIDE;

public:
    virtual void CreateUsersTableIfNotExists() Q_DECL_OVERRIDE;
    virtual void CreateStartupsTableIfNotExists(int roleId) Q_DECL_OVERRIDE;
    virtual void CreateDesktopRolesIfNotExists(int roleId) Q_DECL_OVERRIDE;

public:
    virtual QStringList GetAdminsRoleUserName() Q_DECL_OVERRIDE;
    virtual QString GetUserFCS(const QString &currentUserName) Q_DECL_OVERRIDE;
    virtual QString GetUserRank(const QString &currentUserName) Q_DECL_OVERRIDE;
    virtual int GetUserRole(const QString &currentUserName) Q_DECL_OVERRIDE;

    virtual QString GetUserFCS(QStringView currentUserName) Q_DECL_OVERRIDE;
    virtual QString GetUserRank(QStringView currentUserName) Q_DECL_OVERRIDE;
    virtual int GetUserRole(QStringView currentUserName) Q_DECL_OVERRIDE;

private:
    void ClearTable(QString tableName);
    bool GetBoolFromMessage(QSqlQuery &query);
    void GetStringFromMessage( QString &inputString, const QSqlRecord &record,const int &rowPos);
private:
    const QString m_postgeSqlDatabaseDriverStringKey;
    QSqlDatabase m_db;
    QSqlQueryModel* const m_currentRoleModel;
    const QString m_usersTablePrefix;
    const QString m_startupTablePrefix;
    const QString m_desktopTablePrefix;
};

#endif // SQLDATABASESERIVCE_H
